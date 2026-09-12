# TA-10 — Content Repository Layout and Source Formats

> **Status:** Architecture Complete  
> **Authority:** Repository content-source layout, canonical authoring/exchange formats, path/naming rules, source-control boundaries, coordinate/unit conventions, and source-vs-cooked authority

## 1. Purpose

Project StarForge needs one reproducible path from human-authored assets/data into validated runtime content without turning DCC files, filenames, directory scanning, or editor state into gameplay authority.

TA-10 therefore distinguishes three layers:

1. **Working/DCC Sources** — artist/editor working files that may help author content but are not build authority;
2. **Canonical Source Content** — deterministic repository inputs consumed by the StarForge content pipeline;
3. **Cooked Runtime Content** — generated, versioned runtime-ready products consumed by the game.

Only canonical source content and the pinned content-toolchain determine cooked output.

## 2. Repository Source Root

Canonical source content lives under:

```text
content/
  schemas/
  source/
    definitions/
    models/
    textures/
    environments/
    shaders/
    audio/
    terrain/
    scenes/
    procedural/
```

The directory categories are organizational. `ContentId`, not source path, is authoritative content identity.

A source asset may move between folders without changing save identity if its `ContentId` remains unchanged and all dependencies are updated.

## 3. Generated Output Location

Cooked output is generated into the CMake/build tree, conceptually:

```text
<build-root>/starforge_content/<ContentBuildId>/
```

Generated cooked output is not edited by hand and is not normal source-of-truth content.

The source repository does not require checked-in cooked binaries for ordinary development.

Packaging/release automation may copy a validated cooked build into an install/staging directory, but that copy remains derived output.

## 4. Working / DCC Files

Examples include:

- Blender `.blend`;
- Krita/Photoshop source documents;
- sculpting/project files;
- audio DAW sessions;
- other tool-native project formats.

These are **not** direct StarForge build inputs unless a later explicit architecture change promotes one format.

The pipeline consumes exported canonical interchange files instead.

Large working binaries can use Git LFS or an external art workspace according to the later repository/implementation policy. The game build must not depend on an undocumented workstation-only file.

## 5. Canonical Hand-Authored Definition Format

StarForge v1 uses **UTF-8 JSON** for hand-authored structured gameplay/content definitions.

Definition files use the suffix:

```text
*.sfdef.json
```

Reasons:

- strict machine parsing;
- deterministic schema validation;
- mature tooling;
- explicit arrays/objects/numbers/strings;
- no indentation-sensitive semantics;
- straightforward generation of diagnostics and dependency graphs.

JSON object member order has no gameplay meaning unless a schema explicitly models order as an array.

## 6. JSON Input Rules

Canonical `.sfdef.json` input follows these rules:

- UTF-8 only;
- duplicate object keys are rejected;
- NaN/Infinity are rejected;
- unknown fields are errors unless that schema explicitly provides an extension object;
- required fields must be present;
- numeric range/unit validation is schema-owned;
- arrays are ordered where the schema says order is semantically meaningful;
- path separators inside project-relative source references use `/`;
- `..` parent traversal is rejected for canonical content references;
- filesystem case differences are not used as content identity.

Comments are not part of the canonical JSON grammar. Authoring rationale belongs in adjacent Markdown/design documentation or explicit schema-supported notes fields that are ignored by runtime semantics.

## 7. Canonical 3D Exchange Format

3D model/scene exchange uses **glTF 2.0**:

- `.gltf` plus referenced buffers/images; or
- `.glb`.

`fastgltf` remains the parser selected by TA-1.

DCC-specific scene formats never bypass the glTF import/validation contract.

## 8. 3D Units and Coordinate Convention

Canonical imported spatial content follows TA-3 Context Space:

- right-handed coordinates;
- meters;
- +Y local up for authored ground/interior content;
- normalized rotations;
- explicit scale.

The content cooker does not silently infer centimeters, inches, or arbitrary per-directory scale.

An asset requiring conversion must declare/import that conversion explicitly before it becomes canonical source content.

## 9. Transform Policy

Static source transforms can be baked by the cooker when the owning asset schema permits it.

Rules:

- non-finite transforms are rejected;
- zero scale is rejected;
- negative/mirrored scale must be explicitly supported by the asset class or baked into geometry with corrected winding/tangents;
- non-uniform scale on skinned/physics-sensitive assets is rejected unless the importer has an explicit validated bake path;
- runtime gameplay does not depend on DCC pivot quirks not represented in canonical data.

Semantic sockets/anchors are explicit content metadata rather than inferred from arbitrary object names.

## 10. Canonical Texture Source Formats

Baseline texture source formats are:

- **PNG** for LDR color/data/mask textures;
- **OpenEXR (`.exr`)** for HDR/environment/high-dynamic-range source data where required.

The pipeline may accept generated intermediate images internally, but canonical repository source uses the approved formats above unless an explicit importer is added.

Runtime does not load PNG/EXR as shipping texture authority; TA-10 texture cooking converts them into runtime texture products.

## 11. Texture Semantic Metadata

Every texture input is assigned an explicit semantic, for example:

- BaseColor;
- Emissive;
- Normal;
- MetallicRoughness;
- Occlusion;
- Mask;
- Height/Data;
- EnvironmentHDR;
- UI/Presentation.

Color space, mip generation and compression policy derive from this semantic, not filename guesses such as `_n` or `_srgb`.

Naming conventions may improve readability but are never authoritative metadata.

