# Difficulty and Scaling

> **Status:** Design Complete  
> **Authority:** Player-selectable difficulty profiles, allowed tuning axes, prohibited scaling, application timing, threat relationship, custom difficulty boundary, and fairness invariants

## 1. Purpose

Difficulty changes the pressure and forgiveness of gameplay without rewriting the world, invalidating progression, or giving AI impossible information.

## 2. World Threat Is Independent

GDS-8 Threat remains an authored property of the mission/location:

- Hostile Threat;
- Environmental Threat;
- Operational Complexity;
- Extraction Risk.

Changing Difficulty does not change these 0–5 ratings.

## 3. No Universal Player Scaling

The galaxy does not scale its enemies, loot, station systems, or mission requirements to the player's current equipment.

Previously dangerous content can become easier because the player genuinely gained capability.

## 4. Canonical Difficulty Profiles

Baseline profiles:

- Assisted;
- Standard;
- Veteran;
- Extreme;
- Custom.

**Standard** is the intended default balance reference.

## 5. Difficulty Axes

Difficulty may modify only declared axes:

1. incoming combat damage pressure;
2. enemy reaction time;
3. enemy aiming error/precision;
4. enemy tactical aggression/coordination within existing AI knowledge;
5. environmental exposure accumulation;
6. recovery/repair burden within existing physical rules;
7. optional interface assistance through GDS-13 Custom settings.

## 6. Initial Profile Targets

These are initial tuneable balance targets, not immutable formulas.

| Axis | Assisted | Standard | Veteran | Extreme |
| --- | ---: | ---: | ---: | ---: |
| Player incoming combat damage | 0.70x | 1.00x | 1.15x | 1.30x |
| Enemy reaction-time duration | 1.25x | 1.00x | 0.90x | 0.80x |
| Enemy aim-error magnitude | 1.35x | 1.00x | 0.85x | 0.70x |
| Environmental exposure accumulation | 0.75x | 1.00x | 1.10x | 1.20x |
| Ordinary recovery material burden | 0.75x | 1.00x | 1.10x | 1.20x |

Exact final values are tuneable after playtesting under DD-009.

## 7. Outgoing Player Damage

Baseline Difficulty does **not** modify player outgoing weapon damage.

Weapons retain their actual GDS-9 statistics.

This keeps target armor/weak points and engineering comparisons stable.

## 8. Enemy Health and Armor

Difficulty does **not** inflate enemy Health, armor, shield capacity, or ship hull state by a generic multiplier.

Different enemy models can naturally be stronger because of world/content design.

## 9. AI Knowledge

Difficulty can make AI react faster or coordinate more effectively, but cannot grant:

- omniscient player position;
- knowledge through arbitrary walls;
- hidden target data the AI never perceived;
- teleport flanks;
- impossible line of fire.

GDS-9 information rules remain invariant.

## 10. Enemy Accuracy

Difficulty changes modeled aim error/reaction parameters.

It does not turn enemy attacks into guaranteed hit rolls.

All attacks still use GDS-9 physical hit resolution.

## 11. Environmental Difficulty

Environmental exposure modifiers alter the rate/severity at which a valid exposure affects player survival.

They do not:

- create hazards that are absent from the location;
- remove required pressure sealing logic;
- make vacuum breathable;
- bypass physical protection prerequisites.

## 12. Recovery Burden

Difficulty can tune ordinary repair/medical material amounts or condition penalties within defined recovery recipes.

It cannot:

- permanently kill protected crew on Extreme;
- delete the Primary Ship on Extreme;
- delete Horizon Station on Extreme;
- resurrect robots on Assisted.

Global persistence philosophy is invariant.

## 13. Economy and Rewards

Difficulty does not multiply:

- Credits;
- market prices as a reward/penalty shortcut;
- physical resource yields;
- loot tables;
- Blueprint acquisition;
- Research Evidence;
- reputation;
- story unlocks.

