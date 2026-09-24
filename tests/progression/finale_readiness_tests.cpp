#include "starforge/progression/finale_deployment.hpp"
#include "starforge/progression/finale_readiness.hpp"
#include "starforge/transactions/coordinator.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <array>
#include <utility>
#include <vector>

using namespace starforge::progression;

namespace {

FinaleReadinessView ready_view() {
    return FinaleReadinessView{
        .campaign_phase = CampaignPhase::VergeExpeditionP4,
        .wake_pattern_success = true,
        .second_cascade_crisis_active = true,
        .crownless_route_known = true,
        .mandatory_finale_knowledge_committed = true,
        .ms_f02_terminal_resolution_committed = false,
        .selected_ship_id = 7001,
        .ship_player_controlled = true,
        .ship_operational = true,
        .ship_reach_iv = true,
        .ship_crownless_route_compatible = true,
        .ship_power_sufficient = true,
        .ship_thermal_sufficient = true,
        .ship_navigation_and_sensors_operational = true,
        .ship_life_support_sufficient = true,
        .ship_cargo_capacity_sufficient = true,
        .ship_docking_eva_access_operational = true,
        .ship_fuel_propellant_sufficient = true,
        .ship_has_no_mandatory_capability_damage = true,
        .player_environmental_protection = true,
        .player_suit_seal_operational = true,
        .player_life_support_sufficient = true,
        .player_engineering_capability = true,
        .player_anomaly_scanner_capability = true,
        .player_combat_capability = true,
        .player_inventory_headroom = true,
        .horizon_command_operational = true,
        .horizon_long_range_communications = true,
        .horizon_analysis_data_access = true,
        .horizon_departure_service_interface = true,
        .horizon_strategic_persistence = true,
        .selected_robot_squad_id = 8001,
        .base_field_tcc = 6,
        .tactical_squad_command_load = 4,
        .robot_combat_role_present = true,
        .robot_utility_role_present = true,
        .robot_deployment_valid = true,
        .self_sufficient_support = {
            SupportChannel::NavigationRoute,
            SupportChannel::AnalysisContainment,
        },
    };
}

std::vector<SupportProviderSelection> valid_support() {
    return {
        {
            .channel = SupportChannel::NavigationRoute,
            .kind = SupportProviderKind::SelfSufficient,
            .provider_id = "HORIZON_NAV",
        },
        {
            .channel = SupportChannel::EngineeringRecovery,
            .kind = SupportProviderKind::ExternalCommitment,
            .provider_id = "MERIDIAN_RECOVERY",
            .committed_and_valid = true,
        },
        {
            .channel = SupportChannel::CombatSecurity,
            .kind = SupportProviderKind::ExternalCommitment,
            .provider_id = "HELIOS_ESCORT",
            .committed_and_valid = true,
        },
        {
            .channel = SupportChannel::AnalysisContainment,
            .kind = SupportProviderKind::SelfSufficient,
            .provider_id = "HORIZON_ANALYSIS",
        },
    };
}

struct ManifestFixture {
    starforge::station::LogisticsStore logistics{};
    starforge::station::StorageId storage{1};
    starforge::station::ResourceId fuel{1};
    starforge::station::ResourceId spares{2};

    ManifestFixture() {
        REQUIRE(logistics.add_storage(storage));
        REQUIRE(logistics.deposit(storage, fuel, 10));
        REQUIRE(logistics.deposit(storage, spares, 5));
    }

    std::vector<starforge::station::Reservation> reservations() const {
        return {
            {
                .id = starforge::station::ReservationId{101},
                .source = storage,
                .resource = fuel,
                .quantity = 4,
            },
            {
                .id = starforge::station::ReservationId{102},
                .source = storage,
                .resource = spares,
                .quantity = 2,
            },
        };
    }
};

} // namespace

