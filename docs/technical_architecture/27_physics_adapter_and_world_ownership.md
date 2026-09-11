# Physics Adapter and World Ownership

> **Status:** Architecture Complete  
> **Authority:** Jolt integration boundary, physics-world ownership, backend handles, body lifecycle, mutation rules, and gameplay-authority separation

## 1. Purpose

TA-5 uses Jolt Physics for local collision detection, rigid-body dynamics, constraints, and low-level spatial queries without allowing Jolt to become gameplay authority.

The StarForge `physics` module owns the adapter.

## 2. One Physics World per Active Scene

The one authoritative player-local `SceneInstance` defined by TA-3 owns at most one active StarForge `PhysicsWorld`.

No second hidden full physics world simulates off-screen Horizon while the player is on an external mission.

Off-screen station simulation remains domain-state/scheduled simulation under TA-3/TA-6.

## 3. Main-Thread Authority

Physics-world creation, destruction, gameplay-facing mutation submission, step orchestration, and result consumption are owned by the authoritative simulation/main thread.

No worker job may independently:

- create or destroy an authoritative body;
- teleport an authoritative body;
- change collision layer/filter state;
- attach/detach docking constraints;
- apply gameplay impulses;
- commit physics-derived gameplay consequences.

TA-13 may later permit internal backend parallelism during a bounded physics step, but the world remains externally single-authority.

## 4. Jolt Boundary

Jolt provides commodity mechanics:

- rigid-body integration;
- collision detection;
- contact generation;
- constraints;
- broadphase;
- ray/shape/overlap queries;
- continuous collision support where selected.

StarForge remains authoritative for:

- Player Movement rules;
- ship Flight Assist and thrust authority;
- Hit Resolution semantics;
- Damage Packets and Health;
- docking state transitions;
- station structural validity;
- mission/raid boundaries;
- interaction permissions;
- persistent identity;
- save/load.

## 5. Backend Type Isolation

Jolt backend types such as body IDs, shape references, constraints, and contacts do not cross the `physics` adapter boundary into persistent/domain APIs.

They are never:

- serialized;
- used as Persistent IDs;
- stored in mission/world persistent state;
- exposed to UI/render/audio as authority.

## 6. PhysicsBodyHandle

StarForge exposes a process-local generation-checked handle:

```text
PhysicsBodyHandle {
    index;
    generation;
}
```

It is distinct from:

- `PersistentId<Tag>`;
- `RuntimeEntityHandle`;
- `RenderResourceHandle`.

A stale PhysicsBodyHandle resolves invalid after body destruction/reuse.

## 7. Body Ownership Record

Each physics body adapter record stores enough nonpersistent linkage to identify its runtime/gameplay owner, for example:

- `RuntimeEntityHandle` when body belongs to an active entity;
- scoped Scene/Zone object key for local geometry;
- explicit body role;
- collision layer;
- material/contact metadata reference;
- current SceneGeneration/OriginEpoch where relevant.

The physics record does not own the gameplay object.

## 8. Body Motion Classes

StarForge uses the following logical motion classes:

- Static;
- Kinematic;
- Dynamic;
- QueryOnly/Sensor.

Examples:

- station hull wall: Static;
- powered moving door/elevator: Kinematic;
- spacecraft/debris/grenade: Dynamic;
- trigger volume: QueryOnly/Sensor;
- character collision: Character-controller backend representation rather than an ordinary gameplay Dynamic body.

## 9. Shape Ownership

Collision shapes are renderer-independent.

Shapes come from:

- authored collision content;
- primitive gameplay shapes;
- validated generated station geometry;
- runtime composition approved by owning systems.

Rendering triangle meshes are not automatically used as collision meshes.

## 10. Static Geometry

Large immovable environment geometry uses static collision suitable to its authored scene profile.

Static geometry can be grouped into streaming/topology chunks for manageable activation/deactivation.

TA-3 Stream Cell boundaries may inform chunk ownership but do not become physics authority.

## 11. Dynamic Bodies

Dynamic bodies include objects for which physical momentum and contact response are gameplay relevant, such as:

- spacecraft;
- selected debris;
- thrown grenades;
- movable props when authored as gameplay-physical.

Not every decorative prop becomes a Dynamic body.

## 12. Kinematic Bodies

Kinematic bodies are driven by authoritative gameplay state rather than solver forces.

Examples:

- authored elevators;
- powered doors;
- moving platforms;
- scripted mechanical station components.

Their movement is still collision-authoritative.

## 13. Sensors / Query-Only Bodies

Sensors may detect overlap without producing ordinary blocking response.

They are appropriate for:

- trigger volumes;
- interaction regions;
- docking envelopes;
- hazard volumes;
- mission boundary detectors;
- ladder/mantle authored helper volumes where applicable.

Sensor overlap is evidence/candidate data, not automatic gameplay success.

## 14. No Mutation During Backend Callback

Physics callbacks never directly perform arbitrary world mutation.

Contact/sensor callbacks append compact adapter-owned observations to bounded per-step buffers.

Gameplay consumes those observations after the physics step and decides whether to emit Commands, Damage requests, interaction candidates, alarms, or state transitions.

## 15. Queued World Mutations

Body/shape/layer/constraint changes requested during a tick become a `PhysicsMutationBatch` applied only at an allowed pre-step/post-step boundary.

Examples:

- create/destroy body;
- replace collider;
- move static collision chunk;
- change sensor enabled state;
- attach/release docking constraint.

This avoids invalidating the backend while it is solving contacts.

## 16. Teleport / Authoritative Reposition

Teleport-like body repositioning is not a general gameplay movement operation.

It is permitted only for explicit technical/transition cases such as:

- scene activation/deactivation;
- load reconstruction;
- TA-3 floating-origin rebase;
- validated technical Unstuck;
- authored context transition placement.

Gameplay locomotion, Flight Assist, knockback, and docking approach do not use teleport to bypass physics.

## 17. Floating-Origin Rebase

TA-3 owns `RuntimeOrigin64` and `OriginEpoch`.

At a Stable Simulation Boundary, a rebase shifts all active runtime-space physics bodies by the same delta while preserving:

- relative positions;
- linear velocity;
- angular velocity;
- active contacts/constraints where backend-safe reconstruction permits;
- gameplay Context Space state.

No Damage or gameplay collision event is emitted solely because of a coordinate rebase.

## 18. Scene Generation

Every active PhysicsWorld is tied to the owning SceneGeneration.

Async-prepared collision payload or query result carrying an obsolete SceneGeneration is discarded.

## 19. Physics Materials

Collision material metadata is StarForge-owned and can expose physical/query properties such as:

- friction;
- restitution;
- material penetration resistance reference;
- footstep/surface class;
- magnetic-boot compatibility;
- mantle/step compatibility;
- impact-response category.

A material label alone never applies combat damage.

## 20. Friction and Restitution

Jolt can use authored physical friction/restitution for contact response.

Gameplay-critical locomotion remains controlled by the Character Motor and does not rely on arbitrary mesh material friction to define walking speed.

## 21. Sleeping

Backend sleeping is permitted for eligible Dynamic bodies.

Sleeping is a physics optimization/state and does not pause:

- gameplay timers;
- damage/status processing;
- mission state;
- domain simulation.

Gameplay systems wake bodies when an explicit physical force/state change requires it.

## 22. PhysicsWorld Lifecycle

Canonical lifecycle:

```text
Absent
↓
Constructing
↓
Ready
↓
Active
↓
Quiescing
↓
Destroyed
```

The world becomes Active only as part of the TA-3 Scene activation path.

## 23. Scene Teardown

On scene quiesce:

1. stop accepting new local physics intents;
2. finish current fixed step;
3. consume required contact/query output;
4. commit leased persistent/runtime state as required;
5. destroy runtime-only constraints/bodies;
6. invalidate PhysicsBodyHandles;
7. release backend world.

Physics teardown never deletes persistent gameplay identity.

## 24. Error Policy

Backend allocation/setup failures during scene staging fail scene readiness rather than activating a partially collidable world.

Runtime invariant failures are development-fatal in Debug/validation configurations and produce a controlled technical failure path in shipping builds.

## 25. Debug Validation

The adapter provides development diagnostics for:

- invalid/stale handles;
- body owner mapping;
- collision layers;
- sleeping state;
- active constraints;
- contacts/sensors;
- shape bounds;
- origin epoch;
- scene generation.

TA-14 owns final diagnostics/UI integration.

## 26. Headless Tests

The physics adapter must be constructible in test environments without OpenGL/audio.

Core collision/filter/controller/ship/docking tests can therefore run headlessly.

## 27. Explicit Non-Goals

TA-5 does not make Jolt authority for:

- Health/Damage;
- mission success;
- interaction permission;
- docking completion;
- station structural support;
- persistent identity;
- save serialization;
- rendering visibility.

It also does not introduce a second full off-screen physics scene.

## 28. Dependencies

Depends on TA-1 main-thread/fixed-step authority, TA-2 runtime/persistent identity separation, TA-3 Scene lifecycle/floating origin, GDS Player Movement, Spacecraft Flight/Docking, Combat Hit/Damage, and Station Construction/Damage.

## 29. Open Questions

None within the physics-adapter ownership scope.
