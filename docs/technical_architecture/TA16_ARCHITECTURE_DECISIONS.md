# TA-16 — Architecture Decision Addendum

> **Status:** Accepted / Implementation Locked  
> **Authority:** Final implementation-handoff decisions  
> **Relationship:** Continues the accepted architecture-decision sequence after TA-14 AD-132

TA-16 fixes the implementation snapshot and handoff. These decisions do not change Design Complete gameplay semantics.

---

## AD-133 — TA16-V1 Is the First Reproducible Implementation Baseline

**Status:** Accepted

### Decision

The first implementation baseline is `TA16-V1`: C++23, Visual Studio/Build Tools 17.14.40 / MSVC v143 x64, CMake 4.3.3, LLVM/Clang 23.1.1 for the independent compiler/tooling line, and vcpkg baseline `a1cae005c39be7b18ba319fced856b68d7276271`. Upgrades are explicit baseline changes rather than floating environment drift.

### Rationale

Architecture cannot be called implementation-ready while compiler/build/dependency identity remains implicit.

---

## AD-134 — StarForge Uses glad2 v2.0.8 Generated Source, Not the vcpkg glad1 Port

**Status:** Accepted

### Decision

OpenGL 4.6 Core loader source is generated from glad2 v2.0.8 under a recorded generation contract and reviewed into `third_party/generated/glad/`. The selected vcpkg baseline's `glad` 0.1.36 port is not used as a substitute.

### Rationale

The architecture requires glad2; silently accepting a same-named glad1 package would break the locked backend contract.

---

## AD-135 — Concrete CMake Targets Are the Enforced Module Dependency Contract

**Status:** Accepted

### Decision

TA-16's named `sf_*`, `starforge*`, and test targets define the implementation DAG. Third-party libraries link only through their owning adapter/tool targets; app remains the composition root; gameplay/UI/persistence cannot introduce forbidden backend or reverse dependency edges.

### Rationale

A conceptual modular monolith needs a physical build graph or architectural boundaries will erode through convenient includes/linkage.

---

## AD-136 — Repository-Owned CMake Presets and CTest Labels Are Stable Developer/CI Entry Points

**Status:** Accepted

### Decision

Supported configuration/build/test flows use checked-in CMake presets plus TA-14 semantic CTest labels. Required CI suites prove nonzero discovery before execution. Machine-local IDE settings and hand-maintained executable lists are not certification authority.

### Rationale

Developers and CI must execute the same semantic suites through reproducible entry points.

---

## AD-137 — CI Activates Only Real Gates and Pins Actions by Immutable SHA

**Status:** Accepted

### Decision

TA-16 begins with real `StarForge / Build & Unit` and aggregate `StarForge / CI Gate`. Future TA-14 stable gates activate only when their owning real targets/tests exist. GitHub Actions references use reviewed immutable commit SHAs; placeholder-green jobs are prohibited.

### Rationale

A green check without an implemented contract creates false assurance and undermines branch protection.

---

## AD-138 — Numeric Performance Certification Uses SF-PERF-WIN-01

**Status:** Accepted

### Decision

TA-13 numeric 1080p High percentile certification uses controlled self-hosted reference runner `SF-PERF-WIN-01`: Ryzen 7 5800X, RTX 3080 10 GiB, 32 GiB DDR4-3200, NVMe storage and recorded Windows/driver/firmware state. Generic hosted VMs do not certify those numeric gates.

### Rationale

Performance thresholds require stable hardware identity; cloud-runner variance cannot become acceptance authority.

---

## AD-139 — Implementation Proceeds Dependency-First and Certifies Horizon Test Cell V0 Before Breadth

**Status:** Accepted

### Decision

After TA-16, implementation starts with core/identity/simulation/transactions, then content/persistence foundations, platform/rendering, world/physics/character, and the Horizon Test Cell V0 end-to-end slice. Broad station/AI/mission/spacecraft/raid/campaign work depends on that architectural slice evidence.

### Rationale

The riskiest architecture assumptions should fail cheaply in one narrow vertical slice before large feature breadth is built on them.

---

## AD-140 — Architecture Complete Contracts Become Implementation Locked Under Explicit Change Control

**Status:** Accepted

### Decision

At TA-16 PASS, TA-0 through TA-16 baseline contracts are Implementation Locked for `TA16-V1`. Private compatible implementation freedom remains allowed, but ownership/threading/persistence/determinism/dependency/gameplay semantic changes require the TA-16 C0–C3 change process, with GDS changes preceding technical changes when gameplay semantics move.

### Rationale

Implementation needs stable contracts without pretending private algorithms/data structures can never evolve.

---

## AD-141 — Main Uses Short-Lived PR Integration and Evidence-Based Milestones

**Status:** Accepted

### Decision

`main` is the integration authority; implementation uses short-lived typed branches and reviewed PRs, with squash merge as the default. Stable required checks grow only when real suites exist. Milestones M0–M9 are promoted by exact-SHA evidence rather than calendar declaration. Normal force-push to `main` is prohibited.

### Rationale

Repository process must preserve the same traceability/exact-SHA assumptions used by TA-14 certification and prevent long-lived branch authority forks.

---

## Consolidation Rule

TA-16 finalization consolidates the decision sequence AD-101 through AD-141 into the project-wide decision registry without changing accepted semantics. The TA-12/13/14/16 addendum files remain historical source records for auditability.
