# Station Structural Rules

> **Status:** Design Complete  
> **Authority:** Physical topology, attachment legality, compartment connectivity, and structural validity of the home station

## 1. Purpose

This specification defines the rules that determine whether station construction forms one valid persistent physical structure.

It deliberately avoids real-world finite-element structural simulation while preserving meaningful:

- attachment;
- topology;
- accessibility;
- compartmentalization;
- strategic layout.

## 2. Structural Root

The Command Core is the structural root of the home station.

Every permanent station structural module must have at least one valid structural path to the Command Core.

A path may traverse other structural modules.

## 3. Structural Graph

The station's permanent structure is represented conceptually as a graph:

- modules are nodes;
- compatible structural attachments are edges;
- the Command Core is the root node.

A valid permanent module belongs to the Command Core's connected structural component.

## 4. Construction Lattice

Permanent modules occupy authored volumes on the station's orthogonal 3D construction lattice.

Two incompatible solid occupancy volumes cannot occupy the same construction cells.

The lattice supports full three-dimensional expansion.

## 5. Structural Ports

Modules expose authored **Structural Ports**.

A Structural Port defines:

- position;
- orientation;
- connection class;
- compatible counterpart classes;
- whether the connection can carry an interior passage;
- whether the connection can transfer selected utilities;
- load class where relevant.

Two ports connect only when their transforms and compatibility rules match.

## 6. Structural Connection Classes

The baseline defines these structural classes:

### Standard

Used by normal corridors and room modules.

### Heavy

Used by large, high-mass, or high-load modules.

### Docking Structural

Used by station-side structures designed to support docking hardware or hangars.

### External Mount

Used by external devices such as turrets, sensors, or radiators.

A smaller connection class cannot satisfy a requirement for a stronger authored class.

## 7. No Continuous Stress Simulation

The game does not calculate realistic bending moments, fatigue, or material stress across the complete station.

Structural validity is rule-based.

Large authored modules may require:

- multiple attachment points;
- a Heavy connection;
- a supporting frame type;
- a combination of these.

These requirements are declared by the module definition.

## 8. Attachment Requirement

A newly completed structural module becomes part of the valid station only if it satisfies its authored minimum attachment requirement.

A normal Standard module generally requires at least one compatible path.

Large modules may require more.

## 9. Multi-Attachment Redundancy

A module may have multiple valid structural paths.

If one path is lost, the module remains attached while at least one valid required path remains.

This allows deliberate structural redundancy.

## 10. Disconnection

If damage destroys every valid structural path between a module branch and the Command Core, the branch becomes **Structurally Disconnected**.

A disconnected branch:

- is no longer considered part of the operational home-station structural network;
- cannot rely on station utilities crossing the broken connection;
- may retain local internal reserves if its subsystems define them;
- becomes an emergency state requiring repair/reconnection.

The branch is not automatically deleted.

## 11. Detached Geometry

A structurally disconnected branch does not require full orbital rigid-body simulation.

Its presentation and recovery behavior are governed by damage/repair rules.

The baseline does not require physically simulating an entire detached station wing drifting indefinitely with Newtonian orbital dynamics.

## 12. Structural Destruction

A structural connection can be lost through explicitly defined damage to:

- a connection;
- a module;
- an intervening structural element.

Visual damage alone does not sever topology unless the damage state is defined as structurally destructive.

## 13. Traversal Ports

Interior modules expose **Traversal Ports** separately from pure structural attachment.

A structural connection may:

- contain a traversable passage;
- be non-traversable;
- require an airlock;
- require a door;
- be service-only.

The player cannot traverse a connection merely because two modules are structurally attached.

## 14. Interior Accessibility Graph

Traversable interiors form an accessibility graph.

Nodes are accessible compartments or traversal spaces.

Edges are openable or traversable connections.

A player/crew-usable permanent module must have a valid designed route to the broader station before normal commissioning.

Emergency isolation may later temporarily break that route.

## 15. Doors

Doors separate traversal spaces without automatically separating structure.

Door state can affect:

- traversal;
- atmosphere;
- security;
- hazard containment.

A closed normal internal door remains structurally connected.

## 16. Bulkheads

Bulkheads are pressure-capable structural boundaries.

A bulkhead may contain:

- no opening;
- a pressure-rated door;
- an airlock;
- an authored service penetration.

Bulkhead state matters to atmosphere and damage containment.

## 17. Compartments

A **compartment** is a contiguous interior pressure volume bounded by pressure-rated surfaces and controlled openings.

Compartments are determined by constructed geometry and connection states rather than manually assigned arbitrary room labels.

## 18. Pressure Boundary Requirement

A compartment can be sealed only when every relevant boundary surface and opening is pressure-capable and intact.

A visual room without a valid sealed boundary is not considered pressure-sealed.

Atmospheric calculations belong to `atmosphere_and_oxygen.md`.

## 19. Airlocks

An airlock is a controlled transition between pressure domains.

Standard use cases include:

- station interior to vacuum;
- docked vehicle to station;
- isolated hazardous section to normal station.

An airlock requires an authored chamber and controlled doors; two ordinary doors placed close together do not automatically become a functional airlock.

## 20. Windows

Windows are pressure-boundary elements when authored as pressure-rated.

Window damage can compromise the compartment according to damage and atmosphere rules.

Decorative transparent geometry does not automatically have structural or pressure behavior.

## 21. Shafts and Vertical Access

Vertical station topology can use:

- lift shafts;
- ladder shafts;
- stair modules;
- maintenance shafts.

A vertical structural connection is not automatically traversable unless an appropriate access module exists.

## 22. Maintenance Access

Some functional equipment requires service clearance or maintenance access.

