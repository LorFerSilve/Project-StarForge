# Player Equipment

> **Status:** Design Complete  
> **Authority:** Player equipment slots, equip/unequip rules, Condition, suit power/life-support integration, loadout behavior, modifications, repair profiles, and protective capability

## 1. Purpose

Equipment is the primary source of direct player capability progression. It determines what the player can survive, carry, scan, mine, repair, and use in combat without relying on generic character levels.

## 2. Canonical Equipment Slots

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

Quick consumables use four Inventory Quick Slots and are not Equipment slots.

## 3. Slot Compatibility

Every equippable item declares one or more compatible slots.

An item cannot occupy an incompatible slot.

A carried tool not currently assigned to the Tool slot remains Backpack Storage until a valid swap commits.

## 4. Suit Definition

Every Suit definition explicitly declares the properties it supports. Relevant properties include:

- pressure sealing;
- thermal protection;
- radiation protection;
- armor;
- Suit Energy capacity;
- Life-Support Reserve interface/capacity;
- Magnetic Boots;
- Zero-G maneuvering support;
- movement modifiers;
- modification sockets;
- Condition thresholds and failure behavior.

An omitted capability is absent; technology tier never implies an undocumented feature.

## 5. Helmet Definition

Every Helmet definition declares:

- compatible Suit families;
- seal capability;
- protective ratings;
- visor/HUD capability;
- filter capability if present;
- scanner/sensor integration if present;
- Condition behavior.

Where a sealed Suit requires a helmet, environmental sealing is invalid while the helmet is absent, open, incompatible, or below its required seal Condition.

## 6. Backpack Definition

Backpack Equipment declares:

- Mass Capacity;
- Volume Capacity;
- containment capabilities;
- optional specialized compartments;
- Condition behavior;
- appearance/attachment data.

Backpack equipment does not create ownership duplicates when replaced.

## 7. Weapons

Weapon slots determine carry/readiness only. GDS-9 owns weapon firing, ammunition, reload, damage, spread/recoil, and combat behavior.

Equipping a weapon never creates ammunition.

## 8. Tool Slot

Exactly one primary utility tool can occupy the active Tool slot.

Other portable tools remain in Backpack Storage and require a timed swap to become active.

## 9. Gadget Slots

Gadget A and Gadget B accept compatible reusable or charge/single-use gadgets.

Canonical gadget classes are defined in `tools_and_gadgets.md`.

## 10. Field Equip/Swap Eligibility

Weapons, Tools, and Gadgets may be swapped in the field only when:

- the player is not Incapacitated;
- source and destination items are player-owned and portable;
- no target interaction explicitly locks the current held item;
- the item class permits field swapping;
- all resulting slot/Inventory rules remain valid.

A swap consumes its authored `SwapTime` on Simulation Time and can be interrupted by the item's action rules.

## 11. Suit / Helmet / Backpack Replacement

Suit, Helmet, and Backpack replacement requires a safe equipment/loadout context with sufficient time and physical access.

Baseline replacement is not an instant combat action.

A replacement is blocked when it would immediately create an invalid unavoidable condition, including:

- removing the only valid pressure seal in unsafe pressure/vacuum;
- replacing a Backpack when current contents cannot legally fit the new configuration;
- removing equipment currently required by an active life-critical support connection.

Where the target equipment explicitly supports a dangerous manual override, the player receives a high-consequence confirmation and the real environmental result follows immediately after commit.

## 12. Mission Loadout

A deployment loadout is composed only from actual owned objects/quantities:

- equipped items;
- carried ammunition;
- consumables;
- tools/gadgets;
- mission equipment;
- spare supplies.

Deployment reserves/transfers those same objects under Mission/Inventory ownership rules; no copies are generated.

## 13. Equipment Condition

Durable Equipment uses `EquipmentConditionState`:

- Operational;
- Worn;
- Damaged;
- Critical;
- Disabled.

Exact thresholds are tuneable per item/model.

`Pristine` may be displayed as full Condition but is not a separate functional state beyond Operational unless the item data explicitly requires it.

## 14. Condition Effects

Each durable item declares a **Condition Profile** mapping its condition ranges to exact capability effects.

Allowed effects include documented changes to:

- protection;
- capacity;
- energy efficiency/capacity;
- accuracy/tool efficiency where the owning system permits;
- available modes;
- reliability state;
- Disabled behavior.

No item receives hidden random degradation. If no Condition Profile modifier is declared for a capability, that capability is unchanged until another declared threshold.

## 15. Disabled Equipment

`EquipmentConditionState::Disabled` means the item's active gameplay capability is unavailable until repaired/replaced.

Passive properties survive only when the item definition explicitly marks them as mechanically intact at Disabled state.

## 16. Routine Defeat Protection

Ordinary player defeat does not permanently delete established equipped persistent gear.

Equipment can still:

- lose Condition;
- become Disabled;
- consume ammunition/charges;
- remain physically elsewhere if deliberately dropped before recovery;
- be destroyed/consumed only by an explicit item/mission/world rule that authorizes that physical outcome.

## 17. Repair Profiles

Every repairable Equipment model declares a **Repair Profile** with:

- eligible repair level(s);
- required materials/components;
- required tool/facility;
- repair amount/state transition;
- Simulation-Time work duration;
- whether field repair is permitted;
- whether full restoration requires station/ship service.

