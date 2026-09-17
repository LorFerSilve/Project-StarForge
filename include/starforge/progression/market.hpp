#pragma once

#include "starforge/core/result.hpp"
#include "starforge/progression/progression.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <string_view>

namespace starforge::progression {

enum class MarketError : std::uint8_t {
    InvalidDefinition,
    InvalidQuantity,
    UnknownItem,
    InsufficientStock,
    InsufficientLiquidity,
    InsufficientCredits,
    AccessDenied,
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

class Market final {
public:
    explicit Market(std::string id, std::int64_t liquidity = 0);

    [[nodiscard]] std::string_view id() const noexcept { return id_; }
    [[nodiscard]] std::int64_t liquidity() const noexcept { return liquidity_; }
    [[nodiscard]] bool trade_access() const noexcept { return trade_access_; }
    void set_trade_access(bool allowed) noexcept { trade_access_ = allowed; }

    [[nodiscard]] core::Result<void, MarketError> add_item(MarketItem item);
    [[nodiscard]] std::uint64_t stock(std::string_view item_id) const noexcept;
    [[nodiscard]] core::Result<MarketQuote, MarketError> quote(std::string_view item_id) const;

    // These commit only the economic side of a physical transfer. The caller must first
    // validate and commit authoritative physical ownership/capacity through the owning domain.
    [[nodiscard]] core::Result<void, MarketError> commit_purchase(
        ProgressionState& player, std::string_view item_id, std::uint64_t quantity,
        std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, MarketError> commit_sale(
        ProgressionState& player, std::string_view item_id, std::uint64_t quantity,
        std::uint64_t transaction_id);

private:
    [[nodiscard]] static std::int64_t apply_basis_points(std::int64_t value,
                                                         std::int32_t basis_points) noexcept;

    std::string id_;
    std::int64_t liquidity_{0};
    bool trade_access_{true};
    std::map<std::string, MarketItem> items_{};
};

} // namespace starforge::progression
