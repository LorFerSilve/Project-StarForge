# Raids and Station Defense Cross-Validation

> **Status:** Active Audit  
> **Authority:** GDS-11 consistency review only  
> **Purpose:** Validate offensive raids, target intelligence, preparation, external assault, boarding, interior attack, sabotage, looting, escalation, extraction, outcomes, story integration, and Horizon Station defense against all established upstream domains.

## 1. Scope

This audit checks GDS-11 against:

- Global Game Rules;
- GDS-2 Home Station;
- GDS-3 Crew;
- GDS-4 Resources/Crafting;
- GDS-5 Player;
- GDS-6 Spacecraft;
- GDS-7 World/Factions/Narrative;
- GDS-8 Missions/Exploration;
- GDS-9 Combat;
- GDS-10 Robots.

## 2. Raid as Specialized Mission — Result: PASS

Offensive raids reuse GDS-8:

- Mission ID;
- Mission Instance;
- lifecycle;
- deterministic seed;
- objective graph;
- Threat;
- extraction;
- failure/abandonment;
- rewards.

GDS-11 adds raid phases rather than creating a second mission engine.

## 3. Single-Player Core — Result: PASS

GDS-11 defines complete single-player raid/defense gameplay against NPC/faction targets.

No synchronous or asynchronous PvP is required.

This preserves DD-010.

## 4. No Offline Progression — Result: PASS

Offensive raids and Horizon Station attacks advance only during active game time.

Real-world time while the application is closed does not:

- create;
- progress;
- resolve

raids.

This preserves DD-012.

## 5. One External Mission Instance — Result: PASS

A hostile attack on Horizon Station while the player is on another mission is treated as a persistent Home Station Event, not a second player-deployed Mission Instance.

Therefore GDS-8's one-Deployed-Mission rule remains intact.

## 6. Physical Target Model — Result: PASS

Raid targets use real raid-relevant:

- geometry;
- defenses;
- shield zones;
- access;
- security;
- power/control dependencies;
- atmosphere;
- storage;
- communication;
- reinforcement links.

No global Raid HP bar exists.

## 7. Target Persistence — Result: PASS

Committed target changes can persist:

- destroyed defenses;
- depleted storage;
- breach;
- disabled systems;
- stolen components;
- hacked access;
- faction/security state.

Raid resolution does not automatically restore a pristine target.

## 8. Procedural Determinism — Result: PASS

One Raid/Mission ID has stable:

- major target layout;
- loot;
- defenders;
- access routes;
- reinforcement seed;
- objective placement.

Reload does not reroll a weaker/easier target.

## 9. Intelligence — Result: PASS

Raid Intelligence uses:

- Unknown;
- Suspected;
- Confirmed;
- Stale.

Information comes from valid sensors/recon/hacking/contacts/history.

No omniscient target map or exact hidden loot count is provided by default.

## 10. Intelligence vs Exploration — Result: PASS

GDS-11 Intelligence specializes GDS-8 discovery for strategic raid planning.

GDS-8 remains authoritative for local exploration states.

No contradictory discovery model is introduced.

## 11. Preparation — Result: PASS

Preparation validates actual:

- ship;
- Reach;
- loadout;
- robots/TCC;
- rack/bay space;
- tools;
- survival;
- cargo;
- extraction.

It does not create supplies.

## 12. Hard Requirements — Result: PASS

Deployment blocks only confirmed physical impossibility.

Incomplete/unknown target Intel produces warnings/Unknown Risk rather than revealing secret counters.

This preserves non-omniscient design.

## 13. Player Loadout — Result: PASS

GDS-5 remains authoritative for player inventory, equipment, suit energy, life support, and interaction.

GDS-11 references those states without redefining them.

## 14. Spacecraft Integration — Result: PASS

GDS-6 remains authoritative for:

- local flight;
- docking;
- power;
- thermal;
- fuel/propellant;
- cargo;
- module state;
- strategic travel;
- player ship recovery.

GDS-11 defines how those capabilities are used during a raid.

## 15. External Space Attack — Result: PASS

External raid combat reuses GDS-2 defense architecture and GDS-9 ship combat.

Attackers can target actual:

- shield emitters;
- turrets;
- sensors;
- communications;
- power dependencies;
- docking control;
- defender craft.

No base-wide HP abstraction is added.

## 16. Shield Zones — Result: PASS

GDS-11 preserves GDS-2 local station shield zones.

