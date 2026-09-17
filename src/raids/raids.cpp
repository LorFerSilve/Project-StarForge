#include "starforge/raids/raids.hpp"

#include "starforge/core/rng.hpp"

#include <algorithm>
#include <limits>
#include <ranges>
#include <sstream>
#include <tuple>
#include <utility>

namespace starforge::raids {
namespace {

constexpr std::uint64_t kEvaluationIntervalTicks = 600;

template <typename Map, typename Id>
[[nodiscard]] auto* find_record(Map& records, Id id) noexcept {
    const auto it = records.find(id);
    return it == records.end() ? nullptr : &it->second;
}

template <typename Map, typename Id>
[[nodiscard]] const auto* find_record(const Map& records, Id id) noexcept {
    const auto it = records.find(id);
    return it == records.end() ? nullptr : &it->second;
}

[[nodiscard]] bool checked_add(std::uint64_t left, std::uint64_t right,
                               std::uint64_t& result) noexcept {
    if (right > std::numeric_limits<std::uint64_t>::max() - left) {
        return false;
    }
    result = left + right;
    return true;
}

[[nodiscard]] bool raid_transition_allowed(RaidPhase from, RaidPhase to,
                                           const RaidTransitionEvidence& evidence) noexcept {
    if (from == to) {
        return true;
    }

    switch (from) {
    case RaidPhase::Intelligence:
        return to == RaidPhase::Preparation;
    case RaidPhase::Preparation:
        return to == RaidPhase::Approach;
    case RaidPhase::Approach:
        if (to == RaidPhase::ExternalOperation || to == RaidPhase::AccessOrBreach) {
            return true;
        }
        return to == RaidPhase::InteriorOperation && evidence.access_committed;
    case RaidPhase::ExternalOperation:
        return to == RaidPhase::AccessOrBreach || to == RaidPhase::Withdrawal;
    case RaidPhase::AccessOrBreach:
        return to == RaidPhase::InteriorOperation && evidence.access_committed;
    case RaidPhase::InteriorOperation:
        return to == RaidPhase::ObjectiveLootSabotage ||
               to == RaidPhase::EscalationResponse ||
               to == RaidPhase::Withdrawal;
    case RaidPhase::ObjectiveLootSabotage:
        if (to == RaidPhase::EscalationResponse) {
            return evidence.objective_or_sabotage_committed;
        }
        return to == RaidPhase::Withdrawal;
    case RaidPhase::EscalationResponse:
        return to == RaidPhase::InteriorOperation || to == RaidPhase::Withdrawal;
    case RaidPhase::Withdrawal:
        return to == RaidPhase::Aftermath && evidence.withdrawal_route_reached &&
               evidence.aftermath_ready;
    case RaidPhase::Aftermath:
        return false;
    }
    return false;
}

[[nodiscard]] bool operation_exists(const std::map<RaidId, RaidRecord>& raids,
                                    const std::map<DefenseEventId, DefenseEventRecord>& defenses,
                                    OperationRef operation) noexcept {
    if (operation.id == 0) {
        return false;
    }
    if (operation.kind == OperationRef::Kind::Raid) {
        return raids.contains(RaidId{operation.id});
    }
    return defenses.contains(DefenseEventId{operation.id});
}

template <typename Enum>
[[nodiscard]] unsigned enum_value(Enum value) noexcept {
    return static_cast<unsigned>(value);
}

[[nodiscard]] bool valid_raid_phase(unsigned value) noexcept {
    return value <= enum_value(RaidPhase::Aftermath);
}

[[nodiscard]] bool valid_escalation(unsigned value) noexcept {
    return value <= enum_value(EscalationState::WithdrawalEvacuation);
}

[[nodiscard]] bool valid_response_state(unsigned value) noexcept {
    return value <= enum_value(ReinforcementCallState::CancelledByValidCause);
}

[[nodiscard]] bool valid_defense_phase(unsigned value) noexcept {
    return value <= enum_value(DefensePhase::Resolved);
}

[[nodiscard]] bool valid_dynamic_family(unsigned value) noexcept {
    return value <= enum_value(DynamicEventFamily::RecoveryFollowUp);
}

[[nodiscard]] bool valid_dynamic_state(unsigned value) noexcept {
    return value <= enum_value(DynamicEventState::Cancelled);
}

[[nodiscard]] bool valid_message_state(unsigned value) noexcept {
    return value <= enum_value(StrategicMessageState::Invalidated);
}

[[nodiscard]] bool valid_remote_order_state(unsigned value) noexcept {
    return value <= enum_value(RemoteOrderState::Rejected);
}

[[nodiscard]] bool valid_recovery_status(unsigned value) noexcept {
    return value <= enum_value(RecoveryTransitStatus::Arrived);
}

[[nodiscard]] bool valid_theft_state(unsigned value) noexcept {
    return value <= enum_value(TheftState::Extracted);
}

}  // namespace

core::Result<void, RaidError> RaidRuntime::add_raid(RaidRecord raid) {
    if (!raid.id || !raid.mission_id || raid.target_key == 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }
    if (raids_.contains(raid.id)) {
        return core::Result<void, RaidError>::failure(RaidError::DuplicateRecord);
    }
    raid.revision = std::max<std::uint64_t>(1, raid.revision);
    raids_.emplace(raid.id, std::move(raid));
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::bind_raid_instance(
    RaidId raid_id, missions::MissionInstanceId instance_id) {
    auto* record = find_record(raids_, raid_id);
    if (record == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (!instance_id) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }
    if (record->mission_instance_id && *record->mission_instance_id != instance_id) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }
    if (!record->mission_instance_id) {
        record->mission_instance_id = instance_id;
        ++record->revision;
    }
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::transition_raid(
    RaidId raid_id, RaidPhase next, const RaidTransitionEvidence& evidence) {
    auto* record = find_record(raids_, raid_id);
    if (record == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (!raid_transition_allowed(record->phase, next, evidence)) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidTransition);
    }
    if (record->phase != next) {
        record->phase = next;
        ++record->revision;
    }
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::record_raid_consequence(
    RaidId raid_id, std::uint64_t transaction_id) {
    auto* record = find_record(raids_, raid_id);
    if (record == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (transaction_id == 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }
    if (std::ranges::find(record->committed_consequence_transactions, transaction_id) !=
        record->committed_consequence_transactions.end()) {
        return core::Result<void, RaidError>::success();
    }
    record->committed_consequence_transactions.push_back(transaction_id);
    ++record->revision;
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::set_raid_escalation(
    RaidId raid_id, EscalationState escalation) {
    auto* record = find_record(raids_, raid_id);
    if (record == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (record->escalation != escalation) {
        record->escalation = escalation;
        ++record->revision;
    }
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::add_reinforcement_source(
    ReinforcementSourceRecord source) {
    if (!source.id) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }
    if (reinforcement_sources_.contains(source.id)) {
        return core::Result<void, RaidError>::failure(RaidError::DuplicateRecord);
    }
    source.revision = std::max<std::uint64_t>(1, source.revision);
    reinforcement_sources_.emplace(source.id, source);
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::begin_reinforcement_call(
    ReinforcementResponseRecord response) {
    if (!response.id || !response.source || response.units == 0 ||
        response.state != ReinforcementCallState::NotRequested ||
        response.departure_tick < response.call_start_tick ||
        response.arrival_tick < response.departure_tick) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }
    if (reinforcements_.contains(response.id)) {
        return core::Result<void, RaidError>::failure(RaidError::DuplicateRecord);
    }
    if (!reinforcement_sources_.contains(response.source) ||
        !operation_exists(raids_, defenses_, response.operation)) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }

    response.state = ReinforcementCallState::Calling;
    response.route_valid = false;
    response.revision = std::max<std::uint64_t>(1, response.revision);
    reinforcements_.emplace(response.id, response);
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::interrupt_reinforcement_call(
    ReinforcementResponseId response_id) {
    auto* response = find_record(reinforcements_, response_id);
    if (response == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (response->state != ReinforcementCallState::Calling) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }
    response->state = ReinforcementCallState::Disrupted;
    ++response->revision;
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::commit_reinforcement_call(
    ReinforcementResponseId response_id, std::uint64_t transaction_id,
    bool communication_path_available, bool route_valid) {
    auto* response = find_record(reinforcements_, response_id);
    if (response == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (transaction_id == 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }
    if (response->commit_transaction == transaction_id &&
        response->state != ReinforcementCallState::Calling &&
        response->state != ReinforcementCallState::Disrupted) {
        return core::Result<void, RaidError>::success();
    }
    if (response->state != ReinforcementCallState::Calling) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }
    if (!communication_path_available) {
        return core::Result<void, RaidError>::failure(RaidError::CommunicationUnavailable);
    }
    if (!route_valid) {
        return core::Result<void, RaidError>::failure(RaidError::RouteUnavailable);
    }

    auto* source = find_record(reinforcement_sources_, response->source);
    if (source == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (source->available_units < response->units) {
        return core::Result<void, RaidError>::failure(RaidError::InsufficientFiniteSource);
    }

    source->available_units -= response->units;
    ++source->revision;
    response->state = ReinforcementCallState::Committed;
    response->commit_transaction = transaction_id;
    response->route_valid = true;
    ++response->revision;
    return core::Result<void, RaidError>::success();
}

void RaidRuntime::advance_reinforcements(std::uint64_t now_tick,
                                         bool insertion_available) {
    for (auto& [id, response] : reinforcements_) {
        static_cast<void>(id);
        if (response.state == ReinforcementCallState::Committed &&
            now_tick >= response.departure_tick) {
            response.state = ReinforcementCallState::Responding;
            ++response.revision;
        }
        if (response.state == ReinforcementCallState::Responding &&
            now_tick >= response.arrival_tick && insertion_available) {
            response.state = ReinforcementCallState::Arrived;
            ++response.revision;
        }
    }
}

core::Result<void, RaidError> RaidRuntime::add_defense_event(
    DefenseEventRecord event) {
    if (!event.id || event.resources.shield_energy < 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }
    if (defenses_.contains(event.id)) {
        return core::Result<void, RaidError>::failure(RaidError::DuplicateRecord);
    }
    if (event.source_dynamic_event &&
        !dynamic_events_.contains(*event.source_dynamic_event)) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    event.revision = std::max<std::uint64_t>(1, event.revision);
    defenses_.emplace(event.id, std::move(event));
    return core::Result<void, RaidError>::success();
}

DefenseBoundaryFacts RaidRuntime::derive_offscreen_boundary(
    DefenseEventId defense_id) const noexcept {
    DefenseBoundaryFacts facts{};
    const auto* event = find_record(defenses_, defense_id);
    if (event == nullptr || event->phase == DefensePhase::Resolved ||
        event->phase == DefensePhase::Recovery || event->resources.attacker_units == 0) {
        return facts;
    }

    const auto shots = std::min(event->resources.powered_defense_mounts,
                                event->resources.defense_ammo);
    facts.defense_ammo_spent = shots;
    facts.attacker_losses = std::min(event->resources.attacker_units, shots);

    const auto surviving_attackers = event->resources.attacker_units - facts.attacker_losses;
    if (surviving_attackers == 0) {
        facts.attackers_withdrawn = true;
        return facts;
    }

    const auto requested_shield = static_cast<std::int64_t>(surviving_attackers) * 10;
    facts.shield_energy_spent = std::min(event->resources.shield_energy, requested_shield);
    const auto shield_after = event->resources.shield_energy - facts.shield_energy_spent;

    if (shield_after == 0) {
        facts.boarding_attempt_started = true;
        facts.breach_committed = true;
        const auto pressure = std::max<std::uint32_t>(1, surviving_attackers / 2);
        facts.defender_robot_losses = std::min(event->resources.defender_robots, pressure);
        if (event->resources.defender_robots <= facts.defender_robot_losses) {
            facts.crew_injuries = std::min(event->resources.available_crew,
                                           std::max<std::uint32_t>(1, surviving_attackers / 3));
        }
        facts.module_damage_events = std::max<std::uint32_t>(1, surviving_attackers / 4);
    }

    return facts;
}

core::Result<void, RaidError> RaidRuntime::apply_defense_boundary(
    DefenseEventId defense_id, const DefenseBoundaryFacts& facts,
    DefenseProjectionMode mode, std::uint64_t transaction_id) {
    static_cast<void>(mode);
    auto* event = find_record(defenses_, defense_id);
    if (event == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (event->phase == DefensePhase::Resolved || transaction_id == 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }
    if (std::ranges::find(event->applied_boundary_transactions, transaction_id) !=
        event->applied_boundary_transactions.end()) {
        return core::Result<void, RaidError>::success();
    }
    if (facts.attacker_losses > event->resources.attacker_units ||
        facts.defense_ammo_spent > event->resources.defense_ammo ||
        facts.shield_energy_spent < 0 ||
        facts.shield_energy_spent > event->resources.shield_energy ||
        facts.defender_robot_losses > event->resources.defender_robots ||
        facts.crew_injuries > event->resources.available_crew) {
        return core::Result<void, RaidError>::failure(RaidError::MissingCommittedEvidence);
    }

    event->resources.attacker_units -= facts.attacker_losses;
    event->resources.defense_ammo -= facts.defense_ammo_spent;
    event->resources.shield_energy -= facts.shield_energy_spent;
    event->resources.defender_robots -= facts.defender_robot_losses;
    event->resources.available_crew -= facts.crew_injuries;
    event->crew_injured += facts.crew_injuries;
    event->module_damage_events += facts.module_damage_events;

    if (event->phase == DefensePhase::ApproachDetection) {
        event->phase = DefensePhase::ExternalDefense;
    }
    if (facts.boarding_attempt_started &&
        enum_value(event->phase) < enum_value(DefensePhase::BoardingAttempt)) {
        event->phase = DefensePhase::BoardingAttempt;
        event->escalation = EscalationState::Mobilized;
    }
    if (facts.breach_committed) {
        event->phase = DefensePhase::InteriorDefense;
        event->escalation = EscalationState::ReinforcedFullResponse;
    }
    if (facts.attacker_objective_committed) {
        event->phase = DefensePhase::AttackerObjective;
    }
    if (facts.attackers_withdrawn || event->resources.attacker_units == 0) {
        event->phase = DefensePhase::Recovery;
        event->escalation = EscalationState::WithdrawalEvacuation;
    }

    event->applied_boundary_transactions.push_back(transaction_id);
    ++event->revision;
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::commit_theft(
    DefenseEventId defense_id, StrategicAssetId asset, StrategicOwnerId horizon_owner,
    StrategicOwnerId hostile_carrier_owner, std::uint64_t transaction_id,
    PhysicalTransferPort& ownership) {
    auto* event = find_record(defenses_, defense_id);
    if (event == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (!asset || !horizon_owner || !hostile_carrier_owner || transaction_id == 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }

    auto existing = std::ranges::find_if(event->thefts, [&](const TheftRecord& theft) {
        return theft.asset == asset;
    });
    if (existing != event->thefts.end()) {
        if (existing->theft_transaction == transaction_id) {
            return core::Result<void, RaidError>::success();
        }
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }

    const auto current_owner = ownership.owner(asset);
    if (!current_owner || *current_owner != horizon_owner) {
        return core::Result<void, RaidError>::failure(RaidError::OwnershipMismatch);
    }
    if (!ownership.transfer(asset, horizon_owner, hostile_carrier_owner, transaction_id)) {
        return core::Result<void, RaidError>::failure(RaidError::OwnershipMismatch);
    }

    event->thefts.push_back(TheftRecord{asset, horizon_owner, hostile_carrier_owner, {},
                                        TheftState::Carried, transaction_id, 0});
    ++event->revision;
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::commit_hostile_extraction(
    DefenseEventId defense_id, StrategicAssetId asset,
    StrategicOwnerId hostile_carrier_owner, StrategicOwnerId extracted_owner,
    std::uint64_t transaction_id, PhysicalTransferPort& ownership) {
    auto* event = find_record(defenses_, defense_id);
    if (event == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (!asset || !hostile_carrier_owner || !extracted_owner || transaction_id == 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }

    auto theft = std::ranges::find_if(event->thefts, [&](const TheftRecord& value) {
        return value.asset == asset;
    });
    if (theft == event->thefts.end()) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (theft->state == TheftState::Extracted) {
        if (theft->extraction_transaction == transaction_id) {
            return core::Result<void, RaidError>::success();
        }
        return core::Result<void, RaidError>::failure(RaidError::DuplicateTransaction);
    }
    if (theft->hostile_carrier_owner != hostile_carrier_owner) {
        return core::Result<void, RaidError>::failure(RaidError::OwnershipMismatch);
    }

    const auto current_owner = ownership.owner(asset);
    if (!current_owner || *current_owner != hostile_carrier_owner) {
        return core::Result<void, RaidError>::failure(RaidError::OwnershipMismatch);
    }
    if (!ownership.transfer(asset, hostile_carrier_owner, extracted_owner, transaction_id)) {
        return core::Result<void, RaidError>::failure(RaidError::OwnershipMismatch);
    }

    theft->extracted_owner = extracted_owner;
    theft->state = TheftState::Extracted;
    theft->extraction_transaction = transaction_id;
    ++event->revision;
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::resolve_defense(
    DefenseEventId defense_id, std::uint64_t transaction_id,
    std::uint64_t now_tick, std::uint64_t recovery_grace_ticks) {
    auto* event = find_record(defenses_, defense_id);
    if (event == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (transaction_id == 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }
    if (event->resolution_transaction != 0) {
        if (event->resolution_transaction == transaction_id) {
            return core::Result<void, RaidError>::success();
        }
        return core::Result<void, RaidError>::failure(RaidError::DuplicateTransaction);
    }
    if (event->phase != DefensePhase::Recovery) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }

    event->phase = DefensePhase::Resolved;
    event->resolution_transaction = transaction_id;
    ++event->revision;

    recovery_grace_ = RecoveryGraceState{defense_id, transaction_id, recovery_grace_ticks,
                                         now_tick, false, 1};

    if (event->source_dynamic_event) {
        auto* source = find_record(dynamic_events_, *event->source_dynamic_event);
        if (source != nullptr) {
            source->state = DynamicEventState::Resolved;
            if (std::ranges::find(source->applied_transactions, transaction_id) ==
                source->applied_transactions.end()) {
                source->applied_transactions.push_back(transaction_id);
            }
            ++source->revision;
            if (critical_horizon_event_ == source->id) {
                critical_horizon_event_.reset();
            }
        }
    }
    return core::Result<void, RaidError>::success();
}

core::Result<std::optional<DynamicEventId>, RaidError>
RaidRuntime::evaluate_dynamic_events(
    std::uint64_t now_tick, std::uint64_t save_seed,
    std::span<const DynamicEventCandidate> candidates) {
    advance_strategic_time(now_tick, false);
    if (now_tick < next_evaluation_tick_) {
        return core::Result<std::optional<DynamicEventId>, RaidError>::success(std::nullopt);
    }

    std::uint64_t new_deadline = 0;
    if (!checked_add(now_tick, kEvaluationIntervalTicks, new_deadline)) {
        return core::Result<std::optional<DynamicEventId>, RaidError>::failure(
            RaidError::InvalidTime);
    }

    const auto offered_count = static_cast<std::size_t>(std::ranges::count_if(
        dynamic_events_, [](const auto& entry) {
            return entry.second.state == DynamicEventState::Offered &&
                   !entry.second.critical_horizon;
        }));

    std::vector<DynamicEventCandidate> eligible;
    eligible.reserve(candidates.size());
    for (const auto& candidate : candidates) {
        if (candidate.source_key == 0 || candidate.location_key == 0 ||
            candidate.weight == 0) {
            continue;
        }
        if (candidate.critical_horizon) {
            if (critical_horizon_event_ ||
                (recovery_grace_ && recovery_grace_->remaining_ticks > 0)) {
                continue;
            }
        } else if (offered_count >= 3) {
            continue;
        }
        eligible.push_back(candidate);
    }

    std::ranges::sort(eligible, [](const DynamicEventCandidate& left,
                                   const DynamicEventCandidate& right) {
        return std::tuple{enum_value(left.family), left.source_key, left.location_key,
                          left.critical_horizon, left.weight} <
               std::tuple{enum_value(right.family), right.source_key, right.location_key,
                          right.critical_horizon, right.weight};
    });

    if (eligible.empty()) {
        ++generation_ordinal_;
        next_evaluation_tick_ = new_deadline;
        return core::Result<std::optional<DynamicEventId>, RaidError>::success(std::nullopt);
    }

    std::uint64_t total_weight = 0;
    for (const auto& candidate : eligible) {
        total_weight += candidate.weight;
        if (total_weight > std::numeric_limits<std::uint32_t>::max()) {
            return core::Result<std::optional<DynamicEventId>, RaidError>::failure(
                RaidError::DeterministicGenerationFailed);
        }
    }

    const auto rng_result = core::make_scoped_rng(
        core::ScopedRngDescriptor{save_seed, core::StreamKind::DynamicEventGeneration,
                                  generation_ordinal_ + 1, 1});
    if (!rng_result) {
        return core::Result<std::optional<DynamicEventId>, RaidError>::failure(
            RaidError::DeterministicGenerationFailed);
    }
    auto rng = rng_result.value();
    const auto draw_result = rng.uniform_bounded_u32(static_cast<std::uint32_t>(total_weight));
    if (!draw_result) {
        return core::Result<std::optional<DynamicEventId>, RaidError>::failure(
            RaidError::DeterministicGenerationFailed);
    }

    std::uint64_t draw = draw_result.value();
    std::size_t selected_index = 0;
    for (; selected_index < eligible.size(); ++selected_index) {
        if (draw < eligible[selected_index].weight) {
            break;
        }
        draw -= eligible[selected_index].weight;
    }
    if (selected_index >= eligible.size()) {
        return core::Result<std::optional<DynamicEventId>, RaidError>::failure(
            RaidError::DeterministicGenerationFailed);
    }

    const auto& selected = eligible[selected_index];
    std::uint64_t expiry_tick = 0;
    if (selected.offer_duration_ticks != 0 &&
        !checked_add(now_tick, selected.offer_duration_ticks, expiry_tick)) {
        return core::Result<std::optional<DynamicEventId>, RaidError>::failure(
            RaidError::InvalidTime);
    }

    const DynamicEventId event_id{next_dynamic_event_id_++};
    DynamicEventRecord event{};
    event.id = event_id;
    event.family = selected.family;
    event.state = selected.critical_horizon ? DynamicEventState::Scheduled
                                            : DynamicEventState::Offered;
    event.stable_seed = rng.next_u64();
    event.source_key = selected.source_key;
    event.location_key = selected.location_key;
    event.start_tick = now_tick;
    event.expiry_tick = expiry_tick;
    event.critical_horizon = selected.critical_horizon;
    dynamic_events_.emplace(event.id, event);

    if (selected.critical_horizon) {
        critical_horizon_event_ = event.id;
    }
    ++generation_ordinal_;
    next_evaluation_tick_ = new_deadline;
    return core::Result<std::optional<DynamicEventId>, RaidError>::success(event.id);
}

void RaidRuntime::expire_dynamic_events(std::uint64_t now_tick) {
    for (auto& [id, event] : dynamic_events_) {
        static_cast<void>(id);
        if (event.state == DynamicEventState::Offered && event.expiry_tick != 0 &&
            now_tick >= event.expiry_tick) {
            event.state = DynamicEventState::Expired;
            ++event.revision;
        }
    }
}

core::Result<void, RaidError> RaidRuntime::link_defense_event(
    DynamicEventId dynamic_event_id, DefenseEventId defense_event_id) {
    auto* event = find_record(dynamic_events_, dynamic_event_id);
    auto* defense_record = find_record(defenses_, defense_event_id);
    if (event == nullptr || defense_record == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (!event->critical_horizon ||
        (event->defense_event_id && *event->defense_event_id != defense_event_id) ||
        (defense_record->source_dynamic_event &&
         *defense_record->source_dynamic_event != dynamic_event_id)) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }
    event->defense_event_id = defense_event_id;
    defense_record->source_dynamic_event = dynamic_event_id;
    event->state = DynamicEventState::Active;
    ++event->revision;
    ++defense_record->revision;
    return core::Result<void, RaidError>::success();
}

StrategicMessageId RaidRuntime::queue_message(
    std::uint64_t source_key, std::uint64_t source_tick,
    std::uint64_t delivery_tick, bool degraded_delivery_allowed) {
    const StrategicMessageId id{next_message_id_++};
    messages_.emplace(id, StrategicMessageRecord{id, source_key, source_tick, delivery_tick,
                                                 degraded_delivery_allowed,
                                                 StrategicMessageState::Queued, 0, 1});
    return id;
}

void RaidRuntime::advance_messages(std::uint64_t now_tick,
                                   CommunicationState link) {
    for (auto& [id, record] : messages_) {
        static_cast<void>(id);
        if (record.state != StrategicMessageState::Queued ||
            now_tick < record.delivery_tick) {
            continue;
        }
        const bool can_deliver =
            link == CommunicationState::Available ||
            (link == CommunicationState::Degraded && record.degraded_delivery_allowed);
        if (can_deliver) {
            record.state = StrategicMessageState::Delivered;
            record.delivered_tick = now_tick;
            ++record.revision;
        }
    }
}

core::Result<void, RaidError> RaidRuntime::queue_remote_order(
    RemoteOrderRecord order) {
    if (!order.id || order.target_key == 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }
    if (remote_orders_.contains(order.id)) {
        return core::Result<void, RaidError>::failure(RaidError::DuplicateRecord);
    }
    order.state = RemoteOrderState::Queued;
    order.delivery_transaction = 0;
    order.revision = std::max<std::uint64_t>(1, order.revision);
    remote_orders_.emplace(order.id, order);
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::deliver_remote_order(
    RemoteOrderId order_id, CommunicationState link,
    std::uint64_t current_target_revision, std::uint64_t transaction_id) {
    auto* order = find_record(remote_orders_, order_id);
    if (order == nullptr) {
        return core::Result<void, RaidError>::failure(RaidError::NotFound);
    }
    if (transaction_id == 0) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }
    if (order->delivery_transaction == transaction_id &&
        (order->state == RemoteOrderState::Accepted ||
         order->state == RemoteOrderState::Rejected)) {
        return core::Result<void, RaidError>::success();
    }
    if (order->state != RemoteOrderState::Queued) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }
    if (link != CommunicationState::Available) {
        return core::Result<void, RaidError>::failure(
            RaidError::CommunicationUnavailable);
    }

    order->state = RemoteOrderState::Delivered;
    if (current_target_revision != order->expected_target_revision) {
        order->state = RemoteOrderState::Rejected;
        order->delivery_transaction = transaction_id;
        ++order->revision;
        return core::Result<void, RaidError>::failure(RaidError::StaleRevision);
    }

    order->state = RemoteOrderState::Accepted;
    order->delivery_transaction = transaction_id;
    ++order->revision;
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::start_recovery_transit(
    const RecoveryTransitRequest& request) {
    if (recovery_transit_ && recovery_transit_->status != RecoveryTransitStatus::Arrived) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }
    if (!request.source_mission_instance || request.failure_transaction == 0 ||
        !request.origin || !request.destination) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }

    const auto minimum_duration =
        std::max(request.minimum_service_ticks, request.shortest_route_ticks);
    std::uint64_t minimum_arrival = 0;
    if (!checked_add(request.start_tick, minimum_duration, minimum_arrival) ||
        request.planned_arrival_tick < minimum_arrival) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidTime);
    }

    recovery_transit_ = RecoveryTransitState{
        request.source_mission_instance,
        request.failure_transaction,
        request.origin,
        request.destination,
        request.start_tick,
        minimum_arrival,
        request.planned_arrival_tick,
        request.move_primary_ship,
        request.included_assets,
        RecoveryTransitStatus::InTransit,
        1,
    };
    return core::Result<void, RaidError>::success();
}

core::Result<void, RaidError> RaidRuntime::reroute_recovery(
    StrategicLocationId destination, std::uint64_t now_tick,
    std::uint64_t minimum_service_ticks, std::uint64_t shortest_route_ticks) {
    if (!recovery_transit_ || recovery_transit_->status == RecoveryTransitStatus::Arrived) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidState);
    }
    if (!destination) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidId);
    }

    const auto additional_duration =
        std::max(minimum_service_ticks, shortest_route_ticks);
    std::uint64_t rerouted_arrival = 0;
    if (!checked_add(now_tick, additional_duration, rerouted_arrival)) {
        return core::Result<void, RaidError>::failure(RaidError::InvalidTime);
    }

    recovery_transit_->destination = destination;
    recovery_transit_->planned_arrival_tick =
        std::max(recovery_transit_->planned_arrival_tick, rerouted_arrival);
    recovery_transit_->minimum_arrival_tick =
        std::max(recovery_transit_->minimum_arrival_tick, rerouted_arrival);
    recovery_transit_->status = RecoveryTransitStatus::InTransit;
    ++recovery_transit_->revision;
    return core::Result<void, RaidError>::success();
}

void RaidRuntime::advance_recovery(std::uint64_t now_tick,
                                   bool destination_safe) {
    if (!recovery_transit_ ||
        recovery_transit_->status == RecoveryTransitStatus::Arrived ||
        now_tick < recovery_transit_->planned_arrival_tick) {
        return;
    }

    if (!destination_safe) {
        if (recovery_transit_->status != RecoveryTransitStatus::AwaitingReroute) {
            recovery_transit_->status = RecoveryTransitStatus::AwaitingReroute;
            ++recovery_transit_->revision;
        }
        return;
    }

    recovery_transit_->status = RecoveryTransitStatus::Arrived;
    ++recovery_transit_->revision;
}

void RaidRuntime::advance_strategic_time(std::uint64_t now_tick,
                                         bool critical_recovery) {
    if (!recovery_grace_) {
        return;
    }
    auto& grace = *recovery_grace_;
    if (now_tick <= grace.last_update_tick) {
        grace.paused_for_critical_recovery = critical_recovery;
        return;
    }

    const auto elapsed = now_tick - grace.last_update_tick;
    grace.last_update_tick = now_tick;
    grace.paused_for_critical_recovery = critical_recovery;
    if (!critical_recovery) {
        grace.remaining_ticks =
            elapsed >= grace.remaining_ticks ? 0 : grace.remaining_ticks - elapsed;
    }
    ++grace.revision;
    if (grace.remaining_ticks == 0) {
        recovery_grace_.reset();
    }
}

const RaidRecord* RaidRuntime::raid(RaidId id) const noexcept {
    return find_record(raids_, id);
}

const ReinforcementSourceRecord* RaidRuntime::reinforcement_source(
    ForceSourceId id) const noexcept {
    return find_record(reinforcement_sources_, id);
}

const ReinforcementResponseRecord* RaidRuntime::reinforcement(
    ReinforcementResponseId id) const noexcept {
    return find_record(reinforcements_, id);
}

const DefenseEventRecord* RaidRuntime::defense(DefenseEventId id) const noexcept {
    return find_record(defenses_, id);
}

const DynamicEventRecord* RaidRuntime::dynamic_event(
    DynamicEventId id) const noexcept {
    return find_record(dynamic_events_, id);
}

const StrategicMessageRecord* RaidRuntime::message(
    StrategicMessageId id) const noexcept {
    return find_record(messages_, id);
}

const RemoteOrderRecord* RaidRuntime::remote_order(RemoteOrderId id) const noexcept {
    return find_record(remote_orders_, id);
}

bool RaidRuntime::is_stable_for_snapshot() const noexcept {
    if (critical_horizon_event_) {
        const auto* event = find_record(dynamic_events_, *critical_horizon_event_);
        if (event == nullptr || !event->critical_horizon ||
            (event->state != DynamicEventState::Scheduled &&
             event->state != DynamicEventState::Active)) {
            return false;
        }
    }
    if (recovery_grace_ && !defenses_.contains(recovery_grace_->source_defense)) {
        return false;
    }
    for (const auto& [id, event] : dynamic_events_) {
        static_cast<void>(id);
        if (event.defense_event_id && !defenses_.contains(*event.defense_event_id)) {
            return false;
        }
    }
    for (const auto& [id, response] : reinforcements_) {
        static_cast<void>(id);
        if (!reinforcement_sources_.contains(response.source) ||
            !operation_exists(raids_, defenses_, response.operation)) {
            return false;
        }
    }
    return true;
}

std::string RaidRuntime::serialize() const {
    std::ostringstream out;
    out << "SFS11 " << generation_ordinal_ << ' ' << next_evaluation_tick_ << ' '
        << next_dynamic_event_id_ << ' ' << next_message_id_ << ' '
        << (critical_horizon_event_ ? critical_horizon_event_->raw() : 0) << ' '
        << raids_.size() << ' ' << reinforcement_sources_.size() << ' '
        << reinforcements_.size() << ' ' << defenses_.size() << ' '
        << dynamic_events_.size() << ' ' << messages_.size() << ' '
        << remote_orders_.size() << ' ' << (recovery_grace_.has_value() ? 1 : 0)
        << ' ' << (recovery_transit_.has_value() ? 1 : 0) << '\n';

    for (const auto& [id, record] : raids_) {
        static_cast<void>(id);
        out << "RA " << record.id.raw() << ' ' << record.mission_id.raw() << ' '
            << (record.mission_instance_id ? record.mission_instance_id->raw() : 0)
            << ' ' << record.target_key << ' ' << enum_value(record.phase) << ' '
            << enum_value(record.escalation) << ' ' << record.revision << ' '
            << record.committed_consequence_transactions.size();
        for (const auto transaction : record.committed_consequence_transactions) {
            out << ' ' << transaction;
        }
        out << '\n';
    }

    for (const auto& [id, source] : reinforcement_sources_) {
        static_cast<void>(id);
        out << "FS " << source.id.raw() << ' ' << source.available_units << ' '
            << source.revision << '\n';
    }

    for (const auto& [id, response] : reinforcements_) {
        static_cast<void>(id);
        out << "RR " << response.id.raw() << ' '
            << enum_value(response.operation.kind) << ' ' << response.operation.id << ' '
            << response.source.raw() << ' ' << response.units << ' '
            << enum_value(response.state) << ' ' << response.call_start_tick << ' '
            << response.departure_tick << ' ' << response.arrival_tick << ' '
            << response.commit_transaction << ' ' << response.route_valid << ' '
            << response.revision << '\n';
    }

    for (const auto& [id, event] : defenses_) {
        static_cast<void>(id);
        out << "DE " << event.id.raw() << ' '
            << (event.source_dynamic_event ? event.source_dynamic_event->raw() : 0)
            << ' ' << enum_value(event.phase) << ' ' << enum_value(event.escalation)
            << ' ' << event.resources.attacker_units << ' '
            << event.resources.powered_defense_mounts << ' '
            << event.resources.defense_ammo << ' ' << event.resources.shield_energy
            << ' ' << event.resources.defender_robots << ' '
            << event.resources.available_crew << ' ' << event.crew_injured << ' '
            << event.module_damage_events << ' ' << event.resolution_transaction << ' '
            << event.revision << ' ' << event.applied_boundary_transactions.size();
        for (const auto transaction : event.applied_boundary_transactions) {
            out << ' ' << transaction;
        }
        out << ' ' << event.thefts.size();
        for (const auto& theft : event.thefts) {
            out << ' ' << theft.asset.raw() << ' ' << theft.horizon_owner.raw() << ' '
                << theft.hostile_carrier_owner.raw() << ' '
                << theft.extracted_owner.raw() << ' ' << enum_value(theft.state) << ' '
                << theft.theft_transaction << ' ' << theft.extraction_transaction;
        }
        out << '\n';
    }

    for (const auto& [id, event] : dynamic_events_) {
        static_cast<void>(id);
        out << "DY " << event.id.raw() << ' ' << enum_value(event.family) << ' '
            << enum_value(event.state) << ' ' << event.stable_seed << ' '
            << event.source_key << ' ' << event.location_key << ' '
            << event.start_tick << ' ' << event.expiry_tick << ' '
            << event.critical_horizon << ' ' << event.delivered_to_player << ' '
            << (event.defense_event_id ? event.defense_event_id->raw() : 0) << ' '
            << event.revision << ' ' << event.applied_transactions.size();
        for (const auto transaction : event.applied_transactions) {
            out << ' ' << transaction;
        }
        out << '\n';
    }

    for (const auto& [id, message] : messages_) {
        static_cast<void>(id);
        out << "MS " << message.id.raw() << ' ' << message.source_key << ' '
            << message.source_tick << ' ' << message.delivery_tick << ' '
            << message.degraded_delivery_allowed << ' ' << enum_value(message.state)
            << ' ' << message.delivered_tick << ' ' << message.revision << '\n';
    }

    for (const auto& [id, order] : remote_orders_) {
        static_cast<void>(id);
        out << "RO " << order.id.raw() << ' ' << order.target_key << ' '
            << order.expected_target_revision << ' ' << order.issued_tick << ' '
            << enum_value(order.state) << ' ' << order.delivery_transaction << ' '
            << order.revision << '\n';
    }

    if (recovery_grace_) {
        const auto& grace = *recovery_grace_;
        out << "GR " << grace.source_defense.raw() << ' '
            << grace.source_resolution_transaction << ' ' << grace.remaining_ticks
            << ' ' << grace.last_update_tick << ' '
            << grace.paused_for_critical_recovery << ' ' << grace.revision << '\n';
    }

    if (recovery_transit_) {
        const auto& transit = *recovery_transit_;
        out << "RT " << transit.source_mission_instance.raw() << ' '
            << transit.failure_transaction << ' ' << transit.origin.raw() << ' '
            << transit.destination.raw() << ' ' << transit.start_tick << ' '
            << transit.minimum_arrival_tick << ' ' << transit.planned_arrival_tick
            << ' ' << transit.move_primary_ship << ' ' << enum_value(transit.status)
            << ' ' << transit.revision << ' ' << transit.included_assets.size();
        for (const auto asset : transit.included_assets) {
            out << ' ' << asset.raw();
        }
        out << '\n';
    }

    return out.str();
}

core::Result<RaidRuntime, RaidError> RaidRuntime::deserialize(
    std::string_view data) {
    std::istringstream in{std::string{data}};
    RaidRuntime runtime{};
    std::string magic;
    std::uint64_t critical_id = 0;
    std::size_t raid_count = 0;
    std::size_t source_count = 0;
    std::size_t response_count = 0;
    std::size_t defense_count = 0;
    std::size_t dynamic_count = 0;
    std::size_t message_count = 0;
    std::size_t order_count = 0;
    unsigned has_grace = 0;
    unsigned has_recovery = 0;

    if (!(in >> magic >> runtime.generation_ordinal_ >> runtime.next_evaluation_tick_
          >> runtime.next_dynamic_event_id_ >> runtime.next_message_id_ >> critical_id
          >> raid_count >> source_count >> response_count >> defense_count
          >> dynamic_count >> message_count >> order_count >> has_grace
          >> has_recovery) ||
        magic != "SFS11" || runtime.next_dynamic_event_id_ == 0 ||
        runtime.next_message_id_ == 0 || has_grace > 1 || has_recovery > 1) {
        return core::Result<RaidRuntime, RaidError>::failure(
            RaidError::InvalidSerializedState);
    }

    for (std::size_t index = 0; index < raid_count; ++index) {
        std::string tag;
        RaidRecord record{};
        std::uint64_t mission_instance = 0;
        unsigned phase = 0;
        unsigned escalation = 0;
        std::size_t transaction_count = 0;
        std::uint64_t mission_id = 0;
        if (!(in >> tag >> mission_instance)) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        if (tag != "RA") {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        const std::uint64_t raid_raw = mission_instance;
        if (!(in >> mission_id >> mission_instance >> record.target_key >> phase
              >> escalation >> record.revision >> transaction_count) ||
            raid_raw == 0 || mission_id == 0 || record.target_key == 0 ||
            !valid_raid_phase(phase) || !valid_escalation(escalation)) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        record.id = RaidId{raid_raw};
        record.mission_id = missions::MissionId{mission_id};
        if (mission_instance != 0) {
            record.mission_instance_id = missions::MissionInstanceId{mission_instance};
        }
        record.phase = static_cast<RaidPhase>(phase);
        record.escalation = static_cast<EscalationState>(escalation);
        for (std::size_t transaction_index = 0;
             transaction_index < transaction_count; ++transaction_index) {
            std::uint64_t transaction = 0;
            if (!(in >> transaction) || transaction == 0) {
                return core::Result<RaidRuntime, RaidError>::failure(
                    RaidError::InvalidSerializedState);
            }
            record.committed_consequence_transactions.push_back(transaction);
        }
        if (!runtime.raids_.emplace(record.id, std::move(record)).second) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
    }

    for (std::size_t index = 0; index < source_count; ++index) {
        std::string tag;
        std::uint64_t raw_id = 0;
        ReinforcementSourceRecord source{};
        if (!(in >> tag >> raw_id >> source.available_units >> source.revision) ||
            tag != "FS" || raw_id == 0) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        source.id = ForceSourceId{raw_id};
        if (!runtime.reinforcement_sources_.emplace(source.id, source).second) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
    }

    for (std::size_t index = 0; index < response_count; ++index) {
        std::string tag;
        std::uint64_t raw_id = 0;
        unsigned operation_kind = 0;
        unsigned state = 0;
        ReinforcementResponseRecord response{};
        std::uint64_t source_id = 0;
        if (!(in >> tag >> raw_id >> operation_kind >> response.operation.id
              >> source_id >> response.units >> state >> response.call_start_tick
              >> response.departure_tick >> response.arrival_tick
              >> response.commit_transaction >> response.route_valid
              >> response.revision) ||
            tag != "RR" || raw_id == 0 || source_id == 0 || response.units == 0 ||
            operation_kind > enum_value(OperationRef::Kind::Defense) ||
            !valid_response_state(state) ||
            response.departure_tick < response.call_start_tick ||
            response.arrival_tick < response.departure_tick) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        response.id = ReinforcementResponseId{raw_id};
        response.operation.kind = static_cast<OperationRef::Kind>(operation_kind);
        response.source = ForceSourceId{source_id};
        response.state = static_cast<ReinforcementCallState>(state);
        if (!runtime.reinforcements_.emplace(response.id, response).second) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
    }

    for (std::size_t index = 0; index < defense_count; ++index) {
        std::string tag;
        std::uint64_t raw_id = 0;
        std::uint64_t source_dynamic = 0;
        unsigned phase = 0;
        unsigned escalation = 0;
        DefenseEventRecord event{};
        std::size_t boundary_count = 0;
        if (!(in >> tag >> raw_id >> source_dynamic >> phase >> escalation
              >> event.resources.attacker_units
              >> event.resources.powered_defense_mounts
              >> event.resources.defense_ammo >> event.resources.shield_energy
              >> event.resources.defender_robots >> event.resources.available_crew
              >> event.crew_injured >> event.module_damage_events
              >> event.resolution_transaction >> event.revision >> boundary_count) ||
            tag != "DE" || raw_id == 0 || event.resources.shield_energy < 0 ||
            !valid_defense_phase(phase) || !valid_escalation(escalation)) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        event.id = DefenseEventId{raw_id};
        if (source_dynamic != 0) {
            event.source_dynamic_event = DynamicEventId{source_dynamic};
        }
        event.phase = static_cast<DefensePhase>(phase);
        event.escalation = static_cast<EscalationState>(escalation);
        for (std::size_t boundary_index = 0;
             boundary_index < boundary_count; ++boundary_index) {
            std::uint64_t transaction = 0;
            if (!(in >> transaction) || transaction == 0) {
                return core::Result<RaidRuntime, RaidError>::failure(
                    RaidError::InvalidSerializedState);
            }
            event.applied_boundary_transactions.push_back(transaction);
        }
        std::size_t theft_count = 0;
        if (!(in >> theft_count)) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        for (std::size_t theft_index = 0; theft_index < theft_count; ++theft_index) {
            std::uint64_t asset = 0;
            std::uint64_t horizon = 0;
            std::uint64_t carrier = 0;
            std::uint64_t extracted = 0;
            unsigned state = 0;
            TheftRecord theft{};
            if (!(in >> asset >> horizon >> carrier >> extracted >> state
                  >> theft.theft_transaction >> theft.extraction_transaction) ||
                asset == 0 || horizon == 0 || carrier == 0 ||
                !valid_theft_state(state) || theft.theft_transaction == 0) {
                return core::Result<RaidRuntime, RaidError>::failure(
                    RaidError::InvalidSerializedState);
            }
            theft.asset = StrategicAssetId{asset};
            theft.horizon_owner = StrategicOwnerId{horizon};
            theft.hostile_carrier_owner = StrategicOwnerId{carrier};
            theft.extracted_owner = StrategicOwnerId{extracted};
            theft.state = static_cast<TheftState>(state);
            if (theft.state == TheftState::Extracted &&
                (extracted == 0 || theft.extraction_transaction == 0)) {
                return core::Result<RaidRuntime, RaidError>::failure(
                    RaidError::InvalidSerializedState);
            }
            event.thefts.push_back(theft);
        }
        if (!runtime.defenses_.emplace(event.id, std::move(event)).second) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
    }

    for (std::size_t index = 0; index < dynamic_count; ++index) {
        std::string tag;
        std::uint64_t raw_id = 0;
        unsigned family = 0;
        unsigned state = 0;
        unsigned critical = 0;
        unsigned delivered = 0;
        std::uint64_t defense_id = 0;
        DynamicEventRecord event{};
        std::size_t transaction_count = 0;
        if (!(in >> tag >> raw_id >> family >> state >> event.stable_seed
              >> event.source_key >> event.location_key >> event.start_tick
              >> event.expiry_tick >> critical >> delivered >> defense_id
              >> event.revision >> transaction_count) ||
            tag != "DY" || raw_id == 0 || event.source_key == 0 ||
            event.location_key == 0 || !valid_dynamic_family(family) ||
            !valid_dynamic_state(state) || critical > 1 || delivered > 1) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        event.id = DynamicEventId{raw_id};
        event.family = static_cast<DynamicEventFamily>(family);
        event.state = static_cast<DynamicEventState>(state);
        event.critical_horizon = critical != 0;
        event.delivered_to_player = delivered != 0;
        if (defense_id != 0) {
            event.defense_event_id = DefenseEventId{defense_id};
        }
        for (std::size_t transaction_index = 0;
             transaction_index < transaction_count; ++transaction_index) {
            std::uint64_t transaction = 0;
            if (!(in >> transaction) || transaction == 0) {
                return core::Result<RaidRuntime, RaidError>::failure(
                    RaidError::InvalidSerializedState);
            }
            event.applied_transactions.push_back(transaction);
        }
        if (!runtime.dynamic_events_.emplace(event.id, std::move(event)).second) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
    }

    for (std::size_t index = 0; index < message_count; ++index) {
        std::string tag;
        std::uint64_t raw_id = 0;
        unsigned degraded = 0;
        unsigned state = 0;
        StrategicMessageRecord message{};
        if (!(in >> tag >> raw_id >> message.source_key >> message.source_tick
              >> message.delivery_tick >> degraded >> state
              >> message.delivered_tick >> message.revision) ||
            tag != "MS" || raw_id == 0 || degraded > 1 ||
            !valid_message_state(state)) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        message.id = StrategicMessageId{raw_id};
        message.degraded_delivery_allowed = degraded != 0;
        message.state = static_cast<StrategicMessageState>(state);
        if (!runtime.messages_.emplace(message.id, message).second) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
    }

    for (std::size_t index = 0; index < order_count; ++index) {
        std::string tag;
        std::uint64_t raw_id = 0;
        unsigned state = 0;
        RemoteOrderRecord order{};
        if (!(in >> tag >> raw_id >> order.target_key
              >> order.expected_target_revision >> order.issued_tick >> state
              >> order.delivery_transaction >> order.revision) ||
            tag != "RO" || raw_id == 0 || order.target_key == 0 ||
            !valid_remote_order_state(state)) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        order.id = RemoteOrderId{raw_id};
        order.state = static_cast<RemoteOrderState>(state);
        if (!runtime.remote_orders_.emplace(order.id, order).second) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
    }

    if (has_grace != 0) {
        std::string tag;
        std::uint64_t defense_id = 0;
        unsigned paused = 0;
        RecoveryGraceState grace{};
        if (!(in >> tag >> defense_id >> grace.source_resolution_transaction
              >> grace.remaining_ticks >> grace.last_update_tick >> paused
              >> grace.revision) ||
            tag != "GR" || defense_id == 0 ||
            grace.source_resolution_transaction == 0 || paused > 1) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        grace.source_defense = DefenseEventId{defense_id};
        grace.paused_for_critical_recovery = paused != 0;
        runtime.recovery_grace_ = grace;
    }

    if (has_recovery != 0) {
        std::string tag;
        std::uint64_t mission_instance = 0;
        std::uint64_t origin = 0;
        std::uint64_t destination = 0;
        unsigned move_ship = 0;
        unsigned status = 0;
        RecoveryTransitState transit{};
        std::size_t asset_count = 0;
        if (!(in >> tag >> mission_instance >> transit.failure_transaction >> origin
              >> destination >> transit.start_tick >> transit.minimum_arrival_tick
              >> transit.planned_arrival_tick >> move_ship >> status
              >> transit.revision >> asset_count) ||
            tag != "RT" || mission_instance == 0 || transit.failure_transaction == 0 ||
            origin == 0 || destination == 0 || move_ship > 1 ||
            !valid_recovery_status(status) ||
            transit.planned_arrival_tick < transit.minimum_arrival_tick) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
        transit.source_mission_instance = missions::MissionInstanceId{mission_instance};
        transit.origin = StrategicLocationId{origin};
        transit.destination = StrategicLocationId{destination};
        transit.move_primary_ship = move_ship != 0;
        transit.status = static_cast<RecoveryTransitStatus>(status);
        for (std::size_t asset_index = 0; asset_index < asset_count; ++asset_index) {
            std::uint64_t asset = 0;
            if (!(in >> asset) || asset == 0) {
                return core::Result<RaidRuntime, RaidError>::failure(
                    RaidError::InvalidSerializedState);
            }
            transit.included_assets.push_back(StrategicAssetId{asset});
        }
        runtime.recovery_transit_ = std::move(transit);
    }

    if (critical_id != 0) {
        runtime.critical_horizon_event_ = DynamicEventId{critical_id};
    }

    if (!runtime.is_stable_for_snapshot()) {
        return core::Result<RaidRuntime, RaidError>::failure(
            RaidError::InvalidSerializedState);
    }
    for (const auto& [id, defense] : runtime.defenses_) {
        static_cast<void>(id);
        if (defense.source_dynamic_event &&
            !runtime.dynamic_events_.contains(*defense.source_dynamic_event)) {
            return core::Result<RaidRuntime, RaidError>::failure(
                RaidError::InvalidSerializedState);
        }
    }

    std::string trailing;
    if (in >> trailing) {
        return core::Result<RaidRuntime, RaidError>::failure(
            RaidError::InvalidSerializedState);
    }
    return core::Result<RaidRuntime, RaidError>::success(std::move(runtime));
}

}  // namespace starforge::raids
