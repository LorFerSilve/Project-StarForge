# Resource Processing

> **Status:** Design Complete  
> **Authority:** Canonical raw-to-refined transformations, process classes, efficiencies, byproducts, industrial requirements, and processing conservation rules

## 1. Purpose

Processing converts raw expedition resources into standardized industrial materials.

It forms the first industrial stage between acquisition and manufacturing.

## 2. Processing vs Crafting

Processing transforms bulk/raw material into refined material or standardized industrial feedstock.

Crafting/manufacturing combines refined resources/components into components or finished items.

Examples:

Iron Ore → Refined Iron is Processing.  
Refined Iron + Carbonaceous Feedstock → Structural Steel is Processing.  
Structural Steel → Structural Frame is Crafting/Manufacturing.

## 3. Process Definition

Every process contains:

- Process ID;
- required inputs;
- input quantities;
- outputs;
- output quantities;
- compatible machine class;
- processing duration/work;
- power demand;
- heat load;
- optional water/fluid requirement;
- waste/byproducts;
- blueprint/research requirement.

## 4. Conservation

Processing follows the Resource Model conservation rule.

Efficiency losses are explicitly represented by lower output yield and/or defined waste.

The system cannot create extra valuable material through rounding or machine interruption.

## 5. Processing Efficiency

A process has a baseline recovery efficiency.

Technology/machine upgrades can improve efficiency only up to an authored maximum.

Efficiency can improve output per raw input, but cannot exceed physically abstracted process limits.

## 6. Batch Resolution

Input is consumed according to the process definition.

Output is generated exactly once when the batch reaches its completion point.

Station Manufacturing owns queue/state execution.

## 7. Canonical Baseline Processes

### PROC_IRON_REFINING

Input:
- Iron Ore

Output:
- Refined Iron

Machine:
- Smelter/Refinery

### PROC_BAUXITE_REFINING

Input:
- Bauxite

Output:
- Aluminum

Machine:
- Refinery

### PROC_COPPER_REFINING

Input:
- Copper Ore

Output:
- Copper

Machine:
- Smelter/Refinery

### PROC_SILICON_REFINING

Input:
- Silica

Output:
- Electronic-Grade Silicon

Machine:
- Chemical/Electronics-grade processor

### PROC_GLASS_PRODUCTION

Input:
- Silica

Output:
- Technical Glass

Machine:
- High-temperature processor

### PROC_TITANIUM_REFINING

Input:
- Titanium Ore

Output:
- Titanium

Machine:
- Advanced Refinery

### PROC_TUNGSTEN_REFINING

Input:
- Tungsten Ore

Output:
- Tungsten

Machine:
- High-temperature Refinery

### PROC_PLATINUM_REFINING

Input:
- Platinum Ore

Output:
- Platinum

Machine:
- Advanced Refinery

### PROC_URANIUM_FUEL

Input:
- Uraninite

Output:
- Uranium Fuel Rod

Machine:
- Nuclear Materials Processor

Hazard:
- Radioactive

### PROC_STEEL

Input:
- Refined Iron
- Carbonaceous Feedstock

Output:
- Structural Steel

Machine:
- Smelter/Alloy Processor

### PROC_POLYMER

Input:
- Carbonaceous Feedstock

Output:
- Polymer Resin

Machine:
- Chemical Processor

### PROC_CARBON_COMPOSITE

Input:
- Carbonaceous Feedstock
- Polymer Resin

Output:
- Carbon Composite

Machine:
- Composite Processor/Fabricator

### PROC_SUPERCONDUCTOR

Input:
- Copper
- Titanium
- authored advanced process consumables

Output:
- Superconductive Material

Machine:
- Advanced Materials Processor

Technology:
- Superconductive Materials Research

### PROC_ICE_TO_WATER

Input:
- Water Ice

Output:
- Fresh Water

Machine:
- Water Processor

### PROC_DEUTERIUM_SEPARATION

Input:
- Fresh Water

Output:
- Deuterium

Machine:
- Isotope Separator

Technology:
- Fusion Fuel Handling

### PROC_FUSION_FUEL

