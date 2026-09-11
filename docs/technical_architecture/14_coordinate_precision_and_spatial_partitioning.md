# Coordinate Precision and Spatial Partitioning

> **Status:** Architecture Complete  
> **Authority:** Coordinate spaces, units, persistent/local transforms, floating-origin strategy, precision boundaries, spatial indices, bounds validation, and cross-system coordinate conversion

## 1. Purpose

StarForge contains player-scale interiors, planetary mission zones, EVA spaces, and much larger local spaceflight volumes.

A single raw `float3` world coordinate measured from one permanent galaxy origin would eventually lose useful precision and would contradict the strategic-graph world model.

This document defines the technical coordinate strategy.

## 2. Strategic Space Is Topological

The galaxy is represented primarily as persistent graph identity and route topology.

Strategic travel does not require one continuous galaxy Cartesian coordinate system.

A `StrategicLocationId`/equivalent identifies the node; authored map coordinates used for UI layout are presentation data, not local physics truth.

## 3. Local Context Coordinate Space

Every Active Local Context defines one **Context Space**.

Context Space uses:

- right-handed coordinates;
- meters as the canonical distance unit;
- +Y as the canonical local up axis for authored ground/interior spaces;
- explicit gravity vectors where local gameplay is not aligned to +Y;
- normalized quaternions for orientation.

No gameplay rule depends on a renderer-specific camera forward convention.

## 4. Persistent Local Position

Persistent/local-context positions are represented at architecture level as double-precision meter coordinates:

`LocalPosition64 = glm::dvec3` or an equivalent StarForge-owned wrapper.

The owning Zone/Location identity is always required to interpret the coordinate.

A position without its coordinate-space owner is not a complete persistent location.

## 5. Persistent Transform

A persistent transform contains conceptually:

- owning Zone/Location/Context identity;
- `LocalPosition64`;
- normalized rotation quaternion;
- optional scale only for content/object classes that permit it.

Velocity/angular velocity are stored by the owning gameplay domain where required, not hidden inside a generic transform blob.

## 6. Why Double Precision Is Used

Double precision protects persistent/local spatial relationships across large bounded spaceflight zones and repeated origin shifts.

It does not imply that every physics/render calculation must use double precision.

## 7. Runtime Origin-Relative Space

High-frequency rendering and physics use an origin-relative local coordinate frame.

Conceptually:

`RuntimePosition = LocalPosition64 - RuntimeOrigin64`

The result is converted to the backend's validated working precision.

## 8. RuntimeOrigin64

Each Active Local Context owns:

- `RuntimeOrigin64`;
- `OriginEpoch`.

`RuntimeOrigin64` is expressed in Context Space.

The origin can move through explicit **Origin Rebase** operations.

## 9. Origin Rebase Trigger

A rebase is scheduled when the active reference anchor moves beyond a tuneable precision threshold from RuntimeOrigin64.

The reference anchor is normally:

- player position for on-foot/interior/surface/EVA contexts;
- player-controlled ship position for local spaceflight.

Exact threshold is chosen in TA-13 based on backend precision/performance tests.

## 10. Rebase Commit Boundary

An Origin Rebase occurs only at a Stable Simulation Boundary when:

- physics is not mid-step;
- no coordinate-sensitive transaction is half committed;
- active runtime iterators/jobs cannot observe mixed origins.

## 11. Rebase Semantics

A rebase changes only the technical origin representation.

It does not change authoritative:

- persistent positions;
- distances;
- velocities;
- gameplay timers;
- ownership;
- mission state;
- damage;
- objective progress.

## 12. OriginEpoch

Every rebase increments `OriginEpoch`.

Asynchronous jobs and cached runtime spatial data that depend on origin-relative coordinates carry the epoch.

A result from an old OriginEpoch is rejected or transformed only if its contract explicitly supports conversion.

## 13. Physics Coordinates

TA-5 will define the exact Jolt precision configuration and adapter API.

TA-3 requires only that:

- physics coordinates remain sufficiently close to zero through RuntimeOrigin64;
- physics origin shifts are applied atomically with scene/runtime coordinates;
- physics backend coordinates never become persistent save identity.

## 14. Render Coordinates

Rendering consumes camera/origin-relative float-friendly transforms.

The renderer should not multiply giant galaxy-scale world matrices merely to draw local objects.

TA-4 owns exact render transform packing.

## 15. Audio Coordinates

Spatial audio positions use the same current origin-relative local frame as active presentation.

Audio backend position is technical state and is not serialized.

## 16. Navigation Coordinates

Navigation data is scoped to the active/local zone and can use origin-relative or cell-local coordinates.

Nav results carry SceneGeneration/OriginEpoch or a stable local-space representation so stale results cannot be applied after rebase/scene change.

TA-8 finalizes this boundary.

## 17. Static Cell-Local Geometry

Static streamed geometry can be authored/stored relative to a cell/local anchor.

At runtime it is transformed into current origin-relative coordinates.

This avoids baking one enormous coordinate into every vertex.

## 18. Zone Bounds

Every Zone/Context definition declares validated local bounds appropriate to its category.

Bounds can be:

- axis-aligned volume;
- convex/portal-defined region;
- authored flight volume;
- tile extent plus allowed vertical range.

The exact gameplay dimensions remain GDS/content data.

## 19. Coordinate Range Validation

Content/procedural validation rejects geometry or spawn coordinates outside the owning Zone's supported bounds/profile.

