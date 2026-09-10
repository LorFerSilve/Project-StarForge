# Dynamic Events

> **Status:** Draft  
> **Authority:** Dynamic Event identity, generation, eligibility, concurrency, lifecycle, active-time scheduling, event families, Horizon attack creation, market/world effects, cooldown/grace, persistence, and anti-frustration rules

## 1. Purpose

Dynamic Events make the galaxy respond to active play without creating random unavoidable punishment or violating canonical story/world state.

## 2. Dynamic Event ID

Every generated dynamic event has a persistent **Dynamic Event ID**.

It records:

- event type;
- stable seed;
- source/location;
- involved faction(s);
- eligibility snapshot;
- lifecycle state;
- timers;
- generated mission/market/world effects;
- resolution state.

## 3. Stable Seed

The event seed is fixed at creation.

Reloading does not reroll:

- event type;
- key actors;
- offered major rewards;
- major hazard;
- attacker force;
- target location.

## 4. Canonical Lifecycle

Dynamic Events use:

Candidate  
→ Offered / Scheduled  
→ Active  
→ Resolved / Expired / Cancelled.

Not every event uses an Offered state; a world incident can be Scheduled directly.

## 5. Candidate

A Candidate is evaluated against eligibility but is not yet committed to the world.

Rejected candidates leave no player-visible event.

## 6. Offered

An optional opportunity is visible/known to the player through a valid information source.

It may expire after Active Game Time.

## 7. Scheduled

A committed event is expected to begin/arrive later, such as a convoy or hostile force already dispatched.

## 8. Active

The event currently affects world/market/station/mission state.

## 9. Resolved

Its terminal outcome has committed.

One-time rewards/consequences cannot fire again.

## 10. Expired

An optional opportunity elapsed without acceptance/interaction.

Expiration does not count as mission failure unless an authored accepted obligation says otherwise.

## 11. Cancelled

The event became invalid before activation due to a legitimate world-state change.

The reason is recorded.

## 12. Event Families

Baseline families:

1. Distress / Survivor Rescue;
2. Salvage Opportunity;
3. Resource Discovery / Extraction Window;
4. Trader / Convoy Arrival;
5. Market Shortage / Surplus;
6. Faction Conflict / Patrol;
7. Route Disruption / Restoration Opportunity;
8. Anomaly / Research Event;
9. Station Emergency;
10. Horizon Station Defense Event;
11. NPC Raid/Conflict Opportunity;
12. Recovery / Follow-Up Event.

## 13. Mission-Producing Events

An event can create a GDS-8 Mission Offer.

Once that Mission ID is generated, GDS-8 lifecycle/seed/objectives are authoritative.

The Dynamic Event does not create a parallel mission state machine.

## 14. Horizon Defense Event Creation

Dynamic Events owns **when** a GDS-11 Horizon Defense Event is generated.

GDS-11 owns its execution after `Defense Event ID` creation.

## 15. Horizon Attack Eligibility

A procedural Horizon attack candidate requires all of:

- an attacker faction/group with plausible hostility/motive;
- a valid route/reach to Aster's Rest;
- a world source for the attacking force;
- Horizon Station currently existing/operational enough to be a meaningful target;
- no protected recovery/grace blocker;
- no conflicting campaign-authored state that forbids the attack.

## 16. No Threat From Nothing

An attack cannot generate because a hidden random timer decided so if no faction/world source can plausibly mount it.

## 17. Attack Motivation

Generated attacker objectives can be weighted by known world state:

- resource theft;
- retaliation;
- strategic sabotage;
- recover/capture asset;
- route conflict;
- Blackwake opportunism.

Attackers do not omnisciently target hidden stock they have no intelligence about.

## 18. Recovery Grace

After a **Major or Severe** Horizon defense outcome, procedural Horizon attack generation enters a **Recovery Grace** state.

During Recovery Grace, no new ordinary procedural Horizon raid can be created.

Baseline initial target: **30 minutes of Active Game Time**, with the timer extending while Horizon remains in a defined Critical Recovery state.

The exact duration is tuneable; the existence of recovery protection is fixed.

## 19. Authored Exception

A main-story crisis can override normal procedural Recovery Grace only when explicitly authored and telegraphed.

Procedural events cannot.

## 20. Event Concurrency

Baseline player-facing limits:

- maximum **3** concurrently Offered optional external Dynamic Events;
- maximum **1** active/scheduled Critical Horizon Station Event;
- generated ordinary offers do not count canonical story missions against this cap.

Excess candidates wait/reject rather than flooding the player.

## 21. One External Deployment Rule

Multiple Event/Mission offers may exist, but the player still has only one GDS-8 external Deployed Mission Instance at a time.

## 22. Eligibility Inputs

Generation can consider:

- current Reach access;
- canonical system/route;
- story phase;
- faction relationship;
- previous event history;
- market state;
- target persistence;
- station capability/vulnerability;
- known discoveries;
- current event concurrency;
- cooldown/grace.

It does not directly inspect player gear to scale the galaxy to the player.

## 23. No Universal Player-Power Matching

