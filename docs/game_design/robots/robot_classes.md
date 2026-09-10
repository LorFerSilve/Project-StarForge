# Robot Classes

> **Status:** Design Complete  
> **Authority:** Canonical baseline player robot classes, their primary roles, size/transport class, baseline Command Load, required capabilities, and role boundaries

## 1. Purpose

Robot Classes provide readable tactical and utility roles without turning every robot into a generic chassis with arbitrary stat differences.

## 2. Class Philosophy

A Robot Class defines:

- intended role;
- chassis envelope;
- baseline Command Load;
- transport footprint;
- mandatory capability;
- typical component profile;
- tactical AI priorities.

A class is not a rarity tier.

## 3. Canonical Field Classes

The baseline field roster contains eight classes:

1. Recon Drone;
2. Repair Drone;
3. Hacker Unit;
4. Assault Unit;
5. Shield Unit;
6. Breaching Unit;
7. Demolition Unit;
8. Heavy Mech.

## 4. Size Classes

Baseline robot size classes:

- Light;
- Medium;
- Heavy.

Size affects:

- transport;
- physical pathing;
- service equipment;
- mass envelope;
- cover use;
- docking/rack compatibility.

## 5. Recon Drone

### Size

Light.

### Baseline Command Load

1.

### Primary Role

- scouting;
- threat detection;
- route observation;
- target marking;
- sensor extension.

### Mandatory Capabilities

- mobile sensor platform;
- command communication;
- local navigation.

### Typical Configuration

- hover/thruster or agile legged chassis;
- long-range sensors;
- low armor;
- light/self-defense weapon optional.

### Tactical Strength

Information acquisition with low command burden.

### Weakness

Low durability and limited direct combat power.

## 6. Repair Drone

### Size

Light.

### Baseline Command Load

1.

### Primary Role

- field repair;
- emergency stabilization;
- limited component service;
- support of robots/mission equipment.

### Mandatory Capabilities

- Repair Tool;
- diagnostic sensors;
- manipulator;
- command communication.

### Combat Role

Avoid direct combat by default.

Can defend itself only if configured with compatible weapon capability.

### Weakness

Low armor and limited offensive output.

## 7. Hacker Unit

### Size

Light.

### Baseline Command Load

1.

### Primary Role

- electronic access;
- local network interaction;
- security-system disruption;
- authorized mission hacking;
- sensor/electronic support.

### Mandatory Capabilities

- Hacking Interface;
- electronic sensor package;
- secure control core.

### Important Boundary

The Hacker Unit cannot bypass a system that has no physically/logically valid hacking path.

It cannot create credentials from nothing.

### Weakness

Low direct combat durability.

## 8. Assault Unit

### Size

Medium.

### Baseline Command Load

2.

### Primary Role

General-purpose direct combat.

### Mandatory Capabilities

- combat-capable locomotion;
- weapon mount;
- combat sensors;
- armor;
- command module.

### Typical Weapons

- ballistic rifle equivalent;
- laser/plasma equivalent;
- compact heavy weapon where chassis supports it.

### Strength

Balanced mobility, protection, and offense.

### Weakness

Less specialized than other units.

## 9. Shield Unit

### Size

Medium.

### Baseline Command Load

2.

### Primary Role

- protect squad movement;
- defend player/robots;
- hold choke points.

### Mandatory Capabilities

- shield projector;
- sufficient power/energy;
- defensive control software.

### Shield Geometry

Shield Unit protects only its authored projection geometry.

It does not grant global squad immunity.

### Combat Capability

Can carry a secondary/limited weapon where configuration permits.

### Weakness

High energy consumption; reduced endurance while projecting.

## 10. Breaching Unit

### Size

Medium.

### Baseline Command Load

2.

### Primary Role

- forced entry;
- armored-door breach;
- obstacle removal;
- close-quarters assault support.

### Mandatory Capabilities

At least one:
- powered breaching tool;
- cutting tool;
- controlled breaching charge interface.

### Important Boundary

Breaching applies only to destructible/authorized breach targets.

It does not imply unrestricted wall destruction.

### Weakness

Shorter optimal combat range and specialized loadout.

## 11. Demolition Unit

### Size

Medium.

### Baseline Command Load

2.

### Primary Role

- place controlled explosives;
- destroy specified structural/system targets;
- anti-fortification work;
- mission sabotage.

