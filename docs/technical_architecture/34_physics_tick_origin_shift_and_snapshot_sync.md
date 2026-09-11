# Physics Tick, Origin Shift, and Snapshot Synchronization

> **Status:** Architecture Complete  
> **Authority:** Exact 60 Hz physics phase order, origin-rebase synchronization, activation/deactivation handoff, interpolation snapshots, stale-result protection, and simulation/render consistency

## 1. Purpose

TA-1, TA-2, TA-3, TA-4, and TA-5 now meet at one critical integration point: the fixed simulation tick.

This document defines the authoritative physical update order so character movement, spacecraft, projectiles, collision consequences, origin rebasing, streaming, save boundaries, and RenderSnapshot publication cannot race or observe half-updated state.

## 2. Fixed Tick

Authoritative local real-time simulation uses the TA-1 base tick:

```text
60 Hz
Δt = 1 / 60 s Simulation Time
```

Physics receives exactly this authoritative duration per ordinary tick.

## 3. No Render Delta

Physics, CharacterMotor, projectile movement, flight control, docking capture, and collision consequence evaluation never use variable render-frame delta as authoritative simulation duration.

## 4. Tick Inputs

At the beginning of tick N, local simulation consumes:

- committed state from tick N-1;
- validated player/input intents;
- AI intents available for N;
- committed system/domain state changes;
- active-scene content/streaming state;
- current `RuntimeOrigin64` / `OriginEpoch`;
- current SceneGeneration.

## 5. Canonical Physical Tick Phases

Baseline semantic order:

1. **Tick Begin / State Latch**;
2. **Gameplay Pre-Physics Validation**;
3. **Kinematic/Traversal Target Update**;
4. **Character/Ship/Projectile Intent Resolution**;
5. **Pre-Physics Queries**;
6. **Physics Step**;
7. **Contact/Overlap/Query Normalization**;
8. **Physical State Finalization**;
9. **Collision/Docking/Movement Fact Resolution**;
10. **Gameplay Transaction Commit Window**;
11. **Deferred Physics Mutation Application**;
12. **Stable Simulation Boundary**;
13. **Read Model / RenderSnapshot Publication**;
14. **Tick End**.

Later subsystem TAs can add ordered work inside these semantic phases but cannot bypass them.

## 6. Tick Begin / State Latch

Tick-scoped systems capture the state revisions/epochs they require.

No renderer/worker snapshot can mutate this latched state.

## 7. Pre-Physics Gameplay Validation

Owning gameplay systems validate intents that affect physical motion, such as:

- sprint/jump restrictions;
- ship thrust availability;
- suit energy/thruster authority;
- docking state permissions;
- projectile spawn authorization;
- interaction/traversal requests.

Rejected intents never reach the physics backend as if they were valid forces/actions.

## 8. Kinematic Target Update

Authoritative mechanisms publish this tick's physical target transforms before the world step:

- doors;
- elevators/platforms;
- docking machinery;
- authored moving geometry.

Visual animation alone does not write these targets.

## 9. Motion Intent Resolution

Character/ship/projectile systems transform semantic intents into bounded physical operations:

- CharacterMotor desired movement;
- ship forces/torques;
- projectile swept trajectories/body forces;
- Zero-G thruster acceleration;
- Flight Assist corrections.

## 10. Pre-Physics Queries

Queries required before integration can run after all relevant kinematic target/state updates, such as:

- standing clearance;
- step/mantle probes;
- projectile spawn/muzzle clearance;
- docking approach data;
- camera-independent gameplay probes needed by movement.

## 11. Physics Step

PhysicsWorld executes one authoritative step for `Δt`.

Backend callbacks record contact/overlap data only into tick-local normalized buffers.

They do not directly execute gameplay transactions.

## 12. Contact Normalization

After solver completion, backend facts are converted into StarForge-owned ordered results.

Normalization includes where required:

- stable owner/body ordering;
- contact-manifold grouping;
- impact episode grouping;
- trigger Enter/Exit transitions;
- hit-zone metadata;
- support/ground facts.

