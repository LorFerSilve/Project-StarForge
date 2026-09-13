#include "starforge/transactions/transaction_coordinator.hpp"

#include <algorithm>

namespace starforge::transactions {

core::Result<CommitReceipt, TransactionError> TransactionCoordinator::commit(
    std::vector<PreparedOperation> operations) {
    if (operations.empty()) {
        return core::unexpected(TransactionError::NoOperations);
    }

    std::stable_sort(operations.begin(), operations.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.stable_order < rhs.stable_order;
    });

    for (const auto& operation : operations) {
        if (!operation.validate || !operation.commit || !operation.validate()) {
            return core::unexpected(TransactionError::ParticipantRejected);
        }
    }

    const auto candidate = allocator_.candidate();
    if (!candidate) {
        return core::unexpected(TransactionError::IdAllocationFailed);
    }
    if (!allocator_.commit(*candidate)) {
        return core::unexpected(TransactionError::IdAllocationFailed);
    }

    for (auto& operation : operations) {
        operation.commit();
    }

    return CommitReceipt{*candidate, operations.size()};
}

} // namespace starforge::transactions
