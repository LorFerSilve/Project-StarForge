# TA-13 — Main-Thread Tick, Frame, and Backlog Budgets

> **Status:** Architecture Complete  
> **Authority:** Fixed-tick CPU targets, render-frame CPU allocation, catch-up cap, backlog diagnostics/recovery, main-thread service limits, and no-skip rules

## 1. Purpose

The main thread owns platform events, authoritative simulation, OpenGL submission and final presentation. TA-13 fixes the amount of CPU time each category is allowed to consume before pressure handling is required.

## 2. Fixed Tick Remains 60 Hz

Authoritative fixed-step duration remains:

```text
16.666666... ms
```

This is not a target to consume fully. The simulation must leave headroom for rendering and platform work on the same thread.

## 3. Simulation Tick Targets

Representative benchmark scenes target:

- p50 simulation tick: **<= 2.5 ms**;
- p95: **<= 4.0 ms**;
- p99: **<= 6.0 ms**;
- single-tick warning threshold: **8.0 ms**;
- fixed-period miss threshold: **16.67 ms**.

A tick over 16.67 ms is not skipped; it creates backlog and is diagnosed.

## 4. Main-Thread Frame Target

At 60 FPS High/1080p on reference hardware:

- simulation + worker-result consumption: <= **5.0 ms p95** combined;
- render extraction/submission: <= **4.0 ms p95**;
- UI/input/presentation CPU: <= **1.5 ms p95**;
- platform/event/present orchestration and margin: remaining headroom;
- total measured main-thread work p95: **<= 12.0 ms**.

GPU synchronization stalls are separately measured and attributed.

## 5. Catch-Up Tick Cap

The application executes at most **4 catch-up simulation ticks per rendered frame** during ordinary backlog recovery.

This prevents a transient hitch from turning into an indefinitely non-presenting loop while still recovering Simulation Time without skipping ticks.

## 6. Backlog Levels

Simulation accumulator backlog is classified by whole fixed ticks:

- `Normal`: 0–2 ticks;
- `Warning`: 3–8 ticks;
- `Severe`: 9–30 ticks;
- `Critical`: >30 ticks.

Backlog state is technical telemetry, not gameplay state.

## 7. Backlog Recovery Policy

When backlog is Warning or worse, the application may:

- suppress/defer optional presentation work;
- reduce render frequency while continuing fixed simulation;
- pause speculative streaming and development diagnostics;
- prioritize worker results required by current simulation.

It may not skip queued simulation ticks.

## 8. Critical Backlog

At >30 ticks (~0.5 seconds) of backlog:

- rendering can be reduced to a minimal cadence;
- nonessential worker-result application is postponed;
- development builds emit a high-severity performance diagnostic with phase breakdown;
- the main thread prioritizes authoritative catch-up.

If backlog exceeds **120 ticks (~2 seconds)**, the runtime enters `CriticalPerformanceRecovery`: ordinary high-cost presentation is suspended until backlog drops below 4 ticks.

No Simulation Time is discarded.

## 9. Sustained Failure

If CriticalPerformanceRecovery cannot reduce backlog below 30 ticks after **5 seconds of wall-clock recovery effort** on the supported reference configuration, the runtime records a performance-failure condition suitable for benchmark/QA failure.

Shipping behavior still preserves gameplay state rather than fast-forward skipping.

## 10. Pathological Wall-Time Stalls

TA-1's wall-time safety clamp remains permitted for pathological platform pauses/suspend/debugger stalls, but the application must record when it occurs. It is not used as an ordinary performance tool to hide slow frames.

Window focus/minimize policies remain platform/presentation concerns and cannot silently simulate offline time.

## 11. Per-Phase Soft CPU Targets

Within the 4.0 ms p95 simulation target, representative soft targets are:

| Simulation Work | p95 Target |
|---|---:|
| Input/command validation | 0.25 ms |
| Movement/control pre-physics | 0.50 ms |
| Physics step + collection | 2.00 ms |
| Combat/interactions | 0.75 ms |
| AI/nav result/intent work | 1.00 ms |
| Mission/strategic/station due work | 1.50 ms |
| Transaction/lifecycle/cleanup | 0.75 ms |

These are not additive hard partitions because different scenes stress different subsets. Their purpose is attribution and regression detection.

## 12. Due Work Cannot Be Dropped

A budget overrun never licenses the scheduler to drop:

- damage;
- ownership transfers;
- objective changes;
- timer deadlines;
- reinforcement arrival;
- station conservation thresholds;
- final resolution;
- save/load integrity work required for an active operation.

When too much authoritative work is due, the tick can run long and the content/state that produced the pathological burst is diagnosed.

## 13. Same-Tick Event Chain Limit

The TA-1 diagnostic event-chain guard is fixed at **1024 committed follow-up event dispatches in one simulation tick** before development builds raise a probable runaway-chain invariant failure.

This cap is not a quota for silently dropping the 1025th valid event. Exceeding it is treated as invalid/pathological state requiring diagnosis.

## 14. Strategic Deadline Burst Target

Normal authored/runtime content should produce at most **128 strategic deadline/event resolutions in one base tick**.

A semantic safety cap of **4096 due strategic operations in one tick** exists to catch pathological content/save state; exceeding it fails validation/runtime invariant rather than truncating outcomes.

## 15. Worker Result Consumption

Ordinary worker-result application has a 1.0 ms p95 target per tick from TA-13 worker architecture.

Results whose semantic deadline is not immediate can remain queued; stale results are rejected. Results required to preserve current-tick correctness are applied even if the soft target is exceeded.

## 16. Stable Simulation Boundary Cost

Stable-boundary bookkeeping itself targets **<= 0.5 ms p95**, excluding an explicitly requested save snapshot capture. Save capture has its own TA-13 persistence budget.

## 17. Render Submission Stalls

OpenGL calls that unexpectedly block on GPU completion are recorded separately from normal CPU submission. Repeated stalls > **1.0 ms p95** indicate resource-lifetime/upload/synchronization problems and fail performance review for representative scenes.

## 18. Present/VSync Attribution

Intentional waiting inside VSync/present is not counted as simulation CPU cost, but is measured separately so frame pacing issues remain visible.

## 19. True Pause

True Pause stops authoritative tick execution. It does not create backlog from elapsed wall time.

Resume begins with the existing simulation accumulator contract, not accumulated paused real time.

## 20. Hard Streaming Hold

Hard Streaming Hold also stops Simulation Time and therefore does not create tick backlog. Required loading workers may continue until the hold can end.

## 21. Profiling Granularity

Every fixed tick records nested timing categories sufficient to attribute at least:

- physics;
- gameplay runtime;
- AI/navigation;
- station;
- missions/raids/events;
- persistence capture;
- worker-result apply;
- snapshot/read-model publication.

TA-14 later defines the concrete profiler regression tests.

## 22. Non-Goals

No adaptive Simulation Time, no variable-delta authoritative loop, no tick dropping, no hardware-pressure difficulty scaling, no average-frame-only acceptance.

## 23. Open Questions

None in the TA-13 tick/frame/backlog architecture.