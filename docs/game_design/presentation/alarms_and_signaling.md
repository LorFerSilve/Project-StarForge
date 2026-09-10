# Alarms and Signaling

> **Status:** Design Complete  
> **Authority:** Presentation alarm priority, categories, propagation, acknowledgement, escalation, deduplication, remote signaling, and accessibility of urgent warnings

## 1. Purpose

Alarms communicate urgent known state without becoming constant noise. For every active alarm the player can determine, to the extent current sensors/communications permit:

- what class of incident exists;
- where it is located;
- how severe it is;
- whether it is worsening or stabilizing;
- whether a response is assigned;
- whether the underlying condition remains active.

## 2. Typed Alarm Priority Namespace

Alarm severity uses the dedicated type `AlarmPriority`:

- `AlarmPriority::P0` — Critical / immediate lethal or catastrophic risk;
- `AlarmPriority::P1` — Severe / major system or security threat;
- `AlarmPriority::P2` — Warning / degraded state requiring attention;
- `AlarmPriority::P3` — Advisory / operational information.

These identifiers are **not** the same type as `PowerLoadPriority::P0–P4`, automation report severity, mission threat, Health bands, or any other similarly named state.

Implementation, UI data, save data, and content authoring must keep the namespace/type explicit whenever ambiguity is possible.

## 3. P0 Critical

P0 is reserved for known conditions requiring immediate attention because they threaten life, campaign-critical physical survival, or imminent catastrophic loss.

Examples include:

- critically low Life-Support Reserve in an unsafe environment;
- imminent decompression affecting occupied space;
- uncontrolled reactor/catastrophic energy state;
- confirmed hostile boarding threatening Horizon critical areas;
- ship state immediately threatening occupant survival;
- an active lethal environmental threshold.

P0 presentation can pre-empt lower-priority notification audio/banner space but cannot erase the lower-priority underlying incidents.

## 4. P1 Severe

P1 represents major but not yet immediate-catastrophic state, including examples such as:

- major containable hull breach;
- critical power/cooling loss;
- spreading fire;
- defense line breach;
- ship propulsion disabled under active threat;
- known committed reinforcement force approaching.

## 5. P2 Warning

P2 represents degraded operation needing attention, e.g.:

- low ammunition;
- poor thermal margin;
- low battery reserve;
- blocked manufacturing;
- low robot energy;
- storage near capacity;
- degraded equipment.

## 6. P3 Advisory

P3 is operational information, e.g.:

- Work Order completed;
- robot returned to dock;
- known market/event update;
- Research milestone;
- routine docking status.

Advisories never use the same urgency presentation as P0/P1.

## 7. Gameplay Severity to AlarmPriority Mapping

The owning gameplay system determines its own physical/gameplay severity. Presentation maps that state to an `AlarmPriority` through authored deterministic mapping data.

`AlarmPriority` never changes the underlying gameplay state.

For example, `PlayerHealthState::Critical` and `HorizonRecoveryState::CriticalRecovery` are distinct gameplay states whose presentation can each map to an appropriate AlarmPriority according to context.

## 8. Alarm Identity

Every active alarm instance has an `Alarm ID` referencing:

- owning incident/entity/event;
- AlarmPriority;
- alarm family;
- scope/location when known;
- acknowledgement state;
- active/cleared state;
- parent incident where deduplicated;
- current response summary when known.

An Alarm ID is presentation state tied to an underlying incident; it is not a duplicate gameplay incident.

## 9. Canonical Alarm Families

- Fire / Heat;
- Atmosphere / Pressure;
- Power / Electrical;
- Reactor / Critical Energy;
- Security / Intrusion;
- Structural / Hull;
- Medical / Crew;
- Ship Navigation / Flight;
- Environmental Exposure;
- Automation / Production;
- Communication / Signal.

Each family has distinct icon/pattern/audio identity independent of color.

## 10. Alarm Scope

Scope is one of:

- Local;
- Section / Deck;
- Station / Ship Global;
- Strategic / Remote.

Scope follows the real incident and known propagation; a minor local fault does not automatically trigger a station-wide siren.

## 11. Redundant Channels

P0/P1 conditions are represented by at least two suitable channels, including one non-audio and one independently perceivable fallback. Available channels include:

- local/world indicator;
- HUD/banner;
- icon/pattern/text;
- map/overview marker;
- audio/PA;
- Closed Caption;
- optional haptics.

No critical state depends only on color, audio, haptics, flashing, or camera motion.

## 12. Color

Color reinforces but never solely defines meaning.

Red is reserved primarily for P0/P1 danger/hostility. Icons, text, patterns, placement, and state labels remain sufficient under color-vision accessibility modes.

## 13. Audio

Alarm families use distinct rhythms/timbres. Higher priority increases urgency without requiring extreme loudness.

User volume settings do not alter gameplay state or AI sensing. Muted/reduced alarm audio retains equivalent visual/text/caption paths.

## 14. Knowledge Boundary

Alarm detail contains only legitimately known diagnostic information.

When cause is known:

`Coolant Pump B failed — Reactor Loop 2 temperature rising.`

When only symptoms are known:

`Thermal fault detected — source unresolved.`

Presentation never names an undetected attacker, hidden subsystem, unknown reinforcement ETA, or inaccessible diagnostic cause.

## 15. Detection and Propagation

A gameplay incident creates/updates remote/global alarm presentation only when a valid detection/information path exists.

Horizon propagation can depend on actual:

- sensors;
- local alarm hardware;
- power/backups;
- control/data links;
- Strategic Communication Link for remote player delivery.

Local hardware may continue signaling while remote/global propagation is unavailable.

## 16. Location Signaling

When location is known, alarm presentation can identify:

- module/system;
- section/deck;
- map/overview position;
- directional guidance if Accessibility/Navigation permits and the route itself is known.

Unknown location is represented as unknown rather than fabricated.

## 17. Acknowledgement

The player can acknowledge an alarm.

Acknowledgement means only:

`The player has seen/acknowledged this presentation instance.`

It does not:

- repair the system;
- clear the incident;
- cancel automation;
- change AlarmPriority by itself;
- change gameplay severity.

Acknowledgement may suppress repeated non-escalated presentation according to tuneable repeat policy.

## 18. Clear

An alarm clears only when its owning condition is resolved or when the deterministic mapping no longer classifies that condition as alarm-worthy.

Closing a panel or acknowledging does not clear it.

Clearing can leave a lower-priority repair/inspection advisory when the physical aftermath persists.

## 19. Escalation and De-escalation

When owning state changes, the AlarmPriority mapping updates deterministically.

Example:

`Cooling Degraded → AlarmPriority::P2`
→ `Reactor Temperature Critical → P1`
→ `Containment Failure Imminent → P0`.

Stabilization can lower priority without pretending remaining damage is repaired.

## 20. Deduplication and Parent Incidents

Related alarms are grouped when their causal relationship is known.

Example parent:

`Primary Power Trunk Lost`

Children:

- Life Support B unpowered;
- Dock 2 unpowered;
- Defense Relay 4 unpowered.

Grouping reduces alarm storms but cannot hide an urgent child consequence such as an occupied compartment becoming unbreathable.

## 21. Stable Deduplication Rule

Incidents are merged only when they share an authored causal/grouping key or the owning system explicitly reports the relationship.

Presentation does not guess causality from simultaneous timestamps.

Separate incidents requiring separate action remain separately locatable.

## 22. Automation Response Presentation

Alarm UI can display the authoritative automation response state:

- Unassigned;
- Crew responding;
- Robot dispatched;
- Automated isolation active;
- Blocked;
- Escalated;
- Player intervention required;
- Completed.

These labels reference GDS-2 task state and do not create a second task machine.

## 23. Blocked Response

When known, a blocked alarm response identifies the real blocker, e.g.:

- no route;
- no power;
- no repair part;
- no qualified worker;
- robot unavailable;
- compartment unsafe;
- communication unavailable;
- protected resource authorization required.

## 24. Automation Report Severity Boundary

Station Automation's `AutomationReportSeverity` is separate from `AlarmPriority`.

Automation information can be mapped into alarms/notifications as follows:

- routine `Information` normally becomes a non-alarm notification or P3 advisory;
- `Warning` maps to P2 when the underlying state requires player awareness;
- `Critical` maps to P1 or P0 according to the actual owning incident severity;
- `DecisionRequired` maps according to urgency and can be P3/P2/P1 rather than automatically P0.

