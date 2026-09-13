#pragma once

#include "starforge/core/result.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace starforge::persistence {

enum class BinaryErrorCode : std::uint8_t {
    UnexpectedEnd,
    InvalidBoolean,
    InvalidStringLength,
};

struct BinaryError final {
    BinaryErrorCode code{};
    std::size_t offset{};
};

class BinaryWriter final {
public:
    void write_u8(std::uint8_t value);
    void write_u16(std::uint16_t value);
    void write_u32(std::uint32_t value);
    void write_u64(std::uint64_t value);
    void write_bytes(std::span<const std::byte> bytes);
    void write_string(std::string_view value);
    void write_zeroes(std::size_t count);

    [[nodiscard]] const std::vector<std::byte>& bytes() const noexcept { return bytes_; }
    [[nodiscard]] std::vector<std::byte> take() && noexcept { return std::move(bytes_); }

private:
    std::vector<std::byte> bytes_;
};

class BinaryReader final {
public:
    static constexpr std::uint32_t max_string_bytes = 1U << 20U;

    explicit BinaryReader(std::span<const std::byte> bytes) : bytes_(bytes) {}

    [[nodiscard]] core::Result<std::uint8_t, BinaryError> read_u8();
    [[nodiscard]] core::Result<std::uint16_t, BinaryError> read_u16();
    [[nodiscard]] core::Result<std::uint32_t, BinaryError> read_u32();
    [[nodiscard]] core::Result<std::uint64_t, BinaryError> read_u64();
    [[nodiscard]] core::Result<bool, BinaryError> read_bool();
    [[nodiscard]] core::Result<std::string, BinaryError> read_string();

    [[nodiscard]] std::size_t position() const noexcept { return cursor_; }
    [[nodiscard]] std::size_t remaining() const noexcept { return bytes_.size() - cursor_; }
    [[nodiscard]] bool fully_consumed() const noexcept { return cursor_ == bytes_.size(); }

private:
    [[nodiscard]] bool can_read(std::size_t count) const noexcept {
        return count <= bytes_.size() - cursor_;
    }

    std::span<const std::byte> bytes_;
    std::size_t cursor_{};
};

[[nodiscard]] std::uint32_t crc32c(std::span<const std::byte> bytes) noexcept;

enum class SaveKind : std::uint32_t {
    Manual = 1,
    Quick = 2,
    Autosave = 3,
};

enum class SectionKind : std::uint32_t {
    SaveMetadata = 1,
    SessionCore = 100,
    Player = 110,
    Station = 120,
    Crew = 130,
    InventoryResources = 140,
    Spacecraft = 150,
    Robots = 160,
    Missions = 170,
    RaidsDefense = 180,
    WorldFactions = 190,
    EconomyMarkets = 200,
    ProgressionKnowledge = 210,
    DynamicEvents = 220,
    RecoveryCommunications = 230,
    SimulationInfrastructure = 240,
    LocalContextContinuation = 250,
};

enum class SectionRequirement : std::uint32_t {
    Required = 1,
    Optional = 2,
};

enum class SectionCodec : std::uint32_t {
    None = 0,
};

inline constexpr std::uint32_t save_container_version_v1 = 1U;
inline constexpr std::uint32_t save_header_size_v1 = 128U;
inline constexpr std::uint32_t section_directory_entry_size_v1 = 64U;
inline constexpr std::uint32_t payload_format_version_v1 = 1U;
inline constexpr std::uint32_t endian_marker_v1 = 0x01020304U;
inline constexpr std::uint32_t header_flag_optional_metadata = 1U;
inline constexpr std::array<std::byte, 8> save_magic_v1{
    std::byte{'S'}, std::byte{'F'}, std::byte{'G'}, std::byte{'S'},
    std::byte{'A'}, std::byte{'V'}, std::byte{'E'}, std::byte{0},
};

struct SaveHeaderV1 final {
    std::uint32_t header_flags{};
    std::uint32_t application_save_compat_version{};
    std::uint64_t saved_simulation_tick{};
    std::uint64_t snapshot_sequence{};
    std::uint64_t logical_slot_id{};
    SaveKind save_kind{SaveKind::Manual};
    std::uint32_t section_count{};
    std::uint64_t directory_offset{save_header_size_v1};
    std::uint64_t directory_size{};
    std::uint64_t file_size{};
    std::uint32_t directory_crc32c{};
    std::array<std::uint8_t, 32> content_build_id_sha256{};
};

struct SectionDirectoryEntryV1 final {
    SectionKind section_kind{SectionKind::SaveMetadata};
    std::uint32_t schema_version{};
    SectionRequirement requirement{SectionRequirement::Optional};
    SectionCodec codec{SectionCodec::None};
    std::uint64_t payload_offset{};
    std::uint64_t stored_size{};
    std::uint64_t decoded_size{};
    std::uint32_t payload_crc32c{};
};

enum class ContainerPrimitiveErrorCode : std::uint8_t {
    InvalidHeader,
    HeaderCrcMismatch,
    InvalidDirectoryEntry,
};

struct ContainerPrimitiveError final {
    ContainerPrimitiveErrorCode code{};
    std::size_t offset{};
};

[[nodiscard]] bool is_valid_save_kind(SaveKind kind) noexcept;
[[nodiscard]] bool is_known_section_kind(std::uint32_t raw_kind) noexcept;
[[nodiscard]] SectionRequirement canonical_requirement(SectionKind kind) noexcept;

[[nodiscard]] std::array<std::byte, save_header_size_v1> encode_header_v1(const SaveHeaderV1& header);
[[nodiscard]] core::Result<SaveHeaderV1, ContainerPrimitiveError> decode_header_v1(
    std::span<const std::byte> bytes);

[[nodiscard]] std::array<std::byte, section_directory_entry_size_v1> encode_directory_entry_v1(
    const SectionDirectoryEntryV1& entry);
[[nodiscard]] core::Result<SectionDirectoryEntryV1, ContainerPrimitiveError> decode_directory_entry_v1(
    std::span<const std::byte> bytes);

}  // namespace starforge::persistence
