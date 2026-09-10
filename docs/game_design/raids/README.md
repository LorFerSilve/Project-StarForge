# Raid Design

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for offensive raids against fortified targets and defensive raids against Horizon Station.

## Specifications

- [`overview.md`](overview.md) — **Draft**
- [`target_intelligence.md`](target_intelligence.md) — **Draft**
- [`raid_preparation.md`](raid_preparation.md) — **Draft**
- [`external_space_attack.md`](external_space_attack.md) — **Draft**
- [`boarding_and_breaching.md`](boarding_and_breaching.md) — **Draft**
- [`interior_attack.md`](interior_attack.md) — **Draft**
- [`system_sabotage.md`](system_sabotage.md) — **Draft**
- [`looting.md`](looting.md) — **Draft**
- [`reinforcements_and_escalation.md`](reinforcements_and_escalation.md) — **Draft**
- [`extraction.md`](extraction.md) — **Draft**
- [`victory_failure_and_retreat.md`](victory_failure_and_retreat.md) — **Draft**
- [`station_defense_resolution.md`](station_defense_resolution.md) — **Draft**
- [`story_raid_integration.md`](story_raid_integration.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns:

- raid-specific mission phase structure;
- raid target intelligence and persistent raid-relevant target state;
- raid preparation/validation;
- external fortified-target assault;
- boarding and breaching;
- interior raid control;
- system sabotage;
- physical raid looting/theft;
- escalation and finite reinforcement response;
- raid withdrawal/extraction specialization;
- raid victory/failure/retreat semantics;
- hostile attacks against Horizon Station;
- on-screen/off-screen station-defense execution;
- raid-specific main-story execution.

GDS-2 owns Home Station infrastructure, defenses, security, automation, damage, and repair systems.

GDS-4 owns resource identity, physical ownership, salvage, Research, and Blueprints.

GDS-6 owns spacecraft flight, docking, cargo, power/thermal systems, and persistent ship recovery.

GDS-8 owns the base Mission lifecycle, objectives, extraction/failure transactions, and story Mission IDs.

GDS-9 owns direct combat, weapons, hit/damage, shields/armor, and combat AI.

GDS-10 owns robot bodies, TCC, squads, commands, repair, and robot loss.

GDS-12 owns economy, progression, Dynamic Event generation/pacing, difficulty, long-term target recovery, and final persistence policy.

## Core Baseline

GDS-11 establishes:

- raids as specialized GDS-8 Missions rather than a second mission engine;
- persistent fortified targets rather than pristine reset instances;
- Unknown/Suspected/Confirmed/Stale raid intelligence;
- preparation based on real ship/loadout/robot/cargo/extraction capability;
- valid low-signature and destructive access alternatives;
- physical external defense suppression;
- docking, maintenance access, and authored hull breaching;
- local interior control rather than global station ownership;
- causal sabotage of power, shields, sensors, comms, security, weapons, atmosphere, cooling, logistics, docking, and mission systems;
- physical finite loot transfer rather than percentage theft;
- finite, sourced, deterministic reinforcements;
- physical withdrawal/extraction with no magical squad recall;
- objective-based raid success rather than kill-all victory;
- deterministic Horizon Station defense while player is present or away;
- no real-world offline raids;
- no routine permanent crew death or Horizon Station deletion;
- permanent robot attrition;
- explicit GDS-11 integration for Rook's Wake, forced Sealed Authority assault, and Crownless finale access;
- no baseline synchronous or asynchronous PvP raid system.

## Downstream Resolution

GDS-12 has now cross-validated and resolved:

- target stock/value and raid-profitability context;
- attack-generation pacing;
- Recovery Grace after severe Horizon setbacks;
- long-term target recovery/replenishment;
- reputation/economic consequences;
- Difficulty constraints;
- final save/atomic transaction semantics;
- MS-F01/MS-F02 finale readiness integration.

## Completion State

GDS-11 is not yet Design Complete.

Its remaining scheduled downstream dependency is **GDS-13 Presentation, Onboarding, and Accessibility**, followed by the GDS-14 whole-project audit.
