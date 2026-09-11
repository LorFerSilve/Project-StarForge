# Renderer Ownership and Frame Pipeline

> **Status:** Architecture Complete  
> **Authority:** OpenGL context/resource ownership, render-module boundaries, immutable frame input, frame lifecycle, interpolation, renderer thread affinity, and simulation/presentation separation

## 1. Purpose

TA-4 translates the Design Complete presentation rules into a concrete OpenGL rendering architecture without making rendering a gameplay authority.

The renderer must be deterministic with respect to authoritative input but is itself allowed to be variable-rate and performance-dependent.

## 2. Renderer Module

The StarForge `render` module owns:

- OpenGL 4.6 Core calls;
- render-device/context-facing wrappers;
- GPU resource lifetime;
- shader programs;
- render targets/framebuffers;
- render passes;
- draw submission;
- lighting/shadows;
- post-processing;
- VFX GPU execution;
- development render diagnostics.

Gameplay domains never call OpenGL directly.

## 3. Thread Ownership

The main thread owns:

- GLFW window/context operations;
- all OpenGL API calls;
- authoritative simulation commits;
- render submission/execution.

Workers may prepare immutable CPU-side asset/render data but cannot create, mutate, or delete OpenGL objects.

## 4. OpenGL Context

The shipping baseline uses one primary OpenGL 4.6 Core context attached to the game window.

No multi-context rendering architecture is required.

Background GPU-upload contexts are not part of the baseline.

## 5. RenderDevice

A StarForge-owned `RenderDevice` abstraction is the only low-level layer allowed to own raw OpenGL object names and API state transitions.

It is intentionally thin.

It does not hide:

- OpenGL concepts;
- shader stages;
- framebuffer formats;
- draw calls;
- synchronization;
- resource barriers.

Its purpose is lifetime/state safety, diagnostics, and backend isolation—not emulation of a generic graphics API.

## 6. GPU Object Wrappers

Project-owned RAII wrappers represent at minimum:

- Buffer;
- VertexArray;
- Texture;
- Sampler;
- ShaderProgram;
- Framebuffer;
- Query;
- Sync/Fence where used.

Wrappers are move-only unless immutable shared ownership is explicitly implemented by the render-resource cache.

Destruction occurs on the owning OpenGL thread or through a main-thread deferred deletion queue.

## 7. No Persistent Gameplay Identity in GPU Objects

OpenGL names and render handles are process-local technical resources.

They are never:

- serialized;
- used as gameplay identity;
- written into TA-2 persistent stores.

A persistent `ShipId` or `RobotId` can be rendered by runtime proxies, but its OpenGL buffers/textures are content/render resources rather than the identity itself.

## 8. RenderSnapshot

The renderer consumes one immutable `RenderSnapshot`/equivalent built from the active SceneInstance and presentation read models after an authoritative simulation boundary.

The snapshot contains only data needed for presentation, such as:

- previous/current transform samples;
- render-mesh/material references;
- visibility/category flags;
- lights;
- camera anchors;
- environment/sky data;
- decals;
- effect intents/state;
- known interaction/highlight data;
- origin/scene epochs.

It contains no writable domain-store references.

## 9. Snapshot Generation

A new render snapshot is published after the relevant simulation phases have committed for the current tick.

The renderer never observes half-committed cross-domain state.

The snapshot carries:

- `TickIndex`;
- `SceneGeneration`;
- `OriginEpoch`;
- source state revisions where diagnostic/stale checks require them.

## 10. Variable-Rate Rendering

Authoritative simulation remains fixed at 60 Hz.

Rendering runs once per presented frame when the application is able to render.

Visual frame rate never changes:

- Simulation Time;
- combat damage;
- physics outcomes;
- production;
- travel;
- AI decisions;
- mission state.

## 11. Transform Interpolation

For eligible active local entities, render transforms interpolate between the previous and current authoritative simulation samples using the runtime accumulator interpolation factor.

Baseline renderer does not extrapolate authoritative entity motion into the future.

If an object cannot be safely interpolated—teleport-like authored relocation, scene activation, docking snap commit, origin rebase, spawn/despawn—the renderer marks a discontinuity and uses the committed current transform.

## 12. Origin Rebase Compatibility

TA-3 `OriginEpoch` is included in render input.

When OriginEpoch changes:

- old/new transforms are converted into one common presentation frame before interpolation; or
- interpolation/temporal history is reset for affected objects.

A floating-origin shift cannot produce a visible world jump or giant motion vector.

## 13. Scene Generation Compatibility

Render submissions are valid only for the current `SceneGeneration`.

GPU-upload/worker results tagged with an older scene generation can populate reusable content caches when content identity remains valid, but cannot create stale scene-local draw instances.

## 14. Frame Lifecycle

Canonical render-frame sequence:

1. poll/resolve framebuffer size and render settings;
2. drain bounded GPU upload/deletion queues;
3. acquire current immutable RenderSnapshot;
4. compute interpolated RenderView state;
5. build visibility/draw lists;
6. prepare per-frame/per-view GPU buffers;
7. execute render-pass schedule;
8. composite shipping UI through TA-11 boundary;
9. render development overlays where enabled;
10. present/swap;
11. collect GPU timings/fences and retire transient resources.

## 15. No Renderer-to-Gameplay Mutation

