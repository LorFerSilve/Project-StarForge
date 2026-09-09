# Robot System Overview

> **Status:** Draft  
> **Authority:** Global robot identity, physical existence, ownership, role categories, deployment philosophy, persistence, destruction boundary, energy, and cross-domain authority

## 1. Purpose

Robots are the player's repeatable manufactured workforce and tactical force.

They connect:

- station automation;
- logistics;
- repair;
- security;
- exploration;
- missions;
- combat;
- raids.

## 2. Core Design Principle

Robots are **physical manufactured units**.

They are not:

- abstract troop cards;
- menu-only bonuses;
- instantly summoned effects;
- human crew replacements in every role.

Each persistent player-owned robot has a unique Robot ID and physical state.

## 3. Robot Ownership

A robot can be owned by:

- Player/Home Station;
- faction/NPC actor;
- mission/world entity.

Player-owned robots persist until:

- dismantled;
- transferred;
- destroyed;
- otherwise removed through an explicit authored transaction.

## 4. Robot Role Categories

Baseline player robot roles fall into three groups:

### Station Robots

Perform routine physical work inside/around Horizon Station.

### Field Support Robots

Assist missions with repair, scanning, hacking, logistics, rescue, and other utility.

### Tactical Combat Robots

Form the player's repeatable robotic assault/defense squad.

One robot class can overlap categories if its configuration supports both.

## 5. Humans vs Robots

Human crew provide:

- judgment;
- professional supervision;
- research/medical/social/narrative identity;
- high-level policy.

Robots provide:

- repeatable physical labor;
- hazardous-environment work;
- tactical expendability;
- automation throughput;
- direct mechanical capability.

Robots do not make Crew professions obsolete.

## 6. Human Crew Permanence Boundary

Recruited human crew are protected from routine permanent death under GDS-3.

Robots are not.

A player-owned robot can be permanently Destroyed through ordinary combat/system damage.

This is intentional.

## 7. Robot Persistence

A persistent Robot ID stores at minimum:

- class/chassis;
- installed components;
- software/behavior package;
- condition/integrity;
- subsystem state;
- energy state;
- ammunition/loadout where applicable;
- assignment;
- squad membership;
- current physical location;
- experience/calibration data if the model supports it.

## 8. No Free Replacement

Destroyed robots are not automatically restored after:

- mission failure;
- raid failure;
- station defense;
- save/load.

Replacement requires actual manufacturing/acquisition.

## 9. Robot Wrecks

A destroyed robot can leave a physical wreck if the location/state permits.

A wreck can be:

- recovered;
- salvaged;
- abandoned;
- destroyed further.

Salvage follows GDS-4 and never returns more material than physically valid.

## 10. Disabled vs Destroyed

Robot combat states distinguish:

- Operational;
- Degraded;
- Disabled;
- Destroyed.

Disabled is recoverable if:

- physical chassis remains;
- required parts/tools exist;
- access is possible.

Destroyed means the original Robot ID is no longer repairable as a functioning unit.

## 11. Robot Energy

Robots use finite **Internal Energy Reserve**.

This is a device charge state, not a new crafting currency.

Energy is replenished from actual electrical infrastructure such as:

- station charging dock;
- ship robot rack;
- field power interface;
- compatible portable power source.

Robots do not regenerate electrical energy from nothing.

## 12. Energy Exhaustion

At zero usable energy:

- powered movement stops;
- active tools/weapons stop;
- passive stored data/state remains;
- robot enters Power-Depleted/Disabled state.

A robot can be recharged if recovered and compatible power is available.

## 13. Robot Maintenance

Operational robots require repair/maintenance when damaged.

Routine maintenance can consume:

- actual station power;
- replacement parts;
- components/materials;
- Robotics Specialist/automation capacity where required.

No generic magical Robot Repair resource is introduced.

## 14. Robot Intelligence

Player robots use bounded task/tactical AI.

They can:

- navigate;
- perceive;
- execute commands;
- react to threats;
- perform allowed tasks;
- preserve themselves within policy.

They do not independently choose:

- missions;
- raids;
- diplomacy;
- research direction;
- major construction;
- protected strategic spending.

## 15. Direct Player Participation

