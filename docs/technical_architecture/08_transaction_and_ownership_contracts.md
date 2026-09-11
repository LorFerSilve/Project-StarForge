# Transaction and Ownership Contracts

> **Status:** Architecture Complete  
> **Authority:** Domain-local and cross-domain atomic mutation, physical ownership transfer, transaction preparation/commit, deterministic ordering, idempotency, failure behavior, and Stable Save Boundary interaction

## 1. Purpose

The Design Complete GDS requires physical ownership, Credits, rewards, extraction, crafting, commissioning, raid theft, and story choices to commit exactly once without half-applied state.

This document defines the technical transaction model that enforces those rules.

## 2. Transaction Categories

The architecture distinguishes:

- **Domain-Local Transaction** — all authoritative mutations belong to one domain;
- **Cross-Domain Transaction** — two or more owning domains must mutate atomically;
- **Long-Running Operation** — persistent state that evolves across Simulation Time and is not kept as an open atomic transaction.

## 3. No Long-Lived Atomic Transactions

Atomic transactions execute entirely within one deterministic simulation commit window.

Operations lasting across ticks become persistent state such as:

- Work Order;
- Mission Instance;
- Travel state;
- Repair task;
- Reinforcement Call;
- Dynamic Event;
- Recovery Transit.

Their completion later creates a new atomic transaction.

## 4. TransactionId

Every authoritative transaction receives a non-zero persistent `TransactionId` from the session transaction allocator defined by TA-2 Identity.

A committed TransactionId is never reused within a save timeline.

## 5. Transaction Lifecycle

Canonical transaction states:

1. `Created`;
2. `Preparing`;
3. `Prepared`;
4. `Committing`;
5. `Committed`;
6. `Aborted`.

Only `Committed` changes authoritative gameplay state.

`Aborted` means no participant mutation became authoritative.

## 6. Coordinator Ownership

The `simulation` module owns `TransactionCoordinator` infrastructure.

It does not own domain semantics.

Each participating domain supplies a typed participant adapter that can:

- inspect current authoritative state;
- validate requested operation;
- prepare immutable mutation data;
- expose revision/precondition tokens;
- apply its already-prepared commit mutation.

## 7. Prepare Before Commit

Preparation may:

- validate IDs/references;
- verify capacity;
- verify balances/quantities;
- compute resulting values;
- reserve temporary memory;
- construct immutable prepared deltas;
- capture expected revisions.

Preparation must not publish authoritative mutation.

## 8. Commit Must Be Infallible Under Valid Preparation

The commit phase is designed to contain only deterministic in-memory state replacement/update that has already been validated and resource-prepared.

Commit must not perform:

- filesystem I/O;
- network I/O;
- asset loading;
- heap growth that was not prepared where allocation failure could matter;
- content lookup that can fail unexpectedly;
- user prompts;
- worker waits.

If a programmer invariant is violated during commit, the session enters a controlled fatal/invariant-failure path; the architecture does not pretend a partial commit is a normal gameplay outcome.

## 9. Final Revision Check

Immediately before commit, the coordinator verifies each participant's captured required StateRevision/precondition.

If any required revision changed since preparation:

- transaction aborts before mutation;
- typed result reports stale/conflict state;
- caller may revalidate/reissue.

## 10. Deterministic Commit Order

Participants are committed in a fixed project-owned `DomainCommitOrder`, not in container registration order.

Within one participant, prepared mutations use a stable sequence and typed ID tie-breakers.

The final gameplay state must not depend on:

- hash-map iteration;
- pointer address;
- worker completion;
- platform scheduler timing.

## 11. No Arbitrary Nested Cross-Domain Transaction

A transaction participant may call domain-local pure validation/preparation helpers.

It may not independently start and commit a second cross-domain transaction while the parent is Committing.

Required related mutations join the parent transaction during preparation.

## 12. Physical Ownership Model

Every physical item/resource quantity has one authoritative `OwnerRef`/inventory location at a time.

A physical transfer transaction conceptually performs:

`Source quantity decrease/remove`

and

