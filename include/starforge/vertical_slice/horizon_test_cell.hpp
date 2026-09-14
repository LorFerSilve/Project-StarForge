#pragma once

#include <starforge/core/result.hpp>
#include <starforge/core/revision.hpp>
#include <starforge/core/strong_id.hpp>
#include <starforge/persistence/persistence.hpp>
#include <starforge/transactions/coordinator.hpp>
#include <starforge/world/world.hpp>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace starforge::vertical_slice {

struct HorizonItemTag final {};
using HorizonItemId = core::PersistentId<HorizonItemTag>;

enum class ItemOwner : std::uint8_t {
    WorldContainer = 1U,
    PlayerInventory = 2U,
};

enum class SliceError : std::uint8_t {
    InvalidPersistentId,
    InvalidOwner,
    TransferAlreadyPrepared,
    InvalidTransfer,
    TransactionRejected,
    NotStableSaveBoundary,
    PersistenceFailure,
    MissingSliceSection,
    DuplicateSliceSection,
    InvalidSlicePayload,
};

struct HorizonItemReadModel final {
    HorizonItemId persistent_id{};
    ItemOwner owner{ItemOwner::WorldContainer};
    core::StateRevision revision{};
    std::uint64_t committed_transfer_count{0U};
    std::optional<world::RuntimeEntityHandle> runtime_handle{};
};

class HorizonTestCell final : private transactions::IPreparedTransactionParticipant {
public:
    explicit HorizonTestCell(HorizonItemId item_id = HorizonItemId{1U});

    [[nodiscard]] HorizonItemReadModel read_model() const noexcept;

    [[nodiscard]] world::RuntimeEntityHandle enter_context(world::RuntimeEntityRegistry& registry);
    void leave_context(world::RuntimeEntityRegistry& registry);

    [[nodiscard]] core::Result<void, SliceError> transfer(
        ItemOwner destination,
        transactions::TransactionCoordinator& coordinator,
        world::RuntimeEntityRegistry& registry);

    [[nodiscard]] core::Result<std::vector<std::byte>, SliceError> save(
        std::uint64_t simulation_tick,
        const transactions::TransactionCoordinator& coordinator) const;

    [[nodiscard]] static core::Result<HorizonTestCell, SliceError> load(std::span<const std::byte> bytes);

private:
    [[nodiscard]] transactions::DomainCommitKey commit_key() const noexcept override;
    [[nodiscard]] core::StateRevision expected_revision() const noexcept override;
    [[nodiscard]] core::StateRevision current_revision() const noexcept override;
    void commit() noexcept override;
    void publish_committed_events() noexcept override;

    [[nodiscard]] static core::Result<HorizonTestCell, SliceError> decode_section(
        const persistence::DecodedSection& section);

    HorizonItemId item_id_{};
    ItemOwner owner_{ItemOwner::WorldContainer};
    core::StateRevision revision_{};
    std::uint64_t committed_transfer_count_{0U};
    std::optional<ItemOwner> prepared_owner_{};
    core::StateRevision prepared_revision_{};
    std::optional<world::RuntimeEntityHandle> runtime_handle_{};
};

}  // namespace starforge::vertical_slice
