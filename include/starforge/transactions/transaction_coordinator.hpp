#pragma once

#include "starforge/core/result.hpp"
#include "starforge/core/strong_id.hpp"

#include <cstdint>
#include <functional>
#include <vector>

namespace starforge::transactions {

enum class TransactionError : std::uint8_t {
    NoOperations,
    ParticipantRejected,
    IdAllocationFailed,
};

struct PreparedOperation final {
    std::uint32_t stable_order{0};
    std::function<bool()> validate;
    std::function<void()> commit;
};

struct CommitReceipt final {
    core::TransactionId transaction_id{};
    std::size_t committed_operations{0};
};

class TransactionCoordinator final {
public:
    explicit TransactionCoordinator(std::uint64_t next_transaction_id = 1) noexcept
        : allocator_(next_transaction_id) {}

    [[nodiscard]] core::Result<CommitReceipt, TransactionError> commit(
        std::vector<PreparedOperation> operations);

    [[nodiscard]] std::uint64_t next_transaction_id() const noexcept {
        return allocator_.next_value();
    }

private:
    core::PersistentIdAllocator<core::TransactionIdTag> allocator_;
};

} // namespace starforge::transactions
