# Core Resource Catalog

> **Status:** Draft  
> **Authority:** Canonical baseline physical resources and manufactured components available to Project StarForge's core progression

## 1. Purpose

This document replaces vague placeholders such as "metal", "rare crystal", or "tech component" with canonical resources.

New resources require an explicit design addition to this catalog or a later specialized catalog owned by the same resource authority.

## 2. Catalog Philosophy

The resource set is intentionally compact enough to remain understandable.

Different gameplay functions should reuse materials where sensible rather than creating hundreds of single-recipe resources.

Late-game scarcity comes from access, processing, strategic components, and rare technology rather than huge counts of trivial material names.

# 3. Raw Geological Resources

## RES_RAW_IRON_ORE — Iron Ore

Class: Physical Bulk Resource  
State: Solid Bulk  
Primary Sources: rocky planets, asteroids, industrial salvage  
Primary Uses: refined iron and steel production

## RES_RAW_BAUXITE — Bauxite

Class: Physical Bulk Resource  
State: Solid Bulk  
Primary Sources: planetary mineral deposits  
Primary Uses: aluminum production

## RES_RAW_COPPER_ORE — Copper Ore

Class: Physical Bulk Resource  
State: Solid Bulk  
Primary Sources: rocky planets and metallic asteroids  
Primary Uses: electrical copper

## RES_RAW_SILICA — Silica

Class: Physical Bulk Resource  
State: Solid Bulk  
Primary Sources: planetary rock, regolith, selected asteroids  
Primary Uses: glass, silicon, electronics

## RES_RAW_TITANIUM_ORE — Titanium Ore

Class: Physical Bulk Resource  
State: Solid Bulk  
Primary Sources: high-value planetary/asteroid deposits  
Primary Uses: titanium structures, advanced ship/station components

## RES_RAW_TUNGSTEN_ORE — Tungsten Ore

Class: Physical Bulk Resource  
State: Solid Bulk  
Primary Sources: dense mineral deposits and selected asteroids  
Primary Uses: high-temperature and kinetic-weapon components

## RES_RAW_PLATINUM_ORE — Platinum Ore

Class: Physical Bulk Resource  
State: Solid Bulk  
Primary Sources: rare metallic asteroids, high-value deposits  
Primary Uses: advanced electronics, catalysts, high-tier components

## RES_RAW_URANINITE — Uraninite

Class: Physical Bulk Resource  
State: Solid Bulk  
Hazard: Radioactive  
Primary Sources: radioactive mineral deposits  
Primary Uses: fission fuel processing

## RES_RAW_WATER_ICE — Water Ice

Class: Physical Bulk Resource  
State: Solid Bulk  
Primary Sources: icy moons, comets, icy asteroids  
Primary Uses: fresh water, oxygen production feedstock, selected propellants

## RES_RAW_CARBON_FEEDSTOCK — Carbonaceous Feedstock

Class: Physical Bulk Resource  
State: Solid Bulk  
Primary Sources: carbonaceous asteroids, organic deposits, salvage  
Primary Uses: polymers, carbon composites, industrial chemicals

# 4. Refined Structural and Industrial Materials

## RES_MAT_IRON — Refined Iron

Produced From: Iron Ore  
Uses: basic structural components, steel production, machinery

## RES_MAT_STEEL — Structural Steel

Produced From: Refined Iron + Carbonaceous Feedstock  
Uses: station frames, armor, machinery, basic ship structure

## RES_MAT_ALUMINUM — Aluminum

Produced From: Bauxite  
Uses: lightweight structure, electronics housings, spacecraft components

## RES_MAT_COPPER — Copper

Produced From: Copper Ore  
Uses: power systems, motors, electronics, coils

## RES_MAT_SILICON — Electronic-Grade Silicon

Produced From: Silica  
Uses: circuits, sensors, processors

## RES_MAT_GLASS — Technical Glass

Produced From: Silica  
Uses: pressure windows, optics, sensors, laboratory equipment

## RES_MAT_TITANIUM — Titanium

Produced From: Titanium Ore  
Uses: advanced structure, armor, pressure systems, spacecraft

## RES_MAT_TUNGSTEN — Tungsten

Produced From: Tungsten Ore  
Uses: heat-resistant components, railgun/kinetic systems, advanced machinery

