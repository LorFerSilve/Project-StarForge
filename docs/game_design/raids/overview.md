# Raid System Overview

> **Status:** Design Complete  
> **Authority:** Raid identity, lifecycle, specialization of GDS-8 missions, target persistence, offensive/defensive raid philosophy, raid-scale boundaries, and global raid rules

## 1. Purpose

Raids are multi-phase offensive or defensive operations against stations and other fortified installations.

They combine:

- intelligence;
- spacecraft approach/combat;
- defenses;
- boarding/breaching;
- robot squads;
- interior combat;
- hacking/sabotage;
- physical looting;
- escalation;
- extraction;
- persistent damage and aftermath.

## 2. Raid Is a Specialized Mission

A player offensive Raid is a specialized GDS-8 Mission.

It uses the same:

- Mission ID;
- lifecycle;
- objective graph;
- Mission Instance;
- deterministic seed rules;
- threat model;
- extraction transaction;
- failure/abandonment semantics;
- reward commit rules.

GDS-11 adds raid-specific phases and target-system behavior rather than creating a second unrelated mission engine.

## 3. Core Raid Loop

The canonical offensive loop is:

Target Discovered  
→ Intelligence  
→ Preparation  
→ Deployment / Approach  
→ External Access or Assault  
→ Boarding / Breach  
→ Interior Operation  
→ Objective / Sabotage / Loot  
→ Escalation Response  
→ Withdrawal  
→ Extraction  
→ Aftermath.

A valid raid may skip a phase when a real alternate access route exists.

Example:

- stolen docking authorization can skip destructive external breaching;
- an exposed maintenance EVA route can bypass a defended docking port;
- a purely external sabotage raid may not require boarding.

## 4. Raid Success

Raid success is objective-based.

Destroying every defender is not required unless the raid explicitly contains a Clear Area/Destroy objective.

For an Extraction Required raid, success commits only when:

- all mandatory objectives are complete;
- required cargo/targets are secured;
- authorized extraction commits.

## 5. Raid Objective Families

Baseline raid goals include:

- steal physical resources/cargo;
- recover data/artifact;
- rescue/capture a designated target;
- sabotage a subsystem;
- disable a facility;
- destroy a strategic target;
- create temporary access for another mission;
- secure a command/control node;
- survive/repel an attack;
- defend Horizon Station.

## 6. No Universal Base Destruction Goal

A raid is not automatically a demolition mission.

The player can win by:

- stealing the objective;
- disabling one critical system;
- escaping with a survivor;
- hacking a control node;
- forcing a retreat;
- surviving long enough;
- defending a protected module.

## 7. Raid Target

A Raid Target is a persistent Strategic Location or mission installation containing an explicit raid-relevant state.

It records, where applicable:

- Target ID;
- owning faction;
- known/unknown intelligence;
- external defenses;
- shield zones;
- raid-relevant power/control dependencies;
- valid access points;
- interior raid zones;
- storage/loot state;
- security state;
- reinforcement capability;
- prior damage/sabotage;
- prior raid outcomes.

## 8. Persistent Target State

A raid does not reload a pristine target merely because the player leaves.

Committed changes can persist, including:

- destroyed turret;
- depleted storage;
- disabled relay;
- compromised access route;
- damaged shield emitter;
- stolen unique object;
- changed faction/security posture.

GDS-12 later defines long-term recovery/replenishment pacing.

## 9. No Reload Reroll

For one Raid/Mission ID, reload cannot reroll:

- layout;
- major loot;
- defenders;
- access points;
- reinforcement schedule seed;
- target subsystem placement.

## 10. Raid-Relevant Simulation Boundary

An NPC fortified target does not require the full home-station simulation for every inaccessible room.

However, every system that can affect the raid must be causally modeled.

At minimum where present:

- sensors;
- shields;
- weapons;
- local power dependencies;
- security/doors;
- communications;
- atmosphere/pressure in active interior zones;
- storage/loot;
- reinforcement links;
- mission-critical systems.

Unsimulated background sections cannot secretly grant infinite power, ammunition, defenders, or loot.

## 11. Physical Causality

Raid systems obey actual dependencies.

Examples:

- destroy power relay → only connected dependent systems lose power;
- jam communications → information/reinforcement call capability degrades where that link is used;
- destroy local shield emitter → only its coverage is lost;
- breach hull → affected pressurized zone responds through atmosphere rules;
- empty magazine → turret cannot keep firing without resupply.

## 12. No Single Raid HP Bar

A station/fortified target is not defeated by reducing one global Raid HP value.

Success comes from physical/system objectives.

## 13. Offensive Raids

The player directly participates in offensive raids using:

- selected spacecraft;
- personal loadout;
- GDS-10 Tactical Squad;
- mission tools/cargo capacity.

The player may choose destructive or lower-signature approaches where the target supports them.

## 14. Defensive Raids

Hostile factions can raid Horizon Station during active gameplay.

A defensive raid uses the same conceptual phases in reverse:

Detection  
→ Approach  
→ External Defense  
→ Boarding Attempt  
→ Interior Defense  
→ Attacker Withdrawal/Defeat  
→ Recovery.

## 15. No Real-World Offline Attacks

No raid progresses because the real-world clock advances while the application is closed.

This preserves DD-012.

## 16. Station Attack While Player Is Away

A hostile attack on Horizon Station can occur while the player is deployed elsewhere during Simulation Time.

This does not violate the one-Deployed-Mission rule because the station attack is a persistent Home Station Event, not a second player-deployed external Mission Instance.

Station defenses, crew, robots, resources, and damage continue through deterministic lower-detail simulation.

## 17. No Global Defense Score Auto-Resolve

Off-screen station defense cannot reduce the station to one opaque defense number.

Resolution must derive from actual:

- sensors;
- shield coverage;
- firing arcs;
- ammunition;
- power;
- cooling;
- robot readiness;
- access/security topology;
- attacker composition;
- damage.

Technical simulation fidelity can be lower off-screen, but gameplay rules remain equivalent.

## 18. Player Return During Station Attack

If travel state and time permit, the player may abandon/withdraw from the current external mission and return toward Horizon Station.

The station attack does not pause merely because the player chooses to return.

## 19. Communication of Home Attack

The player receives remote warning only through a valid information path such as:

- functioning station communications;
- allied relay;
- ship communication capability.

If communications are jammed/destroyed, warning can be delayed or absent until another valid source observes/reports the event.

The game never invents omniscient in-world notification.

## 20. Human Crew Protection

Routine station raids do not permanently kill recruited crew.

Crew can be:

- wounded;
- incapacitated;
- isolated;
- forced to evacuate internally.

Permanent crew loss requires an explicitly authored narrative exception or later accepted design change.

## 21. Robot Attrition

Robots can be permanently Destroyed during raids according to GDS-10.

This creates meaningful defensive attrition.

## 22. Horizon Station Core Protection

A routine procedural raid cannot permanently delete Horizon Station or permanently destroy the campaign-critical Command Core.

Attackers can:

- disable it;
- damage supporting infrastructure;
- steal physical resources;
- destroy non-protected modules/robots;
- sabotage systems;
- force a recovery phase.

A campaign-authored finale/event can define exceptional stakes explicitly.

## 23. Knowledge Asset Protection

An attacker cannot make the player forget already acquired:

- Research;
- Blueprint Unlocks;
- Research Evidence;
- discovered routes.

Physical unanalyzed data carriers can be stolen if they physically exist as raid loot.

## 24. No Arbitrary Station Annexation Baseline

The player does not automatically convert any raided NPC station into a second fully player-managed Home Station.

A raid can temporarily seize controls or complete a Capture objective, but persistent multi-base ownership is outside baseline GDS-11 unless a specific authored location says otherwise.

## 25. Raid Scale

Raid complexity can range from:

- Fortified Installation;
- Outpost;
- Standard Station/Base;
- Major Fortress/Shipyard.

Scale describes physical/phase complexity, not a replacement for GDS-8 Threat axes.

## 26. Threat

Raid danger still uses GDS-8:

- Hostile Threat;
- Environmental Threat;
- Operational Complexity;
- Extraction Risk.

No separate opaque Raid Power Rating replaces them.

## 27. Retreat Is Valid Gameplay

The player can decide that the operation is no longer worth the cost.

Retreat:

- does not magically extract;
- requires actual withdrawal;
- can preserve already secured loot/robots;
- normally forfeits unresolved mandatory objectives.

## 28. Resource Conservation

Raid combat, looting, repairs, ammunition, robot losses, and station theft obey physical ownership.

Nothing is created or deleted merely because a raid state changes.

## 29. Safe-Scumming Boundary

A committed raid transaction cannot be undone inside the same later save state by reopening the target.

The player's explicit manual save selection remains a persistence-system concern in GDS-12.

## 30. Multiplayer Boundary

The baseline game contains **no asynchronous PvP raid system** and no synchronous PvP requirement.

All core raid gameplay is single-player against systemic NPC/faction targets and attackers.

A future multiplayer design would require a separate accepted specification.

## 31. Explicit Non-Goals

GDS-11 does not provide:

- abstract troop-card battles;
- one global base HP bar;
- one opaque defense score;
- attacks while the application is closed;
- unlimited defender spawning;
- loot percentage subtraction without physical theft;
- automatic player ownership of every captured base;
- baseline PvP.

## 32. Tuneable Parameters

Tuneable values include raid scale distributions, defender counts, reinforcement timing, objective budgets, response thresholds, and recovery pacing.

## 33. Dependencies

This specification depends on GDS-2 Station, GDS-4 Resources, GDS-6 Spacecraft, GDS-7 World/Factions, GDS-8 Missions, GDS-9 Combat, GDS-10 Robots, and Economy/Persistence/Presentation.

## 34. Open Questions

None in the global raid-system baseline.
