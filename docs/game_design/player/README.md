# Player Design

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for the persistent player character and direct on-foot interaction.

## Specifications

- [`player_character.md`](player_character.md) — **Draft**
- [`controls_and_input.md`](controls_and_input.md) — **Draft**
- [`movement.md`](movement.md) — **Draft**
- [`interaction.md`](interaction.md) — **Draft**
- [`health_damage_death.md`](health_damage_death.md) — **Draft**
- [`inventory.md`](inventory.md) — **Draft**
- [`equipment.md`](equipment.md) — **Draft**
- [`tools_and_gadgets.md`](tools_and_gadgets.md) — **Draft**
- [`field_survival.md`](field_survival.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — first-pass consistency audit

## Authority Boundary

This domain owns:

- player-character persistence;
- direct input/controls;
- on-foot movement;
- interaction;
- player health/incapacitation;
- portable inventory;
- equipment slots/condition;
- portable tools/gadgets;
- player environmental survival.

Combat-specific attack/damage formulas belong under `../combat/`.

Mission extraction and objective lifecycle belong under `../missions/`.

Spacecraft flight/cargo/life support belong under `../spacecraft/`.

External atmosphere/thermal state belongs under station/world domains.

## Core Baseline

GDS-5 establishes:

- first-person on-foot play;
- no generic Player Level;
- no generic RPG stat tree;
- no normal sprint stamina bar;
- walk/sprint/crouch/jump/mantle/zero-g locomotion;
- action-based rebindable controls;
- finite-range physical interaction;
- Health → Incapacitated → recovery instead of routine permadeath;
- deterministic mission-defeat inventory handling;
- Mass + Volume portable capacity;
- explicit equipment slots and Condition;
- finite Suit Energy and Life-Support Reserve;
- canonical Engineering/Mining/Salvage/Scanner/Sample tools;
- environmental pressure, oxygen, temperature, radiation, and EVA protection;
- no hunger/thirst/fatigue micro-management.

## Completion State

GDS-5 is not yet Design Complete.

Spacecraft, World, Missions, Combat, Economy, Presentation/Accessibility, and Persistence must still cross-validate its future-facing interfaces.
