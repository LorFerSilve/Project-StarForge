# Render Graph and Pass Architecture

> **Status:** Architecture Complete  
> **Authority:** Frame-pass ordering, render-target ownership, deferred/forward split, shadow/decal/post-processing schedule, synchronization barriers, and pass-resource dependencies

## 1. Purpose

StarForge uses a fixed-purpose render graph appropriate to the game's scenes rather than a general scripting/render-graph framework.

## 2. Rendering Strategy

Baseline architecture is **hybrid deferred rendering**:

- Opaque and AlphaMasked geometry -> G-buffer;
- lighting -> deferred lighting pass;
- Transparent, shield/glass, and selected special materials -> forward HDR pass;
- particles/VFX -> forward/additive passes;
- shipping UI -> native-resolution final composition.

## 3. Why Hybrid Deferred

This supports:

- many local station/interior lights;
- strong material readability;
- decals/damage projection;
- predictable opaque lighting cost;
- separate transparent/shield treatment;
- direct OpenGL ownership without a heavyweight engine abstraction.

## 4. Fixed Pass Schedule

Canonical baseline frame order:

1. GPU upload/deletion drain;
2. visibility/draw-list preparation;
3. shadow-map updates;
4. G-buffer opaque/alpha-mask pass;
5. deferred decals;
6. screen-space ambient occlusion where enabled;
7. tiled local-light list build;
8. deferred lighting into HDR scene target;
9. sky/background composition;
10. forward special opaque/transparent surfaces;
11. forward transparent geometry;
12. particles/VFX/additive effects;
13. world-space overlays/highlights;
14. bloom chain where enabled;
15. tone mapping;
16. anti-aliasing;
17. shipping UI composition at output resolution;
18. development/debug overlay;
19. present.

A pass can be skipped when it has no work or its feature is disabled, provided dependencies remain valid.

## 5. Lightweight Render Graph

The renderer maintains a StarForge-owned pass/resource dependency description for:

- transient render targets;
- read/write relationships;
- pass ordering;
- resize/recreation;
- diagnostics.

It is not a runtime programmable graph system.

Passes are compiled/configured by C++ renderer code.

## 6. Main G-Buffer

Baseline G-buffer stores sufficient data for PBR opaque lighting:

- depth: `D32F`;
- normal + roughness;
- base-color + metallic;
- emissive + material/auxiliary data as required;
- optional velocity/ID attachment only when a shipping/debug feature explicitly requires it.

Exact packed formats are implementation-lock data, but semantics above are fixed.

## 7. Alpha-Masked Geometry

AlphaMask materials execute in the opaque/G-buffer path with deterministic cutoff from material data.

They write depth and participate in normal deferred lighting/shadows.

## 8. Deferred Decals

Damage, wear, signage overlays, and authored local surface changes that fit decal semantics can be applied after G-buffer geometry and before lighting.

Decals are presentation of persistent/authored state, not their authority.

A decal cannot create damage state merely because it renders.

## 9. Tiled Deferred Lighting

Local dynamic lights are culled into screen-space tiles using compute-shader processing and depth bounds.

Baseline tile size target is 16x16 pixels and remains tuneable only as a performance parameter.

Per-tile light lists are stored in GPU buffers/SSBOs.

The algorithm cannot change whether a gameplay light source exists; it only optimizes which visible pixels evaluate it.

## 10. Primary Directional Light

A scene profile can expose one primary directional/star light for outdoor/space-facing lighting.

It is evaluated separately from local tiled point/spot lights.

Interior-only scenes can omit it.

## 11. Shadow Passes

Shadow maps render before the main G-buffer/lighting pass for lights selected by the shadow-budget policy.

Shadow selection is presentation-only and cannot hide gameplay-critical geometry or collision.

## 12. Directional Shadows

Primary directional light uses Cascaded Shadow Maps.

Baseline architecture uses four cascades with stable camera-relative splitting and texel snapping/stabilization.

Exact split distances/resolutions belong to graphics quality settings.

## 13. Local-Light Shadows

Shadow-capable local lights use:

- 2D shadow atlas for spot/directional-like local projections;
- cubemap/cubemap-array representation for selected point lights.

Only a bounded prioritized set receives real-time local shadows each frame/update.

Priority considers view relevance, projected size, distance, gameplay/presentation importance, and change state.

## 14. Shadow Filtering

Baseline shadow filtering uses bounded PCF-style filtering.

Advanced stochastic/ray-traced shadows are not required.

## 15. Shadow Caching

Static or unchanged local-light shadow results may be cached until:

- caster geometry changes;
- light transform/state changes;
- relevant stream cell changes;
- quality/atlas layout invalidates cache.

Cached shadows are presentation state only.

## 16. SSAO

Screen-space ambient occlusion is an optional graphics feature operating from depth/normal data.

It enhances local contact readability but is never required to perceive a gameplay object/hazard.

Reduced Effects/Low quality can disable it.

## 17. Sky / Background

Sky/background rendering occurs after deferred lighting/depth creation and respects SceneProfile.

Possible inputs include:

- authored skybox/cubemap;
- starfield/background;
- atmospheric sky representation for surface scenes.

