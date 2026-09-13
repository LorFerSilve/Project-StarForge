# TA-16 — CI Workflows, Action Pins, and Required-Check Lock

> **Status:** Implementation Locked  
> **Baseline:** `TA16-V1`  
> **Authority:** Executable GitHub Actions topology, immutable action pins, check-name governance, activation rules, and branch-protection target state

## 1. Core Rule

CI may report success only for contracts that exist and execute. TA-16 therefore starts with a real bootstrap gate and activates the broader TA-14 check set incrementally as their real targets/suites enter the repository.

No placeholder job may return success for an unimplemented subsystem.

## 2. Workflow Files

The locked workflow namespace is:

```text
.github/workflows/ci-pr.yml
.github/workflows/ci-sanitizers.yml
.github/workflows/ci-content.yml
.github/workflows/ci-persistence.yml
.github/workflows/ci-nightly.yml
.github/workflows/ci-performance.yml
```

Only workflows whose owned targets exist are committed/enabled. The filename is reserved before activation so later phases do not invent new CI topology ad hoc.

TA-16 initially materializes `ci-pr.yml` because its bootstrap Build & Unit target exists.

## 3. Initial Executable Checks

At TA-16 closure the real required-check candidates are:

```text
StarForge / Build & Unit
StarForge / CI Gate
```

`Build & Unit` must:

1. checkout the exact candidate SHA;
2. establish the locked vcpkg baseline;
3. configure the locked Windows/MSVC preset;
4. compile project-owned bootstrap targets with warnings-as-errors;
5. prove unit-test discovery is non-zero;
6. run the `unit` CTest suite;
7. execute the headless bootstrap executable;
8. fail on any step failure.

For a pull request, the candidate SHA is the exact PR head SHA. For a push, it is the exact pushed `github.sha`. The workflow explicitly checks out and verifies that SHA rather than relying on an implicit synthetic PR merge ref.

`CI Gate` depends on every currently required upstream gate and fails if a required dependency failed/canceled/skipped unexpectedly.

## 4. Future Stable Check Names

The following TA-14 check names are reserved and become branch-required only when the owning real suite is implemented:

```text
StarForge / Headless Determinism
StarForge / Content Validation
StarForge / Persistence Compatibility
StarForge / Static Analysis
StarForge / Sanitizers
StarForge / Backend Smoke
StarForge / Performance Regression
```

The final ordinary aggregate remains:

```text
StarForge / CI Gate
```

A check name's activation commit must contain the real target/test selection and zero-test guard.

## 5. Immutable Action Pins

TA16-V1 locks GitHub Actions by immutable commit SHA rather than floating tags:

| Action | Human version | Immutable commit |
|---|---:|---|
| `actions/checkout` | v7.0.1 | `3d3c42e5aac5ba805825da76410c181273ba90b1` |
| `actions/cache` | v6.1.0 | `55cc8345863c7cc4c66a329aec7e433d2d1c52a9` |
| `actions/upload-artifact` | v7.0.1 | `043fb46d1a93c77aae656e7c1c64a875d1fc6a0a` |
| `actions/download-artifact` | v8.0.1 | `3e5f45b2cfb9172054b4087a40e8e0b5a5461e7c` |

The initial workflow needs only `checkout`; other pins are reserved for the first workflow that actually requires them.

Third-party convenience Actions are avoided when a short transparent shell/CMake command can perform the same bootstrap step.

## 6. Runner Labels

Ordinary CI baseline:

```text
Windows primary: windows-2022
Linux portability/static/sanitizer: ubuntu-24.04
```

The Windows bootstrap certification job additionally asserts:

```text
Visual Studio 2022 17.14.39
installation build 17.14.37614.0
MSVC v143 14.44 x64 toolset family
```

Hosted image revisions are recorded in workflow evidence. Runner labels may receive provider image servicing; the compiler/tool versions are asserted/recorded separately and a toolchain mismatch fails the certification job rather than silently redefining TA16-V1.

TA-16 cross-validation explicitly rejected `windows-2025` for this baseline because that label currently resolves to a Visual Studio 2026 image. Selecting `windows-2022` preserves the locked Visual Studio 2022 generator/toolset contract instead of weakening version validation.

## 7. vcpkg Bootstrap in CI

CI obtains vcpkg directly from its official repository, checks out exactly:

```text
a1cae005c39be7b18ba319fced856b68d7276271
```

then bootstraps that checkout and exports `VCPKG_ROOT`.

This avoids relying on whatever vcpkg version happens to be preinstalled on a hosted runner.

## 8. CMake Distribution and Compiler Evidence in CI