`Destination quantity increase/create`

inside the same TransactionId.

There is no intermediate authoritative state where both own the same transferred quantity or neither owns it unless the GDS explicitly defines destruction/consumption.

## 13. OwnerRef

Physical ownership metadata uses a tagged owner reference capable of identifying valid owning containers such as:

- PlayerInventory;
- StationInventory/Storage;
- ShipCargo;
- RobotCargo;
- WeaponMagazine/InternalStore;
- WorldContainer;
- WorkInProgress;
- Mission/Extraction secured container where the GDS requires it.

Exact owner kinds are closed/typed and expanded only by approved domain contracts.

## 14. Stackable Resource Transfer

A stack transfer prepares:

- Resource/Item ContentId;
- source owner/slot/quantity;
- destination owner/slot/quantity;
- transfer amount;
- capacity constraints;
- source/destination revisions.

Commit applies both sides atomically.

## 15. Unique Item Transfer

A UniqueItemId has exactly one owner.

Transfer changes its owner reference atomically; it is not cloned into the destination.

## 16. Consumption

Consumption is an explicit transaction where source ownership decreases and no destination physical owner receives the consumed quantity.

The transaction Result/Event records the semantic sink, for example:

- ammunition fired;
- fuel burned;
- crafting material transformed;
- consumable used.

## 17. Transformation / Manufacturing

Manufacturing transforms reserved/input ownership into WorkInProgress and later output.

The architecture does not model transformation as:

`delete inputs now; maybe create output later`

without persistent WorkInProgress state.

At each irreversible boundary, physical ownership remains representable and saveable.

## 18. Credit Transactions

Credits are non-physical ledger state owned by Economy.

Credit mutations use integer smallest-unit representation defined by the economy/content contract; floating-point currency is prohibited.

A trade touching Credits + physical goods is a Cross-Domain Transaction.

## 19. Trade Atomicity

A normal immediate trade commits together:

- buyer Credit debit;
- seller/market Credit/liquidity update where applicable;
- physical goods source removal;
- physical goods destination insertion;
- market stock state.

If any required participant fails validation, none commit.

## 20. Capacity Validation

Transfers that require destination capacity validate capacity during preparation and recheck relevant revision before commit.

The transaction never commits source removal if destination acceptance is not guaranteed.

## 21. Extraction Transaction

Mission extraction is a Cross-Domain Transaction containing exactly the domains required by the mission outcome, for example:

- Mission state;
- player/ship inventory security state;
- robot/passenger extraction state;
- local-context/transition ownership where commit requires it.

`Field-Unsecured -> Vehicle/Extraction-Secured` is committed only once.

## 22. Raid Theft Transaction

Raid theft uses the same physical transfer contract as ordinary inventory movement.

Hostile ownership becomes final only according to GDS-11 extraction semantics.

The architecture does not maintain a separate percentage-based raid-loot ledger.

## 23. Robot Commissioning Transaction

Commissioning commits together as applicable:

- validated completed Work Order state;
- persistent RobotId allocation;
- RobotStore record creation;
- installed component ownership transfer;
- output location/ownership;
- initial charge/ammunition state if physically supplied.

Failure before commit creates no RobotId.

## 24. Reward Transaction

A one-time reward uses a TransactionId persisted by the owning reward/mission state where idempotency is required.

Commit includes:

- reward entitlement state;
- Credit/knowledge/reputation/physical delivery claim changes;
- one-time completion marker.

Reload cannot cause the same committed reward to fire twice.

## 25. Blueprint/Research Transaction

Permanent knowledge unlock commits atomically with its one-time completion/reward state.

The technical layer never represents a Blueprint unlock as an uncommitted UI notification.

## 26. Story Choice Transaction

Irreversible story choices commit all required narrative/world/faction/progression state under one TransactionId or one explicit coordinated transaction family.

The final choice cannot become committed in one domain and uncommitted in another.

## 27. Construction Transaction

Construction placement/deconstruction uses domain transaction semantics for:

- resource ownership/reservation;
- station structural record;
- occupancy/topology changes;
- resulting work order/blueprint state.

