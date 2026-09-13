#include "starforge/transactions/coordinator.hpp"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <vector>

namespace {

class FakeParticipant final : public starforge::transactions::IPreparedTransactionParticipant {
public:
    FakeParticipant(
        int& value,
        starforge::core::StateRevision& revision,
        int delta,
        starforge::transactions::DomainCommitKey key,
        std::vector<int>& trace,
        int trace_code,
        const int* peer_value = nullptr,
        bool* saw_peer_committed = nullptr) noexcept
        : value_(&value),
          revision_(&revision),
          expected_(revision),
          delta_(delta),
          key_(key),
          trace_(&trace),
          trace_code_(trace_code),
          peer_value_(peer_value),
          saw_peer_committed_(saw_peer_committed) {}

    [[nodiscard]] starforge::transactions::DomainCommitKey commit_key() const noexcept override { return key_; }
    [[nodiscard]] starforge::core::StateRevision expected_revision() const noexcept override { return expected_; }
    [[nodiscard]] starforge::core::StateRevision current_revision() const noexcept override { return *revision_; }

    void commit() noexcept override {
        *value_ += delta_;
        static_cast<void>(revision_->advance());
        trace_->push_back(trace_code_);
    }

    void publish_committed_events() noexcept override {
        if (peer_value_ != nullptr && saw_peer_committed_ != nullptr) {
            *saw_peer_committed_ = *peer_value_ != 0;
        }
        trace_->push_back(trace_code_ + 100);
    }

private:
    int* value_;
    starforge::core::StateRevision* revision_;
    starforge::core::StateRevision expected_;
    int delta_;
    starforge::transactions::DomainCommitKey key_;
    std::vector<int>* trace_;
    int trace_code_;
    const int* peer_value_;
    bool* saw_peer_committed_;
};

}  // namespace

TEST_CASE("Transactions.Commit.DeterministicOrderAndPostCommitEvents", "[domain][transactions][determinism]") {
    starforge::transactions::TransactionCoordinator coordinator;
    int player_value = 0;
    int economy_value = 0;
    starforge::core::StateRevision player_revision;
    starforge::core::StateRevision economy_revision;
    std::vector<int> trace;
    trace.reserve(4U);
    bool player_event_saw_economy_commit = false;

    FakeParticipant economy{economy_value, economy_revision, 20, {starforge::transactions::DomainCommitOrder::Economy, 0U}, trace, 2};
    FakeParticipant player{player_value, player_revision, 10, {starforge::transactions::DomainCommitOrder::Player, 0U}, trace, 1, &economy_value, &player_event_saw_economy_commit};

    std::array<starforge::transactions::IPreparedTransactionParticipant*, 2> participants{&economy, &player};
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    REQUIRE_FALSE(coordinator.is_stable_save_boundary());

    const auto result = coordinator.commit();
    REQUIRE(result.committed);
    REQUIRE(result.transaction_id == starforge::core::TransactionId{1U});
    REQUIRE(result.participant_count == 2U);
    REQUIRE(player_value == 10);
    REQUIRE(economy_value == 20);
    REQUIRE(player_revision == starforge::core::StateRevision{1U});
    REQUIRE(economy_revision == starforge::core::StateRevision{1U});
    REQUIRE(player_event_saw_economy_commit);
    REQUIRE(trace == std::vector<int>{1, 2, 101, 102});
    REQUIRE(coordinator.is_stable_save_boundary());
}

TEST_CASE("Transactions.Commit.StaleRevisionAbortsBeforeMutationAndDoesNotConsumeId", "[domain][transactions][determinism]") {
    starforge::transactions::TransactionCoordinator coordinator;
    int value = 0;
    starforge::core::StateRevision revision;
    std::vector<int> trace;
    FakeParticipant participant{value, revision, 5, {starforge::transactions::DomainCommitOrder::Player, 0U}, trace, 1};

    std::array<starforge::transactions::IPreparedTransactionParticipant*, 1> participants{&participant};
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);
    REQUIRE(coordinator.prepare(candidate.value(), participants));

    REQUIRE(revision.advance());
    const auto result = coordinator.commit();
    REQUIRE_FALSE(result.committed);
    REQUIRE(result.error == starforge::transactions::TransactionError::StaleRevision);
    REQUIRE(value == 0);
    REQUIRE(trace.empty());
    REQUIRE(coordinator.is_stable_save_boundary());

    const auto same_candidate = coordinator.candidate_transaction_id();
    REQUIRE(same_candidate);
    REQUIRE(same_candidate.value() == candidate.value());
}

TEST_CASE("Transactions.Prepare.RejectionLeavesCoordinatorStable", "[domain][transactions][determinism]") {
    starforge::transactions::TransactionCoordinator coordinator;
    int first = 0;
    int second = 0;
    starforge::core::StateRevision first_revision;
    starforge::core::StateRevision second_revision;
    std::vector<int> trace;

    FakeParticipant a{first, first_revision, 1, {starforge::transactions::DomainCommitOrder::Player, 0U}, trace, 1};
    FakeParticipant b{second, second_revision, 1, {starforge::transactions::DomainCommitOrder::Player, 0U}, trace, 2};
    std::array<starforge::transactions::IPreparedTransactionParticipant*, 2> duplicates{&a, &b};

    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);
    const auto prepared = coordinator.prepare(candidate.value(), duplicates);
    REQUIRE_FALSE(prepared);
    REQUIRE(prepared.error() == starforge::transactions::TransactionError::DuplicateCommitKey);
    REQUIRE(coordinator.is_stable_save_boundary());
    REQUIRE(first == 0);
    REQUIRE(second == 0);
}

TEST_CASE("Transactions.Prepare.OnlyOnePreparedWindowCanExist", "[domain][transactions]") {
    starforge::transactions::TransactionCoordinator coordinator;
    int value = 0;
    starforge::core::StateRevision revision;
    std::vector<int> trace;
    FakeParticipant participant{value, revision, 1, {starforge::transactions::DomainCommitOrder::Player, 0U}, trace, 1};
    std::array<starforge::transactions::IPreparedTransactionParticipant*, 1> participants{&participant};

    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);
    REQUIRE(coordinator.prepare(candidate.value(), participants));

    const auto second_prepare = coordinator.prepare(candidate.value(), participants);
    REQUIRE_FALSE(second_prepare);
    REQUIRE(second_prepare.error() == starforge::transactions::TransactionError::CoordinatorBusy);
    REQUIRE(coordinator.abort());
    REQUIRE(coordinator.is_stable_save_boundary());
}
