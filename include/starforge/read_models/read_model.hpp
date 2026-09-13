#pragma once

#include "starforge/core/domain.hpp"
#include "starforge/core/id_allocator.hpp"
#include "starforge/core/operation_ids.hpp"
#include "starforge/core/result.hpp"
#include "starforge/core/revision.hpp"
#include "starforge/core/simulation_time.hpp"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

namespace starforge::read_models {

struct SourceRevision final {
    core::DomainId domain{core::DomainId::Core};
    core::StateRevision revision{};
};

struct ReadModelMetadata final {
    core::ReadModelGeneration generation{};
    core::SimulationTick publication_tick{};
    std::vector<SourceRevision> source_revisions{};
};

enum class ReadModelError : std::uint8_t {
    GenerationExhausted,
    DuplicateSourceDomain,
};

template <typename T>
struct PublishedReadModel final {
    ReadModelMetadata metadata{};
    std::shared_ptr<const T> value{};
};

template <typename T>
class ReadModelPublisher final {
public:
    [[nodiscard]] core::Result<PublishedReadModel<T>, ReadModelError> publish(
        T value,
        core::SimulationTick publication_tick,
        std::vector<SourceRevision> source_revisions) {
        std::ranges::sort(source_revisions, [](const SourceRevision& lhs, const SourceRevision& rhs) {
            return lhs.domain < rhs.domain;
        });

        for (std::size_t index = 1; index < source_revisions.size(); ++index) {
            if (source_revisions[index - 1U].domain == source_revisions[index].domain) {
                return core::Result<PublishedReadModel<T>, ReadModelError>::failure(
                    ReadModelError::DuplicateSourceDomain);
            }
        }

        const auto candidate = generations_.candidate();
        if (!candidate) {
            return core::Result<PublishedReadModel<T>, ReadModelError>::failure(
                ReadModelError::GenerationExhausted);
        }
        const auto committed = generations_.commit(candidate.value());
        if (!committed) {
            return core::Result<PublishedReadModel<T>, ReadModelError>::failure(
                ReadModelError::GenerationExhausted);
        }

        PublishedReadModel<T> published;
        published.metadata.generation = committed.value();
        published.metadata.publication_tick = publication_tick;
        published.metadata.source_revisions = std::move(source_revisions);
        published.value = std::make_shared<const T>(std::move(value));
        return core::Result<PublishedReadModel<T>, ReadModelError>::success(std::move(published));
    }

private:
    core::MonotonicIdAllocator<core::ReadModelGeneration> generations_{};
};

}  // namespace starforge::read_models
