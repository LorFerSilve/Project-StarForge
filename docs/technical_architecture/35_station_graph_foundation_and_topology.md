# Station Graph Foundation and Topology

> **Status:** Architecture Complete  
> **Authority:** Persistent Horizon module topology, construction-lattice identity, structural/traversal connections, typed utility attachment metadata, graph revisions, derived-network invalidation, and TA-5 station-geometry handoff

## 1. Purpose

TA-6 needs one canonical representation of **what Horizon is physically made of** before individual utility systems can solve power, atmosphere, thermal, water, logistics, manufacturing, automation, or repair.

The foundation must preserve the GDS distinction between:

- physical structural attachment;
- traversal/accessibility;
- pressure boundaries;
- individual utility channels;
- equipment/function state;
- active-scene collision.

It must not collapse all station behavior into one universal graph.

## 2. Canonical Station Topology Store

Persistent Horizon topology is owned by a project-level `StationTopologyStore`.

It contains stable station-local records for:

- completed modules;
- construction blueprints;
- module ports;
- structural attachments;
- traversal links;
- pressure-boundary elements;
- utility-capable links;
- docking/external-mount relationships;
- persistent topology damage state.

TA-2 persistence rules apply. Backend physics IDs, render handles, navigation nodes, and derived solver objects are never canonical station identity.

## 3. Station-Local Typed IDs

Horizon uses stable typed station-local IDs where global persistent identity is unnecessary:

```text
StationModuleId
StationPortId
StructuralEdgeId
TraversalEdgeId
PressureCellId
PressurePortalId
UtilityLinkId
EquipmentSlotId
ConstructionBlueprintId
```

These are scoped to the persistent Horizon station record and are deterministic through save/load.

Globally meaningful actors/assets such as `CrewId`, `RobotId`, and `ShipId` remain TA-2 persistent IDs.

## 4. Module Record

A completed module record contains at minimum:

- `StationModuleId`;
- immutable `ContentId` for the authored module definition;
- construction-lattice transform;
- orientation;
- construction/commissioning state;
- structural integrity state reference;
- persistent functional/equipment references;
- stable authored port-instance mapping;
- current topology participation flags.

The module record does not duplicate subsystem state owned by Power, Atmosphere, Thermal, Water, Logistics, Manufacturing, or Damage.

## 5. Construction Lattice

Persistent module placement uses the GDS orthogonal 3D station construction lattice.

The topology store owns an occupancy index:

```text
ConstructionCellCoord -> StationModuleId / BlueprintId
```

The occupancy index is a derived/cacheable acceleration structure over canonical module/blueprint transforms. It is deterministically rebuildable from persistent records.

## 6. Authored Module Definition

A module `ContentId` resolves to immutable content containing at minimum:

- occupied lattice cells;
- allowed orientations;
- Structural Ports;
- Traversal Ports;
- pressure cells/boundaries/portals;
- power interfaces;
- thermal/coolant interfaces;
- water interfaces;
- logistics interfaces;
- control/data interfaces where relevant;
- external mounts;
- docking clearance volumes;
- maintenance/service clearance;
- collision-content references;
- commissioning requirements.

TA-10 later defines the concrete content schema/cooking pipeline. TA-6 fixes the runtime contract.

## 7. Structural Graph

The canonical Structural Graph is:

```text
Node = completed structural StationModuleId
Edge = valid StructuralEdgeId between compatible Structural Ports
Root = Command Core StationModuleId
```

A structural edge contains:

- endpoint module/port IDs;
- connection class;
- current physical state;
- authored utility-channel support mask;
- whether the connection also owns a traversal/pressure opening reference;
- damage reference.

## 8. Structural Validity

After any structural-topology mutation, TA-6 deterministically computes the connected component reachable from the Command Core.

Completed structural modules outside that component become `StructurallyDisconnected`.

The module remains persistent. It is not deleted, teleported, or converted into free resources.

## 9. Required Attachments

Module definitions can require:

- one compatible attachment;
- multiple simultaneous attachment points;
- Heavy connection class;
- dedicated supporting frame;
- authored combinations.

Commissioning/build-stage validation evaluates those requirements against the canonical structural graph.