The player receives the same progression opportunities regardless of selected profile.

## 14. Mission Generation

Difficulty does not silently replace a selected mission with a lower/higher Threat version.

Dynamic Events/Missions remain world-generated.

Difficulty changes execution pressure only on allowed axes.

## 15. Procedural Seed Invariance

Changing Difficulty does not reroll:

- Mission seed;
- loot;
- enemy identity/composition seed;
- survivor identity;
- raid target layout;
- Dynamic Event seed.

## 16. Player-Selected Changes

Difficulty can be changed during an existing save.

The change affects future simulation from the next stable simulation boundary.

It does not retroactively rewrite already committed damage, loot, mission results, resource consumption, or event state.

## 17. Combat Change Timing

If Difficulty is changed from True Pause during combat:

- already fired projectiles/Damage Packets keep their committed values/state;
- subsequent damage/AI decisions use the new profile.

This prevents transaction ambiguity.

## 18. Custom Difficulty

Custom allows the player to independently choose supported difficulty axes within bounded options.

Custom cannot disable fundamental game invariants such as:

- physical resource ownership;
- extraction transaction;
- stable procedural seeds;
- one external deployed mission;
- no offline progression.

## 19. Accessibility vs Difficulty

Accessibility options are not inherently difficulty settings.

GDS-13 may expose options such as:

- aim assistance;
- input assistance;
- visual/audio cues;
- subtitle/navigation support.

Those should not be treated as lower-skill penalties or reduce rewards.

## 20. Assisted Is a Full Game Mode

Assisted retains:

- all story content;
- all technologies;
- all items;
- all missions/raids;
- all endings;
- all progression.

No content is withheld because the player uses Assisted.

## 21. Extreme Is Not Ironman

Extreme does not automatically enforce:

- permadeath;
- single save slot;
- disabled manual saving;
- permanent story lockouts.

Ironman is not a baseline mode.

## 22. Difficulty and Station Defense

Off-screen Horizon defense uses the same actual station systems at every Difficulty.

Allowed Difficulty modifiers can affect attacker combat effectiveness/recovery burden, but cannot replace deterministic GDS-11 simulation with a Defense Score.

## 23. Difficulty and Robots

Player robot physical stats are not generically boosted on Assisted or reduced on Extreme.

Enemy pressure changes through allowed combat axes.

Destroyed player robots remain Destroyed at every difficulty.

## 24. Difficulty and Crew

Crew permanence, skill progression, and workload rules do not change by Difficulty.

Recovery rates/material requirements may use the declared recovery-burden axis where appropriate.

## 25. Difficulty and Spacecraft

Ship flight physics, Reach requirements, Fuel/Propellant consumption logic, and fitting constraints remain unchanged.

Combat pressure can differ through incoming damage/enemy behavior only.

## 26. Difficulty and Research

Research prerequisites, evidence, and Blueprints remain identical.

Difficulty never substitutes for missing research or grants bonus Science Points.

## 27. Telemetry/Balance

For balancing, game systems may record the selected Difficulty alongside outcome telemetry.

Difficulty has no hidden effect beyond documented axes.

## 28. Explicit Non-Goals

Difficulty does not provide:

- universal enemy HP inflation;
- universal player damage boosts;
- loot/reward multipliers;
- AI omniscience;
- different story endings;
- automatic player-level matching;
- Extreme permadeath;
- Assisted content removal.

## 29. Tuneable Parameters

The profile multipliers, AI timing/precision ranges, recovery burden, and custom-option bounds are tuneable.

The list of allowed/prohibited axes is a fixed rule.

## 30. Dependencies

This specification depends on GDS-8 Threat, GDS-9 Combat/AI, GDS-5 Survival/Health, GDS-11 Station Defense, Failure/Recovery, Loot/Rewards, and GDS-13 Accessibility.

## 31. Open Questions

None in the difficulty/scaling baseline.
