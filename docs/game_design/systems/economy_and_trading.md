# Economy and Trading

> **Status:** Design Complete  
> **Authority:** Human economic currency, market inventory, buying/selling, price construction, faction/location trade access, barter, market replenishment, services, arbitrage, and anti-exploit rules

## 1. Purpose

The economy connects exploration, industry, factions, spacecraft logistics, missions, raids, and station growth without replacing physical resources with abstract numbers.

Trading should let the player solve shortages, specialize production, profit from regional differences, and access faction technology while preserving physical ownership and transport.

## 2. Canonical Human Currency

Standard human commerce uses **Credits (Cr)**.

Credits are a persistent non-physical ledger balance.

Credits:

- have no Mass or Volume;
- are not a GDS-4 physical Resource;
- are not used as reactor fuel, crafting material, or research evidence;
- can be earned, spent, rewarded, fined, or transferred through valid economic transactions.

## 3. No Premium Currency

The baseline contains no premium currency, paid token, real-money exchange, or gameplay purchase currency.

## 4. Credit Ownership

The player has one authoritative Credit balance.

A completed credit transaction applies one atomic debit/credit operation.

Save/load cannot duplicate a completed transaction.

## 5. Physical Goods Remain Physical

Buying or selling a physical good never turns it into an abstract unlock.

Purchase:

Market Inventory → valid Player/Ship/Station owner.

Sale:

Player/Ship/Station owner → Market Inventory.

GDS-4 single-authoritative-ownership remains mandatory.

## 6. Market Entity

Every trading location has a persistent **Market ID** and can define:

- owning faction/settlement;
- current physical stock;
- accepted item/resource categories;
- available services;
- local Base Value modifiers;
- trade spread;
- purchase budget/liquidity;
- reputation/access requirements;
- active event modifiers;
- replenishment sources.

## 7. No Global Marketplace

The baseline has no galaxy-wide instant auction house.

The player trades through actual reachable locations, contacts, or communication-enabled remote contracts whose physical delivery remains explicitly resolved.

## 8. Market Stock

Physical goods for sale have finite stock.

Buying decreases that stock.

A market cannot sell unlimited Titanium, ammunition, fuel, ship modules, or components unless the content definition explicitly represents an effectively inexhaustible trivial service—which must not be used for progression-critical strategic materials.

## 9. Market Purchase Capacity

A market can have finite purchase budget/liquidity for goods sold by the player.

This prevents one small settlement from buying unlimited industrial output.

Liquidity is an economic state, not a physical Resource.

## 10. Base Value

Tradable items/resources can define a tuneable **Base Credit Value** used only as an economic reference.

Base Value does not determine physical usefulness, rarity tier, damage, or crafting quality.

## 11. Quote Construction

A market quote is constructed from:

- Base Credit Value;
- local supply/demand index;
- location/faction market profile;
- temporary event modifier;
- reputation/service modifier;
- buy/sell spread;
- legality/restriction modifier where applicable.

Exact coefficients are tuneable data.

## 12. Buy and Sell Quotes

For the same ordinary item, market state, and moment:

**Sell-to-player price > Buy-from-player price**

unless an explicit subsidy, contract, shortage premium, or mission reward creates a justified exception.

The normal spread prevents zero-risk same-terminal buy/sell loops.

## 13. Price Transparency

The player can inspect the current quoted price before confirming a transaction.

Known modifiers should be explainable through UI later, for example:

- shortage;
- surplus;
- reputation discount;
- restricted item surcharge.

There is no hidden random price roll at confirmation.

## 14. Regional Price Differences

Different markets may legitimately value the same resource differently.

Examples:

- Kara can sell processed industrial goods relatively efficiently;
- remote Freehold settlements may pay more for Medical Compound;
- frontier stations may pay more for Fuel or Pressure Seal Assemblies.

This creates legitimate cargo-trading/arbitrage gameplay.

## 15. Arbitrage Is Allowed

