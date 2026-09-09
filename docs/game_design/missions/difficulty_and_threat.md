# Mission Difficulty and Threat

> **Status:** Draft  
> **Authority:** Mission threat representation, threat axes, overall threat calculation, intel uncertainty, capability warnings, and non-scaling mission danger

## 1. Purpose

Threat ratings communicate what makes a mission dangerous without converting the galaxy into universal player-scaled content.

## 2. Threat Is Not Difficulty Setting

Mission Threat describes the in-world operation.

Global player-selected Difficulty is a separate GDS-12 concern.

## 3. Threat Axes

Every mission can rate four axes from 0–5:

1. Hostile Threat;
2. Environmental Threat;
3. Operational Complexity;
4. Extraction Risk.

Unknown axes can display a question mark until sufficient intelligence exists.

## 4. Fixed Axis Scale

- **0 — Minimal**
- **1 — Low**
- **2 — Moderate**
- **3 — High**
- **4 — Severe**
- **5 — Extreme**

These labels/values are canonical.

## 5. Hostile Threat

Represents known expected combat opposition.

Inputs can include:

- enemy capability;
- numbers;
- defenses;
- reinforcements;
- hostile ships;
- dangerous wildlife/machines.

GDS-9 defines actual combat power.

## 6. Environmental Threat

Represents known environment danger.

Inputs can include:

- vacuum;
- radiation;
- extreme temperature;
- contamination;
- storms;
- unstable terrain;
- debris/anomaly hazards.

## 7. Operational Complexity

Represents noncombat task difficulty.

Inputs can include:

- multi-stage repair;
- difficult navigation;
- complex search;
- cargo handling;
- escort coordination;
- multiple dependent objectives;
- specialized tools.

## 8. Extraction Risk

Represents difficulty of leaving with player/objective/loot intact.

Inputs can include:

- long return path;
- hostile reinforcements;
- damaged transport;
- contested airspace;
- limited extraction windows;
- hazardous cargo.

## 9. Overall Threat

The canonical **Overall Threat** is the maximum known axis value.

Example:

Hostile: 1  
Environmental: 5  
Operational: 2  
Extraction: 2

Overall Threat = 5 — Extreme.

This avoids hiding one extreme hazard inside an average.

## 10. Unknown Threat

If one or more important axes are unknown, Overall Threat is shown as at least the maximum known value plus an **Intel Incomplete** indicator.

The UI never pretends incomplete intel is exact.

## 11. Threat vs Player Capability

The game may compare mission requirements with the player's current selected:

- ship;
- suit;
- weapons;
- tools;
- cargo/passenger capacity;
- Reach;
- known endurance.

The result is a recommendation, not world scaling.

## 12. Recommendation States

Baseline:

- Favorable;
- Reasonable;
- Risky;
- Critical;
- Unknown.

## 13. No Automatic Enemy Scaling

Accepting a high-threat mission with weak gear does not lower enemy stats.

Returning later with advanced gear does not automatically raise them.

## 14. Location Envelope

Procedural missions draw threat from the GDS-7 sector/location envelope.

The generator can vary within that envelope.

## 15. Story Threat

Story missions have authored threat expectations.

Routine retry does not secretly make them easier unless a future accessibility/difficulty setting explicitly changes combat/system parameters.

## 16. Threat and Rewards

Higher threat can support better reward budgets.

It does not guarantee random higher-rarity material tiers.

GDS-12 owns economic balance.

## 17. Intelligence

Better intel can:

- reveal exact axes;
- identify hazards;
- identify faction/enemy type;
- reveal extraction risks.

Intel does not reduce actual threat by itself.

## 18. Preparation

Preparation can reduce **effective risk** without changing mission's underlying threat rating.

Example:

Radiation Threat 4 remains 4 even if the player equips a superior radiation suit.

The recommendation can improve from Critical to Reasonable.

## 19. Dynamic Escalation

A mission can change threat during play through:

- reinforcements;
- storm;
- alarm;
- objective consequence;
- damaged extraction.

If predictable, the mission warns the player.

## 20. Threat Update

New intelligence can update displayed threat.

The underlying mission state is not rerolled.

## 21. UI

Mission briefing shows four axes, overall threat, intel confidence, known requirements, and current-loadout recommendation.

## 22. Explicit Non-Goals

Threat does not provide:

- universal level scaling;
- one opaque difficulty number only;
- automatic reward rarity tiers;
- hidden exact certainty from incomplete intel;
- automatic nerfing because current gear is weak.

## 23. Tuneable Parameters

Tuneable values include scoring rules inside each fixed 0–5 axis, recommendation thresholds, and generator threat distributions.

## 24. Dependencies

This specification depends on World threat bands, Generation, Hazards, Combat, Extraction, Player/Ship capability, Intelligence, and GDS-12 Difficulty/Economy.

## 25. Open Questions

None in the threat-representation baseline.
