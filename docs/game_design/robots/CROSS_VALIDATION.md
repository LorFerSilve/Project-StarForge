# Robots Cross-Validation

> **Status:** Active Audit  
> **Authority:** GDS-10 consistency review only  
> **Purpose:** Validate robot identity, manufacturing, classes, components, AI, squads, commands, damage/loss, and station automation against established station, crew, resource, spacecraft, mission, and combat rules.

## 1. Scope

This audit checks GDS-10 against:

- Global Game Rules;
- GDS-2 Home Station;
- GDS-3 Survivors and Crew;
- GDS-4 Resources/Crafting/Research;
- GDS-5 Player Character;
- GDS-6 Spacecraft;
- GDS-7 World/Factions;
- GDS-8 Missions/Exploration;
- GDS-9 Combat.

## 2. Manufactured Physical Units — Result: PASS

Player robots are persistent physical units with unique Robot IDs.

They are not:

- abstract troop cards;
- passive percentage bonuses;
- menu-only deployments;
- instant summons.

This matches the physical-world and resource-ownership philosophy.

## 3. Human Crew Boundary — Result: PASS

Robots do not replace human crew as universal specialists.

Crew remains authoritative for:

- professional supervision;
- research;
- full medical roles;
- strategic judgment;
- narrative identity.

Robots provide physical labor, repeatable tactical force, and bounded automation.

This preserves Global Rule 14.

## 4. Robot Permanent Loss — Result: PASS

Unlike recruited human crew, robots can be permanently Destroyed through routine damage.

Destroyed Robot IDs are not restored automatically.

This does not contradict crew permanence because robots and crew intentionally use different persistence philosophies.

## 5. Disabled vs Destroyed — Result: PASS

Robot whole-unit states are:

- Operational;
- Degraded;
- Disabled;
- Destroyed.

Chassis Integrity zero commits Destroyed.

Disabled robots remain recoverable if chassis survives and repair requirements can be met.

This maps cleanly to GDS-9 Disable/Destroy objective semantics.

## 6. Resource Ownership — Result: PASS

Robot manufacturing, component installation, repair, ammunition, salvage, and logistics use actual GDS-4 physical ownership.

No robot action creates materials/components from nothing.

## 7. Manufacturing — Result: PASS

Robot production uses:

Research/Blueprint  
→ resource/component reservation  
→ fabrication/assembly  
→ component installation  
→ software initialization  
→ diagnostics  
→ commissioning.

Robot ID is created only at Commissioning commit.

This prevents unfinished Work Orders from becoming duplicate persistent units.

## 8. Station Manufacturing Interface — Result: PASS

GDS-2 Station Manufacturing now explicitly recognizes the Robot Assembly Bay.

GDS-2 owns:

- power;
- thermal;
- logistics;
- queue;
- WIP;
- facility damage.

GDS-10 owns:

- robot recipe/configuration;
- commissioning;
- Robot ID;
- robot-specific validation.

Authority is not duplicated.

## 9. Blueprint / Research — Result: PASS

Advanced robot chassis/components can require:

- Technology Research;
- Blueprint;
- both.

GDS-10 does not collapse the GDS-4 Research/Blueprint distinction.

## 10. No Robot Currency — Result: PASS

No generic troop/robot training currency or Robot Point exists.

Robot creation consumes actual materials, power, work, infrastructure, and active game time.

## 11. No Loot Rarity — Result: PASS

Robots/components do not use Common/Rare/Epic/Legendary random stat tiers.

Progression comes from:

- chassis;
- components;
- software;
- technology;
- configuration.

This matches GDS-4/GDS-5/GDS-9 progression philosophy.

## 12. Canonical Field Classes — Result: PASS

GDS-10 defines eight baseline field classes:

- Recon Drone;
- Repair Drone;
- Hacker Unit;
- Assault Unit;
- Shield Unit;
- Breaching Unit;
- Demolition Unit;
- Heavy Mech.

Each has a distinct mandatory capability and role.

## 13. Tactical Control Capacity — Result: PASS

Active robot squads use finite Tactical Control Capacity.

Each class has Command Load.

TCC is a command capability, not a physical Resource/currency.

This avoids one arbitrary universal robot-count cap while preserving understandable deployment limits.

## 14. Transport Capacity — Result: PASS

TCC and transport are separate.

GDS-6 Cargo/Utility now explicitly defines:

- Robot Rack Units;
- Robot Rack;
- Heavy Robot Bay;
- robot charging/service interface.

Light/Medium/Heavy physical footprint is therefore connected to actual spacecraft capability.

