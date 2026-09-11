# Station Automation, Control, and Off-Screen Simulation

> **Status:** Architecture Complete  
> **Authority:** Horizon control/data connectivity, automation task runtime, local safety controllers, policy/permission enforcement, deterministic station-system scheduling, and active/off-screen equivalent simulation

## 1. Purpose

Horizon must keep operating while the player is away without becoming a second hidden full 60 Hz scene.

TA-6 therefore separates:

- persistent physical station systems;
- control/observability connectivity;
- automation tasks/policies;
- high-frequency active local representation;
- deterministic off-screen event/deadline advancement.

## 2. Control/Data Network

`ControlData` utility links from TA-6 topology form one or more `StationControlNetwork` components.

Control nodes include as applicable:

- Command Core controllers;
- local module controllers;
- sensors;
- relays/data buses;
- automation controllers;
- security/defense controllers;
- equipment control endpoints.

A device can remain physically operational while central observability/control is degraded.

## 3. Control Connectivity State

For a controlled endpoint, TA-6 derives states such as:

```text
LocallyControlled
CentrallyConnected
Degraded
Isolated
Unavailable
```

The exact effect belongs to the owning device.

Loss of central control does not erase local protection or physical state.

## 4. Sensor Knowledge Boundary

Automation receives only data available through valid authored sensor/control paths or local controller state.

A failed pressure sensor, for example, does not change atmosphere; it changes what central automation can know and therefore what commands it can safely issue.

No off-screen omniscience is granted merely because the player is not present.

## 5. Local Safety Controllers

Critical devices can own local protection logic that operates without central automation, including authored behavior such as:

- breaker trip;
- reactor protection/shutdown;
- airlock interlock;
- local thermal cutoff;
- emergency door behavior.

Local safety is explicit device behavior, not a global automation cheat.

## 6. Automation Task Store

Persistent automation state is owned by `StationAutomationStore`.

A typed `AutomationTaskId` records:

- task type;
- target;
- required capability;
- required resources/access;
- priority/urgency;
- assigned actor/controller;
- state;
- blocking/escalation reason;
- policy/permission references;
- source event/sensor revision.

## 7. Automation Task Lifecycle

Canonical technical states are:

```text
Detected
-> Queued
-> Assigned
-> InProgress
-> Completed
```

with:

```text
Blocked
Escalated
Cancelled
```

Creating a task is not proof that it can be completed.

## 8. Task Deduplication

Automation uses a stable task key such as:

```text
(TaskType, TargetId, Fault/NeedId)
```

to prevent multiple sensors or repeated scheduler passes from creating duplicate work for the same unresolved condition.

A completed/resolved condition closes or invalidates obsolete tasks.

## 9. Policy Store

Persistent policies include domain-specific settings such as:

- reserve thresholds;
- power/water/cooling priorities;
- repair priority;
- production/farming targets;
- emergency isolation;
- protected-resource permissions;
- automatic defense permissions;
- manual lockouts;
- emergency override permissions.

Policies are explicit data, not hidden AI preferences.

## 10. Manual Override

A player manual lock/disable/isolation command has higher authority than routine automation.

Automation can supersede it only when the exact policy has an explicitly enabled emergency override and current conditions satisfy that policy.

## 11. Protected Resource Permission

A task that can be solved only by consuming protected strategic resources enters `Escalated` / `DecisionRequired` rather than silently spending them unless prior policy authorizes that exact class of use.

## 12. Assignment Boundary

TA-6 determines task eligibility/capability requirements.

TA-8 later owns detailed crew/robot navigation/behavior and reports actor availability/working state.

Fixed local controllers and machinery can execute tasks that do not require mobile actors.

## 13. Finite Capacity

Every automation actor/controller/supervisor has finite declared capacity.

One Engineer, repair robot, or local controller cannot complete unlimited simultaneous work.

Scheduling consumes capacity explicitly.

## 14. Escalation

A task escalates when conditions such as these hold:

- no eligible actor/controller;
- required resource missing;
- route/access impossible;
- sensor confidence insufficient;
- authority/permission missing;
- repeated automatic attempt failed;
- urgency exceeds configured authority.

The escalation record contains the actual cause.

## 15. Automation Reports

Automation publishes `AutomationReportSeverity` values as defined by the GDS.

These are not `AlarmPriority` values. Presentation/Alarm systems map an underlying incident to alarms independently.

## 16. Station Simulation Scheduler

Station persistent systems are coordinated by a project-owned `StationSimulationScheduler` driven exclusively by global Simulation Time.

It owns deterministic update ordering and a deadline/event queue for coarse systems.

## 17. Station Phase Order

For a normal station simulation slice, the semantic order is:

1. apply committed topology/policy/actor availability changes;
2. rebuild invalidated topology views;
3. resolve Power/control availability;
4. resolve Water/Logistics deliveries due at the boundary;
5. advance Thermal/Atmosphere environmental state;
6. evaluate device protection/operability changes;
7. advance Manufacturing/Farming/Construction/Repair work;
8. process routine crew aggregate consumption/production facts;
9. detect new faults/shortages/automation tasks;
10. assign/advance eligible automation work;
11. process scheduled station-local events;
12. publish read models/alarms/next deadlines.

When a consequence commits a new topology mutation, affected downstream phases are invalidated/re-run in deterministic order before stale state is used.