Destroying/depleting one emitter affects only its actual coverage/dependencies.

No single station-wide shield value is introduced.

## 17. Station Defense Ammunition — Result: PASS

Defenses consume finite ammunition/power/thermal capacity.

Isolation of supply affects sustained fire while already loaded magazines remain until consumed/damaged.

This preserves GDS-2/GDS-4/GDS-9.

## 18. Boarding — Result: PASS

Boarding requires a valid physical route:

- dock;
- hacked/spoofed port;
- maintenance route;
- existing opening;
- authored breach.

No automatic boarding occurs because shields reach zero.

## 19. Breaching — Result: PASS

Breaching applies only to explicitly breachable/destructible targets.

It respects:

- tools;
- explosives;
- geometry;
- pressure;
- alarms;
- robot size.

No unrestricted destructibility is introduced.

## 20. Atmosphere / Pressure — Result: PASS

A hull breach into a pressurized compartment can cause actual decompression/hazards.

The raid does not suspend GDS-2/GDS-5 environmental rules.

## 21. Heavy Robot Geometry — Result: PASS

GDS-10 Heavy Mechs cannot pass through narrow airlocks/service corridors merely because they are selected for the squad.

Raid access/pathing respects actual clearance.

## 22. Interior Combat — Result: PASS

Interior raid play uses:

- real corridors;
- doors;
- lifts;
- security;
- local control;
- atmosphere;
- defender entry paths.

Cleared rooms are not permanently safe by hidden checkpoint rule.

## 23. Enemy AI — Result: PASS

Defenders reuse GDS-9 non-omniscient perception.

Security/sensor infrastructure can provide legitimate tracking.

Destroying sensors reduces future information but does not erase already observed Last Known Position immediately.

## 24. Robot Commands — Result: PASS

GDS-10 tactical commands remain valid during raids.

Hacking, breaching, repair, shield, demolition, and attack orders require actual:

- capability;
- target;
- path;
- communication;
- energy/ammo.

No raid-specific command magic is introduced.

## 25. Tactical Control Capacity — Result: PASS

Raids do not override GDS-10 TCC/Command Load.

Reserve robots still require actual transport and free command capacity before activation.

## 26. Sabotage Causality — Result: PASS

Sabotage affects only connected systems.

Examples:

- cut local power → connected loads;
- disable comms → dependent calls/data;
- remove component → target loses that component;
- disable cooling → heat accumulates through normal system behavior.

No whole-base switch exists by default.

## 27. Reactor Sabotage — Result: PASS

GDS-11 explicitly rejects "shoot reactor = instant station explosion."

Reactor catastrophe requires authored physical/system conditions and respects safeguards/cooling/containment.

## 28. Hacking Boundary — Result: PASS

Hacking grants only the capability exposed by the compromised interface/network segment.

One terminal does not provide universal station control.

This preserves GDS-2 Security and GDS-10 Hacker boundaries.

## 29. Resource Ownership — Result: PASS

Raid loot uses the GDS-4 single-owner rule.

Physical transfer is always:

Source  
→ Carrier/Player/Robot  
→ Ship/Extraction Owner  
→ later station storage.

No resource duplication occurs through objective/UI state.

## 30. Looting — Result: PASS

Player raids do not grant an abstract percentage of target resources.

The player must physically recover and extract loot.

Target inventories remain depleted after transfer.

## 31. Defensive Theft — Result: PASS

Enemy raids on Horizon Station must physically:

- reach storage;
- load goods;
- carry them out;
- secure them in attacker transport;
- successfully extract.

Resources are not permanently removed before hostile extraction commit.

## 32. Knowledge Assets — Result: PASS

Already committed Research, Blueprints, Research Evidence, and route knowledge cannot be stolen from player memory.

Only physical unanalyzed data/hardware can be stolen/destroyed.

## 33. Reward Boundary — Result: PASS

Field raid loot remains separate from GDS-8 mission Resolution Rewards.

No duplicate reward copy is created because a raid succeeded.

## 34. Reinforcement Source — Result: PASS

Reinforcements require:

- valid source;
- valid communication/call;
- route;
- actual finite force.

No arbitrary infinite spawning exists.

## 35. Reinforcement Call Transaction — Result: PASS

Call lifecycle is explicit:

Not Requested  
→ Calling  
→ Committed  
→ Responding  
→ Arrived

with Disrupted/Cancelled branches.

