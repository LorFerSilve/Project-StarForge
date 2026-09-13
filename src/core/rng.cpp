#include "starforge/core/rng.hpp"

namespace starforge::core {

Pcg32::Pcg32(std::uint64_t seed, std::uint64_t stream) noexcept
    : increment_((stream << 1U) | 1U) {
    (void)next_u32();
    state_ += seed;
    (void)next_u32();
}

std::uint32_t Pcg32::next_u32() noexcept {
    const auto old_state = state_;
    state_ = old_state * 6364136223846793005ULL + increment_;
    const auto xorshifted = static_cast<std::uint32_t>(((old_state >> 18U) ^ old_state) >> 27U);
    const auto rotation = static_cast<std::uint32_t>(old_state >> 59U);
    return (xorshifted >> rotation) | (xorshifted << ((-rotation) & 31U));
}

std::uint64_t splitmix64(std::uint64_t value) noexcept {
    value += 0x9E3779B97F4A7C15ULL;
    value = (value ^ (value >> 30U)) * 0xBF58476D1CE4E5B9ULL;
    value = (value ^ (value >> 27U)) * 0x94D049BB133111EBULL;
    return value ^ (value >> 31U);
}

Pcg32 make_scoped_rng(std::uint64_t session_seed, std::uint64_t scope_key) noexcept {
    return Pcg32{splitmix64(session_seed ^ scope_key), splitmix64(scope_key)};
}

} // namespace starforge::core
