# Resource Model

> **Status:** Draft  
> **Authority:** Resource identity, physical representation class, ownership, quantity, reservation semantics, loss/conservation rules, and distinction between resources, items, knowledge, and currencies

## 1. Purpose

The resource model defines what the game means by a resource and prevents different systems from inventing incompatible inventory behavior.

It is the authoritative basis for:

- mining;
- salvage;
- processing;
- crafting;
- station construction;
- repairs;
- manufacturing;
- food;
- fuel;
- ammunition;
- research inputs;
- mission extraction.

## 2. Resource Classes

Project StarForge distinguishes four fundamentally different classes.

### Physical Bulk Resource

Measured primarily by quantity.

Examples:

- Iron Ore;
- Water Ice;
- Refined Copper;
- Coolant.

### Physical Discrete Component

Counted as individual manufactured or recovered units.

Examples:

- Basic Circuit;
- Magnetic Coil;
- Quantum Processor;
- Pressure Seal Assembly.

### Physical Finished Item

A complete usable object with its own item identity.

Examples:

- weapon;
- tool;
- armor piece;
- robot component assembly;
- ship module.

Finished-item behavior is owned by the relevant equipment/ship/robot domain. GDS-4 owns the recipe that creates it.

### Knowledge Asset

Non-physical progression information.

Examples:

- Blueprint;
- Research Data;
- Analyzed Technology Record.

Knowledge Assets do not occupy cargo volume unless the source is still contained in an unanalyzed physical artifact.

## 3. No Generic Universal Currency Resource

Credits, faction currencies, or economic tender are not treated as physical crafting resources by default.

The economy domain will own monetary value.

A crafting recipe cannot substitute credits for missing physical inputs unless an explicit fabrication service rule later allows purchasing the missing material.

## 4. Resource Definition

Every physical resource definition contains:

- stable Resource ID;
- display name;
- resource class;
- material/category tags;
- storage compatibility;
- base stack/quantity unit;
- mass abstraction where relevant;
- volume abstraction where relevant;
- hazard tags;
- perishability;
- physical state abstraction;
- known acquisition sources;
- known recipe uses.

## 5. Stable Identity

Resource IDs are stable design identifiers.

Renaming display text does not silently create a new resource.

Save data, recipes, loot tables, logistics, and manufacturing all reference stable IDs.

## 6. Quantity

Bulk resources use continuous or discretized quantity units according to resource definition.

Discrete components use integer count.

The game never allows negative resource quantity.

## 7. Ownership

Every physical resource quantity has exactly one authoritative ownership location at any instant.

Examples:

- world deposit;
- world pickup/container;
- player inventory;
- ship cargo;
- station storage;
- machine buffer;
- transfer state;
- construction delivery;
- destroyed/lost state.

The same quantity cannot exist in two locations simultaneously.

## 8. Conservation Rule

Physical resources are conserved except when an explicit gameplay process:

- consumes them;
- transforms them;
- destroys them;
- leaks them;
- discards them.

Save/load, transfer, reservation, UI aggregation, and network reconnection never create duplicate quantity.

## 9. Reservation

Reservation does not move or consume a physical resource.

It marks an owned quantity as promised to a specific valid claimant.

Examples:

- construction job;
- manufacturing job;
- repair;
- research project;
- mission loadout.

Reserved quantity remains physically at its owner until transferred or consumed.

## 10. Consumption

A process consumes input only at the authoritative consumption point defined by its recipe or action.

Cancelling before consumption releases reservation.

Cancelling after consumption does not recreate consumed input unless the process explicitly returns recoverable material.

## 11. Transformation

A valid recipe transforms defined inputs into defined outputs.

The transformation obeys:

`Consumed Inputs = Created Outputs + Explicit Waste/Loss/Byproducts according to recipe abstraction`

The game need not be chemically exact, but it cannot silently produce arbitrary valuable material from nothing.

## 12. Quality

The baseline does **not** use random material quality tiers such as Poor/Common/Rare/Epic ore.

Resource identity is deterministic.

Variation in extraction value comes from:

- deposit richness;
- accessible quantity;
- contamination/waste;
- location difficulty;
- processing technology.

This avoids inventory clutter from many nearly identical material stacks.

## 13. Purity

Raw deposits may have a **Yield Grade** describing how much canonical resource is recovered per unit of extraction work.

