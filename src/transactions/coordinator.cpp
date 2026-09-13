#include "starforge/transactions/coordinator.hpp"

#include <algorithm>

namespace starforge::transactions {

core::Result<core::TransactionId, TransactionError> TransactionCoordinator::candidate_transaction_id() const noexcept {
    const auto candidate = transaction_ids_.candidate();
    if (!candidate) {
        return core::Result<core::TransactionId, TransactionError>::failure(map_allocator_error(candidate.error()));
    }
    return core::Result<core::TransactionId, TransactionError>::success(candidate.value());
}

core::Result<void, TransactionError> TransactionCoordinator::prepare(
    core::TransactionId candidate,
    std::span<IPreparedTransactionParticipant* const> participants) {
    if (state_ != TransactionLifecycleState::Idle) {
        return core::Result<void, TransactionError>::failure(TransactionError::CoordinatorBusy);
    }
    if (participants.empty()) {
        return core::Result<void, TransactionError>::failure(TransactionError::NoParticipants);
    }

    const auto expected_id = candidate_transaction_id();
    if (!expected_id) {
        return core::Result<void, TransactionError>::failure(expected_id.error());
    }
    if (expected_id.value() != candidate || !candidate.valid()) {
        return core::Result<void, TransactionError>::failure(TransactionError::CandidateMismatch);
    }

    std::vector<IPreparedTransactionParticipant*> prepared(participants.begin(), participants.end());
    state_ = TransactionLifecycleState::Preparing;

    if (std::ranges::any_of(prepared, [](const auto* participant) { return participant == nullptr; })) {
        clear_prepared(TransactionLifecycleState::Aborted);
        return core::Result<void, TransactionError>::failure(TransactionError::NullParticipant);
    }

    std::ranges::sort(prepared, [](const auto* lhs, const auto* rhs) {
        return lhs->commit_key() < rhs->commit_key();
    });

    for (std::size_t index = 1; index < prepared.size(); ++index) {
        if (prepared[index - 1U]->commit_key() == prepared[index]->commit_key()) {
            clear_prepared(TransactionLifecycleState::Aborted);
            return core::Result<void, TransactionError>::failure(TransactionError::DuplicateCommitKey);
        }
    }

    for (const auto* participant : prepared) {
        if (participant->current_revision() != participant->expected_revision()) {
            clear_prepared(TransactionLifecycleState::Aborted);
            return core::Result<void, TransactionError>::failure(TransactionError::StaleRevision);
        }
    }

    prepared_transaction_id_ = candidate;
    prepared_participants_ = std::move(prepared);
    state_ = TransactionLifecycleState::Prepared;
    return core::Result<void, TransactionError>::success();
}

TransactionResult TransactionCoordinator::commit() noexcept {
    if (state_ != TransactionLifecycleState::Prepared) {
        return TransactionResult{{}, false, TransactionError::NothingPrepared, 0U};
    }

    for (const auto* participant : prepared_participants_) {
        if (participant->current_revision() != participant->expected_revision()) {
            const auto id = prepared_transaction_id_;
            const auto count = prepared_participants_.size();
            clear_prepared(TransactionLifecycleState::Aborted);
            return TransactionResult{id, false, TransactionError::StaleRevision, count};
        }
    }

    const auto committed_id = transaction_ids_.commit(prepared_transaction_id_);
    if (!committed_id) {
        const auto id = prepared_transaction_id_;
        const auto count = prepared_participants_.size();
        const auto mapped = map_allocator_error(committed_id.error());
        clear_prepared(TransactionLifecycleState::Aborted);
        return TransactionResult{id, false, mapped, count};
    }

    state_ = TransactionLifecycleState::Committing;
    for (auto* participant : prepared_participants_) {
        participant->commit();
    }

    state_ = TransactionLifecycleState::Committed;
    for (auto* participant : prepared_participants_) {
        participant->publish_committed_events();
    }

    const auto id = committed_id.value();
    const auto count = prepared_participants_.size();
    clear_prepared(TransactionLifecycleState::Committed);
    return TransactionResult{id, true, TransactionError::NothingPrepared, count};
}

core::Result<void, TransactionError> TransactionCoordinator::abort() noexcept {
    if (state_ != TransactionLifecycleState::Prepared && state_ != TransactionLifecycleState::Preparing) {
        return core::Result<void, TransactionError>::failure(TransactionError::NothingPrepared);
    }
    clear_prepared(TransactionLifecycleState::Aborted);
    return core::Result<void, TransactionError>::success();
}

void TransactionCoordinator::clear_prepared(TransactionLifecycleState terminal_state) noexcept {
    state_ = terminal_state;
    prepared_participants_.clear();
    prepared_transaction_id_ = core::TransactionId{};
    state_ = TransactionLifecycleState::Idle;
}

TransactionError TransactionCoordinator::map_allocator_error(core::IdAllocationError error) noexcept {
    switch (error) {
        case core::IdAllocationError::Exhausted:
            return TransactionError::AllocatorExhausted;
        case core::IdAllocationError::InvalidRestoredState:
        case core::IdAllocationError::CandidateMismatch:
            return TransactionError::CandidateMismatch;
    }
    return TransactionError::CandidateMismatch;
}

}  // namespace starforge::transactions
