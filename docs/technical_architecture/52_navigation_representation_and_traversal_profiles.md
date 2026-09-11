# TA-8 — Navigation Representation and Traversal Profiles

> **Status:** Architecture Complete
> **Authority:** Navigation backends, navigation-space representation, traversal profiles, capability/access filtering, cover/work/navigation annotations, and the boundary between navigation and gameplay authority

## 1. Purpose

StarForge needs navigation for crew, enemies, robots, wildlife, and selected autonomous local actors across interiors, planetary surfaces, station geometry, and bounded 3D flight/Zero-G spaces.

Navigation must describe where an actor can plausibly travel. It never grants permission, opens doors, ignores hazards, or teleports an actor.

## 2. Backend Boundary

Grounded navigation uses **Recast/Detour** behind a StarForge-owned `navigation` adapter.

Recast/Detour is commodity infrastructure for:

- navmesh generation/cooking;
- polygon connectivity;
- path-corridor queries;
- nearest navigable point queries.

Raw `dtPolyRef`, `dtNavMesh`, `dtNavMeshQuery`, or Recast build types never become gameplay/persistent identities.

## 3. Navigation Authorities

Navigation answers questions such as:

- is a traversable route geometrically available for this profile?;
- which authored traversal links can connect the route?;
- what path corridor and costs apply?;
- what currently known hazard/access overlays affect that route?

Navigation does **not** decide:

- door authorization;
- hacking success;
- mission objectives;
- acceptable-risk policy;
- combat target selection;
- whether a resource should be spent;
- whether a robot is commanded to move.

Those remain owning gameplay/AI domain decisions.

## 4. Navigation Spaces

Each active `SceneInstance` can expose one or more typed `NavigationSpace`s.

Baseline spaces:

1. `GroundNavigationSpace` — interiors and walkable planetary/surface geometry;
2. `FreeFlightNavigationSpace` — bounded 3D space for flying drones and compatible Zero-G autonomous actors;
3. optional specialized authored navigation graph for a content case that cannot be represented safely by the first two.

There is no galaxy-scale navigation mesh.

## 5. Ground Navigation Representation

Ground navigation is a tiled 2.5D navmesh derived from cooked/canonical collision and authored navigation metadata.

The baseline keeps broad mesh classes rather than one unique mesh per actor:

- `GroundNavClass::Small`;
- `GroundNavClass::Standard`;
- `GroundNavClass::Heavy`.

Exact actor capability is then applied with `NavTraversalProfile` filters and typed traversal links.

## 6. Traversal Profile

Every autonomous mobile actor has a `NavTraversalProfile` containing at minimum:

- body radius/clearance class;
- body height/vertical clearance class;
- maximum supported step/ledge height;
- maximum walkable slope;
- grounded or free-flight mobility;
- Heavy/Standard/Small path class;
- door/airlock interface capability;
- ladder capability;
- elevator/lift capability;
- Zero-G/Magnetic-Boot capability where applicable;
- environmental protection tags;
- access/security identity supplied by the owning domain;
- load-bearing requirement where authored;
- traversal-cost policy.

Content cannot infer capabilities merely from actor name.

## 7. Humanoid Baseline

Player-independent humanoid NPCs normally use `GroundNavClass::Standard`.

They may traverse:

- ordinary walkable surfaces;
- stairs/slopes within their CharacterMotor envelope;
- doors they are authorized to operate;
- elevators/lifts through authored traversal links;
- ladders only if their actor/profile supports the authored ladder interaction;
- airlocks only through a valid airlock traversal sequence.

A navmesh polygon behind a locked/sealed door does not grant access.

## 8. Robot Size and Mobility

Robot classes map to traversal constraints rather than generic AI exceptions.

Typical baseline:

- Light ground/legged robots -> Small or Standard profile according to chassis;
- Medium robots -> Standard;
- Heavy Mech -> Heavy;
- hover/flying Recon or support drones -> FreeFlight where their chassis actually supports it.

Heavy profiles can require:

- larger doorway/turning clearance;
- stronger/load-bearing routes where authored;
- Heavy-capable lifts/ramps;
- exclusion from light racks, ladders, narrow maintenance passages, and other incompatible links.

## 9. Free-Flight Navigation

Flying/Zero-G autonomous actors do not fake 3D movement by projecting onto a floor navmesh.

`FreeFlightNavigationSpace` uses a bounded 3D graph/volume representation generated from authored/cooked free-space cells and portals.

A free-flight route respects:

- solid geometry;
- minimum clearance radius;
- sealed bulkheads/doors;
- allowed openings;
- hazard/access overlays;
- actor-specific propulsion/environment capability.

The representation is scoped to the current bounded local scene, never astronomical space.

## 10. Traversal Links

Nonordinary movement uses typed `NavTraversalLink`s.

