#pragma once

#include "starforge/core/result.hpp"

#include <array>
#include <compare>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace starforge::content {

enum class ContentErrorCode : std::uint8_t {
    InvalidContentId,
    InvalidSchemaId,
    InvalidSchemaVersion,
    InvalidContentKind,
    DuplicateContentId,
    KindMismatch,
    NotFound,
};

struct ContentError final {
    ContentErrorCode code{};
};

enum class ContentKind : std::uint16_t {
    Unknown = 0,
    Definition = 1,
    Mesh = 2,
    Skeleton = 3,
    AnimationClip = 4,
    Texture = 5,
    Material = 6,
    Environment = 7,
    ShaderFamily = 8,
    ShaderBundle = 9,
    Audio = 10,
    Collision = 11,
    GroundNavigation = 12,
    FreeFlightNavigation = 13,
    Terrain = 14,
    Scene = 15,
    ProceduralModule = 16,
    MissionTemplate = 17,
    RaidTemplate = 18,
    DynamicEventTemplate = 19,
    WorldDefinition = 20,
};

[[nodiscard]] bool is_valid_content_kind(ContentKind kind) noexcept;

class ContentId final {
public:
    static constexpr std::size_t max_bytes = 255U;

    [[nodiscard]] static core::Result<ContentId, ContentError> parse(std::string_view value);

    [[nodiscard]] const std::string& value() const noexcept { return value_; }
    auto operator<=>(const ContentId&) const = default;

private:
    explicit ContentId(std::string value) : value_(std::move(value)) {}
    std::string value_;
};

class SchemaId final {
public:
    static constexpr std::size_t max_bytes = 255U;

    [[nodiscard]] static core::Result<SchemaId, ContentError> parse(std::string_view value);

    [[nodiscard]] const std::string& value() const noexcept { return value_; }
    auto operator<=>(const SchemaId&) const = default;

private:
    explicit SchemaId(std::string value) : value_(std::move(value)) {}
    std::string value_;
};

struct DefinitionSchema final {
    SchemaId id;
    std::uint32_t version{};
    ContentKind kind{ContentKind::Unknown};

    [[nodiscard]] static core::Result<DefinitionSchema, ContentError> create(
        SchemaId id,
        std::uint32_t version,
        ContentKind kind);
};

using ContentFingerprint = std::array<std::uint8_t, 32>;

struct ContentRegistryEntry final {
    ContentId id;
    ContentKind kind{ContentKind::Unknown};
    std::optional<SchemaId> schema_id;
    std::uint32_t schema_version{};
    std::uint32_t cooked_format_version{1U};
    ContentFingerprint fingerprint{};
    std::string cooked_locator;
};

class ContentRegistry final {
public:
    [[nodiscard]] static core::Result<ContentRegistry, ContentError> build(
        std::vector<ContentRegistryEntry> entries);

    [[nodiscard]] core::Result<std::reference_wrapper<const ContentRegistryEntry>, ContentError>
    resolve(const ContentId& id, ContentKind expected_kind) const;

    [[nodiscard]] const std::vector<ContentRegistryEntry>& entries() const noexcept { return entries_; }

private:
    explicit ContentRegistry(std::vector<ContentRegistryEntry> entries)
        : entries_(std::move(entries)) {}

    std::vector<ContentRegistryEntry> entries_;
};

}  // namespace starforge::content
