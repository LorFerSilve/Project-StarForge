#pragma once

#include <compare>
#include <cstdint>

namespace starforge::simulation {

class SimulationTick final {
public:
    constexpr SimulationTick() = default;
    explicit constexpr SimulationTick(std::uint64_t value) noexcept : value_(value) {}

    [[nodiscard]] constexpr std::uint64_t value() const noexcept { return value_; }
    constexpr void advance() noexcept { ++value_; }
    friend constexpr auto operator<=>(const SimulationTick&, const SimulationTick&) = default;

private:
    std::uint64_t value_{0};
};

class SimulationClock final {
public:
    static constexpr std::uint32_t kFrequencyHz = 60;

    explicit constexpr SimulationClock(SimulationTick initial = {}) noexcept : tick_(initial) {}
    [[nodiscard]] constexpr SimulationTick now() const noexcept { return tick_; }
    constexpr SimulationTick step() noexcept { tick_.advance(); return tick_; }

private:
    SimulationTick tick_{};
};

enum class TickPhase : std::uint8_t {
    InputCommandSampling,
    PreSimulationCommandValidation,
    MovementControlIntent,
    PhysicsStepPreparation,
    PhysicsFixedStep,
    PhysicsResultCollection,
    CombatInteractionResolution,
    LocalActorLogic,
    MissionRaidObjectiveLogic,
    StationStrategicEconomyTimers,
    CrossDomainTransactionCommit,
    CommittedEventDispatch,
    CleanupDeferredDestruction,
    PersistentStateSynchronization,
    StableSimulationBoundary,
};

} // namespace starforge::simulation
