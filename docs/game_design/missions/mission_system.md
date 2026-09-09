# Mission System

> **Status:** Draft  
> **Authority:** Mission identity, lifecycle, acceptance, preparation, deployment, active-instance ownership, resolution, persistence, and mission-state invariants

## 1. Purpose

The Mission System converts world opportunities into structured expeditions with explicit objectives, risk, persistence, extraction, and outcome.

## 2. Mission Identity

Every mission has a persistent **Mission ID**.

A Mission ID owns:

- mission type;
- source/sponsor;
- strategic location;
- generation seed where procedural;
- objective graph;
- known threat data;
- reward definition;
- faction/narrative context;
- current mission state.

The same Mission ID cannot exist twice in one save.

## 3. Mission Sources

A mission can originate from:

- Main Story;
- Faction Contract;
- Distress Signal;
- Exploration Discovery;
- Survivor Encounter;
- Research Lead;
- World Event;
- Station Need;
- Procedural Opportunity.

## 4. Mission Classes

Baseline classes:

- Story Mission;
- Authored Side Mission;
- Procedural Mission;
- Dynamic/Emergency Mission.

Class controls content authority, not difficulty.

## 5. Mission Lifecycle States

A mission uses the following lifecycle:

1. Discovered;
2. Available;
3. Accepted;
4. Prepared;
5. En Route;
6. Deployed;
7. Active;
8. Extraction Available;
9. Extracting;
10. Succeeded;
11. Failed;
12. Abandoned;
13. Resolved.

Not every mission must spend visible time in every transient state, but state transitions remain logically equivalent.

## 6. Discovered

The player knows that an opportunity exists but may not yet know full details.

Examples:

- unidentified distress signal;
- rumored research site;
- faction contact request.

## 7. Available

The mission can be inspected and accepted.

Known information can include:

- location;
- sponsor;
- objective summary;
- threat axes;
- known hazards;
- Reach requirement;
- hard deployment requirements;
- reward categories.

Unknown information remains explicitly unknown.

## 8. Accepted

Acceptance creates a persistent commitment record.

Accepted missions:

- remain in the mission log;
- do not disappear through hidden real-world timers;
- preserve their Mission ID;
- can be prepared or cancelled before deployment.

An accepted mission does not duplicate rewards or reserve world resources unless its definition explicitly requires reservation.

## 9. Multiple Accepted Missions

The player may have multiple Accepted missions.

The baseline does not impose a small arbitrary quest-log cap.

## 10. One Deployed Mission Instance

Only **one external Deployed Mission Instance** can be active for the player at a time.

Other Accepted missions remain pending.

This prevents overlapping ownership of:

- player location;
- mission inventory state;
- deployed ship;
- active mission zone;
- extraction transaction.

## 11. Prepared

Prepared means:

- mission selected for deployment;
- ship selected;
- route valid;
- loadout selected;
- required transport/cargo/passenger capacity validated;
- known unavoidable hard blockers resolved.

Preparation does not consume travel resources until the owning travel transaction commits.

## 12. Deployment Validation

Deployment validates at minimum:

- mission still valid;
- Strategic Location reachable;
- selected ship valid;
- required Reach available;
- required docking/landing capability available;
- mandatory cargo/passenger capacity available;
- required mission-critical equipment exists;
- unavoidable known environmental entry state is survivable.

## 13. Hard Block vs Warning

A **Hard Block** means deployment is physically/logically impossible.

Examples:

- Reach III route with Reach II drive;
- mandatory L-class docking with incompatible ship;
- mandatory rescued passenger capacity of 2 with zero passenger capacity;
- mission spawns directly into unavoidable vacuum while no pressure-rated equipment exists.

A **Warning** means the mission is possible but dangerous.

Examples:

- weak armor;
- low ammunition;
- low radiation endurance;
- severe threat rating;
- insufficient reserve for comfortable return.

Warnings can be accepted.

## 14. En Route

The mission owns a deployment intent while GDS-6 Navigation/Travel executes movement to the target.

The station continues Active-Time Simulation.

## 15. Deployed

Deployment commits the player/ship to the mission's entry context.

At deployment:

- Mission Instance ID becomes active;
- mission seed/state becomes authoritative;
- entry-zone state is created/restored;
- mission inventory security tracking begins;
- objective graph initializes/restores.

## 16. Active

The mission is currently being played.

Active state can include:

- exploration;
- combat;
- mining;
- salvage;
- rescue;
- repair;
- scanning;
- escort;
- objective interaction.

