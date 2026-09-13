#pragma once

#include "starforge/core/domain.hpp"
#include "starforge/core/operation_ids.hpp"
#include "starforge/core/revision.hpp"
#include "starforge/core/simulation_time.hpp"

#include <cstdint>
#include <optional>

namespace starforge::simulation {

enum class CommandSourceKind : std::uint8_t {
    PlayerInput,
    Ui,
    Ai,
    Automation,
    Mission,
    Recovery,
    ScheduledTimer,
    DevelopmentTool,
};

struct CommandMetadata final {
    starforge::core::CommandId command_id{};
    starforge::core::SimulationTick originating_tick{};
    CommandSourceKind source{CommandSourceKind::DevelopmentTool};
    std::optional<starforge::core::TransactionId> correlation_transaction{};
    std::optional<starforge::core::StateRevision> expected_revision{};
    std::uint16_t domain_priority{0};
    std::uint32_t tie_breaker{0};
};

enum class ValidationDisposition : std::uint8_t {
    Accepted,
    Rejected,
    Deferred,
};

template <typename RejectionReason>
struct ValidationResult final {
    ValidationDisposition disposition{ValidationDisposition::Accepted};
    std::optional<RejectionReason> reason{};
};

struct EventMetadata final {
    starforge::core::EventSequence sequence{};
    starforge::core::SimulationTick commit_tick{};
    starforge::core::DomainId producing_domain{starforge::core::DomainId::Core};
    std::optional<starforge::core::TransactionId> transaction_id{};
};

}  // namespace starforge::simulation
