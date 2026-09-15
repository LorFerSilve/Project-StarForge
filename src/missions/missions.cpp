#include "starforge/missions/missions.hpp"

#include <algorithm>
#include <charconv>
#include <sstream>
#include <string_view>

namespace starforge::missions {
namespace {

[[nodiscard]] bool terminal(ObjectiveState state) noexcept {
    return state == ObjectiveState::Completed || state == ObjectiveState::Failed ||
           state == ObjectiveState::Cancelled;
}

[[nodiscard]] bool valid_graph(const std::vector<ObjectiveDefinition>& definitions) {
    for (const auto& definition : definitions) {
        if (!definition.id) {
            return false;
        }
        if (std::ranges::count_if(definitions, [&](const auto& item) { return item.id == definition.id; }) != 1) {
            return false;
        }
        for (const auto prerequisite : definition.prerequisites) {
            if (prerequisite == definition.id ||
                std::ranges::none_of(definitions, [&](const auto& item) { return item.id == prerequisite; })) {
                return false;
            }
        }
    }

    for (const auto& root : definitions) {
        std::vector<ObjectiveId> open{root.id};
        std::vector<ObjectiveId> visited{};
        while (!open.empty()) {
            const auto current = open.back();
            open.pop_back();
            if (std::ranges::find(visited, current) != visited.end()) {
                return false;
            }
            visited.push_back(current);
            const auto it = std::ranges::find_if(definitions, [&](const auto& item) { return item.id == current; });
            if (it != definitions.end()) {
                open.insert(open.end(), it->prerequisites.begin(), it->prerequisites.end());
            }
        }
    }
    return true;
}

[[nodiscard]] bool prerequisites_complete(const MissionInstanceRecord& instance,
                                          const ObjectiveDefinition& definition) {
    return std::ranges::all_of(definition.prerequisites, [&](const auto prerequisite) {
        const auto it = std::ranges::find_if(instance.objectives,
                                             [&](const auto& record) { return record.id == prerequisite; });
        return it != instance.objectives.end() && it->state == ObjectiveState::Completed;
    });
}

void unlock_ready(MissionInstanceRecord& instance, const MissionRecord& mission) {
    for (const auto& definition : mission.objectives) {
        auto record = std::ranges::find_if(instance.objectives,
                                           [&](const auto& item) { return item.id == definition.id; });
        if (record != instance.objectives.end() && record->state == ObjectiveState::Locked &&
            prerequisites_complete(instance, definition)) {
            record->state = ObjectiveState::Available;
        }
    }
}

}  // namespace

core::Result<void, MissionError> MissionRuntime::add_mission(MissionRecord mission) {
    if (!mission.id) {
        return core::Result<void, MissionError>::failure(MissionError::InvalidId);
    }
    if (!valid_graph(mission.objectives)) {
        return core::Result<void, MissionError>::failure(MissionError::InvalidObjectiveGraph);
    }
    if (this->mission(mission.id) != nullptr) {
        return core::Result<void, MissionError>::failure(MissionError::DuplicateMission);
    }
    std::ranges::sort(mission.objectives, {}, &ObjectiveDefinition::id);
    missions_.push_back(std::move(mission));
    std::ranges::sort(missions_, {}, &MissionRecord::id);
    return core::Result<void, MissionError>::success();
}

core::Result<MissionInstanceId, MissionError> MissionRuntime::deploy(MissionId mission_id,
                                                                     std::uint64_t tick) {
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == mission_id; });
    if (mission_it == missions_.end()) {
        return core::Result<MissionInstanceId, MissionError>::failure(MissionError::MissionNotFound);
    }
    if (active_external_) {
        return core::Result<MissionInstanceId, MissionError>::failure(MissionError::ExternalMissionAlreadyActive);
    }
    if (mission_it->state != MissionState::Available && mission_it->state != MissionState::Prepared) {
        return core::Result<MissionInstanceId, MissionError>::failure(MissionError::InvalidState);
    }

    MissionInstanceRecord instance{};
    instance.id = MissionInstanceId{next_instance_id_++};
    instance.mission_id = mission_id;
    instance.attempt_ordinal = ++mission_it->attempt_count;
    instance.deployment_tick = tick;
    for (const auto& definition : mission_it->objectives) {
        instance.objectives.push_back(
            ObjectiveRecord{definition.id, definition.prerequisites.empty() ? ObjectiveState::Available
                                                                           : ObjectiveState::Locked,
                            0});
    }
    mission_it->state = MissionState::Deployed;
    ++mission_it->revision;
    active_external_ = instance.id;
    instances_.push_back(instance);
    return core::Result<MissionInstanceId, MissionError>::success(instance.id);
}

