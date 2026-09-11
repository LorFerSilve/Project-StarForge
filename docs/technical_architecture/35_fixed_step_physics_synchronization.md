# Fixed-Step Physics Synchronization

> **Status:** Architecture Complete  
> **Authority:** Per-tick physics ordering, pre/post-step phases, mutation boundaries, query timing, event publication, render handoff, and Simulation-Time synchronization

## 1. Purpose

TA-5 requires one unambiguous ordering for movement, ship control, physics contacts, combat queries, topology changes, gameplay consequences, and render snapshots.

The goal is deterministic causality at the 60 Hz authoritative simulation boundary.

## 2. Fixed Step

The authoritative physics step uses the TA-1 fixed simulation duration:

```text
1 / 60 second
```

No physics gameplay path uses render-frame delta time as its authoritative integration duration.

## 3. Tick Ownership

The main simulation thread owns the complete fixed-step orchestration.

Worker jobs may prepare immutable non-authoritative inputs, but they do not step the active PhysicsWorld or commit physics-derived gameplay mutations independently.

## 4. Canonical Tick Pipeline

Each real-time local simulation tick executes the following logical phases:

```text
A. Command / Intent Intake
B. Domain Pre-Physics Validation
C. Apply Pending Physics Mutations
D. Character / Ship Control Preparation
E. Physics Step
F. Collect + Normalize Contacts / Trigger Observations
G. Gameplay Query Phase
H. Resolve Physics-Derived Gameplay Consequences
I. Commit Authoritative Domain Mutations
J. Queue Post-Commit Physics Mutations / Events
K. Stable Simulation Boundary
L. Build Immutable Consumer Snapshots
```

Sub-phases can be split internally later, but the causality above is fixed.

## 5. Phase A — Command / Intent Intake

This phase consumes commands already ordered by TA-1/TA-2.

Examples:

- player movement input;
- jump/crouch/mantle request;
- fire weapon;
- interact;
- ship thrust/rotation;
- docking request/assist;
- robot/AI locomotion intents.

Raw OS events are not physics inputs directly.

## 6. Phase B — Domain Pre-Physics Validation

Owning gameplay systems resolve whether requested physical action is currently allowed.

Examples:

- Health permits Sprint;
- Heavy Load blocks Jump;
- EVA suit has thrust capability;
- ship thrusters have power/propellant/condition;
- docking state permits final approach;
- weapon is ready to fire.

Physics receives already-scoped physical intents/capabilities.

## 7. Phase C — Apply Pending Physics Mutations

Safe queued mutations from earlier committed gameplay state are applied before stepping.

Examples:

- add/remove/replace collider;
- activate/deactivate Stream Cell collision;
- door collider/state update;
- apply committed knockback impulse;
- attach/release approved constraint;
- create/destroy projectile/grenade body;
- update mass/inertia after committed transfer.

No arbitrary domain transaction is executed inside this phase.

## 8. Mutation Failure

If a required mutation cannot be applied and continuing would create invalid mandatory collision/constraint state, the simulation does not advance into Phase E.

The session enters the applicable technical hold/failure path.

## 9. Phase D — Control Preparation

CharacterMotor and ShipFlightController compute the bounded forces/velocities/controller requests to submit to the backend.

This includes:

- grounded acceleration;
- jump impulse;
- gravity;
- EVA thrust/stabilization;
- support movement;
- ship forces/torques;
- Flight Assist;
- Docking Assist.

The phase does not process weapon-hit consequences yet.

## 10. Phase E — Physics Step

The active PhysicsWorld integrates one fixed step.

During the step:

- Dynamic bodies integrate;
- constraints solve;
- Kinematic motion participates;
- Character collision/support resolves through its defined integration path;
- contacts/sensors are collected into adapter-owned buffers.

Gameplay callbacks do not mutate domains during this step.

## 11. Internal Backend Parallelism

TA-13 may later allow Jolt to use worker threads internally during Phase E.

