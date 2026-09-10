# Spacecraft Design

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for persistent player-owned spacecraft and ship operation.

## Specifications

- [`overview.md`](overview.md) — **Draft**
- [`ship_classes.md`](ship_classes.md) — **Draft**
- [`ship_construction_and_configuration.md`](ship_construction_and_configuration.md) — **Draft**
- [`ship_systems.md`](ship_systems.md) — **Draft**
- [`flight_model.md`](flight_model.md) — **Draft**
- [`propulsion.md`](propulsion.md) — **Draft**
- [`navigation.md`](navigation.md) — **Draft**
- [`docking_and_hangars.md`](docking_and_hangars.md) — **Draft**
- [`cargo_and_utility.md`](cargo_and_utility.md) — **Draft**
- [`damage_and_repairs.md`](damage_and_repairs.md) — **Draft**
- [`ship_combat.md`](ship_combat.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — refreshed through GDS-12

## Authority Boundary

This domain owns:

- ship identity and hull classes;
- modular fitting/configuration;
- onboard power/thermal/life support;
- local-space flight;
- propulsion and strategic reach;
- navigation/travel state;
- docking/hangar behavior;
- ship cargo/utility;
- ship damage/recovery;
- spacecraft combat integration.

GDS-4 owns canonical resources, recipes, Blueprints, and Research.

GDS-5 owns player movement, EVA, inventory, equipment, and direct interaction.

GDS-7 owns the actual galaxy route graph and locations.

GDS-8 owns mission extraction/deployment.

GDS-9 owns final weapon/damage/shield/armor combat formulas.

GDS-10 owns robot units/TCC while this domain owns their physical spacecraft transport/service interfaces.

GDS-11 owns raid/boarding/station-defense orchestration.

GDS-12 owns ship-facing economy, progression pacing, difficulty constraints, final persistence, and finale readiness validation.

## Core Baseline

GDS-6 establishes:

- persistent player-owned ships;
- Shuttle, Scout, Utility Cutter, Corvette, and Frigate hull classes;
- authored hull variants and modular slots rather than freeform voxel ships;
- mass/power/thermal/life-support fitting validation;
- compact physical onboard systems;
- 6DoF inertial local flight with Flight Assist;
- separate local flight and strategic travel scales;
- Chemical/Electrothermal, Ion/Plasma, Fusion Torch, and Exotic/Quantum drive families;
- Strategic Reach Classes I–IV;
- explicit Fuel vs Propellant distinction;
- finite docking/service connections;
- Mass + Volume ship cargo;
- Vehicle/Extraction-Secured cargo ownership;
- subsystem-specific persistent ship damage;
- Disabled/Derelict recovery rather than routine permanent primary-ship deletion;
- ship-combat integration without preempting GDS-9.

## Downstream Resolution

World/Galaxy, Missions, Combat, Robots, and Raids are now first-pass cross-validated.

GDS-12 has additionally resolved:

- prices/service economy boundary;
- Reach progression pacing;
- TCC ship-support progression;
- final save transaction semantics;
- persistent ship recovery interaction;
- Reach-IV finale ship readiness without requiring a Frigate.

## Completion State

GDS-6 is not yet Design Complete.

Its remaining scheduled downstream dependency is **GDS-13 Presentation, Onboarding, and Accessibility**, followed by the GDS-14 whole-project audit.
