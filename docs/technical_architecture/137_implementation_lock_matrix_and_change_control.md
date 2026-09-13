# TA-16 — Implementation Lock Matrix and Change Control

> **Status:** Implementation Locked  
> **Baseline:** `TA16-V1`  
> **Authority:** Which architecture contracts implementation may rely on as frozen and how changes are governed

## 1. Lock Meaning

`Implementation Locked` means the owning architecture is sufficiently exact for implementation and dependent code may rely on it. It does not mean every tuneable numeric content value is permanent.

A locked contract may expose explicit tuneables, authored data, quality settings or backend-local algorithms. Those remain variable only inside the bounds already defined by the owning specification.

## 2. Phase Lock Matrix

| Architecture phase | Implementation lock | Primary implementation consumers |
|---|:---:|---|
| TA-0 governance | LOCKED | all phases |
| TA-1 runtime/simulation | LOCKED | IMP-1 onward |
| TA-2 identity/state/transactions | LOCKED | IMP-1 onward |
| TA-3 scene/world/streaming | LOCKED | IMP-4 onward |
| TA-4 renderer | LOCKED | IMP-3 onward |
| TA-5 physics/character/spaceflight | LOCKED | IMP-4/10 onward |
| TA-6 station simulation | LOCKED | IMP-7 onward |
| TA-7 runtime entities/combat | LOCKED | IMP-4/6 onward |
| TA-8 AI/navigation | LOCKED | IMP-8 onward |
| TA-9 missions/raids/events | LOCKED | IMP-9/11 onward |
| TA-10 content pipeline | LOCKED | IMP-2/3/14 onward |
| TA-11 input/UI/audio/presentation | LOCKED | IMP-4/12 onward |
| TA-12 persistence | LOCKED | IMP-2 onward |
| TA-13 performance/concurrency | LOCKED | every implementation phase |
| TA-14 verification/CI | LOCKED | every implementation phase |
| TA-15 integration audit | LOCKED | TA-16 + all implementation integration reviews |
| TA-16 implementation handoff | LOCKED | IMP-0 onward |

## 3. Lock Waves

For implementation sequencing, locked contracts are consumed in waves:

```text
Wave A — foundational
TA-0, TA-1, TA-2, TA-13, TA-14, TA-15, TA-16

Wave B — data/persistence/content foundation
TA-10, TA-12

Wave C — active local runtime
TA-3, TA-4, TA-5, TA-7, TA-11

Wave D — systemic gameplay
TA-6, TA-8, TA-9
```

The waves describe dependency consumption, not different authority levels.

## 4. Contract Classes

Implementation-facing statements are classified as:

- **Invariant** — implementation cannot vary without architecture/GDS change;
- **Interface contract** — exact project-owned API/data boundary can evolve mechanically but must preserve semantics and dependency direction;
- **Persistence contract** — change requires schema/version/migration compatibility handling;
- **Determinism contract** — output/order must remain stable under specified schedule/worker variation;
- **Budget hard cap** — cannot be exceeded by silently dropping semantic state;
- **Acceptance target** — measured performance/quality target may drive optimization, never semantic compromise;
- **Authored/tuneable data** — may vary through validated content/settings under explicit bounds.

Code review must know which class a changed assumption belongs to.

## 5. Invariants Requiring Formal Change Control

Examples include:

- 60 Hz authoritative Simulation Time baseline;
- no offline progression;
- True Pause semantics;
- persistent ID vs runtime handle separation;
- one physical owner;
- prepared atomic transaction model;
- one player-local authoritative scene;
- Horizon persistent off-screen truth;
- persistent state first, runtime projection second;
- no backend IDs in saves;
- ContentId logical identity;
- staged all-or-nothing load;
- no presentation/worker authority;
- no performance-driven gameplay simplification;
- headless gameplay testability;
- no general-purpose engine host.

## 6. Ordinary Implementation Freedom

No architecture change is required for choices such as:

- private container choice inside a target when deterministic/public behavior is unchanged;
- cache layout;
- allocator implementation below a locked lifetime contract;
- internal algorithm optimization preserving outputs/order requirements;
- renderer batching strategy preserving snapshots/visibility semantics;
- project-local naming of private source files;
- additional diagnostics;
- additional tests;
- presentation polish within GDS/accessibility/knowledge rules.

## 7. Change Categories

### C0 — Mechanical

No semantic contract changes. Examples: refactor, file move, private rename, build-speed improvement.

Required: normal tests/review.

### C1 — Contract-Compatible Interface Evolution

Public project API changes but semantics/ownership/persistence remain equivalent.

Required: owner tests, dependent compile/test evidence, documentation update where useful.

### C2 — Architecture Baseline Change

Ownership, threading, persistence, deterministic ordering, build dependency direction, backend policy, hard cap or implementation baseline changes.

Required: Architecture Decision, owning TA updates, affected cross-validation and implementation roadmap/CI update.

### C3 — GDS Semantic Change

Player-visible gameplay behavior/design invariant changes.

Required: GDS change first, design revalidation, then corresponding TA/AD/implementation changes.

## 8. Persistence Change Rule

Any change to save bytes, SectionKind, domain Save DTO meaning, PersistentId interpretation, exactly-once markers, ContentId compatibility, RNG continuation or saved tick semantics is at least C2 and must include:

- versioning decision;
- migration path or explicit unsupported boundary;
- current/historical golden updates with rationale;
- corruption/fault tests;
- source-save preservation.

## 9. Content Change Rule

Schema/ContentId/cook-fingerprint compatibility changes are at least C1 and can become C2 if they alter persistence or deterministic runtime meaning.

Renaming/removing a persisted ContentId requires an explicit compatibility/migration rule, never fuzzy lookup.

## 10. Dependency Upgrade Rule

Compiler/CMake/vcpkg baseline/backend version changes are C2 for the implementation baseline even when source API changes are small. They require the upgrade procedure in `131_toolchain_dependency_and_license_lock.md`.

## 11. Test Change Rule

Deleting or weakening a test that protects a locked contract requires review of what evidence replaces it. A failing test is not evidence that the contract should be weakened.

Golden/baseline files are changed only with an explanation of the approved behavior/version change.

## 12. Performance Change Rule

A performance optimization that changes authoritative state, number/timing of semantic events, knowledge, ownership, hit results, production, mission timing, save contents or procedural selection is not an optimization under TA16-V1; it is a C2/C3 change proposal.

## 13. Emergency Fix Rule

A release-blocking crash/corruption fix may be expedited, but it does not bypass authority:

- stop unsafe continuation;
- prefer fail-safe rejection over invented state;
- record the violated invariant;
- add regression evidence;
- backfill the required AD/spec update if the fix exposes a missing contract.

## 14. Decision Continuity

Architecture decisions AD-001 onward form one continuous logical registry. TA-12/13/14 addenda remain immutable historical source records; TA-16 provides a consolidated decision registry/index and carries forward their accepted semantics.

No AD number is reused or silently rewritten.

## 15. Lock Verdict

```text
TA-0 through TA-16 baseline contracts: IMPLEMENTATION LOCKED
Private compatible implementation freedom: ALLOWED
Silent contract drift: PROHIBITED
Persistence/content identity drift: VERSIONED CHANGE ONLY
Architecture baseline changes: AD + owning spec + revalidation
Gameplay semantic changes: GDS FIRST
```
