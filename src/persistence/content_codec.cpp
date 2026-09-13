#include "starforge/persistence/content_codec.hpp"

namespace starforge::persistence {

void write_content_id(BinaryWriter& writer, const content::ContentId& content_id) {
    writer.write_string(content_id.value());
}

core::Result<content::ContentId, ContentCodecError> read_content_id(BinaryReader& reader) {
    const auto offset = reader.position();
    const auto encoded = reader.read_string();
    if (!encoded) {
        return core::Result<content::ContentId, ContentCodecError>::failure(
            ContentCodecError{ContentCodecErrorCode::BinaryDecode, encoded.error().offset});
    }

    auto parsed = content::ContentId::parse(encoded.value());
    if (!parsed) {
        return core::Result<content::ContentId, ContentCodecError>::failure(
            ContentCodecError{ContentCodecErrorCode::InvalidContentId, offset});
    }
    return core::Result<content::ContentId, ContentCodecError>::success(std::move(parsed).value());
}

}  // namespace starforge::persistence
