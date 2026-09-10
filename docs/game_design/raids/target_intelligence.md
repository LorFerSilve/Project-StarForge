# Raid Target Intelligence

> **Status:** Design Complete  
> **Authority:** Raid-intelligence categories, confidence, acquisition, staleness, target revelation, preparation impact, and anti-omniscience rules

## 1. Purpose

Raid preparation should reward reconnaissance without making perfect information mandatory.

Intelligence answers:

- what is at the target;
- what is known with confidence;
- which access routes exist;
- which defenses/systems matter;
- what can be stolen/sabotaged;
- how quickly the target can reinforce.

## 2. Intel Record

Each Raid Target maintains Intel Records for discoverable categories.

A record contains:

- Intel Category;
- Confidence State;
- source;
- observed target state;
- observation time/version;
- uncertainty notes;
- revealed entities/areas.

## 3. Confidence States

Canonical states:

- Unknown;
- Suspected;
- Confirmed;
- Stale.

## 4. Unknown

No reliable actionable data exists.

UI does not reveal the target detail.

## 5. Suspected

There is credible but incomplete information.

Examples:

- possible missile battery;
- suspected service hatch;
- approximate storage section;
- likely reinforcement link.

Suspected intel cannot be displayed as exact certainty.

## 6. Confirmed

The player possesses sufficiently reliable current information for the stated detail.

Confirmed does not mean immutable.

## 7. Stale

Intel was previously reliable but a known target-state change or sufficient active-game evolution makes exact current accuracy uncertain.

Stale data remains visible with a warning rather than disappearing.

## 8. Intel Categories

Baseline categories:

1. Target Identity / Faction;
2. Approach Environment;
3. External Defenses;
4. Shield Coverage;
5. Sensors / Detection;
6. Docking / Access Points;
7. Breachable Entry Points;
8. Interior Layout;
9. Security / Lockdown;
10. Power / Control Dependencies;
11. Communications / Reinforcement Capability;
12. Storage / Loot Locations;
13. Mission-Critical Objective Location;
14. Extraction / Escape Constraints.

## 9. Intelligence Sources

Intel can come from:

- GDS-8 Reconnaissance/Survey mission;
- ship sensors;
- Recon Drone survey;
- hacked network/data;
- recovered map/data carrier;
- faction contact;
- prior visit/raid;
- visual observation during the raid;
- story-authored source.

## 10. No Universal Buy-All-Intel Button

GDS-12 economy may allow purchasing information from valid contacts.

Purchased intel still corresponds to specific categories/confidence and can be incomplete/stale.

## 11. Sensor Intel

Ship/robot/player sensors reveal only what their actual capability can detect.

A sensor scan cannot expose:

- arbitrary rooms behind all shielding;
- exact storage contents without a valid sensing/data path;
- hidden security credentials;
- unknown subsystem dependencies with no evidence.

## 12. Recon Mission

A dedicated recon mission can improve confidence before the raid.

Recon is physically played under GDS-8, not an automatic percentage roll.

Possible outputs:

- access routes;
- turret arcs;
- shield gaps;
- patrol behavior;
- local map sections;
- reinforcement timing clues.

## 13. Recon Drone

GDS-10 Recon Drone can collect raid intel if:

- physically deployed or positioned;
- sensors support target type;
- communication/data recovery succeeds.

Destroyed/lost drone does not magically upload data that was never transmitted or recovered.

## 14. Prior Raid Knowledge

Previously observed persistent geometry/systems remain known unless changed.

Prior destruction or repair can make old operational data Stale.

## 15. Intel and Target Persistence

If a player previously destroyed a turret and the target has not recovered it, the corresponding intel remains current.

If GDS-12 target recovery later replaces or relocates defenses through an authored process, prior intel can become Stale.

## 16. External Defense Intel

Can reveal:

- weapon type;
- approximate/exact position;
- firing arc;
- known ammo state only if observable;
- local sensor support;
- linked shield coverage.

Exact internal ammunition count is not granted without a valid data source.

## 17. Shield Intel

Can reveal:

- emitter positions;
- protected zones;
- known gaps;
- regeneration/support dependencies where discovered.

It does not grant a global shield HP estimate if shields are local zones.

## 18. Access Intel

Possible entries:

- authorized docking port;
- compromised credential path;
- maintenance airlock;
- EVA service hatch;
- cargo dock;
- breachable hull panel;
- damaged structural access.

Each access route has real prerequisites.

## 19. Interior Map Intel

Interior intel can be:

- none;
- partial room graph;
- department-level map;
- detailed known route.

It does not reveal moving defenders unless separately tracked.

## 20. Security Intel

Can reveal:

- door authorization levels;
- sensor/camera coverage;
- alarm nodes;
- security control location;
- lockdown boundaries;
- robot security presence.

## 21. Power / Dependency Intel

Can reveal causal opportunities such as:

- shield emitter powered through relay A;
- turret cluster linked to local magazine/power branch;
- security door network dependent on controller B.

This is high-value intelligence because it enables indirect sabotage.

## 22. Reinforcement Intel

Can reveal:

- whether external help can be called;
- communication node;
- nearby response force;
- approximate response delay;
- docking/arrival route.

Exact timing remains uncertain unless the source justifies precision.

## 23. Loot Intel

Loot intelligence can reveal:

- storage room category;
- likely resource type;
- strategic item presence;
- cargo transfer route.

It cannot guarantee exact quantities that were never observed/data-accessed.

## 24. Objective Intel

Mandatory raid objective location must have at least one valid discovery path.

The raid may begin without exact location if searching is intended gameplay.

## 25. Intelligence Quality and Preparation

Better intelligence can improve:

- route selection;
- equipment choice;
- squad composition;
- cargo planning;
- expected threat visibility;
- sabotage opportunities;
- extraction planning.

Intel does not directly reduce enemy damage/HP.

## 26. Unknown Risk

The player may launch with incomplete intel if no hard requirement is violated.

The briefing explicitly identifies unknown categories.

## 27. Hard Requirement Knowledge

If the game knows a raid is physically impossible with the selected configuration because of a confirmed hard blocker, deployment is blocked.

If the blocker is genuinely unknown, the game does not reveal it omnisciently; the player can discover it during the raid.

## 28. Live Intel Updates

During a raid, observation can update records immediately.

Examples:

- turret visually identified;
- map downloaded;
- reinforcement call detected;
- storage scanned;
- power dependency traced.

## 29. Intel Sharing

Functioning squad/ship/player communication can share valid discoveries among player systems.

Jamming can delay shared updates while local observer still knows its own data.

## 30. No Intel Reroll

Reload does not change intelligence source output for the same persistent target state and Mission ID.

## 31. Presentation Requirement

Raid briefing later distinguishes:

- confirmed;
- suspected;
- stale;
- unknown

without visually presenting all four as equivalent certainty.

## 32. Explicit Non-Goals

Raid intelligence does not provide:

- omniscient target map;
- exact invisible loot counts by default;
- perfect live enemy tracking from old intel;
- free full intel purchase;
- combat-stat buffs simply because intel exists.

## 33. Tuneable Parameters

Tuneable values include sensor resolution, recon exposure/time, staleness thresholds, confidence upgrade thresholds, and information precision.

## 34. Dependencies

This specification depends on GDS-5 Scanner, GDS-6 Sensors, GDS-7 World/Factions, GDS-8 Exploration, GDS-10 Recon/Hacker units, Station Security/Defenses, and Economy/Presentation.

## 35. Open Questions

None in the raid-intelligence baseline.
