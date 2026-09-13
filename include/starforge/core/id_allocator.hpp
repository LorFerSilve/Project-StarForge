#pragma once

#include "starforge/core/result.hpp"

#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace starforge::core {

enum class IdAllocationError : std::uint8_t {
    Exhausted,
    InvalidRestoredState,
    CandidateMismatch,
};

template <typename Id>
concept StrongIdLike = requires(Id id) {
    typename Id::rep_type;
    requires std::unsigned_integral<typename Id::rep_type>;
    { id.raw() } -> std::same_as<typename Id::rep_type>;
};

template <StrongIdLike Id>
class MonotonicIdAllocator {
public:
    using rep_type = typename Id::rep_type;

    MonotonicIdAllocator() noexcept = default;

    [[nodiscard]] static Result<MonotonicIdAllocator, IdAllocationError> from_state(
        rep_type next_value,
        bool exhausted = false) noexcept {
        if (next_value == rep_type{0}) {
            return Result<MonotonicIdAllocator, IdAllocationError>::failure(
                IdAllocationError::InvalidRestoredState);
        }

        MonotonicIdAllocator allocator;
        allocator.next_value_ = next_value;
        allocator.exhausted_ = exhausted;
        return Result<MonotonicIdAllocator, IdAllocationError>::success(allocator);
    }

    [[nodiscard]] Result<Id, IdAllocationError> candidate() const noexcept {
        if (exhausted_) {
            return Result<Id, IdAllocationError>::failure(IdAllocationError::Exhausted);
        }
        return Result<Id, IdAllocationError>::success(Id{next_value_});
    }

    [[nodiscard]] Result<Id, IdAllocationError> commit(Id expected) noexcept {
        const auto candidate_result = candidate();
        if (!candidate_result) {
            return Result<Id, IdAllocationError>::failure(candidate_result.error());
        }
        if (candidate_result.value() != expected || !expected.valid()) {
            return Result<Id, IdAllocationError>::failure(IdAllocationError::CandidateMismatch);
        }

        const Id committed = expected;
        if (next_value_ == std::numeric_limits<rep_type>::max()) {
            exhausted_ = true;
        } else {
            ++next_value_;
        }
        return Result<Id, IdAllocationError>::success(committed);
    }

    [[nodiscard]] rep_type next_raw_value() const noexcept { return next_value_; }
    [[nodiscard]] bool exhausted() const noexcept { return exhausted_; }

private:
    rep_type next_value_{1};
    bool exhausted_{false};
};

}  // namespace starforge::core