## 17. Mission Instance

A Mission Instance owns transient/persistent mission-zone state including:

- spawned entities;
- objective state;
- discovered POIs;
- harvested deposits;
- opened containers;
- rescued characters;
- local hazards;
- dropped items;
- mission-specific world changes.

## 18. No Instance Reroll

Once a procedural Mission ID/Instance is created, its seed and authored choices are stable.

Save/load cannot reroll:

- survivor identity;
- deposit placement;
- major loot;
- objective arrangement;
- hazard schedule;
- faction composition

unless a specific dynamic rule explicitly changes state through gameplay.

## 19. Active-Time Simulation

Mission time advances only while the game simulation runs.

The home station continues to simulate during missions.

No mission progresses because the application is closed.

## 20. Pause

True Pause stops mission simulation.

In-world mission interfaces do not automatically pause unless explicitly classified as Pause UI.

## 21. Extraction Available

Extraction becomes available when the mission's extraction rules permit withdrawal.

This can occur:

- immediately after deployment;
- after a primary objective;
- after reaching an extraction point;
- after restoring transport.

Availability does not equal success.

## 22. Mission Success

A mission succeeds only when its required Success Conditions are committed.

Typical requirements:

- all mandatory Primary Objectives completed;
- required protected entities/cargo secured;
- required extraction transaction completed.

A mission can explicitly be a non-extraction mission if its definition says resolution occurs in place.

## 23. Failed

Failure occurs when a mandatory condition becomes impossible or a defined failure trigger commits.

Examples:

- player ordinary defeat where no rescue window exists;
- required objective entity irrecoverably lost;
- protected rescue target fails under mission rules;
- critical time/event condition expires;
- mission-specific strategic failure.

## 24. Abandoned

Abandonment is a deliberate player decision.

Before deployment:
- Accepted mission can be cancelled cleanly according to its contract rules.

After deployment:
- Abandon marks the mission as unsuccessful;
- does not teleport the player;
- extraction/withdrawal must still occur or ordinary defeat rules apply;
- consumed supplies remain consumed.

## 25. Resolved

Resolved is the terminal bookkeeping state after:

- success rewards committed;
- failure consequences committed;
- faction/narrative changes committed;
- active instance closed or converted to persistent world state.

A Resolved mission cannot grant its one-time reward again.

## 26. Story Mission Retry

Main-story missions cannot permanently brick the campaign through routine tactical failure.

Failure creates a retry/recovery path consistent with Narrative.

## 27. Procedural Mission Replay

A resolved procedural mission is not reopened as the same Mission ID.

A later mission can reuse an archetype/location template but receives a new Mission ID and seed.

## 28. Mission Expiration

Accepted missions do not silently expire.

An Available but unaccepted mission can become unavailable only because:

- a clearly represented world-state change invalidates it;
- an authored event resolves it;
- a future GDS-12 contract-rotation rule explicitly defines visible active-time expiration.

No real-world offline expiry is allowed.

## 29. World-State Commit

Mission results can change persistent world state such as:

- restored facility;
- rescued survivor;
- route discovered;
- faction control flag;
- unique archive recovered.

The mission does not own unrelated global state.

## 30. Autosave Boundaries

Recommended safe autosave points include:

- mission acceptance;
- pre-deployment;
- deployment commit;
- major objective commit;
- extraction commit;
- final resolution.

Final save semantics remain GDS-12 authority.

## 31. Mission Log

Mission log records:

- Mission ID/title;
- class/type;
- location;
- sponsor;
- current state;
- primary/secondary objectives;
- known threat/hazards;
- requirements;
- rewards;
- relevant narrative/faction context.

## 32. Failure Readability

On failure the player is told:

- which condition failed;
- what was lost;
- what was preserved;
- faction/narrative consequences;
- retry/recovery availability.

## 33. Explicit Non-Goals

The Mission System does not provide:

- multiple simultaneous player-deployed mission instances;
- hidden real-world expiration;
- save/load rerolling;
- instant abandonment teleport;
- success merely because loot was picked up;
- duplicate one-time rewards.

## 34. Tuneable Parameters

Tuneable values include accepted-mission UI limits, preparation warning thresholds, mission duration targets, autosave cadence, and offer frequency.

## 35. Dependencies

This specification depends on Global Rules, World, Narrative, Player, Spacecraft, Resources, Crew, Extraction, Rewards, Failure, and future Combat/Robots/Raids/Persistence.

## 36. Open Questions

None in the mission-lifecycle baseline.
