# Cross-Cutting Game Systems

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for systems that span several gameplay domains.

## Specifications

- [`economy_and_trading.md`](economy_and_trading.md) — **Draft**
- [`faction_reputation.md`](faction_reputation.md) — **Draft**
- [`dynamic_events.md`](dynamic_events.md) — **Draft**
- [`difficulty_and_scaling.md`](difficulty_and_scaling.md) — **Draft**
- [`loot_and_reward_rules.md`](loot_and_reward_rules.md) — **Draft**
- [`time_and_simulation.md`](time_and_simulation.md) — **Draft**
- [`save_and_persistence.md`](save_and_persistence.md) — **Draft**
- [`failure_recovery_rules.md`](failure_recovery_rules.md) — **Draft**
- [`multiplayer_position.md`](multiplayer_position.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — GDS-12 first-pass consistency audit

## Authority Boundary

This domain owns cross-system rules for:

- Credits, pricing, markets, stock, liquidity, buying, selling, and services;
- application of faction reputation to cross-domain access/economy without redefining GDS-7 faction identity;
- physical loot/value and mission-reward coordination;
- Simulation Time and no-offline-progression semantics;
- Dynamic Event generation/lifecycle/pacing;
- player-selected Difficulty profiles and allowed scaling axes;
- save types, Stable Save Boundaries, transaction-safe persistence, and deterministic reload behavior;
- cross-domain failure/recovery asset rules;
- baseline multiplayer exclusion/position.

Subsystem-specific behavior remains authoritative in its owning GDS domain. Cross-cutting documents coordinate those rules and must not duplicate their internal simulation.

The parallel [`../progression/`](../progression/) domain contains the GDS-12 cross-domain progression and pacing specifications.

## Core Baseline

GDS-12 establishes:

- Credits (`Cr`) as non-physical ledger currency rather than a crafting resource;
- finite physical market stock and finite market liquidity;
- legitimate regional price differences without same-market buy/sell exploits;
- economy that cannot bypass Research, Blueprints, physical service, fitting, or resource ownership;
- no loot-rarity/random-affix treadmill as core progression;
- physical field loot kept distinct from Resolution Rewards;
- Assisted, Standard, Veteran, Extreme, and Custom Difficulty profiles;
- no universal player-level enemy scaling, difficulty-exclusive progression, or AI omniscience;
- Simulation Time as gameplay-time authority and no real-world offline progression;
- persistent Dynamic Event IDs/seeds/lifecycles and Recovery Grace for severe Horizon setbacks;
- Manual Save, Quick Save, and Autosave through Stable Save Boundaries;
- atomic persistence for ownership, rewards, extraction, trade, and irreversible story transactions;
- causal asset-specific failure rather than a generic death tax;
- single-player PvE as the complete baseline, with no required PvP/shared economy.

## Cross-Validation Result

The GDS-12 first-pass audit found no blocking contradiction against GDS-1 through GDS-11.

GDS-13 has now first-pass defined and cross-validated economy/trade UI, loot/value feedback, Difficulty presentation, Dynamic Event signaling, save/Stable Save Boundary feedback, failure summaries, and relevant accessibility behavior without changing GDS-12 semantics.

## Completion State

Cross-Cutting Systems are:

**First-Pass Complete — Cross-Validation Pending**

Their GDS-13 presentation dependency is first-pass resolved. The remaining dependency is GDS-14 final whole-project consistency audit before Design Complete status.
