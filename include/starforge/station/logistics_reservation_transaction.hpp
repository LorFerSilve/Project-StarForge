#pragma once

#include "starforge/core/result.hpp"
#include "starforge/station/station.hpp"
#include "starforge/transactions/participant.hpp"

#include <cstdint>
#include <map>
#include <vector>

namespace starforge::station {

enum class LogisticsReservationError : std::uint8_t {
    EmptyBatch,
    InvalidReservation,
    DuplicateReservation,
    InsufficientQuantity,
    Overflow,
    RevisionExhausted,
};

class PreparedLogisticsReservation final : public transactions::IPreparedTransactionParticipant {
public:
    PreparedLogisticsReservation(PreparedLogisticsReservation&&) noexcept = default;
    PreparedLogisticsReservation& operator=(PreparedLogisticsReservation&&) noexcept = default;
    PreparedLogisticsReservation(const PreparedLogisticsReservation&) = delete;
    PreparedLogisticsReservation& operator=(const PreparedLogisticsReservation&) = delete;

    [[nodiscard]] static core::Result<PreparedLogisticsReservation, LogisticsReservationError>
    prepare(LogisticsStore& store, std::vector<Reservation> reservations);

    [[nodiscard]] transactions::DomainCommitKey commit_key() const noexcept override;
    [[nodiscard]] core::StateRevision expected_revision() const noexcept override;
    [[nodiscard]] core::StateRevision current_revision() const noexcept override;
    void commit() noexcept override;
    void publish_committed_events() noexcept override;

private:
    using ReservationMap = std::map<ReservationId, Reservation>;

    PreparedLogisticsReservation(LogisticsStore& store,
                                 ReservationMap prepared,
                                 core::StateRevision expected_revision) noexcept;

    LogisticsStore* store_{nullptr};
    ReservationMap prepared_{};
    core::StateRevision expected_revision_{};
};

} // namespace starforge::station