TEST_CASE("departure readiness atomically locks support and reserves the manifest") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::transactions::TransactionCoordinator coordinator;
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);

    auto prepared = prepare_finale_departure(
        readiness,
        fixture.logistics,
        ready_view(),
        valid_support(),
        fixture.reservations(),
        candidate.value());
    REQUIRE(prepared);

    auto participants = prepared.value().participants();
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    const auto result = coordinator.commit();
    REQUIRE(result.committed);

    REQUIRE(readiness.current().has_value());
    CHECK(readiness.current()->status == FinaleReadinessStatus::Deployable);
    CHECK(readiness.current()->selected_ship_id == 7001);
    CHECK(readiness.current()->selected_robot_squad_id == 8001);
    CHECK(readiness.current()->support_providers.size() == 4);
    CHECK(fixture.logistics.reserved(fixture.storage, fixture.fuel) == 4);
    CHECK(fixture.logistics.reserved(fixture.storage, fixture.spares) == 2);
}

TEST_CASE("all four support channels require exactly one valid provider") {
    auto providers = valid_support();
    providers.pop_back();

    const auto report = FinaleReadinessStore::validate(ready_view(), providers);
    REQUIRE_FALSE(report.ready());
    CHECK(std::ranges::find(
              report.blockers, FinaleReadinessBlocker::SupportAnalysis) !=
          report.blockers.end());

    providers = valid_support();
    providers[1].committed_and_valid = false;
    const auto invalid_external = FinaleReadinessStore::validate(ready_view(), providers);
    REQUIRE_FALSE(invalid_external.ready());
    CHECK(std::ranges::find(
              invalid_external.blockers, FinaleReadinessBlocker::SupportEngineering) !=
          invalid_external.blockers.end());
}

TEST_CASE("base robot command capacity cannot be replaced by coalition support") {
    auto view = ready_view();
    view.base_field_tcc = 5;

    const auto report = FinaleReadinessStore::validate(view, valid_support());
    REQUIRE_FALSE(report.ready());
    CHECK(std::ranges::find(
              report.blockers, FinaleReadinessBlocker::RobotFieldTcc) !=
          report.blockers.end());
}

TEST_CASE("stale manifest state aborts before readiness or reservations mutate") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::transactions::TransactionCoordinator coordinator;
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);

    auto prepared = prepare_finale_departure(
        readiness,
        fixture.logistics,
        ready_view(),
        valid_support(),
        fixture.reservations(),
        candidate.value());
    REQUIRE(prepared);

    const starforge::station::ResourceId unrelated{3};
    REQUIRE(fixture.logistics.deposit(fixture.storage, unrelated, 1));

    auto participants = prepared.value().participants();
    const auto result = coordinator.prepare(candidate.value(), participants);
    REQUIRE_FALSE(result);
    CHECK(result.error() == starforge::transactions::TransactionError::StaleRevision);
    CHECK_FALSE(readiness.current().has_value());
    CHECK(fixture.logistics.reserved(fixture.storage, fixture.fuel) == 0);
    CHECK(fixture.logistics.reserved(fixture.storage, fixture.spares) == 0);
}

TEST_CASE("readiness invalidates and revalidates when locked capability changes") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::transactions::TransactionCoordinator coordinator;
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);

    auto view = ready_view();
    auto providers = valid_support();
    auto prepared = prepare_finale_departure(
        readiness,
        fixture.logistics,
        view,
        providers,
        fixture.reservations(),
        candidate.value());
    REQUIRE(prepared);
    auto participants = prepared.value().participants();
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    REQUIRE(coordinator.commit().committed);

    view.ship_operational = false;
    auto blocked = readiness.refresh_status(view, providers, fixture.logistics);
    REQUIRE_FALSE(blocked.ready());
    REQUIRE(readiness.current().has_value());
    CHECK(readiness.current()->status == FinaleReadinessStatus::Invalidated);
    CHECK(fixture.logistics.reserved(fixture.storage, fixture.fuel) == 4);

    view.ship_operational = true;
    auto restored = readiness.refresh_status(view, providers, fixture.logistics);
    REQUIRE(restored.ready());
    CHECK(readiness.current()->status == FinaleReadinessStatus::Deployable);
}

