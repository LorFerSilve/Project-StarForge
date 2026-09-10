# Save and Persistence

> **Status:** Design Complete  
> **Authority:** Save types, Stable Save Boundaries, serialization scope, autosave/quicksave/manual save policy, atomic transactions, deterministic state, loading, migration, and anti-duplication requirements

## 1. Purpose

Saving must preserve the actual systemic state of Project StarForge without forcing arbitrary no-save zones or enabling state duplication through incomplete transactions.

## 2. Baseline Save Modes

The baseline supports:

- Manual Save;
- Quick Save;
- Autosave.

There is no baseline Ironman/single-save restriction.

## 3. Manual Save

The player may request a Manual Save during normal gameplay whenever the game can reach a Stable Save Boundary.

Manual saves are limited by platform/storage capacity rather than an authored small gameplay slot count.

## 4. Quick Save

Quick Save uses the same state rules as Manual Save.

It does not use a less complete serialization model.

The newest Quick Save may replace the previous Quick Save according to UI/platform implementation.

## 5. Autosave

Autosaves occur at important stable milestones and through periodic protection.

Baseline triggers:

- before external mission/raid deployment;
- after mission/raid resolution reaches a stable post-transaction state;
- after strategic travel arrival;
- after major story/faction decision commit;
- after significant Horizon Station Defense resolution/recovery transition;
- periodic autosave every **10 minutes of Active Game Time** when a Stable Save Boundary exists.

## 6. Rolling Autosaves

The baseline retains the latest **10 autosaves**.

This is a user-safety history, not a gameplay resource.

## 7. Stable Save Boundary

A Stable Save Boundary is a simulation state where all atomic gameplay transactions started for the current update have either:

- committed; or
- not begun.

No physical ownership quantity, objective result, or unique identity is half-transferred.

## 8. Saving During Combat

Saving during combat is allowed.

Combat is not itself a reason to forbid saving.

The save captures actual:

- player/enemy Health;
- shields;
- ammo;
- weapon state;
- projectiles/statuses where required;
- AI awareness;
- Last Known Position;
- robot state;
- mission state.

## 9. Saving During Raids

Saving during an offensive raid or Horizon defense is allowed at Stable Save Boundaries.

The save preserves:

- Raid/Defense Event ID;
- target damage;
- breaches;
- stolen/secured cargo;
- reinforcement calls/ETA;
- escalation;
- robot/ship/crew state.

## 10. Temporarily Non-Savable Atomic Transitions

A save snapshot is not taken in the middle of an indivisible transaction such as:

- extraction commit;
- mission failure resolution;
- raid/defense final resolution commit;
- physical inventory ownership transfer;
- manufacturing commissioning/output transfer;
- construction placement/demolition commit;
- unique Blueprint/Research reward commit;
- irreversible story-choice commit;
- player recovery relocation transaction;
- strategic scene/instance ownership transition;
- save/load operation itself.

## 11. Queued Save Request

If the player requests a save during one of these short atomic transitions:

- the request is queued;
- the transaction finishes;
- save occurs at the next Stable Save Boundary.

The request is not silently discarded.

## 12. No Arbitrary Safe-Room Requirement

The player does not need to return to Horizon Station, a bed, checkpoint, or safe room to make a Manual/Quick Save.

## 13. Save Snapshot Scope

A complete save contains or references authoritative persistent state for:

- player;
- Horizon Station;
- crew;
- resources/inventories;
- research/Blueprints/knowledge;
- spacecraft;
- robots;
- factions/reputation;
- galaxy/routes;
- missions;
- raids/Defense Events;
- dynamic events;
- markets/Credits;
- progression phase/gates;
- difficulty/settings relevant to gameplay;
- simulation timers;
- procedural seeds and deterministic random-stream state where required.

## 14. Physical Ownership Integrity

For every physical item/resource, save state records one authoritative owner/location.

A load cannot reconstruct the same quantity at both source and destination.

## 15. Unique Identity Integrity

Persistent entities with IDs remain unique, including:

- Crew ID;
- Robot ID;
- Ship ID;
- Mission ID;
- Raid/Defense Event ID;
- Dynamic Event ID;
- Unique Item ID;
- Reward Transaction ID.

## 16. Procedural Seed Persistence

Generated missions/events/raid targets store stable seeds.

Loading does not re-seed them from wall clock or frame timing.

## 17. Random Stream State

Where a simulation contains future deterministic random outcomes already tied to a persistent event, sufficient random-stream/seed state is saved to prevent load-based rerolling.

## 18. Loading

Loading replaces current session state with the selected committed snapshot.

It does not merge the old current session inventory/economy with the loaded state.

## 19. Real-World Time on Load

Elapsed wall-clock time between save and load is ignored for gameplay progression.

All Strategic Timers resume from saved Simulation Time/remaining duration.

