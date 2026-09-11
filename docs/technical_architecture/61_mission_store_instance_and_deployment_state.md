# TA-9 — Mission Store, Mission Instances, and Deployment State

> **Status:** Architecture Complete  
> **Authority:** Persistent mission records, mission-attempt identity, external deployment exclusivity, mission-instance ownership, retry history, and persistent/runtime mission handoff

## 1. Purpose

TA-9 turns the Design Complete mission lifecycle into explicit technical state ownership without making the active scene or objective UI the mission authority.

The architecture distinguishes:

- the persistent **MissionRecord** describing one canonical mission opportunity/contract;
- a persistent **MissionInstanceRecord** describing one deployed attempt/instance;
- the active local runtime projection used while that instance is the current SceneInstance;
- historical attempt records retained where retry/history requires them.

## 2. Persistent Mission Identity

`MissionId` remains the canonical persistent identity of the mission opportunity.

A `MissionRecord` contains at minimum:

- `MissionId`;
- mission class and primary/secondary type;
- source/sponsor;
- Strategic Location / target identity;
- authored definition/content references;
- stable generation seed/version where procedural;
- persistent generated-content package reference/state;
- threat/hazard knowledge state;
- deployment requirements;
- objective graph definition/state summary;
- extraction policy;
- reward definition and resolution transaction references;
- faction/narrative context;
- canonical GDS lifecycle state;
- retry policy/history;
- current/latest `MissionInstanceId`, when applicable;
- mission revision.

Runtime scene objects never replace this record.

## 3. Mission Instance Identity

TA-9 introduces a strongly typed persistent `MissionInstanceId` under TA-2's `PersistentId<Tag>` rules.

A new ID is allocated atomically when a deployment attempt commits.

A MissionInstanceId is useful because one MissionId can have more than one attempt only where an explicit retry/recovery policy permits it, especially mandatory story content.

`MissionInstanceId` is never a RuntimeEntityHandle and is never regenerated on load.

## 4. One External Deployed Mission

The session root owns an optional:

`ActiveExternalMissionInstanceId`.

The deployment transaction requires it to be null before committing an external mission instance.

The same transaction:

1. revalidates MissionRecord state and deployment requirements;
2. allocates MissionInstanceId;
3. creates the MissionInstanceRecord;
4. associates selected ship/player/robot squad and mission-carried context;
5. marks the MissionRecord as Deployed/appropriate transition state;
6. sets `ActiveExternalMissionInstanceId`;
7. prepares the TA-3 destination local-context transition.

This invariant prevents two external missions from owning the player, ship, extraction state, or mission-zone authority simultaneously.

A Horizon `DefenseEventId` is not stored in this slot and can therefore coexist with the one external deployed mission as required by the GDS.

## 5. Mission Lifecycle Representation

Mission lifecycle uses the GDS states without inventing a second semantic state machine:

```text
Discovered
-> Available
-> Accepted
-> Prepared
-> En Route
-> Deployed
-> Active
-> Extraction Available
-> Extracting
-> Succeeded | Failed | Abandoned
-> Resolved
```

Transient implementation substates may exist only when they cannot change player-visible semantics and remain subordinate to the owning GDS state.

All lifecycle transitions are typed commands validated and committed on the authoritative simulation thread.

## 6. Accepted and Prepared

Acceptance creates a persistent commitment record but does not instantiate a local zone or reserve physical resources unless the mission definition explicitly owns such a reservation.

Preparation stores a validated deployment selection, including where relevant:

- ShipId;
- selected loadout references;
- selected Tactical Squad RobotIds;
- required cargo/passenger plan;
- selected route;
- known hard-block/warning evaluation revision.

Preparation is revalidated before actual deployment because world, ship, inventory, route, or station state may have changed.

## 7. Hard Blocks and Warnings

Deployment validation returns structured results:

- `HardBlock[]` — impossible under current known physical/logical state;
- `Warning[]` — possible but materially risky;
- `Ready`.

A Warning can be explicitly accepted by player intent.

A HardBlock cannot be overridden by presentation/UI.

## 8. En Route

`EnRoute` owns the mission deployment intent while strategic travel executes through the spacecraft/world travel authority.

Mission state does not teleport the player or deduct travel resources itself.

Travel costs and strategic movement commit through their owning systems.

A failed/cancelled strategic departure leaves the mission in the correct pre-deployment state according to the prepared transaction rather than creating a half-deployed instance.

## 9. MissionInstanceRecord

A MissionInstanceRecord persists at minimum:

- MissionInstanceId;
- owning MissionId;
- attempt ordinal;
- stable mission/instance seed references;
- deployment commit Simulation Time;
- current mission-zone/sub-zone identity;
- persistent spawned/unique actor references;
- objective runtime records;
- POI discovery state;
- harvested/depleted deposits;
- opened/consumed containers;
- rescued/protected-character state;
- hazard state/schedules;
- mission-local dropped physical items and ownership references;
- extraction-point/state data;
- committed local world changes;
- attempt outcome/failure reason;
- applied transaction IDs needed for idempotency;
- instance revision.

The instance is not a serialized copy of all active TA-7 components. Active runtime state is exported through TA-2 lease/deactivation contracts into the relevant persistent owners and instance-owned fields.

