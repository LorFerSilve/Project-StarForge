# Missions and Exploration Cross-Validation

> **Status:** Active Audit  
> **Authority:** GDS-8 consistency review only  
> **Purpose:** Validate mission lifecycle, procedural generation, zones, objectives, exploration, threat, hazards, extraction, rewards, failure, and story-mission execution against established global, resource, player, crew, spacecraft, world, and narrative rules.

## 1. Scope

This audit checks GDS-8 against:

- Global Game Rules;
- GDS-3 Survivors and Crew;
- GDS-4 Resources, Mining, Salvage, Research, and Blueprints;
- GDS-5 Player Character, Inventory, Health, Interaction, Tools, and Field Survival;
- GDS-6 Spacecraft, Navigation, Cargo, Docking, and Damage;
- GDS-7 World, Galaxy, Factions, Reputation, and Narrative.

## 2. Mission Instancing — Result: PASS

GDS-8 preserves the global rule that external missions occur in bounded mission instances/zones.

Each Mission ID and active Mission Instance has authoritative state.

The design does not require seamless planet or galaxy simulation.

## 3. One Deployed Mission — Result: PASS

The player may hold multiple Accepted missions but only one external Deployed Mission Instance at a time.

This prevents conflicting ownership of:

- player location;
- deployed ship;
- mission inventory security;
- extraction;
- active zone state.

No established system requires simultaneous deployed missions.

## 4. Active-Time Simulation — Result: PASS

Mission timers, hazards, transit-linked mission progress, and objective timing advance only during active game simulation.

True Pause stops mission simulation.

Closing the game does not advance missions.

This matches Global Rules.

## 5. Mission Failure Philosophy — Result: PASS

Routine failure:

- does not delete unrelated long-term progression;
- does not permanently kill the player;
- does not routinely permanently kill recruited crew;
- preserves previously secured station state;
- keeps consumed supplies consumed;
- can lose Field-Unsecured mission loot.

This matches Global Game Rules and GDS-5.

## 6. Player Inventory Security — Result: PASS

GDS-8 uses the established states:

- Secured Loadout;
- Field-Unsecured;
- Vehicle/Extraction-Secured;
- Station-Secured.

No alternate mission-only inventory ownership system was introduced.

## 7. Extraction Transaction — Result: PASS

Extraction now provides the missing authoritative commit rule.

At successful authorized extraction:

- eligible Field-Unsecured backpack contents become Vehicle/Extraction-Secured;
- they remain Player Inventory rather than being duplicated into ship cargo;
- pre-secured ship cargo remains with the ship;
- Station-Secured still requires actual station ownership/transfer.

This resolves the earlier GDS-5/GDS-6 mission dependency.

## 8. Resource Ownership — Result: PASS

Mining, salvage, pickups, cargo, and reward delivery preserve GDS-4 single authoritative physical ownership.

A mission never creates a second copy because an objective counter changed.

## 9. Mining and Salvage — Result: PASS

Mission Types/Objectives can require mining/salvage.

GDS-4 remains authoritative for:

- deposit reserve;
- Yield Grade;
- extraction output;
- salvage yield.

GDS-8 owns only mission purpose/location/security requirements.

## 10. Research / Blueprint Rewards — Result: PASS

Mission rewards can grant:

- Research Evidence;
- Blueprint Unlock;
- route/location knowledge.

They do not collapse Research and Blueprint into one unlock.

No generic Science Points or Mission XP were introduced.

## 11. Reward Delivery — Result: PASS

Field loot is distinct from mission Resolution Reward.

Physical sponsor rewards use valid physical delivery or a nonphysical Reward Delivery Claim until capacity exists.

Rewards do not spawn weightlessly into full inventory.

## 12. Survivor Rescue — Result: PASS

Mission objectives distinguish survivor states:

- Located;
- Stabilized;
- Escorted;
- Vehicle-Secured passenger;
- Delivered.