TA-6 defines station-specific graph update details while preserving this transaction boundary.

## 28. Damage and High-Frequency Mutation

Not every damage point requires a full cross-domain transaction object.

High-frequency domain-local combat mutations may commit through owning-domain deterministic operations.

A Cross-Domain Transaction is required only when the authoritative outcome must atomically touch multiple owners.

## 29. Transaction Result

Coordinator emits one typed final result:

- Committed;
- Aborted with typed reason;
- InvariantFailure technical path.

A transaction does not remain ambiguously `half successful`.

## 30. Event Emission

Committed participant events are accumulated during preparation/commit and become visible to event dispatch only after the entire transaction is Committed.

No consumer observes participant A's event before participant B has committed.

## 31. Stable Save Boundary

A Stable Save Boundary requires:

- no transaction in `Preparing/Prepared/Committing` that has begun the current atomic commit window;
- all transaction mutations for the current boundary either Committed or Aborted;
- persistent allocator/revision state consistent with committed records.

A queued save waits until this condition holds.

## 32. Save Representation of Transactions

The save does not serialize active short-lived prepared transaction objects.

Because save capture occurs only at a Stable Save Boundary, each short atomic transaction is already Committed or Aborted.

Persistent long-running operation state and required committed TransactionIds are serialized by their owning domain DTOs.

## 33. Idempotency

Idempotency is enforced through the combination of:

- TransactionId;
- owning persistent completion state;
- unique IDs;
- expected revisions;
- one-time reward/choice markers.

The coordinator does not keep an unbounded permanent global list of every historical transaction merely to achieve idempotency.

## 34. Duplicate Command Submission

If the same user action accidentally submits duplicate equivalent commands with different CommandIds, normal domain state/revision validation determines whether both are valid.

For operations that must be exactly-once even under duplicate submission, the caller/owning domain uses an explicit idempotency key or existing persistent operation identity.

## 35. Transaction Priorities

The architecture does not add a universal gameplay priority score to transactions.

Cross-domain phase order plus command sequence is sufficient unless an owning subsystem later has a GDS-defined arbitration rule.

## 36. Error Handling

Expected validation failure aborts cleanly.

Unexpected technical errors are handled before commit wherever possible.

Commit-phase invariant failure is treated as software integrity failure, logged with TransactionId and participant state, and must not continue pretending state is trustworthy.

## 37. Diagnostics

Development transaction trace records:

- TransactionId;
- originating CommandId;
- participating domains;
- prepare outcome;
- captured revisions;
- commit order;
- created/retired IDs;
- physical ownership deltas;
- emitted event types;
- final result.

Sensitive or enormous payloads can be summarized; identity and quantities remain visible.

## 38. Test Requirements

Tests must include:

- inventory transfer conserves quantity;
- unique item has one owner;
- insufficient capacity aborts both sides;
- stale revision aborts before mutation;
- trade cannot debit without item transfer;
- failed Robot commissioning creates no ID;
- reward cannot double-commit;
- extraction cannot duplicate secured loot;
- raid theft is recoverable before hostile extraction and final after valid commit;
- no event dispatch before whole transaction commit;
- save cannot capture Prepared/Committing state;
- deterministic participant order is platform/container independent.

## 39. Performance Constraints

Transaction machinery is mandatory for correctness boundaries, not every trivial scalar update.

Domain-local high-frequency operations remain lightweight.

Cross-domain transactions should avoid general-purpose database-like locking because authoritative mutation is single-threaded.

## 40. Explicit Non-Goals

The transaction system does not require:

- ACID database engine inside gameplay;
- distributed transactions;
- mutex locking between gameplay domains on the main thread;
- rollback of arbitrary side-effecting I/O;
- transactions spanning minutes of gameplay;
- event sourcing as persistence model.

## 41. Dependencies

This specification depends on TA-1 Runtime Model, TA-2 Identity/State/Commands/Serialization, and GDS physical ownership/save rules.

## 42. Open Questions

None in the TA-2 transaction/ownership contract.
