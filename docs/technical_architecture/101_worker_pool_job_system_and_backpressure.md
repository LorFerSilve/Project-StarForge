# TA-13 — Worker Pool, Job Priorities, and Backpressure

> **Status:** Architecture Complete  
> **Authority:** Runtime worker-count policy, job classes, queue bounds, dependencies, cancellation, fairness, result publication, and overload behavior

## 1. Purpose

TA-1 allows bounded asynchronous work but intentionally leaves exact scheduling and capacity open. TA-13 fixes the baseline job-system model and prevents worker queues, completion races, or memory growth from becoming gameplay authority.

## 2. Runtime Worker Pool Size

The runtime owns one general worker pool sized from logical hardware concurrency:

```text
RuntimeWorkers = clamp(HardwareConcurrency - 2, 2, 12)
```

If hardware concurrency is unavailable or reports an unusable value, the baseline falls back to **2 runtime workers**.

The main thread and audio backend callback are never counted as general workers.

## 3. Why a Shared Pool

StarForge does not create one dedicated thread per subsystem.

A shared bounded pool handles jobs such as:

- file I/O staging;
- asset decode/CPU preparation;
- navigation path searches;
- procedural candidates;
- read-only spatial/visibility analysis;
- save encode/CRC/write preparation;
- content cache preparation;
- bounded diagnostic work.

Subsystem-specific queues may exist logically, but execution capacity remains coordinated.

## 4. Job Priority Classes

Runtime jobs use explicit project-owned classes:

1. `RequiredStreaming`;
2. `BlockingLoadPersistence`;
3. `UrgentNavigationAI`;
4. `GameplaySupport`;
5. `PredictiveStreaming`;
6. `BackgroundPersistence`;
7. `PresentationBackground`;
8. `DevelopmentDiagnostics`.

Priority affects service latency only. It never determines authoritative gameplay ordering.

## 5. Fairness and Aging

Strict priority starvation is prohibited.

Each queue supports bounded aging so a lower-priority valid job eventually receives service unless the application is in a declared blocking load/hold state.

Critical required-streaming/load work can temporarily dominate, but ordinary background save completion cannot be starved indefinitely during normal play.

## 6. Queue Capacity

The runtime job system has these semantic safety caps:

- total queued jobs: **2048**;
- `RequiredStreaming`: **128**;
- `BlockingLoadPersistence`: **64**;
- `UrgentNavigationAI`: **256**;
- `GameplaySupport`: **512**;
- `PredictiveStreaming`: **256**;
- `BackgroundPersistence`: **16**;
- `PresentationBackground`: **512**;
- `DevelopmentDiagnostics`: **256**.

The sum of class caps can exceed the global cap; the global cap remains authoritative.

## 7. Queue Full Behavior

When a queue is full:

- stale/cancelled work is removed first;
- equivalent speculative/presentation requests are coalesced where semantics allow;
- optional prefetch/decorative work can be rejected/deferred;
- required jobs must cause backpressure/hold rather than be silently dropped;
- authoritative gameplay facts are never dropped because a worker queue is full.

## 8. Outstanding Navigation Requests

At most **256** navigation path requests may be queued/in flight globally in the active local context.

Routine AI reaching the cap delays new routine path requests deterministically. Urgent safety/combat requests can pre-empt obsolete routine requests but cannot overwrite a valid result by completion race.

## 9. Streaming Concurrency

Runtime content streaming limits:

- concurrent file-read jobs: **8**;
- concurrent heavy CPU decode/preparation jobs: **4**;
- main-thread GPU upload remains TA-4 owned and is not a worker job;
- Jolt/navigation backend mutation remains on their owning safe phases.

These caps control contention and transient memory.

## 10. Persistence Concurrency

Per campaign/session:

- maximum save encode/write generation actively in flight: **1**;
- maximum coalesced pending same-purpose background save request: **1**;
- load operation: exclusive against ordinary live-session load replacement;
- profile-settings write may use an independent tiny storage task because it is not campaign gameplay state.

Manual/Quick user requests are never reported committed until their own generation commits.