A module may be structurally valid but fail operational commissioning if required maintenance access is blocked.

This prevents the player from packing machinery into physically impossible service configurations.

## 23. External Equipment

External equipment attaches to compatible External Mount ports or authored surfaces.

External devices may not require an interior walkable path if their maintenance specification permits:

- EVA servicing;
- robotic servicing;
- internal service access.

## 24. Docking Clearance

Docking structures reserve external approach and occupancy volumes.

Permanent construction cannot intrude into a required active docking-clearance volume.

When no compatible ship is docked, some authored structures may permit configurable clearance, but the docking specification owns those rules.

## 25. Weapon Clearance

Certain defensive weapons require firing or rotation clearance.

Construction that blocks required mechanical motion is invalid unless the defense definition explicitly permits a restricted firing arc.

## 26. Utility Transfer Through Structure

Structural attachment and utility transfer are distinct.

A connection may support some or all of:

- electrical power;
- control/data;
- atmosphere;
- thermal transfer;
- water;
- logistics.

Each connection definition declares supported utility channels.

Subsystem specifications define actual network behavior.

## 27. Utility Isolation

A structurally connected module may be deliberately isolated from one or more utility networks.

Examples:

- electrical breaker open;
- atmosphere valve closed;
- logistics route disabled.

Structural validity does not force utility continuity.

## 28. Construction Dependency

A planned blueprint can use another planned blueprint as a future structural parent.

It does not become structurally complete until the required parent connection exists physically.

## 29. Deconstruction Dependency

A completed module that is the sole structural bridge to other completed modules cannot be individually removed without addressing its dependent branch.

The construction system must either:

- reject the removal; or
- require explicit confirmation of a valid joint deconstruction plan.

## 30. Occupant Safety During Geometry Change

Construction/deconstruction may not knowingly remove the only safe containing or traversal geometry around the player or non-evacuated crew.

The player cannot exploit build mode to:

- delete a room around an occupant;
- trap crew without valid evacuation;
- bypass a locked door by removing its containing wall during live gameplay.

## 31. No Build-Mode Combat Exploits

Construction Mode is unavailable during active station combat.

Therefore the player cannot instantly create walls, remove attack routes, or relocate defenses while hostile combat is ongoing.

Repairs and emergency actions use their dedicated systems instead.

## 32. Structural State Categories

A completed structural element may be:

- **Valid** — all required structural conditions satisfied;
- **Damaged** — structural function remains but durability is reduced;
- **Critical** — connection remains but is close to structural failure according to its damage rules;
- **Broken** — required structural edge is lost;
- **Disconnected** — module still exists but lacks a valid path to the Command Core.

## 33. Repairing Structural Connectivity

Restoring a destroyed path requires:

- physical repair or replacement of the failed structural element;
- required repair resources;
- applicable labor/tool access.

A disconnected branch does not reconnect merely because the player is nearby.

## 34. Structural Failure and Utilities

When a structural connection breaks, utility connections that physically cross that connection also break unless their subsystem explicitly defines an independent alternate route.

The structural system reports topology changes to dependent networks.

## 35. Structural Failure and Atmosphere

A structural failure does not automatically imply decompression.

Decompression occurs only when pressure-boundary integrity is also compromised.

Conversely, a hull breach can cause decompression without severing the structural graph.

## 36. Structural Failure and Navigation

If structural changes make a previously valid route unavailable:

- player navigation reflects physical geometry;
- crew/robot pathfinding must recalculate;
- unreachable assignments cannot continue normally.

Detailed AI response belongs to crew/robot specifications.

## 37. Protected Command Core

The Command Core cannot be deconstructed through normal construction tools.

It may be damaged according to later damage rules but routine play does not permit its permanent deletion.

## 38. No Arbitrary Floating Construction

A permanent module cannot be placed in empty space and later connected unless the blueprint dependency system defines a valid future structural path from existing station structure.

Construction planning may preview future branches, but the physical build order must create valid connectivity.

## 39. Symmetry and Cosmetic Alignment

The game may provide construction aids such as:

- mirroring previews;
- alignment lines;
- repeated spacing;
- copy-planning.

These are planning conveniences.

They do not create new structural rules.

## 40. Structural Identity and Save State

The save state must preserve:

- module transforms;
- port connections;
- structural graph state;
- structural damage;
- compartment-relevant boundaries;
- blocked or broken connections.

Reconstruction after loading must be deterministic.

## 41. Visual Feedback

Construction and diagnostic views must make it possible to inspect:

- structural connections;
- disconnected branches;
- incompatible ports;
- dependent branches;
- damaged structural links.

Normal first-person presentation should also visually distinguish severe structural damage.

## 42. Audio Feedback

Critical structural failure uses clear impact, tearing, alarm, or system-report audio appropriate to the event.

Audio must not be the sole communication method for structural state.

## 43. Explicit Non-Goals

The structural system does not require:

- real-time finite-element stress analysis;
- continuous deformation simulation of the whole station;
- arbitrary curved free-form player geometry;
- every cosmetic prop carrying structural load;
- full realistic orbital simulation of disconnected station branches.

## 44. Tuneable Parameters

Tuneable values may include:

- durability;
- damage thresholds;
- minimum attachment counts for authored modules;
- repair costs;
- repair duration.

The connection rules themselves are fixed design behavior.

## 45. Dependencies

This document depends on:

- [Home Station Overview](overview.md);
- [Station Construction](construction.md);
- `damage_and_repairs.md`;
- `atmosphere_and_oxygen.md`;
- `../player/movement.md`;
- `../crew/`;
- `../robots/`;
- `../spacecraft/docking_and_hangars.md`.

## 46. Open Questions

None in the current structural-rule baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
