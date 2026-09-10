# Loot and Reward Rules

> **Status:** Design Complete  
> **Authority:** Cross-domain loot generation, physical drop provenance, unique loot, enemy recoverables, reward duplication prevention, deterministic procedural loot, replenishment, and reward-value boundaries

## 1. Purpose

Loot should represent recoverable physical objects, resources, or committed knowledge/reward transactions—not randomized stat inflation detached from the world.

## 2. Loot Categories

Canonical cross-domain categories:

- Physical Resource;
- Manufactured Component;
- Ammunition/Consumable;
- Equipment/Weapon;
- Ship/Robot/Station Component;
- Artifact / Unique Physical Object;
- Data Carrier;
- Salvage;
- Credit Reward;
- Knowledge/Access Reward;
- Faction/Reputation consequence.

## 3. Physical Loot Provenance

Every physical loot object/quantity must originate from one valid source:

- world deposit/container;
- actor inventory/loadout residue;
- installed component removed from a system;
- destroyed object salvage envelope;
- market/faction physical delivery;
- mission-authored physical reward.

An enemy death or mission completion cannot manufacture a duplicate of equipment that still exists elsewhere.

## 4. No Generic Loot Rarity

The baseline does not use Common/Rare/Epic/Legendary quality tiers for otherwise identical resources, weapons, armor, robots, or components.

Value comes from actual:

- model;
- technology;
- condition;
- scarcity;
- capability;
- Blueprint/Research access;
- location/difficulty.

## 5. No Random Affix Baseline

Recovered gear does not receive arbitrary random percentage affixes such as +12% Damage or +8% Reload Speed solely because it dropped.

Mechanically different variants must be authored components/models/modifications.

## 6. Deterministic Procedural Loot

For a persistent Mission/Raid/Dynamic Event ID, major procedural loot is generated from:

- stable seed;
- world/location profile;
- faction profile;
- economic state at creation;
- threat/content table.

Reloading cannot reroll better major loot.

## 7. Major vs Incidental Loot

Major loot includes:

- strategic components;
- Blueprints/data carriers;
- mission-critical objects;
- advanced equipment;
- large resource caches.

Incidental loot can include ordinary ammunition, consumables, common salvage, and small resources.

Both remain physically conserved.

## 8. Enemy Equipment Recovery

An enemy does not automatically drop a pristine copy of every visible weapon/armor component.

Recoverability depends on:

- whether the physical item survives;
- condition;
- attachment to actor/chassis;
- access/removal requirements;
- salvage/content rules.

## 9. Actor Inventory Abstraction

NPCs may internally use simplified ammunition/inventory representation for performance.

On defeat, a deterministic **Recoverable Loot Resolution** converts the valid remaining physical envelope into recoverable items.

The resolution cannot output more material/items than the actor's authored carried/equipped envelope permits.

## 10. Salvage

Salvage remains GDS-4 authority.

Destroyed objects can yield less reusable value than intact capture/removal.

Destruction must never increase total recoverable matter beyond the object's salvage envelope.

## 11. Container Loot

Containers have persistent contents.

Opening a container does not generate a second inventory copy.

Removed items remain removed until explicit replenishment creates/replaces stock.

## 12. Resource Deposits

Mining output remains GDS-4 authority.

Loot tables do not cause exhausted deposits to respawn on reload.

## 13. Unique Items

A unique physical item has one persistent Unique Item ID.

It cannot simultaneously exist:

- in a target container;
- in player cargo;
- as mission reward.

A committed transfer updates its one authoritative owner/location.

## 14. Unique Knowledge

Blueprint Unlocks, Research Evidence, and permanent route/story knowledge commit once.

A duplicate physical data source may still be sold/analyzed for an explicitly different benefit, but cannot grant the same permanent unlock twice.

## 15. Field Loot Security

Mission-acquired physical loot begins or remains under GDS-8 security rules:

- Field-Unsecured;
- Vehicle/Extraction-Secured;
- Station-Secured.

This document does not redefine extraction.

## 16. Raid Loot

GDS-11 remains authoritative for physical raid looting and hostile theft.

Raid success does not create an additional percentage-based reward copy of already stolen physical stock.

## 17. Resolution Rewards

GDS-8 Resolution Rewards are separate from Field Acquisition.

They can include:

- Credits;
- reputation change;
- Blueprint Unlock;
- Research Evidence;
- access/route permission;
- sponsor-provided physical item/resource;
- service entitlement.

## 18. Reward Commit

Each one-time reward has a persistent Reward Transaction ID or equivalent committed flag.

It can commit only once.

