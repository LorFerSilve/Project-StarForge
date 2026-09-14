#pragma once

#include <starforge/core/strong_id.hpp>
#include <starforge/world/world.hpp>

#include <algorithm>
#include <cstdint>
#include <optional>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

namespace starforge::gameplay {

struct ItemTag final {};
struct StatusTag final {};
using ItemId = core::PersistentId<ItemTag>;
using StatusId = core::PersistentId<StatusTag>;

using AttackInstanceId = std::uint64_t;

enum class ItemOwner : std::uint8_t {
    World,
    PlayerInventory,
};

enum class EquipmentSlot : std::uint8_t {
    PrimaryWeapon,
};

enum class RuntimeLifecycle : std::uint8_t {
    Active,
    Closing,
    PendingDestroy,
};

enum class ProjectileKind : std::uint8_t {
    Hitscan,
    Swept,
    Dynamic,
};

enum class GameplayError : std::uint8_t {
    InvalidEntity,
    DuplicateItem,
    UnknownItem,
    OwnershipMismatch,
    ItemNotEquipped,
    MissingComponent,
    CannotAct,
    NoAmmunition,
    WeaponCoolingDown,
    StaleTarget,
    InvalidValue,
};

struct ItemRecord final {
    ItemId id{};
    ItemOwner owner{ItemOwner::World};
    std::uint32_t quantity{1U};
};

class InventoryAuthority final {
public:
    [[nodiscard]] bool register_item(ItemRecord record);
    [[nodiscard]] bool transfer(ItemId item, ItemOwner expected_owner, ItemOwner destination);
    [[nodiscard]] std::optional<ItemRecord> find(ItemId item) const noexcept;
    [[nodiscard]] std::size_t item_count() const noexcept { return items_.size(); }

private:
    std::vector<ItemRecord> items_;
};

struct HealthRuntimeComponent final {
    std::int32_t current{100};
    std::int32_t maximum{100};
    bool incapacitated{false};
};

struct ShieldRuntimeComponent final {
    std::int32_t current{0};
    std::int32_t maximum{0};
};

struct CombatActorComponent final {
    std::uint32_t faction{0U};
    bool combat_enabled{true};
};

struct EquipmentRuntimeComponent final {
    std::optional<ItemId> primary_weapon{};
};

struct WeaponRuntimeComponent final {
    ItemId item{};
    ProjectileKind projectile_kind{ProjectileKind::Hitscan};
    std::uint32_t ammunition{0U};
    std::uint32_t magazine_capacity{0U};
    std::uint32_t cooldown_ticks{0U};
    std::uint32_t cooldown_remaining{0U};
    std::int32_t damage{0};
    std::uint32_t projectile_lifetime_ticks{1U};
};

struct StatusInstance final {
    StatusId id{};
    std::int32_t damage_per_tick{0};
    std::uint32_t period_ticks{1U};
    std::uint32_t ticks_until_next{1U};
    std::uint32_t remaining_ticks{0U};
    std::uint64_t ordering_key{0U};
};

struct StatusSetComponent final {
    std::vector<StatusInstance> statuses;
};

struct ProjectileComponent final {
    AttackInstanceId attack_id{0U};
    world::RuntimeEntityHandle source{};
    std::optional<world::RuntimeEntityHandle> target{};
    ProjectileKind kind{ProjectileKind::Swept};
    std::int32_t damage{0};
    std::uint32_t remaining_ticks{1U};
    bool consequence_committed{false};
};

struct CombatResult final {
    AttackInstanceId attack_id{0U};
    std::int32_t shield_damage{0};
    std::int32_t health_damage{0};
    bool incapacitated{false};
    bool projectile_spawned{false};
    std::optional<world::RuntimeEntityHandle> projectile{};
};

struct ActorSaveFacet final {
    std::int32_t health{0};
    std::int32_t shield{0};
    bool incapacitated{false};
    std::vector<StatusInstance> persistent_statuses;
};

template <typename Component>
class ComponentPool final {
public:
    [[nodiscard]] bool contains(world::RuntimeEntityHandle handle) const noexcept {
        if (handle.index >= slots_.size()) {
            return false;
        }
        const auto& slot = slots_[handle.index];
        return slot.has_value() && slot->generation == handle.generation &&
               slot->scene_generation == handle.scene_generation;
    }

    Component& emplace(world::RuntimeEntityHandle handle, Component component) {
        if (handle.index >= slots_.size()) {
            slots_.resize(static_cast<std::size_t>(handle.index) + 1U);
        }
        auto& slot = slots_[handle.index];
        slot = Slot{handle.scene_generation, handle.generation, std::move(component)};
        return slot->value;
    }

