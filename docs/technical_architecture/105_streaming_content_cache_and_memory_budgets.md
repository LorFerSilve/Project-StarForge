# TA-13 — Streaming, Content Cache, and Memory Budgets

> **Status:** Architecture Complete  
> **Authority:** CPU/GPU residency budgets, stream I/O/decode/upload limits, prefetch/eviction policy, memory-pressure response, Hard Streaming Hold performance targets, and cache ownership

## 1. Purpose

TA-3 and TA-10 separate technical residency from gameplay activation. TA-13 now fixes the initial memory/streaming envelope so that scene traversal can be predictable without letting storage speed or cache policy change gameplay.

## 2. CPU Memory Budget Table

64-bit runtime process soft targets:

| Category | Low | Medium | High | Ultra |
|---|---:|---:|---:|---:|
| Total process working set | 3.0 GiB | 3.5 GiB | 4.5 GiB | 6.0 GiB |
| Immutable CPU ContentCache | 512 MiB | 768 MiB | 1.25 GiB | 2.0 GiB |
| Active scene technical/runtime data | 768 MiB | 1.0 GiB | 1.25 GiB | 1.5 GiB |
| Streaming transient decode/staging | 256 MiB | 384 MiB | 512 MiB | 768 MiB |

The process-wide safety ceiling remains **8.0 GiB** before controlled memory-pressure handling refuses additional optional residency.

## 3. Dedicated Runtime State Reserve

Within process memory, at least **512 MiB** of headroom is reserved conceptually for authoritative gameplay/runtime structures, workers, persistence and system overhead rather than allowing ContentCache to consume every available byte.

Caches are first-class eviction candidates; authoritative state is not.

## 4. GPU Residency Budgets

Configured targets:

| Preset | GPU Resident Content Target |
|---|---:|
| Low | 1.5 GiB |
| Medium | 2.0 GiB |
| High | 3.0 GiB |
| Ultra | 4.5 GiB |

These include textures, meshes, render targets, shadow resources, particle buffers and other renderer allocations under TA-4 ownership.

## 5. GPU Budget Accounting

The renderer tracks project-estimated bytes for all owned resources even where the OpenGL driver does not expose exact physical VRAM residency.

Telemetry distinguishes at minimum:

- textures;
- mesh/index buffers;
- render targets/G-buffer/depth;
- shadows;
- UI/text atlases;
- particles/VFX;
- transient upload/staging resources.

## 6. Required Content Priority

Memory priority order is:

1. currently required gameplay/collision/navigation/content;
2. active-scene mandatory render/audio presentation;
3. near-future predictive prefetch;
4. reusable shared cache;
5. decorative/far-detail optional residency.

Eviction walks the reverse order subject to in-flight safety.

## 7. Streaming I/O Concurrency

Normal live gameplay supports:

- up to **8 concurrent file reads**;
- up to **4 heavy decode/CPU preparation jobs**;
- up to **64 queued stream-cell/content requests** in `Required` state;
- up to **256 speculative/predictive requests** before backpressure/coalescing.

A required request is never discarded because speculative requests consumed the queue.

## 8. Live GPU Upload Budget

During ordinary gameplay, main-thread OpenGL upload/finalization targets:

- upload bytes: **<= 8 MiB/rendered frame p95**;
- CPU submission/finalization: **<= 1.0 ms/frame p95**;
- individual large resources are chunked where feasible.

During explicit blocking loading/Hard Streaming Hold, the application may raise the upload batch target to **64 MiB/frame** and spend up to **8 ms** main-thread upload/finalization per presentation iteration because authoritative Simulation Time is stopped.

## 9. Streaming Transient Hard Cap

CPU streaming decode/upload staging has a hard safety cap of **1.0 GiB** across all presets.

Before crossing it:

- stop new speculative reads;
- release completed staging buffers;
- serialize/defer low-priority decodes;
- if required work still cannot fit, hold/fail clearly rather than allocating without bound.

## 10. Prefetch Lead-Time Targets

Predictive streaming aims for required content to be ready at least:

- **2 seconds** of expected player travel before boundary crossing for ordinary interior/surface movement;
- **4 seconds** for high-speed local-spaceflight approaches;
- one portal/adjacency layer beyond the immediate required set for ordinary interiors where memory permits.

These are technical prefetch targets and do not activate gameplay early.

## 11. Cell Size Guidance

Stream cells should be authored/cooked so one ordinary cell's unique required technical residency does not exceed:

- CPU cooked/decoded resident payload target: **64 MiB**;
- GPU unique resident payload target: **128 MiB**.

Large landmarks may exceed a single-cell target only with shared/chunked assets and explicit benchmark validation.

## 12. Activation Cost

