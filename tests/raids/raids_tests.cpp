#include "starforge/raids/raids.hpp"

#include <catch2/catch_test_macros.hpp>

#include <map>

using namespace starforge::raids;

namespace {

class TestOwnership final : public PhysicalTransferPort {
public:
    void seed(StrategicAssetId asset, StrategicOwnerId owner_id) {
        owners_.emplace(asset, owner_id);
    }

    [[nodiscard]] bool transfer(StrategicAssetId asset,
                                StrategicOwnerId expected_owner,
                                StrategicOwnerId destination,
                                std::uint64_t transaction_id) override {
        if (transaction_id == 0) {
            return false;
        }
        const auto it = owners_.find(asset);
        if (it == owners_.end() || it->second != expected_owner) {
            return false;
        }
        it->second = destination;
        ++transfer_count_;
        return true;
    }

    [[nodiscard]] std::optional<StrategicOwnerId> owner(
        StrategicAssetId asset) const override {
        const auto it = owners_.find(asset);
        return it == owners_.end() ? std::nullopt
                                   : std::optional<StrategicOwnerId>{it->second};
    }

    [[nodiscard]] std::uint32_t transfer_count() const noexcept {
        return transfer_count_;
    }

private:
    std::map<StrategicAssetId, StrategicOwnerId> owners_{};
    std::uint32_t transfer_count_{0};
};

DefenseEventRecord make_defense(DefenseEventId id) {
    DefenseEventRecord event{};
    event.id = id;
    event.resources = DefenseResourceState{
        .attacker_units = 6,
        .powered_defense_mounts = 2,
        .defense_ammo = 5,
        .shield_energy = 30,
        .defender_robots = 3,
        .available_crew = 4,
    };
    return event;
}

}  // namespace

TEST_CASE("IMP-11 offensive raid stays mission-specialized and requires committed access") {
    RaidRuntime runtime{};
    RaidRecord raid{};
    raid.id = RaidId{1};
    raid.mission_id = starforge::missions::MissionId{10};
    raid.target_key = 9001;

    REQUIRE(runtime.add_raid(raid));
    REQUIRE(runtime.bind_raid_instance(
        RaidId{1}, starforge::missions::MissionInstanceId{20}));
    REQUIRE(runtime.transition_raid(
        RaidId{1}, RaidPhase::Preparation, {}));
    REQUIRE(runtime.transition_raid(
        RaidId{1}, RaidPhase::Approach, {}));

    REQUIRE_FALSE(runtime.transition_raid(
        RaidId{1}, RaidPhase::InteriorOperation, {}));

    RaidTransitionEvidence access{};
    access.access_committed = true;
    REQUIRE(runtime.transition_raid(
        RaidId{1}, RaidPhase::InteriorOperation, access));

    REQUIRE(runtime.record_raid_consequence(RaidId{1}, 101));
    REQUIRE(runtime.record_raid_consequence(RaidId{1}, 101));

    const auto* stored = runtime.raid(RaidId{1});
    REQUIRE(stored != nullptr);
    REQUIRE(stored->mission_id == starforge::missions::MissionId{10});
    REQUIRE(stored->mission_instance_id ==
            starforge::missions::MissionInstanceId{20});
    REQUIRE(stored->committed_consequence_transactions.size() == 1);
}

