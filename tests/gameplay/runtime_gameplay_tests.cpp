#include <starforge/gameplay/runtime_gameplay.hpp>

#include <catch2/catch_test_macros.hpp>

namespace gameplay = starforge::gameplay;
namespace world = starforge::world;

TEST_CASE("inventory ownership is conserved and equipment is only a reference") {
    world::RuntimeEntityRegistry registry{41U};
    gameplay::InventoryAuthority inventory;
    REQUIRE(inventory.register_item({.id = gameplay::ItemId{7U}, .owner = gameplay::ItemOwner::World}));

    gameplay::RuntimeGameplay runtime{registry, inventory};
    const auto actor = runtime.create_actor({.current = 100, .maximum = 100});
    const auto weapon = runtime.create_weapon_proxy({
        .item = gameplay::ItemId{7U},
        .ammunition = 4U,
        .magazine_capacity = 4U,
        .damage = 10,
    });

    REQUIRE(runtime.pickup(gameplay::ItemId{7U}));
    REQUIRE_FALSE(runtime.pickup(gameplay::ItemId{7U}));
    REQUIRE(runtime.equip_primary(actor, gameplay::ItemId{7U}, weapon));

    const auto item = inventory.find(gameplay::ItemId{7U});
    REQUIRE(item.has_value());
    CHECK(item->owner == gameplay::ItemOwner::PlayerInventory);
    REQUIRE(runtime.equipment(actor) != nullptr);
    CHECK(runtime.equipment(actor)->primary_weapon == gameplay::ItemId{7U});
    CHECK(inventory.item_count() == 1U);
}

TEST_CASE("hitscan commits fire cost before shield and health consequences") {
    world::RuntimeEntityRegistry registry{42U};
    gameplay::InventoryAuthority inventory;
    REQUIRE(inventory.register_item({.id = gameplay::ItemId{8U}, .owner = gameplay::ItemOwner::PlayerInventory}));

    gameplay::RuntimeGameplay runtime{registry, inventory};
    const auto attacker = runtime.create_actor({.current = 100, .maximum = 100});
    const auto target = runtime.create_actor({.current = 50, .maximum = 50}, {.current = 10, .maximum = 10});
    const auto weapon_proxy = runtime.create_weapon_proxy({
        .item = gameplay::ItemId{8U},
        .projectile_kind = gameplay::ProjectileKind::Hitscan,
        .ammunition = 2U,
        .magazine_capacity = 2U,
        .damage = 25,
    });
    REQUIRE(runtime.equip_primary(attacker, gameplay::ItemId{8U}, weapon_proxy));

    const auto hit = runtime.fire(attacker, target);
    CHECK(hit.shield_damage == 10);
    CHECK(hit.health_damage == 15);
    REQUIRE(runtime.weapon(weapon_proxy) != nullptr);
    CHECK(runtime.weapon(weapon_proxy)->ammunition == 1U);
    REQUIRE(runtime.health(target) != nullptr);
    CHECK(runtime.health(target)->current == 35);

    const auto miss = runtime.fire(attacker, std::nullopt);
    CHECK(miss.health_damage == 0);
    CHECK(runtime.weapon(weapon_proxy)->ammunition == 0U);
}

TEST_CASE("periodic status progression uses fixed ticks deterministically") {
    world::RuntimeEntityRegistry registry{43U};
    gameplay::InventoryAuthority inventory;
    gameplay::RuntimeGameplay runtime{registry, inventory};
    const auto target = runtime.create_actor({.current = 30, .maximum = 30});

    runtime.apply_status(target, {
        .id = gameplay::StatusId{1U},
        .damage_per_tick = 3,
        .period_ticks = 2U,
        .ticks_until_next = 2U,
        .remaining_ticks = 5U,
    });

    for (int tick = 0; tick < 5; ++tick) {
        runtime.fixed_tick();
    }
    REQUIRE(runtime.health(target) != nullptr);
    CHECK(runtime.health(target)->current == 24);
    const auto exported = runtime.export_actor(target);
    CHECK(exported.health == 24);
    CHECK(exported.persistent_statuses.empty());
}

