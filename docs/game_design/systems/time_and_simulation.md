# Time and Simulation

> **Status:** Design Complete  
> **Authority:** Real Time, Active Game Time, Simulation Time, True Pause, interface-time rules, Strategic Timers, off-screen simulation, offline behavior, and deterministic elapsed-time handling

## 1. Purpose

Every timed system must agree on exactly which clock advances it and when that clock stops.

## 2. Canonical Time Concepts

The design distinguishes three clocks/concepts:

- **Real Time** — operating-system/wall-clock time outside gameplay authority;
- **Active Game Time** — cumulative unpaused playable time used primarily for player-facing playtime/UX statistics;
- **Simulation Time** — the authoritative gameplay clock consumed by all world, actor, production, travel, event, hazard, recovery, and economy systems.

In the baseline, normal Simulation Time advances at **1.0x** while gameplay is unpaused. Therefore Active Game Time and Simulation Time normally accumulate at the same rate, but **only Simulation Time is authoritative for gameplay outcomes**.

## 3. Real Time Is Never Gameplay Authority

Changing the system clock, leaving the game closed, or waiting outside the application cannot:

- produce resources;
- grow crops;
- finish Research;
- manufacture items/robots;
- heal crew/player;
- recharge systems;
- replenish markets;
- generate/advance raids;
- expire mission/event offers;
- move reinforcements;
- repair NPC targets;
- complete travel.

Real-world timestamps may be stored as informational save metadata only.

## 4. Simulation Time

Simulation Time owns every gameplay-relevant duration or ETA.

Examples:

- production;
- Research;
- construction work;
- farming;
- healing/recovery;
- strategic travel;
- mission timers;
- status-effect duration;
- reinforcement arrival;
- market replenishment;
- Dynamic Event expiration/cooldown;
- Recovery Grace;
- raid-target recovery;
- crew work/training;
- robot work/charging.

A subsystem may expose a friendly duration label, but the underlying authoritative timer uses Simulation Time.

## 5. Active Game Time

Active Game Time is a cumulative player-facing measure of unpaused gameplay participation.

It may be used for:

- save metadata;
- statistics;
- periodic autosave cadence;
- non-gameplay UX analytics.

It must not independently decide a gameplay state transition.

## 6. True Pause

**True Pause** freezes Simulation Time and Active Game Time.

While True Pause is active:

- actors stop;
- AI stops;
- projectiles stop;
- hazards stop;
- station/ship simulation stops;
- production/Research stop;
- travel stops;
- market/event/recovery timers stop;
- Horizon Defense Events stop;
- reinforcement timers stop.

## 7. Entering True Pause

The explicit Pause command always attempts to enter True Pause in baseline single-player gameplay except during a short non-interruptible atomic transition that must first reach its Stable Save Boundary.

If Pause is requested during such a transition:

1. the request is retained;
2. the atomic transaction completes;
3. True Pause activates before the next ordinary Simulation Time step.

## 8. Gameplay Interfaces Are Live by Default

Opening an ordinary gameplay interface does **not** pause Simulation Time unless that interface explicitly invokes True Pause.

Live by default:

- inventory;
- station management;
- galaxy/navigation planning;
- mission briefing while physically in world;
- robot squad management;
- trade terminal;
- Research/manufacturing terminal;
- tactical station interfaces.

The UI must visibly distinguish **Simulation Running** from **Paused** where ambiguity is possible.

## 9. No Partial-Pause Gameplay Interface

A normal baseline interface may not freeze only one gameplay subsystem while unrelated Simulation Time continues merely for convenience.

If an accessibility or planning option pauses gameplay, it invokes global True Pause.

This prevents hidden exploits such as freezing a local hazard while station production or a remote attack continues.

## 10. Construction Mode

GDS-2 Construction Mode is a specialized global True-Pause state while open.

It freezes the complete gameplay simulation, not only station machinery.

Construction Mode still cannot be entered during active station combat under its owning rules.

## 11. Cinematic / Conversation Hold

An authored noninteractive sequence may invoke a **Cinematic/Conversation Hold**.

When invoked, it freezes Simulation Time equivalently to True Pause unless the authored sequence explicitly declares itself live gameplay.

Ordinary radio chatter never pauses merely because dialogue is playing.

## 12. Simulation Rate

Baseline gameplay Simulation Rate is 1.0x.

Player-controlled fast-forward, slow-motion, or selective time acceleration is not part of the baseline.

Adding one later requires a formal design change because it changes the interaction of combat, station simulation, travel, hazards, events, and recovery.

## 13. Frame-Rate Independence

Gameplay results use elapsed Simulation Time, not rendered frame count.

Changing FPS cannot alter the real gameplay rate of:

- damage over time;
- production;
- AI reaction timers;
- market cycles;
- travel;
- status duration;
- cooling/heating;
- resource consumption.

## 14. Technical Tick Rates

