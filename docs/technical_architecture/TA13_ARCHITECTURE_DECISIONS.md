# TA-13 — Architecture Decision Addendum

> **Status:** Accepted  
> **Authority:** TA-13 concurrency, performance, memory, streaming, and profiling decisions  
> **Relationship:** Continues the accepted architecture-decision sequence after TA-12 AD-111

This addendum records the accepted project-wide decisions introduced by TA-13. TA-16 may mechanically consolidate them into the central decision log without changing semantics.

---

## AD-112 — Performance Budgets Never Authorize Gameplay-Semantic Degradation

**Status:** Accepted

### Decision

StarForge classifies budgets as semantic hard caps, memory hard caps, soft runtime budgets, acceptance targets, or tuneable presentation budgets. When performance/memory pressure occurs, technical/presentation work degrades or the game enters a controlled Hold/failure before authoritative gameplay rules are simplified, skipped, or rewritten.

### Rationale

Hardware speed must affect latency/smoothness rather than combat, ownership, AI knowledge, station conservation, rewards, mission state, or persistence correctness.

---

## AD-113 — Runtime Async Work Uses One Bounded Shared Worker Pool

**Status:** Accepted

### Decision

The runtime general worker pool is `clamp(HardwareConcurrency - 2, 2, 12)`, with typed priority classes, bounded queues, fairness/aging, cooperative cancellation, immutable inputs/results, and revision/generation validation. Worker completion order is never semantic authority.

### Rationale

A bounded shared pool avoids thread-per-system complexity and unbounded task/memory growth while allowing CPU parallelism without changing deterministic outcomes.

---

## AD-114 — Simulation Catch-Up Is Bounded Per Render Frame but Never Skips Ticks

**Status:** Accepted

### Decision

Ordinary backlog recovery runs at most four catch-up simulation ticks per rendered frame. Backlog pressure can reduce render/presentation work and eventually enter CriticalPerformanceRecovery, but authoritative Simulation Time is never skipped to regain real-time alignment.

### Rationale

This prevents both spiral-of-death presentation lockout and hardware-dependent loss of gameplay time.

---

## AD-115 — Active Local Runtime Uses Explicit Entity/Physics Scale Envelopes

**Status:** Accepted

### Decision

One active SceneInstance targets <=8,192 live runtime entities and <=8,192 physics bodies, with semantic safety capacities of 16,384 each, plus explicit budgets for high-frequency actors, projectiles, contacts, queries, CharacterMotors and spacecraft. Exceeding required-state capacity is a validation/technical failure, not permission to despawn gameplay state.

### Rationale

The custom runtime needs a measurable scale target while persistent/off-screen state remains independent from local high-frequency representation.

---

## AD-116 — Horizon, AI, Navigation, Mission, and Strategic Scaling Preserve Chronological Semantics

**Status:** Accepted

### Decision

Station graph/work, AI/perception/pathfinding, objective/event and off-screen scheduler workloads have explicit scale/time envelopes. Due chronological boundaries and committed facts are never skipped to meet a CPU budget; soft overload feeds the global backlog policy instead.

### Rationale

Performance optimization must not grant production after failure, teleport actors, omit reinforcement timing, or change mission/event outcomes.

---

## AD-117 — Residency Is Budgeted and Evictable; Required Gameplay Content Holds Before Semantic Compromise

**Status:** Accepted

### Decision

CPU/GPU content residency, streaming staging, I/O/decode concurrency and GPU uploads are bounded. Under pressure StarForge cancels speculative work and evicts inactive/presentation detail first. If mandatory collision/navigation/gameplay content cannot become ready safely, TA-3 Hard Streaming Hold or controlled activation failure occurs.

### Rationale

Memory/storage differences can change loading time but cannot substitute geometry, alter activation, or reroll committed content.

---

## AD-118 — Reference Performance Target Is 60 FPS at 1080p High With Percentile Gates

**Status:** Accepted

### Decision

TA-13 targets fixed 60 Hz simulation plus 60 FPS at 1920x1080 High on hardware pinned later by TA-16. Core acceptance includes simulation p95 <=4.0 ms, main-thread work p95 <=12.0 ms, GPU p95 <=13.5 ms and GPU p99 <=16.0 ms in representative benchmark scenes.

### Rationale

Percentile-based budgets provide implementation headroom and expose hitches better than average FPS while deferring final hardware naming until implementation planning.

---

## AD-119 — Presentation Budgets May Virtualize Decorative Work but Never Critical Semantics

**Status:** Accepted

### Decision

Renderer, VFX, UI, audio, animation, marker, notification and haptic systems use bounded queues/residency and can reduce decorative presentation according to quality/accessibility policy. Critical readable cues remain, and AI hearing continues from GameplaySoundEvents independently of playback voice/mix budgets.

### Rationale

Presentation overload must not change stealth, knowledge, hit resolution, objectives, alarms, accessibility or other gameplay authority.

---

## AD-120 — Persistence and Content Build Have Explicit Memory/Concurrency Envelopes

**Status:** Accepted

### Decision

Stable Save Boundary capture targets <=4 ms p95, an in-flight snapshot targets <=256 MiB with 1 GiB safety cap, one campaign generation writes at a time, load staging remains bounded/all-or-nothing, and offline content cooking uses `clamp(HardwareConcurrency - 1, 1, 16)` workers under `min(70% physical RAM, 12 GiB)` build memory cap.

### Rationale

Save/load/build operations must remain reproducible and crash-safe without unbounded memory or concurrency, and performance pressure may never omit sections/migrations/content validation.

---

## AD-121 — Performance Certification Uses Standardized Representative Benchmarks and Traceable Percentile Regressions

**Status:** Accepted

### Decision

TA-13 defines a benchmark matrix covering dense Horizon, Horizon defense, surface combat, local spaceflight, mixed boarding, off-screen Horizon concurrency, streaming traversal, save/load stress, UI/audio presentation and content building. Steady-state runs use 30 s warm-up + 120 s measurement, repeated at least three times, with bounded profiler overhead and p95/p99 regression thresholds.

### Rationale

Performance architecture must be validated against real systemic workloads instead of empty rooms or isolated averages, while TA-14 later converts selected metrics into automated gates and TA-16 pins reference hardware.

---

## Consolidation Rule

TA-16 contract locking may consolidate AD-112 through AD-121 into `ARCHITECTURE_DECISIONS.md` together with the TA-12 addendum. Consolidation is editorial only and must preserve the accepted decisions above.