## RES_MAT_PLATINUM — Platinum

Produced From: Platinum Ore  
Uses: advanced electronics, catalysts, precision components

## RES_MAT_POLYMER — Polymer Resin

Produced From: Carbonaceous Feedstock through chemical processing  
Uses: seals, insulation, medical consumables, lightweight components

## RES_MAT_CARBON_COMPOSITE — Carbon Composite

Produced From: Carbonaceous Feedstock + Polymer Resin  
Uses: lightweight advanced structures, armor panels, robotics

## RES_MAT_SUPERCONDUCTOR — Superconductive Material

Produced From: Copper + Titanium + advanced processing inputs  
Uses: high-power coils, advanced reactors, engines, shields

# 5. Energy and Operational Resources

## RES_OP_FRESH_WATER — Fresh Water

Class: Physical Bulk Resource  
State: Fluid  
Primary Sources: Water Ice processing, station wastewater recycling, trade/recovery  
Uses: crew support, farming, oxygen generation, coolant/propellant/deuterium processing

## RES_OP_WASTEWATER — Wastewater

Class: Physical Bulk Resource  
State: Fluid  
Primary Sources: crew/farming/compatible station processes  
Uses: station water recycling back into Fresh Water

Wastewater is not a crafting ingredient unless an explicit future process requires it. Its primary authority remains the station water-recycling system.

## RES_ENR_URANIUM_FUEL — Uranium Fuel Rod

Class: Discrete Component  
Hazard: Radioactive  
Produced From: Uraninite  
Uses: fission reactors

## RES_ENR_HELIUM3 — Helium-3

Class: Physical Bulk Resource  
State: Gas / cryogenic storage abstraction  
Hazard: Pressurized  
Primary Sources: lunar/regolith extraction, gas-resource missions, trade  
Uses: fusion fuel processing

## RES_ENR_DEUTERIUM — Deuterium

Class: Physical Bulk Resource  
State: Gas / cryogenic storage abstraction  
Hazard: Pressurized  
Produced From: Fresh Water through isotope separation  
Uses: fusion fuel processing

## RES_ENR_FUSION_PELLET — Fusion Fuel Pellet

Class: Discrete Component  
Produced From: Helium-3 + Deuterium  
Uses: fusion reactors and advanced propulsion

## RES_OP_COOLANT — Industrial Coolant

Class: Physical Bulk Resource  
State: Fluid  
Uses: thermal loops, repairs, selected manufacturing

## RES_OP_PROPELLANT — Reaction Propellant

Class: Physical Bulk Resource  
State: Fluid/Gas abstraction  
Uses: spacecraft systems whose propulsion design requires consumable reaction mass

## RES_OP_NUTRIENT_MIX — Nutrient Mix

Class: Physical Bulk Resource  
State: Packaged/Bulk Consumable  
Uses: station agriculture

## RES_OP_MEDICAL_COMPOUND — Medical Compound

Class: Physical Bulk Resource  
State: Packaged Consumable  
Uses: medical treatment and advanced medbay recipes

# 6. Food Resources

## RES_FOOD_FRESH_PRODUCE — Fresh Produce

Class: Physical Bulk Resource  
Perishable: Yes  
Sources: station agriculture, selected settlements  
Uses: crew consumption, ration processing

## RES_FOOD_RATIONS — Packaged Rations

Class: Physical Bulk Resource  
Perishable: No under normal storage timescale  
Produced From: Fresh Produce + packaging/processing inputs  
Uses: crew reserve, expedition provisioning

# 7. Standard Manufactured Components

## RES_CMP_STRUCTURAL_FRAME — Structural Frame

Produced From: Structural Steel  
Uses: station/ship construction and structural repairs

## RES_CMP_PRESSURE_SEAL — Pressure Seal Assembly

Produced From: Polymer Resin + Aluminum  
Uses: airlocks, pressure systems, breach repairs

## RES_CMP_BASIC_CIRCUIT — Basic Circuit

Produced From: Copper + Electronic-Grade Silicon  
Uses: simple electronics, automation, sensors

## RES_CMP_ADVANCED_CIRCUIT — Advanced Circuit

Produced From: Basic Circuits + Platinum + Electronic-Grade Silicon  
Uses: advanced station/ship/robot electronics

## RES_CMP_POWER_REGULATOR — Power Regulator