TEST_CASE("losing a reserved departure resource invalidates deployability") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::transactions::TransactionCoordinator coordinator;
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);

    auto view = ready_view();
    auto providers = valid_support();
    auto prepared = prepare_finale_departure(
        readiness,
        fixture.logistics,
        view,
        providers,
        fixture.reservations(),
        candidate.value());
    REQUIRE(prepared);
    auto participants = prepared.value().participants();
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    REQUIRE(coordinator.commit().committed);

    REQUIRE(fixture.logistics.release(starforge::station::ReservationId{101}));
    const auto report = readiness.refresh_status(view, providers, fixture.logistics);
    REQUIRE_FALSE(report.ready());
    CHECK(std::ranges::find(
              report.blockers, FinaleReadinessBlocker::ManifestReservationLost) !=
          report.blockers.end());
    CHECK(readiness.current()->status == FinaleReadinessStatus::Invalidated);
}

TEST_CASE("finale readiness snapshot preserves the locked deployment attempt") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::transactions::TransactionCoordinator coordinator;
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);

    auto prepared = prepare_finale_departure(
        readiness,
        fixture.logistics,
        ready_view(),
        valid_support(),
        fixture.reservations(),
        candidate.value());
    REQUIRE(prepared);
    auto participants = prepared.value().participants();
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    REQUIRE(coordinator.commit().committed);

    auto restored = FinaleReadinessStore::restore(readiness.snapshot());
    REQUIRE(restored);
    REQUIRE(restored.value().current().has_value());
    CHECK(restored.value().current()->selected_ship_id == 7001);
    CHECK(restored.value().current()->manifest_reservations.size() == 2);

    auto invalid = readiness.snapshot();
    invalid.current->transaction_id = 999'999;
    CHECK_FALSE(FinaleReadinessStore::restore(std::move(invalid)));
}


TEST_CASE("MS-F02 deployment atomically activates mission and locks deployed readiness") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::missions::MissionRuntime missions;
    starforge::transactions::TransactionCoordinator coordinator;

    starforge::missions::MissionRecord finale{};
    finale.id = starforge::missions::MissionId{9001};
    finale.retryable = true;
    finale.objectives = {
        {starforge::missions::ObjectiveId{1}, {}, true, false},
        {starforge::missions::ObjectiveId{2}, {starforge::missions::ObjectiveId{1}}, true, false},
    };
    REQUIRE(missions.add_mission(std::move(finale)));

    const auto readiness_tx = coordinator.candidate_transaction_id();
    REQUIRE(readiness_tx);
    auto prepared_readiness = prepare_finale_departure(
        readiness,
        fixture.logistics,
        ready_view(),
        valid_support(),
        fixture.reservations(),
        readiness_tx.value());
    REQUIRE(prepared_readiness);
    auto readiness_participants = prepared_readiness.value().participants();
    REQUIRE(coordinator.prepare(readiness_tx.value(), readiness_participants));
    REQUIRE(coordinator.commit().committed);

    const auto deployment_tx = coordinator.candidate_transaction_id();
    REQUIRE(deployment_tx);
    auto deployment = prepare_finale_mission_deployment(
        readiness,
        fixture.logistics,
        missions,
        ready_view(),
        valid_support(),
        starforge::missions::MissionId{9001},
        777,
        deployment_tx.value());
    REQUIRE(deployment);

    const auto expected_instance = deployment.value().mission_instance_id();
    auto participants = deployment.value().participants();
    REQUIRE(coordinator.prepare(deployment_tx.value(), participants));
    REQUIRE(coordinator.commit().committed);

    REQUIRE(missions.active_external_instance() == expected_instance);
    REQUIRE(missions.mission(starforge::missions::MissionId{9001}) != nullptr);
    CHECK(missions.mission(starforge::missions::MissionId{9001})->state ==
          starforge::missions::MissionState::Active);
    REQUIRE(missions.instance(expected_instance) != nullptr);
    CHECK(missions.instance(expected_instance)->deployment_tick == 777);
    CHECK(missions.instance(expected_instance)->objectives.front().state ==
          starforge::missions::ObjectiveState::Active);

    REQUIRE(readiness.current().has_value());
    CHECK(readiness.current()->status == FinaleReadinessStatus::Deployed);
    CHECK(readiness.current()->deployment_transaction_id == deployment_tx.value().raw());
    CHECK(readiness.current()->mission_instance_id == expected_instance.raw());
    CHECK(fixture.logistics.reserved(fixture.storage, fixture.fuel) == 4);
    CHECK(fixture.logistics.reserved(fixture.storage, fixture.spares) == 2);
}

