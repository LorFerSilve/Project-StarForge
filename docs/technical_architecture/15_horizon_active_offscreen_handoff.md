# Horizon Active/Off-Screen Handoff

> **Status:** Architecture Complete  
> **Authority:** Horizon local-scene activation/deactivation, off-screen simulation boundary, active-event handoff, crew/robot leasing, station-system continuity, and return-to-Horizon reconstruction

## 1. Purpose

Horizon must continue to exist and evolve while the player is away, but the project must not run two full local simulation worlds at once.

This document defines the exact technical handoff between:

- Horizon as the Active Local Context;
- Horizon as persistent/off-screen strategic state;
- Horizon Defense Events that continue while another player-local context is active.

## 2. Station Authority Never Moves to the Scene

The authoritative station structural/utility/inventory/production state remains in Station and related domain stores whether Horizon is on-screen or off-screen.

The active Horizon scene is a runtime projection plus leased high-frequency actor state.

## 3. Horizon Active Mode

When Horizon is the Active Local Context:

- station geometry/runtime proxies are realized from current StationStore state;
- active crew/robots receive TA-2 Activation Leases for local locomotion/combat facets;
- player local state is active;
- relevant doors, hazards, construction interactions, combat actors, and local physics exist in the SceneInstance;
- station strategic systems continue through their normal authoritative domain schedules.

## 4. No Duplicate Station Simulation

Turning Horizon into an Active Local Context does not create a second power/atmosphere/production simulation beside the persistent StationStore.

Active gameplay reads/commands the same owning station systems.

## 5. Leaving Horizon

The leave-Horizon handoff occurs at a Stable Simulation Boundary.

Canonical sequence:

1. validate departure/transition command;
2. finish current atomic transactions;
3. quiesce local Horizon scene;
4. commit local persistent consequences;
5. return crew/robot/player/ship leased facets as required;
6. deactivate local-only runtime state;
7. switch Horizon to off-screen scheduling profile;
8. commit player/ship strategic departure context;
9. destroy/evict Horizon scene resources according to cache policy.

## 6. Off-Screen Horizon State

When Horizon is off-screen:

- StationStore remains authoritative;
- resources/ownership remain authoritative;
- production/research/logistics continue through Simulation Time schedules;
- crew/robot strategic assignments continue;
- power/thermal/atmosphere/water/system state continues;
- Dynamic Events and Defense Events can occur;
- no full local render/physics/locomotion scene is required.

## 7. Off-Screen Crew and Robots

Crew/robots not participating in the player's current local context return relevant leased facets to their persistent stores before Horizon deactivation.

Their off-screen work uses strategic/task state, not hidden 60 Hz locomotion.

## 8. Simultaneous External Mission and Horizon Event

If the player is in an external mission while Horizon is attacked:

- the external mission remains the only Active Local Context;
- Horizon Defense operates through persistent station/raid/crew/robot data plus a bounded tactical working model defined later by TA-9/TA-8;
- no second player-local SceneInstance is created;
- commits still occur through the same domain stores/transactions.

## 9. Off-Screen Tactical Workspace

Later domains can construct an `OffscreenTacticalWorkspace` from immutable snapshots of:

- station topology;
- defenses/firing arcs;
- power/thermal state;
- security state;
- crew/robots;
- attacker composition;
- ammunition/resources;
- current damage.

This workspace is derived working state, not a second authoritative station.

## 10. Workspace Commit Rule

Off-screen calculations can prepare deterministic outcomes, but gameplay mutation still commits on the main/simulation thread through owning domains.

Worker/tactical calculation cannot directly write StationStore/RobotStore/CrewStore.

## 11. Event Handoff During Return

If the player returns to Horizon while a Defense Event is still active:

1. off-screen defense processing reaches a Stable Simulation Boundary;
2. all due prepared outcomes commit;
3. off-screen tactical workspace generation is frozen/invalidated;
4. current persistent Horizon/attacker state becomes the activation source;
5. Horizon SceneInstance stages from that exact committed state;
6. surviving attackers/defenders receive runtime representations;
7. the same Defense Event ID continues;
8. local combat resumes without event reset or duplicate units.

## 12. No Event Restart

Returning to Horizon does not restart:

- reinforcement timers;
- attacker objectives;
- stolen cargo state;
- damage;
- defender ammunition;
- robot losses;
- crew incapacitation;
- alarm/security state.

## 13. Leaving During an Active Defense

If gameplay rules permit the player to leave Horizon during an active Defense Event:

- local combat reaches a stable handoff boundary;
- persistent combat-relevant state commits;
- remaining event state transitions to the off-screen tactical representation;
- no actor is healed/repositioned/reset merely because detail level changes.

## 14. Structural State Projection

On return, station runtime geometry is rebuilt from the current structural graph and damage state.

