# Robot Design

> **Domain Status:** Design Complete — GDS-14 Promotion Passed

Authoritative design domain for manufactured robotic units.

## Specifications

- [`overview.md`](overview.md) — **Design Complete**
- [`manufacturing.md`](manufacturing.md) — **Design Complete**
- [`robot_classes.md`](robot_classes.md) — **Design Complete**
- [`robot_components_and_upgrades.md`](robot_components_and_upgrades.md) — **Design Complete**
- [`robot_ai.md`](robot_ai.md) — **Design Complete**
- [`squad_composition.md`](squad_composition.md) — **Design Complete**
- [`tactical_commands.md`](tactical_commands.md) — **Design Complete**
- [`damage_repair_and_loss.md`](damage_repair_and_loss.md) — **Design Complete**
- [`station_robots.md`](station_robots.md) — **Design Complete**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns persistent Robot IDs, manufacturing/commissioning, classes/components, Internal Energy Reserve, bounded player-robot AI, Tactical Control Capacity/Command Load, squads/deployment, tactical commands/ROE, robot damage/repair/recovery/loss, and station robot roles.

GDS-2 owns station task scheduling/infrastructure. GDS-3 owns human crew. GDS-4 owns Resources/Crafting/Research. GDS-6 owns spacecraft racks/bays. GDS-8 owns mission deployment/extraction/failure. GDS-9 owns generic combat math. GDS-11 owns raid/defense orchestration. GDS-12 owns economy/TCC progression/Difficulty/persistence/finale thresholds. GDS-13 owns robot/squad UI, feedback, visual class language, audio/VFX, onboarding, and accessibility.

## Core Baseline

GDS-10 establishes physical manufactured persistent robots; routine permanent robot destruction unlike human crew; eight canonical field classes; Light/Medium/Heavy sizes; finite TCC; separate transport/command constraints; Robot Rack/Heavy Bay requirements; finite energy/ammunition; component progression rather than rarity/levels; non-omniscient AI; ROE/commands; Disabled vs Destroyed; repair/towing/wreck/Lost state; and physical station automation roles.

## Downstream Resolution

GDS-11 and GDS-12 are first-pass resolved.

GDS-13 now resolves:

- distinct class silhouettes/tool profiles;
- Robot ID/Class/condition/energy/ammo/assignment presentation;
- simultaneous TCC and transport-capacity UX;
- command-state/communications/fallback feedback;
- Disabled/Destroyed/Lost readability;
- permanent-destruction onboarding before meaningful risk;
- robot audio/VFX tied to actual tools, energy and damage;
- accessible command/target presentation without hidden-information leakage.

## Completion State

GDS-10 is Design Complete after the GDS-14 promotion sweep.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The GDS-14 Design Complete promotion gate has passed.


## GDS-14 Promotion State

All **9** authoritative owning specifications in this domain passed the GDS-14 maturity gate and are **Design Complete**. Historical `CROSS_VALIDATION.md` material remains audit evidence and does not override current owning specifications.
