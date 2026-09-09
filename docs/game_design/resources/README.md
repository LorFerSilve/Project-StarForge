# Resources, Crafting & Research

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for physical resources, acquisition, processing, crafting, blueprints, research, and production dependencies.

## Specifications

- [`resource_model.md`](resource_model.md) — **Draft**
- [`resource_catalog.md`](resource_catalog.md) — **Draft**
- [`gathering_and_salvage.md`](gathering_and_salvage.md) — **Draft**
- [`mining.md`](mining.md) — **Draft**
- [`processing.md`](processing.md) — **Draft**
- [`crafting.md`](crafting.md) — **Draft**
- [`blueprints.md`](blueprints.md) — **Draft**
- [`research.md`](research.md) — **Draft**
- [`production_chains.md`](production_chains.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — first-pass consistency audit

## Authority Boundary

This domain owns:

- physical resource identity;
- canonical material/component catalog;
- mining and salvage acquisition rules;
- raw-to-refined processes;
- crafting recipe rules;
- blueprint ownership;
- research/evidence rules;
- production dependency graphs.

Station machinery owns job execution and logistics under `../station/`.

Player inventory/tools belong under `../player/`.

Ship cargo belongs under `../spacecraft/`.

Mission extraction belongs under `../missions/`.

Economic value belongs under `../systems/`.

Finished weapon, ship, robot, armor, and station-module definitions remain authoritative in their own future domains but must register recipes using canonical GDS-4 resources.

## Core Baseline

The current first-pass establishes:

- one authoritative owner for every physical quantity;
- no generic material rarity tiers;
- a canonical core resource catalog;
- finite deposits and contextual salvage;
- explicit processing recipes;
- blueprint knowledge separate from research technology;
- persistent non-spendable Research Evidence rather than generic Science Points;
- canonical fusion, farming, medical, coolant, propellant, superconductive, and quantum chains;
- anti-duplication and anti-infinite-loop rules.

## Completion State

GDS-4 is not yet Design Complete.

Player, Spacecraft, World, Missions, Combat, and Robots are now first-pass cross-validated against GDS-4. Economy/Progression/Persistence still owns final prices, pacing, and balance, while finished content recipes continue to register through the canonical resource authority.
