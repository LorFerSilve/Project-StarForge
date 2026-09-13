# TA-14 — Headless Determinism, Transactions, and Scenario Testing

> **Status:** Architecture Complete  
> **Authority:** Headless composition, deterministic replay, transaction/invariant suites, worker-count equivalence, active/off-screen equivalence, scenario scripts, and authoritative-state checkpoint testing

## 1. Purpose

StarForge's most important correctness properties do not require rendering. They require deterministic simulation, valid ownership, causal ordering, exactly-once transitions, and persistence-safe domain boundaries.

This specification defines the mandatory headless verification layer.

## 2. Headless Composition Root

The test composition root can instantiate gameplay without:

- GLFW window;
- OpenGL context;
- physical audio device;
- user profile;
- network access.

Presentation/platform services are replaced by deterministic null/test adapters.

The same domain implementations used by the shipping runtime remain under test.

## 3. Canonical Scenario Runner

A `HeadlessScenarioRunner` conceptually owns:

- initial content registry fixture;
- deterministic SaveSeed/RNG streams;
- SessionRoot;
- fixed 60 Hz tick driver;
- scripted command timeline;
- worker-result injection/controlled worker executor;
- checkpoint/state-hash collection;
- invariant probes;
- optional save/load cycle points.

## 4. Scripted Inputs

Scenario inputs are semantic typed commands/actions stamped to SimulationTick.

They do not depend on render frames or wall-clock delay.

## 5. Deterministic Checkpoints

Scenario checkpoints can record selected canonical state such as:

- persistent IDs and owner relationships;
- inventory/resource quantities;
- Health/status values;
- mission/objective state;
- station conserved state;
- ship/robot/crew state;
- strategic timers/events;
- RNG stream state;
- active-context identity.

Checkpoint serialization/order is project-owned and stable enough for diagnostics.

## 6. State Equivalence Hash

A diagnostic canonical hash may summarize checkpoint state.

Rules:

- hash input uses stable sorted semantic keys;
- no pointers/runtime/backend handles;
- hash algorithm/version is explicit;
- hash is only regression evidence, not save/security authority;
- mismatches produce a structural diff where possible.

## 7. Transaction Tests

Every cross-domain transaction family requires tests for:

- all preconditions valid -> all mutations commit;
- one participant rejects -> no participant mutates;
- stale revision -> reject without partial change;
- repeated same TransactionId/exactly-once marker -> no duplicate consequence;
- save/load after commit -> consequence remains exactly once;
- failure before commit -> no leaked owner/credit/item/resource state.

## 8. Ownership Conservation

Property tests cover physical ownership conservation for:

- unique items;
- stack resources;
- logistics TransferCargoOwner;
- raid theft/extraction;
- mission field/security transitions;
- robot commissioning components;
- trade transactions.

For conserved quantities, sum across all valid owners before/after must match explicit production/destruction rules.

## 9. Credits Conservation / Ledger

Credit tests assert integer smallest-unit arithmetic and atomic trade/reward transitions.

No floating rounding or partial seller/buyer state is permitted.

## 10. Persistent Identity

Tests assert:

- type distinction;
- nonzero IDs;
- monotonic non-reuse within timeline;
- failed creation does not consume committed identity where contract says so;
- save/load preserves IDs;
- migrations preserve IDs unless explicit deterministic split rule applies.

## 11. Runtime Handle Staleness

Generation-checked RuntimeEntity/Physics/Content handles require stale-handle tests:

1. allocate slot;
2. destroy/reclaim;
3. reuse index with new generation;
4. old handle must reject.

Runtime handles never appear as persistent identities in save DTOs.

## 12. Activation Lease Tests

For each persistent actor type, tests verify:

- activation transfers only declared high-frequency facets;
- dormant store copy is not independently mutated;
- deactivation validates ID/epoch;
- leased state returns exactly once;
- save while active produces one logical DTO;
- stale activation result cannot overwrite newer state.

## 13. Deterministic Worker Equivalence

Mandatory scenario matrix runs with:

```text
1 worker (where test executor supports)
2 workers
default worker count
maximum supported runtime worker count
```

Completion timing/order may differ, but authoritative checkpoint state must be equivalent.

For production architecture default/max remain TA-13 values; one-worker test mode exists to simplify reproducibility.

## 14. Frame-Rate Equivalence

Where the App/presentation harness participates, scripted runs use multiple render schedules such as:

```text
30 FPS
60 FPS
144 FPS
irregular frame cadence
```

The same number/timeline of fixed simulation ticks must produce equivalent authoritative checkpoints.

## 15. True Pause Tests

Tests prove True Pause stops:

- gameplay ticks;
- station production;
- status timers;
- Dynamic Events;
- travel/recovery;
- strategic timers.

