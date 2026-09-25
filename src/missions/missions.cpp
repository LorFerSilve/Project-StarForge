#include "starforge/missions/missions.hpp"

#include <algorithm>
#include <functional>
#include <limits>
#include <sstream>
#include <string_view>
#include <utility>

namespace starforge::missions {
namespace {

[[nodiscard]] bool terminal(ObjectiveState state) noexcept {
    return state == ObjectiveState::Completed || state == ObjectiveState::Failed ||
           state == ObjectiveState::Cancelled;
}

[[nodiscard]] bool valid_graph(const std::vector<ObjectiveDefinition>& definitions) {
    for (const auto& definition : definitions) {
        if (!definition.id ||
            std::ranges::count_if(definitions, [&](const auto& item) { return item.id == definition.id; }) != 1) {
            return false;
        }
        for (const auto prerequisite : definition.prerequisites) {
            if (prerequisite == definition.id ||
                std::ranges::none_of(definitions, [&](const auto& item) { return item.id == prerequisite; })) {
                return false;
            }
        }
    }

    enum class Visit : std::uint8_t { Unvisited, Visiting, Visited };
    std::vector<Visit> visits(definitions.size(), Visit::Unvisited);
    const auto index_of = [&](ObjectiveId id) {
        return static_cast<std::size_t>(std::ranges::find_if(definitions, [&](const auto& item) {
                                            return item.id == id;
                                        }) - definitions.begin());
    };
    std::function<bool(std::size_t)> acyclic = [&](std::size_t index) {
        if (visits[index] == Visit::Visiting) {
            return false;
        }
        if (visits[index] == Visit::Visited) {
            return true;
        }
        visits[index] = Visit::Visiting;
        for (const auto prerequisite : definitions[index].prerequisites) {
            if (!acyclic(index_of(prerequisite))) {
                return false;
            }
        }
        visits[index] = Visit::Visited;
        return true;
    };
    for (std::size_t index = 0; index < definitions.size(); ++index) {
        if (!acyclic(index)) {
            return false;
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

PreparedMissionDeployment::PreparedMissionDeployment(
    MissionRuntime& runtime,
    MissionId mission_id,
    MissionInstanceId instance_id,
    std::vector<MissionInstanceRecord> next_instances,
    core::StateRevision expected_revision) noexcept
    : runtime_(&runtime),
      mission_id_(mission_id),
      instance_id_(instance_id),
      next_instances_(std::move(next_instances)),
      expected_revision_(expected_revision) {}

transactions::DomainCommitKey PreparedMissionDeployment::commit_key() const noexcept {
    return {.domain = transactions::DomainCommitOrder::Missions, .stable_ordinal = 0U};
}

core::StateRevision PreparedMissionDeployment::expected_revision() const noexcept {
    return expected_revision_;
}

core::StateRevision PreparedMissionDeployment::current_revision() const noexcept {
    return runtime_->deployment_revision_;
}

void PreparedMissionDeployment::commit() noexcept {
    auto mission_it = std::ranges::find_if(
        runtime_->missions_,
        [&](const auto& item) { return item.id == mission_id_; });

    const auto prepared_instance = std::ranges::find_if(
        next_instances_,
        [&](const auto& item) { return item.id == instance_id_; });

    mission_it->attempt_count = prepared_instance->attempt_ordinal;
    mission_it->state = MissionState::Active;
    ++mission_it->revision;

    runtime_->active_external_ = instance_id_;
    runtime_->instances_.swap(next_instances_);
    ++runtime_->next_instance_id_;
    runtime_->touch_deployment_revision();
}

void PreparedMissionDeployment::publish_committed_events() noexcept {}

void MissionRuntime::touch_deployment_revision() noexcept {
    static_cast<void>(deployment_revision_.advance());
}

core::Result<void, MissionError> MissionRuntime::add_mission(MissionRecord mission) {
    if (!mission.id) return core::Result<void, MissionError>::failure(MissionError::InvalidId);
    if (!valid_graph(mission.objectives)) return core::Result<void, MissionError>::failure(MissionError::InvalidObjectiveGraph);
    if (this->mission(mission.id)) return core::Result<void, MissionError>::failure(MissionError::DuplicateMission);
    std::ranges::sort(mission.objectives, {}, &ObjectiveDefinition::id);
    missions_.push_back(std::move(mission));
    std::ranges::sort(missions_, {}, &MissionRecord::id);
    touch_deployment_revision();
    return core::Result<void, MissionError>::success();
}

core::Result<MissionInstanceId, MissionError> MissionRuntime::deploy(MissionId mission_id, std::uint64_t tick) {
    if (next_instance_id_ == std::numeric_limits<std::uint64_t>::max()) {
        return core::Result<MissionInstanceId, MissionError>::failure(
            MissionError::InstanceIdExhausted);
    }
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == mission_id; });
    if (mission_it == missions_.end()) return core::Result<MissionInstanceId, MissionError>::failure(MissionError::MissionNotFound);
    if (active_external_) return core::Result<MissionInstanceId, MissionError>::failure(MissionError::ExternalMissionAlreadyActive);
    if (mission_it->state != MissionState::Available && mission_it->state != MissionState::Prepared)
        return core::Result<MissionInstanceId, MissionError>::failure(MissionError::InvalidState);

    MissionInstanceRecord instance{};
    instance.id = MissionInstanceId{next_instance_id_++};
    instance.mission_id = mission_id;
    instance.attempt_ordinal = ++mission_it->attempt_count;
    instance.deployment_tick = tick;
    for (const auto& definition : mission_it->objectives) {
        instance.objectives.push_back({definition.id, definition.prerequisites.empty() ? ObjectiveState::Available
                                                                                     : ObjectiveState::Locked, 0});
    }
    mission_it->state = MissionState::Deployed;
    ++mission_it->revision;
    active_external_ = instance.id;
    instances_.push_back(instance);
    touch_deployment_revision();
    return core::Result<MissionInstanceId, MissionError>::success(instance.id);
}

core::Result<PreparedMissionDeployment, MissionError> MissionRuntime::prepare_deployment(
    MissionId mission_id, std::uint64_t tick, core::TransactionId transaction_id) {
    if (!transaction_id.valid()) {
        return core::Result<PreparedMissionDeployment, MissionError>::failure(MissionError::InvalidId);
    }
    if (!deployment_revision_.can_advance()) {
        return core::Result<PreparedMissionDeployment, MissionError>::failure(
            MissionError::RevisionExhausted);
    }
    if (next_instance_id_ == std::numeric_limits<std::uint64_t>::max()) {
        return core::Result<PreparedMissionDeployment, MissionError>::failure(
            MissionError::InstanceIdExhausted);
    }

    auto mission_it = std::ranges::find_if(
        missions_, [&](const auto& item) { return item.id == mission_id; });
    if (mission_it == missions_.end()) {
        return core::Result<PreparedMissionDeployment, MissionError>::failure(
            MissionError::MissionNotFound);
    }
    if (active_external_) {
        return core::Result<PreparedMissionDeployment, MissionError>::failure(
            MissionError::ExternalMissionAlreadyActive);
    }
    if (mission_it->state != MissionState::Available &&
        mission_it->state != MissionState::Prepared) {
        return core::Result<PreparedMissionDeployment, MissionError>::failure(
            MissionError::InvalidState);
    }
    if (mission_it->attempt_count == std::numeric_limits<std::uint32_t>::max()) {
        return core::Result<PreparedMissionDeployment, MissionError>::failure(
            MissionError::GenerationFailed);
    }

    MissionInstanceRecord instance{};
    instance.id = MissionInstanceId{next_instance_id_};
    instance.mission_id = mission_id;
    instance.attempt_ordinal = mission_it->attempt_count + 1U;
    instance.deployment_tick = tick;
    instance.applied_transactions.push_back(transaction_id.raw());
    instance.objectives.reserve(mission_it->objectives.size());
    for (const auto& definition : mission_it->objectives) {
        instance.objectives.push_back({
            definition.id,
            definition.prerequisites.empty() ? ObjectiveState::Active : ObjectiveState::Locked,
            0});
    }

    auto next_instances = instances_;
    next_instances.push_back(std::move(instance));
    return core::Result<PreparedMissionDeployment, MissionError>::success(
        PreparedMissionDeployment{
            *this,
            mission_id,
            MissionInstanceId{next_instance_id_},
            std::move(next_instances),
            deployment_revision_});
}

core::Result<void, MissionError> MissionRuntime::activate(MissionInstanceId instance_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    if (mission_it == missions_.end() || mission_it->state != MissionState::Deployed || active_external_ != instance_id)
        return core::Result<void, MissionError>::failure(MissionError::InvalidState);
    mission_it->state = MissionState::Active;
    ++mission_it->revision;
    for (auto& objective : instance_it->objectives) if (objective.state == ObjectiveState::Available) objective.state = ObjectiveState::Active;
    ++instance_it->revision;
    touch_deployment_revision();
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::commit_objective(MissionInstanceId instance_id, ObjectiveId objective_id,
                                                                  std::uint64_t transaction_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    if (mission_it == missions_.end() || active_external_ != instance_id ||
        (mission_it->state != MissionState::Active && mission_it->state != MissionState::ExtractionAvailable))
        return core::Result<void, MissionError>::failure(MissionError::InvalidState);
    if (transaction_id == 0) return core::Result<void, MissionError>::failure(MissionError::InvalidId);
    if (std::ranges::find(instance_it->applied_transactions, transaction_id) != instance_it->applied_transactions.end())
        return core::Result<void, MissionError>::success();
    auto objective = std::ranges::find_if(instance_it->objectives, [&](const auto& item) { return item.id == objective_id; });
    if (objective == instance_it->objectives.end()) return core::Result<void, MissionError>::failure(MissionError::ObjectiveNotFound);
    if (terminal(objective->state)) return core::Result<void, MissionError>::failure(MissionError::ObjectiveTerminal);
    const auto definition = std::ranges::find_if(mission_it->objectives, [&](const auto& item) { return item.id == objective_id; });
    if (definition == mission_it->objectives.end()) return core::Result<void, MissionError>::failure(MissionError::ObjectiveNotFound);
    if (objective->state == ObjectiveState::Locked || !prerequisites_complete(*instance_it, *definition))
        return core::Result<void, MissionError>::failure(MissionError::ObjectiveNotReady);
    objective->state = ObjectiveState::Completed;
    objective->terminal_transaction = transaction_id;
    instance_it->applied_transactions.push_back(transaction_id);
    unlock_ready(*instance_it, *mission_it);
    ++instance_it->revision;
    const bool complete = std::ranges::all_of(mission_it->objectives, [&](const auto& item) {
        if (!item.mandatory || item.extraction) return true;
        const auto record = std::ranges::find_if(instance_it->objectives, [&](const auto& value) { return value.id == item.id; });
        return record != instance_it->objectives.end() && record->state == ObjectiveState::Completed;
    });
    if (complete && mission_it->state == MissionState::Active) {
        mission_it->state = MissionState::ExtractionAvailable;
        ++mission_it->revision;
    }
    touch_deployment_revision();
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::secure_extraction(MissionInstanceId instance_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    const auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    if (mission_it == missions_.end() || mission_it->state != MissionState::ExtractionAvailable || active_external_ != instance_id)
        return core::Result<void, MissionError>::failure(MissionError::ExtractionUnavailable);
    instance_it->extraction_secured = true;
    ++instance_it->revision;
    touch_deployment_revision();
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::extract(MissionInstanceId instance_id, std::uint64_t transaction_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end() || !instance_it->extraction_secured || transaction_id == 0)
        return core::Result<void, MissionError>::failure(MissionError::ExtractionUnavailable);
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    if (mission_it == missions_.end() || mission_it->state != MissionState::ExtractionAvailable || active_external_ != instance_id)
        return core::Result<void, MissionError>::failure(MissionError::ExtractionUnavailable);
    if (std::ranges::find(instance_it->applied_transactions, transaction_id) != instance_it->applied_transactions.end())
        return core::Result<void, MissionError>::success();
    instance_it->applied_transactions.push_back(transaction_id);
    instance_it->rewards_applied = true;
    ++instance_it->revision;
    mission_it->state = MissionState::Succeeded;
    ++mission_it->revision;
    active_external_.reset();
    touch_deployment_revision();
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::fail(MissionInstanceId instance_id) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == instance_it->mission_id; });
    if (mission_it == missions_.end() || active_external_ != instance_id ||
        (mission_it->state != MissionState::Deployed && mission_it->state != MissionState::Active &&
         mission_it->state != MissionState::ExtractionAvailable))
        return core::Result<void, MissionError>::failure(MissionError::InvalidState);
    mission_it->state = MissionState::Failed;
    ++mission_it->revision;
    active_external_.reset();
    touch_deployment_revision();
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::prepare_retry(MissionId mission_id) {
    auto mission_it = std::ranges::find_if(missions_, [&](const auto& item) { return item.id == mission_id; });
    if (mission_it == missions_.end()) return core::Result<void, MissionError>::failure(MissionError::MissionNotFound);
    if (mission_it->state != MissionState::Failed || !mission_it->retryable)
        return core::Result<void, MissionError>::failure(MissionError::InvalidState);
    mission_it->state = MissionState::Prepared;
    ++mission_it->revision;
    touch_deployment_revision();
    return core::Result<void, MissionError>::success();
}

core::Result<void, MissionError> MissionRuntime::advance_hazards(MissionInstanceId instance_id, std::uint64_t tick) {
    auto instance_it = std::ranges::find_if(instances_, [&](const auto& item) { return item.id == instance_id; });
    if (instance_it == instances_.end()) return core::Result<void, MissionError>::failure(MissionError::InstanceNotFound);
    instance_it->hazard_tick = std::max(instance_it->hazard_tick, tick);
    ++instance_it->revision;
    touch_deployment_revision();
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
std::optional<MissionInstanceId> MissionRuntime::active_external_instance() const noexcept { return active_external_; }

std::string MissionRuntime::serialize() const {
    std::ostringstream out;
    out << "SFM1 " << next_instance_id_ << ' ' << (active_external_ ? active_external_->raw() : 0) << ' '
        << missions_.size() << ' ' << instances_.size() << '\n';
    for (const auto& value : missions_) {
        out << "M " << value.id.raw() << ' ' << static_cast<unsigned>(value.state) << ' ' << value.generated.stable_seed << ' '
            << value.generated.generation_version << ' ' << value.generated.layout_variant << ' ' << value.generated.hazard_profile << ' '
            << value.generated.reward_units << ' ' << value.attempt_count << ' ' << value.retryable << ' ' << value.revision << ' '
            << value.objectives.size();
        for (const auto& objective : value.objectives) {
            out << ' ' << objective.id.raw() << ' ' << objective.mandatory << ' ' << objective.extraction << ' ' << objective.prerequisites.size();
            for (const auto prerequisite : objective.prerequisites) out << ' ' << prerequisite.raw();
        }
        out << '\n';
    }
    for (const auto& value : instances_) {
        out << "I " << value.id.raw() << ' ' << value.mission_id.raw() << ' ' << value.attempt_ordinal << ' ' << value.deployment_tick << ' '
            << value.hazard_tick << ' ' << value.extraction_secured << ' ' << value.rewards_applied << ' ' << value.revision << ' '
            << value.objectives.size();
        for (const auto& objective : value.objectives) out << ' ' << objective.id.raw() << ' ' << static_cast<unsigned>(objective.state) << ' ' << objective.terminal_transaction;
        out << ' ' << value.applied_transactions.size();
        for (const auto transaction : value.applied_transactions) out << ' ' << transaction;
        out << '\n';
    }
    return out.str();
}

core::Result<MissionRuntime, MissionError> MissionRuntime::deserialize(std::string_view data) {
    std::istringstream in{std::string{data}};
    MissionRuntime runtime{};
    std::string magic;
    std::uint64_t active = 0;
    std::size_t mission_count = 0, instance_count = 0;
    if (!(in >> magic >> runtime.next_instance_id_ >> active >> mission_count >> instance_count) || magic != "SFM1" || runtime.next_instance_id_ == 0)
        return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
    for (std::size_t i = 0; i < mission_count; ++i) {
        char marker{}; MissionRecord value{}; std::uint64_t id{}; unsigned state{}; std::size_t objective_count{};
        if (!(in >> marker >> id >> state >> value.generated.stable_seed >> value.generated.generation_version >> value.generated.layout_variant >>
              value.generated.hazard_profile >> value.generated.reward_units >> value.attempt_count >> value.retryable >> value.revision >> objective_count) || marker != 'M')
            return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        value.id = MissionId{id}; value.state = static_cast<MissionState>(state);
        for (std::size_t j = 0; j < objective_count; ++j) {
            ObjectiveDefinition definition{}; std::uint32_t oid{}; std::size_t prerequisite_count{};
            if (!(in >> oid >> definition.mandatory >> definition.extraction >> prerequisite_count)) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
            definition.id = ObjectiveId{oid};
            for (std::size_t k = 0; k < prerequisite_count; ++k) { std::uint32_t p{}; if (!(in >> p)) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState); definition.prerequisites.emplace_back(p); }
            value.objectives.push_back(std::move(definition));
        }
        if (!value.id || !valid_graph(value.objectives) || runtime.mission(value.id)) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        runtime.missions_.push_back(std::move(value));
    }
    for (std::size_t i = 0; i < instance_count; ++i) {
        char marker{}; MissionInstanceRecord value{}; std::uint64_t id{}, mission_id{}; std::size_t objective_count{};
        if (!(in >> marker >> id >> mission_id >> value.attempt_ordinal >> value.deployment_tick >> value.hazard_tick >> value.extraction_secured >>
              value.rewards_applied >> value.revision >> objective_count) || marker != 'I') return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        value.id = MissionInstanceId{id}; value.mission_id = MissionId{mission_id};
        for (std::size_t j = 0; j < objective_count; ++j) { std::uint32_t oid{}; unsigned state{}; ObjectiveRecord record{}; if (!(in >> oid >> state >> record.terminal_transaction)) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState); record.id = ObjectiveId{oid}; record.state = static_cast<ObjectiveState>(state); value.objectives.push_back(record); }
        std::size_t transaction_count{}; if (!(in >> transaction_count)) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        for (std::size_t j = 0; j < transaction_count; ++j) { std::uint64_t transaction{}; if (!(in >> transaction)) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState); value.applied_transactions.push_back(transaction); }
        if (!value.id || !runtime.mission(value.mission_id) || runtime.instance(value.id)) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        const auto* definition = runtime.mission(value.mission_id);
        if (value.objectives.size() != definition->objectives.size() || std::ranges::any_of(value.objectives, [&](const auto& record) {
                return std::ranges::none_of(definition->objectives, [&](const auto& item) { return item.id == record.id; });
            })) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        runtime.instances_.push_back(std::move(value));
    }
    if (!in) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
    if (active != 0) {
        const MissionInstanceId active_id{active};
        const auto* active_instance = runtime.instance(active_id);
        if (!active_instance) return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        const auto* active_mission = runtime.mission(active_instance->mission_id);
        if (!active_mission || (active_mission->state != MissionState::Deployed && active_mission->state != MissionState::Active &&
                                active_mission->state != MissionState::ExtractionAvailable))
            return core::Result<MissionRuntime, MissionError>::failure(MissionError::InvalidSerializedState);
        runtime.active_external_ = active_id;
    }
    return core::Result<MissionRuntime, MissionError>::success(std::move(runtime));
}

core::Result<GeneratedMissionPackage, MissionError> generate_offer(const GenerationRequest& request, GenerationCursor& cursor) {
    if (request.save_seed == 0 || request.family_key == 0 || request.generation_version == 0 || request.max_attempts == 0)
        return core::Result<GeneratedMissionPackage, MissionError>::failure(MissionError::GenerationFailed);
    if (cursor.family_key != request.family_key || cursor.revision != request.expected_cursor_revision)
        return core::Result<GeneratedMissionPackage, MissionError>::failure(MissionError::InvalidSnapshot);
    const auto ordinal = cursor.ordinal++;
    ++cursor.revision;
    const auto scope_key = core::splitmix64_mix(request.family_key ^ core::splitmix64_mix(ordinal));
    const auto rng_result = core::make_scoped_rng({request.save_seed, core::StreamKind::MissionGeneration, scope_key, request.generation_version});
    if (!rng_result) return core::Result<GeneratedMissionPackage, MissionError>::failure(MissionError::GenerationFailed);
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