### Mandatory Capabilities

- demolition payload carrier;
- charge placement/manipulation;
- blast-safety logic.

### Difference From Breacher

Breacher opens access.

Demolition Unit destroys designated targets/fortifications.

Some component overlap can exist, but mission AI priorities differ.

### Weakness

Limited ammunition/payload and collateral-risk management.

## 12. Heavy Mech

### Size

Heavy.

### Baseline Command Load

4.

### Primary Role

- heavy direct combat;
- fire support;
- tanking/area control;
- anti-armor targets.

### Mandatory Capabilities

- heavy chassis;
- heavy armor;
- high-output power system;
- heavy weapon hardpoint(s);
- combat sensors.

### Strength

Highest direct durability/firepower in baseline field roster.

### Weaknesses

- high Command Load;
- large transport footprint;
- high power/ammunition demand;
- restricted pathing;
- expensive repair/replacement.

## 13. Command Load Table

Baseline:

| Class | Size | Command Load |
| --- | --- | ---: |
| Recon Drone | Light | 1 |
| Repair Drone | Light | 1 |
| Hacker Unit | Light | 1 |
| Assault Unit | Medium | 2 |
| Shield Unit | Medium | 2 |
| Breaching Unit | Medium | 2 |
| Demolition Unit | Medium | 2 |
| Heavy Mech | Heavy | 4 |

These are baseline balance values under the fixed TCC model.

## 14. Transport Footprint

Baseline footprint categories:

- Light = 1 Robot Rack Unit;
- Medium = 2 Robot Rack Units;
- Heavy = requires Heavy Robot Bay and 4 Rack-Unit equivalent support.

A Heavy robot cannot be split across ordinary racks.

## 15. Class vs Configuration

Class defines required role capability.

Optional components can change performance.

Example:

An Assault Unit can mount:
- kinetic rifle;
- laser;
- EMP weapon.

It remains an Assault Unit because its primary role/chassis doctrine remains general combat.

## 16. Invalid Role Drift

If a configuration removes a class's mandatory capability, the robot becomes **Configuration Invalid for Class Deployment** until corrected/reclassified through a compatible service process.

Example:
- Shield Unit without operational shield projector cannot deploy as Shield Unit.

## 17. Multi-Role Configuration

A robot may gain secondary capability if slots allow it.

Example:
- Assault Unit + small repair manipulator.

Secondary capability does not erase primary class Command Load/role.

## 18. Class Change

Changing class requires:

- compatible chassis;
- mandatory new components;
- valid software;
- service facility;
- configuration validation.

Some chassis cannot convert between classes.

Heavy Mech cannot become a Light Recon Drone.

## 19. Class Research

Advanced classes can be locked behind Technology Research/Blueprints.

The baseline does not require all eight to be available at game start.

Exact progression order belongs to GDS-12.

## 20. Field Environment Compatibility

A class can require environmental capability:

- vacuum;
- radiation;
- atmosphere;
- gravity;
- terrain.

Class name alone does not imply universal environmental survival.

## 21. Human Crew Boundary

No robot class replaces:

- Scientist;
- Medic as full medical authority;
- Navigator strategic judgment;
- Intelligence Officer strategic analysis;
- Robotics Specialist supervision

unless a specific automation capability explicitly supports a bounded task.

## 22. Mission Objective Compatibility

Typical best-fit:

- Recon → Survey/Recon;
- Repair → Restoration/Defense support;
- Hacker → Recovery/Boarding/Access;
- Assault → Elimination/Defense;
- Shield → Defense/Escort;
- Breacher → Boarding/Access;
- Demolition → Sabotage/Destroy;
- Heavy → High-threat Combat/Defense.

These are preferences, not mission exclusivity.

## 23. Explicit Non-Goals

Classes do not use:

- rarity tiers;
- universal class leveling;
- every class carrying every tool;
- Heavy units in light racks;
- software-only role magic;
- unlimited environmental compatibility.

## 24. Tuneable Parameters

Command Load, footprint, chassis integrity, energy, speed, armor, weapon hardpoints, sensor range, and class-specific output are tuneable.

## 25. Dependencies

This specification depends on Components, Manufacturing, Squad Composition, GDS-9 Combat, GDS-6 Transport, GDS-8 Missions, and Station Robots.

## 26. Open Questions

None in the canonical field-class baseline.
