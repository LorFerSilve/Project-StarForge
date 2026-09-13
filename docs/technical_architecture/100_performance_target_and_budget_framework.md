# TA-13 — Performance Target and Budget Framework

> **Status:** Architecture Complete  
> **Authority:** Performance-target taxonomy, reference workload, percentile gates, budget classes, overload states, semantic invariants, and measurement rules

## 1. Purpose

TA-13 converts the qualitative scalability hooks from TA-1 through TA-12 into explicit measurable budgets without allowing performance shortcuts to change gameplay outcomes.

The budgets in TA-13 are implementation targets and safety caps. TA-16 pins the concrete reference hardware used to certify them.

## 2. Primary Runtime Target

The baseline implementation target is:

- authoritative simulation: fixed **60 Hz**;
- presentation target: **60 FPS** at 1920×1080 on `High` graphics preset;
- world UI: native output resolution;
- no automatic dynamic resolution;
- no skipped Simulation Time;
- no gameplay-semantic degradation under load.

A machine can render faster or slower, but authoritative simulation remains 60 Hz.

## 3. Reference Hardware Boundary

TA-13 defines numeric software budgets but does not name the final shipping minimum/recommended CPU/GPU.

TA-16 must pin at least:

- one Windows x64 reference CPU;
- one reference GPU/driver family;
- physical RAM amount;
- storage class;
- target display resolution/preset.

The numeric budgets in this phase become implementation gates on that reference configuration.

## 4. Budget Classes

Every budget is one of:

- **Semantic Hard Cap** — exceeding it is invalid content/state or requires a controlled technical failure/hold; data is never silently dropped;
- **Memory Hard Cap** — allocation/request is refused or held before unsafe memory growth; gameplay state remains intact;
- **Soft Runtime Budget** — exceeding it records pressure and triggers allowed technical adaptation;
- **Acceptance Target** — percentile/benchmark goal used by performance certification;
- **Tuneable Presentation Budget** — graphics/audio/VFX cache detail that can vary by quality tier without changing gameplay.

## 5. Percentile Rule

Performance gates use distributions, not averages alone.

For benchmark runs the runtime records at minimum:

- p50;
- p95;
- p99;
- maximum;
- count of budget violations;
- contiguous violation duration.

A good average does not hide repeated long stalls.

## 6. Frame and Tick Reference Durations

At 60 Hz:

```text
Fixed Simulation Tick = 16.666666... ms
60 FPS Frame Period    = 16.666666... ms
```

The main thread contains both simulation and OpenGL submission, therefore subsystem budgets intentionally sum to less than the full frame period.

## 7. High-Preset Reference CPU/GPU Targets

For representative TA-13 benchmark scenes on the TA-16 reference PC:

- authoritative simulation tick p95: **<= 4.0 ms**;
- authoritative simulation tick p99: **<= 6.0 ms**;
- physics step p95: **<= 2.0 ms**;
- CPU render extraction/submission p95: **<= 4.0 ms**;
- total main-thread frame work p95: **<= 12.0 ms**;
- GPU frame p95 at 1080p High: **<= 13.5 ms**;
- GPU frame p99: **<= 16.0 ms**;
- UI/layout/text CPU p95: **<= 1.0 ms**;
- worker-result application on simulation thread p95: **<= 1.0 ms/tick**.

These leave scheduling/headroom rather than budgeting exactly 16.67 ms everywhere.

## 8. Simulation Correctness Wins Over Frame Rate

When the process is overloaded it may:

- render fewer frames;
- defer optional worker jobs;
- evict inactive cache content;
- lower presentation residency inside the selected quality policy;
- reduce decorative particles/effects where GDS permits;
- enter Hard Streaming Hold when required content is unavailable.

It may not:

- skip authoritative ticks;
- advance timers from wall time;
- omit due damage/objective/transaction facts;
- simplify ownership;
- make AI omniscient or less constrained;
- remove collision/gameplay geometry;
- change rewards/resources;
- resolve raids through a fake score;
- truncate save state.

