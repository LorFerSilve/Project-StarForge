#include "starforge/content/content_model.hpp"

#include <catch2/catch_test_macros.hpp>

#include <optional>
#include <utility>
#include <vector>

TEST_CASE("Content.ContentId.CanonicalGrammarIsEnforced", "[content]") {
    using starforge::content::ContentId;

    const auto valid = ContentId::parse("weapon.ballistic.rifle_mk1");
    REQUIRE(valid);
    REQUIRE(valid.value().value() == "weapon.ballistic.rifle_mk1");

    REQUIRE_FALSE(ContentId::parse("weapon"));
    REQUIRE_FALSE(ContentId::parse("Weapon.ballistic"));
    REQUIRE_FALSE(ContentId::parse("weapon..rifle"));
    REQUIRE_FALSE(ContentId::parse("weapon/rifle"));
    REQUIRE_FALSE(ContentId::parse("weapon.-rifle"));
}

TEST_CASE("Content.Registry.IsSortedUniqueAndKindChecked", "[content]") {
    using namespace starforge::content;

    auto weapon = ContentId::parse("weapon.ballistic.rifle_mk1");
    auto scene = ContentId::parse("scene.horizon.test_cell");
    REQUIRE(weapon);
    REQUIRE(scene);

    std::vector<ContentRegistryEntry> entries;
    entries.push_back(ContentRegistryEntry{
        std::move(scene).value(), ContentKind::Scene, std::nullopt, 0U, 1U, {}, "scene/test"});
    entries.push_back(ContentRegistryEntry{
        std::move(weapon).value(), ContentKind::Definition, std::nullopt, 0U, 1U, {}, "defs/rifle"});

    const auto registry = ContentRegistry::build(std::move(entries));
    REQUIRE(registry);
    REQUIRE(registry.value().entries().front().id.value() == "scene.horizon.test_cell");

    const auto lookup_id = ContentId::parse("weapon.ballistic.rifle_mk1");
    REQUIRE(lookup_id);
    const auto resolved = registry.value().resolve(lookup_id.value(), ContentKind::Definition);
    REQUIRE(resolved);
    REQUIRE(resolved.value().get().cooked_locator == "defs/rifle");

    const auto mismatch = registry.value().resolve(lookup_id.value(), ContentKind::Texture);
    REQUIRE_FALSE(mismatch);
    REQUIRE(mismatch.error().code == ContentErrorCode::KindMismatch);
}

TEST_CASE("Content.Registry.DuplicateIdentityIsRejectedDeterministically", "[content]") {
    using namespace starforge::content;

    auto first = ContentId::parse("item.resource.iron");
    auto second = ContentId::parse("item.resource.iron");
    REQUIRE(first);
    REQUIRE(second);

    std::vector<ContentRegistryEntry> entries;
    entries.push_back(ContentRegistryEntry{
        std::move(first).value(), ContentKind::Definition, std::nullopt, 0U, 1U, {}, "a"});
    entries.push_back(ContentRegistryEntry{
        std::move(second).value(), ContentKind::Definition, std::nullopt, 0U, 1U, {}, "b"});

    const auto registry = ContentRegistry::build(std::move(entries));
    REQUIRE_FALSE(registry);
    REQUIRE(registry.error().code == ContentErrorCode::DuplicateContentId);
}

TEST_CASE("Content.DefinitionSchema.RequiresPositiveVersion", "[content]") {
    using namespace starforge::content;

    auto schema_id = SchemaId::parse("starforge.weapon");
    REQUIRE(schema_id);

    auto valid = DefinitionSchema::create(
        std::move(schema_id).value(), 1U, ContentKind::Definition);
    REQUIRE(valid);

    auto invalid_schema_id = SchemaId::parse("starforge.weapon");
    REQUIRE(invalid_schema_id);
    const auto invalid = DefinitionSchema::create(
        std::move(invalid_schema_id).value(), 0U, ContentKind::Definition);
    REQUIRE_FALSE(invalid);
    REQUIRE(invalid.error().code == ContentErrorCode::InvalidSchemaVersion);
}
