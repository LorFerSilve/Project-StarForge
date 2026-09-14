#include <starforge/vertical_slice/horizon_test_cell.hpp>

#include <starforge/persistence/primitives.hpp>

#include <array>

namespace starforge::vertical_slice {
namespace {
constexpr std::uint32_t slice_magic = 0x354D5049U;  // "IMP5" in little endian storage.
constexpr std::uint32_t slice_schema_version = 1U;

[[nodiscard]] SliceError map_transaction_error() noexcept {
    return SliceError::TransactionRejected;
}
}  // namespace

HorizonTestCell::HorizonTestCell(HorizonItemId item_id) : item_id_{item_id} {
    if (!item_id_.valid()) {
        item_id_ = HorizonItemId{1U};
    }
}

HorizonItemReadModel HorizonTestCell::read_model() const noexcept {
    return {
        .persistent_id = item_id_,
        .owner = owner_,
        .revision = revision_,
        .committed_transfer_count = committed_transfer_count_,
        .runtime_handle = runtime_handle_,
    };
}

world::RuntimeEntityHandle HorizonTestCell::enter_context(world::RuntimeEntityRegistry& registry) {
    if (runtime_handle_.has_value() && registry.contains(*runtime_handle_)) {
        return *runtime_handle_;
    }
    runtime_handle_.reset();
    if (owner_ == ItemOwner::WorldContainer) {
        runtime_handle_ = registry.create();
        return *runtime_handle_;
    }
    return {};
}

void HorizonTestCell::leave_context(world::RuntimeEntityRegistry& registry) {
    if (runtime_handle_.has_value() && registry.contains(*runtime_handle_)) {
        registry.destroy(*runtime_handle_);
    }
    runtime_handle_.reset();
}

core::Result<void, SliceError> HorizonTestCell::transfer(
    ItemOwner destination,
    transactions::TransactionCoordinator& coordinator,
    world::RuntimeEntityRegistry& registry) {
    if (destination != ItemOwner::WorldContainer && destination != ItemOwner::PlayerInventory) {
        return core::Result<void, SliceError>::failure(SliceError::InvalidOwner);
    }
    if (prepared_owner_.has_value()) {
        return core::Result<void, SliceError>::failure(SliceError::TransferAlreadyPrepared);
    }
    if (destination == owner_) {
        return core::Result<void, SliceError>::failure(SliceError::InvalidTransfer);
    }

    prepared_owner_ = destination;
    prepared_revision_ = revision_;
    const auto candidate = coordinator.candidate_transaction_id();
    if (!candidate) {
        prepared_owner_.reset();
        return core::Result<void, SliceError>::failure(map_transaction_error());
    }

    std::array<transactions::IPreparedTransactionParticipant*, 1> participants{this};
    const auto prepared = coordinator.prepare(candidate.value(), participants);
    if (!prepared) {
        prepared_owner_.reset();
        return core::Result<void, SliceError>::failure(map_transaction_error());
    }

    const auto result = coordinator.commit();
    if (!result.committed) {
        prepared_owner_.reset();
        return core::Result<void, SliceError>::failure(map_transaction_error());
    }

    if (owner_ == ItemOwner::PlayerInventory) {
        if (runtime_handle_.has_value() && registry.contains(*runtime_handle_)) {
            registry.destroy(*runtime_handle_);
        }
        runtime_handle_.reset();
    } else if (!runtime_handle_.has_value()) {
        runtime_handle_ = registry.create();
    }
    return core::Result<void, SliceError>::success();
}

core::Result<std::vector<std::byte>, SliceError> HorizonTestCell::save(
    std::uint64_t simulation_tick,
    const transactions::TransactionCoordinator& coordinator) const {
    if (!coordinator.is_stable_save_boundary()) {
        return core::Result<std::vector<std::byte>, SliceError>::failure(SliceError::NotStableSaveBoundary);
    }

    persistence::BinaryWriter writer;
    writer.write_u32(slice_magic);
    writer.write_u32(slice_schema_version);
    writer.write_u64(item_id_.raw());
    writer.write_u8(static_cast<std::uint8_t>(owner_));
    writer.write_u64(revision_.raw());
    writer.write_u64(committed_transfer_count_);

    persistence::SaveSnapshot snapshot{
        .saved_simulation_tick = simulation_tick,
        .snapshot_sequence = 1U,
        .logical_slot_id = 1U,
        .save_kind = persistence::SaveKind::Manual,
        .application_save_compat_version = 1U,
        .content_build_id_sha256 = {},
        .sections = {{
            .kind = persistence::SectionKind::InventoryResources,
            .schema_version = slice_schema_version,
            .body = std::move(writer).take(),
        }},
    };

    const auto encoded = persistence::encode_save_v1(snapshot);
    if (!encoded) {
        return core::Result<std::vector<std::byte>, SliceError>::failure(SliceError::PersistenceFailure);
    }
    return core::Result<std::vector<std::byte>, SliceError>::success(encoded.value());
}

core::Result<HorizonTestCell, SliceError> HorizonTestCell::load(std::span<const std::byte> bytes) {
    const auto staged = persistence::stage_load_v1(bytes);
    if (!staged) {
        return core::Result<HorizonTestCell, SliceError>::failure(SliceError::PersistenceFailure);
    }

    const persistence::DecodedSection* slice_section = nullptr;
    for (const auto& section : staged.value().sections) {
        if (section.kind != persistence::SectionKind::InventoryResources) {
            continue;
        }
        if (slice_section != nullptr) {
            return core::Result<HorizonTestCell, SliceError>::failure(SliceError::DuplicateSliceSection);
        }
        slice_section = &section;
    }
    if (slice_section == nullptr) {
        return core::Result<HorizonTestCell, SliceError>::failure(SliceError::MissingSliceSection);
    }
    return decode_section(*slice_section);
}

transactions::DomainCommitKey HorizonTestCell::commit_key() const noexcept {
    return {.domain = transactions::DomainCommitOrder::Inventory, .stable_ordinal = 0U};
}

core::StateRevision HorizonTestCell::expected_revision() const noexcept {
    return prepared_revision_;
}

core::StateRevision HorizonTestCell::current_revision() const noexcept {
    return revision_;
}

void HorizonTestCell::commit() noexcept {
    if (!prepared_owner_.has_value()) {
        return;
    }
    owner_ = *prepared_owner_;
    static_cast<void>(revision_.advance());
    ++committed_transfer_count_;
}

void HorizonTestCell::publish_committed_events() noexcept {
    prepared_owner_.reset();
}

core::Result<HorizonTestCell, SliceError> HorizonTestCell::decode_section(
    const persistence::DecodedSection& section) {
    if (section.schema_version != slice_schema_version) {
        return core::Result<HorizonTestCell, SliceError>::failure(SliceError::InvalidSlicePayload);
    }

    persistence::BinaryReader reader{section.body};
    const auto magic = reader.read_u32();
    const auto version = reader.read_u32();
    const auto item_id = reader.read_u64();
    const auto owner = reader.read_u8();
    const auto revision = reader.read_u64();
    const auto transfer_count = reader.read_u64();
    if (!magic || !version || !item_id || !owner || !revision || !transfer_count || !reader.fully_consumed() ||
        magic.value() != slice_magic || version.value() != slice_schema_version || item_id.value() == 0U ||
        (owner.value() != static_cast<std::uint8_t>(ItemOwner::WorldContainer) &&
         owner.value() != static_cast<std::uint8_t>(ItemOwner::PlayerInventory))) {
        return core::Result<HorizonTestCell, SliceError>::failure(SliceError::InvalidSlicePayload);
    }

    HorizonTestCell result{HorizonItemId{item_id.value()}};
    result.owner_ = static_cast<ItemOwner>(owner.value());
    result.revision_ = core::StateRevision{revision.value()};
    result.committed_transfer_count_ = transfer_count.value();
    result.runtime_handle_.reset();
    return core::Result<HorizonTestCell, SliceError>::success(std::move(result));
}

}  // namespace starforge::vertical_slice
