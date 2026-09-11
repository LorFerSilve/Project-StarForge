# Construction, Repair, and Topology Mutation

> **Status:** Architecture Complete  
> **Authority:** Station blueprint/build state, material/labor progression, commissioning, deconstruction, persistent damage/repair work, and atomic topology mutation/invalidation

## 1. Purpose

Construction and repair are the only routine ways Horizon's physical topology and operational infrastructure change over long-term play.

TA-6 must ensure that those changes:

- consume real resources;
- respect structural/access/safety rules;
- never mutate live physics/graphs halfway through a transaction;
- invalidate dependent utility networks exactly once;
- remain recoverable and persistent.

## 2. Construction Blueprint Record

A persistent `ConstructionBlueprintId` contains:

- module ContentId;
- lattice transform/orientation;
- dependency blueprint/module IDs;
- authored resource requirements;
- reserved/delivered/consumed material milestones;
- current construction state;
- WorkOrderId;
- priority;
- placement-validation revision;
- commissioning requirements/state.

Blueprint geometry is planning data, not completed station topology.

## 3. Placement Validation

Blueprint placement validation consumes immutable snapshots of:

- construction-lattice occupancy;
- structural ports/compatibility;
- planned dependencies;
- required clearances;
- docking/weapon/no-build volumes;
- research/content availability;
- protected station regions.

A valid preview does not reserve or consume resources until the blueprint-placement transaction commits.

## 4. Blueprint Commit

Placing a blueprint atomically:

- allocates ConstructionBlueprintId;
- records placement/dependency data;
- updates planning occupancy;
- optionally creates reservation requests according to player policy;
- creates/schedules construction work state.

It does not create an operational module.

## 5. Build State Machine

Canonical build states mirror the GDS:

```text
Planned
-> AwaitingMaterials
-> Ready
-> UnderConstruction
-> StructuralComplete
-> Commissioning
-> Completed
```

with `Cancelled` terminal alternative.

Transitions are explicit commits, not percentages inferred by renderer animation.

## 6. Material Milestones

Construction definitions specify which resources are:

- required;
- reserved;
- delivered into a ConstructionBuffer owner;
- consumed at explicit build milestones.

No resource is consumed merely because a blueprint exists.

## 7. Labor Progress

Construction uses TA-6 Work Scheduling.

Labor can come from eligible construction systems, crew, robots, or player assistance. Progress only advances while required materials/access/safety/labor remain valid.

## 8. Structural Completion Commit

The `StructuralComplete` transition is a topology mutation transaction.

It revalidates:

- required parent structural attachment;
- occupancy/clearance;
- required material milestones;
- current damage/combat restrictions;
- occupant safety.

On commit it:

- creates the persistent completed StationModuleId if not already allocated by the chosen build implementation;
- activates its structural node/edges;
- activates authored physical boundary geometry appropriate to this build stage;
- increments relevant topology revisions;
- creates a `StationGeometryDelta` for TA-5 safe application.

Functional producers/consumers remain unavailable until commissioning.

## 9. Commissioning

Commissioning validates the module's actual current requirements such as:

- structural connection;
- traversal/service access;
- power/control;
- atmosphere/pressure compatibility;
- cooling;
- water/fluid;
- logistics;
- specialist setup.

Commissioning cannot fabricate a missing connection. A Completed module can later become Unpowered/Isolated/Faulted without reverting construction state.

## 10. Construction Mode Boundary

Construction Mode pauses Simulation Time under the GDS contract and cannot be entered during active station combat.

Placement/planning edits can therefore operate over a stable station snapshot. Exiting/confirming produces normal TA-2 commands/commits.

## 11. Planned Dependency Chains

Blueprint dependencies form an acyclic planning graph.

A child blueprint can be validly planned against a future parent but cannot reach StructuralComplete until the parent's required structural connection is physically committed.

Cycle validation rejects impossible dependency loops.

## 12. Cancellation

Before any material consumption, cancellation removes the blueprint and releases reservations.

After consumption begins:

- unconsumed reservation/delivered material is recovered according to physical ownership/access;
- consumed material is not recreated;
- authored salvage can be produced once through an explicit salvage transaction.

## 13. Deconstruction Plan

Completed permanent modules are removed only through a persistent Deconstruction WorkOrder.

Preflight validation computes:

- dependent structural branch;
- utility/network impact;
- occupants and evacuation routes;
- docked-ship dependency;
- pressure/environmental consequence;
- combat state;
- Command Core protection.

## 14. Joint Deconstruction

If a target is the sole structural bridge, single removal is rejected unless the player confirms a valid joint deconstruction set.

The joint set is captured with source topology revision and revalidated immediately before each irreversible topology milestone.

No hidden cascade deletion occurs.

## 15. Deconstruction Topology Commit

At the physical-removal milestone the transaction:

- changes module/edge presence state;
- invalidates structural/traversal/utility topology;
- preserves/determines remaining local resource/gas states through explicit subsystem rules;
- generates TA-5 geometry deltas;
- creates recoverable salvage only once according to the deconstruction definition.

## 16. Occupant Safety

A topology mutation cannot knowingly remove the only containing/traversal geometry around non-evacuated actors unless an explicit damage/event rule—not Construction Mode—causes that failure.

