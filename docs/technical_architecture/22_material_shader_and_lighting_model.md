# Material, Shader, and Lighting Model

> **Status:** Architecture Complete  
> **Authority:** Surface-material representation, shader families/permutations, glTF alignment, light representation, PBR BRDF, image-based lighting, shadow participation, emissive semantics, and shader validation/hot reload

## 1. Purpose

StarForge requires a coherent stylized-grounded material/lighting model that is technically achievable in OpenGL while remaining compatible with the glTF asset pipeline.

## 2. Material Baseline

Baseline world materials use metallic-roughness physically based shading compatible with glTF 2.0 semantics.

Canonical material inputs:

- Base Color;
- Metallic;
- Roughness;
- Normal;
- Occlusion;
- Emissive;
- Alpha Mode;
- Alpha Cutoff;
- Double-Sided flag.

## 3. Texture Color Space

- Base Color and Emissive color textures are interpreted as sRGB/perceptual color inputs and decoded to linear space on sampling.
- Metallic/Roughness, Normal, Occlusion, masks, data maps, and lookup textures are linear.

Content metadata owns semantic texture type; artists do not manually compensate in shaders.

## 4. MaterialInstance

A `MaterialInstance`/runtime equivalent contains:

- stable Material ContentId;
- shader-family identifier;
- immutable/shared base parameters;
- texture resource references;
- bounded runtime parameter overrides where explicitly supported;
- render classification such as Opaque/Mask/Blend.

It is presentation data, not physical gameplay material authority.

## 5. Material vs Gameplay Material

Visual material does not determine collision, damage resistance, penetration, thermal state, or resource identity.

Gameplay material/armor properties are owned by their gameplay domains.

A render material can be selected from that authoritative state, but changing a texture cannot change armor rating.

## 6. Shader Language

Shipping shaders use GLSL **4.60 Core**.

Shader source is project-owned and compiled/linked by the render module.

No external shader-language runtime/compiler framework is required in the baseline beyond OpenGL/driver GLSL compilation.

## 7. Shader Families

Baseline shader families:

- PBR Deferred Opaque/Mask;
- PBR Forward Transparent;
- Shield/Energy Surface;
- Unlit/Emissive World;
- Decal;
- Particle/VFX;
- Sky/Atmosphere;
- World Overlay/Highlight;
- Post Process;
- Shipping UI boundary shader(s) later owned with TA-11;
- Debug.

A family has explicit supported features rather than one unbounded universal shader.

## 8. Shader Variants

Compile-time variants are generated from a bounded `ShaderVariantKey` built from real feature needs such as:

- skinning;
- alpha mask;
- normal map;
- double sided;
- selected specialized surface feature.

Runtime values such as roughness or light count are not separate compiled variants.

## 9. Variant Explosion Prevention

New shader-key bits require architecture/render review.

Material authoring cannot introduce arbitrary textual `#define` combinations per asset.

The renderer reports variant count and unused/permutation pressure in development diagnostics.

## 10. Shader Interface Layout

Stable semantic GPU interfaces are grouped into explicit blocks/buffers, for example:

- PerFrame;
- PerView;
- PerObject/Instance;
- MaterialData;
- LightData;
- ShadowData.

The implementation uses UBO/SSBO bindings allocated from a centralized renderer binding registry.

Shader code does not depend on ad-hoc magic binding numbers duplicated across files.

## 11. Interface Validation

At shader link/load time, the renderer validates expected:

- block bindings;
- attribute locations/semantics;
- sampler bindings;
- required outputs;
- layout sizes where introspection permits.

An incompatible mandatory shader is rejected with diagnostics.

## 12. Vertex Semantics

Baseline mesh vertex semantics support:

- position;
- normal;
- tangent;
- UV0;
- optional UV1;
- vertex color where content requires;
- joints/weights for skinned meshes.

TA-10 owns packed runtime vertex formats and asset conversion.

## 13. PBR BRDF

Baseline direct-light BRDF uses:

- GGX/Trowbridge-Reitz normal distribution;
- Smith-style geometric visibility;
- Schlick Fresnel;
- energy-conserving diffuse/specular combination.

The implementation can use optimized mathematically equivalent forms, but does not switch to unrelated per-material lighting models arbitrarily.

## 14. Stylized-Grounded Boundary

Stylization is achieved primarily through:

- authored material ranges;
- controlled light/exposure/color grading;
- simplified detail;
- silhouettes;
- restrained effects.

The baseline does not use nonphysical toon bands as the universal world-lighting model.

## 15. Light Types

Baseline runtime light types:

- Directional;
- Point;
- Spot;
- Emissive-only visual surface.

Area-light shading is not required in the baseline.

## 16. Light Representation

A render light record contains presentation-facing data such as:

- type;
- position/direction in runtime frame;
- linear color/intensity;
- range/cone;
- shadow eligibility;
- priority/importance metadata;
- optional cookie/profile reference where supported later.

Gameplay power state determines whether an authored powered light should emit; renderer does not infer operational state from visuals.

## 17. Physical Units

The architecture uses internally consistent physically-inspired light/exposure units where practical, but TA-4 does not require artists to work exclusively in strict photometric units for every effect light.

World/task lighting presets are calibrated against the common renderer/exposure pipeline.

## 18. Inverse-Square Attenuation

