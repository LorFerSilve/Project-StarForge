# Horizon Station Defense Resolution

> **Status:** Design Complete  
> **Authority:** Hostile raids against Horizon Station, attack creation, detection, defensive phases, on-screen/off-screen simulation, attacker objectives, theft, boarding, player return, station failure/recovery, and ordinary-loss protection

## 1. Purpose

Horizon Station defense is the defensive counterpart to offensive raids.

It must make station layout, defenses, crew, robots, power, logistics, security, and redundancy matter without collapsing into an opaque auto-resolve score.

## 2. Defensive Raid Identity

Every hostile attack on Horizon Station has a persistent **Defense Event ID**.

It records:

- attacker faction/force;
- attacker objective set;
- approach route;
- force composition;
- transport/cargo capacity;
- escalation state;
- current phase;
- casualties/damage;
- stolen cargo ownership;
- retreat/extraction state;
- resolution.

## 3. Creation Boundary

GDS-12 Dynamic Events later determines:

- when an attack can be generated;
- strategic frequency;
- economic/war-state weighting.

GDS-11 owns how a created Defense Event executes.

## 4. No Real-World Offline Creation/Progression

A Defense Event cannot begin or advance solely because real-world time passes while the game is closed.

It can occur/advance only during active game simulation.

## 5. Attacker Objectives

A hostile raid has one or more explicit objectives such as:

- steal resources;
- steal a physical strategic object;
- sabotage power/communications/research/defense;
- destroy a designated module;
- disable station operation;
- rescue/recover an NPC/faction asset;
- capture a temporary control node;
- punitive damage/withdrawal.

Attackers do not simply "damage until hidden score reaches zero."

## 6. Attacker Objective Knowledge

Raiders know only target information granted by their authored/generated intelligence.

They do not automatically know:

- hidden storage contents;
- all station layout;
- every power dependency;
- every robot position.

Target selection uses their actual intelligence and live observation.

## 7. Canonical Defense Phases

Baseline:

1. Approach / Detection;
2. External Defense;
3. Boarding Attempt;
4. Interior Defense;
5. Attacker Objective / Theft / Sabotage;
6. Attacker Withdrawal;
7. Recovery.

A raid can end during any phase.

## 8. Approach / Detection

Horizon Station sensors attempt to detect/classify incoming forces according to actual:

- sensor coverage;
- range;
- target signature;
- jamming;
- system condition;
- faction/security policy.

## 9. Early Warning

If detected early enough, station systems can:

- raise alarm;
- activate shields;
- ready turrets;
- launch defensive robots/drones;
- close sensitive access;
- recall/position crew;
- reserve power/ammo.

Response delay depends on actual automation/crew/control capability.

## 10. Surprise Attack

If attackers enter close range before detection, the station begins later in the response chain.

Surprise does not grant attackers bonus damage; it changes readiness/time/position.

## 11. Player Present

If player is at Horizon Station when attack begins:

- player can directly operate defenses;
- pilot ship if available;
- command robots;
- fight boarders;
- repair/sabotage response;
- manage station systems.

All normal GDS-2/GDS-5/GDS-6/GDS-9/GDS-10 rules apply.

## 12. Player Away

If player is elsewhere during active gameplay:

- attack continues;
- station automation, crew, defenses, robots, and resources act according to configured policies;
- simulation can use lower detail but must preserve equivalent system causality.

## 13. Off-Screen Simulation Inputs

Off-screen resolution must consume actual current state:

- station geometry/defense arcs;
- shield zones/capacity;
- sensor coverage;
- power availability/priorities;
- thermal limits;
- loaded ammunition and logistics;
- station robot locations/condition/ammo/energy;
- crew assignments/health;
- security doors/zones;
- atmosphere/pressure;
- damage/faults already present;
- attacker ships/robots/personnel/cargo.

## 14. No Defense Score

The system may calculate derived values internally for optimization, but no single Defense Score is authoritative for outcome.

A station with a high aggregate score but one real uncovered approach/power single-point failure can still be breached.

## 15. Deterministic Off-Screen Resolution

Given the same:

- station state;
- Defense Event seed;
- player policies;
- elapsed active simulation time;

resolution must be reproducible.

Reload/frame rate cannot reroll a favorable battle outcome.

## 16. Simulation Granularity

When off-screen, combat may resolve at coarser simulation ticks/engagement nodes rather than rendering every projectile.

However each result must map back to valid GDS-9/GDS-2 state:

- ammo consumed;
- shield capacity changed;
- module damaged;
- robot damaged/destroyed;
- attacker disabled/destroyed;
- breach created;
- cargo moved.

## 17. External Defense

Station defenses engage according to:

- target information;
- engagement policy;
- firing arc;
- range;
- line of fire;
- power;
- thermal state;
- ammo;
- weapon condition.

## 18. Attacker External Tactics

Attackers can attempt to:

