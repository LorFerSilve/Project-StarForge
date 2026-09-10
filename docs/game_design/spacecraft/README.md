# Spacecraft Design

> **Domain Status:** Design Complete — GDS-14 Promotion Passed

Authoritative design domain for persistent player-owned spacecraft and ship operation.

## Specifications

- [`overview.md`](overview.md) — **Design Complete**
- [`ship_classes.md`](ship_classes.md) — **Design Complete**
- [`ship_construction_and_configuration.md`](ship_construction_and_configuration.md) — **Design Complete**
- [`ship_systems.md`](ship_systems.md) — **Design Complete**
- [`flight_model.md`](flight_model.md) — **Design Complete**
- [`propulsion.md`](propulsion.md) — **Design Complete**
- [`navigation.md`](navigation.md) — **Design Complete**
- [`docking_and_hangars.md`](docking_and_hangars.md) — **Design Complete**
- [`cargo_and_utility.md`](cargo_and_utility.md) — **Design Complete**
- [`damage_and_repairs.md`](damage_and_repairs.md) — **Design Complete**
- [`ship_combat.md`](ship_combat.md) — **Design Complete**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns ship identity/classes, fitting, ship systems, local-space flight, propulsion/Reach, navigation/travel, docking/hangars, cargo/utility, damage/recovery, and spacecraft combat integration.

GDS-4 owns Resources/Blueprints/Research. GDS-5 owns player/EVA/inventory interaction. GDS-7 owns galaxy routes/locations. GDS-8 owns mission extraction/deployment. GDS-9 owns combat resolution. GDS-10 owns robot units/TCC while this domain owns ship transport/service interfaces. GDS-11 owns raid/boarding orchestration. GDS-12 owns economy/progression/Difficulty/persistence/finale readiness. GDS-13 owns spacecraft HUD, flight/docking feedback, audio/VFX, onboarding and accessibility.

## Core Baseline

GDS-6 establishes persistent player-owned authored hulls; modular fitting; mass/power/thermal/life-support validation; 6DoF inertial flight with Flight Assist; separate local and strategic travel; Chemical/Electrothermal, Ion/Plasma, Fusion Torch and Exotic/Quantum drives; Reach I–IV; Fuel vs Propellant; finite docking/service connections; Mass + Volume cargo; Vehicle/Extraction-Secured ownership; subsystem damage; Disabled/Derelict recovery; and combat integration without preempting GDS-9.

## Downstream Resolution

World/Galaxy, Missions, Combat, Robots, Raids, and GDS-12 are first-pass cross-validated.

GDS-13 now resolves:

- third-person/cockpit flight presentation parity;
- orientation vs velocity-vector HUD;
- target/power/thermal/fuel/propellant/system warnings;
- physical docking alignment/capture/Hard Dock feedback;
- spacecraft exterior pilot-telemetry audio model;
- thruster/drive/damage VFX tied to actual ship state;
- ship fitting/Reach blocker UX;
- remappable ship controls, camera/motion settings and readable navigation assistance.

## Completion State

GDS-6 is Design Complete after the GDS-14 promotion sweep.

All scheduled downstream subsystem/presentation dependencies are now first-pass resolved. The GDS-14 Design Complete promotion gate has passed.


## GDS-14 Promotion State

All **11** authoritative owning specifications in this domain passed the GDS-14 maturity gate and are **Design Complete**. Historical `CROSS_VALIDATION.md` material remains audit evidence and does not override current owning specifications.
