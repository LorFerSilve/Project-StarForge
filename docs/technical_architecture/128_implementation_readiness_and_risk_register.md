# TA-15 — Implementation Readiness and Risk Register

> **Status:** Architecture Complete  
> **Authority:** Final classification of architecture blockers, implementation risks, TA-16 lock items, and post-lock change-control expectations

## 1. Purpose

TA-15 must distinguish true architecture gaps from normal implementation risks and intentionally deferred TA-16 choices.

A project can be architecture-ready while still having engineering risk. The key requirement is that implementation knows **what the contract is** and how success/failure is verified.

## 2. Blocking Findings

**Count: 0.**

No unresolved issue requires implementation to invent gameplay semantics, state ownership, thread authority, persistence behavior, content identity, failure policy, or test strategy.

## 3. Required Corrections

**Count: 0.**

No canonical TA-0 through TA-14 owning specification currently contains a known contradiction that must be rewritten before TA-16.

Historical references such as “later TA-13” in earlier phase documents are interpreted as original ownership handoffs whose later phase now exists; they do not override the completed owning specifications/roadmap.

## 4. TA-16 Lock Items

The following remain intentionally unresolved at architecture level and are **required TA-16 outputs**:

1. exact C++ compiler/toolset versions;
2. exact vcpkg baseline and dependency versions;
3. exact CMake target names and target-level dependency map;
4. CMake presets and build configurations;
5. exact CTest labels/fixture target names;
6. executable `.github/workflows/*.yml` files;
7. pinned GitHub Actions versions and permissions;
8. branch-required status-check materialization;
9. concrete reference CPU/GPU/RAM/storage and controlled performance runner identity;
10. initial source directory/scaffolding layout matching the TA-15 DAG;
11. dependency-ordered implementation phases;
12. first vertical slice and acceptance criteria;
13. per-contract `Implementation Locked` sequence;
14. merge/branch strategy during implementation;
15. milestone/exit criteria for each implementation phase;
16. consolidation of accepted TA-12/13/14 architecture-decision addenda into the central decision log without semantic change.

These are not architecture blockers because their semantic constraints are already defined.

## 5. Engineering Risk R1 — Custom Renderer Complexity

**Area:** TA-4 / TA-10 / TA-13 / TA-14  
**Risk:** Hybrid deferred OpenGL renderer, shadows, tiled lights, transparency, streaming uploads and presentation features can exceed frame/GPU-memory budgets.

**Architecture mitigation:**

- fixed render graph;
- explicit resource ownership;
- scalable presentation-only tiers;
- no mandatory real-time GI/TAA/motion blur;
- TA-13 GPU/residency budgets;
- P1-P5/P9 benchmarks;
- backend smoke and performance gates.

**Residual status:** Engineering risk, not semantic blocker.

## 6. Engineering Risk R2 — Station System Scale

**Area:** TA-6 / TA-13  
**Risk:** Power/atmosphere/thermal/water/logistics/automation can become expensive in large Horizon configurations.

**Architecture mitigation:**

- separate subsystem solvers;
- invalidation/revision-based graph work;
- chronological coarse off-screen scheduling;
- explicit topology/work/entity envelopes;
- no 60 Hz requirement for strategic systems;
- performance benchmark P1/P2/P6.

**Residual status:** Engineering/performance risk, contracts clear.

## 7. Engineering Risk R3 — Active/Off-Screen Equivalence

**Area:** TA-3 / TA-6 / TA-8 / TA-9  
**Risk:** Coarse off-screen simulation could diverge from active outcomes.

**Architecture mitigation:**

- same persistent ownership/state;
- Simulation-Time chronological boundaries;
- actual resources/topology/actor capability inputs;
- no second hidden scene/Defense Score;
- deterministic equivalence tests and P6 benchmark.

**Residual status:** Test/implementation risk, not architecture gap.

## 8. Engineering Risk R4 — Save Compatibility

**Area:** TA-2 / TA-10 / TA-12 / TA-14  
**Risk:** Long-term schema/content evolution can corrupt or invalidate campaign saves.

**Architecture mitigation:**

- explicit section/domain versions;
- source-preserving staged migrations;
- stable ContentIds;
- binary goldens;
- migration matrices;
- corruption/fault injection;
- immutable generation writes.

**Residual status:** Ongoing compatibility engineering risk.

## 9. Engineering Risk R5 — Determinism Under Concurrency

**Area:** TA-1 / TA-2 / TA-8 / TA-9 / TA-10 / TA-13  
**Risk:** Worker completion, hash/container order or frame timing could leak into authoritative results.

**Architecture mitigation:**

- main-thread commits;
- immutable versioned jobs;
- semantic result ordering;
- explicit PCG32 streams;
- deterministic container/serialization ordering;
- worker-count/frame-rate equivalence tests.

**Residual status:** Implementation discipline risk, architecture closed.

## 10. Engineering Risk R6 — Physics/Gameplay Edge Cases

