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
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns player-character persistence, direct controls, on-foot movement, interaction, biological Health/Incapacitation, portable inventory, equipment, tools/gadgets and environmental survival.

GDS-9 owns combat attack/damage formulas. GDS-8 owns mission/extraction lifecycle. GDS-6 owns spacecraft flight/cargo/life support. Station/World own external environment state. GDS-12 owns economy/progression/Difficulty/persistence coordination. GDS-13 owns player-facing HUD, interaction feedback, controls presentation/onboarding and accessibility.

## Core Baseline

GDS-5 establishes first-person play; no Player Level/stat tree; no sprint stamina bar; walk/sprint/crouch/jump/mantle/zero-G locomotion; action-based rebindable controls; finite-range physical interaction; Health → Incapacitated → recovery rather than routine permadeath; deterministic mission-defeat inventory handling; Mass + Volume portable capacity; explicit equipment slots/Condition; finite Suit Energy and Life-Support Reserve; Engineering/Mining/Salvage/Scanner/Sample tools; pressure/oxygen/temperature/radiation/EVA survival; and no personal hunger/thirst/fatigue micromanagement.

## Downstream Resolution

Spacecraft, World, Missions, Combat and GDS-12 Economy/Progression/Difficulty/Persistence are first-pass cross-validated against Player.

GDS-13 now resolves:

- first-person HUD and contextual interaction prompts;
- Health/Shield/Suit Energy/Life-Support presentation;
- Mass/Volume and equipment ownership feedback;
- exact interaction blockers and commit feedback;
- mission/combat/environment onboarding;
- fully remappable core controls;
- FOV, shake/head-bob, reticle, hold/toggle, subtitles/captions and other accessibility behavior;
- bounded aim assistance that cannot alter physical hit resolution.

## Completion State

GDS-5 is not yet Design Complete.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The remaining gate is **GDS-14 — Cross-System Consistency Audit**.