Dynamic Events are drawn from world-valid threat envelopes.

An event can be too dangerous for the current player.

Threat/recommendation communicates that risk under GDS-8.

## 24. Geographic Validity

Events must attach to actual GDS-7 Strategic Locations/routes or valid procedural side locations.

They cannot relocate canonical factions/story sites.

## 25. Information and Discovery

The player learns of an event only through a valid source such as:

- station/ship communications;
- faction message;
- distress beacon;
- sensor contact;
- physical encounter;
- market/news contact.

An undiscovered event need not appear magically on the map.

## 26. Distress Event

Can generate a Rescue/Repair/Escort mission with stable survivor/ship/location identity.

Ignoring it can lead to authored expiration/resolution, but it cannot delete mandatory campaign progress.

## 27. Salvage Event

Can represent a newly detected wreck, post-battle debris field, or temporary safe salvage window.

Loot is generated once and physically conserved.

## 28. Resource Event

Can reveal a temporary-access deposit/site.

The deposit remains finite under GDS-4.

## 29. Trader / Convoy Event

A convoy is a world supply source that can:

- temporarily add market stock;
- create escort/trade opportunity;
- be attacked by valid factions;
- depart after active-time schedule.

It is not a menu-only stock reset.

## 30. Market Event

Shortage/Surplus changes market modifiers/stock through Economy rules.

It cannot create negative-price loops or duplicate stock.

## 31. Faction Conflict

Can create:

- defense;
- escort;
- raid;
- recovery;
- patrol;
- negotiation opportunity.

Faction reputation consequences use existing faction authority.

## 32. Route Event

A route disruption can affect access/risk only when a plausible route condition exists.

Mandatory campaign routes cannot be permanently removed without an alternate recovery path.

## 33. Anomaly Event

Can create Research Evidence opportunity or hazardous survey mission.

It cannot award generic Science Points.

## 34. Station Emergency

Internal station emergencies remain GDS-2 execution authority.

Dynamic Events can trigger them from valid state/world causes.

## 35. Event Expiration

Expiration uses Active Game Time.

True Pause/offline time stops the timer.

## 36. Scheduled Arrival

A Scheduled event such as convoy/attack has a persistent ETA.

Destroying its originating communication after dispatch does not retroactively cancel a committed force unless the event's rules provide a recall path.

## 37. Cooldowns

Event families can use Active-Time cooldowns to prevent repetition.

The same exact event type/location should not repeatedly fire without world justification.

Exact cooldown durations are tuneable.

## 38. Anti-Frustration Weighting

Generation must consider unresolved severe player setbacks.

Examples:

- do not repeatedly generate Horizon raids during Critical Recovery;
- avoid stacking multiple ordinary high-pressure station emergencies without an authored crisis;
- avoid offering only unreachable missions.

This affects event eligibility/weights, not hidden combat scaling.

## 39. No Mandatory Story Softlock

A procedural Dynamic Event cannot permanently destroy/steal the sole required object, character, route, or Knowledge Asset needed to continue the main campaign.

## 40. Target Recovery Integration

Persistent NPC Raid Targets can receive recovery events/processes:

Damaged/Depleted  
→ Recovery Scheduled  
→ repair/replenishment progresses during Active Game Time  
→ recovered components/stock commit.

Recovery uses plausible faction/economic support and cannot restore unique stolen objects.

## 41. Target Recovery Rate

Exact repair/replenishment times are tuneable by target/faction/scale.

The fixed rule is that recovery:

- is not instantaneous;
- is persistent;
- does not happen offline;
- cannot duplicate stolen unique items;
- can be interrupted by new valid events.

## 42. Event Rewards

Rewards use GDS-8 and Loot/Reward Rules.

Each event outcome reward commits once.

## 43. Ignoring Optional Events

Optional events are not chores.

Ignoring an Offered event may change local world/economic state only where the event explicitly defines a natural outcome.

The player is not globally penalized simply for not accepting procedurally generated content.

## 44. Deterministic Resolution

Off-screen event resolution that legitimately proceeds during Active Game Time uses the saved event seed/state.

Save/load cannot reroll its outcome.

## 45. Save/Persistence

Persist:

- Dynamic Event ID;
- seed;
- lifecycle;
- source/location;
- timers;
- generated mission link;
- committed world/economic effects;
- cooldown/grace state.

## 46. Explicit Non-Goals

Dynamic Events do not provide:

- real-world daily events;
- offline attacks;
- endless event spam;
- gear-matched world scaling;
- arbitrary threats without world source;
- procedural permanent campaign softlocks;
- reload rerolling.

## 47. Tuneable Parameters

Generation weights, offer duration, cooldowns, recovery grace duration, attack frequency, target recovery rates, concurrency caps, and event-specific probabilities are tuneable except where an initial baseline cap is explicitly stated.

## 48. Dependencies

This specification depends on GDS-2 Station Events, GDS-7 World/Factions, GDS-8 Missions, GDS-11 Defense Events, Economy, Loot, Time, Persistence, Failure/Recovery, and Progression.

## 49. Open Questions

None in the Dynamic Event baseline.
