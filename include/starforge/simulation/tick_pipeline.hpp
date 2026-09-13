#pragma once

#include "starforge/core/result.hpp"
#include "starforge/core/simulation_time.hpp"
#include "starforge/simulation/phase.hpp"

#include <cstdint>
#include <functional>
#include <vector>

namespace starforge::simulation {

class ITickSystem {
public:
    virtual ~ITickSystem() = default;
    [[nodiscard]] virtual SimulationPhase phase() const noexcept = 0;
    [[nodiscard]] virtual std::uint32_t stable_order() const noexcept = 0;
    virtual void tick(starforge::core::SimulationTick tick) = 0;
};

enum class TickPipelineError : std::uint8_t {
    DuplicatePhaseOrder,
};

class TickPipeline final {
public:
    [[nodiscard]] starforge::core::Result<void, TickPipelineError> register_system(ITickSystem& system);
    void execute(starforge::core::SimulationTick tick);

    [[nodiscard]] std::size_t system_count() const noexcept { return systems_.size(); }

private:
    std::vector<std::reference_wrapper<ITickSystem>> systems_;
};

}  // namespace starforge::simulation
