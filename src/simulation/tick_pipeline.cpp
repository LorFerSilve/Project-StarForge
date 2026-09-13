#include "starforge/simulation/tick_pipeline.hpp"

#include <algorithm>

namespace starforge::simulation {

starforge::core::Result<void, TickPipelineError> TickPipeline::register_system(ITickSystem& system) {
    const auto duplicate = std::ranges::find_if(systems_, [&system](const auto existing) {
        return existing.get().phase() == system.phase() &&
               existing.get().stable_order() == system.stable_order();
    });
    if (duplicate != systems_.end()) {
        return starforge::core::Result<void, TickPipelineError>::failure(
            TickPipelineError::DuplicatePhaseOrder);
    }

    systems_.push_back(system);
    std::ranges::sort(systems_, [](const auto lhs, const auto rhs) {
        if (lhs.get().phase() != rhs.get().phase()) {
            return lhs.get().phase() < rhs.get().phase();
        }
        return lhs.get().stable_order() < rhs.get().stable_order();
    });
    return starforge::core::Result<void, TickPipelineError>::success();
}

void TickPipeline::execute(starforge::core::SimulationTick tick) {
    for (ITickSystem& system : systems_) {
        system.tick(tick);
    }
}

}  // namespace starforge::simulation
