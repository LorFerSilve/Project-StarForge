# TA-7 — Runtime Entity Registry and Handles

> **Status:** Architecture Complete  
> **Authority:** High-frequency local runtime entity identity, registry ownership, allocation, validity, scene scoping, and entity composition boundary

## 1. Purpose

TA-7 introduces the active gameplay runtime entity layer used inside the one authoritative player-local `SceneInstance` defined by TA-3.

The runtime layer exists for high-frequency local behavior such as:

- player and actor presence;
- local transforms and movement-facing state;
- combat-facing state;
- transient projectiles/effects with gameplay meaning;
- interactable runtime proxies;
- active item/equipment presentation and use state;
- AI-facing local state later owned by TA-8.

It is not a replacement for persistent domain stores.

## 2. Ownership

Each active `SceneInstance` owns exactly one `RuntimeEntityRegistry`.

The registry is created during scene activation/staging and destroyed with that scene generation.

No process-global runtime entity registry exists.

Off-screen Horizon simulation does not keep a hidden second full registry.

## 3. RuntimeEntityHandle

Runtime references use a generation-checked handle:

```cpp
struct RuntimeEntityHandle {
    uint32_t index;
    uint32_t generation;
};
```

Conceptual invariants:

- zero/invalid sentinel is explicit;
- slot reuse increments generation;
- stale handles fail validation;
- handles are process/session runtime references only;
- handles are never serialized as persistent identity.

## 4. Scene Generation

A runtime entity is additionally scoped to the owning `SceneGeneration`.

Cross-scene retained references use a compound validation context equivalent to:

```text
SceneGeneration + RuntimeEntityHandle
```

A handle from a destroyed/replaced scene can never resolve in the new scene merely because an index/generation pair coincidentally matches.

## 5. Persistent Identity Mapping

An entity representing a persistent object can carry a typed persistent binding such as:

- `CrewId`;
- `RobotId`;
- `ShipId`;
- `UniqueItemId` where an active world object requires it;
- persistent world-object identity;
- station module/device identity;
- mission/world target identity.

The registry maintains explicit bidirectional mappings only for currently active bindings:

```text
PersistentId -> RuntimeEntityHandle
RuntimeEntityHandle -> typed persistent binding
```

The persistent ID remains the long-lived identity.

## 6. Transient Runtime Entities

Some entities have no persistent ID.

Examples:

- a normal in-flight plasma projectile;
- a temporary melee attack volume;
- transient explosion gameplay resolver state;
- nonpersistent combat debris;
- short-lived interaction helper volume.

Transient entities use runtime handles only and must never be referenced by persistent save/history as if the handle were identity.

If a gameplay object must survive unload/save/context transition, the owning domain must give it persistent state/identity or an equivalent persistent record.

## 7. Entity Record

The registry's entity slot contains only small registry metadata, conceptually:

- generation;
- alive/lifecycle flags;
- scene generation;
- component-presence mask/index metadata;
- optional typed persistent binding;
- activation epoch when backed by an Activation Lease;
- deterministic creation ordinal if needed for runtime ordering diagnostics.

Large gameplay state belongs in component pools or owning persistent stores.

## 8. No Universal GameObject Base Class

StarForge does not use an inheritance tree such as:

```text
GameObject
  -> Actor
      -> Character
          -> Enemy
```

as the authoritative runtime model.

Behavior is composed through typed components and domain systems.

Small ordinary C++ classes may still implement subsystems/controllers where appropriate; the restriction is against making one polymorphic object hierarchy the universal gameplay state owner.

## 9. Entity Creation

Runtime entity creation is requested through a typed spawn/activation command or owning system operation.

Canonical creation sequence:

1. validate scene/context and spawn intent;
2. allocate registry slot and new generation-valid handle;
3. install required components in deterministic schema order;
4. create needed physics/render/audio proxies through their owning adapters;
5. register persistent mapping if applicable;
6. commit Activation Lease if persistent high-frequency facets are transferred;
7. mark entity `Active` at the next legal simulation boundary.

