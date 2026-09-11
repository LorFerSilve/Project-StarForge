# TA-7 — Actor Lifecycle, Activation, and Deactivation

> **Status:** Architecture Complete
> **Authority:** Persistent actor activation/deactivation, runtime actor composition, lease transfer, scene entry/exit, and actor closing behavior

## 1. Actor Boundary

A runtime Actor is an entity with actor-facing capabilities, not a persistent identity class hierarchy.

Actor-capable entities include player, crew, robots, hostile humanoids/machines, creatures and selected combat-capable world systems.

## 2. Persistent Actors

Persistent actors remain owned by their domain stores. Their high-frequency facets are leased into runtime components through TA-2 Activation Leases.

Typical leased facets can include local transform/velocity, current active Health/shield, active equipment use state, temporary combat/status state and later TA-8 tactical state.

Identity, long-term assignment, history, progression, faction relation and other strategic fields remain store-owned unless explicitly leased.

## 3. Activation Descriptor

Each persistent actor type has a typed activation adapter producing an immutable `ActorActivationDescriptor` containing only data needed to construct the runtime representation plus lease metadata/revisions.

Activation never reads raw mutable cross-domain pointers into components.

## 4. Activation Sequence

Canonical order:

1. persistent owner validates eligibility;
2. required content is resolved;
3. activation descriptor is prepared;
4. RuntimeEntityHandle is allocated;
5. actor/runtime components are installed;
6. TA-5 physics and TA-4 render/audio proxies are prepared;
7. persistent ID mapping is registered;
8. Activation Lease commits atomically;
9. entity becomes Active next legal phase.

Failure before step 8 leaves the persistent record Dormant.

## 5. Player Actor

Exactly one runtime entity represents the locally controlled player while a player-local scene is active.

Player identity remains persistent; camera/input/controller state is runtime-only unless an owning GDS state explicitly persists it.

## 6. Crew and Robot Actors

Crew/robots can activate only when actually present in the active context.

A crew/robot stored at Horizon while the player is on another mission remains persistent/off-screen and is not represented by a hidden runtime entity.

## 7. Hostile/Procedural Actors

A hostile actor can be either:

- backed by persistent Mission/World identity where the encounter state must survive/re-enter; or
- runtime-transient where the owning Mission/World state needs only terminal/aggregate outcome.

The owning mission/world specification decides this before spawn; TA-7 never invents persistence based on convenience.

## 8. Closing

An actor enters `Closing` when it must leave active normal behavior but still has owned operations to settle.

While Closing it cannot begin ordinary new attacks/interactions/movement intents unless the owning transition explicitly permits them.

Examples include deactivation, death/destruction resolution, scene transition, extraction handoff and despawn after committed mission outcome.

## 9. Deactivation

Canonical persistent-actor deactivation:

1. mark Closing;
2. stop accepting ordinary new local commands;
3. resolve/cancel local actions under their owning rules;
4. prepare returned leased facets;
5. validate PersistentId + ActivationEpoch;
6. atomically return leased facets to persistent owner;
7. remove persistent/runtime mapping;
8. queue backend proxy removal;
9. mark PendingDestroy;
10. reclaim entity/components at safe structural boundary.

## 10. No Silent Despawn

A persistent actor cannot simply disappear because distance culling or streaming residency changes.

Logical deactivation requires the owning scene/mission/world rules to permit it and must return authoritative leased state.

## 11. Incapacitation / Disablement

`Incapacitated` or `Disabled` normally changes actor capability components/state; it does not itself destroy the runtime entity.

The entity remains for rescue, looting, objective resolution, repair, capture or presentation until the owning gameplay state transitions it further.

## 12. Defeat / Destruction

A defeat/destruction result first commits in the owning gameplay domain. Runtime lifecycle then reflects that committed result.

A destroyed runtime actor can remain as a corpse/wreck/interactable proxy if the owning world/mission persistence requires it.

## 13. Scene Transition

TA-3 destination staging occurs before source actor deactivation commit where required.

The player is never simultaneously authoritative in two runtime registries.

## 14. Save

Stable save capture exports active leased actor facets through TA-2 and persistent owner DTOs. Runtime lifecycle flags that have no persistent semantic meaning are not serialized.

## 15. Tests

Tests cover atomic activation, failed activation rollback, one active mapping per persistent actor, stale epoch rejection, deactivation round-trip, incapacitated actor retention, transition uniqueness and save capture while active.

## 16. Non-Goals

No persistent actor object hierarchy, no distance-based semantic despawn, no dual active/dormant authority, and no automatic deletion on Health zero.

## 17. Dependencies

Depends on TA-2 Activation Leases, TA-3 scene transition, TA-5 physics lifecycle and `44`/`45`.

## 18. Open Questions

None.
