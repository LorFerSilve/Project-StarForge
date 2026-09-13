# IMP-1 / M1 Completion Report

> **Implementation phase:** IMP-1 — Core, Identity, Deterministic Simulation, and Transactions  
> **Milestone:** M1 — Deterministic Core Ready  
> **Architecture baseline:** `TA16-V1`  
> **Status:** COMPLETE

## Integrated implementation

IMP-1 was integrated to `main` through PR #2 using the repository's locked squash-merge policy.

- Verified PR head: `aa733e99cf36b57141f7c49a76d2956f6d46ff78`
- Integrated `main` commit: `b88d56e61cb44617e605e3464e770d50834db9a2`
- Base before IMP-1: `54c0361359b16588e09650e3cebd2c9c1c5cef05`

The implementation materializes the IMP-1 foundation defined by TA-16:

- typed Result/error, invariant/assert and diagnostics contracts;
- strongly typed persistent/operation identities and monotonic non-reusing allocation;
- state revisions and activation generations;
- SplitMix64-derived scoped PCG32 deterministic RNG;
- 60 Hz simulation clock and deterministic tick pipeline shell;
- typed command/result/event envelopes;
- prepared transaction coordination with deterministic participant ordering and final revision recheck;
- immutable generation-stamped read-model publication;
- headless deterministic composition-root exercise;
- real `sf_diagnostics`, `sf_simulation`, `sf_transactions`, `sf_read_models`, and `starforge_tests_domain` targets.

No platform, renderer, physics, content-schema, or persistence authority is introduced by IMP-1.

## Exit evidence

GitHub Actions run `34767814898` executed against exact PR head `aa733e99cf36b57141f7c49a76d2956f6d46ff78` and completed successfully.

Required IMP-1 checks:

| Check | Result |
|---|---|
| `StarForge / Build & Unit` | PASS |
| `StarForge / Headless Determinism` | PASS |
| `StarForge / CI Gate` | PASS |

The determinism gate configured, built, discovered non-zero domain tests, ran the semantic domain suite, and proved repeated headless digest stability. Domain coverage includes selected worker partition counts and transaction rejection/commit invariants.

## Architecture review

The integrated change remains within the locked IMP-1 target boundary and does not add GLFW, OpenGL, Jolt, audio, persistence, or content-model dependencies. The transaction coordinator performs a final revision recheck before committing, uses stable participant commit ordering, and publishes committed events only after all participant commits complete.

The duplicate competing PR #3 was not used because its exact-head CI failed the deterministic-domain-test discovery gate; it was closed rather than merged.

## Known limitations

These are intentional phase boundaries, not IMP-1 defects:

- no ContentId registry or persistence codec yet;
- no save/load format yet;
- no platform/window/render backend yet;
- no physics/world/player runtime yet;
- no production gameplay systems yet.

Repository branch protection/ruleset enforcement is an administrative repository setting and is not represented as implementation evidence here. Development continues to follow the locked PR + exact-head green-CI integration policy.

## Dependency handoff

IMP-1 exit requirements are satisfied and M1 is the current implementation evidence state.

The next formal roadmap dependency is:

```text
IMP-2 — Content Model and Persistence Primitives
```

Future implementation work must keep the IMP-1 deterministic-core regression suite green.