Destroying communications after call commit does not automatically erase the dispatched force.

## 36. Active-Time ETA — Result: PASS

Reinforcement ETA progresses only during active simulation and persists through save/load.

No reload timer reset exists.

## 37. Finite Escalation — Result: PASS

Raid-wide escalation has a ceiling based on target/world resources.

The system does not spawn progressively stronger infinite waves as a hidden punishment for looting time.

## 38. Defender Repair — Result: PASS

NPC station repair requires:

- worker/robot/automation;
- access;
- parts;
- power;
- time.

Destroyed/disabled systems do not simply respawn when off camera.

## 39. Evacuation — Result: PASS

Defenders can evacuate people/loot/objectives only through real transport/logistics.

Nothing teleports because alert state changed.

## 40. Extraction — Result: PASS

Raid extraction uses GDS-8 and requires physical withdrawal.

The player ship is not a safe zone until actual extraction commit.

Extraction under fire is possible where valid.

## 41. Robot Extraction — Result: PASS

Owned robots are safe only when actually secured through GDS-10/GDS-8.

No magical squad recall occurs.

## 42. Retreat — Result: PASS

Retreat is a physical behavior and can occur before/after success/failure/abandonment.

It does not refund consumed resources or undo committed target damage.

## 43. Victory — Result: PASS

Raid success is objective-based.

Kill count does not replace:

- theft;
- sabotage;
- rescue;
- capture;
- hack;
- extraction.

## 44. Target Capture — Result: PASS

Capture of a command node/person/asset is allowed where objective says so.

Baseline raids do not automatically turn enemy stations into additional fully managed player Home Stations.

This prevents an undeclared multi-base simulation dependency.

## 45. Horizon Station Attack — Result: PASS

Created hostile station attacks use actual:

- attacker force;
- target objective;
- station defenses;
- security;
- robots;
- crew;
- power;
- ammo;
- geometry;
- cargo.

No opaque global Defense Score determines the result.

## 46. Player Away From Horizon — Result: PASS

During active gameplay elsewhere, lower-detail station combat can run deterministically.

Outcomes must map back to actual state:

- ammo spent;
- shields changed;
- modules damaged;
- robots lost;
- cargo moved/stolen;
- breaches created.

## 47. Player Notification — Result: PASS

Remote attack warning requires a valid communications/information path.

No in-world omniscient alert is assumed.

## 48. Player Return — Result: PASS

Returning from another mission does not restart the station attack.

The Defense Event continues in active time and the player joins its current state.

GDS-8 resolves abandonment/extraction from the external mission.

## 49. Crew Permanence — Result: PASS

Routine raids can wound/incapacitate human crew but cannot permanently kill them.

This preserves DD-013/GDS-3.

## 50. Robot Attrition — Result: PASS

Robots can be permanently Destroyed during offensive or defensive raids.

No post-raid restoration is granted.

This preserves GDS-10.

## 51. Horizon Core Protection — Result: PASS

Routine procedural raids cannot permanently delete Horizon Station or campaign-critical Command Core identity.

Attackers can still cause serious physical/system setbacks.

This prevents ordinary dynamic events from hard-bricking campaign progression.

## 52. Station Recovery — Result: PASS

After a raid, damage/hazards/security compromise persist.

Recovery requires real:

- repair;
- parts;
- power;
- crew/robots;
- time.

There is no free one-click reset.

## 53. Station Defenses — Result: PASS

GDS-11 completes the pending raid interface for GDS-2 Defenses:

- attacker target selection;
- suppression;
- saturation;
- boarding transition;
- off-screen use;
- physical defensive failure.

GDS-2 remains owner of the defense hardware itself.

## 54. Station Security — Result: PASS

GDS-11 completes the pending boarding/raid interface for GDS-2 Security:

- hostile access;
- lockdown;
- local hacking;
- boarder tracking;
- internal response;
- recovery.

Security remains bounded by sensors/access infrastructure.

## 55. Story Integration — Result: PASS

GDS-11 now explicitly binds full raid mechanics to:

- MS-A301 hostile Rook's Wake branch;
- MS-A302 only when a forced fortified assault occurs;
- MS-F02 Crownless access/assault phases.

GDS-7/GDS-8 story objectives/outcomes remain unchanged.

## 56. Rook's Wake — Result: PASS

Rook's Wake remains a mobile/fortified Blackwake base in Kestrel.

