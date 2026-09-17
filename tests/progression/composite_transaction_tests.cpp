#include "starforge/progression/progression.hpp"

#include <catch2/catch_test_macros.hpp>

#include <limits>

using namespace starforge::progression;

TEST_CASE("composite progression rewards commit atomically under one transaction id") {
    ProgressionState state;
    REQUIRE(state.credit(100, 1));

    ProgressionMutation reward{
        .credit_delta = 25,
        .reputation_deltas = {{"Helios", 15}, {"Meridian", -12}},
        .evidence_id = "EVID_MISSION_01",
        .evidence_values = {{"Energy", 3}},
        .campaign_flags = {"MS_A01_COMPLETE"},
    };
    REQUIRE(state.apply(reward, 2));
    CHECK(state.credits() == 125);
    CHECK(state.reputation("Helios") == 15);
    CHECK(state.reputation("Meridian") == -12);
    CHECK(state.evidence("Energy") == 3);
    CHECK(state.has_evidence("EVID_MISSION_01"));
    CHECK(state.campaign_flag("MS_A01_COMPLETE"));
    CHECK_FALSE(state.apply(reward, 2));
    CHECK(state.credits() == 125);
}

TEST_CASE("failed composite progression transaction leaves every domain unchanged") {
    ProgressionState state;
    REQUIRE(state.credit(10, 10));
    ProgressionMutation invalid{
        .credit_delta = -11,
        .reputation_deltas = {{"Helios", 50}},
        .evidence_id = "EVID_FAILED",
        .evidence_values = {{"Energy", 5}},
        .campaign_flags = {"SHOULD_NOT_COMMIT"},
    };
    CHECK_FALSE(state.apply(invalid, 11));
    CHECK(state.credits() == 10);
    CHECK(state.reputation("Helios") == 0);
    CHECK_FALSE(state.has_evidence("EVID_FAILED"));
    CHECK_FALSE(state.campaign_flag("SHOULD_NOT_COMMIT"));

    invalid.credit_delta = 0;
    REQUIRE(state.apply(invalid, 11));
}

TEST_CASE("composite transaction validates all fields before committing credits") {
    ProgressionState state;
    ProgressionMutation invalid{
        .credit_delta = 50,
        .reputation_deltas = {{"", 10}},
    };
    CHECK_FALSE(state.apply(invalid, 20));
    CHECK(state.credits() == 0);

    invalid.reputation_deltas = {{"Helios", std::numeric_limits<std::int32_t>::min()}};
    REQUIRE(state.apply(invalid, 20));
    CHECK(state.credits() == 50);
    CHECK(state.reputation("Helios") == -100);
}
