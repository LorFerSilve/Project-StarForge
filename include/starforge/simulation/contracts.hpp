#pragma once

#include "starforge/core/revision.hpp"
#include "starforge/core/strong_id.hpp"
#include "starforge/simulation/simulation_clock.hpp"

#include <optional>

namespace starforge::simulation {

template <typename Payload, typename RevisionTag>
struct CommandEnvelope final {
    core::CommandId command_id{};
    SimulationTick originating_tick{};
    std::optional<core::TransactionId> transaction_id{};
    std::optional<core::Revision<RevisionTag>> expected_revision{};
    Payload payload;
};

enum class ValidationDisposition : unsigned char {
    Accepted,
    Rejected,
    Deferred,
};

template <typename Rejection>
struct ValidationResult final {
    ValidationDisposition disposition{ValidationDisposition::Accepted};
    std::optional<Rejection> rejection{};
};

template <typename Payload, typename Rejection>
struct CommandResultEnvelope final {
    core::CommandId command_id{};
    std::optional<core::TransactionId> transaction_id{};
    ValidationDisposition disposition{ValidationDisposition::Accepted};
    std::optional<Rejection> rejection{};
    Payload payload;
};

template <typename Payload>
struct EventEnvelope final {
    core::EventSequence sequence{};
    SimulationTick commit_tick{};
    std::optional<core::TransactionId> transaction_id{};
    Payload payload;
};

} // namespace starforge::simulation