Live streaming activation/deactivation targets from TA-13 runtime budgets:

- structural entity/proxy activation/deactivation: **<= 1.5 ms main-thread/frame p95**;
- ordinary activation batch: **<= 128 runtime entities/rendered frame**;
- collision/nav registration is included in owning subsystem telemetry.

Large initial scene activation occurs under loading/hold and uses separate loading targets.

## 13. Eviction Delay

Inactive resident content is not immediately thrashed out after crossing a boundary.

Default technical minimum eviction hysteresis target: **5 seconds of Presentation Time** since last residency need, unless Critical memory pressure requires earlier eviction.

This timer is technical only and never serialized/gameplay-authoritative.

## 14. ContentCache Policy

The immutable CPU ContentCache:

- keys by ContentId/registry generation and cooked product identity;
- uses reference/pin counts for active consumers;
- tracks estimated bytes and recency;
- can evict unpinned immutable entries;
- never owns persistent gameplay state;
- invalidates generation-sensitive entries on content hot reload.

## 15. Texture Residency

Texture-quality settings can control mip residency within the fixed TA-4 semantics.

Under memory pressure the runtime may drop high-detail mips first, but cannot change:

- collision;
- scanner knowledge;
- material gameplay semantics;
- authored visibility/interaction state.

## 16. Mesh LOD Residency

The renderer can evict non-required high-detail mesh LODs while retaining sufficient valid representation for current visibility/quality policy.

Physics/nav products are separate cooked assets and are not tied to render LOD eviction.

## 17. Audio Content Residency

Audio-specific resident/stream buffers are accounted separately in TA-13 presentation budgets but also count toward process memory. Large music/dialogue tracks prefer streaming rather than full PCM residency.

## 18. Navigation/Terrain Residency

Cooked nav/terrain data follows active/prefetch scene topology and can be evicted only when no active actor/path/publication depends on the generation.

A nav tile or terrain chunk cannot be removed merely because it is currently off-camera.

## 19. Hard Streaming Hold Targets

On the TA-16 reference PC and required benchmark traversal routes:

- warm traversal target: **0 Hard Streaming Holds**;
- any unexpected hold > **250 ms** is a warning;
- any unexpected hold > **500 ms** fails the representative streaming acceptance scene;
- explicit authored scene-transition loading screens are measured separately.

Correctness still wins if a hold is necessary on slower hardware.

## 20. Cold Load Targets

Cold-load measurements begin from empty project caches and include storage/read/decode/backend preparation.

TA-13 architecture targets:

- first required local-context content-ready state: **<= 10 seconds p95** on the TA-16 reference storage/hardware for benchmark scenes;
- subsequent warm re-entry: **<= 3 seconds p95** for an equivalent context when reusable cache remains.

Final shipping loading targets can be tightened after implementation profiling but cannot be loosened by changing gameplay semantics.

## 21. Memory Pressure Response

When CPU/GPU memory enters Red/Critical:

1. cancel speculative prefetch;
2. evict unpinned inactive CPU cache entries;
3. evict inactive GPU resources;
4. reduce allowed presentation mip/LOD/detail inside current quality tier;
5. trim decorative VFX/audio cache;
6. defer new optional residency;
7. if mandatory content still cannot fit, enter a controlled loading/activation failure or Hold.

## 22. No Semantic Eviction

Memory pressure cannot:

- erase persistent loot;
- despawn required active actors;
- reset a cell;
- discard a projectile with gameplay consequence;
- remove collision/navigation needed by current actors;
- forget mission/objective state.

Only technical representations/caches can be evicted.

## 23. Fragmentation / Allocation Strategy

Large long-lived content allocations should use subsystem-owned allocators/pools where measurement justifies it. TA-13 does not require a universal custom allocator.

Telemetry records committed/resident bytes and allocation churn before introducing specialized memory infrastructure.

## 24. Development Leak Gate

Repeated scene enter/leave benchmark loops must return cache/runtime memory to a stable plateau. Monotonic growth after **10 identical transition cycles** is a leak/regression failure unless explicitly attributable to a bounded warming cache that reaches its configured cap.

## 25. Diagnostics

Streaming/memory overlays expose:

- process/project pool bytes;
- GPU estimated bytes by category;
- cache hit/miss/eviction rates;
- required/speculative queue depths;
- read/decode/upload latency;
- cell resident/active states;
- prefetch lead time;
- Hard Streaming Hold cause/duration;
- in-flight staging bytes.

## 26. Non-Goals

No seamless infinite-world promise, no portable exact VRAM-free query requirement, no runtime recooking, no gameplay changes based on memory pressure, no raw source fallback.

## 27. Open Questions

None in the TA-13 streaming/content-cache/memory budget architecture.