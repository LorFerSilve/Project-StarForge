#include "starforge/economy/market.hpp"
#include "starforge/station/logistics_transaction.hpp"
#include "starforge/transactions/coordinator.hpp"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <utility>

using namespace starforge::economy;

namespace {

EconomyState account_with(std::int64_t credits) {
    auto restored = EconomyState::restore(EconomySnapshot{.credits = credits});
    REQUIRE(restored);
    return std::move(restored).value();
}

} // namespace

TEST_CASE("market quotes preserve ordinary same-terminal spread") {
    Market market{"KARA", 10'000};
    REQUIRE(market.add_item(
        MarketItem{.id = "TITANIUM", .base_credit_value = 100, .stock = 20}));
    const auto quote = market.quote("TITANIUM");
    REQUIRE(quote);
    CHECK(quote.value().sell_to_player == 110);
    CHECK(quote.value().buy_from_player == 90);
    CHECK(quote.value().sell_to_player > quote.value().buy_from_player);
}

TEST_CASE("physical purchase commits credits market stock and destination quantity atomically") {
    auto account = account_with(1'000);
    Market market{"KARA", 500};
    REQUIRE(market.add_item(MarketItem{.id = "FUEL", .base_credit_value = 100, .stock = 3}));

    starforge::station::LogisticsStore logistics;
    const starforge::station::StorageId destination{1U};
    const starforge::station::ResourceId fuel{1U};
    REQUIRE(logistics.add_storage(destination));

    starforge::transactions::TransactionCoordinator coordinator;
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);

    auto physical = starforge::station::PreparedLogisticsMutation::prepare_deposit(
        logistics, destination, fuel, 2);
    REQUIRE(physical);
    auto economic = market.prepare_purchase(account, "FUEL", 2, candidate.value());
    REQUIRE(economic);

    std::array<starforge::transactions::IPreparedTransactionParticipant*, 2> participants{
        &economic.value(), &physical.value()};
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    const auto result = coordinator.commit();
    REQUIRE(result.committed);

    CHECK(account.credits() == 780);
    CHECK(market.stock("FUEL") == 1);
    CHECK(market.liquidity() == 720);
    CHECK(logistics.quantity(destination, fuel) == 2);
}

TEST_CASE("stale receiving inventory aborts purchase before any economic mutation") {
    auto account = account_with(500);
    Market market{"KARA", 500};
    REQUIRE(market.add_item(MarketItem{.id = "MODULE", .base_credit_value = 100, .stock = 1}));

    starforge::station::LogisticsStore logistics;
    const starforge::station::StorageId destination{1U};
    const starforge::station::ResourceId module{1U};
    const starforge::station::ResourceId unrelated{2U};
    REQUIRE(logistics.add_storage(destination));

    starforge::transactions::TransactionCoordinator coordinator;
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);
    auto physical = starforge::station::PreparedLogisticsMutation::prepare_deposit(
        logistics, destination, module, 1);
    REQUIRE(physical);
    auto economic = market.prepare_purchase(account, "MODULE", 1, candidate.value());
    REQUIRE(economic);

    REQUIRE(logistics.deposit(destination, unrelated, 1));

    std::array<starforge::transactions::IPreparedTransactionParticipant*, 2> participants{
        &economic.value(), &physical.value()};
    const auto prepared = coordinator.prepare(candidate.value(), participants);
    REQUIRE_FALSE(prepared);
    CHECK(prepared.error() == starforge::transactions::TransactionError::StaleRevision);

    CHECK(account.credits() == 500);
    CHECK(market.stock("MODULE") == 1);
    CHECK(market.liquidity() == 500);
    CHECK(logistics.quantity(destination, module) == 0);
}

TEST_CASE("physical sale commits source removal market stock and credits atomically") {
    auto account = account_with(0);
    Market market{"FREEHOLD", 500};
    REQUIRE(market.add_item(
        MarketItem{.id = "MEDICAL", .base_credit_value = 100, .stock = 0}));

    starforge::station::LogisticsStore logistics;
    const starforge::station::StorageId source{1U};
    const starforge::station::ResourceId medical{1U};
    REQUIRE(logistics.add_storage(source));
    REQUIRE(logistics.deposit(source, medical, 2));

    starforge::transactions::TransactionCoordinator coordinator;
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);

    auto physical = starforge::station::PreparedLogisticsMutation::prepare_withdraw(
        logistics, source, medical, 1);
    REQUIRE(physical);
    auto economic = market.prepare_sale(account, "MEDICAL", 1, candidate.value());
    REQUIRE(economic);

    std::array<starforge::transactions::IPreparedTransactionParticipant*, 2> participants{
        &economic.value(), &physical.value()};
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    REQUIRE(coordinator.commit().committed);

    CHECK(account.credits() == 90);
    CHECK(market.stock("MEDICAL") == 1);
    CHECK(market.liquidity() == 410);
    CHECK(logistics.quantity(source, medical) == 1);
}

TEST_CASE("insufficient physical ownership rejects sale before economic preparation") {
    auto account = account_with(0);
    Market market{"FREEHOLD", 500};
    REQUIRE(market.add_item(
        MarketItem{.id = "MEDICAL", .base_credit_value = 100, .stock = 0}));

    starforge::station::LogisticsStore logistics;
    const starforge::station::StorageId source{1U};
    const starforge::station::ResourceId medical{1U};
    REQUIRE(logistics.add_storage(source));

    const auto physical = starforge::station::PreparedLogisticsMutation::prepare_withdraw(
        logistics, source, medical, 1);
    REQUIRE_FALSE(physical);
    CHECK(account.credits() == 0);
    CHECK(market.stock("MEDICAL") == 0);
    CHECK(market.liquidity() == 500);
}

TEST_CASE("market replenishment and snapshot remain simulation-time driven") {
    Market market{"FRONTIER", 100};
    REQUIRE(market.add_item(MarketItem{.id = "FUEL", .base_credit_value = 100, .stock = 1}));
    MarketReplenishment plan;
    plan.interval_ticks = 60;
    plan.next_update = starforge::core::SimulationTick{120};
    plan.liquidity_per_cycle = 25;
    plan.maximum_liquidity = 175;
    plan.stock.emplace(
        "FUEL", StockReplenishment{.quantity_per_cycle = 2, .maximum_stock = 6});
    REQUIRE(market.configure_replenishment(std::move(plan)));

    market.advance_economy(starforge::core::SimulationTick{300});
    CHECK(market.stock("FUEL") == 6);
    CHECK(market.liquidity() == 175);
    CHECK(market.next_economic_update().raw() == 360);

    REQUIRE(market.set_trade_access(false));
    const auto snapshot = market.snapshot();
    auto restored = Market::restore(snapshot);
    REQUIRE(restored);
    CHECK(restored.value().stock("FUEL") == 6);
    CHECK(restored.value().liquidity() == 175);
    CHECK_FALSE(restored.value().trade_access());
    CHECK(restored.value().revision() == market.revision());
}
