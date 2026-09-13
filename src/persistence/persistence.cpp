#include "starforge/persistence/persistence.hpp"

#include <algorithm>
#include <limits>
#include <utility>

namespace starforge::persistence {
namespace {

constexpr std::uint64_t max_save_bytes = 64ULL * 1024ULL * 1024ULL;
constexpr std::uint32_t max_section_count = 256U;
constexpr std::uint64_t max_section_bytes = 16ULL * 1024ULL * 1024ULL;

[[nodiscard]] std::uint64_t align_up_8(std::uint64_t value) noexcept {
    return (value + 7U) & ~std::uint64_t{7U};
}

[[nodiscard]] core::Result<std::vector<std::byte>, PersistenceError> frame_payload(
    const SaveSection& section) {
    if (section.schema_version == 0U || section.body.size() > max_section_bytes - 16U) {
        return core::Result<std::vector<std::byte>, PersistenceError>::failure(
            PersistenceError{PersistenceErrorCode::InvalidSnapshot, 0U});
    }

    BinaryWriter writer;
    writer.write_u32(payload_format_version_v1);
    writer.write_u32(static_cast<std::uint32_t>(section.kind));
    writer.write_u32(section.schema_version);
    writer.write_u32(0U);
    writer.write_bytes(section.body);
    return core::Result<std::vector<std::byte>, PersistenceError>::success(std::move(writer).take());
}

[[nodiscard]] bool range_inside(
    std::uint64_t offset,
    std::uint64_t size,
    std::uint64_t total) noexcept {
    return offset <= total && size <= total - offset;
}

}  // namespace

core::Result<std::vector<std::byte>, PersistenceError> encode_save_v1(
    const SaveSnapshot& snapshot) {
    if (snapshot.sections.empty() || snapshot.sections.size() > max_section_count
        || snapshot.snapshot_sequence == 0U || snapshot.logical_slot_id == 0U
        || !is_valid_save_kind(snapshot.save_kind)) {
        return core::Result<std::vector<std::byte>, PersistenceError>::failure(
            PersistenceError{PersistenceErrorCode::InvalidSnapshot, 0U});
    }

    auto sections = snapshot.sections;
    std::sort(sections.begin(), sections.end(), [](const auto& lhs, const auto& rhs) {
        return static_cast<std::uint32_t>(lhs.kind) < static_cast<std::uint32_t>(rhs.kind);
    });

    for (std::size_t index = 0; index < sections.size(); ++index) {
        const auto raw_kind = static_cast<std::uint32_t>(sections[index].kind);
        if (!is_known_section_kind(raw_kind) || sections[index].schema_version == 0U) {
            return core::Result<std::vector<std::byte>, PersistenceError>::failure(
                PersistenceError{PersistenceErrorCode::InvalidSnapshot, index});
        }
        if (index > 0U && sections[index - 1U].kind == sections[index].kind) {
            return core::Result<std::vector<std::byte>, PersistenceError>::failure(
                PersistenceError{PersistenceErrorCode::InvalidSnapshot, index});
        }
    }

    const std::uint64_t directory_size =
        static_cast<std::uint64_t>(sections.size()) * section_directory_entry_size_v1;
    std::uint64_t next_payload_offset = align_up_8(save_header_size_v1 + directory_size);

    std::vector<std::vector<std::byte>> payloads;
    std::vector<SectionDirectoryEntryV1> entries;
    payloads.reserve(sections.size());
    entries.reserve(sections.size());

    bool has_metadata = false;
    for (const auto& section : sections) {
        const auto framed = frame_payload(section);
        if (!framed) {
            return core::Result<std::vector<std::byte>, PersistenceError>::failure(framed.error());
        }

        const auto& payload = framed.value();
        if (payload.size() > max_section_bytes
            || next_payload_offset > max_save_bytes
            || payload.size() > max_save_bytes - next_payload_offset) {
            return core::Result<std::vector<std::byte>, PersistenceError>::failure(
                PersistenceError{PersistenceErrorCode::SafetyLimitExceeded, 0U});
        }

        entries.push_back(SectionDirectoryEntryV1{
            section.kind,
            section.schema_version,
            canonical_requirement(section.kind),
            SectionCodec::None,
            next_payload_offset,
            static_cast<std::uint64_t>(payload.size()),
            static_cast<std::uint64_t>(payload.size()),
            crc32c(payload),
        });
        has_metadata = has_metadata || section.kind == SectionKind::SaveMetadata;
        payloads.push_back(payload);
        next_payload_offset = align_up_8(next_payload_offset + payload.size());
    }

    if (!entries.empty()) {
        next_payload_offset = entries.back().payload_offset + entries.back().stored_size;
    }
    if (next_payload_offset > max_save_bytes) {
        return core::Result<std::vector<std::byte>, PersistenceError>::failure(
            PersistenceError{PersistenceErrorCode::SafetyLimitExceeded, 0U});
    }

    BinaryWriter directory_writer;
    for (const auto& entry : entries) {
        const auto encoded = encode_directory_entry_v1(entry);
        directory_writer.write_bytes(encoded);
    }
    const auto& directory = directory_writer.bytes();

    SaveHeaderV1 header{};
    header.header_flags = has_metadata ? header_flag_optional_metadata : 0U;
    header.application_save_compat_version = snapshot.application_save_compat_version;
    header.saved_simulation_tick = snapshot.saved_simulation_tick;
    header.snapshot_sequence = snapshot.snapshot_sequence;
    header.logical_slot_id = snapshot.logical_slot_id;
    header.save_kind = snapshot.save_kind;
    header.section_count = static_cast<std::uint32_t>(entries.size());
    header.directory_size = directory_size;
    header.file_size = next_payload_offset;
    header.directory_crc32c = crc32c(directory);
    header.content_build_id_sha256 = snapshot.content_build_id_sha256;
    const auto encoded_header = encode_header_v1(header);

    BinaryWriter file_writer;
    file_writer.write_bytes(encoded_header);
    file_writer.write_bytes(directory);
    if (file_writer.bytes().size() < entries.front().payload_offset) {
        file_writer.write_zeroes(entries.front().payload_offset - file_writer.bytes().size());
    }

    for (std::size_t index = 0; index < entries.size(); ++index) {
        if (file_writer.bytes().size() < entries[index].payload_offset) {
            file_writer.write_zeroes(entries[index].payload_offset - file_writer.bytes().size());
        }
        file_writer.write_bytes(payloads[index]);
    }

    auto file = std::move(file_writer).take();
    if (file.size() != header.file_size) {
        return core::Result<std::vector<std::byte>, PersistenceError>::failure(
            PersistenceError{PersistenceErrorCode::InvalidSnapshot, file.size()});
    }
    return core::Result<std::vector<std::byte>, PersistenceError>::success(std::move(file));
}

core::Result<StagedLoad, PersistenceError> stage_load_v1(
    std::span<const std::byte> file_bytes) {
    if (file_bytes.size() > max_save_bytes) {
        return core::Result<StagedLoad, PersistenceError>::failure(
            PersistenceError{PersistenceErrorCode::SafetyLimitExceeded, 0U});
    }

    const auto header_result = decode_header_v1(file_bytes);
    if (!header_result) {
        const auto code = header_result.error().code == ContainerPrimitiveErrorCode::HeaderCrcMismatch
            ? PersistenceErrorCode::HeaderCrcMismatch
            : PersistenceErrorCode::InvalidHeader;
        return core::Result<StagedLoad, PersistenceError>::failure(
            PersistenceError{code, header_result.error().offset});
    }
    const auto header = header_result.value();

    if (header.file_size != file_bytes.size() || header.section_count == 0U
        || header.section_count > max_section_count
        || !range_inside(header.directory_offset, header.directory_size, header.file_size)) {
        return core::Result<StagedLoad, PersistenceError>::failure(
            PersistenceError{PersistenceErrorCode::InvalidDirectory, 64U});
    }

    const auto directory = file_bytes.subspan(
        static_cast<std::size_t>(header.directory_offset),
        static_cast<std::size_t>(header.directory_size));
    if (crc32c(directory) != header.directory_crc32c) {
        return core::Result<StagedLoad, PersistenceError>::failure(
            PersistenceError{PersistenceErrorCode::DirectoryCrcMismatch,
                static_cast<std::size_t>(header.directory_offset)});
    }

    std::vector<SectionDirectoryEntryV1> entries;
    entries.reserve(header.section_count);
    std::uint32_t previous_kind = 0U;
    std::uint64_t previous_end = align_up_8(header.directory_offset + header.directory_size);
    bool metadata_seen = false;

    for (std::uint32_t index = 0; index < header.section_count; ++index) {
        const std::size_t entry_offset =
            static_cast<std::size_t>(index) * section_directory_entry_size_v1;
        const auto entry_result =
            decode_directory_entry_v1(directory.subspan(entry_offset, section_directory_entry_size_v1));
        if (!entry_result) {
            return core::Result<StagedLoad, PersistenceError>::failure(
                PersistenceError{PersistenceErrorCode::InvalidDirectory,
                    static_cast<std::size_t>(header.directory_offset) + entry_offset});
        }

        const auto entry = entry_result.value();
        const auto raw_kind = static_cast<std::uint32_t>(entry.section_kind);
        if (raw_kind <= previous_kind || entry.payload_offset % 8U != 0U
            || entry.stored_size > max_section_bytes
            || !range_inside(entry.payload_offset, entry.stored_size, header.file_size)
            || entry.payload_offset < previous_end) {
            return core::Result<StagedLoad, PersistenceError>::failure(
                PersistenceError{PersistenceErrorCode::InvalidDirectory,
                    static_cast<std::size_t>(header.directory_offset) + entry_offset});
        }

        if (!is_known_section_kind(raw_kind)) {
            if (entry.requirement == SectionRequirement::Required) {
                return core::Result<StagedLoad, PersistenceError>::failure(
                    PersistenceError{PersistenceErrorCode::UnsupportedRequiredSection,
                        static_cast<std::size_t>(header.directory_offset) + entry_offset});
            }
        } else if (entry.requirement != canonical_requirement(entry.section_kind)) {
            return core::Result<StagedLoad, PersistenceError>::failure(
                PersistenceError{PersistenceErrorCode::InvalidDirectory,
                    static_cast<std::size_t>(header.directory_offset) + entry_offset});
        }

        metadata_seen = metadata_seen || entry.section_kind == SectionKind::SaveMetadata;
        previous_kind = raw_kind;
        previous_end = entry.payload_offset + entry.stored_size;
        entries.push_back(entry);
    }

    const bool header_says_metadata = (header.header_flags & header_flag_optional_metadata) != 0U;
    if (metadata_seen != header_says_metadata) {
        return core::Result<StagedLoad, PersistenceError>::failure(
            PersistenceError{PersistenceErrorCode::InvalidDirectory, 20U});
    }

    StagedLoad staged{};
    staged.header = header;
    staged.sections.reserve(entries.size());

    for (const auto& entry : entries) {
        if (!is_known_section_kind(static_cast<std::uint32_t>(entry.section_kind))) {
            continue;
        }

        const auto payload = file_bytes.subspan(
            static_cast<std::size_t>(entry.payload_offset),
            static_cast<std::size_t>(entry.stored_size));
        if (crc32c(payload) != entry.payload_crc32c) {
            if (entry.requirement == SectionRequirement::Optional) {
                continue;
            }
            return core::Result<StagedLoad, PersistenceError>::failure(
                PersistenceError{PersistenceErrorCode::PayloadCrcMismatch,
                    static_cast<std::size_t>(entry.payload_offset)});
        }

        BinaryReader reader(payload);
        const auto format = reader.read_u32();
        const auto kind_echo = reader.read_u32();
        const auto schema_echo = reader.read_u32();
        const auto payload_flags = reader.read_u32();
        if (!format || !kind_echo || !schema_echo || !payload_flags
            || format.value() != payload_format_version_v1
            || kind_echo.value() != static_cast<std::uint32_t>(entry.section_kind)
            || schema_echo.value() != entry.schema_version
            || payload_flags.value() != 0U) {
            return core::Result<StagedLoad, PersistenceError>::failure(
                PersistenceError{PersistenceErrorCode::PayloadPrologueMismatch,
                    static_cast<std::size_t>(entry.payload_offset)});
        }

        const auto body_offset = reader.position();
        std::vector<std::byte> body(
            payload.begin() + static_cast<std::ptrdiff_t>(body_offset), payload.end());
        staged.sections.push_back(DecodedSection{
            entry.section_kind,
            entry.schema_version,
            std::move(body),
        });
    }

    return core::Result<StagedLoad, PersistenceError>::success(std::move(staged));
}

}  // namespace starforge::persistence
