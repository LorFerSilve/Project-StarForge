# Interior Attack

> **Status:** Design Complete  
> **Authority:** Interior raid traversal, local control, defender behavior, bridgeheads, security interaction, robot use, local resupply, room/sector clearing, and objective movement

## 1. Purpose

The interior phase turns a raid into a physical operation through a defended station rather than a sequence of abstract menu outcomes.

## 2. Interior Raid Space

Interior combat occurs in bounded GDS-8 Interior/Mixed Mission Zones representing the raid-relevant sections of the target.

The playable layout includes only spaces relevant to:

- entry;
- objectives;
- security;
- power/control;
- loot;
- defender movement;
- extraction;
- systemic hazards.

## 3. Physical Traversal

The player and robots must move through actual:

- corridors;
- rooms;
- doors;
- lifts;
- maintenance routes;
- hangars;
- pressure boundaries;
- breaches.

No room transition teleports combatants across blocked topology.

## 4. Interior Knowledge

Known interior map state comes from GDS-11 Intelligence/GDS-8 Exploration.

The player may enter with:

- no map;
- partial map;
- full structural map;
- additional security/system overlays.

A structural map does not reveal live defenders unless they are actually tracked.

## 5. Bridgehead

A secured boarding area can function as the raid Bridgehead.

It can provide:

- rally point;
- access to player ship;
- robot reserve deployment;
- physical resupply;
- loot transfer route;
- disabled-robot recovery.

The Bridgehead remains vulnerable to counterattack.

## 6. Local Control

The player can gain temporary local control of specific systems/areas.

Examples:

- one door network;
- one security checkpoint;
- one power relay;
- one turret controller;
- one lift bank.

Local control does not imply whole-station ownership.

## 7. Security Zones

Target interiors can use GDS-2-compatible security classifications and access states.

Raid-relevant states include:

- open/normal access;
- restricted;
- locked;
- emergency sealed;
- jammed;
- destroyed.

## 8. Door Interaction

A blocked door can be handled through supported routes:

- valid credential;
- hacking;
- local power restoration;
- manual override;
- breach;
- alternate path.

The game does not assume every locked door is hackable or destructible.

## 9. Lockdown

A defender lockdown can:

- close doors;
- isolate sectors;
- restrict lifts;
- protect command/storage areas;
- reroute defenders.

It cannot close destroyed or physically obstructed doors.

## 10. Counter-Lockdown Play

The attacker can respond by:

- hacking a local controller;
- cutting power to the lock actuator where physically relevant;
- breaching the barrier;
- finding another route;
- restoring a compromised route.

## 11. Defender Awareness

Interior defenders use GDS-9 perception and shared information.

They do not know exact player position through all walls.

Security cameras/sensors/alarms can provide legitimate tracking.

## 12. Destroying Surveillance

Disabling cameras/sensors can reduce defender information.

Already observed Last Known Position remains until confidence decays or updates.

## 13. Defender Roles

Interior opposition can include:

- armed personnel;
- security robots;
- turrets;
- automated doors/lockdowns;
- specialist defenders;
- environmental/system hazards.

Exact faction composition remains content data.

## 14. Defender Spawn/Entry

Defenders/reinforcements enter through valid:

- occupied rooms;
- barracks/security posts;
- lifts;
- corridors;
- docking routes;
- external reinforcement insertion.

They do not appear in visibly empty sealed rooms without an authored insertion mechanism.

## 15. Room Clearing

A room is not globally marked "safe forever" merely because current hostiles are defeated.

It can remain:

- accessible;
- known;
- locally controlled;
- vulnerable to counterattack.

Objective Clear Area state follows GDS-9/GDS-8.

## 16. Local Control State

A raid can tag an area as **Attacker-Controlled** when:

- qualifying local defenders are no longer immediately contesting it;
- the player/squad has valid access;
- no target system currently denies occupation.

This is tactical state, not permanent ownership.

## 17. Control Loss

An Attacker-Controlled area can become Contested if defenders re-enter or local security/weapon systems reactivate.

## 18. Robot Squad Use

GDS-10 robots can:

- follow/hold;
- defend Bridgehead;
- scout;
- attack designated targets;
- repair;
- hack;
- shield;
- breach;
- demolish.

Their TCC, energy, ammo, pathing, communication, and damage remain fully active.

## 19. Heavy Mech Interiors

Heavy Mechs can only traverse compatible geometry.

Typical valid spaces:

- hangars;
- cargo halls;
- large industrial corridors;
- exterior platforms.

They can be tactically powerful but physically excluded from narrow crew/service routes.

## 20. Robot Command Communications

Interior structure can degrade command links.

Possible causes:

- distance;
- shielding;
- jamming;
- damaged relay/robot communication hardware.

Disconnected robots use GDS-10 Fallback Orders.

## 21. Squad Splitting

The player may divide commanded robots into command groups and position them separately while remaining within TCC.

The player still controls one active Tactical Squad identity.

