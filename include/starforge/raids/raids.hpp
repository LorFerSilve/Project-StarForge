#pragma once

#include "starforge/core/result.hpp"
#include "starforge/core/strong_id.hpp"
#include "starforge/missions/missions.hpp"

#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace starforge::raids {

struct RaidTag;
struct DefenseEventTag;
struct DynamicEventTag;
struct ReinforcementResponseTag;
struct StrategicMessageTag;
struct RemoteOrderTag;
struct ForceSourceTag;
struct StrategicLocationTag;
struct StrategicAssetTag;
struct StrategicOwnerTag;

using RaidId = core::PersistentId<RaidTag>;
using DefenseEventId = core::PersistentId<DefenseEventTag>;
using DynamicEventId = core::PersistentId<DynamicEventTag>;
using ReinforcementResponseId = core::PersistentId<ReinforcementResponseTag>;
using StrategicMessageId = core::PersistentId<StrategicMessageTag>;
using RemoteOrderId = core::PersistentId<RemoteOrderTag>;
using ForceSourceId = core::PersistentId<ForceSourceTag>;
using StrategicLocationId = core::PersistentId<StrategicLocationTag>;
using StrategicAssetId = core::PersistentId<StrategicAssetTag>;
using StrategicOwnerId = core::PersistentId<StrategicOwnerTag>;

enum class RaidPhase : std::uint8_t {
    Intelligence,
    Preparation,
    Approach,
    ExternalOperation,
    AccessOrBreach,
    InteriorOperation,
    ObjectiveLootSabotage,
    EscalationResponse,
    Withdrawal,
    Aftermath,
};

enum class EscalationState : std::uint8_t {
    Quiet,
    Suspicious,
    Alerted,
    Mobilized,
    ReinforcementsEnRoute,
    ReinforcedFullResponse,
    WithdrawalEvacuation,
};

enum class ReinforcementCallState : std::uint8_t {
    NotRequested,
    Calling,
    Committed,
    Disrupted,
    Responding,
    Arrived,
    CancelledByValidCause,
};

enum class DefensePhase : std::uint8_t {
    ApproachDetection,
    ExternalDefense,
    BoardingAttempt,
    InteriorDefense,
    AttackerObjective,
    AttackerWithdrawal,
    Recovery,
    Resolved,
};

enum class DefenseProjectionMode : std::uint8_t {
    Active,
    Offscreen,
};

enum class DynamicEventFamily : std::uint8_t {
    Distress,
    Salvage,
    ResourceDiscovery,
    TraderConvoy,
    Market,
    FactionConflict,
    Route,
    AnomalyResearch,
    StationEmergency,
    HorizonDefense,
    NpcRaid,
    RecoveryFollowUp,
};

enum class DynamicEventState : std::uint8_t {
    Offered,
    Scheduled,
    Active,
    Resolved,
    Expired,
    Cancelled,
};

enum class CommunicationState : std::uint8_t {
    Available,
    Degraded,
    Unavailable,
};

enum class StrategicMessageState : std::uint8_t {
    Queued,
    Delivered,
    Invalidated,
};

enum class RemoteOrderState : std::uint8_t {
    Queued,
    Delivered,
    Accepted,
    Rejected,
};

enum class RecoveryTransitStatus : std::uint8_t {
    InTransit,
    AwaitingReroute,
    Arrived,
};

enum class TheftState : std::uint8_t {
    Carried,
    Extracted,
};

enum class RaidError : std::uint8_t {
    InvalidId,
    DuplicateRecord,
    NotFound,
    InvalidState,
    InvalidTransition,
    MissingCommittedEvidence,
    DuplicateTransaction,
    InsufficientFiniteSource,
    CommunicationUnavailable,
    RouteUnavailable,
    InsertionUnavailable,
    OwnershipMismatch,
    OptionalOfferCapacityReached,
    CriticalEventSlotOccupied,
    RecoveryGraceActive,
    StaleRevision,
    InvalidTime,
    InvalidSerializedState,
    DeterministicGenerationFailed,
};

struct RaidTransitionEvidence final {
    bool access_committed{false};
    bool objective_or_sabotage_committed{false};
    bool withdrawal_route_reached{false};
    bool aftermath_ready{false};
};

struct RaidRecord final {
    RaidId id{};
    missions::MissionId mission_id{};
    std::optional<missions::MissionInstanceId> mission_instance_id{};
    std::uint64_t target_key{0};
    RaidPhase phase{RaidPhase::Intelligence};
    EscalationState escalation{EscalationState::Quiet};
    std::vector<std::uint64_t> committed_consequence_transactions{};
    std::uint64_t revision{1};
};