TEST_CASE("projectile consequence commits before deferred reclamation") {
    world::RuntimeEntityRegistry registry{44U};
    gameplay::InventoryAuthority inventory;
    REQUIRE(inventory.register_item({.id = gameplay::ItemId{9U}, .owner = gameplay::ItemOwner::PlayerInventory}));
    gameplay::RuntimeGameplay runtime{registry, inventory};
    const auto attacker = runtime.create_actor({.current = 100, .maximum = 100});
    const auto target = runtime.create_actor({.current = 40, .maximum = 40});
    const auto weapon_proxy = runtime.create_weapon_proxy({
        .item = gameplay::ItemId{9U},
        .projectile_kind = gameplay::ProjectileKind::Swept,
        .ammunition = 1U,
        .magazine_capacity = 1U,
        .damage = 12,
        .projectile_lifetime_ticks = 2U,
    });
    REQUIRE(runtime.equip_primary(attacker, gameplay::ItemId{9U}, weapon_proxy));

    const auto fired = runtime.fire(attacker, target);
    REQUIRE(fired.projectile_spawned);
    REQUIRE(fired.projectile.has_value());
    const auto projectile = *fired.projectile;
    REQUIRE(registry.contains(projectile));

    runtime.fixed_tick();
    CHECK(runtime.health(target)->current == 40);
    runtime.fixed_tick();
    CHECK(runtime.health(target)->current == 28);
    CHECK(runtime.lifecycle(projectile) == gameplay::RuntimeLifecycle::PendingDestroy);
    CHECK(registry.contains(projectile));

    runtime.flush_deferred_destruction();
    CHECK_FALSE(registry.contains(projectile));
    CHECK(runtime.projectile(projectile) == nullptr);
}

TEST_CASE("incapacitation does not immediately reclaim the target") {
    world::RuntimeEntityRegistry registry{45U};
    gameplay::InventoryAuthority inventory;
    REQUIRE(inventory.register_item({.id = gameplay::ItemId{10U}, .owner = gameplay::ItemOwner::PlayerInventory}));
    gameplay::RuntimeGameplay runtime{registry, inventory};
    const auto attacker = runtime.create_actor({.current = 100, .maximum = 100});
    const auto target = runtime.create_actor({.current = 10, .maximum = 10});
    const auto weapon_proxy = runtime.create_weapon_proxy({
        .item = gameplay::ItemId{10U},
        .ammunition = 1U,
        .magazine_capacity = 1U,
        .damage = 20,
    });
    REQUIRE(runtime.equip_primary(attacker, gameplay::ItemId{10U}, weapon_proxy));

    const auto result = runtime.fire(attacker, target);
    REQUIRE(result.incapacitated);
    CHECK(registry.contains(target));
    CHECK(runtime.lifecycle(target) == gameplay::RuntimeLifecycle::Active);
    REQUIRE(runtime.health(target) != nullptr);
    CHECK(runtime.health(target)->incapacitated);
}

TEST_CASE("stale generation cannot resolve old runtime component state") {
    world::RuntimeEntityRegistry registry{46U};
    gameplay::InventoryAuthority inventory;
    gameplay::RuntimeGameplay runtime{registry, inventory};
    const auto first = runtime.create_actor({.current = 10, .maximum = 10});
    runtime.request_destroy(first);
    runtime.flush_deferred_destruction();
    REQUIRE_FALSE(registry.contains(first));

    const auto replacement = runtime.create_actor({.current = 20, .maximum = 20});
    CHECK(replacement.index == first.index);
    CHECK(replacement.generation != first.generation);
    CHECK(runtime.health(first) == nullptr);
    REQUIRE(runtime.health(replacement) != nullptr);
    CHECK(runtime.health(replacement)->current == 20);
}
