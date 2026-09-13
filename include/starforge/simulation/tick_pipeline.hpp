#pragma once

#include "starforge/simulation/simulation_clock.hpp"

#include <array>
#include <functional>
#include <vector>

namespace starforge::simulation {

class TickPipeline final {
public:
    using Handler = std::function<void(SimulationTick)>;

    void add_handler(TickPhase phase, Handler handler);
    void execute(SimulationTick tick) const;

private:
    static constexpr std::size_t kPhaseCount = 15;
    std::array<std::vector<Handler>, kPhaseCount> handlers_{};
};

} // namespace starforge::simulation
