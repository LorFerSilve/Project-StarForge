# TA-10 — Content Validation, Diagnostics, and Build Integration

> **Status:** Architecture Complete  
> **Authority:** Headless content compiler commands, validation stages, diagnostic structure, deterministic cook/build behavior, CMake integration boundary, provenance/license checks, developer inspection tools, and future CI gate contract

## 1. Purpose

A large systemic game cannot rely on discovering malformed content only after launching a scene.

TA-10 therefore requires a headless StarForge content compiler/validator that catches source, schema, dependency, geometry, shader and cross-domain content failures before runtime.

## 2. StarForge Content Tool

Implementation provides one project-owned CLI/tool layer, conceptually named:

```text
starforge-content
```

The exact executable/target name can be locked in TA-16, but the architecture requires a single coherent command surface over the TA-10 pipeline.

## 3. Baseline Commands

The tool supports conceptual operations such as:

```text
validate
cook
build
clean
list
inspect
why-rebuild
show-deps
show-reverse-deps
```

Commands use the same schema/registry/cooker libraries rather than separate inconsistent validators.

## 4. Validation Stages

Validation proceeds through explicit layers:

1. source/encoding/path validation;
2. JSON/schema validation;
3. ContentId/kind/reference validation;
4. dependency graph validation;
5. asset-format parsing/import validation;
6. domain-semantic validation;
7. cross-content validation;
8. cook product validation;
9. runtime-interface validation;
10. registry/build integrity validation.

Later stages never excuse an earlier invalid source contract.

## 5. Source Validation

Checks include:

- approved extension;
- UTF-8 text validity;
- duplicate JSON key rejection;
- no absolute/escaping paths;
- source file present;
- source provenance metadata where required;
- canonical path normalization.

## 6. Schema Validation

Checks include:

- known SchemaId;
- supported SchemaVersion;
- required fields;
- unknown fields;
- enum values;
- numeric ranges;
- units;
- ordered data invariants;
- typed ContentId reference syntax.

## 7. Reference Validation

Checks include:

- referenced ContentId exists;
- expected ContentKind matches;
- required reference is non-null;
- optional reference obeys schema;
- deprecated target warning/error policy;
- migration-only references are not used as ordinary runtime references.

## 8. Dependency Validation

Checks include:

- build graph acyclic;
- source/import dependency exists;
- reverse-dependency graph can be generated;
- target cook profile compatibility;
- dependency fingerprint can be computed;
- no hidden absolute workstation dependency.

## 9. Geometry Validation

glTF/mesh validation uses `72` and reports specific failures such as:

- unsupported required extension;
- invalid accessor;
- non-finite transform;
- missing required vertex semantic;
- invalid skin/skeleton;
- incompatible material binding;
- invalid LOD;
- collision source selection failure.

## 10. Texture/Shader Validation

Checks include:

- texture semantic/color-space match;
- valid KTX2 cook;
- mip-chain rules;
- material/shader-family compatibility;
- bounded shader variants;
- shader include graph;
- offline glslang validation;
- TA-4 runtime interface metadata consistency.

## 11. Collision/Nav/Terrain Validation

Checks include:

- collision shape class validity;
- CollisionMaterialId existence;
- dynamic/static geometry compatibility;
- grounded nav class cook success;
- traversal-link endpoint validity;
- free-flight cell/portal validity;
- terrain bounds/tile/scale validity;
- collision/nav dependency alignment;
- required mission/procedural anchors are geometrically feasible.

## 12. Gameplay Semantic Validation

Checks include cross-domain relationships such as:

- weapon/ammo compatibility;
- station utility-port validity;
- ship slot/module compatibility;
- robot chassis/module/traversal compatibility;
- mission objective template completeness;
- extraction-capable procedural module requirement;
- Dynamic Event -> Mission/Defense template compatibility;
- reward/profile references;
- no content definition attempting to allocate persistent identity.

## 13. Validation Severity

Diagnostics have closed severities:

- Info;
- Warning;
- Error;
- FatalToolFailure.

A content build succeeds only with zero Errors/FatalToolFailures.

Warnings remain visible and can be promoted to errors for selected CI/release categories.

## 14. Stable Diagnostic Code

Every diagnostic has a stable project code, for example conceptually:

```text
SFCONTENT-E1001 DuplicateContentId
SFCONTENT-E2104 MissingRequiredTexture
SFCONTENT-E3302 NavLinkEndpointInvalid
```

Exact numbering is implementation data, but stable codes are required so CI/tests/documentation can match failures without parsing prose.

## 15. Diagnostic Context

A diagnostic should include as much structured context as applicable:

- code/severity;
- ContentId;
- source path;
- line/field/JSON pointer;
- schema/version;
- expected vs actual kind/value;
- dependency chain;
- importer/cooker stage;
- suggested remediation when unambiguous.

## 16. No Silent Autofix in CI

Validation/cook commands do not silently mutate canonical source to make it pass.

Developer-only explicit formatting/fix commands may exist for safe mechanical transformations, but CI validates the checked-in source as-is.

## 17. Deterministic Build Output

The content build guarantees that logical output ordering and fingerprints do not depend on:

- filesystem enumeration;
- worker completion;
- current time;
- checkout root;
- machine user.

