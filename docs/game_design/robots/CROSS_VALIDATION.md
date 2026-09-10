# Robots Cross-Validation

> **Status:** Active Audit  
> **Authority:** GDS-10 consistency review  
> **Audit Revision:** Refreshed after GDS-12

## 1. Scope

This audit validates robot identity, manufacturing, classes, components, AI, squads, commands, damage/loss, station automation, raid use, economy, progression, Difficulty, and persistence against GDS-1 through GDS-12.

## 2. Persistent Manufactured Units — Result: PASS

Player robots remain physical persistent Robot IDs with actual configuration, location, condition, energy, ammunition, assignment, squad state, and loss/destruction state.

They are not abstract troop cards or instant summons.

## 3. Human Crew Boundary — Result: PASS

Robots do not replace human crew as universal specialists. Human professional supervision, narrative identity, medical roles, Research, and strategic judgment remain distinct.

## 4. Permanent Robot Attrition — Result: PASS

Destroyed robots remain permanently Destroyed through ordinary gameplay and require a newly manufactured/acquired replacement Robot ID. This intentionally differs from recruited human crew permanence.

## 5. Resource/Manufacturing — Result: PASS

Robot construction, components, repair, ammunition, charging, dismantling, and salvage consume or transfer real GDS-4/GDS-2 resources and work.

No Robot Point/training currency is introduced.

## 6. No Robot Rarity/Level — Result: PASS

Progression remains chassis/component/software/technology/configuration based. There are no generic Robot Levels or Common/Rare/Epic/Legendary stat tiers.

## 7. Classes — Result: PASS

The eight baseline field roles remain distinct:

- Recon Drone;
- Repair Drone;
- Hacker Unit;
- Assault Unit;
- Shield Unit;
- Breaching Unit;
- Demolition Unit;
- Heavy Mech.

Station robot roles remain physical automation actors rather than passive bonuses.

## 8. TCC vs Transport — Result: PASS

Tactical Control Capacity and physical transport remain separate constraints.

A squad must satisfy both GDS-10 Command Load/TCC and GDS-6 rack/bay/service capacity.

## 9. GDS-12 TCC Progression — Result: PASS

The previously unresolved TCC progression is now explicit:

- Command Link I = 2 TCC;
- Command Link II = 4 TCC;
- Command Link III = 6 TCC;
- Command Link IV = 8 TCC;
- strongest valid ship Command Support contribution = +2;
- normal external cap = 10;
- Horizon Defense support = up to +4 with cap 12;
- explicit finale coalition support = +2 temporary TCC with finale external cap 12.

TCC is still capability, not currency.

## 10. AI and Communication — Result: PASS

Robot AI uses own sensors, squad information, player designation, and valid mission intel. Connected/Degraded/Disconnected communication states and configured fallback behavior prevent omniscient remote control.

## 11. Commands/ROE — Result: PASS

Commands remain physical capability requests and cannot teleport, fabricate resources, bypass access, or hack impossible systems. ROE remains explicit and deterministic.

## 12. Combat — Result: PASS

GDS-9 owns hit/damage/shield/armor/status formulas. GDS-10 owns robot subsystem consequences, whole-unit Disabled/Destroyed state, and recovery.

## 13. Internal Energy/Ammunition — Result: PASS

Robots consume finite energy and weapon ammunition. Recharge/resupply transfers actual station/ship/field power/resources; GDS-12 Difficulty/Progression does not grant hidden free capacity.

## 14. Station Automation — Result: PASS

Station robots remain eligible physical workers for GDS-2 task scheduling only when they possess actual capability, access, resources, energy, authorization, and a path.

## 15. GDS-11 Raid Integration — Result: PASS

The previously pending raid dependency is now resolved.

GDS-11 uses actual robot:

- squad preparation;
- hacking;
- breaching;
- demolition;
- repair;
- shield support;
- combat;
- extraction/recovery;
- station defense assignment.

No raid-specific robot magic is added.

## 16. Raid Extraction/Loss — Result: PASS

Robots are safe only when physically secured under GDS-8/GDS-10 extraction rules. No automatic squad refund or recall exists.

## 17. Economy — Result: PASS

GDS-12 now supplies the economic context for components, robot purchase/service where offered, manufacturing inputs, maintenance, and replacements.

Credits cannot bypass Research, Blueprints, assembly/service capability, TCC, or physical transport.

## 18. Replacement Economics — Result: PASS

Permanent robot destruction creates genuine material/economic replacement burden. Late-game industrial maturity can reduce practical replacement friction but never resurrect the destroyed Robot ID for free.

## 19. Difficulty — Result: PASS

Difficulty does not alter:

- Command Load;
- TCC;
- transport footprint;
- permanent-destruction rule;
- robot unlock ownership;
- robot physical ownership.

Higher difficulty cannot make robot AI/enemy AI omniscient.

## 20. Persistence — Result: PASS

GDS-12 now resolves final save transaction semantics for:

- Robot ID;
- location;
- configuration;
- condition;
- energy;
- ammo;
- squad/assignment;
- command/fallback state;
- Disabled/Lost/Destroyed state.

Atomic persistence prevents commissioning/destruction/extraction duplication.

## 21. Progression — Result: PASS

GDS-12 defines intended bands:

- P1: Recon/Repair/basic Assault and Link I;
- P2: Assault/Hacker/Shield and Link II/III;
- P3: Breaching/Demolition/Heavy Mech, Advanced Robotics, Link III/IV and Heavy Robot Bay;
- P4: specialization/hardening for Verge operations.

Phases do not grant free robots or technologies.

## 22. Finale Minimum — Result: PASS

MS-F01 requires:

- at least 6 base Field TCC before temporary coalition support;
- a valid Tactical Squad with combined Command Load >= 4;
- combat/security capability;
- at least one relevant utility capability;
- normal transport/energy/ammo/environment validation.

Maximum TCC, Heavy Mech, or any one class is not mandatory.

## 23. Finale Coalition Support — Result: PASS

Temporary +2 TCC support affects command capacity only. It does not create robots, remove transport requirements, or refill energy/ammunition.

## 24. Postgame — Result: PASS

P5 preserves robot fleet state and supports specialization/redundancy without an endless robot-level ladder.

## 25. Remaining Downstream Dependency — GDS-13

GDS-13 remains responsible for squad-selection UX, commands/ROE feedback, TCC/transport presentation, robot status/subsystem feedback, station robot management, and accessibility.

## 26. Conclusion

The previously pending **GDS-11 raid** and **GDS-12 economy/progression/difficulty/persistence** dependencies are now first-pass resolved.

No blocking contradiction exists between GDS-10 and GDS-1 through GDS-12.

GDS-10 remains:

**First-Pass Complete — Cross-Validation Pending**

Its remaining scheduled downstream design dependency is GDS-13, followed by GDS-14 final audit.
