#include "starforge/progression/progression.hpp"

#include <limits>
#include <utility>

namespace starforge::progression {

bool ProgressionState::transaction_seen(std::uint64_t transaction_id) const noexcept {
    return transaction_id == 0 || committed_transactions_.contains(transaction_id);
}

void ProgressionState::commit_transaction(std::uint64_t transaction_id) {
    committed_transactions_.insert(transaction_id);
}

std::int32_t ProgressionState::reputation(std::string_view faction) const noexcept {
    const auto it = reputation_.find(std::string(faction));
    return it == reputation_.end() ? 0 : it->second;
}

ReputationTier ProgressionState::tier(std::string_view faction) const noexcept {
    return reputation_tier(reputation(faction));
}

core::Result<void, ProgressionError> ProgressionState::change_reputation(
    std::string faction, std::int32_t delta, std::uint64_t transaction_id) {
    if (faction.empty()) return core::Result<void, ProgressionError>::failure(ProgressionError::InvalidReputation);
    if (transaction_seen(transaction_id)) return core::Result<void, ProgressionError>::failure(ProgressionError::DuplicateTransaction);
    const auto current = static_cast<std::int64_t>(reputation(faction));
    const auto requested = current + static_cast<std::int64_t>(delta);
    const auto next = static_cast<std::int32_t>(std::clamp<std::int64_t>(requested, -100, 100));
    reputation_[std::move(faction)] = next;
    commit_transaction(transaction_id);
    return core::Result<void, ProgressionError>::success();
}

void ProgressionState::integrate_evidence(
    std::string evidence_id, const std::map<std::string, std::uint32_t>& values) {
    if (evidence_id.empty() || evidence_ids_.contains(evidence_id)) return;
    evidence_ids_.insert(std::move(evidence_id));
    for (const auto& [domain, value] : values) {
        auto& total = evidence_totals_[domain];
        const auto maximum = std::numeric_limits<std::uint32_t>::max();
        total = value > maximum - total ? maximum : total + value;
    }
}

std::uint32_t ProgressionState::evidence(std::string_view domain) const noexcept {
    const auto it = evidence_totals_.find(std::string(domain));
    return it == evidence_totals_.end() ? 0U : it->second;
}

bool ProgressionState::has_evidence(std::string_view evidence_id) const noexcept {
    return evidence_ids_.contains(std::string(evidence_id));
}

bool ProgressionState::has_technology(std::string_view technology_id) const noexcept {
    return technologies_.contains(std::string(technology_id));
}

bool ProgressionState::has_blueprint(std::string_view blueprint_id) const noexcept {
    return blueprints_.contains(std::string(blueprint_id));
}

core::Result<void, ProgressionError> ProgressionState::complete_research(
    const ResearchProject& project, std::uint64_t transaction_id) {
    if (project.id.empty()) return core::Result<void, ProgressionError>::failure(ProgressionError::MissingPrerequisite);
    if (transaction_seen(transaction_id)) return core::Result<void, ProgressionError>::failure(ProgressionError::DuplicateTransaction);
    if (completed_research_.contains(project.id)) return core::Result<void, ProgressionError>::failure(ProgressionError::AlreadyCompleted);
    for (const auto& prerequisite : project.prerequisites) {
        if (!technologies_.contains(prerequisite)) return core::Result<void, ProgressionError>::failure(ProgressionError::MissingPrerequisite);
    }
    for (const auto& requirement : project.evidence_thresholds) {
        if (evidence(requirement.domain) < requirement.minimum) return core::Result<void, ProgressionError>::failure(ProgressionError::MissingEvidence);
    }
    for (const auto& evidence_id : project.unique_evidence) {
        if (!evidence_ids_.contains(evidence_id)) return core::Result<void, ProgressionError>::failure(ProgressionError::MissingEvidence);
    }
    completed_research_.insert(project.id);
    technologies_.insert(project.technology_outputs.begin(), project.technology_outputs.end());
    blueprints_.insert(project.blueprint_outputs.begin(), project.blueprint_outputs.end());
    commit_transaction(transaction_id);
    return core::Result<void, ProgressionError>::success();
}

bool ProgressionState::campaign_flag(std::string_view flag) const noexcept {
    return campaign_flags_.contains(std::string(flag));
}

void ProgressionState::set_campaign_flag(std::string flag) {
    if (!flag.empty()) campaign_flags_.insert(std::move(flag));
}

core::Result<void, ProgressionError> ProgressionState::commit_finale(
    FinalChoice choice, std::uint64_t transaction_id, core::SimulationTick commit_tick) {
    if (transaction_seen(transaction_id)) return core::Result<void, ProgressionError>::failure(ProgressionError::DuplicateTransaction);
    if (postgame_resolution_.has_value()) return core::Result<void, ProgressionError>::failure(ProgressionError::AlreadyCompleted);
    postgame_resolution_ = PostgameResolutionState{
        .choice = choice,
        .transaction_id = transaction_id,
        .commit_tick = commit_tick,
        .schema_version = 1U,
        .main_campaign_complete = false,
    };
    commit_transaction(transaction_id);
    return core::Result<void, ProgressionError>::success();
}

core::Result<void, ProgressionError> ProgressionState::complete_main_campaign(
    std::uint64_t transaction_id) {
    if (transaction_seen(transaction_id)) return core::Result<void, ProgressionError>::failure(ProgressionError::DuplicateTransaction);
    if (!postgame_resolution_.has_value()) {
        return core::Result<void, ProgressionError>::failure(ProgressionError::InvalidFinaleState);
    }
    if (postgame_resolution_->main_campaign_complete) {
        return core::Result<void, ProgressionError>::failure(ProgressionError::AlreadyCompleted);
    }
    postgame_resolution_->main_campaign_complete = true;
    commit_transaction(transaction_id);
    return core::Result<void, ProgressionError>::success();
}

ProgressionSnapshot ProgressionState::snapshot() const {
    return ProgressionSnapshot{
        .reputation = reputation_,
        .evidence_totals = evidence_totals_,
        .evidence_ids = evidence_ids_,
        .technologies = technologies_,
        .blueprints = blueprints_,
        .completed_research = completed_research_,
        .campaign_flags = campaign_flags_,
        .committed_transactions = committed_transactions_,
        .postgame_resolution = postgame_resolution_,
    };
}

core::Result<ProgressionState, ProgressionError> ProgressionState::restore(
    ProgressionSnapshot snapshot) {
    if (snapshot.committed_transactions.contains(0)) {
        return core::Result<ProgressionState, ProgressionError>::failure(
            ProgressionError::InvalidSnapshot);
    }
    for (const auto& [faction, value] : snapshot.reputation) {
        if (faction.empty() || value < -100 || value > 100) {
            return core::Result<ProgressionState, ProgressionError>::failure(
                ProgressionError::InvalidSnapshot);
        }
    }
    for (const auto& value : snapshot.evidence_ids) {
        if (value.empty()) return core::Result<ProgressionState, ProgressionError>::failure(ProgressionError::InvalidSnapshot);
    }
    for (const auto& value : snapshot.technologies) {
        if (value.empty()) return core::Result<ProgressionState, ProgressionError>::failure(ProgressionError::InvalidSnapshot);
    }
    for (const auto& value : snapshot.blueprints) {
        if (value.empty()) return core::Result<ProgressionState, ProgressionError>::failure(ProgressionError::InvalidSnapshot);
    }
    for (const auto& value : snapshot.completed_research) {
        if (value.empty()) return core::Result<ProgressionState, ProgressionError>::failure(ProgressionError::InvalidSnapshot);
    }
    for (const auto& value : snapshot.campaign_flags) {
        if (value.empty()) return core::Result<ProgressionState, ProgressionError>::failure(ProgressionError::InvalidSnapshot);
    }
    if (snapshot.postgame_resolution.has_value()) {
        const auto& postgame = *snapshot.postgame_resolution;
        if (postgame.transaction_id == 0 || postgame.schema_version != 1U ||
            !snapshot.committed_transactions.contains(postgame.transaction_id)) {
            return core::Result<ProgressionState, ProgressionError>::failure(
                ProgressionError::InvalidSnapshot);
        }
    }

    ProgressionState state;
    state.reputation_ = std::move(snapshot.reputation);
    state.evidence_totals_ = std::move(snapshot.evidence_totals);
    state.evidence_ids_ = std::move(snapshot.evidence_ids);
    state.technologies_ = std::move(snapshot.technologies);
    state.blueprints_ = std::move(snapshot.blueprints);
    state.completed_research_ = std::move(snapshot.completed_research);
    state.campaign_flags_ = std::move(snapshot.campaign_flags);
    state.committed_transactions_ = std::move(snapshot.committed_transactions);
    state.postgame_resolution_ = std::move(snapshot.postgame_resolution);
    return core::Result<ProgressionState, ProgressionError>::success(std::move(state));
}

} // namespace starforge::progression
