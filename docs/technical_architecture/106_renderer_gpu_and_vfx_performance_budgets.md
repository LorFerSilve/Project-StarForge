# TA-13 — Renderer, GPU, and VFX Performance Budgets

> **Status:** Architecture Complete  
> **Authority:** Render CPU/GPU targets, visible-scene complexity envelopes, draw/light/shadow/particle budgets, render-snapshot memory, and graphics-tier degradation policy

## 1. Purpose

TA-4 defines the renderer architecture. TA-13 defines the initial measurable complexity envelope for the hybrid deferred/forward renderer without making visual budgets gameplay authority.

## 2. Reference Render Target

Primary acceptance target on TA-16 reference hardware:

- output: **1920×1080**;
- graphics preset: **High**;
- render scale: **100%**;
- VSync timing measured both Off and On where useful;
- world simulation at fixed 60 Hz.

## 3. GPU Frame Targets

Representative High-preset benchmark scenes:

- GPU p50: **<= 10.0 ms**;
- GPU p95: **<= 13.5 ms**;
- GPU p99: **<= 16.0 ms**;
- repeated frames >16.67 ms are a performance regression unless the scene is explicitly a stress/failure test.

## 4. CPU Render Targets

Main-thread render work targets:

- RenderSnapshot extraction/visibility preparation: **<= 1.5 ms p95**;
- draw/pass submission: **<= 2.5 ms p95**;
- total renderer CPU contribution: **<= 4.0 ms p95**;
- avoid repeated GPU synchronization stalls > **1.0 ms p95**.

## 5. RenderSnapshot Memory

The two stable simulation presentation states plus current render extraction data target:

- ordinary combined CPU memory: **<= 64 MiB**;
- semantic hard safety cap: **256 MiB**.

Large immutable shared mesh/material data stays in ContentCache/render resources rather than being copied into each snapshot.

## 6. Visible Geometry Envelope

High-preset target per view:

- submitted visible instances: **<= 100,000**;
- visible triangles after LOD/culling: **<= 5 million** ordinary target;
- draw submissions across all passes: **<= 4,000/frame**;
- distinct opaque/alpha-masked material batches: **<= 1,500/frame**.

Instancing/indirect batching can reduce submission count without changing persistent identity.

## 7. Low/Medium/Ultra Scaling

Presentation complexity targets scale approximately:

| Metric | Low | Medium | High | Ultra |
|---|---:|---:|---:|---:|
| Visible triangle target | 2.0 M | 3.0 M | 5.0 M | 7.5 M |
| Draw submission target | 2,500 | 3,250 | 4,000 | 5,000 |
| GPU particle live target | 50k | 100k | 250k | 500k |
| Shadowed local lights | 4 | 6 | 8 | 12 |

These are presentation budgets only. Gameplay geometry/collision/knowledge remains unchanged.

## 8. Light Budget

High-preset visible local-light envelope:

- point/spot lights considered after coarse culling: **<= 512**;
- lights reaching tiled lighting lists: **<= 256 visible relevant lights** ordinary target;
- concurrently shadowed local lights: **<= 8** High;
- primary directional light cascades: fixed **4** from TA-4.

Lights beyond shadow budget remain lit without a fresh real-time shadow allocation according to deterministic presentation priority.

## 9. Tiled-Light List Safety

Each screen tile has a bounded light-index list. Initial semantic presentation safety cap is **256 light indices/tile**.

If an authored scene exceeds it:

- development builds diagnose the offending tile/lights;
- renderer applies documented presentation-only light prioritization for overflow;
- gameplay systems never query this reduced list as world truth.

## 10. Transparency Budget

High target:

- ordinary sorted transparent draw submissions: **<= 512/frame**;
- high-cost refractive/shield special surfaces: **<= 128 visible/frame**;
- transparent sorting CPU: **<= 0.5 ms p95**.

No baseline universal OIT is required.

## 11. Decal Budget

Visible/deferred decals target **<= 1,024/frame** High.

Persistent gameplay facts such as damage remain stored in owning domains; decals can expire/aggregate for presentation without erasing damage state.

