# TA-10 — Collision, Navigation, and Terrain Cooking

> **Status:** Architecture Complete  
> **Authority:** Offline collision derivation, collision-material bindings, grounded navigation cooking, free-flight navigation products, traversal annotations, bounded terrain cooking, static/dynamic geometry rules, and gameplay-safe runtime instantiation boundaries

## 1. Purpose

Rendered geometry, physics collision and navigation often originate from related source assets but are not interchangeable runtime authorities.

TA-10 therefore creates explicit, separately versioned cooked products for:

- collision;
- grounded navigation;
- free-flight navigation;
- terrain render/collision/nav products.

A visible mesh never automatically becomes authoritative collision or navigation merely because it exists.

## 2. Collision Product

A cooked Collision Content asset is a StarForge-owned project format containing only the geometry/material/shape information required by the TA-5 physics adapter.

It can describe:

- primitive shapes;
- convex hulls;
- convex compounds;
- static triangle-mesh collision;
- heightfield collision where approved;
- hit/query helper geometry where a gameplay schema explicitly requires it.

Raw Jolt runtime pointers/IDs are never serialized into content.

## 3. Collision Source Selection

Collision geometry is selected explicitly by an import/scene definition.

Allowed sources include:

- dedicated glTF collision mesh/node;
- deterministic primitive definition;
- generated convex hull from declared source mesh;
- terrain heightfield;
- explicit authored compound-shape definition.

The cooker does not infer collision from object names or render-material names alone.

## 4. Static vs Dynamic Collision

Baseline rules:

- large world/station/terrain geometry can use static triangle-mesh collision;
- movable dynamic props/actors should use primitive/convex/compound collision suitable for TA-5/Jolt behavior;
- dynamic concave triangle meshes are rejected unless a later explicitly reviewed physics use case supports them;
- visual-only geometry can intentionally have no collision.

## 5. Collision Simplification

Collision geometry should be simpler than render geometry where possible.

Simplification can use:

- authored proxy geometry;
- deterministic convex decomposition/hull generation if a pinned implementation is added;
- explicit primitive approximations.

A generic render LOD is not automatically valid collision geometry.

## 6. Collision Material Binding

Every relevant collision surface resolves an explicit `CollisionMaterialId` as required by TA-5.

The binding can be authored:

- per primitive;
- per collision submesh/group;
- through an explicit scene/collision definition.

Collision material is independent from TA-4 visual Material ContentId.

## 7. CollisionMaterial Definition

A CollisionMaterial content definition can include the TA-5 semantic fields such as:

- penetration resistance;
- ricochet behavior metadata;
- friction/restitution defaults;
- footstep/presentation surface category;
- contact/hazard tags where the owning gameplay design permits them.

TA-10 validates values and identity; combat/physics domains remain authority for behavior.

## 8. Physics Backend Boundary

Version-1 cooked collision stores project-owned geometry/shape descriptors rather than opaque raw Jolt object memory.

At runtime the TA-5 adapter instantiates Jolt shapes from the validated cooked product.

Reasons:

- keeps backend lifetime explicit;
- avoids serializing C++ memory layout;
- avoids tying persistent content identity to Jolt object IDs;
- allows a Jolt version upgrade to recook/instantiate cleanly.

Backend-specific acceleration data may be added later as a version-tagged optional optimization, never as the sole semantic geometry record.

## 9. Ground Navigation Cook

Static grounded navigation is cooked offline using pinned Recast behind the StarForge navigation-content tool.

Inputs include:

- approved collision/walkable source geometry;
- nav class parameters;
- authored area metadata;
- traversal-link anchors;
- exclusion/override volumes;
- scene/tile bounds.

Output is one or more versioned GroundNavigation Content products.

## 10. Ground Navigation Classes

The cook produces the TA-8 baseline classes:

- Small;
- Standard;
- Heavy.

A project can share underlying source geometry while cooking different clearance/radius profiles.

Exact agent dimensions are content/profile data validated against TA-8 movement envelopes.

## 11. Nav Tile Representation

Ground-navigation output is tiled.

