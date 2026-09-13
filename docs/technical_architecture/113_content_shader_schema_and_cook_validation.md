# TA-14 — Content, Shader, Schema, and Cook Validation

> **Status:** Architecture Complete  
> **Authority:** Source-content validation, schema/reference/provenance checks, deterministic cook tests, shader validation, generated registry checks, hot-reload safety tests, and content CI boundaries

## 1. Purpose

TA-10 makes cooked content part of the executable product. TA-14 must therefore treat invalid or nondeterministically cooked content as a build failure rather than a runtime surprise.

## 2. Validation Stages

Canonical content verification stages are:

```text
Source Syntax
-> Schema
-> Typed References
-> Semantic Rules
-> Provenance / License
-> Cook
-> Cooked Product Validation
-> Registry Validation
-> Determinism Comparison
```

Each stage reports stable diagnostics.

## 3. Source Layout Validation

The validator checks:

- canonical `content/` source locations;
- allowed file types;
- UTF-8 structured definitions;
- no generated cooked files committed into source directories unless explicitly owned there;
- no duplicate logical ContentId declarations.

## 4. Closed Schema Validation

Every `*.sfdef.json` definition is checked against its versioned closed schema.

Unknown fields fail unless the schema explicitly permits an extension point.

Required fields, enum values, numeric ranges and ContentKind constraints are enforced before cook.

## 5. ContentId Validation

Tests assert:

- lowercase dotted canonical form;
- path independence;
- uniqueness;
- type-compatible typed references;
- no silent fuzzy aliasing;
- explicit rename/removal migration tables where required.

## 6. Cross-Reference Validation

The content graph verifies all mandatory references resolve and do not create prohibited dependency cycles.

Examples include:

- item -> mesh/material/audio;
- mission module -> compatible zone/layout pieces;
- material -> shader family/textures;
- scene -> collision/nav products;
- robot/ship definitions -> required capabilities/components.

## 7. Gameplay Semantic Validation

Content validation also enforces architecture-visible semantic constraints that can be checked statically, such as:

- positive capacities/masses where required;
- valid resource/output relationships;
- required extraction/mission module compatibility metadata;
- authored traversal links matching supported profiles;
- no content-defined persistent runtime IDs;
- no content-side reward/resource authority outside declared definitions.

It does not replace runtime validation for state-dependent rules.

## 8. Provenance and Licensing

Every distributable third-party/source asset requiring attribution/license tracking has machine-checkable provenance metadata.

Validation can fail for missing required provenance or forbidden license classifications according to project policy.

TA-14 does not publish or redistribute third-party license-protected source assets through test artifacts unless permitted.

## 9. glTF Validation

Import tests cover representative valid and invalid glTF fixtures and assert:

- supported attribute/layout combinations;
- transform conversion;
- skin/skeleton limits;
- animation channel validity;
- socket/tag rules;
- deterministic StarForge-owned output.

Unsupported constructs fail with precise source context.

## 10. Mesh Optimization Determinism

Given identical source, pinned meshoptimizer version and cook settings:

- optimized vertex/index output fingerprint is stable;
- generated LOD product fingerprints are stable;
- different worker counts do not change result identity.

An intentional meshoptimizer upgrade requires reviewed golden changes.

## 11. Texture Validation

Texture source tests validate:

- supported dimensions/formats;
- semantic color space;
- mip policy;
- compression profile;
- KTX2 readback;
- alpha requirements where applicable;
- deterministic cooked fingerprint.

## 12. Material Validation

Material tests verify:

- metallic/roughness semantics;
- referenced textures use compatible semantic/color-space classes;
- alpha mode is valid;
- required ShaderFamily exists;
- variant key is bounded/recognized.

## 13. Shader Source Validation

All shipping GLSL required by the baseline passes pinned glslang validation in CI.

Validation includes all required preprocessor/variant combinations, not only the default variant.

## 14. Shader Negative Fixtures

Deliberately invalid shader fixtures prove that diagnostics/gates detect:

- syntax error;
- interface mismatch where glslang can detect it;
- unsupported version/profile;
- missing required include/generated binding metadata.

## 15. OpenGL Driver Compile Boundary

Offline validation does not replace actual OpenGL driver compile/link smoke on supported backend/reference runners.

Both gates are required for implementation/release certification when the renderer exists.