TEST_CASE("MS-F02 deployment aborts atomically when manifest reservations change") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::missions::MissionRuntime missions;
    starforge::transactions::TransactionCoordinator coordinator;

    starforge::missions::MissionRecord finale{};
    finale.id = starforge::missions::MissionId{9002};
    finale.objectives = {{starforge::missions::ObjectiveId{1}, {}, true, false}};
    REQUIRE(missions.add_mission(std::move(finale)));

    const auto readiness_tx = coordinator.candidate_transaction_id();
    REQUIRE(readiness_tx);
    auto prepared_readiness = prepare_finale_departure(
        readiness,
        fixture.logistics,
        ready_view(),
        valid_support(),
        fixture.reservations(),
        readiness_tx.value());
    REQUIRE(prepared_readiness);
    auto readiness_participants = prepared_readiness.value().participants();
    REQUIRE(coordinator.prepare(readiness_tx.value(), readiness_participants));
    REQUIRE(coordinator.commit().committed);

    const auto deployment_tx = coordinator.candidate_transaction_id();
    REQUIRE(deployment_tx);
    auto deployment = prepare_finale_mission_deployment(
        readiness,
        fixture.logistics,
        missions,
        ready_view(),
        valid_support(),
        starforge::missions::MissionId{9002},
        888,
        deployment_tx.value());
    REQUIRE(deployment);

    REQUIRE(fixture.logistics.release(starforge::station::ReservationId{101}));

    auto participants = deployment.value().participants();
    const auto prepare_result = coordinator.prepare(deployment_tx.value(), participants);
    REQUIRE_FALSE(prepare_result);
    CHECK(prepare_result.error() == starforge::transactions::TransactionError::StaleRevision);
    CHECK_FALSE(missions.active_external_instance().has_value());
    CHECK(missions.mission(starforge::missions::MissionId{9002})->state ==
          starforge::missions::MissionState::Available);
    CHECK(readiness.current()->status == FinaleReadinessStatus::Deployable);
    CHECK(readiness.current()->mission_instance_id == 0);
}

TEST_CASE("MS-F02 deployment aborts atomically when mission ownership changes") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::missions::MissionRuntime missions;
    starforge::transactions::TransactionCoordinator coordinator;

    starforge::missions::MissionRecord finale{};
    finale.id = starforge::missions::MissionId{9003};
    finale.objectives = {{starforge::missions::ObjectiveId{1}, {}, true, false}};
    REQUIRE(missions.add_mission(std::move(finale)));

    const auto readiness_tx = coordinator.candidate_transaction_id();
    REQUIRE(readiness_tx);
    auto prepared_readiness = prepare_finale_departure(
        readiness,
        fixture.logistics,
        ready_view(),
        valid_support(),
        fixture.reservations(),
        readiness_tx.value());
    REQUIRE(prepared_readiness);
    auto readiness_participants = prepared_readiness.value().participants();
    REQUIRE(coordinator.prepare(readiness_tx.value(), readiness_participants));
    REQUIRE(coordinator.commit().committed);

    const auto deployment_tx = coordinator.candidate_transaction_id();
    REQUIRE(deployment_tx);
    auto deployment = prepare_finale_mission_deployment(
        readiness,
        fixture.logistics,
        missions,
        ready_view(),
        valid_support(),
        starforge::missions::MissionId{9003},
        999,
        deployment_tx.value());
    REQUIRE(deployment);

    starforge::missions::MissionRecord unrelated{};
    unrelated.id = starforge::missions::MissionId{9010};
    unrelated.objectives = {{starforge::missions::ObjectiveId{1}, {}, true, false}};
    REQUIRE(missions.add_mission(std::move(unrelated)));

    auto participants = deployment.value().participants();
    const auto prepare_result = coordinator.prepare(deployment_tx.value(), participants);
    REQUIRE_FALSE(prepare_result);
    CHECK(prepare_result.error() == starforge::transactions::TransactionError::StaleRevision);
    CHECK_FALSE(missions.active_external_instance().has_value());
    CHECK(readiness.current()->status == FinaleReadinessStatus::Deployable);
}