Each cooked tile has stable content-local identity metadata and can contain a version-tagged Detour-compatible payload plus StarForge-owned header/annotations.

Raw `dtPolyRef` values are runtime/backend-local and are not ContentIds or save identity.

A Detour format/version mismatch produces recook/load failure rather than silent reinterpretation.

## 12. Dynamic Doors / Airlocks

Door/airlock open/closed/security state is **not permanently baked** into static navmesh reachability.

Static cook represents the potential spatial connectivity, while TA-8 runtime traversal links and current gameplay state determine whether an actor may use it.

A currently locked door therefore remains a semantic link/blocker, not a reason to generate a different source navmesh file for each security state.

## 13. Traversal Links

Authored traversal links are explicit structured content with:

- stable scene/content-local key;
- link kind;
- endpoints;
- compatible nav classes;
- required capabilities;
- owning gameplay object/anchor reference;
- static geometry validity;
- initial/default state metadata.

Kinds align with TA-8:

- Door;
- Airlock;
- Elevator/Lift;
- Ladder;
- approved Jump/Mantle;
- Breach Passage;
- Dock/Boarding;
- Magnetic-Boot transfer;
- FreeFlight portal.

## 14. Navigation Annotations

Cooked navigation/scene products can include explicit annotations such as:

- cover anchors;
- work positions;
- interaction positions;
- formation/holding areas;
- patrol/search anchors;
- reinforcement insertion anchors;
- landing/docking approach anchors.

Annotation presence does not itself grant permission or trigger AI behavior.

TA-8 AI selects/uses them according to current knowledge, task and capability.

## 15. Nav Validation Against Collision

Every cooked grounded nav tile must be consistent with the collision/scene geometry revision it was built from.

The build dependency graph fingerprints that relation.

A changed collision source invalidates dependent nav content automatically.

## 16. Free-Flight Navigation Cook

Flying/Zero-G autonomous actors use the project-owned TA-8 bounded 3D free-space representation.

TA-10 cooks:

- free-space cells/volumes;
- portals/connectivity;
- clearance data;
- semantic access points;
- static obstruction relation;
- authored traversal boundaries.

This product is project-owned; Recast/Detour is not stretched into a fake 3D volume system.

## 17. Free-Flight Generation

Generation can use deterministic voxelization/spatial sampling of approved static geometry plus authored overrides.

The algorithm/tool version and cell/clearance parameters are part of the content fingerprint.

The resulting runtime graph must not depend on worker order.

## 18. Hazard / Security Boundary

Static nav cook can store authored area categories but **current** hazards/security remain runtime overlays under TA-8.

Examples:

- a corridor is structurally traversable in content;
- current fire/vacuum can make it high-cost/blocked for one actor;
- current security can deny a door;
- none of those states require recooking canonical nav content.

Persistent topology destruction/construction can invalidate/rebuild affected runtime navigation through TA-6/TA-8 rules.

## 19. Terrain Definition

A Terrain Content asset represents one bounded surface-zone terrain set.

It declares:

- local bounds;
- tile grid/layout;
- height source;
- meters-per-sample;
- vertical scale/reference;
- render material layers/masks;
- collision policy;
- grounded-nav cook policy;
- stream-cell mapping;
- optional procedural placement sockets/regions.

## 20. Bounded Terrain

Terrain is finite and tied to a Mission/World Zone definition.

The cooker rejects content that relies on an infinite implicit extension beyond the declared zone.

Distant presentation outside gameplay-active bounds can use separate nonauthoritative backdrop content.

## 21. Terrain Tile Identity

Terrain tiles have deterministic content-local coordinates/keys derived from the Terrain Content asset and declared grid.

They are not new persistent gameplay IDs.

Persistent harvested resources/POIs/world changes reference their own gameplay identity/state rather than relying on a terrain tile file name.

## 22. Terrain Render Cook

The cooker can generate render products per terrain tile, including:

- vertex/index geometry or validated heightfield representation;
- normal/tangent data;
- material-layer weights;
- render LODs;
- bounds.

