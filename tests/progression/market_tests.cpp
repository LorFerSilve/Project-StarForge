#include "starforge/progression/market.hpp"

#include <catch2/catch_test_macros.hpp>

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
