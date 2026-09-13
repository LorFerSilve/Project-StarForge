#pragma once

#include "starforge/core/result.hpp"
#include "starforge/persistence/primitives.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace starforge::persistence {

enum class PersistenceErrorCode : std::uint8_t {
    InvalidSnapshot,
    InvalidHeader,
    HeaderCrcMismatch,
    InvalidDirectory,
    DirectoryCrcMismatch,
    UnsupportedRequiredSection,
    PayloadBounds,
    PayloadCrcMismatch,
    PayloadPrologueMismatch,
    TrailingBytes,
    SafetyLimitExceeded,
};

struct PersistenceError final {
    PersistenceErrorCode code{};
    std::size_t offset{};
};

struct SaveSection final {
    SectionKind kind{SectionKind::SessionCore};
    std::uint32_t schema_version{1U};
    std::vector<std::byte> body;
};

struct SaveSnapshot final {
    std::uint64_t saved_simulation_tick{};
    std::uint64_t snapshot_sequence{1U};
    std::uint64_t logical_slot_id{1U};
    SaveKind save_kind{SaveKind::Manual};
    std::uint32_t application_save_compat_version{1U};
    std::array<std::uint8_t, 32> content_build_id_sha256{};
    std::vector<SaveSection> sections;
};

struct DecodedSection final {
    SectionKind kind{SectionKind::SessionCore};
    std::uint32_t schema_version{};
    std::vector<std::byte> body;
};

struct StagedLoad final {
    SaveHeaderV1 header;
    std::vector<DecodedSection> sections;
};

class SaveSectionExporter {
public:
    virtual ~SaveSectionExporter() = default;
    [[nodiscard]] virtual core::Result<SaveSection, PersistenceError> export_section() const = 0;
};

class StagingSectionImporter {
public:
    virtual ~StagingSectionImporter() = default;
    [[nodiscard]] virtual core::Result<void, PersistenceError> import_to_staging(
        const DecodedSection& section) = 0;
};

[[nodiscard]] core::Result<std::vector<std::byte>, PersistenceError> encode_save_v1(
    const SaveSnapshot& snapshot);

[[nodiscard]] core::Result<StagedLoad, PersistenceError> stage_load_v1(
    std::span<const std::byte> file_bytes);

}  // namespace starforge::persistence
