# Alarms and Signaling

> **Status:** Draft  
> **Authority:** Priority model, alarm categories, station/ship/mission signaling, visual/audio redundancy, acknowledgement, escalation, deduplication, and accessibility of critical warnings

## 1. Purpose

Alarms must reliably communicate urgent state without becoming constant noise.

The player should be able to answer:

- what happened;
- where;
- how severe;
- whether it is worsening;
- whether action is required;
- whether the condition remains active.

## 2. Alarm Priority

Canonical priorities:

- **P0 — Critical / Immediate Lethal or Catastrophic Risk**;
- **P1 — Severe / Major System or Security Threat**;
- **P2 — Warning / Degraded State Requiring Attention**;
- **P3 — Advisory / Operational Information**.

This priority scale is presentation-only and not a replacement for gameplay severity values.

## 3. P0 Critical

Examples:

- player life-support reserve critically low in unsafe environment;
- imminent decompression in occupied compartment;
- uncontrolled reactor/catastrophic system condition;
- hostile boarding at Horizon;
- ship state immediately threatening player survival;
- active lethal environmental threshold.

P0 can interrupt lower-priority UI/audio.

## 4. P1 Severe

Examples:

- major hull breach in unoccupied/containable area;
- critical power loss;
- fire spreading;
- defense line breached;
- ship propulsion disabled under threat;
- reinforcement force committed/arriving when known.

## 5. P2 Warning

Examples:

- low ammunition;
- thermal margin poor;
- battery reserve low;
- manufacturing blocked;
- robot energy low;
- storage near capacity;
- equipment degraded.

## 6. P3 Advisory

Examples:

- Work Order complete;
- robot returned to dock;
- market/event update;
- research milestone;
- routine docking status.

Advisories must not sound like emergencies.

## 7. Alarm Channels

Critical alarms use redundant combinations of:

- local/world indicator;
- HUD/banner;
- icon/symbol;
- text/category;
- audio tone/PA;
- optional haptic;
- map/overview marker.

No critical alarm depends on one channel alone.

## 8. Alarm Identity

Different alarm families have distinct recognizable patterns.

Canonical families:

- Fire/Heat;
- Atmosphere/Pressure;
- Power/Electrical;
- Reactor/Critical Energy;
- Security/Intrusion;
- Structural/Hull;
- Medical/Crew;
- Ship Navigation/Flight;
- Environmental Exposure;
- Automation/Production;
- Communication/Signal.

## 9. Color

Alarm colors reinforce semantic severity/category but do not replace icon/text/pattern.

Red is reserved primarily for P0/P1 critical danger/hostility rather than used for every minor unavailable action.

## 10. Alarm Tone Design

Each major family has a distinct rhythm/timbre.

Priority can increase urgency without requiring extreme loudness.

No high-volume repeated siren is the default response to every warning.

## 11. Local vs Global Alarm

An incident has a scope:

- Local;
- Section/Deck;
- Station/Ship Global;
- Strategic/Remote.

A minor fault in one workshop should not trigger full-station sirens.

## 12. Station Alarm Propagation

Horizon alarm propagation depends on functioning:

- sensors/detection;
- control/data links;
- power;
- local alarm hardware where relevant.

If communications are damaged, some areas may have only local indication.

## 13. Player Knowledge

Alarm text contains only known diagnostic detail.

Example:

Known:
`Coolant Pump B failed — Reactor Loop 2 temperature rising.`

Unknown:
`Thermal fault detected — source unresolved.`

No omniscient diagnostic message leaks inaccessible information.

## 14. Location Signaling

Where location is known, alarm includes:

- module/ship system;
- deck/section;
- map/overview marker;
- directional route assistance if enabled.

## 15. Acknowledgement

The player can acknowledge an alarm to reduce repeated presentation.

Acknowledgement means:

`Player has seen the alert.`

It does **not** mean:

- problem solved;
- automation disabled;
- alarm state cleared.

## 16. Alarm Clear

An alarm clears only when its owning gameplay condition resolves or transitions to a lower severity.

Closing the UI does not clear the underlying incident.

## 17. Escalation

If an incident worsens, its alarm can transition upward in priority.

Example:

`Cooling Degraded (P2)`
→ `Reactor Temperature Critical (P1)`
→ `Containment Failure Imminent (P0)`

Transitions follow actual gameplay state.

## 18. De-escalation

When emergency response stabilizes an incident, signaling can reduce priority while leaving a residual repair advisory.

Example:

`Hull Breach — P0`
→ emergency bulkhead seals
→ `Compartment Isolated / Repair Required — P2`.

## 19. Deduplication

Multiple instances of the same low-priority event aggregate.

Critical events remain individually locatable if separate response is required.

## 20. Alarm Storm Control

During cascading failures the system groups related alerts under a parent incident when causal relationship is known.

