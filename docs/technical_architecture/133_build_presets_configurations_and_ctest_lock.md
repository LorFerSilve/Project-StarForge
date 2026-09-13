# TA-16 — Build Presets, Configurations, and CTest Lock

> **Status:** Implementation Locked  
> **Baseline:** `TA16-V1`  
> **Authority:** Stable configure/build/test entry points used by developers and CI

## 1. Build Entry-Point Rule

Supported builds are driven through repository-owned CMake presets. Ad-hoc IDE settings are not independent build authority.

The baseline configurations remain:

```text
Debug
RelWithDebInfo
Release
```

Sanitizer/static-analysis variants are explicit presets/toolchain options over real targets.

## 2. Initial Presets

The first checked-in presets are:

```text
configure:
  windows-msvc-debug
  windows-msvc-relwithdebinfo
  linux-clang-debug

build:
  windows-msvc-debug
  windows-msvc-relwithdebinfo
  linux-clang-debug

test:
  windows-unit
  linux-unit
```

Future implementation adds stable presets without changing the meaning of these names.

## 3. Windows Generator

Windows uses the Visual Studio 17 2022 generator with x64 architecture and the locked VS 17.14 toolset family. Multi-config build presets select Debug/RelWithDebInfo explicitly.

## 4. Linux Generator

Linux portability/headless verification uses Unix Makefiles with Clang 23.1.1 for the baseline. This avoids adding an independent Ninja-version dependency to the initial lock.

## 5. vcpkg Integration

Presets consume:

```text
$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
```

`VCPKG_ROOT` must point to the repository/baseline checkout required by the current implementation baseline.

The bootstrap presets request the `tests` manifest feature. Runtime/content/dev-tool features are enabled only by presets/phases that build targets requiring them.

## 6. Project Build Options

Canonical CMake options are:

```text
STARFORGE_BUILD_TESTS=ON/OFF
STARFORGE_BUILD_DEVTOOLS=ON/OFF
STARFORGE_WARNINGS_AS_ERRORS=ON/OFF
STARFORGE_ENABLE_ASAN=ON/OFF
STARFORGE_ENABLE_UBSAN=ON/OFF
STARFORGE_ENABLE_TSAN=ON/OFF
```

Unsupported compiler/platform combinations fail configure rather than silently ignoring a requested sanitizer.

## 7. Language and Warning Baseline

Every project-owned C++ target uses:

```text
CXX_STANDARD 23
CXX_STANDARD_REQUIRED ON
CXX_EXTENSIONS OFF
```

Project warnings are strict and warnings-as-errors is enabled in CI for project-owned source. Third-party warnings are not promoted into project errors.

## 8. CTest Labels

TA-14's stable labels are now implementation locked:

```text
unit
domain
headless
scenario
backend
content
persistence
determinism
fault
slow
performance
```

Additional labels may narrow a suite but may not replace these semantic labels.

## 9. Locked Test Targets

Test executables map to labels as follows:

| Target | Required primary labels |
|---|---|
| `starforge_tests_unit` | `unit` |
| `starforge_tests_domain` | `domain` |
| `starforge_tests_headless` | `headless` |
| `starforge_tests_scenarios` | `scenario;headless;determinism` |
| `starforge_tests_backend` | `backend` |
| `starforge_tests_content` | `content` |
| `starforge_tests_persistence` | `persistence` |
| `starforge_tests_performance` | `performance;slow` |

Individual tests can carry multiple labels.

## 10. Test Naming

CTest/Catch2 semantic names follow:

```text
<Domain>.<Feature>.<Behavior>
```

Names are stable behavioral contracts, not raw method names.

## 11. Zero-Test Protection

Every required CI test gate must prove discovery count > 0 before treating its CTest invocation as passing.

The repository owns `cmake/RequireTests.cmake`, which runs CTest discovery for a build directory/label and fails when the expected suite has zero discovered tests.

This guard is executed before the required suite.

## 12. Fixtures

When persistent/content/scenario fixtures are introduced, canonical fixture roots are:

```text
tests/fixtures/content/
tests/fixtures/persistence/
tests/fixtures/scenarios/
tests/fixtures/world/
tests/fixtures/physics/
tests/fixtures/navigation/
```

Golden/fixture provenance metadata is committed beside the fixture. Writable tests copy fixtures to test-owned temporary directories.

## 13. Fast Local Loop

The baseline Windows fast loop is:

```text
cmake --preset windows-msvc-debug
cmake --build --preset windows-msvc-debug
cmake -DSTARFORGE_TEST_BUILD_DIR=out/build/windows-msvc-debug -DSTARFORGE_TEST_LABEL=unit -P cmake/RequireTests.cmake
ctest --preset windows-unit
```

Equivalent Linux commands use the `linux-clang-debug` / `linux-unit` presets.

## 14. Configuration Semantics

Optimization/debug configuration never changes gameplay/content semantics. A distinct content profile is explicit content input; compiler optimization mode is not a hidden content fingerprint input unless a future owning specification deliberately adds it.

## 15. Sanitizer Policy

ASan/UBSan builds use the same tests/fixtures and cannot redefine expected behavior. TSan remains scheduled/release certification where supported because of higher cost/platform constraints.

A sanitizer-disabled unsupported platform cannot masquerade as a passed sanitizer job.

## 16. Build Output

All generated build state is under:

```text
out/build/<preset>/
out/test/<preset>/      (when explicit reports are emitted)
out/artifacts/          (local generated evidence only)
```

`out/` is ignored by Git and is never source/content authority.

## 17. Preset Change Control

Changing an existing stable preset's compiler family, architecture, semantic features, or meaning requires review. Adding a new optimization/diagnostic convenience preset is allowed when it does not silently replace certification coverage.

## 18. Lock Verdict

```text
Configurations: LOCKED
Initial presets: LOCKED
C++23 behavior: LOCKED
CTest semantic labels: LOCKED
Test target names: LOCKED
Zero-test protection: REQUIRED
Fast local loop: LOCKED
Build output root: LOCKED
```
