# TA-10 Cross-Validation — Content and Asset Pipeline

> **Status:** PASS  
> **Scope:** TA-10 documents `70`–`79` cross-validated against Design Complete GDS and TA-1 through TA-9

## 1. Result

TA-10 is cross-validated as a coherent content-production/runtime-loading architecture.

The audit focuses on:

- source-vs-cooked authority;
- ContentId/save compatibility;
- reproducible cooking;
- render/physics/navigation separation;
- procedural mission stability;
- registry/cache/runtime ownership;
- hot-reload safety;
- deterministic scene staging;
- future TA authority boundaries.

---

## 2. Source, Repository, and Authority

| # | Check | Result |
|---:|---|:---:|
| 1 | Canonical source, DCC working files, and cooked runtime content are distinct | PASS |
| 2 | DCC-native files are not hidden build authority | PASS |
| 3 | Canonical source has one repository root/layout | PASS |
| 4 | Cooked output is generated outside canonical source | PASS |
| 5 | Source path is not ContentId | PASS |
| 6 | Source path relocation can preserve identity | PASS |
| 7 | Hand-authored definitions use deterministic UTF-8 JSON | PASS |
| 8 | Duplicate JSON keys are rejected | PASS |
| 9 | Unknown fields fail unless schema explicitly supports extension | PASS |
| 10 | Absolute/workstation paths are rejected | PASS |
| 11 | Parent-path escape is rejected | PASS |
| 12 | glTF 2.0 remains canonical 3D exchange | PASS |
| 13 | Context Space remains right-handed/meters/+Y up | PASS |
| 14 | PNG/EXR texture source semantics are explicit | PASS |
| 15 | WAV/FLAC source audio does not pre-empt TA-11 playback authority | PASS |
| 16 | Terrain is bounded and explicitly scaled | PASS |
| 17 | Filenames cannot create gameplay behavior | PASS |
| 18 | External asset provenance/license can be validated | PASS |
| 19 | Timestamps are not final content-validity authority | PASS |
| 20 | Identical source/tool/profile has deterministic semantic output | PASS |

## 3. ContentId, Schemas, Registry, and Compatibility

| # | Check | Result |
|---:|---|:---:|
| 21 | TA-2 ContentId remains stable authored identity | PASS |
| 22 | TA-10 refines ContentId grammar without changing save representation | PASS |
| 23 | Canonical IDs are lowercase logical dotted names | PASS |
| 24 | ContentKind is explicit and closed | PASS |
| 25 | One ContentId cannot resolve to multiple kinds | PASS |
| 26 | Common schema envelope is defined | PASS |
| 27 | SchemaId and SchemaVersion are explicit | PASS |
| 28 | Incompatible schema change increments version | PASS |
| 29 | Units/ranges are schema-owned | PASS |
| 30 | Required vs optional content refs are distinct | PASS |
| 31 | Semantic references are distinct from build dependencies | PASS |
| 32 | Semantic reference cycles do not automatically create build cycles | PASS |
| 33 | Duplicate ContentId is deterministic hard error | PASS |
| 34 | Registry ordering is filesystem-order independent | PASS |
| 35 | Runtime registry indices are nonpersistent | PASS |
| 36 | SHA-256 ContentFingerprint excludes timestamps/absolute path | PASS |
| 37 | ContentBuildId identifies complete content build but not gameplay identity | PASS |
| 38 | Save compatibility still resolves stable ContentIds | PASS |
| 39 | Rename/removal requires explicit compatibility/migration metadata | PASS |
| 40 | Fuzzy/silent alias substitution is prohibited | PASS |

## 4. glTF, Mesh, Skeleton, and Animation Import

| # | Check | Result |
|---:|---|:---:|
| 41 | fastgltf remains parser behind StarForge importer | PASS |
| 42 | Unsupported required glTF extension fails | PASS |
| 43 | No global hidden handedness conversion | PASS |
| 44 | One source unit resolves to one meter | PASS |
| 45 | Import definition explicitly selects content | PASS |
| 46 | DCC node name alone is not gameplay identity | PASS |
| 47 | Static transform baking is deterministic | PASS |
| 48 | Invalid/nonfinite transforms fail | PASS |
| 49 | Triangle production topology is explicit | PASS |
| 50 | Runtime vertex layout is StarForge-owned/versioned | PASS |
| 51 | Raw C++ struct layout is not serialized | PASS |
| 52 | Skinned runtime supports deterministic max-four influence reduction | PASS |
| 53 | Missing/invalid normals/tangents have explicit policy | PASS |
| 54 | UV/material requirements are validated | PASS |
| 55 | 16/32-bit index selection is deterministic | PASS |
| 56 | meshoptimizer is build-time optimization, not identity authority | PASS |
| 57 | LOD policy is explicit and render-only | PASS |
| 58 | Gameplay sockets/anchors use explicit metadata | PASS |
| 59 | Collision source selection is explicit | PASS |
| 60 | Skeleton/animation products are immutable content, not actor state | PASS |

