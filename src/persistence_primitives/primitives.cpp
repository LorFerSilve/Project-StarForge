#include "starforge/persistence/primitives.hpp"

#include <algorithm>
#include <limits>

namespace starforge::persistence {
namespace {

template <typename UInt>
void append_little_endian(std::vector<std::byte>& output, UInt value) {
    for (std::size_t index = 0; index < sizeof(UInt); ++index) {
        output.push_back(static_cast<std::byte>((value >> (index * 8U)) & static_cast<UInt>(0xFFU)));
    }
}

template <typename UInt>
[[nodiscard]] UInt read_little_endian(std::span<const std::byte> bytes, std::size_t offset) noexcept {
    UInt value{};
    for (std::size_t index = 0; index < sizeof(UInt); ++index) {
        value |= static_cast<UInt>(std::to_integer<std::uint8_t>(bytes[offset + index]))
            << (index * 8U);
    }
    return value;
}

template <typename UInt, std::size_t Size>
void write_fixed(std::array<std::byte, Size>& output, std::size_t offset, UInt value) noexcept {
    for (std::size_t index = 0; index < sizeof(UInt); ++index) {
        output[offset + index] =
            static_cast<std::byte>((value >> (index * 8U)) & static_cast<UInt>(0xFFU));
    }
}

[[nodiscard]] bool has_exactly_one_requirement_bit(std::uint32_t flags) noexcept {
    return flags == static_cast<std::uint32_t>(SectionRequirement::Required)
        || flags == static_cast<std::uint32_t>(SectionRequirement::Optional);
}

}  // namespace

void BinaryWriter::write_u8(std::uint8_t value) {
    bytes_.push_back(static_cast<std::byte>(value));
}

void BinaryWriter::write_u16(std::uint16_t value) {
    append_little_endian(bytes_, value);
}

void BinaryWriter::write_u32(std::uint32_t value) {
    append_little_endian(bytes_, value);
}

void BinaryWriter::write_u64(std::uint64_t value) {
    append_little_endian(bytes_, value);
}

void BinaryWriter::write_bytes(std::span<const std::byte> bytes) {
    bytes_.insert(bytes_.end(), bytes.begin(), bytes.end());
}

void BinaryWriter::write_string(std::string_view value) {
    write_u32(static_cast<std::uint32_t>(value.size()));
    for (const char value_byte : value) {
        bytes_.push_back(static_cast<std::byte>(static_cast<unsigned char>(value_byte)));
    }
}

void BinaryWriter::write_zeroes(std::size_t count) {
    bytes_.insert(bytes_.end(), count, std::byte{0});
}

core::Result<std::uint8_t, BinaryError> BinaryReader::read_u8() {
    if (!can_read(1U)) {
        return core::Result<std::uint8_t, BinaryError>::failure(
            BinaryError{BinaryErrorCode::UnexpectedEnd, cursor_});
    }
    return core::Result<std::uint8_t, BinaryError>::success(
        std::to_integer<std::uint8_t>(bytes_[cursor_++]));
}

core::Result<std::uint16_t, BinaryError> BinaryReader::read_u16() {
    if (!can_read(sizeof(std::uint16_t))) {
        return core::Result<std::uint16_t, BinaryError>::failure(
            BinaryError{BinaryErrorCode::UnexpectedEnd, cursor_});
    }
    const auto value = read_little_endian<std::uint16_t>(bytes_, cursor_);
    cursor_ += sizeof(std::uint16_t);
    return core::Result<std::uint16_t, BinaryError>::success(value);
}

core::Result<std::uint32_t, BinaryError> BinaryReader::read_u32() {
    if (!can_read(sizeof(std::uint32_t))) {
        return core::Result<std::uint32_t, BinaryError>::failure(
            BinaryError{BinaryErrorCode::UnexpectedEnd, cursor_});
    }
    const auto value = read_little_endian<std::uint32_t>(bytes_, cursor_);
    cursor_ += sizeof(std::uint32_t);
    return core::Result<std::uint32_t, BinaryError>::success(value);
}

core::Result<std::uint64_t, BinaryError> BinaryReader::read_u64() {
    if (!can_read(sizeof(std::uint64_t))) {
        return core::Result<std::uint64_t, BinaryError>::failure(
            BinaryError{BinaryErrorCode::UnexpectedEnd, cursor_});
    }
    const auto value = read_little_endian<std::uint64_t>(bytes_, cursor_);
    cursor_ += sizeof(std::uint64_t);
    return core::Result<std::uint64_t, BinaryError>::success(value);
}

core::Result<bool, BinaryError> BinaryReader::read_bool() {
    const auto raw = read_u8();
    if (!raw) {
        return core::Result<bool, BinaryError>::failure(raw.error());
    }
    if (raw.value() > 1U) {
        return core::Result<bool, BinaryError>::failure(
            BinaryError{BinaryErrorCode::InvalidBoolean, cursor_ - 1U});
    }
    return core::Result<bool, BinaryError>::success(raw.value() == 1U);
}

core::Result<std::string, BinaryError> BinaryReader::read_string() {
    const auto length = read_u32();
    if (!length) {
        return core::Result<std::string, BinaryError>::failure(length.error());
    }
    if (length.value() > max_string_bytes || !can_read(length.value())) {
        return core::Result<std::string, BinaryError>::failure(
            BinaryError{length.value() > max_string_bytes
                    ? BinaryErrorCode::InvalidStringLength
                    : BinaryErrorCode::UnexpectedEnd,
                cursor_});
    }

    std::string value;
    value.reserve(length.value());
    for (std::uint32_t index = 0; index < length.value(); ++index) {
        value.push_back(static_cast<char>(std::to_integer<unsigned char>(bytes_[cursor_ + index])));
    }
    cursor_ += length.value();
    return core::Result<std::string, BinaryError>::success(std::move(value));
}

std::uint32_t crc32c(std::span<const std::byte> bytes) noexcept {
    std::uint32_t crc = 0xFFFFFFFFU;
    for (const std::byte byte : bytes) {
        crc ^= std::to_integer<std::uint8_t>(byte);
        for (std::uint32_t bit = 0; bit < 8U; ++bit) {
            const std::uint32_t mask = 0U - (crc & 1U);
            crc = (crc >> 1U) ^ (0x82F63B78U & mask);
        }
    }
    return ~crc;
}

bool is_valid_save_kind(SaveKind kind) noexcept {
    return kind == SaveKind::Manual || kind == SaveKind::Quick || kind == SaveKind::Autosave;
}

bool is_known_section_kind(std::uint32_t raw_kind) noexcept {
    if (raw_kind == static_cast<std::uint32_t>(SectionKind::SaveMetadata)) {
        return true;
    }
    return raw_kind >= static_cast<std::uint32_t>(SectionKind::SessionCore)
        && raw_kind <= static_cast<std::uint32_t>(SectionKind::LocalContextContinuation)
        && raw_kind % 10U == 0U;
}

SectionRequirement canonical_requirement(SectionKind kind) noexcept {
    return kind == SectionKind::SaveMetadata
        ? SectionRequirement::Optional
        : SectionRequirement::Required;
}

std::array<std::byte, save_header_size_v1> encode_header_v1(const SaveHeaderV1& header) {
    std::array<std::byte, save_header_size_v1> bytes{};
    std::copy(save_magic_v1.begin(), save_magic_v1.end(), bytes.begin());
    write_fixed(bytes, 8U, save_container_version_v1);
    write_fixed(bytes, 12U, save_header_size_v1);
    write_fixed(bytes, 16U, endian_marker_v1);
    write_fixed(bytes, 20U, header.header_flags);
    write_fixed(bytes, 24U, header.application_save_compat_version);
    write_fixed(bytes, 28U, section_directory_entry_size_v1);
    write_fixed(bytes, 32U, header.saved_simulation_tick);
    write_fixed(bytes, 40U, header.snapshot_sequence);
    write_fixed(bytes, 48U, header.logical_slot_id);
    write_fixed(bytes, 56U, static_cast<std::uint32_t>(header.save_kind));
    write_fixed(bytes, 60U, header.section_count);
    write_fixed(bytes, 64U, header.directory_offset);
    write_fixed(bytes, 72U, header.directory_size);
    write_fixed(bytes, 80U, header.file_size);
    write_fixed(bytes, 88U, header.directory_crc32c);
    write_fixed(bytes, 92U, std::uint32_t{0U});
    for (std::size_t index = 0; index < header.content_build_id_sha256.size(); ++index) {
        bytes[96U + index] = static_cast<std::byte>(header.content_build_id_sha256[index]);
    }
    write_fixed(bytes, 92U, crc32c(bytes));
    return bytes;
}

core::Result<SaveHeaderV1, ContainerPrimitiveError> decode_header_v1(
    std::span<const std::byte> bytes) {
    if (bytes.size() < save_header_size_v1) {
        return core::Result<SaveHeaderV1, ContainerPrimitiveError>::failure(
            ContainerPrimitiveError{ContainerPrimitiveErrorCode::InvalidHeader, bytes.size()});
    }

    const auto header_bytes = bytes.first(save_header_size_v1);
    if (!std::equal(save_magic_v1.begin(), save_magic_v1.end(), header_bytes.begin())
        || read_little_endian<std::uint32_t>(header_bytes, 8U) != save_container_version_v1
        || read_little_endian<std::uint32_t>(header_bytes, 12U) != save_header_size_v1
        || read_little_endian<std::uint32_t>(header_bytes, 16U) != endian_marker_v1
        || read_little_endian<std::uint32_t>(header_bytes, 28U) != section_directory_entry_size_v1) {
        return core::Result<SaveHeaderV1, ContainerPrimitiveError>::failure(
            ContainerPrimitiveError{ContainerPrimitiveErrorCode::InvalidHeader, 0U});
    }

    auto crc_bytes = std::array<std::byte, save_header_size_v1>{};
    std::copy(header_bytes.begin(), header_bytes.end(), crc_bytes.begin());
    const auto stored_crc = read_little_endian<std::uint32_t>(header_bytes, 92U);
    write_fixed(crc_bytes, 92U, std::uint32_t{0U});
    if (crc32c(crc_bytes) != stored_crc) {
        return core::Result<SaveHeaderV1, ContainerPrimitiveError>::failure(
            ContainerPrimitiveError{ContainerPrimitiveErrorCode::HeaderCrcMismatch, 92U});
    }

    SaveHeaderV1 header{};
    header.header_flags = read_little_endian<std::uint32_t>(header_bytes, 20U);
    header.application_save_compat_version = read_little_endian<std::uint32_t>(header_bytes, 24U);
    header.saved_simulation_tick = read_little_endian<std::uint64_t>(header_bytes, 32U);
    header.snapshot_sequence = read_little_endian<std::uint64_t>(header_bytes, 40U);
    header.logical_slot_id = read_little_endian<std::uint64_t>(header_bytes, 48U);
    header.save_kind = static_cast<SaveKind>(read_little_endian<std::uint32_t>(header_bytes, 56U));
    header.section_count = read_little_endian<std::uint32_t>(header_bytes, 60U);
    header.directory_offset = read_little_endian<std::uint64_t>(header_bytes, 64U);
    header.directory_size = read_little_endian<std::uint64_t>(header_bytes, 72U);
    header.file_size = read_little_endian<std::uint64_t>(header_bytes, 80U);
    header.directory_crc32c = read_little_endian<std::uint32_t>(header_bytes, 88U);
    for (std::size_t index = 0; index < header.content_build_id_sha256.size(); ++index) {
        header.content_build_id_sha256[index] =
            std::to_integer<std::uint8_t>(header_bytes[96U + index]);
    }

    if ((header.header_flags & ~header_flag_optional_metadata) != 0U
        || !is_valid_save_kind(header.save_kind)
        || header.directory_offset != save_header_size_v1
        || header.directory_size
            != static_cast<std::uint64_t>(header.section_count) * section_directory_entry_size_v1) {
        return core::Result<SaveHeaderV1, ContainerPrimitiveError>::failure(
            ContainerPrimitiveError{ContainerPrimitiveErrorCode::InvalidHeader, 20U});
    }

    return core::Result<SaveHeaderV1, ContainerPrimitiveError>::success(header);
}

std::array<std::byte, section_directory_entry_size_v1> encode_directory_entry_v1(
    const SectionDirectoryEntryV1& entry) {
    std::array<std::byte, section_directory_entry_size_v1> bytes{};
    write_fixed(bytes, 0U, static_cast<std::uint32_t>(entry.section_kind));
    write_fixed(bytes, 4U, entry.schema_version);
    write_fixed(bytes, 8U, static_cast<std::uint32_t>(entry.requirement));
    write_fixed(bytes, 12U, static_cast<std::uint32_t>(entry.codec));
    write_fixed(bytes, 16U, entry.payload_offset);
    write_fixed(bytes, 24U, entry.stored_size);
    write_fixed(bytes, 32U, entry.decoded_size);
    write_fixed(bytes, 40U, entry.payload_crc32c);
    return bytes;
}

core::Result<SectionDirectoryEntryV1, ContainerPrimitiveError> decode_directory_entry_v1(
    std::span<const std::byte> bytes) {
    if (bytes.size() < section_directory_entry_size_v1) {
        return core::Result<SectionDirectoryEntryV1, ContainerPrimitiveError>::failure(
            ContainerPrimitiveError{ContainerPrimitiveErrorCode::InvalidDirectoryEntry, bytes.size()});
    }

    const auto entry_bytes = bytes.first(section_directory_entry_size_v1);
    const auto raw_kind = read_little_endian<std::uint32_t>(entry_bytes, 0U);
    const auto raw_flags = read_little_endian<std::uint32_t>(entry_bytes, 8U);
    const auto raw_codec = read_little_endian<std::uint32_t>(entry_bytes, 12U);
    const auto reserved0 = read_little_endian<std::uint32_t>(entry_bytes, 44U);
    const auto reserved1 = read_little_endian<std::uint64_t>(entry_bytes, 48U);
    const auto reserved2 = read_little_endian<std::uint64_t>(entry_bytes, 56U);

    if (!has_exactly_one_requirement_bit(raw_flags)
        || raw_codec != static_cast<std::uint32_t>(SectionCodec::None)
        || reserved0 != 0U || reserved1 != 0U || reserved2 != 0U) {
        return core::Result<SectionDirectoryEntryV1, ContainerPrimitiveError>::failure(
            ContainerPrimitiveError{ContainerPrimitiveErrorCode::InvalidDirectoryEntry, 8U});
    }

    SectionDirectoryEntryV1 entry{};
    entry.section_kind = static_cast<SectionKind>(raw_kind);
    entry.schema_version = read_little_endian<std::uint32_t>(entry_bytes, 4U);
    entry.requirement = static_cast<SectionRequirement>(raw_flags);
    entry.codec = static_cast<SectionCodec>(raw_codec);
    entry.payload_offset = read_little_endian<std::uint64_t>(entry_bytes, 16U);
    entry.stored_size = read_little_endian<std::uint64_t>(entry_bytes, 24U);
    entry.decoded_size = read_little_endian<std::uint64_t>(entry_bytes, 32U);
    entry.payload_crc32c = read_little_endian<std::uint32_t>(entry_bytes, 40U);

    if (entry.schema_version == 0U || entry.stored_size != entry.decoded_size) {
        return core::Result<SectionDirectoryEntryV1, ContainerPrimitiveError>::failure(
            ContainerPrimitiveError{ContainerPrimitiveErrorCode::InvalidDirectoryEntry, 4U});
    }

    return core::Result<SectionDirectoryEntryV1, ContainerPrimitiveError>::success(entry);
}

}  // namespace starforge::persistence