## 22. Player Separation

Robots may hold a Bridgehead while the player explores another route.

They continue consuming:

- ammo;
- energy;
- damage state

if engaged.

## 23. Interior Resupply

Supplies can move from the player ship/loot/storage only through valid physical routes.

Examples:

- player carries ammo;
- Logistics-capable robot moves cargo;
- reserve robot brings supplies;
- target ammo storage is captured and compatible.

## 24. Enemy Supply

Defenders can draw from raid-relevant local magazines/armories where valid.

Destroying/isolation of supply can reduce sustained defense.

The target is not granted infinite ammo because it is an NPC base.

## 25. Hacking

A successful hack grants only defined local/system capability.

Possible results:

- open door;
- suppress camera;
- disable one turret network;
- obtain map;
- create temporary credential;
- interrupt reinforcement call;
- access objective data.

No universal master terminal exists by default.

## 26. Interior Power Manipulation

The attacker may manipulate local power if a real dependency exists.

Possible outcomes:

- lights out;
- door behavior changes;
- local turrets offline;
- environmental support degraded;
- shield/control subsystem affected.

Power loss can also harm the attacker by disabling lifts/doors/life support.

## 27. Lighting

Loss of lights changes visibility but does not automatically blind:

- thermal sensors;
- low-light sensors;
- player equipment;
- robots

that have appropriate capability.

## 28. Atmosphere

Interior combat respects actual local atmosphere/pressure state.

Breaches, fire, damaged life support, toxic release, or deliberate venting can create hazards.

## 29. Deliberate Venting

An attacker/defender can vent a compartment only through a valid controllable system/physical route.

Venting is not a generic kill switch.

Actors with pressure-safe equipment/robot compatibility may remain capable.

## 30. Fire

Combat/system damage can create fire where the target material/environment supports it.

Fire interacts with pressure/oxygen and can block routes or damage systems.

## 31. Friendly Fire

Player robots/allies remain physically damageable according to GDS-9.

Tight corridors therefore create firing-line considerations.

## 32. Objective Navigation

Mandatory objectives can be:

- known exact target;
- search area;
- discovered through intel/hacking/exploration.

The system never points through undiscovered walls with omniscient exact coordinates unless mission intelligence justifies it.

## 33. Optional Objectives

Interior raids can include optional:

- secondary loot;
- extra sabotage;
- rescue;
- data collection;
- defense suppression;
- prisoner release.

Optional work increases time/exposure but is not required for base success.

## 34. Time Pressure

There is no universal interior raid timer.

Pressure emerges from:

- reinforcements;
- alarms;
- limited suit/robot energy;
- ammo;
- ship risk;
- hazards;
- mission-specific timer where explicit.

## 35. Counterattack

Defenders can attempt to:

- retake rooms;
- cut off Bridgehead;
- disable player's docked ship;
- repair/restart defenses;
- protect loot/objective;
- activate reinforcement path;
- trap squad through lockdown.

## 36. Defender Repair

NPC defenders may restore a system only if raid content grants:

- repair actor/automation;
- access;
- time;
- parts/power.

A destroyed turret does not respawn because the player looked away.

## 37. Tactical Withdrawal

The player can begin withdrawing at any time.

Interior retreat means physically returning toward a valid extraction route.

The player can leave optional loot/objectives behind.

## 38. Lost Route

If the original Bridgehead/extraction route becomes inaccessible, the player must:

- restore it;
- find another valid route;
- create another breach;
- use alternate extraction.

The game does not teleport the player out.

## 39. Persistent Interior State

Committed state can persist:

- opened/breached doors;
- destroyed defenders;
- destroyed/disabled systems;
- looted containers;
- hacked persistent access;
- pressure/fire state;
- robot/player damage;
- objective state.

## 40. Edge Cases

If a defender repairs a previously disabled security controller before the player exits, doors/turrets using it can legitimately reactivate.

If loot blocks player capacity, the player must choose what to leave/transfer; objective completion does not increase backpack size.

If the original ship-side Bridgehead is overrun, robots there can fight/retreat under their actual orders rather than being auto-recalled.

If a local power cut disables the lift needed for extraction, alternate power/route must be found.

## 41. Explicit Non-Goals

Interior attack does not provide:

- teleporting room travel;
- permanently safe cleared rooms;
- infinite defender ammo;
- universal hackable doors;
- global base ownership from local control;
- omniscient objective markers;
- automatic extraction when route is lost.

## 42. Tuneable Parameters

Defender density, patrol/response timing, local control radius, communication attenuation, repair response, hazard intensity, and supply placement are tuneable.

## 43. Dependencies

This specification depends on Boarding/Breaching, GDS-2 Security/Atmosphere/Power, GDS-8 Objectives/Exploration, GDS-9 Combat/AI, GDS-10 Robots/Commands, Sabotage, Looting, Escalation, and Extraction.

## 44. Open Questions

None in the interior-attack baseline.
