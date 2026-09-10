# Production Chains

> **Status:** Design Complete  
> **Authority:** Canonical dependency graph connecting raw acquisition, processing, intermediate components, operational resources, and major technology tiers

## 1. Purpose

This document makes the resource economy inspectable as a graph.

It prevents recipes from becoming isolated one-off definitions and ensures that progression has understandable upstream dependencies.

## 2. Chain Layers

Canonical chains use these layers:

1. Acquisition
2. Raw Resource
3. Refined Material / Operational Resource
4. Standard Component
5. Advanced Component
6. Finished System / Construction / Equipment

Finished-system recipes are completed by their owning future domains.

## 3. Ferrous Structural Chain

```text
Iron Deposit / Salvage
        ↓
     Iron Ore
        ↓
   Refined Iron
        ↓ + Carbonaceous Feedstock
 Structural Steel
        ↓
 Structural Frame
        ↓
Station structure / machinery / ship structure
```

## 4. Aluminum Lightweight Chain

```text
Bauxite Deposit
      ↓
   Bauxite
      ↓
  Aluminum
      ├── Pressure Seal Assembly (+ Polymer Resin)
      ├── Servo Assembly (+ Copper + Basic Circuit)
      └── Lightweight station/ship/equipment structure
```

## 5. Copper-Electronics Chain

```text
Copper Ore
    ↓
  Copper
    ├──────────────┐
    ↓              ↓
Magnetic Coil   Basic Circuit ← Electronic-Grade Silicon
    │              ↓
    │         Advanced Circuit ← Platinum + Silicon
    │              ├── Sensor Package (+ Glass + Platinum)
    │              └── Advanced systems
    └── motors / generators / weapons
```

## 6. Silicon and Optics Chain

```text
Silica
 ├── Electronic-Grade Silicon → Circuits
 └── Technical Glass → Sensors / pressure windows / labs
```

## 7. Polymer and Composite Chain

```text
Carbonaceous Feedstock
     ├── Polymer Resin
     │      ├── Pressure Seal Assembly (+ Aluminum)
     │      ├── Packaged Rations (+ Fresh Produce)
     │      ├── Industrial Coolant (+ Fresh Water)
     │      └── Carbon Composite (+ Carbonaceous Feedstock)
     └── Structural Steel (+ Refined Iron)
```

## 8. Titanium Thermal/Advanced Structure Chain

```text
Titanium Ore
    ↓
 Titanium
    ├── Heat Exchanger Core (+ Copper)
    ├── Superconductive Material (+ Copper)
    └── advanced structure / armor / pressure systems
```

## 9. Tungsten Kinetic Chain

```text
Tungsten Ore
     ↓
  Tungsten
     ↓
High-temperature components / kinetic penetrators / advanced machinery
```

Finished ammunition and weapon definitions are owned by combat/defense domains.

## 10. Platinum Advanced Electronics Chain

```text
Platinum Ore
     ↓
  Platinum
     ├── Advanced Circuit
     ├── Sensor Package
     └── Quantum Processor
```

## 11. Fission Chain

```text
Uraninite Deposit
      ↓
   Uraninite
      ↓ Nuclear Materials Processor
Uranium Fuel Rod
      ↓
Fission power systems
```

Fission reactor object recipes belong to station/ship content, but their fuel dependency is fixed.

## 12. Water and Life-Support Chain

```text
Water Ice
   ↓
Fresh Water
   ├── Crew consumption
   ├── Farming
   ├── Oxygen-generation feedstock
   ├── Industrial Coolant (+ Polymer Resin)
   ├── Reaction Propellant
   └── Deuterium
```

Wastewater recycling returns a fraction to Fresh Water through the station water system and is not a separate industrial recipe chain.

## 13. Agriculture and Food Chain

```text
Fresh Water + Nutrient Mix + Power + Farm
                  ↓
            Fresh Produce
             ├──────────────→ Crew consumption
             ├── + Polymer Resin → Packaged Rations
             ├── + Fresh Water + Carbonaceous Feedstock → Nutrient Mix
             └── + Fresh Water + Polymer Resin → Medical Compound
```

The Nutrient Mix branch is intentionally lossy.

It recycles part of agricultural output but still requires external Carbonaceous Feedstock and water losses, preventing a closed infinite loop.

## 14. Fusion Chain

```text
Fresh Water
    ↓ Isotope Separation
 Deuterium
     + Helium-3
         ↓
 Fusion Fuel Pellet
         +
Superconductive components
         ↓
Fusion reactors / advanced propulsion
```

Fusion technology therefore depends simultaneously on exploration fuel access and advanced station industry.

