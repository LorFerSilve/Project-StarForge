#include "starforge/progression/progression.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace starforge::progression;

TEST_CASE("credits conserve committed ledger transactions") {
    ProgressionState state;
    REQUIRE(state.credit(500, 1));
    REQUIRE(state.debit(125, 2));
    CHECK(state.credits() == 375);
    CHECK_FALSE(state.credit(500, 1));
    CHECK_FALSE(state.debit(376, 3));
    CHECK(state.credits() == 375);
}

TEST_CASE("reputation uses canonical independent bounded tiers") {
    ProgressionState state;
    REQUIRE(state.change_reputation("Helios", 31, 10));
    CHECK(state.tier("Helios") == ReputationTier::Trusted);
    CHECK(state.tier("Meridian") == ReputationTier::Neutral);
    REQUIRE(state.change_reputation("Helios", 500, 11));
    CHECK(state.reputation("Helios") == 100);
    CHECK(state.tier("Helios") == ReputationTier::Allied);
}

TEST_CASE("research evidence is persistent non-spendable knowledge") {
    ProgressionState state;
    state.integrate_evidence("EVID_VOLTAIC_CRYSTAL", {{"Materials", 2}, {"Energy", 2}});
    state.integrate_evidence("EVID_VOLTAIC_CRYSTAL", {{"Materials", 2}, {"Energy", 2}});
    CHECK(state.evidence("Materials") == 2);

    ResearchProject project{
        .id = "ADVANCED_MATERIALS",
        .evidence_thresholds = {{"Materials", 2}},
        .unique_evidence = {"EVID_VOLTAIC_CRYSTAL"},
        .technology_outputs = {"TECH_ADVANCED_METALLURGY"},
        .blueprint_outputs = {"BP_ADVANCED_ALLOY"},
    };
    REQUIRE(state.complete_research(project, 20));
    CHECK(state.has_technology("TECH_ADVANCED_METALLURGY"));
    CHECK(state.has_blueprint("BP_ADVANCED_ALLOY"));
    CHECK(state.evidence("Materials") == 2);
    CHECK_FALSE(state.complete_research(project, 21));
}

TEST_CASE("research prerequisites gate completion") {
    ProgressionState state;
    ResearchProject project{
        .id = "FUSION",
        .prerequisites = {"TECH_FUSION_FUEL_HANDLING"},
        .technology_outputs = {"TECH_FUSION_ENGINEERING"},
    };
    CHECK_FALSE(state.complete_research(project, 30));
}

TEST_CASE("finale transaction commits exactly once and preserves postgame state") {
    ProgressionState state;
    state.set_campaign_flag("MS_F01_COMPLETE");
    REQUIRE(state.commit_finale("ENDING_STABILIZE", 40));
    CHECK(state.finale_completed());
    CHECK(state.campaign_flag("MS_F01_COMPLETE"));
    CHECK(state.campaign_flag("ENDING_STABILIZE"));
    CHECK_FALSE(state.commit_finale("ENDING_SEVER", 41));
}