## 10. Structural Redundancy

Multiple structural paths are represented as independent edges.

Loss of one edge does not disconnect a branch while another valid required path remains.

No hidden aggregate “station integrity score” replaces connectivity.

## 11. Traversal Graph

Traversal is a separate graph.

A `TraversalEdge` references authored traversable connectivity such as:

- open corridor connection;
- door;
- hatch;
- airlock route;
- ladder/shaft;
- lift/elevator path;
- maintenance access.

A structural edge does not imply a traversal edge.

## 12. Traversal Dynamic State

Traversal availability can depend on:

- door/hatch physical state;
- lock/security state;
- damage/jam state;
- pressure-safety interlock;
- elevator position/operability;
- current construction state.

TA-8 later consumes this state for navigation. TA-6 owns the station topology facts.

## 13. Pressure Geometry Foundation

Authored modules contribute stable `PressureCellId` instances and `PressurePortalId` instances.

A Pressure Cell represents one bounded gas-accounting volume with an authored geometric volume.

A Pressure Portal represents a possible gas-flow path between:

- two Pressure Cells; or
- a Pressure Cell and vacuum/exterior.

Portal state is derived from real boundary elements such as doors, hatches, breaches, ducts, and service penetrations.

## 14. Why Pressure Cells Are Stable

TA-6 does **not** destroy and recreate canonical gas ownership whenever a door opens or closes.

Each pressure cell keeps its own gas quantities. Portals control exchange between cells.

A higher-level UI/AI “compartment” can be derived from pressure-boundary connectivity without requiring gas duplication during split/merge operations.

## 15. Utility Link Foundation

Utility-capable structure exposes typed links.

Supported channel kinds include:

```text
Power
ControlData
Ventilation
Coolant
WaterFresh
WaterWaste
LogisticsStandard
LogisticsBulk
```

A physical connection can support several channel kinds, but each subsystem builds its own graph/solver view.

## 16. No Universal Utility Graph Solver

TA-6 explicitly rejects one generic “flow graph” for every station system.

The topology foundation supplies shared endpoint/connectivity facts, while:

- Power solves electrical capacity allocation;
- Atmosphere conserves gas through pressure portals/ventilation;
- Thermal tracks heat/coolant;
- Water tracks fresh/wastewater quantities;
- Logistics tracks discrete physical ownership/reservations/transfers.

These systems have different conserved quantities and failure semantics.

## 17. Topology Revisions

The station maintains monotonic revisions:

```text
StructuralTopologyRevision
TraversalTopologyRevision
PressureTopologyRevision
PowerTopologyRevision
ThermalTopologyRevision
WaterTopologyRevision
LogisticsTopologyRevision
```

A subsystem rebuilds/revalidates only when its relevant input revision changes.

## 18. Topology Mutation Set

Topology can change through committed gameplay facts such as:

- module construction stage reaching Structural Complete;
- commissioning enabling an interface;
- deconstruction;
- structural destruction;
- repair/reconstruction;
- door/hatch/valve/breaker state change;
- breach creation/sealing;
- utility-link damage;
- docking connection establishment/removal.

Every mutation occurs through TA-2 Command/Validate/Commit/Event rules.

## 19. Dependency Invalidation

A topology commit publishes typed invalidation facts.

Example:

```text
Structural edge Broken
↓
StructuralTopologyRevision++
↓
reachable structural set recomputed
↓
dependent physical utility links crossing that edge invalidated
↓
Power/Thermal/Water/Logistics topology revisions increment as applicable
↓
Atmosphere pressure-boundary revision increments only if pressure integrity also changed
```

Structural failure alone does not imply decompression.

## 20. Deterministic Rebuild Order

When one commit invalidates multiple station systems, derived rebuild order is:

1. structural connectivity;
2. pressure-boundary/traversal physical topology;
3. utility graph endpoint availability;
4. Power/Control availability;
5. Water/Thermal/Atmosphere/Logistics solver topology;
6. production/automation eligibility;
7. read models/alarms.

This ordering is architectural, not a claim that all subsystem dynamics are solved in one instant.

## 21. Disconnected Branches

A structurally disconnected branch keeps:

- its module records;
- local stored resources;
- local atmosphere;
- local power storage/generation;
- local water/coolant;
- local equipment state;
- damage/faults.

Cross-severed utility edges become unavailable.

Independent local networks can continue if their own requirements are satisfied.

## 22. Detached-Branch Physical Presentation

The GDS does not require full Newtonian simulation of a severed Horizon wing.

The persistent branch remains station-domain state with an authored emergency/detached presentation state.

TA-5 only receives active local collision geometry that TA-6 declares physically present in the current scene.

A future explicit design change would be required for free-flying whole-wing rigid-body simulation.

## 23. Construction Blueprint Topology

Blueprints exist in a Planning Graph distinct from completed operational topology.

A blueprint can depend on another blueprint for future structural support.

Planned edges do not become operational Structural/Utility edges before their required construction stage commits.

## 24. Structural Complete Boundary

When a blueprint reaches `Structural Complete`:

- its completed module identity is committed;
- structural geometry/topology can participate;
- authored pressure boundary elements can physically exist according to build definition;
- functional utility consumers/producers remain unavailable until their owning commissioning conditions pass.

## 25. Commissioning Boundary

Commissioning is a validation/transaction stage.

It can require current valid:

- structural support;
- traversal/service access;
- power/control;
- atmosphere;
- cooling;
- water/fluid;
- logistics;
- specialist setup.

Completion never fabricates missing infrastructure.

## 26. Deconstruction Planning

Before deconstruction commit, TA-6 computes dependent structural branches and affected utility topology.

If the target is a sole structural bridge, deconstruction either:

- rejects; or
- operates under an explicitly approved joint deconstruction plan.

The affected set is stable/revalidated before commit.

## 27. Occupant Safety

Construction/deconstruction validation consumes occupancy/read models for player, crew, robots, docked ships, pressure state, and traversal escape.

TA-6 does not directly move occupants to make an invalid operation succeed.

## 28. Active-Scene Geometry Handoff

TA-6 produces `StationGeometryDelta` records after committed topology/damage changes.

TA-5 consumes these only at its defined safe deferred-physics mutation phase.

Examples:

- add completed module static collision;
- remove deconstructed collision;
- switch door Kinematic state;
- create/remove breach collision;
- change destroyed structural obstacle state.

TA-6 never calls Jolt directly.

## 29. Render Handoff

TA-6 exposes station state through immutable TA-2/TA-4 read/snapshot projection.

Renderer geometry/presentation is not station-state authority.

## 30. Save / Load

Canonical topology persistence includes:

- modules/blueprints;
- transforms/orientations;
- port/edge state;
- damage references;
- pressure-cell/portal state references;
- utility-link persistent state;
- topology revisions or deterministically reconstructable equivalents.

Derived adjacency maps, occupancy caches, connected components, and solver workspaces can be rebuilt during staging load.

## 31. Validation on Load

Staging validation checks at minimum:

- unique station-local IDs;
- module ContentId validity;
- lattice occupancy consistency;
- endpoint existence;
- compatible structural ports;
- Command Core uniqueness/root validity;
- no impossible completed-module overlap;
- pressure-cell/portal endpoint validity;
- utility-link endpoint validity;
- blueprint dependency validity.

Failure prevents live-session replacement under TA-2 all-or-nothing loading.

## 32. Debugging

Development tools must visualize:

- construction lattice occupancy;
- Structural Graph;
- Command Core reachable set;
- Traversal Graph;
- pressure cells/portals;
- utility channel links;
- topology revisions;
- invalidation causes;
- disconnected branches.

## 33. Explicit Non-Goals

The topology foundation does not implement:

- finite-element structural mechanics;
- unrestricted voxel/freeform station building;
- one universal utility solver;
- full free-flight simulation of detached station branches;
- navigation pathfinding;
- rendering ownership;
- direct damage formulas.

## 34. Dependencies

Depends on TA-2 state/transaction contracts, TA-3 scene/world projection, TA-5 station collision handoff, GDS Station Construction/Structural Rules, Spacecraft docking, and Damage/Repair.

## 35. Open Questions

None within the TA-6 station-topology foundation.
