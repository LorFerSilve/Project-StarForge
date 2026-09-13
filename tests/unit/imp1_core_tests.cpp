#include "starforge/core/rng.hpp"
#include "starforge/core/strong_id.hpp"
#include "starforge/read_models/snapshot.hpp"
#include "starforge/simulation/contracts.hpp"
#include "starforge/simulation/simulation_clock.hpp"
#include "starforge/simulation/tick_pipeline.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <vector>

namespace {
struct RobotTag;
struct RevisionTag;
}

TEST_CASE("persistent id candidates are transactional and deterministic") {
    starforge::core::PersistentIdAllocator<RobotTag> allocator;
    const auto first = allocator.candidate();
    REQUIRE(first);
    REQUIRE(first->value() == 1);

    // Merely preparing a candidate does not consume it.
    const auto same_candidate = allocator.candidate();
    REQUIRE(same_candidate);
    REQUIRE(*same_candidate == *first);

    REQUIRE(allocator.commit(*first));
    const auto second = allocator.candidate();
    REQUIRE(second);
    REQUIRE(second->value() == 2);
}

TEST_CASE("scoped PCG32 streams replay exactly") {
    auto lhs = starforge::core::make_scoped_rng(0x12345678ULL, 17ULL);
    auto rhs = starforge::core::make_scoped_rng(0x12345678ULL, 17ULL);
    auto other = starforge::core::make_scoped_rng(0x12345678ULL, 18ULL);

    for (int i = 0; i < 64; ++i) {
        REQUIRE(lhs.next_u32() == rhs.next_u32());
    }
    REQUIRE(lhs.next_u32() != other.next_u32());
}

TEST_CASE("simulation clock advances only by fixed authoritative ticks") {
    starforge::simulation::SimulationClock clock;
    REQUIRE(clock.now().value() == 0);
    REQUIRE(clock.step().value() == 1);
    REQUIRE(clock.step().value() == 2);
    STATIC_REQUIRE(starforge::simulation::SimulationClock::kFrequencyHz == 60);
}

TEST_CASE("tick pipeline preserves canonical phase ordering") {
    starforge::simulation::TickPipeline pipeline;
    std::vector<int> order;

    pipeline.add_handler(starforge::simulation::TickPhase::CommittedEventDispatch,
                         [&](auto) { order.push_back(12); });
    pipeline.add_handler(starforge::simulation::TickPhase::InputCommandSampling,
                         [&](auto) { order.push_back(1); });
    pipeline.add_handler(starforge::simulation::TickPhase::CrossDomainTransactionCommit,
                         [&](auto) { order.push_back(11); });

    pipeline.execute(starforge::simulation::SimulationTick{4});
    REQUIRE(order == std::vector<int>{1, 11, 12});
}

TEST_CASE("read model snapshots are immutable and generation stamped") {
    starforge::read_models::SnapshotPublisher<int> publisher{7};
    const auto old_snapshot = publisher.snapshot();
    REQUIRE(*old_snapshot == 7);
    REQUIRE(publisher.generation().value() == 1);

    publisher.publish(9);
    REQUIRE(*old_snapshot == 7);
    REQUIRE(*publisher.snapshot() == 9);
    REQUIRE(publisher.generation().value() == 2);
}

TEST_CASE("command and event envelopes carry typed correlation metadata") {
    struct Payload { int value; };
    using Command = starforge::simulation::CommandEnvelope<Payload, RevisionTag>;
    Command command{
        starforge::core::CommandId{1},
        starforge::simulation::SimulationTick{10},
        starforge::core::TransactionId{4},
        starforge::core::Revision<RevisionTag>{3},
        Payload{42}};

    REQUIRE(command.command_id.value() == 1);
    REQUIRE(command.originating_tick.value() == 10);
    REQUIRE(command.transaction_id->value() == 4);
    REQUIRE(command.expected_revision->value() == 3);
    REQUIRE(command.payload.value == 42);
}