Profiting by transporting goods between genuinely different markets is intended gameplay when it requires actual:

- capital;
- cargo capacity;
- travel;
- fuel/propellant;
- risk;
- opportunity cost;
- changing market conditions.

The economy must prevent only closed no-risk duplication loops, not sensible trade.

## 16. Transaction Preconditions

A physical purchase requires:

- market access;
- sufficient Credits;
- item stock;
- legal/relationship permission;
- valid receiving capacity/owner;
- delivery/transfer path where required.

A sale requires:

- player ownership;
- market accepts item;
- market liquidity/budget;
- physical transfer access;
- item state/condition eligibility.

## 17. Capacity Failure

If the destination cannot receive the physical item:

- transaction does not commit; or
- only an explicitly supported partial quantity commits.

Credits are charged only for the committed quantity.

No purchased physical item disappears because cargo is full.

## 18. Partial Stack Trading

Bulk resources/ammunition can be traded in valid partial quantities.

Quantity, price, source ownership, and destination ownership commit atomically.

## 19. Equipment Condition and Price

Used/damaged equipment can have lower market value according to its actual Condition.

Condition never silently resets on sale/purchase.

A trader selling repaired equipment must have an explicit repair/refurbishment process or generated stock replacement.

## 20. Faction Access

GDS-7 Reputation tiers remain authoritative for relationship state.

A market can require minimum relationship for:

- docking;
- ordinary trade;
- military equipment;
- advanced Blueprints;
- ship variants;
- restricted services.

GDS-12 does not redefine the -100..+100 reputation scale.

## 21. Reputation Pricing

Reputation may influence trade spread or service fees.

It cannot make a positive-price purchase negative or generate Credits by repeatedly buying/selling the same unchanged good at one market.

## 22. Restricted Goods

Faction/content definitions can mark goods:

- Open;
- Licensed;
- Restricted;
- Contraband.

Restrictions affect access/security/consequences, not physical identity.

## 23. Black-Market Trade

Blackwake or independent contacts can trade restricted/stolen goods through explicit market/contact entities.

Black-market access can carry:

- worse spreads;
- limited stock;
- reputation/security risk;
- unique intelligence or stolen technology.

It is not an omniscient catalog of every item in the game.

## 24. Barter

Locations that do not use standard Credits—or specific deals—can use explicit barter.

Barter transaction:

Offered physical goods/services ↔ requested physical goods/services.

Both sides commit atomically.

Barter does not create a second hidden currency.

## 25. The Continuance

The Continuance does not automatically participate in normal Credit markets.

Its exchanges can require:

- authentication state;
- specific components;
- data/evidence access;
- repairs/services;
- machine-to-machine authorization.

These remain explicit exchanges, not a numeric Continuance reputation currency.

## 26. Services

Markets/factions can sell services such as:

- refueling;
- repair;
- docking;
- medical treatment;
- ship refit;
- data analysis;
- cargo handling;
- training/certification;
- intelligence.

A service consumes actual relevant physical inputs/capacity when the underlying system requires them.

Paying Credits does not bypass a physically impossible service.

## 27. Repair Service

Paid repair can combine:

- Credit fee;
- provider-owned parts/materials;
- provider facility/work time.

If required strategic component is unavailable, Credits alone cannot repair it.

## 28. Refueling Service

Refueling transfers actual Fuel/Propellant into ship storage.

Market stock decreases or provider supply logic accounts for the transfer.

## 29. Blueprint Trade

A faction/vendor can sell a Blueprint Unlock or physical blueprint/data carrier only when that design is part of its authored inventory/access.

If a complete Blueprint is purchased as knowledge, it is committed once under GDS-4 Blueprint rules.

Already-owned knowledge cannot be purchased repeatedly for duplicate progression.

## 30. Duplicate Blueprint/Data Value

A duplicate physical blueprint/data carrier may have economic value if a market wants it.

Selling the duplicate does not remove the player's already committed Knowledge Asset.