Implementation may use different fixed/variable/coarse technical ticks for different systems.

Every implementation must integrate the same authoritative Simulation Time and produce gameplay-equivalent causal results.

## 15. Strategic Timer

A **Strategic Timer** stores at minimum:

- owning entity/event/transaction ID;
- remaining Simulation Time or Simulation-Time deadline;
- current state;
- completion transaction identity where required.

It never relies solely on a future wall-clock timestamp.

## 16. Timer Pause Rule

All Strategic Timers stop during:

- True Pause;
- Construction Mode;
- Cinematic/Conversation Hold when that hold pauses gameplay;
- application shutdown.

## 17. Strategic Travel

GDS-6 strategic travel consumes Simulation Time.

While In Transit:

- Horizon continues simulating;
- Dynamic Events can progress/generate if eligible;
- player ship systems/events can progress;
- the application being closed does not advance travel.

## 18. Missions

Mission timers, hazards, objectives with durations, and local encounter timing consume Simulation Time.

True Pause freezes them.

## 19. Reinforcements

GDS-11 reinforcement ETA uses Strategic Timer semantics and therefore Simulation Time.

Destroying communications after a Reinforcement Call has committed does not rewind elapsed or committed state unless the raid rule provides a valid recall/cancellation path.

## 20. Research, Manufacturing and Construction Work

Research, crafting, processing, construction work, and robot manufacturing consume Simulation Time.

They may progress while the player is away from Horizon during active unpaused gameplay if all physical/automation requirements remain valid.

## 21. Crew Work and Training

Crew work, training, treatment, and recovery consume Simulation Time.

No work/training/healing accrues during True Pause or while the application is closed.

## 22. Markets

Market replenishment, liquidity recovery, convoy-linked stock changes, and authored economic timers consume Simulation Time.

There are no wall-clock vendor resets.

## 23. Dynamic Events

Dynamic Event:

- eligibility scheduling;
- offer expiration;
- scheduled arrival;
- cooldown;
- Recovery Grace;
- target recovery/replenishment

all consume Simulation Time.

## 24. Horizon Station While Player Is Away

During an active external mission/travel state, Horizon continues through the same Simulation Time for:

- power/thermal/atmosphere;
- production;
- farming;
- repairs;
- crew tasks;
- robot tasks;
- Dynamic Events;
- Defense Events.

Off-screen technical resolution may be coarser but cannot invent free throughput/resources or different elapsed time.

## 25. Remote Station Attack + True Pause

True Pause freezes both:

- the player's current local mission/travel context; and
- a simultaneous Horizon Defense Event.

The player is never losing station assets while reading the true pause menu.

## 26. Recovery Transit

Any GDS-14-defined Recovery Transit consumes Simulation Time.

During Recovery Transit, other valid world systems continue unless True Pause is active.

Recovery therefore cannot function as zero-time strategic teleportation.

## 27. Save/Load

Saving records authoritative Simulation Time and every Strategic Timer's state.

Loading resumes exactly from that recorded state.

Elapsed real-world time between save and load is ignored.

## 28. Timer Completion

When a timer reaches completion, its completion transaction commits exactly once.

Examples:

- Work Order output;
- Research completion;
- training advancement;
- market replenishment;
- reinforcement arrival;
- Dynamic Event transition.

## 29. Simultaneous Timer Completion

If several timers complete in one Simulation Time step, deterministic stable event ordering resolves their transactions.

The result cannot depend on container/hash iteration order, render frame rate, or reload timing.

## 30. Information Boundary

A timer completing does not automatically grant the player knowledge of it.

Notification still requires the owning system's valid sensor/communication/information path.

## 31. World Calendar

An in-world calendar/time counter can be derived from Simulation Time for setting/presentation.

It does not use Real Time as gameplay authority.

## 32. Autosave Cadence

Periodic autosave may use Active Game Time because it is a user-safety cadence rather than a world-state mechanic.

The current first-pass target remains every 10 minutes of Active Game Time when a Stable Save Boundary exists.

## 33. AFK Behavior

No anti-AFK system is required.

If gameplay is unpaused and Simulation Time is advancing, all relevant systems continue even when the player provides no input.

## 34. Explicit Non-Goals

The baseline contains no:

- offline progression;
- wall-clock gameplay timer;
- real-world daily reset;
- FPS-dependent gameplay rate;
- hidden partial-pause interface;
- baseline fast-forward/slow-motion system.

## 35. Tuneable Parameters

Durations, timer cadence, off-screen tick granularity, production/recovery time, and event intervals are tuneable.

Clock ownership and pause semantics are fixed rules.

## 36. Dependencies

This specification coordinates every timed gameplay domain and depends on Global Rules, Construction, Crew, Resources/Research, Spacecraft Travel, Missions, Raids, Economy, Dynamic Events, Persistence, Recovery, and Presentation.

## 37. Open Questions

None after GDS-14 time-authority reconciliation.
