# TA-14 — GitHub Actions CI Workflows, Required Checks, and Caching

> **Status:** Architecture Complete  
> **Authority:** GitHub Actions workflow topology, trigger policy, required-status design, job dependencies, matrix strategy, caching, concurrency/cancellation, security permissions, and CI failure semantics

## 1. Purpose

StarForge CI must convert the test/quality architecture into a small set of stable, reviewable status checks suitable for branch protection without turning every internal job name into a permanent repository contract.

## 2. Workflow Principle

CI is layered into fast pull-request gates, deeper scheduled/reference gates, and explicit release/implementation-certification gates.

A faster workflow may omit expensive tests only when another mandatory workflow owns those tests before release/Implementation Locked certification.

## 3. Planned Workflow Files

When implementation scaffolding begins, TA-16 creates/locks workflow files conceptually equivalent to:

```text
.github/workflows/ci-pr.yml
.github/workflows/ci-sanitizers.yml
.github/workflows/ci-content.yml
.github/workflows/ci-persistence.yml
.github/workflows/ci-performance.yml
.github/workflows/ci-nightly.yml
```

Exact filenames may be refined by TA-16, but required status-check semantics remain stable.

## 4. Stable Required Status Checks

The initial branch-protection contract should expose these stable aggregate checks once implementation exists:

```text
StarForge / Build & Unit
StarForge / Headless Determinism
StarForge / Content Validation
StarForge / Persistence Compatibility
StarForge / Static Analysis
StarForge / Sanitizers
StarForge / CI Gate
```

`StarForge / CI Gate` is the final aggregate job depending on all ordinary required PR checks.

This avoids branch rules depending on every matrix subjob name.

## 5. Backend Smoke Status

Backend smoke is exposed separately as:

```text
StarForge / Backend Smoke
```

It becomes PR-required once a stable CI environment can exercise the required backend subset reliably.

Reference-GPU visual/performance certification remains separate from ordinary cloud PR blocking.

## 6. Performance Status

TA-13 performance certification uses:

```text
StarForge / Performance Regression
```

This is required for release/Implementation Locked milestones and selected protected integration paths once reference runners exist.

It is not faked on generic cloud VMs whose hardware variability invalidates numeric GPU/CPU budgets.

## 7. PR Workflow

Ordinary pull requests run, in parallel where dependencies allow:

1. configure/toolchain metadata;
2. build with warnings-as-errors;
3. unit/domain tests;
4. headless deterministic scenarios;
5. content schema/reference/glslang validation;
6. persistence current/golden/core migration/corruption smoke;
7. static analysis/format check;
8. supported sanitizer suites;
9. backend smoke where environment supports it;
10. aggregate CI Gate.

## 8. Push-to-Main

Pushes/merges to `main` rerun the same required correctness gates rather than assuming the PR result still applies.

Additional post-merge integration suites may run as configured.

## 9. Nightly / Scheduled

Scheduled workflows cover expensive matrices such as:

- longer deterministic S2/S3 scenarios;
- broader worker-count matrix;
- full migration/corruption corpus;
- extended sanitizer/TSan runs;
- clean content cook determinism;
- repeated transition/leak tests;
- large fuzz corpus batches;
- representative performance smoke where runner suitability exists.

Nightly failure creates a visible engineering signal and cannot be ignored indefinitely before release certification.

## 10. Release / Implementation Certification

A release/Implementation Locked candidate requires a known commit to have passing evidence for:

- ordinary required CI;
- full persistence compatibility suite;
- content determinism suite;
- required backend smoke;
- sanitizer certification;
- TA-13 performance matrix on reference hardware;
- any TA-15/TA-16-specific integration gate.

## 11. Matrix Strategy

CI matrices are bounded and purposeful.

Architecture anticipates combinations such as:

- Windows x64 + MSVC primary;
- Linux x64 + Clang for sanitizers/static analysis/headless portability;
- Debug/Release/Profile where relevant;
- selected worker counts for determinism.

TA-16 pins exact supported versions/runner labels.

## 12. Matrix Explosion Rule

Not every compiler × config × sanitizer × worker count runs on every PR.

Coverage is distributed across PR/nightly/reference workflows while preserving defined certification requirements.

## 13. Determinism Matrix

At least one ordinary CI suite compares deterministic scenario checkpoints under multiple worker counts.

Scheduled suites expand the matrix where runtime permits.

Different completion speed is acceptable; semantic state divergence fails.

## 14. Workflow Concurrency

For pull requests, newer commits can cancel superseded in-progress workflow runs for that same PR to conserve resources.

Cancellation is infrastructure behavior only; a canceled run is not a passing result.

Protected merge requires checks for the current head commit.

## 15. Main / Release Cancellation

Main/release certification jobs should not be casually canceled by a later unrelated commit if their artifacts/evidence are required for that exact candidate SHA.

## 16. Permissions

Workflow permissions use least privilege.

