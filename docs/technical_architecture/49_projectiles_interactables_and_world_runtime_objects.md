# TA-7 — Projectiles, Interactables, and World Runtime Objects

> **Status:** Architecture Complete
> **Authority:** Runtime representation of projectiles, world items, containers, interactables, hazards, mission objects, doors and other local gameplay objects

## 1. Purpose

Not every runtime entity is an Actor. TA-7 therefore defines lightweight local representations for physical/projected gameplay objects without transferring persistent authority away from their owning domains.

## 2. Projectile Runtime

Fast projectiles follow TA-5 swept project-owned motion by default. A `ProjectileComponent` can contain:

- projectile definition ContentId;
- source/owner semantic binding;
- attack instance identity;
- local position/velocity;
- remaining lifetime/range;
- payload reference;
- collision profile;
- bounded owner-ignore state;
- deterministic creation ordinal.

Dynamic bounce/rolling projectiles additionally reference a TA-5 Dynamic physics body.

## 3. Projectile Lifetime

Projectile lifecycle is runtime-only unless an owning GDS/persistence rule explicitly requires persistence.

Canonical runtime lifecycle:

```text
Spawned -> Active -> Impacted/Expired/Cancelled -> PendingDestroy
```

Impact consequence commits before the runtime projectile is reclaimed.

## 4. No Projectile-Owned Damage Authority

Projectile runtime produces physical/attack facts. Combat resolves and owning domains commit damage/status/outcomes.

Destroying the projectile entity before consequence commit is prohibited.

## 5. World Item Proxy

A `WorldItemProxyComponent` references the actual physical ownership record of an item/stack in the world/container system.

It may cache presentation/collision state but cannot be a second authoritative copy of quantity, Condition, security state or owner.

## 6. Containers

A runtime container exposes interaction and a revisioned view of its real owned contents.

Opening a container does not activate every stored object as a separate runtime entity unless visible/physical representation requires it.

Transfers use Inventory/Resource transactions.

## 7. InteractableComponent

Every runtime interactable exposes a typed runtime contract equivalent to the GDS interaction target:

- target identity/binding;
- available action descriptors;
- range;
- LOS/access profile;
- primary/alternate priority;
- sustained/instant mode;
- current blocker summary constrained by knowledge;
- source revision/state token.

The component does not execute arbitrary string commands.

## 8. Interaction Targeting

Player target acquisition uses deterministic candidate filtering and ordering. TA-5 queries establish physical line/range facts; TA-7 ranks only eligible candidates under the GDS priority rules.

Stable target identity is the final tie-breaker.

## 9. Sustained Interaction Runtime

Sustained interactions create a runtime action instance linked to player and target. Progress uses Simulation Time and is revalidated continuously according to target rules.

The target owner commits the result at the defined checkpoint/completion milestone.

## 10. Doors and Kinematic World Objects

A runtime door/elevator/mechanism entity represents an already-persistent or authored world object.

Its logical state is owned by the relevant station/world/mission domain; TA-5 kinematic physics and TA-4 presentation project that committed state.

A finished animation does not by itself commit `Open`, `Unlocked`, `Depressurized`, or any other gameplay state.

## 11. Hazards

A `HazardRuntimeComponent` references authored/persistent hazard state and exposes local geometry/intensity/activation facts needed for high-frequency exposure/contact.

Environmental consequence remains with Survival/Combat/Station/etc. as defined by GDS.

## 12. Mission Objects

Mission-relevant runtime objects carry stable Mission/Objective/World target bindings where required.

Destroying/despawning the runtime projection cannot erase or complete the objective by itself. The mission owner consumes committed typed facts.

## 13. Station Objects

Active Horizon modules/devices/doors/breaches use TA-6 `StationRuntimeProjection` identities. TA-7 can represent interactive/high-frequency pieces as entities, but StationStore/topology remains persistent authority.

## 14. Temporary Volumes

Temporary melee/query/interaction helper volumes may use runtime entities/components when practical, but must not become persistent identity or survive longer than the owning action.

## 15. Streaming

Residency alone does not imply gameplay activation. Runtime world entities are created only when TA-3 logical activation requires them.

Eviction cannot discard unresolved persistent outcome; deactivation first returns/commits state as applicable.

## 16. Save

Persistent world-object outcome is saved by World/Mission/Station/Inventory owners. Pure runtime proxies are reconstructed on load. Runtime-local projectile state is saved only if a future explicit persistence contract requires it.

## 17. Tests

Tests cover projectile consequence-before-destroy, bounded owner ignore, world-item single ownership, deterministic target ranking, stale interaction revision rejection, door logical/animation separation, mission proxy nonauthority and streaming/deactivation persistence.

## 18. Non-Goals

No universal scene-node-as-gameplay-object model, no visual-state authority, no container item duplication, no projectile damage in physics callbacks and no runtime despawn as mission completion.

## 19. Dependencies

Depends on TA-3 activation/streaming, TA-5 queries/projectiles, TA-6 station projection, GDS-5 Interaction/Inventory and GDS-8/9 mission/combat boundaries.

## 20. Open Questions

None.
