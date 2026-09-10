# Station Defenses

> **Status:** Design Complete  
> **Authority:** External home-station defensive layers, targeting, shield coverage, weapon operation, interception, ammunition support, and defensive failure behavior

## 1. Purpose

Station defense translates strategic base-defense gameplay into a physical 3D space-station system.

Effectiveness depends on layout, coverage, power, cooling, sensors, ammunition, and redundancy.

There is no single global defense number that replaces these systems.

## 2. Defense Layers

The baseline layers are:

1. detection and tracking;
2. shield coverage;
3. long and medium-range weapons;
4. point defense;
5. defensive drones;
6. internal security after boarding.

Internal security is defined separately.

## 3. Detection

Automated defense needs valid target information from:

- station sensors;
- allied or ship sensors;
- local weapon sensors where defined.

Sensor loss can reduce range or accuracy without necessarily disabling every local weapon.

## 4. Target Classes

Target classes include:

- large ship;
- small ship;
- boarding craft;
- missile;
- drone;
- dangerous debris.

A weapon definition declares eligible classes.

## 5. Engagement Policy

The player configures:

- hostile-only behavior;
- target priority;
- ammunition conservation;
- engagement range;
- hold-fire state.

Neutral and friendly entities are not normal automatic targets.

## 6. Weapon Families

The baseline supports:

- Laser Turret: fast precise energy weapon, heavy power/thermal demand.
- Railgun or Kinetic Turret: high impact, physical ammunition.
- Missile Battery: guided long-range fire, missile inventory and tracking.
- Point-Defense Cannon: anti-missile and anti-drone.
- EMP or Disruption System: specialized electronic disruption.
- Defensive Drone Bay: launches finite defensive craft.

Exact statistics are content-level tuneable data.

## 7. Physical Placement

Defensive modules are physically placed outside the station.

Placement determines:

- firing arc;
- line of sight;
- vulnerability;
- network access;
- coverage overlap.

## 8. Firing Arc

Each weapon has an authored mechanical firing envelope.

It cannot fire through the station or rotate through impossible geometry.

## 9. Line of Fire

Direct-fire weapons verify a valid line before firing.

Automated defense avoids knowingly shooting through protected friendly station geometry.

## 10. Shield Model

Station shields use local coverage zones instead of one universal station-wide HP bar.

A shield device defines a protected spatial volume or hull sector.

Coverage requires:

- functioning shield hardware;
- power;
- thermal support;
- remaining shield capacity.

## 11. Shield Overlap

Overlapping shield zones provide redundancy or authored reinforcement.

Stacking has explicit limits; unlimited overlapping emitters cannot create infinite strength.

## 12. Shield Capacity and Collapse

Each shield zone tracks:

- maximum capacity;
- current capacity;
- regeneration;
- overload/collapse state.

Protected incoming damage reduces the relevant shield zone first according to combat rules.

A depleted zone collapses locally.

## 13. Shield Regeneration

Regeneration requires:

- available power;
- functional hardware;
- acceptable thermal condition;
- no blocking fault.

## 14. Power and Thermal Cost

Defensive systems can create extreme power and heat demand.

A station can have sufficient weapons but insufficient infrastructure to sustain them.

## 15. Ammunition

Kinetic and missile weapons require physical ammunition.

Ammunition must be stored, routed, and loaded.

An isolated central stockpile does not give an empty turret infinite shots.

## 16. Local Magazines

Ammo weapons have finite ready-use magazines.

Logistics resupplies them when valid routes and stock exist.

Temporary logistics failure therefore does not instantly empty an already loaded weapon.

## 17. Point Defense and Saturation

Point defense has finite tracking, firing, ammunition, and thermal capacity.

Enough simultaneous threats can saturate coverage.

## 18. Defensive Drones

Drone bays have finite ready craft.

Drones require maintenance, repair, and replacement according to robot/spacecraft rules.

## 19. Manual Weapon Control

Selected defenses can be manually controlled from an appropriate station interface.

Manual control uses the same physical constraints and resources as automated fire.

## 20. Weapons Automation

A Weapons Specialist or control system can manage:

- target priorities;
- firing policy;
- ammunition conservation;
- coverage coordination;
- failure reporting.

It does not bypass missing resources or broken hardware.

## 21. Friendly-Fire Safety

Automated direct fire avoids predicted friendly obstructions under normal policy.

## 22. Defensive Damage

Attackers can target:

- sensors;
- shield emitters;
- weapons;
- power routes;
- cooling;
- ammunition;
- drone bays.

This supports systemic raid strategy.

## 23. Defense States

A defense can be:

- Offline
- Standby
- Tracking
- Engaging
- Reloading or Recharging
- Cooling Limited
- Ammo Empty
- Unpowered
- Faulted
- Destroyed

## 24. Combat Alert

Confirmed hostile approach transitions the station to a defense alert.

Construction Mode becomes unavailable.

Defense, security, crew, and automation react according to their policies.

## 25. Player Away From Station

During active gameplay elsewhere, defense continues to simulate.

Optimized off-screen resolution must respect actual installed coverage, resources, power, cooling, and damage.

No attacks progress because real-world offline time passes.

## 26. Strategic Layout

Good layouts can use:

- overlapping arcs;
- protected sensors;
- distributed ammunition;
- redundant power;
- shield overlap;
- separated single points of failure.

## 27. Defense UI

The tactical view shows:

- weapon coverage;
- shield coverage;
- blind spots;
- tracked threats;
- ammo;
- power or thermal limitation;
- damage;
- drone readiness.

## 28. Visual and Audio Feedback

Defense visibly and audibly communicates:

- turret tracking;
- firing;
- missile launches;
- shield impacts;
- shield collapse;
- reload/cooldown;
- alarms;
- damage.

## 29. Persistence

Configuration, ammunition, shield state, damage, drone readiness, and target policy persist.

## 30. Edge Cases

If sensors fail, local-sensor weapons keep only their authored local capability.

If ammunition storage is destroyed, already loaded magazines remain unless directly damaged.

If one shield emitter is destroyed, unrelated valid shield zones remain.

If power is insufficient, configured priorities determine which defense remains active.

If a friendly ship blocks a direct firing line, automatic fire holds or selects another shot.

## 31. Progression

Early game: few weapons, weak local shields, blind spots.

Mid game: layered weapons, point defense, sensor redundancy, shield zones.

Late game: overlapping defense, distributed magazines, powerful weapons, drones, redundant support infrastructure.

## 32. Explicit Non-Goals

The baseline does not provide:

- one global defense score;
- infinite range;
- infinite ammunition;
- weapons firing through own station;
- one station-wide shield HP value;
- offline real-world attacks.

## 33. Tuneable Parameters

Tuneable values include range, damage, fire rate, tracking, magazine size, shield capacity, regeneration, power demand, thermal load, and coverage.

## 34. Dependencies

This specification depends on power, thermal, logistics, damage, security, combat, robots, raids, sensors, and spacecraft.

## 35. Open Questions

None in the current baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
