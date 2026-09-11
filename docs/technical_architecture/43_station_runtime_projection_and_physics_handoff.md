# Station Runtime Projection and Physics Handoff

> **Status:** Architecture Complete  
> **Authority:** Projection of persistent Horizon state into the active SceneInstance, station static/kinematic physics representation, runtime equipment/door/dock handoff, active/off-screen lease boundaries, and safe topology-to-physics synchronization

## 1. Purpose

TA-6 persistent station systems must become a traversable physical Horizon scene when the player is home without making physics/render runtime state the source of truth.

This specification defines that handoff.

## 2. Persistent Authority

Persistent TA-6 stores remain authority for:

- module presence/transform;
- structure/topology;
- doors/equipment state;
- pressure boundaries;
- power/environment/logistics/work state;
- damage/faults;
- construction/repair;
- station defense/security configuration.

The active scene is a projection of those facts.

## 3. Station Runtime Projection

When Horizon activates, TA-3 staging builds a `StationRuntimeProjection` containing:

- module runtime instances;
- static station collision;
- kinematic doors/elevators/machinery where required;
- pressure/breach trigger regions;
- traversal/interactions;
- active equipment runtime facets;
- docking interfaces;
- defense/security runtime facets;
- render/light/audio presentation inputs;
- runtime mappings back to stable station IDs.

## 4. Stable Mapping

Every projected object that can affect gameplay maps back to stable identity such as:

```text
StationModuleId
StationPortId
PressureCellId
PressurePortalId
EquipmentSlotId
DockingInterfaceId
StationDamageTargetId
```

Backend `PhysicsBodyHandle`, render handles, and runtime entity handles are process-local only.

## 5. Static Collision

Completed stationary structural geometry is projected to TA-5 as `WorldStatic` collision where appropriate.

Collision geometry comes from authored/cooked module content, not the render mesh by implication.

Blueprint-only geometry is not normal solid station collision unless a build-stage definition explicitly exposes temporary construction collision.

## 6. Kinematic Geometry

Moving station geometry such as:

- doors;
- pressure hatches;
- elevators/lifts;
- moving service machinery;
- docking mechanisms;

uses TA-5 Kinematic bodies or other project-owned motion representations.

The owning station equipment state machine controls desired motion. Physics validates/executes physical movement.

## 7. Door Physical State

A door's canonical state includes logical/security/operability facts plus a physical movement state.

The projected Kinematic body reports actual blocking/opening state back as physical facts.

Traversal and atmosphere portals change according to committed physical door state, not merely the visual animation request.

## 8. Door Obstruction

If a closing door encounters an obstruction, TA-5 produces contact/block facts.

The door equipment policy decides whether to:

- stop;
- reopen;
- apply bounded force;
- fault/jam;
- continue under an explicit heavy/security profile.

Physics does not invent security semantics.

## 9. Pressure Portal Synchronization

Pressure portals reference the real boundary element state.

For a door/hatch:

```text
command open
-> physical movement begins
-> aperture/open fraction updates from committed physical state
-> atmosphere conductance updates
```

The system does not wait for a purely cosmetic animation event nor instantly expose full aperture at button press.

## 10. Breach Projection

A committed breach can produce:

- changed static collision/occlusion;
- atmosphere exterior portal;
- hazard/interaction region;
- visual/audio effect intent;
- navigation invalidation.

All derive from the same persistent BreachState.

## 11. Geometry Delta Queue

TA-6 topology/damage/construction commits publish `StationGeometryDelta` objects.

Delta kinds include:

```text
AddStaticGeometry
RemoveStaticGeometry
ReplaceCollisionShape
CreateKinematicElement
RemoveKinematicElement
ChangeDoorMechanismState
CreateBreach
SealBreach
ChangeDockingGeometry
ChangeTraversalBlocker
```

## 12. Safe Application

`StationGeometryDelta` is applied only during TA-5's deferred safe physics-mutation phase after gameplay commit.

No construction, damage, or repair code directly edits Jolt inside a contact callback or mid-step.

## 13. Delta Revision Protection

Each geometry delta carries:

- source Station topology/damage revision;
- SceneGeneration;
- target stable ID;
- delta sequence.

Stale deltas from an old scene generation or superseded target revision are discarded/reconciled rather than applied blindly.

## 14. Commit Before Projection

The authoritative order is always:

```text
Gameplay validation
-> persistent station transaction commit
-> topology revisions/invalidation
-> runtime geometry delta
-> safe physics mutation
-> render/navigation/read-model update
```

The active scene never mutates first and asks persistence to catch up later.

## 15. Physics Mutation Failure

If a technically valid persistent topology commit cannot be projected due to an implementation/runtime fault, the engine enters a technical recovery/fatal diagnostic path rather than rolling gameplay back silently or maintaining divergent authority.

Implementation must make these projections strongly tested and expected-infallible after prevalidation.

## 16. Active Equipment Leases