Point/spot direct illumination follows physically plausible inverse-square attenuation with a smooth bounded range cutoff for performance.

Range cutoff must not create an obvious hard visual boundary.

## 19. Image-Based Lighting

PBR ambient/specular response uses image-based lighting through:

- environment cubemap;
- prefiltered specular representation;
- diffuse irradiance representation;
- BRDF integration lookup.

TA-10 owns source/environment asset preparation.

## 20. Environment Probes

Scene profiles provide at least one fallback environment lighting source.

Interior/Horizon scenes can provide authored local reflection/environment probes selected by spatial influence.

Baseline probe selection blends at most a small bounded number of relevant probes rather than running a general real-time GI system.

## 21. No Real-Time Global Illumination Baseline

The baseline does not require:

- path-traced GI;
- voxel GI;
- DDGI;
- screen-space GI.

Indirect appearance is supplied by environment/probe lighting, material emissive presentation, ambient occlusion, and authored direct lights.

## 22. Emissive Surfaces

Emissive material contributes visible radiance/bloom source.

It does **not** automatically create scene illumination.

If an emissive panel must cast meaningful light, content also defines an explicit light source or other approved lighting representation.

This preserves predictable performance and system-state truthfulness.

## 23. Powered Lighting

Station/ship powered fixtures are driven by authoritative system/read-model state.

Examples:

- Operational -> normal light parameters;
- emergency -> emergency fixture/state;
- unpowered -> emission/light disabled;
- damaged/flicker -> renderer receives an explicit allowed presentation state/event.

Random decorative flicker cannot imply a system fault that does not exist.

## 24. Shadow Participation

Materials/renderables declare:

- casts shadow;
- receives shadow;
- alpha-mask shadow behavior;
- two-sided shadow requirement where justified.

Transparent blended surfaces do not universally cast full opaque shadows.

## 25. Damage and Wear

Damage/wear can affect presentation through:

- alternate material parameters;
- decals;
- mesh state;
- emissive state;
- VFX.

Persistent authoritative damage remains outside renderer.

## 26. Glass

Baseline glass uses the forward transparent path.

It supports:

- Fresnel reflection;
- bounded transmission/tint;
- environment reflection;
- optional simple screen-space refraction/distortion where enabled.

Glass never reveals geometry that should be occluded by gameplay knowledge; it simply renders physically visible scene geometry.

## 27. Shield Materials

Shield surfaces use a specialized forward shader receiving actual shield state/intents.

They can visualize:

- localized impact;
- depletion;
- recharge;
- projection boundary.

The shader never generates shield gameplay state.

## 28. Anomaly Materials

Anomaly/Fracture surfaces can use specialized distortion/emissive behavior within GDS-13 photosensitivity/readability limits.

Reduced Effects/Photosensitivity settings select reduced shader/effect parameters or fallback variants without changing world state.

## 29. Material Parameter Storage

Shared material data is GPU-buffered and indexed by render draw/instance data.

Per-object temporary parameter overrides are bounded and cannot trigger one unique material allocation per object by default.

## 30. Texture Binding Baseline

TA-4 baseline does **not** require bindless-texture extensions.

Texture/sampler binding uses core OpenGL mechanisms with batching/material organization to remain portable across the OpenGL 4.6 target.

Optional extensions can be profiled later but cannot become hidden mandatory baseline dependencies.

## 31. Samplers

Sampler objects are reusable renderer-owned resources defining filtering/wrap/aniso state separately from texture storage.

Material content references semantic sampler presets rather than mutating texture object sampling state arbitrarily.

## 32. Anisotropic Filtering

When supported by the target extension/driver, anisotropic filtering is exposed as a graphics quality option.

The renderer has a valid trilinear fallback and does not require anisotropy for correctness.

## 33. Shader Hot Reload

Development builds support shader hot reload through the render/content development pipeline.

Rules:

1. compile/link candidate program separately;
2. validate interface contract;
3. on success, swap at a frame boundary;
4. on failure, keep the previous valid program and report diagnostics.

Hot reload is not a shipping gameplay dependency.

## 34. Shader Cache

The renderer can retain compiled/linked program objects per variant for the process lifetime/resource cache.

Persistent driver-binary shader caches are optional optimization and are not required for first implementation correctness.

## 35. Color Grading

Post-lighting color grading uses a project-owned bounded grading stage/LUT where art direction requires it.

Accessibility/high-readability requirements take precedence over a cinematic grade that obscures gameplay.

## 36. Test Requirements

Validation must cover:

- sRGB versus linear texture semantics;
- shader-interface mismatch rejection;
- bounded variant generation;
- powered-light state truthfulness;
- emissive surface not automatically becoming gameplay/world light;
- material alpha classification;
- reduced-effects/anomaly material path;
- missing texture fallback without gameplay mutation.

## 37. Explicit Non-Goals

No baseline requirement for:

- universal toon shading;
- real-time GI;
- ray tracing;
- mandatory bindless textures;
- arbitrary per-asset shader source;
- material-driven gameplay physics/damage;
- emissive geometry automatically producing dynamic lights.

## 38. Dependencies

Depends on TA-4 render graph/camera, TA-3 SceneProfiles/environment, TA-10 glTF/content pipeline, GDS-13 Visual/VFX/Accessibility, and gameplay read models for powered/damaged state.

## 39. Open Questions

None in the material/shader/lighting architecture.
