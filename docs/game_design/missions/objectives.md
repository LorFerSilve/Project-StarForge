# Mission Objectives

> **Status:** Draft  
> **Authority:** Objective identity, hierarchy, dependency graph, state machine, progress, completion/failure semantics, optional objectives, and objective persistence

## 1. Purpose

Objectives translate mission intent into explicit, testable gameplay conditions.

## 2. Objective Identity

Every objective has an **Objective ID** unique within its Mission ID.

An objective defines:

- category;
- description;
- state;
- prerequisites;
- completion condition;
- failure condition;
- visibility;
- optional/mandatory status;
- relevant world targets;
- progress data.

## 3. Objective Classes

Baseline classes:

- Primary;
- Secondary;
- Optional;
- Extraction;
- Hidden/Discoverable.

## 4. Primary Objective

A mission succeeds only if all mandatory Primary Objectives required by its success graph are completed.

## 5. Secondary Objective

Secondary Objectives are mission-relevant but not required for base success unless their branch becomes mandatory through a prior choice.

They can modify:

- rewards;
- reputation;
- world state;
- extraction risk.

## 6. Optional Objective

Optional Objectives never retroactively make a successfully completed Primary mission fail.

## 7. Extraction Objective

Represents required mission withdrawal/return condition.

It is mandatory only for missions whose success requires extraction.

## 8. Hidden/Discoverable Objective

A hidden objective becomes visible only after:

- exploration;
- scanner discovery;
- dialogue;
- evidence;
- world interaction.

It cannot be required for base mission success before the player has a valid way to discover it unless it is a deliberately hidden optional objective.

## 9. Objective States

Canonical states:

- Locked;
- Available;
- Active;
- Completed;
- Failed;
- Cancelled.

## 10. Locked

Prerequisites are not yet satisfied.

A Locked objective cannot progress.

## 11. Available

The objective can be started/progressed.

## 12. Active

The player has begun the objective or the mission system has activated it.

## 13. Completed

Completion condition has committed exactly once.

## 14. Failed

Failure condition has committed and cannot be recovered inside the current branch.

## 15. Cancelled

The objective is no longer relevant because:

- another branch was chosen;
- mission was abandoned;
- world state invalidated it in an authored way.

Cancelled is not the same as Failed.

## 16. Dependency Graph

Objectives form a directed acyclic dependency graph for baseline missions.

Cycles are not allowed.

## 17. Branching Objectives

A mission can branch.

Example:

Restore Relay
- Repair Primary Power
- OR Install Emergency Bypass

Completing either valid branch can unlock the next objective if authored as OR dependency.

## 18. AND / OR Dependencies

Dependency rules support:

- AND — all prerequisites required;
- OR — any designated prerequisite branch required.

The objective definition explicitly states which.

## 19. Progress Objectives

Some objectives track quantitative progress.

Examples:

- 0/40 Titanium Ore secured;
- 2/3 sensor nodes scanned;
- 4/6 survivors located.

Progress is based on authoritative system state, not duplicated counters when avoidable.

## 20. Resource Objectives

A resource objective distinguishes:

- Acquired;
- Carried Field-Unsecured;
- Vehicle/Extraction-Secured;
- Delivered/Station-Secured.

The objective states exactly which security level is required.

## 21. Kill/Neutralize Objectives

Combat objectives must define whether completion means:

- kill;
- destroy;
- disable;
- drive off;
- capture.

"Neutralize" cannot remain ambiguous in implementation data.

GDS-9 defines combat mechanics.

## 22. Repair Objectives

A repair objective completes only when the target reaches the explicitly required operational state.

Starting a repair interaction is not enough.

## 23. Scan Objectives

A scan objective completes only when required scan data is committed.

Looking at the target is not sufficient.

## 24. Rescue Objectives

Rescue objectives distinguish:

- Located;
- Stabilized;
- Escorted;
- Boarded/Secured Passenger;
- Delivered.

The mission defines which state is required for success.

## 25. Escort Objectives

Escort progress follows the actual protected entity.

The objective does not teleport the entity to checkpoints.

## 26. Timed Objectives

Timed objectives are allowed only when time pressure creates meaningful gameplay.

A timer:

- advances only in active game time;
- pauses under true Pause;
- clearly communicates remaining time;
- defines what happens at zero.

## 27. No Hidden Mandatory Timer

A mandatory timer must be visible once it begins.

## 28. Optional Failure

Failure of an Optional Objective does not fail the mission.

It can affect rewards/world state.

## 29. Primary Failure

If a mandatory Primary Objective becomes irrecoverably Failed and no alternate branch remains, Mission state becomes Failed.

## 30. Objective Commit Atomicity

Completion/failure is committed once.

If reward/world-state actions trigger on completion, save/load cannot replay them.

## 31. Objective Target Identity

Unique objective targets use persistent IDs.

The game cannot spawn a replacement archive/component/character merely because the original was already collected.

## 32. Objective Markers

Markers can show:

- exact location;
- search area;
- direction;
- no marker.

Marker precision depends on known information.

The UI must not reveal undiscovered hidden targets omnisciently.

## 33. Search Areas

A Search Area indicates a bounded known region.

Exploration/scanning is required to find the exact target.

## 34. Objective Updates

The mission log records meaningful state transitions.

It avoids notification spam for every minor counter increment unless useful.

## 35. Multi-Zone Objectives

Objectives can span sub-zones while remaining one Mission Instance.

Progress persists across transitions.

## 36. Cooperative Future Boundary

The baseline is single-player.

Objective rules should not depend on multiple human players.

## 37. Edge Cases

If an objective target is destroyed by another systemic event before required recovery, the objective follows its explicit failure/alternate branch.

If a required cargo stack is split, authoritative secured quantity determines progress.

If the player completes an Optional Objective after Primary completion but before extraction, its reward can still commit if the mission remains Active.

If extraction commits first, unresolved optional objectives close according to their definitions.

## 38. Explicit Non-Goals

Objectives do not provide:

- cyclic dependency graphs;
- hidden mandatory timers;
- ambiguous neutralization requirements;
- duplicate counters disconnected from physical ownership;
- mandatory undiscoverable hidden goals.

## 39. Tuneable Parameters

Tuneable values include quotas, timers, marker precision, scan duration, search radius, and optional reward modifiers.

## 40. Dependencies

This specification depends on Mission System, Mission Types, Exploration, Extraction, Resources, Crew, Combat, Interaction, and Persistence.

## 41. Open Questions

None in the objective-framework baseline.
