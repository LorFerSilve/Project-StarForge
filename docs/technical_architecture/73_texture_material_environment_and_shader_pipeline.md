# TA-10 — Texture, Material, Environment, and Shader Pipeline

> **Status:** Architecture Complete  
> **Authority:** Texture source interpretation, KTX2 cooking, GPU format policy, mip generation, material conversion, environment/IBL cooking, shader source dependency/variant builds, offline validation, and render-resource handoff

## 1. Purpose

TA-4 defines how materials/shaders/textures behave at runtime. TA-10 must convert authored source into deterministic render-ready products without letting source filenames, image-decoder behavior, or shader build order affect gameplay.

## 2. Canonical Texture Runtime Container

StarForge version-1 cooked textures use **KTX2** as the canonical runtime texture container.

The pipeline uses pinned Khronos **KTX-Software** tooling/library behind StarForge-owned content/render adapters.

KTX2 provides:

- explicit dimensions/levels/formats;
- mip-chain storage;
- cubemap/array capability;
- GPU block-compressed texture support;
- Basis Universal support where a target profile explicitly uses it.

Content identity remains StarForge `ContentId`; KTX object/file identity is not gameplay identity.

## 3. Source-to-Cooked Texture Flow

Conceptually:

```text
PNG / EXR
  -> validate semantic metadata
  -> decode/normalize
  -> semantic-aware processing
  -> generate mip chain
  -> compress/select runtime GPU format
  -> validate KTX2
  -> register Texture ContentId
```

Runtime never needs the original PNG/EXR for ordinary shipping play.

## 4. Texture Semantic Classes

Every texture definition declares one semantic class, including:

- BaseColor;
- Emissive;
- Normal;
- MetallicRoughness;
- Occlusion;
- ORM packed data;
- GenericMask;
- Height/Data;
- EnvironmentHDR;
- UI/Presentation.

The semantic determines color space, mip filter and compression rules.

## 5. Color Space

TA-4 remains authoritative:

- BaseColor / Emissive color are sRGB source semantics;
- Normal / Metallic / Roughness / Occlusion / masks / data maps are linear;
- HDR environment source is linear.

The cooker records the matching runtime format semantics.

Changing a file name cannot alter color-space interpretation.

## 6. Baseline Desktop GPU Formats

For the Windows x64 OpenGL 4.6 baseline, preferred cooked formats are:

- BaseColor / Emissive color: BC7 sRGB;
- Normal map: BC5 linear when XY reconstruction is valid;
- single-channel mask/data: BC4 when quality permits;
- packed multi-channel linear data/ORM: BC7 linear;
- HDR environment: BC6H or validated RGB16F fallback when required by the cook profile;
- small special textures where compression hurts quality: explicit uncompressed format.

The content profile records exact format so runtime never guesses.

## 7. Basis Universal Boundary

KTX2 can store Basis Universal payloads, but Basis is not mandatory for the primary desktop shipping profile.

It may be used for:

- portable development/content distribution profiles;
- future platform targets;
- asset classes where runtime transcode provides measured value.

If used, transcode target selection is deterministic for the declared target profile and does not change gameplay semantics.

## 8. Mipmaps

Mip chains are generated offline by default.

Semantic-aware rules include:

- BaseColor/Emissive: color-aware filtering in linear working space where appropriate;
- Normal: vector-aware filtering and renormalization;
- Masks/data: channel-semantic filtering;
- alpha-masked textures: preserve declared alpha-test coverage within bounded tolerance;
- EnvironmentHDR: dedicated environment-map processing.

Runtime `glGenerateMipmap` is not the canonical shipping texture cook path.

## 9. Texture Dimensions

Cook validation enforces:

- nonzero finite dimensions;
- target-hardware maximum compatibility;
- cubemap faces consistent in dimensions/format;
- mip chain completeness where required;
- schema-approved aspect/size rules for specialized textures.

TA-13 later sets numeric memory/resolution budgets.

## 10. Normal Map Convention

StarForge uses one declared tangent-space normal-map convention for cooked content.

