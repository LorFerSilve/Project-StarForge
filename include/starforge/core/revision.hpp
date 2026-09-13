#pragma once

#include <compare>
#include <cstdint>
#include <limits>

namespace starforge::core {

template <typename Tag>
class MonotonicRevision {
public:
    using rep_type = std::uint64_t;

    constexpr MonotonicRevision() noexcept = default;
    explicit constexpr MonotonicRevision(rep_type value) noexcept : value_(value) {}

    [[nodiscard]] constexpr rep_type raw() const noexcept { return value_; }

    [[nodiscard]] constexpr bool can_advance() const noexcept {
        return value_ != std::numeric_limits<rep_type>::max();
    }

    constexpr bool advance() noexcept {
        if (!can_advance()) {
            return false;
        }
        ++value_;
        return true;
    }

    friend constexpr auto operator<=>(MonotonicRevision, MonotonicRevision) noexcept = default;

private:
    rep_type value_{0};
};

struct StateRevisionTag final {};
struct ActivationEpochTag final {};

using StateRevision = MonotonicRevision<StateRevisionTag>;
using ActivationEpoch = MonotonicRevision<ActivationEpochTag>;

}  // namespace starforge::core