Yield Grade belongs to the deposit, not to a separate item rarity version of the ore.

Processed output uses the canonical material definition.

## 14. Perishability

Only explicitly perishable resources track spoilage.

Baseline perishable resource:

- Fresh Produce.

Spoilage state belongs to the actual stored quantity and is affected by storage conditions.

Most metals, components, and minerals do not decay over time.

## 15. Hazard Tags

Resources can require special storage through tags such as:

- Radioactive;
- Cryogenic;
- Pressurized;
- Reactive;
- Biohazard;
- High-Energy;
- Secure Strategic.

A hazard tag has no effect unless an owning subsystem defines the corresponding handling requirement.

## 16. Physical State Abstraction

Resources may be categorized for storage as:

- Solid Bulk;
- Fluid;
- Gas;
- Discrete Component;
- Packaged Consumable.

The game does not require thermodynamic phase simulation for every resource.

A resource changes category only through an explicit process.

## 17. Portable vs Industrial Scale

Player inventory is intended for portable quantities and items.

Large industrial quantities primarily travel through:

- ship cargo;
- station logistics;
- containers;
- machinery.

The resource model does not imply that a player can carry tonnes of ore because it is represented by a number.

Exact player carry constraints belong to GDS-5.

## 18. Secured vs Field-Held Resources

Resources acquired in a mission can be in one of these ownership states:

- Field-Unsecured;
- Vehicle/Extraction-Secured;
- Station-Secured.

Mission and extraction rules define the transition.

A Field-Unsecured quantity can be lost on mission failure according to global rules.

## 19. Knowledge Assets

Knowledge Assets are account/save progression state with authoritative provenance.

They include:

### Blueprint Unlock

Permission to manufacture/build a specific design.

### Research Data

Scientific/technical information used by research projects.

### Analyzed Technology Record

Persistent knowledge produced by analyzing artifacts or recovered technology.

Knowledge Assets:

- do not occupy normal cargo storage;
- cannot be duplicated through item transfer;
- persist once successfully secured/recorded according to research rules.

## 20. Physical Data Carriers

A data drive, artifact, alien device, or black box can be a physical item before analysis.

Until successfully extracted and analyzed, it remains a physical mission object.

Analysis can produce a Knowledge Asset.

The physical carrier may remain, be consumed, or become inert according to its analysis recipe.

## 21. Resource Discovery

The player can encounter a resource before fully understanding all of its uses.

Discovery and analysis can reveal:

- name;
- hazard;
- processing method;
- recipe relevance.

Unknown resource identity must still use a stable hidden Resource ID internally.

## 22. Resource UI

Resource displays communicate where relevant:

- name;
- quantity;
- category;
- storage requirement;
- reservation;
- accessibility;
- hazard;
- known sources;
- known uses.

The UI must distinguish accessible from isolated inventory.

## 23. Persistence

Physical ownership, quantity, reservation, perishability, and transfer state persist or reconstruct deterministically.

Knowledge Assets persist independently of physical inventory after their acquisition conditions are satisfied.

## 24. Edge Cases

If a storage owner is destroyed, its resource state follows explicit cargo-loss/salvage rules.

If a resource is reserved and the owning container becomes inaccessible, reservation remains valid but unavailable until the claimant or logistics system resolves it.

If the source is destroyed before transfer, untransferred quantity cannot be delivered.

If a Knowledge Asset is unlocked, deleting the original analyzed physical artifact does not erase learned knowledge unless an explicit narrative mechanic says otherwise.

## 25. Explicit Non-Goals

The model does not use:

- random ore rarity colors;
- duplicated global inventory ownership;
- negative quantities;
- free conversion of currency into material inside recipes;
- arbitrary decay for metals/components;
- one inventory representation for both knowledge and physical matter.

## 26. Tuneable Parameters

Tuneable values include resource mass/volume abstraction, stack limits, spoilage rate, storage efficiency, and deposit-yield parameters.

## 27. Dependencies

This specification is authoritative for all GDS-4 documents and is consumed by station logistics, manufacturing, missions, player inventory, spacecraft cargo, economy, robots, raids, and progression.

## 28. Open Questions

None in the current resource-model baseline.

The document remains Draft until player inventory, mission extraction, economy, and spacecraft cargo are cross-validated.
