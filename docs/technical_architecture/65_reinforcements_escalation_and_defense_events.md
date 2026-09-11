# TA-9 — Reinforcements, Escalation, and Horizon Defense Events

> **Status:** Architecture Complete  
> **Authority:** Raid escalation state, reinforcement-call persistence, finite response forces, DefenseEvent lifecycle, active/off-screen Horizon attack execution, hostile theft/extraction, and recovery transition

## 1. Purpose

TA-9 must support two related but distinct strategic event types:

1. escalation/reinforcement inside a player offensive Raid;
2. a hostile persistent `DefenseEventId` attacking Horizon, which may execute while the player is elsewhere.

Neither system may create arbitrary waves, duplicate units/resources, or reduce station defense to one opaque score.

## 2. Raid Escalation State

Escalation is stored persistently for each applicable Raid/Defense operation using the GDS states:

```text
Quiet
Suspicious
Alerted
Mobilized
ReinforcementsEnRoute
ReinforcedFullResponse
WithdrawalEvacuation
```

Escalation is separate from:

- local AI awareness;
- station Security Alarm state;
- generic Mission lifecycle;
- Raid phase.

Committed facts can cause deterministic transitions between these systems.

## 3. Escalation Transition Rules

Each escalation trigger is typed and authored/generated with an explicit effect.

Examples:

- confirmed unauthorized docking;
- observed weapon discharge;
- breach detection;
- objective theft;
- sabotage;
- defender casualty;
- explicit alarm activation.

Perception can determine whether an event was observed; after observation, the escalation effect is not a hidden random percentage.

## 4. Reinforcement Response Record

A persistent `ReinforcementResponseRecord` exists for every committed reinforcement attempt that must survive save/load or strategic travel.

It contains conceptually:

- owning RaidId or DefenseEventId;
- source identity/type;
- finite force manifest or references to real source assets;
- communication/call endpoint;
- call state;
- call start/commit Simulation Time;
- route/source state snapshot/revisions;
- departure/arrival deadlines;
- current strategic location/route state;
- arrival/withdrawal entry plan;
- stable composition seed/version where generated;
- consumed/reserved source capacity;
- response revision.

The record can use a mission/event-local stable ResponseId when globally unique identity is unnecessary; if promoted to a globally persistent object it follows TA-2 typed PersistentId rules.

## 5. Reinforcement Call State Machine

Canonical call states are represented directly:

```text
NotRequested
Calling
Committed
Disrupted
Responding
Arrived
CancelledByValidCause
```

`Calling` is interruptible.

`Committed` means the request/order has successfully left the target and a valid response has accepted it.

## 6. Call Commit Transaction

The Call -> Committed transition is a prepared transaction that validates:

- valid world/faction response source;
- communication path and call authority;
- source still has the declared finite response capability;
- route/reach feasibility;
- no duplicate reservation/dispatch of the same source capacity.

The same commit creates/reserves the response manifest and deducts/reserves source availability as required.

This prevents one patrol from being dispatched to two raids simultaneously because two call callbacks fired in the same tick.

## 7. Post-Commit Communication Loss

After call commit, destroying/jamming the caller's communication node does not automatically unsend the force.

It can still:

- prevent further calls;
- reduce live tactical coordination;
- block updates/recall if no valid path remains.

A committed response can be cancelled only by a valid modeled external cause, such as the response source becoming unable to depart before departure or an explicit authorized recall path.

## 8. Responding / Strategic Transit

A responding reinforcement force advances through strategic route/travel abstraction using Simulation Time.

It does not exist as a hidden active local Scene while traveling.

Travel deadlines/state are persisted and stop under True Pause/application shutdown because Simulation Time stops.

## 9. Arrival

Arrival requires a valid local entry route:

- local-space approach;
- docking route;
- boarding craft;
- breach operation;
- authored insertion route.

The force is projected into TA-7/TA-8 runtime only after the local arrival transition commits.

It never spawns inside a sealed occupied room merely because ETA reached zero.

## 10. Finite Response Budget

Every target/event has finite reinforcement sources/manifests.

A full-response state cannot generate indefinite waves.