TEST_CASE("IMP-11 reinforcement calls reserve finite source exactly once") {
    RaidRuntime runtime{};
    REQUIRE(runtime.add_defense_event(make_defense(DefenseEventId{1})));
    REQUIRE(runtime.add_reinforcement_source(
        ReinforcementSourceRecord{ForceSourceId{7}, 3, 1}));

    ReinforcementResponseRecord disrupted{};
    disrupted.id = ReinforcementResponseId{11};
    disrupted.operation = OperationRef::defense(DefenseEventId{1});
    disrupted.source = ForceSourceId{7};
    disrupted.units = 2;
    disrupted.call_start_tick = 10;
    disrupted.departure_tick = 20;
    disrupted.arrival_tick = 30;
    REQUIRE(runtime.begin_reinforcement_call(disrupted));
    REQUIRE(runtime.interrupt_reinforcement_call(ReinforcementResponseId{11}));
    REQUIRE_FALSE(runtime.commit_reinforcement_call(
        ReinforcementResponseId{11}, 100, true, true));
    REQUIRE(runtime.reinforcement_source(ForceSourceId{7})->available_units == 3);

    ReinforcementResponseRecord committed = disrupted;
    committed.id = ReinforcementResponseId{12};
    committed.state = ReinforcementCallState::NotRequested;
    REQUIRE(runtime.begin_reinforcement_call(committed));
    REQUIRE(runtime.commit_reinforcement_call(
        ReinforcementResponseId{12}, 101, true, true));
    REQUIRE(runtime.commit_reinforcement_call(
        ReinforcementResponseId{12}, 101, false, false));
    REQUIRE(runtime.reinforcement_source(ForceSourceId{7})->available_units == 1);

    REQUIRE_FALSE(runtime.interrupt_reinforcement_call(
        ReinforcementResponseId{12}));
    runtime.advance_reinforcements(30, false);
    REQUIRE(runtime.reinforcement(ReinforcementResponseId{12})->state ==
            ReinforcementCallState::Responding);
    runtime.advance_reinforcements(30, true);
    REQUIRE(runtime.reinforcement(ReinforcementResponseId{12})->state ==
            ReinforcementCallState::Arrived);

    ReinforcementResponseRecord impossible = committed;
    impossible.id = ReinforcementResponseId{13};
    impossible.units = 2;
    REQUIRE(runtime.begin_reinforcement_call(impossible));
    const auto exhausted = runtime.commit_reinforcement_call(
        ReinforcementResponseId{13}, 102, true, true);
    REQUIRE_FALSE(exhausted);
    REQUIRE(exhausted.error() == RaidError::InsufficientFiniteSource);
}

TEST_CASE("IMP-11 active and offscreen defense share semantic boundary authority") {
    RaidRuntime active{};
    RaidRuntime offscreen{};
    const auto initial = make_defense(DefenseEventId{5});
    REQUIRE(active.add_defense_event(initial));
    REQUIRE(offscreen.add_defense_event(initial));

    const auto facts = offscreen.derive_offscreen_boundary(DefenseEventId{5});
    REQUIRE(offscreen.apply_defense_boundary(
        DefenseEventId{5}, facts, DefenseProjectionMode::Offscreen, 501));
    REQUIRE(active.apply_defense_boundary(
        DefenseEventId{5}, facts, DefenseProjectionMode::Active, 501));

    const auto* active_record = active.defense(DefenseEventId{5});
    const auto* offscreen_record = offscreen.defense(DefenseEventId{5});
    REQUIRE(active_record != nullptr);
    REQUIRE(offscreen_record != nullptr);
    REQUIRE(active_record->resources == offscreen_record->resources);
    REQUIRE(active_record->phase == offscreen_record->phase);
    REQUIRE(active_record->escalation == offscreen_record->escalation);
    REQUIRE(active_record->crew_injured == offscreen_record->crew_injured);
    REQUIRE(active_record->module_damage_events ==
            offscreen_record->module_damage_events);

    REQUIRE(offscreen.apply_defense_boundary(
        DefenseEventId{5}, facts, DefenseProjectionMode::Offscreen, 501));
    REQUIRE(offscreen.defense(DefenseEventId{5})
                ->applied_boundary_transactions.size() == 1);

    RaidRuntime objective{};
    REQUIRE(objective.add_defense_event(make_defense(DefenseEventId{51})));
    DefenseBoundaryFacts objective_facts{};
    objective_facts.attacker_objective_committed = true;
    REQUIRE(objective.apply_defense_boundary(
        DefenseEventId{51}, objective_facts, DefenseProjectionMode::Active, 502));
    REQUIRE(objective.defense(DefenseEventId{51})->phase ==
            DefensePhase::AttackerObjective);
}

