# External Space Attack

> **Status:** Design Complete  
> **Authority:** Raid approach contact, external defense engagement, shield-zone attack, turret/sensor/comms targeting, approach corridors, suppression windows, ship/robot exterior support, and transition to boarding access

## 1. Purpose

The external phase creates a physical path to the target rather than treating boarding as an automatic button.

It can involve:

- stealth/low-signature approach;
- sensor evasion;
- direct spacecraft combat;
- selective subsystem attack;
- defense suppression;
- docking access creation;
- EVA support.

## 2. Phase Is Conditional

External combat is **not mandatory in every raid**.

A valid access plan may avoid or shorten it through:

- accepted docking authorization;
- spoofed credentials;
- maintenance approach outside major defense arcs;
- previously disabled defenses;
- alliance/story access;
- unpowered/abandoned target.

If the target engages or the chosen plan requires force, the external attack rules apply.

## 3. External Raid Space

The external raid zone is a bounded Local Spaceflight/EVA Mission Zone under GDS-8.

It contains raid-relevant:

- target station geometry;
- shield coverage;
- defensive emplacements;
- docking points;
- sensor nodes;
- communications;
- boarding routes;
- local hazards;
- defending craft;
- approach/departure boundaries.

## 4. Approach States

Canonical approach states:

- Outside Detection Envelope;
- Detected but Unclassified;
- Tracked;
- Challenged;
- Hostile Engagement;
- Access Window Available.

Not every target uses every state.

## 5. Detection

Detection uses actual:

- target sensors;
- player ship signature;
- range;
- line/occlusion/environment;
- target alert state;
- spoofing/jamming where supported.

There is no arbitrary distance at which every station automatically knows exact player identity.

## 6. Challenge / Identification

A controlled station can challenge an approaching ship.

Possible player responses depend on available capability:

- valid authorization;
- spoofed identity;
- withdrawal;
- silence/evasion;
- hostile action.

Failure of spoofing/hacking follows the owning system and can escalate alert.

## 7. Hostility Trigger

External defenses can engage when:

- target correctly identifies player as hostile;
- player fires/destructively acts;
- player violates a defended exclusion zone under known policy;
- raid/story state already marks the target hostile.

Unknown contacts are not automatically omniscient enemies without policy justification.

## 8. Defensive Target Classes

Raid targets can field:

- station laser turrets;
- rail/kinetic turrets;
- missile batteries;
- point-defense;
- EMP/disruption systems;
- defensive drones/ships;
- local shield emitters;
- sensor arrays.

Their mechanics reuse GDS-2 Defenses and GDS-9 Combat.

## 9. No Global Station Defense HP

The attacker chooses actual targets.

Examples:

- destroy a shield emitter;
- disable local turret power;
- destroy sensor array;
- jam communication node;
- attack docking control;
- eliminate defending craft;
- exploit an uncovered physical route.

## 10. Shield Coverage

Station shield emitters protect authored spatial volumes/sectors under GDS-2.

A raid can:

- attack emitter hardware where physically exposed;
- overload/deplete shield through damage;
- cut/disrupt its power/control dependency;
- find an unprotected route.

Destroying one emitter does not collapse unrelated shield zones.

## 11. Shield Suppression Window

If a shield is temporarily:

- depleted;
- EMP-disabled;
- power-isolated;
- hacked offline,

an **Access Window** can exist until it regenerates/restores.

The player must physically exploit the window.

The game does not freeze the disabled system because the player reached a checkpoint.

## 12. Turret Neutralization

A turret can be:

- avoided outside its arc;
- jammed/disabled if supported;
- deprived of target data;
- deprived of power/ammunition;
- physically destroyed.

Each route has actual dependencies.

## 13. Sensor Attack

Destroying/jamming sensors can reduce:

- target tracking;
- long-range engagement;
- missile lock quality;
- defender shared awareness.

Local sensors can preserve limited capability.

Sensor loss does not blind every defense if local sensing remains.

## 14. Communication Attack

Destroying/jamming communications can:

- delay/prevent reinforcement call;
- reduce external coordination;
- delay target information sharing.

It does not automatically disable local turrets, doors, or security unless those systems truly depend on that communication path.

## 15. Power Attack

If an exposed raid-relevant power node supplies defenses, damaging it can disable connected loads.

The target's dependency map determines effect.

The game does not apply a generic "power node destroyed = whole station off" rule.

## 16. Defensive Ammunition

Target kinetic/missile defenses consume finite modeled raid-relevant ammunition/magazines.

They cannot fire indefinitely after their valid supply is depleted/isolated.

## 17. Defensive Craft

Defending ships/drones enter through valid:

- hangar;
- patrol position;
- local approach route;
- reinforcement arrival.

They do not spawn visibly from nothing.

## 18. Point-Defense Saturation

