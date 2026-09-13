#include "starforge/transactions/transaction_coordinator.hpp"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <vector>

TEST_CASE("rejected transaction performs no partial mutation and consumes no id") {
    starforge::transactions::TransactionCoordinator coordinator;
    int left = 10;
    int right = 20;

    std::vector<starforge::transactions::PreparedOperation> operations;
    operations.push_back({20, [] { return true; }, [&] { left = 11; }});
    operations.push_back({10, [] { return false; }, [&] { right = 21; }});

    const auto result = coordinator.commit(std::move(operations));
    REQUIRE_FALSE(result);
    REQUIRE(result.error() == starforge::transactions::TransactionError::ParticipantRejected);
    REQUIRE(left == 10);
    REQUIRE(right == 20);
    REQUIRE(coordinator.next_transaction_id() == 1);
}

TEST_CASE("prepared transaction commits in deterministic stable order") {
    constexpr std::array<std::array<int, 3>, 3> permutations{{
        {{30, 10, 20}},
        {{20, 30, 10}},
        {{10, 20, 30}},
    }};

    for (const auto& permutation : permutations) {
        starforge::transactions::TransactionCoordinator coordinator;
        std::vector<int> order;
        std::vector<starforge::transactions::PreparedOperation> operations;

        for (const auto key : permutation) {
            operations.push_back({
                static_cast<std::uint32_t>(key),
                [] { return true; },
                [&, key] { order.push_back(key); }});
        }

        const auto result = coordinator.commit(std::move(operations));
        REQUIRE(result);
        REQUIRE(result->transaction_id.value() == 1);
        REQUIRE(result->committed_operations == 3);
        REQUIRE(order == std::vector<int>{10, 20, 30});
        REQUIRE(coordinator.next_transaction_id() == 2);
    }
}

TEST_CASE("ambiguous transaction order key is rejected before mutation") {
    starforge::transactions::TransactionCoordinator coordinator;
    int mutations = 0;
    std::vector<starforge::transactions::PreparedOperation> operations;
    operations.push_back({10, [] { return true; }, [&] { ++mutations; }});
    operations.push_back({10, [] { return true; }, [&] { ++mutations; }});

    const auto result = coordinator.commit(std::move(operations));
    REQUIRE_FALSE(result);
    REQUIRE(result.error() == starforge::transactions::TransactionError::DuplicateOrderKey);
    REQUIRE(mutations == 0);
    REQUIRE(coordinator.next_transaction_id() == 1);
}
