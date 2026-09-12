# TA-10 — glTF Mesh, Scene, Skeleton, and Animation Import

> **Status:** Architecture Complete  
> **Authority:** glTF 2.0 import, mesh conversion, vertex/index formats, skin/skeleton/animation extraction, LOD/mesh optimization, semantic sockets, transform validation, and render-vs-gameplay metadata boundaries

## 1. Purpose

StarForge uses glTF 2.0 as its canonical 3D exchange format, but runtime systems must not directly depend on raw glTF DOM objects or DCC naming conventions.

This specification defines the deterministic import/cook path from glTF source into StarForge-owned mesh/scene/skeleton/animation products.

## 2. Parser Boundary

`fastgltf` is the canonical glTF 2.0 parser selected by TA-1.

The content cooker owns:

- file discovery;
- parser configuration;
- validation;
- semantic interpretation;
- coordinate/unit checks;
- StarForge runtime conversion;
- diagnostics.

`fastgltf` objects are temporary build-time parse representation and never serialized as StarForge runtime or save types.

## 3. Supported Source Containers

Supported canonical exchange:

- `.gltf` with referenced buffer/image assets;
- `.glb`.

All references must resolve inside approved content-source roots or explicit generated intermediate roots.

Remote HTTP/data acquisition is not part of the content build.

## 4. glTF Version / Extensions

The importer requires glTF 2.0.

Extensions are classified as:

- **Supported Required** — explicitly implemented and validated;
- **Supported Optional** — used when present;
- **Ignored Optional** — safe presentation-only metadata that can be skipped;
- **Unsupported Required** — hard import failure.

Unknown required extensions never silently pass.

The supported-extension list is versioned content-toolchain data and printed by the cooker.

## 5. Coordinate System

StarForge Context Space is right-handed, meters, +Y up.

glTF source is imported without a global handedness inversion.

The cooker validates/bakes source transforms into the StarForge local-space convention while preserving semantic orientation.

Renderer camera-forward convention is irrelevant to content gameplay semantics.

## 6. Units

One canonical source unit equals one meter after glTF export.

The importer does not infer centimeters/inches from object names or authoring application.

An asset with incorrect authored scale must be fixed at export or through an explicit asset import manifest before canonical source acceptance.

## 7. Asset Import Definition

Each imported 3D asset has a StarForge definition describing what to extract.

Conceptually:

```text
ModelImportDefinition {
  ContentId
  SourcePath
  RootNodeSelection
  MeshSelection
  MaterialBindingPolicy
  SkeletonPolicy
  AnimationClipSelection
  SocketMappings
  CollisionSourceMappings
  LodPolicy
  CookFlags
}
```

This avoids relying on arbitrary scene contents that happen to be present in the DCC export.

## 8. Scene Node Import

Node hierarchy can be imported for:

- static scene composition;
- skeletal hierarchy;
- explicit sockets/anchors;
- authored submesh transforms;
- scene-instance transforms.

Nodes not selected/required by the import definition can be excluded.

Node names are diagnostics/authoring labels; they are not persistent gameplay identity by themselves.

## 9. Transform Baking

For static geometry, the cooker can bake node transforms into vertex data where this reduces runtime hierarchy complexity and preserves authored semantics.

For skeletons/sockets or transform-animated nodes, hierarchy is preserved where required.

Transform baking is deterministic and records the resulting bounds.

## 10. Mesh Primitive Validation

Each selected primitive validates:

- supported primitive topology;
- valid POSITION accessor;
- finite attribute values;
- matching vertex counts for enabled attributes;
- legal indices;
- valid material mapping;
- nondegenerate bounds;
- skin attribute consistency where applicable.

Unsupported production topology fails rather than being rendered incorrectly.

## 11. Primitive Topology

Baseline shipping mesh geometry is triangle-based.

Triangle strips/fans can be deterministically expanded during cooking when supported by the importer.

Line/point geometry is not ordinary world-mesh content unless a specific debug/effect schema owns it.

## 12. Runtime Static Vertex Format

Version-1 cooked PBR static mesh uses a StarForge-owned packed layout conceptually containing:

- Position: 32-bit float x3 in mesh-local meters;
- Normal: signed normalized packed representation;
- Tangent + handedness: signed normalized packed representation;
- UV0: 16-bit float x2 when precision validation passes, otherwise declared high-precision variant;
- optional UV1: same policy;
- optional Vertex Color: normalized 8-bit RGBA where required.