## 18. Why Power Precedes Most Active Work

Power allocation is needed to determine whether pumps, machines, life support, and controllers can operate during the slice.

Thermal consequences can then reduce future capability and trigger shutdown/reallocation at the next deterministic boundary within the same chronological simulation advance.

No circular algebraic “solve everything simultaneously” is required.

## 19. Significant Boundary Queue

The scheduler tracks the earliest future Simulation-Time boundary from systems such as:

- storage/tank depletion/full;
- battery reserve/empty/full;
- temperature threshold;
- atmosphere/habitability threshold;
- crop tolerance/harvest;
- work completion;
- spoilage;
- repair completion;
- automation retry/escalation;
- Dynamic/Horizon defense event;
- scheduled local station event.

## 20. Active Horizon Mode

When Horizon is the Active Local Context:

- persistent station systems remain authority;
- actor/physics/render representations are projected locally;
- fast doors, impacts, decompression forces, combat, and interactions can operate at TA-1/TA-5 frequencies;
- lower-frequency network/work systems still use StationSimulationScheduler cadences.

Being rendered does not replace persistent station stores.

## 21. Off-Screen Horizon Mode

When the player leaves Horizon:

- the same persistent stores remain active;
- no second full PhysicsWorld/renderer/nav scene is kept alive;
- high-frequency actor state returns through TA-2 Activation Lease handoff;
- station systems advance through scheduled Simulation-Time slices/events;
- off-screen tactical defense uses its own bounded abstraction under Raid/Defense architecture.

## 22. Chronological Coarse Advance

To advance from Simulation Time `T0` to `T1` off-screen:

1. find earliest significant system/event boundary `Tb <= T1`;
2. integrate continuous conserved-state systems only to `Tb`;
3. advance work/transfers only to `Tb`;
4. commit all facts caused at `Tb` in deterministic order;
5. rebuild invalidated dependencies;
6. schedule resulting new boundaries;
7. repeat until `T1`.

The scheduler cannot jump directly from `T0` to `T1` if an earlier event changes the future trajectory.

## 23. Equivalent Outcome Requirement

Off-screen optimization may use larger mathematical steps but must preserve gameplay-equivalent outcomes for the same starting state and external commands/events.

It may not grant:

- production after power loss would have stopped it;
- crop growth after water depletion;
- continued cooling after pump failure;
- repair after actor loss;
- missed decompression threshold;
- skipped raid damage;
- extra resources from step rounding.

## 24. Threshold Splitting

Every conserved/process system provides `NextBoundaryTime()` or equivalent predictive deadline when possible.

When exact prediction is not possible, it provides a maximum safe integration horizon and can subdivide deterministically.

TA-13 later sets performance caps, but correctness wins over coarse-step size.

## 25. Horizon Defense Event Integration

A station attack injects committed/scheduled defense facts into the same persistent station stores:

- damage;
- resource theft/transfer;
- crew/robot state;
- doors/security;
- power/thermal/atmosphere consequences.

Off-screen defense never operates on a duplicate “raid copy” of Horizon.

## 26. Player Return Mid-Event

If Horizon becomes active during an unresolved defense/emergency:

- scheduler advances persistent state to the transition commit time;
- high-frequency actor representations are reconstructed from that same state;
- the same event IDs, attackers/defenders/assets/damage/resources continue;
- no event restart or resource reset occurs.

## 27. Remote Commands

Strategic Communication rules govern whether the player can observe or command Horizon while away.

When communication is unavailable, the scheduler still executes existing policies/local automation but accepts no magical live player command.

Delayed messages/commands keep their actual communication timestamps/order.

## 28. True Pause / Holds

If global Simulation Time is stopped by True Pause or Hard Streaming Hold, StationSimulationScheduler also stops.

Workers may prepare non-authoritative results, but no station timer/work/resource/environment progress commits.

## 29. Save Boundary

A save captures persistent station stores plus scheduler/deadline state at a TA-2 Stable Save Boundary.

No automation/work/network commit is half-applied.

Derived next-boundary queues can either be serialized when useful or deterministically rebuilt from persistent state and current Simulation Time.

## 30. Failure Handling

A solver/task failure is surfaced as explicit `Blocked`, `Faulted`, or technical diagnostic state. TA-6 does not silently assume success to keep off-screen simulation moving.

A technical invariant violation aborts the affected simulation operation in development/testing and must not fabricate resources/state.

## 31. Debugging

Development tools expose:

- ControlData graph/connectivity;
- sensor confidence/knowledge;
- automation tasks/assignments/capacity;
- policy/permission/lockout reason;
- scheduler current time;
- pending deadlines/events;
- active/off-screen mode;
- coarse-step splits;
- per-system advance duration;
- equivalence/reconciliation counters.

A deterministic trace can record boundary sequence for replay/regression testing.

## 32. Non-Goals

No real-world offline progression, hidden omniscience, autonomous strategic decisions, infinite automation capacity, second hidden Horizon scene, or coarse stepping that skips gameplay consequences.

## 33. Dependencies

Depends on TA-1 Simulation Time, TA-2 state/events/transactions, TA-3 Horizon handoff, TA-6 topology/network/work systems, GDS Automation, Crew/Robots, Systems Time/Communications, Raids/Station Defense, and Presentation alarms.

## 34. Open Questions

None within TA-6 automation/off-screen simulation architecture.
