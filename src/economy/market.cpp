#include "starforge/economy/market.hpp"

#include <algorithm>
#include <limits>
#include <utility>

namespace starforge::economy {

PreparedMarketTrade::PreparedMarketTrade(
    EconomyState& account,
    Market& market,
    std::string item_id,
    std::int64_t next_credits,
    std::set<std::uint64_t> committed_transactions,
    std::uint64_t next_stock,
    std::int64_t next_liquidity,
    core::StateRevision expected_account_revision,
    core::StateRevision expected_market_revision) noexcept
    : account_(&account),
      market_(&market),
      item_id_(std::move(item_id)),
      next_credits_(next_credits),
      committed_transactions_(std::move(committed_transactions)),
      next_stock_(next_stock),
      next_liquidity_(next_liquidity),
      expected_account_revision_(expected_account_revision),
      expected_market_revision_(expected_market_revision) {}

transactions::DomainCommitKey PreparedMarketTrade::commit_key() const noexcept {
    return {.domain = transactions::DomainCommitOrder::Economy, .stable_ordinal = 0U};
}

core::StateRevision PreparedMarketTrade::expected_revision() const noexcept {
    return core::StateRevision{0U};
}

core::StateRevision PreparedMarketTrade::current_revision() const noexcept {
    const bool unchanged =
        account_->revision_ == expected_account_revision_ &&
        market_->revision_ == expected_market_revision_;
    return core::StateRevision{unchanged ? 0U : 1U};
}

void PreparedMarketTrade::commit() noexcept {
    account_->credits_ = next_credits_;
    account_->committed_transactions_.swap(committed_transactions_);
    static_cast<void>(account_->revision_.advance());

    const auto item = market_->items_.find(item_id_);
    item->second.stock = next_stock_;
    market_->liquidity_ = next_liquidity_;
    static_cast<void>(market_->revision_.advance());
}

void PreparedMarketTrade::publish_committed_events() noexcept {}

Market::Market(std::string id, std::int64_t liquidity)
    : id_(std::move(id)), liquidity_(liquidity < 0 ? 0 : liquidity) {}

bool Market::set_trade_access(bool allowed) noexcept {
    if (trade_access_ == allowed || !revision_.can_advance()) {
        return false;
    }
    trade_access_ = allowed;
    static_cast<void>(revision_.advance());
    return true;
}

core::Result<void, MarketError> Market::add_item(MarketItem item) {
    if (item.id.empty() || item.base_credit_value <= 0 ||
        item.factors.supply_demand_basis_points <= 0 ||
        item.factors.profile_basis_points <= 0 ||
        item.factors.event_basis_points <= 0 ||
        item.factors.reputation_basis_points <= 0 ||
        item.factors.legality_basis_points <= 0 ||
        item.factors.spread_basis_points < 0 ||
        item.factors.spread_basis_points >= 10'000) {
        return core::Result<void, MarketError>::failure(MarketError::InvalidDefinition);
    }
    if (!revision_.can_advance()) {
        return core::Result<void, MarketError>::failure(MarketError::RevisionExhausted);
    }
    items_[item.id] = std::move(item);
    static_cast<void>(revision_.advance());
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
    if (!revision_.can_advance()) {
        return core::Result<void, MarketError>::failure(MarketError::RevisionExhausted);
    }
    replenishment_ = std::move(plan);
    static_cast<void>(revision_.advance());
    return core::Result<void, MarketError>::success();
}

void Market::advance_economy(core::SimulationTick now) noexcept {
    const auto interval = replenishment_.interval_ticks;
    const auto next = replenishment_.next_update.raw();
    if (interval == 0 || now.raw() < next || !revision_.can_advance()) {
        return;
    }

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
    static_cast<void>(revision_.advance());
}

MarketSnapshot Market::snapshot() const {
    return {
        .id = id_,
        .liquidity = liquidity_,
        .trade_access = trade_access_,
        .items = items_,
        .replenishment = replenishment_,
        .revision = revision_.raw(),
    };
}

core::Result<Market, MarketError> Market::restore(MarketSnapshot snapshot) {
    if (snapshot.id.empty() || snapshot.liquidity < 0) {
        return core::Result<Market, MarketError>::failure(MarketError::InvalidDefinition);
    }

    Market restored{std::move(snapshot.id), snapshot.liquidity};
    restored.trade_access_ = snapshot.trade_access;
    for (auto& [item_id, item] : snapshot.items) {
        if (item_id.empty() || item.id != item_id) {
            return core::Result<Market, MarketError>::failure(MarketError::InvalidDefinition);
        }
        const auto added = restored.add_item(std::move(item));
        if (!added) {
            return core::Result<Market, MarketError>::failure(added.error());
        }
    }

    const auto& plan = snapshot.replenishment;
    const bool has_replenishment = plan.interval_ticks != 0 || plan.next_update.raw() != 0 ||
                                   plan.liquidity_per_cycle != 0 || plan.maximum_liquidity != 0 ||
                                   !plan.stock.empty();
    if (has_replenishment) {
        if (plan.interval_ticks == 0) {
            return core::Result<Market, MarketError>::failure(MarketError::InvalidDefinition);
        }
        const auto configured = restored.configure_replenishment(std::move(snapshot.replenishment));
        if (!configured) {
            return core::Result<Market, MarketError>::failure(configured.error());
        }
    }

    restored.revision_ = core::StateRevision{snapshot.revision};
    return core::Result<Market, MarketError>::success(std::move(restored));
}

std::int64_t Market::apply_basis_points(std::int64_t value, std::int32_t basis_points) noexcept {
    if (value <= 0 || basis_points <= 0) {
        return 0;
    }
    const auto safe_max = std::numeric_limits<std::int64_t>::max() / basis_points;
    if (value > safe_max) {
        return std::numeric_limits<std::int64_t>::max();
    }
    return (value * basis_points) / 10'000;
}

core::Result<MarketQuote, MarketError> Market::quote(std::string_view item_id) const {
    const auto it = items_.find(std::string(item_id));
    if (it == items_.end()) {
        return core::Result<MarketQuote, MarketError>::failure(MarketError::UnknownItem);
    }
    const auto& item = it->second;
    auto midpoint = item.base_credit_value;
    midpoint = apply_basis_points(midpoint, item.factors.supply_demand_basis_points);
    midpoint = apply_basis_points(midpoint, item.factors.profile_basis_points);
    midpoint = apply_basis_points(midpoint, item.factors.event_basis_points);
    midpoint = apply_basis_points(midpoint, item.factors.reputation_basis_points);
    midpoint = apply_basis_points(midpoint, item.factors.legality_basis_points);
    if (midpoint <= 0) {
        return core::Result<MarketQuote, MarketError>::failure(MarketError::InvalidDefinition);
    }

    const auto spread = item.factors.spread_basis_points;
    const auto sell = apply_basis_points(midpoint, 10'000 + spread);
    const auto buy = apply_basis_points(midpoint, 10'000 - spread);
    if (sell <= buy || buy <= 0) {
        return core::Result<MarketQuote, MarketError>::failure(MarketError::InvalidDefinition);
    }
    return core::Result<MarketQuote, MarketError>::success(MarketQuote{sell, buy});
}

core::Result<PreparedMarketTrade, MarketError> Market::prepare_purchase(
    EconomyState& account, std::string_view item_id, std::uint64_t quantity,
    core::TransactionId transaction_id) {
    return prepare_trade(account, item_id, quantity, transaction_id, true);
}

core::Result<PreparedMarketTrade, MarketError> Market::prepare_sale(
    EconomyState& account, std::string_view item_id, std::uint64_t quantity,
    core::TransactionId transaction_id) {
    return prepare_trade(account, item_id, quantity, transaction_id, false);
}

core::Result<PreparedMarketTrade, MarketError> Market::prepare_trade(
    EconomyState& account, std::string_view item_id, std::uint64_t quantity,
    core::TransactionId transaction_id, bool purchase) {
    if (!trade_access_) {
        return core::Result<PreparedMarketTrade, MarketError>::failure(MarketError::AccessDenied);
    }
    if (quantity == 0) {
        return core::Result<PreparedMarketTrade, MarketError>::failure(MarketError::InvalidQuantity);
    }
    if (!transaction_id.valid() || account.transaction_seen(transaction_id)) {
        return core::Result<PreparedMarketTrade, MarketError>::failure(
            MarketError::DuplicateTransaction);
    }
    if (!revision_.can_advance() || !account.revision_.can_advance()) {
        return core::Result<PreparedMarketTrade, MarketError>::failure(
            MarketError::RevisionExhausted);
    }

    const auto item = items_.find(std::string(item_id));
    if (item == items_.end()) {
        return core::Result<PreparedMarketTrade, MarketError>::failure(MarketError::UnknownItem);
    }
    const auto quoted = quote(item_id);
    if (!quoted) {
        return core::Result<PreparedMarketTrade, MarketError>::failure(quoted.error());
    }

    const auto unit_price = purchase ? quoted.value().sell_to_player
                                     : quoted.value().buy_from_player;
    if (unit_price <= 0 ||
        quantity > static_cast<std::uint64_t>(
                       std::numeric_limits<std::int64_t>::max() / unit_price)) {
        return core::Result<PreparedMarketTrade, MarketError>::failure(
            MarketError::InvalidQuantity);
    }
    const auto total = unit_price * static_cast<std::int64_t>(quantity);

    auto next_credits = account.credits_;
    auto next_liquidity = liquidity_;
    auto next_stock = item->second.stock;

    if (purchase) {
        if (next_stock < quantity) {
            return core::Result<PreparedMarketTrade, MarketError>::failure(
                MarketError::InsufficientStock);
        }
        if (next_credits < total) {
            return core::Result<PreparedMarketTrade, MarketError>::failure(
                MarketError::InsufficientCredits);
        }
        if (next_liquidity > std::numeric_limits<std::int64_t>::max() - total) {
            return core::Result<PreparedMarketTrade, MarketError>::failure(
                MarketError::InvalidQuantity);
        }
        next_credits -= total;
        next_liquidity += total;
        next_stock -= quantity;
    } else {
        if (next_liquidity < total) {
            return core::Result<PreparedMarketTrade, MarketError>::failure(
                MarketError::InsufficientLiquidity);
        }
        if (next_credits > std::numeric_limits<std::int64_t>::max() - total ||
            std::numeric_limits<std::uint64_t>::max() - next_stock < quantity) {
            return core::Result<PreparedMarketTrade, MarketError>::failure(
                MarketError::InvalidQuantity);
        }
        next_credits += total;
        next_liquidity -= total;
        next_stock += quantity;
    }

    auto next_transactions = account.committed_transactions_;
    next_transactions.insert(transaction_id.raw());

    return core::Result<PreparedMarketTrade, MarketError>::success(
        PreparedMarketTrade{
            account,
            *this,
            std::string(item_id),
            next_credits,
            std::move(next_transactions),
            next_stock,
            next_liquidity,
            account.revision_,
            revision_,
        });
}

} // namespace starforge::economy
