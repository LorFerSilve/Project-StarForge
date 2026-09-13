#include "starforge/core/build_contract.hpp"
#include "starforge/core/rng.hpp"
#include "starforge/simulation/clock.hpp"
#include "starforge/simulation/tick_pipeline.hpp"

#include <charconv>
#include <cstdint>
#include <iostream>
#include <string_view>

namespace {

class DigestSystem final : public starforge::simulation::ITickSystem {
public:
    DigestSystem(starforge::simulation::SimulationPhase phase, std::uint32_t order, std::uint64_t& digest) noexcept
        : phase_(phase), order_(order), digest_(&digest) {}

    [[nodiscard]] starforge::simulation::SimulationPhase phase() const noexcept override { return phase_; }
    [[nodiscard]] std::uint32_t stable_order() const noexcept override { return order_; }

    void tick(starforge::core::SimulationTick tick) override {
        constexpr std::uint64_t kFnvPrime = UINT64_C(1099511628211);
        *digest_ ^= tick.raw();
        *digest_ *= kFnvPrime;
        *digest_ ^= static_cast<std::uint64_t>(phase_);
        *digest_ *= kFnvPrime;
        *digest_ ^= order_;
        *digest_ *= kFnvPrime;
    }

private:
    starforge::simulation::SimulationPhase phase_;
    std::uint32_t order_;
    std::uint64_t* digest_;
};

[[nodiscard]] bool parse_ticks(int argc, char** argv, std::uint64_t& ticks) noexcept {
    for (int index = 1; index + 1 < argc; ++index) {
        if (std::string_view{argv[index]} != "--ticks") {
            continue;
        }
        const std::string_view value{argv[index + 1]};
        const auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), ticks);
        return error == std::errc{} && end == value.data() + value.size();
    }
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    std::uint64_t requested_ticks = 1U;
    if (!parse_ticks(argc, argv, requested_ticks)) {
        std::cerr << "invalid --ticks value\n";
        return 2;
    }

    std::uint64_t digest = UINT64_C(14695981039346656037);
    starforge::simulation::TickPipeline pipeline;
    DigestSystem commit{starforge::simulation::SimulationPhase::CrossDomainTransactionCommit, 0U, digest};
    DigestSystem input{starforge::simulation::SimulationPhase::InputCommandSampling, 0U, digest};
    DigestSystem stable{starforge::simulation::SimulationPhase::StableSimulationBoundary, 0U, digest};

    if (!pipeline.register_system(commit) || !pipeline.register_system(stable) || !pipeline.register_system(input)) {
        std::cerr << "failed to construct deterministic headless tick pipeline\n";
        return 3;
    }

    starforge::simulation::SimulationClock clock{pipeline};
    const auto run = clock.run_ticks(requested_ticks);
    if (!run) {
        std::cerr << "headless simulation clock failed\n";
        return 4;
    }

    const auto rng_result = starforge::core::make_scoped_rng({
        UINT64_C(0x5A17F0A6D1C0FFEE),
        starforge::core::StreamKind::DeterminismProbe,
        requested_ticks,
        1U,
    });
    if (!rng_result) {
        std::cerr << "failed to construct deterministic RNG probe\n";
        return 5;
    }
    auto rng = rng_result.value();
    digest ^= rng.next_u64();

    std::cout << "Project StarForge headless IMP-1 foundation\n"
              << "implementation_baseline=" << starforge::core::build_contract_name() << '\n'
              << "authoritative_simulation_hz=" << starforge::core::kAuthoritativeSimulationHz << '\n'
              << "final_tick=" << clock.current_tick().raw() << '\n'
              << "determinism_digest=" << digest << '\n';
    return 0;
}
