#include "starforge/simulation/tick_pipeline.hpp"

#include <cstddef>

namespace starforge::simulation {

void TickPipeline::add_handler(TickPhase phase, Handler handler) {
    handlers_.at(static_cast<std::size_t>(phase)).push_back(std::move(handler));
}

void TickPipeline::execute(SimulationTick tick) const {
    for (const auto& phase_handlers : handlers_) {
        for (const auto& handler : phase_handlers) {
            handler(tick);
        }
    }
}

} // namespace starforge::simulation