CMake 4.3.3 is installed from Kitware's official GitHub release asset rather than from a floating runner installation or a package-manager mirror:

```text
asset:  cmake-4.3.3-windows-x86_64.zip
source: Kitware/CMake release v4.3.3
SHA-256: 935ade9e5e8723583c07f44c5592cea2a1c8f65c56ca7e07b34c025c880e0bd6
```

CI verifies the archive digest before extraction and then verifies `cmake --version == 4.3.3`. PyPI is not certification authority for this snapshot; absence of a matching wheel may not silently downgrade or upgrade CMake.

A workflow log records:

```text
candidate commit SHA
runner OS/image metadata
Visual Studio installation path/version
MSVC v143 14.44 toolset directory
cl.exe file version
CMake archive SHA-256
cmake --version
vcpkg baseline SHA
```

A mismatched CMake/compiler certification environment does not silently pass as TA16-V1 evidence.

## 9. Workflow Permissions

Ordinary PR/push verification declares:

```yaml
permissions:
  contents: read
```

No source/ref/release/issue/package write permission is needed for correctness CI.

Untrusted pull-request code receives no repository secret by default.

## 10. Trigger / Concurrency Policy

`ci-pr.yml` runs on:

- pull requests targeting `main`;
- pushes to `main`;
- manual dispatch for diagnostics.

Pull-request workflows use per-PR/ref concurrency with `cancel-in-progress: true` so obsolete commits do not waste resources. A canceled run is never evidence for the new SHA.

Main/release/reference certification retains exact-SHA evidence according to TA-14.

## 11. Zero-Test Protection

Every test-bearing required job executes `cmake/RequireTests.cmake` for its required label before CTest. Unexpected zero tests is an explicit failure.

No filter typo, missing test executable, or discovery failure can produce a green semantic gate.

## 12. Cache Policy

Caches are optional accelerators. `actions/cache` is used only after the cached path/key is understood and always with the locked SHA.

Cache keys include relevant:

- OS/toolchain;
- vcpkg baseline/manifest hash;
- target/profile dimensions.

A clean cache miss must produce the same build/test result.

## 13. Artifact Policy

Failure/evidence artifacts use the locked `upload-artifact` SHA only when useful data exists. Retention follows TA-14 evidence classes.

CI never auto-updates format output, goldens, baselines, generated source, content, or dependencies.

## 14. Branch-Protection Target State

The repository's target `main` governance after checks have appeared is:

- pull request required for implementation changes;
- required conversation/review policy according to repository collaboration state;
- required status checks for currently activated stable gates;
- `StarForge / CI Gate` required;
- branch must contain evidence for current head SHA;
- force-push/deletion disabled for normal contributors;
- administrators intentionally bypass only for repository recovery and then restore a green `main`;
- no required check is added before its real job has executed successfully at least once.

At TA-16 audit time the GitHub rulesets API returns no repository ruleset. The exact target-state contract is therefore locked here; activation is an administrative repository-setting operation once the first real check names have materialized. Lack of an admin-capable connector must not be hidden by inventing a ruleset result.

## 15. Required-Check Evolution

When a new real suite enters implementation:

1. implement its target/tests;
2. add zero-test guard;
3. add/enable its workflow job;
4. run it on the integration commit;
5. inspect evidence;
6. then add its stable check name to `main` ruleset requirements.

Removing a required gate requires the inverse reviewed governance change and must explain who assumes its coverage.

## 16. Nightly and Performance

Nightly expands worker-count/frame-rate/migration/corruption/leak matrices when those real suites exist.

`StarForge / Performance Regression` runs only on the TA-16 controlled reference runner. Generic hosted VMs can run coarse smoke but cannot certify TA-13 numeric GPU/CPU thresholds.

## 17. Initial CI Gate Pseudocode

```text
Build & Unit success
        ↓
CI Gate checks dependency result == success
        ↓
PASS

Build & Unit failure/cancel/skip
        ↓
CI Gate FAIL
```

As more required gates activate, all become dependencies of CI Gate.

## 18. Lock Verdict

```text
Workflow namespace: LOCKED
Initial executable workflow: ci-pr.yml
Initial real checks: Build & Unit + CI Gate
Windows certification runner: windows-2022 + exact VS 2022 17.14.39 assertion
CMake certification asset/digest: LOCKED
Exact PR-head checkout: REQUIRED
Future stable check names: RESERVED
Action versions/SHAs: LOCKED
Permissions: LEAST PRIVILEGE
Zero-test guard: REQUIRED
Placeholder-green jobs: PROHIBITED
Main ruleset target state: LOCKED; admin activation follows first successful check materialization
```