The pipeline normalizes imported source into that convention at cook time rather than allowing per-material shader guesses.

Any source requiring Y-channel inversion declares it in import metadata; the cook result is canonical.

## 11. Material Definition

A StarForge Material Content asset contains conceptually:

```text
MaterialDefinition {
  ContentId
  ShaderFamilyId
  RenderClass
  BaseParameters
  TextureRefs
  SamplerPresetRefs
  FeatureFlags
  ShadowFlags
}
```

Material data is presentation-only as fixed by TA-4.

Collision/damage/armor behavior references separate gameplay/collision content.

## 12. glTF Material Conversion

Imported glTF metallic-roughness material semantics are translated into StarForge MaterialDefinition inputs.

A model import definition either:

- binds glTF material slots to existing Material ContentIds; or
- explicitly requests generation of a named StarForge material definition during content authoring/cook preparation.

An anonymous unstable glTF material index is never the long-term runtime/save identity.

## 13. Material Feature Closure

A MaterialDefinition can enable only features supported by its declared Shader Family.

The validator rejects:

- unsupported texture slot;
- incompatible alpha mode;
- arbitrary shader define text;
- unsupported double-sided/special-surface combination;
- feature bit with no shader-family contract.

## 14. Sampler Presets

Sampler state uses project-owned semantic presets referenced by ContentId or closed preset ID, for example:

- RepeatTrilinear;
- ClampTrilinear;
- NearestData;
- ShadowCompare;
- UIClamp.

Per-texture ad-hoc sampler mutation is not canonical authoring behavior.

Anisotropy remains TA-4 graphics-quality policy.

## 15. Environment Asset Cook

An Environment Content asset can reference equirectangular EXR or explicit cubemap source.

The offline cook produces validated runtime products such as:

- environment cubemap;
- diffuse irradiance cubemap/representation;
- prefiltered specular mip chain;
- optional project-standard BRDF integration LUT reference;
- exposure/reference metadata.

These products are presentation content and do not represent strategic world lighting physics.

## 16. Environment Determinism

Environment convolution/sample patterns are fixed/versioned by the content tool.

Worker order or GPU vendor cannot define canonical cooked IBL output.

The baseline content cooker uses a deterministic CPU/offline algorithm or a controlled deterministic build path; runtime driver-generated environment preprocessing is not build authority.

## 17. Shader Source Layout

Shader sources live under:

```text
content/source/shaders/
```

A Shader Family has one structured definition listing:

- stage source files;
- allowed feature/variant bits;
- required vertex semantics;
- expected uniform/storage blocks;
- sampler/texture bindings;
- render-pass compatibility;
- fallback/required classification.

## 18. Shader Includes

Shared GLSL include files are resolved through the content dependency graph.

Rules:

- include path is project-relative and normalized;
- cycles are build errors;
- generated include order is deterministic;
- every included file contributes to the shader bundle fingerprint;
- no workstation absolute include path.

## 19. Shader Variant Manifest

Each ShaderFamily defines a closed `ShaderVariantKey` set consistent with TA-4.

Variant bits can include real compile-time needs such as:

- skinning;
- alpha mask;
- normal map;
- double-sided specialized path;
- explicitly reviewed specialized feature.

A Material cannot invent new `#define` strings.

## 20. Offline GLSL Validation

TA-10 uses pinned Khronos **glslang** as an offline/reference GLSL validation tool for every declared shipping shader variant.

Offline validation catches syntax/interface-class errors early.

Final OpenGL runtime shader compilation/linking remains renderer/driver authority as required by TA-4; glslang output does not replace the runtime OpenGL program.

## 21. Cooked Shader Bundle

The content build emits a versioned StarForge ShaderBundle containing:

- normalized/preprocessed stage source or deterministic source package;
- ShaderFamily ContentId;
- ShaderVariantKey;
- source dependency fingerprints;
- expected interface contract metadata;
- diagnostic source mapping.

Runtime compiles/links this bundle through the renderer and then performs TA-4 interface validation.

## 22. No Shipping Runtime Shader Discovery

Shipping runtime does not recursively scan directories for `.vert/.frag` files to discover shader families.

