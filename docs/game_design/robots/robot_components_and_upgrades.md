# Robot Components and Upgrades

> **Status:** Draft  
> **Authority:** Robot component architecture, slot categories, compatibility, power/energy draw, software packages, installation/removal, condition, and upgrade philosophy

## 1. Purpose

Robots progress through explicit engineering configuration rather than generic levels or rarity.

## 2. Robot Configuration

A Robot Configuration consists of:

- Chassis;
- Mobility System;
- Control Core;
- Power System / Internal Energy Reserve;
- Sensor Package;
- Armor/Protection;
- Weapon Mount(s) where supported;
- Utility Module(s);
- Communication Module;
- Software Package(s).

Not every chassis exposes every slot separately.

## 3. Chassis Authority

The Chassis defines:

- size class;
- mass envelope;
- base structural integrity;
- component slot layout;
- movement support;
- transport footprint;
- baseline Command Load;
- hardpoint limits.

## 4. Mobility Systems

Possible mobility architectures:

- Wheeled;
- Tracked;
- Legged;
- Hover/Thruster Drone;
- EVA Thruster;
- Fixed/Emplaced.

A chassis defines compatible mobility options.

## 5. Control Core

Required for autonomous player-robot operation.

Defines:

- baseline processing capability;
- software compatibility;
- command interface;
- autonomy ceiling.

A destroyed/disabled Control Core can disable the unit.

## 6. Power System

Defines:

- Internal Energy Reserve capacity;
- peak discharge capability;
- recharge compatibility;
- continuous system support.

A robot does not operate active systems without adequate power.

## 7. Sensors

Sensor modules can provide:

- visual targeting;
- low-light;
- thermal;
- rangefinding;
- environment scan;
- electronic detection.

Sensors reveal only supported information.

## 8. Armor

Robot armor uses GDS-9 channel protection/penetration principles.

Armor can differ by chassis zone.

## 9. Weapon Mounts

Weapon Mounts define:

- compatible weapon size/type;
- firing arc;
- ammunition/feed compatibility;
- power/heat support.

A robot cannot equip a weapon solely because inventory contains it.

## 10. Utility Modules

Examples:

- Repair Tool;
- Hacking Interface;
- Shield Projector;
- Breaching Tool;
- Cargo Manipulator;
- Medical Stabilizer;
- Mining Tool;
- Demolition Charge Carrier.

Each provides one explicit capability.

## 11. Communication Module

Provides:

- command reception;
- squad data;
- shared perception where allowed;
- telemetry.

Communication range/reliability depends on actual module/environment.

## 12. Software Packages

Software can configure behavior such as:

- Assault;
- Guardian;
- Repair;
- Recon;
- Logistics;
- Breach;
- Security.

Software changes allowed behavior/priorities, not physical impossible capabilities.

## 13. No Software Magic

Installing Repair software on a robot without a repair tool does not let it repair.

Installing Combat software on a chassis without a weapon does not create one.

## 14. Slot Compatibility

Each component declares:

- compatible chassis/slot;
- mass;
- power requirement;
- thermal requirement where relevant;
- control requirement;
- physical size.

Invalid combinations cannot be installed.

## 15. Mass Budget

Installed component mass must remain within chassis limits.

Mass can affect:

- mobility;
- transport footprint;
- energy endurance;
- acceleration.

Exact formulas are tuneable by chassis.

## 16. Power Budget

Peak and continuous component demand cannot exceed power-system capability without explicit overload behavior.

## 17. Thermal Budget

High-output weapons/tools can create heat.

A robot configuration must provide required heat rejection where the component definition requires it.

## 18. Energy Reserve

Internal Energy Reserve is a finite charge state.

Active consumers include:

- locomotion;
- sensors;
- weapons/tools where powered;
- shield;
- communication;
- computing.

Low-energy behavior follows AI fallback policy.

## 19. Ammunition

Physical-ammunition weapons carry finite robot ammunition.

Ammo storage/feed is part of the robot configuration.

No infinite combat ammo exists.

## 20. Shield Module

A compatible robot can mount a shield generator.

It uses GDS-9 shield rules and robot power.

It does not protect the entire squad unless its coverage geometry explicitly does so.

## 21. Component Condition

Persistent components have condition/integrity where relevant.

Damage can disable an individual component while chassis survives.

## 22. Installation

Component installation/removal requires:

- valid service facility or field-service capability;
- access;
- compatible tools;
- active time;
- safe enough state.

Heavy chassis components normally require station/ship service equipment.

## 23. Field Replacement

Small field-serviceable components can be replaced by a Repair-capable actor where model allows it.

Field replacement physically consumes a spare component.

## 24. Upgrades

An upgrade means installing:

- improved component;
- new compatible technology;
- alternate software;
- new weapon/tool.

There is no generic "+1 Robot Upgrade Level."

## 25. Sidegrades

Many upgrades should create tradeoffs.

Examples:

Heavy Armor:
- more protection;
- more mass;
- more energy/mobility burden.

Long-Range Sensors:
- better detection;
- higher power use;
- higher cost.

Shield Projector:
- rechargeable protection;
- high energy draw;
- reduced endurance.

## 26. Research / Blueprint

Advanced components require appropriate GDS-4 knowledge.

A researched technology does not create a physical component.

## 27. Faction Technology

Faction technology can produce variant components with different engineering tradeoffs.

No incompatible parallel robot resource system is required.

## 28. Software Security

Captured software/control systems can require analysis before installation.

Unknown hostile control code is not automatically trusted by Horizon Station.

## 29. Configuration Validation

Before commissioning/deployment, configuration validates:

- mandatory control;
- locomotion;
- power;
- compatible slots;
- mass;
- required cooling;
- ammunition/support;
- command interface.

## 30. Disabled Configuration

A robot can remain physically assembled but nondeployable because configuration is invalid/damaged.

The system reports exact blockers.

## 31. Persistence

Installed components, software, ammo, energy, condition, and configuration persist.

## 32. Explicit Non-Goals

Robot upgrades do not use:

- generic level points;
- random rarity;
- software granting absent hardware;
- infinite energy;
- infinite ammo;
- universal component compatibility.

## 33. Tuneable Parameters

Mass, power, energy capacity, protection, slot counts, heat, Command Load modifiers, ammo capacity, and component effects are tuneable.

## 34. Dependencies

This specification depends on Robot Classes, GDS-4 Resources/Blueprints, GDS-9 Combat, GDS-6 transport/service, Repair, AI, and Progression.

## 35. Open Questions

None in the component/configuration baseline.
