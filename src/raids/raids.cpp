#include "starforge/raids/raids.hpp"

#include <algorithm>
#include <charconv>
#include <sstream>

namespace starforge::raids {
namespace {
constexpr std::uint64_t kRecoveryGraceTicks = 60ULL * 60ULL * 10ULL;

template <typename Id>
[[nodiscard]] bool valid(Id id) noexcept { return id.raw() != 0; }

[[nodiscard]] std::vector<std::string_view> split(std::string_view value, char delimiter) {
    std::vector<std::string_view> parts;
    while (true) {
        const auto position = value.find(delimiter);
        parts.push_back(value.substr(0, position));
        if (position == std::string_view::npos) break;
        value.remove_prefix(position + 1);
    }
    return parts;
}

[[nodiscard]] bool parse_u64(std::string_view text, std::uint64_t& value) {
    const auto* begin = text.data();
    const auto* end = begin + text.size();
    const auto [ptr, ec] = std::from_chars(begin, end, value);
    return ec == std::errc{} && ptr == end;
}
}  // namespace

core::Result<void, RaidError> RaidRuntime::add_defense_event(DefenseEventRecord event) {
    if (!valid(event.id) || event.revision == 0) return RaidError::InvalidId;
    if (defense_event(event.id) != nullptr) return RaidError::DuplicateEvent;
    for (std::size_t i = 0; i < event.cargo.size(); ++i) {
        if (!valid(event.cargo[i].id)) return RaidError::InvalidSnapshot;
        for (std::size_t j = i + 1; j < event.cargo.size(); ++j)
            if (event.cargo[i].id == event.cargo[j].id) return RaidError::InvalidSnapshot;
    }
    defenses_.push_back(std::move(event));
    return {};
}

core::Result<void, RaidError> RaidRuntime::advance_defense(DefenseEventId id, const DefenseStep& step,
                                                           std::uint64_t tick) {
    auto* event = const_cast<DefenseEventRecord*>(defense_event(id));
    if (event == nullptr) return RaidError::EventNotFound;
    if (event->phase == DefensePhase::Resolved || tick < event->simulation_tick) return RaidError::InvalidState;
    event->simulation_tick = tick;
    event->station_damage += step.station_damage;
    if (step.defensive_capability >= step.attacker_capability && event->phase != DefensePhase::Recovery) {
        event->phase = DefensePhase::Withdrawal;
    } else {
        switch (event->phase) {
        case DefensePhase::Approach: event->phase = DefensePhase::ExternalDefense; break;
        case DefensePhase::ExternalDefense:
            event->phase = step.boarding_route_available ? DefensePhase::BoardingAttempt : DefensePhase::Withdrawal;
            break;
        case DefensePhase::BoardingAttempt: event->phase = DefensePhase::InteriorDefense; break;
        case DefensePhase::InteriorDefense: event->phase = DefensePhase::Objective; break;
        case DefensePhase::Objective: event->phase = DefensePhase::Withdrawal; break;
        case DefensePhase::Withdrawal: event->phase = DefensePhase::Recovery; break;
        case DefensePhase::Recovery: break;
        case DefensePhase::Resolved: return RaidError::InvalidState;
        }
    }
    ++event->revision;
    return {};
}

core::Result<void, RaidError> RaidRuntime::call_reinforcements(DefenseEventId id, std::uint32_t finite_budget) {
    auto* event = const_cast<DefenseEventRecord*>(defense_event(id));
    if (event == nullptr) return RaidError::EventNotFound;
    if (finite_budget == 0 || event->reinforcement != ReinforcementState::None) return RaidError::InvalidState;
    event->reinforcement_budget = finite_budget;
    event->reinforcement = ReinforcementState::Calling;
    ++event->revision;
    return {};
}

core::Result<void, RaidError> RaidRuntime::advance_reinforcements(DefenseEventId id) {
    auto* event = const_cast<DefenseEventRecord*>(defense_event(id));
    if (event == nullptr) return RaidError::EventNotFound;
    switch (event->reinforcement) {
    case ReinforcementState::Calling: event->reinforcement = ReinforcementState::Committed; break;
    case ReinforcementState::Committed: event->reinforcement = ReinforcementState::Responding; break;
    case ReinforcementState::Responding:
        if (event->reinforcements_arrived >= event->reinforcement_budget) return RaidError::InvalidState;
        ++event->reinforcements_arrived;
        event->reinforcement = event->reinforcements_arrived == event->reinforcement_budget
                                   ? ReinforcementState::Arrived : ReinforcementState::Committed;
        break;
    default: return RaidError::InvalidState;
    }
    ++event->revision;
    return {};
}

core::Result<void, RaidError> RaidRuntime::steal_cargo(DefenseEventId id, CargoId cargo_id,
                                                       std::uint64_t transaction_id) {
    auto* event = const_cast<DefenseEventRecord*>(defense_event(id));
    if (event == nullptr) return RaidError::EventNotFound;
    auto it = std::find_if(event->cargo.begin(), event->cargo.end(), [cargo_id](const auto& cargo) { return cargo.id == cargo_id; });
    if (it == event->cargo.end() || it->owner != CargoOwner::Station) return RaidError::InvalidState;
    if (transaction_id == 0 || it->terminal_transaction == transaction_id) return RaidError::DuplicateTransaction;
    it->owner = CargoOwner::AttackerCarrier;
    it->terminal_transaction = transaction_id;
    ++event->revision;
    return {};
}

core::Result<void, RaidError> RaidRuntime::commit_hostile_extraction(DefenseEventId id, CargoId cargo_id,
                                                                    std::uint64_t transaction_id) {
    auto* event = const_cast<DefenseEventRecord*>(defense_event(id));
    if (event == nullptr) return RaidError::EventNotFound;
    auto it = std::find_if(event->cargo.begin(), event->cargo.end(), [cargo_id](const auto& cargo) { return cargo.id == cargo_id; });
    if (it == event->cargo.end() || (it->owner != CargoOwner::AttackerCarrier && it->owner != CargoOwner::AttackerTransport))
        return RaidError::InvalidState;
    if (transaction_id == 0 || it->terminal_transaction == transaction_id) return RaidError::DuplicateTransaction;
    it->owner = CargoOwner::Extracted;
    it->terminal_transaction = transaction_id;
    ++event->revision;
    return {};
}

core::Result<void, RaidError> RaidRuntime::resolve_defense(DefenseEventId id, Resolution resolution,
                                                           std::uint64_t tick) {
    auto* event = const_cast<DefenseEventRecord*>(defense_event(id));
    if (event == nullptr) return RaidError::EventNotFound;
    if (resolution == Resolution::Unresolved || event->resolution != Resolution::Unresolved || tick < event->simulation_tick)
        return RaidError::InvalidState;
    event->resolution = resolution;
    event->phase = DefensePhase::Resolved;
    event->simulation_tick = tick;
    ++event->revision;
    if (resolution == Resolution::Failed) recovery_grace_until_ = std::max(recovery_grace_until_, tick + kRecoveryGraceTicks);
    return {};
}

bool RaidRuntime::can_generate_severe_station_attack(std::uint64_t tick) const noexcept {
    return tick >= recovery_grace_until_;
}

core::Result<void, RaidError> RaidRuntime::add_dynamic_event(DynamicEventRecord event) {
    if (!valid(event.id)) return RaidError::InvalidId;
    if (event.severe_station_attack && !can_generate_severe_station_attack(0)) return RaidError::InvalidState;
    if (std::any_of(dynamic_events_.begin(), dynamic_events_.end(), [&event](const auto& existing) { return existing.id == event.id; }))
        return RaidError::DuplicateEvent;
    dynamic_events_.push_back(event);
    return {};
}

const DefenseEventRecord* RaidRuntime::defense_event(DefenseEventId id) const noexcept {
    const auto it = std::find_if(defenses_.begin(), defenses_.end(), [id](const auto& event) { return event.id == id; });
    return it == defenses_.end() ? nullptr : &*it;
}

std::string RaidRuntime::serialize() const {
    std::ostringstream out;
    out << "SFRAID1\n" << recovery_grace_until_ << '\n' << defenses_.size() << '\n';
    for (const auto& event : defenses_) {
        out << event.id.raw() << '|' << event.seed << '|' << static_cast<unsigned>(event.phase) << '|'
            << static_cast<unsigned>(event.reinforcement) << '|' << event.reinforcement_budget << '|'
            << event.reinforcements_arrived << '|' << event.simulation_tick << '|' << event.station_damage << '|'
            << static_cast<unsigned>(event.resolution) << '|' << event.revision << '|' << event.cargo.size() << '\n';
        for (const auto& cargo : event.cargo)
            out << cargo.id.raw() << '|' << static_cast<unsigned>(cargo.owner) << '|' << cargo.terminal_transaction << '\n';
    }
    return out.str();
}

core::Result<RaidRuntime, RaidError> RaidRuntime::deserialize(std::string_view data) {
    const auto lines = split(data, '\n');
    if (lines.size() < 3 || lines[0] != "SFRAID1") return RaidError::InvalidSerializedState;
    RaidRuntime runtime;
    std::uint64_t count = 0;
    if (!parse_u64(lines[1], runtime.recovery_grace_until_) || !parse_u64(lines[2], count)) return RaidError::InvalidSerializedState;
    std::size_t line = 3;
    for (std::uint64_t index = 0; index < count; ++index) {
        if (line >= lines.size()) return RaidError::InvalidSerializedState;
        const auto fields = split(lines[line++], '|');
        if (fields.size() != 11) return RaidError::InvalidSerializedState;
        std::uint64_t values[11]{};
        for (std::size_t i = 0; i < 11; ++i) if (!parse_u64(fields[i], values[i])) return RaidError::InvalidSerializedState;
        if (values[2] > static_cast<unsigned>(DefensePhase::Resolved) || values[3] > static_cast<unsigned>(ReinforcementState::Cancelled) || values[8] > static_cast<unsigned>(Resolution::Failed))
            return RaidError::InvalidSerializedState;
        DefenseEventRecord event{DefenseEventId{values[0]}, values[1], static_cast<DefensePhase>(values[2]), static_cast<ReinforcementState>(values[3]), static_cast<std::uint32_t>(values[4]), static_cast<std::uint32_t>(values[5]), values[6], values[7], {}, static_cast<Resolution>(values[8]), values[9]};
        for (std::uint64_t cargo_index = 0; cargo_index < values[10]; ++cargo_index) {
            if (line >= lines.size()) return RaidError::InvalidSerializedState;
            const auto cargo_fields = split(lines[line++], '|');
            std::uint64_t cargo_values[3]{};
            if (cargo_fields.size() != 3 || !parse_u64(cargo_fields[0], cargo_values[0]) || !parse_u64(cargo_fields[1], cargo_values[1]) || !parse_u64(cargo_fields[2], cargo_values[2]) || cargo_values[1] > static_cast<unsigned>(CargoOwner::Extracted))
                return RaidError::InvalidSerializedState;
            event.cargo.push_back({CargoId{cargo_values[0]}, static_cast<CargoOwner>(cargo_values[1]), cargo_values[2]});
        }
        const auto added = runtime.add_defense_event(std::move(event));
        if (!added) return RaidError::InvalidSerializedState;
    }
    return runtime;
}

}  // namespace starforge::raids
