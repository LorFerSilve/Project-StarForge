# System Sabotage

> **Status:** Draft  
> **Authority:** Raid sabotage target classes, sabotage methods, persistent/temporary disablement, causal dependency effects, overload/destruction boundaries, repair/recovery, and anti-global-switch rules

## 1. Purpose

Sabotage lets the player defeat a stronger fortified target by attacking dependencies instead of destroying every defense directly.

## 2. Sabotage Principle

A sabotage action affects only systems that are physically/logically connected to the compromised target.

The game does not use generic:

- "disable all defenses";
- "turn off whole base";
- "hack station"

buttons unless an authored target truly owns that scope.

## 3. Canonical Sabotage Target Classes

Baseline:

1. Power Distribution;
2. Shield Support;
3. Sensors / Targeting;
4. Communications / Reinforcement Link;
5. Security Control;
6. Weapon Control / Ammunition Supply;
7. Atmosphere / Life Support;
8. Thermal / Cooling;
9. Logistics / Cargo Routing;
10. Docking / Hangar Control;
11. Mission-Specific Research/Industrial System;
12. Reactor/Primary Generator Controls.

## 4. Sabotage Methods

A target can support one or more:

- Physical Destruction;
- Physical Disablement;
- Hacking / Control Override;
- Power Isolation;
- EMP / Electronic Disruption;
- Controlled Demolition;
- Overload / Unsafe Operation;
- Removal/Theft of Critical Component;
- Mechanical Lockout.

The target definition states valid methods.

## 5. Sabotage States

A raid-relevant subsystem can be:

- Normal;
- Compromised;
- Disabled Temporary;
- Disabled Persistent;
- Destroyed;
- Recovering/Repairing.

Not every subsystem needs all states.

## 6. Compromised

The attacker has gained some control/access without fully disabling the target.

Examples:

- security credential injected;
- targeting data corrupted;
- door network access obtained.

## 7. Disabled Temporary

Function is unavailable for a bounded active-time/status condition.

Examples:

- EMP disruption;
- temporary software lockout;
- shield power interruption while a breaker is held open.

## 8. Disabled Persistent

Function remains unavailable until an explicit repair/reset/reconfiguration occurs.

Examples:

- control fuse removed;
- relay physically cut;
- actuator jammed;
- critical cable severed.

## 9. Destroyed

The subsystem's physical hardware reaches its destruction state.

Repair can require replacement/rebuild rather than simple reset.

## 10. Power Distribution Sabotage

A power node can affect only its connected loads.

Possible consequences:

- local turrets offline;
- shield emitter offline;
- doors/lifts affected;
- sensors offline;
- lighting lost;
- environmental support degraded.

Redundant routes can preserve service.

## 11. Power Isolation Is Not Destruction

Opening/cutting a breaker or relay can disable downstream systems without damaging them.

Defenders may restore the path if:

- control/access exists;
- hardware remains functional;
- no attacker holds the relevant node.

## 12. Shield Sabotage

Possible targets:

- shield emitter;
- local controller;
- power feed;
- cooling support.

Result applies only to actual shield coverage dependent on that target.

## 13. Sensor Sabotage

Can reduce:

- tracking;
- weapon engagement quality;
- shared defender awareness;
- missile lock;
- target identification.

Local unaffected sensors remain functional.

## 14. Communications Sabotage

Can:

- block/delay new reinforcement calls;
- isolate sectors;
- reduce shared information;
- interrupt external coordination.

A reinforcement already dispatched stays dispatched unless its own travel/control rules cause cancellation.

## 15. Security-Control Sabotage

Can affect only the compromised security segment.

Possible results:

- selected doors unlock;
- cameras disabled;
- local alarm suppressed;
- security robot authorization changed;
- one turret-control segment interrupted.

Network segmentation limits scope.

## 16. Weapon-Control Sabotage

Possible effects:

- disable one battery/cluster;
- remove targeting authorization;
- jam feed mechanism;
- cut power;
- corrupt local firing solution.

It does not delete ammunition from storage unless that storage is physically destroyed/removed.

## 17. Ammunition Supply Sabotage

The attacker can:

- isolate magazine logistics;
- destroy local magazine;
- steal ammunition;
- jam loader/feed.

Already loaded rounds remain unless directly affected.

## 18. Atmosphere / Life-Support Sabotage

Possible actions:

- shut local circulation;
- vent selected compartment;
- disable scrubber;
- release contamination if target contains it.

These actions require real system access and can endanger:

- player;
- robots;
- defenders;
- loot/objectives.

## 19. No Generic Vent-All Button

One console cannot depressurize every station compartment unless that authored control system genuinely has that authority and intact actuators/path.

Safety/segmentation can limit remote control.

## 20. Thermal / Cooling Sabotage

Disabling cooling can force:

- weapon throttling;
- shield limitation;
- reactor/system shutdown;
- thermal damage

after actual heat accumulation.

Cooling sabotage does not cause instant explosion without thermal cause.

## 21. Logistics Sabotage

Can block:

- ammunition delivery;
- repair parts;
- cargo movement;
- production input/output.

Physical inventory remains where it currently exists.

## 22. Docking / Hangar Sabotage

Can:

- prevent defender launches;
- trap/disable docking clamps;
- open/close valid routes;
- delay reinforcement docking;
- create player extraction risk.

