#pragma once

#include "starforge/core/id_allocator.hpp"
#include "starforge/core/operation_ids.hpp"
#include "starforge/core/result.hpp"
#include "starforge/transactions/participant.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace starforge::transactions {

enum class TransactionLifecycleState : std::uint8_t {
    Idle,
    Preparing,
    Prepared,
    Committing,
    Committed,
    Aborted,
};

enum class TransactionError : std::uint8_t {
    CoordinatorBusy,
    NoParticipants,
    NullParticipant,
    DuplicateCommitKey,
    CandidateMismatch,
    AllocatorExhausted,
    StaleRevision,
    NothingPrepared,
};

struct TransactionResult final {
    core::TransactionId transaction_id{};
    bool committed{false};
    TransactionError error{TransactionError::NothingPrepared};
    std::size_t participant_count{0};
};

class TransactionCoordinator final {
public:
    TransactionCoordinator() = default;

    [[nodiscard]] core::Result<core::TransactionId, TransactionError> candidate_transaction_id() const noexcept;

    [[nodiscard]] core::Result<void, TransactionError> prepare(
        core::TransactionId candidate,
        std::span<IPreparedTransactionParticipant* const> participants);

    [[nodiscard]] TransactionResult commit() noexcept;
    [[nodiscard]] core::Result<void, TransactionError> abort() noexcept;

    [[nodiscard]] TransactionLifecycleState state() const noexcept { return state_; }
    [[nodiscard]] bool is_stable_save_boundary() const noexcept { return state_ == TransactionLifecycleState::Idle; }

private:
    void clear_prepared(TransactionLifecycleState terminal_state) noexcept;
    [[nodiscard]] static TransactionError map_allocator_error(core::IdAllocationError error) noexcept;

    core::MonotonicIdAllocator<core::TransactionId> transaction_ids_{};
    TransactionLifecycleState state_{TransactionLifecycleState::Idle};
    core::TransactionId prepared_transaction_id_{};
    std::vector<IPreparedTransactionParticipant*> prepared_participants_{};
};

}  // namespace starforge::transactions