Exact C++ struct padding is implementation detail and is never serialized by raw `sizeof` memory dump; the cooked binary layout is explicitly versioned.

## 13. Runtime Skinned Vertex Format

Skinned meshes extend compatible static attributes with:

- joint indices;
- normalized skin weights.

Baseline supports **up to four effective influences per vertex** in the shipping runtime format.

If source contains more:

1. the cooker deterministically selects the highest-weight influences;
2. renormalizes weights;
3. reports a quality diagnostic;
4. can reject the asset when discarded weight exceeds the configured content-quality threshold.

## 14. Normals

Production PBR geometry requires valid normals.

If source normals are absent, the cooker may generate deterministic smooth/face normals only when the import definition explicitly permits generation.

Assets requiring intentional hard/split normals should export them correctly.

## 15. Tangents

A mesh using tangent-space normal mapping requires valid tangents.

Preferred path:

- export tangents in canonical glTF source.

A deterministic pipeline tangent-generation path may be provided, but it must produce a declared versioned result and cannot silently differ between machines.

An asset cannot ship with a normal map and undefined tangent basis.

## 16. UVs

UV validation includes:

- finite values;
- required UV set exists for every referenced texture binding;
- no assumption that UVs are restricted to 0..1 when wrap modes permit otherwise;
- lightmap-specific UV semantics are not invented because baseline TA-4 has no baked-GI/lightmap requirement.

## 17. Mesh Bounds

The cooker produces validated:

- local AABB;
- bounding sphere or equivalent culling bound;
- per-submesh bounds where useful.

Bounds are derived data and do not replace collision geometry.

## 18. Index Format

Cooker selects 16-bit indices when the primitive safely fits the addressable vertex range; otherwise 32-bit indices are used.

The selection is deterministic and recorded in the cooked mesh header.

## 19. Mesh Optimization

Offline mesh optimization uses pinned **meshoptimizer** behind a StarForge content-tool wrapper.

Permitted operations include:

- vertex-cache optimization;
- vertex-fetch optimization;
- overdraw-oriented triangle ordering where appropriate;
- deterministic mesh simplification for generated LODs.

The library is a build-time algorithm provider, not runtime content identity or gameplay authority.

## 20. LOD Policy

Each renderable mesh uses an explicit LOD policy:

- `None`;
- `Authored`;
- `Generated`;
- `Hybrid`.

Generated LODs derive from LOD0 with pinned meshoptimizer/tool settings.

Critical silhouettes or gameplay-readable shapes can provide authored LODs.

LOD changes affect rendering only; collision/hit/navigation authority uses separately declared products.

## 21. LOD Error Metadata

Generated LODs store geometric-error/bounds metadata suitable for TA-4 screen-space LOD selection.

TA-13 later sets performance/budget thresholds; TA-10 guarantees the metadata exists and is deterministic.

## 22. Material Mapping

A glTF material is converted/bound to a StarForge Material ContentId through the import definition/content pipeline.

The importer does not create permanent runtime material identity from an unstable glTF array index.

Material semantics must satisfy TA-4 metallic-roughness rules.

## 23. Texture References

Images referenced by glTF are content dependencies.

The cooker resolves them into canonical texture source inputs / Texture ContentIds and then through TA-10 texture cooking.

Runtime mesh products reference Texture/Material ContentIds or cooked registry handles, not source image paths.

## 24. Semantic Sockets / Anchors

Gameplay/presentation sockets such as:

- weapon muzzle;
- hand grip;
- docking visual anchor;
- character equipment socket;
- effect emitter;
- camera presentation anchor

are declared through explicit import metadata mapping semantic Socket IDs to selected glTF nodes/transforms.

A naming convention can help exporters but is not the sole semantic contract.

## 25. Gameplay Anchors

Mission spawn points, interaction anchors, traversal links, security doors, resource nodes and other gameplay-critical scene metadata live in StarForge scene/procedural definitions.

A mesh node name such as `spawn_enemy_01` cannot create gameplay behavior by itself.

## 26. Collision Source Geometry

The import definition can designate selected glTF meshes/nodes as source geometry for collision cooking.

Those selections are consumed by TA-10 collision cooking and are excluded from render output unless independently selected as visible geometry.

Collision behavior is not inferred from visual material name.

## 27. Skeleton Import