Canonical link kinds include:

- Door;
- Airlock;
- Elevator/Lift;
- Ladder;
- Jump/Mantle only for actors explicitly supporting the maneuver;
- Breach Passage once the owning target is actually breached/open;
- Dock/boarding connector;
- Magnetic-Boot transfer surface where authored;
- FreeFlight portal.

A link contains stable scene/content key, endpoints, required capability tags, current operational state, and dynamic cost/block state.

## 11. Door and Airlock Semantics

Closed but operable doors can remain represented as potential traversal links.

Path planning may include them only if the requesting actor currently has:

- capability to operate the interface;
- sufficient authorization/command policy;
- a safe/valid pressure/security sequence according to owning systems.

The navigation layer never opens the door itself.

For an airlock, route execution is a multi-step gameplay interaction, not one instant edge traversal.

## 12. Elevators

Elevators are time-dependent traversal links.

Path planning can include a functioning compatible lift, but execution requires:

1. reach call/entry point;
2. request/await lift through gameplay interface;
3. physically enter;
4. ride while simulation continues;
5. physically exit.

The nav path is suspended/revalidated around the transition rather than teleporting the actor between floors.

## 13. Hazard Cost Overlay

Known hazards are represented through dynamic area cost/block overlays.

Examples:

- fire;
- vacuum;
- toxic atmosphere;
- severe heat/cold;
- radiation;
- active hostile kill zone where doctrine permits avoidance.

The actor's equipment/protection and AI risk policy determine whether an area is:

- forbidden;
- high cost;
- ordinary cost.

Unknown hazards are not magically included in an actor's route knowledge.

## 14. Security and Permission Overlay

Security is not baked permanently into navmesh geometry.

Path queries receive an access filter derived from current actor/faction/mission/security state.

A path through a geometrically traversable but unauthorized security door is invalid unless the current task explicitly has a valid override/hacking/breach plan.

## 15. Navigation Annotations

Authored/cooked scene data can provide typed annotations such as:

- work positions;
- cover candidates;
- observation/vantage points;
- muster points;
- safe waiting positions;
- robot rack/bay approach points;
- repair/hack/breach interaction anchors;
- patrol anchors;
- reinforcement entry anchors.

Annotations are candidates only. Owning AI/gameplay systems validate them before use.

## 16. Cover Data

Cover is derived from real world geometry plus authored/cooked annotation metadata.

A cover point can carry:

- position and facing;
- supported stance/height;
- protected directions;
- peek/fire positions;
- actor-size compatibility;
- current validity revision.

Cover never makes geometry bulletproof; TA-5/GDS-9 physical hit rules remain authoritative.

## 17. Destination Projection

A requested destination is accepted only if it can be projected to a valid location under the actor's traversal profile within a bounded tolerance.

The AI must not silently snap a target across:

- wall;
- floor/ceiling;
- locked boundary;
- incompatible elevation;
- hazardous forbidden boundary.

## 18. Local Avoidance Boundary

Global/path-corridor navigation and local collision avoidance are distinct.

Pathfinding selects a legal route corridor. Runtime locomotion uses project-owned short-range steering/yielding over TA-7 spatial neighbors plus TA-5 CharacterMotor/physics.

Local avoidance may reduce NPC-vs-NPC blocking, but can never disable world collision or traverse a forbidden polygon/link.

## 19. Navigation State Revisions

Every active navigation space has a monotonic `NavigationRevision`.

Topology/cost changes that can invalidate path semantics increment the relevant revision.

Paths remember the revision(s) against which they were planned.

## 20. Persistence Boundary

Cooked navmesh/3D navigation structures are technical reconstructible scene data, not save authority.

Saves persist gameplay-relevant state such as:

- actor logical destination/task;
- current command/activity;
- location;
- path/task state only where needed for exact resume semantics.

Runtime polygon refs, query objects, path node pointers, and temporary steering state are not persistent identity.

## 21. Headless Boundary

Navigation queries and path validation must run without renderer/audio/UI.

Navigation cooking can be a build/content-pipeline concern later under TA-10; runtime tests can load deterministic cooked test navigation fixtures.

## 22. Explicit Non-Goals

TA-8 does not introduce:

- one navigation mesh for the whole galaxy;
- one actor-specific navmesh per individual NPC;
- navmesh as door/security authority;
- floor-navmesh projection for true flying/Zero-G movement;
- teleport path recovery;
- cover immunity;
- pathfinding that ignores TA-5 collision shape constraints.

## 23. Dependencies

Depends on TA-3 Scene/Zone/streaming, TA-5 collision/CharacterMotor, TA-6 station topology/doors/hazards, TA-7 runtime entities, Crew presence, Robots, and GDS-9 enemy behavior.

## 24. Open Questions

None within the TA-8 navigation-representation baseline.