## 11. Job Input Contract

Every gameplay-adjacent worker job receives immutable input including all freshness tokens needed by its consumer, such as:

- SceneGeneration;
- ContentGeneration;
- NavigationRevision;
- RuntimeEntity generation;
- domain/record StateRevision;
- request sequence;
- deterministic seed/range for procedural work.

Workers never borrow mutable store references.

## 12. Job Result Contract

Results are immutable and return through typed result channels.

The main/owning thread:

1. checks cancellation;
2. checks all generation/revision tokens;
3. validates semantic preconditions;
4. applies/commits in deterministic project order or discards the result.

Completion time is never semantic priority.

## 13. Result-Application Budget

Main-thread application of worker results has a **1.0 ms p95 per simulation tick** acceptance target.

If more valid results exist than fit the ordinary budget:

- urgent results are processed first by semantic priority;
- routine advisory results remain queued for later consumption;
- due authoritative events are not deferred merely to protect this performance target.

## 14. Bounded Result Mailboxes

Typed worker-result mailboxes have a global soft target of **1024 queued results** and hard safety cap of **4096**.

On pressure:

- stale superseded results are deleted;
- duplicate newest-wins presentation/cache results can coalesce;
- authoritative committed events are not stored in this mailbox and therefore cannot be dropped here.

## 15. Cancellation

Jobs must expose cooperative cancellation at bounded safe points when they can become obsolete, especially:

- nav path requests;
- predictive streaming;
- content decode after scene transition;
- development analysis.

Cancellation does not mutate gameplay. It only avoids wasting technical work.

## 16. Worker Dependencies

Workers do not synchronously block waiting for another worker from the same bounded pool.

Dependent work uses:

- explicit dependency counters;
- continuations;
- resubmission after prerequisite publication.

This prevents pool deadlock.

## 17. Long Job Rule

Normal runtime jobs should target **<= 4 ms CPU slices** before a cancellation/dependency checkpoint when algorithmically feasible.

Jobs expected to take tens/hundreds of milliseconds must be chunkable or isolated as blocking-load/content-build work so one worker cannot monopolize the pool indefinitely.

## 18. Worker Allocation Is Not Determinism

Changing `RuntimeWorkers` from 2 to 12 may change latency but cannot change:

- procedural selection;
- path acceptance ordering;
- target selection;
- mission contents;
- station scheduling;
- transaction order;
- save bytes except permitted metadata timestamps.

## 19. Locking Policy

Worker jobs prefer immutable snapshots and thread-local scratch.

Shared locks are limited to technical queues/caches. No worker holds a lock while waiting for main-thread gameplay mutation or another worker dependency.

Authoritative domain stores remain main-thread mutation authority.

## 20. Transient Job Memory

Runtime worker/job scratch and queued immutable payloads have a combined soft target of **256 MiB** and hard safety cap of **512 MiB**.

Large streaming/save payloads count toward their dedicated TA-13 memory pools rather than hiding in arbitrary job allocations.

## 21. Backpressure Diagnostics

Telemetry records per class:

- queue depth;
- oldest wait age;
- submit/reject/coalesce/cancel counts;
- execution time p50/p95/p99;
- result stale-reject count;
- result apply latency;
- transient bytes.

A growing queue is visible before it becomes a gameplay stall.

## 22. True Pause and Holds

Workers may continue technical work during True Pause/Hard Streaming Hold if their work does not advance gameplay.

Simulation-owned result application waits for the next legal simulation phase unless it is required to end the hold itself.

## 23. Development Content Builds

`starforge-content` uses a separate process/tool scheduling policy:

```text
BuildWorkers = clamp(HardwareConcurrency - 1, 1, 16)
```

The tool also obeys the content-build memory cap in TA-13 and dependency-order determinism.

## 24. Non-Goals

No work stealing is semantically required; no lock-free universal queue is required; no thread-per-domain model; no worker-thread gameplay commits; no completion-race authority; no unbounded async task creation.

## 25. Open Questions

None in the TA-13 worker-pool/backpressure architecture.