Input:
- Helium-3
- Deuterium

Output:
- Fusion Fuel Pellet

Machine:
- Fusion Fuel Processor

Technology:
- Fusion Fuel Handling

### PROC_COOLANT

Input:
- Fresh Water
- Polymer Resin

Output:
- Industrial Coolant

Machine:
- Chemical Processor

### PROC_PROPELLANT

Input:
- Fresh Water

Output:
- Reaction Propellant

Machine:
- Electrolysis / Propellant Processor

### PROC_NUTRIENT_MIX

Input:
- Fresh Produce
- Fresh Water
- Carbonaceous Feedstock

Output:
- Nutrient Mix

Machine:
- Chemical/Food Processor

The recipe represents controlled recovery of agricultural biomass plus imported mineral/carbon feedstock. It is intentionally lossy so agriculture does not create a closed infinite resource loop.

### PROC_MEDICAL_COMPOUND

Input:
- Fresh Produce
- Fresh Water
- Polymer Resin

Output:
- Medical Compound

Machine:
- Medical/Chemical Processor

Alien Biomaterial is reserved for advanced medical research/finished treatments rather than the standard Medical Compound recipe.

## 8. Process Unlocking

Common primitive processes are available from early progression.

Advanced processes require:

- research;
- blueprint;
- specialist knowledge;
- advanced machinery.

Exact unlock ownership belongs to Blueprints and Research.

## 9. Hazardous Processing

Hazardous processes can require:

- specialized machine;
- containment;
- qualified facility;
- safe storage.

Uranium and exotic processing cannot run in a generic basic fabricator.

## 10. Water Processing

Water processing converts Water Ice to Fresh Water.

Wastewater recycling is station-water infrastructure, not a mining processing recipe.

This avoids duplicate authority.

## 11. Food Processing Boundary

Crop growth is station farming.

Converting Fresh Produce into Packaged Rations is a crafting/manufacturing recipe.

## 12. Exotic Processing

Voltaic Crystal can be processed only after its relevant technology is understood.

Alien Biomaterial, Exotic Matter Sample, and Ancient Technology Core are primarily strategic/research inputs rather than bulk-smelted resources.

Any destructive analysis must explicitly state whether the sample is consumed.

## 13. Byproducts

A process defines byproducts only when they matter mechanically.

The game does not create dozens of useless waste resources solely for realism.

Untracked industrial waste is part of efficiency abstraction.

## 14. Processing Queues

Machine queue behavior is not redefined here.

Station Manufacturing owns:

- reservation;
- waiting;
- processing;
- output blocking;
- interruption;
- persistence.

This document owns the transformation recipe.

## 15. Processing UI

Recipe information displays:

- inputs;
- outputs;
- machine requirement;
- efficiency;
- duration;
- power/heat;
- technology lock;
- hazards.

## 16. Edge Cases

If machine efficiency changes mid-batch, the process uses the batch rule defined by manufacturing; it cannot retroactively duplicate output.

If output storage is blocked, completed output remains in the machine buffer.

If a hazardous input loses containment, damage/event systems resolve it rather than silently continuing processing.

If input quality does not exist as a resource property, machines cannot invent a random quality modifier.

## 17. Progression

Early game:
- common ore refining;
- water processing;
- basic steel/polymers.

Mid game:
- titanium/tungsten/platinum;
- nuclear handling;
- advanced components;
- improved recovery.

Late game:
- superconductors;
- fusion fuel;
- exotic material handling;
- advanced strategic processes.

## 18. Explicit Non-Goals

Processing does not provide:

- alchemy-style arbitrary conversion between unrelated resources;
- random output rarity;
- undefined universal recycler converting anything into anything;
- byproduct spam without gameplay purpose;
- processing without compatible machinery.

## 19. Tuneable Parameters

Tuneable values include input/output ratios, efficiency caps, process durations, power demand, heat load, and waste fractions.

## 20. Dependencies

This specification depends on Resource Catalog, station Manufacturing, Power, Thermal, Water, Research, Blueprints, and economy/balance.

## 21. Open Questions

None in the current processing baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
