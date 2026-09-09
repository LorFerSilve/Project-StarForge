# Player Equipment

> **Status:** Draft  
> **Authority:** Player equipment slots, equip/unequip rules, item condition, power integration, loadout behavior, modifications boundary, and protective equipment capability

## 1. Purpose

Equipment is the primary source of direct player capability progression.

It defines what the player can survive, carry, scan, mine, repair, and use in combat.

## 2. Equipment Slots

The baseline slots are:

- Suit;
- Helmet;
- Backpack;
- Primary Weapon A;
- Primary Weapon B;
- Sidearm;
- Tool;
- Gadget A;
- Gadget B.

Quick consumables are Inventory Quick Slots rather than equipment slots.

## 3. Slot Compatibility

Every equippable item declares compatible slot(s).

An item cannot occupy an incompatible slot.

Two-handed/large tools may temporarily occupy the held Tool state while still stored in backpack when not equipped, according to item definition.

## 4. Suit

The Suit provides the primary environmental/armor chassis.

A Suit may define:

- pressure sealing;
- thermal protection;
- radiation protection;
- armor;
- internal battery;
- life-support connection;
- magnetic boots;
- mobility modifiers;
- module sockets.

## 5. Helmet

The Helmet completes compatible sealed-suit environmental protection.

A suit requiring a sealed helmet is not vacuum-safe if the helmet is absent/open/damaged beyond seal capability.

Helmet features can include:

- visor HUD;
- filters;
- scanner integration;
- radiation shielding.

## 6. Backpack

Backpack defines portable Mass and Volume Capacity and optional specialized containment.

Backpack changes do not alter resource ownership.

## 7. Weapons

Weapon slots define carry/readiness, not weapon mechanics.

Weapons are fully specified under GDS-9.

Equipping a weapon does not create ammunition.

## 8. Tool Slot

One primary Tool can be immediately ready for direct utility work.

Other portable tools may remain in backpack and require switching.

## 9. Gadget Slots

Gadgets are reusable or charge-based utility equipment providing specific capabilities such as:

- portable shield projector;
- deployable sensor;
- rescue beacon;
- portable power interface;
- mission utility device.

Exact gadgets are content definitions.

## 10. Equipping

Normal equip/unequip occurs:

- at station loadout interfaces; or
- in the field if the item is portable and current state allows safe swapping.

Certain suits/backpacks cannot be removed in hostile environments if doing so would immediately violate safety.

## 11. Safety Lock

The game blocks an unequip action that would cause immediate unavoidable lethal exposure unless the player uses an explicit dangerous override where the equipment supports removal.

Example: removing sealed helmet in vacuum.

## 12. Loadout

A mission loadout is the selected set of:

- equipped items;
- carried ammunition;
- consumables;
- tools;
- gadgets;
- spare mission supplies.

Loadout does not create copies; items transfer from actual owned inventory.

## 13. Item Condition

Persistent equipment has Condition.

Baseline condition states:

- Pristine/Operational;
- Worn;
- Damaged;
- Critical;
- Disabled.

Exact thresholds are tuneable.

## 14. Condition Effects

Condition affects only documented equipment capabilities.

Examples:

- damaged suit loses protection efficiency;
- damaged tool slows/blocks advanced operation;
- damaged shield reduces capacity;
- disabled equipment cannot provide its active capability.

There is no hidden random stat degradation.

## 15. No Routine Permanent Gear Destruction

Ordinary defeat does not permanently delete persistent equipped items.

Equipment can become Disabled and require repair.

Specific consumable/single-use devices may be destroyed by use because their item definition says so.

## 16. Equipment Repair

Repair requires:

- compatible materials/components;
- valid tool/facility;
- access;
- active game time.

Station repair/manufacturing rules integrate with equipment repair.

## 17. Field Repair

Field repair can restore limited condition for eligible gear using portable repair consumables/tools.

Full restoration of advanced gear can require station facilities.

## 18. Suit Energy

Active suit systems can draw from a common **Suit Energy Reserve**.

Possible consumers:

- personal shield;
- zero-g thrusters;
- active scanner;
- environmental processing;
- powered tools when connected.

The exact energy model is equipment data.

## 19. Suit Energy Recharge

Suit energy can recharge from:

- station power interface;
- ship interface;
- compatible field power source;
- replaceable/portable power cell where defined.

Energy does not regenerate from nothing.

## 20. Priority During Low Energy

The default suit priority is:

1. life-support safety;
2. environmental protection/control;
3. emergency mobility;
4. personal shield;
5. scanning/tools;
6. discretionary gadgets.

The player can configure eligible priorities.

Passive physical armor does not require energy.

## 21. Suit Life-Support Reserve

A sealed suit has finite breathable-gas/life-support endurance where independent atmosphere is required.

Field Survival owns consumption/exposure.

## 22. Ammunition

Weapons use physical ammunition/energy according to combat definitions.

Equipment slots do not grant infinite ammunition.

## 23. Modifications

Equipment can support authored modification sockets.

A modification must define:

- compatible equipment;
- effect;
- cost;
- install/remove rules.

There is no universal random affix system in the baseline.

## 24. No Loot-Rarity Stat Tiers

Equipment is differentiated by design/model, technology, modifications, and condition.

The baseline does not require Common/Rare/Epic/Legendary versions of the same item with random stat multipliers.

## 25. Equipment Comparison

UI shows actual mechanical differences:

- protection;
- capacity;
- energy;
- mass;
- movement modifier;
- environment compatibility;
- condition;
- modification slots.

## 26. Equipped Mass

Equipped gear contributes to player carried Mass.

Heavy load rules apply to total carried mass.

## 27. Field Swapping

Weapon/tool/gadget swapping takes a short action time defined by item class.

Inventory cannot instantaneously replace every equipped item without gameplay time where combat relevance exists.

## 28. Suit/Helmet Swapping

Suit and helmet replacement normally requires a safe non-combat environment or dedicated equipment interface.

It is not an instant combat action.

## 29. Persistence

Equipped-slot assignment, condition, energy, Life-Support Reserve where gameplay relevant, modifications, and ammunition state persist.

## 30. Edge Cases

If the Suit becomes Disabled in vacuum, Field Survival uses remaining passive seal/emergency reserve only if the suit definition provides it.

If the Backpack is destroyed/disabled, capacity does not vanish instantly; the item enters damaged storage state and field-resolution rules prevent resource duplication/loss without explicit consequence.

If a Gadget is consumed, its equipment slot clears.

If Suit Energy reaches zero, passive protection remains while active systems shut down according to priority.

## 31. Explicit Non-Goals

Equipment does not provide:

- random rarity colors as core progression;
- gear deletion on routine defeat;
- infinite suit power;
- instant suit changes during combat/vacuum;
- hidden condition effects.

## 32. Tuneable Parameters

Tuneable values include slot swap times, condition thresholds, armor/protection values, suit energy, recharge rate, Life-Support Reserve capacity, mass, and modification values.

## 33. Dependencies

This specification depends on Inventory, Health, Field Survival, Movement, Tools, GDS-4 crafting/resources, GDS-9 Combat, Missions, and Station repair/manufacturing.

## 34. Open Questions

None in the equipment-framework baseline.

Specific weapon/tool/suit models are future content definitions.