- destroy/suppress shield emitters;
- disable sensors;
- cut power-facing defenses;
- saturate point defense;
- destroy turrets;
- attack docking/hangar areas;
- launch boarding craft;
- force a breach.

Their actions are limited by their actual weapons/capability.

## 19. Shield Defense

Horizon uses GDS-2 local shield zones.

Attackers can exploit:

- uncovered areas;
- destroyed emitters;
- temporary depletion;
- power/cooling failure.

There is no whole-station shield HP.

## 20. Defensive Robots

Available Security Sentinels/Tactical robots can be assigned to:

- external defense if environment/mobility permits;
- hangar/dock defense;
- internal choke points;
- critical modules;
- mobile reserve.

GDS-10 owns their physical behavior.

## 21. Human Crew Defense

Security/Weapons-capable human crew can support defense according to assignments/equipment.

Routine attack cannot permanently kill recruited crew.

Crew can become:

- Wounded;
- Critical;
- Incapacitated;
- isolated.

## 22. Civilian / Noncombat Crew

Noncombat specialists prioritize:

- evacuation to safe compartment;
- emergency station tasks;
- medical/engineering/logistics duties

rather than automatically acting as soldiers.

## 23. Boarding Attempt

Attackers require a valid route:

- authorized/spoofed dock;
- forced dock;
- boarding craft;
- maintenance access;
- hull breach.

They cannot teleport inside after shields reach zero.

## 24. Boarding Craft

A hostile boarding craft is a physical transport with:

- capacity;
- approach path;
- vulnerability;
- docking/breach capability;
- carried attackers;
- potential theft cargo capacity.

Destroying it before deployment can eliminate those carried attackers/assets.

## 25. Boarding Detection

A physical breach/unauthorized access can trigger GDS-2 Security response if functioning sensors detect it.

## 26. Lockdown

Security Officer/automation/player can trigger actual:

- doors;
- lifts;
- sector isolation;
- access restrictions.

Lockdown cannot close destroyed/blocked barriers.

## 27. Interior Defense

Once boarders enter, internal defense uses:

- local security sensors;
- crew/robot defenders;
- internal weapons if installed;
- actual corridors/doors;
- pressure/environment;
- target priorities.

## 28. Defender Priorities

Default defensive priority hierarchy:

1. prevent catastrophic station loss/hazard;
2. protect Command Core and campaign-critical control;
3. protect life-support/crew survival;
4. prevent attacker objective completion;
5. protect critical power/thermal/security;
6. protect strategic storage/research hardware;
7. recover damaged systems/robots;
8. pursue fleeing attackers where safe.

Player policies/mission context can modify noncritical priorities.

## 29. Attackers Do Not Know Every Priority

Defender policy does not automatically reveal protected target locations to attackers.

## 30. Interior Attacker Navigation

Raiders navigate through actual known/discovered topology.

If their objective location is unknown, they must:

- search;
- hack;
- interrogate data/system;
- follow signs/logistics;
- use their intelligence.

## 31. Attacker Security Hacking

Hostile hackers obey the same bounded access principle as player hacking.

One compromised terminal does not automatically grant entire Horizon Station.

## 32. Attacker Sabotage

Enemy sabotage uses the same GDS-11 causal target rules.

They can only affect systems actually reached/compromised.

## 33. Attacker Looting

Enemy theft uses physical GDS-11 Looting:

Station Storage  
→ Attacker Carrier/Robot  
→ Boarding Craft/Ship  
→ successful hostile extraction.

No percentage is removed at phase start/end.

## 34. Attacker Cargo Limits

Hostile units have finite:

- carry mass/volume;
- transport cargo;
- loading throughput;
- available time.

They prioritize according to raid objective/intelligence.

## 35. Interrupted Theft

Stolen resources remain recoverable until attacker extraction commits.

If carrier is disabled/destroyed, cargo stays physically present in the station/ship/wreck.

## 36. Knowledge Assets

Attackers cannot delete already committed Research/Blueprint/Research Evidence from player knowledge.

They can steal/destroy physical unanalyzed data/hardware.

## 37. Command Core Protection

Routine raids can:

- disable Command Core support systems;
- damage interfaces/connected infrastructure;
- temporarily compromise local controls.

They cannot permanently remove Horizon Station ownership or delete campaign-critical Command Core identity.

## 38. Critical System Failure

If station power/life-support/thermal systems fail during attack, normal GDS-2 consequences occur.

The raid does not suspend engineering simulation.

## 39. Fires / Breaches

Combat damage can create persistent:

- hull breach;
- decompression;
- fire;
- coolant loss;
- power failure;
- blocked route.

Automation/crew can respond if capability exists.

## 40. Player Remote Information

If communication path exists, player away from station can receive:

- attack warning;
- current known phase;
- major system damage;
- request/decision alerts.

Exact remote-control abilities require installed communication/control capability and are not assumed universal.

