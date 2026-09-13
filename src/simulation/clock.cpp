#include "starforge/simulation/clock.hpp"

#include <limits>

namespace starforge::simulation {

starforge::core::Result<bool, SimulationClockError> SimulationClock::step() {
    if (true_paused_) {
        return starforge::core::Result<bool, SimulationClockError>::success(false);
    }
    if (current_tick_.raw() == std::numeric_limits<starforge::core::SimulationTick::rep_type>::max()) {
        return starforge::core::Result<bool, SimulationClockError>::failure(
            SimulationClockError::TickOverflow);
    }

    current_tick_ = starforge::core::SimulationTick{current_tick_.raw() + 1U};
    pipeline_->execute(current_tick_);
    return starforge::core::Result<bool, SimulationClockError>::success(true);
}

starforge::core::Result<void, SimulationClockError> SimulationClock::run_ticks(std::uint64_t count) {
    for (std::uint64_t index = 0; index < count; ++index) {
        const auto result = step();
        if (!result) {
            return starforge::core::Result<void, SimulationClockError>::failure(result.error());
        }
        if (!result.value()) {
            break;
        }
    }
    return starforge::core::Result<void, SimulationClockError>::success();
}

}  // namespace starforge::simulation
