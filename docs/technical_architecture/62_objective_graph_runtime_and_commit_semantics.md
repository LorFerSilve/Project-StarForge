# TA-9 — Objective Graph Runtime and Commit Semantics

> **Status:** Architecture Complete  
> **Authority:** Objective definition/runtime separation, DAG evaluation, authoritative progress sources, exactly-once state transitions, timed objectives, branching, and mission outcome derivation

## 1. Purpose

Objectives must translate committed world/gameplay facts into explicit mission progression without becoming a second copy of Health, inventory, resource, combat, rescue, or world state.

TA-9 therefore defines objective state as **derived/committed mission progress over authoritative domain facts**, not as arbitrary counters mutated by presentation or interaction callbacks.

## 2. Definition Versus Runtime State

Each mission contains immutable or generated `ObjectiveDefinition` data and persistent per-instance `ObjectiveRecord` state.

`ObjectiveDefinition` contains at minimum:

- ObjectiveId unique within MissionId;
- class: Primary / Secondary / Optional / Extraction / Hidden;
- authored description/content references;
- mandatory/optional semantics;
- prerequisite expression;
- activation/start rule;
- completion condition;
- failure condition;
- visibility/discovery rule;
- target references or target-selection rule;
- progress projection rule;
- optional timer rule;
- downstream branch effects.

`ObjectiveRecord` contains:

- current GDS objective state;
- discovery/visibility state where needed;
- committed progress metadata only when no stronger source exists;
- timer start/deadline when applicable;
- terminal transition TransactionId;
- branch-selection data;
- objective revision.

## 3. Objective IDs

ObjectiveId is stable within one MissionId and persisted as mission-local identity.

It is not a RuntimeEntityHandle.

Unique targets are referenced by persistent IDs or stable generated mission-local persistent references rather than by scene object pointers.

## 4. Directed Acyclic Graph

The objective dependency graph must be acyclic.

Generation/content validation rejects:

- direct cycles;
- indirect cycles;
- unreachable mandatory objectives;
- mandatory hidden objectives with no valid discovery route;
- contradictory terminal branches.

Prerequisite expressions support explicit AND / OR semantics.

## 5. Canonical States

Objective state is exactly the GDS set:

```text
Locked
Available
Active
Completed
Failed
Cancelled
```

Terminal objective states are `Completed`, `Failed`, and `Cancelled` unless an authored retry creates a new MissionInstance.

A save/load never reopens a terminal objective in the same instance.

## 6. Objective Evaluation Model

Objective evaluation consumes typed committed facts and immutable owning-domain read state.

Examples:

- Combat outcome fact: target `RobotId(42)` reached Destroyed;
- Inventory/security read: 40 Titanium is Vehicle-Secured;
- Rescue read: CharacterId is Boarded/Secured Passenger;
- Repair read: generator operational state is Online;
- Scan commit: required ScanDataId exists;
- World state: relay route is Verified;
- Extraction commit: ExtractionTransactionId succeeded.

The evaluator produces a proposed objective transition. The transition is not authoritative until the Mission domain commits it.

## 7. No Shadow Counters When Source State Exists

TA-9 must not maintain a second authoritative quantity when another domain already owns the truth.

Example:

```text
Objective: secure 40 Titanium
```

The objective should query/tag eligible secured physical ownership rather than incrementing a hidden `oreCounter` every time a pickup callback fires.

A dedicated progress value is permitted where progress is intrinsically objective-owned, such as:

- 2/3 abstract calibrated scan nodes whose scan commits are themselves objective-local evidence;
- elapsed authored timer state;
- branch-specific sequence steps.

## 8. Typed Condition Families

The baseline technical condition vocabulary supports project-owned typed evaluators such as:

- `DomainPredicateCondition`;
- `PersistentEntityStateCondition`;
- `QuantityAndSecurityCondition`;
- `CommittedInteractionCondition`;
- `ScanEvidenceCondition`;
- `TimerDeadlineCondition`;
- `ExtractionCommitCondition`;
- `WorldFlagCondition`;
- `CompositeCondition` using AND/OR.

TA-10 later defines final content serialization/schema. TA-9 does not require a general scripting VM for objective logic.

## 9. Activation and Unlocking

After a committed objective transition, the Mission objective resolver evaluates dependents in deterministic ObjectiveId order.

A Locked objective becomes Available only when its prerequisite expression is satisfied.

Becoming Available does not automatically mark it Active unless its definition specifies immediate activation.

## 10. Branch Selection

When an authored OR branch is exclusive, the first valid committed branch selection can:

- record the chosen branch;
- unlock its downstream objectives;
- Cancel mutually exclusive objectives.

Branch selection itself is a mission-domain transaction when later consequences depend on it.

Worker timing, UI order, or hash iteration cannot choose a branch.

## 11. Completion Exactly Once

Completion follows:

1. owning-domain state/fact satisfies the condition;
2. Objective resolver validates prerequisites and current state;
3. a prepared ObjectiveTransition transaction is created;
4. the objective state changes to Completed;
5. transition TransactionId is stored;
6. dependent unlocks/mission consequence intents are emitted after commit.

Repeated facts after completion are ignored/idempotently recognized.

## 12. Failure Exactly Once

Failure follows the same transaction discipline.

A mandatory Primary failure causes Mission failure only after the objective graph proves there is no remaining valid authored alternate branch satisfying the mission success graph.

Optional objective failure never fails the base mission by itself.

## 13. Cancellation

Cancelled means the objective is no longer relevant due to a committed branch, mission abandonment, extraction closure, or explicit world invalidation.

