# TA-14 — Test Architecture, Layers, Targets, and Conventions

> **Status:** Architecture Complete  
> **Authority:** Test taxonomy, CMake/CTest/Catch2 target structure, naming, isolation, failure policy, deterministic test conventions, fixtures, and implementation-facing test boundaries

## 1. Purpose

StarForge requires a verification architecture that catches semantic regressions before they become renderer-, timing-, platform-, or content-dependent failures.

TA-14 therefore defines one project-wide test taxonomy and one implementation-facing contract for how tests are built, discovered, named, isolated, repeated, reported, and promoted into CI gates.

## 2. Core Principle

A test must execute at the lowest layer capable of proving the behavior.

Gameplay/domain behavior that does not require GLFW, OpenGL, an audio device, or a real filesystem must not depend on those systems merely because the shipping application does.

## 3. Test Layers

Canonical layers are:

1. **Unit** — pure algorithms/value types/small components;
2. **Domain Contract** — domain stores, commands, transactions, ownership, schedulers, migration functions;
3. **Headless Integration** — multiple gameplay domains under the test composition root;
4. **Deterministic Scenario** — scripted fixed-seed multi-system simulations with checkpoint equivalence;
5. **Backend Smoke** — Jolt, OpenGL, GLFW/platform, miniaudio, FreeType/HarfBuzz and Recast/Detour adapter integration;
6. **Golden / Compatibility** — binary saves, content outputs, RNG sequences, schemas and migrations;
7. **Fault / Corruption** — injected failures and invalid data;
8. **Performance / Budget** — TA-13 benchmark and regression checks.

A higher layer supplements rather than replaces lower-layer coverage.

## 4. CMake / CTest Ownership

The implementation uses CMake targets and CTest as the top-level test runner.

Catch2 is the baseline C++ test framework.

The target architecture reserves logical groups such as:

- `starforge_tests_unit`;
- `starforge_tests_domain`;
- `starforge_tests_headless`;
- `starforge_tests_backend`;
- `starforge_tests_persistence`;
- `starforge_tests_content`;
- `starforge_tests_scenarios`.

Exact physical target splitting can be finalized by TA-16 as long as CTest labels preserve these semantic layers.

## 5. CTest Labels

Tests carry stable labels, at minimum:

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

A test can carry multiple labels.

CI workflows select suites through labels rather than hand-maintained executable lists.

## 6. Naming

Canonical test names use:

```text
<Domain>.<Feature>.<Behavior>
```

Example:

```text
Inventory.Transfer.UniqueItemMovesExactlyOnce
Mission.Objective.ExtractionRequiresCommittedSecurity
Persistence.Load.CorruptRequiredSectionRejectsWholeSession
```

Names describe behavior, not implementation method names.

## 7. Arrange / Act / Assert

Tests keep setup, operation, and assertions separable where practical.

A test should expose the semantic invariant being proved rather than reproducing production implementation internally.

## 8. Test Independence

Every test is isolated from prior tests.

A test may not depend on:

- execution order;
- another test leaving a file behind;
- global RNG state;
- wall-clock date/time;
- network access;
- current user profile;
- developer machine paths;
- unspecified thread completion order.

Temporary storage uses test-owned directories with deterministic fixture setup.

## 9. No Hidden Retries

A failing deterministic test fails.

CI does not automatically retry tests until they pass.

A workflow-level retry may exist only for explicitly classified infrastructure failures, never to mask a semantic/flaky test.

## 10. Flaky Test Policy

A reproducibly nondeterministic test is a product defect or test defect and blocks the relevant gate until fixed.

There is no permanent `allow_failure` category for semantic correctness tests.

Temporary quarantine requires:

- linked issue;
- owner;
- reason;
- expiry/review date;
- exclusion from release/Implementation Locked certification where the test protects a locked contract.

## 11. Assertions

Tests prefer semantic assertions over opaque full-structure equality where only a subset is relevant.

For deterministic scenario checkpoints, canonical state hashes can supplement explicit semantic assertions.

Diagnostic hashes are never gameplay/save/security authority.

## 12. Floating-Point Assertions

Floating-point comparisons use domain-appropriate absolute/relative tolerances.

Tests must not hide genuine simulation divergence with excessively broad epsilon values.