## 20. Autosave Before Risk

The pre-deployment autosave exists so ordinary mission/raid failure never forces the player to lose unrelated prior station management because of a later tactical mistake.

It does not change the rules of failure inside the newer save state.

## 21. User Choice and Save-Scumming

The baseline intentionally permits the player to select an older manual/autosave.

Deterministic seeds prevent repeated loading of the same snapshot from rerolling major procedural content.

This is a single-player design choice, not an exploit that requires Ironman restrictions.

## 22. Irreversible Story Choice

A final/major choice commits atomically in the current save timeline.

The player can only choose differently by deliberately loading a snapshot from before that transaction.

The post-choice state cannot re-open the choice as if it had not occurred.

## 23. Mission Completion

A completed unique story Mission ID remains completed in all descendant saves.

Loading an older save legitimately restores the older timeline before completion.

## 24. Reward Atomicity

One-time reward transaction state is saved with the reward recipient state.

A crash/reload cannot produce:

- reward committed without flag; or
- flag committed without reward

because both belong to the same atomic transaction.

## 25. Market Transactions

Physical trade and Credit debit/credit commit in one transaction.

Save state cannot contain paid Credits with untransferred goods or transferred goods without payment unless that transaction type explicitly represents debt/claim.

## 26. Manufacturing/Research

Work-in-progress persists with:

- inputs/reservations;
- progress;
- output state;
- completion transaction.

Loading does not refund inputs or duplicate completed output.

## 27. Timed Systems

All timers store Simulation Time state, not only a real-world completion timestamp.

## 28. Off-Screen Horizon Simulation

If an external mission and Horizon event coexist, the same save snapshot records both.

Loading restores the exact simultaneous state rather than recomputing the station attack from scratch.

## 29. Player Incapacitation

A save may capture an Incapacitated state if it is stable and the recovery/failure transaction has not yet begun.

Once failure/recovery commit begins, saving waits for its Stable Save Boundary.

## 30. Crash Safety

Autosave/manual writes should be implemented as write-new-then-commit/replace rather than overwriting the only valid copy in place.

A failed write must not corrupt the last known valid save.

This is an implementation requirement derived from persistence authority.

## 31. Save Version

Every save includes a schema/version identifier.

## 32. Migration

Later game versions can migrate older saves through explicit deterministic migration logic.

A migration must preserve legitimate:

- ownership;
- unique IDs;
- completed progression;
- knowledge;
- long-term assets

as closely as possible.

## 33. Invalid/Corrupt Save

A save that fails integrity/version validation is not partially loaded into gameplay.

The player can select another valid save/autosave.

## 34. Save Metadata

Presentation can show metadata such as:

- active playtime;
- current location;
- story act/progression phase;
- save type;
- real-world creation timestamp.

Real-world timestamp is informational only.

## 35. Difficulty Persistence

Selected Difficulty and Custom axes persist with the save/profile state as appropriate.

Changing it updates future simulation only.

## 36. Settings Boundary

Pure presentation/input settings can be profile-global rather than save-specific.

Gameplay-state settings that change simulation must be stored with sufficient save context.

## 37. No Cloud Requirement

Cloud synchronization is not required for core gameplay.

Local saves must provide the complete baseline experience.

## 38. No Multiplayer Authority

Because baseline is single-player, saves do not require server-authoritative progression or anti-cheat signatures for core gameplay.

## 39. Persistence Tiers

The design conceptually distinguishes:

- **Permanent Knowledge/Identity:** committed Blueprints, Research, discovered routes, crew/ship identities;
- **Persistent Physical State:** inventories, station/ship/robot condition, market stock, target damage;
- **Persistent Event State:** missions, raids, timers, dynamic events;
- **Ephemeral Presentation State:** transient camera/UI animation not required for gameplay reconstruction.

## 40. Serialization Completeness Test

A gameplay state is save-ready only if loading it can determine all consequential next states without inventing:

- who owns an item;
- whether a reward already fired;
- whether a robot is destroyed;
- whether a call was sent;
- how long a timer remains;
- which procedural seed applies.

## 41. Explicit Non-Goals

Persistence does not require:

- checkpoint-only saving;
- no-save combat;
- Ironman;
- one save slot;
- wall-clock progression;
- reload-based procedural rerolls;
- server authority for single-player.

## 42. Tuneable Parameters

Periodic autosave interval and rolling autosave count are tuneable UX/safety values; baseline targets are 10 active minutes and 10 rolling slots.

The Stable Save Boundary and atomicity rules are fixed.

## 43. Dependencies

This specification coordinates all persistent domains and depends on Time/Simulation, GDS-4 Ownership, GDS-8 Mission transactions, GDS-11 raids, Economy, Dynamic Events, and Failure/Recovery.

## 44. Open Questions

None in the save/persistence baseline.
