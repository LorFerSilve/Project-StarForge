# TA-16 — Scope, Lock Authority, and Exit Criteria

> **Status:** Implementation Locked  
> **Authority:** Final Technical Architecture handoff contract before dependency-ordered implementation

## 1. Purpose

TA-16 converts the Architecture Complete result of TA-15 into an executable implementation contract. It does not redesign gameplay and does not reopen accepted architecture by convenience. It fixes the concrete repository/toolchain/target/test/CI/implementation boundaries required so source development can proceed without inventing structure ad hoc.

## 2. Entry Evidence

TA-16 begins from the accepted TA-15 verdict:

- GDS Design Complete;
- TA-0 through TA-14 integrated;
- TA-15 cross-validation 260/260 PASS;
- architecture blockers 0;
- required corrections 0;
- implementation-critical open architecture questions 0;
- build-level and semantic ownership DAGs realizable;
- every implementation area has an owner and verification path.

## 3. TA-16 Authority

TA-16 may lock implementation details that were deliberately deferred by Architecture Complete specifications, including:

- exact toolchain/dependency snapshots;
- concrete CMake targets and source layout;
- presets/configurations;
- CTest labels and suite targets;
- executable CI entry points;
- controlled reference hardware identity;
- implementation ordering;
- vertical-slice acceptance;
- branch/merge/milestone policy;
- Implementation Locked change control.

TA-16 may not silently change GDS gameplay semantics, persistent ownership, transaction rules, Simulation Time, active/off-screen behavior, save semantics, content identity, presentation knowledge boundaries, or TA-13 semantic-preservation rules.

## 4. Lock States

The implementation handoff uses four states:

1. **Architecture Complete** — technical behavior is defined and cross-validated;
2. **Implementation Locked** — exact implementation-facing contract is frozen for the baseline;
3. **Implemented** — code satisfying the locked contract exists and passes its required verification;
4. **Certified** — required milestone/release evidence exists for an exact commit SHA.

Implementation Locked does not mean code already exists. It means implementation no longer has discretion to reinterpret the contract.

## 5. Baseline Lock Identifier

The first implementation baseline is:

```text
STARFORGE_IMPLEMENTATION_BASELINE = TA16-V1
```

This identifier is documentation/build metadata. It is not save identity, ContentId, gameplay identity, or a compatibility substitute for explicit schema/container versions.

## 6. Required TA-16 Outputs

TA-16 closes all sixteen TA-15 lock categories:

1. compiler/toolset versions;
2. vcpkg baseline and direct dependency resolution;
3. exact CMake target names and dependency direction;
4. CMake presets/configurations;
5. CTest labels/fixture conventions;
6. executable GitHub Actions workflow(s) backed by real targets;
7. immutable GitHub Actions pins and least-privilege permissions;
8. stable required-check contract and ruleset target state;
9. concrete reference hardware/runner identity;
10. source/scaffolding layout;
11. dependency-ordered implementation phases;
12. first vertical slice and measurable acceptance criteria;
13. per-contract Implementation Locked sequence;
14. branch/merge strategy;
15. milestone/exit criteria;
16. architecture-decision consolidation/registry continuity.

## 7. Bootstrap Materialization Rule

AD-132 prohibits placeholder-green CI. Therefore TA-16 is allowed and required to materialize the smallest non-gameplay bootstrap needed to prove the locked build contract:

- root CMake project;
- vcpkg manifest/baseline;
- CMake presets;
- project warning/options modules;
- one compiled `sf_core` bootstrap contract library;
- one real headless bootstrap executable;
- Catch2 unit tests exercising the compiled contract;
- zero-test-discovery guard;
- an executable CI workflow that builds and runs those tests.

This bootstrap is implementation infrastructure, not gameplay implementation. It may not add player movement, rendering, physics, station simulation, missions, AI, content behavior, or persistence behavior.

## 8. No Fake Completeness

TA-16 must not create empty libraries, disabled jobs, unconditional success steps, or tests that merely `return true` and then claim subsystem coverage.

Only checks backed by implemented behavior become required. Future TA-14 aggregate gates are activated when their owning real targets/suites exist.

## 9. Source-Tree Authority

The locked source layout distinguishes:

- `include/starforge/` — public project-owned C++ interfaces/value types;
- `src/` — implementation targets;
- `apps/` — composition-root executables;
- `tools/` — offline/development executables;
- `tests/` — verification only;
- `content/` — canonical source content;
- `shaders/` — canonical shader source where applicable;
- `cmake/` — project CMake policy/modules;
- `third_party/generated/` — reviewed generated third-party source such as glad2 when required;
- `docs/` — design/architecture/implementation authority;
- `.github/workflows/` — executable CI policy.

Generated build output belongs under `out/` and is never source authority.

## 10. Implementation Lock Rule

After TA-16 closure, TA-0 through TA-15 baseline contracts are Implementation Locked for `TA16-V1`.

An implementation may choose local algorithms/data structures only where the owning specification intentionally leaves freedom and the choice preserves:

- externally observable semantics;
- ownership/mutation authority;
- deterministic ordering requirements;
- persistence compatibility;
- backend boundaries;
- performance hard caps;
- required tests/diagnostics.

## 11. Change-Control Rule

If implementation discovers that a locked contract is infeasible or materially wrong:

1. stop implementing the conflicting behavior;
2. identify the owning GDS/TA contract;
3. write an explicit architecture decision/change with rationale and affected invariants;
4. update owning specifications rather than patching around them;
5. rerun relevant cross-validation/tests;
6. update baseline/lock metadata if the contract changed;
7. only then continue implementation.

Code is never allowed to become de facto authority merely because it already exists.

## 12. TA-16 Exit Gate

TA-16 is complete only when all of the following are true:

- all sixteen lock categories have a canonical owner/artifact;
- toolchain/dependency snapshot is exact and reproducible;
- glad2 mismatch with the vcpkg glad1 port is explicitly resolved;
- concrete CMake DAG preserves TA-15 dependency direction;
- presets and test labels are fixed;
- bootstrap CMake configure/build/test contract is real;
- CI runs those real contracts with zero-test protection;
- no placeholder-green future subsystem gate exists;
- implementation phases and first vertical slice are dependency-ordered;
- milestone/branch/change-control rules are fixed;
- reference performance hardware/runner is named;
- architecture-decision sequence remains continuous;
- TA16 cross-validation has no blocker;
- root/technical roadmap state is synchronized.

## 13. Post-TA-16 Authorization

A PASS opens the implementation gate in this order:

```text
TA-16 Implementation Locked / PASS
        ↓
IMP-1 Core + Deterministic Simulation Foundation
        ↓
subsequent dependency-ordered implementation phases
```

TA-16 does not authorize breadth-first gameplay construction. Each implementation phase must satisfy its own exit evidence before dependent phases rely on it.

## 14. Non-Goals

TA-16 does not implement gameplay systems, build production assets, define new GDS behavior, create a general engine SDK, guarantee final content quantities, or certify final performance before representative implementation exists.