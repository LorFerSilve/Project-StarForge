# Physics Adapter and World Lifecycle

> **Status:** Architecture Complete  
> **Authority:** Jolt integration boundary, PhysicsWorld ownership/lifetime, physics handles, body descriptors, fixed-step execution, callbacks, backend isolation, and scene lifecycle integration

## 1. Purpose

Project StarForge uses Jolt Physics for collision and rigid-body mechanics without allowing Jolt types or callbacks to become gameplay authority.

This document defines the backend boundary that every later physics-facing system must use.

## 2. Core Boundary

Jolt is responsible for commodity physics mechanisms:

- broadphase;
- narrowphase;
- rigid-body integration;
- shape overlap;
- sweeps/casts;
- contact generation;
- constraint solving.

StarForge remains authoritative for:

- gameplay identity;
- movement rules;
- Health/Damage;
- docking state;
- interaction validity;
- mission boundaries;
- station topology;
- ship disable/destroy semantics;
- persistence.

## 3. Module Boundary

Only the StarForge `physics` module may include Jolt headers outside a narrow implementation boundary.

Gameplay modules depend on StarForge-owned types such as conceptually:

```cpp
PhysicsBodyHandle
PhysicsCharacterHandle
PhysicsShapeHandle
PhysicsConstraintHandle
PhysicsQueryResult
CollisionLayer
CollisionMaterialId
```

No persistent domain DTO contains a Jolt `BodyID`, pointer, shape object, constraint object, or allocator-owned backend reference.

## 4. PhysicsWorld Ownership

Each authoritative active `SceneInstance` owns exactly one StarForge `PhysicsWorld`.

The `PhysicsWorld`:

- is created during scene staging/activation;
- exists only for the active/local scene lifecycle;
- contains bodies/colliders required by that scene;
- is destroyed after all scene-owned physics handles are retired.

Horizon off-screen strategic simulation does not keep a second hidden full Jolt world.

## 5. PhysicsWorld Lifecycle

Canonical lifecycle:

```text
Absent
↓
Creating
↓
Ready
↓
Active
↓
Quiescing
↓
Destroying
↓
Absent
```

A world becomes Active only when required static geometry, mandatory collision proxies, and initial active body descriptors have been validated.

## 6. Scene Generation

Every `PhysicsWorld` is associated with the owning TA-3 `SceneGeneration`.

Physics handles/results/jobs carrying an obsolete SceneGeneration are rejected.

A late result from an unloaded scene can never mutate the replacement scene.

## 7. Handle Model

Backend objects are referenced through generation-checked project handles.

Conceptually:

```cpp
struct PhysicsBodyHandle {
    uint32_t index;
    uint32_t generation;
};
```

A stale handle resolves as invalid rather than accidentally referencing a reused backend slot.

## 8. Persistent Identity Mapping

A physics body can optionally carry a lightweight StarForge owner reference such as:

- `RuntimeEntityHandle`;
- stable local object key;
- ship/runtime actor reference;
- static-scene collision key.

This metadata supports query/contact routing only.

It is not a replacement for persistent IDs or gameplay stores.

## 9. Body Classes

StarForge exposes explicit body intent categories:

- Static;
- Kinematic;
- Dynamic;
- Sensor/Trigger;
- Character-specific controller representation.

The owning gameplay system selects the body class through a validated descriptor.

## 10. Static Bodies

Static bodies represent collision that does not move during ordinary simulation, such as:

- terrain;
- station structural collision;
- authored interior geometry;
- immovable mission structures.

When station construction/damage changes geometry, TA-6 owns when/how replacement static/kinematic collision is generated; TA-5 owns safe physics-world insertion/removal.

## 11. Kinematic Bodies

Kinematic bodies represent externally driven physical geometry such as:

- doors;
- elevators/platforms;
- docking clamps/machinery;
- selected station mechanisms.

Their target transform comes from authoritative gameplay/system state.

Physics can resolve contacts against them but does not independently decide their gameplay destination.