## 5. Texture, Material, Environment, and Shader Pipeline

| # | Check | Result |
|---:|---|:---:|
| 61 | KTX2 is canonical cooked texture container | PASS |
| 62 | KTX-Software remains behind StarForge content/render boundary | PASS |
| 63 | Texture semantics determine color space | PASS |
| 64 | BaseColor/Emissive remain sRGB semantics | PASS |
| 65 | Normal/data/masks remain linear semantics | PASS |
| 66 | Desktop GPU compression policy is explicit | PASS |
| 67 | Basis Universal is optional profile tool, not mandatory desktop semantics | PASS |
| 68 | Mips are canonical offline products | PASS |
| 69 | Normal-map filtering/renormalization is explicit | PASS |
| 70 | Alpha-mask mip policy preserves coverage | PASS |
| 71 | Material remains presentation data | PASS |
| 72 | Render Material and CollisionMaterial remain separate | PASS |
| 73 | Material feature set is closed by ShaderFamily | PASS |
| 74 | Sampler behavior uses semantic presets | PASS |
| 75 | Environment/IBL is cooked deterministically | PASS |
| 76 | Shader variants remain bounded | PASS |
| 77 | Shader include graph is dependency-tracked | PASS |
| 78 | glslang is offline validator, not runtime OpenGL program authority | PASS |
| 79 | Runtime still compiles/links GLSL through TA-4 renderer | PASS |
| 80 | Failed texture/shader hot reload preserves previous valid presentation state | PASS |

## 6. Collision, Navigation, and Terrain Cooking

| # | Check | Result |
|---:|---|:---:|
| 81 | Render geometry does not automatically become collision | PASS |
| 82 | Collision product is project-owned/versioned | PASS |
| 83 | Jolt runtime identity is not serialized into content | PASS |
| 84 | Static vs dynamic collision class is explicit | PASS |
| 85 | CollisionMaterialId binding is independent of visual material | PASS |
| 86 | Recast static ground nav cook remains behind adapter | PASS |
| 87 | Small/Standard/Heavy nav classes are preserved | PASS |
| 88 | Detour poly refs remain runtime/backend-local | PASS |
| 89 | Door/security state is not permanently baked as gameplay permission | PASS |
| 90 | Traversal links use explicit typed metadata | PASS |
| 91 | Navigation annotations do not grant AI permission by themselves | PASS |
| 92 | Collision change invalidates dependent nav | PASS |
| 93 | Free-flight nav remains separate project-owned 3D representation | PASS |
| 94 | Current hazards/security remain runtime overlays | PASS |
| 95 | Terrain content is finite/bounded | PASS |
| 96 | Terrain tile identity is content-local, not persistent gameplay identity | PASS |
| 97 | Render LOD cannot change authoritative collision | PASS |
| 98 | Terrain nav derives from authoritative geometry/collision source | PASS |
| 99 | Procedural placement regions expose static feasibility metadata | PASS |
| 100 | Missing required collision/nav blocks safe scene readiness | PASS |

## 7. Gameplay Definitions and Procedural Modules

| # | Check | Result |
|---:|---|:---:|
| 101 | Content Definition is distinct from persistent instance | PASS |
| 102 | Loading content never allocates gameplay IDs | PASS |
| 103 | Gameplay data uses closed schemas | PASS |
| 104 | No general-purpose scripting VM was introduced | PASS |
| 105 | Content configures implemented behavior only | PASS |
| 106 | GDS-fixed formulas/invariants cannot be replaced by arbitrary expressions | PASS |
| 107 | Tuneable values can live in validated content | PASS |
| 108 | World definitions preserve canonical GDS geography | PASS |
| 109 | Scene definitions do not hide mutable world state | PASS |
| 110 | MissionTemplate is distinct from MissionId/MissionInstanceId | PASS |
| 111 | Story mission templates preserve mandatory authored content | PASS |
| 112 | Objective templates use closed semantic predicates, not memory expressions | PASS |
| 113 | Raid templates do not own persistent target consequences | PASS |
| 114 | DynamicEvent templates do not own DynamicEventId lifecycle | PASS |
| 115 | Procedural modules expose typed sockets/interfaces | PASS |
| 116 | Module geometry/nav/socket feasibility is statically validated | PASS |
| 117 | TA-9 retains dynamic/world candidate validation | PASS |
| 118 | Committed generated mission stores selected module ContentIds/choices | PASS |
| 119 | Later module additions cannot reroll committed mission layout | PASS |
| 120 | Reward/resource definitions cannot grant/create ownership merely by loading | PASS |