Implementation never decides these rules ad hoc from item tier.

## 18. Field Repair

Field repair is available only for an item whose Repair Profile permits it.

It consumes the listed physical inputs and uses a valid player/robot/crew repair action.

Field repair cannot exceed the maximum Condition/state allowed by that Repair Profile.

## 19. Full Service Repair

When a Repair Profile requires Full Service, restoration above the field-repair limit needs an eligible station/ship facility and any required specialist, component, power, and work inputs.

Docking or opening a repair menu alone never heals an item.

## 20. Suit Energy Reserve

Active Suit/Helmet systems draw from the shared **Suit Energy Reserve** unless an item explicitly has its own independent energy store.

Eligible suit consumers are:

- environmental thermal regulation;
- active life-support machinery when the suit model requires power;
- Zero-G thrusters;
- Magnetic Boots when their model uses powered adhesion;
- Personal Shield;
- active scanner/sensor functions;
- powered Tool connection;
- powered Gadgets connected to the Suit bus.

Only installed/active consumers draw energy.

## 21. Low-Energy Priority

Default Suit Energy priority is:

1. active life-support safety;
2. environmental regulation/seal-support systems;
3. emergency mobility;
4. Personal Shield;
5. scanning/tools;
6. discretionary Gadgets.

The player may configure only priorities marked configurable by the Suit. A protected safety consumer cannot be silently demoted below its authored minimum unless an explicit dangerous override exists.

Passive armor/seal properties require no energy unless their item definition says otherwise.

## 22. Suit Energy Recharge

Recharge requires a valid source:

- station interface;
- spacecraft interface;
- compatible field power source;
- physical replaceable/portable power cell where the Suit supports one.

Recharge transfers/stores finite energy and follows source/output limits. Energy never regenerates from nothing.

## 23. Life-Support Reserve

A sealed Suit uses the finite Life-Support Reserve model defined in Field Survival.

The equipment definition supplies capacity/interface/protection properties; Field Survival owns consumption and exposure.

## 24. Modifications

Equipment may expose authored modification sockets.

Every Modification definition declares:

- compatible host/category/socket;
- exact effect;
- installation/removal requirements;
- physical ownership/cost;
- whether removal is reversible;
- Condition/energy interactions where applicable.

No random affix generator exists in the baseline.

## 25. No Loot-Rarity Stat Tiers

Equipment strength comes from model design, Research/Blueprint access, installed components/modifications, configuration, and Condition.

The baseline has no Common/Rare/Epic/Legendary copies of the same item with randomized percentage multipliers.

## 26. Equipment Comparison

UI compares the actual known mechanical properties relevant to the slot, including:

- protection;
- capacity;
- energy;
- Mass;
- movement modifiers;
- environmental compatibility;
- Condition;
- sockets/modifications;
- known repair requirements.

Unknown properties are not revealed solely by comparison UI.

## 27. Carried Mass

All equipped gear contributes to Player carried Mass.

Movement uses total carried Mass and Inventory Heavy Load state.

## 28. Persistence

Save state preserves:

- equipped slot assignment;
- unique item identity where applicable;
- Condition/Profile state;
- Suit Energy;
- Life-Support Reserve where applicable;
- modifications;
- weapon ammunition/charge state owned by Combat;
- active field swap/repair state only when at a Stable Save Boundary that supports it.

Load never repairs, recharges, or duplicates equipment automatically.

## 29. Presentation

Equipment UI/HUD communicates known:

- slot compatibility;
- Condition/state;
- energy/reserve;
- environmental protection;
- modifications;
- Mass/capacity;
- field-swap eligibility;
- repair requirements;
- blocker causes.

Success feedback occurs after equip/repair/transfer commit.

## 30. Edge Cases

- A Suit becoming Disabled in vacuum does not automatically destroy its passive seal; the Suit's Condition Profile states whether seal remains and Field Survival applies it.
- A Backpack replacement is blocked if contents cannot fit; no hidden overflow is created.
- A Gadget consumed at use clears its Equipment slot after the consumption transaction commits.
- At zero Suit Energy, passive properties remain only as defined; powered consumers shut down by priority.
- If a field swap is interrupted before commit, ownership/slot assignment remains at its last committed state.
- Repairing weapon hardware does not create ammunition or refill magazines.

## 31. Tuneable Parameters

Tuneable values include:

- item Condition thresholds;
- SwapTime;
- protection/armor values;
- Mass;
- Suit Energy capacity/recharge;
- Life-Support capacity;
- modification values;
- repair quantities/durations/costs.

Slot set, physical ownership, Repair/Condition Profile requirements, and energy conservation are fixed.

## 32. Explicit Non-Goals

The baseline does not include:

- random loot rarity tiers/affixes;
- gear deletion merely because the player was defeated;
- infinite suit power;
- instant suit/helmet/backpack changes in combat/vacuum;
- hidden Condition penalties;
- automatic repair from docking/menu access;
- equipment-generated ammunition/resources.

## 33. Dependencies

Depends on Inventory, Player Health, Field Survival, Movement, Tools/Gadgets, GDS-4 Resources/Crafting/Blueprints, GDS-9 Combat, Missions, Station/Spacecraft repair and power/service systems, Time/Simulation, Save/Persistence, and Presentation.

## 34. Open Questions

None. Specific item-model numerical data is authored content governed by these fixed contracts and is not an unresolved gameplay rule.
