#include "starforge/core/id_allocator.hpp"
#include "starforge/core/rng.hpp"
#include "starforge/core/strong_id.hpp"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <bit>
#include <concepts>
#include <cstdint>
#include <limits>

namespace {
struct CrewTag final {};
struct RobotTag final {};
using CrewId = starforge::core::PersistentId<CrewTag>;
using RobotId = starforge::core::PersistentId<RobotTag>;
}  // namespace

TEST_CASE("Core.Identity.PersistentIdsAreStronglyTyped", "[unit][identity]") {
    STATIC_REQUIRE(!std::same_as<CrewId, RobotId>);
    STATIC_REQUIRE(sizeof(CrewId) == sizeof(std::uint64_t));
    REQUIRE_FALSE(CrewId{}.valid());
    REQUIRE(CrewId{42U}.raw() == 42U);
}

TEST_CASE("Core.Identity.FailedPreparationDoesNotConsumePersistentId", "[unit][identity]") {
    starforge::core::MonotonicIdAllocator<CrewId> allocator;

    const auto first_candidate = allocator.candidate();
    REQUIRE(first_candidate);
    REQUIRE(first_candidate.value() == CrewId{1U});

    const auto same_candidate = allocator.candidate();
    REQUIRE(same_candidate);
    REQUIRE(same_candidate.value() == CrewId{1U});

    const auto committed = allocator.commit(first_candidate.value());
    REQUIRE(committed);
    REQUIRE(committed.value() == CrewId{1U});

    const auto second_candidate = allocator.candidate();
    REQUIRE(second_candidate);
    REQUIRE(second_candidate.value() == CrewId{2U});
}

TEST_CASE("Core.Identity.AllocatorNeverWrapsOrReuses", "[unit][identity]") {
    const auto restored = starforge::core::MonotonicIdAllocator<CrewId>::from_state(
        std::numeric_limits<std::uint64_t>::max());
    REQUIRE(restored);
    auto allocator = restored.value();

    const auto final_candidate = allocator.candidate();
    REQUIRE(final_candidate);
    REQUIRE(final_candidate.value().raw() == std::numeric_limits<std::uint64_t>::max());
    REQUIRE(allocator.commit(final_candidate.value()));
    REQUIRE(allocator.exhausted());

    const auto exhausted = allocator.candidate();
    REQUIRE_FALSE(exhausted);
    REQUIRE(exhausted.error() == starforge::core::IdAllocationError::Exhausted);
}

TEST_CASE("Core.Rng.Pcg32GoldenVectorIsLocked", "[unit][rng][golden]") {
    auto rng = starforge::core::Pcg32::seeded(42U, 54U);
    constexpr std::array<std::uint32_t, 6> expected{
        UINT32_C(0xA15C02B7),
        UINT32_C(0x7B47F409),
        UINT32_C(0xBA1D3330),
        UINT32_C(0x83D2F293),
        UINT32_C(0xBFA4784B),
        UINT32_C(0xCBED606E),
    };

    for (const auto value : expected) {
        REQUIRE(rng.next_u32() == value);
    }
}

TEST_CASE("Core.Rng.SplitMix64GoldenVectorIsLocked", "[unit][rng][golden]") {
    std::uint64_t state = 0U;
    constexpr std::array<std::uint64_t, 5> expected{
        UINT64_C(0xE220A8397B1DCDAF),
        UINT64_C(0x6E789E6AA1B965F4),
        UINT64_C(0x06C45D188009454F),
        UINT64_C(0xF88BB8A8724C81EC),
        UINT64_C(0x1B39896A51A8749B),
    };

    for (const auto value : expected) {
        REQUIRE(starforge::core::splitmix64_next(state) == value);
    }
}

TEST_CASE("Core.Rng.BoundedFloatAndShuffleContractsAreLocked", "[unit][rng][golden]") {
    auto bounded_rng = starforge::core::Pcg32::seeded(42U, 54U);
    constexpr std::array<std::uint32_t, 5> bounded_expected{3U, 7U, 4U, 5U, 5U};
    for (const auto expected : bounded_expected) {
        const auto value = bounded_rng.uniform_bounded_u32(10U);
        REQUIRE(value);
        REQUIRE(value.value() == expected);
    }

    auto float_rng = starforge::core::Pcg32::seeded(42U, 54U);
    REQUIRE(std::bit_cast<std::uint32_t>(float_rng.uniform01_float()) == UINT32_C(0x3F215C02));
    REQUIRE(std::bit_cast<std::uint64_t>(float_rng.uniform01_double()) == UINT64_C(0x3FDED1FD026E874C));

    auto shuffle_rng = starforge::core::Pcg32::seeded(42U, 54U);
    std::array<int, 10> values{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    REQUIRE(starforge::core::deterministic_shuffle<int>(values, shuffle_rng));
    REQUIRE(values == std::array<int, 10>{8, 2, 6, 4, 5, 1, 7, 0, 9, 3});
}

TEST_CASE("Core.Rng.ScopedDerivationAndResumeAreStable", "[unit][rng][golden]") {
    const auto scoped_result = starforge::core::make_scoped_rng({
        UINT64_C(0x123456789ABCDEF0),
        starforge::core::StreamKind::MissionGeneration,
        42U,
        1U,
    });
    REQUIRE(scoped_result);
    auto scoped = scoped_result.value();
    REQUIRE(scoped.state() == starforge::core::Pcg32State{
                                  UINT64_C(0xEE1AF8330989C07D),
                                  UINT64_C(0x645CD046CB6DC375)});
    REQUIRE(scoped.next_u32() == UINT32_C(0x1AC3B5B6));
    REQUIRE(scoped.next_u32() == UINT32_C(0xE801502F));

    auto original = starforge::core::Pcg32::seeded(42U, 54U);
    static_cast<void>(original.next_u32());
    static_cast<void>(original.next_u32());
    static_cast<void>(original.next_u32());
    const auto snapshot = original.state();

    const auto resumed_result = starforge::core::Pcg32::from_state(snapshot);
    REQUIRE(resumed_result);
    auto resumed = resumed_result.value();
    for (int index = 0; index < 16; ++index) {
        REQUIRE(resumed.next_u32() == original.next_u32());
    }
}
