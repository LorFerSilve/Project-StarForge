# Combat Design

> **Domain Status:** Design Complete — GDS-14 Promotion Passed

Authoritative design domain for direct combat rules.

## Specifications

- [`combat_system.md`](combat_system.md) — **Design Complete**
- [`damage_model.md`](damage_model.md) — **Design Complete**
- [`hit_resolution.md`](hit_resolution.md) — **Design Complete**
- [`weapons.md`](weapons.md) — **Design Complete**
- [`ammunition_and_reload.md`](ammunition_and_reload.md) — **Design Complete**
- [`armor_and_shields.md`](armor_and_shields.md) — **Design Complete**
- [`status_effects.md`](status_effects.md) — **Design Complete**
- [`on_foot_combat.md`](on_foot_combat.md) — **Design Complete**
- [`enemy_combat_behavior.md`](enemy_combat_behavior.md) — **Design Complete**
- [`spacecraft_combat_resolution.md`](spacecraft_combat_resolution.md) — **Design Complete**
- [`combat_objective_resolution.md`](combat_objective_resolution.md) — **Design Complete**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns direct-combat hit detection, Damage Packets/channels, shields/armor/penetration, weapons, ammunition/reload, statuses, on-foot combat, generic combat AI, spacecraft combat resolution and combat-objective semantics.

GDS-5 owns player Health/Incapacitation/movement/inventory. GDS-6 owns ship flight/power/thermal/module condition/recovery. GDS-8 owns mission lifecycle/extraction/failure. GDS-10 owns robot classes/control/persistence. GDS-11 owns raid/defense orchestration. GDS-12 owns economy/progression/Difficulty/persistence context. GDS-13 owns combat HUD, reticle/hit feedback, VFX/audio, onboarding and accessibility.

## Core Baseline

GDS-9 establishes physical deterministic hit resolution; authored weak points instead of random criticals; Kinetic/Thermal/Explosive/Electrical-EMP/Corrosive channels; Shield → Armor → target damage; deterministic shield overflow; Penetration/mitigation; finite physical ammo/reload; distinct weapon families; no loot-rarity weapon tiers; deterministic statuses; first-person combat without required stamina/dodge-i-frames; non-omniscient AI; subsystem-aware spacecraft combat; and exact combat-objective outcome semantics.

## Downstream Resolution

Robots, Raids and GDS-12 are first-pass cross-validated against GDS-9.

GDS-13 now resolves:

- adaptive combat HUD/reticle;
- optional knowledge-constrained hit/weak-point feedback;
- shield/armor/status/damage-direction presentation;
- weapon/reload/dry-fire transaction feedback;
- physical damage-channel VFX/audio language;
- non-omniscient marker/music/caption behavior;
- combat onboarding;
- reduced motion/effects and color/audio redundancy;
- bounded aim assistance without bullet magnetism, hidden hit chance, wall targeting or automatic weak-point selection.

## Completion State

GDS-9 is Design Complete after the GDS-14 promotion sweep.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The GDS-14 Design Complete promotion gate has passed.


## GDS-14 Promotion State

All **11** authoritative owning specifications in this domain passed the GDS-14 maturity gate and are **Design Complete**. Historical `CROSS_VALIDATION.md` material remains audit evidence and does not override current owning specifications.