TA-4 selects visual LOD at runtime.

## 23. Terrain Collision Cook

Authoritative surface collision derives from the canonical terrain height/source at an approved precision independent from current render LOD.

Reducing visual LOD cannot alter where the player/AI physically stands.

## 24. Terrain Navigation Cook

Grounded nav derives from authoritative terrain collision/source plus authored exclusions/links.

A distant visual-only terrain tile does not automatically become nav-active.

## 25. Procedural Placement Boundary

Terrain/scene content can define compatible placement regions/sockets/tags for TA-9 procedural generation.

The cooker validates geometry/nav compatibility of those regions.

TA-9 still chooses actual persistent/generated POIs, enemies, resources and mission identities using its deterministic generator.

## 26. Mission Feasibility Metadata

To support TA-9 pre-commit validation, cooked scene/procedural modules expose machine-readable feasibility metadata such as:

- compatible nav classes;
- required traversal link types;
- docking/landing interfaces;
- cargo-clearance class;
- pressure/environment category;
- mandatory anchor availability;
- extraction-compatible exits;
- local bounds;
- module sockets/portals.

The metadata summarizes validated geometry; it does not replace runtime state checks.

## 27. Static Scene Cook Dependency

A scene that references:

- mesh A;
- collision B;
- ground nav C;
- free-flight nav D;
- terrain E

records explicit build dependencies on those ContentIds/fingerprints.

Changing B invalidates C and dependent scene products where required.

## 28. Cooked Product Versioning

Collision/Nav/Terrain products each carry their own format version.

The runtime validates version compatibility before instantiation.

A newer cooker may require recooking old build output; source ContentId remains stable.

## 29. Runtime Loading

Shipping runtime loads only cooked collision/navigation/terrain products from the Content Registry.

It does not:

- invoke Recast to rebuild ordinary static nav every scene load;
- parse render glTF as physics truth;
- inspect filenames for collision;
- generate infinite terrain.

Dynamic nav/topology changes remain bounded runtime operations under TA-6/TA-8.

## 30. Required Content Failure

Missing or corrupt mandatory collision/navigation content blocks safe scene activation under TA-3.

The runtime may not substitute:

- no collision;
- a generic box;
- empty navmesh

for a required gameplay surface and continue as if valid.

Optional navigation for purely decorative autonomous actors can be explicitly optional only when scene correctness does not depend on it.

## 31. Diagnostics

Cook diagnostics identify:

- source ContentId/path;
- product type;
- invalid triangle/shape/link/tile;
- expected nav class;
- collision material binding;
- terrain tile coordinate;
- dependency fingerprint;
- backend cook version where relevant.

## 32. Tests

Required tests cover:

- render mesh does not imply collision automatically;
- static triangle vs dynamic convex policy;
- CollisionMaterialId binding validation;
- deterministic primitive/convex cook;
- nav tile deterministic generation;
- collision change invalidates dependent nav;
- door/security state remains runtime link state;
- Small/Standard/Heavy cook separation;
- free-flight cell/portal determinism;
- terrain render LOD does not alter collision;
- terrain collision/nav alignment;
- bounded zone enforcement;
- mission feasibility metadata matches actual cooked anchors/interfaces;
- missing required collision/nav blocks scene readiness;
- raw backend IDs never serialize as content identity.

## 33. Explicit Non-Goals

No baseline requirement for:

- visible render mesh automatically serving as final collision;
- dynamic arbitrary concave triangle bodies;
- one universal nav representation for grounded and 3D flight;
- security/hazard state permanently baked into navmesh;
- runtime infinite terrain generation;
- Jolt pointer/object serialization;
- Detour polygon references in saves;
- procedural mission generator owning collision/nav cooking.

## 34. Dependencies

Depends on TA-3 scene/terrain bounds, TA-5 collision/Jolt adapter, TA-6 topology mutation, TA-8 navigation/traversal, TA-9 procedural feasibility validation, and TA-10 glTF/registry/dependency pipeline.

## 35. Open Questions

None in the collision/navigation/terrain cooking contract.
