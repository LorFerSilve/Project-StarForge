# Player Inventory

> **Status:** Design Complete  
> **Authority:** Portable physical ownership, backpack storage, carried Mass/Volume, stack behavior, quick access, transfer rules, auto-pickup eligibility, overflow prevention, and mission-security state

## 1. Purpose

Player Inventory supports exploration, salvage, mining, combat supplies, tools, and mission objectives while preserving physical ownership and finite carrying capacity.

## 2. Ownership

Player Inventory is a physical ownership location under the GDS-4 Resource Model.

A quantity/item owned by Player Inventory does not simultaneously exist in station storage, ship cargo, another container, or the world.

All transfers are atomic ownership changes.

## 3. Inventory Structure

The player's portable inventory consists of:

- Equipped Slots;
- Backpack Storage;
- Quick Slots.

Equipped items remain player-owned. They do not consume backpack Volume when their equipment slot says so, but their Mass always contributes to carried Mass.

Quick Slots are references to eligible player-owned inventory/equipment objects and never duplicate them.

## 4. Capacity Model

Backpack acceptance uses both:

- **Mass Capacity**;
- **Volume Capacity**.

A normal pickup/transfer commits only when the resulting inventory remains within both hard limits and all containment rules pass.

## 5. Hard Capacity

The player cannot exceed hard Mass or Volume Capacity through normal gameplay transfer.

For a divisible stack:

- the maximum valid quantity may transfer;
- all remainder stays at the source.

For an indivisible item:

- the whole transfer either commits or does not commit.

## 6. Heavy Load Threshold

Backpack/equipment data defines a tuneable soft Mass threshold below hard Mass Capacity.

At or above that threshold the player enters `LoadState::Heavy`.

Heavy Load effects are authoritative in Movement:

- no sprint;
- no normal jump;
- no mantle;
- reduced walking/crouching acceleration/speed.

Volume has no soft penalty; it is a hard packing limit.

## 7. Backpack Progression

Backpacks are Equipment and may differ through explicitly authored:

- Mass Capacity;
- Volume Capacity;
- containment classes;
- quick-access features;
- durability/Condition;
- specialized storage.

No backpack gains hidden capacity from generic Player Level.

## 8. Stackable Resources

A stack preserves all gameplay-relevant compatibility data including:

- Resource ID;
- quantity;
- ownership;
- quality/state fields explicitly defined by that Resource;
- perishability batch/state where applicable;
- mission/security flags;
- reservation/protection flags where applicable.

Only compatible stacks merge.

## 9. Finished Items

Finished durable items normally remain individual inventory objects when they carry unique mutable state such as:

- Condition;
- ammunition/charge;
- modifications;
- Unique Item ID;
- mission identity.

Identical consumables may stack only when their item definition declares stackability and all relevant state is compatible.

## 10. Quick Slots

There are exactly four Quick Slots.

A Quick Slot references an eligible carried consumable/gadget/item.

When the referenced final quantity/object is consumed, transferred away, or destroyed, the Quick Slot clears automatically.

## 11. Equipped Items

Equipped Slots are owned by Player Inventory/Equipment authority but are separate from Backpack Storage capacity accounting as defined by the item's slot.

Equipped Mass remains included in total carried Mass.

## 12. Mission Security States

Portable mission ownership distinguishes:

- `SecurityState::SecuredLoadout` — owned before deployment and committed into the deployed loadout;
- `SecurityState::FieldUnsecured` — acquired during the external mission and not yet secured;
- `SecurityState::VehicleExtractionSecured` — successfully secured to a valid extraction/vehicle ownership path;
- `SecurityState::StationSecured` — committed into persistent station ownership.

Pickup alone normally creates Field-Unsecured state.

## 13. Ordinary Mission-Defeat Inventory Transaction

On ordinary external defeat:

- recoverable Secured Loadout equipment remains player-owned, subject to documented Condition damage;
- unconsumed Secured Loadout ammunition/consumables remain owned and follow Recovery Transit with the player where mission rules permit;
- consumed quantities remain consumed;
- Field-Unsecured items/resources are lost unless a specific persistent recovery cache/world owner is authored;
- Vehicle/Extraction-Secured cargo follows the actual surviving extraction owner rather than the defeated player;
- Station-Secured resources are unaffected.

The transaction executes exactly once.

## 14. Deliberately Dropped Secured Gear

Dropping an owned item transfers it to a world/container owner.

A formerly Secured Loadout item is not protected merely because it was once equipped.

If the current Mission/World persistence does not preserve its dropped owner through defeat/resolution, it can be lost.

This prevents duplication through deliberate pre-defeat dropping.

## 15. Transfer to Horizon Storage

Player-to-station transfer requires a valid reachable storage/logistics intake or another explicitly supported physical interface.

Opening an inventory menu does not globally merge Player and Horizon inventories.

## 16. Transfer to Spacecraft

Player-to-ship transfer requires an accessible compatible cargo/storage interface.

After commit the item/quantity belongs to that spacecraft cargo owner and no longer occupies Player Inventory.

## 17. World Drop

Eligible items may be dropped at a valid nearby world position/container.

The target Mission/World persistence determines how long that owner remains recoverable.

Protected items may expose `DropPolicy::Forbidden`; this is explicit authored data, not hidden logic.

## 18. Containment Model

