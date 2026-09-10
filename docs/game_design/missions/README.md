# Mission Design

> **Domain Status:** Design Complete — GDS-14 Promotion Passed

Authoritative design domain for structured expeditions, exploration, objectives, mission instances, extraction, rewards, and failure.

## Specifications

- [`mission_system.md`](mission_system.md) — **Design Complete**
- [`mission_types.md`](mission_types.md) — **Design Complete**
- [`mission_generation.md`](mission_generation.md) — **Design Complete**
- [`mission_zones.md`](mission_zones.md) — **Design Complete**
- [`objectives.md`](objectives.md) — **Design Complete**
- [`exploration.md`](exploration.md) — **Design Complete**
- [`difficulty_and_threat.md`](difficulty_and_threat.md) — **Design Complete**
- [`environmental_hazards.md`](environmental_hazards.md) — **Design Complete**
- [`extraction.md`](extraction.md) — **Design Complete**
- [`rewards.md`](rewards.md) — **Design Complete**
- [`failure_and_abandonment.md`](failure_and_abandonment.md) — **Design Complete**
- [`story_mission_execution.md`](story_mission_execution.md) — **Design Complete**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns mission identity/lifecycle, mission instances, archetypes, generation, zones, objective graphs, exploration/discovery, Threat, mission hazards, extraction, rewards/claims, failure/abandonment, and gameplay execution for GDS-7 main-story Mission IDs.

GDS-4 owns Resources/Research/Blueprint knowledge. GDS-5 owns player movement/survival/inventory. GDS-6 owns spacecraft/travel/cargo. GDS-7 owns geography/narrative/factions. GDS-9 owns combat. GDS-10 owns robots. GDS-11 owns fortified raids. GDS-12 owns economy, global Difficulty, Dynamic Events, progression pacing and final persistence/failure coordination. GDS-13 owns mission-facing presentation/onboarding/accessibility.

## Core Baseline

GDS-8 establishes:

- multiple Accepted missions but only one Deployed Mission Instance;
- persistent Mission IDs and deterministic procedural seeds;
- bounded Surface, Interior, Local Spaceflight, EVA, and Mixed zones;
- objective DAGs with Primary/Secondary/Optional/Extraction states;
- exploration states from Unknown through Resolved;
- four-axis 0–5 mission Threat;
- explicit hazard states/patterns;
- physical extraction rather than pickup-to-bank;
- deterministic Field-Unsecured → Vehicle/Extraction-Secured transitions;
- Station-Secured only through actual station ownership;
- resolution rewards separated from field loot;
- deterministic failure/abandonment transactions;
- fixed execution contracts for all canonical main-story missions.

## Downstream Resolution

Combat, Robots, Raids, and GDS-12 cross-cutting systems are first-pass cross-validated against GDS-8.

GDS-13 now resolves:

- Mission Log/lifecycle presentation;
- exact/search-area/directional/unknown objective guidance;
- four-axis Threat display;
- hazard signaling;
- extraction and ownership-state feedback;
- causal failure/reward summaries;
- Dynamic Event notification/mission transition presentation;
- campaign-integrated mission/extraction tutorials;
- accessibility guidance that cannot reveal hidden mission information.

## Completion State

GDS-8 is Design Complete after the GDS-14 promotion sweep.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The GDS-14 Design Complete promotion gate has passed.


## GDS-14 Promotion State

All **12** authoritative owning specifications in this domain passed the GDS-14 maturity gate and are **Design Complete**. Historical `CROSS_VALIDATION.md` material remains audit evidence and does not override current owning specifications.
