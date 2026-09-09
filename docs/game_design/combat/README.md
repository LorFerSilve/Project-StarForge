# Combat Design

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for direct combat rules.

## Specifications

- [`combat_system.md`](combat_system.md) — **Draft**
- [`damage_model.md`](damage_model.md) — **Draft**
- [`hit_resolution.md`](hit_resolution.md) — **Draft**
- [`weapons.md`](weapons.md) — **Draft**
- [`ammunition_and_reload.md`](ammunition_and_reload.md) — **Draft**
- [`armor_and_shields.md`](armor_and_shields.md) — **Draft**
- [`status_effects.md`](status_effects.md) — **Draft**
- [`on_foot_combat.md`](on_foot_combat.md) — **Draft**
- [`enemy_combat_behavior.md`](enemy_combat_behavior.md) — **Draft**
- [`spacecraft_combat_resolution.md`](spacecraft_combat_resolution.md) — **Draft**
- [`combat_objective_resolution.md`](combat_objective_resolution.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — first-pass consistency audit

## Authority Boundary

This domain owns direct-combat resolution:

- hit detection;
- Damage Packets;
- combat damage channels;
- shield overflow;
- armor penetration/mitigation;
- weapons;
- ammunition/reload;
- status effects;
- on-foot combat flow;
- generic enemy combat AI;
- spacecraft weapon/damage resolution;
- combat-objective outcome semantics.

GDS-5 owns player biological Health/Incapacitation, movement, inventory, and equipment persistence.

GDS-6 owns spacecraft flight, power, thermal, module condition, Disabled/Derelict state, and recovery.

GDS-8 owns mission lifecycle, objectives, extraction, and failure transactions.

GDS-10 owns player robot classes, squad control, manufacturing, and robot persistence.

GDS-11 owns raid/defense orchestration.

## Core Baseline

GDS-9 establishes:

- deterministic physical hit resolution rather than RPG hit rolls;
- no random critical-hit chance;
- Kinetic, Thermal, Explosive, Electrical/EMP, and Corrosive channels;
- Shield → Armor → target damage ordering;
- deterministic shield overflow;
- explicit armor Penetration and mitigation formula;
- physical finite ammunition and reload transactions;
- distinct ballistic, shotgun, precision, rail, laser, plasma, explosive, and melee weapon families;
- no loot-rarity weapon tiers;
- deterministic status application;
- first-person combat without stamina/dodge-i-frame requirements;
- enemy AI without omniscience or magical flanking;
- subsystem-aware spacecraft combat;
- exact Kill/Incapacitate/Destroy/Disable/Drive Off/Capture/Protect/Survive/Clear Area objective semantics.

## Completion State

GDS-9 is not yet Design Complete.

Robots are now first-pass cross-validated against GDS-9. Raids, Economy/Progression/Difficulty/Persistence, and Presentation remain downstream combat dependencies.