Save/load cannot re-award it.

## 19. Physical Resolution Reward

If a mission awards a physical good, delivery must occur through:

- valid physical destination; or
- GDS-8 Reward Delivery Claim.

No item is silently inserted into full inventory.

## 20. Credits

Credit rewards alter the non-physical Credit ledger once.

They do not create a physical money object unless a specific collectible/story object explicitly exists.

## 21. Reputation Rewards

Reputation change applies only to the relevant faction(s) and follows GDS-7 thresholds.

No generic global morality reward is added.

## 22. Difficulty and Rewards

Changing player-selected Difficulty does **not** multiply:

- Credits;
- resource yield;
- Blueprint chance;
- Research Evidence;
- unique loot;
- reputation.

Difficulty therefore cannot be farmed for superior economy/progression.

## 23. Threat and Loot

Harder world locations can contain more valuable resources/technology because their authored content differs.

This is not a Difficulty-setting bonus.

## 24. Loot Table Eligibility

A generated loot entry must satisfy:

- location/faction plausibility;
- progression/content rules;
- uniqueness constraints;
- owner/storage compatibility;
- no mandatory-story duplication.

Invalid entries are rejected before mission/event presentation.

## 25. Blueprint Sources

A Blueprint can be placed only where GDS-4 Blueprint source rules permit it.

Procedural generation cannot accidentally bypass a mandatory technology/story gate by dropping a prohibited design early.

## 26. Research Evidence Sources

Research Evidence must correspond to an actual evidence source/analysis.

Enemies do not drop generic Science Points.

## 27. Loot Replenishment

A persistent location's depleted loot can recover only through an explicit process such as:

- economic replenishment;
- faction resupply;
- production;
- new occupant stock;
- new Dynamic Event.

Simply leaving/re-entering does not refill it.

## 28. Replenishment Identity

Replenished stock is new world stock created by the economic/world event, not restoration of already stolen unique objects.

Unique items do not respawn unless a specific recovery rule relocates the same identity.

## 29. Procedural Repeat Content

Repeat missions can generate new ordinary loot only through a new Mission/Dynamic Event ID with valid world source.

This is not a reset of a previously depleted persistent target.

## 30. Loot Destruction

Physical loot can be damaged/destroyed by:

- explosion;
- fire;
- decompression/loss;
- contamination;
- container destruction;
- deliberate disposal.

Destruction is a physical state change, not a failure-screen tax.

## 31. Player Jettison/Discard

Discarding or jettisoning a physical item transfers it to a world object/destruction state according to owning subsystem.

It cannot remain in inventory simultaneously.

## 32. Overcapacity

Loot transfer blocks or partially transfers when destination Mass/Volume/capacity is insufficient.

Excess remains at its previous owner/location.

## 33. Loot Information

UI may summarize estimated value, but does not reveal exact hidden loot unless player information supports it.

Raid Intelligence remains authoritative for target knowledge.

## 34. Farming Prevention

The design prevents trivial farming through:

- stable seeds;
- persistent target depletion;
- one-time reward commits;
- explicit replenishment;
- no reload rerolls;
- no difficulty reward multiplier.

It does not prevent legitimate repeated mining/trade/missions where new world supply exists.

## 35. Boss/Elite Loot

Boss/elite enemies may guard or carry high-value authored loot.

Their defeat is not required to produce a random rarity shower.

Unique or advanced rewards remain fixed/seeded and progression-valid.

## 36. Failure

Mission failure preserves/losses loot exactly through GDS-8 security state.

No separate generic loot-loss percentage applies.

## 37. Persistence

Persist:

- container/deposit state;
- unique item ownership;
- generated major loot seed/state;
- Reward Transaction commits;
- physical delivery claims;
- replenishment state.

## 38. Explicit Non-Goals

Loot does not use:

- rarity-color stat inflation;
- random affix treadmill;
- enemy death as item creation from nothing;
- reload rerolling;
- percentage loss taxes;
- difficulty reward multipliers;
- room-exit respawns;
- generic Science Point drops.

## 39. Tuneable Parameters

Ordinary loot quantities, condition distributions, salvage fractions, content weights, economic values, and replenishment cadence are tuneable.

## 40. Dependencies

This specification depends on GDS-4 Resource/Blueprint/Research/Salvage, GDS-5 Inventory/Equipment, GDS-8 Rewards/Extraction, GDS-9 enemy equipment, GDS-10 robot wrecks, GDS-11 Looting, Economy, Dynamic Events, and Persistence.

## 41. Open Questions

None in the cross-domain loot/reward baseline.
