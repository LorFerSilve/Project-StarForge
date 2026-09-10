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
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

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

GDS-12 owns robot-facing economy, bounded TCC progression, difficulty constraints, persistence, replacement pacing, and finale readiness thresholds.

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

## Downstream Resolution

GDS-11 has resolved raid/defense integration.

GDS-12 has resolved:

- TCC progression;
- component/replacement economy context;
- robot attrition/recovery pacing;
- final save semantics;
- robot-related difficulty constraints;
- finale minimum TCC/squad requirements.

## Completion State

GDS-10 is not yet Design Complete.

Its remaining scheduled downstream dependency is **GDS-13 Presentation, Onboarding, and Accessibility**, followed by the GDS-14 whole-project audit.