## 9. Pressure States

Every major runtime budget reports one of:

- `Green` — below soft budget;
- `Yellow` — >= 80% soft budget or repeated p95 violation;
- `Red` — soft budget exceeded or queue/memory pressure sustained;
- `Critical` — hard cap/required-content safety boundary approached.

Pressure state is diagnostic/technical policy, never gameplay difficulty.

## 10. Critical-Pressure Response Order

The default pressure response order is:

1. cancel/discard stale async work;
2. coalesce redundant non-authoritative work;
3. postpone optional/decorative jobs;
4. evict inactive caches;
5. reduce quality-tier-approved presentation residency/detail;
6. stop issuing speculative prefetch work;
7. Hard Streaming Hold or controlled subsystem/session failure if required correctness cannot be maintained.

Gameplay semantics are never the final degradation layer.

## 11. Runtime Memory Reference Targets

64-bit Windows process soft working-set targets, excluding unrelated OS processes and opaque driver allocations:

| Preset | Process Soft Target | CPU Content Cache Soft Target |
|---|---:|---:|
| Low | 3.0 GiB | 512 MiB |
| Medium | 3.5 GiB | 768 MiB |
| High | 4.5 GiB | 1.25 GiB |
| Ultra | 6.0 GiB | 2.0 GiB |

The baseline process **memory safety ceiling is 8.0 GiB** before the application must refuse additional optional residency/enter controlled pressure handling rather than growing unboundedly.

This is not the final minimum-RAM specification; TA-16 maps it to actual hardware.

## 12. GPU Residency Reference Targets

Configured renderer residency targets:

| Preset | GPU Residency Target |
|---|---:|
| Low | 1.5 GiB |
| Medium | 2.0 GiB |
| High | 3.0 GiB |
| Ultra | 4.5 GiB |

Because portable OpenGL VRAM queries are not guaranteed, these are project budgets, not assumptions that every driver exposes exact free VRAM. TA-16 validates them on reference hardware.

## 13. Mandatory vs Optional Residency

Mandatory gameplay-correct content takes precedence over optional high-detail presentation residency.

If mandatory content cannot fit while preserving the selected scene contract:

- inactive optional caches are evicted;
- higher-detail texture/LOD residency can be reduced within quality policy;
- decorative content can be reduced;
- if still impossible, scene activation fails or holds clearly.

The runtime never substitutes different collision/navigation/gameplay content merely to fit memory.

## 14. Benchmark Warm-Up

Performance runs distinguish:

- **cold** path: caches initially empty;
- **warm** path: representative runtime caches established.

Steady-state frame/tick percentile gates use warm runs. Streaming/load-specific gates separately measure cold behavior.

## 15. No Hidden Adaptive Gameplay

Performance pressure cannot dynamically alter:

- enemy count already authored/committed;
- robot squad rules;
- crew task semantics;
- objective requirements;
- station conservation equations;
- mission procedural selection;
- reinforcement manifests;
- save contents.

Content authors can create scalability-aware encounters, but runtime hardware pressure never rewrites an already committed outcome.

## 16. Determinism Boundary

Different worker counts, cache hit rates, GPU speed, storage speed, or render FPS can change:

- completion latency of advisory work;
- presentation smoothness;
- whether optional assets remain cached;
- when a Hard Streaming Hold becomes necessary.

They cannot change accepted authoritative results.

## 17. Development Budget Assertions

Development builds expose soft/hard budget assertions and telemetry. Semantic hard-cap violations fail loudly with typed diagnostics rather than silently clipping data.

Shipping builds preserve correctness/safety checks while reducing expensive diagnostic detail where appropriate.

## 18. Later-Phase Ownership

TA-14 defines automated performance regression execution/CI gating.

TA-15 validates the complete architecture against these budgets.

TA-16 pins hardware, CMake targets, implementation order, and which budgets become milestone exit criteria.

## 19. Open Questions

None in the TA-13 performance-target framework.