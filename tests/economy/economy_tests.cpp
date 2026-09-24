#include "starforge/economy/economy.hpp"
#include "starforge/transactions/coordinator.hpp"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <limits>
#include <utility>

using namespace starforge::economy;

namespace {

EconomyState restore_balance(std::int64_t credits) {
    auto restored = EconomyState::restore(EconomySnapshot{.credits = credits});
    REQUIRE(restored);
    return std::move(restored).value();
}

} // namespace

TEST_CASE("credit ledger mutations are prepared and committed exactly once") {
    EconomyState state;
    starforge::transactions::TransactionCoordinator coordinator;

    auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);
    auto credit = state.prepare_credit_delta(500, candidate.value());
    REQUIRE(credit);

    std::array<starforge::transactions::IPreparedTransactionParticipant*, 1> participants{
        &credit.value()};
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    REQUIRE(coordinator.commit().committed);
    CHECK(state.credits() == 500);

    CHECK_FALSE(state.prepare_credit_delta(1, starforge::core::TransactionId{1U}));

    candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);
    auto debit = state.prepare_credit_delta(-125, candidate.value());
    REQUIRE(debit);
    participants[0] = &debit.value();
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    REQUIRE(coordinator.commit().committed);
    CHECK(state.credits() == 375);
}

TEST_CASE("credit overflow rejects preparation without consuming coordinator id") {
    auto state = restore_balance(std::numeric_limits<std::int64_t>::max());
    starforge::transactions::TransactionCoordinator coordinator;

    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);
    CHECK_FALSE(state.prepare_credit_delta(1, candidate.value()));

    const auto same_candidate = coordinator.candidate_transaction_id();
    REQUIRE(same_candidate);
    CHECK(same_candidate.value() == candidate.value());

    auto debit = state.prepare_credit_delta(
        -std::numeric_limits<std::int64_t>::max(), candidate.value());
    REQUIRE(debit);
    std::array<starforge::transactions::IPreparedTransactionParticipant*, 1> participants{
        &debit.value()};
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    REQUIRE(coordinator.commit().committed);
    CHECK(state.credits() == 0);
}

TEST_CASE("economy snapshot preserves ledger idempotency and revision") {
    EconomyState state;
    starforge::transactions::TransactionCoordinator coordinator;
    const auto candidate = coordinator.candidate_transaction_id();
    REQUIRE(candidate);
    auto mutation = state.prepare_credit_delta(250, candidate.value());
    REQUIRE(mutation);
    std::array<starforge::transactions::IPreparedTransactionParticipant*, 1> participants{
        &mutation.value()};
    REQUIRE(coordinator.prepare(candidate.value(), participants));
    REQUIRE(coordinator.commit().committed);

    const auto snapshot = state.snapshot();
    auto restored = EconomyState::restore(snapshot);
    REQUIRE(restored);
    CHECK(restored.value().credits() == 250);
    CHECK(restored.value().revision() == state.revision());
    CHECK_FALSE(restored.value().prepare_credit_delta(
        1, starforge::core::TransactionId{1U}));
}