## 12. Environment Source Assets

Environment lighting source can use:

- equirectangular HDR EXR;
- authored cubemap faces through an explicit environment definition.

The cooker generates the TA-4 environment/IBL runtime products.

## 13. Shader Sources

Shipping shader source is project-owned **GLSL 4.60 Core**.

Canonical shader files live under `content/source/shaders/` and use normal stage extensions such as:

- `.vert`;
- `.frag`;
- `.comp` where a compute stage is actually required;
- shared `.glsl` include fragments.

Shader-family/variant declarations are structured content definitions; arbitrary per-material textual define injection is prohibited.

## 14. Audio Source Boundary

Canonical source audio accepted by the content pipeline is lossless:

- WAV;
- FLAC.

TA-10 owns discovery, fingerprints, metadata, dependency tracking and generic cooked-asset packaging.

TA-11 owns final playback categories, streaming policy, channel layout, loudness policy, spatialization, buses and the final runtime codec profile. TA-10 must not pre-empt those audio semantics.

## 15. Terrain Source Format

A bounded surface terrain asset is defined by a `.sfdef.json` terrain manifest plus explicit referenced source maps/data.

The baseline supports:

- height field source: 16-bit PNG or floating-point EXR;
- material/biome masks: PNG;
- optional authored object/POI placement data: JSON definition/reference;
- explicit local bounds and tile layout;
- explicit meters-per-sample / vertical scale metadata.

No terrain dimensions or scale are inferred from filename or image dimensions alone.

## 16. Scene Source Definitions

Scenes/zones use structured StarForge definitions that reference content assets by `ContentId` and declare authored semantic structure such as:

- static geometry groups;
- lights/probes;
- collision products;
- navigation products/annotations;
- spawn/interaction anchors;
- environment providers;
- stream-cell grouping;
- explicit entry/exit/transition anchors.

A glTF scene graph can supply transform hierarchy, but persistent gameplay semantics live in StarForge-owned scene/content definitions.

## 17. Procedural Source Definitions

Procedural mission/world modules use explicit structured definitions under `content/source/procedural/`.

They define reusable authored building blocks and compatibility metadata, not runtime-generated Mission IDs or persistent world state.

TA-9 remains authority for candidate generation, persistent identity creation, anti-reroll state and mission commit.

## 18. Path Rules

Canonical source paths:

- are repository-relative;
- use `/` in manifests;
- cannot escape the approved content roots;
- cannot depend on absolute workstation paths;
- cannot depend on environment-specific drive letters;
- must be normalized before dependency hashing.

Runtime content lookup never uses source path as the public API.

## 19. File Naming

File names are developer-facing and should be lowercase snake_case where practical.

Content identity is not derived by mechanically converting a filename into `ContentId`.

The file declares or is bound to its ContentId explicitly.

## 20. Source Asset Provenance

Each externally sourced asset must have provenance/license metadata sufficient to support the TA-1 license gate and later release packaging.

Required provenance metadata can include:

- creator/source;
- license/SPDX-style identifier where applicable;
- attribution/notice requirement;
- modification notes;
- internal ownership status.

Content with unresolved redistribution rights fails release validation.

## 21. Large Binary Source Control

Large canonical binary sources such as `.glb`, high-resolution PNG/EXR and lossless audio should use Git LFS once they exceed the repository threshold set during TA-16/scaffolding.

Whether bytes are in normal Git or LFS does not change content identity, hashing, or cooking behavior.

## 22. No Timestamp Authority

File modification timestamps can be used as a cheap development hint for deciding what to inspect, but they are never the final build-validity authority.

Cook correctness uses content/dependency fingerprints defined later in TA-10.

## 23. Deterministic Source Interpretation

Given identical:

- canonical source bytes;
- schema versions;
- tool versions;
- cook profile;
- platform-target profile;

the content pipeline must produce semantically identical cooked output independent of:

- source checkout location;
- machine username;
- wall-clock time;
- absolute path;
- thread completion order.

## 24. Required vs Optional Source

A missing source required to build a referenced mandatory content product is a build error.

A presentation fallback can exist only when the owning schema explicitly declares the dependency optional and runtime architecture permits a fallback.

Missing collision/navigation/objective-critical content is never silently replaced by a cosmetic placeholder.

## 25. Explicit Non-Goals

TA-10 does not require:

- a custom DCC/editor;
- runtime loading of `.blend`/PSD/DAW project files;
- raw source JSON/glTF/PNG/EXR scanning in shipping gameplay;
- filename-derived gameplay behavior;
- infinite procedural terrain;
- arbitrary executable scripts embedded in content;
- source path as save identity.

## 26. Test Requirements

Validation must prove:

- unsupported source formats fail clearly;
- duplicate JSON keys fail;
- illegal absolute/parent paths fail;
- wrong unit/terrain scale metadata fails;
- semantic texture color-space input is explicit;
- DCC working files are not required by a clean content build;
- checkout path/timestamps do not change semantic cooked output;
- a moved source file can retain the same ContentId;
- missing required source blocks cooking;
- missing optional presentation source follows only its declared fallback path.

## 27. Dependencies

Depends on TA-1 toolchain, TA-2 ContentId, TA-3 coordinate/scene model, TA-4 renderer/materials, TA-5 collision, TA-8 navigation, TA-9 mission/procedural architecture, and Design Complete GDS content requirements.

## 28. Open Questions

None in the source-format/repository-layout contract.