## 12. Dynamic Bodies

Dynamic bodies include:

- spacecraft;
- debris where gameplay-relevant;
- movable physical props when explicitly required;
- physical projectiles whose design requires rigid-body mechanics.

Dynamic-body transform/velocity is physics authority for its active leased high-frequency physical state.

## 13. Sensors

Sensor/trigger shapes report overlap without producing ordinary solid response.

Examples:

- interaction volumes;
- mission trigger volumes;
- docking capture envelopes;
- hazard/contact volumes where overlap is sufficient.

A sensor overlap is a physical fact, not automatic gameplay success.

## 14. Shape Ownership

Reusable immutable collision shapes are cached by project-owned shape/content keys where appropriate.

Bodies reference shapes through physics-resource handles.

Shape cache lifetime can exceed one entity but cannot exceed the owning PhysicsWorld/backend lifetime unless explicitly implemented as safe shared immutable backend state.

## 15. Shape Types

Baseline adapter supports the subset required by StarForge:

- box;
- sphere;
- capsule;
- convex hull;
- triangle/static mesh;
- compound shapes;
- authored height/terrain representation if TA-10 content pipeline requires it.

Arbitrary dynamic concave triangle-mesh bodies are not a baseline requirement.

## 16. Scale Policy

Runtime collision shapes are created at validated physical scale.

Negative scale and arbitrary post-creation nonuniform shape scaling are not relied upon as general physics behavior.

Content import/cooking owns conversion into valid collision geometry.

## 17. Unit Convention

Physics uses SI-compatible project units:

- meters;
- seconds;
- kilograms;
- radians.

No hidden centimeter conversion exists at the physics boundary.

## 18. Runtime Coordinate Frame

Physics runs in TA-3 origin-relative runtime coordinates.

Persistent double-precision Context Space is converted into local runtime coordinates before body creation/update.

Jolt is never treated as galaxy-coordinate authority.

## 19. Fixed-Step Ownership

Physics steps exactly once per authoritative TA-1 base simulation tick during ordinary real-time local simulation.

Baseline:

```text
60 Hz Simulation Tick
→ gameplay pre-physics intents
→ physics step
→ contact/query results
→ gameplay post-physics resolution
→ commit/snapshot boundary
```

Physics is never stepped according to render-frame delta.

## 20. No Independent Physics Clock

Jolt does not accumulate its own wall-clock time.

When `Simulation Time` is frozen by True Pause, Hard Streaming Hold, or transition hold:

- PhysicsWorld does not advance;
- dynamic bodies do not integrate;
- contacts do not progress;
- character/ship physical simulation remains frozen.

## 21. Sub-Stepping Boundary

The architecture does not introduce a second authoritative global physics rate.

A later implementation can internally subdivide selected difficult physics calculations inside one 60 Hz simulation tick if profiling/robustness requires it, provided:

- external gameplay sees one tick boundary;
- total simulated duration remains exactly the tick duration;
- outcome ordering remains deterministic enough for project requirements;
- no subsystem advances additional Simulation Time.

## 22. Simulation Tick Phases

TA-5 establishes the local physical phase ordering:

1. consume committed previous-tick state;
2. gather player/AI control intents;
3. update kinematic targets and character intents;
4. apply forces/thrust/impulses authorized for this tick;
5. perform pre-step required queries;
6. step PhysicsWorld;
7. gather contacts/overlaps/physical outcomes;
8. resolve movement grounding/docking/collision candidate state;
9. emit typed gameplay facts/intents for owning domains;
10. commit allowed gameplay consequences;
11. publish physical state for RenderSnapshot/interpolation.

Later TA-7 defines exact runtime-entity scheduling without changing this authority order.

## 23. Contact Listener Boundary

Backend contact callbacks do not directly:

- apply Health damage;
- complete mission objectives;
- change inventory;
- destroy gameplay entities;
- commit docking;
- play presentation effects.

Callbacks capture normalized contact facts into StarForge-owned per-tick buffers.