High-frequency equipment facets can use TA-2 Activation Leases while Horizon is active.

Examples:

- door motion phase;
- turret aiming state;
- local moving machinery pose;
- active defense tracking runtime;
- temporary interaction state.

Persistent policy, damage, ownership, resource, and network state remains in owning domain stores.

## 17. Leaving Horizon

Before Horizon deactivation:

1. active interactions/transitions reach a safe scene handoff boundary;
2. leased high-frequency station facets commit back to persistent state;
3. relevant physical transforms/mechanism states are reconciled;
4. transient runtime handles are discarded;
5. off-screen scheduler continues from the same Simulation Time/state.

No door, projectile consequence, resource transfer, or damage disappears because the scene unloads.

## 18. Re-Entering Horizon

Scene staging reconstructs runtime geometry/equipment from the latest persistent state.

If a door was damaged open, a compartment depressurized, a turret destroyed, or a module reconstructed while off-screen, the newly projected scene starts in that exact state.

No authored default scene state overrides persistence.

## 19. Docking Handoff

Station docking interfaces expose physical TA-5 capture geometry plus persistent station service/network endpoints.

On hard dock:

- TA-5 owns the physical constraint;
- station/ship domains own docking state;
- TA-6 can add temporary shore-power/logistics/atmosphere/service links only after docking commit;
- undock removes those links in the ordered docking transaction.

Docking never merges station/ship stores.

## 20. Station Defense Projection

When Horizon is active, defense installations project:

- physical mounts/collision;
- targeting/sensor runtime facets;
- weapon runtime interfaces;
- ammo/power/thermal dependencies;
- damage targets.

Combat/TA-7 later owns active combat actors/projectiles. TA-6 owns their station infrastructure inputs and persistent configuration.

## 21. Security Projection

Security infrastructure can project:

- locked/controlled doors;
- checkpoints;
- local sensors;
- alarms;
- access-control terminals;
- containment geometry.

Security policy/state remains persistent and cannot be bypassed because a runtime object was streamed/recreated.

## 22. Construction Mode Runtime

Construction Mode uses a stable station snapshot and preview-only geometry/read models.

Preview geometry:

- is not normal collision authority;
- cannot block actors or seal atmosphere;
- cannot provide power/logistics;
- becomes physical only through committed build-stage transition.

## 23. Incomplete Construction Geometry

When a build definition declares temporary/incomplete physical geometry, that geometry is an explicit construction-state projection with its own collision/traversal metadata.

It is not inferred from progress percentage or visual mesh alone.

## 24. Damage Visual/Physical Separation

Cosmetic damage may change rendering without physics/topology.

Physical collision/topology changes only when the authored damage state creates a committed `StationGeometryDelta`/topology mutation.

## 25. Navigation Handoff

Traversal/geometry/topology changes publish invalidation regions/revisions to TA-8 navigation.

TA-6 does not perform pathfinding synchronously inside station transactions.

## 26. Interaction Handoff

TA-7 later projects station interactive objects using stable equipment/module IDs and TA-2 commands.

An interaction collider/prompt can request a command but cannot directly mutate station stores.

## 27. Atmosphere Force Handoff

Rapid decompression publishes bounded flow facts. TA-5 applies eligible physical force to active characters/props.

Off-screen atmosphere uses equivalent persistent consequences without requiring rigid-body simulation of every loose object.

## 28. Fire / Hazard Handoff

Station event/damage systems own persistent fire/hazard state. Active projection supplies local trigger/visual/physics interfaces; TA-6 environmental networks consume the persistent heat/gas consequences.

## 29. Render Read Models

TA-4 receives immutable station render data representing actual state:

- powered/unpowered lights;
- equipment activity;
- damage/breach visuals;
- radiator/machinery state;
- construction state;
- alarms/effects intents.

Renderer never decides station operation.

## 30. Save Boundary

Runtime physics/render handles are never saved. A save captures persistent station state plus active leased logical facets required by TA-2 export.

On load, Horizon runtime projection is recreated after persistence validation.

## 31. Debugging

Development diagnostics expose:

- stable-ID -> runtime-handle mappings;
- active StationGeometryDelta queue;
- source/target revisions;
- static/kinematic projection state;
- pressure portal aperture vs door pose;
- docking temporary links;
- active lease state;
- stale delta rejection;
- persistent/runtime reconciliation checksums.

## 32. Non-Goals

No runtime scene as persistence authority, no direct Jolt calls from station domains, no preview geometry with gameplay function, no automatic ship/station inventory merge, and no hidden full physics scene while Horizon is off-screen.

## 33. Dependencies

Depends on TA-2 Activation Leases, TA-3 SceneInstance/Horizon handoff, TA-4 render snapshots, TA-5 Physics/docking, all TA-6 station systems, and GDS Station/Spacecraft/Combat/Raids/Security/Defenses.

## 34. Open Questions

None within TA-6 station runtime-projection architecture.