A player may overwhelm point-defense through enough simultaneous valid threats, but every missile/drone/shot is still physical/finite.

There is no abstract saturation percentage that bypasses actual capacity.

## 19. Player Ship Role

The player's spacecraft can:

- fight;
- suppress defenses;
- hold position;
- provide sensors;
- deploy robots;
- support EVA/boarding;
- carry loot/extract.

Damage/power/thermal/ammo remain GDS-6/GDS-9 authority.

## 20. Ship Exposure Tradeoff

Moving close enough to dock/breach can expose the player ship to:

- turrets;
- missiles;
- debris;
- collision;
- boarding counterattack;
- shield overlap.

A safer stand-off position can require EVA or another transport route.

## 21. Robot Exterior Use

Compatible robots may deploy externally only if:

- physically supported by ship/bay;
- environmental configuration supports space/EVA;
- valid path/deployment exists;
- TCC/command link permits it.

A ground-only Heavy Mech is not automatically a space drone.

## 22. Exterior Robot Tasks

Possible tasks:

- Recon Drone sensor survey;
- Hacker Unit physical interface access if EVA-capable;
- Repair Drone support to player ship/boarding equipment;
- Demolition/Breaching Unit external charge placement where compatible;
- Shield/Assault unit support only when movement/environment allows.

## 23. Docking Under Fire

A Hard Dock attempt uses GDS-6 docking mechanics.

Docking can fail because of:

- relative motion;
- damaged docking system;
- occupied/locked port;
- active defenses;
- destroyed connector;
- physical obstruction.

The game does not snap the ship to a port merely because a raid objective says "board."

## 24. Forced Docking

Forced docking can mean:

- override/hack docking clamps;
- physically mate to a compatible but unauthorized port;
- use a specialized boarding adapter if installed.

It still requires compatible geometry and controlled relative motion.

## 25. Hull Breach Access

Creating a hull breach can require:

- sufficient weapon/tool capability;
- authored breachable geometry;
- proximity/access;
- safe standoff;
- subsequent pressurized/EVA traversal capability.

Station-scale weapons do not imply every wall is destructible.

## 26. Breach Consequences

An exterior breach can cause:

- decompression;
- atmosphere loss;
- alarm;
- structural hazard;
- defender rerouting;
- blocked/unsafe access.

These consequences persist according to target simulation.

## 27. No Free Breach Teleport

Destroying a panel does not teleport player/robots inside.

The resulting opening must be physically traversed.

## 28. External Objective Completion

Some raids can end externally.

Examples:

- destroy a communications array;
- disable a weapon platform;
- scan/mark target;
- plant exterior device;
- force defending ship retreat.

If all mandatory objectives are external, boarding is not required.

## 29. Transition to Boarding

Boarding becomes available when at least one valid entry method reaches its authored usable state.

Examples:

- dock secured;
- airlock hacked/open;
- breach created;
- maintenance route reached.

## 30. Retreat During External Phase

The player can break off before boarding.

Withdrawal requires actual local-space departure/strategic escape under GDS-6/GDS-8.

Consumed ammo, fuel, robot loss, and damage remain.

## 31. Reinforcement Interaction

External actions can accelerate escalation through:

- firing;
- alarms;
- communication detection;
- defender distress call.

Destroying/jamming reinforcement communication can prevent or delay only the paths dependent on it.

## 32. Persistence

Committed external state persists:

- destroyed defenses;
- shield state;
- ammo state;
- ship damage;
- breaches;
- sensor/comms state;
- surviving defending craft;
- access state.

## 33. Edge Cases

If a shield emitter recovers while the player is physically inside its projected volume, the shield model defines whether the envelope can safely reform; it may block/project around existing geometry but cannot teleport/destroy actors by undefined behavior.

If the player's docked ship becomes Disabled after boarding begins, interior raid can continue but extraction readiness updates immediately.

If all docking ports are destroyed but an EVA breach remains valid, boarding can continue through that route.

If target communications are destroyed after a reinforcement call was already successfully committed, the already-dispatched force is not cancelled unless their command path explicitly requires it.

## 34. Explicit Non-Goals

External attack does not provide:

- mandatory combat in every raid;
- one defense HP bar;
- station-wide shield collapse from one emitter;
- infinite defensive ammo;
- ship snap-docking;
- arbitrary wall destruction;
- reinforcement cancellation after an already committed dispatch without cause.

## 35. Tuneable Parameters

Detection ranges, alert response, weapon ranges, shield capacity/recharge, suppression duration, docking exposure, reinforcement call delay, and defense composition are tuneable.

## 36. Dependencies

This specification depends on Target Intelligence, Raid Preparation, GDS-2 Defenses, GDS-6 Flight/Docking/Combat, GDS-8 Mission Zones, GDS-9 Combat, GDS-10 Robots, Boarding/Breaching, and Escalation.

## 37. Open Questions

None in the external-attack baseline.