Technical representation must not silently clamp invalid gameplay positions.

## 20. Local Spaceflight Bounds

Local Spaceflight Zones can be substantially larger than on-foot zones.

They remain bounded and use floating-origin/runtime-relative coordinates.

Crossing an authored exit boundary invokes an explicit transition/navigation rule rather than continuing indefinitely.

## 21. Interior Coordinates

Interiors can use one Context Space even across multiple streamed cells.

A room/cell can have a local transform relative to the context root for compact authored data.

## 22. Moving Parent Structures

An interior hosted by a moving ship/station section uses an explicit reference-frame transform chain.

Gameplay position is not encoded by mutating every child persistent position each frame.

TA-5/TA-7 will define moving-platform/character integration.

## 23. Reference Frame Types

Baseline technical spatial references distinguish:

- Context Space;
- StreamCell Local Space;
- Runtime Origin-Relative Space;
- Object Local Space;
- Presentation/View Space.

Conversions are explicit functions/types where practical.

## 24. No Untyped `vec3` Across Boundaries

Architecture APIs that cross domain/back-end boundaries should use named wrappers/structures when coordinate-space ambiguity would be dangerous.

Examples:

- `ContextPosition`;
- `RuntimePosition`;
- `CellLocalPosition`.

Raw GLM vectors can remain implementation details inside a clearly scoped module.

## 25. Gameplay Spatial Query Index

The active Scene owns a StarForge **GameplaySpatialIndex** for non-authoritative acceleration of queries such as:

- nearby interactables;
- nearby AI candidates;
- streaming interest helpers;
- audio emitter lookup;
- gameplay broad candidate sets.

It does not replace physics collision or render culling structures.

## 26. Spatial Index Structure

Baseline architecture uses a 3D loose hashed grid for dynamic/queryable runtime objects.

Properties:

- cell size configured per context profile;
- entries keyed by RuntimeEntityHandle plus query/category mask;
- large objects can occupy multiple grid cells or a dedicated oversized list;
- insertion/removal/update occurs on the simulation thread.

## 27. Why a Loose Hashed Grid

The active contexts are bounded but can differ greatly in size.

A loose hashed grid provides:

- simple incremental updates;
- good nearby-query behavior;
- no requirement for one giant fixed dense array;
- straightforward diagnostics.

TA-13 can replace/tune implementation details if profiling preserves API semantics.

## 28. Static Spatial Acceleration

Static geometry acceleration is backend/domain-specific:

- physics broadphase/static bodies handle collision;
- renderer owns render culling structures;
- navigation owns nav acceleration;
- cell content can include authored/static BVHs where useful.

No universal spatial tree is forced on every subsystem.

## 29. Streaming Cells vs Spatial Grid

Streaming cells and GameplaySpatialIndex cells are intentionally distinct abstractions.

An object can move across many spatial-grid cells while remaining in one StreamCell, or vice versa.

## 30. Query Determinism

If a spatial query result affects authoritative gameplay, candidate ordering must be normalized deterministically before tie-sensitive logic.

Default tie-breakers can include:

1. distance using deterministic comparison policy;
2. typed Persistent ID where present;
3. RuntimeEntityHandle raw ordering only for purely transient equal-priority objects.

Hash bucket iteration order is never authoritative.

## 31. Rebase and Spatial Index

Origin Rebase cannot invalidate authoritative spatial relationships.

The spatial index either:

- stores Context Space/double anchors; or
- is rebuilt/shifted coherently at the rebase boundary.

The chosen implementation must be tested for equivalent query results before/after rebase.

## 32. Precision-Sensitive Interactions

Docking, hit detection, close-range character interaction, and breach/door alignment operate in origin-relative local precision near the active reference frame.

Large local context coordinates therefore do not degrade close-range precision simply because the strategic location is far from another star system.

## 33. Save/Load

Save DTOs store persistent/local coordinates in their owning domain format using stable Context/Zone identity plus double-precision coordinates where spatial persistence is required.

RuntimeOrigin64 and cached spatial-grid state are technical reconstruction data and do not need to be persisted as gameplay authority.

A save can restore with a different RuntimeOrigin64 while preserving identical gameplay positions.

## 34. Debugging

Development tools expose:

- Context Space origin/identity;
- RuntimeOrigin64;
- OriginEpoch;
- player/ship distance from origin;
- spatial-grid occupancy;
- oversized entries;
- zone bounds;
- coordinate-space conversions;
- rebase count/reasons.

## 35. Test Requirements

Tests cover:

- strategic location identity independent from Cartesian galaxy coordinates;
- save/load position equivalence under a different runtime origin;
- origin rebase preserves relative transforms/velocities;
- stale OriginEpoch job rejection;
- spatial query equivalence across rebase;
- deterministic candidate ordering;
- bounds validation;
- large local-space coordinates retain close-range interaction precision.

## 36. Explicit Non-Goals

The coordinate architecture does not require:

- one continuous galaxy Cartesian space;
- double precision in every GPU/physics operation;
- serialization of backend coordinates;
- one universal spatial tree;
- infinite local flight space;
- arbitrary free-form coordinate-system conventions per asset.

## 37. Dependencies

Depends on TA-2 identity/revisions, TA-3 Scene/Streaming, GDS Galaxy Structure/Mission Zones/Spacecraft, and later TA-4/TA-5/TA-8/TA-10/TA-13.

## 38. Open Questions

None in the TA-3 coordinate/precision/spatial-partitioning contract.