struct ReinforcementSourceRecord final {
    ForceSourceId id{};
    std::uint32_t available_units{0};
    std::uint64_t revision{1};
};

struct OperationRef final {
    enum class Kind : std::uint8_t { Raid, Defense };
    Kind kind{Kind::Raid};
    std::uint64_t id{0};

    [[nodiscard]] static OperationRef raid(RaidId value) noexcept {
        return OperationRef{Kind::Raid, value.raw()};
    }
    [[nodiscard]] static OperationRef defense(DefenseEventId value) noexcept {
        return OperationRef{Kind::Defense, value.raw()};
    }

    friend bool operator==(const OperationRef&, const OperationRef&) = default;
};

struct ReinforcementResponseRecord final {
    ReinforcementResponseId id{};
    OperationRef operation{};
    ForceSourceId source{};
    std::uint32_t units{0};
    ReinforcementCallState state{ReinforcementCallState::NotRequested};
    std::uint64_t call_start_tick{0};
    std::uint64_t departure_tick{0};
    std::uint64_t arrival_tick{0};
    std::uint64_t commit_transaction{0};
    bool route_valid{false};
    std::uint64_t revision{1};
};

struct DefenseResourceState final {
    std::uint32_t attacker_units{0};
    std::uint32_t powered_defense_mounts{0};
    std::uint32_t defense_ammo{0};
    std::int64_t shield_energy{0};
    std::uint32_t defender_robots{0};
    std::uint32_t available_crew{0};

    friend bool operator==(const DefenseResourceState&, const DefenseResourceState&) = default;
};

struct DefenseBoundaryFacts final {
    std::uint32_t attacker_losses{0};
    std::uint32_t defense_ammo_spent{0};
    std::int64_t shield_energy_spent{0};
    std::uint32_t defender_robot_losses{0};
    std::uint32_t crew_injuries{0};
    std::uint32_t module_damage_events{0};
    bool boarding_attempt_started{false};
    bool breach_committed{false};
    bool attacker_objective_committed{false};
    bool attackers_withdrawn{false};

    friend bool operator==(const DefenseBoundaryFacts&, const DefenseBoundaryFacts&) = default;
};

struct TheftRecord final {
    StrategicAssetId asset{};
    StrategicOwnerId horizon_owner{};
    StrategicOwnerId hostile_carrier_owner{};
    StrategicOwnerId extracted_owner{};
    TheftState state{TheftState::Carried};
    std::uint64_t theft_transaction{0};
    std::uint64_t extraction_transaction{0};
};

struct DefenseEventRecord final {
    DefenseEventId id{};
    std::optional<DynamicEventId> source_dynamic_event{};
    DefensePhase phase{DefensePhase::ApproachDetection};
    EscalationState escalation{EscalationState::Quiet};
    DefenseResourceState resources{};
    std::uint32_t crew_injured{0};
    std::uint32_t module_damage_events{0};
    std::vector<TheftRecord> thefts{};
    std::vector<std::uint64_t> applied_boundary_transactions{};
    std::uint64_t resolution_transaction{0};
    std::uint64_t revision{1};
};

class PhysicalTransferPort {
public:
    virtual ~PhysicalTransferPort() = default;
    [[nodiscard]] virtual bool transfer(StrategicAssetId asset,
                                        StrategicOwnerId expected_owner,
                                        StrategicOwnerId destination,
                                        std::uint64_t transaction_id) = 0;
    [[nodiscard]] virtual std::optional<StrategicOwnerId> owner(StrategicAssetId asset) const = 0;
};

struct DynamicEventCandidate final {
    DynamicEventFamily family{DynamicEventFamily::Distress};
    std::uint64_t source_key{0};
    std::uint64_t location_key{0};
    std::uint32_t weight{1};
    bool critical_horizon{false};
    std::uint64_t offer_duration_ticks{0};
};

struct DynamicEventRecord final {
    DynamicEventId id{};
    DynamicEventFamily family{DynamicEventFamily::Distress};
    DynamicEventState state{DynamicEventState::Offered};
    std::uint64_t stable_seed{0};
    std::uint64_t source_key{0};
    std::uint64_t location_key{0};
    std::uint64_t start_tick{0};
    std::uint64_t expiry_tick{0};
    bool critical_horizon{false};
    bool delivered_to_player{false};
    std::optional<DefenseEventId> defense_event_id{};
    std::vector<std::uint64_t> applied_transactions{};
    std::uint64_t revision{1};
};