Even then:

- Phase E remains one externally atomic simulation stage;
- no gameplay mutation races the backend;
- observation normalization/consumption happens only after the step joins/completes.

## 12. Phase F — Observation Normalization

Raw backend contact/sensor observations are transformed into deterministic StarForge data.

This includes:

- stale handle/SceneGeneration rejection;
- contact-manifold aggregation;
- trigger pair coalescing;
- support-state extraction;
- impact episode update;
- deterministic sorting.

## 13. Character Post-Step State

CharacterMotor finalizes:

- actual resolved transform;
- velocity;
- support state;
- landing observation;
- blocked/step/mantle result;
- movement-state transition inputs.

Gameplay Movement remains the semantic owner of locomotion state.

## 14. Ship Post-Step State

Spacecraft runtime state reads:

- resolved transform;
- velocity/angular velocity;
- contacts;
- docking-relative observations;
- collision-impact facts.

This data can then drive Damage/Docking decisions.

## 15. Phase G — Gameplay Query Phase

Immediate gameplay queries execute against the post-integration PhysicsWorld state for the tick.

Examples:

- hitscan/multi-trace;
- projectile sweep from prior to proposed position where its integration contract requires it;
- melee sweep;
- explosion overlap/occlusion;
- interaction focus/LOS;
- mantle/magnetic follow-up probes;
- docking clearance/capture validation;
- AI LOS queries scheduled for this tick.

## 16. Fire Timing

A weapon command accepted in Phase B resolves its physical hit query in Phase G of the same tick after movement/physics integration.

Therefore the shot uses:

- the authoritative post-movement shooter muzzle transform;
- the authoritative post-movement target collision state.

This ordering is fixed and testable.

## 17. Projectile Timing

Short-lived gameplay projectile systems use their contract-specific integration/query path consistently every fixed tick.

A projectile cannot move once using render delta and again using physics delta.

Dynamic grenades obtain motion from Phase E; gameplay-owned swept plasma/rocket projectiles resolve in Phase G from their fixed-step state.

## 18. Phase H — Consequence Resolution

Owning gameplay systems consume observations/query hits and prepare consequences.

Examples:

- Hit Resolution;
- Damage Packets;
- Health/Hull/Module consequences;
- trigger-driven hazard candidates;
- interaction acceptance;
- docking capture eligibility;
- collision damage;
- mission boundary events.

Physics facts are input, not final gameplay truth.

## 19. Phase I — Authoritative Commit

TA-2 transactions/domain mutations commit in deterministic authority order.

Examples:

- Health decreases;
- module becomes Destroyed;
- door state changes;
- docking Capture commits;
- projectile detonation commits;
- interaction transfers an item;
- mission trigger/objective state changes.

A physics consequence that affects multiple domains follows TA-2 atomic transaction rules.

## 20. Phase J — Post-Commit Outputs

Committed gameplay can enqueue changes that physics will realize at the next safe mutation boundary.

Examples:

- destroyed wall collider replacement;
- knockback impulse;
- body destruction;
- new wreck collider;
- hard-dock constraint;
- door opening collision change.

Committed events are also published to known typed consumers.

## 21. Same-Tick vs Next-Tick Physical Mutation

Gameplay consequences can commit in Tick N while their backend collider/constraint representation is applied at the safe mutation boundary before Tick N+1.

During the intervening Stable Simulation Boundary:

- domain state is authoritative;
- presentation/read models can show the committed state;
- simulation does not perform another physics step using a known contradictory collider state.

Required mutation application is therefore a gate before the next physics step.

## 22. Stable Simulation Boundary

After all Tick N domain commits and required bookkeeping complete, the runtime reaches the TA-1 Stable Simulation Boundary.

Eligible operations include:

- save snapshot capture;
- True Pause activation;
- floating-origin rebase scheduling/application according to TA-3;
- scene-transition quiesce;
- deterministic consumer snapshot extraction.

## 23. True Pause

