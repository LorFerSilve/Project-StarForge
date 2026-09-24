#include "starforge/economy/economy.hpp"

#include <limits>
#include <utility>

namespace starforge::economy {

PreparedCreditMutation::PreparedCreditMutation(
    EconomyState& state,
    std::int64_t next_credits,
    std::set<std::uint64_t> committed_transactions,
    core::StateRevision expected_revision) noexcept
    : state_(&state),
      next_credits_(next_credits),
      committed_transactions_(std::move(committed_transactions)),
      expected_revision_(expected_revision) {}

transactions::DomainCommitKey PreparedCreditMutation::commit_key() const noexcept {
    return {.domain = transactions::DomainCommitOrder::Economy, .stable_ordinal = 0U};
}

core::StateRevision PreparedCreditMutation::expected_revision() const noexcept {
    return expected_revision_;
}

core::StateRevision PreparedCreditMutation::current_revision() const noexcept {
    return state_->revision_;
}

void PreparedCreditMutation::commit() noexcept {
    state_->credits_ = next_credits_;
    state_->committed_transactions_.swap(committed_transactions_);
    static_cast<void>(state_->revision_.advance());
}

void PreparedCreditMutation::publish_committed_events() noexcept {}

bool EconomyState::transaction_seen(core::TransactionId transaction_id) const noexcept {
    return !transaction_id.valid() || committed_transactions_.contains(transaction_id.raw());
}

core::Result<PreparedCreditMutation, EconomyError> EconomyState::prepare_credit_delta(
    std::int64_t delta, core::TransactionId transaction_id) {
    if (delta == 0) {
        return core::Result<PreparedCreditMutation, EconomyError>::failure(
            EconomyError::InvalidAmount);
    }
    if (transaction_seen(transaction_id)) {
        return core::Result<PreparedCreditMutation, EconomyError>::failure(
            EconomyError::DuplicateTransaction);
    }
    if (!revision_.can_advance()) {
        return core::Result<PreparedCreditMutation, EconomyError>::failure(
            EconomyError::RevisionExhausted);
    }

    auto next_credits = credits_;
    if (delta > 0) {
        if (delta > std::numeric_limits<std::int64_t>::max() - credits_) {
            return core::Result<PreparedCreditMutation, EconomyError>::failure(
                EconomyError::InvalidAmount);
        }
        next_credits += delta;
    } else {
        if (delta == std::numeric_limits<std::int64_t>::min()) {
            return core::Result<PreparedCreditMutation, EconomyError>::failure(
                EconomyError::InsufficientCredits);
        }
        const auto debit = -delta;
        if (credits_ < debit) {
            return core::Result<PreparedCreditMutation, EconomyError>::failure(
                EconomyError::InsufficientCredits);
        }
        next_credits -= debit;
    }

    auto next_transactions = committed_transactions_;
    next_transactions.insert(transaction_id.raw());
    return core::Result<PreparedCreditMutation, EconomyError>::success(
        PreparedCreditMutation{*this, next_credits, std::move(next_transactions), revision_});
}

EconomySnapshot EconomyState::snapshot() const {
    return {
        .credits = credits_,
        .committed_transactions = committed_transactions_,
        .revision = revision_.raw(),
    };
}

core::Result<EconomyState, EconomyError> EconomyState::restore(EconomySnapshot snapshot) {
    if (snapshot.credits < 0 || snapshot.committed_transactions.contains(0U)) {
        return core::Result<EconomyState, EconomyError>::failure(EconomyError::InvalidSnapshot);
    }

    EconomyState state;
    state.credits_ = snapshot.credits;
    state.committed_transactions_ = std::move(snapshot.committed_transactions);
    state.revision_ = core::StateRevision{snapshot.revision};
    return core::Result<EconomyState, EconomyError>::success(std::move(state));
}

} // namespace starforge::economy