Where a third-party tool emits nondeterministic metadata, the pipeline normalizes/excludes it from canonical output or rejects the tool path.

## 18. Clean Build

The content tool can build from an empty cooked cache using only repository canonical source + pinned dependencies/toolchain.

Undocumented manually generated local files are not allowed dependencies.

## 19. CMake Integration

CMake remains the authoritative build system.

TA-10 content integration provides targets conceptually such as:

```text
content_validate
content_cook
content_build
```

Exact target names are finalized in TA-16.

The executable game target depends on an appropriate validated cooked-content target/staging step rather than embedding a second build system.

## 20. Build Configuration Boundary

Content semantics are independent from Debug/Release C++ optimization mode.

A development content build can include extra diagnostics/source maps while shipping content omits unnecessary debug metadata.

Removing debug metadata cannot alter gameplay values/identities.

## 21. Target Cook Profiles

The tool requires an explicit target profile such as:

```text
windows_x64_gl46
```

The profile influences runtime formats/compression/capability checks and is included in fingerprints.

A missing/unknown target profile is a build error.

## 22. Third-Party Tool Invocation

When external build-time tools/libraries are used, StarForge wrappers own:

- input preparation;
- version check;
- arguments/options;
- output validation;
- diagnostic translation;
- fingerprint/tool-version contribution.

A developer's shell PATH must not silently select a random incompatible tool version.

## 23. Pinned Tools

TA-10 uses pinned versions of relevant tools/libraries, including:

- fastgltf;
- meshoptimizer;
- KTX-Software;
- glslang;
- Recast/Detour;
- project content compiler itself.

Version changes are explicit commits and trigger affected recooking.

## 24. Provenance / License Gate

Content validation can require provenance metadata for externally sourced assets.

Release validation fails for content marked with:

- unknown license;
- forbidden redistribution;
- required attribution not tracked;
- unresolved source provenance.

TA-1's dependency license gate and TA-10's asset provenance gate are separate but complementary.

## 25. Content Inventory

The tool can emit inventory reports by:

- ContentKind;
- namespace;
- source directory;
- cooked byte size;
- dependency count;
- provenance/license category;
- deprecated status;
- unused/unreferenced status where computable.

Unused content is a warning/report by default, not automatically deleted.

## 26. Dependency Inspection

`show-deps`/`why-rebuild` style tooling reports:

- direct dependencies;
- transitive dependencies;
- reverse dependents;
- changed fingerprint input;
- tool/schema version cause;
- source path cause.

This is essential for diagnosing incremental builds without weakening correctness.

## 27. Asset Inspection

Development tooling can inspect cooked metadata without launching gameplay, including:

- mesh counts/LODs/bounds;
- texture format/mips/colorspace;
- material/shader variant;
- collision shapes/materials;
- nav tile classes/links;
- terrain tile metadata;
- scene dependency closure;
- procedural sockets/feasibility tags.

## 28. Determinism Verification

A dedicated test mode can cook the same selected content twice with varied job scheduling and compare:

- logical registry;
- fingerprints;
- deterministic binary outputs where required;
- normalized semantic representation where third-party container metadata prevents byte-for-byte equivalence.

Any accepted non-byte-stable field must be documented and excluded from semantic fingerprinting.

## 29. Golden Fixtures

The repository maintains small representative content fixtures for pipeline tests, including:

- static PBR mesh;
- skinned mesh + animation;
- texture semantic set;
- shader family with variants/includes;
- collision + nav tile;
- bounded terrain tile;
- scene definition;
- procedural module;
- gameplay definition graph.

Fixtures are intentionally small and CI-friendly.

## 30. Failure Atomicity

A failed build/cook does not publish a successful registry pointing to invalid new outputs.

Previous validated development build can remain usable, but the new requested build returns failure.

## 31. Exit Status

CLI commands return machine-readable success/failure exit status suitable for CMake/CI.

Tool crash is distinguished from ordinary content validation failure in diagnostics.

## 32. Runtime Diagnostics Bridge

Cooked products preserve enough debug metadata/fingerprints that runtime load errors can be traced back to ContentId and build manifest information without requiring source files in shipping.

## 33. Future CI Boundary

TA-14 later defines exact GitHub Actions jobs/gates.

TA-10 requires those gates to include at minimum:

- clean content validation;
- deterministic representative cook tests;
- required shader validation;
- schema/reference integrity;
- content unit/integration tests.

TA-14 may add budgets/sanitizers/tooling but cannot remove TA-10 correctness gates.

## 34. Performance Boundary

TA-10 records build timings and cooked-size metrics.

TA-13 owns hard performance/memory/streaming budgets; TA-14 owns CI enforcement strategy.

Correctness validation is not disabled to make the content build faster.

## 35. Explicit Non-Goals

No baseline requirement for a GUI game editor, source-mutating CI, unpinned external command-line tools, file-timestamp-only cache validation, runtime-only discovery of content errors, or content compilation that changes gameplay state.

## 36. Dependencies

Depends on TA-1 CMake/vcpkg/toolchain, TA-2 deterministic identity/fingerprints principles, TA-3 through TA-9 consumer contracts, all TA-10 cooking specs, and later TA-13/TA-14/TA-16 for budgets/CI/target implementation details.

## 37. Open Questions

None in the validation/diagnostics/build-integration contract.