## 10. Active Local Projection

When a MissionInstance is the active player-local context:

```text
MissionRecord + MissionInstanceRecord + World/Location state
        ↓
TA-3 Scene staging
        ↓
TA-7 runtime entities
TA-8 navigation/AI
TA-5 physics
```

Scene residency, visibility, or RuntimeEntityHandle lifetime does not determine mission persistence.

## 11. Multi-Zone Missions

A single MissionInstance can contain multiple local sub-zones.

Sub-zone transitions update current zone identity and persistent zone state while preserving:

- MissionInstanceId;
- objective graph;
- secured cargo/passengers;
- discovered POIs;
- defeated/disabled persistent actors;
- opened/harvested state;
- extraction state.

Changing sub-zone never creates a new MissionId.

## 12. Mission Seed and Attempt Seed

The mission's committed major procedural package is stable for its MissionId.

A story retry does not silently reroll major content.

Where an authored retry explicitly permits attempt-local variation, that variation uses a deterministic child stream derived from:

- MissionId;
- stable mission seed;
- attempt ordinal;
- authored retry-generation version.

The permitted reset/variation fields must be declared by content. Persistent world consequences from the previous attempt are never reverted merely to make a retry pristine.

## 13. Attempt Outcome Versus Mission Resolution

`Failed` or `Abandoned` can describe the outcome of the currently deployed attempt.

For ordinary procedural/side missions, that normally proceeds to terminal resolution under the contract.

For retryable mandatory story missions, an explicit recovery command can transition the MissionRecord from the failed-attempt state to its authored retry-ready state and later create a **new MissionInstanceId**.

The historical failed MissionInstance remains recorded; no attempt ID is reused.

## 14. No Automatic Refund on Retry

Creating a new attempt does not refund or resurrect:

- ammunition;
- fuel/propellant;
- consumables;
- destroyed robots;
- committed target damage;
- transferred/stolen resources;
- legitimately committed world changes.

An exception requires an explicitly authored protected tutorial/story reset policy and an exactly defined transaction scope.

## 15. Mission Success

The MissionRecord can enter `Succeeded` only after the objective/extraction framework reports all authored mandatory success conditions committed.

The mission state machine does not infer success from:

- player reaching a location;
- loot pickup alone;
- all enemies being dead;
- a UI screen opening.

## 16. Failure and Abandonment

Failure is committed only when an authored mandatory condition becomes irrecoverably impossible or an explicit failure trigger commits.

Abandonment is a player command. After deployment it does not move the player or ship; it closes success-only mission progression while valid withdrawal/extraction and ordinary defeat rules continue.

Consumed and physically transferred state remains committed.

## 17. Resolution Transaction

`Resolved` is entered only after a prepared resolution transaction settles all required consequences, such as:

- one-time reward state;
- reputation/faction consequences;
- narrative flags;
- world-location consequences;
- generated follow-up event requests;
- instance closure/conversion;
- clearing `ActiveExternalMissionInstanceId` when appropriate.

The MissionRecord stores the resolution TransactionId or equivalent idempotency marker.

Re-running resolution after load is therefore rejected as already committed.

## 18. Active Instance Closure

A successful/failed/abandoned instance may remain physically active long enough for valid withdrawal or aftermath behavior.

Runtime closure occurs only when the owning mission/extraction/context-transition conditions permit it.

Clearing the external deployed slot and destroying the current scene are separate but ordered operations.

## 19. Save Boundary

Mission acceptance, deployment, major objective commits, extraction and final resolution are Stable Save Boundary-friendly commits.

A save snapshot never contains:

- a MissionInstanceId allocated without its record;
- an external deployed slot pointing at no instance;
- both pre- and post-extraction physical ownership;
- a resolved mission with an unapplied mandatory consequence batch.

## 20. Read Models

Mission UI/presentation later consumes immutable read models containing only player-known information.

Hidden objective targets, unknown hazards, hidden reinforcement state, or undelivered event knowledge are not exposed merely because the MissionStore knows they exist.

## 21. Diagnostics

Development diagnostics expose:

- MissionId;
- MissionInstanceId;
- attempt ordinal;
- lifecycle state;
- objective revision;
- extraction state;
- current strategic/local context;
- applied resolution TransactionId;
- active-slot ownership.

## 22. Test Requirements

Tests must prove:

- only one external MissionInstance can deploy;
- DefenseEvent can coexist without taking that slot;
- failed deployment does not allocate orphan IDs;
- save/load preserves MissionId/InstanceId/attempt ordinal;
- story retry allocates a new instance without rerolling forbidden content;
- no retry refunds committed physical loss;
- final resolution is exactly once;
- scene unload does not resolve or erase a mission;
- stale runtime handles never identify mission-persistent actors.

## 23. Explicit Non-Goals

TA-9 does not use scene presence as mission truth, allow two external deployed missions, reopen resolved procedural missions under the same MissionId, reroll failed attempts through load, or treat abandonment as teleportation.

## 24. Dependencies

Depends on TA-2 identity/transactions/persistence contracts, TA-3 Scene transitions, TA-7 runtime activation, TA-8 AI/navigation, and the Design Complete GDS-8 mission lifecycle.

## 25. Open Questions

None in the MissionStore / MissionInstance baseline.