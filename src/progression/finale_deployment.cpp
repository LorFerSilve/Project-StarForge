#include "starforge/progression/finale_deployment.hpp"

#include <utility>

namespace starforge::progression {

core::Result<PreparedFinaleMissionDeployment, FinaleDeploymentError>
prepare_finale_mission_deployment(
    FinaleReadinessStore& readiness,
    station::LogisticsStore& logistics,
    missions::MissionRuntime& mission_runtime,
    const FinaleReadinessView& view,
    const std::vector<SupportProviderSelection>& support_providers,
    missions::MissionId mission_id,
    std::uint64_t deployment_tick,
    core::TransactionId transaction_id) {
    if (!readiness.current().has_value()) {
        return core::Result<PreparedFinaleMissionDeployment, FinaleDeploymentError>::failure(
            FinaleDeploymentError::ReadinessMissing);
    }
    if (readiness.current()->status == FinaleReadinessStatus::Deployed) {
        return core::Result<PreparedFinaleMissionDeployment, FinaleDeploymentError>::failure(
            FinaleDeploymentError::AlreadyDeployed);
    }

    const auto report = readiness.refresh_status(view, support_providers, logistics);
    if (!report.ready() || !readiness.current().has_value() ||
        readiness.current()->status != FinaleReadinessStatus::Deployable) {
        return core::Result<PreparedFinaleMissionDeployment, FinaleDeploymentError>::failure(
            FinaleDeploymentError::ReadinessInvalidated);
    }

    auto manifest_guard = station::PreparedLogisticsReservationGuard::prepare(
        logistics, readiness.current()->manifest_reservations);
    if (!manifest_guard) {
        return core::Result<PreparedFinaleMissionDeployment, FinaleDeploymentError>::failure(
            FinaleDeploymentError::ManifestReservationLost);
    }

    auto mission = mission_runtime.prepare_deployment(
        mission_id, deployment_tick, transaction_id);
    if (!mission) {
        return core::Result<PreparedFinaleMissionDeployment, FinaleDeploymentError>::failure(
            FinaleDeploymentError::MissionDeploymentRejected);
    }

    const auto mission_instance_id = mission.value().instance_id();
    auto activation = readiness.prepare_deployment_activation(
        transaction_id, mission_instance_id.raw());
    if (!activation) {
        return core::Result<PreparedFinaleMissionDeployment, FinaleDeploymentError>::failure(
            FinaleDeploymentError::ReadinessActivationRejected);
    }

    return core::Result<PreparedFinaleMissionDeployment, FinaleDeploymentError>::success(
        PreparedFinaleMissionDeployment{
            .manifest_guard = std::move(manifest_guard).value(),
            .mission = std::move(mission).value(),
            .readiness = std::move(activation).value(),
        });
}

} // namespace starforge::progression
