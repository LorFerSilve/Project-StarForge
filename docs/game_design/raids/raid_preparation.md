# Raid Preparation

> **Status:** Design Complete  
> **Authority:** Pre-raid loadout planning, objective selection, access plan, ship/squad preparation, hard requirements, warnings, cargo/extraction planning, and deployment commit

## 1. Purpose

Preparation converts intelligence into an executable physical raid plan.

The system must expose why a configuration is valid, risky, or impossible without auto-solving the raid for the player.

## 2. Preparation Inputs

A raid preparation state references:

- Raid/Mission ID;
- selected primary objective branch where choices exist;
- known intelligence;
- selected spacecraft;
- player loadout;
- active Tactical Squad;
- reserve robots;
- ammunition/consumables;
- tools;
- breaching/hacking equipment;
- cargo capacity;
- extraction plan;
- mission-specific supplies.

## 3. Raid Plan

A raid plan contains four explicit plans:

1. Approach Plan;
2. Access Plan;
3. Interior Objective Plan;
4. Withdrawal Plan.

These plans inform readiness checks and UI.

They do not script all player behavior after deployment.

## 4. Approach Plan

Possible approach categories:

- Direct Assault;
- Low-Signature Approach;
- Authorized/Spoofed Docking;
- Remote/EVA Approach;
- Mission-authored special route.

The target must physically support the chosen method.

## 5. Access Plan

Possible access methods:

- legal/authorized dock;
- spoofed credential dock;
- hacked docking control;
- forced docking;
- maintenance airlock;
- EVA service access;
- breaching target;
- boarding through an already damaged opening.

Each method defines exact prerequisites.

## 6. Interior Objective Plan

The player identifies intended priority such as:

- reach storage;
- disable shield control;
- steal archive;
- sabotage reactor support;
- rescue target;
- capture command node.

This is planning metadata, not a requirement to follow only that route.

## 7. Withdrawal Plan

The player selects/acknowledges intended extraction method:

- return to player ship;
- designated dock/airlock;
- external EVA return;
- mission transport;
- local-space departure.

A raid cannot be launched with a known mandatory extraction that the selected transport can never perform.

## 8. Hard Requirements

A **Hard Requirement** blocks deployment when the game has confirmed that the raid cannot be validly attempted.

Examples:

- insufficient Strategic Reach;
- no valid transport to target;
- mandatory Heavy Mech objective but no Heavy Robot Bay;
- required hacking objective but no valid hacking-capable actor/tool;
- required breach with no compatible breaching capability;
- unavoidable lethal environment with no valid protection;
- mandatory extracted object exceeds all selected transport capacity;
- no valid return/extraction capability.

## 9. Warnings Are Not Blocks

Preparation warnings do not block launch.

Examples:

- low ammunition;
- incomplete intelligence;
- weak shield protection;
- high hostile threat;
- no Repair Drone;
- limited cargo room;
- no reserve robots;
- likely point-defense saturation;
- selected route has unknown interior map.

The player can deliberately accept these risks.

## 10. No Omniscient Hard Block

The validator can only hard-block a target-specific requirement when that blocker is known to the player/system through legitimate knowledge or is intrinsic to the selected equipment itself.

It cannot reveal a secret defense by saying "missing counter to hidden weapon."

## 11. Player Loadout Validation

The player must satisfy GDS-5 inventory/equipment rules.

Preparation does not create:

- ammo;
- medkits;
- suit energy;
- tools;
- grenades.

All deployment supplies leave actual station/ship ownership through existing loadout transactions.

## 12. Robot Squad Validation

GDS-10 remains authoritative for:

- TCC;
- Command Load;
- Robot Rack Units;
- Heavy Robot Bay;
- energy;
- ammo;
- configuration validity;
- environment compatibility.

Raid preparation displays those results rather than redefining them.

## 13. Recommended Robot Roles

Intelligence can produce recommendations.

Examples:

- shield-heavy defense → Hacker/EMP/Sabotage opportunity;
- armored access → Breaching Unit;
- complex interior security → Hacker Unit;
- long operation → Repair Drone;
- open heavy-combat sector → Heavy Mech;
- uncertain layout → Recon Drone.

Recommendations never become hidden mandatory class locks unless the objective truly requires that capability.

## 14. Heavy Robot Interior Compatibility

Before launch, known access/interior geometry can identify whether a Heavy Mech can traverse the intended route.

If no known path can physically fit the Heavy unit but the Heavy unit is optional, deployment is allowed with a warning that it may remain external/in hangar areas.

If the Heavy unit is mandatory for the objective and no valid route exists, deployment is blocked.

## 15. Ammunition Planning

The preparation screen shows:

