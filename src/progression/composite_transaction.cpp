#include "starforge/progression/progression.hpp"

#include <limits>

namespace starforge::progression {

core::Result<void, ProgressionError> ProgressionState::apply(
    const ProgressionMutation& mutation, std::uint64_t transaction_id) {
    if (transaction_seen(transaction_id)) {
        return core::Result<void, ProgressionError>::failure(ProgressionError::DuplicateTransaction);
    }

    // Validate the complete progression-domain transaction before touching authoritative state.
    auto next_reputation = reputation_;
    for (const auto& [faction, delta] : mutation.reputation_deltas) {
        if (faction.empty()) {
            return core::Result<void, ProgressionError>::failure(ProgressionError::InvalidReputation);
        }
        const auto current = static_cast<std::int64_t>(reputation(faction));
        const auto requested = current + static_cast<std::int64_t>(delta);
        next_reputation[faction] = static_cast<std::int32_t>(
            std::clamp<std::int64_t>(requested, -100, 100));
    }

    if ((!mutation.evidence_values.empty() && mutation.evidence_id.empty()) ||
        (!mutation.evidence_id.empty() && evidence_ids_.contains(mutation.evidence_id))) {
        return core::Result<void, ProgressionError>::failure(ProgressionError::AlreadyCompleted);
    }
    for (const auto& [domain, value] : mutation.evidence_values) {
        (void)value;
        if (domain.empty()) {
            return core::Result<void, ProgressionError>::failure(ProgressionError::MissingEvidence);
        }
    }
    for (const auto& flag : mutation.campaign_flags) {
        if (flag.empty()) {
            return core::Result<void, ProgressionError>::failure(ProgressionError::InvalidSnapshot);
        }
    }

    reputation_ = std::move(next_reputation);
    if (!mutation.evidence_id.empty()) {
        evidence_ids_.insert(mutation.evidence_id);
        for (const auto& [domain, value] : mutation.evidence_values) {
            auto& total = evidence_totals_[domain];
            const auto maximum = std::numeric_limits<std::uint32_t>::max();
            total = value > maximum - total ? maximum : total + value;
        }
    }
    campaign_flags_.insert(mutation.campaign_flags.begin(), mutation.campaign_flags.end());
    commit_transaction(transaction_id);
    return core::Result<void, ProgressionError>::success();
}

} // namespace starforge::progression