A previously destroyed/broken connection remains broken.

Scene reconstruction cannot silently repair topology because the authored intact mesh exists.

## 15. Compartment/Atmosphere Projection

Current pressure-boundary and atmosphere state determines runtime compartment/environment representation.

A depressurized room returns depressurized unless the authoritative system changed while off-screen.

## 16. Door/Security Projection

Door/lockdown/access states are read from their owning systems during staging.

Returning to Horizon does not normalize every door to an authored default.

## 17. Construction State

Completed modules, work-in-progress construction, blocked plans, and damaged modules remain domain-owned and project into runtime geometry/interactions according to current state.

Incomplete construction cannot appear fully built solely because its final asset is available.

## 18. Resource/Inventory Continuity

Station inventories do not unload with the scene.

Off-screen logistics/trade/production continue to mutate the same authoritative ownership records.

When Horizon is reactivated, local container/interactable views reflect current quantities/ownership.

## 19. Docked Ships

Ships left docked at Horizon retain persistent ShipId/location/damage/cargo state off-screen.

If an off-screen attack damages or steals from valid ship-connected systems under GDS rules, the result persists into the later active scene.

## 20. Station Robots in External Squad

A RobotId physically deployed with the player cannot simultaneously be activated/assigned as a Horizon station robot.

Transport/location state determines which context owns its active lease eligibility.

## 21. Crew Presence

Crew physically traveling with the player where an authored mission supports it cannot also perform off-screen Horizon duties.

Persistent location/assignment validates this before task scheduling.

## 22. Strategic Communications

Remote information/control while Horizon is off-screen uses the GDS Strategic Communications state.

Scene absence does not grant remote omniscience.

UI read models receive only data that the communication contract allows.

## 23. Recovery Destination

If player Recovery Transit selects Horizon, the return transition stages Horizon from its current authoritative state at arrival time.

If Horizon is not currently a valid safe recovery destination, Recovery domain selects another valid destination before scene activation.

## 24. Streaming Cache Reuse

Recently evicted Horizon cells/assets can remain technical caches to reduce return loading time.

Cached residency cannot preserve stale gameplay authority.

All gameplay state is revalidated against current domain revisions before activation.

## 25. Revision Validation

Horizon staging records the Station/domain StateRevisions it used.

Before final activation, revisions are checked.

If relevant persistent state changed during asynchronous staging, affected activation data is rebuilt/refreshed before commit.

## 26. Save Capture While Horizon Is Off-Screen

Save export reads Horizon directly from its persistent stores and current off-screen event state.

No temporary scene activation is required merely to serialize Horizon.

## 27. Save Capture While Horizon Is Active

TA-2 Save DTO export merges persistent station state with active leased actor/local facets at Stable Save Boundary.

The save remains logically identical in meaning to an off-screen save.

## 28. Determinism Boundary

Changing whether Horizon is on-screen or off-screen can change presentation/detail computation but cannot change outcomes merely because:

- renderer runs;
- physics world exists;
- asset streaming timing differs.

Where an off-screen approximation replaces detailed physics/combat, its equivalence contract is owned/tested by the later subsystem architecture.

## 29. Failure Handling

If Horizon staging fails while the player is arriving:

- gameplay does not enter a half-activated station;
- session remains in transition/loading hold;
- persistent Horizon state stays intact;
- technical error is surfaced with diagnostics/retry/controlled failure policy.

## 30. Diagnostics

Tools expose:

- Horizon mode: Active/OffScreen/Transitioning;
- current station revisions;
- active crew/robot leases;
- off-screen scheduled tasks/events;
- Defense Event handoff state;
- staging revision mismatches;
- resident Horizon cells.

## 31. Test Requirements

Tests cover:

- leave Horizon returns all relevant leases;
- off-screen station systems continue without a SceneInstance;
- external mission + Horizon Defense does not create second active player scene;
- active/off-screen defense handoff preserves attacker/defender IDs, ammo, damage, timers, stolen cargo;
- returning Horizon does not restore authored default state;
- deployed RobotId cannot also be scheduled at Horizon;
- save behavior equivalent active vs off-screen for same logical state.

## 32. Explicit Non-Goals

This architecture does not require:

- two fully active local physics scenes;
- hidden 60 Hz crew walking while Horizon is off-screen;
- scene unload pausing station production;
- scene activation repairing/resetting Horizon;
- DefensePower scalar authority;
- remote control bypassing Strategic Communications.

## 33. Dependencies

Depends on TA-1 runtime scheduling, TA-2 activation/revisions/transactions, TA-3 scene/streaming, GDS Station, Crew, Robots, Raids, Communications, Recovery, and later TA-6/TA-8/TA-9.

## 34. Open Questions

None in the TA-3 Horizon active/off-screen handoff.