## 15. No Robot Teleport Deployment — Result: PASS

Field robots deploy from:

- player ship rack/bay;
- valid mission transport;
- persistent location;
- station defense storage.

They cannot be summoned from Horizon Station into a mission.

This matches GDS-8 physical mission-zone rules.

## 16. Squad Deployment — Result: PASS

One normal external mission uses one primary player Tactical Squad.

Deployment validates:

- TCC;
- transport footprint;
- bay/rack compatibility;
- charging/support;
- operational state;
- environmental compatibility.

Reserve units physically aboard ship can activate only when capacity permits.

## 17. Robot Extraction — Result: PASS

Robots are recovered only when physically secured in a valid extraction owner/method.

Proximity to the ship is insufficient.

The player can knowingly leave units behind, with actual Lost/recovery consequences.

This matches GDS-8 extraction philosophy.

## 18. Mission Failure — Result: PASS

Robot outcome follows actual mission state:

- secured robot preserved;
- field Operational/Disabled robot follows recovery fiction;
- Destroyed robot remains Destroyed;
- wreck recovered only when physically secured/covered by valid recovery.

No automatic squad refund exists.

## 19. Internal Energy — Result: PASS

Robots use finite Internal Energy Reserve.

It is a device charge state, not a new physical resource ID.

Recharge consumes actual electrical power from compatible station/ship/field interfaces.

No free energy loop exists.

## 20. Ammunition — Result: PASS

Armed robots use finite GDS-9 ammunition/energy weapon behavior.

Manufacturing/repair does not create free ammunition.

Station logistics and ship loadout physically resupply robot ammunition.

## 21. Components — Result: PASS

Robot components define actual:

- chassis;
- mobility;
- core;
- power;
- sensors;
- armor;
- weapons;
- utility;
- communications;
- software.

Software cannot grant absent physical capability.

## 22. Power / Mass / Thermal Validation — Result: PASS

Robot configurations validate:

- component compatibility;
- mass envelope;
- power;
- energy;
- thermal requirements;
- ammunition/support;
- command interface.

Invalid configurations are explicitly nondeployable.

## 23. Combat Model — Result: PASS

Robots reuse GDS-9:

- hit detection;
- damage channels;
- shield/armor;
- penetration;
- status effects;
- finite ammunition;
- friendly fire;
- machine disable/destroy semantics.

GDS-10 adds robot-specific hit zones/subsystem consequences without redefining combat formulas.

## 24. Robot Hit Zones — Result: PASS

Baseline zones:

- Control Core;
- Power;
- Mobility;
- Sensors/Communication;
- Weapon/Tool;
- Chassis/Armor.

Subsystem loss can degrade/disable capability before Chassis destruction.

## 25. AI Knowledge — Result: PASS

Player robot AI uses:

- own sensors;
- squad-shared information;
- player designation;
- mission intel.

No omniscience is introduced.

Communication loss triggers configured fallback rather than magical command reception.

## 26. Communications — Result: PASS

Canonical communication states:

- Connected;
- Degraded;
- Disconnected.

Disconnected robots follow one configured Fallback Order.

This creates deterministic behavior under jamming/range/control damage.

## 27. Tactical Commands — Result: PASS

Commands use a clear state machine:

Issued  
→ Accepted  
→ Executing  
→ Completed / Blocked / Failed / Cancelled.

Physical capability/path/access is always required.

No tactical command teleports, hacks impossible systems, or creates missing repair parts.

## 28. Rules of Engagement — Result: PASS

Canonical ROE:

- Hold Fire;
- Defensive;
- Engage Hostiles;
- Engage Designated Target Only.

Direct player target commands provide explicit authorization only for the selected target.

## 29. Strategic Autonomy Boundary — Result: PASS

Robots do not independently:

- select missions;
- initiate raids;
- change diplomacy;
- choose research;
- make irreversible story choices;
- spend protected strategic resources outside policy.

This preserves Global Automation Ceiling.

## 30. Station Automation — Result: PASS

Station robots become physical eligible actors for GDS-2 Automation tasks.

They still require:

- capability;
- access;
- energy;
- resources;
- authorization;
- physical path.

GDS-2 owns task lifecycle/scheduling.

GDS-10 owns robot capability/execution.

## 31. Station Logistics — Result: PASS

Logistics robots transfer ownership:

Source  
→ Robot Cargo  
→ Destination.

No resource teleportation occurs.

Blocked destination preserves physical cargo.

## 32. Station Security — Result: PASS

Security Sentinels reuse:

- GDS-2 access/alert/lockdown;
- GDS-9 combat;
- GDS-10 robot body/AI.

