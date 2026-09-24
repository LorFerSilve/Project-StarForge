#pragma once

#include "starforge/core/operation_ids.hpp"
#include "starforge/core/result.hpp"
#include "starforge/missions/missions.hpp"
#include "starforge/progression/finale_readiness.hpp"
#include "starforge/station/logistics_reservation_transaction.hpp"
#include "starforge/transactions/participant.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace starforge::progression {

enum class FinaleDeploymentError : std::uint8_t {
    ReadinessMissing,
    ReadinessInvalidated,
    AlreadyDeployed,
    ManifestReservationLost,
    MissionDeploymentRejected,
    ReadinessActivationRejected,
};

struct PreparedFinaleMissionDeployment final {
    station::PreparedLogisticsReservationGuard manifest_guard;
    missions::PreparedMissionDeployment mission;
    PreparedFinaleDeploymentActivation readiness;

    [[nodiscard]] missions::MissionInstanceId mission_instance_id() const noexcept {
        return mission.instance_id();
    }

    [[nodiscard]] std::array<transactions::IPreparedTransactionParticipant*, 3>
    participants() noexcept {
        return {&manifest_guard, &mission, &readiness};
    }
};

[[nodiscard]] core::Result<PreparedFinaleMissionDeployment, FinaleDeploymentError>
prepare_finale_mission_deployment(
    FinaleReadinessStore& readiness,
    station::LogisticsStore& logistics,
    missions::MissionRuntime& missions,
    const FinaleReadinessView& view,
    const std::vector<SupportProviderSelection>& support_providers,
    missions::MissionId mission_id,
    std::uint64_t deployment_tick,
    core::TransactionId transaction_id);

} // namespace starforge::progression