GDS-3 remains authoritative for survivor identity, recruitment, profession, and persistence.

Proximity to a ship is not rescue completion.

## 13. Temporary Passengers / Ship Capacity — Result: PASS

Extraction validates actual passenger and life-support capacity.

GDS-6 remains authoritative for spacecraft occupancy.

No mission teleports rescued survivors into unavailable seats.

## 14. Spacecraft Mission Integration — Result: PASS

GDS-8 uses GDS-6 for:

- travel;
- Reach;
- landing;
- docking;
- local flight;
- ship cargo;
- ship damage;
- recovery.

Missions define when these capabilities are required without redefining them.

## 15. World Geography — Result: PASS

GDS-8 Story Mission Execution uses the fixed GDS-7 canonical systems/locations.

Procedural missions can add side content but cannot randomize mandatory campaign geography.

## 16. Story Mission Chain — Result: PASS

All 18 canonical GDS-7 main-story mission/stage IDs are mapped to:

- mission archetype;
- location;
- zone composition;
- mandatory objective chain;
- extraction mode;
- failure/retry contract;
- fixed story output.

GDS-8 therefore does not leave main-story mission structure to implementation.

## 17. Main-Story Retry — Result: PASS

Routine tactical failure cannot permanently brick campaign progress.

Unique story evidence/characters remain governed by authored retry/persistence rules.

No mandatory campaign evidence relies on procedural reroll.

## 18. Procedural Determinism — Result: PASS

Procedural Mission IDs receive persistent seeds.

Reloading cannot reroll:

- survivor identity;
- major loot;
- deposit placement;
- objective layout;
- hazard schedule;
- major encounter composition seed.

This matches GDS-3 anti-reroll survivor philosophy.

## 19. Procedural World Validity — Result: PASS

Generation validates:

- biome;
- faction;
- resource plausibility;
- route/Reach;
- objective connectivity;
- required access;
- extraction path;
- unique-content duplication.

Broken missions are discarded before presentation.

## 20. Mission Zones — Result: PASS

Zones are bounded and support:

- Surface;
- Interior;
- Local Spaceflight;
- EVA;
- Mixed.

Active mission resources/containers/targets do not respawn from room changes or save/load.

## 21. Exploration — Result: PASS

Exploration uses:

Unknown → Detected → Identified → Surveyed → Resolved.

Scanner/ship sensors reveal only supported information.

No omniscient map or universal scanner wallhack was introduced.

## 22. Mission Threat — Result: PASS

Threat uses four independent 0–5 axes:

- Hostile;
- Environmental;
- Operational;
- Extraction.

Overall Threat is the maximum known axis.

Threat does not automatically scale to the player.

This preserves DD-015 / Global Rule 20.

## 23. Difficulty Boundary — Result: PASS

Mission Threat describes world danger.

Global player-selected Difficulty remains GDS-12 authority.

No conflict exists between fixed location threat and future accessibility/difficulty modifiers.

## 24. Environmental Hazards — Result: PASS

Hazards consume World environment and feed Player/Ship systems.

GDS-8 defines:

- local pattern;
- telegraphing;
- activation;
- persistence;
- mitigation paths.

It does not redefine biological damage or ship system formulas.

## 25. No Arbitrary Instant Death — Result: PASS

Unknown hazards may surprise the player but must follow plausible detection/environment rules.

Known unavoidable lethal entry conditions become deployment Hard Requirements.

## 26. Objectives — Result: PASS

Objectives use a directed acyclic dependency graph with:

- Primary;
- Secondary;
- Optional;
- Extraction;
- Hidden/Discoverable.

Mandatory hidden objectives require a valid discovery route.

Objective completion/failure commits once.

## 27. Failure Transaction — Result: PASS

Failure has a fixed transaction order covering:

- objective freeze;
- ownership snapshot;
- player defeat;
- secured cargo preservation;
- Field-Unsecured loss;
- survivor/ship state;
- faction/narrative consequences;
- rewards;
- instance closure;
- retry state.