## 15. Superconductor Chain

```text
Copper + Titanium
       ↓ Advanced Materials Processing
Superconductive Material
       + Advanced Circuit
       ↓
Superconductive Coil
       ↓
Fusion / shields / advanced engines / high-energy systems
```

## 16. Quantum Chain

```text
Voltaic Crystal
      +
Superconductive Material
      +
Exotic Matter Sample
      ↓
Quantum Substrate
      +
Advanced Circuit
      +
Platinum
      ↓
Quantum Processor
      ↓
Advanced navigation / computation / AI / exotic technology
```

This chain intentionally requires both industrial maturity and rare exploration discoveries.

## 17. Xenobiology Chain

```text
Alien Biomaterial
       ↓ Analysis
Xenobiology Evidence / Technology
       ↓
Advanced medical / biological research
```

Alien Biomaterial is not a routine input to standard Medical Compound.

This preserves its strategic research value.

## 18. Ancient Technology Chain

```text
Ancient Technology Core
       ↓ Secure extraction
Analysis / Evidence
       ↓
TECH_ANCIENT_SYSTEMS
       ↓
Specific ancient blueprints / projects
```

An Ancient Core is not a generic universal upgrade token.

Its exact downstream use is defined by discovered research/blueprint content.

## 19. Repair Chains

Repairs consume resources matching actual damaged systems.

Examples:

- hull/structure → Structural Steel / Structural Frame;
- pressure breach → Pressure Seal Assembly;
- electronics → Basic/Advanced Circuit;
- cooling → Heat Exchanger Core / Coolant;
- actuators → Servo Assembly.

There is no universal Repair Material.

## 20. Construction Chains

Station construction consumes combinations of:

- Structural Frames;
- refined materials;
- canonical electronic/mechanical components;
- technology-specific advanced components.

Each station module definition will register an exact construction recipe.

## 21. Ship Chains

Future ship definitions must reuse canonical chains:

- structure → Steel / Aluminum / Titanium / Carbon Composite;
- power → regulators / coils / fuel;
- propulsion → coils / advanced circuits / fuel;
- sensors → Sensor Package;
- thermal → Heat Exchanger Core.

New ship-only resources require explicit Resource Catalog justification.

## 22. Robot Chains

Robot definitions must reuse:

- Structural material;
- Carbon Composite;
- Servo Assembly;
- Basic/Advanced Circuit;
- Sensor Package;
- power component.

New robot-only resources require explicit catalog justification.

## 23. Defense Chains

Future defense definitions must reuse:

- structure;
- Servo Assembly;
- Sensor Package;
- circuits;
- coils;
- ammunition inputs;
- shield superconductive/energy resources.

## 24. Dependency Depth Philosophy

Early-game recipes should be shallow enough to understand and recover from.

Late-game recipes can be deeper, but every intermediate component must have multiple meaningful uses or strong strategic purpose.

The game avoids chains whose only purpose is adding clicks.

## 25. Bottlenecks

The economy intentionally creates different bottleneck types:

- raw quantity bottleneck: Iron/Carbon;
- logistics bottleneck: throughput/storage;
- technology bottleneck: Superconductive Materials;
- geographic bottleneck: Helium-3/rare ores;
- discovery bottleneck: Exotic Matter/Ancient Core;
- specialist bottleneck: advanced research/operation.

No single resource should be the sole bottleneck for every progression branch.

## 26. Closed-Loop Prevention

Any recycling loop must have:

- less than 100% material recovery under normal technology; or
- an external energy/material requirement that prevents net free resource creation.

A player cannot manufacture infinite resources by cycling recipes.

## 27. Recipe Registration Rule

Every future finished-system recipe must:

1. reference only canonical resources/components;
2. identify its blueprint;
3. identify technology prerequisites;
4. identify compatible machine/construction method;
5. declare tuneable quantities;
6. be added to the relevant production-chain graph.

## 28. Explicit Non-Goals

Production chains do not exist to maximize resource count or crafting steps.

The design rejects:

- purposeless intermediate items;
- universal generic components;
- infinite recycling;
- resource teleportation;
- endgame recipes requiring huge quantities of trivial materials as their main challenge.

## 29. Tuneable Parameters

Quantities, process duration, yields, efficiency, and economic value are tuneable.

Resource identities and dependency relationships are fixed until an explicit design change.

## 30. Dependencies

This document integrates Resource Catalog, Processing, Crafting, Blueprints, Research, station manufacturing/logistics/farming/water, crew, and future finished-system domains.

## 31. Open Questions

None in the canonical chain topology.

Future finished-system domains will extend the final layer without redefining upstream resource identity.
