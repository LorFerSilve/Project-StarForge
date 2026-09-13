#include "starforge/read_models/read_model.hpp"
#include "starforge/read_models/read_model_utils.hpp"

#include <catch2/catch_test_macros.hpp>

#include <type_traits>
#include <vector>

namespace {
struct InventoryView final {
    int item_count{0};
    int credits{0};
};
}  // namespace

TEST_CASE("ReadModels.Publish.PublishedStateIsImmutableAndGenerationOrdered", "[domain][read-models][determinism]") {
    starforge::read_models::ReadModelPublisher<InventoryView> publisher;

    auto first = publisher.publish(
        InventoryView{2, 100},
        starforge::core::SimulationTick{10U},
        {
            {starforge::core::DomainId::Economy, starforge::core::StateRevision{4U}},
            {starforge::core::DomainId::Player, starforge::core::StateRevision{7U}},
        });
    REQUIRE(first);
    STATIC_REQUIRE(std::is_const_v<std::remove_reference_t<decltype(*first.value().value)>>);
    REQUIRE(first.value().metadata.generation == starforge::core::ReadModelGeneration{1U});
    REQUIRE(first.value().value->item_count == 2);

    auto second = publisher.publish(
        InventoryView{3, 80},
        starforge::core::SimulationTick{11U},
        {
            {starforge::core::DomainId::Player, starforge::core::StateRevision{8U}},
            {starforge::core::DomainId::Economy, starforge::core::StateRevision{5U}},
        });
    REQUIRE(second);
    REQUIRE(second.value().metadata.generation == starforge::core::ReadModelGeneration{2U});
    REQUIRE(second.value().value->item_count == 3);
    REQUIRE(first.value().value->item_count == 2);
    REQUIRE(first.value().value->credits == 100);
}

TEST_CASE("ReadModels.Publish.SourceRevisionsAreCanonicalAndQueryable", "[domain][read-models][determinism]") {
    starforge::read_models::ReadModelPublisher<InventoryView> publisher;
    auto published = publisher.publish(
        InventoryView{1, 10},
        starforge::core::SimulationTick{20U},
        {
            {starforge::core::DomainId::Economy, starforge::core::StateRevision{9U}},
            {starforge::core::DomainId::Player, starforge::core::StateRevision{3U}},
        });
    REQUIRE(published);

    REQUIRE(starforge::read_models::source_revision_matches(
        published.value().metadata,
        starforge::core::DomainId::Player,
        starforge::core::StateRevision{3U}));
    REQUIRE_FALSE(starforge::read_models::source_revision_matches(
        published.value().metadata,
        starforge::core::DomainId::Player,
        starforge::core::StateRevision{4U}));

    const std::vector<starforge::read_models::SourceRevision> current{
        {starforge::core::DomainId::Player, starforge::core::StateRevision{3U}},
        {starforge::core::DomainId::Economy, starforge::core::StateRevision{9U}},
    };
    REQUIRE(starforge::read_models::all_source_revisions_match(published.value().metadata, current));
}

TEST_CASE("ReadModels.Publish.DuplicateSourceDomainRejectsWithoutConsumingGeneration", "[domain][read-models]") {
    starforge::read_models::ReadModelPublisher<InventoryView> publisher;
    auto rejected = publisher.publish(
        InventoryView{},
        starforge::core::SimulationTick{1U},
        {
            {starforge::core::DomainId::Player, starforge::core::StateRevision{1U}},
            {starforge::core::DomainId::Player, starforge::core::StateRevision{2U}},
        });
    REQUIRE_FALSE(rejected);
    REQUIRE(rejected.error() == starforge::read_models::ReadModelError::DuplicateSourceDomain);

    auto first_valid = publisher.publish(
        InventoryView{},
        starforge::core::SimulationTick{1U},
        {{starforge::core::DomainId::Player, starforge::core::StateRevision{1U}}});
    REQUIRE(first_valid);
    REQUIRE(first_valid.value().metadata.generation == starforge::core::ReadModelGeneration{1U});
}