When available local reserves and committed external responses are exhausted, escalation can remain severe but cannot invent stronger tiers from nothing.

## 11. Stable Reinforcement Content

Procedural major response composition/timing derives from the stable Raid/DynamicEvent/Defense seed and declared response source.

Once committed, reload cannot reroll a weaker manifest or reset ETA.

## 12. Horizon DefenseEvent Identity

Every hostile attack against Horizon owns a persistent `DefenseEventId` and `DefenseEventRecord`.

The record contains at minimum:

- attacker faction/source;
- stable event seed/version;
- attacker objective set;
- approach route;
- finite attacker force/transport manifests;
- current defense phase;
- escalation state;
- response/reinforcement state;
- known attacker intelligence about Horizon;
- damage/casualty/recovery references;
- stolen cargo ownership references;
- withdrawal/extraction state;
- resolution TransactionId/state;
- event revision.

## 13. Defense Event Creation Boundary

`66` Dynamic Events owns eligibility and the atomic creation of DefenseEventId.

Once DefenseEventId commits, this specification owns execution.

Dynamic Events retains a relation to the DefenseEvent and does not mirror its tactical phase/state.

## 14. Horizon Defense Phases

The persistent defense phase uses the GDS structure:

```text
ApproachDetection
ExternalDefense
BoardingAttempt
InteriorDefense
AttackerObjective
AttackerWithdrawal
Recovery
Resolved
```

An attack may end before later phases if attackers are defeated/withdraw or their objective becomes impossible.

## 15. Coexistence With External Mission

A DefenseEvent is a Home Station Event, not `ActiveExternalMissionInstanceId`.

Therefore this state is legal:

```text
Player: external MissionInstanceId(17)
Horizon: active DefenseEventId(4)
```

Both advance under the same global Simulation Time according to their own scheduling and communication rules.

## 16. Player Present Mode

When Horizon is the active local Scene, the DefenseEvent projects attackers/defenders/current objectives into:

- TA-5 physics;
- TA-7 runtime entities;
- TA-8 AI/navigation;
- TA-6 station systems.

The persistent DefenseEventRecord and Horizon stores remain authority.

## 17. Player Away Mode

When the player is away, no second full Horizon Scene is created.

The DefenseEvent advances through deterministic chronological engagement/event boundaries using actual TA-6 Horizon state and TA-8 off-screen actor abstractions.

Coarse resolution can aggregate individual projectile exchanges, but every committed outcome maps to concrete state such as:

- ammo consumed;
- shield energy changed;
- module/system damage;
- robot damage/destruction;
- crew injury/incapacitation;
- attacker damage/destruction;
- breach/fire/security state;
- physical cargo transfer.

## 18. No Defense Score Authority

Derived analytics such as estimated defensive strength are allowed for diagnostics/AI heuristics.

They can never be the authoritative battle resolution variable.

Coverage gaps, power bottlenecks, firing arcs, ammunition, access topology, thermal failure, sensors, crew, robots, and attacker composition remain causally relevant.

## 19. Deterministic Coarse Combat

Given the same:

- DefenseEventRecord/seed;
- Horizon persistent state;
- attacker persistent state;
- configured policies;
- Simulation-Time commands/events;

coarse off-screen resolution must produce the same semantic outcomes independent of frame rate or thread timing.

Random tactical variation, if permitted, uses scoped event-owned TA-2 RNG streams with persisted/derivable sequence scope.

## 20. Attacker Knowledge

Attackers consume a knowledge model derived from event intelligence and live observations.

They do not read hidden Horizon storage, topology, robot positions, or utility dependencies merely because the simulation owns them.

TA-8 perception/knowledge rules remain in force when active; off-screen abstraction preserves equivalent known/unknown target data.

## 21. Boarding

A successful BoardingAttempt requires an actual route/capability such as:

- dock/spoof;
- boarding craft;
- breach;
- maintenance access.

Transitioning to InteriorDefense cannot materialize attackers inside Horizon without the corresponding access commit.

## 22. Hostile Loot Ownership

Theft is represented by physical ownership:

```text
Horizon storage
 -> hostile carrier/robot
 -> hostile boarding craft/ship
 -> hostile extraction commit
```

Cargo is recoverable while still physically present/reachable before hostile extraction.

No percentage theft is applied at phase start/end.

## 23. Hostile Extraction Commit

Permanent player loss occurs only when the hostile extraction transaction validates and commits.

That transaction changes the relevant physical owner/security/world-location state exactly once.

Destroying/stranding the hostile carrier before extraction leaves the stolen assets in their actual current owner/location for recovery.

## 24. Protected Horizon/Crew Rules

Routine procedural DefenseEvents cannot:

- permanently delete Horizon identity/Command Core;
- remove player ownership of Horizon;
- permanently kill recruited human crew;
- delete committed Research/Blueprint/Research Evidence/route knowledge.

They can cause real permitted damage, crew injury/incapacitation, resource theft, security compromise, and robot destruction.

## 25. Player Return Mid-Defense

When the player returns:

1. the DefenseEvent advances to the TA-3 transition commit boundary;
2. Horizon persistent state and event state are frozen at that boundary;
3. the active Horizon Scene is reconstructed/projected from the same stores;
4. existing attacker/defender identities/state activate through normal leases;
5. the raid continues from the current phase.

Nothing restarts or rerolls.

## 26. Defense Success / Failure

Defense success means attackers no longer maintain a valid successful raid and their mandatory objective is failed/abandoned; fleeing enemies need not all be killed.

Defense failure means the attackers achieved the event's explicit required objective and, where required, committed its extraction/consequence.

Routine failure does not equal game over.

## 27. Recovery Phase

Recovery is a persistent post-combat phase coordinating actual TA-6 tasks/state:

- contain hazards;
- restore minimum power/life support/control;
- treat/recover crew;
- recover disabled robots/cargo;
- reset security;
- repair/rebuild.

The DefenseEvent can resolve while Horizon remains in broader Critical Recovery state; pristine restoration is not required.

## 28. Recovery Grace Handoff

When an ordinary procedural DefenseEvent reaches terminal resolution, it emits exactly one typed event to the GDS-12/`66` strategic event scheduler to create/refresh Recovery Grace.

The DefenseEvent does not itself schedule the next attack.

The handoff stores the source DefenseEventId/TransactionId to prevent duplicate grace refresh on load.

## 29. Save / Load

Persist:

- DefenseEventId/seed;
- phase/escalation;
- force manifests;
- response calls/ETAs;
- attacker objective progress through authoritative domains;
- attacker/defender persistent outcomes;
- physical stolen-cargo ownership;
- recovery relation;
- applied transaction IDs.

Reload cannot restore ammo, resurrect robots, unsend calls, reroll attackers, or return extracted cargo.

## 30. Diagnostics

Debug views show independently:

- defense phase;
- local security state;
- escalation state;
- reinforcement responses;
- attacker knowledge;
- actual station system state;
- coarse engagement boundaries/results;
- physical cargo ownership;
- remote communication delivery state.

## 31. Test Requirements

Tests prove:

- finite response source cannot duplicate across calls;
- pre-commit call interruption prevents dispatch;
- post-commit communication failure does not unsend;
- ETA uses Simulation Time;
- arrival requires valid local insertion;
- no infinite reinforcement sequence;
- DefenseEvent can coexist with external MissionInstance;
- off-screen and active transitions preserve event identity/state;
- DefenseScore cannot determine authoritative outcome;
- theft remains recoverable until extraction;
- ordinary event respects Horizon/crew/knowledge protections;
- resolution refreshes Recovery Grace exactly once.

## 32. Explicit Non-Goals

No unlimited waves, arbitrary timed punishment spawns, reinforcement without source/route, one-number defense resolution, percent theft, interior teleport insertion, routine Horizon deletion, or post-load reset.

## 33. Dependencies

Depends on `64`, TA-2 transactions/identity/RNG, TA-3 Horizon transitions, TA-6 station simulation, TA-7/8 runtime AI, GDS-11 raids/defense, and `66` Dynamic Event scheduling.

## 34. Open Questions

None in the escalation/reinforcement/DefenseEvent baseline.