An item/resource may declare a `ContainmentRequirement`.

The carrying container/backpack/equipment either satisfies that requirement or the item's authored `UncontainedBehavior` applies.

Canonical `UncontainedBehavior` values are:

- `BlockPickup` — transfer cannot commit;
- `PermitWithExposure` — transfer commits only after explicit danger confirmation and applies the defined exposure/hazard state.

Implementation may not arbitrarily choose between these outcomes.

## 19. Fluids and Gases

Bulk fluid/gas resources cannot exist loose in ordinary Backpack Storage.

They require a compatible container item. That container owns the physical quantity and contributes its full resulting Mass/Volume.

## 20. Mining and Salvage Output

Portable extraction output enters Player Inventory/container only while all capacity/containment rules pass.

There is no automatic station teleportation.

If output cannot be accepted, extraction pauses/redirects/drops output according to the owning Mining/Salvage target rule rather than deleting the excess.

## 21. Auto Pickup

The baseline includes user-configurable **Auto Pickup**, default **On**, for objects explicitly tagged `AutoPickupEligible`.

Only these content categories may receive that tag in baseline:

- ordinary loose ammunition compatible with the player's carried/equipped weapons;
- ordinary low-impact stackable consumables explicitly approved by their item definition.

The tag is forbidden for:

- raw resources/ore;
- equipment/weapons/tools;
- artifacts/Research carriers;
- mission-critical/strategic/unique items;
- hazardous materials;
- fluid/gas containers;
- robot/ship/station components;
- large cargo.

Auto Pickup requires:

- valid physical proximity;
- unobstructed pickup eligibility equivalent to normal pickup;
- ownership permission;
- Mass/Volume/containment capacity.

It commits the same ownership transaction as manual pickup and cannot pull items through walls or from remote containers.

Pickup radius is tuneable.

## 22. Sorting and Filtering

Inventory UI provides sorting/filtering by:

- category/type;
- Mass;
- known Credit value where applicable;
- mission/security state;
- Condition;
- quantity/name as appropriate.

Sorting/filtering changes presentation only.

## 23. Stack Split

The player can split a stack into two quantities whose sum equals the original.

The split itself does not change owner until a subsequent transfer/drop commits.

## 24. Stack Merge

Compatible stacks may merge while conserving total quantity.

Incompatible state/batches never overwrite one another merely to simplify UI.

## 25. Reservations

An object/quantity reserved by construction/manufacturing/research/another authoritative job cannot be transferred into Player Inventory unless the reservation is first explicitly released or the owning system authorizes transfer.

## 26. Inventory While Incapacitated

While Incapacitated the player cannot perform ordinary live inventory rearrangement.

Post-resolution/recovery UI may show outcomes and permit management only after the owning recovery state explicitly allows direct control.

## 27. Backpack Replacement

A Backpack cannot be unequipped/replaced when the destination backpack configuration cannot legally contain current Backpack Storage.

The player must first transfer/drop/reconfigure contents.

No overflow bag or hidden temporary storage is created automatically.

## 28. UI / Feedback

Inventory presentation communicates:

- carried Mass / hard capacity / Heavy threshold;
- used Volume / capacity;
- equipped gear;
- contents;
- security state;
- containment/hazard state;
- Condition;
- stack quantity;
- reservation/protection;
- transfer blocker;
- ownership-transfer completion only after commit.

## 29. Persistence

Save state preserves:

- exact owner/location;
- quantities/stacks/batches;
- unique IDs;
- item state/Condition;
- equipped/Quick Slot references;
- mission/security state;
- containers and contained resources;
- reservations relevant to the item.

Save/load cannot duplicate a split/transfer/Auto Pickup transaction.

## 30. Edge Cases

- A partially fitting stack transfers only its maximum legal quantity.
- An indivisible item that does not fit remains completely at source.
- A dropped fluid container keeps ownership of its contents.
- Auto Pickup ignores an eligible item that currently does not fit; it does not repeatedly spam the player.
- If a Quick Slot's stack is partly consumed, it continues referencing the surviving stack/object.
- If a reserved item is force-released by its owning system, Inventory sees it as movable only after release commits.
- If Backpack Condition degrades below a capacity-affecting threshold, the item definition must provide an explicit safe overflow policy; baseline backpack models do **not** lose hard capacity solely from Condition unless such a policy is defined.

## 31. Tuneable Parameters

Tuneable values include:

- backpack Mass/Volume capacities;
- Heavy Load threshold;
- stack limits;
- item Mass/Volume;
- containment/container capacity;
- Auto Pickup proximity radius;
- UI sorting defaults.

Ownership conservation, dual-capacity enforcement, Quick Slot reference semantics, and Auto Pickup eligibility restrictions are fixed.

## 32. Explicit Non-Goals

The baseline does not include:

- unlimited carrying;
- weightless bulk ore;
- global station inventory access from the field;
- duplicated Quick Slot items;
- loose backpack gases/fluids;
- automatic strategic-resource pickup;
- defeat refund of consumed supplies;
- hidden overflow storage;
- ownership duplication.

## 33. Dependencies

Depends on GDS-4 Resource Model/Catalog, Equipment, Movement, Interaction, Missions/Extraction/Failure, Spacecraft Cargo, Station Logistics, Economy, Save/Persistence, and GDS-13 UI/Accessibility.

## 34. Open Questions

None.