The renderer cannot directly:

- apply damage;
- mark enemies discovered;
- change inventory;
- complete objectives;
- move entities;
- commit selection/interactions;
- change power/atmosphere systems.

Gameplay interaction selection is validated by gameplay/physics/query systems, not by whether a mesh was drawn.

## 16. Render Proxy

An active runtime entity can expose a lightweight `RenderProxy`/component containing only presentation-facing references and flags.

Typical fields:

- Mesh/Model ContentId or runtime render-resource handle;
- Material set;
- local bounds;
- render layer/category;
- shadow flags;
- interpolation/discontinuity flags.

TA-7 owns exact runtime component layout.

## 17. Static Scene Rendering

Static authored/assembled scene geometry can use batched scene render records independent of runtime gameplay entity count.

A wall does not need a high-frequency gameplay entity solely because it is visible.

Persistent damaged/changed state selects the appropriate render representation through TA-3 world projection.

## 18. Render Layers

Baseline logical categories:

- Opaque;
- AlphaMasked;
- Transparent;
- Additive/VFX;
- Decal;
- Sky/Background;
- WorldOverlay;
- ShippingUI;
- Debug.

These are render scheduling classes, not gameplay layers or physics collision layers.

## 19. Internal Color Space

Lighting/rendering calculations operate in linear HDR color space.

Baseline main HDR target uses 16-bit floating-point color (`RGBA16F` or equivalent).

Color textures authored as perceptual color are sampled with sRGB decoding where appropriate.

Non-color data textures remain linear.

## 20. Output

Baseline shipping output is SDR sRGB.

Tone mapping converts linear HDR scene color to display output before final shipping UI composition.

Native HDR-display output is not required by the initial architecture and can be added later without changing gameplay contracts.

## 21. Depth Convention

TA-4 uses reversed-Z depth with `glClipControl(..., GL_ZERO_TO_ONE)` and floating-point depth.

Baseline perspective cameras use:

- depth clear = 0;
- depth comparison = `GREATER`/`GREATER_EQUAL` as appropriate;
- finite near plane;
- effectively infinite far projection where the view profile supports it.

This improves local-spaceflight depth precision while retaining normal player-scale near precision.

## 22. Temporal State

Renderer-owned temporal state can include:

- exposure adaptation;
- occlusion history;
- temporal effect history if introduced;
- previous camera matrices;
- previous-frame GPU query results.

It is non-authoritative and nonserialized.

Scene transition, OriginEpoch discontinuity, camera hard cut, resize, or incompatible quality-setting change invalidates relevant temporal history.

## 23. Pause

True Pause stops Simulation Time and authoritative snapshot advancement.

The renderer can continue presenting:

- paused world image;
- UI;
- cursor;
- menu animations driven by presentation real time where accessibility permits.

World simulation VFX whose meaning is tied to Simulation Time do not continue evolving as though gameplay were running.

## 24. Loading / Hard Streaming Hold

During TA-3 Transition Hold or Hard Streaming Hold, world simulation is frozen.

Renderer may present:

- last valid frame;
- loading scene/screen;
- progress indicator based on technical work;
- UI.

Rendering never advances gameplay to hide loading.

## 25. Framebuffer Size Zero

When the window framebuffer becomes zero-sized/minimized:

- no zero-sized render targets are created;
- expensive render passes can be skipped;
- simulation policy remains owned by App/Simulation, not by renderer minimization;
- resize is handled once a valid framebuffer size returns.

## 26. Diagnostics

Development builds expose:

- OpenGL debug callback/messages;
- CPU/GPU frame timing;
- pass timing;
- draw/triangle/instance counts;
- resource memory estimates;
- upload/deletion queue size;
- render target inspection;
- overdraw/wireframe/bounds modes where useful.

Dear ImGui may present these diagnostics but is not shipping UI.

## 27. Error Policy

Recoverable render-resource/content failures use explicit fallback presentation only when gameplay remains understandable.

Examples:

- missing noncritical material texture -> diagnostic fallback texture;
- missing required shader/program for current mandatory scene path -> render initialization/content error, not silent invisible geometry.

The renderer never fabricates gameplay success/failure because presentation failed.

## 28. Headless Boundary

Core gameplay tests do not instantiate RenderDevice/OpenGL.

Render integration tests are separate and explicitly require a graphics context.

## 29. Explicit Non-Goals

TA-4 does not introduce:

- a second render thread;
- multiple required GL contexts;
- renderer-owned gameplay state;
- a generic cross-API RHI;
- persistent GPU handles;
- seamless-galaxy rendering;
- Dear ImGui as shipping HUD.

## 30. Test Requirements

Tests/validation must cover:

- RenderSnapshot immutability;
- no renderer mutation of gameplay stores;
- interpolation discontinuities;
- OriginEpoch reset behavior;
- SceneGeneration stale submission rejection;
- minimized/resize behavior;
- resource destruction on owning thread;
- headless game tests requiring no OpenGL.

## 31. Dependencies

Depends on TA-1 runtime/thread ownership, TA-2 read models/revisions, TA-3 SceneInstance/origin/streaming, GDS-13 Presentation, and later TA-10/TA-11 content/UI integration.

## 32. Open Questions

None in the renderer ownership/frame-pipeline architecture.
