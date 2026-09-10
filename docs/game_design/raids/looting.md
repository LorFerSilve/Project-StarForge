# Raid Looting

> **Status:** Draft  
> **Authority:** Raid loot sources, physical transfer, target inventory depletion, data/knowledge handling, theft from Horizon Station, cargo security, partial loot, and anti-duplication rules

## 1. Purpose

Raid loot must be a physical logistics problem rather than an abstract reward percentage.

The player can only keep what is actually:

- found;
- transferred;
- carried;
- secured;
- extracted.

The same rule applies when enemies steal from Horizon Station.

## 2. Loot Categories

Baseline raid loot categories:

1. Bulk Resources;
2. Manufactured Components;
3. Ammunition/Consumables;
4. Weapons/Equipment;
5. Robot/Ship Components;
6. Artifacts / Unique Physical Objects;
7. Data Carriers;
8. Mission-Critical Cargo;
9. Salvage from destroyed systems;
10. Captured intact equipment where allowed.

## 3. Physical Loot Source

Every physical loot quantity originates from one authoritative owner such as:

- storage container;
- local magazine;
- cargo hold;
- work-in-progress;
- installed component;
- defender inventory;
- world object;
- destroyed-system salvage state.

Opening a loot interface does not create a second copy.

## 4. Target Storage

Raid-relevant target storage has finite current contents.

Once a quantity is removed:

- source quantity decreases;
- destination quantity increases;
- target persistent state reflects depletion.

Reload/re-enter does not refill it for the same target state.

## 5. Generated Target Loot

Procedural raid target major loot is determined by persistent target/Mission seed and world/economic state at creation.

Reloading does not reroll a better stockpile.

## 6. Loot Discovery

The player may know:

- exact container;
- approximate department;
- suspected valuable storage;
- nothing

depending on Intel/Exploration.

Loot does not glow through all walls by default.

## 7. Player Backpack Loot

Portable loot transferred to Player Inventory during the raid is normally:

**Field-Unsecured**

until GDS-8 extraction security commits.

## 8. Ship Cargo Loot

Loot physically transferred into valid player ship cargo can become:

**Vehicle/Extraction-Secured**

according to GDS-6/GDS-8.

It remains physically owned by the ship cargo hold.

## 9. No Automatic Ship Transfer

Being inside a raided station connected to the player's ship does not automatically move all selected loot to the ship.

Transfer requires:

- player carry;
- logistics-capable robot;
- target/player cargo system;
- physical conveyor/loading interface;
- authored transfer mechanism.

## 10. Logistics Robots

A compatible GDS-10 robot can move loot only when it has:

- cargo/manipulator capability;
- path;
- authorization;
- capacity;
- energy.

Robot cargo becomes an intermediate physical owner.

## 11. Heavy Loot

Some objects cannot fit in player backpack.

Examples:

- large reactor component;
- turret assembly;
- heavy artifact container;
- disabled robot/wreck.

Recovery requires:

- manipulator;
- trolley/cargo device;
- robot;
- ship cargo interface;
- appropriate bay.

## 12. Partial Loot

The player may take part of a stack/container.

The remaining quantity stays at the target.

A full container does not vanish merely because one item was removed.

## 13. Looting Time

Loot transfer is not universally instant.

Time/throughput can depend on:

- item size;
- quantity;
- interface;
- distance;
- carrier;
- security container;
- logistics system.

Exact timings are tuneable.

## 14. Secure Containers

A secure container can require:

- credential;
- hacking;
- physical breach;
- power/control;
- key;
- destructive opening.

Security protects access, not magical ownership.

## 15. Destroying Containers

Destroying a storage container can:

- damage contents;
- scatter contents;
- destroy fragile items;
- expose hazardous material.

It is not guaranteed to produce all contents intact.

## 16. Ammunition Loot

Compatible ammunition remains a physical GDS-9 finished item.

Target local magazines can be:

- looted;
- destroyed;
- isolated.

Removing rounds both denies defender resupply and creates loot if successfully extracted.

## 17. Installed Component Theft

An installed component can become loot only through a valid removal/dismantling interaction.

Sequence:

Installed Target Component  
→ removal completes  
→ target system loses component  
→ physical item enters carrier/container.

The component cannot remain installed and also be looted.

## 18. Dismantling for Salvage

Large installed systems can be dismantled after combat only if:

- accessible;
- tool/capability exists;
- enough time;
- target state allows it.

Salvage yields follow GDS-4.

## 19. Tactical Tradeoff

Extended looting increases exposure to:

- reinforcements;
- defender repair;
- ship attack;
- ammo/energy use;
- environmental hazards.

There is no artificial loot timer required; systemic pressure provides the cost.

## 20. Unique Physical Objects

A unique artifact/data device uses one persistent identity.

Once removed from target, it does not respawn in the original room.

If lost later, world/mission recovery rules determine its location/state.

## 21. Data Carriers

A physical data carrier can contain:

- Blueprint data;
- Research Evidence source;
- route intelligence;
- faction records;
- story evidence.

The carrier remains physical until secured/analyzed.

## 22. Knowledge Commit

Recovering a data carrier does not necessarily instantly create permanent Knowledge Assets.