Construction validation cannot silently teleport occupants.

## 17. Damage Representation

Station Damage owns persistent target records with separate:

```text
StructuralIntegrityState
FunctionalConditionState
FaultState[]
BreachState[]
```

A module can remain structurally intact while functional equipment is Disabled.

## 18. Damage Input Boundary

Combat/Events/Physics provide committed damage/contact/hazard facts to the station damage resolver.

TA-6 resolves those against authored station targets and can produce:

- integrity loss;
- functional degradation;
- fault creation;
- breach creation;
- structural-edge break;
- utility-link loss;
- Destroyed/Wrecked state.

No visual mesh damage alone changes topology.

## 19. Damage Topology Mutation

When damage crosses an authored topology threshold, the damage transaction includes the topology mutation or schedules an immediately ordered dependent mutation within the same authoritative commit sequence.

After commit:

```text
Topology revisions increment
-> canonical structural reachability updates
-> utility links crossing lost structure invalidate
-> pressure topology changes only if pressure boundary was actually compromised
-> downstream station systems re-solve
-> TA-5 geometry delta applies safely
```

## 20. Breaches

A breach has stable target/location identity, severity/conductance, current stabilization state, and repair requirement.

Sealing a breach is a specific repair/stabilization action. Generic “module repaired” does not automatically remove unrelated breaches.

## 21. Repair Task Record

A Repair WorkOrder references:

- exact target/fault/breach;
- severity/priority;
- required resources/components;
- access/environment requirements;
- required capability/tools;
- assigned labor;
- progress;
- blocking reason;
- repair milestone(s).

## 22. Stabilization vs Full Repair

Emergency stabilization and full repair are separate state transitions.

Examples:

- temporary breach patch;
- coolant leak isolation;
- power bypass;
- structural brace;
- fire suppression.

Stabilization can reduce/escalation risk while leaving persistent damage/faults for later repair.

## 23. Repair Material Delivery

Repair inputs are reserved/delivered through TA-6 Logistics. A repair cannot consume inaccessible station-wide inventory.

Emergency repair kits are simply valid local physical input owners.

## 24. Repair Progress

Repair uses TA-6 Work Scheduling and progresses only while:

- valid labor/capability remains;
- required materials remain available/delivered;
- access remains valid;
- environmental safety requirements pass;
- target remains in a compatible repairable state.

Interruption preserves valid progress.

## 25. Repair Completion Transaction

A completion transaction revalidates target revision/fault identity and then atomically:

- consumes final inputs;
- updates structural/functional/fault state;
- removes/seals the exact repaired fault/breach where specified;
- performs any resulting topology restoration;
- emits committed repair event.

It does not clear unrelated faults.

## 26. Reconstruction

Destroyed modules requiring reconstruction use a construction-like work order with explicit wreck/salvage state.

Reconstruction does not resurrect removed resources or reset neighboring damage.

## 27. Repair Priority

Automation default priority mirrors the GDS safety ordering but is represented as configurable WorkOrder policy, not a hidden global modifier.

Manual repair lockouts persist and are respected unless a specifically enabled emergency policy authorizes the exact override.

## 28. Damage During Repair

Targets remain vulnerable during live simulation. New committed damage can:

- change target revision;
- invalidate the current repair milestone;
- add independent faults;
- convert the job to reconstruction.

The scheduler revalidates rather than granting invulnerability.

## 29. Graph Invalidation Coalescing

Several topology mutations committed in one authoritative operation can be coalesced into one dependent-network rebuild epoch.

Coalescing cannot hide intermediate gameplay consequences when separate commits/time boundaries are meaningful.

## 30. Active Scene Handoff

All station collision changes are emitted as project-owned `StationGeometryDelta` records and consumed at TA-5's deferred safe physics-mutation phase.

TA-6 never edits Jolt bodies directly during damage/construction logic.

## 31. Navigation Handoff

Topology/traversal revisions are exposed to TA-8. TA-6 does not synchronously run pathfinding during its commit window.

## 32. Off-Screen Damage / Repair

The same persistent target states, material requirements, labor capacity, and topology mutations apply off-screen.

No rendered physics body is required for off-screen repair progress. Defense/event simulations provide equivalent committed damage facts against persistent targets.

## 33. Persistence

Save state preserves blueprints, dependency graph, construction milestones, material state, work progress, completed modules, deconstruction jobs, structural/functional damage, faults, breaches, stabilization, repair/reconstruction tasks, and topology-driving states.

Derived graph/cache/physics objects are reconstructed.

## 34. Debugging

Diagnostics expose placement validation, planning dependencies, material milestones, work progress, commissioning blockers, deconstruction dependency set, damage layers, faults/breaches, repair requirements, topology invalidations, and emitted geometry deltas.

## 35. Non-Goals

No arbitrary vertex/voxel building, instant relocation, hidden dependent deletion, free repair, full refund, construction during active combat, or direct Jolt mutation from station logic.

## 36. Dependencies

Depends on TA-2 transactions, TA-5 deferred geometry mutation, TA-6 topology/logistics/work scheduling, GDS Construction/Structural Rules/Damage & Repair/Resources/Crew/Robots/Combat/Raids.

## 37. Open Questions

None within TA-6 construction/repair architecture.
