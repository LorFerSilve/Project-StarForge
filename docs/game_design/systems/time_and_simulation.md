# Time and Simulation

> **Status:** Draft  
> **Authority:** Active game time, simulation advancement, True Pause, interface-time rules, strategic timers, off-screen simulation, offline behavior, and deterministic elapsed-time handling

## 1. Purpose

Every timed system must agree on when time advances and when it does not.

## 2. Canonical Time Concepts

The design distinguishes:

- **Real Time** — wall-clock time outside the game simulation;
- **Active Game Time** — elapsed time while gameplay simulation is running;
- **Simulation Time** — authoritative time consumed by systems during Active Game Time;
- **Strategic Timer** — a persisted simulation-time countdown/ETA used by missions, travel, research, production, events, reinforcements, recovery, and markets.

## 3. No Real-World Offline Progression

Real-world time while the application is closed does not advance gameplay.

It does not:

- produce resources;
- grow crops;
- finish research;
- manufacture items/robots;
- heal crew/player;
- recharge systems;
- replenish markets;
- generate/advance raids;
- expire mission offers;
- move reinforcements;
- repair NPC targets.

## 4. True Pause

**True Pause** freezes Simulation Time.

While True Pause is active:

- actors stop;
- projectiles stop;
- hazards stop;
- production/research stop;
- travel/event timers stop;
- market/recovery timers stop.

## 5. Entering True Pause

The explicit Pause command always attempts to enter True Pause in baseline single-player gameplay except during a short non-interruptible atomic transition that must first reach its stable boundary.

If pause is requested during such a transaction, it activates immediately after that transaction commits.

## 6. Gameplay Interfaces Are Live by Default

Opening ordinary in-world interfaces does **not** pause the simulation unless the interface explicitly enters True Pause.

Live interfaces include by default:

- inventory;
- station management;
- galaxy/navigation planning;
- mission briefing while physically in world;
- robot squad management;
- trade terminal;
- research/manufacturing terminal;
- tactical station interfaces.

The player can use the explicit Pause command when they want the single-player simulation frozen.

## 7. Construction Exception

Existing GDS-2 Construction Mode explicitly freezes station simulation while editing/building according to its own authority.

That mode is therefore treated as a specialized True-Pause-equivalent design state for affected simulation.

It does not allow construction during an invalid combat state.

## 8. Authored Cinematic/Conversation Hold

An authored noninteractive sequence may explicitly request a **Cinematic/Conversation Hold** when gameplay would otherwise interrupt mandatory communication/presentation.

This hold freezes relevant Simulation Time like True Pause unless the sequence explicitly declares live action.

No dialogue is assumed to pause by default merely because text is on screen.

## 9. Active Time Source

Systems advance from authoritative Simulation Time rather than operating-system wall clock.

This prevents clock changes and save/load from creating progression.

## 10. Simulation Rates

Normal simulation rate is 1.0x.

The baseline does not require player-controlled fast-forward or slow-motion.

If added later, it requires a formal design change because it affects production, combat, hazards, events, and timing.

## 11. Frame-Rate Independence

Timed gameplay results are based on elapsed Simulation Time, not rendered frame count.

Damage-over-time, production, AI timers, market recovery, and movement-related timers must not become stronger/faster solely because FPS changes.

## 12. Fixed/Variable Technical Ticks

Implementation may use different technical tick rates for different systems.

Gameplay equivalence is mandatory: lower-detail/off-screen simulation must integrate the same authoritative elapsed time and preserve causal state.

## 13. Strategic Timers

A Strategic Timer stores at minimum:

- owning object/event ID;
- duration/remaining Simulation Time;
- state;
- completion transaction identity where needed.

It does not store only a wall-clock completion timestamp.

## 14. Pause and Strategic Timers

All normal Strategic Timers stop during True Pause and while the application is closed.

## 15. Travel

Strategic travel advances through Active Game Time according to GDS-6.

Travel does not complete while the application is closed.

## 16. Missions

Mission timing and hazards advance only while the relevant simulation is active.

True Pause freezes them.

## 17. Reinforcements

GDS-11 reinforcement ETA uses Strategic Timer semantics.

Save/load preserves the remaining ETA.

## 18. Research and Manufacturing

Research, crafting, processing, construction work, and robot manufacturing consume Active Game Time.

Being away from Horizon Station during active gameplay may allow valid station work to progress through off-screen simulation.

Closing the game stops it.

## 19. Crew Work and Training

Crew can work/train while off-screen during active gameplay if their actual assignment and systems remain valid.

No XP/training accrues offline.

## 20. Market Replenishment

Economic cycles use Active Game Time.

No vendor refresh occurs because the player quit the game for a day.

## 21. Dynamic Events

Event eligibility, expiration, scheduled arrival, and recovery grace use Active Game Time.

## 22. NPC/Raid Target Recovery

Persistent target repair/replenishment can progress during Active Game Time through abstract world/economic simulation.

It does not happen offline and must respect the target's recovery state/rules.

## 23. Horizon Station While Player Away

Horizon Station continues during active external missions:

- production;
- farming;
- repairs;
- crew tasks;
- robot tasks;
- events/defense

according to actual capability.

Off-screen resolution can be coarser but cannot invent free throughput/resources.

## 24. True Pause During Remote Station Attack

True Pause freezes both the player's current local mission and simultaneous Home Station Defense Event.

The station is not allowed to continue losing while the player is in the pause menu.

## 25. Save/Load

Saving records authoritative timer state.

Loading resumes from the recorded Simulation Time/remaining timers.

Elapsed real-world time between save and load is ignored.

## 26. Transaction Completion

When a timer reaches completion, its completion transaction commits once.

Examples:

- manufacturing output;
- research completion;
- training advancement;
- market replenishment;
- reinforcement arrival.

Save/load cannot trigger completion twice.

## 27. Simultaneous Timers

If multiple timers complete in one simulation step, deterministic event ordering resolves them.

The order must be stable for the same save state/seed.

## 28. Player Notifications

A timer/event finishing does not imply omniscient player knowledge.

Remote notification still requires the owning system's information/communication rules.

## 29. World Calendar

The setting can expose an in-world calendar/time counter based on Active Game Time.

It is presentation/world flavor unless a system explicitly uses it.

Real-world date is never gameplay authority.

## 30. No AFK Exploit Requirement

The baseline does not require anti-AFK systems.

If the simulation is actively running, systems may progress normally even if the player is standing still, subject to danger/events/resource limits.

## 31. Explicit Non-Goals

Time rules do not include:

- mobile-style offline progress;
- wall-clock vendor resets;
- real-world daily login rewards;
- FPS-dependent production/damage;
- hidden interfaces that arbitrarily pause only one subsystem;
- baseline fast-forward.

## 32. Tuneable Parameters

Durations, timer cadence, off-screen tick granularity, production/recovery time, and event intervals are tuneable.

The conditions under which Simulation Time advances are fixed.

## 33. Dependencies

This specification coordinates all timed systems and depends on Global Rules, GDS-2 Construction/Automation, GDS-3 Crew, GDS-4 Production/Research, GDS-6 Travel, GDS-8 Missions, GDS-11 Raids, Economy, Dynamic Events, and Persistence.

## 34. Open Questions

None in the time/simulation baseline.