## 31. Intelligence Trade

Raid/location intel can be purchased only from a contact with an authored information source.

Purchased information creates specific GDS-11 Intel Records at a justified confidence state.

There is no universal 'buy full map' button.

## 32. Mission Payments

GDS-8 Resolution Rewards may include Credits.

Credit reward commits once when its reward condition commits.

Physical mission rewards remain governed by physical delivery/Reward Delivery Claim.

## 33. Fines and Fees

Factions/locations can charge explicit:

- docking fees;
- permit fees;
- contract penalties;
- fines.

A fine cannot silently remove physical resources as a shortcut.

If the player cannot pay, the system follows the authored access/debt/hostility consequence; Credit balance never silently becomes negative unless a specific debt contract explicitly supports it.

## 34. Baseline No Negative Credit Balance

Normal transactions cannot reduce Credits below zero.

Debt exists only through a separately authored obligation/contract and is not the default economy state.

## 35. Market Replenishment

Markets recover stock/liquidity through **active-game economic cycles** backed by world abstraction such as:

- local production;
- convoy arrival;
- faction logistics;
- resource extraction;
- imported supply.

Replenishment never advances because the application is closed.

## 36. Replenishment and Persistence

Current market stock, liquidity, active modifiers, and next Simulation-Time economic update persist through save/load.

Reloading cannot refill a market.

## 37. Dynamic Events

GDS-12 Dynamic Events can create:

- shortage;
- surplus;
- disrupted supply;
- emergency demand;
- convoy delivery;
- route reopening.

These events modify existing market state through explicit causes.

## 38. Market Recovery After Raids

A raided target/market does not instantly regenerate stolen stock.

Recovery requires world/economic replenishment over Simulation Time and can be interrupted by later events.

## 39. Player Production and Selling

The player may profit from manufacturing.

Markets still enforce:

- demand/value;
- finite purchasing liquidity;
- physical transfer;
- production costs.

The design should allow industrial specialization without one trivial recipe becoming an infinite-credit printer.

## 40. Anti-Exploit Invariants

The following must always hold:

1. Credits are created only by explicit income transactions.
2. Physical goods remain single-owner.
3. One market's normal buy/sell spread cannot create same-state profit.
4. Selling does not preserve a duplicate item.
5. Buying does not leave the same market stock unchanged.
6. Market refill cannot be triggered by save/load or offline time.
7. Quest/reward payments commit once.

## 41. Economic Scarcity

Scarcity can come from:

- low stock;
- distance;
- faction restriction;
- hazardous route;
- high demand;
- rare production chain;
- story/event disruption.

Scarcity is not represented by arbitrary item rarity colors.

## 42. Price Balance Goals

Economic balance should make trade useful without making missions/mining/manufacturing irrelevant.

Credits should be a flexibility tool, not a universal substitute for:

- Research;
- Blueprints;
- physical access;
- faction relationship;
- station capability.

## 43. Persistence

Persist:

- Credit balance;
- market stock;
- liquidity;
- current quotes/modifiers;
- trade access;
- contract/payment state;
- replenishment timers/seeds.

## 44. Explicit Non-Goals

The economy does not provide:

- premium currency;
- global instant marketplace;
- infinite vendor stock for strategic goods;
- infinite vendor buying budget;
- Credits as crafting material;
- purchase of Research Evidence points;
- same-terminal infinite arbitrage;
- offline market reset.

## 45. Tuneable Parameters

Base values, spreads, reputation modifiers, stock quantities, liquidity, replenishment cadence, service fees, event multipliers, and transaction limits are tuneable.

## 46. Dependencies

This specification depends on GDS-4 Resource Ownership/Blueprints, GDS-6 Cargo/Docking, GDS-7 Factions/Reputation/World, GDS-8 Rewards, GDS-11 physical loot/target persistence, Dynamic Events, and Save/Persistence.

## 47. Open Questions

None in the baseline economy/trading rules.