## 13. Physical State Finalization

The active physical representations finalize:

- character position/velocity/support;
- ship pose/velocity/angular velocity;
- projectile position/state;
- dynamic-body transforms;
- docking relative physical state;
- Zero-G support/attachment state.

## 14. Gameplay Fact Resolution

Owning systems consume physical facts and produce commands/transactions for:

- landing/collision damage;
- projectile/melee hits;
- docking capture eligibility;
- trigger state changes;
- hazard exposure;
- movement locomotion transitions;
- structural/dock strain candidates.

## 15. Transaction Commit Window

TA-2 transactions that are fully prepared can commit after physical facts are known.

Examples:

- Damage Packet consequences;
- docking state transition;
- component destruction;
- actor incapacitation;
- mission trigger consequence where all nonphysics requirements pass.

Commit remains main-thread deterministic.

## 16. Deferred Physics Mutation

Gameplay consequences that require physical object changes are not applied unsafely from inside callbacks.

Examples:

- destroy a body;
- replace a collision shape;
- release docking constraint;
- spawn impact debris body;
- disable trigger;
- change physical layer.

These requests are applied at an explicit post-commit physics-safe mutation boundary.

If a mutation logically must affect the just-committed gameplay state immediately, the object is marked noninteractive/retiring until physical retirement completes before the next step.

## 17. Stable Simulation Boundary

After all required authoritative tick consequences and physics-safe mutations complete, the runtime reaches a TA-1 Stable Simulation Boundary.

At this point:

- no physics step is in flight;
- no callback is mutating buffers;
- transaction commit window is complete;
- active high-frequency state is coherent;
- origin rebase/save snapshot/scene transition can be considered.

## 18. Floating-Origin Rebase Eligibility

TA-3 rebase can occur only at a Stable Simulation Boundary.

A rebase request is generated when origin-distance/precision policy determines it is needed.

TA-13 later fixes numeric thresholds/budgets.

## 19. Rebase Operation

Conceptually:

1. compute new `RuntimeOrigin64` in persistent Context Space;
2. compute runtime translation delta;
3. update/rebase all active physics bodies/character state/constraints consistently;
4. update scene/runtime spatial structures that use origin-relative coordinates;
5. increment `OriginEpoch`;
6. preserve all persistent Context Space positions and velocities;
7. publish no gameplay movement event solely because of rebase.

## 20. Rebase Invariants

A rebase must not change:

- relative distance between bodies;
- relative velocity;
- orientation/angular velocity;
- docking relative transform;
- grounding/support relation;
- collision damage state;
- mission/objective state;
- player knowledge;
- physical ownership.

## 21. Constraint Handling During Rebase

Bodies connected by docking/other physical constraints rebase in one coordinated operation.

The architecture does not individually teleport one constrained participant across ticks.

## 22. Contact State During Rebase

Rebase cannot create synthetic impact/damage events.

Backend contact caches/manifolds may be safely preserved or rebuilt according to Jolt-supported technique, but StarForge suppresses any contact episode that exists solely because the coordinate frame changed.

## 23. Stale Origin Epoch

Any asynchronous physical-preparation/query result that carries old `OriginEpoch` and depends on runtime coordinates is rejected/recomputed.

Synchronous main-thread physics step results belong to the current epoch by construction.

## 24. Scene Generation Protection

Physics results also carry/validate TA-3 SceneGeneration where they can survive beyond immediate scope.

An unloaded scene can never apply a late projectile/query/contact result to a new scene.

## 25. Activation Lease Synchronization

When a persistent entity becomes active:

- TA-2 Activation Lease transfers designated high-frequency physical facets to runtime;
- physics representation is created from the leased state;
- persistent dormant copies no longer independently mutate those leased fields.

When deactivating:

- final physical state is captured at Stable Simulation Boundary;
- validated against ActivationEpoch;
- returned to owning persistent domain;
- physics handle is retired.

## 26. Save Snapshot Synchronization

At a Stable Save Boundary, active physical state is exported into project-owned Save DTO/read snapshot form.

