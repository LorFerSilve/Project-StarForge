# Gathering and Salvage

> **Status:** Draft  
> **Authority:** Non-mining resource acquisition, world pickups, dismantling, salvage yields, container recovery, artifact recovery, and resource securing prior to extraction

## 1. Purpose

Gathering and salvage give expeditions multiple material-acquisition methods beyond drilling mineral deposits.

The system supports:

- exploration reward;
- derelict recovery;
- battlefield salvage;
- abandoned facilities;
- defeated machinery;
- resource containers;
- artifacts and data carriers.

## 2. Acquisition Methods

The baseline distinguishes:

- loose pickup;
- container looting;
- component recovery;
- dismantling;
- wreck salvage;
- artifact recovery;
- biological/sample collection.

Mining is authoritative in `mining.md`.

## 3. Loose Pickup

Portable physical resources/items can exist as world pickups.

Picking one up transfers authoritative ownership from World to Player/Container ownership.

A pickup cannot remain available in the world after successful transfer.

## 4. Containers

Containers can hold physical resources and items.

Opening a container reveals inventory according to:

- access;
- security;
- mission rules;
- generated/authored contents.

Viewing content does not transfer ownership.

## 5. Dismantling

Eligible machinery or structures can be dismantled when:

- the target is designated salvageable;
- the player has required tool/capability;
- access is safe enough;
- mission rules allow it.

Dismantling consumes time and produces authored salvage outputs.

## 6. No Universal Dismantling

The player cannot dismantle every wall, prop, or scenery object.

Only gameplay-designated salvage targets participate.

This avoids turning every environment into a voxel/resource-extraction problem.

## 7. Salvage Yield

A salvageable target defines:

- guaranteed recoverable materials;
- recoverable components;
- damage-sensitive yield;
- optional rare recovery;
- dismantling work requirement.

Destroyed condition can reduce recoverable yield.

## 8. Deterministic Core Yield

Core structural salvage is deterministic from the target and its condition.

Rare recoverable components may use authored probability where appropriate.

Randomness cannot create more material than the target's designed salvage envelope.

## 9. Damage Effect

A target damaged before salvage can lose:

- component recovery chance;
- recoverable material quantity;
- intact special parts.

This creates a tradeoff between destructive combat and careful capture/recovery.

## 10. Wreck Salvage

Large ship/station wrecks expose salvage nodes rather than requiring freeform cutting of every polygon.

Nodes may represent:

- reactor component;
- electronics bay;
- hull section;
- cargo hold;
- engine machinery;
- data core.

## 11. Defeated Robots

Destroyed robots can yield compatible:

- structural material;
- circuits;
- servos;
- sensors;
- specialized robot components.

Exact robot salvage tables belong to robot content but use canonical resources.

## 12. Defeated Organic Enemies

Organic enemies do not automatically drop metal crafting materials.

If biologically meaningful, they can yield:

- Alien Biomaterial;
- research samples;
- authored biological resources.

## 13. Enemy Equipment Drops

Enemies may leave physical equipment, ammunition, or components when plausible.

The game should not make every enemy explode into abstract colored crafting loot.

## 14. Artifact Recovery

Artifacts are special physical mission objects.

An artifact may:

- occupy cargo;
- require secure containment;
- be extracted;
- later be analyzed.

Analysis can produce Knowledge Assets or unlock research.

## 15. Data Carrier Recovery

A physical data carrier can contain:

- Research Data;
- blueprint fragment;
- faction intelligence;
- narrative records.

The Knowledge Asset is not granted until the applicable secure acquisition/analysis condition is met.

## 16. Biological Sample Collection

Biological collection requires a compatible tool/container where the sample definition requires it.

Improper collection can reduce or destroy the sample.

## 17. Tool Requirements

Gathering interactions can require:

- general interaction tool;
- cutting/salvage tool;
- scanner;
- secure sample container;
- powered dismantling tool.

Exact player tool mechanics belong to GDS-5.

## 18. Time and Exposure

Salvage work takes active game time.

The player can be interrupted by:

- enemies;
- environmental hazards;
- mission objectives;
- extraction pressure.

This prevents valuable salvage from being a zero-risk post-combat menu action by default.

## 19. Capacity

Recovered material must fit valid portable or vehicle cargo capacity.

Excess material remains at the source or cannot be recovered.

The game does not silently send expedition salvage directly to station storage.

## 20. Field-Unsecured State

Gathered resources remain Field-Unsecured until mission/extraction rules secure them.

Ordinary mission failure can cause loss of Field-Unsecured material.

## 21. Securing Cargo

A mission may provide ways to secure resource ownership before final extraction, such as:

- depositing into the player's landed ship;
- sending a cargo pod;
- using an extraction container.

Exact rules belong to missions/spacecraft.

## 22. Salvage Priority

When capacity is limited, the player chooses what to take.

This creates meaningful value decisions between:

- bulk material;
- rare components;
- artifacts;
- mission cargo;
- survival supplies.

## 23. Procedural Loot Rules

Procedural containers and salvage use authored loot tables.

A loot table must define:

- eligible resource set;
- quantity ranges;
- contextual weighting;
- rarity/chance for special components.

It cannot produce resources inconsistent with location/context without an explicit reason.

## 24. No Generic Loot Rarity Requirement

Resources themselves do not become Common/Rare/Epic variants.

A rare find is rare because:

- the resource is uncommon;
- the component is valuable;
- the location is dangerous;
- the loot-table probability is low.

## 25. Salvage Skill

The baseline does not require a separate player "Salvage Level" before basic salvage.

Better tools/technology can improve:

- speed;
- accessible target classes;
- component recovery;
- safety.

## 26. Automation

Remote mining/salvage automation is not assumed for ordinary expedition zones.

Specific deployable mission machinery may automate extraction when mission design explicitly supports it.

## 27. Persistence

Persistent mission locations preserve already-looted containers and dismantled persistent salvage targets according to mission/world persistence rules.

A save/load cannot regenerate the same deterministic container into duplicate resources.

## 28. Edge Cases

If player inventory fills during dismantling, output remains at the salvage target/local container rather than disappearing or teleporting.

If a target is destroyed while being salvaged, remaining yield recalculates from the new condition.

If a data carrier is picked up but mission fails before securing it, knowledge is not automatically unlocked unless analysis/secure transfer already occurred.

If an artifact container is breached and its definition requires containment, the artifact can become damaged/lost.

## 29. Visual Feedback

Salvage targets clearly communicate:

- whether salvageable;
- required tool;
- estimated recoverable categories when scanned;
- progress;
- blocked reason.

## 30. Audio Feedback

Dismantling, container access, sample collection, and successful recovery use distinct feedback tied to actual progress.

## 31. Explicit Non-Goals

The system does not provide:

- automatic vacuuming of an entire map into inventory;
- universal prop dismantling;
- direct station teleportation of field loot;
- resource rarity recolors;
- unlimited carrying.

## 32. Tuneable Parameters

Tuneable values include salvage duration, yield quantities, condition modifiers, container quantity ranges, rare-component chance, and tool efficiency.

## 33. Dependencies

This specification depends on Resource Model, Resource Catalog, missions, player inventory/tools, spacecraft cargo, robots, combat, and world persistence.

## 34. Open Questions

None in the current gathering/salvage baseline.

The document remains Draft until mission extraction, player tools/inventory, and world persistence are defined.
