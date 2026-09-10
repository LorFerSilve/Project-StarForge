# Boarding and Breaching

> **Status:** Design Complete  
> **Authority:** Raid entry methods, docking capture, airlock intrusion, hull breaching, pressure/safety consequences, robot traversal constraints, defender response, and entry-state persistence

## 1. Purpose

Boarding converts successful approach/access work into a physical transition from outside the target to its interior.

Boarding must respect:

- geometry;
- docking;
- access control;
- atmosphere;
- combat;
- robot size/mobility;
- breaching tools;
- security response.

## 2. Canonical Entry Methods

Baseline entry methods:

1. Authorized Docking;
2. Spoofed/Hacked Docking;
3. Forced Docking;
4. Maintenance Airlock/Service Hatch;
5. Existing Structural Opening;
6. Created Hull Breach;
7. Mission-authored compatible boarding interface.

## 3. Authorized Docking

Uses a valid target docking port and accepted authorization.

It can still be revoked if:

- hostile action is detected;
- credentials are invalidated;
- station control locks the port before capture;
- the physical port becomes unavailable.

## 4. Spoofed/Hacked Docking

Requires a valid information/control path.

A successful spoof/hack can grant only the access capability actually compromised.

It does not automatically:

- disable all security;
- unlock all doors;
- make every defender friendly;
- suppress later identity checks.

## 5. Forced Docking

Requires:

- physical docking compatibility or boarding adapter;
- controlled relative motion;
- enough local access to capture/override clamps;
- surviving the approach.

Forced docking can damage:

- player's docking system;
- target port;
- local structure

if performed outside normal limits.

## 6. Maintenance Entry

A maintenance hatch/airlock is valid only if:

- it physically exists;
- player can reach it;
- environmental protection is sufficient;
- access can be opened/breached.

Its advantage may be lower defense exposure.

Its disadvantage may be:

- narrow geometry;
- no Heavy robot access;
- difficult extraction;
- local hazards.

## 7. Existing Structural Opening

Previous damage can create a persistent access route.

The opening remains usable only if:

- geometry remains traversable;
- pressure/environment permits traversal;
- later repair has not closed it.

## 8. Created Hull Breach

A new breach requires an authored breach-capable target surface and sufficient tool/explosive/weapon capability.

The sequence is:

1. reach placement/attack position;
2. identify breachable target;
3. apply breaching work/damage;
4. commit structural opening;
5. resolve atmosphere/security consequences;
6. physically traverse opening.

## 9. Breach Types

Baseline breach outcomes:

- Controlled Access Breach;
- Violent Hull Breach;
- Door/Barrier Breach.

## 10. Controlled Access Breach

Uses cutting/engineering methods intended to create a traversable opening with limited collateral damage.

Typically:

- slower;
- quieter than explosives;
- lower structural/pressure shock;
- requires specialized tool/time.

## 11. Violent Hull Breach

Uses explosives/high-energy attack.

Typically:

- fast;
- loud;
- immediate alert;
- decompression/collateral risk;
- can damage nearby systems/loot/actors.

## 12. Door / Barrier Breach

Targets:

- locked door;
- security shutter;
- blast door;
- internal barrier

that is explicitly destructible/breachable.

The barrier's structure/armor determines required capability.

## 13. Breaching Unit

GDS-10 Breaching Unit can execute Breach Target only when:

- target is known and breachable;
- tool/payload is compatible;
- route is reachable;
- safety/authorization permits action.

## 14. Demolition Unit

A Demolition Unit can create destructive entry when the target accepts explosive demolition.

It is not required for all breaches.

## 15. Player Breaching

The player can use compatible:

- cutting tool;
- charge;
- weapon;
- hacking interface

where the target supports it.

No raid requires robots simply because robots exist, unless objective/access explicitly requires their capability.

## 16. Pressure State

An interior boundary can separate:

- Pressurized Safe;
- Pressurized Unsafe/Contaminated;
- Depressurized/Vacuum;
- Unknown.

Opening the boundary can equalize/vent atmosphere according to owning environment rules.

## 17. Breach Decompression

A hull breach into a pressurized compartment can cause:

- air loss;
- local movement/impulse hazards;
- alarms;
- emergency bulkhead closure;
- crew/defender evacuation;
- fire behavior change;
- suit requirement.

The station does not remain fully pressurized through a permanent open vacuum breach without an explicit containment field/system.

## 18. Pressure Safety for Player

If the player is not pressure-safe, preparation/live UI can warn against opening a known vacuum boundary.

The player cannot survive vacuum because the breach is mission-critical.

## 19. Pressure Safety for Robots

Robot environmental compatibility remains GDS-10 authority.

A robot that is not vacuum-compatible cannot traverse a depressurized breach route safely.

## 20. Airlock Use

Airlocks use actual cycling/interlock behavior where functional.

A hostile player can:

- cycle normally if authorized;
- hack controls;
- force/manual override;
- destroy/breach doors.

Destroying the airlock can remove safe pressure cycling.

## 21. Local Security Response

Opening an unauthorized entry can trigger:

- breach alarm;
- local lockdown;
- security robot dispatch;
- defender repositioning;
- interior door closure;
- reinforcement escalation.

Only functioning sensors/control paths can generate/share the appropriate response.

## 22. Entry Secured State

A boarding route can become **Secured Entry** when:

- player can traverse it;
- immediate access obstruction is resolved;
- route remains physically usable.

Secured does not mean permanently safe from counterattack.

## 23. Bridgehead

The player may establish a temporary **Bridgehead** around the entry.

A Bridgehead is a tactical condition, not a magical safe zone.

It can contain:

- player ship/airlock access;
- deployed robots;
- cleared local compartment;
- rally point;
- resupply/recovery route.

Enemies can counterattack it.

## 24. Robot Deployment Through Entry

A robot can traverse only if:

- size clearance is sufficient;
- mobility supports route;
- pressure/environment compatible;
- command connection/path valid.

## 25. Heavy Robot Constraint

Heavy Mechs frequently cannot use maintenance hatches/narrow crew airlocks.

They may be limited to:

- cargo docks;
- hangars;
- large breaches;
- exterior combat;
- major service corridors.

This is determined by physical geometry, not class exception teleporting.

## 26. Entry Resupply

If the player's ship remains connected and accessible, the Bridgehead can support physical transfer of:

- ammo;
- spare parts;
- reserve robots;
- loot;
- wounded/disabled assets.

Transfers require actual route/time/capacity.

## 27. Counter-Boarding

Defenders can attack the player's entry/ship if:

- route exists;
- they know it;
- combat/security AI permits it.

The player's docked ship is not automatically invulnerable during interior operations.

## 28. Boarding Ship Vulnerability

A docked ship can remain exposed to:

- external weapons;
- boarding counterattack;
- docking clamp sabotage;
- power/pressure link damage.

The player can choose safer stand-off/EVA methods at the cost of harder logistics.

## 29. Boarding Without Player Ship Attached

EVA/remote access is allowed where physically valid.

This can reduce ship risk but complicates:

- robot deployment;
- loot movement;
- extraction;
- oxygen/energy endurance.

## 30. Hacking an Entry

A Hacker Unit/player tool must access a valid local/network interface.

Successful entry hack grants only the relevant door/dock/airlock state.

It cannot skip physical traversal.

## 31. Silent Entry

A low-signature boarding attempt can avoid immediate alarm only if:

- sensors do not detect it;
- no alarmed access control event occurs;
- defenders do not observe it;
- the method itself is not inherently loud/alerting.

A violent explosive hull breach cannot be treated as silent by a stealth stat.

## 32. Entry Loss

A previously secured route can become unusable because of:

- defender lockdown;
- structural collapse;
- ship departure/destruction/disablement;
- shield reactivation;
- fire/hazard;
- pressure state;
- enemy control.

Extraction readiness updates immediately.

## 33. Alternate Entry

The player can switch to another known/discovered entry if the first fails.

No mission reset is required.

## 34. Entry Persistence

Committed state persists:

- breached wall/door;
- hacked local controller where hack persists;
- damaged airlock;
- pressure loss;
- secured docking state;
- destroyed defenders/sensors.

Reload does not restore the pre-breach barrier.

## 35. Objective Integration

A raid can define entry itself as:

- required objective;
- prerequisite;
- optional stealth bonus;
- alternate branch.

Objective completion only commits after the physical access condition is satisfied.

## 36. Edge Cases

If a docking port is destroyed while the player's ship is attached, GDS-6/target damage determines separation/damage; the ship is not silently teleported free.

If decompression closes an emergency bulkhead behind the player, the entry route can become blocked and alternate extraction may be required.

If a Heavy robot was deployed into a hangar but later lockdown closes all heavy-clearance routes, it can become isolated rather than shrinking through doors.

If a breach exposes nearby loot to vacuum, physical items can be displaced/lost if their object rules allow it.

## 37. Explicit Non-Goals

Boarding/breaching does not provide:

- automatic boarding after shield damage;
- universal wall destructibility;
- pressure-agnostic entry;
- Heavy robot path exceptions;
- global security shutdown from one door hack;
- invulnerable docked player ship;
- silent explosive breach.

## 38. Tuneable Parameters

Breach duration, charge yield, door resistance, decompression rates, alarm reaction delay, docking tolerance, and local defender response are tuneable.

## 39. Dependencies

This specification depends on External Attack, GDS-2 Atmosphere/Security, GDS-5 Field Survival/Interaction, GDS-6 Docking, GDS-8 Zones/Objectives, GDS-9 Combat, GDS-10 Tactical Commands/size, Interior Attack, and Escalation.

## 40. Open Questions

None in the boarding/breaching baseline.
