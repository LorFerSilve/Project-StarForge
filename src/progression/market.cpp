#include "starforge/progression/market.hpp"

#include <algorithm>
#include <limits>
#include <utility>

namespace starforge::progression {

Market::Market(std::string id, std::int64_t liquidity)
    : id_(std::move(id)), liquidity_(liquidity < 0 ? 0 : liquidity) {}

core::Result<void, MarketError> Market::add_item(MarketItem item) {
    if (item.id.empty() || item.base_credit_value <= 0 || item.factors.spread_basis_points < 0 ||
        item.factors.spread_basis_points >= 10'000) {
        return core::Result<void, MarketError>::failure(MarketError::InvalidDefinition);
    }
    items_[item.id] = std::move(item);
    return core::Result<void, MarketError>::success();
}

std::uint64_t Market::stock(std::string_view item_id) const noexcept {
    const auto it = items_.find(std::string(item_id));
    return it == items_.end() ? 0U : it->second.stock;
}

core::Result<void, MarketError> Market::configure_replenishment(MarketReplenishment plan) {
    if (plan.interval_ticks == 0 || plan.liquidity_per_cycle < 0 || plan.maximum_liquidity < 0 ||
        plan.maximum_liquidity < liquidity_) {
        return core::Result<void, MarketError>::failure(MarketError::InvalidDefinition);
    }
    for (const auto& [item_id, rule] : plan.stock) {
        const auto it = items_.find(item_id);
        if (it == items_.end() || rule.maximum_stock < it->second.stock) {
            return core::Result<void, MarketError>::failure(MarketError::InvalidDefinition);
        }
    }
    replenishment_ = std::move(plan);
    return core::Result<void, MarketError>::success();
}

void Market::advance_economy(core::SimulationTick now) noexcept {
    const auto interval = replenishment_.interval_ticks;
    const auto next = replenishment_.next_update.raw();
    if (interval == 0 || now.raw() < next) return;

    const auto cycles = ((now.raw() - next) / interval) + 1U;
    for (const auto& [item_id, rule] : replenishment_.stock) {
        auto it = items_.find(item_id);
        if (it == items_.end() || it->second.stock >= rule.maximum_stock ||
            rule.quantity_per_cycle == 0) {
            continue;
        }
        const auto missing = rule.maximum_stock - it->second.stock;
        const auto cycles_to_fill = (missing / rule.quantity_per_cycle) +
                                    ((missing % rule.quantity_per_cycle) != 0U ? 1U : 0U);
        const auto applied_cycles = std::min(cycles, cycles_to_fill);
        const auto addition = applied_cycles == cycles_to_fill
                                  ? missing
                                  : applied_cycles * rule.quantity_per_cycle;
        it->second.stock += addition;
    }

    if (liquidity_ < replenishment_.maximum_liquidity && replenishment_.liquidity_per_cycle > 0) {
        const auto missing = replenishment_.maximum_liquidity - liquidity_;
        const auto per_cycle = static_cast<std::uint64_t>(replenishment_.liquidity_per_cycle);
        const auto missing_unsigned = static_cast<std::uint64_t>(missing);
        const auto cycles_to_fill = (missing_unsigned / per_cycle) +
                                    ((missing_unsigned % per_cycle) != 0U ? 1U : 0U);
        const auto applied_cycles = std::min(cycles, cycles_to_fill);
        const auto addition = applied_cycles == cycles_to_fill
                                  ? missing_unsigned
                                  : applied_cycles * per_cycle;
        liquidity_ += static_cast<std::int64_t>(addition);
    }

    const auto max_tick = std::numeric_limits<std::uint64_t>::max();
    if (cycles > (max_tick - next) / interval) {
        replenishment_.next_update = core::SimulationTick{max_tick};
    } else {
        replenishment_.next_update = core::SimulationTick{next + (cycles * interval)};
    }
}

std::int64_t Market::apply_basis_points(std::int64_t value, std::int32_t basis_points) noexcept {
    if (value <= 0 || basis_points <= 0) return 0;
    const auto safe_max = std::numeric_limits<std::int64_t>::max() / basis_points;
    if (value > safe_max) return std::numeric_limits<std::int64_t>::max();
    return (value * basis_points) / 10'000;
}

core::Result<MarketQuote, MarketError> Market::quote(std::string_view item_id) const {
    const auto it = items_.find(std::string(item_id));
    if (it == items_.end()) return core::Result<MarketQuote, MarketError>::failure(MarketError::UnknownItem);
    const auto& item = it->second;
    auto midpoint = item.base_credit_value;
    midpoint = apply_basis_points(midpoint, item.factors.supply_demand_basis_points);
    midpoint = apply_basis_points(midpoint, item.factors.profile_basis_points);
    midpoint = apply_basis_points(midpoint, item.factors.event_basis_points);
    midpoint = apply_basis_points(midpoint, item.factors.reputation_basis_points);
    midpoint = apply_basis_points(midpoint, item.factors.legality_basis_points);
    if (midpoint <= 0) return core::Result<MarketQuote, MarketError>::failure(MarketError::InvalidDefinition);

    const auto spread = item.factors.spread_basis_points;
    const auto sell = apply_basis_points(midpoint, 10'000 + spread);
    const auto buy = apply_basis_points(midpoint, 10'000 - spread);
    if (sell <= buy || buy <= 0) return core::Result<MarketQuote, MarketError>::failure(MarketError::InvalidDefinition);
    return core::Result<MarketQuote, MarketError>::success(MarketQuote{sell, buy});
}

core::Result<void, MarketError> Market::commit_purchase(
    ProgressionState& player, std::string_view item_id, std::uint64_t quantity,
    std::uint64_t transaction_id) {
    if (!trade_access_) return core::Result<void, MarketError>::failure(MarketError::AccessDenied);
    if (quantity == 0) return core::Result<void, MarketError>::failure(MarketError::InvalidQuantity);
    auto it = items_.find(std::string(item_id));
    if (it == items_.end()) return core::Result<void, MarketError>::failure(MarketError::UnknownItem);
    if (it->second.stock < quantity) return core::Result<void, MarketError>::failure(MarketError::InsufficientStock);
    const auto quoted = quote(item_id);
    if (!quoted) return core::Result<void, MarketError>::failure(quoted.error());
    if (quantity > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max() / quoted.value().sell_to_player))
        return core::Result<void, MarketError>::failure(MarketError::InvalidQuantity);
    const auto total = quoted.value().sell_to_player * static_cast<std::int64_t>(quantity);
    if (player.credits() < total) return core::Result<void, MarketError>::failure(MarketError::InsufficientCredits);
    if (liquidity_ > std::numeric_limits<std::int64_t>::max() - total)
        return core::Result<void, MarketError>::failure(MarketError::InvalidQuantity);
    if (!player.debit(total, transaction_id)) return core::Result<void, MarketError>::failure(MarketError::InvalidDefinition);
    it->second.stock -= quantity;
    liquidity_ += total;
    return core::Result<void, MarketError>::success();
}

core::Result<void, MarketError> Market::commit_sale(
    ProgressionState& player, std::string_view item_id, std::uint64_t quantity,
    std::uint64_t transaction_id) {
    if (!trade_access_) return core::Result<void, MarketError>::failure(MarketError::AccessDenied);
    if (quantity == 0) return core::Result<void, MarketError>::failure(MarketError::InvalidQuantity);
    auto it = items_.find(std::string(item_id));
    if (it == items_.end()) return core::Result<void, MarketError>::failure(MarketError::UnknownItem);
    const auto quoted = quote(item_id);
    if (!quoted) return core::Result<void, MarketError>::failure(quoted.error());
    if (quantity > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max() / quoted.value().buy_from_player))
        return core::Result<void, MarketError>::failure(MarketError::InvalidQuantity);
    if (std::numeric_limits<std::uint64_t>::max() - it->second.stock < quantity)
        return core::Result<void, MarketError>::failure(MarketError::InvalidQuantity);
    const auto total = quoted.value().buy_from_player * static_cast<std::int64_t>(quantity);
    if (liquidity_ < total) return core::Result<void, MarketError>::failure(MarketError::InsufficientLiquidity);
    if (!player.credit(total, transaction_id)) return core::Result<void, MarketError>::failure(MarketError::InvalidDefinition);
    it->second.stock += quantity;
    liquidity_ -= total;
    return core::Result<void, MarketError>::success();
}

} // namespace starforge::progression
