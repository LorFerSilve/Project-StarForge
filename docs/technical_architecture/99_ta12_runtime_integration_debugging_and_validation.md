# TA-12 — Runtime Integration, Debugging, and Validation

> **Status:** Architecture Complete  
> **Authority:** Persistence integration with the fixed-tick runtime, save/load phase ownership, presentation handoff, content/runtime reconstruction, deterministic operation ordering, debug controls, headless validation, and TA-12 closure criteria

## 1. Purpose

TA-12 closes persistence only if save/load behavior is placed unambiguously inside the existing TA-1 through TA-11 runtime architecture.

This specification defines where persistence participates, what it may observe, what it may mutate, and how the final implementation is validated without turning file I/O into gameplay authority.

## 2. Runtime Ownership Summary

Persistence integrates with these authorities:

- **TA-1:** Stable Simulation Boundary, main-thread authoritative commit ownership, worker-job boundary;
- **TA-2:** DomainStores, Save DTOs, IDs, transactions, migrations, RNG/integrity;
- **TA-3:** scene/context transitions, Hard Streaming Hold, persistent world reconstruction;
- **TA-4/5/8/11:** backend/presentation state is reconstructed, never serialized as authority;
- **TA-6/7:** active leased state exports coherently without forced deactivation;
- **TA-9:** mission/raid/event/finale exactly-once state persists;
- **TA-10:** ContentId and ContentBuildId compatibility;
- **TA-11:** save/load UI, settings/profile, presentation reconstruction.

## 3. Fixed-Tick Save Integration

A pending save request is inspected only at the TA-1/TA-7 Stable Simulation Boundary.

Canonical tick interaction:

```text
... gameplay/domain commits
-> deferred lifecycle/physics mutation
-> Stable Simulation Boundary
-> if save requested: capture immutable SaveSnapshot
-> publish normal read/presentation state
-> next gameplay tick
```

Snapshot capture does not insert a second gameplay commit phase.

## 4. No Mid-Tick Snapshot

Persistence cannot snapshot directly from:

- physics callback;
- combat resolution halfway through consequences;
- UI button callback;
- worker completion callback;
- render frame;
- audio callback;
- open cross-domain transaction.

A user request from any presentation surface becomes a pending request until the next eligible Stable Save Boundary.

## 5. Capture Before Presentation Event Does Not Lose State

The save captures authoritative state after all current-tick gameplay commits.

Presentation events for those commits may be published immediately after capture in the same stable-boundary handoff.

Because presentation one-shots are not gameplay authority, the save does not need to preserve a pending toast/sound merely to prove the underlying commit occurred.

Persistent alarms/incidents are reconstructed from owning state after load.

## 6. Snapshot Export Ordering

Required section export occurs in ascending SectionKind order.

Within each domain, record encoding/export ordering uses the domain's deterministic schema key.

Export ordering must not depend on:

- RuntimeEntityRegistry slot layout;
- unordered container buckets;
- worker completion;
- render/audio iteration;
- filesystem layout.

## 7. Save Worker Handoff

After immutable SaveSnapshot capture:

1. main thread hands immutable snapshot ownership/reference to persistence worker infrastructure;
2. worker encodes sections/container and computes CRCs;
3. worker writes pending file;
4. worker revalidates pending container;
5. storage commit occurs;
6. completion result returns through a typed persistence completion queue;
7. main/presentation state publishes `Committed` or `Failed` result.

The completion queue cannot mutate gameplay domains.

## 8. Session Token

Every save/load operation carries a process-local `SessionToken`/generation identifying the SessionRoot that originated it.

Rules:

- token is not serialized as gameplay identity;
- save completion for an old session remains valid storage history but cannot update newer session gameplay state;
- stale operation UI notifications are correlated by operation ID/session generation;
- load staging result is discarded if its owning application request/session transition has been superseded before activation.

## 9. Load Application State

A load operation moves the application into a controlled loading state where authoritative gameplay ticks for the current session do not continue past the chosen transition boundary.

Loading does not use a partial pause in which Horizon/markets/timers continue.

Simulation Time does not advance while the selected save is being decoded/migrated/staged.

## 10. Old Session Lifetime During Load

The old SessionRoot remains logically intact until staging reaches `ReadyToActivate`.

Its presentation can be replaced by a loading screen, but gameplay state is not destructively cleared early.

This allows a pre-swap load failure/cancellation to return safely to the prior session where product flow permits.

## 11. Final Swap Ordering

Canonical activation handoff:

```text
staged DTOs/stores validated
-> required ContentIds resolved
-> RuntimeActivationPlan validated
-> application reaches replacement boundary
-> old gameplay tick authority stops
-> SessionRoot pointer/owner swapped once
-> old runtime/backend state retired
-> new SceneInstance/runtime/backends constructed
-> initial immutable ReadModels published
-> presentation reconstructs
-> first new authoritative tick permitted
```

