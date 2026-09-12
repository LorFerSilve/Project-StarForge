# TA-10 — Content Runtime Loading, Failure, and Scene Integration

> **Status:** Architecture Complete  
> **Authority:** Runtime content-resolution flow, scene staging integration, required-content readiness, consumer instantiation order, content-generation freshness, runtime failure behavior, save/content compatibility boundary, and gameplay-independence from load timing

## 1. Purpose

Cooked content becomes meaningful only when runtime systems can resolve and stage it without allowing I/O timing, optional presentation assets, stale jobs or partial backend creation to change gameplay outcomes.

This specification closes the TA-10 runtime integration boundary.

## 2. Runtime Loading Layers

Runtime content loading is conceptually split into:

1. registry resolution;
2. dependency-set expansion;
3. asynchronous file I/O;
4. cooked-format validation/decode;
5. immutable CPU asset preparation;
6. consumer-specific preparation;
7. required-readiness evaluation;
8. scene/context activation at TA-3 boundary.

No stage individually grants gameplay existence before the owning scene/domain transaction does.

## 3. Registry First

A content request must resolve through the validated current Content Registry before file I/O.

Unknown ContentId/ContentKind mismatch fails immediately with typed diagnostics.

The runtime does not discover missing content by scanning directories.

## 4. Scene Staging

TA-3 destination staging obtains a cooked `SceneCompositionData` and expands its mandatory content closure.

Typical required groups include:

- gameplay-critical scene geometry;
- collision;
- applicable navigation;
- environment needed for safe gameplay readability;
- required actor/ship/station definitions;
- required mission/objective/procedural definitions;
- mandatory shaders/materials/textures for visible gameplay geometry;
- required traversal/interaction anchors.

## 5. Required Scene Readiness

A destination Scene becomes technically Ready only when all required content has:

- resolved registry entries;
- passed cooked integrity/version checks;
- produced valid CPU representation;
- completed required consumer instantiation/preparation;
- matched current SceneGeneration/content generation.

Optional presentation assets do not block readiness unless their absence would make gameplay misleading/unsafe.

## 6. Renderer Readiness

For required visible geometry, renderer readiness includes successful required GPU resource preparation/upload according to TA-4 staging rules.

A collision surface with no visible required geometry cannot silently activate if the player would encounter an invisible wall/floor critical to navigation.

Approved cosmetic detail can remain deferred.

## 7. Physics Readiness

Required collision content must instantiate valid TA-5 physics shapes before scene activation.

No-collision or generic-box fallback is prohibited for mandatory gameplay geometry.

Physics backend creation failure makes the required content not Ready.

## 8. Navigation Readiness

A scene requiring autonomous grounded/free-flight navigation must have the corresponding valid TA-8 navigation product/runtime space before actors depending on it become active.

A scene can explicitly classify navigation optional only when no required gameplay/AI correctness depends on it.

## 9. Procedural Mission Scene Staging

For a committed TA-9 GeneratedMissionPackage:

- selected module ContentIds/layout choices are already persistent/stable;
- TA-10 resolves exactly those committed modules;
- it does not ask the generator to select replacements because an asset loaded slowly;
- missing incompatible required module content is a content/save compatibility failure, not permission to reroll the mission.

## 10. Story Mission Content

Canonical story mission content resolves the authored MissionTemplate/Scene/module ContentIds fixed by the current content build/migration.

Loading cannot replace mandatory geography/character/evidence with a procedurally similar asset.

## 11. Runtime Load Scheduling

I/O/decode jobs may execute concurrently.

Scheduling priorities can consider:

- required vs optional;
- current scene staging;
- predicted stream-cell need;
- size/cost;
- consumer urgency.

Priority changes performance only. It cannot alter entity/objective/resource generation or content selection.

## 12. Immutable Worker Inputs

Workers receive immutable request/registry metadata including:

- ContentId;
- ContentFingerprint;
- ContentGeneration;
- CookedLocator;
- expected kind/version;
- SceneGeneration when relevant.

They do not mutate authoritative gameplay stores.

## 13. Main-Thread / Owner Commit

Worker completion produces prepared immutable data.

Consumer/backend ownership is committed through its owning thread/boundary:

- OpenGL upload on main/context thread;
- physics mutation at TA-5 safe boundary;
- nav-space publication at TA-8 safe revision boundary;
- gameplay definition visibility through immutable registry/read access.

## 14. Stale Result Policy

Before publication, a prepared result validates:

- ContentGeneration/fingerprint still current;
- request not cancelled/superseded;
- SceneGeneration still relevant for scene-local product;
- dependent runtime revisions still compatible where required.

Stale results are discarded or retained only as reusable cache data when immutable identity still matches.

## 15. Hard Streaming Hold

If required content is not ready before gameplay would enter it, TA-3 Hard Streaming Hold applies.

Simulation Time stops according to TA-3 rules; the game does not let enemies, hazards, station work or mission timers advance solely because required bytes are still loading for the player's destination.

## 16. Optional Deferred Content

Deferred optional assets may become resident after activation.

Their arrival can improve presentation but cannot:

- spawn gameplay entities;
- reveal hidden POIs;
- change collision;
- complete objectives;
- alter AI knowledge;
- change procedural content.

## 17. Stream Cells

TA-3 stream-cell residency uses scene dependency metadata to request content.

Content residency and gameplay activation remain separate:

- a cell can have assets resident but actors inactive;
- prefetch does not spawn resources/enemies;
- eviction does not delete persistent state.

## 18. Terrain Streaming

Terrain stream cells can load render/collision/nav tile products according to scene interest.

Required physical/nav tile readiness follows the same safety rules as other scene geometry.

Distant visual-only terrain can use lower-priority presentation residency.

## 19. Content Failure Categories

Runtime classifies failures such as:

- UnknownContentId;
- ContentKindMismatch;
- MissingCookedFile;
- UnsupportedCookedVersion;
- RegistryContentMismatch;
- CorruptCookedProduct;
- DependencyMissing;
- ConsumerInstantiationFailed;
- ShaderCompileOrInterfaceFailed;
- KtxValidationFailed;
- NavBackendVersionMismatch;
- ContentGenerationStale.

## 20. Required Failure Behavior

A required-content failure:

- prevents affected scene/use from becoming Ready;
- does not mutate gameplay state as if content existed;
- leaves current valid scene/session state intact where possible;
- surfaces actionable diagnostics;
- permits controlled return/cancel from a not-yet-committed transition when architecture allows.

It does not silently substitute unrelated content.

## 21. Optional Failure Behavior

Optional presentation content can use only its declared fallback/deferred path.

The runtime records the failure and continues without changing gameplay state.

## 22. Shader Failure

Required shader failure follows TA-4:

- development hot reload keeps previous valid program;
- initial required scene shader without valid fallback blocks correct readiness;
- approved debug/cosmetic optional shader can use a declared fallback.

## 23. Registry Failure

If the Content Registry itself is invalid/corrupt/incompatible, a playable session cannot start.

The runtime reports content-install/build integrity failure before attempting to deserialize gameplay saves into unresolved content.

## 24. Save Compatibility

TA-12 owns save migration/serialization implementation.

TA-10 runtime requires:

1. validated current Content Registry;
2. migration if saved required ContentIds/schema meaning changed;
3. all post-migration required ContentIds resolve to expected kinds.

No fuzzy name/path fallback is allowed.

## 25. ContentBuildId Diagnostics

When save/build IDs differ, the loader reports both IDs for diagnostics but does not reject solely because hashes differ.

Compatibility is determined by explicit required ContentId/schema/migration rules, not equality of one build hash.

## 26. Live Development Generation Swap

