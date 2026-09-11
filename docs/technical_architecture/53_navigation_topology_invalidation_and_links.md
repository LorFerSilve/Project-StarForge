# TA-8 — Navigation Topology Invalidation and Dynamic Links

> **Status:** Architecture Complete
> **Authority:** Navigation change detection, tile/link invalidation, rebuild staging, revision commit, and propagation from station/world topology changes into runtime navigation

## 1. Purpose

StarForge navigation must respond to doors, construction, destruction, breaches, docking, elevators, hazards, and streamed geometry without rebuilding the entire world or allowing stale paths to bypass new blockers.

## 2. Change Sources

Navigation-relevant changes can originate from:

- TA-3 scene/stream-cell activation;
- TA-5 collision geometry changes;
- TA-6 station construction/deconstruction;
- TA-6 structural failure/repair;
- door/airlock state;
- elevator availability;
- docking/boarding connections;
- authored destruction/breach state;
- hazard/security cost overlays;
- temporary tactical blockers;
- scripted mission/world changes.

## 3. Persistent-State-First Rule

Navigation never mutates before the owning gameplay state commits.

Canonical order:

```text
Owning gameplay/domain transaction commits
-> TA-5/scene geometry or logical traversal state updates
-> NavigationInvalidation record queued
-> navigation rebuild/overlay update staged
-> valid navigation revision commits
```

A speculative nav rebuild cannot become authority for whether a door was destroyed or a bridge exists.

## 4. Invalidation Types

Typed invalidation categories:

- `GeometryTileInvalidation`;
- `TraversalLinkStateInvalidation`;
- `AreaCostInvalidation`;
- `SecurityFilterInvalidation`;
- `FreeFlightCellInvalidation`;
- `AnnotationInvalidation`.

Each invalidation carries affected navigation space/region and source revision.

## 5. Cheap Dynamic Changes

Frequent state changes do not require navmesh recooking when they can be represented as dynamic overlays.

Examples:

- ordinary door Open/Closed/Locked state;
- security permission;
- temporary hazard cost;
- elevator available/unavailable;
- active docking connector;
- temporary area avoidance.

These update link/filter/area state and increment `NavigationRevision` as required.

## 6. Geometry Rebuild Changes

Changes that alter walkable geometry/clearance can invalidate one or more nav tiles.

Examples:

- new station module completed;
- module/bridge destroyed;
- large debris permanently blocks passage;
- breach creates authored traversable opening;
- repaired structure restores floor/bridge;
- mission geometry changes state.

Only bounded affected tiles/cells are rebuilt.

## 7. Tile Ownership

A grounded nav tile is identified by a project-owned stable tile coordinate/key scoped to one `NavigationSpace` and nav class.

Raw Recast/Detour tile memory address/reference is not stable identity.

## 8. Rebuild Staging

Geometry rebuild follows:

1. persistent/scene geometry change commits;
2. affected tile keys identified;
3. immutable navigation-build input snapshot produced;
4. worker can rebuild candidate tile(s);
5. worker result returns source SceneGeneration + GeometryRevision + request sequence;
6. main simulation validates freshness;
7. candidate is integrated at a safe navigation mutation boundary;
8. `NavigationRevision` increments;
9. affected active paths are marked for revalidation.

A stale rebuild result is discarded.

## 9. No Half-Rebuilt Navigation

Actors never query a nav structure in the middle of a tile replacement.

Tile integration is atomic from the perspective of the simulation thread.

If multiple adjacent tiles must change together for correctness, they are prepared as one `NavigationUpdateBatch` and committed together.

## 10. Existing Path Revalidation

A path corridor stores the `NavigationRevision` and relevant link-state revisions used to build it.

When those change:

- a path is not immediately assumed invalid everywhere;
- the next path-following validation checks remaining corridor/link compatibility;
- if affected segment remains valid, execution may continue;
- otherwise the actor requests a replan.

## 11. Door State Changes Mid-Path

If a door becomes locked/unsafe while an actor approaches:

- current path link becomes blocked for that actor;
- actor stops before invalid traversal boundary;
- AI chooses alternate route, valid door operation, authorized override, or Blocked state according to task rules.

No actor crosses because the path was planned earlier.

## 12. Door Transition During Crossing

Door/airlock/elevator crossing uses an explicit traversal-link execution state.

If the mechanism changes while the actor is physically in the threshold:

- TA-5 collision/safety interlock determines physically legal result;
- actor completes only a physically valid side transition;
- subsequent route is recalculated.

Navigation does not snap the actor through the closing mechanism.

## 13. Hazard Overlay Updates

Hazard knowledge/cost can change independently of geometry.

A hazard update records:

- affected area/cells;
- hazard type/severity;
- knowledge source where relevant;
- actor-profile cost/block policy input;
- revision.

Unknown hazard state is not inserted into an actor's knowledge-aware path cost until the actor/squad legitimately knows it, except where a universal physical blocker exists.

## 14. Security Overlay Updates

Security lock state and actor authorization are query-time filters.

Changing one crew member's authorization does not require navmesh recooking.

Changing a physical door from intact to destroyed may require link/geometry update because the physical traversal changed.

## 15. Construction Safety

A module under construction does not become traversable solely because future navigation geometry has been generated.

Traversability becomes available only after the construction milestone that the owning TA-6 state marks physically complete and the resulting navigation update commits.

## 16. Structural Collapse

If a path surface disappears:

- persistent structural state commits first;
- collision mutates through TA-5 deferred phase;
- nav tile becomes invalid;
- actors on/near the area resolve physical state through TA-5;
- AI then replans from the actual resulting position.

Navigation cannot keep an invisible bridge alive.

## 17. Docking Connections

A Hard Dock can create a temporary traversal connection between ship/station/local structures only when:

- logical Hard Dock is committed;
- physical attachment exists;
- pressure/access/door conditions allow traversal;
- compatible traversal link is enabled.

Undocking disables that link before actors can plan new cross-connector paths.

## 18. Streaming Interaction

A path may reference only navigation data guaranteed resident for the planned corridor or explicitly use a portal toward a stream boundary.

If required destination navigation is not resident in time, TA-3 Hard Streaming Hold can stop Simulation Time before crossing.

Streaming residency itself does not activate actors or grant traversal.

## 19. Free-Flight Cell Updates

3D free-flight navigation uses the same freshness model:

- solid volume change invalidates affected cells/edges;
- worker prepares candidate updates;
- main simulation validates SceneGeneration/GeometryRevision;
- atomic graph revision commits;
- active routes revalidate.

## 20. Deterministic Update Ordering

When multiple navigation mutations are ready on one tick, the simulation integrates them in stable order by:

1. NavigationSpace stable key;
2. update semantic priority;
3. affected tile/cell/link stable key;
4. request sequence.

Worker completion order is never semantic ordering.

## 21. Failure Handling

If navigation rebuild fails technically:

- the new/affected region is treated as navigation unavailable for autonomous traversal;
- current physical world remains authoritative;
- actor commands can become Blocked;
- diagnostics identify the failed tile/cell/update;
- no fallback path through raw collision geometry is invented.

## 22. Test Requirements

Tests cover:

- door lock mid-route;
- destroyed bridge/path invalidation;
- module construction enabling route only after commit;
- stale async tile result rejection;
- hazard/security overlay changes without full rebuild;
- Hard Dock link enable/disable;
- atomic multi-tile update;
- actor standing on collapsing geometry;
- stream boundary navigation;
- deterministic mutation ordering.

## 23. Explicit Non-Goals

No full-scene nav rebuild for every door toggle, no navigation-before-gameplay topology mutation, no stale worker integration, no invisible old geometry kept for path convenience, and no worker completion race as gameplay authority.

## 24. Dependencies

Depends on `52_navigation_representation_and_traversal_profiles.md`, TA-3 streaming/scene generation, TA-5 safe physics mutation, TA-6 topology mutation, and TA-7 runtime phase ordering.

## 25. Open Questions

None.