The hostile story path requires archive recovery and extraction, not destruction of the base.

Negotiation remains a valid non-raid branch.

## 57. Sealed Authority — Result: PASS

Authorized/service access remains ordinary mission play.

Only an escalated forced fortified assault uses GDS-11.

The required archive cannot be substituted by destroying its storage.

## 58. Second Cascade — Result: PASS

GDS-11 owns fortified access/control-path fighting during Crownless.

It cannot choose or redefine the final Stabilize/Sever/Contain decision.

## 59. Faction Reputation Boundary — Result: PASS

GDS-11 records facts such as:

- identification;
- casualties;
- sabotage;
- theft;
- destruction;
- negotiated betrayal.

GDS-7/GDS-12 remain authoritative for reputation/economic consequences.

## 60. World Geography — Result: PASS

Raid targets remain attached to GDS-7 Strategic Locations and routes.

GDS-11 does not move canonical story geography.

## 61. No Automatic Player Scaling — Result: PASS

Raid targets/defenders retain world threat capability.

GDS-11 does not reduce/raise them to match current player equipment.

This preserves DD-015.

## 62. No Full Destruction Requirement — Result: PASS

GDS-11 supports strategic subsystem attacks while preserving the project rule against unrestricted world destruction.

Only authored breach/destruction targets can be destroyed.

## 63. Persistence — Result: PASS WITH DOWNSTREAM DEPENDENCY

GDS-11 defines persistable state for:

- Raid/Defense Event ID;
- target damage;
- loot ownership;
- escalation/calls;
- breaches;
- robot losses;
- security compromise;
- station aftermath.

GDS-12 still owns final save-slot/autosave transaction policy and long-term target recovery timing.

## 64. Economy / Dynamic Events — Result: PASS WITH DOWNSTREAM DEPENDENCY

GDS-12 must still define:

- target stock/economic values;
- raid profitability balance;
- attack generation frequency;
- recovery/replenishment pace;
- service/repair costs;
- anti-frustration event pacing.

No GDS-11 rule requires a currently undefined currency.

## 65. Difficulty — Result: PASS WITH DOWNSTREAM DEPENDENCY

GDS-12 may tune:

- AI reaction;
- damage modifiers if formally adopted;
- reinforcement timing;
- event pressure.

It may not create:

- omniscience;
- infinite waves;
- percentage theft;
- physical rule violations.

## 66. Presentation — Result: PASS WITH DOWNSTREAM DEPENDENCY

GDS-13 must define:

- raid briefing/intel UI;
- approach warnings;
- alarm/escalation feedback;
- squad/Bridgehead controls;
- objective/system overlays;
- loot/extraction warnings;
- Horizon remote attack alerts;
- after-action report.

Underlying rules are already fixed.

## 67. Internal GDS-11 Consistency — Result: PASS

### Intelligence vs Preparation

Intel describes known target state; Preparation validates only legitimate known/intrinsic constraints.

### External Attack vs Boarding

External attack creates/opens physical access; Boarding traverses it.

### Boarding vs Interior

Boarding owns entry/Bridgehead; Interior owns movement/control after entry.

### Sabotage vs Station Systems

Sabotage chooses/changes target state; GDS-2/target systems own resulting physical network behavior.

### Looting vs Extraction

Looting moves ownership; Extraction determines secure departure.

### Escalation vs Spawning

Escalation activates/requests real finite forces; it does not create arbitrary enemies.

### Offensive vs Defensive Raids

Both use the same physical principles for access, sabotage, looting, withdrawal, and extraction.

## 68. Dependencies Preventing Design Complete

GDS-11 remains first-pass pending:

### GDS-12 Economy, Progression, Difficulty, Dynamic Events, Persistence

Required for:

- raid availability/generation pacing;
- profitability/reward economics;
- target recovery/replenishment;
- Horizon attack frequency;
- final save/retry transaction policy;
- coalition/support economics;
- difficulty tuning.

### GDS-13 Presentation

Required for:

- final raid UX/HUD;
- intelligence visualization;
- tactical command presentation;
- remote station-attack feedback;
- alarms/audio/VFX;
- onboarding/accessibility.

## 69. First-Pass Conclusion

No blocking contradiction was found.

GDS-11 can be marked:

**First-Pass Complete — Cross-Validation Pending**

The next dependency-driven phase is GDS-12 Economy, Progression, Difficulty, and Cross-Cutting Systems.
