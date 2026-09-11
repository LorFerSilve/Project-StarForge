# GPU Resource Upload and Lifecycle

> **Status:** Architecture Complete  
> **Authority:** GPU resource identity, CPU/GPU asset handoff, upload queues, immutable/static buffers, dynamic ring buffers, synchronization, deletion, cache ownership, fallback resources, and scene/resource lifetime

## 1. Purpose

TA-4 must make GPU lifetime explicit so streamed scenes can load/unload without leaks, stale handles, gameplay stalls, or hidden ownership.

## 2. Resource Ownership

OpenGL resources are owned exclusively by the render module.

Gameplay/content systems refer to renderable assets by stable ContentId/runtime render-resource references, never raw GLuint values.

## 3. Resource Classes

Baseline render resource classes:

- Mesh GPU resource;
- Texture GPU resource;
- Sampler;
- Shader program/variant;
- Material GPU data;
- Environment/IBL resource;
- Render target;
- Dynamic frame/instance buffer;
- Particle/effect buffer.

## 4. CPU Asset Handoff

TA-10/content pipeline owns asset discovery, parsing, decoding, validation, and CPU-side runtime representation.

Workers may prepare immutable upload payloads containing, for example:

- vertex/index data;
- decoded/transcoded texture levels;
- material tables;
- environment-map data.

The renderer validates the payload and performs OpenGL upload on the main/context thread.

## 5. Upload Queue

A bounded main-thread `GpuUploadQueue` accepts immutable prepared upload jobs.

Each job carries:

- RenderAsset/Content identity;
- resource kind;
- source generation/version;
- byte size estimate;
- required/optional priority;
- scene/content lifetime relevance.

## 6. Upload Budget

The renderer drains uploads using a configurable per-frame CPU/GPU time/byte budget during normal gameplay.

Blocking TA-3 scene activation content can use an explicit loading-stage upload budget while Simulation Time is held.

The renderer does not advance gameplay merely to finish uploads.

## 7. Required vs Optional Resource

A resource is marked:

- Required for safe/correct scene activation; or
- Optional presentation enhancement.

Missing Required resources prevent the destination scene from becoming Ready when their absence would create misleading/invisible gameplay geometry.

Missing Optional resources can use a valid fallback while diagnostics record the issue.

## 8. GPU Resource State

Canonical lifecycle:

`Unloaded -> CPUPrepared -> UploadQueued -> Uploading -> Resident -> Retiring -> Destroyed`.

Failure branches return typed diagnostic state and do not expose a half-valid GPU object as Resident.

## 9. Resource Handle

Runtime renderer references use a generation-checked `RenderResourceHandle`/equivalent.

Reusing an internal cache slot increments generation so stale scene/render records cannot bind a newly unrelated resource.

RenderResourceHandle is nonpersistent and nonserialized.

## 10. Content Cache

Shared immutable assets are cached by stable asset/content identity plus relevant build/runtime variant key.

Examples:

- same mesh used by many station modules;
- same material textures across robots;
- shader program variant reused by many draws.

Scene unload decreases references/interest but need not immediately delete reusable cached resources.

## 11. Cache Eviction

Eviction is based on:

- active references;
- pending scene need;
- recent usage;
- estimated GPU memory pressure;
- resource priority.

Eviction is presentation/resource management only and cannot delete gameplay state.

## 12. Static Mesh Buffers

Mesh vertex/index data that does not need frequent CPU mutation uses immutable/static GPU storage.

OpenGL immutable storage APIs are preferred where compatible with the access pattern.

Gameplay deformation/state changes use dedicated dynamic mechanisms rather than remapping every static asset.

## 13. Dynamic Per-Frame Data

Per-frame/per-view/instance/light data uses persistently mapped or efficiently streamed ring-buffer allocations where supported by the baseline OpenGL 4.6 path.

The renderer owns synchronization so CPU never overwrites ranges still consumed by GPU.

## 14. Ring Buffer

Dynamic ring buffer is divided into frame/fence-protected regions or equivalent allocator segments.

Allocation failure under ordinary load triggers:

1. retire completed regions;
2. bounded wait only if correctness requires and budget permits;
3. fallback smaller submission/chunking where possible;
4. diagnostic/fatal configuration error if required data fundamentally exceeds designed capacity.

It never silently corrupts in-flight GPU data.

## 15. GPU Fences

OpenGL sync objects/fences are used only where CPU/GPU ownership transfer or resource retirement genuinely requires them.

The renderer avoids `glFinish` as a normal frame synchronization mechanism.

## 16. Texture Upload

Texture resources are created with explicit:

- dimensions;
- format/color-space semantic;
- mip count;
- storage;
- sampler association.

Mipmaps come from validated pipeline/runtime generation according to TA-10 policy.

## 17. Texture Streaming Boundary

TA-4 supports whole-resource and mip-residency management, but full virtual-texture/page streaming is not required in the baseline.

