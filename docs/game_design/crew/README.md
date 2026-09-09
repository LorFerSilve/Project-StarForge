# Crew Design

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for survivors and persistent station crew.

## Specifications

- [`overview.md`](overview.md) — **Draft**
- [`survivor_encounters.md`](survivor_encounters.md) — **Draft**
- [`recruitment.md`](recruitment.md) — **Draft**
- [`professions.md`](professions.md) — **Draft**
- [`skills_and_traits.md`](skills_and_traits.md) — **Draft**
- [`assignments.md`](assignments.md) — **Draft**
- [`automation_responsibilities.md`](automation_responsibilities.md) — **Draft**
- [`crew_needs.md`](crew_needs.md) — **Draft**
- [`experience_and_progression.md`](experience_and_progression.md) — **Draft**
- [`injuries_incapacitation_and_death.md`](injuries_incapacitation_and_death.md) — **Draft**
- [`crew_presence_and_behavior.md`](crew_presence_and_behavior.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — first-pass consistency audit

## Domain Baseline

The crew domain currently establishes these high-level rules:

- survivors are encountered in the world and are not abstract recruit cards;
- rescue and recruitment are separate states;
- recruited crew is persistent;
- crew quality comes from profession, skills, traits, background, and special knowledge rather than loot rarity tiers;
- crew has one Primary Assignment at a time;
- specialist supervision has finite workload capacity;
- crew primarily enables automation, supervision, diagnosis, and exception handling;
- ordinary crew needs are handled at station scale rather than individual survival micromanagement;
- skills progress from real work and training;
- routine gameplay does not permanently kill recruited crew;
- crew physically exists and navigates through the station when locally simulated;
- off-screen crew simulation must preserve equivalent gameplay outcomes.

## Authority Boundary

Station infrastructure rules are authoritative under `../station/`.

Mission rescue context belongs under `../missions/`.

Combat rules belong under `../combat/`.

Robot behavior belongs under `../robots/`.

Narrative character arcs belong under `../narrative/`.

This domain defines how persistent people interact with those systems and must not redefine their underlying rules.

## Completion State

The crew domain is **not yet Design Complete**.

Its own first-pass rules are present, but several specifications depend on future authoritative domains such as missions, resources, combat, robots, narrative, player movement, and time/persistence.

Those dependencies are explicitly tracked in [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md).