Cancelled is not Failed and cannot trigger failure-only consequences unless its definition explicitly says the cancellation source has another consequence.

## 14. Combat Objective Semantics

Combat conditions must reference an explicit expected end state:

- Kill;
- Destroy;
- Disable;
- DriveOff;
- Capture.

There is no implementation-level generic `Neutralized` condition unless it resolves to an explicit authored set of acceptable committed states.

## 15. Resource Security Semantics

Resource objectives encode the required security class explicitly:

- Acquired;
- Field-Unsecured;
- Vehicle/Extraction-Secured;
- Delivered/Station-Secured.

An item changing owner/security state can therefore cause progress to rise or fall before terminal completion where the objective definition permits dynamic progress.

Completion that must remain permanent after a valid hand-in uses a committed delivery/turn-in transaction, not a continually queried carried quantity.

## 16. Rescue Semantics

Rescue conditions use exact authored stages:

- Located;
- Stabilized;
- Escorted;
- Boarded/Secured Passenger;
- Delivered.

Proximity to a ship is not Boarded.

A rescue objective cannot invent passenger capacity or teleport the target.

## 17. Repair and Sabotage Semantics

A Repair objective completes only when the target system reaches the required committed operational state.

A Sabotage objective similarly specifies its required persistent result, for example:

- Disabled;
- Isolated;
- Destroyed;
- Compromised;
- timer-bounded offline state.

Starting an interaction is never sufficient.

## 18. Scan / Evidence Semantics

A scan completes only when the owning scan/research interaction commits the required evidence/data result.

Looking at a target, showing a progress bar, or rendering scanner VFX has no objective authority.

## 19. Timed Objectives

Timed objective start commits:

- TimerStartSimulationTime;
- DeadlineSimulationTime;
- timer definition/version.

The deadline advances only with Simulation Time.

True Pause and other global Simulation-Time holds therefore stop it automatically.

The expiration check is processed at deterministic strategic/runtime deadline boundaries and commits exactly one defined result.

## 20. No Hidden Mandatory Timer

Presentation reads a mandatory active timer only after its start has committed.

The MissionStore cannot run a secret mandatory countdown that the player-facing knowledge model is forbidden to expose by the GDS.

## 21. Hidden / Discoverable Objectives

Hidden objective existence is kept out of player read models until its discovery condition commits.

The objective system can know the definition without revealing its target marker, exact location, or requirement to UI/AI that lacks the knowledge.

## 22. Marker Knowledge Boundary

Objective marker data is generated from mission/player knowledge:

- exact point;
- bounded search area;
- direction/general area;
- no marker.

Marker generation never reads hidden world truth solely for convenience.

## 23. Multi-Zone Progress

Objective records live in MissionInstance persistent state, not in one sub-zone Scene.

Leaving/reloading a sub-zone preserves objective state.

A target destroyed or collected in an unloaded previous sub-zone remains so because the target/world owner preserves the committed consequence.

## 24. Extraction Objective

Extraction is represented as an ObjectiveDefinition only where the mission requires it, but its completion predicate is the authoritative extraction commit from the extraction/ownership transaction.

Entering a trigger volume is insufficient.

## 25. Mission Outcome Resolver

After every terminal/branch-relevant objective transition, `MissionOutcomeResolver` evaluates the mission success/failure graph.

It can propose:

- no mission change;
- Extraction Available;
- Succeeded for an in-place-resolution mission;
- Failed;
- optional/secondary consequence preparation.

The proposal still commits through MissionStore state transitions.

## 26. Same-Tick Conflicts

If same-tick facts can produce conflicting objective results, ordering is determined by explicit semantic priority and stable transaction/fact identity.

Example: protected target delivered and destroyed in the same simulation boundary.

The owning target/transfer domains first settle the authoritative physical/health state; Objective resolution then observes the committed ordered result rather than arbitrarily processing callbacks.

## 27. Persistence and Replay Safety

Each terminal transition retains enough idempotency data that load cannot replay:

- one-time reward triggers;
- world-state consequences;
- branch choice;
- narrative beat;
- extraction success.

Objective read/progress state can be reconstructed from owning domains where appropriate, but terminal commits remain explicit history.

## 28. Debugging

Developer tooling exposes:

- DAG visualization;
- objective current state;
- prerequisite satisfaction;
- active completion/failure predicates;
- source domain revisions;
- timer deadline;
- terminal TransactionId;
- hidden-state visibility separately from player-known visibility.

Debug views are read-only unless a typed development command explicitly changes simulation state.

## 29. Test Requirements

Tests prove:

- DAG validation and cycle rejection;
- deterministic AND/OR unlock ordering;
- exactly-once completion/failure;
- optional failure does not fail mission;
- alternate branch prevents premature mission fail;
- no duplicated resource counter against physical ownership;
- security-state requirements are respected;
- true pause freezes timers;
- hidden objectives do not leak through read models;
- extraction trigger alone cannot complete extraction objective;
- save/load cannot replay terminal consequences.

## 30. Explicit Non-Goals

No cyclic objective graphs, arbitrary string-evaluated scripting baseline, hidden mandatory timers, ambiguous neutralization, UI-owned progression, or callback-owned rewards.

## 31. Dependencies

Depends on `61`, TA-2 transactions/read models, TA-7 committed gameplay facts, TA-8 knowledge boundaries, GDS-8 Objectives/Extraction, and owning combat/resource/crew/world systems.

## 32. Open Questions

None in the objective-runtime baseline.