Owning gameplay systems consume those facts after the physics step.

## 24. Contact Fact

A normalized contact fact can contain:

- PhysicsBodyHandle A/B;
- owner references A/B;
- contact point(s);
- contact normal;
- relative velocity;
- impulse/estimated impulse data available from backend;
- CollisionMaterialId A/B;
- tick index;
- SceneGeneration.

Only fields with stable gameplay use are exposed.

## 25. Deterministic Ordering

Where multiple contact/query results can affect gameplay ordering, the adapter canonicalizes output using stable project keys/handles rather than relying on undocumented Jolt callback order.

Exact backend solver floating-point bitwise determinism across all hardware is not a baseline promise.

Gameplay transaction/event ordering remains deterministic through StarForge ordering rules.

## 26. Worker Threads

Jolt may internally use worker jobs only behind the physics adapter if/when TA-13 authorizes the worker configuration.

Regardless of backend execution:

- authoritative tick entry/exit is main-thread controlled;
- gameplay domains cannot mutate state from Jolt worker callbacks;
- results are consumed at controlled boundaries.

The initial implementation may run Jolt synchronously/single-coordinator-thread for simplicity.

## 27. Creation and Destruction

Body creation/destruction requests are queued to physics-safe synchronization points.

A gameplay system cannot destroy a body while a backend callback is actively using it.

Deferred destruction preserves handle-generation safety.

## 28. Scene Activation

During TA-3 scene activation:

1. collision content is resident;
2. immutable shapes are validated/cooked;
3. PhysicsWorld is created;
4. required static/kinematic bodies are registered;
5. active leased entities create their physical representations;
6. mandatory collision queries validate spawn/entry transforms;
7. PhysicsWorld becomes Active.

Failure before step 7 prevents local-scene activation.

## 29. Scene Deactivation

During quiesce/deactivation:

1. stop accepting new gameplay body creation;
2. finish current simulation boundary;
3. capture active authoritative physical state for TA-2 lease return/save projection;
4. invalidate gameplay→physics handle mappings;
5. safely remove/retire bodies/constraints;
6. destroy PhysicsWorld.

## 30. Save Boundary

Raw PhysicsWorld state is not serialized.

Save DTOs contain project-owned physical state needed to reconstruct gameplay, such as applicable:

- persistent position/orientation;
- velocity/angular velocity;
- locomotion state;
- ship physical state;
- authored mechanism state.

Backend solver caches/manifolds are reconstructed on load.

## 31. Load Reconstruction

On load/scene activation:

- project DTO/domain state is validated first;
- valid runtime origin is chosen;
- bodies are recreated from authoritative descriptors;
- velocities/state are restored;
- collision overlap/spawn validation runs;
- technical safe-position recovery can apply only where the GDS permits it.

## 32. Debugging Contract

Physics adapter exposes development diagnostics for:

- body count by class/layer;
- active/sleeping dynamic bodies;
- contact count;
- query count;
- character grounding state;
- constraints;
- broadphase/layer visualization;
- body/owner mapping;
- origin epoch.

Jolt debug drawing is routed through TA-4 debug rendering rather than drawn directly by gameplay code.

## 33. Error Handling

Invalid body descriptors, impossible shape data, stale handles, illegal layer combinations, or scene-generation mismatch produce explicit diagnostics.

Critical required collision creation failure blocks scene activation rather than silently removing collision.

## 34. Explicit Non-Goals

The adapter does not provide:

- persistent Jolt object identity;
- direct backend access from gameplay;
- physics-owned Health/Damage;
- render-frame stepping;
- second off-screen Horizon physics world;
- universal dynamic concave bodies;
- savefiles containing raw physics backend state.

## 35. Dependencies

Depends on TA-1 simulation timing/threading, TA-2 identity/activation/state contracts, TA-3 Scene lifecycle/origin model, TA-4 debug rendering, and Design Complete movement/spacecraft/combat rules.

## 36. Open Questions

None in the physics-adapter/world-lifecycle architecture baseline.
