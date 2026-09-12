# TA-10 — Content Dependency Graph, Incremental Build, and Hot Reload

> **Status:** Architecture Complete  
> **Authority:** Content build graph, dependency discovery, deterministic fingerprints, incremental invalidation, atomic cooked publication, development file watching, hot-reload safety classes, and content-generation freshness

## 1. Purpose

StarForge content builds must be fast enough for development without making timestamps, file-watcher races, worker order or partial recooks semantic authority.

The build system therefore uses an explicit dependency graph plus deterministic fingerprints.

## 2. Build Graph

Every cookable source/product is a node in a project-owned content graph.

A node records conceptually:

- logical ContentId or source-internal node key;
- source files;
- schema/cooker version;
- target cook profile;
- direct build dependencies;
- source/dependency fingerprints;
- expected output products.

## 3. Dependency Discovery

Dependencies come from explicit/validated sources such as:

- typed ContentId references;
- glTF external buffers/images;
- model import definitions;
- shader `#include` graph;
- scene references;
- collision -> source mesh relation;
- nav -> collision/geometry relation;
- terrain -> maps/materials/nav relation;
- procedural module -> scene/collision/nav relation.

Filesystem proximity is not a dependency rule.

## 4. Semantic Reference vs Build Edge

Not every gameplay reference creates an eager build-order dependency.

The content compiler distinguishes:

- semantic runtime reference;
- build artifact dependency.

This allows legitimate content-reference cycles while keeping the actual cook graph executable.

## 5. Fingerprint Inputs

Node fingerprint uses TA-10 SHA-256 over deterministic inputs including:

- normalized canonical source bytes;
- relevant direct dependency fingerprints;
- schema version;
- cooker/tool version;
- target profile;
- relevant cook options.

Absolute path, machine name and wall-clock timestamp are excluded.

## 6. Timestamp Boundary

File modification time can be used as a fast cache hint, but a timestamp match/mismatch is not final correctness proof.

A content whose bytes/tool inputs have not changed remains equivalent even if checkout timestamps differ.

## 7. Incremental Invalidations

When a node fingerprint changes, the build invalidates:

- that node's cooked output;
- every transitive build-dependent product whose own fingerprint incorporates the changed dependency.

Unrelated content remains reusable.

## 8. Examples

Changing a BaseColor PNG invalidates:

- its Texture product;
- materials/bundles that embed required derived metadata if applicable;
- scene/package products whose cook truly embeds the changed dependency.

It does **not** automatically invalidate unrelated collision/nav if those products do not depend on render texture bytes.

Changing a collision source invalidates:

- collision product;
- dependent grounded nav;
- scene/procedural feasibility product that embeds their revisions.

## 9. Deterministic Build Scheduling

Build jobs can run concurrently.

Worker completion order does not decide:

- registry ordering;
- ContentId assignment;
- candidate selection;
- dependency order;
- final authoritative output.

The graph/topological order and stable ContentId/source keys provide deterministic merge/publication order.

## 10. Build Cache Key

Cooked-cache key includes at minimum:

```text
ContentId / node key
ContentFingerprint
CookedFormatVersion
TargetCookProfile
```

A cached output is reused only when all validity metadata matches.

## 11. Atomic Output Publication

A cook writes to a temporary/staging output first.

Only after complete validation does it atomically publish/rename the new cooked product and update generated registry state.

A failed cook leaves the previous validated output untouched for development use, but the current requested content build is still marked failed until resolved.

## 12. Registry Publication

The Content Registry is generated only after the requested build graph reaches a coherent validated state.

The registry never points partly to old and partly to failed-new products as one successful build.

## 13. Content Build Generation

Development runtime associates loaded content with a monotonically increasing process-local `ContentGeneration` plus ContentBuildId/fingerprints.

Hot-reload candidate publication creates a newer generation only after required candidate validation succeeds.

`ContentGeneration` is not save identity.

## 14. File Watching

Development file watching is an optimization that triggers recook requests.

Rules:

- coalesce duplicate rapid notifications;
- re-read canonical source after file-write stabilization;
- rebuild through ordinary dependency/fingerprint validation;
- watcher event ordering is not content semantics;
- missed watcher events are recoverable by explicit build/scan.

Shipping runtime has no required source watcher.

## 15. Hot Reload Safety Classes

TA-10 classifies content changes into three baseline classes:

1. **PresentationSafe**;
2. **SceneReactivationRequired**;
3. **SessionRestartRequired**.

The classifier derives from ContentKind/schema and change type, not developer guess at runtime.

## 16. PresentationSafe

Examples can include:

- texture bytes;
- material presentation parameters;
- compatible shader implementation;
- selected VFX presentation content;
- later TA-11 audio presentation content where safe.

Requirements:

- no gameplay schema meaning changes;
- candidate validates;
- consumer can generation-swap safely;
- failure preserves previous valid resource.

## 17. SceneReactivationRequired

Typical examples:

- static mesh topology used by active scene composition;
- collision;
- static navigation;
- terrain topology;
- scene stream-cell layout;
- procedural module geometry/anchors.

These cannot be silently swapped into an active authoritative scene if doing so would mix old/new physics/nav/entity placement.

Development tooling either:

- defers the new content until next scene activation; or
- performs an explicit controlled scene teardown/restage path in a test environment.

## 18. SessionRestartRequired

Typical gameplay-semantic changes include:

- weapon/item gameplay definition;
- ship/station module functional definition;
- mission/objective template;
- Dynamic Event/Raid gameplay template;
- progression/research/economy semantics;
- world canonical definitions already materialized in persistent state.

Normal live saves do not mutate those definitions in-place.

Development test harnesses may start/reset a controlled session against the new build.

## 19. Hot Reload Does Not Allocate Gameplay Identity

Reloading a mesh/definition/template never creates:

- MissionId;
- RobotId;
- ShipId;
- CrewId;
- Resource ownership;
- DynamicEventId.

Only owning gameplay transactions create persistent identities.

## 20. Hot Reload and Runtime Handles

Content/render runtime references carry generation/fingerprint metadata.

After a successful presentation swap:

- new lookup returns new generation;
- old backend resources retire safely;
- stale handles fail generation checks;
- in-flight frames/jobs can finish only under their explicit old-generation lifetime contract.

## 21. Hot Reload and Async Jobs

Worker results produced from old content carry the source ContentGeneration/fingerprint.

Before applying a result, the consumer validates freshness.

Examples:

- old mesh upload cannot replace newer texture/material generation;
- old nav cook cannot publish after newer collision geometry;
- old scene preparation cannot activate after registry generation changed.

## 22. Shader Hot Reload

Shader hot reload follows TA-4/TA-10:

1. invalidate include/variant dependents;
2. rebuild ShaderBundle;
3. run offline validation;
4. renderer compiles/links candidate;
5. validate interface;
6. swap at frame boundary;
7. retire old program later.

Failure keeps the old valid program.

## 23. Texture Hot Reload

Texture change:

1. recook KTX2;
2. validate semantic/format;
3. prepare upload;
4. swap renderer resource generation at safe boundary;
5. retire old resource after in-flight use.

No gameplay content generation changes merely because a texture changes.

## 24. Gameplay Definition Reload

When a SessionRestartRequired definition changes, the development runtime can show the new definition as `PendingRestart` but does not reinterpret existing gameplay records mid-session.

This is intentional safety, not a limitation to bypass with direct memory editing.

## 25. Dependency Inspection

Developer tooling exposes queries such as:

- why was this asset rebuilt?;
- direct dependencies;
- reverse dependents;
- fingerprint components;
- current/pending ContentGeneration;
- source -> cooked chain;
- hot-reload safety class.

## 26. Build Graph Cycles

True build-dependency cycles are errors.

Diagnostic reports the complete cycle chain.

The compiler does not break the cycle by arbitrary path order.

## 27. Deleted Source

Deleting a canonical source file invalidates its product and reverse dependents.

If required ContentIds still reference it, the build fails.

Old cooked output is not silently retained as a successful new content build.

## 28. Renamed Source

Moving/renaming a source path while preserving declared ContentId and identical semantic bytes can produce the same logical content identity.

Source-path metadata changes for diagnostics, not save identity.

## 29. Tool Upgrade

Changing a cooker/library version that can affect output is included in relevant fingerprints and invalidates affected cooked products.

Examples:

- meshoptimizer upgrade;
- KTX compressor upgrade;
- glslang validator contract change;
- Recast cook version change.

## 30. Reproducible Clean Build

A clean build from source must not depend on stale incremental outputs.

CI/content validation periodically compares clean and incremental outputs/fingerprints for equivalence.

## 31. No Gameplay Dependence on Build Speed

Faster/slower recook, file watching, cache hits or worker scheduling cannot change procedural mission selection, loot, objectives, world state or simulation time.

Content building is development/build infrastructure, not gameplay simulation.

## 32. Tests

Required tests cover:

- timestamp-only change does not alter fingerprint;
- byte/schema/tool change does;
- reverse dependency invalidation;
- unrelated content stays cached;
- build cycle detection;
- atomic output publication;
- failed candidate preserves previous dev resource;
- presentation hot reload generation swap;
- collision/nav change requires scene reactivation;
- gameplay definition change requires session restart;
- stale worker result rejected;
- clean vs incremental logical output equivalence;
- source path move preserves ContentId.

## 33. Explicit Non-Goals

No baseline requirement for live gameplay-schema mutation, shipping source watchers, source timestamps as authoritative cache keys, hot reload that rewrites persistent saves, or file-watcher event order as content semantics.

## 34. Dependencies

Depends on TA-2 revisions/generation patterns, TA-3 scene activation, TA-4 render hot reload, TA-8 nav invalidation, TA-10 registry/cooked formats, and later TA-13 worker/performance budgets.

## 35. Open Questions

None in the dependency/incremental/hot-reload contract.
