#include "starforge/station/logistics_reservation_transaction.hpp"

#include <limits>
#include <set>
#include <utility>

namespace starforge::station {

PreparedLogisticsReservation::PreparedLogisticsReservation(
    LogisticsStore& store,
    ReservationMap prepared,
    core::StateRevision expected_revision) noexcept
    : store_(&store),
      prepared_(std::move(prepared)),
      expected_revision_(expected_revision) {}

core::Result<PreparedLogisticsReservation, LogisticsReservationError>
PreparedLogisticsReservation::prepare(
    LogisticsStore& store, std::vector<Reservation> reservations) {
    if (reservations.empty()) {
        return core::Result<PreparedLogisticsReservation, LogisticsReservationError>::failure(
            LogisticsReservationError::EmptyBatch);
    }
    if (!store.revision_.can_advance()) {
        return core::Result<PreparedLogisticsReservation, LogisticsReservationError>::failure(
            LogisticsReservationError::RevisionExhausted);
    }

    using ResourceKey = std::pair<StorageId, ResourceId>;
    std::map<ResourceKey, std::int64_t> requested{};
    ReservationMap prepared{};

    for (auto& reservation : reservations) {
        if (!reservation.id || !reservation.source || !reservation.resource ||
            reservation.quantity <= 0 || !store.inventory_.contains(reservation.source)) {
            return core::Result<PreparedLogisticsReservation, LogisticsReservationError>::failure(
                LogisticsReservationError::InvalidReservation);
        }
        if (store.reservations_.contains(reservation.id) ||
            prepared.contains(reservation.id)) {
            return core::Result<PreparedLogisticsReservation, LogisticsReservationError>::failure(
                LogisticsReservationError::DuplicateReservation);
        }

        const ResourceKey key{reservation.source, reservation.resource};
        auto& aggregate = requested[key];
        if (reservation.quantity > std::numeric_limits<std::int64_t>::max() - aggregate) {
            return core::Result<PreparedLogisticsReservation, LogisticsReservationError>::failure(
                LogisticsReservationError::Overflow);
        }
        aggregate += reservation.quantity;
        prepared.emplace(reservation.id, std::move(reservation));
    }

    for (const auto& [key, amount] : requested) {
        const auto [storage, resource] = key;
        const auto total = store.quantity(storage, resource);
        const auto already_reserved = store.reserved(storage, resource);
        if (already_reserved < 0 || total < already_reserved ||
            amount > total - already_reserved) {
            return core::Result<PreparedLogisticsReservation, LogisticsReservationError>::failure(
                LogisticsReservationError::InsufficientQuantity);
        }
    }

    return core::Result<PreparedLogisticsReservation, LogisticsReservationError>::success(
        PreparedLogisticsReservation{store, std::move(prepared), store.revision_});
}

transactions::DomainCommitKey PreparedLogisticsReservation::commit_key() const noexcept {
    return {.domain = transactions::DomainCommitOrder::Inventory, .stable_ordinal = 1U};
}

core::StateRevision PreparedLogisticsReservation::expected_revision() const noexcept {
    return expected_revision_;
}

core::StateRevision PreparedLogisticsReservation::current_revision() const noexcept {
    return store_->revision_;
}

void PreparedLogisticsReservation::commit() noexcept {
    store_->reservations_.merge(prepared_);
    static_cast<void>(store_->revision_.advance());
}

void PreparedLogisticsReservation::publish_committed_events() noexcept {}

PreparedLogisticsReservationGuard::PreparedLogisticsReservationGuard(
    LogisticsStore& store, core::StateRevision expected_revision) noexcept
    : store_(&store), expected_revision_(expected_revision) {}

core::Result<PreparedLogisticsReservationGuard, LogisticsReservationError>
PreparedLogisticsReservationGuard::prepare(
    LogisticsStore& store, std::vector<ReservationId> reservations) {
    if (reservations.empty()) {
        return core::Result<PreparedLogisticsReservationGuard, LogisticsReservationError>::failure(
            LogisticsReservationError::EmptyBatch);
    }

    std::set<ReservationId> unique;
    for (const auto reservation : reservations) {
        if (!reservation || !unique.insert(reservation).second) {
            return core::Result<PreparedLogisticsReservationGuard, LogisticsReservationError>::failure(
                LogisticsReservationError::InvalidReservation);
        }
        if (!store.has_reservation(reservation)) {
            return core::Result<PreparedLogisticsReservationGuard, LogisticsReservationError>::failure(
                LogisticsReservationError::InsufficientQuantity);
        }
    }

    return core::Result<PreparedLogisticsReservationGuard, LogisticsReservationError>::success(
        PreparedLogisticsReservationGuard{store, store.revision()});
}

transactions::DomainCommitKey PreparedLogisticsReservationGuard::commit_key() const noexcept {
    return {.domain = transactions::DomainCommitOrder::Inventory, .stable_ordinal = 2U};
}

core::StateRevision PreparedLogisticsReservationGuard::expected_revision() const noexcept {
    return expected_revision_;
}

core::StateRevision PreparedLogisticsReservationGuard::current_revision() const noexcept {
    return store_->revision();
}

void PreparedLogisticsReservationGuard::commit() noexcept {}

void PreparedLogisticsReservationGuard::publish_committed_events() noexcept {}

} // namespace starforge::station