The player can accidentally sabotage their own planned exit route.

## 23. Research/Industrial Sabotage

Mission-specific systems can be:

- disabled;
- destroyed;
- data-wiped only where actual data-state exists;
- contaminated;
- physically stripped.

Objective must define required outcome exactly.

## 24. Reactor / Primary Generator Sabotage

Reactor sabotage is high-risk and cannot use a generic "shoot reactor = station explodes" rule.

Valid outcomes may include:

- controlled shutdown;
- power trip;
- coolant disruption;
- output overload;
- physical reactor damage.

## 25. Reactor Safeguards

A functioning reactor can have:

- automatic shutdown;
- breaker isolation;
- containment;
- cooling protections.

Overriding safeguards requires explicit capability/actions.

## 26. Reactor Catastrophe Boundary

Catastrophic reactor failure occurs only if the reactor's authored system state reaches such a condition.

It is not a universal raid shortcut.

Possible consequences must be explicitly defined:

- radiation;
- fire;
- local structural damage;
- power loss;
- evacuation;
- mission failure.

A normal reactor hit does not automatically vaporize the whole station.

## 27. Hacking

Hacking follows GDS-2/GDS-10 boundaries.

A Hacker Unit/player can alter only target capabilities exposed by a valid interface/path.

## 28. Hacking Duration

Hacking can require sustained interaction.

Progress behavior on interruption is target-specific but must be explicit:

- resets;
- checkpoints;
- persists.

No implementation may guess.

## 29. Hacking Detection

A hack can be:

- silent until detected;
- immediately logged;
- alarm-triggering;
- detectable by security AI.

Target definition states behavior.

## 30. EMP Sabotage

EMP is normally temporary unless it causes actual hardware damage through GDS-9.

Mission objective `Disable Persistent` cannot be completed by a temporary EMP unless explicitly allowed.

## 31. Demolition

Controlled demolition requires:

- explicit destructible target;
- payload;
- placement;
- arming;
- safety/authorization;
- detonation.

Collateral damage follows blast geometry.

## 32. Critical Component Theft

Some systems can be disabled by physically removing a component.

Sequence:

Component installed in target  
→ removed  
→ component becomes physical loot owner state  
→ system becomes Disabled/Degraded.

The component cannot remain installed and also exist in player inventory.

## 33. Defender Repair / Recovery

Defenders may restore sabotage if they possess:

- suitable worker/automation;
- access;
- replacement part/resource;
- power;
- time;
- security control.

## 34. Attacker Denial

The player can make repair harder by:

- destroying instead of disabling;
- removing critical parts;
- blocking route;
- disabling repair automation;
- maintaining control of the area.

This costs more time/resources/risk.

## 35. Sabotage Persistence

After raid resolution, sabotage result maps to target persistent state:

- Temporary effects expire through their own rules;
- Persistent disablement remains until actual target recovery;
- Destroyed hardware remains destroyed until replaced/rebuilt;
- stolen components remain absent.

GDS-12 owns long-term target recovery pacing.

## 36. Faction Consequences

GDS-7/GDS-12 can apply faction/reputation/world consequences based on:

- target ownership;
- evidence/identification;
- severity;
- strategic impact.

Sabotage math itself does not change reputation directly.

## 37. Objective Integration

Sabotage objectives must specify exact target state:

- Compromise;
- Temporarily Disable;
- Persistently Disable;
- Destroy;
- Remove Component;
- Overload to defined state.

The vague verb "sabotage" alone is not sufficient implementation data.

## 38. Player Risk

Sabotage can create hazards that affect attacker equally.

Examples:

- power cut blocks lift;
- venting removes breathable atmosphere;
- coolant loss overheats route;
- reactor fault raises radiation;
- security hack locks wrong segment after recovery.

## 39. No Universal One-Best Sabotage

A good raid can support multiple approaches with tradeoffs.

Examples:

- destroy turret: permanent but noisy;
- cut power: reversible but efficient;
- hack control: quieter but requires access/time;
- steal magazine: yields loot but requires cargo/time.

## 40. Edge Cases

If a shield loses power and later a redundant route restores it, shield can recover normally; completed objective counts only if it required temporary disablement or if persistent criteria remain satisfied.

If the player removes a critical component then loses it before extraction, the target remains missing that component until it recovers/replaces it, while the player does not receive the loot.

If defenders repair a system after its mission objective already committed, the objective remains completed unless mission explicitly requires the system to stay disabled through extraction.

If a reactor safe-shuts down after attempted overload, a Destroy objective is not satisfied simply because power is off.

## 41. Explicit Non-Goals

Sabotage does not provide:

- one-button whole-station shutdown;
- universal vent-all console;
- instant reactor explosion;
- temporary EMP counting as permanent destruction;
- repair without parts/access;
- duplicate stolen components.

## 42. Tuneable Parameters

Hack time, repair response, component durability, overload thresholds, shutdown delays, system redundancy, detection sensitivity, and sabotage duration are tuneable.

## 43. Dependencies

This specification depends on GDS-2 Station Systems/Security, GDS-4 ownership, GDS-8 Objectives, GDS-9 Damage/Status, GDS-10 Hacker/Breacher/Demolition/Repair, Interior Attack, Escalation, and future Persistence.

## 44. Open Questions

None in the sabotage baseline.
