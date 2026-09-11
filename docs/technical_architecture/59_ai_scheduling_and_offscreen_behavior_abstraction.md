# TA-8 — AI Scheduling and Off-Screen Behavior Abstraction

> **Status:** Architecture Complete
> **Authority:** AI update scheduling, active/reduced-detail modes, off-screen logical movement/task execution, active↔off-screen handoff, station automation actor coordination, and equivalence constraints

## 1. Purpose

StarForge cannot run every crew member, robot, enemy, and off-screen station actor with full 60 Hz perception/pathfollowing at all times. Reduced-detail AI is allowed only when it preserves the same gameplay-relevant causality, capabilities, resources, timing, hazards, and ownership.

## 2. AI Simulation Detail Modes

Canonical modes:

1. `ActiveLocal` — actor has TA-7 runtime entity and local high-detail AI;
2. `ReducedLocal` — actor remains inside the active local context but receives reduced expensive decision/perception cadence while still physically represented/authoritative where needed;
3. `OffScreenLogical` — no local runtime locomotion entity; persistent/logical AI state advances through topology/task abstractions;
4. `Dormant` — no currently progressing behavior beyond persistent state/deadlines explicitly owned elsewhere.

A gameplay facet has exactly one authority mode at a time.

## 3. ActiveLocal

ActiveLocal actors can use:

- TA-7 runtime components;
- TA-5 movement/physics;
- detailed path following;
- local perception;
- combat actions;
- interaction/traversal links.

High-level decisions can still be scheduled below 60 Hz.

## 4. ReducedLocal

ReducedLocal is a performance scheduling mode, not a different ruleset.

It may reduce frequency of:

- perception refresh;
- tactical rescoring;
- nonurgent path replans;
- cosmetic/off-duty decisions.

It may not skip:

- actual physical movement/collision when represented;
- due status/action deadlines;
- damage;
- critical hazard reaction boundaries;
- command changes;
- ownership transactions.

## 5. OffScreenLogical

Off-screen actors are represented by persistent semantic state such as:

- logical location/compartment/module/route segment;
- current task/command;
- travel origin/destination;
- travel segment progress/deadline;
- Health/damage/status where applicable;
- equipment/ammo/energy/resources;
- known hazards/access;
- communication/knowledge state;
- assignment/ROE/fallback.

Centimeter-level transform, local avoidance, animation, and backend handles are not required.

## 6. No Off-Screen Teleport Advantage

Off-screen movement has explicit Simulation-Time cost derived from logical route segments and actor mobility.

An actor cannot complete travel faster because its scene is unloaded.

Blocked doors, disconnected station branches, airlocks, incompatible Heavy routes, hazards, and required equipment remain relevant.

## 7. Logical Navigation Graph

For Horizon/off-screen station movement, TA-8 derives a persistent/logical traversal graph from TA-6 topology:

- compartment/module nodes;
- traversable connection edges;
- door/airlock/elevator state;
- security/access;
- hazard state;
- size/mobility compatibility;
- estimated physical travel cost.

This is not a second physical station simulation and is reconstructible from persistent station state.

## 8. Logical Route Execution

An off-screen route is a sequence of semantic segments.

Each segment defines:

- start/end logical location;
- minimum travel duration/cost;
- traversal requirements;
- relevant topology/access revision;
- hazard/protection requirements;
- next significant boundary.

The actor advances only to the earliest valid segment/event boundary before revalidation.

## 9. Chronological Boundaries

Off-screen actor advance participates in TA-6 chronological scheduling.

Potential boundaries include:

- arrival at next compartment/portal;
- airlock/elevator phase completion;
- hazard threshold;
- task start/completion;
- resource/ammo/energy depletion;
- injury/incapacitation;
- assignment/command change;
- station topology change;
- raid/defense event;
- communication change.

The scheduler cannot jump past an earlier consequence.

## 10. Station Automation Actor Coordination

TA-6 remains owner of `AutomationTask` creation, priority, policy, resources, and persistent task state.

TA-8 exposes `MobileActorAvailability` containing:

- actor identity/type;
- assignment/role capability;
- current logical location;
- availability/current task;
- protection/equipment capability;
- estimated route/access status;
- finite task capacity.

TA-6 can assign a task through its persistent policy/assignment rules; TA-8 then executes travel and hands-on work.

## 11. Execution Reporting

Mobile AI reports typed execution facts back to TA-6, for example:

- Accepted;
- Traveling;
- Arrived;
- InProgress;
- Blocked(Path);
- Blocked(Resource);
- Blocked(Hazard);
- Interrupted;
- ActorUnavailable;
- PhysicalWorkMilestoneReached.

The report does not directly mark a repair/manufacturing task complete unless the owning TA-6 transaction commits it.

## 12. Active ↔ Off-Screen Handoff

When an actor leaves high-detail simulation:

1. stop accepting new local-only action intents;
2. settle due physical/combat/interactions at Stable Simulation Boundary;
3. return TA-2 leased persistent facets;
4. derive authoritative logical location and semantic task state;
5. unregister runtime path/perception/AI state;
6. enter OffScreenLogical mode.

Reverse activation reconstructs runtime state from the same persistent/logical record.

## 13. Handoff Position

On activation, a physical spawn position must be consistent with the actor's current logical location/travel state and valid current navigation/collision.

