# Mission Zones

> **Status:** Design Complete  
> **Authority:** Bounded playable mission environments, zone categories, instance boundaries, entry/exit points, local persistence, traversal topology, and zone cleanup/resolution

## 1. Purpose

Mission Zones are bounded 3D spaces where external mission gameplay occurs.

They create focused environments without simulating entire planets or seamless star systems.

## 2. Zone Categories

Baseline categories:

- Surface Zone;
- Interior Zone;
- Local Spaceflight Zone;
- EVA Exterior Zone;
- Mixed Zone.

## 3. Surface Zone

Represents a bounded area on a planet/moon.

Can contain:

- terrain;
- structures;
- deposits;
- hazards;
- landing/extraction site;
- POIs.

## 4. Interior Zone

Represents an authored/procedural enclosed environment such as:

- station;
- ship;
- colony facility;
- bunker;
- research site.

Can use real:

- doors;
- pressure;
- power;
- atmosphere;
- damage;
- access state

where the owning systems support them.

## 5. Local Spaceflight Zone

A bounded 3D flight volume around:

- station;
- asteroid field;
- wreck field;
- orbital facility;
- combat encounter.

GDS-6 Flight governs piloting.

## 6. EVA Exterior Zone

A player-scale exterior space around:

- ship;
- station;
- structure;
- asteroid.

GDS-5 governs zero-g movement/survival.

## 7. Mixed Zone

Connects two or more categories.

Example:

Local Spaceflight  
→ dock disabled ship  
→ Interior Boarding Zone  
→ EVA exterior repair  
→ return to ship.

## 8. Zone Identity

Each active zone has:

- Zone Instance ID;
- owning Mission Instance ID or persistent World Location ID;
- world template;
- seed where procedural;
- environmental state;
- entity state;
- objective state;
- discovery state.

## 9. Entry Points

A zone declares valid Entry Points.

Examples:

- ship landing site;
- docking connector;
- airlock;
- boarding breach;
- local-flight arrival volume;
- mission transport insertion.

## 10. Exit Points

Exit Points are physical/logical routes out.

They can include:

- player ship;
- extraction shuttle;
- docking connector;
- local-flight transition;
- strategic navigation exit.

## 11. Entry Safety

Normal mission generation cannot spawn the player in an unavoidable instant-failure state without the required equipment.

Known unavoidable entry hazards are deployment Hard Requirements.

## 12. Boundaries

Zone boundaries are explicit gameplay boundaries.

They can be represented through:

- terrain/geometry;
- navigation range;
- hazardous impassable environment;
- mission operation boundary;
- local-flight transition boundary.

## 13. Boundary Feedback

Approaching a nonphysical boundary provides clear warning before enforcement.

The game does not use unexplained invisible walls where a plausible presentation exists.

## 14. Boundary Crossing

Leaving through an unauthorized boundary does not silently succeed the mission.

The zone either:

- blocks traversal;
- transitions through an explicit exit;
- warns and returns/redirects;
- triggers mission withdrawal where authored.

## 15. Traversal Connectivity

Every required objective and extraction route must be reachable under at least one valid state path.

Procedural validation enforces this.

## 16. Locked Areas

Locked areas require explicit solutions such as:

- access authorization;
- repair;
- power restoration;
- alternate route;
- combat/breach;
- mission key/data.

There is no universal invisible quest lock.

## 17. Local State Persistence

During an active mission, the zone remembers:

- defeated/disabled entities where applicable;
- opened containers;
- harvested deposits;
- destroyed authored objects;
- repair state;
- door/access state;
- discovered POIs;
- dropped items;
- survivor state.

## 18. No Resource Respawn During Active Mission

Harvested resource deposits and one-time salvage do not respawn because the player:

- leaves a room;
- changes sub-zone;
- saves/loads.

## 19. Sub-Zones

Large missions can stream/transition between sub-zones.

Sub-zones remain part of the same Mission Instance and share objective/inventory state.

## 20. Player Ship Presence

When the player's ship physically enters/lands in a mission zone, it remains the same persistent GDS-6 ship.

The mission does not spawn a disposable duplicate.

## 21. Ship Safety

A landed/docked ship can remain vulnerable if the mission/combat rules allow threats to reach it.

It is not automatically invulnerable because it is an extraction point.

## 22. Environmental State

Zone environment can define:

- atmosphere;
- gravity;
- temperature;
- radiation;
- contamination;
- weather;
- vacuum;
- local hazards.

Player Field Survival consumes these states.

## 23. Zone Maps

Exploration can reveal local map information.

The map shows only justified discovered/known data.

## 24. Repeatable Locations

A persistent Strategic Location can host later missions.

Resolved unique changes persist.

A new procedural mission can instantiate fresh mission-specific content only where the location template allows it.

## 25. Story Locations

Canonical GDS-7 story locations preserve:

- identity;
- required geometry landmarks;
- mandatory evidence/characters;
- major resolved state.

Procedural assembly cannot erase story-critical content.

## 26. Cleanup on Resolution

When a nonpersistent mission zone closes:

- unresolved Field-Unsecured mission-world items cease to be available unless a rule explicitly preserves recovery;
- Vehicle/Extraction-Secured cargo remains with its owner;
- unique persistent outcomes commit to world state;
- transient actors/objects are released.

## 27. Revisit Rule

A resolved one-shot mission instance is not re-entered as though still active.

If the Strategic Location remains visitable, a later visit uses its persistent post-resolution world state or a new mission instance.

## 28. Save/Load

Active zone state must restore equivalently.

The player cannot reload to regenerate deposits, containers, survivors, or objective placement.

## 29. Explicit Non-Goals

Mission Zones do not require:

- full planets;
- endless space;
- resource respawn during active instance;
- duplicate player ships;
- unexplained invisible quest locks;
- arbitrary room resets.

## 30. Tuneable Parameters

Tuneable values include zone dimensions, sub-zone count, streaming thresholds, POI density, boundary warning distance, and traversal complexity.

## 31. Dependencies

This specification depends on World locations, Mission System, Generation, Exploration, Hazards, Player, Spacecraft, Combat, and Persistence.

## 32. Open Questions

None in the mission-zone baseline.
