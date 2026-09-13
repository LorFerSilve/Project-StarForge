#include "starforge/core/rng.hpp"

#include <bit>

namespace starforge::core {
namespace {

constexpr std::uint64_t kPcg32Multiplier = UINT64_C(6364136223846793005);
constexpr std::uint64_t kScopeKindMultiplier = UINT64_C(0xD1B54A32D192ED03);
constexpr std::uint64_t kScopeKeyMultiplier = UINT64_C(0x94D049BB133111EB);
constexpr std::uint64_t kVersionMultiplier = UINT64_C(0xBF58476D1CE4E5B9);

}  // namespace

Pcg32 Pcg32::seeded(std::uint64_t initial_state, std::uint64_t sequence) noexcept {
    Pcg32 rng{Pcg32State{0U, (sequence << 1U) | 1U}};
    static_cast<void>(rng.next_u32());
    rng.state_.state += initial_state;
    static_cast<void>(rng.next_u32());
    return rng;
}

Result<Pcg32, RngError> Pcg32::from_state(Pcg32State state) noexcept {
    if ((state.increment & 1U) == 0U) {
        return Result<Pcg32, RngError>::failure(RngError::InvalidIncrement);
    }
    return Result<Pcg32, RngError>::success(Pcg32{state});
}

std::uint32_t Pcg32::next_u32() noexcept {
    const std::uint64_t old_state = state_.state;
    state_.state = old_state * kPcg32Multiplier + state_.increment;
    const auto xor_shifted = static_cast<std::uint32_t>(((old_state >> 18U) ^ old_state) >> 27U);
    const auto rotation = static_cast<std::uint32_t>(old_state >> 59U);
    return std::rotr(xor_shifted, static_cast<int>(rotation));
}

std::uint64_t Pcg32::next_u64() noexcept {
    const auto high = static_cast<std::uint64_t>(next_u32());
    const auto low = static_cast<std::uint64_t>(next_u32());
    return (high << 32U) | low;
}

Result<std::uint32_t, RngError> Pcg32::uniform_bounded_u32(std::uint32_t bound) noexcept {
    if (bound == 0U) {
        return Result<std::uint32_t, RngError>::failure(RngError::InvalidBound);
    }

    const auto threshold = static_cast<std::uint32_t>((UINT64_C(1) << 32U) % bound);
    for (;;) {
        const std::uint32_t value = next_u32();
        if (value >= threshold) {
            return Result<std::uint32_t, RngError>::success(value % bound);
        }
    }
}

float Pcg32::uniform01_float() noexcept {
    constexpr float kScale = 1.0F / 16777216.0F;
    return static_cast<float>(next_u32() >> 8U) * kScale;
}

double Pcg32::uniform01_double() noexcept {
    constexpr double kScale = 1.0 / 9007199254740992.0;
    return static_cast<double>(next_u64() >> 11U) * kScale;
}

Result<Pcg32, RngError> make_scoped_rng(const ScopedRngDescriptor& descriptor) noexcept {
    if (descriptor.save_seed == 0U) {
        return Result<Pcg32, RngError>::failure(RngError::InvalidSaveSeed);
    }
    if (descriptor.stream_kind == StreamKind::Invalid) {
        return Result<Pcg32, RngError>::failure(RngError::InvalidStreamKind);
    }
    if (descriptor.generation_algorithm_version == 0U) {
        return Result<Pcg32, RngError>::failure(RngError::InvalidGenerationVersion);
    }

    std::uint64_t derivation_state = descriptor.save_seed;
    derivation_state ^= static_cast<std::uint64_t>(descriptor.stream_kind) * kScopeKindMultiplier;
    derivation_state ^= descriptor.scope_key * kScopeKeyMultiplier;
    derivation_state ^= static_cast<std::uint64_t>(descriptor.generation_algorithm_version) * kVersionMultiplier;

    const std::uint64_t initial_state = splitmix64_next(derivation_state);
    const std::uint64_t sequence = splitmix64_next(derivation_state);
    return Result<Pcg32, RngError>::success(Pcg32::seeded(initial_state, sequence));
}

}  // namespace starforge::core
