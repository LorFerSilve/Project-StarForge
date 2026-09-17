#include "starforge/progression/market.hpp"

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
    const auto total = quoted.value().buy_from_player * static_cast<std::int64_t>(quantity);
    if (liquidity_ < total) return core::Result<void, MarketError>::failure(MarketError::InsufficientLiquidity);
    if (!player.credit(total, transaction_id)) return core::Result<void, MarketError>::failure(MarketError::InvalidDefinition);
    if (std::numeric_limits<std::uint64_t>::max() - it->second.stock < quantity)
        return core::Result<void, MarketError>::failure(MarketError::InvalidQuantity);
    it->second.stock += quantity;
    liquidity_ -= total;
    return core::Result<void, MarketError>::success();
}

} // namespace starforge::progression
