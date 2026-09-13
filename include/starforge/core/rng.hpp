#pragma once

#include "starforge/core/result.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>

namespace starforge::core {

enum class RngError : std::uint8_t {
    InvalidIncrement,
    InvalidBound,
    BoundTooLarge,
    InvalidSaveSeed,
    InvalidStreamKind,
    InvalidGenerationVersion,
};

enum class StreamKind : std::uint32_t {
    Invalid = 0,
    DeterminismProbe = 1,
    MissionGeneration = 10,
    DynamicEventGeneration = 11,
    RaidTargetGeneration = 12,
    LootGeneration = 13,
    EncounterComposition = 14,
    FactionMarketVariation = 15,
};

struct Pcg32State final {
    std::uint64_t state{0};
    std::uint64_t increment{1};

    friend constexpr bool operator==(const Pcg32State&, const Pcg32State&) noexcept = default;
};

class Pcg32 final {
public:
    [[nodiscard]] static Pcg32 seeded(std::uint64_t initial_state, std::uint64_t sequence) noexcept;
    [[nodiscard]] static Result<Pcg32, RngError> from_state(Pcg32State state) noexcept;

    [[nodiscard]] std::uint32_t next_u32() noexcept;
    [[nodiscard]] std::uint64_t next_u64() noexcept;
    [[nodiscard]] Result<std::uint32_t, RngError> uniform_bounded_u32(std::uint32_t bound) noexcept;
    [[nodiscard]] float uniform01_float() noexcept;
    [[nodiscard]] double uniform01_double() noexcept;

    [[nodiscard]] Pcg32State state() const noexcept { return state_; }

private:
    explicit Pcg32(Pcg32State state) noexcept : state_(state) {}

    Pcg32State state_{};
};

[[nodiscard]] constexpr std::uint64_t splitmix64_mix(std::uint64_t value) noexcept {
    value = (value ^ (value >> 30U)) * UINT64_C(0xBF58476D1CE4E5B9);
    value = (value ^ (value >> 27U)) * UINT64_C(0x94D049BB133111EB);
    return value ^ (value >> 31U);
}

[[nodiscard]] constexpr std::uint64_t splitmix64_next(std::uint64_t& state) noexcept {
    state += UINT64_C(0x9E3779B97F4A7C15);
    return splitmix64_mix(state);
}

struct ScopedRngDescriptor final {
    std::uint64_t save_seed{0};
    StreamKind stream_kind{StreamKind::Invalid};
    std::uint64_t scope_key{0};
    std::uint32_t generation_algorithm_version{0};
};

[[nodiscard]] Result<Pcg32, RngError> make_scoped_rng(const ScopedRngDescriptor& descriptor) noexcept;

template <typename T>
[[nodiscard]] Result<void, RngError> deterministic_shuffle(std::span<T> values, Pcg32& rng) noexcept {
    if (values.size() < 2U) {
        return Result<void, RngError>::success();
    }
    if (values.size() > static_cast<std::size_t>(UINT32_MAX)) {
        return Result<void, RngError>::failure(RngError::BoundTooLarge);
    }

    for (std::size_t index = values.size() - 1U; index > 0U; --index) {
        const auto selection = rng.uniform_bounded_u32(static_cast<std::uint32_t>(index + 1U));
        if (!selection) {
            return Result<void, RngError>::failure(selection.error());
        }
        std::swap(values[index], values[selection.value()]);
    }
    return Result<void, RngError>::success();
}

}  // namespace starforge::core
