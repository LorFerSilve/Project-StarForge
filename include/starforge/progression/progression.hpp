#pragma once

#include "starforge/core/result.hpp"

#include <algorithm>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace starforge::progression {

enum class ProgressionError : std::uint8_t {
    InvalidAmount,
    InsufficientCredits,
    DuplicateTransaction,
    InvalidReputation,
    MissingPrerequisite,
    MissingEvidence,
    AlreadyCompleted,
    InvalidFinaleState,
    InvalidSnapshot,
};

enum class ReputationTier : std::uint8_t {
    Enemy,
    Hostile,
    Distrusted,
    Neutral,
    Accepted,
    Trusted,
    Allied,
};

[[nodiscard]] constexpr ReputationTier reputation_tier(std::int32_t value) noexcept {
    if (value <= -61) return ReputationTier::Enemy;
    if (value <= -31) return ReputationTier::Hostile;
    if (value <= -11) return ReputationTier::Distrusted;
    if (value <= 10) return ReputationTier::Neutral;
    if (value <= 30) return ReputationTier::Accepted;
    if (value <= 60) return ReputationTier::Trusted;
    return ReputationTier::Allied;
}

struct ResearchRequirement final {
    std::string domain;
    std::uint32_t minimum{0};
};

struct ResearchProject final {
    std::string id;
    std::vector<std::string> prerequisites;
    std::vector<ResearchRequirement> evidence_thresholds;
    std::vector<std::string> unique_evidence;
    std::vector<std::string> technology_outputs;
    std::vector<std::string> blueprint_outputs;
};

// Persistence-facing value snapshot. Runtime-only state is deliberately absent; this is the
// authoritative progression payload embedded by the owning save DTO at a Stable Save Boundary.
struct ProgressionSnapshot final {
    std::int64_t credits{0};
    std::map<std::string, std::int32_t> reputation{};
    std::map<std::string, std::uint32_t> evidence_totals{};
    std::set<std::string> evidence_ids{};
    std::set<std::string> technologies{};
    std::set<std::string> blueprints{};
    std::set<std::string> completed_research{};
    std::set<std::string> campaign_flags{};
    std::set<std::uint64_t> committed_transactions{};
    bool finale_completed{false};
};

class ProgressionState final {
public:
    [[nodiscard]] std::int64_t credits() const noexcept { return credits_; }
    [[nodiscard]] core::Result<void, ProgressionError> credit(std::int64_t amount,
                                                               std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, ProgressionError> debit(std::int64_t amount,
                                                              std::uint64_t transaction_id);

    [[nodiscard]] std::int32_t reputation(std::string_view faction) const noexcept;
    [[nodiscard]] ReputationTier tier(std::string_view faction) const noexcept;
    [[nodiscard]] core::Result<void, ProgressionError> change_reputation(
        std::string faction, std::int32_t delta, std::uint64_t transaction_id);

    void integrate_evidence(std::string evidence_id,
                            const std::map<std::string, std::uint32_t>& values);
    [[nodiscard]] std::uint32_t evidence(std::string_view domain) const noexcept;
    [[nodiscard]] bool has_evidence(std::string_view evidence_id) const noexcept;
    [[nodiscard]] bool has_technology(std::string_view technology_id) const noexcept;
    [[nodiscard]] bool has_blueprint(std::string_view blueprint_id) const noexcept;
    [[nodiscard]] core::Result<void, ProgressionError> complete_research(
        const ResearchProject& project, std::uint64_t transaction_id);

    [[nodiscard]] bool campaign_flag(std::string_view flag) const noexcept;
    void set_campaign_flag(std::string flag);
    [[nodiscard]] core::Result<void, ProgressionError> commit_finale(
        std::string ending_flag, std::uint64_t transaction_id);
    [[nodiscard]] bool finale_completed() const noexcept { return finale_completed_; }

    [[nodiscard]] ProgressionSnapshot snapshot() const;
    [[nodiscard]] static core::Result<ProgressionState, ProgressionError> restore(
        ProgressionSnapshot snapshot);

private:
    [[nodiscard]] bool transaction_seen(std::uint64_t transaction_id) const noexcept;
    void commit_transaction(std::uint64_t transaction_id);

    std::int64_t credits_{0};
    std::map<std::string, std::int32_t> reputation_{};
    std::map<std::string, std::uint32_t> evidence_totals_{};
    std::set<std::string> evidence_ids_{};
    std::set<std::string> technologies_{};
    std::set<std::string> blueprints_{};
    std::set<std::string> completed_research_{};
    std::set<std::string> campaign_flags_{};
    std::set<std::uint64_t> committed_transactions_{};
    bool finale_completed_{false};
};

} // namespace starforge::progression
