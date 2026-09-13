#pragma once

#include <cstdint>

namespace starforge::simulation {

enum class SimulationPhase : std::uint8_t {
    InputCommandSampling = 0,
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

}  // namespace starforge::simulation
