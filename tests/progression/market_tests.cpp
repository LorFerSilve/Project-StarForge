#include "starforge/progression/market.hpp"

#include <catch2/catch_test_macros.hpp>

#include <utility>

using namespace starforge::progression;

TEST_CASE("market quotes preserve ordinary same-terminal spread") {
    Market market{"KARA", 10'000};
    REQUIRE(market.add_item(MarketItem{.id = "TITANIUM", .base_credit_value = 100, .stock = 20}));
    const auto quote = market.quote("TITANIUM");
    REQUIRE(quote);
    CHECK(quote.value().sell_to_player == 110);
    CHECK(quote.value().buy_from_player == 90);
    CHECK(quote.value().sell_to_player > quote.value().buy_from_player);
}

TEST_CASE("market purchase is finite and conserves credits and liquidity") {
    ProgressionState player;
    REQUIRE(player.credit(1'000, 1));
    Market market{"KARA", 500};
    REQUIRE(market.add_item(MarketItem{.id = "FUEL", .base_credit_value = 100, .stock = 3}));

    REQUIRE(market.commit_purchase(player, "FUEL", 2, 2));
    CHECK(player.credits() == 780);
    CHECK(market.stock("FUEL") == 1);
    CHECK(market.liquidity() == 720);
    CHECK_FALSE(market.commit_purchase(player, "FUEL", 2, 3));
    CHECK(player.credits() == 780);
}

TEST_CASE("market sale respects finite purchase liquidity") {
    ProgressionState player;
    Market market{"FREEHOLD", 100};
    REQUIRE(market.add_item(MarketItem{.id = "MEDICAL", .base_credit_value = 100, .stock = 0}));

    REQUIRE(market.commit_sale(player, "MEDICAL", 1, 10));
    CHECK(player.credits() == 90);
    CHECK(market.stock("MEDICAL") == 1);
    CHECK(market.liquidity() == 10);
    CHECK_FALSE(market.commit_sale(player, "MEDICAL", 1, 11));
    CHECK(player.credits() == 90);
}

TEST_CASE("market access gates transactions without mutation") {
    ProgressionState player;
    REQUIRE(player.credit(500, 20));
    Market market{"RESTRICTED", 500};
    REQUIRE(market.add_item(MarketItem{.id = "MODULE", .base_credit_value = 100, .stock = 1}));
    market.set_trade_access(false);
    CHECK_FALSE(market.commit_purchase(player, "MODULE", 1, 21));
    CHECK(player.credits() == 500);
    CHECK(market.stock("MODULE") == 1);
}

TEST_CASE("market replenishment advances only on explicit simulation time") {
    Market market{"FRONTIER", 100};
    REQUIRE(market.add_item(MarketItem{.id = "FUEL", .base_credit_value = 100, .stock = 1}));
    MarketReplenishment plan;
    plan.interval_ticks = 60;
    plan.next_update = starforge::core::SimulationTick{120};
    plan.liquidity_per_cycle = 25;
    plan.maximum_liquidity = 175;
    plan.stock.emplace("FUEL", StockReplenishment{.quantity_per_cycle = 2, .maximum_stock = 6});
    REQUIRE(market.configure_replenishment(std::move(plan)));

    market.advance_economy(starforge::core::SimulationTick{119});
    CHECK(market.stock("FUEL") == 1);
    CHECK(market.liquidity() == 100);
    CHECK(market.next_economic_update().raw() == 120);

    market.advance_economy(starforge::core::SimulationTick{120});
    CHECK(market.stock("FUEL") == 3);
    CHECK(market.liquidity() == 125);
    CHECK(market.next_economic_update().raw() == 180);

    market.advance_economy(starforge::core::SimulationTick{300});
    CHECK(market.stock("FUEL") == 6);
    CHECK(market.liquidity() == 175);
    CHECK(market.next_economic_update().raw() == 360);
}

TEST_CASE("market replenishment rejects definitions that would discard current state") {
    Market market{"KARA", 100};
    REQUIRE(market.add_item(MarketItem{.id = "FUEL", .base_credit_value = 100, .stock = 5}));

    MarketReplenishment plan;
    plan.interval_ticks = 60;
    plan.next_update = starforge::core::SimulationTick{60};
    plan.maximum_liquidity = 99;
    plan.stock.emplace("FUEL", StockReplenishment{.quantity_per_cycle = 1, .maximum_stock = 4});
    CHECK_FALSE(market.configure_replenishment(std::move(plan)));
    CHECK(market.stock("FUEL") == 5);
    CHECK(market.liquidity() == 100);
}

TEST_CASE("market snapshot preserves finite economy state and simulation schedule") {
    Market market{"KARA", 500};
    REQUIRE(market.add_item(MarketItem{.id = "FUEL", .base_credit_value = 100, .stock = 3}));
    MarketReplenishment plan;
    plan.interval_ticks = 60;
    plan.next_update = starforge::core::SimulationTick{180};
    plan.liquidity_per_cycle = 25;
    plan.maximum_liquidity = 750;
    plan.stock.emplace("FUEL", StockReplenishment{.quantity_per_cycle = 2, .maximum_stock = 8});
    REQUIRE(market.configure_replenishment(std::move(plan)));
    market.set_trade_access(false);

    const auto restored_result = Market::restore(market.snapshot());
    REQUIRE(restored_result);
    auto restored = restored_result.value();
    CHECK(restored.id() == "KARA");
    CHECK(restored.liquidity() == 500);
    CHECK(restored.stock("FUEL") == 3);
    CHECK_FALSE(restored.trade_access());
    CHECK(restored.next_economic_update().raw() == 180);

    restored.advance_economy(starforge::core::SimulationTick{180});
    CHECK(restored.stock("FUEL") == 5);
    CHECK(restored.liquidity() == 525);
    CHECK(restored.next_economic_update().raw() == 240);
}

TEST_CASE("market restore rejects malformed persisted invariants") {
    Market market{"KARA", 100};
    REQUIRE(market.add_item(MarketItem{.id = "FUEL", .base_credit_value = 100, .stock = 2}));

    auto mismatched_item = market.snapshot();
    mismatched_item.items.at("FUEL").id = "OTHER";
    CHECK_FALSE(Market::restore(std::move(mismatched_item)));

    auto invalid_liquidity = market.snapshot();
    invalid_liquidity.liquidity = -1;
    CHECK_FALSE(Market::restore(std::move(invalid_liquidity)));

    auto invalid_schedule = market.snapshot();
    invalid_schedule.replenishment.next_update = starforge::core::SimulationTick{60};
    CHECK_FALSE(Market::restore(std::move(invalid_schedule)));
}
