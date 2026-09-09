# Reputation and Relations

> **Status:** Draft  
> **Authority:** Player-faction reputation state, relationship tiers, reputation changes, hostility, access, faction conflict side effects, and relation visibility

## 1. Purpose

Reputation records how each faction evaluates the player's demonstrated actions.

It is not a morality score.

## 2. Independent Reputation

Each standard faction tracks its own player relationship.

Actions affecting Helios do not automatically change Meridian reputation unless an explicit event links them.

## 3. Reputation Scale

Standard human factions use a normalized reputation value:

-100 to +100.

The UI also shows a named tier.

## 4. Relationship Tiers

Baseline:

- **Enemy:** -100 to -61
- **Hostile:** -60 to -31
- **Distrusted:** -30 to -11
- **Neutral:** -10 to +10
- **Accepted:** +11 to +30
- **Trusted:** +31 to +60
- **Allied:** +61 to +100

Exact value thresholds are fixed by this baseline; reward values per action are tuneable.

## 5. Enemy

Faction treats the player as a strategic enemy.

Possible consequences:

- attack-on-sight in controlled military areas;
- no normal docking/trade;
- hostile missions/raids;
- restricted route access.

## 6. Hostile

Faction actively opposes the player but local exceptions can exist.

## 7. Distrusted

Faction permits limited interaction but restricts valuable access.

## 8. Neutral

Normal baseline interaction where local rules permit.

## 9. Accepted

Unlocks improved contracts/services/contacts.

## 10. Trusted

Unlocks sensitive missions, advanced technology access, and stronger support.

## 11. Allied

Represents major strategic partnership.

It does not make the player's station faction-owned.

## 12. Reputation Sources

Reputation can change through:

- mission outcomes;
- rescues;
- faction aid;
- trade/contract completion;
- attacking faction assets;
- sabotage;
- stealing secured resources;
- choices in faction conflicts;
- major story decisions.

## 13. No Passive Decay

Reputation does not drift toward zero merely because active game time passes.

A faction remembers meaningful history.

## 14. No Offline Change

Reputation does not change while the game is closed.

## 15. Transparent Consequences

Before a deliberate major action, the game communicates known likely faction consequences when the player has sufficient context.

Example:

Attacking this Helios convoy will significantly reduce Helios reputation.

## 16. Hidden Consequences

Consequences may be unknown only when:

- the action is not observed;
- faction identity is hidden;
- intelligence is incomplete.

The system does not secretly punish ordinary actions without discoverable logic.

## 17. Witness/Knowledge Boundary

A faction can react only when it can plausibly learn of an action through:

- witnesses;
- sensors;
- records;
- communications;
- mission ownership;
- later investigation.

Exact stealth detection belongs to Combat/Missions.

## 18. Hostility State

Local hostility and global reputation are related but distinct.

A neutral faction patrol can become temporarily hostile because the player attacks it.

Immediate hostility can occur before global reputation updates.

## 19. Ceasefire

A hostile local encounter can end through:

- mission resolution;
- surrender;
- withdrawal;
- authored negotiation.

A ceasefire does not automatically reset faction reputation.

## 20. Reputation Rewards

Higher reputation can unlock:

- docking;
- services;
- trade inventory;
- blueprints;
- research data;
- route information;
- survivors/specialists;
- faction missions;
- ship variants.

GDS-12 owns prices/reward balancing.

## 21. Reputation Restrictions

Low reputation can cause:

- docking denial;
- higher security;
- restricted trade;
- blocked contracts;
- hostile patrols.

## 22. Faction Conflict Choice

Supporting one faction can explicitly reduce relation with an opposing faction when the chosen action materially harms it.

This is not automatic zero-sum diplomacy for every mission.

## 23. Major Irreversible Decisions

Some main-story/faction decisions can impose a minimum/maximum relation shift or permanently alter access.

Such cases must be clearly authored and communicated.

## 24. Repairing Relations

Hostile relations can generally be improved through:

- reparations/service;
- neutral intermediary missions;
- rescue/aid;
- ceasefire questline.

Enemy-tier story lockouts can require a specific reconciliation path.

The baseline avoids accidentally making the entire campaign unwinnable through one early mistake.

## 25. Continuance Relation

The Continuance does **not** use the -100 to +100 standard reputation scale.

Instead, interaction uses node-specific states such as:

- Unknown;
- Unauthorized;
- Tolerated;
- Authenticated;
- Cooperative;
- Containment Hostile.

These states are narrative/technology-driven.

## 26. Minor Factions

Minor factions can use either:

- the standard reputation scale;
- a simplified authored relation state.

## 27. Faction-to-Faction Relations

World state can define relations between factions.

The player does not directly control these values.

Later dynamic systems may change them through authored events.

## 28. Territory Access

Reputation affects access but does not rewrite physical ownership.

A player with Allied reputation still does not own a Helios station.

## 29. UI

Faction screen shows:

- tier;
- numeric reputation;
- known benefits/restrictions;
- recent major changes;
- known conflicts;
- active warrants/hostility where applicable.

## 30. Persistence

Reputation, relationship tier, major decision flags, warrants, and Continuance node states persist.

## 31. Edge Cases

If reputation crosses multiple thresholds at once, all resulting tier changes resolve once.

If the player destroys an unidentified faction ship and later its identity is discovered, delayed reputation consequences can occur only if evidence of responsibility exists.

If a faction becomes story-hostile despite prior positive reputation, the story state must explain whether this is local leadership, civil conflict, or faction-wide change.

## 32. Explicit Non-Goals

Reputation does not provide:

- one universal morality bar;
- passive decay;
- automatic zero-sum changes;
- ownership of faction assets from high reputation;
- hidden arbitrary punishment.

## 33. Tuneable Parameters

Tuneable values include reputation gains/losses, service unlock requirements within the fixed tier structure, and recovery mission values.

## 34. Dependencies

This specification depends on Factions, Missions, Economy, Narrative, Combat, Raids, and Persistence.

## 35. Open Questions

None in the standard reputation framework.
