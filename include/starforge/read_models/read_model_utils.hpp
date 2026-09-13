#pragma once

#include "starforge/read_models/read_model.hpp"

#include <span>

namespace starforge::read_models {

[[nodiscard]] bool source_revision_matches(
    const ReadModelMetadata& metadata,
    core::DomainId domain,
    core::StateRevision revision) noexcept;

[[nodiscard]] bool all_source_revisions_match(
    const ReadModelMetadata& metadata,
    std::span<const SourceRevision> current) noexcept;

}  // namespace starforge::read_models