TEST_CASE("IMP-11 hostile theft remains physical and extraction commits once") {
    RaidRuntime runtime{};
    REQUIRE(runtime.add_defense_event(make_defense(DefenseEventId{6})));

    TestOwnership ownership{};
    const StrategicAssetId asset{40};
    const StrategicOwnerId horizon{100};
    const StrategicOwnerId carrier{101};
    const StrategicOwnerId extracted{102};
    ownership.seed(asset, horizon);

    REQUIRE(runtime.commit_theft(
        DefenseEventId{6}, asset, horizon, carrier, 600, ownership));
    REQUIRE(runtime.commit_theft(
        DefenseEventId{6}, asset, horizon, carrier, 600, ownership));
    REQUIRE(ownership.transfer_count() == 1);
    REQUIRE(ownership.owner(asset) == carrier);
    REQUIRE(runtime.defense(DefenseEventId{6})->thefts.front().state ==
            TheftState::Carried);

    REQUIRE(runtime.commit_hostile_extraction(
        DefenseEventId{6}, asset, carrier, extracted, 601, ownership));
    REQUIRE(runtime.commit_hostile_extraction(
        DefenseEventId{6}, asset, carrier, extracted, 601, ownership));
    REQUIRE(ownership.transfer_count() == 2);
    REQUIRE(ownership.owner(asset) == extracted);
    REQUIRE(runtime.defense(DefenseEventId{6})->thefts.front().state ==
            TheftState::Extracted);
}

TEST_CASE("IMP-11 dynamic event generation is deterministic and recovery grace blocks attacks") {
    RaidRuntime left{};
    RaidRuntime right{};
    const DynamicEventCandidate candidates[] = {
        {DynamicEventFamily::Distress, 1, 10, 2, false, 120},
        {DynamicEventFamily::Salvage, 2, 20, 1, false, 120},
    };

    const auto first = left.evaluate_dynamic_events(0, 123456, candidates);
    const auto replay = right.evaluate_dynamic_events(0, 123456, candidates);
    REQUIRE(first);
    REQUIRE(replay);
    REQUIRE(first.value() == replay.value());
    REQUIRE(left.serialize() == right.serialize());

    RaidRuntime grace_runtime{};
    REQUIRE(grace_runtime.add_defense_event(make_defense(DefenseEventId{30})));
    DefenseBoundaryFacts terminal{};
    terminal.attacker_losses = 6;
    terminal.attackers_withdrawn = true;
    REQUIRE(grace_runtime.apply_defense_boundary(
        DefenseEventId{30}, terminal, DefenseProjectionMode::Offscreen, 700));
    REQUIRE(grace_runtime.resolve_defense(
        DefenseEventId{30}, 701, 100, 300));

    const DynamicEventCandidate attack[] = {
        {DynamicEventFamily::HorizonDefense, 50, 60, 1, true, 0},
    };
    const auto blocked =
        grace_runtime.evaluate_dynamic_events(100, 9999, attack);
    REQUIRE(blocked);
    REQUIRE_FALSE(blocked.value().has_value());

    grace_runtime.advance_strategic_time(200, true);
    REQUIRE(grace_runtime.recovery_grace().has_value());
    REQUIRE(grace_runtime.recovery_grace()->remaining_ticks == 300);

    grace_runtime.advance_strategic_time(500, false);
    REQUIRE_FALSE(grace_runtime.recovery_grace().has_value());

    const auto allowed =
        grace_runtime.evaluate_dynamic_events(700, 9999, attack);
    REQUIRE(allowed);
    REQUIRE(allowed.value().has_value());
    REQUIRE(grace_runtime.critical_horizon_event() == allowed.value());
}

TEST_CASE("IMP-11 communication delivery and remote orders respect link and revision") {
    RaidRuntime runtime{};
    const auto message = runtime.queue_message(44, 10, 20, false);
    runtime.advance_messages(20, CommunicationState::Unavailable);
    REQUIRE(runtime.message(message)->state == StrategicMessageState::Queued);

    runtime.advance_messages(25, CommunicationState::Available);
    REQUIRE(runtime.message(message)->state == StrategicMessageState::Delivered);
    REQUIRE(runtime.message(message)->source_tick == 10);
    REQUIRE(runtime.message(message)->delivered_tick == 25);

    RemoteOrderRecord order{};
    order.id = RemoteOrderId{1};
    order.target_key = 88;
    order.expected_target_revision = 7;
    order.issued_tick = 30;
    REQUIRE(runtime.queue_remote_order(order));

    REQUIRE_FALSE(runtime.deliver_remote_order(
        RemoteOrderId{1}, CommunicationState::Unavailable, 7, 800));
    REQUIRE(runtime.remote_order(RemoteOrderId{1})->state ==
            RemoteOrderState::Queued);

    REQUIRE(runtime.deliver_remote_order(
        RemoteOrderId{1}, CommunicationState::Available, 7, 800));
    REQUIRE(runtime.deliver_remote_order(
        RemoteOrderId{1}, CommunicationState::Available, 7, 800));
    REQUIRE(runtime.remote_order(RemoteOrderId{1})->state ==
            RemoteOrderState::Accepted);
}

