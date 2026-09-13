# TA-13 — Profiling, Telemetry, Benchmark Scenes, and Degradation Policy

> **Status:** Architecture Complete  
> **Authority:** Runtime profiler data model, benchmark scene matrix, percentile methodology, regression thresholds, trace capture, leak/stall diagnostics, and allowed degradation order

## 1. Purpose

Numeric budgets are useful only when they are measured consistently against representative workloads. TA-13 therefore defines one project-owned profiling vocabulary and benchmark matrix consumed later by TA-14 CI and TA-16 implementation milestones.

## 2. Profiler Principle

Development profiling must answer:

- what consumed CPU/GPU time;
- what consumed memory;
- what queued/stalled;
- whether simulation fell behind;
- what content/identity caused the cost;
- whether pressure triggered a technical degradation/hold.

A single total-frame number is insufficient.

## 3. Timing Sources

CPU timings use a monotonic high-resolution platform clock.

GPU timings use OpenGL timer queries/fences through TA-4 without blocking every frame for immediate results.

Simulation Time is never used as a profiler wall-clock source.

## 4. Core Frame/Simulation Metrics

Every development build can capture:

- wall frame duration;
- simulation tick duration;
- simulation accumulator/backlog ticks;
- catch-up tick count;
- main-thread phase timings;
- worker queue depths/ages/utilization;
- render CPU/GPU pass timings;
- present/VSync wait;
- memory/residency categories;
- streaming read/decode/upload timing;
- audio callback utilization/underruns;
- save/load/build metrics when active.

## 5. Domain Metrics

At minimum separate counters/timers exist for:

- runtime entities/components/lifecycle;
- Jolt bodies/contacts/queries;
- station graph solvers/scheduler boundaries;
- navigation requests/rebuilds;
- AI perception/decision work;
- missions/objectives/strategic events;
- content cache/stream cells;
- UI/text/markers/presentation events;
- audio voices/streams/occlusion;
- persistence snapshot/encode/load;
- VFX/particles/animation.

## 6. Stable Identity in Traces

Development traces can include safe project identifiers such as ContentId, typed persistent ID raw value plus type, scene/cell key, subsystem and generation/revision.

Pointers/backend handles are diagnostic-only and never replace semantic identity in traces.

## 7. Sampling Window

Steady-state benchmark runs use:

- **30 seconds warm-up** not included in percentile pass/fail;
- **120 seconds measured runtime** minimum;
- at least **3 repeated runs** per scenario/configuration;
- report worst run and aggregate distribution.

Short microbenchmarks supplement but do not replace scenario runs.

## 8. Frame-Time Acceptance

For 1080p High reference runs:

- main-thread p95 <= TA-13 **12.0 ms** target;
- GPU p95 <= **13.5 ms**;
- simulation tick p95 <= **4.0 ms**;
- no sustained Severe/Critical simulation backlog;
- no semantic hard-cap violation.

Averages are informational only.

## 9. Performance Regression Threshold

Once a benchmark has an accepted baseline implementation, a change is flagged when any critical metric:

- regresses p95 by **>10%** and at least **0.25 ms**;
- regresses p99 by **>15%** and at least **0.5 ms**;
- adds a new hard-cap violation;
- adds a new unexpected Hard Streaming Hold;
- increases peak memory by **>10% or 128 MiB**, whichever is larger, without explicit accepted reason.

TA-14 decides which flags block CI automatically.

## 10. Benchmark P1 — Dense Horizon Operations

Representative Horizon scene includes:

- dense modular interior;
- crew and robot movement;
- active Power/Atmosphere/Thermal/Water/Logistics;
- farming/manufacturing/repair work;
- many station lights;
- management HUD/read models;
- doors/elevators/navigation invalidation.

It targets the upper ordinary TA-6 scale rather than an empty starter base.

## 11. Benchmark P2 — Horizon Emergency / Boarding Defense

Includes:

- active DefenseEvent;
- combat actors and robots;
- decompression/fire/damage;
- doors/breaches/topology mutation;
- local alarms/audio/VFX;
- reinforcement/strategic scheduling.

This stresses physics + station + AI + presentation simultaneously.

## 12. Benchmark P3 — Surface Expedition Combat

Includes:

- streamed terrain/surface cells;
- resource/POI interactions;
- enemy AI/perception;
- projectiles/status effects;
- environmental hazards;
- objective progression;
- predictive streaming under player traversal.

## 13. Benchmark P4 — Local Spaceflight Battle

Includes:

- multiple Dynamic spacecraft;
- 6DoF control;
- ship weapons/projectiles;
- many lights/VFX/particles;
- local-space streaming;
- docking/approach geometry nearby.

It validates TA-5/TA-4 high-speed local context budgets.

## 14. Benchmark P5 — Mixed Boarding Mission

One MissionInstance traverses:

```text
Local Spaceflight -> EVA -> Exterior -> Breach -> Interior Combat -> Extraction
```

It stresses scene-profile transitions, streaming, floating origin, nav representation changes, AI, physics and objective state without changing MissionInstance identity.

## 15. Benchmark P6 — External Mission + Off-Screen Horizon

While a demanding external mission runs, Horizon concurrently executes:

