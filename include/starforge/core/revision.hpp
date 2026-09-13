#pragma once

#include <compare>
#include <cstdint>

namespace starforge::core {

template <typename Tag>
class Revision final {
public:
    constexpr Revision() = default;
    explicit constexpr Revision(std::uint64_t value) noexcept : value_(value) {}

    [[nodiscard]] constexpr std::uint64_t value() const noexcept { return value_; }
    constexpr void advance() noexcept { ++value_; }

    friend constexpr auto operator<=>(const Revision&, const Revision&) = default;

private:
    std::uint64_t value_{0};
};

} // namespace starforge::core