core::Result<void, MissionError> MissionRuntime::activate(MissionInstanceId instance_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) {
        return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    }
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    if (mission_it == missions_.end() || mission_it->state != MissionState::Deployed) {
        return core::Result<void, MissionError>::failure(MissionError::InvalidState);
    }
    mission_it->state = MissionState::Active;
    ++mission_it->revision;
    for (auto& objective : instance_it->objectives) {
        if (objective.state == ObjectiveState::Available) {
            objective.state = ObjectiveState::Active;
        }
    }
    ++instance_it->revision;
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::commit_objective(MissionInstanceId instance_id,
                                                                   ObjectiveId objective_id,
                                                                   std::uint64_t transaction_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) {
        return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    }
    if (transaction_id == 0) {
        return core::Result<void, MissionError>::failure(MissionError::InvalidId);
    }
    if (std::ranges::find(instance_it->applied_transactions, transaction_id) !=
        instance_it->applied_transactions.end()) {
        return core::Result<void, MissionError>::success();
    }
    auto objective = std::ranges::find_if(instance_it->objectives,
                                          [&](const auto& item) { return item.id == objective_id; });
    if (objective == instance_it->objectives.end()) {
        return core::Result<void, MissionError>::failure(MissionError::ObjectiveNotFound);
    }
    if (terminal(objective->state)) {
        return core::Result<void, MissionError>::failure(MissionError::ObjectiveTerminal);
    }
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    const auto definition = std::ranges::find_if(mission_it->objectives,
                                                 [&](const auto& item) { return item.id == objective_id; });
    if (objective->state == ObjectiveState::Locked || !prerequisites_complete(*instance_it, *definition)) {
        return core::Result<void, MissionError>::failure(MissionError::ObjectiveNotReady);
    }
    objective->state = ObjectiveState::Completed;
    objective->terminal_transaction = transaction_id;
    instance_it->applied_transactions.push_back(transaction_id);
    unlock_ready(*instance_it, *mission_it);
    ++instance_it->revision;

    const bool mandatory_complete = std::ranges::all_of(mission_it->objectives, [&](const auto& item) {
        if (!item.mandatory || item.extraction) {
            return true;
        }
        const auto record = std::ranges::find_if(instance_it->objectives,
                                                 [&](const auto& value) { return value.id == item.id; });
        return record != instance_it->objectives.end() && record->state == ObjectiveState::Completed;
    });
    if (mandatory_complete && mission_it->state == MissionState::Active) {
        mission_it->state = MissionState::ExtractionAvailable;
        ++mission_it->revision;
    }
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::secure_extraction(MissionInstanceId instance_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) {
        return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    }
    const auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    if (mission_it == missions_.end() || mission_it->state != MissionState::ExtractionAvailable) {
        return core::Result<void, MissionError>::failure(MissionError::ExtractionUnavailable);
    }
    instance_it->extraction_secured = true;
    ++instance_it->revision;
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::extract(MissionInstanceId instance_id,
                                                         std::uint64_t transaction_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end() || !instance_it->extraction_secured) {
        return core::Result<void, MissionError>::failure(MissionError::ExtractionUnavailable);
    }
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    if (std::ranges::find(instance_it->applied_transactions, transaction_id) != instance_it->applied_transactions.end()) {
        return core::Result<void, MissionError>::success();
    }
    instance_it->applied_transactions.push_back(transaction_id);
    instance_it->rewards_applied = true;
    ++instance_it->revision;
    mission_it->state = MissionState::Succeeded;
    ++mission_it->revision;
    active_external_.reset();
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::fail(MissionInstanceId instance_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) {
        return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    }
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    mission_it->state = MissionState::Failed;
    ++mission_it->revision;
    active_external_.reset();
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::prepare_retry(MissionId mission_id) {
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == mission_id; });
    if (mission_it == missions_.end()) {
        return core::Result<void, MissionError>::failure(MissionError::MissionNotFound);
    }
    if (mission_it->state != MissionState::Failed || !mission_it->retryable) {
        return core::Result<void, MissionError>::failure(MissionError::InvalidState);
    }
    mission_it->state = MissionState::Prepared;
    ++mission_it->revision;
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::advance_hazards(MissionInstanceId instance_id,
                                                                  std::uint64_t tick) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) {
        return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    }
    instance_it->hazard_tick = std::max(instance_it->hazard_tick, tick);
    ++instance_it->revision;
    return core::Result<void, MissionError>::success();
}