## 16. Collision Cook Validation

Cooked collision tests verify:

- geometry integrity;
- collision material references;
- bounds;
- expected static/dynamic use class;
- deterministic product fingerprint;
- no accidental render-only decorative geometry becoming mandatory collision without authored rule.

## 17. Ground Navigation Cook Validation

Recast/Detour cook validation asserts:

- expected tiles produced;
- traversal profiles represented;
- authored links resolve;
- bounded tile metadata;
- deterministic product for identical pinned input/tool version.

## 18. Free-Flight Navigation Validation

Project-owned 3D navigation cook tests verify connectivity, actor-radius classes, obstacle metadata and deterministic graph/volume output.

## 19. Terrain / Streaming Metadata Validation

Terrain/stream-cell metadata tests cover:

- bounded cell extents;
- dependency references;
- collision/nav readiness metadata;
- no required cell with missing blocking product;
- stable cell keys.

## 20. Procedural Module Validation

Mission/procedural module tests verify static feasibility metadata needed by TA-9, including:

- compatible zone/profile;
- entry/extraction possibilities;
- required traversal classes;
- objective placement constraints;
- ContentId stability.

Generation still performs runtime state-dependent validation.

## 21. Clean Cook Determinism

A deterministic cook gate performs at least two clean cooks from the same source/toolchain and compares:

- output file set;
- Content Registry;
- ContentBuildId;
- per-entry ContentFingerprint;
- relevant byte-identical products where the format contract requires it.

Unexpected differences fail.

## 22. Worker-Count Cook Determinism

Representative clean/incremental cook tests run with multiple worker counts and require identical semantic output/fingerprints.

Worker completion order cannot change registry ordering or product identity.

## 23. Incremental Cook Correctness

Fixtures change one source/dependency at a time and assert:

- all affected outputs rebuild;
- unrelated outputs remain unchanged;
- registry generation/fingerprints update correctly;
- deleted/renamed sources trigger explicit compatibility diagnostics.

## 24. No-Op Build

A no-change incremental content build must produce no recooked products and no changed ContentBuildId.

## 25. Atomic Publication Test

Cook/build tests inject a failure before registry publication and assert the prior complete cooked generation remains usable.

A partial new generation cannot become the runtime registry.

## 26. Hot Reload Safety Tests

Each hot-reload class is tested:

- `PresentationSafe` applies only after valid replacement and preserves previous valid version on failure;
- `SceneReactivationRequired` cannot partially mutate a live scene;
- `SessionRestartRequired` never silently hot-applies.

Stale handles/generations reject.

## 27. Runtime Missing Content Tests

Required-content absence/corruption must:

- prevent scene readiness/activation;
- invoke Hold/failure policy;
- never substitute gameplay collision/nav/mission content with a presentation placeholder.

Optional presentation-only fallback is tested only where semantic equivalence is guaranteed.

## 28. Content Diagnostic Format

A content diagnostic includes where applicable:

```text
severity
code
ContentId
source path
schema/tool version
field/location
message
related ContentId
```

Codes are stable enough for CI summaries and fixture assertions.

## 29. Content CI Modes

TA-14 defines these logical modes:

- **validate:** schema/references/provenance/shader syntax, no full expensive cook required;
- **incremental fixture cook:** fast PR correctness;
- **determinism cook:** dedicated gate/schedule;
- **clean representative cook:** release/nightly/implementation milestone;
- **full project cook:** release/content certification when project size warrants.

## 30. Artifact Policy

On content gate failure CI retains bounded diagnostic artifacts such as:

- logs;
- failing generated registry excerpt;
- deterministic fingerprint diff;
- relevant non-restricted fixture output.

It does not upload the entire proprietary/raw content tree by default.

## 31. Explicit Non-Goals

Content validation does not:

- auto-fix source content in CI;
- silently rewrite ContentIds;
- approve licenses by guessing;
- treat runtime loading success as sufficient schema validation;
- allow a nondeterministic product because gameplay may not notice immediately.

## 32. Dependencies

Depends on TA-10 content architecture, pinned tools from TA-16, TA-3 streaming requirements, TA-4 shaders/materials, TA-5 collision, TA-8 navigation, TA-9 procedural feasibility and TA-13 build/performance budgets.

## 33. Open Questions

None within content validation/cook testing scope.