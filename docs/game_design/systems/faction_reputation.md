# Faction Reputation Integration

> **Status:** Design Complete  
> **Authority:** Cross-domain routing of reputation consequences into trade, missions, raids, services, dynamic events, attribution, and one-time reward transactions

## 1. Purpose

This document connects the existing GDS-7 reputation system to other gameplay domains without redefining its numeric scale or tiers.

## 2. Numeric Authority

`world/reputation_and_relations.md` remains authoritative for:

- human faction reputation range -100 to +100;
- tier thresholds;
- independent per-faction values;
- no passive decay;
- Continuance relationship model.

GDS-12 does not create a second reputation scale.

## 3. Reputation Change Event

Every reputation adjustment must originate from an explicit committed **Reputation Change Event** containing:

- affected faction;
- signed amount;
- reason/source ID;
- attribution/knowledge condition where relevant;
- one-time transaction identity;
- resulting value/tier.

## 4. No Duplicate Award

The same mission/objective/reward transaction cannot apply the same reputation change twice through save/load or repeated UI entry.

## 5. Faction Independence

A reputation change affects only factions explicitly connected to the event.

Helping Freehold does not automatically increase every other human faction.

## 6. Third-Party Consequences

One action can affect several factions only when world knowledge/political relationships justify it.

Each faction receives a separate explicit change event.

## 7. Attribution

Hostile acts such as raids, theft, sabotage, or killings affect a faction only when the faction can credibly attribute responsibility through:

- direct identification;
- surviving witnesses;
- sensor/security records;
- recovered evidence;
- later intelligence linkage;
- player admission/contract state.

## 8. Unknown Attacker

A target may know it was attacked while not knowing the player was responsible.

No player reputation penalty is applied until an attribution path commits.

## 9. Delayed Attribution

A later Dynamic Event/intelligence discovery can identify the player.

That later event can apply the deferred consequence once, using the original incident ID as provenance.

## 10. Witness/Evidence Persistence

Relevant identification/evidence state persists and cannot be rerolled by reload for the same event seed/state.

## 11. Mission Reputation

GDS-8 Resolution Rewards can include explicit reputation changes for:

- successful contract;
- rescue;
- delivery;
- defense;
- discovery;
- failed/abandoned obligation where defined.

## 12. Raid Reputation

GDS-11 reports factual raid outcomes such as:

- identified attacker;
- casualties;
- theft;
- sabotage;
- destruction;
- surrender/negotiated outcome.

This integration converts those facts into relevant faction change events.

## 13. No Universal Moral Score

There is no global Karma/Good/Evil value.

Consequences remain faction- and event-specific.

## 14. Trade Access

Reputation tiers can gate market/services under Economy rules.

Examples:

- Hostile: ordinary docking/trade blocked;
- Neutral: basic trade;
- Accepted/Trusted: better services/restricted catalogs;
- Allied: strategic support/advanced access.

Exact vendor catalog remains location/faction content.

## 15. Pricing

Positive/negative reputation can modify trade spread/service fees within Economy constraints.

It cannot create same-market infinite arbitrage.

## 16. Blueprint/Technology Access

Faction relationship can unlock the opportunity to acquire faction Blueprints, training, or data.

Reputation alone does not satisfy underlying Research/Blueprint prerequisites or create physical items.

## 17. Mission Availability

Factions can require minimum tier for contracts or story branches.

Low reputation can unlock hostile/reconciliation alternatives where authored.

Mandatory campaign progress must retain at least one viable route.

## 18. Dynamic Events

Event generation can weight/allow faction events using actual reputation and prior actions.

A procedural event cannot silently change reputation without an explicit event outcome.

## 19. Reputation Caps

Values clamp to the GDS-7 -100..+100 range.

Excess positive/negative change beyond the limit is discarded after recording the resulting boundary value.

## 20. No Passive Decay

Time, inactivity, offline duration, or avoiding a faction does not change reputation by itself.

## 21. Continuance

Continuance does not use Credit-style reputation numbers.

Cross-cutting systems consume its node/authentication state as defined by GDS-7.

## 22. Economic Consequence vs Reputation

Paying a fine or compensation does not automatically restore reputation unless the authored settlement transaction explicitly includes a reputation change.

Credits and reputation are distinct state.

## 23. Reconciliation

A hostile human faction can expose authored reconciliation routes such as:

- reparations;
- service mission;
- evidence transfer;
- negotiated truce.

There is no generic one-click reputation reset.

## 24. Difficulty

Difficulty does not multiply reputation rewards/penalties.

## 25. Save/Persistence

Persist:

- per-faction reputation values/tiers;
- incident attribution;
- deferred consequence provenance;
- committed Reputation Change Event IDs;
- Continuance node states.

## 26. Explicit Non-Goals

This integration does not provide:

- global Karma;
- passive reputation decay;
- omniscient faction attribution;
- one-click reputation purchase;
- shared universal faction standing;
- difficulty-based reputation multipliers.

## 27. Tuneable Parameters

Per-event reputation amounts, price modifiers, access thresholds within existing tiers, and reconciliation costs are tuneable content/balance values.

## 28. Dependencies

This specification depends on GDS-7 Reputation/Factions, GDS-8 Rewards, GDS-11 raid factual outcomes, Economy, Dynamic Events, Progression, and Persistence.

## 29. Open Questions

None in the cross-cutting faction-reputation integration baseline.
