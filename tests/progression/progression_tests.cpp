#include "starforge/progression/progression.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <limits>

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

TEST_CASE("credits reject overflow without consuming the transaction id") {
    ProgressionState state;
    const auto maximum = std::numeric_limits<std::int64_t>::max();
    REQUIRE(state.credit(maximum, 1));
    CHECK_FALSE(state.credit(1, 2));
    CHECK(state.credits() == maximum);
    REQUIRE(state.debit(maximum, 3));
    REQUIRE(state.credit(1, 2));
    CHECK(state.credits() == 1);
}

TEST_CASE("reputation uses canonical independent bounded tiers") {
    ProgressionState state;
    REQUIRE(state.change_reputation("Helios", 31, 10));
    CHECK(state.tier("Helios") == ReputationTier::Trusted);
    CHECK(state.tier("Meridian") == ReputationTier::Neutral);
    REQUIRE(state.change_reputation("Helios", 500, 11));
    CHECK(state.reputation("Helios") == 100);
    CHECK(state.tier("Helios") == ReputationTier::Allied);
    REQUIRE(state.change_reputation("Helios", std::numeric_limits<std::int32_t>::min(), 12));
    CHECK(state.reputation("Helios") == -100);
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

TEST_CASE("research evidence accumulation saturates instead of wrapping") {
    ProgressionState state;
    const auto maximum = std::numeric_limits<std::uint32_t>::max();
    state.integrate_evidence("EVID_A", {{"Materials", maximum}});
    state.integrate_evidence("EVID_B", {{"Materials", 1}});
    CHECK(state.evidence("Materials") == maximum);
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

TEST_CASE("progression snapshot preserves exactly-once finale and research across reload") {
    ProgressionState state;
    REQUIRE(state.credit(750, 100));
    REQUIRE(state.change_reputation("Helios", 45, 101));
    state.integrate_evidence("EVID_CORE", {{"Energy", 4}});
    ResearchProject project{
        .id = "CORE_RESEARCH",
        .evidence_thresholds = {{"Energy", 4}},
        .unique_evidence = {"EVID_CORE"},
        .technology_outputs = {"TECH_CORE"},
        .blueprint_outputs = {"BP_CORE"},
    };
    REQUIRE(state.complete_research(project, 102));
    state.set_campaign_flag("MS_F01_COMPLETE");
    REQUIRE(state.commit_finale("ENDING_STABILIZE", 103));

    auto restored_result = ProgressionState::restore(state.snapshot());
    REQUIRE(restored_result);
    auto restored = std::move(restored_result).value();

    CHECK(restored.credits() == 750);
    CHECK(restored.reputation("Helios") == 45);
    CHECK(restored.evidence("Energy") == 4);
    CHECK(restored.has_evidence("EVID_CORE"));
    CHECK(restored.has_technology("TECH_CORE"));
    CHECK(restored.has_blueprint("BP_CORE"));
    CHECK(restored.campaign_flag("MS_F01_COMPLETE"));
    CHECK(restored.campaign_flag("ENDING_STABILIZE"));
    CHECK(restored.finale_completed());

    // Transaction history is persistent authority: replay after reload cannot duplicate effects.
    CHECK_FALSE(restored.credit(750, 100));
    CHECK_FALSE(restored.complete_research(project, 102));
    CHECK_FALSE(restored.commit_finale("ENDING_SEVER", 103));
    CHECK_FALSE(restored.commit_finale("ENDING_SEVER", 104));
    CHECK(restored.credits() == 750);
    CHECK(restored.campaign_flag("ENDING_STABILIZE"));
    CHECK_FALSE(restored.campaign_flag("ENDING_SEVER"));
}

TEST_CASE("progression restore rejects invalid authoritative state") {
    ProgressionSnapshot snapshot;
    snapshot.credits = -1;
    CHECK_FALSE(ProgressionState::restore(snapshot));

    snapshot.credits = 0;
    snapshot.reputation["Helios"] = 101;
    CHECK_FALSE(ProgressionState::restore(snapshot));

    snapshot.reputation.clear();
    snapshot.committed_transactions.insert(0);
    CHECK_FALSE(ProgressionState::restore(snapshot));
}
