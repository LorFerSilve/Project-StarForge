#pragma once

#include "starforge/core/operation_ids.hpp"
#include "starforge/core/result.hpp"
#include "starforge/core/revision.hpp"
#include "starforge/transactions/participant.hpp"

#include <cstdint>
#include <set>

namespace starforge::economy {

class EconomyState;
class Market;
class PreparedMarketTrade;

enum class EconomyError : std::uint8_t {
    InvalidAmount,
    InsufficientCredits,
    DuplicateTransaction,
    RevisionExhausted,
    InvalidSnapshot,
};

struct EconomySnapshot final {
    std::int64_t credits{0};
    std::set<std::uint64_t> committed_transactions{};
    std::uint64_t revision{0};
};

class PreparedCreditMutation final : public transactions::IPreparedTransactionParticipant {
public:
    PreparedCreditMutation(PreparedCreditMutation&&) noexcept = default;
    PreparedCreditMutation& operator=(PreparedCreditMutation&&) noexcept = default;
    PreparedCreditMutation(const PreparedCreditMutation&) = delete;
    PreparedCreditMutation& operator=(const PreparedCreditMutation&) = delete;

    [[nodiscard]] transactions::DomainCommitKey commit_key() const noexcept override;
    [[nodiscard]] core::StateRevision expected_revision() const noexcept override;
    [[nodiscard]] core::StateRevision current_revision() const noexcept override;
    void commit() noexcept override;
    void publish_committed_events() noexcept override;

private:
    friend class EconomyState;

    PreparedCreditMutation(EconomyState& state,
                           std::int64_t next_credits,
                           std::set<std::uint64_t> committed_transactions,
                           core::StateRevision expected_revision) noexcept;

    EconomyState* state_{nullptr};
    std::int64_t next_credits_{0};
    std::set<std::uint64_t> committed_transactions_{};
    core::StateRevision expected_revision_{};
};

class EconomyState final {
public:
    [[nodiscard]] std::int64_t credits() const noexcept { return credits_; }
    [[nodiscard]] core::StateRevision revision() const noexcept { return revision_; }

    [[nodiscard]] core::Result<PreparedCreditMutation, EconomyError> prepare_credit_delta(
        std::int64_t delta, core::TransactionId transaction_id);

    [[nodiscard]] EconomySnapshot snapshot() const;
    [[nodiscard]] static core::Result<EconomyState, EconomyError> restore(EconomySnapshot snapshot);

private:
    friend class PreparedCreditMutation;
    friend class Market;
    friend class PreparedMarketTrade;

    [[nodiscard]] bool transaction_seen(core::TransactionId transaction_id) const noexcept;

    std::int64_t credits_{0};
    std::set<std::uint64_t> committed_transactions_{};
    core::StateRevision revision_{};
};

} // namespace starforge::economy