- player ammo by compatible weapon;
- robot ammo;
- ship weapon ammo;
- station/target resupply assumptions only when actually available.

There is no generic "raid ammo pool."

## 16. Energy Planning

Preparation shows current:

- suit energy;
- robot Internal Energy Reserve;
- ship batteries/fuel/propellant;
- relevant shield state.

It does not restore them automatically.

## 17. Breaching Supplies

Breaching equipment can include:

- Breaching Unit tool;
- cutting equipment;
- controlled breaching charges;
- compatible EVA access equipment.

A breaching charge is a physical consumable if its device definition says so.

## 18. Hacking Capability

Hacking validation requires at least one actual route-capable method such as:

- player hacking tool;
- Hacker Unit;
- authorized access credential;
- ship-system hack interface where explicitly supported.

Having a high general "tech score" is not sufficient.

## 19. Cargo Planning

The player can reserve free ship cargo for expected raid loot.

Cargo planning distinguishes:

- Mass;
- Volume;
- containment;
- secure storage;
- robot/wreck recovery space;
- mandatory objective cargo.

## 20. No Loot Teleport Assumption

Projected loot capacity assumes the player must physically move loot to a valid owner.

A large target storage room does not mean the player can take everything.

## 21. Salvage Planning

If the player intends to recover:

- disabled robot;
- robot wreck;
- heavy component;
- turret part;
- captured equipment,

recovery capacity must exist.

## 22. Extraction Readiness

The selected spacecraft must retain a plausible extraction path under current known state.

Preparation can warn if:

- drive margin is poor;
- ship is already damaged;
- fuel/propellant reserve is low;
- target defenses threaten departure;
- route requires a disabled system to remain functional.

## 23. Minimum Return Reserve

The system can calculate a known minimum fuel/propellant/life-support reserve for planned return travel.

The player can override a recommended reserve only if the resulting trip remains physically possible or an alternate recovery path exists.

## 24. Repairs Before Deployment

Damaged equipment/robots/ship can be deployed if still operational and no hard requirement fails.

Preparation clearly reports degraded capability.

There is no mandatory full-repair gate.

## 25. Intelligence Confidence in Readiness

Readiness output distinguishes:

- Confirmed Valid;
- Valid With Warnings;
- Unknown Risk;
- Blocked.

Unknown Risk indicates incomplete intel, not a secretly calculated failure chance.

## 26. Target Alert Before Deployment

A target can already be:

- unaware;
- suspicious;
- alerted/fortified

because of prior player actions or world state.

Preparation displays known state.

Declining/reopening the same Raid ID does not reset it.

## 27. Raid Cost Is Physical

Potential raid cost comes from actual:

- ammunition;
- consumables;
- ship fuel/propellant;
- robot attrition;
- equipment/ship damage;
- repair resources.

No universal Entry Fee is required by GDS-11.

## 28. Deployment Commit

On deployment commit:

1. selected physical loadout becomes deployed under GDS-5/GDS-8;
2. selected robots/ship occupancy are locked to actual transport state;
3. mission enters En Route/Deployed as appropriate;
4. consumed travel resources begin following GDS-6;
5. raid target state is not regenerated.

## 29. Plan Editing After Deployment

Once deployed, the preparation plan becomes reference information.

The player can adapt tactically.

Changing route does not require returning to a menu unless a physical loadout change requires station/ship access.

## 30. Abort Before Departure

Before deployment commit, cancellation releases unconsumed reservations according to existing systems.

## 31. Abort After Departure

After deployment/travel begins, cancellation becomes mission abandonment/retreat under GDS-8/GDS-11.

Already consumed resources are not refunded.

## 32. Raid Presets

The player may save raid loadout/squad presets.

A preset references desired owned items/Robot IDs/configurations and does not duplicate them.

## 33. Main-Story Raid Branches

Main-story raid branches use the same preparation validator.

Narrative importance cannot silently supply missing physical breaching, transport, survival, or extraction capability unless the mission explicitly provides it.

## 34. Explicit Non-Goals

Raid preparation does not provide:

- auto-winning recommended composition;
- hidden-enemy omniscient hard blocks;
- free ammunition/energy;
- abstract raid tickets;
- automatic full repair;
- unlimited cargo assumptions;
- teleporting reserve robots.

## 35. Tuneable Parameters

Tuneable values include warning thresholds, recommended reserves, cargo forecast margins, readiness UI severity, and preparation presets.

## 36. Dependencies

This specification depends on Target Intelligence, GDS-5 Loadout, GDS-6 Travel/Cargo, GDS-8 Mission deployment, GDS-9 Combat, GDS-10 Squad/Transport, and Economy/Presentation.

## 37. Open Questions

None in the raid-preparation baseline.
