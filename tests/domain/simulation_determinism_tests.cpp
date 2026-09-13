#include "starforge/core/rng.hpp"
#include "starforge/simulation/clock.hpp"
#include "starforge/simulation/command_queue.hpp"
#include "starforge/simulation/tick_pipeline.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <thread>
#include <vector>

namespace {

class RecordingSystem final : public starforge::simulation::ITickSystem {
public:
    RecordingSystem(
        starforge::simulation::SimulationPhase phase,
        std::uint32_t order,
        std::vector<std::uint32_t>& trace) noexcept
        : phase_(phase), order_(order), trace_(&trace) {}

    [[nodiscard]] starforge::simulation::SimulationPhase phase() const noexcept override { return phase_; }
    [[nodiscard]] std::uint32_t stable_order() const noexcept override { return order_; }

    void tick(starforge::core::SimulationTick) override {
        trace_->push_back(static_cast<std::uint32_t>(phase_) * 100U + order_);
    }

private:
    starforge::simulation::SimulationPhase phase_;
    std::uint32_t order_;
    std::vector<std::uint32_t>* trace_;
};

struct TestCommand final {
    starforge::simulation::CommandMetadata metadata{};
    int payload{0};
};

[[nodiscard]] std::uint64_t worker_partition_digest(std::size_t worker_count) {
    constexpr std::size_t kTaskCount = 257U;
    std::vector<std::uint64_t> values(kTaskCount, 0U);
    std::vector<std::jthread> workers;
    workers.reserve(worker_count);

    for (std::size_t worker = 0; worker < worker_count; ++worker) {
        workers.emplace_back([worker, worker_count, &values] {
            for (std::size_t ordinal = worker; ordinal < values.size(); ordinal += worker_count) {
                auto rng_result = starforge::core::make_scoped_rng({
                    UINT64_C(0xA55A5AA55AA55AA5),
                    starforge::core::StreamKind::DeterminismProbe,
                    static_cast<std::uint64_t>(ordinal),
                    1U,
                });
                values[ordinal] = rng_result.value().next_u64();
            }
        });
    }
    workers.clear();

    std::uint64_t digest = UINT64_C(14695981039346656037);
    constexpr std::uint64_t kFnvPrime = UINT64_C(1099511628211);
    for (std::size_t ordinal = 0; ordinal < values.size(); ++ordinal) {
        digest ^= static_cast<std::uint64_t>(ordinal);
        digest *= kFnvPrime;
        digest ^= values[ordinal];
        digest *= kFnvPrime;
    }
    return digest;
}

}  // namespace

TEST_CASE("Simulation.Pipeline.PhaseOrderIsIndependentOfRegistrationOrder", "[domain][headless][determinism]") {
    std::vector<std::uint32_t> trace;
    RecordingSystem stable{starforge::simulation::SimulationPhase::StableSimulationBoundary, 0U, trace};
    RecordingSystem input_second{starforge::simulation::SimulationPhase::InputCommandSampling, 2U, trace};
    RecordingSystem commit{starforge::simulation::SimulationPhase::CrossDomainTransactionCommit, 0U, trace};
    RecordingSystem input_first{starforge::simulation::SimulationPhase::InputCommandSampling, 1U, trace};

    starforge::simulation::TickPipeline pipeline;
    REQUIRE(pipeline.register_system(stable));
    REQUIRE(pipeline.register_system(input_second));
    REQUIRE(pipeline.register_system(commit));
    REQUIRE(pipeline.register_system(input_first));

    pipeline.execute(starforge::core::SimulationTick{1U});
    REQUIRE(trace == std::vector<std::uint32_t>{1U, 2U, 1000U, 1400U});
}

TEST_CASE("Simulation.Clock.TruePauseAdvancesZeroSimulationTime", "[domain][headless][determinism]") {
    std::vector<std::uint32_t> trace;
    RecordingSystem stable{starforge::simulation::SimulationPhase::StableSimulationBoundary, 0U, trace};
    starforge::simulation::TickPipeline pipeline;
    REQUIRE(pipeline.register_system(stable));

    starforge::simulation::SimulationClock clock{pipeline};
    clock.set_true_paused(true);
    const auto paused = clock.step();
    REQUIRE(paused);
    REQUIRE_FALSE(paused.value());
    REQUIRE(clock.current_tick() == starforge::core::SimulationTick{0U});
    REQUIRE(trace.empty());

    clock.set_true_paused(false);
    REQUIRE(clock.run_ticks(3U));
    REQUIRE(clock.current_tick() == starforge::core::SimulationTick{3U});
    REQUIRE(trace.size() == 3U);
}

TEST_CASE("Simulation.Commands.AuthoritativeDrainUsesStableOrderingKey", "[domain][headless][determinism]") {
    starforge::simulation::DeterministicCommandQueue<TestCommand> queue;
    const auto source = starforge::simulation::CommandSourceKind::DevelopmentTool;
    queue.push({{starforge::core::CommandId{4U}, starforge::core::SimulationTick{9U}, source, {}, {}, 2U, 0U}, 40});
    queue.push({{starforge::core::CommandId{3U}, starforge::core::SimulationTick{8U}, source, {}, {}, 2U, 0U}, 30});
    queue.push({{starforge::core::CommandId{2U}, starforge::core::SimulationTick{8U}, source, {}, {}, 1U, 1U}, 20});
    queue.push({{starforge::core::CommandId{1U}, starforge::core::SimulationTick{8U}, source, {}, {}, 1U, 0U}, 10});

    const auto drained = queue.drain_ordered();
    REQUIRE(drained.size() == 4U);
    REQUIRE(drained[0].payload == 10);
    REQUIRE(drained[1].payload == 20);
    REQUIRE(drained[2].payload == 30);
    REQUIRE(drained[3].payload == 40);
    REQUIRE(queue.empty());
}

TEST_CASE("Simulation.Rng.WorkerPartitionCountCannotChangeAuthoritativeOutcome", "[domain][headless][determinism]") {
    const auto baseline = worker_partition_digest(1U);
    REQUIRE(worker_partition_digest(1U) == baseline);
    REQUIRE(worker_partition_digest(2U) == baseline);
    REQUIRE(worker_partition_digest(4U) == baseline);
    REQUIRE(worker_partition_digest(8U) == baseline);
}
