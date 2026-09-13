#pragma once

#include "starforge/core/result.hpp"
#include "starforge/core/simulation_time.hpp"
#include "starforge/simulation/tick_pipeline.hpp"

#include <cstdint>

namespace starforge::simulation {

enum class SimulationClockError : std::uint8_t {
    TickOverflow,
};

class SimulationClock final {
public:
    explicit SimulationClock(TickPipeline& pipeline) noexcept : pipeline_(&pipeline) {}

    [[nodiscard]] starforge::core::SimulationTick current_tick() const noexcept { return current_tick_; }
    [[nodiscard]] bool true_paused() const noexcept { return true_paused_; }

    void set_true_paused(bool paused) noexcept { true_paused_ = paused; }

    [[nodiscard]] starforge::core::Result<bool, SimulationClockError> step();
    [[nodiscard]] starforge::core::Result<void, SimulationClockError> run_ticks(std::uint64_t count);

private:
    TickPipeline* pipeline_;
    starforge::core::SimulationTick current_tick_{};
    bool true_paused_{false};
};

}  // namespace starforge::simulation
