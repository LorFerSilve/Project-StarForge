#pragma once

#include <cstdint>

namespace starforge::core {

class Pcg32 final {
public:
    Pcg32(std::uint64_t seed, std::uint64_t stream) noexcept;

    [[nodiscard]] std::uint32_t next_u32() noexcept;
    [[nodiscard]] std::uint64_t state() const noexcept { return state_; }
    [[nodiscard]] std::uint64_t increment() const noexcept { return increment_; }

private:
    std::uint64_t state_{0};
    std::uint64_t increment_{0};
};

[[nodiscard]] std::uint64_t splitmix64(std::uint64_t value) noexcept;
[[nodiscard]] Pcg32 make_scoped_rng(std::uint64_t session_seed, std::uint64_t scope_key) noexcept;

} // namespace starforge::core
