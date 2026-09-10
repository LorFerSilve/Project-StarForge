# Crew Design

> **Domain Status:** Design Complete — GDS-14 Promotion Passed

Authoritative design domain for survivors and persistent station crew.

## Specifications

- [`overview.md`](overview.md) — **Design Complete**
- [`survivor_encounters.md`](survivor_encounters.md) — **Design Complete**
- [`recruitment.md`](recruitment.md) — **Design Complete**
- [`professions.md`](professions.md) — **Design Complete**
- [`skills_and_traits.md`](skills_and_traits.md) — **Design Complete**
- [`assignments.md`](assignments.md) — **Design Complete**
- [`automation_responsibilities.md`](automation_responsibilities.md) — **Design Complete**
- [`crew_needs.md`](crew_needs.md) — **Design Complete**
- [`experience_and_progression.md`](experience_and_progression.md) — **Design Complete**
- [`injuries_incapacitation_and_death.md`](injuries_incapacitation_and_death.md) — **Design Complete**
- [`crew_presence_and_behavior.md`](crew_presence_and_behavior.md) — **Design Complete**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Domain Baseline

The crew domain establishes:

- survivors are encountered physically and are not abstract recruit cards;
- rescue and recruitment are separate states;
- recruited crew is persistent;
- crew quality comes from profession, skills, traits, background, and Special Knowledge rather than loot rarity;
- crew has one Primary Assignment at a time;
- specialist supervision has finite Workload Capacity;
- crew primarily enables automation, supervision, diagnosis, and exception handling;
- ordinary crew needs are handled at station scale rather than individual life-sim micromanagement;
- skills progress from meaningful work and training;
- routine gameplay does not permanently kill recruited crew;
- crew physically exists and navigates through the station when locally simulated;
- off-screen crew simulation preserves equivalent gameplay causality.

## Authority Boundary

Station infrastructure is authoritative under `../station/`.
Mission rescue context belongs under `../missions/`.
Combat rules belong under `../combat/`.
Robot behavior belongs under `../robots/`.
Narrative character arcs belong under `../narrative/`.
GDS-12 owns cross-cutting time, persistence, economy, Difficulty, and campaign-progression coordination.
GDS-13 owns crew-facing UI, local visual presence, feedback, alarms, onboarding, subtitles/captions, and accessibility.

This domain defines how persistent people interact with those systems and must not redefine their underlying rules.

## Downstream Resolution

Missions, Resources/Research, Spacecraft, Narrative/Factions, Player, Combat, Robots, Raids, and GDS-12 cross-cutting systems are first-pass cross-validated against Crew.

GDS-13 now additionally resolves:

- crew roster/identity/Profession/Skill/Assignment presentation;
- health/incapacitation and availability feedback;
- visible local habitation/work presence;
- automation-response presentation;
- survivor/recruitment onboarding;
- color-independent and scalable crew UI;
- subtitles/captions for crew dialogue where applicable.

## Completion State

The Crew domain is **not yet Design Complete**.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The GDS-14 Design Complete promotion gate has passed.


## GDS-14 Promotion State

All **11** authoritative owning specifications in this domain passed the GDS-14 maturity gate and are **Design Complete**. Historical `CROSS_VALIDATION.md` material remains audit evidence and does not override current owning specifications.