It cannot be chosen to provide tactical advantage, bypass a locked boundary, or skip remaining travel.

If the actor is mid-logical transit, activation either reconstructs a valid location on that segment or advances/rewinds only according to an explicit deterministic segment projection rule.

## 14. Off-Screen Combat Boundary

TA-8 does not invent a generic hidden combat simulator for all contexts.

- Horizon defense/off-screen tactical resolution belongs to TA-9/Raid architecture using persistent actors/resources;
- mission actors within the one deployed local Mission Instance normally remain governed by mission/runtime activation rules;
- off-screen local actors can perform noncombat travel/work and bounded authored behaviors.

Any future reduced combat resolver must obey the same Health/ammo/energy/knowledge/physical-access semantics and be explicitly specified.

## 15. Off-Screen Robot Commands

A robot in OffScreenLogical mode continues an already valid command/fallback only when the command does not require active-local precision unavailable to the abstraction.

Examples that can often continue:

- Return to Transport along known station/ship route;
- move between station logical locations;
- routine station repair assignment;
- recharge/maintenance travel.

Precision tactical combat commands are resolved by the appropriate active/raid context, not approximated ad hoc.

## 16. Off-Screen Crew Behavior

Crew can continue:

- traveling to assignment/workplace;
- routine work under TA-6 WorkOrders;
- evacuation/muster movement;
- medical/recovery movement;
- emergency repair/response tasks.

All use actual logical routes, durations, equipment, hazards and capacity.

## 17. AI Scheduler

The active-local runtime owns a project `AIScheduler` that determines which agents receive expensive perception/high-level decision work on each Simulation-Time tick.

The scheduler does not own AI state; it schedules due updates.

Inputs include:

- actor stable identity;
- AI class/role;
- current urgency/awareness;
- last/next update tick;
- pending urgent interrupt;
- current context relevance.

## 18. Scheduling Classes

Conceptual scheduling classes:

- `CriticalImmediate` — urgent interrupt requiring next valid AI phase;
- `CombatActive`;
- `CommandActive`;
- `RoutineActive`;
- `IdleCosmetic`.

Exact periods/budgets are TA-13 parameters.

No class is allowed to miss authoritative deadlines owned by TA-7/Combat/Status/Station systems.

## 19. Deterministic Scheduling

For agents due on the same tick, scheduler order is stable by:

1. scheduling class/semantic priority;
2. due tick;
3. persistent stable identity if available, otherwise runtime stable sort key.

CPU pressure cannot arbitrarily skip one actor based on container order.

## 20. Backlog Policy Boundary

TA-13 will define numeric per-frame budgets/backlog limits.

Correctness rule is fixed now:

- urgent/life-critical updates cannot be silently dropped;
- routine expensive decisions may be deferred to a later Simulation-Time tick within their allowed cadence;
- deferred scheduling cannot advance an actor's behavior without running the missing decision;
- overload diagnostics must be visible.

## 21. Worker Jobs

Pathfinding and other pure expensive calculations may run on workers using immutable snapshots.

High-level AI authority remains on main simulation thread.

Every worker result carries freshness revisions/epochs and is consumed only at defined TA-7/TA-8 phases.

## 22. No Completion-Order Semantics

If two path/AI-support worker results finish in opposite orders on two machines, semantic processing order remains the stable scheduler/actor/request order.

Worker completion timing never determines target priority, task assignment, or combat result.

## 23. True Pause and Holds

All AI scheduling, off-screen travel, perception memory decay, crew work movement, and robot command progress use Simulation Time.

True Pause and TA-3 Hard Streaming Hold freeze their gameplay progression.

Workers may technically finish an already-submitted calculation during a hold, but the result cannot advance gameplay until a valid simulation phase consumes it.

## 24. Save / Load

Save captures semantic AI/task/travel state at a Stable Save Boundary.

On load:

- no wall-clock time is added;
- runtime schedulers reconstruct next due updates deterministically;
- worker jobs are resubmitted as needed;
- off-screen travel resumes from committed logical progress;
- no resource/ammo/energy is restored merely because high-detail state was unloaded.

## 25. Equivalence Tests

Representative tests compare active vs off-screen outcomes for identical starting state/external events, including:

- crew traveling through multiple compartments;
- temporary door closure;
- repair task interrupted by hazard;
- robot recharge return;
- evacuation before decompression threshold;
- assignment target becoming inaccessible;
- player returning during unresolved task/emergency.

Results must match gameplay-relevant state even if centimeter-level paths differ.

## 26. Diagnostics

Diagnostics expose:

- AI detail mode per actor;
- next scheduled update;
- scheduler class;
- deferred update/backlog count;
- off-screen logical route/segment/deadline;
- active/off-screen handoff history;
- TA-6 task execution reports;
- stale worker rejects;
- equivalence-test traces.

## 27. Explicit Non-Goals

No second hidden full Horizon AI scene, no off-screen teleport, no off-screen free repair/resources, no generic unresolved hidden combat shortcut, no wall-clock progression, and no worker completion race as AI authority.

## 28. Dependencies

Depends on TA-1 scheduling/workers, TA-2 Activation Leases, TA-3 active/off-screen model, TA-6 station scheduler/automation, TA-7 runtime phases, and `52`–`58`.

## 29. Open Questions

None. Numeric scheduler budgets are intentionally deferred to TA-13.