#pragma once

#include "starforge/core/result.hpp"
#include "starforge/progression/progression.hpp"

#include <cstdint>
#include <set>
#include <string>
#include <vector>

namespace starforge::progression {

enum class CampaignPhase : std::uint8_t {
    RecoveryP0 = 0,
    LocalOperatorP1,
    FrontierNetworkP2,
    StrategicPowerP3,
    VergeExpeditionP4,
    PostgameP5,
};

enum class CampaignProgressionError : std::uint8_t {
    DuplicateTransaction,
    InvalidTransition,
    InvalidGate,
    MissingCapability,
    MissingStoryState,
    MissingTechnology,
    MissingBlueprint,
    InsufficientReputation,
    MissingEvidence,
    MissingFinaleCommit,
    InvalidSnapshot,
};

struct ReputationGate final {
    std::string faction;
    std::int32_t minimum{-100};
};

struct EvidenceGate final {
    std::string domain;
    std::uint32_t minimum{0};
};

// Cross-domain capabilities are observations only. Their owning systems remain authoritative;
// this view prevents campaign progression from duplicating ship/station/crew/robot/route state.
struct CapabilityView final {
    std::set<std::string> capabilities{};
};

struct CampaignGate final {
    std::set<std::string> required_capabilities{};
    std::set<std::string> required_campaign_flags{};
    std::set<std::string> required_technologies{};
    std::set<std::string> required_blueprints{};
    std::vector<ReputationGate> reputation{};
    std::vector<EvidenceGate> evidence{};
};

struct CampaignProgressionSnapshot final {
    CampaignPhase phase{CampaignPhase::RecoveryP0};
    std::set<std::uint64_t> committed_transactions{};
};

class CampaignProgression final {
public:
    [[nodiscard]] CampaignPhase phase() const noexcept { return phase_; }

    [[nodiscard]] core::Result<void, CampaignProgressionError> validate_advance(
        CampaignPhase target,
        const CampaignGate& gate,
        const CapabilityView& capabilities,
        const ProgressionState& progression) const;

    [[nodiscard]] core::Result<void, CampaignProgressionError> advance(
        CampaignPhase target,
        const CampaignGate& gate,
        const CapabilityView& capabilities,
        const ProgressionState& progression,
        std::uint64_t transaction_id);

    [[nodiscard]] CampaignProgressionSnapshot snapshot() const;
    [[nodiscard]] static core::Result<CampaignProgression, CampaignProgressionError> restore(
        CampaignProgressionSnapshot snapshot);

private:
    [[nodiscard]] bool transaction_seen(std::uint64_t transaction_id) const noexcept;

    CampaignPhase phase_{CampaignPhase::RecoveryP0};
    std::set<std::uint64_t> committed_transactions_{};
};

} // namespace starforge::progression