## 12. Particle/VFX Budget

High-preset:

- live GPU presentation particles: **<= 250,000**;
- CPU-side active emitters: **<= 512**;
- new particle spawns: **<= 50,000/frame** ordinary burst target;
- CPU VFX preparation: **<= 0.75 ms p95**;
- GPU VFX/particle work: **<= 2.0 ms p95** in combat benchmark scenes.

Critical effect readability retains its TA-4/TA-11 floor at every quality tier.

## 13. Skinned Rendering

High target:

- visible skinned actors: **<= 128**;
- bones per rendered skeleton semantic content cap: **256**;
- total visible bone transforms uploaded/evaluated: **<= 24,576/frame** ordinary target.

Animation presentation may use distance-based update interpolation where visual-only and knowledge-safe; gameplay actor state remains 60 Hz authority.

## 14. Shadow Budgets

Shadow settings scale:

- CSM cascade count remains 4; resolution/distance varies by preset;
- local shadow-map updates are budgeted by projected importance and dirty state;
- High target local shadow rendering: **<= 2.5 ms GPU p95**;
- unchanged static shadow data can be cached where valid.

A shadow budget overrun never disables the underlying light/gameplay state.

## 15. Post-Processing Budget

At 1080p High, SSAO + bloom + tone map + FXAA combined GPU target: **<= 2.5 ms p95**.

Accessibility overrides may reduce effects further; required readability cannot depend on these passes.

## 16. UI Composition Budget

Native-resolution shipping UI composition GPU target: **<= 0.8 ms p95** at 1080p, excluding CPU layout/text work budgeted in TA-13 presentation spec.

UI is composited after tone mapping and is never downscaled with 3D render scale.

## 17. Dynamic Buffering

Per-frame dynamic GPU data uses bounded ring/fence-managed buffers. Ordinary dynamic upload target, excluding streamed asset uploads:

- transforms/material instance/light/UI/VFX dynamic data: **<= 32 MiB/frame** High.

Buffer growth is capped and diagnosed rather than reallocating unboundedly every frame.

## 18. Shader/Material Residency

The active scene target:

- resident linked shader programs/declared variants: **<= 512**;
- active material instances: **<= 8,192**;
- distinct material definitions referenced by active scene: **<= 2,048**.

TA-10 content validation should prevent combinatorial shader-family explosion.

## 19. Render-Target Memory

At 1080p High, render-target/depth/G-buffer/lighting/post-process resources target **<= 512 MiB** total project-estimated GPU memory, including resize generations waiting on safe retirement.

Retired generations must drain; repeated resize cannot accumulate them indefinitely.

## 20. GPU Upload Interaction

Live streamed asset uploads obey TA-13's **8 MiB/frame** normal budget and 1.0 ms CPU finalization target. Render-critical mandatory content not ready triggers TA-3 loading/hold semantics rather than placeholder gameplay geometry.

## 21. Quality Pressure Response

Allowed presentation-only responses include:

- lower texture mip residency;
- earlier visual LOD transition within preset policy;
- fewer shadowed local lights;
- reduced decorative particle density;
- SSAO/bloom reduction where setting/accessibility permits;
- eviction of inactive GPU cache resources.

Not allowed:

- remove a gameplay actor;
- remove collision;
- hide a known critical objective/hazard;
- change actual light/power state;
- alter AI visibility/perception.

## 22. Performance Scene Coverage

Renderer certification includes:

- dense powered Horizon interior;
- emergency-light Horizon with smoke/fire;
- open surface exterior;
- local-space ship battle;
- mixed boarding/EVA/interior raid;
- UI-heavy station management view.

## 23. Metrics

Frame telemetry records per-pass CPU/GPU time, draws, triangles, visible instances, lights, shadow updates, transparency, particles, skinned actors, render-target bytes, GPU resource bytes and synchronization stalls.

## 24. Non-Goals

No ray tracing/GI budget, no TAA history budget, no automatic dynamic resolution, no hardware-pressure gameplay culling, no exact driver-VRAM reliance.

## 25. Open Questions

None in the TA-13 renderer/GPU/VFX budget architecture.