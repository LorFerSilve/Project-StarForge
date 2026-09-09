# Spacecraft Combat Resolution

> **Status:** Draft  
> **Authority:** GDS-9 damage/weapon/targeting behavior specific to spacecraft combat, ship weapon classes, shield/armor interaction, subsystem targeting, missile guidance, disable/destruction resolution, and combat escape boundary

## 1. Purpose

This document completes the combat-side authority that GDS-6 intentionally deferred.

GDS-6 continues to own:

- ship flight;
- power;
- thermal;
- cargo;
- module condition states;
- Disabled/Derelict recovery;
- docking;
- persistent ship identity.

GDS-9 owns how attacks hit and damage those systems.

## 2. Ship Combat Damage Pipeline

A valid ship hit resolves:

1. hit/impact;
2. ship shield absorption;
3. armor/structural mitigation;
4. hit-zone/subsystem routing;
5. module/hull condition damage;
6. eligible status effects;
7. impulse/system notifications.

## 3. Ship Hit Zones

A Hull Variant exposes authored zones such as:

- Forward Hull;
- Port Hull;
- Starboard Hull;
- Aft Hull;
- Dorsal/Ventral sections where needed;
- Main Drive;
- Maneuvering Thrusters;
- Reactor/Power;
- Radiator/Thermal;
- Sensors;
- Shield Generator;
- Weapon Mounts;
- Cargo;
- Docking System.

Exact geometry is hull data.

## 4. Targetable Subsystems

A subsystem can be deliberately targeted only if:

- sensor/targeting quality is sufficient;
- target subsystem is known/identified;
- firing solution supports precision;
- subsystem collider/zone is exposed enough.

The UI cannot provide perfect subsystem lock against an unidentified target.

## 5. Subsystem Hit

If attack hits an identified subsystem zone:

- shield still resolves first if covering it;
- local armor resolves;
- remaining damage applies to that module/system;
- excess structural damage can also affect hull according to hull definition.

## 6. Hull Hit

A hit not assigned to a specific module applies to the relevant hull/armor zone.

## 7. Ship Shields

Baseline player/ordinary ship shields use a single hull-wide shield Capacity unless a specific model later adds directional segments.

Damage Model Shield Cost/Overflow rules apply.

## 8. Ship Armor

Ship armor uses the same protection curve with ship-scale ratings and attack Penetration.

On-foot and ship numerical scales can use different data ranges while sharing the formula.

## 9. Scale Compatibility

On-foot small arms normally have negligible or insufficient penetration against proper spacecraft armor unless:

- firing at exposed/unarmored component;
- weapon is explicitly anti-materiel;
- target is a very light craft.

This is data-driven through armor/penetration, not a special invisible immunity.

## 10. Canonical Ship Weapon Families

Baseline ship weapon classes:

1. Laser Cannon;
2. Railgun / Kinetic Cannon;
3. Missile Launcher;
4. Point-Defense System;
5. EMP / Disruption Weapon;
6. Plasma / High-Energy Cannon where advanced technology supports it.

## 11. Laser Cannon

Behavior:
- hitscan;
- Thermal damage;
- precise line of sight;
- high power/heat load;
- good subsystem pressure.

## 12. Railgun / Kinetic Cannon

Behavior:
- high Penetration;
- Kinetic damage;
- strong hull/subsystem attack;
- ammunition + power/heat requirements.

Gameplay hit resolution can use hitscan at normal local-combat distances.

## 13. Missile Launcher

Behavior:
- physical projectile;
- Explosive payload;
- requires ammunition;
- guided variants use target lock.

Missiles can be intercepted by Point Defense.

## 14. Point Defense

Purpose:
- engage incoming missiles/eligible small threats.

Point Defense has:
- firing arc;
- tracking rate;
- range;
- ammunition/energy;
- heat;
- reaction/targeting constraints.

It does not guarantee missile immunity.

## 15. EMP / Disruption Weapon

Purpose:
- shield drain;
- electronics disruption;
- temporary system disable/jam.

It does not automatically destroy hull.

## 16. Plasma / High-Energy Cannon

Advanced weapon.

Behavior:
- projectile or authored short-range energy packet;
- Thermal damage;
- high power/heat;
- visible/dodgeable where projectile-based.

## 17. Weapon Arcs

Ship weapons fire only inside their mount's physical firing arc.

A turret cannot shoot through owning hull.

## 18. Fixed Weapons

Fixed weapons require the pilot to orient the ship.

## 19. Turreted Weapons

Turrets can track independently within:
- traverse;
- elevation;
- slew rate;
- sensor lock.

## 20. Target Lock

A Target Lock is a sensor/targeting solution.

Lock quality can affect:

