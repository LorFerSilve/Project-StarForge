#include <starforge/gameplay/runtime_gameplay.hpp>

#include <limits>

namespace starforge::gameplay {

bool InventoryAuthority::register_item(ItemRecord record) {
    if (!record.id.valid() || record.quantity == 0U || find(record.id).has_value()) {
        return false;
    }
    items_.push_back(record);
    std::ranges::sort(items_, {}, [](const ItemRecord& item) { return item.id.raw(); });
    return true;
}

bool InventoryAuthority::transfer(ItemId item, ItemOwner expected_owner, ItemOwner destination) {
    const auto found = std::ranges::find(items_, item, &ItemRecord::id);
    if (found == items_.end() || found->owner != expected_owner) {
        return false;
    }
    found->owner = destination;
    return true;
}

std::optional<ItemRecord> InventoryAuthority::find(ItemId item) const noexcept {
    const auto found = std::ranges::find(items_, item, &ItemRecord::id);
    if (found == items_.end()) {
        return std::nullopt;
    }
    return *found;
}

RuntimeGameplay::RuntimeGameplay(world::RuntimeEntityRegistry& registry, InventoryAuthority& inventory)
    : registry_{registry}, inventory_{inventory} {}

world::RuntimeEntityHandle RuntimeGameplay::create_actor(
    HealthRuntimeComponent health, ShieldRuntimeComponent shield, CombatActorComponent actor) {
    if (health.maximum <= 0 || health.current < 0 || health.current > health.maximum ||
        shield.maximum < 0 || shield.current < 0 || shield.current > shield.maximum) {
        throw std::invalid_argument{"invalid actor health or shield state"};
    }
    const auto entity = registry_.create();
    lifecycles_.push_back({entity, RuntimeLifecycle::Active});
    health_.emplace(entity, health);
    shields_.emplace(entity, shield);
    actors_.emplace(entity, actor);
    equipment_.emplace(entity, EquipmentRuntimeComponent{});
    statuses_.emplace(entity, StatusSetComponent{});
    return entity;
}

world::RuntimeEntityHandle RuntimeGameplay::create_weapon_proxy(WeaponRuntimeComponent weapon) {
    if (!weapon.item.valid() || weapon.magazine_capacity == 0U || weapon.ammunition > weapon.magazine_capacity ||
        weapon.damage <= 0 || weapon.projectile_lifetime_ticks == 0U) {
        throw std::invalid_argument{"invalid weapon runtime state"};
    }
    const auto item = inventory_.find(weapon.item);
    if (!item.has_value()) {
        throw std::invalid_argument{"weapon proxy requires a persistent inventory item"};
    }
    const auto entity = registry_.create();
    lifecycles_.push_back({entity, RuntimeLifecycle::Active});
    weapons_.emplace(entity, weapon);
    weapon_proxies_.push_back({weapon.item, entity});
    std::ranges::sort(weapon_proxies_, {}, [](const auto& entry) { return entry.first.raw(); });
    return entity;
}

bool RuntimeGameplay::pickup(ItemId item) {
    return inventory_.transfer(item, ItemOwner::World, ItemOwner::PlayerInventory);
}

bool RuntimeGameplay::equip_primary(world::RuntimeEntityHandle actor, ItemId item,
                                    world::RuntimeEntityHandle weapon_proxy) {
    if (!active(actor) || !active(weapon_proxy)) {
        return false;
    }
    auto* equipment = equipment_.get(actor);
    const auto* weapon = weapons_.get(weapon_proxy);
    const auto persistent_item = inventory_.find(item);
    if (equipment == nullptr || weapon == nullptr || weapon->item != item || !persistent_item.has_value() ||
        persistent_item->owner != ItemOwner::PlayerInventory) {
        return false;
    }
    equipment->primary_weapon = item;
    return true;
}

CombatResult RuntimeGameplay::fire(world::RuntimeEntityHandle actor,
                                   std::optional<world::RuntimeEntityHandle> target) {
    if (!active(actor)) {
        throw std::invalid_argument{"actor is not active"};
    }
    const auto* actor_state = actors_.get(actor);
    const auto* actor_health = health_.get(actor);
    const auto* equipment = equipment_.get(actor);
    if (actor_state == nullptr || actor_health == nullptr || equipment == nullptr ||
        !actor_state->combat_enabled || actor_health->incapacitated || !equipment->primary_weapon.has_value()) {
        throw std::logic_error{"actor cannot fire"};
    }

    const auto proxy = std::ranges::find(weapon_proxies_, *equipment->primary_weapon,
                                         [](const auto& entry) { return entry.first; });
    if (proxy == weapon_proxies_.end() || !active(proxy->second)) {
        throw std::logic_error{"equipped weapon proxy is unavailable"};
    }
    auto* weapon = weapons_.get(proxy->second);
    if (weapon == nullptr) {
        throw std::logic_error{"equipped weapon component is unavailable"};
    }
    if (weapon->cooldown_remaining != 0U) {
        throw std::logic_error{"weapon is cooling down"};
    }
    if (weapon->ammunition == 0U) {
        throw std::logic_error{"weapon has no ammunition"};
    }

    --weapon->ammunition;
    weapon->cooldown_remaining = weapon->cooldown_ticks;
    const auto attack_id = next_attack_id_++;
    if (next_attack_id_ == 0U) {
        next_attack_id_ = 1U;
    }

    if (weapon->projectile_kind == ProjectileKind::Hitscan) {
        if (!target.has_value()) {
            return {.attack_id = attack_id};
        }
        if (!active(*target)) {
            throw std::invalid_argument{"stale combat target"};
        }
        return commit_damage(attack_id, *target, weapon->damage);
    }

    if (target.has_value() && !active(*target)) {
        throw std::invalid_argument{"stale combat target"};
    }
    const auto projectile_entity = registry_.create();
    lifecycles_.push_back({projectile_entity, RuntimeLifecycle::Active});
    projectiles_.emplace(projectile_entity, ProjectileComponent{
        .attack_id = attack_id,
        .source = actor,
        .target = target,
        .kind = weapon->projectile_kind,
        .damage = weapon->damage,
        .remaining_ticks = weapon->projectile_lifetime_ticks,
        .consequence_committed = false,
    });
    return {
        .attack_id = attack_id,
        .projectile_spawned = true,
        .projectile = projectile_entity,
    };
}

void RuntimeGameplay::apply_status(world::RuntimeEntityHandle target, StatusInstance status) {
    if (!active(target) || !status.id.valid() || status.period_ticks == 0U || status.remaining_ticks == 0U ||
        status.damage_per_tick < 0) {
        throw std::invalid_argument{"invalid status application"};
    }
    auto* set = statuses_.get(target);
    if (set == nullptr) {
        throw std::invalid_argument{"target has no status set"};
    }
    status.ticks_until_next = std::min(status.ticks_until_next, status.period_ticks);
    if (status.ticks_until_next == 0U) {
        status.ticks_until_next = status.period_ticks;
    }
    status.ordering_key = next_status_ordering_key_++;
    set->statuses.push_back(status);
    std::ranges::sort(set->statuses, {}, &StatusInstance::ordering_key);
}

void RuntimeGameplay::fixed_tick() {
    tick_weapons();
    tick_statuses();
    tick_projectiles();
}

void RuntimeGameplay::request_destroy(world::RuntimeEntityHandle entity) {
    auto* state = lifecycle_ptr(entity);
    if (state == nullptr || *state == RuntimeLifecycle::PendingDestroy) {
        return;
    }
    *state = RuntimeLifecycle::Closing;
    *state = RuntimeLifecycle::PendingDestroy;
}

void RuntimeGameplay::flush_deferred_destruction() {
    std::vector<world::RuntimeEntityHandle> pending;
    for (const auto& record : lifecycles_) {
        if (record.state == RuntimeLifecycle::PendingDestroy && registry_.contains(record.entity)) {
            pending.push_back(record.entity);
        }
    }
    std::ranges::sort(pending, {}, &world::RuntimeEntityHandle::index);
    for (const auto entity : pending) {
        remove_all_components(entity);
        registry_.destroy(entity);
    }
    std::erase_if(lifecycles_, [this](const LifecycleRecord& record) {
        return !registry_.contains(record.entity);
    });
    std::erase_if(weapon_proxies_, [this](const auto& entry) {
        return !registry_.contains(entry.second);
    });
}

RuntimeLifecycle RuntimeGameplay::lifecycle(world::RuntimeEntityHandle entity) const {
    const auto* state = lifecycle_ptr(entity);
    if (state == nullptr) {
        throw std::invalid_argument{"invalid runtime entity"};
    }
    return *state;
}

const HealthRuntimeComponent* RuntimeGameplay::health(world::RuntimeEntityHandle entity) const noexcept {
    return health_.get(entity);
}

const ShieldRuntimeComponent* RuntimeGameplay::shield(world::RuntimeEntityHandle entity) const noexcept {
    return shields_.get(entity);
}

const EquipmentRuntimeComponent* RuntimeGameplay::equipment(world::RuntimeEntityHandle entity) const noexcept {
    return equipment_.get(entity);
}

const WeaponRuntimeComponent* RuntimeGameplay::weapon(world::RuntimeEntityHandle entity) const noexcept {
    return weapons_.get(entity);
}

const ProjectileComponent* RuntimeGameplay::projectile(world::RuntimeEntityHandle entity) const noexcept {
    return projectiles_.get(entity);
}

ActorSaveFacet RuntimeGameplay::export_actor(world::RuntimeEntityHandle entity) const {
    const auto* health = health_.get(entity);
    const auto* shield = shields_.get(entity);
    const auto* statuses = statuses_.get(entity);
    if (health == nullptr || shield == nullptr || statuses == nullptr) {
        throw std::invalid_argument{"actor is missing required save facets"};
    }
    return {
        .health = health->current,
        .shield = shield->current,
        .incapacitated = health->incapacitated,
        .persistent_statuses = statuses->statuses,
    };
}

bool RuntimeGameplay::active(world::RuntimeEntityHandle entity) const noexcept {
    if (!registry_.contains(entity)) {
        return false;
    }
    const auto* state = lifecycle_ptr(entity);
    return state != nullptr && *state == RuntimeLifecycle::Active;
}

RuntimeLifecycle* RuntimeGameplay::lifecycle_ptr(world::RuntimeEntityHandle entity) noexcept {
    const auto found = std::ranges::find(lifecycles_, entity, &LifecycleRecord::entity);
    return found == lifecycles_.end() ? nullptr : &found->state;
}

const RuntimeLifecycle* RuntimeGameplay::lifecycle_ptr(world::RuntimeEntityHandle entity) const noexcept {
    const auto found = std::ranges::find(lifecycles_, entity, &LifecycleRecord::entity);
    return found == lifecycles_.end() ? nullptr : &found->state;
}

CombatResult RuntimeGameplay::commit_damage(AttackInstanceId attack_id,
                                            world::RuntimeEntityHandle target,
                                            std::int32_t damage) {
    auto* health = health_.get(target);
    auto* shield = shields_.get(target);
    if (health == nullptr || shield == nullptr || damage < 0) {
        throw std::invalid_argument{"target cannot receive combat damage"};
    }
    const auto shield_damage = std::min(shield->current, damage);
    shield->current -= shield_damage;
    const auto remaining = damage - shield_damage;
    const auto health_damage = std::min(health->current, remaining);
    health->current -= health_damage;
    if (health->current == 0) {
        health->incapacitated = true;
        if (auto* actor = actors_.get(target); actor != nullptr) {
            actor->combat_enabled = false;
        }
    }
    return {
        .attack_id = attack_id,
        .shield_damage = shield_damage,
        .health_damage = health_damage,
        .incapacitated = health->incapacitated,
    };
}

void RuntimeGameplay::tick_statuses() {
    std::vector<world::RuntimeEntityHandle> actors;
    for (const auto& record : lifecycles_) {
        if (record.state == RuntimeLifecycle::Active && statuses_.contains(record.entity)) {
            actors.push_back(record.entity);
        }
    }
    std::ranges::sort(actors, {}, &world::RuntimeEntityHandle::index);
    for (const auto entity : actors) {
        auto* set = statuses_.get(entity);
        if (set == nullptr) {
            continue;
        }
        for (auto& status : set->statuses) {
            if (status.remaining_ticks == 0U) {
                continue;
            }
            --status.remaining_ticks;
            if (status.ticks_until_next > 0U) {
                --status.ticks_until_next;
            }
            if (status.ticks_until_next == 0U) {
                static_cast<void>(commit_damage(next_attack_id_++, entity, status.damage_per_tick));
                status.ticks_until_next = status.period_ticks;
            }
        }
        std::erase_if(set->statuses, [](const StatusInstance& status) { return status.remaining_ticks == 0U; });
    }
}

void RuntimeGameplay::tick_weapons() {
    for (const auto& proxy : weapon_proxies_) {
        if (auto* weapon = weapons_.get(proxy.second); weapon != nullptr && weapon->cooldown_remaining > 0U) {
            --weapon->cooldown_remaining;
        }
    }
}

void RuntimeGameplay::tick_projectiles() {
    std::vector<world::RuntimeEntityHandle> active_projectiles;
    for (const auto& record : lifecycles_) {
        if (record.state == RuntimeLifecycle::Active && projectiles_.contains(record.entity)) {
            active_projectiles.push_back(record.entity);
        }
    }
    std::ranges::sort(active_projectiles, {}, &world::RuntimeEntityHandle::index);
    for (const auto entity : active_projectiles) {
        auto* projectile = projectiles_.get(entity);
        if (projectile == nullptr || projectile->remaining_ticks == 0U) {
            continue;
        }
        --projectile->remaining_ticks;
        if (projectile->remaining_ticks != 0U) {
            continue;
        }
        if (projectile->target.has_value() && active(*projectile->target)) {
            static_cast<void>(commit_damage(projectile->attack_id, *projectile->target, projectile->damage));
            projectile->consequence_committed = true;
        }
        request_destroy(entity);
    }
}

void RuntimeGameplay::remove_all_components(world::RuntimeEntityHandle entity) noexcept {
    health_.remove(entity);
    shields_.remove(entity);
    actors_.remove(entity);
    equipment_.remove(entity);
    weapons_.remove(entity);
    statuses_.remove(entity);
    projectiles_.remove(entity);
}

}  // namespace starforge::gameplay
