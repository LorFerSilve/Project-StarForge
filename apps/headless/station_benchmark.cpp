#include "starforge/station/station.hpp"

#include <cstdint>
#include <iostream>

int main() {
    using namespace starforge::station;

    HorizonStationSimulation station;
    if (!station.topology.add_module({StationModuleId{1}, true}) ||
        !station.atmosphere.add_cell({PressureCellId{1}, 210'000, 790'000}) ||
        !station.environment.add_node({StationModuleId{1}, 2'000'000, 250'000, 25'000}) ||
        !station.power.add_producer({PowerNodeId{1}, 5'000, true}) ||
        !station.power.add_consumer({PowerNodeId{2}, 3'000, PowerPriority::P0, true}) ||
        !station.power.add_consumer({PowerNodeId{3}, 1'500, PowerPriority::P2, true})) {
        std::cerr << "failed to configure Horizon station benchmark\n";
        return 1;
    }

    constexpr std::uint64_t benchmark_ticks = 10'000;
    station.advance(benchmark_ticks, SimulationMode::Offscreen);
    const auto state = station.snapshot();
    if (state.tick != benchmark_ticks || state.allocated_power_w != 4'500 || state.gas_mmol != 1'000'000) {
        std::cerr << "station benchmark invariant failure\n";
        return 2;
    }

    std::cout << "horizon-station benchmark ticks=" << state.tick
              << " topology_revision=" << state.topology_revision
              << " allocated_power_w=" << state.allocated_power_w << '\n';
    return 0;
}
