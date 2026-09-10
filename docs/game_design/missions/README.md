# Mission Design

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for structured expeditions, exploration, objectives, mission instances, extraction, rewards, and failure.

## Specifications

- [`mission_system.md`](mission_system.md) — **Draft**
- [`mission_types.md`](mission_types.md) — **Draft**
- [`mission_generation.md`](mission_generation.md) — **Draft**
- [`mission_zones.md`](mission_zones.md) — **Draft**
- [`objectives.md`](objectives.md) — **Draft**
- [`exploration.md`](exploration.md) — **Draft**
- [`difficulty_and_threat.md`](difficulty_and_threat.md) — **Draft**
- [`environmental_hazards.md`](environmental_hazards.md) — **Draft**
- [`extraction.md`](extraction.md) — **Draft**
- [`rewards.md`](rewards.md) — **Draft**
- [`failure_and_abandonment.md`](failure_and_abandonment.md) — **Draft**
- [`story_mission_execution.md`](story_mission_execution.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns:

- mission identity/lifecycle;
- mission instance state;
- mission archetypes;
- procedural mission generation;
- bounded mission zones;
- objective graphs;
- local exploration/discovery;
- mission threat representation;
- mission-level environmental hazard behavior;
- extraction transactions;
- mission-result rewards/claims;
- failure and abandonment;
- gameplay execution contract for GDS-7 main-story missions.

GDS-4 owns physical resource acquisition yields and Research/Blueprint knowledge.

GDS-5 owns player movement, survival, health, tools, and inventory.

GDS-6 owns spacecraft operation, travel, cargo, docking, and ship damage.

GDS-7 owns world geography, faction canon, and narrative story beats.

GDS-9 owns final combat mechanics.

GDS-10 owns robot mechanics.

GDS-11 owns full fortified-station raid mechanics.

GDS-12 owns economy, global difficulty, Dynamic Events, progression pacing, and final persistence/failure coordination.

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

Combat, Robots, and Raids are first-pass cross-validated against GDS-8.

GDS-12 has additionally resolved:

- Credit/reward economy boundary;
- global Difficulty interaction with mission Threat;
- Dynamic Event mission creation/pacing;
- Stable Save Boundary and anti-reroll persistence;
- failure/recovery coordination;
- long-term raid-target recovery context;
- exact MS-F01 finale capability/readiness thresholds.

## Completion State

GDS-8 is not yet Design Complete.

Its remaining scheduled downstream dependency is **GDS-13 Presentation, Onboarding, and Accessibility**, followed by the GDS-14 whole-project audit.