Produced From: Copper + Basic Circuit + Structural material  
Uses: power infrastructure, machines, ship systems

## RES_CMP_MAGNETIC_COIL — Magnetic Coil

Produced From: Copper + Structural Steel  
Uses: motors, generators, early propulsion, weapons

## RES_CMP_SUPERCONDUCTIVE_COIL — Superconductive Coil

Produced From: Superconductive Material + Advanced Circuit  
Uses: fusion systems, shields, advanced engines, high-energy weapons

## RES_CMP_SERVO — Servo Assembly

Produced From: Aluminum/Steel + Copper + Basic Circuit  
Uses: robots, turrets, doors, mechanical automation

## RES_CMP_SENSOR — Sensor Package

Produced From: Technical Glass + Advanced Circuit + Platinum  
Uses: station sensors, ships, robots, targeting

## RES_CMP_HEAT_EXCHANGER — Heat Exchanger Core

Produced From: Copper + Titanium  
Uses: cooling systems, reactors, industrial equipment

## RES_CMP_QUANTUM_SUBSTRATE — Quantum Substrate

Produced From: Voltaic Crystal + Superconductive Material + Exotic Matter Sample  
Uses: late-game quantum computation components

## RES_CMP_QUANTUM_PROCESSOR — Quantum Processor

Produced From: Advanced Circuit + Platinum + Quantum Substrate  
Uses: late-game computation, advanced navigation, AI/control, exotic technology

# 8. Exotic Resources

## RES_EXO_VOLTAIC_CRYSTAL — Voltaic Crystal

Class: Physical Bulk/Discrete Strategic Material  
Hazard: High-Energy  
Primary Sources: rare planetary/anomaly deposits  
Uses: advanced energy conversion, shields, high-energy research

## RES_EXO_ALIEN_BIOMATERIAL — Alien Biomaterial

Class: Physical Bulk Strategic Material  
Hazard: Biohazard where uncontained  
Primary Sources: alien ecosystems, alien structures, specialized missions  
Uses: advanced medical/biological research and technology

## RES_EXO_EXOTIC_MATTER_SAMPLE — Exotic Matter Sample

Class: Discrete Strategic Material  
Hazard: Secure Strategic / High-Energy  
Primary Sources: anomalies, late-game research sites, advanced enemy technology  
Uses: experimental propulsion and late-game physics research

## RES_EXO_ANCIENT_CORE — Ancient Technology Core

Class: Discrete Strategic Component  
Hazard: Secure Strategic  
Primary Sources: ancient structures, major bosses/raid objectives, narrative sites  
Uses: unique research unlocks and late-game technology

# 9. Ammunition Inputs

The game does not define every bullet as a crafting resource.

Ammunition recipes use canonical industrial inputs and create finished ammunition items or magazine resources owned by combat/defense domains.

Typical inputs include:

- Structural Steel;
- Tungsten;
- Polymer Resin;
- propellant;
- electronics for guided ammunition.

# 10. Medical and Repair Inputs

Medical treatment primarily consumes Medical Compound plus authored finished medical items.

Repairs primarily consume canonical structural materials/components matching the damaged system.

There is no generic magical "Repair Material" resource.

# 11. Research Inputs

Research may consume:

- physical samples from this catalog;
- manufactured components;
- physical artifacts;
- Research Data Knowledge Assets.

There is no generic "science point" resource dropped directly by enemies.

# 12. Catalog Expansion Rule

A new physical resource should be added only when at least one is true:

1. it creates a distinct acquisition decision;
2. it creates a distinct storage/hazard decision;
3. it supports a meaningful production chain;
4. it is strategically/narratively unique;
5. reusing an existing resource would make the system materially less understandable.

A new resource must not be added merely to lengthen grind.

## 13. Explicit Non-Goals

The core catalog avoids:

- dozens of colored quality variants;
- hundreds of single-use crafting junk items;
- generic "tech parts" when a canonical component is sufficient;
- magical universal repair resources;
- enemy-dropped generic science points.

## 14. Dependencies

Detailed recipes are authoritative in processing, crafting, production chains, and future equipment catalogs.

World/faction design determines final geographic distribution without redefining resource identity.

## 15. Open Questions

None for the canonical core catalog.

Future GDS phases may propose additions through explicit design change rather than informal invention.