The owning incident—not the automation word `Critical`—determines final AlarmPriority.

## 25. Security

Security presentation distinguishes suspicious activity, confirmed intrusion, external attack, boarding, local hostile presence, lockdown, and recovery.

Not every contact is P0. Priority depends on actual confirmed threat/context.

## 26. Offensive Raid Signaling

Raid UI exposes only known escalation state such as:

- suspicion/alarm state;
- detected reinforcement call;
- Reinforcement Call committed;
- arrival estimate when known;
- known response posture.

Unknown enemy reaction remains hidden.

## 27. Horizon Defense Remote Signaling

When Horizon is attacked while the player is away, remote alerts require a valid Strategic Communication Link.

Delivered detail can include only currently known:

- attack detection;
- defense phase;
- severity;
- major damage;
- boarding/theft state;
- requested decision;
- outcome.

If the link is Unavailable, Horizon continues simulating without magical remote notification. Stored/delayed reports can arrive later under Strategic Communications.

## 28. Ship Warnings

Ship alarm families cover actual detected states including:

- collision/proximity;
- hull/pressure;
- reactor/power;
- thermal;
- propulsion;
- life support;
- weapon/ammunition;
- Fuel/Reaction Propellant;
- detected hostile targeting/missile;
- docking.

Fuel and Reaction Propellant are named separately when known.

## 29. Missile Warning

A missile warning exists only after valid detection/track.

It can show direction, classification, time-to-impact estimate, and countermeasure/point-defense state only to the fidelity current sensors support.

## 30. Player Survival Warnings

Player/suit warning mapping prioritizes actual known:

- Life-Support Reserve;
- Suit Energy;
- pressure seal;
- temperature;
- radiation/contaminant exposure;
- biological Health;
- Personal Shield state.

These remain distinguishable from weapon/combat feedback.

## 31. Mission Timers and Hazards

A mission countdown/hazard alarm is shown only when:

- the underlying timer/hazard exists;
- the player has legitimately detected/learned it;
- the owning Mission defines its player-facing warning state.

All gameplay countdowns use Simulation Time.

## 32. Pause

Global True Pause freezes the underlying gameplay incidents and Simulation Time.

The pause UI may show a static snapshot of already known active alarms. No new gameplay alarm state advances while paused.

## 33. Save / Load

Alarm acknowledgement/presentation state may be persisted where needed for continuity, while the underlying incident persists under its owning system.

Loading cannot clear a fire, boarding event, leak, or other persistent gameplay condition merely because presentation reconstructs.

If an incident has already cleared in the saved state, its alarm does not resurrect.

## 34. Photosensitivity

Baseline alarm lights avoid rapid high-contrast strobing.

Photosensitivity Safe Mode replaces remaining flashing/pulsing presentation with safe steady/low-frequency alternatives while preserving icon/text/audio/caption semantics.

## 35. Haptics

Haptic warnings are optional and disableable. They never carry unique critical information.

## 36. Failure / Incident Summary

After a major incident/raid, the summary can consolidate committed known outcomes including:

- root/primary cause when known;
- major damage;
- consumed resources;
- injured crew;
- destroyed robots;
- stolen/extracted cargo;
- unresolved recovery/repair tasks.

It never reports an uncommitted loss as final.

## 37. Tuneable Parameters

Tuneable parameters include:

- tones/cadence/volume;
- repeat interval;
- banner duration;
- aggregation thresholds;
- authored mapping thresholds from gameplay severity to AlarmPriority;
- visual intensity.

The four-value AlarmPriority namespace, knowledge boundary, acknowledgement semantics, and critical redundancy are fixed.

## 38. Explicit Non-Goals

The baseline has no universal red alert for minor issues, acknowledgement-as-fix, color-only/audio-only critical state, omniscient diagnostics, wall-clock alarm timers, priority namespace sharing with power loads, or presentation-only severity that changes gameplay.

## 39. Dependencies

Depends on Station Events/Automation/Security/Power, Player Health/Survival, Spacecraft, Missions, Raids, Robots, Dynamic Events, Strategic Communications, HUD, Audio, VFX, and Accessibility.

## 40. Open Questions

None.
