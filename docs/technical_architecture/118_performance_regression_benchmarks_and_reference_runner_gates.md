# TA-14 — Performance Regression, Benchmarks, and Reference-Runner Gates

> **Status:** Architecture Complete  
> **Authority:** TA-13 performance automation, benchmark execution, baseline management, percentile regression classification, reference-runner requirements, noise handling, and performance certification gates

## 1. Purpose

TA-13 defines budgets and representative scenarios. TA-14 defines how those budgets become reproducible engineering evidence without allowing noisy cloud hardware to create misleading pass/fail results.

## 2. Performance Correctness Split

Performance gates never replace semantic correctness gates.

Every benchmark scenario also verifies:

- no semantic hard-cap violation;
- no authoritative state divergence;
- no unexpected Hard Streaming Hold where the scenario forbids it;
- no test/assertion/invariant failure.

A fast but semantically wrong run fails.

## 3. Runner Classes

Performance execution uses three classes:

1. **Generic CI runner** — compile/test/performance smoke only; no authoritative GPU budget certification;
2. **Stable CPU benchmark runner** — controlled CPU/RAM/storage for headless/runtime regressions;
3. **Reference graphics runner** — TA-16-pinned CPU/GPU/driver/display configuration for renderer/GPU certification.

Results from different runner classes are not directly mixed into one baseline.

## 4. Reference Hardware Ownership

TA-16 pins the concrete reference hardware and runner labels.

TA-14 requires the runner identity/configuration to be recorded with every accepted baseline.

## 5. Scenario Matrix

Performance certification includes TA-13 scenarios:

```text
P1 Dense Horizon Operations
P2 Horizon Emergency / Boarding Defense
P3 Surface Expedition Combat
P4 Local Spaceflight Battle
P5 Mixed Boarding Mission
P6 External Mission + Off-Screen Horizon
P7 Streaming Traversal
P8 Save / Load Stress
P9 UI / Audio / Presentation Stress
P10 Content Build
```

## 6. Deterministic Inputs

Each scenario specifies:

- scenario version;
- content fixture/build;
- fixed seed;
- scripted semantic input timeline;
- graphics/presentation preset where applicable;
- worker-count policy;
- warm-up/measurement duration.

A baseline is invalid if scenario semantics changed without scenario-version update/review.

## 7. Warm-Up / Measurement

Steady-state runtime benchmark:

```text
30 s warm-up
120 s measurement minimum
>= 3 repeated runs
```

as required by TA-13.

Cold-start/stream/load tests can use separate explicit timing windows.

## 8. Reported Metrics

Performance result artifacts contain applicable:

- p50/p95/p99/max frame/tick times;
- main-thread/subsystem timings;
- GPU pass/frame timings;
- worker queue latency/depth/utilization;
- active entity/physics/AI counts;
- memory peak/residency;
- streaming read/decode/upload;
- Hard Streaming Hold count/duration;
- save/load/build timings;
- audio callback utilization/underruns.

## 9. TA-13 Hard Targets

Reference 1080p High certification checks include:

- simulation tick p95 <= 4.0 ms;
- main-thread p95 <= 12.0 ms;
- GPU p95 <= 13.5 ms;
- GPU p99 <= 16.0 ms;
- no sustained Severe/Critical simulation backlog;
- no semantic hard-cap violation.

Exact additional scenario-specific limits come from TA-13 specs.

## 10. Regression Threshold

Against an accepted same-runner/same-scenario baseline, a critical metric is flagged when it:

- regresses p95 by >10% and >=0.25 ms;
- regresses p99 by >15% and >=0.5 ms;
- adds a hard-cap violation;
- adds an unexpected Hard Streaming Hold;
- increases peak memory by >10% or 128 MiB, whichever is larger, without accepted explanation.

## 11. Blocking Classification

Reference-runner regression blocks when:

- hard TA-13 acceptance target fails;
- hard-cap violation appears;
- deterministic semantic mismatch appears;
- regression repeats beyond the controlled noise policy;
- memory/hold regression exceeds threshold without approved baseline change.

## 12. Noise Handling

Performance CI does not “rerun until green.”

A suspected noisy result can trigger a predefined confirmation set, for example additional controlled repetitions on the same runner.

The original sample remains in the artifact/report.

A baseline is changed only by reviewed decision, not automatically from the latest run.

## 13. Baseline Storage

Accepted performance baselines are versioned project data/artifacts tied to:

- scenario version;
- commit/release/milestone;
- runner identity;
- toolchain/driver;
- content fixture/build;
- graphics configuration.

Ephemeral Actions cache is not baseline authority.

## 14. Baseline Update

A baseline update requires explanation such as:

- intentional feature/content cost;
- accepted architecture change;
- reference hardware/toolchain change;
- benchmark scenario version change;
- optimization improvement being promoted.

Updating the baseline cannot make a TA-13 hard target disappear.

## 15. Generic PR Performance Smoke

Ordinary cloud PR jobs may run short headless/micro benchmarks to catch catastrophic regressions such as:

- accidental O(N²) explosion;
- enormous allocation growth;
- save snapshot exceeds safety cap;
- content build explosion;
- simulation backlog immediately critical.

These are coarse smoke gates, not final FPS certification.

## 16. Reference Performance Workflow

`StarForge / Performance Regression` runs on a controlled runner for:

- release/Implementation Locked candidate;
- scheduled cadence;
- manual performance-sensitive PR opt-in;
- selected protected integration workflow.

## 17. Change Classification

PR labels/path heuristics may suggest performance-sensitive changes but cannot permanently skip certification for a release candidate.

Renderer/physics/AI/station/content/runtime changes are obvious triggers; any code can still affect performance indirectly.

## 18. GPU Query Discipline

Benchmark collection uses delayed OpenGL timer-query/fence results and does not force `glFinish` every frame merely to measure.

Measurement method must not fundamentally serialize the renderer.

## 19. VSync / Frame Cap

Reference GPU benchmark disables external presentation limits that would hide actual frame cost, unless the specific test is about VSync/frame pacing.

Fixed simulation remains 60 Hz regardless.

## 20. Warm vs Cold Streaming

P7 reports warm and cold behavior separately.

Warm-route acceptance target includes zero unexpected Hard Streaming Holds.

Cold-load behavior records I/O/decode/activation latency and remains governed by correctness/Hold semantics.

## 21. Memory Leak Certification

Repeated scene/context transitions run enough cycles to establish a resource plateau after warm-up.

Unbounded upward trend in process/GPU/backend-resource categories fails.

Known intentional cache growth must converge within declared TA-13 budgets.

## 22. Worker-Count Performance vs Determinism

Benchmarks can compare worker counts for throughput, but every configuration must also pass the same authoritative checkpoint-equivalence test.

A faster result that changes mission/AI/path/content outcome is invalid.

## 23. Save / Load Performance

P8 measures:

- Stable Save Boundary capture;
- snapshot size/peak memory;
- encode/CRC/write/commit;
- cold load/decode/migration/validation;
- runtime reconstruction.

No performance path can skip CRC/migrations/section validation.

## 24. Content Build Performance

P10 records clean/incremental/no-op cook behavior under the TA-13 build-worker/RAM envelope.

Outputs/fingerprints must remain deterministic across tested worker counts.

## 25. Presentation Performance

P9 can degrade optional presentation according to TA-13/TA-11 quality/accessibility rules.

Benchmark success cannot be achieved by disabling critical alarms/captions/UI readability or changing AI hearing.

## 26. CriticalPerformanceRecovery Test

A controlled stress fixture deliberately exceeds normal budget and verifies:

- backlog state transitions;
- optional work reduction;
- no Simulation Time skipping;
- eventual controlled recovery/failure behavior;
- no silent gameplay-event loss.

This is correctness under overload, not a requirement that overload remain 60 FPS.

## 27. Performance Artifact

A benchmark artifact includes:

- summary JSON/CSV or project format;
- percentile tables;
- counts/budget violations;
- runner/toolchain metadata;
- scenario/seed/version;
- state equivalence result;
- bounded trace around hitches/regressions.

## 28. Human Summary

CI produces a concise summary showing:

- current vs baseline;
- percentage/absolute delta;
- hard-target status;
- new hard-cap/hold failures;
- artifact link/identifier.

## 29. Profiler Overhead

Reference measurements use the TA-13 approved low-overhead telemetry mode.

Full tracing is enabled only for diagnosis and is not compared directly to low-overhead baseline numbers without calibration.

## 30. No Average-FPS Gate

Average FPS alone is never the primary pass/fail signal.

Percentiles, hitches, backlog, memory and semantic invariants are required.

## 31. Explicit Non-Goals

TA-14 does not:

- certify performance on arbitrary cloud VM timing;
- auto-update performance baselines;
- allow performance wins to waive correctness;
- lock final reference hardware before TA-16;
- promise every PR runs the entire 10-scenario reference matrix.

## 32. Dependencies

Depends on TA-13 numeric budgets/profiler, TA-14 deterministic scenarios/CI, TA-4 GPU timing, TA-12 save metrics, TA-10 content build metrics, and TA-16 reference hardware/toolchain pinning.

## 33. Open Questions

None within performance-regression gate architecture.