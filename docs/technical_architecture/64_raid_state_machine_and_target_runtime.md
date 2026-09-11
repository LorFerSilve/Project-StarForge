# TA-9 — Raid State Machine and Persistent Target Runtime

> **Status:** Architecture Complete  
> **Authority:** Offensive raid specialization of MissionInstance, persistent raid-target state, raid phases, target-system projection, looting/sabotage consequences, retreat, and aftermath handoff

## 1. Purpose

A player offensive Raid is not a second mission engine.

TA-9 models it as a specialized MissionRecord/MissionInstance with additional `RaidRecord` and `RaidTargetState` needed for fortified multi-phase operations.

Mission lifecycle, objectives, extraction, failure, rewards, and one-external-deployment rules remain those defined by `61` and `62`.

## 2. Raid Identity

`RaidId` is a TA-2 persistent typed ID.

For a player offensive raid:

- one RaidId references one owning MissionId;
- one deployed raid attempt references one MissionInstanceId;
- RaidId stores raid-specialized strategic state that is not duplicated into the generic MissionRecord.

A RaidId is never a runtime scene handle.

## 3. Raid Target Identity

A Raid target is a persistent Strategic Location, installation, ship/base target, or authored persistent target record.

`RaidTargetState` references its true owning world/location records rather than copying all target state into the raid.

Raid-specific target metadata can include:

- known/unknown intelligence references;
- faction/security posture;
- valid external approach/access points;
- raid-relevant shield coverage;
- local power/control dependencies;
- security/door state references;
- storage/loot-owner references;
- reinforcement sources/capabilities;
- relevant interior zone definitions;
- prior committed damage/sabotage;
- target recovery state.

## 4. Persistent Consequence Principle

When a raid action legitimately damages, steals from, disables, hacks, or changes a persistent target, that owning target/world state commits **when the consequence happens**.

Raid failure later does not roll it back.

Examples:

- destroyed turret remains destroyed;
- stolen unique object remains physically transferred;
- disabled relay remains disabled until repaired/recovered;
- compromised access route remains compromised according to security rules;
- depleted storage remains depleted.

The RaidRecord may reference these consequences, but it is not their alternate owner.

## 5. Canonical Raid Phase State

Raid-specific execution uses the following phase set:

```text
Intelligence
Preparation
Approach
ExternalOperation
AccessOrBreach
InteriorOperation
ObjectiveLootSabotage
EscalationResponse
Withdrawal
Aftermath
```

These phases exist **inside** the generic Mission lifecycle.

For example, Mission state may already be Active while RaidPhase moves from ExternalOperation through InteriorOperation.

## 6. Phase Graph, Not Blind Linear Counter

Raid phases form an authored transition graph rather than a mandatory numeric sequence.

Real alternate access can skip phases.

Examples:

- valid spoofed docking authorization: Approach -> InteriorOperation;
- exterior sabotage objective: ExternalOperation -> Withdrawal;
- maintenance EVA route: Approach -> AccessOrBreach without destroying external defenses.

A phase transition therefore validates authored prerequisites and actual target state.

## 7. Intelligence Phase

Raid intelligence consumes actual player-known information from world, faction, exploration, scanning, and prior activity.

The target's complete internal state is not automatically copied into player mission knowledge.

Intelligence can reveal, at authored fidelity:

- approach lanes;
- defense presence;
- known access points;
- suspected objective location;
- communications/reinforcement capability;
- known hazards.

Unknown state remains unknown.

## 8. Preparation Phase

Preparation extends generic Mission preparation with raid-specific validation such as:

- Tactical Squad composition/Command Load;
- breach/hack/repair capability;
- cargo extraction capacity;
- docking/boarding capability;
- ship combat/readiness needs;
- optional intelligence-derived warnings.

Preparation does not reserve or create target loot.

## 9. Approach / External Operation

The raid enters the target local context through TA-3/TA-5 spacecraft/scene rules.

External defenses are projections of persistent target systems and consume actual:

- power;
- cooling;
- ammunition/energy;
- sensor knowledge;
- firing arcs;
- subsystem condition.

There is no global target Raid HP.

## 10. Access / Breach

Access is acquired through a real authored route such as:

- authorized/spoofed dock;
- forced docking;
- breach-capable hull/door point;
- maintenance route;
- EVA access;
- hacking path.

A raid phase change never teleports the player/robots past the access boundary.

Physical/security state commits through the owning systems before phase advancement.

## 11. Interior Operation

Interior zones use normal TA-5 Physics, TA-7 runtime entities, TA-8 AI/navigation, station/environment rules where applicable, and persistent target references.

The raid layer coordinates phase/objective meaning but does not replace:

- Combat;
- AI;
- doors/security;
- atmosphere;
- inventory/ownership;
- robot command behavior.

## 12. Objective / Loot / Sabotage

Raid objectives are ordinary TA-9 ObjectiveRecords with raid-specific target predicates.

Examples:

- physical resource transfer to player/ship owner;
- unique data/artifact recovery;
- target system reaches Disabled/Destroyed;
- security node Compromised;
- captive/recruited target reaches required rescue/capture state.

