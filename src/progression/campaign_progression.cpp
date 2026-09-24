#include "starforge/progression/campaign_progression.hpp"

#include <utility>

namespace starforge::progression {

namespace {

[[nodiscard]] constexpr std::uint8_t phase_value(CampaignPhase phase) noexcept {
    return static_cast<std::uint8_t>(phase);
}

[[nodiscard]] bool valid_phase(CampaignPhase phase) noexcept {
    return phase_value(phase) <= phase_value(CampaignPhase::PostgameP5);
}

} // namespace

bool CampaignProgression::transaction_seen(std::uint64_t transaction_id) const noexcept {
    return transaction_id == 0 || committed_transactions_.contains(transaction_id);
}

core::Result<void, CampaignProgressionError> CampaignProgression::validate_advance(
    CampaignPhase target,
    const CampaignGate& gate,
    const CapabilityView& capabilities,
    const ProgressionState& progression) const {
    if (!valid_phase(phase_) || !valid_phase(target) ||
        phase_value(target) != static_cast<std::uint8_t>(phase_value(phase_) + 1U)) {
        return core::Result<void, CampaignProgressionError>::failure(
            CampaignProgressionError::InvalidTransition);
    }

    for (const auto& capability : gate.required_capabilities) {
        if (capability.empty()) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::InvalidGate);
        }
        if (!capabilities.capabilities.contains(capability)) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::MissingCapability);
        }
    }

    for (const auto& flag : gate.required_campaign_flags) {
        if (flag.empty()) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::InvalidGate);
        }
        if (!progression.campaign_flag(flag)) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::MissingStoryState);
        }
    }

    for (const auto& technology : gate.required_technologies) {
        if (technology.empty()) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::InvalidGate);
        }
        if (!progression.has_technology(technology)) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::MissingTechnology);
        }
    }

    for (const auto& blueprint : gate.required_blueprints) {
        if (blueprint.empty()) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::InvalidGate);
        }
        if (!progression.has_blueprint(blueprint)) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::MissingBlueprint);
        }
    }

    for (const auto& requirement : gate.reputation) {
        if (requirement.faction.empty() || requirement.minimum < -100 || requirement.minimum > 100) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::InvalidGate);
        }
        if (progression.reputation(requirement.faction) < requirement.minimum) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::InsufficientReputation);
        }
    }

    for (const auto& requirement : gate.evidence) {
        if (requirement.domain.empty()) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::InvalidGate);
        }
        if (progression.evidence(requirement.domain) < requirement.minimum) {
            return core::Result<void, CampaignProgressionError>::failure(
                CampaignProgressionError::MissingEvidence);
        }
    }

    if (target == CampaignPhase::PostgameP5 && !progression.finale_completed()) {
        return core::Result<void, CampaignProgressionError>::failure(
            CampaignProgressionError::MissingFinaleCommit);
    }

    return core::Result<void, CampaignProgressionError>::success();
}

core::Result<void, CampaignProgressionError> CampaignProgression::advance(
    CampaignPhase target,
    const CampaignGate& gate,
    const CapabilityView& capabilities,
    const ProgressionState& progression,
    std::uint64_t transaction_id) {
    if (transaction_seen(transaction_id)) {
        return core::Result<void, CampaignProgressionError>::failure(
            CampaignProgressionError::DuplicateTransaction);
    }

    const auto validation = validate_advance(target, gate, capabilities, progression);
    if (!validation) {
        return validation;
    }

    phase_ = target;
    committed_transactions_.insert(transaction_id);
    return core::Result<void, CampaignProgressionError>::success();
}

CampaignProgressionSnapshot CampaignProgression::snapshot() const {
    return CampaignProgressionSnapshot{
        .phase = phase_,
        .committed_transactions = committed_transactions_,
    };
}

core::Result<CampaignProgression, CampaignProgressionError> CampaignProgression::restore(
    CampaignProgressionSnapshot snapshot) {
    if (!valid_phase(snapshot.phase) || snapshot.committed_transactions.contains(0)) {
        return core::Result<CampaignProgression, CampaignProgressionError>::failure(
            CampaignProgressionError::InvalidSnapshot);
    }

    CampaignProgression restored;
    restored.phase_ = snapshot.phase;
    restored.committed_transactions_ = std::move(snapshot.committed_transactions);
    return core::Result<CampaignProgression, CampaignProgressionError>::success(
        std::move(restored));
}

} // namespace starforge::progression
