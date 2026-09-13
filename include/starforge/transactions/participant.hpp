#pragma once

#include "starforge/core/revision.hpp"

#include <compare>
#include <cstdint>

namespace starforge::transactions {

enum class DomainCommitOrder : std::uint16_t {
    Core = 10,
    Player = 20,
    Inventory = 30,
    Economy = 40,
    Station = 50,
    Crew = 60,
    Robots = 70,
    Spacecraft = 80,
    Missions = 90,
    Strategic = 100,
    Progression = 110,
};

struct DomainCommitKey final {
    DomainCommitOrder domain{DomainCommitOrder::Core};
    std::uint32_t stable_ordinal{0};

    friend constexpr auto operator<=>(const DomainCommitKey&, const DomainCommitKey&) noexcept = default;
};

class IPreparedTransactionParticipant {
public:
    virtual ~IPreparedTransactionParticipant() = default;

    [[nodiscard]] virtual DomainCommitKey commit_key() const noexcept = 0;
    [[nodiscard]] virtual core::StateRevision expected_revision() const noexcept = 0;
    [[nodiscard]] virtual core::StateRevision current_revision() const noexcept = 0;

    // Preparation has already completed. These operations must not allocate or fail.
    virtual void commit() noexcept = 0;
    virtual void publish_committed_events() noexcept = 0;
};

}  // namespace starforge::transactions