No raid phase itself grants objective completion.

## 13. Physical Looting

Loot uses TA-2/GDS physical ownership transfers.

A storage quantity cannot simultaneously remain owned by the target and exist as player cargo.

For player theft:

```text
Target Storage Owner
  -> Player/Robot/Transfer Owner
  -> Player Ship / extracted physical owner
```

according to actual movement/transfer rules.

Raid outcome does not apply an abstract loot percentage.

## 14. Sabotage

Sabotage commits an explicit target-system state:

- Disabled;
- Isolated;
- Compromised;
- Destroyed;
- temporary authored offline state with a Simulation-Time recovery rule.

Raid phase state does not itself disable equipment.

Defenders may later repair/counter-sabotage through real capability, resources, routes, and time.

## 15. Escalation Coupling

`RaidRecord` references the escalation/reinforcement state defined in `65`.

Escalation is orthogonal to phase.

A raid can be InteriorOperation while escalation is Alerted, Mobilized, or Reinforced.

The two state machines interact through committed facts but do not collapse into one enum.

## 16. Withdrawal

Withdrawal is a tactical/strategic raid phase, not instant extraction.

The player/robots/cargo must physically reach a valid withdrawal/extraction route.

Mandatory unresolved objectives are handled by Mission failure/abandonment semantics.

Already secured physical state follows actual ownership/security.

## 17. Extraction

The raid succeeds only when generic Mission extraction success conditions commit where extraction is required.

Raid phase can request/enter withdrawal, but extraction authority remains the GDS-8/TA-9 mission extraction transaction.

## 18. Aftermath

Aftermath prepares persistent consequences such as:

- faction/reputation response;
- target recovery scheduling;
- world-state changes;
- follow-up Dynamic Events;
- salvage state;
- story consequences.

These effects use typed prepared transactions and are committed before the raid reaches terminal Mission `Resolved` state.

## 19. Raid Runtime Projection

While active, `RaidRuntimeProjection` contains only high-frequency/bounded orchestration needed by the current SceneInstance, for example:

- current phase;
- target-system runtime references;
- active raid-zone references;
- escalation read state;
- known access points;
- active extraction routes;
- local phase triggers.

Persistent target state is not copied into mutable raid-runtime ownership.

## 20. Multi-Zone Raid Continuity

External space, EVA, boarding, interior sections, and withdrawal can be distinct local sub-zones under one MissionInstance.

Transitioning zones preserves RaidId, MissionInstanceId, objective graph, escalation state, target damage, loot ownership, reinforcement state, and committed world state.

## 21. Defender State

Defenders are finite world/raid assets.

Defeated/disabled actors do not respawn merely because a phase changes or sub-zone reloads.

If an authored system has reserves, `65` must explicitly mobilize/dispatch them from a finite source.

## 22. Target Recovery After Raid

A persistent target can later enter recovery/replenishment through Dynamic Event/world systems.

Recovery:

- uses Simulation Time;
- requires plausible faction/world support;
- cannot restore stolen unique objects as duplicates;
- cannot erase explicitly persistent story damage without authored authority;
- produces committed target state changes independent of old RaidRuntime state.

## 23. Failure / Retry

Offensive raid failure follows Mission failure and cross-domain recovery rules.

It preserves:

- target damage already committed;
- ammo/fuel/consumables spent;
- robots destroyed;
- loot physically lost/secured;
- faction/security consequences already committed.

A retry, where permitted, creates a new MissionInstance under `61`; it does not rebuild a pristine target unless explicit authored recovery changed the target between attempts.

## 24. Save/Load

Persist at least:

- RaidId;
- owning MissionId;
- current relevant MissionInstanceId;
- raid phase;
- target reference;
- phase history/revision as needed;
- escalation/reinforcement references;
- committed consequence TransactionIds where idempotency requires them.

Reload reconstructs current target state from its owners and never respawns a pristine raid copy.

## 25. Debugging

Developer tools display separately:

- generic Mission lifecycle;
- Raid phase;
- escalation state;
- objective graph;
- target persistent-state revisions;
- physical loot ownership;
- known-vs-true target intelligence;
- valid access/extraction routes.

## 26. Test Requirements

Tests prove:

- Raid remains a specialized MissionInstance;
- alternate valid access can skip a phase without teleporting;
- target damage persists after raid failure;
- phase transition alone cannot grant loot/sabotage/objective progress;
- no global Raid HP decides success;
- unloaded/reloaded sub-zones preserve defenders/loot/damage;
- withdrawal does not equal extraction;
- new attempts observe current recovered/damaged target state.

## 27. Explicit Non-Goals

No separate offensive-raid quest engine, global base HP bar, phase-based resource percentage transfer, infinite hidden defenders, pristine target reset on load, or phase transition that bypasses physical access.

## 28. Dependencies

Depends on `61`–`63`, TA-2 ownership/transactions, TA-3 local contexts, TA-5/7/8 active systems, GDS-11 Raids, and `65` reinforcement/escalation.

## 29. Open Questions

None in the offensive raid-state baseline.