**Area:** TA-5 / TA-7 / TA-9  
**Risk:** Fast projectiles, docking, moving platforms, mantle/Zero-G, collision episodes and load/rebase contacts can generate edge-case bugs.

**Architecture mitigation:**

- typed facts;
- sweeps;
- explicit character/ship model split;
- physical docking constraints;
- deferred mutation;
- no synthetic load/rebase impacts;
- adapter/headless/sanitizer tests.

**Residual status:** Implementation/test risk.

## 11. Engineering Risk R7 — Navigation/AI Knowledge Discipline

**Area:** TA-8  
**Risk:** AI code may accidentally consume world truth or stale routes for convenience.

**Architecture mitigation:**

- explicit Perception/Memory knowledge model;
- project navigation adapters;
- revision-validated paths;
- action validation at execution;
- no teleport recovery;
- knowledge/determinism tests.

**Residual status:** Implementation discipline risk.

## 12. Engineering Risk R8 — Content Pipeline Reproducibility

**Area:** TA-10 / TA-14  
**Risk:** DCC/library/platform differences could produce nondeterministic cooked products.

**Architecture mitigation:**

- pinned tools in TA-16;
- fingerprints/dependency graph;
- closed schemas;
- clean/incremental/worker-count deterministic cook tests;
- provenance/license checks.

**Residual status:** Toolchain lock/testing risk.

## 13. Engineering Risk R9 — CI/Reference Runner Availability

**Area:** TA-13 / TA-14 / TA-16  
**Risk:** Reliable GPU/performance certification requires stable controlled hardware unavailable on generic cloud runners.

**Architecture mitigation:**

- semantic correctness remains cloud/headless-testable;
- reference performance gate is separate;
- TA-16 must pin controlled runner hardware;
- performance baseline changes require review.

**Residual status:** Infrastructure risk, not architecture blocker.

## 14. Engineering Risk R10 — Scope Pressure

**Area:** whole project  
**Risk:** Custom renderer + systemic station + AI + persistence + content tooling is large for a purpose-built project.

**Architecture mitigation:**

- Design Complete scope;
- no general-purpose engine/plugin/scripting/networking requirements;
- dependency-ordered TA-16 implementation roadmap;
- vertical slice before breadth;
- explicit non-goals in owning specs;
- per-contract Implementation Lock rather than uncontrolled big-bang coding.

**Residual status:** Project-management/implementation sequencing risk.

## 15. Risk Acceptance Rule

TA-16 does not need to eliminate all engineering risk before coding.

It must:

- sequence risky dependencies early enough to validate them;
- define measurable milestone exits;
- materialize tests/benchmarks before relying on a subsystem;
- refuse silent semantic shortcuts when an implementation is difficult;
- route any true infeasibility back through architecture/GDS change control.

## 16. Implementation Readiness Matrix

| Area | Ownership defined | Thread/lifetime defined | Persistence defined | Failure defined | Test path defined | Budget defined where relevant | Ready for TA-16 lock |
|---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| Core/simulation | PASS | PASS | PASS | PASS | PASS | PASS | YES |
| World/scene/streaming | PASS | PASS | PASS | PASS | PASS | PASS | YES |
| Rendering | PASS | PASS | N/A persistent backend | PASS | PASS | PASS | YES |
| Physics/character/spaceflight | PASS | PASS | reconstruction contract | PASS | PASS | PASS | YES |
| Station simulation | PASS | PASS | PASS | PASS | PASS | PASS | YES |
| Runtime entities/combat | PASS | PASS | activation sync | PASS | PASS | PASS | YES |
| AI/navigation | PASS | PASS | consequential state handoff | PASS | PASS | PASS | YES |
| Missions/raids/events | PASS | PASS | PASS | PASS | PASS | PASS | YES |
| Content/assets | PASS | PASS | ContentId compatibility | PASS | PASS | PASS | YES |
| Input/UI/audio/presentation | PASS | PASS | profile/read-model rules | PASS | PASS | PASS | YES |
| Persistence | PASS | PASS | primary authority | PASS | PASS | PASS | YES |
| Performance/concurrency | PASS | PASS | N/A | PASS | PASS | primary authority | YES |
| Diagnostics/testing/CI | PASS | PASS | evidence rules | PASS | primary authority | performance gates | YES |

## 17. TA-16 Entry Criteria

TA-16 may begin because:

- GDS is Design Complete;
- TA-0 through TA-14 are Architecture Complete/Complete;
- TA-15 found no unresolved blocker or required correction;
- every GDS domain has technical traceability;
- dependency cycles are avoidable through existing interfaces;
- remaining decisions are implementation-lock details explicitly assigned to TA-16.

## 18. Verdict

```text
Architecture blockers: 0
Required corrections: 0
TA-16 lock items: 16 explicit categories
Engineering risks: 10 tracked categories
Risk with undefined mitigation: 0
Implementation area lacking test path: 0
Implementation area lacking owner: 0
TA-16 entry readiness: PASS
```
