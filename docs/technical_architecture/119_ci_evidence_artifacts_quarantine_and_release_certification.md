# TA-14 — CI Evidence, Artifacts, Quarantine, and Release Certification

> **Status:** Architecture Complete  
> **Authority:** Test evidence retention, artifact policy, failure reproduction bundles, quarantine governance, flaky-test handling, certification manifests, and release/Implementation Locked verification handoff

## 1. Purpose

A passing status check is useful only if the project can determine exactly what ran, with what inputs/toolchain, and what evidence exists when it fails.

TA-14 therefore defines one bounded evidence model for ordinary CI, scheduled certification, and later TA-16 Implementation Locked milestones.

## 2. Evidence Principle

Every required gate must be able to answer:

- which commit was tested;
- which suite/version/configuration ran;
- which toolchain/runner was used;
- whether intended tests actually executed;
- what failed and how to reproduce it;
- where bounded supporting artifacts are stored.

## 3. Machine-Readable Test Results

CTest/Catch2 suites export machine-readable result data suitable for CI aggregation.

Reports include:

- suite/test names;
- pass/fail/skip;
- duration;
- failure text;
- labels;
- seed/scenario context where relevant.

## 4. Zero-Test Failure

If a required suite discovers zero tests unexpectedly, the gate fails.

An explicit intentionally empty suite is not allowed to satisfy a branch-required correctness check.

## 5. Success Artifacts

Fast PR jobs normally retain only compact summaries/results unless project policy needs more.

Large logs/traces are primarily failure artifacts.

Performance/reference jobs retain successful benchmark summaries because they are historical baseline evidence.

## 6. Failure Artifacts

Depending on suite, failure evidence can include:

- test report;
- console/structured log;
- assertion diagnostic code;
- deterministic command/seed replay bundle;
- state checkpoint diff;
- sanitizer report;
- crash stack/minidump where safe;
- content validation/fingerprint diff;
- persistence fixture/failure stage;
- profiler trace excerpt;
- renderer image diff for controlled image tests.

## 7. Artifact Size Bounds

Artifacts are bounded and curated.

A runaway log/core/trace must not create unlimited uploads.

Oversized artifacts are truncated/filtered with explicit notice while retaining the most useful tail/context according to subsystem policy.

## 8. Artifact Privacy / Secrets

Artifacts must not contain:

- credentials/tokens;
- arbitrary user documents;
- private local filesystem data beyond normalized diagnostics;
- unlicensed proprietary third-party source content unless repository policy permits distribution.

CI performs basic secret-safe handling and relies on least-privilege workflow permissions.

## 9. Retention Classes

Evidence is classified conceptually as:

- **PR diagnostic:** short retention;
- **nightly diagnostic:** medium retention;
- **compatibility golden:** version-controlled/long-lived;
- **performance baseline/certification:** long-lived for relevant milestone/release lifetime;
- **release certification manifest:** retained with release/milestone evidence.

Exact day counts are repository/TA-16 policy.

## 10. Golden Fixtures Are Not Ephemeral Artifacts

Required save/content/RNG/schema golden fixtures are version-controlled or otherwise immutable project test inputs.

They cannot exist only as expiring CI artifacts.

## 11. Failure Reproduction Bundle

A deterministic scenario failure bundle contains applicable:

```text
commit SHA
suite/scenario version
seed
command timeline
worker count
fixture/content build version
last good checkpoint
failing SimulationTick
state diff / diagnostic code
```

The bundle is designed for local replay without internet dependence.

## 12. Flaky Definition

A test is flaky when identical declared semantic inputs/configuration can produce both pass and fail without an intentional source/environment change.

A single manual rerun passing does not automatically classify the original failure as infrastructure noise.

## 13. Quarantine Entry

Temporary quarantine requires a machine-readable/reviewable record with:

- test ID;
- issue/link;
- owner;
- first observed commit/date;
- reason/evidence;
- affected gates;
- expiry/review deadline.

## 14. Quarantine Restrictions

A quarantined test:

- continues to execute where practical and reports its result;
- cannot satisfy a required locked-contract proof by being ignored;
- cannot remain indefinitely;
- cannot hide a sanitizer, persistence compatibility, ownership, determinism, or security/integrity failure for release certification.

## 15. Quarantine Expiry

Expired quarantine automatically becomes a gate failure or requires explicit reviewed renewal.

Silent permanent skip is prohibited.

## 16. Infrastructure Failure

CI distinguishes semantic test failure from infrastructure failure such as:

- hosted runner unavailable;
- artifact service outage;
- runner disk exhaustion unrelated to project budget;
- transient checkout service failure.

Infrastructure failure can be rerun but never converted directly to semantic PASS.

