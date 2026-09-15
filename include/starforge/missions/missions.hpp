#pragma once

#include "starforge/core/result.hpp"
#include "starforge/core/rng.hpp"
#include "starforge/core/strong_id.hpp"

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace starforge::missions {

struct MissionTag;
struct MissionInstanceTag;
using MissionId = core::PersistentId<MissionTag>;
using MissionInstanceId = core::PersistentId<MissionInstanceTag>;
using ObjectiveId = core::StrongId<struct ObjectiveTag, std::uint32_t>;

enum class MissionState : std::uint8_t {
    Available,
    Accepted,
    Prepared,
    Deployed,
    Active,
    ExtractionAvailable,
    Extracting,
    Succeeded,
    Failed,
    Abandoned,
    Resolved,
};

enum class ObjectiveState : std::uint8_t { Locked, Available, Active, Completed, Failed, Cancelled };
enum class MissionError : std::uint8_t {
    InvalidId,
    DuplicateMission,
    MissionNotFound,
    InstanceNotFound,
    InvalidState,
    ExternalMissionAlreadyActive,
    InvalidObjectiveGraph,
    ObjectiveNotFound,
    ObjectiveNotReady,
    ObjectiveTerminal,
    DuplicateTransaction,
    ExtractionUnavailable,
    GenerationFailed,
    InvalidSnapshot,
    InvalidSerializedState,
};

struct ObjectiveDefinition final {
    ObjectiveId id{};
    std::vector<ObjectiveId> prerequisites{};
    bool mandatory{true};
    bool extraction{false};
};

struct ObjectiveRecord final {
    ObjectiveId id{};
    ObjectiveState state{ObjectiveState::Locked};
    std::uint64_t terminal_transaction{0};
};

struct GeneratedMissionPackage final {
    std::uint64_t stable_seed{0};
    std::uint32_t generation_version{1};
    std::uint32_t layout_variant{0};
    std::uint32_t hazard_profile{0};
    std::uint32_t reward_units{0};

    friend bool operator==(const GeneratedMissionPackage&, const GeneratedMissionPackage&) = default;
};

struct MissionRecord final {
    MissionId id{};
    MissionState state{MissionState::Available};
    GeneratedMissionPackage generated{};
    std::vector<ObjectiveDefinition> objectives{};
    std::uint32_t attempt_count{0};
    bool retryable{false};
    std::uint64_t revision{1};
};

struct MissionInstanceRecord final {
    MissionInstanceId id{};
    MissionId mission_id{};
    std::uint32_t attempt_ordinal{0};
    std::vector<ObjectiveRecord> objectives{};
    std::uint64_t deployment_tick{0};
    std::uint64_t hazard_tick{0};
    bool extraction_secured{false};
    bool rewards_applied{false};
    std::vector<std::uint64_t> applied_transactions{};
    std::uint64_t revision{1};
};

struct GenerationCursor final {
    std::uint64_t family_key{0};
    std::uint64_t ordinal{0};
    std::uint64_t revision{1};
};

struct GenerationRequest final {
    std::uint64_t save_seed{0};
    std::uint64_t family_key{0};
    std::uint64_t expected_cursor_revision{0};
    std::uint32_t generation_version{1};
    std::uint32_t max_attempts{8};
};

class MissionRuntime final {
public:
    [[nodiscard]] core::Result<void, MissionError> add_mission(MissionRecord mission);
    [[nodiscard]] core::Result<MissionInstanceId, MissionError> deploy(MissionId mission_id,
                                                                      std::uint64_t tick);
    [[nodiscard]] core::Result<void, MissionError> activate(MissionInstanceId instance_id);
    [[nodiscard]] core::Result<void, MissionError> commit_objective(MissionInstanceId instance_id,
                                                                    ObjectiveId objective_id,
                                                                    std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, MissionError> secure_extraction(MissionInstanceId instance_id);
    [[nodiscard]] core::Result<void, MissionError> extract(MissionInstanceId instance_id,
                                                           std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, MissionError> fail(MissionInstanceId instance_id);
    [[nodiscard]] core::Result<void, MissionError> prepare_retry(MissionId mission_id);
    [[nodiscard]] core::Result<void, MissionError> advance_hazards(MissionInstanceId instance_id,
                                                                   std::uint64_t tick);

    [[nodiscard]] const MissionRecord* mission(MissionId id) const noexcept;
    [[nodiscard]] const MissionInstanceRecord* instance(MissionInstanceId id) const noexcept;
    [[nodiscard]] std::optional<MissionInstanceId> active_external_instance() const noexcept;
    [[nodiscard]] std::string serialize() const;
    [[nodiscard]] static core::Result<MissionRuntime, MissionError> deserialize(std::string_view data);

private:
    std::vector<MissionRecord> missions_{};
    std::vector<MissionInstanceRecord> instances_{};
    std::optional<MissionInstanceId> active_external_{};
    std::uint64_t next_instance_id_{1};
};

[[nodiscard]] core::Result<GeneratedMissionPackage, MissionError>
generate_offer(const GenerationRequest& request, GenerationCursor& cursor);

}  // namespace starforge::missions