## 8. Cooked Content, Dependency Graph, Cache, and Hot Reload

| # | Check | Result |
|---:|---|:---:|
| 121 | Shipping runtime consumes cooked content rather than source formats | PASS |
| 122 | Version-1 loose cooked files avoid premature package/archive complexity | PASS |
| 123 | Project cooked headers are explicitly versioned | PASS |
| 124 | KTX2 remains standardized unwrapped texture container | PASS |
| 125 | Each asset kind can version independently | PASS |
| 126 | Runtime Content Registry is immutable in normal shipping play | PASS |
| 127 | ContentHandle is generation-checked and nonpersistent | PASS |
| 128 | CPU ContentCache owns immutable CPU assets only | PASS |
| 129 | Renderer/physics/nav own their backend resources | PASS |
| 130 | Required vs optional load classification is explicit | PASS |
| 131 | Required asset failure cannot become cosmetic fallback | PASS |
| 132 | Build graph has explicit dependency sources | PASS |
| 133 | Incremental build uses fingerprints, not timestamp alone | PASS |
| 134 | Tool/schema/profile changes participate in fingerprints | PASS |
| 135 | Build worker order cannot change published registry/output semantics | PASS |
| 136 | Cooked output publication is atomic | PASS |
| 137 | Failed new content build does not publish incoherent registry | PASS |
| 138 | PresentationSafe/SceneReactivationRequired/SessionRestartRequired hot reload classes are explicit | PASS |
| 139 | Gameplay definitions are not silently hot-mutated into live saves | PASS |
| 140 | Stale content worker results carry generation/fingerprint checks | PASS |

## 9. Validation, Runtime Loading, Save, and Future TA Boundaries

| # | Check | Result |
|---:|---|:---:|
| 141 | Headless StarForge content tool is required | PASS |
| 142 | Validation stages cover source through runtime-interface integrity | PASS |
| 143 | Diagnostics have stable machine-readable codes | PASS |
| 144 | CI validation does not silently rewrite source | PASS |
| 145 | CMake remains authoritative build system | PASS |
| 146 | Third-party content tools are pinned/wrapped | PASS |
| 147 | Provenance/license validation is supported | PASS |
| 148 | Clean build needs only canonical source + pinned toolchain | PASS |
| 149 | Runtime registry resolves before I/O | PASS |
| 150 | Scene readiness waits for required CPU/backend content | PASS |
| 151 | TA-3 Hard Streaming Hold remains authoritative when required content is late | PASS |
| 152 | Optional deferred content cannot spawn/reveal/change gameplay | PASS |
| 153 | Procedural mission scene load cannot reroll committed modules | PASS |
| 154 | Story mission load cannot replace mandatory content with similar assets | PASS |
| 155 | Active scene content generation rejects stale async results | PASS |
| 156 | Persistent world state reprojects over cooked scene after reload | PASS |
| 157 | Horizon topology remains save-owned, not overwritten by module content | PASS |
| 158 | Off-screen simulation does not require render assets resident | PASS |
| 159 | TA-11/12/13/14/15/16 retain presentation, persistence implementation, budgets, CI, audit and implementation-lock authorities | PASS |
| 160 | No implementation-critical TA-10 open question remains | PASS |

---

## 10. Authority Boundary Review

TA-10 intentionally leaves the following to later phases:

- **TA-11 — Input, UI, Audio, and Presentation Integration:** shipping UI/HUD, input mapping, audio playback/codec/streaming categories, miniaudio integration, animation/presentation state-machine use, subtitles/captions/accessibility presentation;
- **TA-12 — Persistence Implementation Architecture:** exact save bytes, section directory, migration execution, crash-safe writes, save-slot layout and content-compatibility load implementation;
- **TA-13 — Concurrency, Performance, Memory, and Streaming Budgets:** numeric content-build/runtime I/O budgets, cache sizes, texture/mesh memory budgets, streaming thresholds, worker budgets and maximum active residency;
- **TA-14 — Testing, Diagnostics, and CI Architecture:** concrete CI jobs/gates, deterministic content regression jobs, warnings/sanitizers/tool enforcement;
- **TA-15 — Architecture Integration Audit:** final cross-TA validation;
- **TA-16 — Implementation Roadmap and Contract Locking:** exact CMake targets, vcpkg versions, directory scaffolding, tool executable names and implementation sequencing.

No later phase may silently change TA-10 identity/reproducibility/gameplay-authority semantics merely for convenience.

## 11. Final Result

```text
TA-10 — Content and Asset Pipeline
Status: ARCHITECTURE COMPLETE
Cross-validation: PASS
Checks: 160 / 160 PASS
Blocking contradictions: 0
Implementation-critical open questions: 0
```

**Next dependency:** TA-11 — Input, UI, Audio, and Presentation Integration.
