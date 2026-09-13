#pragma once

#include "starforge/content/content_model.hpp"
#include "starforge/core/result.hpp"
#include "starforge/persistence/primitives.hpp"

#include <cstddef>
#include <cstdint>

namespace starforge::persistence {

enum class ContentCodecErrorCode : std::uint8_t {
    BinaryDecode,
    InvalidContentId,
};

struct ContentCodecError final {
    ContentCodecErrorCode code{};
    std::size_t offset{};
};

void write_content_id(BinaryWriter& writer, const content::ContentId& content_id);

[[nodiscard]] core::Result<content::ContentId, ContentCodecError> read_content_id(
    BinaryReader& reader);

}  // namespace starforge::persistence