A Skeleton ContentId owns immutable authored skeleton definition containing:

- joint hierarchy;
- parent indices;
- bind/rest transforms;
- inverse bind data as required;
- stable semantic joint labels where explicitly mapped;
- bounds/validation metadata.

Skeleton ContentId is distinct from any persistent actor identity.

## 28. Skeleton Validation

Validation rejects:

- cycles;
- missing referenced joints;
- non-finite bind data;
- duplicate semantic joint mapping;
- incompatible inverse bind count;
- unsupported hierarchy transforms;
- skin referencing joints outside the selected skeleton.

## 29. Animation Clip Import

Selected glTF animations become immutable `AnimationClip` Content assets.

A clip contains normalized project-owned data for:

- target skeleton/node channels;
- translation/rotation/scale channels as permitted;
- key times;
- interpolation mode;
- clip duration;
- optional loop/default-playback metadata supplied by content definition.

TA-11/presentation runtime later owns playback/state-machine presentation behavior; TA-10 owns reliable clip bytes and identity.

## 30. Animation Time

Animation source time is normalized to seconds in cooked presentation data.

Animation playback time is not Simulation Time authority for gameplay transactions unless a gameplay system explicitly synchronizes a semantic event through its own contract.

## 31. Root Motion Boundary

Baseline gameplay locomotion remains CharacterMotor/spacecraft authority.

Animation root transforms cannot silently move the authoritative player/NPC through the world.

If later content requires root-motion-assisted presentation, it must be reconciled through the owning locomotion architecture rather than asset playback directly mutating persistent position.

## 32. Morph Targets

Morph targets are not a baseline requirement for version-1 gameplay architecture.

If future facial/deformation content requires them, TA-10 can add a versioned mesh format extension without changing ContentId identity.

## 33. Mesh Compression Boundary

The baseline cooked mesh is optimized and packed but does not require a complex runtime geometry decompressor.

Optional meshoptimizer-compatible stream compression can be added if TA-13 proves I/O/storage benefit, provided decode is deterministic and the cooked format version records it.

Correctness does not depend on compression.

## 34. Deterministic Cook

Cooked mesh/skeleton/animation output cannot include nondeterministic values such as:

- pointer addresses;
- hash-map iteration order;
- source absolute path;
- current time;
- worker completion order.

Equivalent source/toolchain input produces byte-stable or semantically hash-stable output according to the content-build test policy.

## 35. Runtime Boundary

Shipping runtime consumes StarForge cooked mesh/skeleton/animation products through the Content Registry.

It does not parse raw glTF for ordinary gameplay.

Development tooling can inspect source glTF, but scene activation uses validated cooked output.

## 36. Failure Policy

Import/cook failure returns typed diagnostics.

Examples:

- UnsupportedGltfExtension;
- InvalidAccessor;
- MissingRequiredAttribute;
- NonFiniteTransform;
- InvalidSkin;
- MaterialBindingMissing;
- TextureDependencyMissing;
- CollisionSourceMissing;
- LodGenerationFailed.

The cooker never emits a half-valid mesh product and marks it successful.

## 37. Tests

Required tests cover:

- `.gltf` and `.glb` parsing;
- coordinate/unit preservation;
- transform bake determinism;
- static/skinned vertex conversion;
- >4 skin influence reduction/rejection threshold;
- 16/32-bit index selection;
- material/texture ContentId mapping;
- missing normal/tangent handling;
- socket mapping;
- collision-source selection;
- skeleton cycle rejection;
- animation clip extraction;
- meshoptimizer output determinism under pinned version/options;
- source path relocation not changing logical asset identity;
- runtime never requiring glTF DOM state.

## 38. Explicit Non-Goals

No baseline requirement for:

- runtime glTF scene as gameplay object model;
- DCC node names as gameplay scripts;
- arbitrary glTF extension support;
- runtime mesh simplification;
- collision inferred from visible triangle mesh automatically;
- root animation directly owning locomotion;
- morph-target-heavy facial system;
- raw glTF array indices in saves.

## 39. Dependencies

Depends on TA-1 `fastgltf`, TA-2 ContentId, TA-3 coordinate/scene model, TA-4 mesh/material renderer contract, TA-5 collision boundary, TA-10 registry/source layout, and later TA-11 presentation animation integration.

## 40. Open Questions

None in the glTF import/mesh/skeleton/animation contract.