struct RecoveryGraceState final {
    DefenseEventId source_defense{};
    std::uint64_t source_resolution_transaction{0};
    std::uint64_t remaining_ticks{0};
    std::uint64_t last_update_tick{0};
    bool paused_for_critical_recovery{false};
    std::uint64_t revision{1};
};

struct StrategicMessageRecord final {
    StrategicMessageId id{};
    std::uint64_t source_key{0};
    std::uint64_t source_tick{0};
    std::uint64_t delivery_tick{0};
    bool degraded_delivery_allowed{false};
    StrategicMessageState state{StrategicMessageState::Queued};
    std::uint64_t delivered_tick{0};
    std::uint64_t revision{1};
};

struct RemoteOrderRecord final {
    RemoteOrderId id{};
    std::uint64_t target_key{0};
    std::uint64_t expected_target_revision{0};
    std::uint64_t issued_tick{0};
    RemoteOrderState state{RemoteOrderState::Queued};
    std::uint64_t delivery_transaction{0};
    std::uint64_t revision{1};
};

struct RecoveryTransitRequest final {
    missions::MissionInstanceId source_mission_instance{};
    std::uint64_t failure_transaction{0};
    StrategicLocationId origin{};
    StrategicLocationId destination{};
    std::uint64_t start_tick{0};
    std::uint64_t minimum_service_ticks{0};
    std::uint64_t shortest_route_ticks{0};
    std::uint64_t planned_arrival_tick{0};
    bool move_primary_ship{false};
    std::vector<StrategicAssetId> included_assets{};
};

struct RecoveryTransitState final {
    missions::MissionInstanceId source_mission_instance{};
    std::uint64_t failure_transaction{0};
    StrategicLocationId origin{};
    StrategicLocationId destination{};
    std::uint64_t start_tick{0};
    std::uint64_t minimum_arrival_tick{0};
    std::uint64_t planned_arrival_tick{0};
    bool move_primary_ship{false};
    std::vector<StrategicAssetId> included_assets{};
    RecoveryTransitStatus status{RecoveryTransitStatus::InTransit};
    std::uint64_t revision{1};
};