TA-10 owns content asset format/import details.

## 18. Forward Special Materials

Materials not well represented by the deferred G-buffer can use a forward HDR path.

Examples:

- shield surfaces;
- refractive glass;
- holographic displays;
- selected energy/anomaly surfaces.

They consume the same scene light/environment data where appropriate.

## 19. Transparency

Transparent geometry is excluded from deferred opaque G-buffer lighting.

Baseline classes:

- AlphaBlend;
- Additive;
- Refractive/Special.

Standard AlphaBlend geometry is sorted back-to-front using view-space depth with deterministic stable tie-breakers.

Additive effects need not be depth-sorted where blending semantics make order irrelevant.

## 20. Depth Interaction for Transparency

Transparent geometry depth-tests against opaque depth but normally does not write the primary depth buffer.

Special materials can use explicit depth-write/prepass behavior only through a declared shader/material family.

## 21. No Universal Order-Independent Transparency Baseline

Weighted blended OIT or per-pixel linked lists are not required in the baseline.

If later profiling/art proves conventional sorting insufficient for a specific feature, that feature requires an architecture decision rather than silently replacing the transparency model.

## 22. Particles and VFX

Particle/VFX rendering occurs after major transparent world surfaces unless an effect family explicitly requires earlier distortion/depth interaction.

Gameplay-critical projectile/hazard state remains in simulation; GPU particles are presentation only.

## 23. World Overlays

Known/eligible interaction highlights, scanner overlays, objective visualization, and accessibility highlighting render through explicit world-overlay passes.

They receive only Presentation-Knowledge-compliant data.

They cannot query hidden simulation state directly.

## 24. Bloom

Bloom operates on HDR scene luminance before tone mapping.

It is restrained and user/quality adjustable.

Reduced Effects and Photosensitivity modes can reduce/disable bloom without changing gameplay state.

## 25. Tone Mapping

Baseline uses a project-owned filmic/ACES-style tone-mapping curve from HDR scene color to SDR output.

Exact coefficients/exposure defaults are presentation tuning data.

Tone mapping occurs before shipping UI so UI colors/text remain predictable.

## 26. Anti-Aliasing

Baseline shipping anti-aliasing is **FXAA** after tone mapping.

Reasons:

- simple and stable with deferred rendering;
- no gameplay temporal history dependency;
- low implementation risk;
- compatible with floating-origin and mixed scene profiles.

AA can be Off or FXAA in the initial baseline.

TAA/MSAA are not required by TA-4 baseline and need a later explicit architecture decision if added.

## 27. Motion Blur

Motion blur is not implemented in the baseline renderer.

This satisfies the GDS requirement that motion blur is optional and Off by default while avoiding unnecessary temporal complexity.

## 28. UI Composition

The shipping UI is composed after world tone mapping/AA at native framebuffer/output resolution.

UI is not rendered at reduced 3D render scale.

TA-11 owns UI layout, glyph rendering, focus/navigation, and input semantics.

## 29. Debug Pass

Development-only debug geometry/UI can render after shipping world/UI composition as appropriate.

It can visualize:

- bounds;
- lights;
- shadow frusta;
- stream cells;
- physics shapes later;
- origin axes;
- overdraw/culling information.

Debug visibility never becomes gameplay knowledge in shipping builds.

## 30. Render-Target Lifetime

Transient render targets are owned by a render-target pool keyed by compatible description and framebuffer generation.

Targets are recreated/invalidated on:

- framebuffer resize;
- render-scale change;
- format/quality change requiring a new layout.

Old resources retire safely through the render deletion/fence policy.

## 31. Memory Barriers

Compute/SSBO/image stages use explicit OpenGL memory barriers between producer and dependent consumer passes.

Barrier placement is owned by the render-pass resource dependency layer, not scattered arbitrarily through gameplay code.

## 32. Framebuffer Completeness

Every framebuffer configuration is validated in development builds.

An incomplete mandatory framebuffer is a render initialization/reconfiguration failure, not a reason to draw silently missing scene content.

## 33. Pass Failure

Optional post-process pass failure can disable that optional feature with diagnostics if the base scene remains valid.

Failure of mandatory passes such as depth/G-buffer/deferred lighting/tone-map output is fatal to the current render configuration and triggers typed recovery/error handling.

## 34. Test Requirements

Validation must cover:

- pass dependency ordering;
- resize target recreation;
- optional pass skip paths;
- transparent stable sorting;
- shadow cache invalidation;
- knowledge-safe overlay input;
- HDR -> tone map -> native UI ordering;
- barrier/resource-state diagnostics.

## 35. Explicit Non-Goals

TA-4 does not require:

- runtime graph scripting;
- ray tracing;
- path tracing;
- real-time global illumination;
- TAA;
- MSAA;
- universal order-independent transparency;
- cinematic motion blur.

## 36. Dependencies

Depends on renderer ownership/frame pipeline, TA-3 scene profiles, GDS-13 Visual/VFX/Accessibility, and later TA-10 content/TA-11 UI.

## 37. Open Questions

None in the render graph/pass architecture.
