# Spacecraft Combat

> **Status:** Design Complete  
> **Authority:** Ship-combat participation model, weapon mounting/operation boundary, shields, targeting interfaces, tactical power/thermal interaction, disable-vs-destroy philosophy, and handoff to GDS-9 Combat

## 1. Purpose

GDS-6 defines how spacecraft participate in combat without preempting the universal combat/damage formulas owned by GDS-9.

## 2. Combat Loop

Baseline ship combat combines:

- piloting;
- positioning;
- target selection;
- weapon use;
- power management;
- heat management;
- shields;
- subsystem damage;
- disengagement/escape.

## 3. Direct Player Participation

The player directly pilots the current ship during normal ship combat.

Combat is not resolved only through an abstract strategy menu.

## 4. Weapon Hardpoints

Weapons mount to compatible hull hardpoints.

Hardpoint classes:

- Light;
- Medium;
- Heavy;
- Defensive.

Utility mounts are not automatically weapon hardpoints.

## 5. Weapon Families Boundary

GDS-9 can define ship-compatible families such as:

- laser;
- kinetic/rail;
- missile;
- point defense;
- EMP/disruption.

GDS-6 only defines their integration requirements.

## 6. Weapon Requirements

A ship weapon can require:

- hardpoint;
- power;
- thermal capacity;
- ammunition;
- sensor/target data;
- line/firing arc;
- operational condition.

## 7. Fixed vs Turreted Weapons

A weapon mount defines:

- Fixed;
- Limited Gimbal;
- Turreted.

The mount's physical firing arc is authoritative.

## 8. No Through-Hull Fire

Weapons cannot intentionally fire through the owning ship's solid geometry.

## 9. Targeting

Ship targeting can provide:

- target lock;
- lead/aim assistance;
- range;
- relative velocity;
- subsystem targeting where sensor/combat rules permit.

Sensor damage can reduce targeting capability.

## 10. Manual Fire

The player can fire directly controlled weapons.

Input/weapon behavior is defined by GDS-9.

## 11. Automated Defense

Point-defense or turret automation can engage according to configured rules.

Automation has finite tracking/firing capability.

## 12. Shields

Ship shields are local to the ship.

They require:

- shield module;
- power;
- thermal support;
- capacity.

GDS-9 owns damage absorption/overflow formulas.

## 13. Shield Regeneration

Regeneration can require:

- no blocking fault;
- sufficient power;
- thermal margin;
- delay after damage.

Exact values belong to combat/equipment data.

## 14. Armor

Hull armor can mitigate incoming damage according to GDS-9.

Armor contributes mass.

## 15. Power Tradeoffs

During combat the player can prioritize:

- propulsion;
- shields;
- weapons;
- sensors;
- life support/essential control.

A power-limited ship cannot maximize every subsystem simultaneously.

## 16. Thermal Tradeoffs

Sustained:

- boosting;
- shield regeneration;
- energy-weapon fire;
- reactor output

can compete for thermal capacity.

Overheat can force tactical cooldown/load reduction.

## 17. Ammunition

Kinetic/missile weapons consume physical ammunition.

Ammo occupies ship inventory/magazine mass.

No automatic ammunition refill occurs during combat.

## 18. Local Magazines

Weapons can have local magazines fed from ship stores according to module design.

Reload/feed time is finite.

## 19. Subsystem Targeting

When sensors/weapon rules allow it, attacks can target:

- propulsion;
- weapons;
- sensors;
- shield generator;
- radiator;
- power;
- docking systems.

This supports disable/sabotage tactics.

## 20. Disable vs Destroy

Ship combat supports disabling a target without necessarily annihilating it.

A disabled ship can become:

- boarding target;
- salvage target;
- mission capture objective.

## 21. Boarding Setup

GDS-6 supports the physical preconditions for boarding:

- target disabled or access opportunity;
- compatible docking/breaching method;
- relative-motion control.

Raid/Combat/Mission domains own actual boarding gameplay.

## 22. Collision/Ramming

Collision can cause combat damage.

Ramming is dangerous to both participants according to mass/velocity/damage rules.

No ship receives magical immunity because it initiated the collision.

## 23. Retreat

A player can attempt disengagement by:

- creating distance;
- reaching a travel/escape condition;
- restoring drive;
- using mission-specific extraction.

Combat does not guarantee instant escape when a strategic drive button is pressed.

## 24. Strategic Drive During Combat

Strategic travel initiation can be blocked by:

- insufficient warm-up;
- active disruption;
- location rule;
- invalid navigation;
- damaged drive.

Exact combat lock/escape rules are finalized by GDS-8/GDS-9.

## 25. Player Defeat in Ship Combat

If the player is defeated while operating their persistent ship:

- ordinary loss does not permanently delete the established ship;
- Mission/Combat resolve player and ship recovery;
- ship damage/cargo consequences remain systemic.

## 26. Enemy Ship Outcome

Enemy/nonpersistent ships can be:

- destroyed;
- disabled;
- surrendered/captured where later systems support it.

They do not inherit player-owned persistence protection unless explicitly persistent.

## 27. Combat HUD Requirements

Ship combat UI must eventually expose:

- current target;
- range/relative motion;
- weapon state;
- ammunition;
- shield;
- hull/module warnings;
- power;
- heat;
- propulsion;
- lock state.

Presentation belongs to GDS-13.

## 28. No Combat Rating Shortcut

A UI may summarize ship combat capability, but actual combat uses real installed systems.

## 29. Persistence

Combat-relevant module condition, ammunition, shield state, heat, power, and damage persist across valid saves/transitions.

## 30. Edge Cases

If shields collapse while weapons are firing, power can be reprioritized without creating extra generation.

If sensor lock is lost, guided weapons follow GDS-9 guidance rules rather than retaining magical perfect tracking.

If a target becomes disabled, weapons do not automatically stop unless engagement policy says so.

If the player boards a disabled ship, both ships retain their actual physical states.

## 31. Explicit Non-Goals

GDS-6 ship combat does not define:

- final damage-type table;
- final weapon statistics;
- armor penetration formulas;
- enemy combat AI;
- universal combat balance.

Those belong to GDS-9.

## 32. Tuneable Parameters

Tuneable values include mount traverse, shield capacity/regeneration, weapon power/heat envelopes, magazine capacity, targeting range, and disengagement thresholds.

## 33. Dependencies

This specification depends on Ship Systems, Flight, Configuration, Damage, Cargo, Docking, GDS-9 Combat, GDS-8 Missions, GDS-11 Raids, and GDS-10 Robots.

## 34. Open Questions

None in the ship-combat integration baseline.

Final weapon/damage mechanics are deferred to their authoritative GDS-9 owner rather than left ambiguous here.