TA-10 may define staged mip loading if profiling/content scale requires it, using the same resource-generation/queue ownership.

## 18. Placeholder Resources

Project-owned fallback resources include at minimum:

- missing texture checker/diagnostic;
- fallback normal;
- fallback material;
- fallback noncritical mesh where technically useful.

Fallback presentation must be visually obvious in development logs/tools and must not conceal missing required collision/gameplay geometry.

## 19. Shader Resource Lifetime

Shader programs are renderer-owned and cached by family+variant.

Hot-reloaded candidate programs are compiled/validated separately.

The prior valid program remains active until the replacement is ready and swapped at a frame boundary.

Old program retires after no in-flight frame references remain.

## 20. Render Target Lifetime

Frame render targets are owned by the render-target pool rather than content cache.

They are keyed by:

- framebuffer generation;
- internal render extent;
- format;
- sample/count/layout requirements.

Resize/render-scale changes invalidate incompatible targets.

## 21. Scene-Local Render Resources

Some resources can be SceneGeneration-local:

- scene visibility buffers;
- local probe selection tables;
- transient decal buffers;
- local effect state.

They are invalidated when the owning scene is destroyed.

Shared Content resources can survive scene transitions.

## 22. Origin Rebase

GPU static mesh resources do not change on floating-origin rebase.

Only scene/object transforms and relevant per-frame spatial data are updated to the new runtime origin.

Rebase therefore does not trigger asset re-upload.

## 23. Deferred Deletion

OpenGL resources are never destroyed while commands from an in-flight frame can still reference them.

Deletion uses:

- frame retirement/fence knowledge;
- deferred destruction queue;
- main/context thread execution.

## 24. Context Shutdown

Application shutdown order guarantees:

1. local scene quiesced/deactivated;
2. render submissions stop;
3. in-flight work is retired/finished in controlled shutdown;
4. render caches/resources destroyed;
5. OpenGL context/window destroyed last.

RAII destructors must not call OpenGL after context destruction.

## 25. Context Loss Boundary

Desktop OpenGL context loss/recreation is not treated as a transparent normal gameplay event in the initial baseline.

If the active OpenGL context becomes irrecoverably invalid, the application performs controlled render-fatal handling and preserves save/session integrity where possible rather than attempting an unvalidated hot context rebuild.

Window framebuffer resize/minimize is fully supported and is not context loss.

## 26. Out-of-Memory / Allocation Failure

GPU allocation failure follows priority:

1. attempt safe eviction of unused optional cached resources;
2. retry required allocation once under controlled policy;
3. reduce optional presentation allocations only if the current graphics configuration permits a valid fallback;
4. fail scene/render configuration with explicit diagnostics if required content cannot be represented.

The renderer never substitutes invisible mandatory geometry silently.

## 27. Upload Staleness

Worker upload payloads include source identity/version and where relevant SceneGeneration.

If the request becomes stale before upload:

- reusable content payload can still populate the cache if identity/version remains desired;
- scene-local payload is discarded;
- no stale runtime render instance is activated.

## 28. Memory Accounting

Development builds estimate/report GPU memory by resource category even when exact driver allocation is unavailable.

Metrics include:

- textures;
- mesh buffers;
- dynamic buffers;
- render targets;
- shadows;
- particles/effects;
- cached unused resources.

## 29. Resource Labels

OpenGL debug labels/names are attached to significant project-owned objects in development builds where KHR_debug/core functionality permits it.

Labels include useful ContentId/pass/resource context without becoming gameplay identity.

## 30. No Gameplay Wait on Cosmetic Upload

Optional presentation resource upload cannot stall authoritative simulation.

If it is genuinely required to understand/play the destination safely, it must be classified as Required and handled by TA-3 scene staging/Hard Streaming Hold rather than an ad-hoc gameplay stall.

## 31. Determinism Boundary

GPU upload completion order cannot change:

- entity spawn;
- loot;
- enemy AI;
- collision;
- objective state;
- procedural generation.

Only presentation readiness is affected.

## 32. Test Requirements

Validation must cover:

- stale RenderResourceHandle rejection;
- required versus optional resource behavior;
- scene-local resource retirement;
- shared cache survival across scene change;
- no OpenGL destruction after context shutdown;
- deferred deletion waits for safe retirement;
- resize target invalidation;
- stale upload discard;
- upload order cannot alter gameplay state.

## 33. Explicit Non-Goals

No baseline requirement for:

- multiple upload contexts;
- virtual texturing;
- mandatory bindless textures;
- renderer asset parser ownership;
- raw GLuint outside render module;
- transparent context-loss recovery during active gameplay;
- `glFinish` every frame.

## 34. Dependencies

Depends on TA-1 main-thread OpenGL ownership, TA-2 ContentId/read-model boundaries, TA-3 scene streaming/generations, TA-4 renderer/material/pass architecture, and later TA-10 asset pipeline.

## 35. Open Questions

None in the GPU resource upload/lifecycle architecture.
