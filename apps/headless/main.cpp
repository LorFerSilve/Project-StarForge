#include "starforge/core/build_contract.hpp"
#include "starforge/core/rng.hpp"
#include "starforge/simulation/simulation_clock.hpp"
#include "starforge/simulation/tick_pipeline.hpp"

#include <cstdint>
#include <iostream>

int main() {
    starforge::simulation::SimulationClock clock;
    starforge::simulation::TickPipeline pipeline;
    std::uint64_t stable_boundaries = 0;

    pipeline.add_handler(
        starforge::simulation::TickPhase::StableSimulationBoundary,
        [&](starforge::simulation::SimulationTick) { ++stable_boundaries; });

    auto rng = starforge::core::make_scoped_rng(0x53544152464F5247ULL, 1ULL);
    for (int i = 0; i < 8; ++i) {
        const auto tick = clock.step();
        pipeline.execute(tick);
        (void)rng.next_u32();
    }

    std::cout << "Project StarForge headless deterministic core\n"
              << "implementation_baseline=" << starforge::core::build_contract_name() << '\n'
              << "authoritative_simulation_hz=" << starforge::core::kAuthoritativeSimulationHz << '\n'
              << "ticks=" << clock.now().value() << '\n'
              << "stable_boundaries=" << stable_boundaries << '\n'
              << "rng_state=" << rng.state() << '\n';
    return stable_boundaries == 8 ? 0 : 1;
}
