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

GDS-11 is not yet Design Complete.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The remaining gate is **GDS-14 — Cross-System Consistency Audit**.
