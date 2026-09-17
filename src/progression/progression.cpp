#include "starforge/progression/progression.hpp"

namespace starforge::progression {

bool ProgressionState::transaction_seen(std::uint64_t transaction_id) const noexcept {
    return transaction_id == 0 || committed_transactions_.contains(transaction_id);
}

void ProgressionState::commit_transaction(std::uint64_t transaction_id) {
    committed_transactions_.insert(transaction_id);
}

core::Result<void, ProgressionError> ProgressionState::credit(std::int64_t amount,
                                                               std::uint64_t transaction_id) {
    if (amount <= 0) return ProgressionError::InvalidAmount;
    if (transaction_seen(transaction_id)) return ProgressionError::DuplicateTransaction;
    credits_ += amount;
    commit_transaction(transaction_id);
    return {};
}

core::Result<void, ProgressionError> ProgressionState::debit(std::int64_t amount,
                                                              std::uint64_t transaction_id) {
    if (amount <= 0) return ProgressionError::InvalidAmount;
    if (transaction_seen(transaction_id)) return ProgressionError::DuplicateTransaction;
    if (credits_ < amount) return ProgressionError::InsufficientCredits;
    credits_ -= amount;
    commit_transaction(transaction_id);
    return {};
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
    if (faction.empty()) return ProgressionError::InvalidReputation;
    if (transaction_seen(transaction_id)) return ProgressionError::DuplicateTransaction;
    const auto next = std::clamp(reputation(faction) + delta, -100, 100);
    reputation_[std::move(faction)] = next;
    commit_transaction(transaction_id);
    return {};
}

void ProgressionState::integrate_evidence(
    std::string evidence_id, const std::map<std::string, std::uint32_t>& values) {
    if (evidence_id.empty() || evidence_ids_.contains(evidence_id)) return;
    evidence_ids_.insert(std::move(evidence_id));
    for (const auto& [domain, value] : values) evidence_totals_[domain] += value;
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
    if (project.id.empty()) return ProgressionError::MissingPrerequisite;
    if (transaction_seen(transaction_id)) return ProgressionError::DuplicateTransaction;
    if (completed_research_.contains(project.id)) return ProgressionError::AlreadyCompleted;
    for (const auto& prerequisite : project.prerequisites) {
        if (!technologies_.contains(prerequisite)) return ProgressionError::MissingPrerequisite;
    }
    for (const auto& requirement : project.evidence_thresholds) {
        if (evidence(requirement.domain) < requirement.minimum) return ProgressionError::MissingEvidence;
    }
    for (const auto& evidence_id : project.unique_evidence) {
        if (!evidence_ids_.contains(evidence_id)) return ProgressionError::MissingEvidence;
    }
    completed_research_.insert(project.id);
    technologies_.insert(project.technology_outputs.begin(), project.technology_outputs.end());
    blueprints_.insert(project.blueprint_outputs.begin(), project.blueprint_outputs.end());
    commit_transaction(transaction_id);
    return {};
}

bool ProgressionState::campaign_flag(std::string_view flag) const noexcept {
    return campaign_flags_.contains(std::string(flag));
}

void ProgressionState::set_campaign_flag(std::string flag) {
    if (!flag.empty()) campaign_flags_.insert(std::move(flag));
}

core::Result<void, ProgressionError> ProgressionState::commit_finale(
    std::string ending_flag, std::uint64_t transaction_id) {
    if (ending_flag.empty()) return ProgressionError::InvalidFinaleState;
    if (transaction_seen(transaction_id)) return ProgressionError::DuplicateTransaction;
    if (finale_completed_) return ProgressionError::AlreadyCompleted;
    campaign_flags_.insert(std::move(ending_flag));
    finale_completed_ = true;
    commit_transaction(transaction_id);
    return {};
}

} // namespace starforge::progression