## 17. Runner Health

Reference/self-hosted runners perform preflight health checks for:

- free disk;
- expected hardware/driver identity;
- expected toolchain;
- stale processes;
- thermal/power mode where relevant to performance;
- writable temp/artifact locations.

A failed preflight invalidates the benchmark rather than recording misleading numbers.

## 18. Certification Manifest

A release or TA-16 Implementation Locked milestone can produce a project-owned certification manifest containing:

- commit SHA;
- architecture/GDS maturity identifiers;
- toolchain/dependency versions;
- ContentBuildId/fixture versions used;
- required CI check results;
- sanitizer certification;
- persistence compatibility suite result;
- content determinism result;
- backend smoke result;
- performance baseline/reference-runner result;
- known accepted waivers/quarantines, ideally none for locked critical contracts.

## 19. Manifest Authority

The certification manifest is evidence, not gameplay/runtime authority.

It cannot make a failing test pass; it only records the actual reviewed state of evidence.

## 20. Commit Immutability

Certification evidence is always tied to an exact commit SHA.

A later commit requires new evidence according to its milestone/gate policy.

## 21. Release Candidate Rule

A release candidate cannot be certified if any mandatory correctness gate is:

- failing;
- canceled;
- unexpectedly skipped;
- absent for the candidate SHA;
- satisfied only by an expired/forbidden quarantine.

## 22. Performance Certification Rule

Performance certification requires the correct reference-runner class and scenario version.

A generic cloud-runner green result cannot substitute for missing reference GPU/CPU evidence.

## 23. Compatibility Certification Rule

Persistence compatibility certification must include all still-supported historical fixture versions/migration chains designated by current support policy.

Dropping historical support requires an explicit product/architecture decision, not deleting a failing fixture.

## 24. Content Certification Rule

Content certification requires current schema/reference/provenance validation and deterministic cook evidence appropriate to the milestone.

A cached cooked tree alone is insufficient.

## 25. Sanitizer Certification Rule

Required sanitizer suite must execute against the candidate's project code.

A known reproducible ASan/UBSan project-owned failure blocks certification.

## 26. Architecture Document Validation

Documentation/index validation checks that authoritative architecture paths referenced by roadmap/readmes exist and phase status is internally consistent.

This is a lightweight gate and does not pretend prose correctness can be fully automated.

## 27. CI Configuration Testing

Workflow changes are reviewed like code.

Where practical, local/static workflow validation checks YAML/schema/expression issues before merge.

A CI workflow cannot weaken its own required gate by changing path filters to skip relevant source accidentally.

## 28. Path Filters

Path filters may skip expensive subsystem jobs only where the aggregate correctness policy still guarantees required coverage.

For release/Implementation Locked certification, full relevant suites run regardless of PR path heuristic.

## 29. Artifact Naming

Artifacts include stable context such as:

```text
<gate>-<platform>-<commit-short>-<run-id>
```

and internal metadata carries full SHA.

## 30. CI Summary

The aggregate CI summary reports:

- required gate status;
- test counts;
- quarantined tests;
- sanitizer/content/persistence status;
- performance status when run;
- artifact references;
- toolchain/runner identity.

## 31. Debugging Failed CI

The expected workflow for a semantic failure is:

1. inspect summary/diagnostic code;
2. retrieve bounded failure artifact;
3. reproduce locally with recorded command/seed/fixture;
4. fix code/test/architecture;
5. rerun relevant local suite;
6. push new commit and obtain fresh CI evidence.

## 32. No Manual Green Override as Normal Flow

Repository administration may technically override branch checks, but TA-14 architecture treats bypass as exceptional governance, not an ordinary development path.

Any override for a milestone/release needs documented reason and follow-up evidence.

## 33. Historical Trend Data

Performance/test-duration/flakiness trend reporting may be retained where useful, but no external analytics service becomes the only source of certification truth.

## 34. Test Duration Budgets

CI records suite durations and flags significant growth so the project can keep fast feedback without deleting important tests.

Optimization choices include sharding, fixture reuse and test-layer separation—not weakening semantic assertions.

## 35. Artifact Reproducibility

An artifact should identify enough source/tool/fixture metadata that another developer can understand which configuration produced it even after the ephemeral runner is gone.

## 36. Explicit Non-Goals

TA-14 does not define:

- final artifact retention day counts;
- production crash upload service;
- user telemetry backend;
- release packaging/signing/distribution;
- automatic bug filing from every failure.

Those can be added later where needed.

## 37. Dependencies

Depends on all TA-14 suites, GitHub Actions architecture, TA-12/TA-13 goldens/performance evidence, and TA-16 implementation/milestone governance.

## 38. Open Questions

None within CI evidence/certification scope.