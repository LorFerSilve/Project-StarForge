# Spacecraft Damage and Repairs

> **Status:** Design Complete  
> **Authority:** Ship structural/functional damage states, subsystem faults, breaches, disabled/derelict state, emergency repair, service repair, recovery, and routine ship-loss constraints

## 1. Purpose

Ship damage makes combat, collision, hazards, and poor engineering consequential without making established ships disposable in routine play.

## 2. Damage Layers

Spacecraft use:

- Hull/Structural Integrity;
- Armor state where installed;
- Module Condition;
- Fault State;
- Pressure Boundary state;
- Shield state where equipped.

Combat owns attack/damage formulas.

## 3. Hull States

Baseline structural states:

- Intact;
- Damaged;
- Critical;
- Disabled/Compromised.

A normal gameplay ship is not reduced to one HP number for every subsystem.

## 4. Module States

Modules can be:

- Operational;
- Degraded;
- Faulted;
- Disabled;
- Destroyed.

## 5. Damage Sources

Possible sources:

- weapons;
- collision;
- debris;
- overheating;
- reactor/energy event;
- boarding sabotage;
- environmental hazard;
- docking accident.

## 6. Damage Targeting

Damage can affect explicit ship zones/modules.

Examples:

- main drive;
- maneuvering thrusters;
- reactor;
- battery;
- radiator;
- sensors;
- shield generator;
- weapon;
- cargo hold;
- docking connector;
- life support.

## 7. Systemic Consequence

Damaging a subsystem changes its actual capability.

Examples:

- destroyed thruster reduces control;
- damaged radiator reduces sustained output;
- sensor damage reduces information;
- cargo damage risks cargo;
- drive damage can block strategic travel.

## 8. Hull Breach

Walkable/pressurized ships can suffer a pressure breach.

A breach affects actual cabin atmosphere.

Response can include:

- compartment isolation;
- suit use;
- emergency patch;
- full repair.

## 9. Compact Craft Breach

For cockpit-only compact craft, a breach can use one cabin-volume model rather than detailed compartment simulation.

## 10. Fire

Internal fire can result from explicit damage/events.

Fire consumes atmosphere, adds heat, and damages systems according to owning rules.

## 11. Propellant Leak

Tank/line damage can leak Reaction Propellant or other stored fluid/gas.

Leak:

- reduces physical inventory;
- can disable propulsion endurance;
- may create hazard depending on resource.

## 12. Coolant Leak

Thermal damage can leak Industrial Coolant.

Cooling performance degrades as inventory falls.

## 13. Electrical/Control Fault

Damage can isolate or disable modules without destroying hull.

Fault diagnosis/repair can restore operation if components remain recoverable.

## 14. Emergency Stabilization

Possible temporary fixes:

- seal breach;
- isolate leaking tank;
- bypass failed power path;
- disable damaged module;
- patch coolant line;
- reset local controller.

Stabilization is not full repair.

## 15. Player Field Repair

The player can perform eligible ship repairs while:

- landed/docked;
- EVA with valid access;
- inside walkable service spaces.

Requires:

- Engineering Multitool or specialized tool;
- materials/components;
- safe enough access;
- active time.

## 16. EVA Repair

External repair can require EVA.

The ship may continue drifting unless stabilized/docked.

The player must obey GDS-5 Field Survival.

## 17. Crew Repair

Engineer/Mechanic crew can assist/supervise repair when physically present and assigned.

They do not bypass materials/access.

## 18. Repair Drones

Compatible ships can carry repair drones.

Drones have finite capacity and can repair only eligible targets.

## 19. Station Service Repair

A suitable hangar/service berth can provide:

- faster diagnosis;
- heavy component replacement;
- hull repair;
- alignment;
- complete refit.

Docking alone does not automatically repair.

## 20. Repair Materials

Repairs consume GDS-4 canonical resources/components matching the actual damaged system.

There is no generic Ship Repair Token.

## 21. Disabled Ship

A ship becomes **Disabled** when it cannot safely continue normal operational flight.

Possible reasons:

- no usable propulsion/control;
- catastrophic power failure;
- critical hull compromise;
- unrecoverable local life support for occupants;
- other explicit safety condition.

Disabled does not automatically mean destroyed.

## 22. Derelict State

A Disabled ship with no current active operator/crew control can be classified Derelict.

It remains a persistent physical asset.

## 23. Routine Recovery Philosophy

Ordinary player mission failure does not permanently delete an established owned Primary Ship.

Recovery can involve:

- tow;
- rescue;
- station retrieval;
- emergency repair;
- mission failure recovery fiction.

The ship can return heavily damaged and costly to restore.

## 24. Permanent Ship Loss

Permanent loss is outside routine defeat.

It can occur only through:

- explicit player-confirmed abandonment/scrapping;
- authored high-stakes narrative/mission state clearly communicated in advance;
- future accepted design change.

## 25. Towing

A disabled ship can be towed by:

- compatible player/ally craft;
- station rescue;
- mission service.

Tow capability requires explicit utility equipment/mission support.

## 26. Emergency Distress

A disabled ship can emit distress if:

- emergency power/communication exists.

Missions/world determine response.

## 27. Destruction Boundary

Combat may visually/technically produce severe destruction, but persistent owned-ship loss must respect the routine-loss rule.

Enemy/nonpersistent ships can use different destruction outcomes under Combat.

## 28. Repair Queue

Docked automated repairs can use a queue with:

- target;
- severity;
- required resources;
- labor/facility;
- progress;
- blocker.

Station repair/manufacturing principles apply.

## 29. Repair Completion

Full repair restores normal baseline condition but does not upgrade the module.

## 30. Salvage

Scrapped/destroyed ship parts can yield salvage according to GDS-4.

Salvage is below full original recipe return by default.

## 31. Damage UI

The ship diagnostic interface shows:

- hull state;
- module condition;
- breaches;
- leaks;
- power/thermal faults;
- disabled capabilities;
- repair requirements.

## 32. Persistence

Damage, faults, leaks, breaches, repair progress, disabled/derelict state, and recovery state persist.

## 33. Edge Cases

If the main drive is disabled but maneuver thrusters remain, local movement can continue while strategic travel stays blocked.

If life support fails but occupants use suits, the ship can remain physically flyable while occupant endurance becomes limited.

If cargo hold is breached, only cargo exposed by that damage rule is at risk.

If the player repairs one fault on a multi-fault module, unrelated faults remain.

If a disabled owned ship is recovered, its pre-recovery damage state remains except for explicit emergency stabilization performed by recovery service.

## 34. Explicit Non-Goals

Damage/repair does not provide:

- automatic dock healing;
- generic repair currency;
- routine permanent Primary Ship deletion;
- damage with no subsystem consequence;
- free full-material salvage.

## 35. Tuneable Parameters

Tuneable values include hull integrity, module thresholds, leak rates, repair costs/duration, condition penalties, and recovery cost.

## 36. Dependencies

This specification depends on Ship Systems, Flight, Propulsion, Cargo, Docking, GDS-4 resources, GDS-5 tools/EVA, Crew, Combat, Missions, and Persistence.

## 37. Open Questions

None in the spacecraft damage/recovery baseline.