Where exact binary equality is required—container bytes, integer ownership, IDs, RNG sequences—the test uses exact equality.

## 13. Time

Authoritative tests use `SimulationTick` / canonical durations.

Wall time is permitted only for performance/infrastructure timing tests and is never used to decide expected gameplay state.

## 14. Randomness

Every test using randomness declares its seed/stream explicitly.

On failure the test report includes:

- seed;
- stream kind;
- scenario/configuration version;
- failing checkpoint/tick where applicable.

No test uses nondeterministic `random_device` as semantic input.

## 15. Property / Generated Tests

Deterministic generated-case tests are encouraged for:

- ownership conservation;
- graph/topology operations;
- serialization round trips;
- migration transforms;
- inventory capacity;
- transaction validation;
- path/traversal constraints.

Generated tests use bounded reproducible seeds and report the minimized/replayable failing input.

No third-party property-testing dependency is required by architecture; Catch2 plus project-owned generators is sufficient unless TA-16 approves another tool.

## 16. Fixtures

Fixtures are immutable source test data where possible.

Writable copies are created per test.

Fixture categories include:

- authored content definitions;
- cooked-content mini registries;
- current/historical save files;
- corrupt save variants;
- deterministic world/mission seeds;
- representative station topology;
- navigation/collision micro-scenes;
- shader/material test assets.

## 17. Fixture Provenance

Binary/golden fixtures record:

- generating tool/version;
- schema/container version;
- source fixture identity;
- intended invariant;
- update procedure.

A developer cannot replace a golden file merely because the current implementation differs.

## 18. Golden Update Rule

Golden updates require intentional review.

The change must explain whether it represents:

- expected compatible evolution;
- version/migration addition;
- approved architecture/content change.

CI never auto-updates goldens.

## 19. Parallel Test Execution

Tests may execute in parallel only when isolated.

Shared fixed ports/global files/singleton backend resources must either be removed, namespaced, or labeled serial.

Parallel execution order cannot affect semantic results.

## 20. Test-Only Hooks

Production systems can expose explicit test adapters/diagnostic interfaces where necessary, but test code cannot bypass production validation/commit rules to manufacture a passing result.

Development/debug commands that issue real typed commands are preferred over direct private-store mutation for integration tests.

## 21. Coverage Policy

Line/branch coverage is diagnostic, not the primary quality measure.

TA-14 does not define a universal percentage gate because high coverage can still miss transaction, determinism, persistence, and cross-domain invariants.

Coverage reports are retained and used to identify untested risk areas.

## 22. Required Contract Mapping

Every Architecture Complete subsystem must identify at least one test family for:

- ordinary success;
- validation rejection;
- boundary/limit behavior;
- save/load where persistent;
- deterministic ordering where relevant;
- failure/recovery path where defined.

TA-15 audits the mapping.

## 23. Headless Default

Gameplay tests default to the headless composition root.

Backend smoke tests are the exception, not the default.

## 24. Fast Local Loop

A developer must be able to run a fast correctness loop without launching the game.

The intended local commands are conceptually:

```text
cmake --build <build> --target <tests>
ctest --preset <fast-preset>
```

Exact presets are TA-16 implementation data.

## 25. Slow Suites

Long deterministic scenarios, migrations, corruption matrices and benchmark suites are labeled `slow`/`performance` and may run in dedicated CI jobs.

They are still first-class tests and cannot become permanently optional for release certification.

## 26. Failure Output

A failed test reports enough replay context to reproduce it locally, including applicable:

- test/scenario name;
- build/configuration;
- seed;
- SimulationTick;
- stable IDs/ContentIds;
- worker count;
- relevant revisions/generations;
- fixture version;
- assertion/invariant failure.

## 27. Explicit Non-Goals

TA-14 does not require:

- tests through the shipping UI for every rule;
- network-based test services;
- screenshot-only approval as gameplay correctness;
- randomized unreproducible fuzz seeds without capture;
- code coverage as a substitute for architecture invariant tests.

## 28. Dependencies

Depends on TA-0 through TA-13, CMake/Catch2 baseline, headless runtime composition, deterministic RNG/state, project-owned diagnostics and later TA-16 target/preset locking.

## 29. Open Questions

None within test taxonomy/convention scope.