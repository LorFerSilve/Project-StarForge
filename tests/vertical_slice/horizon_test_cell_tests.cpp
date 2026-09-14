#include <starforge/persistence/persistence.hpp>
#include <starforge/transactions/coordinator.hpp>
#include <starforge/vertical_slice/horizon_test_cell.hpp>
#include <starforge/world/world.hpp>

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <cstdint>

namespace {
using starforge::vertical_slice::HorizonTestCell;
using starforge::vertical_slice::ItemOwner;

struct ScenarioCheckpoint final {
    std::uint64_t persistent_id{};
    std::uint64_t revision{};
    std::uint64_t transfer_count{};
    ItemOwner owner{ItemOwner::WorldContainer};
};

ScenarioCheckpoint run_transfer_scenario(const std::uint32_t render_frames_between_ticks) {
    // Render cadence is deliberately an observation-only input: authoritative
    // transfer occurs through the transaction coordinator, never per frame.
    REQUIRE(render_frames_between_ticks > 0U);
    starforge::world::RuntimeEntityRegistry registry{101U};
    starforge::transactions::TransactionCoordinator coordinator;
    HorizonTestCell cell;
    const auto handle = cell.enter_context(registry);
    REQUIRE(handle.valid());
    REQUIRE(registry.contains(handle));

    for (std::uint32_t frame = 0U; frame < render_frames_between_ticks; ++frame) {
        const auto observed = cell.read_model();
        REQUIRE(observed.owner == ItemOwner::WorldContainer);
    }

    const auto transferred = cell.transfer(ItemOwner::PlayerInventory, coordinator, registry);
    REQUIRE(transferred.has_value());
    const auto state = cell.read_model();
    return {state.persistent_id.raw(), state.revision.raw(), state.committed_transfer_count, state.owner};
}
}  // namespace

TEST_CASE("Horizon item has exactly one owner across a typed transaction") {
    starforge::world::RuntimeEntityRegistry registry{1U};
    starforge::transactions::TransactionCoordinator coordinator;
    HorizonTestCell cell;

    const auto runtime = cell.enter_context(registry);
    REQUIRE(runtime.valid());
    REQUIRE(registry.size() == 1U);
    REQUIRE(cell.read_model().owner == ItemOwner::WorldContainer);

    const auto result = cell.transfer(ItemOwner::PlayerInventory, coordinator, registry);
    REQUIRE(result.has_value());
    REQUIRE(coordinator.is_stable_save_boundary());
    REQUIRE(cell.read_model().owner == ItemOwner::PlayerInventory);
    REQUIRE(cell.read_model().committed_transfer_count == 1U);
    REQUIRE_FALSE(cell.read_model().runtime_handle.has_value());
    REQUIRE(registry.size() == 0U);

    const auto duplicate = cell.transfer(ItemOwner::PlayerInventory, coordinator, registry);
    REQUIRE_FALSE(duplicate.has_value());
    REQUIRE(cell.read_model().committed_transfer_count == 1U);
}

TEST_CASE("save load preserves persistent truth without serializing runtime handles") {
    starforge::transactions::TransactionCoordinator coordinator;
    HorizonTestCell original;
    starforge::world::RuntimeEntityRegistry first_registry{7U};

    const auto first_handle = original.enter_context(first_registry);
    REQUIRE(first_handle.valid());
    const auto saved = original.save(4242U, coordinator);
    REQUIRE(saved.has_value());

    const auto staged = starforge::persistence::stage_load_v1(saved.value());
    REQUIRE(staged.has_value());
    REQUIRE(staged.value().header.saved_simulation_tick == 4242U);

    original.leave_context(first_registry);
    REQUIRE(first_registry.size() == 0U);

    auto loaded_result = HorizonTestCell::load(saved.value());
    REQUIRE(loaded_result.has_value());
    auto loaded = std::move(loaded_result).value();
    REQUIRE(loaded.read_model().persistent_id == original.read_model().persistent_id);
    REQUIRE(loaded.read_model().owner == ItemOwner::WorldContainer);
    REQUIRE_FALSE(loaded.read_model().runtime_handle.has_value());

    starforge::world::RuntimeEntityRegistry second_registry{8U};
    const auto second_handle = loaded.enter_context(second_registry);
    REQUIRE(second_handle.valid());
    REQUIRE(second_handle.scene_generation != first_handle.scene_generation);
    REQUIRE(loaded.read_model().persistent_id == original.read_model().persistent_id);
    REQUIRE(second_registry.size() == 1U);
}

TEST_CASE("committed inventory ownership survives reload and does not respawn") {
    starforge::transactions::TransactionCoordinator coordinator;
    starforge::world::RuntimeEntityRegistry first_registry{11U};
    HorizonTestCell cell;
    static_cast<void>(cell.enter_context(first_registry));

    REQUIRE(cell.transfer(ItemOwner::PlayerInventory, coordinator, first_registry).has_value());
    const auto saved = cell.save(9001U, coordinator);
    REQUIRE(saved.has_value());

    auto loaded_result = HorizonTestCell::load(saved.value());
    REQUIRE(loaded_result.has_value());
    auto loaded = std::move(loaded_result).value();
    REQUIRE(loaded.read_model().owner == ItemOwner::PlayerInventory);
    REQUIRE(loaded.read_model().committed_transfer_count == 1U);

    starforge::world::RuntimeEntityRegistry replacement_registry{12U};
    const auto handle = loaded.enter_context(replacement_registry);
    REQUIRE_FALSE(handle.valid());
    REQUIRE(replacement_registry.size() == 0U);
}

TEST_CASE("authoritative Horizon checkpoints are independent of render cadence") {
    const auto low_cadence = run_transfer_scenario(1U);
    const auto high_cadence = run_transfer_scenario(240U);

    REQUIRE(low_cadence.persistent_id == high_cadence.persistent_id);
    REQUIRE(low_cadence.revision == high_cadence.revision);
    REQUIRE(low_cadence.transfer_count == high_cadence.transfer_count);
    REQUIRE(low_cadence.owner == high_cadence.owner);
}