- production;
- environmental networks;
- automation;
- crew/robot logical tasks;
- a scheduled/emergency event where appropriate.

This benchmark proves off-screen persistent simulation cost remains bounded without changing outcomes.

## 16. Benchmark P7 — Streaming Traversal

Cold and warm scripted routes traverse:

- dense interior portals;
- surface cell boundaries;
- high-speed spaceflight interest volumes;
- mixed-zone transitions.

Measurements include prefetch lead, queue depths, memory peaks, activation cost and Hard Streaming Holds.

Warm reference route acceptance target is **zero unexpected Hard Streaming Holds**.

## 17. Benchmark P8 — Save/Load Stress

Synthetic but semantically valid campaign state approaches TA-13 persistence/state scale.

Measures:

- Stable Save Boundary capture;
- snapshot peak memory;
- encode/CRC/write/commit;
- catalog behavior;
- cold load/decode/migration/validation;
- active-context reconstruction;
- gameplay state hash/equivalence after load.

## 18. Benchmark P9 — UI / Audio / Presentation Stress

Includes:

- large virtualized management tables;
- dense but knowledge-valid markers/alarms;
- subtitles/captions;
- many audio emitters and streamed tracks;
- VFX/animation density;
- accessibility modes.

Presentation pressure must not alter gameplay/AI hearing.

## 19. Benchmark P10 — Content Build

Offline content benchmark includes:

- no-op dependency scan;
- single `.sfdef.json` recook;
- texture recook;
- mesh + LOD recook;
- navigation/collision recook;
- clean representative content subset.

Outputs/fingerprints must remain deterministic across worker counts.

## 20. Deterministic Scenario Inputs

Performance scenarios use fixed seeds, content revisions and scripted input timelines so timing comparisons run equivalent gameplay.

Performance instrumentation cannot inject commands that alter scenario semantics differently between runs.

## 21. State Equivalence Hashes

Headless/development benchmark runs may compute project-owned diagnostic hashes over canonical selected authoritative state at checkpoints.

The hash is not save/security authority; it detects accidental semantic divergence between optimized/worker-count/off-screen variants.

## 22. Profiler Overhead

When detailed profiling is disabled, always-on budget telemetry targets **< 2% CPU overhead**.

Normal development profiler target: **< 5% overhead**.

Full trace/instrumentation modes can exceed that and are not used for final performance numbers unless overhead is calibrated.

## 23. Trace Ring

Development runtime retains a rolling lightweight trace of at least **30 seconds** for major frame/tick/budget events so a hitch can be dumped after it happens.

The ring uses bounded memory, target **<= 64 MiB**.

## 24. Hitch Capture

Automatic development hitch markers trigger on:

- simulation tick > 8 ms;
- main-thread frame work > 16.67 ms;
- GPU frame > 20 ms;
- streaming hold beginning;
- worker queue Red/Critical;
- audio underrun;
- save capture > 16 ms;
- allocation/memory pressure transition.

Hitch markers retain surrounding trace context.

## 25. Memory Leak Scenario

Every scene benchmark supports repeated load/unload loops. After **10 identical cycles**, memory must reach a stable plateau inside bounded caches.

Unexplained monotonic CPU/GPU/backend-resource growth is a release-gate failure.

## 26. Allowed Degradation Matrix

Under pressure StarForge may degrade in this order:

1. development/debug instrumentation;
2. speculative/predictive work;
3. inactive cache residency;
4. decorative effects/particles;
5. shadow/texture/render-only detail within preset/accessibility rules;
6. noncritical audio voice detail/cache;
7. render cadence;
8. enter loading/Hard Streaming Hold.

It may never degrade authoritative gameplay semantics.

## 27. Quality Preset Validation

Each Low/Medium/High/Ultra preset gets benchmark coverage proving:

- no gameplay state difference from quality selection;
- critical visibility/accessibility cues remain;
- memory targets fit intended budget;
- lower tiers actually reduce presentation cost.

## 28. Worker Count Matrix

Determinism/performance regression runs include runtime worker counts:

- 2;
- representative default;
- 12.

Authoritative checkpoint results must remain equivalent. Only latency/timing metrics can differ.

## 29. Frame Rate Matrix

Representative runs include presentation caps/rates around:

- 30 FPS;
- 60 FPS;
- 120+ FPS where hardware allows.

Fixed 60 Hz authoritative outcomes remain equivalent.

## 30. Tooling Surface

Development tooling exposes:

- live frame/tick timeline;
- worker queues;
- memory pools;
- streaming cells;
- renderer pass timings;
- physics counters;
- station graphs/scheduler;
- AI/nav counters;
- persistence operations;
- audio callback/voice state;
- UI/presentation counts;
- capture-to-file trace command.

Dear ImGui is appropriate for these development-only panels.

## 31. Shipping Telemetry Boundary

TA-13 requires local diagnostics but does not require online analytics/telemetry upload. No remote service is needed for core performance operation.

## 32. Later Phase Ownership

TA-14 converts selected benchmarks/budgets into automated tests and CI gates.

TA-15 checks cross-architecture feasibility.

TA-16 pins reference hardware and implementation milestone acceptance.

## 33. Open Questions

None in the TA-13 profiling/benchmark/degradation architecture.