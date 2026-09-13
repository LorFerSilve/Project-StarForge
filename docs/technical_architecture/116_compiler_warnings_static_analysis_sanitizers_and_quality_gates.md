# TA-14 — Compiler Warnings, Static Analysis, Sanitizers, and Quality Gates

> **Status:** Architecture Complete  
> **Authority:** Warning policy, formatting/static-analysis gates, sanitizer matrices, dependency/build hygiene, undefined-behavior detection, and source-quality CI boundaries

## 1. Purpose

Architecture correctness must be supported by mechanical source-quality gates before runtime tests execute.

TA-14 therefore defines a layered compiler/static-analysis/sanitizer policy that is strict enough to catch lifetime, UB, threading and API-boundary mistakes without pretending lint alone proves gameplay correctness.

## 2. Compiler Baseline

TA-16 pins exact supported compiler/toolchain versions.

TA-14 requires at least:

- primary Windows/MSVC build;
- Clang-based analysis/sanitizer build on a supported CI platform;
- C++23 conformance for project code.

Additional compilers may be added when supported by the implementation roadmap.

## 3. Warnings-as-Errors

Project-owned source compiles with a high warning level and warnings-as-errors in CI.

Third-party/vendor code is isolated so external warnings do not force project-wide warning suppression.

## 4. Warning Suppression

A warning suppression must be:

- narrow;
- documented where non-obvious;
- attached to the smallest practical scope;
- never used to silence a real correctness warning globally.

## 5. Required Warning Themes

Toolchain policy must detect where supported:

- implicit narrowing/sign conversion risks;
- uninitialized use;
- unreachable/suspicious control flow;
- missing virtual/destructor issues;
- shadowing where harmful;
- switch/enum omissions;
- lifetime/reference misuse warnings;
- deprecated/unsafe APIs;
- format mismatches.

Exact flags are TA-16 compiler-specific data.

## 6. Formatting

`clang-format` (or the project-pinned formatter if TA-16 changes it) is a deterministic gate for project C/C++ source.

CI checks formatting; it does not automatically commit formatting changes.

Markdown/content formatting is governed separately and must not block on stylistic whitespace that is semantically intentional.

## 7. clang-tidy / Static Analysis

A pinned Clang static-analysis configuration covers project-owned code.

Priority categories include:

- bugprone;
- performance;
- modernize where architecture-compatible;
- concurrency/lifetime;
- readability rules that prevent ambiguity;
- project-specific forbidden patterns where feasible.

## 8. Architecture-Specific Static Checks

Custom grep/lint or clang-tidy checks may enforce boundaries such as:

- OpenGL calls outside render module forbidden;
- raw Jolt backend types outside adapter forbidden;
- miniaudio backend access outside audio layer forbidden;
- runtime handles in persistence DTO headers forbidden;
- `std::random_device` / untracked global RNG in gameplay code forbidden;
- direct filesystem/raw-source loading from shipping gameplay modules forbidden;
- Dear ImGui in shipping UI module forbidden.

These checks supplement, not replace, compilation/tests.

## 9. Include / Dependency Hygiene

CI can validate module include/dependency direction to prevent forbidden cycles and backend leakage.

TA-15 audits final dependency graph; TA-14 provides mechanical evidence.

## 10. Undefined Behavior Sanitizer

A Clang/GCC-supported CI configuration runs UndefinedBehaviorSanitizer over headless/domain/integration suites.

UBSan failures block the gate.

## 11. Address Sanitizer

AddressSanitizer runs representative unit/headless/backend-compatible suites to detect:

- use-after-free;
- out-of-bounds;
- double free;
- lifetime errors;
- many leak patterns where supported.

ASan failure blocks the gate.

## 12. Leak Detection

LeakSanitizer or platform-equivalent leak checking runs where stable/supported.

Known third-party process-global allocations must be documented and isolated; project-owned growing leaks are not waived permanently.

## 13. Thread Sanitizer

ThreadSanitizer is a scheduled/manual architecture gate where supported by compiler/platform dependencies.

It targets:

- worker pool queues;
- immutable-result handoff;
- content/persistence background jobs;
- profiler/log queues;
- test executor parallelism.

Because backend libraries/platform support can limit TSan practicality, it need not run on every PR, but unresolved project-owned races block release/Implementation Locked certification.

## 14. Sanitizer Scope

Sanitizer suites prioritize headless/project-owned code and adapters that can execute reliably under instrumentation.

Performance thresholds are disabled under sanitizers; correctness remains enabled.

## 15. Debug STL / Iterator Checks

Where toolchains provide useful checked iterators/runtime assertions, dedicated development configurations may enable them for local/CI diagnostic jobs.

They are not performance reference configurations.

## 16. Integer / Bounds Safety

Serialization/content parsers explicitly test integer overflow, size conversion and allocation-bound checks.

Static analysis plus corruption tests together protect these boundaries.

## 17. Floating-Point Fast-Math

Authoritative simulation code does not enable unsafe fast-math modes that can silently violate required deterministic/IEEE assumptions unless a later explicit decision validates the consequences.

Renderer-only shader/compiler optimizations remain TA-4/toolchain scope.

## 18. RTTI / Exceptions

TA-14 does not change TA-1 policy that C++ exceptions are not ordinary gameplay control flow.

If exceptions remain enabled for third-party/tooling needs, tests ensure project domain logic uses typed Result/error paths as designed.

## 19. Dependency Vulnerability / Supply-Chain Checks

CI records the pinned vcpkg manifest/baseline and dependency versions.

Where available, automated dependency/security advisories may be surfaced as a separate engineering signal.

A vulnerability gate must distinguish:

- actually shipped/runtime-reachable dependency;
- build-only tool;
- false/irrelevant advisory.

TA-14 does not silently auto-upgrade dependencies because deterministic output and architecture compatibility require reviewed version changes.

## 20. Generated Code / Cooked Outputs

Generated files are either reproducibly regenerated/validated or intentionally version-controlled according to TA-10/TA-16.

Static-analysis/formatting gates avoid rewriting generated third-party/cooked data as project source.

## 21. Build Reproducibility Signal

CI records:

- compiler version;
- CMake version;
- vcpkg baseline;
- dependency versions;
- relevant build options.

A failure report therefore identifies the actual toolchain used.

## 22. Multiple Configurations

At minimum architecture anticipates:

- Debug/development correctness build;
- optimized Release/Profile build;
- sanitizer build(s).

TA-16 names exact CMake presets/configurations.

## 23. No Warning Baseline Debt

Implementation starts from a clean warning baseline.

New warnings are not accepted as “existing debt” during scaffolding.

## 24. Static-Analysis Baseline Debt

If a third-party/tool limitation produces unavoidable existing findings, any baseline suppression list must:

- exclude project-owned true positives;
- identify rule/path/reason;
- be reviewable;
- not grow silently.

## 25. Source Quality PR Gate

The ordinary source-quality gate conceptually contains:

```text
configure
-> build with warnings-as-errors
-> format check
-> static analysis selected scope
-> fast unit/domain tests
```

Sanitizers can run as parallel jobs.

## 26. Sanitizer Failure Artifacts

CI retains sanitizer logs with symbolized stack traces where possible.

Secrets/user-private data are not included.

## 27. Fatal vs Advisory

Blocking by default:

- compile error;
- project warning;
- format mismatch;
- selected high-confidence static-analysis failure;
- ASan/UBSan finding;
- reproducible project-owned race from TSan certification.

Informational/advisory signals are explicitly labeled and never masquerade as passing required gates.

## 28. Explicit Non-Goals

TA-14 does not require:

- every static-analysis check available;
- sanitizer performance certification;
- automatic dependency upgrades;
- suppressing legitimate warnings to keep CI green;
- treating lint success as gameplay proof.

## 29. Dependencies

Depends on C++23/CMake/vcpkg baseline, TA-0 module boundaries, TA-1 ownership/threading, TA-10 toolchain determinism, TA-13 profiling configurations, and TA-16 final compiler/version presets.

## 30. Open Questions

None within source-quality/sanitizer architecture.