- guided missile tracking;
- subsystem targeting;
- firing lead;
- target information.

Lock does not increase raw weapon damage.

## 21. Missile Lock

Guided missile launch can require minimum lock state.

After launch, guidance can be degraded by:

- sensor loss;
- jamming;
- target maneuver;
- decoy/countermeasure where later content defines it;
- missile damage/interception.

## 22. Unguided Ordnance

Unguided rockets/missiles follow physical trajectory without target correction.

## 23. Leading Targets

Hitscan weapons use current line-of-sight aim.

Projectile weapons require lead according to projectile speed/relative motion.

Targeting assistance can display a lead solution if sensors support it.

## 24. Ship Weapon Ammunition

Kinetic/missile weapons consume actual ship ammunition.

GDS-6 cargo/magazine mass applies.

Reload/feed behavior is finite.

## 25. Energy Weapons

Laser/plasma systems consume:
- power;
- heat capacity;
- ammunition/cell only if their module definition requires it.

They do not have infinite sustained output merely because they lack physical rounds.

## 26. Power Allocation

Weapon availability follows actual GDS-6 ship power.

If generation is insufficient:
- batteries can support bursts;
- lower-priority systems can shed;
- weapon output/recharge can stop.

## 27. Thermal Limitation

Sustained weapons add heat.

Overthermal ship state can:
- throttle firing;
- disable weapon;
- force cooling.

## 28. Shield Regeneration

Ship shield regeneration uses:
- power;
- thermal capacity;
- Recharge Delay;
- operational shield module.

Taking qualifying shield damage resets delay.

## 29. Armor Breach / Hull Damage

Combat damage can lead to:
- hull integrity loss;
- pressure breach;
- coolant leak;
- propellant leak;
- module damage

through GDS-6 damage rules.

GDS-9 does not create those resources/state independently.

## 30. Disable

A spacecraft counts as **Combat Disabled** when it cannot continue effective combat/flight under the objective's declared criteria.

Possible criteria:
- propulsion disabled;
- all weapons disabled;
- avionics/control disabled;
- ship enters GDS-6 Disabled state.

Mission Objective data must specify which.

## 31. Destroy

A spacecraft counts as **Destroyed** only when its target definition reaches a nonrecoverable destruction state for that actor.

Persistent player ships retain GDS-6 ordinary-loss protection and do not become permanently deleted merely because combat damage would destroy an ordinary NPC ship.

## 32. Capture / Boarding

Capture requires:
- target cannot effectively prevent boarding, or an authored access opportunity exists;
- relative motion controlled;
- valid docking/breach method;
- boarding mission/raid completion.

Reducing Hull HP alone does not equal Capture.

## 33. Drive Off

A ship objective counts as **Drive Off** when the target commits a retreat/departure state that removes it from the encounter according to mission rules.

## 34. Collision / Ramming Damage

Collision Damage Packet considers:
- relative velocity;
- participating mass;
- collision geometry/material;
- armor/structure.

Exact coefficients are tuneable.

Both actors can take damage.

## 35. Combat Escape

Strategic escape requires GDS-6 Navigation/Propulsion conditions.

Combat can interfere through:
- drive damage;
- warm-up;
- disruption;
- mission/location blocker.

There is no instant invulnerable warp button.

## 36. Enemy Ship AI

Generic perception/target principles come from Enemy Combat Behavior.

Ship-specific tactics can include:
- maintain firing envelope;
- pursue;
- break away;
- protect weak side;
- missile standoff;
- disable/board target.

Final faction ship tactics can be content/GDS-11 dependent.

## 37. Player Ship Defeat

If player's persistent ship becomes Disabled:
- GDS-6 recovery state applies;
- GDS-8 mission failure/rescue can apply;
- combat does not delete ship ownership.

## 38. UI Requirements

Presentation later shows:
- target shield/hull state where known;
- lock;
- range/relative velocity;
- weapon readiness;
- selected subsystem where known;
- missile warning;
- player power/heat;
- damage alerts.

## 39. Explicit Non-Goals

Spacecraft combat does not provide:

- subsystem targeting without sensor knowledge;
- missiles immune to interception;
- infinite energy-weapon output;
- hull destruction automatically meaning capture;
- permanent player ship deletion on routine loss;
- target lock as damage multiplier.

## 40. Tuneable Parameters

Ship weapon damage, penetration, tracking, missile speed, point-defense efficiency, collision coefficients, shield/armor values, lock times, and heat/power costs are tuneable.

## 41. Dependencies

This specification depends on GDS-6 Ship Combat/Damage/Systems/Flight, Damage Model, Hit Resolution, Armor/Shields, Status Effects, Missions, and future Raids.

## 42. Open Questions

None in the spacecraft combat-resolution baseline.
