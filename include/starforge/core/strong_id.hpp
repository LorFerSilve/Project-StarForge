#pragma once

#include <compare>
#include <concepts>
#include <cstdint>
#include <type_traits>

namespace starforge::core {

template <typename Tag, std::unsigned_integral Rep = std::uint64_t>
class StrongId {
public:
    using tag_type = Tag;
    using rep_type = Rep;

    constexpr StrongId() noexcept = default;
    explicit constexpr StrongId(Rep value) noexcept : value_(value) {}

    [[nodiscard]] constexpr Rep raw() const noexcept { return value_; }
    [[nodiscard]] constexpr bool valid() const noexcept { return value_ != Rep{0}; }
    [[nodiscard]] explicit constexpr operator bool() const noexcept { return valid(); }

    friend constexpr auto operator<=>(StrongId, StrongId) noexcept = default;

private:
    Rep value_{0};
};

template <typename Tag>
struct PersistentIdTag final {};

template <typename Tag>
using PersistentId = StrongId<PersistentIdTag<Tag>, std::uint64_t>;

static_assert(std::is_trivially_copyable_v<StrongId<struct StrongIdSanityTag>>);

}  // namespace starforge::core