const MissionRecord* MissionRuntime::mission(MissionId id) const noexcept {
    const auto it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == id; });
    return it == missions_.end() ? nullptr : &*it;
}

const MissionInstanceRecord* MissionRuntime::instance(MissionInstanceId id) const noexcept {
    const auto it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == id; });
    return it == instances_.end() ? nullptr : &*it;
}

std::optional<MissionInstanceId> MissionRuntime::active_external_instance() const noexcept {
    return active_external_;
}

std::string MissionRuntime::serialize() const {
    std::ostringstream out;
    out << "SFM1 " << next_instance_id_ << ' ' << (active_external_ ? active_external_->raw() : 0) << ' '
        << missions_.size() << ' ' << instances_.size() << '\n';
    for (const auto& mission_value : missions_) {
        out << "M " << mission_value.id.raw() << ' ' << static_cast<unsigned>(mission_value.state) << ' '
            << mission_value.generated.stable_seed << ' ' << mission_value.generated.generation_version << ' '
            << mission_value.generated.layout_variant << ' ' << mission_value.generated.hazard_profile << ' '
            << mission_value.generated.reward_units << ' ' << mission_value.attempt_count << ' '
            << mission_value.retryable << ' ' << mission_value.revision << ' ' << mission_value.objectives.size();
        for (const auto& objective : mission_value.objectives) {
            out << ' ' << objective.id.raw() << ' ' << objective.mandatory << ' ' << objective.extraction << ' '
                << objective.prerequisites.size();
            for (const auto prerequisite : objective.prerequisites) {
                out << ' ' << prerequisite.raw();
            }
        }
        out << '\n';
    }
    for (const auto& instance_value : instances_) {
        out << "I " << instance_value.id.raw() << ' ' << instance_value.mission_id.raw() << ' '
            << instance_value.attempt_ordinal << ' ' << instance_value.deployment_tick << ' '
            << instance_value.hazard_tick << ' ' << instance_value.extraction_secured << ' '
            << instance_value.rewards_applied << ' ' << instance_value.revision << ' '
            << instance_value.objectives.size();
        for (const auto& objective : instance_value.objectives) {
            out << ' ' << objective.id.raw() << ' ' << static_cast<unsigned>(objective.state) << ' '
                << objective.terminal_transaction;
        }
        out << ' ' << instance_value.applied_transactions.size();
        for (const auto transaction : instance_value.applied_transactions) {
            out << ' ' << transaction;
        }
        out << '\n';
    }
    return out.str();
}