Example:

`Primary Power Trunk Lost`
with child consequences:
- Life Support B unpowered;
- Dock 2 unpowered;
- Defense Relay 4 unpowered.

The player can inspect details without hearing three identical sirens.

## 21. Causal Priority

When known, UI emphasizes root cause and critical consequences.

It must not hide urgent downstream danger merely because a root cause exists.

## 22. Automation Response

Alarm presentation can show current response state:

- Unassigned;
- Crew responding;
- Robot dispatched;
- Automated isolation active;
- Blocked;
- Player intervention required.

This is informational and follows GDS-2 automation task state.

## 23. Blocked Response

If automated response is blocked, alarm identifies known reason:

- no route;
- no power;
- no repair part;
- no qualified worker;
- robot unavailable;
- compartment unsafe.

## 24. Station Security Alarms

Security states distinguish:

- suspicious activity;
- confirmed intrusion;
- external attack;
- boarding;
- local hostile presence;
- lockdown.

Not every enemy detection triggers the highest station-wide alarm.

## 25. Raid Escalation Signaling

Offensive raid presentation exposes only known escalation information.

Examples:

- target suspicious;
- alarm raised;
- reinforcement call detected;
- reinforcements committed;
- arrival estimate when known.

Unknown enemy response remains unknown.

## 26. Horizon Defense Signaling

When Horizon is attacked while the player is away, remote notification can communicate:

- attack detected;
- current defense phase;
- known severity;
- critical damage;
- boarding/theft state;
- outcome.

Remote detail depends on functioning communications/sensors.

## 27. Ship Alarms

Ship alarm categories include:

- collision/proximity;
- hull/pressure;
- reactor/power;
- thermal;
- propulsion;
- life support;
- weapon/ammo;
- fuel/propellant;
- missile/hostile targeting where detected;
- docking.

## 28. Missile Warning

A missile warning requires valid detection/track.

It can indicate:

- incoming threat;
- direction;
- time-to-impact estimate when sensor solution supports it;
- countermeasure/point-defense state.

## 29. Fuel/Propellant Warning

Warnings distinguish Fuel from Propellant rather than using generic `Low Fuel` when the specific resource is known.

## 30. Player Survival Alarms

Suit warnings prioritize:

- Life-Support Reserve;
- Suit Energy;
- pressure seal;
- temperature;
- radiation/toxic exposure;
- critical Health/Shield state.

Warnings must remain distinguishable in combat.

## 31. Mission Alarms

Mission-specific timed/escalating hazards can use alarms only when the player has an in-world/system source for the warning.

The UI should not announce an unknown reactor countdown the player never detected.

## 32. Alarm Timers

If an actual countdown exists and is known, display its timebase clearly.

All gameplay countdowns use Simulation Time/mission state, not wall-clock real-world time.

## 33. Save/Load

Active alarm state persists with its owning gameplay incident.

Loading cannot clear a persistent fire/boarding/failure condition simply because the alarm UI restarted.

## 34. Pause Behavior

When true pause is active, alarm simulation pauses with gameplay.

The pause menu may preserve a static visible summary of critical active alerts.

## 35. Accessibility — Visual

Alarms support:

- non-color icons;
- text labels;
- scalable warning size;
- optional stronger contrast;
- reduced flashing;
- directional indicators.

## 36. Accessibility — Audio

Players can adjust alarm volume separately enough that alarms remain usable without overwhelming other sound.

Closed captions identify critical alarms by category/location where known.

## 37. Accessibility — Haptics

Haptic alarm pulses are optional and disableable.

They cannot be the sole warning channel.

## 38. Photosensitivity

Alarm lights avoid rapid high-contrast strobing.

Photosensitivity Safe mode replaces flashing with steady/pulsing low-frequency alternatives plus icon/text/audio.

## 39. Notification vs Alarm

Routine completion messages are notifications, not alarms.

The design must avoid alert fatigue by reserving alarm language for states requiring awareness/action.

## 40. Failure Summary

After an emergency/raid resolves, a summary can consolidate:

- cause;
- major damage;
- consumed resources;
- injured crew;
- destroyed robots;
- stolen cargo;
- unresolved repair tasks.

## 41. Explicit Non-Goals

No universal red alert for minor issues, no acknowledgement-as-fix behavior, no audio-only alarms, no full-screen flashing requirement, no omniscient diagnostics, and no real-world timer alarms.

## 42. Tuneable Parameters

Tone, cadence, volume, repeat interval, banner duration, aggregation thresholds, priority escalation thresholds, and visual intensity are tuneable.

## 43. Dependencies

References Station Events/Automation/Security, Player Survival, Spacecraft, Missions, Raids, Robots, Dynamic Events, HUD, Audio, VFX, and Accessibility.

## 44. Open Questions

None in the alarms/signaling baseline.
