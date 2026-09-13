# TA-16 — Branch, Merge, Milestone, and Release Strategy

> **Status:** Implementation Locked  
> **Baseline:** `TA16-V1`  
> **Authority:** Repository integration flow and evidence required before implementation milestones depend on one another

## 1. Main Branch Role

`main` is the authoritative integration branch.

Once implementation CI/ruleset enforcement is active, normal implementation reaches `main` through reviewed pull requests with the current required checks green for the candidate head SHA.

`main` is not a scratch branch for half-integrated architectural experiments.

## 2. Branch Model

StarForge uses short-lived branches rather than long-running parallel release/develop branches.

Canonical prefixes:

```text
impl/<imp-phase>-<slug>
fix/<slug>
refactor/<slug>
test/<slug>
docs/<slug>
tool/<slug>
content/<slug>
```

Examples:

```text
impl/imp1-simulation-clock
impl/imp3-opengl-context
fix/save-crc-validation
test/ownership-property-cases
```

## 3. Branch Lifetime

Branches should represent a reviewable dependency slice. Large phases are delivered through multiple coherent PRs rather than one phase-long branch.

A branch must rebase/merge current `main` as needed before final integration when upstream contracts changed materially.

## 4. Merge Strategy

Default merge policy is **squash merge** for implementation PRs so each reviewable change becomes one coherent `main` commit while branch-local iteration remains disposable.

A deliberately structured multi-commit merge can be retained only when the commit sequence itself has value and each retained commit is buildable/reviewable.

Force-pushing `main` is prohibited in normal development.

## 5. Pull Request Contract

A PR that changes implementation must state:

- owning IMP phase;
- owning TA/GDS contracts;
- affected targets;
- tests added/changed;
- persistence/content compatibility impact;
- performance/concurrency impact where applicable;
- whether it is C0/C1/C2/C3 under TA-16 change control;
- any follow-up dependency unlocked by the merge.

Small mechanical PRs can keep this concise but cannot omit material compatibility/architecture impact.

## 6. Review Rule

Architecture-sensitive changes require explicit review against the owning contract before merge. Examples include:

- new cross-target dependency;
- backend type crossing an adapter boundary;
- persistent schema/ID change;
- new worker mutation path;
- tick-order change;
- new global/singleton state;
- save/load semantics;
- ContentId/schema behavior;
- CI required-gate weakening.

## 7. CI Rule

The merge candidate must pass every currently activated required check for its exact head SHA.

A prior SHA's green run, manually rerun unrelated commit, or canceled superseded workflow is not merge evidence.

## 8. Required Check Growth

Required checks are activated progressively as real suites exist:

1. Build & Unit + CI Gate — TA-16 bootstrap;
2. Headless Determinism — IMP-1;
3. Persistence Compatibility — IMP-2;
4. Backend Smoke — IMP-3/4 as stable adapters exist;
5. Content Validation — IMP-2/14 as cooker/schema targets exist;
6. Static Analysis — IMP-1 once exact LLVM tooling is active;
7. Sanitizers — IMP-1/4 once supported presets execute real tests;
8. Performance Regression — only on controlled reference milestones.

A later gate can be added earlier if its actual owned behavior exists; it cannot be added as an empty success placeholder.

## 9. Milestone Model

Implementation milestones are evidence states, not dates.

### M0 — Technical Architecture / Bootstrap Locked

- TA-16 PASS;
- IMP-0 build/test/CI bootstrap green;
- implementation gate open.

### M1 — Deterministic Core Ready

- IMP-1 exit complete;
- headless simulation/transaction/RNG identity tests green;
- no platform dependency in core correctness.

### M2 — Data / Persistence Foundation Ready

- IMP-2 exit complete;
- current save binary golden/corruption/content-ID tests green.

### M3 — Local Runtime Foundation Ready

- IMP-3 and IMP-4 exits complete;
- renderer/platform/physics/world backend smoke green.

### M4 — Vertical Slice V0 Certified

- IMP-5 acceptance matrix green;
- save/load/ownership/frame-cadence/headless equivalence proven.

### M5 — Systemic Gameplay Foundation

- IMP-6 through IMP-9 exits complete;
- combat/station/AI/mission foundations integrate without authority regressions.

### M6 — Full Mobility / Strategic Loop

- IMP-10 and IMP-11 exits complete;
- spacecraft, raids, defense/events/recovery integrate.

### M7 — Player-Facing Systems / Campaign Loop

- IMP-12 and IMP-13 exits complete;
- shipping UI/audio/accessibility/economy/progression/narrative loop available.

### M8 — Production Content / Scale Ready

- IMP-14/15 exit evidence;
- deterministic content pipeline and representative scale/performance/persistence hardening green.

### M9 — Release Candidate Certification

- approved content complete;
- no unresolved critical correctness/security/save-loss issue;
- exact-SHA TA-14 release evidence complete;
- reference performance certification complete;
- third-party notices/provenance complete.

## 10. Milestone Promotion

A milestone becomes accepted only through a repository artifact or release/implementation report that records:

- exact commit SHA;
- required test/check results;
- known accepted limitations;
- benchmark/runner identity where required;
- migration/content baseline identifiers where relevant;
- next dependency.

## 11. Regression Rule

Once a milestone contract is depended on, later phases must keep its required regression suite green.

A new feature does not get to invalidate ownership/save/determinism contracts from an earlier milestone and call the old milestone obsolete.

## 12. Release Branches

Long-lived release branches are not required initially. If stabilization later needs one, use a short-lived `release/<version>` branch cut from an accepted `main` SHA; fixes merge/cherry-pick back to `main` so authority does not fork permanently.

## 13. Versioning

Executable/product versioning is independent from save/container/schema versions. A product version bump does not imply a save migration unless the persistence contract actually changed.

Implementation baseline `TA16-V1` remains architecture/build metadata and is not a substitute for semantic versions of persisted formats.

## 14. Hotfix Rule

A release hotfix must:

- identify the failing invariant;
- add/retain a regression test where feasible;
- preserve save compatibility or explicitly migrate/version it;
- be integrated back to `main`;
- regenerate exact-SHA evidence for affected release gates.

## 15. No Calendar-Driven Waivers

A target date cannot waive:

- deterministic correctness;
- ownership conservation;
- save/load integrity;
- required content validation;
- sanitizer-critical defects;
- reference performance hard caps when claiming that milestone;
- accessibility/knowledge invariants.

Scope can be reduced only through explicit GDS/project governance, not hidden implementation shortcuts.

## 16. Lock Verdict

```text
Integration branch: main
Working branches: short-lived typed prefixes
Default merge: squash PR
Exact-head CI: REQUIRED
Force-push main: PROHIBITED in normal flow
Milestones: evidence-based M0-M9
Long-lived develop branch: NONE
Release branch: optional short-lived only
Calendar waiver of correctness: PROHIBITED
```
