#include "starforge/progression/progression.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <limits>

using namespace starforge::progression;

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

TEST_CASE("finale choice commits exactly once before phase-E campaign completion") {
    ProgressionState state;
    state.set_campaign_flag("MS_F01_COMPLETE");
    REQUIRE(state.commit_finale(FinalChoice::Stabilize, 40, starforge::core::SimulationTick{12'345}));
    REQUIRE(state.postgame_resolution().has_value());
    CHECK(state.postgame_resolution()->choice == FinalChoice::Stabilize);
    CHECK(state.postgame_resolution()->transaction_id == 40);
    CHECK(state.postgame_resolution()->commit_tick.raw() == 12'345);
    CHECK_FALSE(state.main_campaign_complete());
    CHECK(state.campaign_flag("MS_F01_COMPLETE"));
    CHECK_FALSE(state.commit_finale(FinalChoice::Sever, 41, starforge::core::SimulationTick{12'346}));

    REQUIRE(state.complete_main_campaign(42));
    CHECK(state.main_campaign_complete());
    CHECK_FALSE(state.complete_main_campaign(43));
}

TEST_CASE("campaign completion cannot precede final resolution") {
    ProgressionState state;
    CHECK_FALSE(state.complete_main_campaign(50));
    REQUIRE(state.commit_finale(FinalChoice::Contain, 51, starforge::core::SimulationTick{900}));
    REQUIRE(state.complete_main_campaign(50));
}

TEST_CASE("progression snapshot preserves exactly-once finale and research across reload") {
    ProgressionState state;
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
    REQUIRE(state.commit_finale(FinalChoice::Stabilize, 103, starforge::core::SimulationTick{77'777}));
    REQUIRE(state.complete_main_campaign(104));

    auto restored_result = ProgressionState::restore(state.snapshot());
    REQUIRE(restored_result);
    auto restored = std::move(restored_result).value();

    CHECK(restored.reputation("Helios") == 45);
    CHECK(restored.evidence("Energy") == 4);
    CHECK(restored.has_evidence("EVID_CORE"));
    CHECK(restored.has_technology("TECH_CORE"));
    CHECK(restored.has_blueprint("BP_CORE"));
    CHECK(restored.campaign_flag("MS_F01_COMPLETE"));
    REQUIRE(restored.postgame_resolution().has_value());
    CHECK(restored.postgame_resolution()->choice == FinalChoice::Stabilize);
    CHECK(restored.postgame_resolution()->transaction_id == 103);
    CHECK(restored.postgame_resolution()->commit_tick.raw() == 77'777);
    CHECK(restored.main_campaign_complete());

    // Persistent authority rejects both same-ID replay and a conflicting second ending.
    CHECK_FALSE(restored.complete_research(project, 102));
    CHECK_FALSE(restored.commit_finale(FinalChoice::Sever, 103, starforge::core::SimulationTick{80'000}));
    CHECK_FALSE(restored.commit_finale(FinalChoice::Sever, 105, starforge::core::SimulationTick{80'000}));
    CHECK_FALSE(restored.complete_main_campaign(106));
    CHECK(restored.postgame_resolution()->choice == FinalChoice::Stabilize);
}

TEST_CASE("progression restore rejects invalid authoritative state") {
    ProgressionSnapshot snapshot;
    snapshot.reputation["Helios"] = 101;
    CHECK_FALSE(ProgressionState::restore(snapshot));

    snapshot.reputation.clear();
    snapshot.committed_transactions.insert(0);
    CHECK_FALSE(ProgressionState::restore(snapshot));

    snapshot.committed_transactions.clear();
    snapshot.postgame_resolution = PostgameResolutionState{
        .choice = FinalChoice::Sever,
        .transaction_id = 77,
        .commit_tick = starforge::core::SimulationTick{500},
        .schema_version = 1U,
    };
    CHECK_FALSE(ProgressionState::restore(snapshot));

    snapshot.committed_transactions.insert(77);
    snapshot.postgame_resolution->schema_version = 2U;
    CHECK_FALSE(ProgressionState::restore(snapshot));
}
