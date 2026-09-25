#pragma once

#include "starforge/core/result.hpp"
#include "starforge/station/station.hpp"
#include "starforge/transactions/participant.hpp"

#include <cstdint>
#include <map>

namespace starforge::station {

enum class LogisticsMutationError : std::uint8_t {
    InvalidStorage,
    InvalidResource,
    InvalidQuantity,
    InsufficientQuantity,
    Overflow,
    RevisionExhausted,
};

class PreparedLogisticsMutation final : public transactions::IPreparedTransactionParticipant {
public:
    PreparedLogisticsMutation(PreparedLogisticsMutation&&) noexcept = default;
    PreparedLogisticsMutation& operator=(PreparedLogisticsMutation&&) noexcept = default;
    PreparedLogisticsMutation(const PreparedLogisticsMutation&) = delete;
    PreparedLogisticsMutation& operator=(const PreparedLogisticsMutation&) = delete;

    [[nodiscard]] static core::Result<PreparedLogisticsMutation, LogisticsMutationError>
    prepare_deposit(LogisticsStore& store, StorageId storage, ResourceId resource,
                    std::int64_t quantity);

    [[nodiscard]] static core::Result<PreparedLogisticsMutation, LogisticsMutationError>
    prepare_withdraw(LogisticsStore& store, StorageId storage, ResourceId resource,
                     std::int64_t quantity);

    [[nodiscard]] transactions::DomainCommitKey commit_key() const noexcept override;
    [[nodiscard]] core::StateRevision expected_revision() const noexcept override;
    [[nodiscard]] core::StateRevision current_revision() const noexcept override;
    void commit() noexcept override;
    void publish_committed_events() noexcept override;

private:
    using ResourceMap = std::map<ResourceId, std::int64_t>;

    PreparedLogisticsMutation(LogisticsStore& store,
                              StorageId storage,
                              ResourceId resource,
                              std::int64_t next_quantity,
                              core::StateRevision expected_revision,
                              ResourceMap::node_type prepared_node) noexcept;

    LogisticsStore* store_{nullptr};
    StorageId storage_{};
    ResourceId resource_{};
    std::int64_t next_quantity_{0};
    core::StateRevision expected_revision_{};
    ResourceMap::node_type prepared_node_{};
};

} // namespace starforge::station
