# Cross-Cutting Game Systems

> **Domain Status:** First-Pass Complete — GDS-14 Audited

Authoritative design domain for systems that span several gameplay domains.

## Specifications

### GDS-12 Baseline

- [`economy_and_trading.md`](economy_and_trading.md)
- [`faction_reputation.md`](faction_reputation.md)
- [`dynamic_events.md`](dynamic_events.md) — GDS-14 corrected
- [`difficulty_and_scaling.md`](difficulty_and_scaling.md)
- [`loot_and_reward_rules.md`](loot_and_reward_rules.md)
- [`time_and_simulation.md`](time_and_simulation.md) — GDS-14 corrected
- [`save_and_persistence.md`](save_and_persistence.md)
- [`failure_recovery_rules.md`](failure_recovery_rules.md)
- [`multiplayer_position.md`](multiplayer_position.md)
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — GDS-12 first-pass audit

### GDS-14 Gap-Closure Specifications

The whole-project audit discovered three cross-domain rules that had been referenced by several systems but lacked one sufficiently explicit authoritative home. They are now canonical:

- [`horizon_recovery_state.md`](horizon_recovery_state.md) — Normal / Recovering / Critical Recovery and Recovery Grace interaction;
- [`communications_and_remote_control.md`](communications_and_remote_control.md) — local/strategic communication availability and remote-order boundary;
- [`recovery_transit_and_destination.md`](recovery_transit_and_destination.md) — external-defeat recovery destination/timing and anti-fast-travel behavior.

These are gap-closure rules, not unrelated new feature scope.

## Authority Boundary

This domain owns cross-system rules for:

- Credits, pricing, markets, stock, liquidity, buying, selling, and services;
- faction-reputation application to economy/access without redefining faction identity;
- physical loot/value and mission-reward coordination;
- Simulation Time, Active Game Time metadata, True Pause, and no-offline-progression semantics;
- Dynamic Event generation/lifecycle/pacing;
- Horizon Recovery State and Recovery Grace;
- player-selected Difficulty profiles and allowed scaling axes;
- save types, Stable Save Boundaries, transaction-safe persistence, and deterministic reload behavior;
- cross-domain failure/recovery asset rules;
- external-defeat Recovery Transit/destination;
- strategic communication and remote-control availability;
- baseline multiplayer exclusion/position.

Subsystem-specific behavior remains authoritative in its owning GDS domain.

The parallel [`../progression/`](../progression/) domain owns cross-domain progression/pacing rather than the mechanics being progressed.

## GDS-14 Corrected Invariants

### Time

Simulation Time is the **only gameplay-progression clock**.

Active Game Time remains user-facing unpaused playtime/UX cadence and may drive periodic autosave frequency, but it does not independently advance world state.

True Pause freezes the complete simulation.

### Horizon Recovery

Every resolved ordinary procedural Horizon Defense Event starts/refreshes Recovery Grace.

If Horizon is in Critical Recovery, remaining Recovery Grace countdown is held until the station is stably out of that condition.

Critical Recovery is derived from actual station-system conditions and is not a global station-health score.

### Communications

There is no universal always-on interstellar telemetry.

Strategic Communication Links are Available, Degraded, or Unavailable according to actual ship/station/relay/world infrastructure.

Live remote Horizon orders require a valid link and a capable receiving control/automation system.

### External Defeat Recovery

Ordinary external defeat does not instantly teleport the player and all assets to Horizon.

The player can enter Recovery Transit; destination must be a valid actual recovery location, consumes Simulation Time, and does not automatically move the Primary Ship, robots, cargo, crew, or passengers.

## Core Baseline

The cross-cutting design establishes:

- Credits (`Cr`) as non-physical ledger currency;
- finite physical market stock and liquidity;
- legitimate regional price differences without same-market buy/sell exploits;
- no loot-rarity/random-affix treadmill as core progression;
- physical field loot distinct from Resolution Rewards;
- Assisted, Standard, Veteran, Extreme, and Custom Difficulty profiles;
- no universal player-level scaling, difficulty-exclusive progression, or AI omniscience;
- Simulation Time as sole gameplay-time authority;
- persistent Dynamic Event IDs/seeds/lifecycles;
- deterministic Recovery Grace;
- Manual, Quick, and Autosave through Stable Save Boundaries;
- atomic persistence for ownership, rewards, extraction, trade, and irreversible story transactions;
- causal asset-specific failure rather than generic death tax;
- infrastructure-based strategic communication;
- causal defeat recovery rather than respawn/fast travel;
- single-player PvE as complete baseline, with no required PvP/shared economy.

## Audit Status

GDS-14 found and resolved cross-system ambiguities in:

1. gameplay clock terminology/authority;
2. Horizon Critical Recovery and Recovery Grace trigger;
3. strategic communication/remote control;
4. external defeat recovery destination/timing;
5. cross-domain priority/state namespace qualification.

The corrected rules are validated under [`../audit/`](../audit/).

## Completion State

This domain is **GDS-14 audited**.

Individual specification status promotion remains governed by the final GDS-14 report and [`../00_design_authority.md`](../00_design_authority.md).

No implementation permission is implied by this index.