When True Pause is active:

- no fixed physics ticks run;
- Dynamic bodies do not integrate;
- Character/ship movement does not advance;
- projectile/grenade gameplay does not advance;
- collision damage does not advance;
- Simulation Time is frozen.

Renderer/UI can continue presentation according to TA-4/GDS-13 rules.

## 24. Hard Streaming Hold

TA-3 Hard Streaming Hold stops progression before the next required physics step.

Physics does not integrate actors through nonresident mandatory collision while content is loading.

## 25. Floating-Origin Rebase Ordering

A rebase happens only at a Stable Simulation Boundary when no backend step is active.

Ordering:

1. finalize Tick N;
2. select rebase delta;
3. shift active physics runtime coordinates consistently;
4. update `RuntimeOrigin64`/`OriginEpoch` atomically;
5. invalidate/rebuild epoch-sensitive caches;
6. begin Tick N+1.

No gameplay collision/damage event is emitted solely by this shift.

## 26. Scene Transition Ordering

Scene quiesce begins only after the current fixed tick has completed and all required observations/consequences have been consumed.

No physics callback from the old SceneGeneration can commit after the context switch.

## 27. Render Snapshot Handoff

TA-4 `RenderSnapshot` is built after the Stable Simulation Boundary from committed state.

Renderer interpolation uses previous/current committed snapshots.

Interpolated render transforms never write back into physics.

## 28. Audio / VFX Events

Impact, thruster, hit, explosion, landing, docking, and similar presentation events are emitted only after their authoritative physical/gameplay event has been normalized/committed as required.

Presentation timing cannot create physics consequences.

## 29. Deterministic Ordering Keys

Where several physics-derived observations/results occur in one tick, sorting uses project-owned stable keys such as:

- TickIndex;
- logical owner IDs/runtime keys;
- query sequence/order assigned before execution;
- stable subshape/hit-zone key;
- Transaction/Event sequence.

Backend pointer/address/callback order is prohibited as gameplay tie-breaker.

## 30. Physics Backlog

TA-1 accumulator/backlog policy remains authoritative.

If real time falls behind, the runtime processes fixed simulation ticks according to the approved backlog policy; it does not increase the physics dt to catch up and thereby change collision behavior.

TA-13 finalizes performance limits/degradation handling.

## 31. Save Boundary

A save may be requested during active movement/combat/flight.

Snapshot capture waits until the next Stable Save Boundary after the current fixed-step consequences/transactions have completed.

Backend contact manifolds/caches are not serialized as persistence authority.

## 32. Load Reconstruction

On load:

- domain state is decoded/validated in staging;
- Scene/PhysicsWorld is reconstructed;
- bodies/controllers/constraints are rebuilt from logical state;
- no simulation tick runs until required collision and Hard Dock states are valid;
- session activates atomically.

## 33. Test Contract

Headless deterministic tests must cover at minimum:

- same movement intent across different render rates;
- jump/landing ordering;
- moving support inheritance;
- zero-G drift/stabilization;
- muzzle obstruction;
- projectile sweep;
- collision impact aggregation;
- ship thrust/mass response;
- Flight Assist;
- docking capture/release;
- collider destruction/update before next step;
- floating-origin invariance;
- save boundary during active physics.

TA-14 expands the final automated matrix.

## 34. Explicit Non-Goals

This synchronization model does not permit:

- variable-dt authoritative physics;
- gameplay mutation inside backend callbacks;
- render transforms feeding physics;
- half-applied collider updates across another step;
- worker-thread physics authority;
- Simulation Time advancing during True Pause/Hard Streaming Hold.

## 35. Dependencies

Depends on TA-1 runtime/fixed tick, TA-2 commands/transactions/read models, TA-3 scene/streaming/floating origin, TA-4 RenderSnapshot, all TA-5 physics contracts, and the corresponding Design Complete GDS.

## 36. Open Questions

None within fixed-step physics synchronization.
