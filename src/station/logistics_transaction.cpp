#include "starforge/station/logistics_transaction.hpp"

#include <limits>
#include <utility>

namespace starforge::station {

PreparedLogisticsMutation::PreparedLogisticsMutation(
    LogisticsStore& store,
    StorageId storage,
    ResourceId resource,
    std::int64_t next_quantity,
    core::StateRevision expected_revision,
    ResourceMap::node_type prepared_node) noexcept
    : store_(&store),
      storage_(storage),
      resource_(resource),
      next_quantity_(next_quantity),
      expected_revision_(expected_revision),
      prepared_node_(std::move(prepared_node)) {}

core::Result<PreparedLogisticsMutation, LogisticsMutationError>
PreparedLogisticsMutation::prepare_deposit(
    LogisticsStore& store, StorageId storage, ResourceId resource, std::int64_t quantity) {
    if (!storage || !store.inventory_.contains(storage)) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::InvalidStorage);
    }
    if (!resource) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::InvalidResource);
    }
    if (quantity <= 0) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::InvalidQuantity);
    }
    if (!store.revision_.can_advance()) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::RevisionExhausted);
    }

    auto& resources = store.inventory_.at(storage);
    const auto existing = resources.find(resource);
    const auto current = existing == resources.end() ? 0 : existing->second;
    if (quantity > std::numeric_limits<std::int64_t>::max() - current) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::Overflow);
    }
    const auto next = current + quantity;

    ResourceMap::node_type prepared_node;
    if (existing == resources.end()) {
        ResourceMap staging;
        staging.emplace(resource, next);
        prepared_node = staging.extract(resource);
    }

    return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::success(
        PreparedLogisticsMutation{
            store, storage, resource, next, store.revision_, std::move(prepared_node)});
}

core::Result<PreparedLogisticsMutation, LogisticsMutationError>
PreparedLogisticsMutation::prepare_withdraw(
    LogisticsStore& store, StorageId storage, ResourceId resource, std::int64_t quantity) {
    if (!storage || !store.inventory_.contains(storage)) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::InvalidStorage);
    }
    if (!resource) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::InvalidResource);
    }
    if (quantity <= 0) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::InvalidQuantity);
    }
    if (!store.revision_.can_advance()) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::RevisionExhausted);
    }

    const auto current = store.quantity(storage, resource);
    const auto available = current - store.reserved(storage, resource);
    if (available < quantity) {
        return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::failure(
            LogisticsMutationError::InsufficientQuantity);
    }

    return core::Result<PreparedLogisticsMutation, LogisticsMutationError>::success(
        PreparedLogisticsMutation{
            store, storage, resource, current - quantity, store.revision_, {}});
}

transactions::DomainCommitKey PreparedLogisticsMutation::commit_key() const noexcept {
    return {.domain = transactions::DomainCommitOrder::Inventory, .stable_ordinal = 0U};
}

core::StateRevision PreparedLogisticsMutation::expected_revision() const noexcept {
    return expected_revision_;
}

core::StateRevision PreparedLogisticsMutation::current_revision() const noexcept {
    return store_->revision_;
}

void PreparedLogisticsMutation::commit() noexcept {
    auto& resources = store_->inventory_.find(storage_)->second;
    const auto existing = resources.find(resource_);
    if (existing != resources.end()) {
        existing->second = next_quantity_;
    } else {
        static_cast<void>(resources.insert(std::move(prepared_node_)));
    }
    static_cast<void>(store_->revision_.advance());
}

void PreparedLogisticsMutation::publish_committed_events() noexcept {}

} // namespace starforge::station
