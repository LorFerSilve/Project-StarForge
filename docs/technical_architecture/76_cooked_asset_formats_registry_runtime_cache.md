# TA-10 — Cooked Asset Formats, Registry, and Runtime Cache

> **Status:** Architecture Complete  
> **Authority:** Cooked artifact formats, runtime Content Registry, ContentBuildId, loose-file baseline, typed runtime lookup, CPU content cache, required/optional loading, and consumer handoff

## 1. Purpose

Shipping runtime must consume validated runtime-ready content rather than reparsing authoring files. TA-10 therefore places a versioned cooked-content layer between canonical source and runtime consumers.

## 2. Shipping Boundary

Normal shipping runtime loads:

- the generated Content Registry;
- versioned cooked assets referenced by the registry.

It does not normally parse source `.sfdef.json`, glTF/GLB, PNG/EXR, terrain source manifests, or shader directory trees.

## 3. Loose Cooked Files Baseline

Version-1 uses individual cooked files addressed by the registry. A custom monolithic package archive is not required initially.

This keeps incremental builds, debugging, hot reload and dependency inspection simple. A later archive layer may pack identical cooked products without changing ContentIds.

## 4. Baseline Cooked Kinds

Project-owned products include conceptually:

- `.sfmesh`;
- `.sfskeleton`;
- `.sfanim`;
- `.sfmaterial`;
- `.sfenvironment`;
- `.sfshader`;
- `.sfcollision`;
- `.sfnav`;
- `.sfflightnav`;
- `.sfterrain`;
- `.sfscene`;
- `.sfdef`;
- `.sfproc`.

Textures remain standardized `.ktx2`. Audio receives its final runtime representation with TA-11 while still participating in TA-10 identity/dependency/fingerprint infrastructure.

## 5. Project-Owned Cooked Header

Every project-owned cooked blob has an explicit versioned header conceptually containing:

```text
Magic
HeaderVersion
ContentKind
CookedFormatVersion
ContentId
ContentFingerprint
PayloadSize
Flags
SectionDirectory
```

No cooked format is a raw `sizeof(CppStruct)` memory dump.

Version-1 project fields use explicit little-endian encoding.

## 6. KTX2 Metadata

KTX2 is not wrapped in another container. The Content Registry stores the StarForge metadata around it:

- Texture ContentId;
- fingerprint;
- semantic/color-space class;
- target profile;
- cooked locator.

Runtime validates the KTX2 product against that registry entry.

## 7. Per-Kind Format Versions

Each cooked kind owns its own `CookedFormatVersion`.

Changing mesh layout does not force collision/nav/definition format changes. Unsupported required versions fail with a clear `RecookRequired/UnsupportedCookedVersion` diagnostic.

## 8. Runtime Content Registry

A validated build emits:

- an immutable binary runtime Content Registry;
- a human-readable development manifest generated from the same validated graph.

Each runtime entry contains at minimum:

```text
ContentId
ContentKind
SchemaId/Version where applicable
CookedFormatVersion
ContentFingerprint
CookedLocator
Required/Optional classification
Dependency metadata
```

## 9. Cooked Locator

`CookedLocator` is package/build-relative location metadata. It is not save identity and can change between builds while ContentId remains stable.

## 10. Runtime Lookup

Public runtime lookup is typed:

```text
resolve<MeshAsset>(ContentId)
resolve<CollisionAsset>(ContentId)
resolve<GameplayDefinition>(ContentId)
```

Expected ContentKind is validated. A shader ID cannot be accepted through a weapon-definition lookup just because both are string-backed IDs.

## 11. Runtime Content Handle

After lookup, runtime may use a compact generation-checked `ContentHandle`/registry index.

Rules:

- nonpersistent;
- valid only for the current registry generation/ContentBuildId;
- never written into saves;
- invalidated when a development registry generation is replaced.

## 12. ContentBuildId

The registry carries the SHA-256 `ContentBuildId` defined by TA-10.

It identifies one complete cooked content set for diagnostics/compatibility checks. It never replaces stable per-entry ContentIds or explicit save migration.

## 13. CPU Runtime Representation

Cooked bytes decode into immutable StarForge-owned CPU representations such as:

- MeshUploadData;
- MaterialRuntimeData;
- CollisionDescriptor;
- NavigationTileData;
- SceneCompositionData;
- GameplayDefinitionData;
- ShaderBundleData.

Consumers do not receive writable parser/file-buffer state as authority.

## 14. CPU Content Cache

`ContentCache` may retain immutable loaded/decoded assets keyed by:

```text
ContentId + ContentFingerprint + relevant runtime variant/profile
```

Cache residency affects performance only. Eviction cannot change gameplay state, procedural outcomes or persistent ownership.

## 15. Cache Lifecycle

Conceptual state:

```text
Unloaded
 -> IOQueued
 -> Reading
 -> Validating
 -> CPUReady
 -> ConsumerPrepared/Resident
 -> Retiring
 -> Unloaded
```

Failed validation never exposes a partial object as `CPUReady`.

## 16. Required vs Optional Dependencies

Dependency edges used for runtime readiness are classified as:

- **Required**;
- **OptionalPresentation**;
- **DeferredOptional**.

Only the owning schema can mark a dependency optional.

Missing required collision/nav/mandatory geometry cannot be replaced by a cosmetic placeholder.

## 17. Required Failure

If required content cannot load:

- destination Scene does not become Ready;
- authoritative gameplay does not pretend the asset exists;
- TA-3 loading/Hard Streaming Hold semantics apply where relevant;
- a typed diagnostic identifies the failed ContentId/dependency chain.

## 18. Optional Presentation Fallback

Approved optional presentation assets can use TA-4 project fallbacks. This is logged and cannot alter physics, navigation, objective state, damage, rewards or world truth.

## 19. Consumer Ownership

ContentCache owns immutable CPU content; backend runtime resources remain consumer-owned:

- renderer owns OpenGL resources;
- physics adapter owns Jolt runtime shapes/bodies;
- navigation adapter owns Detour/query runtime objects;
- audio layer later owns miniaudio/backend resources.

## 20. Render Handoff

```text
Cooked render asset
 -> ContentCache CPUReady
 -> immutable upload payload
 -> TA-4 GpuUploadQueue
 -> renderer-owned RenderResourceHandle
```

OpenGL upload remains context-thread authority.

## 21. Physics Handoff

```text
Cooked CollisionDescriptor
 -> content validation
 -> TA-5 adapter
 -> Jolt runtime shape
```

Raw Jolt identity is never written into the registry or save.

## 22. Navigation Handoff

Cooked GroundNavigation/FreeFlightNavigation products are validated and instantiated by TA-8 adapters. Runtime polygon/cell handles remain technical local state.

## 23. Scene Dependency Set

A cooked Scene asset exposes the dependency closure required to stage its scene/profile.

The loader resolves mandatory dependencies before atomic context transition, while optional presentation assets can follow declared fallback/deferred behavior.

## 24. Dependency Closure Uses

Generated dependency metadata supports:

- scene preloading;
- incremental recooking;
- diagnostics;
- required/optional staging;
- later TA-13 I/O/memory planning.

It does not imply every semantic gameplay reference must be eagerly loaded into each scene.

## 25. Chunked Products

Large project-owned cooked blobs may contain versioned sections/chunks for data such as:

- mesh LODs;
- scene stream cells;
- terrain tiles;
- animation tracks.

TA-13 later decides where partial residency materially helps. Correctness does not require streaming every chunk independently.

## 26. No Raw Source Fallback in Shipping

If a cooked asset is missing, shipping runtime does not silently locate raw source and recook it on the player's machine.

That would bypass validation/tool-version/reproducibility contracts.

## 27. Startup Validation

The Content Registry is validated before save/session content resolution.

Required bootstrap/fallback/core content is checked before entering playable state.

## 28. Save Loading

TA-12 resolves save-stored ContentIds only against a validated registry or explicit migration. Missing required saved content never triggers fuzzy filename/path search.

## 29. Scene Lifetime

Shared ContentCache entries can outlive one SceneInstance. Scene-local runtime entities/render objects/physics objects still obey their own generation and teardown rules.

## 30. Async Freshness

I/O/decode/preparation jobs carry:

- ContentId;
- fingerprint/registry generation;
- request generation;
- SceneGeneration when relevant.

Stale results cannot overwrite a newer content generation merely because a worker finished later.

## 31. Memory Accounting

Content/cache entries report estimated byte usage by category where practical. TA-13 owns final CPU/GPU/I/O budgets and eviction thresholds.

## 32. Development Manifest

The readable manifest includes useful fields such as:

- ContentId/kind;
- source path;
- cooked locator;
- fingerprint;
- schema/cooked version;
- direct dependencies;
- provenance/license status;
- byte size;
- target profile/tool version.

It is diagnostics, not gameplay state.

## 33. Integrity Validation

Before `CPUReady`, the runtime validates applicable:

- header magic/version;
- ContentId/kind match;
- payload ranges/counts;
- dependency compatibility;
- standardized container validity such as KTX2.

Malformed content is rejected.

## 34. Tests

Required tests cover:

- cooked-header version rejection;
- expected-kind lookup;
- ContentHandle invalidation across registry generations;
- no runtime index in saves;
- required dependency blocks scene readiness;
- optional presentation fallback;
- shared cache survives scene teardown safely;
- backend resources remain consumer-owned;
- stale async result rejection;
- runtime succeeds without source authoring files;
- same logical registry produces the same ContentBuildId.

## 35. Explicit Non-Goals

No baseline requirement for a monolithic asset archive, raw-source shipping fallback, registry indices in saves, generic ContentCache ownership of backend resources, virtual texturing, CDN content streaming, or live third-party mod registry merging.

## 36. Dependencies

Depends on TA-2 ContentId/save contracts, TA-3 scene staging, TA-4 GPU resources, TA-5 physics, TA-8 navigation, TA-10 source/registry/cooking pipeline, and later TA-12/TA-13 implementation details.

## 37. Open Questions

None in the cooked-format/registry/runtime-cache contract.
