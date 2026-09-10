# Raid Design

> **Domain Status:** Design Complete — GDS-14 Promotion Passed

Authoritative design domain for offensive raids against fortified targets and defensive raids against Horizon Station.

## Specifications

- [`overview.md`](overview.md) — **Design Complete**
- [`target_intelligence.md`](target_intelligence.md) — **Design Complete**
- [`raid_preparation.md`](raid_preparation.md) — **Design Complete**
- [`external_space_attack.md`](external_space_attack.md) — **Design Complete**
- [`boarding_and_breaching.md`](boarding_and_breaching.md) — **Design Complete**
- [`interior_attack.md`](interior_attack.md) — **Design Complete**
- [`system_sabotage.md`](system_sabotage.md) — **Design Complete**
- [`looting.md`](looting.md) — **Design Complete**
- [`reinforcements_and_escalation.md`](reinforcements_and_escalation.md) — **Design Complete**
- [`extraction.md`](extraction.md) — **Design Complete**
- [`victory_failure_and_retreat.md`](victory_failure_and_retreat.md) — **Design Complete**
- [`station_defense_resolution.md`](station_defense_resolution.md) — **Design Complete**
- [`story_raid_integration.md`](story_raid_integration.md) — **Design Complete**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns raid-specific phase structure, target intelligence/persistent raid state, preparation, external assault, boarding/breaching, interior control, sabotage, physical looting/theft, escalation/reinforcements, withdrawal/extraction, victory/failure/retreat, Horizon defense resolution, and raid-specific story execution.

GDS-2 owns Home Station systems. GDS-4 owns Resources/ownership/Research/Blueprints. GDS-6 owns spacecraft. GDS-8 owns base Mission lifecycle/objectives/extraction/failure. GDS-9 owns direct combat. GDS-10 owns robot bodies/TCC. GDS-12 owns economy/progression/Dynamic Events/Difficulty/long-term recovery/persistence. GDS-13 owns raid planning/HUD/feedback/VFX/audio/alarms/onboarding/accessibility.

## Core Baseline

GDS-11 establishes specialized GDS-8 raids; persistent fortified targets; Unknown/Suspected/Confirmed/Stale intelligence; preparation based on real capability; low-signature and destructive access alternatives; external defense suppression; physical docking/maintenance/hull breach; local interior control; causal sabotage; physical finite loot; finite sourced reinforcements; physical withdrawal/extraction; objective-based success; deterministic Horizon defense on- or off-screen; no offline raids; no routine crew/Horizon deletion; permanent robot attrition; canonical story-raid integration; and no baseline PvP raid system.

## Downstream Resolution

GDS-12 resolves economy, event pacing, Recovery Grace, target recovery, reputation, Difficulty, save semantics and finale integration.

GDS-13 now resolves:

- raid intelligence/planning UX preserving uncertainty;
- Approach/Access/Interior/Withdrawal presentation;
- bridgehead and extraction HUD;
- sabotage/loot ownership feedback;
- known escalation/reinforcement signaling without information leakage;
- Horizon defense alarms and remote reporting;
- raid VFX/audio tied to physical state;
- staged raid tutorial with a required systemic non-kill-all teaching example;
- accessibility without wallhacks or hidden target revelation.

## Completion State

GDS-11 is Design Complete after the GDS-14 promotion sweep.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The GDS-14 Design Complete promotion gate has passed.


## GDS-14 Promotion State

All **13** authoritative owning specifications in this domain passed the GDS-14 maturity gate and are **Design Complete**. Historical `CROSS_VALIDATION.md` material remains audit evidence and does not override current owning specifications.