    [[nodiscard]] Component* get(world::RuntimeEntityHandle handle) noexcept {
        if (!contains(handle)) {
            return nullptr;
        }
        return &slots_[handle.index]->value;
    }

    [[nodiscard]] const Component* get(world::RuntimeEntityHandle handle) const noexcept {
        if (!contains(handle)) {
            return nullptr;
        }
        return &slots_[handle.index]->value;
    }

    void remove(world::RuntimeEntityHandle handle) noexcept {
        if (contains(handle)) {
            slots_[handle.index].reset();
        }
    }

private:
    struct Slot final {
        std::uint64_t scene_generation{0U};
        std::uint32_t generation{0U};
        Component value{};
    };
    std::vector<std::optional<Slot>> slots_;
};

class RuntimeGameplay final {
public:
    RuntimeGameplay(world::RuntimeEntityRegistry& registry, InventoryAuthority& inventory);

    [[nodiscard]] world::RuntimeEntityHandle create_actor(
        HealthRuntimeComponent health,
        ShieldRuntimeComponent shield = {},
        CombatActorComponent actor = {});
    [[nodiscard]] world::RuntimeEntityHandle create_weapon_proxy(WeaponRuntimeComponent weapon);

    [[nodiscard]] bool equip_primary(world::RuntimeEntityHandle actor, ItemId item,
                                     world::RuntimeEntityHandle weapon_proxy);
    [[nodiscard]] bool pickup(ItemId item);

    [[nodiscard]] CombatResult fire(world::RuntimeEntityHandle actor,
                                    std::optional<world::RuntimeEntityHandle> target);
    void apply_status(world::RuntimeEntityHandle target, StatusInstance status);
    void fixed_tick();

    void request_destroy(world::RuntimeEntityHandle entity);
    void flush_deferred_destruction();

    [[nodiscard]] RuntimeLifecycle lifecycle(world::RuntimeEntityHandle entity) const;
    [[nodiscard]] const HealthRuntimeComponent* health(world::RuntimeEntityHandle entity) const noexcept;
    [[nodiscard]] const ShieldRuntimeComponent* shield(world::RuntimeEntityHandle entity) const noexcept;
    [[nodiscard]] const EquipmentRuntimeComponent* equipment(world::RuntimeEntityHandle entity) const noexcept;
    [[nodiscard]] const WeaponRuntimeComponent* weapon(world::RuntimeEntityHandle entity) const noexcept;
    [[nodiscard]] const ProjectileComponent* projectile(world::RuntimeEntityHandle entity) const noexcept;
    [[nodiscard]] ActorSaveFacet export_actor(world::RuntimeEntityHandle entity) const;

private:
    struct LifecycleRecord final {
        world::RuntimeEntityHandle entity{};
        RuntimeLifecycle state{RuntimeLifecycle::Active};
    };

    [[nodiscard]] bool active(world::RuntimeEntityHandle entity) const noexcept;
    [[nodiscard]] RuntimeLifecycle* lifecycle_ptr(world::RuntimeEntityHandle entity) noexcept;
    [[nodiscard]] const RuntimeLifecycle* lifecycle_ptr(world::RuntimeEntityHandle entity) const noexcept;
    [[nodiscard]] CombatResult commit_damage(AttackInstanceId attack_id,
                                             world::RuntimeEntityHandle target,
                                             std::int32_t damage);
    void tick_statuses();
    void tick_weapons();
    void tick_projectiles();
    void remove_all_components(world::RuntimeEntityHandle entity) noexcept;

    world::RuntimeEntityRegistry& registry_;
    InventoryAuthority& inventory_;
    std::vector<LifecycleRecord> lifecycles_;
    ComponentPool<HealthRuntimeComponent> health_;
    ComponentPool<ShieldRuntimeComponent> shields_;
    ComponentPool<CombatActorComponent> actors_;
    ComponentPool<EquipmentRuntimeComponent> equipment_;
    ComponentPool<WeaponRuntimeComponent> weapons_;
    ComponentPool<StatusSetComponent> statuses_;
    ComponentPool<ProjectileComponent> projectiles_;
    std::vector<std::pair<ItemId, world::RuntimeEntityHandle>> weapon_proxies_;
    AttackInstanceId next_attack_id_{1U};
    std::uint64_t next_status_ordering_key_{1U};
};

}  // namespace starforge::gameplay
