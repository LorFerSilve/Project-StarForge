#include "starforge/read_models/read_model_utils.hpp"

#include <algorithm>

namespace starforge::read_models {

bool source_revision_matches(
    const ReadModelMetadata& metadata,
    core::DomainId domain,
    core::StateRevision revision) noexcept {
    const auto it = std::ranges::find_if(metadata.source_revisions, [domain](const SourceRevision& source) {
        return source.domain == domain;
    });
    return it != metadata.source_revisions.end() && it->revision == revision;
}

bool all_source_revisions_match(
    const ReadModelMetadata& metadata,
    std::span<const SourceRevision> current) noexcept {
    if (metadata.source_revisions.size() != current.size()) {
        return false;
    }
    return std::ranges::all_of(current, [&metadata](const SourceRevision& source) {
        return source_revision_matches(metadata, source.domain, source.revision);
    });
}

}  // namespace starforge::read_models
