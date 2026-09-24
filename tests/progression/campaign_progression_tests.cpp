#include "starforge/progression/campaign_progression.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace starforge::progression;

namespace {

ProgressionState progression_with_gate_state() {
    ProgressionState progression;
    progression.set_campaign_flag("OUTSIDE_CONTACT_ESTABLISHED");
    REQUIRE(progression.change_reputation("Helios", 35, 101));
    progression.integrate_evidence("EVID_REACH", {{"Navigation", 3}});

    ResearchProject research{
        .id = "REACH_II_RESEARCH",
        .evidence_thresholds = {{"Navigation", 3}},
        .unique_evidence = {"EVID_REACH"},
        .technology_outputs = {"TECH_REACH_II"},
        .blueprint_outputs = {"BP_REACH_II_DRIVE"},
    };
    REQUIRE(progression.complete_research(research, 102));
    return progression;
}

} // namespace

TEST_CASE("campaign phase advancement is explicit capability based and exactly once") {
    auto progression = progression_with_gate_state();
    CampaignProgression campaign;

    CampaignGate gate{
        .required_capabilities = {"HORIZON_REPEATABLE_LOCAL_EXPEDITIONS", "VERIFIED_LOCAL_ROUTE_NETWORK"},
        .required_campaign_flags = {"OUTSIDE_CONTACT_ESTABLISHED"},
        .required_technologies = {"TECH_REACH_II"},
        .required_blueprints = {"BP_REACH_II_DRIVE"},
        .reputation = {{"Helios", 30}},
        .evidence = {{"Navigation", 3}},
    };
    CapabilityView capabilities{{
        "HORIZON_REPEATABLE_LOCAL_EXPEDITIONS",
        "VERIFIED_LOCAL_ROUTE_NETWORK",
    }};

    REQUIRE(campaign.advance(CampaignPhase::LocalOperatorP1, gate, capabilities, progression, 500));
    CHECK(campaign.phase() == CampaignPhase::LocalOperatorP1);
    CHECK_FALSE(campaign.advance(CampaignPhase::FrontierNetworkP2, CampaignGate{}, CapabilityView{},
                                 progression, 500));
}

TEST_CASE("campaign gates read external capabilities without owning or inventing them") {
    auto progression = progression_with_gate_state();
    CampaignProgression campaign;
    CampaignGate gate{
        .required_capabilities = {"VERIFIED_LOCAL_ROUTE_NETWORK"},
        .required_campaign_flags = {"OUTSIDE_CONTACT_ESTABLISHED"},
    };

    const auto result = campaign.advance(
        CampaignPhase::LocalOperatorP1, gate, CapabilityView{}, progression, 501);
    REQUIRE_FALSE(result);
    CHECK(result.error() == CampaignProgressionError::MissingCapability);
    CHECK(campaign.phase() == CampaignPhase::RecoveryP0);
}

TEST_CASE("campaign phases cannot skip canonical progression states") {
    auto progression = progression_with_gate_state();
    CampaignProgression campaign;

    const auto result = campaign.advance(
        CampaignPhase::FrontierNetworkP2, CampaignGate{}, CapabilityView{}, progression, 502);
    REQUIRE_FALSE(result);
    CHECK(result.error() == CampaignProgressionError::InvalidTransition);
    CHECK(campaign.phase() == CampaignPhase::RecoveryP0);
}

TEST_CASE("postgame phase requires committed finale resolution") {
    ProgressionState progression;
    CampaignProgressionSnapshot snapshot{
        .phase = CampaignPhase::VergeExpeditionP4,
        .committed_transactions = {1, 2, 3, 4},
    };
    auto restored = CampaignProgression::restore(snapshot);
    REQUIRE(restored);

    auto campaign = std::move(restored).value();
    auto blocked = campaign.advance(
        CampaignPhase::PostgameP5, CampaignGate{}, CapabilityView{}, progression, 600);
    REQUIRE_FALSE(blocked);
    CHECK(blocked.error() == CampaignProgressionError::MissingFinaleCommit);

    REQUIRE(progression.commit_finale(
        FinalChoice::Contain, 601, starforge::core::SimulationTick{42'000}));
    REQUIRE(campaign.advance(
        CampaignPhase::PostgameP5, CampaignGate{}, CapabilityView{}, progression, 600));
    CHECK(campaign.phase() == CampaignPhase::PostgameP5);
}

TEST_CASE("campaign progression snapshot preserves phase and replay protection") {
    auto progression = progression_with_gate_state();
    CampaignProgression campaign;
    REQUIRE(campaign.advance(
        CampaignPhase::LocalOperatorP1, CampaignGate{}, CapabilityView{}, progression, 700));

    auto restored = CampaignProgression::restore(campaign.snapshot());
    REQUIRE(restored);
    CHECK(restored.value().phase() == CampaignPhase::LocalOperatorP1);

    CHECK_FALSE(restored.value().advance(
        CampaignPhase::FrontierNetworkP2, CampaignGate{}, CapabilityView{}, progression, 700));
}

TEST_CASE("campaign progression restore rejects malformed persisted state") {
    CampaignProgressionSnapshot snapshot;
    snapshot.committed_transactions.insert(0);
    CHECK_FALSE(CampaignProgression::restore(snapshot));

    snapshot.committed_transactions.clear();
    snapshot.phase = static_cast<CampaignPhase>(255);
    CHECK_FALSE(CampaignProgression::restore(snapshot));
}