Raw Jolt state is not copied into the save.

The immutable snapshot can then serialize asynchronously while gameplay continues from a later tick.

## 27. RenderSnapshot Publication

TA-4 receives previous/current committed physical presentation data only after the Stable Simulation Boundary.

For renderable bodies this can include:

- transform;
- velocity;
- angular velocity;
- effect/presentation state;
- origin epoch/generation metadata as needed.

Renderer interpolates presentation without modifying physical state.

## 28. Interpolation Pair

The runtime retains at least:

- previous committed presentation transform;
- current committed presentation transform.

Origin rebase handling must ensure the pair is expressed in a coherent presentation frame so interpolation does not visually jump by the rebase delta.

## 29. Origin Rebase and Renderer

Preferred architecture:

- renderer consumes Context Space/origin-relative snapshot metadata sufficient to transform both previous/current poses into the current render frame;
- `OriginEpoch` change invalidates incompatible cached spatial/render assumptions;
- visual camera does not experience an artificial teleport caused by rebase.

## 30. Pause

During global True Pause:

- no simulation tick advances;
- PhysicsWorld does not step;
- projectiles/fuses using Simulation Time do not advance;
- ship/character state remains fixed;
- RenderSnapshot can continue being presented.

## 31. Hard Streaming Hold

During TA-3 Hard Streaming Hold:

- simulation/physics do not advance;
- worker loading can continue;
- renderer can present/loading UI as allowed;
- no local hazard/collision consequences accumulate.

## 32. Scene Transition Hold

During staged context transition commit/activation holds, physics follows TA-3 scene state:

- origin scene quiesces at Stable Simulation Boundary;
- destination PhysicsWorld is staged/validated;
- no simultaneous two-player-local authoritative physics worlds advance.

## 33. Simulation Backlog

If real rendering/system time falls behind, TA-13 owns backlog policy.

Regardless of catch-up strategy:

- each simulated physics step represents exactly one authorized fixed Simulation-Time increment;
- render-frame skipping cannot merge physics gameplay into one variable large step by default.

## 34. Worker Query Boundary

TA-13/TA-8 may later authorize selected immutable physics/nav preparation jobs.

No asynchronous worker can issue mutable Jolt operations against the live PhysicsWorld outside the adapter synchronization contract.

## 35. Failure Handling

A fatal physics-world invariant violation during active gameplay is treated as a technical runtime error, not silently repaired by changing gameplay state.

Examples:

- stale body generation used as valid;
- required player collider disappears;
- required dock constraint cannot remain represented after committed Hard Dock;
- invalid rebase corrupts relative positions.

Development builds assert/diagnose aggressively.

## 36. Diagnostics

Per-tick diagnostics can record:

- tick index;
- physics step duration;
- active/static/kinematic/dynamic body counts;
- queries;
- contacts/manifolds;
- trigger transitions;
- deferred mutations;
- rebase event/OriginEpoch;
- physics consequence transaction count.

## 37. Tests

Required integration tests include:

- render FPS independence;
- True Pause freezes physics;
- Hard Streaming Hold freezes physics;
- origin rebase preserves relative positions/velocities;
- rebase creates no collision damage;
- Activation Lease round-trip preserves state;
- save snapshot captures coherent active physical state;
- renderer interpolation remains visually/contextually continuous across rebase;
- stale SceneGeneration/OriginEpoch results are rejected;
- callback-triggered body destruction is safely deferred.

## 38. Explicit Non-Goals

No variable-delta authoritative physics, mid-step origin shift, callback-direct gameplay transaction, two simultaneously advancing player-local PhysicsWorlds, raw backend save snapshot, or renderer feedback into authoritative physical state exists in the baseline.

## 39. Dependencies

Depends on all TA-5 physics contracts plus TA-1 fixed simulation, TA-2 leases/transactions/save snapshots, TA-3 scene/origin/transition model, TA-4 RenderSnapshot/interpolation, and future TA-13 performance scheduling.

## 40. Open Questions

None in physics tick/origin/snapshot synchronization architecture.