TEST_CASE("IMP-11 recovery transit enforces chronology and does not imply ship recall") {
    RaidRuntime runtime{};
    RecoveryTransitRequest request{};
    request.source_mission_instance =
        starforge::missions::MissionInstanceId{91};
    request.failure_transaction = 900;
    request.origin = StrategicLocationId{1};
    request.destination = StrategicLocationId{2};
    request.start_tick = 100;
    request.minimum_service_ticks = 40;
    request.shortest_route_ticks = 80;
    request.planned_arrival_tick = 180;
    request.move_primary_ship = false;
    request.included_assets = {StrategicAssetId{10}};

    REQUIRE(runtime.start_recovery_transit(request));
    runtime.advance_recovery(179, true);
    REQUIRE(runtime.recovery_transit()->status ==
            RecoveryTransitStatus::InTransit);

    runtime.advance_recovery(180, false);
    REQUIRE(runtime.recovery_transit()->status ==
            RecoveryTransitStatus::AwaitingReroute);

    REQUIRE(runtime.reroute_recovery(
        StrategicLocationId{3}, 180, 10, 50));
    REQUIRE(runtime.recovery_transit()->planned_arrival_tick >= 230);
    REQUIRE_FALSE(runtime.recovery_transit()->move_primary_ship);

    runtime.advance_recovery(229, true);
    REQUIRE(runtime.recovery_transit()->status ==
            RecoveryTransitStatus::InTransit);
    runtime.advance_recovery(230, true);
    REQUIRE(runtime.recovery_transit()->status ==
            RecoveryTransitStatus::Arrived);
}

TEST_CASE("IMP-11 save load preserves strategic state and idempotency markers") {
    RaidRuntime runtime{};
    RaidRecord raid{};
    raid.id = RaidId{1};
    raid.mission_id = starforge::missions::MissionId{2};
    raid.target_key = 3;
    REQUIRE(runtime.add_raid(raid));
    REQUIRE(runtime.record_raid_consequence(RaidId{1}, 10));

    REQUIRE(runtime.add_defense_event(make_defense(DefenseEventId{4})));
    REQUIRE(runtime.add_reinforcement_source(
        ReinforcementSourceRecord{ForceSourceId{5}, 4, 1}));
    ReinforcementResponseRecord response{};
    response.id = ReinforcementResponseId{6};
    response.operation = OperationRef::defense(DefenseEventId{4});
    response.source = ForceSourceId{5};
    response.units = 2;
    response.call_start_tick = 10;
    response.departure_tick = 20;
    response.arrival_tick = 30;
    REQUIRE(runtime.begin_reinforcement_call(response));
    REQUIRE(runtime.commit_reinforcement_call(
        ReinforcementResponseId{6}, 11, true, true));

    const auto encoded = runtime.serialize();
    auto decoded = RaidRuntime::deserialize(encoded);
    REQUIRE(decoded);
    REQUIRE(decoded.value().serialize() == encoded);
    REQUIRE(decoded.value().is_stable_for_snapshot());
    REQUIRE(decoded.value().reinforcement_source(ForceSourceId{5})
                ->available_units == 2);
    REQUIRE(decoded.value().record_raid_consequence(RaidId{1}, 10));
    REQUIRE(decoded.value().raid(RaidId{1})
                ->committed_consequence_transactions.size() == 1);
}

TEST_CASE("IMP-11 load rejects dangling strategic links") {
    REQUIRE_FALSE(RaidRuntime::deserialize(
        "SFS11 0 0 1 1 99 0 0 0 0 0 0 0 0 0\n"));
}
