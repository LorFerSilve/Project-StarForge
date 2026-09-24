#pragma once

#include "starforge/core/result.hpp"
#include "starforge/core/simulation_time.hpp"
#include "starforge/economy/economy.hpp"

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>

namespace starforge::economy {

enum class MarketError : std::uint8_t {
    InvalidDefinition,
    InvalidQuantity,
    UnknownItem,
    InsufficientStock,
    InsufficientLiquidity,
    InsufficientCredits,
    AccessDenied,
    DuplicateTransaction,
    RevisionExhausted,
};

struct MarketQuoteFactors final {
    std::int32_t supply_demand_basis_points{10'000};
    std::int32_t profile_basis_points{10'000};
    std::int32_t event_basis_points{10'000};
    std::int32_t reputation_basis_points{10'000};
    std::int32_t legality_basis_points{10'000};
    std::int32_t spread_basis_points{1'000};
};

struct MarketItem final {
    std::string id;
    std::int64_t base_credit_value{0};
    std::uint64_t stock{0};
    MarketQuoteFactors factors{};
};

struct MarketQuote final {
    std::int64_t sell_to_player{0};
    std::int64_t buy_from_player{0};
};

struct StockReplenishment final {
    std::uint64_t quantity_per_cycle{0};
    std::uint64_t maximum_stock{0};
};

struct MarketReplenishment final {
    std::uint64_t interval_ticks{0};
    core::SimulationTick next_update{};
    std::int64_t liquidity_per_cycle{0};
    std::int64_t maximum_liquidity{0};
    std::map<std::string, StockReplenishment> stock{};
};

struct MarketSnapshot final {
    std::string id;
    std::int64_t liquidity{0};
    bool trade_access{true};
    std::map<std::string, MarketItem> items{};
    MarketReplenishment replenishment{};
    std::uint64_t revision{0};
};

class Market;

class PreparedMarketTrade final : public transactions::IPreparedTransactionParticipant {
public:
    PreparedMarketTrade(PreparedMarketTrade&&) noexcept = default;
    PreparedMarketTrade& operator=(PreparedMarketTrade&&) noexcept = default;
    PreparedMarketTrade(const PreparedMarketTrade&) = delete;
    PreparedMarketTrade& operator=(const PreparedMarketTrade&) = delete;

    [[nodiscard]] transactions::DomainCommitKey commit_key() const noexcept override;
    [[nodiscard]] core::StateRevision expected_revision() const noexcept override;
    [[nodiscard]] core::StateRevision current_revision() const noexcept override;
    void commit() noexcept override;
    void publish_committed_events() noexcept override;

private:
    friend class Market;

    PreparedMarketTrade(EconomyState& account,
                        Market& market,
                        std::string item_id,
                        std::int64_t next_credits,
                        std::set<std::uint64_t> committed_transactions,
                        std::uint64_t next_stock,
                        std::int64_t next_liquidity,
                        core::StateRevision expected_account_revision,
                        core::StateRevision expected_market_revision) noexcept;

    EconomyState* account_{nullptr};
    Market* market_{nullptr};
    std::string item_id_{};
    std::int64_t next_credits_{0};
    std::set<std::uint64_t> committed_transactions_{};
    std::uint64_t next_stock_{0};
    std::int64_t next_liquidity_{0};
    core::StateRevision expected_account_revision_{};
    core::StateRevision expected_market_revision_{};
};

class Market final {
public:
    explicit Market(std::string id, std::int64_t liquidity = 0);

    [[nodiscard]] std::string_view id() const noexcept { return id_; }
    [[nodiscard]] std::int64_t liquidity() const noexcept { return liquidity_; }
    [[nodiscard]] bool trade_access() const noexcept { return trade_access_; }
    [[nodiscard]] core::StateRevision revision() const noexcept { return revision_; }
    [[nodiscard]] core::SimulationTick next_economic_update() const noexcept {
        return replenishment_.next_update;
    }
    [[nodiscard]] bool set_trade_access(bool allowed) noexcept;

    [[nodiscard]] core::Result<void, MarketError> add_item(MarketItem item);
    [[nodiscard]] std::uint64_t stock(std::string_view item_id) const noexcept;
    [[nodiscard]] core::Result<MarketQuote, MarketError> quote(std::string_view item_id) const;
    [[nodiscard]] core::Result<void, MarketError> configure_replenishment(MarketReplenishment plan);
    void advance_economy(core::SimulationTick now) noexcept;

    [[nodiscard]] MarketSnapshot snapshot() const;
    [[nodiscard]] static core::Result<Market, MarketError> restore(MarketSnapshot snapshot);

    [[nodiscard]] core::Result<PreparedMarketTrade, MarketError> prepare_purchase(
        EconomyState& account, std::string_view item_id, std::uint64_t quantity,
        core::TransactionId transaction_id);
    [[nodiscard]] core::Result<PreparedMarketTrade, MarketError> prepare_sale(
        EconomyState& account, std::string_view item_id, std::uint64_t quantity,
        core::TransactionId transaction_id);

private:
    friend class PreparedMarketTrade;

    [[nodiscard]] static std::int64_t apply_basis_points(std::int64_t value,
                                                         std::int32_t basis_points) noexcept;
    [[nodiscard]] core::Result<PreparedMarketTrade, MarketError> prepare_trade(
        EconomyState& account, std::string_view item_id, std::uint64_t quantity,
        core::TransactionId transaction_id, bool purchase);

    std::string id_;
    std::int64_t liquidity_{0};
    bool trade_access_{true};
    std::map<std::string, MarketItem> items_{};
    MarketReplenishment replenishment_{};
    core::StateRevision revision_{};
};

} // namespace starforge::economy
