# Visibility, Culling, Batching, and Transparency

> **Status:** Architecture Complete  
> **Authority:** Render visibility construction, frustum/distance/LOD culling, occlusion policy, instancing/batching, draw ordering, transparency sorting, and relationship to TA-3 streaming/spatial systems

## 1. Purpose

Rendering must scale to dense station interiors, modular structures, spacecraft exteriors, and bounded mission zones without allowing performance structures to become gameplay authority.

## 2. Visibility Inputs

The renderer receives immutable candidate render records from the active SceneInstance/RenderSnapshot.

Candidate presence is already constrained by:

- current SceneGeneration;
- stream-cell residency;
- logical scene projection;
- presentation knowledge/visibility rules where overlays are involved.

The renderer then applies presentation-only visibility optimization.

## 3. Visibility Pipeline

Canonical world-geometry visibility sequence:

1. scene/residency eligibility;
2. render-layer/filter eligibility;
3. camera frustum test;
4. distance/LOD policy;
5. optional occlusion rejection;
6. material/pass classification;
7. batching/instancing;
8. stable draw ordering.

No culling result changes gameplay simulation.

## 4. Bounds

Renderable objects expose conservative local/world bounds suitable for culling.

Baseline bounds:

- sphere and/or AABB for cheap tests;
- more precise bounds only where required for large/irregular objects.

Bounds are presentation metadata and do not replace physics collision shapes.

## 5. Frustum Culling

Every normal world pass uses CPU-side frustum culling against the active `ViewDescriptor` as the first broad rejection stage.

Large static batches/cells are tested hierarchically before individual child objects where possible.

## 6. TA-3 Spatial Separation

TA-3 gameplay spatial grid is not reused as the sole renderer visibility structure.

Renderer may consume scene-cell/group metadata derived from TA-3, but owns its own render-oriented bounds and visibility lists.

## 7. Streaming Residency vs Visibility

A resident stream cell can be completely outside the camera and not drawn.

A visible region that is not resident cannot be fabricated by renderer; TA-3 Hard Streaming Hold/loading policy handles required content.

Renderer culling never requests gameplay activation directly.

## 8. Distance Culling

Purely decorative objects/effects can have authored/render-quality maximum draw distances.

Gameplay-critical visible geometry such as:

- doors;
- cover;
- hazards;
- active projectiles;
- interactables;
- objective-critical structures

cannot disappear at a distance where their absence would misrepresent collision or gameplay state.

## 9. Level of Detail

Renderable content can provide discrete mesh LODs.

LOD selection uses projected size/distance with hysteresis to prevent rapid oscillation.

LOD changes alter presentation only.

Collision/nav/gameplay geometry remains independently authoritative.

## 10. LOD Policy

Baseline content target supports:

- LOD0 high detail;
- LOD1 medium;
- LOD2 low;
- optional impostor/billboard only for content categories explicitly supporting it.

Assets are not required to have every LOD if their scale/use does not warrant it.

## 11. LOD Hysteresis

Switching thresholds use hysteresis or temporal stability so an object near one threshold does not alternate every frame.

Exact screen-size thresholds belong to TA-10/content and graphics quality data.

## 12. Occlusion Culling

Baseline renderer supports conservative occlusion culling as an optimization but does not require it for first frame correctness.

Architecture target uses previous-frame hierarchical depth (Hi-Z/HZB) or equivalent GPU/CPU-assisted conservative visibility history after the core deferred path is stable.

False-positive visible draws are acceptable; false-negative hiding of actually visible geometry is not.

## 13. Occlusion Warm-Up / Camera Cuts

After:

- scene activation;
- camera hard cut;
- large OriginEpoch discontinuity;
- resize/reprojection invalidation

historical occlusion rejection is disabled/reset until valid history exists.

## 14. Portal/Room Culling

Interior scenes may use authored compartment/portal visibility hints for performance.

These are optional acceleration data, not authoritative door/pressure topology.

A visually open path cannot be hidden merely because stale portal metadata says closed.

## 15. Static Batching

Static scene geometry sharing compatible mesh/material/state can be grouped into persistent scene render batches.

Static batching must preserve:

- required per-object transforms/IDs where overlays/damage state need them;
- independent state changes for gameplay-relevant objects;
- stream-cell unload ownership.

Objects expected to change independently are not baked irreversibly into one monolithic batch.

## 16. Instancing

Repeated compatible geometry uses hardware instancing where profitable.

Typical candidates:

- station structural modules;
- repeated props;
- rocks/debris;
- robot-class visuals;
- vegetation/industrial clutter where used.

Instancing groups by compatible:

- mesh;
- shader variant;
- material/resource bindings;
- render state/pass.

## 17. Instance Data

Per-instance GPU data can include:

- transform;
- previous transform if required by a feature;
- material index/override;
- object flags;
- render/picking/debug identifier where needed.