All valid shader families/variants are registry entries in the cooked content build.

## 23. Shader Build Failure

If any required shipping variant fails offline validation, the content build fails.

Development-only optional debug shader failures can be isolated only when explicitly classified as optional and do not affect required scenes.

## 24. Shader Hot Reload Source Path

Development shader hot reload performs:

```text
source change
 -> dependency invalidation
 -> rebuild affected ShaderBundle(s)
 -> offline validation
 -> renderer candidate compile/link
 -> TA-4 interface validation
 -> frame-boundary swap
```

On failure the previous valid runtime program remains active.

## 25. Texture Hot Reload Source Path

Development texture hot reload performs:

```text
source change
 -> recook KTX2 candidate
 -> validate
 -> prepare CPU/GPU upload
 -> publish new resource generation
```

Existing runtime handles are generation-checked; stale resource references cannot silently bind a different texture allocation.

## 26. Required vs Optional Render Content

A Material/Mesh/Scene dependency declares whether the texture/material/shader is:

- required for safe/correct presentation; or
- optional enhancement with an approved fallback.

Missing collision-critical visibility geometry is never reclassified as optional merely because a fallback material exists.

## 27. Fallback Resources

TA-4 fallback resources remain project-owned.

TA-10 registry marks which optional content can legally resolve to:

- missing-texture diagnostic;
- fallback normal;
- fallback material;
- fallback shader family.

Fallbacks do not alter physical/gameplay state.

## 28. Cook Profile

Texture/shader cooking receives an explicit target profile, conceptually:

```text
windows_x64_gl46
```

The profile defines:

- permitted GPU texture formats;
- compression policy;
- optional presentation quality tiers at build time;
- shader capability baseline.

A target profile is part of the content fingerprint.

## 29. No Quality Setting in Content Identity

Runtime graphics quality can choose mip residency, shadow quality, effects, etc. under TA-4/TA-13.

The stable Texture/Material ContentId does not change merely because the user selects Low vs Ultra.

Distinct authored assets need distinct ContentIds only when they are genuinely different content definitions.

## 30. Deterministic Compression

Texture compressor/tool versions and relevant settings are pinned.

Given identical source/tool/profile, compression output must be reproducible enough for content fingerprint/build validation.

Any tool that injects timestamps or nondeterministic metadata must be normalized or excluded from fingerprint-sensitive output.

## 31. Build-Time Dependencies

TA-10 adds these content-tool dependencies behind project wrappers:

- Khronos KTX-Software for KTX2 creation/validation/runtime-compatible texture handling;
- meshoptimizer for mesh optimization/LOD (specified in `72`);
- Khronos glslang for offline GLSL validation.

Exact versions are pinned in vcpkg/tool bootstrap during TA-16 implementation locking.

## 32. Tests

Tests cover:

- sRGB/linear semantic mapping;
- BC format selection by semantic;
- normal-map canonicalization;
- deterministic mip generation;
- alpha coverage preservation tolerance;
- KTX2 validation;
- environment cubemap/convolution determinism;
- material feature validation;
- invalid sampler/slot rejection;
- shader include-cycle rejection;
- shader variant closure;
- all declared variants pass glslang;
- runtime interface metadata matches TA-4 expectations;
- failed hot reload retains previous valid asset;
- optional fallback does not mutate gameplay state.

## 33. Explicit Non-Goals

No baseline requirement for:

- runtime PNG/EXR parsing as normal shipping path;
- runtime canonical mip generation;
- mandatory Basis Universal for desktop;
- bindless textures;
- arbitrary material-authored GLSL;
- shader permutation generated from every runtime value;
- driver-specific precompiled shader binary as canonical asset;
- material textures defining collision/damage.

## 34. Dependencies

Depends on TA-1 OpenGL/toolchain, TA-4 material/shader/GPU resource architecture, TA-10 source/registry/glTF pipeline, and later TA-11 presentation/audio/UI boundaries plus TA-13 budgets.

## 35. Open Questions

None in the texture/material/environment/shader pipeline contract.