## 41. Player Remote Orders

Baseline player can issue high-level station policies/orders remotely only if:

- station and player ship communication exists;
- corresponding automation/control system supports it.

The player cannot first-person-control a turret across interstellar distance without an explicit remote-control system.

## 42. Player Return

If player travels back while raid continues:

- elapsed attack state persists;
- already destroyed/looted assets remain so;
- the player enters the current phase rather than restarting attack.

## 43. Current External Mission

Returning can require abandoning/failing the current deployed mission.

GDS-8 resolves its loot/robot/extraction consequences.

The station attack does not freeze while the decision is made outside true Pause.

## 44. Attacker Withdrawal Conditions

Raiders may withdraw when:

- objective completed;
- cargo capacity sufficiently filled;
- casualty/damage threshold reached;
- reinforcement/escape doctrine triggers;
- target becomes too dangerous;
- transport endangered;
- mission-specific timer/command orders it.

## 45. Attacker Retreat Is Physical

Raiders must return to:

- boarding craft;
- docked ship;
- EVA extraction;
- external fleet route.

They can be intercepted during withdrawal.

## 46. Hostile Extraction Commit

Stolen physical assets become permanently lost to the player only when the hostile extraction owner successfully leaves under its valid extraction rules.

## 47. Defense Success

Defense succeeds when hostile mandatory objectives fail/are abandoned and attackers no longer maintain an active successful raid state.

It does not require destroying every fleeing enemy.

## 48. Defense Failure

Defense can fail when attackers achieve and extract/commit their required objective.

Examples:

- steal resources successfully;
- destroy target module;
- complete persistent sabotage;
- remove strategic object.

Horizon Station remains player-owned under routine events.

## 49. No Routine Game Over

A normal defense failure is a setback, not campaign deletion.

It can leave:

- severe station damage;
- resource shortages;
- destroyed robots;
- crew injuries;
- failed defenses;
- security compromise;
- recovery workload.

## 50. Recovery Phase

After attackers are gone/neutralized:

1. confirm hostile clearance;
2. contain fire/breach/environment hazards;
3. stabilize critical power/life support;
4. recover injured crew;
5. recover disabled robots/cargo;
6. reset compromised security;
7. repair/rebuild damaged systems;
8. restore normal operating posture.

## 51. No One-Click Recovery

Station recovery uses actual repair/resources/time/automation.

The game can provide a repair-task summary/queue but cannot restore everything for free.

## 52. Attacker Wrecks / Salvage

Defeated attacker ships/robots/equipment can produce physical salvage where the state permits.

GDS-4 owns yield.

Defense therefore can sometimes recover value, but not guaranteed profit.

## 53. Post-Attack Security

Compromised credentials/controllers remain compromised until:

- revoked;
- reset;
- repaired;
- replaced

according to security rules.

## 54. Repeat Attack Boundary

A new attack cannot assume the station is repaired/replenished if it is not.

GDS-12 event generation must consider current persistent station state and anti-frustration pacing.

## 55. Persistence

Defense Event phase, attacker state, station damage, ammo, shields, crew/robot health, stolen cargo, security compromise, and recovery tasks persist.

## 56. Save/Load

Reload cannot:

- reroll attacker composition for same Defense Event ID;
- restore spent station ammo;
- resurrect destroyed robots;
- return extracted stolen cargo;
- undo committed breaches/damage.

## 57. Edge Cases

If the player is away and communications fail before the attack is detected remotely, the station continues defending without player notification until another valid information path exists.

If attackers seize cargo but their extraction ship is Disabled at the station, the cargo remains physically in that hostile owner and can be recovered by defeating/reclaiming it.

If all crew are Incapacitated but automated security/robots remain operational, defense can continue.

If all station external weapons are destroyed but internal security prevents objective completion, defense can still succeed.

If the station repels the raid but life support later fails from raid damage, the raid can be technically won while the station remains in a critical Recovery state.

## 58. Explicit Non-Goals

Station defense does not provide:

- one opaque defense score;
- attacks while game is closed;
- automatic player omniscience while away;
- percentage resource theft;
- infinite attacker waves;
- free post-raid repair;
- routine permanent crew death;
- routine Horizon Station deletion.

## 59. Tuneable Parameters

Attack force sizes, arrival timing, objective priorities, withdrawal thresholds, remote-warning delay, coarse simulation tick, attacker cargo, repair urgency, and event pacing are tuneable/later-domain values.

## 60. Dependencies

This specification depends on GDS-2 Defenses/Security/Automation/Damage, GDS-3 Crew, GDS-4 Ownership/Salvage, GDS-6 Spacecraft, GDS-7 Factions, GDS-9 Combat, GDS-10 Robots, GDS-11 Looting/Sabotage/Escalation, and GDS-12 Dynamic Events/Persistence.

## 61. Open Questions

None in the Horizon Station defense-resolution baseline.