They have no omniscient intruder information.

Security Officer remains useful for response policy/supervision.

## 33. Robotics Specialist — Result: PASS

Robotics Specialist supports:

- diagnostics;
- maintenance;
- production supervision;
- advanced robot operation.

The specialist does not create resources or bypass physical capability.

This resolves the primary GDS-3 Robotics Specialist dependency.

## 34. Off-Screen Station Simulation — Result: PASS

Station robots can be lower-detail simulated while player is away during active gameplay.

Outcomes still respect:

- time;
- energy;
- access;
- damage;
- resource consumption;
- task capacity.

No offline real-world progression is introduced.

## 35. Hacking — Result: PASS

Hacker Unit can execute only valid authorized hacking paths.

It does not provide universal system takeover.

This matches GDS-2 Security hacking boundary.

## 36. Breaching / Demolition — Result: PASS

Breaching applies only to authored breachable targets.

Demolition applies only to explicit destructible targets.

No unrestricted geometry destruction is introduced.

This preserves Global Rule 27.

## 37. Shield Unit — Result: PASS

Shield Unit uses physical projection geometry and actual energy.

It does not provide global squad immunity.

GDS-9 shield rules remain authoritative.

## 38. Repair Drone — Result: PASS

Repair requires:

- correct tool;
- parts/materials;
- physical access;
- time;
- compatible target.

Repair Drone does not replace Engineer/Mechanic/Robotics Specialist expertise in every context.

## 39. Main Story / Finale — Result: PASS

GDS-8 MS-F01 can now prepare actual robot squad/transport/TCC capacity.

GDS-8/MS-F02 can deploy robot roles using this fixed framework.

Exact finale coalition capability thresholds remain GDS-12.

## 40. Raids — Result: PASS WITH DOWNSTREAM DEPENDENCY

GDS-10 now provides:

- assault composition;
- hacking;
- breaching;
- demolition;
- shields;
- repair/support;
- tactical commands;
- robot loss.

GDS-11 must still define:

- raid preparation;
- phase orchestration;
- station defense;
- target intelligence;
- breach/escalation;
- raid extraction/loot outcome.

## 41. Economy / Progression — Result: PASS WITH DOWNSTREAM DEPENDENCY

GDS-12 still owns:

- exact TCC progression;
- component/chassis prices;
- production/replacement economic pacing;
- maintenance economics;
- exact technology unlock order.

The fixed robot rules do not depend on unknown currency.

## 42. Persistence — Result: PASS WITH DOWNSTREAM DEPENDENCY

Robot state is explicitly persistable:

- Robot ID;
- location;
- components;
- condition;
- energy;
- ammo;
- assignment;
- squad;
- AI command;
- Lost/Destroyed state.

GDS-12 remains owner of final save transaction mechanics.

## 43. Presentation — Result: PASS WITH DOWNSTREAM DEPENDENCY

GDS-13 must define:

- squad selection UX;
- command feedback;
- robot status HUD;
- TCC/transport presentation;
- damage/subsystem feedback;
- station robot management UX.

No presentation uncertainty changes underlying robot rules.

## 44. Internal GDS-10 Consistency — Result: PASS

### Manufacturing vs Components

Manufacturing creates the unit; Components define its physical configuration.

### Classes vs Components

Class defines mandatory role capability; Components provide it.

### AI vs Commands

Commands define player intent; AI executes within physical/safety limits.

### Squad vs Spacecraft

Squad owns TCC/composition; Spacecraft owns physical rack/bay capability.

### Damage vs Combat

GDS-9 owns damage math; GDS-10 owns robot subsystem consequences/recovery.

### Station Robots vs Automation

Automation owns task scheduling; Robot domain owns physical worker capability.

## 45. Dependencies Preventing Design Complete

GDS-10 remains first-pass pending:

### GDS-11 Raids and Station Defense
- assault-phase robot deployment;
- raid breach/hack/demolition orchestration;
- station defense force assignment;
- robot raid extraction/recovery.

### GDS-12 Economy, Progression, Difficulty, Persistence
- final TCC progression;
- recipes/cost balancing;
- maintenance/replacement economics;
- final save semantics;
- robot-related difficulty tuning.

### GDS-13 Presentation
- squad/command UX;
- robot indicators;
- station fleet management;
- feedback/audio/VFX.

## 46. First-Pass Conclusion

No blocking contradiction was found.

GDS-10 can be marked:

**First-Pass Complete — Cross-Validation Pending**

The next dependency-driven phase is GDS-11 Raids and Station Defense.