Presentation/file workers may continue without modifying authoritative gameplay.

## 16. Hard Streaming Hold Tests

Tests prove a Hard Streaming Hold:

- occurs only at stable boundary;
- stops Simulation Time;
- allows required loading workers;
- resumes without giving actors extra/missing ticks;
- produces same post-resume gameplay state regardless of load delay.

## 17. Station Conservation / Off-Screen Equivalence

TA-6 suites compare representative active and off-screen advances from the same state.

They verify:

- power boundaries;
- atmosphere gas conservation;
- thermal energy/coolant behavior;
- Fresh/Wastewater conservation;
- logistics ownership;
- WorkOrder exactly-once completion;
- chronological threshold splitting.

Optimization may change computational path, not gameplay-equivalent outcome.

## 18. AI Knowledge Tests

Headless tests ensure AI cannot read hidden world truth.

Cases include:

- target behind occlusion;
- stale last-known position;
- hearing-only approximate location;
- squad-shared confidence/precision;
- communication loss;
- reconnect without replaying obsolete robot command packets.

## 19. Navigation Tests

Navigation/path tests cover:

- traversal-profile compatibility;
- door/airlock/security authorization separate from route geometry;
- stale NavigationRevision rejection;
- dynamic topology invalidation;
- no teleport stuck recovery;
- deterministic result acceptance independent of worker completion.

## 20. Combat Tests

Headless combat verifies:

- camera-to-muzzle reconciliation;
- friendly physical blocking;
- hitscan versus swept projectile semantics;
- status tick timing;
- melee once-per-attack hit accounting;
- shield/armor/Health ordering;
- collision fact -> gameplay damage separation;
- no synthetic load/rebase impact.

## 21. Mission / Objective Tests

Mission suites cover:

- one external deployed MissionInstance;
- persistent attempt history;
- objective DAG prerequisites;
- exactly-once objective transitions;
- extraction security semantics;
- failure without rollback/refund;
- story retry retaining world consequences;
- procedural anti-reroll across reload/worker count.

## 22. Raid / Defense / Dynamic Event Tests

Tests verify:

- offensive raid uses MissionInstance execution;
- Horizon DefenseEvent persists active/off-screen;
- physical theft ownership chain;
- committed reinforcement call cannot be unsent;
- Recovery Grace timing;
- DynamicEvent -> Mission/Defense handoff exactly once;
- event existence separate from player knowledge.

## 23. Finale Tests

MS-F02 tests guarantee:

- intent is reversible before FinalResolution commit;
- only `Stabilize`, `Sever`, `Contain` are valid;
- exactly one final resolution commits;
- save/load cannot select a second ending;
- all affected domains commit atomically.

## 24. Recovery Transit Tests

Recovery tests verify:

- causal destination/path;
- no hidden fast travel;
- player/ship/robot/cargo recovery separation;
- Horizon state continues chronologically where Simulation Time advances;
- destination invalidation selects only a valid recovery path.

## 25. Scenario Suite Tiers

Canonical tiers:

- **S0 micro scenarios:** <=100 ticks;
- **S1 domain integration:** <=10,000 ticks;
- **S2 systemic scenarios:** minutes of simulated gameplay;
- **S3 campaign/strategic soak:** long headless runs with checkpoints.

TA-14 CI architecture assigns S0/S1 to ordinary PR gates and schedules broader S2/S3 coverage separately where runtime cost requires.

## 26. Soak Tests

Headless soak runs seek:

- state leaks;
- timer duplication;
- growing queues;
- identity reuse;
- numerical drift outside defined tolerances;
- event-loop runaway;
- unbounded strategic backlog.

A soak test uses bounded deterministic seeds and emits periodic checkpoints.

## 27. Failure Reproduction Bundle

A scenario failure artifact includes enough to replay:

- scenario ID/version;
- content/fixture version;
- seed/RNG stream states where practical;
- command timeline;
- worker-count/configuration;
- last successful checkpoint;
- failing tick;
- state diff/trace excerpt.

## 28. No Golden-Hash-Only Acceptance

A matching state hash is useful but cannot be the only assertion for critical ownership/transaction invariants.

Core tests also assert semantic facts so a hash implementation bug cannot hide a violation.

## 29. Explicit Non-Goals

The headless layer does not certify:

- OpenGL visual output;
- real audio device playback;
- platform window behavior;
- GPU performance.

Those belong to backend smoke/performance layers.

## 30. Dependencies

Depends on TA-1 fixed runtime, TA-2 determinism/transactions, TA-3 holds/transitions, TA-5 physics facts, TA-6 station, TA-7 runtime, TA-8 AI/navigation, TA-9 strategic state, TA-12 persistence and TA-13 worker/performance contracts.

## 31. Open Questions

None within headless/deterministic scenario scope.