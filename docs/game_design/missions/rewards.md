# Mission Rewards

> **Status:** Design Complete  
> **Authority:** Mission-result reward categories, reward commit, physical reward delivery, knowledge/faction rewards, optional-objective rewards, claim state, and anti-duplication rules

## 1. Purpose

Mission rewards connect external activity to long-term progression without confusing field loot with contract/story compensation.

## 2. Two Reward Sources

The system distinguishes:

### Field Acquisition

Physical items/resources found during the mission.

Examples:

- mined ore;
- salvage;
- containers;
- recovered modules;
- samples.

These obey physical ownership/extraction rules.

### Resolution Reward

A reward granted because a mission outcome was achieved.

Examples:

- faction reputation;
- Blueprint;
- Research Evidence;
- route data;
- sponsor payment;
- promised physical supplies;
- service/access unlock.

## 3. Reward Definition

Each mission defines a Reward Package containing zero or more:

- Economic Reward entitlement;
- Physical Resource/Item Reward;
- Blueprint Unlock;
- Research Evidence;
- Faction Reputation;
- Route/Location Intelligence;
- Service/Access Unlock;
- Narrative/World-State Unlock;
- Survivor/Character Outcome.

## 4. No Generic Mission XP

The baseline does not require a universal mission XP bar or Player Level reward.

Progression uses the established capability systems.

## 5. Economic Reward Boundary

GDS-12 defines:

- currency;
- prices;
- economic value.

GDS-8 can grant an **Economic Reward entitlement** without defining its final currency amount/system here.

## 6. Physical Reward

A sponsor can promise canonical GDS-4 physical resources/items.

These do not spawn weightlessly into a full player backpack.

## 7. Reward Delivery Claim

If a physical reward cannot or should not be placed immediately into a valid physical destination, mission resolution creates a persistent **Reward Delivery Claim**.

A Reward Delivery Claim is:

- non-physical entitlement metadata;
- not a Resource;
- not craftable;
- not tradable by default;
- consumed exactly once when the physical reward is delivered.

## 8. Claim Delivery

Physical delivery can occur through:

- sponsor station cargo service;
- home-station delivery service where world/economy permits;
- designated faction contact;
- mission-end physical handoff.

Delivery requires:

- valid destination;
- capacity;
- compatible containment;
- ownership transfer.

## 9. Full Storage

If destination storage cannot accept a physical reward:

- delivery does not delete the excess;
- the undelivered portion remains Claimable;
- no duplicate physical copy is created.

## 10. Blueprint Reward

Blueprint Unlock commits to the persistent GDS-4 Knowledge Asset system.

A Blueprint reward is not a physical cargo item unless the mission explicitly rewards a physical data carrier before analysis/claim.

## 11. Research Evidence Reward

Research Evidence commits according to GDS-4.

Duplicate unique evidence follows GDS-4 duplicate rules rather than multiplying infinitely.

## 12. Reputation Reward

Faction reputation changes according to GDS-7.

The mission reward definition states:

- faction;
- direction;
- authored magnitude class/value later balanced.

## 13. Route / Location Reward

A mission can unlock:

- Rumored;
- Mapped;
- Verified

route/location intelligence according to World/Navigation rules.

## 14. Access Reward

Possible access unlocks:

- docking permission;
- faction service;
- restricted contract pool;
- shipyard access;
- research contact.

Access state is persistent.

## 15. Optional Objective Rewards

Optional Objectives can add separate Reward Components.

Each optional component commits only if its Objective ID is Completed at mission resolution.

## 16. Partial Success

A mission can define partial-resolution rewards where its design explicitly supports them.

Partial reward does not imply mission Success unless success conditions are met.

## 17. Failure Rewards

Failure normally grants no success-only Resolution Reward.

However, already-secured physical field loot remains governed by ownership/extraction.

A mission can explicitly grant a consolation/story outcome where appropriate.

## 18. Abandonment Rewards

Abandonment grants no success-only reward unless an explicitly completed independent Secondary/Optional reward was defined to commit before final mission success.

Default optional rewards commit at mission resolution, not immediately.

## 19. One-Time Commit

Every Reward Component has an internal commit state.

Canonical states:

- Pending;
- Earned;
- Claimable;
- Delivered;
- Forfeited.

A component transitions forward exactly once.

## 20. Reward Atomicity

Mission resolution commits each eligible reward exactly once.

Save/load cannot:

- re-add reputation;
- re-unlock Blueprint;
- duplicate physical claims;
- re-grant route data.

## 21. Story Rewards

Main-story mission outputs defined in GDS-7 map to Reward Components such as:

- narrative flag;
- evidence;
- faction relation;
- route unlock;
- technology availability.

GDS-8 does not invent alternate story rewards.

## 22. Resource Scarcity

Mission rewards use canonical resources.

Late-game rewards should emphasize strategic value rather than huge trivial-material quantities, consistent with Global Rules.

## 23. Reward Preview

Before mission acceptance, the player sees:

- guaranteed known reward categories;
- known optional bonuses;
- unknown/conditional rewards where intentionally hidden.

The system does not promise an exact hidden random reward as guaranteed.

## 24. Procedural Reward Seed

Procedural mission major reward selection is stable with the Mission ID/seed.

Reload/decline does not reroll a better reward for the same mission.

## 25. Loot vs Reward UI

The result screen distinguishes:

- extracted field loot;
- lost field loot;
- resolution rewards;
- pending physical claims;
- reputation;
- knowledge/access unlocks.

## 26. No Reward Teleport From World

A unique physical object already recovered in the field is not also granted again as a Resolution Reward unless the mission explicitly defines a separate sponsor copy.

## 27. Edge Cases

If a Blueprint was already owned before mission completion, duplicate handling follows GDS-4 rather than creating another permanent unlock.

If faction reputation is already at +100/-100, mission result still records the action but clamps the standard numeric value.

If a physical reward claim is partially delivered, the remaining quantity stays Claimable.

If a mission succeeds while communications to sponsor are unavailable, earned reward state persists and delivery/notification can occur later.

## 28. Explicit Non-Goals

Rewards do not provide:

- universal mission XP;
- arbitrary rarity inflation;
- weightless physical rewards into full inventory;
- duplicate one-time unlocks;
- save-scum reward rerolls;
- magical station storage insertion without ownership transfer.

## 29. Tuneable Parameters

Tuneable values include economic amounts, physical reward quantities, reputation changes, optional bonuses, and reward budgets.

## 30. Dependencies

This specification depends on GDS-4 Resources/Blueprints/Research, GDS-7 Reputation/World/Narrative, Extraction, Objectives, GDS-12 Economy/Progression, and Persistence.

## 31. Open Questions

None in the reward-delivery/commit baseline.

Final economic denominations and balance are GDS-12 authority.