TEST_CASE("deployed finale readiness cannot create a second MS-F02 deployment") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::missions::MissionRuntime missions;
    starforge::transactions::TransactionCoordinator coordinator;

    starforge::missions::MissionRecord finale{};
    finale.id = starforge::missions::MissionId{9004};
    finale.objectives = {{starforge::missions::ObjectiveId{1}, {}, true, false}};
    REQUIRE(missions.add_mission(std::move(finale)));

    const auto readiness_tx = coordinator.candidate_transaction_id();
    REQUIRE(readiness_tx);
    auto prepared_readiness = prepare_finale_departure(
        readiness,
        fixture.logistics,
        ready_view(),
        valid_support(),
        fixture.reservations(),
        readiness_tx.value());
    REQUIRE(prepared_readiness);
    auto readiness_participants = prepared_readiness.value().participants();
    REQUIRE(coordinator.prepare(readiness_tx.value(), readiness_participants));
    REQUIRE(coordinator.commit().committed);

    const auto deployment_tx = coordinator.candidate_transaction_id();
    REQUIRE(deployment_tx);
    auto deployment = prepare_finale_mission_deployment(
        readiness,
        fixture.logistics,
        missions,
        ready_view(),
        valid_support(),
        starforge::missions::MissionId{9004},
        1000,
        deployment_tx.value());
    REQUIRE(deployment);
    auto participants = deployment.value().participants();
    REQUIRE(coordinator.prepare(deployment_tx.value(), participants));
    REQUIRE(coordinator.commit().committed);

    const auto duplicate_tx = coordinator.candidate_transaction_id();
    REQUIRE(duplicate_tx);
    auto duplicate = prepare_finale_mission_deployment(
        readiness,
        fixture.logistics,
        missions,
        ready_view(),
        valid_support(),
        starforge::missions::MissionId{9004},
        1001,
        duplicate_tx.value());
    REQUIRE_FALSE(duplicate);
    CHECK(duplicate.error() == FinaleDeploymentError::AlreadyDeployed);
}

TEST_CASE("deployed finale readiness snapshot preserves mission binding") {
    ManifestFixture fixture;
    FinaleReadinessStore readiness;
    starforge::missions::MissionRuntime missions;
    starforge::transactions::TransactionCoordinator coordinator;

    starforge::missions::MissionRecord finale{};
    finale.id = starforge::missions::MissionId{9005};
    finale.objectives = {{starforge::missions::ObjectiveId{1}, {}, true, false}};
    REQUIRE(missions.add_mission(std::move(finale)));

    const auto readiness_tx = coordinator.candidate_transaction_id();
    REQUIRE(readiness_tx);
    auto prepared_readiness = prepare_finale_departure(
        readiness,
        fixture.logistics,
        ready_view(),
        valid_support(),
        fixture.reservations(),
        readiness_tx.value());
    REQUIRE(prepared_readiness);
    auto readiness_participants = prepared_readiness.value().participants();
    REQUIRE(coordinator.prepare(readiness_tx.value(), readiness_participants));
    REQUIRE(coordinator.commit().committed);

    const auto deployment_tx = coordinator.candidate_transaction_id();
    REQUIRE(deployment_tx);
    auto deployment = prepare_finale_mission_deployment(
        readiness,
        fixture.logistics,
        missions,
        ready_view(),
        valid_support(),
        starforge::missions::MissionId{9005},
        1002,
        deployment_tx.value());
    REQUIRE(deployment);
    const auto instance_id = deployment.value().mission_instance_id();
    auto participants = deployment.value().participants();
    REQUIRE(coordinator.prepare(deployment_tx.value(), participants));
    REQUIRE(coordinator.commit().committed);

    auto restored = FinaleReadinessStore::restore(readiness.snapshot());
    REQUIRE(restored);
    REQUIRE(restored.value().current().has_value());
    CHECK(restored.value().current()->status == FinaleReadinessStatus::Deployed);
    CHECK(restored.value().current()->mission_instance_id == instance_id.raw());
    CHECK(restored.value().current()->deployment_transaction_id == deployment_tx.value().raw());
}
