# TA-14 — Architecture Decision Addendum

> **Status:** Accepted  
> **Authority:** TA-14 testing, diagnostics, quality, CI, performance-certification, and evidence decisions  
> **Relationship:** Continues the accepted architecture-decision sequence after TA-13 AD-121

This addendum records the accepted project-wide decisions introduced by TA-14. TA-16 may mechanically consolidate them into the central decision log without changing semantics.

---

## AD-122 — Gameplay Verification Is Headless by Default and Layered by Smallest Sufficient Scope

**Status:** Accepted

### Decision

StarForge uses CMake/CTest with Catch2 and separates unit/domain, headless integration, deterministic scenario, backend smoke, golden/compatibility, fault/corruption, and performance layers. Gameplay rules run without GLFW/OpenGL/audio unless the backend itself is under test.

### Rationale

Fast deterministic semantic tests catch authority/transaction failures earlier and avoid coupling game correctness to presentation hardware.

---

## AD-123 — Deterministic Scenario Certification Compares Semantic Checkpoints Across Worker and Frame Schedules

**Status:** Accepted

### Decision

Scripted scenarios use fixed seeds and SimulationTick-stamped semantic inputs. Worker-count and render-cadence variants may differ in completion latency but must produce equivalent authoritative checkpoints, with state hashes used only as diagnostic evidence alongside explicit semantic assertions.

### Rationale

Hardware concurrency and render FPS must never become hidden gameplay inputs.

---

## AD-124 — Third-Party/Platform Backends Are Certified Through StarForge-Owned Adapter Smoke Tests

**Status:** Accepted

### Decision

Jolt, Recast/Detour, OpenGL/GLFW, miniaudio, FreeType/HarfBuzz, fastgltf, KTX2, meshoptimizer and glslang receive adapter-level smoke/integration tests. Raw backend IDs/output are not persistent or gameplay authority.

### Rationale

The project must detect integration/lifetime/capability failures without rewriting gameplay tests around library internals.

---

## AD-125 — Content Is a Build-Tested Product With Deterministic Cook and Shader Gates

**Status:** Accepted

### Decision

Closed schemas, typed references, provenance, all required GLSL variants, collision/nav products, procedural-module feasibility, clean/incremental/no-op cooks and worker-count deterministic ContentBuildId/fingerprints are CI-verifiable. CI never auto-rewrites content or goldens.

### Rationale

Invalid/nondeterministic cooked content is a build failure, not a runtime fallback opportunity.

---

## AD-126 — Persistence Compatibility Is Protected by Binary Goldens, Migration Matrices, Corruption Tests, and Write Fault Injection

**Status:** Accepted

### Decision

The exact v1 container bytes, supported historical migrations, ContentId compatibility, CRC/truncation/size rejection, all-or-nothing staging, atomic-save crash points, catalog recovery, autosave rotation, RNG continuation and profile fail-soft behavior are covered by dedicated fixture suites. Historical goldens are never auto-updated.

### Rationale

Save compatibility/crash safety require evidence across versions and failure boundaries, not only round-trip tests of the latest build.

---

## AD-127 — Engineering Diagnostics Are Typed, Bounded, Correlated, and Never Gameplay Authority

**Status:** Accepted

### Decision

Development diagnostics use stable codes/structured context, bounded logging/traces, strong invariant assertions and read-only-by-default debug inspectors. External invalid data uses shipping validation rather than debug assertions, and Fatal corrupted state is not continued or heuristically crash-saved.

### Rationale

Complex systemic failures must be reproducible without allowing logs/debug UI to create a second mutation path.

---

## AD-128 — Project Source Uses Warnings-as-Errors, Deterministic Formatting, Static Analysis, and Sanitizer Certification

**Status:** Accepted

### Decision

Project-owned code builds under high warning levels with warnings-as-errors, deterministic formatting and pinned static analysis. ASan/UBSan are blocking correctness gates; leak/race checks run where supported, with TSan part of scheduled/release certification. Third-party warnings are isolated.

### Rationale

Lifetime/UB/threading defects should be mechanically rejected before they become simulation/persistence corruption.

---

## AD-129 — GitHub Actions Exposes Stable Aggregate Required Checks With Least Privilege

**Status:** Accepted

### Decision

The implementation CI exposes stable aggregate checks including Build & Unit, Headless Determinism, Content Validation, Persistence Compatibility, Static Analysis, Sanitizers, Backend Smoke where stable, and a final `StarForge / CI Gate`. Workflows run least-privilege and untrusted PRs receive no secrets by default.

### Rationale

Branch protection should depend on stable semantic gates rather than volatile matrix-job names, while CI remains safe for public contributions.

---

## AD-130 — Reference Performance Certification Uses Controlled Runners and TA-13 Percentile Gates

**Status:** Accepted

### Decision

Generic cloud runners may perform coarse performance smoke only. TA-13 CPU/GPU percentile and memory/streaming regression certification requires TA-16-pinned controlled/reference runners, versioned scenario baselines, deterministic semantic equivalence, and reviewed baseline changes rather than rerun-until-green behavior.

### Rationale

Noisy/unknown hardware cannot provide trustworthy frame-time certification, and performance success cannot waive correctness.

---

## AD-131 — Flaky Tests and CI Evidence Use Explicit Quarantine, Retention, and Exact-SHA Certification

**Status:** Accepted

### Decision

Required suites fail on unexpected zero-test discovery. Flaky tests require issue/owner/reason/expiry quarantine and cannot hide critical determinism/persistence/sanitizer failures for release/Implementation Locked certification. Evidence/artifacts are bounded, privacy-safe, reproducible and tied to an exact commit SHA.

### Rationale

A manual rerun or silent skip must never erase evidence that a supposedly deterministic contract is unstable.

---

## AD-132 — Executable CI Workflows Are Materialized Only With TA-16 Real Build/Test Targets

**Status:** Accepted

### Decision

TA-14 defines workflow topology, gates, security and evidence semantics but does not add placeholder-green GitHub Actions before CMake/CTest/source scaffolding exists. TA-16 creates/pins executable workflow YAML together with real targets, presets, runner/toolchain versions and branch-required checks.

### Rationale

A CI badge/check is valuable only when it executes real project contracts; placeholder workflows would provide false assurance before implementation begins.

---

## Consolidation Rule

TA-16 contract locking may consolidate AD-122 through AD-132 into `ARCHITECTURE_DECISIONS.md` together with prior addenda. Consolidation is editorial only and must preserve the accepted decisions above.