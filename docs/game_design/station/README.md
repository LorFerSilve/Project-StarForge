# Home Station Design

> **Domain Status:** Design Complete — GDS-14 Promotion Passed

Authoritative design domain for the player's persistent home space station.

## Specifications

- [`overview.md`](overview.md) — **Design Complete**
- [`construction.md`](construction.md) — **Design Complete**
- [`structural_rules.md`](structural_rules.md) — **Design Complete**
- [`power.md`](power.md) — **Design Complete**
- [`atmosphere_and_oxygen.md`](atmosphere_and_oxygen.md) — **Design Complete**
- [`thermal_systems.md`](thermal_systems.md) — **Design Complete**
- [`water.md`](water.md) — **Design Complete**
- [`farming_and_food.md`](farming_and_food.md) — **Design Complete**
- [`storage_and_logistics.md`](storage_and_logistics.md) — **Design Complete**
- [`manufacturing.md`](manufacturing.md) — **Design Complete**
- [`damage_and_repairs.md`](damage_and_repairs.md) — **Design Complete**
- [`automation.md`](automation.md) — **Design Complete**
- [`defenses.md`](defenses.md) — **Design Complete**
- [`security.md`](security.md) — **Design Complete**
- [`station_events.md`](station_events.md) — **Design Complete**

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

GDS-2 is Design Complete after the GDS-14 promotion sweep.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The GDS-14 Design Complete promotion gate has passed.


## GDS-14 Promotion State

All **15** authoritative owning specifications in this domain passed the GDS-14 maturity gate and are **Design Complete**. Historical `CROSS_VALIDATION.md` material remains audit evidence and does not override current owning specifications.
