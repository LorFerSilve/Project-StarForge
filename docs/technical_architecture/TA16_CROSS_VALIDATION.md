# TA-16 — Cross-Validation and Implementation-Readiness Audit

> **Static verdict:** PASS  
> **Dynamic closure condition:** the exact final PR head must pass `StarForge / Build & Unit` and `StarForge / CI Gate`  
> **Baseline:** `TA16-V1`  
> **Authority:** Final cross-validation required by `130_ta16_scope_lock_authority_and_exit_criteria.md`

## 1. Purpose

This audit verifies that TA-16 is not merely a documentation handoff. It cross-checks the locked implementation contracts against the repository artifacts that actually materialize IMP-0 and records any blocker that must be resolved before implementation can begin.

The audit is intentionally split into:

1. **static contract validation** — repository/document consistency that can be proven from the candidate tree; and
2. **dynamic candidate validation** — configure/build/test/headless execution on the exact candidate SHA through the locked GitHub Actions checks.

A static PASS does not override a red dynamic gate.

## 2. Candidate Scope

TA-16 cross-validates these authoritative artifacts:

```text
130_ta16_scope_lock_authority_and_exit_criteria.md
131_toolchain_dependency_and_license_lock.md
132_cmake_target_dependency_and_source_layout_lock.md
133_build_presets_configurations_and_ctest_lock.md
134_ci_workflows_actions_and_required_check_lock.md
135_reference_hardware_and_performance_runner_lock.md
136_implementation_roadmap_and_vertical_slice.md
137_implementation_lock_matrix_and_change_control.md
138_branch_merge_milestone_and_release_strategy.md
139_ta16_final_implementation_handoff.md
TA16_ARCHITECTURE_DECISIONS.md
ARCHITECTURE_DECISION_REGISTRY.md
TA_ROADMAP.md
```

and these materialized bootstrap artifacts:

```text
CMakeLists.txt
CMakePresets.json
vcpkg.json
cmake/StarForgeProjectOptions.cmake
cmake/StarForgeWarnings.cmake
cmake/RequireTests.cmake
src/core/
include/starforge/core/
apps/headless/
tests/unit/
.github/workflows/ci-pr.yml
third_party/generated/glad/README.md
```

## 3. Blocker Found During Cross-Validation

The first TA-16 bootstrap workflow on `main` failed before configure/build because its runner/toolchain assumptions were contradictory:

```text
locked compiler distribution: Visual Studio 2022 17.14.40
workflow runner label:       windows-2025
actual hosted image:         Visual Studio 2026 (18.x)
```

This was a real TA-16 blocker because the implementation handoff requires current-head bootstrap evidence and explicitly prohibits silently accepting a different compiler distribution.

### Resolution

The baseline and executable workflow were made mutually certifiable without weakening version checks:

```text
primary hosted certification image: windows-2022
Visual Studio:                      2022 17.14.39
installation build:                17.14.37614.0
MSVC family:                       v143 14.44 x64
CMake:                             4.3.3
vcpkg baseline:                    a1cae005c39be7b18ba319fced856b68d7276271
```

The workflow now:

- selects `windows-2022` explicitly;
- requires a VS 2022 17.14 installation;
- asserts exact installation build `17.14.37614.0`;
- asserts an installed `14.44.*` MSVC x64 toolset;
- records the concrete `cl.exe` file version;
- continues to fail on unexpected toolchain drift.

This is a pre-closure correction of `TA16-V1`, not a post-lock C2 architecture migration.

## 4. Sixteen-Item TA-16 Exit Matrix

| # | Required closure category | Cross-validation result | Evidence owner |
|---:|---|:---:|---|
| 1 | exact compiler/toolset | PASS | `131`, `134`, `ci-pr.yml` |
| 2 | vcpkg/dependency snapshot | PASS | `131`, `vcpkg.json`, CI exact SHA checkout |
| 3 | concrete target registry / DAG | PASS | `132`, root/module CMake |
| 4 | repository-owned presets | PASS | `133`, `CMakePresets.json` |
| 5 | CTest labels / target conventions | PASS | `133`, `tests/unit/CMakeLists.txt` |
| 6 | executable CI over real targets | PASS (static) / dynamic gate required | `134`, `ci-pr.yml` |
| 7 | immutable Action pins / least privilege | PASS | `134`, `ci-pr.yml` |
| 8 | required-check / ruleset target state | PASS | `134`, `138`; activation remains admin setting |
| 9 | concrete performance reference | PASS | `135`, AD-138 |
| 10 | source/scaffolding layout | PASS | `132`, candidate tree |
| 11 | dependency-ordered implementation phases | PASS | `136` |
| 12 | first vertical slice and acceptance | PASS | `136` Horizon Test Cell V0 |
| 13 | per-contract Implementation Locked state | PASS | `137` |
| 14 | branch / merge strategy | PASS | `138`, AD-141 |
| 15 | milestone exits | PASS | `138` M0–M9 |
| 16 | AD consolidation / continuity | PASS | AD-001…AD-141 registry |

Static result: **16/16 PASS**.

## 5. Bootstrap Target Validation

The candidate tree materializes exactly the promised non-gameplay bootstrap target set:

```text
sf_core
starforge-headless
starforge_tests_unit
```

Validation findings:

- `sf_core` is a real static library and exposes only its owned public include root;
- `starforge-headless` is a real executable linked to `StarForge::Core`;
- `starforge_tests_unit` is a real Catch2 executable linked to the same core contract;
- no renderer/physics/station/AI/mission placeholder target is created early;
- the root CMake composition remains subordinate to module-local ownership.

Result: **PASS**.

## 6. Unit Discovery and Zero-Test Protection

`starforge_tests_unit` uses Catch2 discovery and labels discovered tests `unit`.

`cmake/RequireTests.cmake` independently runs CTest discovery for the requested label, parses `Total Tests`, and fails when the count is absent or less than one. The required CI job executes this guard before `ctest --preset windows-unit`.

The current bootstrap source contains real behavioral/build-contract tests for:

- `TA16-V1` baseline identity;
- authoritative 60 Hz simulation contract;
- required C++23 language baseline.

Result: **PASS**. A missing/discovery-broken test suite cannot become green through an empty CTest selection.

## 7. Preset / CI Agreement

Cross-check:

```text
CI configure  -> windows-msvc-debug
CI build      -> windows-msvc-debug
zero-test dir -> out/build/windows-msvc-debug, Debug, label unit
CI test       -> windows-unit
headless path -> Debug/starforge-headless.exe
```

These paths/configurations agree with the multi-config Visual Studio preset model. The bootstrap enables only the vcpkg `tests` feature because the materialized targets do not yet require runtime/content/dev-tool dependencies.

Result: **PASS**.

## 8. glad2 Boundary

`TA16-V1` explicitly rejects the vcpkg glad1 package as a replacement for glad2. The repository reserves `third_party/generated/glad/` as reviewed generated-source authority; the actual loader is not required until the OpenGL adapter enters implementation.

No current IMP-0 target claims or links an OpenGL loader.

Result: **PASS** — contract locked without fake early renderer materialization.

## 9. Roadmap / Vertical Slice Cross-Check

The implementation sequence is dependency-first:

```text
IMP-0 bootstrap
  -> IMP-1 core / identity / deterministic simulation / transactions
  -> IMP-2 content + persistence primitives
  -> IMP-3 platform + renderer foundation
  -> IMP-4 world + physics + character
  -> IMP-5 Horizon Test Cell V0
  -> broader systemic gameplay phases
```

Horizon Test Cell V0 forces the critical ownership/persistence/runtime reconstruction path to work before broad feature expansion. Its acceptance criteria explicitly cover PersistentId stability, runtime-handle replacement, ownership conservation, save/load, context unload/reload, authoritative headless agreement and backend reconstruction.

Result: **PASS**.

## 10. Change-Control Cross-Check

The locked architecture distinguishes:

- **C0** — mechanical/non-contractual;
- **C1** — contract-compatible interface evolution;
- **C2** — architecture baseline change;
- **C3** — GDS semantic change.

Compiler/CMake/vcpkg baseline changes are C2 after TA-16 closure. Gameplay semantic changes require GDS authority first. This prevents implementation convenience from silently rewriting architecture or design.

Result: **PASS**.

## 11. Decision Continuity

Canonical numbering is contiguous:

```text
AD-001 ... AD-100  central architecture log
AD-101 ... AD-111  TA-12
AD-112 ... AD-121  TA-13
AD-122 ... AD-132  TA-14
AD-133 ... AD-141  TA-16
next available:    AD-142
```

No gap/reuse was introduced by the toolchain correction; AD-133 remains the owning baseline decision and was corrected before final TA-16 promotion.

Result: **PASS**.

## 12. Repository Governance Residual

The desired `main` ruleset is contractually locked, but repository administration currently remains outside the available connector's write scope. Therefore this audit does **not** claim that branch protection has been activated.

Required administrative follow-up after the first successful materialization of the stable checks and before normal IMP-1 integration:

```text
require pull requests for main
require StarForge / CI Gate
require currently activated upstream stable checks as appropriate
disable normal force-push/deletion
retain deliberate administrator recovery bypass only
```

This is an operational repository-setting action, not an unresolved software-architecture contract. It must not be represented as already configured.

## 13. Dynamic Closure Gate

TA-16 receives its final PASS only when the exact final candidate head produced by this cross-validation change has:

```text
StarForge / Build & Unit = success
StarForge / CI Gate       = success
```

The Build & Unit job must reach and pass all of:

1. exact SHA checkout;
2. exact VS/MSVC certification;
3. exact CMake certification;
4. exact vcpkg checkout/bootstrap;
5. configure;
6. build;
7. nonzero unit discovery;
8. unit tests;
9. headless bootstrap execution.

A prior SHA's green run does not satisfy this condition.

## 14. Final Verdict

```text
Static TA-16 categories:        16/16 PASS
Static architecture blockers:   0
Known CI contract blocker:      RESOLVED in candidate
Placeholder-green gates:        0
Bootstrap targets:              REAL
Cross-validation artifact:      PRESENT
Dynamic exact-head gate:        REQUIRED FOR FINAL PROMOTION
Administrative main ruleset:    TARGET STATE LOCKED; ACTIVATION NOT CLAIMED
Next dependency after promotion: IMP-1
```

**Promotion rule:** merge/promote TA-16 only with the exact final PR head green in both initial stable checks. Once that condition is satisfied, TA-16 is formally complete and `IMP-1 — Core, Identity, Deterministic Simulation, and Transactions` is the next authorized implementation dependency.