class RaidRuntime final {
public:
    [[nodiscard]] core::Result<void, RaidError> add_raid(RaidRecord raid);
    [[nodiscard]] core::Result<void, RaidError> bind_raid_instance(
        RaidId raid_id, missions::MissionInstanceId instance_id);
    [[nodiscard]] core::Result<void, RaidError> transition_raid(
        RaidId raid_id, RaidPhase next, const RaidTransitionEvidence& evidence);
    [[nodiscard]] core::Result<void, RaidError> record_raid_consequence(
        RaidId raid_id, std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, RaidError> set_raid_escalation(
        RaidId raid_id, EscalationState escalation);

    [[nodiscard]] core::Result<void, RaidError> add_reinforcement_source(
        ReinforcementSourceRecord source);
    [[nodiscard]] core::Result<void, RaidError> begin_reinforcement_call(
        ReinforcementResponseRecord response);
    [[nodiscard]] core::Result<void, RaidError> interrupt_reinforcement_call(
        ReinforcementResponseId response_id);
    [[nodiscard]] core::Result<void, RaidError> commit_reinforcement_call(
        ReinforcementResponseId response_id, std::uint64_t transaction_id,
        bool communication_path_available, bool route_valid);
    void advance_reinforcements(std::uint64_t now_tick, bool insertion_available);

    [[nodiscard]] core::Result<void, RaidError> add_defense_event(
        DefenseEventRecord event);
    [[nodiscard]] DefenseBoundaryFacts derive_offscreen_boundary(
        DefenseEventId defense_id) const noexcept;
    [[nodiscard]] core::Result<void, RaidError> apply_defense_boundary(
        DefenseEventId defense_id, const DefenseBoundaryFacts& facts,
        DefenseProjectionMode mode, std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, RaidError> commit_theft(
        DefenseEventId defense_id, StrategicAssetId asset, StrategicOwnerId horizon_owner,
        StrategicOwnerId hostile_carrier_owner, std::uint64_t transaction_id,
        PhysicalTransferPort& ownership);
    [[nodiscard]] core::Result<void, RaidError> commit_hostile_extraction(
        DefenseEventId defense_id, StrategicAssetId asset,
        StrategicOwnerId hostile_carrier_owner, StrategicOwnerId extracted_owner,
        std::uint64_t transaction_id, PhysicalTransferPort& ownership);
    [[nodiscard]] core::Result<void, RaidError> resolve_defense(
        DefenseEventId defense_id, std::uint64_t transaction_id,
        std::uint64_t now_tick, std::uint64_t recovery_grace_ticks);

    [[nodiscard]] core::Result<std::optional<DynamicEventId>, RaidError>
    evaluate_dynamic_events(std::uint64_t now_tick, std::uint64_t save_seed,
                            std::span<const DynamicEventCandidate> candidates);
    void expire_dynamic_events(std::uint64_t now_tick);
    [[nodiscard]] core::Result<void, RaidError> link_defense_event(
        DynamicEventId dynamic_event_id, DefenseEventId defense_event_id);

    [[nodiscard]] StrategicMessageId queue_message(std::uint64_t source_key,
                                                   std::uint64_t source_tick,
                                                   std::uint64_t delivery_tick,
                                                   bool degraded_delivery_allowed);
    void advance_messages(std::uint64_t now_tick, CommunicationState link);
    [[nodiscard]] core::Result<void, RaidError> queue_remote_order(
        RemoteOrderRecord order);
    [[nodiscard]] core::Result<void, RaidError> deliver_remote_order(
        RemoteOrderId order_id, CommunicationState link,
        std::uint64_t current_target_revision, std::uint64_t transaction_id);

    [[nodiscard]] core::Result<void, RaidError> start_recovery_transit(
        const RecoveryTransitRequest& request);
    [[nodiscard]] core::Result<void, RaidError> reroute_recovery(
        StrategicLocationId destination, std::uint64_t now_tick,
        std::uint64_t minimum_service_ticks, std::uint64_t shortest_route_ticks);
    void advance_recovery(std::uint64_t now_tick, bool destination_safe);
    void advance_strategic_time(std::uint64_t now_tick, bool critical_recovery);

    [[nodiscard]] const RaidRecord* raid(RaidId id) const noexcept;
    [[nodiscard]] const ReinforcementSourceRecord* reinforcement_source(
        ForceSourceId id) const noexcept;
    [[nodiscard]] const ReinforcementResponseRecord* reinforcement(
        ReinforcementResponseId id) const noexcept;
    [[nodiscard]] const DefenseEventRecord* defense(DefenseEventId id) const noexcept;
    [[nodiscard]] const DynamicEventRecord* dynamic_event(DynamicEventId id) const noexcept;
    [[nodiscard]] const StrategicMessageRecord* message(StrategicMessageId id) const noexcept;
    [[nodiscard]] const RemoteOrderRecord* remote_order(RemoteOrderId id) const noexcept;
    [[nodiscard]] const std::optional<RecoveryGraceState>& recovery_grace() const noexcept {
        return recovery_grace_;
    }
    [[nodiscard]] const std::optional<RecoveryTransitState>& recovery_transit() const noexcept {
        return recovery_transit_;
    }
    [[nodiscard]] std::optional<DynamicEventId> critical_horizon_event() const noexcept {
        return critical_horizon_event_;
    }
    [[nodiscard]] std::uint64_t generation_ordinal() const noexcept {
        return generation_ordinal_;
    }
    [[nodiscard]] std::uint64_t next_evaluation_tick() const noexcept {
        return next_evaluation_tick_;
    }
    [[nodiscard]] bool is_stable_for_snapshot() const noexcept;

    [[nodiscard]] std::string serialize() const;
    [[nodiscard]] static core::Result<RaidRuntime, RaidError> deserialize(
        std::string_view data);

private:
    std::map<RaidId, RaidRecord> raids_{};
    std::map<ForceSourceId, ReinforcementSourceRecord> reinforcement_sources_{};
    std::map<ReinforcementResponseId, ReinforcementResponseRecord> reinforcements_{};
    std::map<DefenseEventId, DefenseEventRecord> defenses_{};
    std::map<DynamicEventId, DynamicEventRecord> dynamic_events_{};
    std::map<StrategicMessageId, StrategicMessageRecord> messages_{};
    std::map<RemoteOrderId, RemoteOrderRecord> remote_orders_{};
    std::optional<RecoveryGraceState> recovery_grace_{};
    std::optional<RecoveryTransitState> recovery_transit_{};
    std::optional<DynamicEventId> critical_horizon_event_{};
    std::uint64_t generation_ordinal_{0};
    std::uint64_t next_evaluation_tick_{0};
    std::uint64_t next_dynamic_event_id_{1};
    std::uint64_t next_message_id_{1};
};

}  // namespace starforge::raids
