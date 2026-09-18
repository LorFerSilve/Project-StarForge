#pragma once

#include "starforge/core/result.hpp"
#include "starforge/core/simulation_time.hpp"

#include <algorithm>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace starforge::progression {

enum class ProgressionError : std::uint8_t {
    InvalidAmount, InsufficientCredits, DuplicateTransaction, InvalidReputation,
    MissingPrerequisite, MissingEvidence, AlreadyCompleted, InvalidFinaleState, InvalidSnapshot,
};

enum class ReputationTier : std::uint8_t { Enemy, Hostile, Distrusted, Neutral, Accepted, Trusted, Allied };
enum class FinalChoice : std::uint8_t { Stabilize, Sever, Contain };

struct PostgameResolutionState final {
    FinalChoice choice{FinalChoice::Stabilize};
    std::uint64_t transaction_id{0};
    core::SimulationTick commit_tick{};
    std::uint32_t schema_version{1U};
    bool main_campaign_complete{false};
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

struct ResearchRequirement final { std::string domain; std::uint32_t minimum{0}; };
struct ResearchProject final {
    std::string id;
    std::vector<std::string> prerequisites;
    std::vector<ResearchRequirement> evidence_thresholds;
    std::vector<std::string> unique_evidence;
    std::vector<std::string> technology_outputs;
    std::vector<std::string> blueprint_outputs;
};

// One authoritative story/reward transaction. Every field is validated before any mutation;
// the supplied transaction id is committed exactly once after the complete mutation succeeds.
struct ProgressionMutation final {
    std::int64_t credit_delta{0};
    std::map<std::string, std::int32_t> reputation_deltas{};
    std::string evidence_id{};
    std::map<std::string, std::uint32_t> evidence_values{};
    std::vector<std::string> campaign_flags{};
};

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
    std::optional<PostgameResolutionState> postgame_resolution{};
};

class ProgressionState final {
public:
    [[nodiscard]] std::int64_t credits() const noexcept { return credits_; }
    [[nodiscard]] core::Result<void, ProgressionError> credit(std::int64_t amount, std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, ProgressionError> debit(std::int64_t amount, std::uint64_t transaction_id);
    [[nodiscard]] core::Result<void, ProgressionError> apply(const ProgressionMutation& mutation,
                                                              std::uint64_t transaction_id);

    [[nodiscard]] std::int32_t reputation(std::string_view faction) const noexcept;
    [[nodiscard]] ReputationTier tier(std::string_view faction) const noexcept;
    [[nodiscard]] core::Result<void, ProgressionError> change_reputation(std::string faction, std::int32_t delta,
                                                                         std::uint64_t transaction_id);

    void integrate_evidence(std::string evidence_id, const std::map<std::string, std::uint32_t>& values);
    [[nodiscard]] std::uint32_t evidence(std::string_view domain) const noexcept;
    [[nodiscard]] bool has_evidence(std::string_view evidence_id) const noexcept;
    [[nodiscard]] bool has_technology(std::string_view technology_id) const noexcept;
    [[nodiscard]] bool has_blueprint(std::string_view blueprint_id) const noexcept;
    [[nodiscard]] core::Result<void, ProgressionError> complete_research(const ResearchProject& project,
                                                                         std::uint64_t transaction_id);

    [[nodiscard]] bool campaign_flag(std::string_view flag) const noexcept;
    void set_campaign_flag(std::string flag);
    [[nodiscard]] core::Result<void, ProgressionError> commit_finale(FinalChoice choice, std::uint64_t transaction_id,
                                                                     core::SimulationTick commit_tick);
    [[nodiscard]] core::Result<void, ProgressionError> complete_main_campaign(std::uint64_t transaction_id);
    [[nodiscard]] bool finale_completed() const noexcept { return postgame_resolution_.has_value(); }
    [[nodiscard]] bool main_campaign_complete() const noexcept {
        return postgame_resolution_.has_value() && postgame_resolution_->main_campaign_complete;
    }
    [[nodiscard]] const std::optional<PostgameResolutionState>& postgame_resolution() const noexcept {
        return postgame_resolution_;
    }

    [[nodiscard]] ProgressionSnapshot snapshot() const;
    [[nodiscard]] static core::Result<ProgressionState, ProgressionError> restore(ProgressionSnapshot snapshot);

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
    std::optional<PostgameResolutionState> postgame_resolution_{};
};

} // namespace starforge::progression