No presentation screen can expose the new session as playable before required runtime reconstruction succeeds.

## 12. Backend Reconstruction Order

Project composition uses dependency-safe reconstruction, conceptually:

1. session/domain stores current;
2. current active Location/Zone/Scene definition resolved;
3. runtime entity registry and activation descriptors created;
4. navigation/physics world prepared;
5. persistent-backed actors/local continuation activated;
6. docking/constraints/spatial state reconstructed;
7. render proxies/resources connected;
8. audio semantic state/listener reconstructed;
9. UI/read models/persistent alarms published.

Exact internal target order can be refined in TA-16 as long as these authority dependencies are preserved.

## 13. Physics/Simulation Resume

The first physics/gameplay tick after load starts from saved authoritative positions/velocities/state.

Loading itself cannot:

- cause a collision impulse;
- advance projectiles/status timers;
- apply an extra AI decision tick;
- complete WorkOrders;
- trigger Dynamic Events;
- consume fuel/ammo;
- advance Recovery Transit.

All such changes require the next normal Simulation Time tick.

## 14. Timers Due at Resume Boundary

The project uses a single deterministic resume convention:

- save represents complete state **after** `saved_simulation_tick` has reached its Stable Simulation Boundary;
- the next ordinary authoritative tick is `saved_simulation_tick + 1`;
- timers/actions that became due during the saved tick must already have been processed before capture;
- timers due at the next tick execute normally after resume.

This prevents double-processing the saved tick.

## 15. Save Request During True Pause

True Pause freezes Simulation Time but the game is already at a stable, non-mutating state between ticks.

A Manual/Quick Save requested while True Paused may capture from the current last Stable Simulation Boundary without advancing a gameplay tick, provided:

- no application-level atomic transition/load is in progress;
- no persistence capture already owns the boundary;
- all active-state exporters can produce the current coherent snapshot.

Saving does not unpause or advance gameplay.

## 16. Autosave During True Pause

Periodic Active Game Time autosave triggers do not become newly due while True Paused.

A milestone autosave that was already queued before pause can capture at the paused stable state if its trigger's authoritative transaction has committed.

## 17. Hard Streaming Hold

Hard Streaming Hold stops Simulation Time because required gameplay content is unavailable.

Persistence behavior:

- an existing valid stable authoritative state can be saved if all state required for its Save DTO export is available;
- saving cannot treat missing required content as resolved;
- loading another save can proceed through its own staging/content validation;
- no gameplay progresses simply because persistence workers continue.

## 18. Scene Transition Interaction

If a save request occurs during an indivisible inter-context transition:

- request waits;
- destination/source ownership transition commits or aborts;
- capture occurs at the next Stable Save Boundary;
- snapshot contains one authoritative context/location result, never half source + half destination.

## 19. Persistence vs Hot Reload

Development content hot reload cannot race load compatibility decisions.

A load staging operation records the ContentRegistry generation/ContentBuildId used.

If the registry changes before final staging gate:

- load staging is invalidated/revalidated against the new registry;
- stale content handles are discarded;
- the session is not activated against a mixed content generation.

## 20. Profile Persistence Integration

Profile/application settings writes:

- do not require Stable Save Boundary;
- can occur while no campaign is loaded;
- can continue under True Pause;
- cannot advance or mutate gameplay;
- use their own atomic candidate/replace path;
- publish a separate profile persistence result.

## 21. Save Catalog Runtime Integration

Catalog scans are asynchronous/non-gameplay work.

A catalog update publishes an immutable `SaveCatalogReadModel` generation to menus.

It cannot alter the currently loaded SessionRoot or automatically load another generation.

## 22. Debug Commands

Development-only debug commands can request:

- save now at next stable boundary;
- inspect pending save operations;
- inject storage failure at selected stage;
- force catalog rebuild;
- run full current-session persistence invariant scan;
- dump current Save DTOs;
- dry-run encode/decode/migration round-trip;
- compare current session export to a freshly loaded staging reconstruction.

Debug commands still use normal persistence boundaries unless explicitly running isolated test data.

## 23. Persistence Round-Trip Validator

A development/headless validator can perform:

```text
current synthetic/session state
-> SaveSnapshot
-> encode container
-> decode
-> migrate (no-op/current where applicable)
-> staging import
-> full validation
-> re-export current DTOs
-> deterministic semantic comparison
```

It does not need to activate OpenGL/audio to prove gameplay persistence completeness.

## 24. Serialization Completeness Probe

For selected system tests, the harness can:

1. save at a known stable tick;
2. clone expected future command inputs;
3. continue original session N deterministic ticks;
4. load the save into a fresh headless composition;
5. replay the same future inputs;
6. compare authoritative domain outcomes.

This tests whether consequential continuation state was omitted.

