#include "starforge/core/build_contract.hpp"
#include "starforge/core/rng.hpp"
#include "starforge/simulation/simulation_clock.hpp"
#include "starforge/simulation/tick_pipeline.hpp"
#include "starforge/transactions/transaction_coordinator.hpp"

#include <charconv>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

namespace {

std::uint32_t parse_worker_count(int argc, char** argv) {
    constexpr std::string_view prefix{"--workers="};
    for (int index = 1; index < argc; ++index) {
        const std::string_view argument{argv[index]};
        if (!argument.starts_with(prefix)) {
            continue;
        }

        std::uint32_t value = 0;
        const auto text = argument.substr(prefix.size());
        const auto result = std::from_chars(text.data(), text.data() + text.size(), value);
        if (result.ec != std::errc{} || result.ptr != text.data() + text.size() || value == 0) {
            return 0;
        }
        return value;
    }
    return 1;
}

std::vector<std::uint32_t> prepared_order(std::uint32_t workers) {
    if (workers >= 4) {
        return {40, 30, 20, 10};
    }
    if (workers >= 2) {
        return {20, 40, 10, 30};
    }
    return {10, 20, 30, 40};
}

} // namespace

int main(int argc, char** argv) {
    const auto workers = parse_worker_count(argc, argv);
    if (workers == 0) {
        std::cerr << "invalid --workers value\n";
        return 2;
    }

    starforge::simulation::SimulationClock clock;
    starforge::simulation::TickPipeline pipeline;
    std::uint64_t stable_boundaries = 0;
    pipeline.add_handler(
        starforge::simulation::TickPhase::StableSimulationBoundary,
        [&](starforge::simulation::SimulationTick) { ++stable_boundaries; });

    auto rng = starforge::core::make_scoped_rng(0x53544152464F5247ULL, 1ULL);
    for (int index = 0; index < 8; ++index) {
        const auto tick = clock.step();
        pipeline.execute(tick);
        (void)rng.next_u32();
    }

    starforge::transactions::TransactionCoordinator coordinator;
    std::uint64_t transaction_checksum = 0;
    std::vector<starforge::transactions::PreparedOperation> operations;
    for (const auto key : prepared_order(workers)) {
        operations.push_back({
            key,
            [] { return true; },
            [&, key] { transaction_checksum = transaction_checksum * 131ULL + key; }});
    }

    const auto transaction = coordinator.commit(std::move(operations));
    if (!transaction) {
        std::cerr << "deterministic transaction failed\n";
        return 3;
    }

    std::cout << "Project StarForge headless deterministic core\n"
              << "implementation_baseline=" << starforge::core::build_contract_name() << '\n'
              << "authoritative_simulation_hz=" << starforge::core::kAuthoritativeSimulationHz << '\n'
              << "ticks=" << clock.now().value() << '\n'
              << "stable_boundaries=" << stable_boundaries << '\n'
              << "rng_state=" << rng.state() << '\n'
              << "transaction_id=" << transaction->transaction_id.value() << '\n'
              << "transaction_checksum=" << transaction_checksum << '\n';

    return stable_boundaries == 8 ? 0 : 1;
}