A partially prepared entity is not gameplay-active.

## 10. Spawn Failure

If preparation fails before activation commit:

- no persistent lease is committed;
- no visible/alive entity remains in the registry;
- allocated components/proxies are rolled back/deferred-cleaned;
- persistent ownership is unchanged.

## 11. Runtime Entity Kinds

For diagnostics and coarse filtering the registry may expose an authored `RuntimeEntityKind`, such as:

- Player;
- HumanoidActor;
- RobotActor;
- CreatureActor;
- Spacecraft;
- Turret;
- WorldItem;
- InteractableObject;
- Projectile;
- HazardProxy;
- MissionObject;
- StationDevice;
- OtherGameplayObject.

`RuntimeEntityKind` is not a behavior hierarchy and does not replace component presence.

## 12. Deterministic Runtime Ordering

Any gameplay behavior whose result depends on processing order must use explicit deterministic ordering.

Preferred keys are:

1. owning semantic priority/order where defined;
2. persistent ID when present;
3. stable runtime creation ordinal/handle index as a final local tie-breaker.

Hash-container iteration order is never gameplay authority.

## 13. Entity Lookup

The registry supports:

- handle validity check;
- component-presence query;
- persistent binding lookup;
- runtime handle lookup from active persistent ID;
- lifecycle state query.

Systems do not retain raw pointers across structural registry/component mutations unless a specific pool contract guarantees validity for that phase.

## 14. Lifecycle States

Canonical runtime lifecycle states are:

```text
Preparing
  -> Active
  -> Closing
  -> PendingDestroy
  -> Reclaimed
```

`Closing` means the object still exists long enough to resolve owned in-flight semantics but no longer accepts ordinary new actions requiring normal presence.

`PendingDestroy` means logical gameplay removal has committed and backend/component cleanup is deferred to a safe mutation phase.

## 15. Runtime Ownership vs Domain Ownership

The runtime registry owns:

- active entity slots;
- local component instances;
- runtime mappings;
- local lifecycle.

It does not own by mere presence:

- physical inventory ownership;
- Credits;
- mission objective truth;
- faction reputation;
- research/blueprint state;
- persistent station topology;
- persistent Crew/Robot/Ship identity/history.

## 16. Thread Affinity

Registry structural mutation is authoritative simulation-thread work.

Workers may inspect immutable snapshots or produce prepared results tagged with scene/revision/generation preconditions.

Workers cannot allocate/destroy authoritative runtime entities directly.

## 17. Save Boundary

The registry itself is not serialized wholesale.

At a Stable Save Boundary:

- active persistent entities export leased authoritative facets through TA-2;
- transient entities are saved only when an owning persistent specification explicitly requires their gameplay state to survive save/load;
- runtime handles, pool indices, physics handles, render handles, and pointer addresses are never persisted.

## 18. Debugging

Developer diagnostics expose:

- entity count by kind;
- handle/index/generation;
- scene generation;
- lifecycle state;
- component set;
- persistent binding;
- ActivationEpoch;
- physics/render proxy handles;
- pending destruction reason.

## 19. Test Requirements

Tests must cover:

- stale handle rejection after slot reuse;
- scene-generation rejection;
- activation mapping uniqueness;
- transient versus persistent entity identity;
- creation rollback;
- deterministic iteration where behavior depends on order;
- deferred reclamation;
- save export never persisting runtime handles.

## 20. Explicit Non-Goals

This registry is not:

- a universal persistent ECS;
- a generic editor object database;
- a scripting object model;
- a networking replication identity layer;
- a polymorphic GameObject inheritance root.

## 21. Dependencies

Depends on TA-1 fixed simulation phases, TA-2 identity/Activation Leases, TA-3 SceneGeneration, TA-5 physics handles, and TA-6 persistent station projection.

## 22. Open Questions

None in the TA-7 registry baseline. Numeric entity/component budgets belong to TA-13.
