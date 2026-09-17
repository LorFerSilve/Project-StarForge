#pragma once

#include "starforge/core/result.hpp"
#include "starforge/core/strong_id.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace starforge::raids {

using RaidId = core::PersistentId<struct RaidTag>;
using DefenseEventId = core::PersistentId<struct DefenseEventTag>;
using DynamicEventId = core::PersistentId<struct DynamicEventTag>;
using CargoId = core::PersistentId<struct RaidCargoTag>;

enum class RaidError : std::uint8_t {
    InvalidId,
    DuplicateEvent,
    EventNotFound,
    InvalidState,
    DuplicateTransaction,
    InvalidSnapshot,
    InvalidSerializedState,
};

enum class DefensePhase : std::uint8_t {
    Approach,
    ExternalDefense,
    BoardingAttempt,
    InteriorDefense,
    Objective,
    Withdrawal,
    Recovery,
    Resolved,
};

enum class ReinforcementState : std::uint8_t { None, Calling, Committed, Responding, Arrived, Cancelled };
enum class Resolution : std::uint8_t { Unresolved, Defended, Failed };

enum class CargoOwner : std::uint8_t { Station, AttackerCarrier, AttackerTransport, Extracted };

struct CargoRecord final {
    CargoId id{};
    CargoOwner owner{CargoOwner::Station};
    std::uint64_t terminal_transaction{0};
};

struct DefenseEventRecord final {
    DefenseEventId id{};
    std::uint64_t seed{0};
    DefensePhase phase{DefensePhase::Approach};
    ReinforcementState reinforcement{ReinforcementState::None};
    std::uint32_t reinforcement_budget{0};
    std::uint32_t reinforcements_arrived{0};
    std::uint64_t simulation_tick{0};
    std::uint64_t station_damage{0};
    std::vector<CargoRecord> cargo{};
    Resolution resolution{Resolution::Unresolved};
    std::uint64_t revision{1};
};

struct DefenseStep final {
    std::uint32_t defensive_capability{0};
    std::uint32_t attacker_capability{0};
    std::uint32_t station_damage{0};
    bool boarding_route_available{false};
};

struct DynamicEventRecord final {
    DynamicEventId id{};
    std::uint64_t seed{0};
    bool severe_station_attack{false};
    bool resolved{false};
};

class RaidRuntime final {
public:
    [[nodiscard]] core::Result<void, RaidError> add_defense_event(DefenseEventRecord event);
    [[nodiscard]] core::Result<void, RaidError> advance_defense(DefenseEventId id,
                                                               const DefenseStep& step,
                                                               std::uint64_t tick);
    [[nodiscard]] core::Result<void, RaidError> call_reinforcements(DefenseEventId id,
                                                                    std::uint32_t finite_budget);
    [[nodiscard]] core::Result<void, RaidError> advance_reinforcements(DefenseEventId id);
    [[nodiscard]] core::Result<void, RaidError> steal_cargo(DefenseEventId id,
                                                            CargoId cargo_id,
                                                            std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, RaidError> commit_hostile_extraction(DefenseEventId id,
                                                                         CargoId cargo_id,
                                                                         std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, RaidError> resolve_defense(DefenseEventId id, Resolution resolution,
                                                                std::uint64_t tick);
    [[nodiscard]] bool can_generate_severe_station_attack(std::uint64_t tick) const noexcept;
    [[nodiscard]] core::Result<void, RaidError> add_dynamic_event(DynamicEventRecord event);

    [[nodiscard]] const DefenseEventRecord* defense_event(DefenseEventId id) const noexcept;
    [[nodiscard]] std::string serialize() const;
    [[nodiscard]] static core::Result<RaidRuntime, RaidError> deserialize(std::string_view data);

private:
    std::vector<DefenseEventRecord> defenses_{};
    std::vector<DynamicEventRecord> dynamic_events_{};
    std::uint64_t recovery_grace_until_{0};
};

}  // namespace starforge::raids
