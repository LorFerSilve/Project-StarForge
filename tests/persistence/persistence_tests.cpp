#include "starforge/persistence/content_codec.hpp"
#include "starforge/persistence/persistence.hpp"
#include "starforge/persistence/primitives.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

namespace {

[[nodiscard]] std::string hex_encode(const std::vector<std::byte>& bytes) {
    constexpr char digits[] = "0123456789abcdef";
    std::string result;
    result.reserve(bytes.size() * 2U);
    for (const auto byte : bytes) {
        const auto value = std::to_integer<std::uint8_t>(byte);
        result.push_back(digits[value >> 4U]);
        result.push_back(digits[value & 0x0FU]);
    }
    return result;
}

[[nodiscard]] std::string read_golden_hex() {
    std::ifstream input(
        std::string(STARFORGE_TEST_SOURCE_DIR) + "/fixtures/imp2_minimal_save_v1.hex",
        std::ios::binary);
    return std::string(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>());
}

[[nodiscard]] starforge::persistence::SaveSnapshot minimal_snapshot() {
    using namespace starforge::persistence;

    SaveSnapshot snapshot{};
    snapshot.saved_simulation_tick = 42U;
    snapshot.snapshot_sequence = 7U;
    snapshot.logical_slot_id = 9U;
    snapshot.save_kind = SaveKind::Manual;
    snapshot.application_save_compat_version = 1U;
    for (std::size_t index = 0; index < snapshot.content_build_id_sha256.size(); ++index) {
        snapshot.content_build_id_sha256[index] = static_cast<std::uint8_t>(index);
    }
    snapshot.sections.push_back(SaveSection{
        SectionKind::SessionCore,
        1U,
        {std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF}},
    });
    return snapshot;
}

}  // namespace

TEST_CASE("Persistence.Primitives.Crc32cUsesCastagnoliPolynomial", "[persistence]") {
    const std::string reference = "123456789";
    const auto bytes = std::as_bytes(std::span(reference.data(), reference.size()));
    REQUIRE(starforge::persistence::crc32c(bytes) == 0xE3069283U);
}

TEST_CASE("Persistence.ContentId.EncodesCanonicalLogicalIdentity", "[persistence]") {
    using namespace starforge;

    auto content_id = content::ContentId::parse("weapon.ballistic.rifle_mk1");
    REQUIRE(content_id);

    persistence::BinaryWriter writer;
    persistence::write_content_id(writer, content_id.value());
    persistence::BinaryReader reader(writer.bytes());
    const auto decoded = persistence::read_content_id(reader);
    REQUIRE(decoded);
    REQUIRE(decoded.value() == content_id.value());
    REQUIRE(reader.fully_consumed());

    persistence::BinaryWriter invalid_writer;
    invalid_writer.write_string("Weapon.invalid");
    persistence::BinaryReader invalid_reader(invalid_writer.bytes());
    const auto invalid = persistence::read_content_id(invalid_reader);
    REQUIRE_FALSE(invalid);
    REQUIRE(invalid.error().code == persistence::ContentCodecErrorCode::InvalidContentId);
}

TEST_CASE("Persistence.Container.MinimalV1MatchesExactGolden", "[persistence]") {
    using namespace starforge::persistence;

    const auto encoded = encode_save_v1(minimal_snapshot());
    REQUIRE(encoded);

    auto golden = read_golden_hex();
    while (!golden.empty() && (golden.back() == '\n' || golden.back() == '\r')) {
        golden.pop_back();
    }
    REQUIRE(hex_encode(encoded.value()) == golden);

    const auto reencoded = encode_save_v1(minimal_snapshot());
    REQUIRE(reencoded);
    REQUIRE(reencoded.value() == encoded.value());
}

TEST_CASE("Persistence.Container.CurrentVersionRoundTripsThroughStaging", "[persistence]") {
    using namespace starforge::persistence;

    const auto encoded = encode_save_v1(minimal_snapshot());
    REQUIRE(encoded);

    const auto staged = stage_load_v1(encoded.value());
    REQUIRE(staged);
    REQUIRE(staged.value().header.saved_simulation_tick == 42U);
    REQUIRE(staged.value().header.snapshot_sequence == 7U);
    REQUIRE(staged.value().sections.size() == 1U);
    REQUIRE(staged.value().sections.front().kind == SectionKind::SessionCore);
    REQUIRE(staged.value().sections.front().schema_version == 1U);
    REQUIRE(staged.value().sections.front().body
        == std::vector<std::byte>{std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF}});
}

TEST_CASE("Persistence.Container.RequiredPayloadCorruptionIsRejected", "[persistence]") {
    using namespace starforge::persistence;

    auto encoded = encode_save_v1(minimal_snapshot());
    REQUIRE(encoded);
    encoded.value().back() ^= std::byte{0x01};

    const auto staged = stage_load_v1(encoded.value());
    REQUIRE_FALSE(staged);
    REQUIRE(staged.error().code == PersistenceErrorCode::PayloadCrcMismatch);
}

TEST_CASE("Persistence.Container.DirectoryCorruptionIsRejectedBeforePayloadDecode", "[persistence]") {
    using namespace starforge::persistence;

    auto encoded = encode_save_v1(minimal_snapshot());
    REQUIRE(encoded);
    encoded.value()[128U] ^= std::byte{0x01};

    const auto staged = stage_load_v1(encoded.value());
    REQUIRE_FALSE(staged);
    REQUIRE(staged.error().code == PersistenceErrorCode::DirectoryCrcMismatch);
}

TEST_CASE("Persistence.Container.TruncatedInputDoesNotReachSemanticDecode", "[persistence]") {
    using namespace starforge::persistence;

    auto encoded = encode_save_v1(minimal_snapshot());
    REQUIRE(encoded);
    encoded.value().resize(100U);

    const auto staged = stage_load_v1(encoded.value());
    REQUIRE_FALSE(staged);
    REQUIRE(staged.error().code == PersistenceErrorCode::InvalidHeader);
}

TEST_CASE("Persistence.Primitives.BooleanAndBoundsAreStrict", "[persistence]") {
    using namespace starforge::persistence;

    const std::vector<std::byte> invalid_bool{std::byte{2U}};
    BinaryReader bool_reader(invalid_bool);
    const auto value = bool_reader.read_bool();
    REQUIRE_FALSE(value);
    REQUIRE(value.error().code == BinaryErrorCode::InvalidBoolean);

    const std::vector<std::byte> short_u64{std::byte{1U}, std::byte{2U}};
    BinaryReader integer_reader(short_u64);
    const auto integer = integer_reader.read_u64();
    REQUIRE_FALSE(integer);
    REQUIRE(integer.error().code == BinaryErrorCode::UnexpectedEnd);
}
