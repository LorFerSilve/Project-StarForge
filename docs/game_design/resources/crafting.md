# Crafting

> **Status:** Draft  
> **Authority:** Component/finished-item recipe rules, station vs field crafting boundary, canonical component recipes, cancellation semantics, and crafting unlock requirements

## 1. Purpose

Crafting converts refined materials and components into usable components, consumables, and finished items.

Industrial crafting is executed by station manufacturing infrastructure.

## 2. Crafting Definition

A Crafting Recipe defines:

- Recipe ID;
- output item/resource;
- output quantity;
- required physical inputs;
- compatible fabrication class;
- duration/work;
- power;
- heat;
- prerequisite blueprint/research;
- optional specialist requirement;
- byproducts where meaningful.

## 3. Station Crafting

Most permanent progression objects require station fabrication.

Examples:

- station components;
- ship components;
- robot parts;
- advanced electronics;
- ammunition;
- rations;
- high-tier equipment.

The manufacturing system owns job execution.

## 4. Field Crafting

Field crafting is deliberately limited.

Portable crafting can create only explicitly approved emergency/field items such as:

- basic repair consumable;
- simple ammunition where equipment permits;
- emergency seal/patch;
- selected mission utility.

Field crafting cannot produce:

- station modules;
- ship engines;
- reactors;
- large robot chassis;
- industrial machine modules.

GDS-5 owns the player interaction/tool interface.

## 5. Recipe Unlock

A recipe can be:

- Known;
- Discovered but Locked;
- Unlocked;
- Deprecated by design change.

Only Unlocked recipes can be executed.

## 6. Blueprint Relationship

A Blueprint is the authoritative design knowledge for a craftable/buildable object.

A recipe can require a Blueprint Unlock plus researched prerequisite technologies.

Having materials alone never reveals or bypasses an unknown design.

## 7. Canonical Core Component Recipes

Exact balancing quantities are tuneable, but required material identities are fixed by this baseline.

### RECIPE_STRUCTURAL_FRAME

Inputs:
- Structural Steel

Output:
- Structural Frame

### RECIPE_PRESSURE_SEAL

Inputs:
- Polymer Resin
- Aluminum

Output:
- Pressure Seal Assembly

### RECIPE_BASIC_CIRCUIT

Inputs:
- Copper
- Electronic-Grade Silicon

Output:
- Basic Circuit

### RECIPE_ADVANCED_CIRCUIT

Inputs:
- Basic Circuit
- Platinum
- Electronic-Grade Silicon

Output:
- Advanced Circuit

### RECIPE_POWER_REGULATOR

Inputs:
- Copper
- Basic Circuit
- Aluminum

Output:
- Power Regulator

### RECIPE_MAGNETIC_COIL

Inputs:
- Copper
- Structural Steel

Output:
- Magnetic Coil

### RECIPE_SUPERCONDUCTIVE_COIL

Inputs:
- Superconductive Material
- Advanced Circuit

Output:
- Superconductive Coil

### RECIPE_SERVO

Inputs:
- Aluminum
- Copper
- Basic Circuit

Output:
- Servo Assembly

### RECIPE_SENSOR

Inputs:
- Technical Glass
- Advanced Circuit
- Platinum

Output:
- Sensor Package

### RECIPE_HEAT_EXCHANGER

Inputs:
- Copper
- Titanium

Output:
- Heat Exchanger Core

### RECIPE_QUANTUM_SUBSTRATE

Inputs:
- Voltaic Crystal
- Superconductive Material
- Exotic Matter Sample

Output:
- Quantum Substrate

Technology:
- Quantum Materials

### RECIPE_QUANTUM_PROCESSOR

Inputs:
- Advanced Circuit
- Platinum
- Quantum Substrate

Output:
- Quantum Processor

Technology:
- Quantum Computing

### RECIPE_RATIONS

Inputs:
- Fresh Produce
- Polymer Resin

Output:
- Packaged Rations

## 8. Finished Equipment Recipes

Weapon, armor, ship, and robot domains own the identity/specification of finished equipment.

Their recipes are registered in GDS-4 and must use canonical resources/components.

Those future domains may not invent undefined crafting materials without updating Resource Catalog.

## 9. Recipe Quantity Rules

Quantities are tuneable balancing parameters.

Material identities and required technology relationships are fixed design.

This allows balancing without redesigning the entire resource graph.

## 10. Component Reuse

Core components should be reused across multiple systems where mechanically sensible.

Examples:

- Basic Circuit in doors, tools, early machines;
- Servo Assembly in robots and turrets;
- Sensor Package in ships, station sensors, robots;
- Superconductive Coil in shields, fusion systems, advanced engines.

This creates understandable resource demand.

## 11. No Universal Component

No single "Tech Part" is used as a substitute for all advanced electronics.

Specific canonical components provide readable production dependencies.

## 12. Crafting Execution

Station Manufacturing owns:

- reservations;
- input delivery;
- machine queue;
- power/heat requirements;
- output buffer;
- cancellation;
- persistence.

Crafting does not define a parallel competing queue system.

## 13. Cancellation

Cancellation follows station Manufacturing:

- unconsumed inputs are released;
- consumed inputs do not reappear;
- unfinished salvage exists only if recipe explicitly permits it.

## 14. Manual Assistance

Where a fabricator supports it, player/crew assistance can modify processing speed through Manufacturing rules.

Assistance cannot reduce required material to zero or bypass technology.

## 15. Crafting Failure

Standard recipes do not randomly fail while all defined conditions are satisfied.

Failure comes from explicit system states such as:

- machine fault;
- power loss;
- heat;
- damage;
- missing input;
- interrupted hazardous process.

## 16. Batch Crafting

Identical consumables/components can be crafted in batches.

A batch still respects exact total resource consumption and storage capacity.

## 17. Repair vs Craft

Repairing an existing object and crafting a new object are distinct actions.

Repair may consume a subset of materials/components based on damage.

Crafting creates a new finished object.

## 18. Disassembly

Finished objects are not universally reversible into their full recipe inputs.

If dismantling is allowed, salvage rules determine recoverable output based on object and condition.

## 19. UI

Crafting UI shows:

- recipe;
- output;
- required quantity;
- available/accessible/reserved inputs;
- machine;
- technology/blueprint requirement;
- duration;
- current blockers.

## 20. Edge Cases

If a blueprint is unlocked mid-queue elsewhere, existing valid queues can use it only after unlock state is committed.

If an input becomes reserved by an earlier higher-priority job, this job waits.

If output inventory fills, output remains in machine buffer.

If a future content domain proposes a recipe using a nonexistent material, the resource catalog must be updated first.

## 21. Explicit Non-Goals

Crafting does not provide:

- instant industrial menu fabrication;
- materials-free crafting;
- random recipe success chance;
- universal Tech Parts;
- full material refunds on disassembly;
- field construction of giant station/ship systems.

## 22. Tuneable Parameters

Tuneable values include recipe quantities, batch size, processing duration, and energy demand.

## 23. Dependencies

This specification depends on Resource Catalog, Processing, Blueprints, Research, station Manufacturing, player field tools, and future equipment/ship/robot domains.

## 24. Open Questions

None in the current core crafting baseline.

Finished weapon, ship, robot, and armor recipes remain intentionally owned by their future content domains and must register only canonical resources/components.
