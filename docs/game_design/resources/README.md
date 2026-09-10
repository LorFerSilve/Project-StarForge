# Resources, Crafting & Research

> **Domain Status:** Design Complete — GDS-14 Promotion Passed

Authoritative design domain for physical resources, acquisition, processing, crafting, blueprints, research, and production dependencies.

## Specifications

- [`resource_model.md`](resource_model.md) — **Design Complete**
- [`resource_catalog.md`](resource_catalog.md) — **Design Complete**
- [`gathering_and_salvage.md`](gathering_and_salvage.md) — **Design Complete**
- [`mining.md`](mining.md) — **Design Complete**
- [`processing.md`](processing.md) — **Design Complete**
- [`crafting.md`](crafting.md) — **Design Complete**
- [`blueprints.md`](blueprints.md) — **Design Complete**
- [`research.md`](research.md) — **Design Complete**
- [`production_chains.md`](production_chains.md) — **Design Complete**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns physical Resource identity/catalog, mining/salvage acquisition, processing, crafting recipes, Blueprint ownership, Research/Evidence and production dependency graphs.

Station machinery owns job execution/logistics. Player owns portable inventory/tools. Spacecraft owns ship cargo. Missions own extraction. GDS-12 owns economic value, pacing and persistence coordination. GDS-13 owns resource/crafting/research presentation and onboarding.

Finished weapon, ship, robot, armor and station-module definitions remain authoritative in their own domains and register their recipes through canonical GDS-4 resources.

## Core Baseline

GDS-4 establishes one authoritative owner for every physical quantity; no generic material rarity tiers; canonical resources; finite deposits/contextual salvage; explicit processing recipes; Blueprint knowledge separate from Research; persistent non-spendable Research Evidence; canonical industrial chains; and anti-duplication/anti-infinite-loop rules.

## Downstream Resolution

Player, Spacecraft, World, Missions, Combat, Robots and GDS-12 Economy/Progression/Persistence are first-pass cross-validated against GDS-4.

GDS-13 now resolves:

- physical source/ownership/pickup/transfer feedback;
- Mass/Volume and reservation presentation;
- Work Order state/blocker UX;
- Blueprint vs Technology Research vs Research Evidence presentation;
- field-loot/extraction security feedback;
- market value presentation without changing physical ownership;
- onboarding for acquisition, processing, crafting and research;
- accessible color-independent item/resource state communication.

## Completion State

GDS-4 is Design Complete after the GDS-14 promotion sweep.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The GDS-14 Design Complete promotion gate has passed.


## GDS-14 Promotion State

All **9** authoritative owning specifications in this domain passed the GDS-14 maturity gate and are **Design Complete**. Historical `CROSS_VALIDATION.md` material remains audit evidence and does not override current owning specifications.
