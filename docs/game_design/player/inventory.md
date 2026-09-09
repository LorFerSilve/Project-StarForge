# Player Inventory

> **Status:** Draft  
> **Authority:** Portable physical ownership, backpack storage, carried mass/volume, stack behavior, quick access, transfer rules, overflow prevention, and field resource state

## 1. Purpose

Player inventory supports exploration, salvage, mining, combat supplies, tools, and mission objectives while preventing unrealistic unlimited carrying.

## 2. Ownership

Inventory is a physical ownership location under the GDS-4 Resource Model.

An item/resource in Player Inventory does not simultaneously exist in station storage, ship cargo, or the world.

## 3. Inventory Structure

The player's portable inventory consists of:

- Equipped Slots;
- Backpack Storage;
- Quick Slots.

Equipped items remain owned by the player but are not counted as backpack slots.

They still count toward carried mass.

## 4. Capacity Model

Backpack capacity uses both:

- **Mass Capacity**;
- **Volume Capacity**.

An item/resource can be accepted only if both resulting limits remain within hard capacity.

This prevents dense/bulky resources from all behaving identically.

## 5. Hard Capacity

The player cannot exceed hard Mass or Volume Capacity through normal pickup/transfer.

When transfer would exceed either limit:

- transfer is rejected or partially accepted where stack splitting is possible;
- remainder stays at source.

## 6. Soft Load Threshold

A configurable fraction of Mass Capacity defines Heavy Load.

Heavy Load affects Movement but does not block carrying until hard capacity.

Volume has no separate soft-load penalty; it is a hard packing abstraction.

## 7. Backpack Progression

Backpacks are equipment.

Different backpacks can change:

- Mass Capacity;
- Volume Capacity;
- specialized storage;
- hazard containment;
- quick-access capability.

Progression is equipment-driven.

## 8. Stackable Resources

Bulk resources/components can stack according to Resource Catalog data.

Stacks preserve:

- Resource ID;
- quantity;
- perishability state where relevant;
- reservation/mission ownership flags.

## 9. Finished Items

Finished items generally occupy individual inventory entries because they can have:

- condition;
- ammunition;
- installed modifications;
- unique identity.

Identical pristine consumables may stack where content rules permit.

## 10. Quick Slots

The player has four Quick Slots.

Quick Slots reference eligible carried consumables/gadgets and do not duplicate items.

Using the item consumes/activates the same authoritative inventory object.

## 11. Equipped Items

Equipped objects occupy Equipment slots rather than backpack volume where their slot definition says so.

Their mass still contributes to total carried mass.

## 12. Mission Ownership State

Portable mission inventory distinguishes origin/security state:

- Secured Loadout — physical gear/resources that were already owned and committed from station/ship storage before mission deployment;
- Field-Unsecured — resources/items acquired during the current external mission and not yet secured;
- Vehicle/Extraction-Secured — mission-acquired resources successfully committed to an authorized extraction/vehicle store;
- Station-Secured — resources already returned to persistent station ownership.

A pickup acquired in the field enters Field-Unsecured unless a mission rule explicitly commits it directly to a valid secure extraction owner.

## 13. Ordinary Mission-Defeat Inventory Transaction

On ordinary external-mission defeat:

- persistent Secured Loadout equipment returns with the player, subject to condition damage;
- unconsumed Secured Loadout ammunition and consumables return with the player;
- ammunition/consumables already consumed during the mission remain consumed;
- Field-Unsecured resources/items are lost unless the specific mission defines a recoverable post-defeat state;
- Vehicle/Extraction-Secured resources remain secured and are not lost merely because the player is later defeated;
- Station-Secured resources are unaffected.

This transaction is deterministic and occurs exactly once.

## 14. Deliberately Dropped Loadout

A Secured Loadout item deliberately dropped into the world remains a persistent owned item only while the mission/location persistence system still tracks that object as recoverable.

If the mission ends in ordinary defeat while that item remains abandoned outside the player's recovered loadout, the item is treated as lost unless a mission-specific recovery rule preserves it.

This prevents deliberate dropping from duplicating protected gear.

## 15. Transfer to Station

At the home station, inventory can transfer to reachable station storage/logistics.

Transfer follows physical/logistics access rather than global teleportation unless a compatible intake interface exists.

## 16. Transfer to Ship

Inventory can transfer to compatible spacecraft cargo at an accessible cargo interface.

Ship cargo ownership is separate.

## 17. World Drop

The player may drop eligible items.

Dropping transfers ownership from Player Inventory to a world pickup/container at a valid nearby position.

Mission/world persistence determines how long dropped objects persist.

## 18. Protected Items

Some mission/narrative items may be non-droppable while required.

The UI explicitly marks this.

"Non-droppable" does not mean duplicated or massless unless the item definition says so.

## 19. Hazardous Materials

Hazard-tagged resources can require:

- compatible container;
- specialized backpack compartment;
- protective equipment.

Without required containment, pickup can be blocked or cause explicit exposure consequences.

## 20. Fluids and Gases

Bulk fluids/gases cannot normally be carried loose in backpack storage.

They require a compatible portable container item.

The container owns the fluid quantity.

## 21. Mining Output

Portable mining output enters a compatible backpack/container only while capacity exists.

No automatic station teleportation occurs.

## 22. Auto-Pickup

The baseline permits small automatic pickup convenience only for explicitly low-impact nearby consumables/ammunition where combat design later approves it.

Resources, strategic items, artifacts, and large components require explicit pickup/transfer.

## 23. Sorting

Inventory UI can sort/filter by:

- category;
- mass;
- value when economy exists;
- mission status;
- resource/item type.

Sorting changes presentation, not ownership.

## 24. Split Stack

The player can split stackable quantities for:

- transfer;
- dropping;
- storage.

Splitting conserves total quantity.

## 25. Merge Stack

Compatible stacks can merge when all gameplay-relevant stack properties are compatible.

Incompatible perishability/condition states do not silently overwrite data.

## 26. Reserved Items

Items reserved for a station/manufacturing job cannot normally be moved into Player Inventory unless reservation is explicitly released.

## 27. Inventory During Incapacitation

The inventory cannot be rearranged while Incapacitated unless a specific recovery UI permits management after mission resolution.

## 28. Inventory UI

The interface shows:

- carried Mass / Capacity;
- used Volume / Capacity;
- equipped gear;
- backpack contents;
- mission-security state;
- hazards;
- item condition;
- stack quantity;
- blockers.

## 29. Persistence

Inventory ownership, quantities, stack state, item condition, quick-slot references, and mission flags persist.

Save/load cannot duplicate split/transfer states.

## 30. Edge Cases

If a stack partially fits, only the maximum valid quantity transfers.

If removing the backpack would make current contents exceed replacement capacity, backpack removal is blocked until inventory is reduced.

If a container holding fluid is dropped, the fluid remains owned by that container.

If a Quick Slot item is consumed completely, the slot becomes empty rather than referencing a nonexistent item.

## 31. Explicit Non-Goals

Inventory does not provide:

- unlimited carrying;
- weightless tonnes of ore;
- global station inventory access in the field;
- duplicate Quick Slot items;
- loose backpack storage of arbitrary gases/fluids;
- mission failure refund of consumed supplies.

## 32. Tuneable Parameters

Tuneable values include base backpack mass/volume, soft-load threshold, stack limits, item mass/volume, and specialty container capacity.

## 33. Dependencies

This specification depends on GDS-4 Resource Model/Catalog, Equipment, Movement, Interaction, Missions, Spacecraft Cargo, Station Logistics, and Economy.

## 34. Open Questions

None in the portable-inventory baseline.
