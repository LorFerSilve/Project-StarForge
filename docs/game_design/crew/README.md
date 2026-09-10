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

This domain defines how persistent people interact with those systems and must not redefine their underlying rules.

## Downstream Resolution

Missions, Resources/Research, Spacecraft, Narrative/Factions, Player, Combat, Robots, and Raids have now been first-pass cross-validated against Crew.

GDS-12 additionally resolves:

- Simulation Time for work/training/healing/off-screen activity;
- final save/persistence semantics;
- crew progression pacing;
- economy interaction boundaries;
- failure/recovery coordination;
- finale support integration.

## Completion State

The Crew domain is **not yet Design Complete**.

Its remaining scheduled downstream dependency is **GDS-13 Presentation, Onboarding, and Accessibility**, followed by the GDS-14 whole-project audit.
