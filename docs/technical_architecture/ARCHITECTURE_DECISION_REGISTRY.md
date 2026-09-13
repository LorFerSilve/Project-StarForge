# Project StarForge — Consolidated Architecture Decision Registry

> **Status:** Implementation Locked  
> **Authority:** Canonical numbering/source registry for accepted architecture decisions  
> **Coverage:** AD-001 through AD-141, no gaps and no reused numbers

## 1. Purpose

This file completes TA-16's decision-log consolidation without rewriting accepted decision bodies. The detailed decision/rationale text remains in its original accepted source file; this registry makes the full sequence centrally discoverable and fixes which source owns each number.

If a summary below appears less specific than the source decision, the source decision controls. Owning technical specifications still control detailed mechanics.

## 2. Canonical Source Ranges

| Decision range | Canonical detailed source | Status |
|---|---|:---:|
| AD-001 — AD-100 | `ARCHITECTURE_DECISIONS.md` | Accepted |
| AD-101 — AD-111 | `TA12_ARCHITECTURE_DECISIONS.md` | Accepted |
| AD-112 — AD-121 | `TA13_ARCHITECTURE_DECISIONS.md` | Accepted |
| AD-122 — AD-132 | `TA14_ARCHITECTURE_DECISIONS.md` | Accepted |
| AD-133 — AD-141 | `TA16_ARCHITECTURE_DECISIONS.md` | Accepted / Implementation Locked |

The addendum files are retained deliberately as immutable historical source records; consolidation means one authoritative registry/number sequence, not destructive duplication of the same accepted prose.

## 3. AD-001 — AD-100

AD-001 through AD-100 are already consolidated directly in `ARCHITECTURE_DECISIONS.md`. They cover the modular monolith, C++/OpenGL/runtime authority, persistent identity/transactions/persistence/RNG, scene/streaming/render/physics/station/runtime entity/AI/navigation/mission/content/input/UI/audio/presentation architecture.

**Registry status:** 100 accepted decisions, contiguous.

## 4. TA-12 Decisions — AD-101 through AD-111

| ID | Decision |
|---|---|
| AD-101 | Campaign saves use immutable committed generations. |
| AD-102 | Save Container V1 has fixed 128-byte header and 64-byte directory entries. |
| AD-103 | `SectionKind` plus explicit domain codecs own save payload meaning. |
| AD-104 | Crash-safe save commit validates a pending file before atomic replacement/promotion. |
| AD-105 | Load is staged and replaces the live `SessionRoot` exactly once after validation. |
| AD-106 | Migration is deterministic, source-preserving and identity-preserving where contractually required. |
| AD-107 | `ContentBuildId` is metadata; actual `ContentId` resolution/compatibility controls saved content meaning. |
| AD-108 | Save catalog/manifests are rebuildable caches, not gameplay authority. |
| AD-109 | Profile/application settings use separate fail-soft persistence. |
| AD-110 | Persistence diagnostics/recovery reuse shipping parsers and never heuristically repair authoritative state. |
| AD-111 | Loading advances zero gameplay time; the first resumed gameplay tick is saved tick + 1. |

Detailed authority: `TA12_ARCHITECTURE_DECISIONS.md`.

## 5. TA-13 Decisions — AD-112 through AD-121

| ID | Decision |
|---|---|
| AD-112 | Performance budgets never authorize gameplay-semantic degradation. |
| AD-113 | Runtime asynchronous work uses one bounded shared worker pool. |
| AD-114 | Catch-up is bounded per render frame but Simulation Time ticks are never skipped. |
| AD-115 | Active local runtime has explicit entity/physics scale envelopes. |
| AD-116 | Station/AI/navigation/mission/strategic scaling preserves chronological semantics. |
| AD-117 | Residency is budgeted/evictable; required gameplay content Holds before semantic compromise. |
| AD-118 | Reference software target is 60 FPS at 1080p High with percentile gates. |
| AD-119 | Decorative presentation may virtualize/degrade; critical semantics may not. |
| AD-120 | Persistence and content build have explicit memory/concurrency envelopes. |
| AD-121 | Performance certification uses representative scenarios and traceable percentile regressions. |

Detailed authority: `TA13_ARCHITECTURE_DECISIONS.md`.

## 6. TA-14 Decisions — AD-122 through AD-132

| ID | Decision |
|---|---|
| AD-122 | Gameplay verification is headless by default and layered by smallest sufficient scope. |
| AD-123 | Deterministic scenarios compare semantic checkpoints across worker/frame schedules. |
| AD-124 | Third-party/platform backends are certified through StarForge-owned adapter smoke tests. |
| AD-125 | Content is a build-tested product with deterministic cook/shader gates. |
| AD-126 | Persistence compatibility is protected by binary goldens, migrations, corruption tests and write fault injection. |
| AD-127 | Diagnostics are typed, bounded, correlated and never gameplay authority. |
| AD-128 | Project source uses warnings-as-errors, deterministic formatting, static analysis and sanitizer certification. |
| AD-129 | GitHub Actions exposes stable aggregate required checks with least privilege. |
| AD-130 | Numeric performance certification uses controlled runners and TA-13 percentile gates. |
| AD-131 | Flaky tests/evidence use explicit quarantine, retention and exact-SHA certification. |
| AD-132 | Executable CI workflows are materialized only with real TA-16 build/test targets. |

Detailed authority: `TA14_ARCHITECTURE_DECISIONS.md`.

## 7. TA-16 Decisions — AD-133 through AD-141

| ID | Decision |
|---|---|
| AD-133 | `TA16-V1` is the first reproducible implementation baseline. |
| AD-134 | StarForge uses glad2 v2.0.8 generated source, not the vcpkg glad1 port. |
| AD-135 | Concrete CMake targets are the enforced module dependency contract. |
| AD-136 | Repository-owned CMake presets and CTest labels are stable developer/CI entry points. |
| AD-137 | CI activates only real gates and pins Actions by immutable SHA. |
| AD-138 | Numeric performance certification uses controlled runner `SF-PERF-WIN-01`. |
| AD-139 | Implementation is dependency-first and certifies Horizon Test Cell V0 before breadth. |
| AD-140 | Architecture Complete contracts become Implementation Locked under explicit change control. |
| AD-141 | `main` uses short-lived PR integration and evidence-based milestones. |

Detailed authority: `TA16_ARCHITECTURE_DECISIONS.md`.

## 8. Continuity Audit

```text
First decision: AD-001
Last decision:  AD-141
Expected count: 141
Number gaps:    0
Number reuse:   0
Accepted source ranges: 5
TA-12/13/14 addenda centrally registered: YES
TA-16 continuation registered: YES
```

## 9. New Decision Procedure

The next project-wide architecture decision, if required, is **AD-142**.

A new decision must:

- identify its owning GDS/TA/IMP context;
- state whether it changes `TA16-V1`;
- update the owning technical specification;
- update this registry;
- run affected validation/evidence;
- never reuse or renumber a historical AD.

## 10. Lock Verdict

The architecture decision sequence AD-001 through AD-141 is centrally registered, contiguous, and Implementation Locked for the first implementation baseline. Historical addenda remain audit evidence rather than competing numbering authorities.
