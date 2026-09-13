#include "starforge/content/content_model.hpp"

#include <algorithm>
#include <utility>

namespace starforge::content {
namespace {

[[nodiscard]] bool valid_logical_id(std::string_view value, std::size_t max_bytes) noexcept {
    if (value.size() < 3U || value.size() > max_bytes) {
        return false;
    }

    std::size_t segments = 1U;
    bool segment_has_character = false;
    for (const char ch : value) {
        if (ch == '.') {
            if (!segment_has_character) {
                return false;
            }
            ++segments;
            segment_has_character = false;
            continue;
        }

        const bool alpha = ch >= 'a' && ch <= 'z';
        const bool digit = ch >= '0' && ch <= '9';
        if (!alpha && !digit && ch != '_' && ch != '-') {
            return false;
        }

        if (!segment_has_character && (ch == '_' || ch == '-')) {
            return false;
        }
        segment_has_character = true;
    }

    return segment_has_character && segments >= 2U;
}

}  // namespace

bool is_valid_content_kind(ContentKind kind) noexcept {
    const auto raw = static_cast<std::uint16_t>(kind);
    return raw >= static_cast<std::uint16_t>(ContentKind::Definition)
        && raw <= static_cast<std::uint16_t>(ContentKind::WorldDefinition);
}

core::Result<ContentId, ContentError> ContentId::parse(std::string_view value) {
    if (!valid_logical_id(value, max_bytes)) {
        return core::Result<ContentId, ContentError>::failure(
            ContentError{ContentErrorCode::InvalidContentId});
    }
    return core::Result<ContentId, ContentError>::success(ContentId{std::string(value)});
}

core::Result<SchemaId, ContentError> SchemaId::parse(std::string_view value) {
    if (!valid_logical_id(value, max_bytes)) {
        return core::Result<SchemaId, ContentError>::failure(
            ContentError{ContentErrorCode::InvalidSchemaId});
    }
    return core::Result<SchemaId, ContentError>::success(SchemaId{std::string(value)});
}

core::Result<DefinitionSchema, ContentError> DefinitionSchema::create(
    SchemaId id,
    std::uint32_t version,
    ContentKind kind) {
    if (version == 0U) {
        return core::Result<DefinitionSchema, ContentError>::failure(
            ContentError{ContentErrorCode::InvalidSchemaVersion});
    }
    if (!is_valid_content_kind(kind)) {
        return core::Result<DefinitionSchema, ContentError>::failure(
            ContentError{ContentErrorCode::InvalidContentKind});
    }
    return core::Result<DefinitionSchema, ContentError>::success(
        DefinitionSchema{std::move(id), version, kind});
}

core::Result<ContentRegistry, ContentError> ContentRegistry::build(
    std::vector<ContentRegistryEntry> entries) {
    for (const auto& entry : entries) {
        if (!is_valid_content_kind(entry.kind)) {
            return core::Result<ContentRegistry, ContentError>::failure(
                ContentError{ContentErrorCode::InvalidContentKind});
        }
        if (entry.schema_id.has_value() != (entry.schema_version != 0U)) {
            return core::Result<ContentRegistry, ContentError>::failure(
                ContentError{ContentErrorCode::InvalidSchemaVersion});
        }
        if (entry.cooked_format_version == 0U) {
            return core::Result<ContentRegistry, ContentError>::failure(
                ContentError{ContentErrorCode::InvalidSchemaVersion});
        }
    }

    std::sort(entries.begin(), entries.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.id.value() < rhs.id.value();
    });

    const auto duplicate = std::adjacent_find(entries.begin(), entries.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.id == rhs.id;
    });
    if (duplicate != entries.end()) {
        return core::Result<ContentRegistry, ContentError>::failure(
            ContentError{ContentErrorCode::DuplicateContentId});
    }

    return core::Result<ContentRegistry, ContentError>::success(ContentRegistry{std::move(entries)});
}

core::Result<std::reference_wrapper<const ContentRegistryEntry>, ContentError>
ContentRegistry::resolve(const ContentId& id, ContentKind expected_kind) const {
    const auto it = std::lower_bound(entries_.begin(), entries_.end(), id, [](const auto& entry, const auto& needle) {
        return entry.id.value() < needle.value();
    });

    if (it == entries_.end() || it->id != id) {
        return core::Result<std::reference_wrapper<const ContentRegistryEntry>, ContentError>::failure(
            ContentError{ContentErrorCode::NotFound});
    }
    if (it->kind != expected_kind) {
        return core::Result<std::reference_wrapper<const ContentRegistryEntry>, ContentError>::failure(
            ContentError{ContentErrorCode::KindMismatch});
    }
    return core::Result<std::reference_wrapper<const ContentRegistryEntry>, ContentError>::success(std::cref(*it));
}

}  // namespace starforge::content
