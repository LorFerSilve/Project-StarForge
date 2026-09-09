# Robot Design

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for manufactured robotic units.

## Specifications

- [`overview.md`](overview.md) — **Draft**
- [`manufacturing.md`](manufacturing.md) — **Draft**
- [`robot_classes.md`](robot_classes.md) — **Draft**
- [`robot_components_and_upgrades.md`](robot_components_and_upgrades.md) — **Draft**
- [`robot_ai.md`](robot_ai.md) — **Draft**
- [`squad_composition.md`](squad_composition.md) — **Draft**
- [`tactical_commands.md`](tactical_commands.md) — **Draft**
- [`damage_repair_and_loss.md`](damage_repair_and_loss.md) — **Draft**
- [`station_robots.md`](station_robots.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — first-pass consistency audit

## Authority Boundary

This domain owns:

- persistent player-owned Robot IDs;
- robot manufacturing/commissioning;
- canonical robot classes;
- robot component architecture;
- Internal Energy Reserve;
- player robot AI;
- Tactical Control Capacity and Command Load;
- squad composition/deployment;
- tactical commands and ROE;
- robot-specific damage/subsystem consequences;
- repair/recovery/loss;
- station robot roles and physical automation behavior.

GDS-2 owns station task scheduling, manufacturing infrastructure, power, logistics, and security orchestration.

GDS-3 owns human crew professions/supervision.

GDS-4 owns physical resources, crafting, Blueprint, Research, and salvage.

GDS-6 owns spacecraft robot racks/bays and transport/service capability.

GDS-8 owns mission deployment/extraction/failure.

GDS-9 owns generic hit/damage/weapon/shield/armor/status combat mathematics.

GDS-11 owns raid and station-defense orchestration.

## Core Baseline

GDS-10 establishes:

- robots as physical manufactured persistent units rather than troop cards;
- routine permanent robot destruction, unlike human crew;
- eight canonical field classes;
- Light/Medium/Heavy robot sizes;
- finite Tactical Control Capacity;
- separate transport and command constraints;
- physical Robot Rack Units and Heavy Robot Bay requirements;
- finite robot energy and ammunition;
- component-based progression rather than generic robot levels/rarity;
- bounded non-omniscient robot AI;
- explicit ROE and tactical command state machine;
- Disabled versus Destroyed semantics;
- field repair, towing, wreck recovery, and Lost state;
- station Maintenance, Logistics, Agriculture, Security, and Utility robot roles;
- physical station automation without teleporting work/resources.

## Completion State

GDS-10 is not yet Design Complete.

Raids/Station Defense, Economy/Progression/Difficulty/Persistence, and Presentation must still cross-validate robot-facing behavior.