It does not require whole-engine bit-identical physics on unsupported platforms; comparison targets contract-defined authoritative state/tolerances.

## 25. Active-Context Continuation Tests

Required representative fixtures include save/load during:

- ordinary Horizon traversal;
- combat with active status/projectile state;
- external Mission with Field-Unsecured cargo;
- extraction available but not committed;
- offensive Raid with breaches/escalation/reinforcement state;
- Horizon Defense;
- local spacecraft flight/docking state;
- Incapacitated stable player state;
- Recovery Transit;
- WorkOrder/production active state;
- Dynamic Event coexistence.

## 26. Exactly-Once Persistence Tests

Round-trip/load tests must prove no duplication/replay of:

- physical item/resource ownership;
- Credits transactions;
- mission resolution rewards;
- Blueprint/Research grants;
- reinforcement calls;
- manufacturing outputs;
- extraction security transitions;
- finale Stabilize/Sever/Contain choice.

## 27. Deterministic Procedural Tests

Tests save/load before and after procedural future choices and verify:

- same committed generated mission/event state;
- same RNG cursor continuation;
- same candidate ordering independent of worker timing;
- no reload reroll;
- GenerationAlgorithmVersion respected.

## 28. Migration Integration Tests

For every supported old fixture:

- game loader and inspector use same migration chain;
- migration is deterministic;
- source bytes remain unchanged;
- current staged domain invariants pass;
- subsequent new save writes current schema/container;
- reloading that new save requires no historical migration.

## 29. Crash/Fault Integration Tests

Headless storage backend simulates failure/crash at every TA-12 write stage.

Invariant:

> At no pre-commit failure point does the operation remove the prior valid generation.

After post-commit crash, catalog rebuild discovers the committed file without requiring the old index.

## 30. Corrupt Input Safety

Persistence parsers/decoders treat save files as untrusted lengths/counts.

Tests verify malformed input cannot:

- allocate unbounded memory beyond configured ceilings;
- read outside buffers;
- invoke backend/gameplay mutation;
- partially replace SessionRoot;
- execute content/script code from save bytes.

## 31. Performance Measurement Hooks

TA-12 defines measurement points but not numeric budgets:

- stable-boundary capture time;
- snapshot bytes before/after encoding;
- per-section encode/decode/CRC time;
- storage write/flush time;
- catalog scan time;
- migration time;
- domain/cross-domain validation time;
- activation preparation/load transition time.

TA-13 assigns budgets and scaling targets.

## 32. Concurrency Handoff to TA-13

TA-12 permits worker execution only where immutable inputs/results preserve semantics.

TA-13 must later decide:

- worker count/priorities;
- whether section encode/CRC can parallelize;
- save snapshot memory budget;
- max queued operations;
- I/O throttling;
- catalog scan concurrency;
- load decode/migration parallelism.

Worker completion order must never change save bytes or loaded state.

## 33. CI Handoff to TA-14

TA-14 must turn TA-12 contracts into concrete CI gates for:

- current encode/decode round trips;
- exact v1 golden bytes;
- historical migration fixtures;
- corruption/fuzz cases;
- ownership/ID integrity;
- crash fault matrix;
- deterministic procedural continuation;
- profile persistence;
- save inspector parity;
- active-context serialization completeness.

## 34. Integration Audit Handoff to TA-15

TA-15 must verify persistence against every technical domain, particularly:

- active lease/export authority;
- station off-screen state;
- runtime entity continuation;
- AI knowledge/tactical continuation;
- mission/raid/event transactions;
- content migration;
- presentation reconstruction;
- concurrency/performance assumptions.

## 35. Implementation-Lock Handoff to TA-16

TA-16 will define:

- C++ target/source layout;
- exact storage platform adapter choice/API usage;
- serializer helper classes/functions;
- concrete schema version constants;
- exact initial migration support fixtures;
- test target names;
- implementation order and vertical-slice save/load milestone.

No TA-12 implementation code is authorized before that handoff.

## 36. TA-12 Closure Criteria

TA-12 can be `Architecture Complete` only when:

- save container bytes are exact;
- all baseline SectionKinds/owners are fixed;
- manual/quick/autosave storage semantics are fixed;
- crash commit/recovery behavior is fixed;
- load staging/validation/swap order is fixed;
- migration/content compatibility is fixed;
- profile settings persistence boundary is fixed;
- diagnostics/recovery tooling boundary is fixed;
- active runtime integration/resume semantics are fixed;
- cross-validation finds no GDS/TA contradiction or implementation-critical open question.

## 37. Explicit Non-Goals

TA-12 does not define:

- numeric performance/memory budgets;
- final worker-thread scheduling;
- CI workflow names/gates;
- C++ directory/target names;
- cloud synchronization;
- server authority;
- anti-cheat save signing;
- implementation code.

Those remain later-phase concerns where applicable.

## 38. Open Questions

None.
