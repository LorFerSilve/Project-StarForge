#pragma once

#include "starforge/core/result.hpp"

#include <compare>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace starforge::core {

template <typename Tag, typename Rep = std::uint64_t>
class StrongId final {
    static_assert(std::is_unsigned_v<Rep>);

public:
    using value_type = Rep;

    constexpr StrongId() = default;
    explicit constexpr StrongId(Rep value) noexcept : value_(value) {}

    [[nodiscard]] constexpr Rep value() const noexcept { return value_; }
    [[nodiscard]] constexpr bool valid() const noexcept { return value_ != 0; }
    explicit constexpr operator bool() const noexcept { return valid(); }

    friend constexpr auto operator<=>(const StrongId&, const StrongId&) = default;

private:
    Rep value_{0};
};

enum class IdAllocationError : std::uint8_t {
    Exhausted,
    CandidateMismatch,
};

template <typename Id>
class MonotonicIdAllocator final {
public:
    using value_type = typename Id::value_type;

    explicit constexpr MonotonicIdAllocator(value_type next_value = 1) noexcept
        : next_value_(next_value) {}

    [[nodiscard]] constexpr Result<Id, IdAllocationError> candidate() const noexcept {
        if (next_value_ == 0 || next_value_ == std::numeric_limits<value_type>::max()) {
            return unexpected(IdAllocationError::Exhausted);
        }
        return Id{next_value_};
    }

    [[nodiscard]] constexpr Result<void, IdAllocationError> commit(Id id) noexcept {
        if (!id.valid() || id.value() != next_value_) {
            return unexpected(IdAllocationError::CandidateMismatch);
        }
        ++next_value_;
        return {};
    }

    [[nodiscard]] constexpr value_type next_value() const noexcept { return next_value_; }

private:
    value_type next_value_{1};
};

template <typename Tag>
using PersistentId = StrongId<Tag, std::uint64_t>;

template <typename Tag>
using PersistentIdAllocator = MonotonicIdAllocator<PersistentId<Tag>>;

struct CommandIdTag;
struct JobIdTag;
struct EventSequenceTag;
struct TransactionIdTag;
struct ReadModelGenerationTag;

using CommandId = StrongId<CommandIdTag>;
using JobId = StrongId<JobIdTag>;
using EventSequence = StrongId<EventSequenceTag>;
using TransactionId = PersistentId<TransactionIdTag>;
using ReadModelGeneration = StrongId<ReadModelGenerationTag>;

} // namespace starforge::core