Ordinary build/test jobs default to repository read access and artifact/status capabilities supplied by Actions.

They do not require write access to source, releases, issues, packages, or branch refs unless a separately reviewed workflow explicitly needs it.

## 17. Fork / Untrusted PR Safety

Untrusted pull-request code is never given repository secrets by default.

Workflows that require privileged credentials are separated and must not execute arbitrary untrusted code with those credentials.

## 18. No CI Auto-Commit

Validation workflows do not:

- rewrite formatting and push;
- update goldens;
- regenerate content and commit;
- mutate branch rules;
- auto-upgrade dependencies.

CI reports differences; reviewed source changes fix them.

## 19. Dependency Pinning

GitHub Actions used by workflows are pinned to reviewed major/version or immutable commit policy chosen by repository governance.

TA-16 records exact workflow dependency pins at scaffolding time.

## 20. vcpkg Cache

Dependency caching can key on:

- OS/toolchain;
- architecture;
- vcpkg baseline/manifest hash;
- relevant feature/configuration set.

A cache miss affects build time, not build semantics.

## 21. CMake Build Cache

Compiler/build caching may be introduced when deterministic/safe for the supported toolchain.

CI must periodically perform clean builds so a stale cache cannot hide missing dependencies/generated inputs.

## 22. Content Cache

Content-cook cache keys use TA-10 fingerprints/tool versions.

Clean-cook determinism jobs deliberately bypass or isolate cache when proving reproducibility.

## 23. Cache Trust Boundary

Caches are performance optimizations, never correctness authority.

A build/test must remain correct after full cache eviction.

No required golden/baseline exists only in an ephemeral cache.

## 24. Job Timeouts

Every CI job has an explicit bounded timeout suitable to its suite.

Timeout is a failure/infrastructure diagnostic, not permission to mark unexecuted tests passed.

Exact minutes are TA-16/workflow tuning data.

## 25. Test Sharding

Large deterministic/scenario suites may shard by stable test IDs.

Sharding must not introduce cross-shard state dependencies or different semantic configuration.

All shards are required for aggregate success.

## 26. Fail Fast

Matrix jobs may use fail-fast selectively.

For expensive debugging, collecting multiple independent failures can be more valuable than immediate cancellation.

Aggregate required status fails if any required shard/job fails.

## 27. Artifacts

Failure jobs retain bounded artifacts defined by TA-14 evidence policy, such as:

- CTest/JUnit results;
- logs;
- sanitizer reports;
- deterministic state diffs;
- content diagnostics/fingerprint diffs;
- persistence failure fixture/context;
- crash dumps where safe;
- performance JSON summaries/traces on benchmark jobs.

## 28. Test Result Format

CTest/Catch2 results are exported in a machine-readable form suitable for GitHub summaries/artifacts.

Human-readable step summaries identify the failing suite/test without requiring log archaeology.

## 29. Aggregate Gate

`StarForge / CI Gate` executes only after all required upstream jobs and has simple semantics:

```text
all required dependencies success -> PASS
anything required failed/canceled/skipped unexpectedly -> FAIL
```

No custom logic may convert a failed required job into success.

## 30. Branch Protection Contract

Once implementation CI exists and is stable, repository rulesets should require the stable aggregate checks for protected `main` according to TA-16 governance.

Changing required check names becomes a deliberate repository-governance change.

## 31. Architecture-Only Phase Boundary

TA-14 defines CI architecture but does **not** create fake executable build workflows before CMake/source scaffolding exists.

TA-16 is responsible for materializing the workflow YAML together with real targets/presets so CI starts from executable contracts rather than placeholder green jobs.

## 32. No Empty Green Gate

A workflow/job that runs no intended tests due to a filter/discovery error must fail or explicitly report zero-test policy violation.

A green CI check cannot mean “nothing executed.”

## 33. Flake Visibility

Workflow reruns retain the original failure evidence.

A manual rerun passing does not erase classification of the earlier failure; flaky behavior requires investigation/quarantine policy.

## 34. CI Metadata

Every run summary records at least:

- commit SHA;
- workflow version from that SHA;
- runner OS/image;
- compiler/CMake versions;
- vcpkg baseline;
- test suite configuration;
- content fixture/build version where applicable.

## 35. External Services

Core CI must not depend on third-party SaaS services to decide gameplay correctness.

Optional reporting integrations cannot become a hidden single point of truth.

## 36. Explicit Non-Goals

TA-14 does not yet:

- create implementation workflows before TA-16;
- pin final runner/compiler versions;
- guarantee cloud GPU hardware;
- allow secrets in untrusted PR execution;
- auto-merge based solely on CI.

## 37. Dependencies

Depends on TA-14 test/source-quality specs, GitHub/GitHub Actions baseline, TA-13 performance architecture, existing repository ruleset governance, and TA-16 scaffolding/target/toolchain locking.

## 38. Open Questions

None within CI workflow/status architecture.