This removes frame-order/save-load ambiguity.

## 28. Abandonment — Result: PASS

Active mission abandonment:

- forfeits success;
- does not teleport the player;
- does not refund supplies;
- retains physical withdrawal/extraction requirements.

## 29. Combat Authority Boundary — Result: PASS

GDS-8 defines where/why combat can occur and what combat outcome an objective requires.

GDS-9 is now first-pass authoritative for:

- damage;
- weapons;
- armor;
- shields;
- enemy combat behavior;
- Kill/Incapacitate/Destroy/Disable/Drive Off/Capture/Clear Area semantics.

**Result: PASS**

GDS-8 retains mission lifecycle/objective authority while GDS-9 supplies exact combat resolution.

## 30. Robot Authority Boundary — Result: PASS

GDS-8 can reserve robot deployment/transport objectives or finale capability requirements.

GDS-10 remains authoritative for robot classes, AI, squad commands, manufacturing, and damage.

## 31. Raid Authority Boundary — Result: PASS

Ordinary small-scale Boarding Operation is a GDS-8 mission type.

Large fortified station assault with intelligence, external defense, breaching, sabotage, looting, escalation, and extraction remains GDS-11.

MS-A301 explicitly hands a full hostile assault route to GDS-11 if that branch becomes a true Raid.

## 32. Economy Boundary — Result: PASS

GDS-8 defines reward entitlement/claim semantics.

GDS-12 remains authoritative for:

- currency;
- prices;
- final economic reward values;
- contract rotation economy.

No placeholder currency was made a physical Resource.

## 33. Internal GDS-8 Consistency — Result: PASS

### System vs Objectives

Mission state owns lifecycle; Objectives own individual completion graph.

### Generation vs Zones

Generator creates validated zone content; Zone owns active local state.

### Exploration vs Objectives

Discovery can reveal objectives without automatically completing them.

### Threat vs Hazards

Hazards contribute to Environmental Threat; Threat does not replace actual hazard state.

### Extraction vs Failure

Successful extraction secures ownership; failure uses current actual security snapshot.

### Rewards vs Extraction

Field acquisitions require extraction; sponsor Resolution Rewards use separate commit/delivery semantics.

### Story Execution vs Narrative

GDS-8 implements fixed GDS-7 story beats without changing canon.

## 34. Dependencies Preventing Design Complete

GDS-8 remains first-pass pending:

### GDS-9 Combat — First-Pass Resolved

**Result: PASS**

GDS-9 now defines combat encounter resolution primitives, exact combat-objective outcome semantics, generic enemy AI, physical friendly-fire/protected-target behavior, and combat conditions used by contested extraction.

GDS-8 remains authoritative for mission/extraction state transitions.

### GDS-10 Robots — First-Pass Resolved

**Result: PASS**

GDS-10 now defines physical robot mission deployment, Tactical Control Capacity, squad composition, support/repair/hacking/breaching/demolition roles, robot extraction/loss, and finale-ready squad capability.

GDS-8 remains authoritative for Mission Instance and extraction/failure state.

### GDS-11 Raids

Required for:
- full station-assault branches;
- hostile archive raids;
- finale raid/boarding phases.

### GDS-12 Economy, Progression, Difficulty, Persistence

Required for:
- exact reward values;
- procedural contract refresh;
- difficulty modifiers;
- final autosave/manual-save transaction semantics;
- coalition capability thresholds;
- dynamic world-event mission creation.

### GDS-13 Presentation

Required for:
- mission briefing UX;
- threat visualization;
- objective markers;
- extraction feedback;
- local map;
- failure/reward screens.

## 35. First-Pass Conclusion

No blocking contradiction was found.

GDS-8 can be marked:

**First-Pass Complete — Cross-Validation Pending**

The next dependency-driven phase is GDS-9 Combat.