Persistent gameplay IDs are not required in every GPU instance record.

## 18. Dynamic Batch Construction

Visible dynamic draws are sorted/grouped after culling.

Opaque ordering prioritizes state/material locality and front-to-back depth where beneficial.

Sorting is deterministic for equivalent keys by using stable technical tie-breakers rather than pointer address.

## 19. Opaque Ordering

Opaque/AlphaMasked geometry can be ordered by a composite key such as:

1. render pass/shader variant;
2. material/resource group;
3. mesh/VAO;
4. approximate front-to-back depth;
5. stable render-instance key.

Exact bit packing is implementation-lock detail.

## 20. Transparent Ordering

AlphaBlend transparent objects use back-to-front ordering based on view-space sort depth.

Stable tie-breaker uses a render-instance/local stable key, never memory address.

For large transparent meshes, content authors should avoid geometry that requires impossible object-level sorting; specialized material/mesh splitting is preferred.

## 21. Additive Ordering

Pure additive particles/effects can render without strict back-to-front sorting where mathematically/order visually equivalent.

They still depth-test against opaque depth as appropriate.

## 22. Refractive Ordering

Refractive/screen-distortion surfaces render in a declared forward-special stage with access to the required pre-refraction scene color/depth.

They cannot sample final UI or reveal hidden geometry.

## 23. Shields

Shield surfaces are classified separately enough to preserve:

- impact visibility;
- coverage geometry;
- transparent depth interaction;
- accessibility reduction.

Shield rendering has no effect on actual shield collision/damage.

## 24. Decals

Decals are culled by volume/bounds and can be batched by material.

Persistent scorch/damage decals are reconstructed from content/persistent damage projection; transient decals can be render-only.

## 25. Skinned Meshes

Animated characters/robots use GPU skinning in the baseline renderer.

Skinning data is uploaded per visible animated instance/skeleton according to TA-7/TA-10 runtime animation architecture later.

CPU skinning is not the default shipping path.

## 26. Visibility and Knowledge

Normal physically visible world geometry is rendered from scene visibility.

Information overlays/highlights use a separate filtered data path respecting Presentation Knowledge Boundary.

Renderer must not infer hostility/objective state from hidden domain data just because the mesh is resident.

## 27. Critical Effects Floor

Graphics-quality culling can reduce decorative complexity, but gameplay-critical visual telegraphs retain an explicit minimum representation.

Examples:

- incoming projectile cue;
- hazard boundary;
- shield break;
- breach/decompression cue;
- objective interaction indicator.

## 28. Particle Culling

Particle systems use distance/frustum/effect-priority budgets.

Low-priority decorative particles are culled/reduced before high-priority gameplay telegraphs.

## 29. Draw Indirect Boundary

Multi-draw indirect/indirect command buffers are permitted optimizations under OpenGL 4.6 when profiling justifies them.

They are not required for correctness and cannot become a gameplay dependency.

## 30. GPU-Driven Culling Boundary

Fully GPU-driven scene submission is not required in baseline TA-4.

Initial architecture keeps authoritative render-list construction understandable on CPU with selected GPU compute assistance such as tiled light culling/particles/optional occlusion.

This preserves debuggability during the first custom renderer implementation.

## 31. Visibility Debugging

Development tools visualize:

- frustum;
- object/cell bounds;
- LOD level;
- culled-by reason;
- batch/instance group;
- transparent sort order;
- occlusion-history state.

## 32. Performance Metrics

Renderer records at minimum:

- submitted candidates;
- frustum culled;
- distance/LOD culled;
- occlusion culled where enabled;
- draw calls;
- instances;
- triangles/primitives;
- transparent draws;
- shadow draws.

## 33. Failure Safety

Culling metadata failure should prefer drawing too much over hiding required geometry.

A missing LOD falls back to another valid LOD rather than making the object invisible.

## 34. Test Requirements

Tests/validation must cover:

- deterministic stable sort keys;
- LOD hysteresis;
- camera-cut occlusion reset;
- resident-but-invisible separation;
- visibility cannot change gameplay activation;
- critical effects remain under Low/Reduced Effects;
- transparent back-to-front ordering;
- static batch preserves independently mutable gameplay visuals.

## 35. Explicit Non-Goals

No baseline requirement for:

- universal GPU-driven rendering;
- mandatory occlusion culling for correctness;
- rendering all resident cells;
- one renderer spatial structure reused by physics/nav/gameplay;
- OIT for every transparent surface;
- batching that destroys object-state independence.

## 36. Dependencies

Depends on TA-3 streaming/SceneInstance/spatial boundaries, TA-4 camera/render passes/materials, later TA-7 runtime entities and TA-10 asset LOD/mesh processing, plus GDS-13 readability/accessibility.

## 37. Open Questions

None in the visibility/culling/batching architecture.