core::Result<MissionRuntime, MissionError> MissionRuntime::deserialize(std::string_view data) {
    std::istringstream in{std::string{data}};
    std::string magic;
    MissionRuntime runtime{};
    std::uint64_t active = 0;
    std::size_t mission_count = 0;
    std::size_t instance_count = 0;
    if (!(in >> magic >> runtime.next_instance_id_ >> active >> mission_count >> instance_count) || magic != "SFM1") {
        return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
    }
    for (std::size_t index = 0; index < mission_count; ++index) {
        char marker = 0;
        MissionRecord value{};
        std::uint64_t id = 0;
        unsigned state = 0;
        std::size_t objective_count = 0;
        if (!(in >> marker >> id >> state >> value.generated.stable_seed >> value.generated.generation_version >>
              value.generated.layout_variant >> value.generated.hazard_profile >> value.generated.reward_units >>
              value.attempt_count >> value.retryable >> value.revision >> objective_count) || marker != 'M') {
            return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        }
        value.id = MissionId{id};
        value.state = static_cast<MissionState>(state);
        for (std::size_t objective_index = 0; objective_index < objective_count; ++objective_index) {
            ObjectiveDefinition definition{};
            std::uint32_t objective_id = 0;
            std::size_t prerequisite_count = 0;
            if (!(in >> objective_id >> definition.mandatory >> definition.extraction >> prerequisite_count)) {
                return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
            }
            definition.id = ObjectiveId{objective_id};
            for (std::size_t prerequisite_index = 0; prerequisite_index < prerequisite_count; ++prerequisite_index) {
                std::uint32_t prerequisite = 0;
                in >> prerequisite;
                definition.prerequisites.emplace_back(prerequisite);
            }
            value.objectives.push_back(std::move(definition));
        }
        runtime.missions_.push_back(std::move(value));
    }
    for (std::size_t index = 0; index < instance_count; ++index) {
        char marker = 0;
        MissionInstanceRecord value{};
        std::uint64_t id = 0;
        std::uint64_t mission_id = 0;
        std::size_t objective_count = 0;
        if (!(in >> marker >> id >> mission_id >> value.attempt_ordinal >> value.deployment_tick >> value.hazard_tick >>
              value.extraction_secured >> value.rewards_applied >> value.revision >> objective_count) || marker != 'I') {
            return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        }
        value.id = MissionInstanceId{id};
        value.mission_id = MissionId{mission_id};
        for (std::size_t objective_index = 0; objective_index < objective_count; ++objective_index) {
            std::uint32_t objective_id = 0;
            unsigned state = 0;
            ObjectiveRecord record{};
            in >> objective_id >> state >> record.terminal_transaction;
            record.id = ObjectiveId{objective_id};
            record.state = static_cast<ObjectiveState>(state);
            value.objectives.push_back(record);
        }
        std::size_t transaction_count = 0;
        in >> transaction_count;
        for (std::size_t transaction_index = 0; transaction_index < transaction_count; ++transaction_index) {
            std::uint64_t transaction = 0;
            in >> transaction;
            value.applied_transactions.push_back(transaction);
        }
        runtime.instances_.push_back(std::move(value));
    }
    if (!in) {
        return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
    }
    if (active != 0) {
        runtime.active_external_ = MissionInstanceId{active};
    }
    return core::Result<MissionRuntime, MissionError>::success(std::move(runtime));
}

core::Result<GeneratedMissionPackage, MissionError> generate_offer(const GenerationRequest& request,
                                                                    GenerationCursor& cursor) {
    if (request.save_seed == 0 || request.family_key == 0 || request.generation_version == 0 ||
        request.max_attempts == 0) {
        return core::Result<GeneratedMissionPackage, MissionError>::failure(MissionError::GenerationFailed);
    }
    if (cursor.family_key != request.family_key || cursor.revision != request.expected_cursor_revision) {
        return core::Result<GeneratedMissionPackage, MissionError>::failure(MissionError::InvalidSnapshot);
    }

    const auto ordinal = cursor.ordinal++;
    ++cursor.revision;
    const auto scope_key = core::splitmix64_mix(request.family_key ^ core::splitmix64_mix(ordinal));
    const auto rng_result = core::make_scoped_rng({request.save_seed, core::StreamKind::MissionGeneration,
                                                   scope_key, request.generation_version});
    if (!rng_result) {
        return core::Result<GeneratedMissionPackage, MissionError>::failure(MissionError::GenerationFailed);
    }
    auto rng = rng_result.value();
    GeneratedMissionPackage package{};
    package.stable_seed = rng.next_u64();
    package.generation_version = request.generation_version;
    package.layout_variant = rng.uniform_bounded_u32(8).value();
    package.hazard_profile = rng.uniform_bounded_u32(6).value();
    package.reward_units = 50U + rng.uniform_bounded_u32(151).value();
    return core::Result<GeneratedMissionPackage, MissionError>::success(package);
}

}  // namespace starforge::missions