Robots support the player rather than replacing core action gameplay.

The player still personally participates in:

- exploration;
- combat;
- boarding;
- mission objectives;
- raid decisions.

## 16. Field Deployment

A robot deployed to an external mission must physically arrive through a valid transport/deployment method.

Baseline methods:

- transported aboard player ship;
- deployed from a ship robot rack/bay;
- already present at persistent world location;
- authored mission transport.

No baseline orbital teleport/summon exists.

## 17. Transport Capacity

Field robot deployment is limited by actual transport support.

A ship can define:

- Robot Rack Slots;
- Heavy Robot Bay capability;
- charging/support capacity.

A robot cannot be deployed if no compatible transport/deployment capacity exists.

## 18. Tactical Control Capacity

The player has finite **Tactical Control Capacity (TCC)**.

Each tactical robot class has a Command Load.

The active commanded squad must satisfy:

Total Command Load <= Available Tactical Control Capacity.

TCC is a command-system capability, not a currency/resource.

## 19. TCC Sources

Available Tactical Control Capacity can come from:

- player command equipment;
- ship command/sensor module;
- station defense control while defending Horizon Station;
- later research/upgrades.

Exact values are tuneable progression data.

## 20. Transport and TCC Are Separate

A player can have:

- enough ship space but insufficient command capacity;
- enough command capacity but insufficient transport space.

Both constraints must be satisfied for deployment.

## 21. No Arbitrary Account Army Cap

The player may own more robots than can be deployed simultaneously.

Limits apply to:

- storage;
- maintenance;
- charging;
- transport;
- command capacity;
- mission/raid deployment.

## 22. Station Assignment

Station robots can be assigned to:

- automation pools;
- station zones;
- specific responsibilities;
- standby/reserve;
- security.

Station Automation remains authoritative for task scheduling.

## 23. Field Assignment

Field robots join a Tactical Squad or Support Detachment.

Their behavior is controlled by:

- squad role;
- tactical commands;
- Rules of Engagement;
- autonomy fallback.

## 24. Robot Experience

The baseline does not require a universal Robot Level.

A persistent robot can accumulate:

- calibration;
- service history;
- model-specific proficiency

only where a later component/class explicitly uses it.

Primary progression comes from:

- chassis;
- components;
- software;
- research;
- manufacturing quality/design.

## 25. No Loot Rarity

Robots do not use Common/Rare/Epic/Legendary tiers.

Value comes from actual configuration and technology.

## 26. Enemy Robots

Enemy machines can use GDS-9 generic machine combat behavior.

GDS-10 player robot rules do not imply every enemy machine is recruitable/manufacturable.

## 27. Continuance Boundary

The Continuance is a faction/machine network under GDS-7.

Continuance bodies can reuse robot combat/system principles where appropriate, but they are not automatically player robot classes.

## 28. Station Defense

Player security robots can defend Horizon Station.

GDS-11 owns station-defense orchestration.

GDS-10 owns the physical robot bodies and commands.

## 29. Mission Failure

Mission failure does not automatically restore destroyed robots.

Robot outcome follows actual physical state:

- extracted operational robot persists;
- extracted disabled robot can be repaired;
- destroyed robot remains destroyed;
- abandoned disabled robot can be lost or recoverable according to mission/world state.

## 30. Save/Load

Robot identity, physical location, condition, energy, components, squad/assignment, and destruction state persist.

Reloading cannot recreate a destroyed Robot ID after the destruction transaction committed.

## 31. Explicit Non-Goals

Robots are not:

- abstract troop cards;
- human crew rarity replacements;
- infinitely summonable;
- energy-free;
- automatically resurrected;
- a universal strategic AI;
- governed by a generic Robot Level.

## 32. Tuneable Parameters

Energy capacity, Command Load, transport footprint, maintenance cost, movement, sensors, armor, weapon capacity, and charging time are tuneable.

## 33. Dependencies

This specification depends on GDS-2 Station Automation, GDS-3 Crew, GDS-4 Resources/Crafting, GDS-6 Spacecraft, GDS-8 Missions, GDS-9 Combat, and future Raids/Progression/Presentation.

## 34. Open Questions

None in the global robot-system baseline.
