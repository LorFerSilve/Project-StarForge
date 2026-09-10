# Home Station Design

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for the player's persistent home space station.

## Specifications

- [`overview.md`](overview.md) — **Draft**
- [`construction.md`](construction.md) — **Draft**
- [`structural_rules.md`](structural_rules.md) — **Draft**
- [`power.md`](power.md) — **Draft**
- [`atmosphere_and_oxygen.md`](atmosphere_and_oxygen.md) — **Draft**
- [`thermal_systems.md`](thermal_systems.md) — **Draft**
- [`water.md`](water.md) — **Draft**
- [`farming_and_food.md`](farming_and_food.md) — **Draft**
- [`storage_and_logistics.md`](storage_and_logistics.md) — **Draft**
- [`manufacturing.md`](manufacturing.md) — **Draft**
- [`damage_and_repairs.md`](damage_and_repairs.md) — **Draft**
- [`automation.md`](automation.md) — **Draft**
- [`defenses.md`](defenses.md) — **Draft**
- [`security.md`](security.md) — **Draft**
- [`station_events.md`](station_events.md) — **Draft**

## Authority Boundary

This domain owns Horizon Station construction, structure, connected utilities, farming, storage/logistics, manufacturing, damage/repair, automation, defenses, security, and station-local events.

Crew behavior is authoritative under `../crew/`. Resource recipes are authoritative under `../resources/`. Robot bodies/capability belong under `../robots/`. Raid/defense-event orchestration belongs under `../raids/`. Cross-cutting economy/progression/save/simulation belongs under `../systems/` and `../progression/`. GDS-13 owns station visual language, overlays/UI, interaction feedback, VFX/audio, alarms, onboarding and accessibility.

## Downstream Resolution

Crew, Resources, Player, Spacecraft, Missions, Combat, Robots, Raids, and GDS-12 cross-cutting systems have first-pass defined their station-facing boundaries.

GDS-13 now resolves:

- functional module/network visual language;
- construction preview/validity feedback;
- power/atmosphere/thermal/logistics diagnostic overlays;
- operational/degraded/damaged state readability;
- physical manufacturing/farming/storage activity presentation;
- crew/robot station presence;
- damage/repair persistence visuals;
- P0–P3 alarm hierarchy and cascading-failure aggregation;
- station restoration/construction/automation/emergency/defense onboarding;
- color-independent, scalable and reduced-motion/effects accessibility.

## Completion State

GDS-2 is not yet Design Complete.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The remaining gate is **GDS-14 — Cross-System Consistency Audit**.