PresentationSafe hot reload publishes a newer ContentGeneration only after validation.

Existing consumers receive invalidation/change notification through typed content-resource mechanisms rather than direct writable registry mutation.

## 27. Active Scene Geometry Change

SceneReactivationRequired content is not swapped into an active authoritative scene piecemeal.

It remains pending until:

- next normal scene activation; or
- an explicit development-only controlled restage that quiesces runtime state and re-establishes physics/nav/render coherently.

## 28. Gameplay Definition Change

SessionRestartRequired content cannot reinterpret already active/persistent records in-place.

Development UI reports that restart/reload is required.

## 29. Content and Persistent Runtime Activation

Loading an Actor/Robot/Ship definition does not activate a persistent actor.

TA-2/TA-7 Activation Lease and owning gameplay state decide whether one instance becomes active.

Content describes the instance's type/configuration only.

## 30. Content and World Projection

Loading a scene/mesh does not recreate harvested resources, destroyed targets, opened doors or completed objectives.

TA-3 persistent world/zone state is projected onto the cooked scene after content readiness.

## 31. Content and Horizon Projection

Horizon station runtime geometry comes from persistent station topology plus authored module definitions.

Reloading module content does not overwrite the save's constructed/damaged topology.

## 32. Content and Off-Screen Simulation

Off-screen TA-6/TA-8/TA-9 simulation uses immutable gameplay definitions required for logical simulation, not active render/physics assets.

The system does not keep every mesh/texture GPU-resident merely because an off-screen persistent object exists.

## 33. Loading Screen / UI Boundary

TA-11 later owns loading presentation.

TA-10 exposes read-only progress/diagnostic state such as:

- required assets total/ready;
- current stage;
- failed ContentId;
- optional deferred count;
- scene staging generation.

UI cannot mark the scene Ready itself.

## 34. Cancellation

A pre-commit scene/content transition can cancel pending technical requests.

Cancelled workers may finish, but stale generation/request checks prevent publication into the cancelled scene.

Reusable immutable content may remain cached if its identity remains valid.

## 35. Shutdown

Shutdown order respects consumer ownership:

1. stop new scene/content requests;
2. cancel/quiesce scene-local work;
3. release scene/runtime consumer references;
4. renderer/physics/nav/audio destroy their resources under their own rules;
5. content cache/registry shutdown;
6. worker/tool infrastructure shutdown.

Content cache destructors do not directly destroy OpenGL/Jolt resources they do not own.

## 36. Determinism Boundary

For identical committed gameplay/content state:

- fast SSD vs slow SSD;
- different worker count;
- cache hit vs cold load;
- GPU upload completion order;
- optional asset availability timing

cannot change gameplay state or procedural results.

Only loading duration/presentation readiness differs.

## 37. Tests

Required integration tests cover:

- registry-first lookup;
- required mesh/collision/nav staging;
- optional deferred texture arrival;
- Hard Streaming Hold when required cell content is late;
- no mission reroll from missing/slow procedural module load;
- stale scene/content generation rejection;
- cancelled transition does not activate destination;
- persistent zone state reapplied after scene reload;
- save ContentId mismatch requires migration/fails clearly;
- off-screen actor does not require render mesh residency;
- same gameplay outcome under different content job completion orders.

## 38. Explicit Non-Goals

No baseline requirement for runtime raw-source cooking, load-order-driven gameplay, partial collision placeholders for mandatory content, hot gameplay-definition mutation of live saves, source-directory discovery, or keeping all off-screen presentation assets resident.

## 39. Dependencies

Depends on TA-2 identity/store/save contracts, TA-3 streaming/transitions, TA-4 GPU lifetime, TA-5 physics, TA-6 Horizon projection, TA-7 activation, TA-8 navigation, TA-9 committed mission/event state, and TA-10 registry/cooked/hot-reload architecture.

## 40. Open Questions

None in the runtime loading/failure/scene-integration contract.