The relevant GDS-4 analysis/reward rule defines when:

- Blueprint Unlock;
- Research Evidence;
- route data

becomes persistent knowledge.

## 23. Already Known Knowledge

Stealing a second copy of already-known data does not duplicate permanent knowledge unlocks.

The physical carrier may still have economic/story value if defined.

## 24. No Knowledge Theft From Player Memory

During defensive raids, attackers cannot remove already committed:

- Blueprint Unlocks;
- Research;
- Research Evidence;
- discovered route knowledge.

They can steal a physical unanalyzed data carrier or hardware storing not-yet-committed data if such an object exists.

## 25. Mission Objective Cargo

A raid can require a specific item to be:

- acquired;
- Vehicle/Extraction-Secured;
- delivered.

The objective explicitly states which state is required.

## 26. Loot and Raid Success

Optional loot does not determine raid success unless an objective requires it.

The player can succeed with minimal loot or retreat with loot after failing the primary objective.

## 27. Failure Before Extraction

Field-Unsecured raid loot follows GDS-8 failure rules.

Already Vehicle/Extraction-Secured ship cargo remains with the actual surviving ship owner unless an explicit interception/destruction event changes it.

## 28. Loot Left Behind

Unsecured loot abandoned in a persistent target remains in its current owner/location if the world retains that object.

One-shot procedural instance closure can resolve unretained loose loot according to GDS-8 without granting it to player.

## 29. Enemy Theft From Horizon Station

Enemy raiders must physically:

1. breach/reach storage;
2. gain access;
3. transfer resources into attacker carrier/cargo;
4. transport the loot back toward extraction;
5. successfully depart with it.

The game does not subtract a percentage of station stock because defense failed.

## 30. Attacker Cargo Capacity

Hostile raiders have finite carrying/cargo capacity.

They cannot steal more than:

- target available quantity;
- accessible transfer throughput;
- remaining carrying capacity;
- time before withdrawal/destruction.

## 31. Interrupted Enemy Theft

If an enemy carrier holding stolen station resources is:

- destroyed;
- disabled;
- captured;
- forced to drop cargo

before successful extraction, the resources remain physically recoverable rather than already lost to the world.

## 32. Enemy Extraction Commit

Station resources become permanently removed from player ownership only when the hostile extraction owner successfully commits departure/escape under the defensive raid rules.

## 33. Loot Priority

Hostile raid profiles can prioritize:

- high-value compact goods;
- ammunition;
- refined materials;
- advanced components;
- food/medical supplies;
- mission-specific strategic target.

They do not have omniscient knowledge of hidden storage unless Intel/security breach provides it.

## 34. Protected Strategic Resources

Player station policies can mark resources protected/restricted.

This affects:

- storage location;
- access/security;
- automation permissions.

It does not create theft immunity if attackers physically breach the protection.

## 35. Loot Destruction

A raid may destroy resources instead of stealing them through:

- explosion;
- fire;
- decompression loss;
- container destruction;
- hazardous contamination.

Destruction must be physically caused.

## 36. Captured Enemy Equipment

Intact enemy weapons/robot parts/ship parts can be loot only where compatible with item/resource rules.

Unknown technology may require research before use.

## 37. Overcapacity

If player destination cannot hold additional loot:

- transfer blocks or is partial;
- excess remains at source/carrier;
- nothing is deleted.

## 38. Cargo Jettison

The player can jettison raid loot under GDS-6.

Jettisoned loot loses secure ownership and becomes a world object according to mission/world rules.

## 39. Loot Summary

Raid result presentation later separates:

- physically extracted loot;
- unresolved/unsecured loot;
- lost/destroyed loot;
- sponsor Resolution Rewards;
- enemy theft during defensive raid.

## 40. Persistence

Target inventory quantities, opened containers, removed components, stolen cargo, player/attacker carrier ownership, and unique item state persist through valid saves.

## 41. Edge Cases

If a player removes a component but dies before extracting it, the target remains missing the component if that physical world state persists; the player does not gain the item unless it was secured.

If enemy raiders steal a resource into a boarding craft but the craft is destroyed outside Horizon Station, the resource can become recoverable wreck cargo rather than being magically returned to storage.

If a target container was looted in a prior raid and has not undergone explicit replenishment, it remains depleted.

If a unique data carrier is secured in ship cargo before player defeat, its security follows actual ship outcome rather than being duplicated back at the target.

## 42. Explicit Non-Goals

Raid looting does not use:

- abstract percentage theft;
- automatic mass transfer;
- infinite loot respawn;
- duplicate installed components;
- unlimited hostile cargo;
- automatic permanent knowledge loss;
- loot rarity inflation disconnected from actual items/resources.

## 43. Tuneable Parameters

Transfer speed, hostile carrying capacity, container security, salvage yields, target stock distributions, fragile-content damage, and loot priority weights are tuneable.

## 44. Dependencies

This specification depends on GDS-4 ownership/salvage, GDS-5 Inventory, GDS-6 Cargo, GDS-8 Extraction/Rewards/Failure, GDS-10 robot cargo/recovery capability, Sabotage, Escalation, and Station Defense Resolution.

## 45. Open Questions

None in the raid-looting baseline.
