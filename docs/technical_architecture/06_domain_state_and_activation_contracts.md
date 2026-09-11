# Domain State and Activation Contracts

> **Status:** Architecture Complete  
> **Authority:** Persistent domain-store ownership, record lifecycle, deterministic enumeration, activation authority transfer, state revisions, dormant/active synchronization, and session composition

## 1. Purpose

Project StarForge contains both long-lived strategic state and high-frequency active local entities.

This document defines how authoritative domain state is stored and how state transitions safely between persistent domain representation and active runtime representation.

## 2. Session State Root

One loaded game session owns a `GameState`/equivalent aggregate created by the `app` composition root.

It contains explicit domain stores rather than one universal mutable object registry.

Conceptual stores include:

- PlayerStore;
- StationStore;
- CrewStore;
- Resource/InventoryStore;
- SpacecraftStore;
- MissionStore;
- RobotStore;
- Raid/DefenseStore;
- WorldState/FactionStore;
- EconomyStore;
- Progression/KnowledgeStore;
- DynamicEventStore;
- Recovery/Communication state;
- allocator/transaction/timer infrastructure state.

Exact C++ target/type layout can split or combine closely coupled stores where ownership remains equivalent.

## 3. Domain Ownership Rule

Every authoritative gameplay field has exactly one owning domain/store.

No field is simultaneously mutable authority in two stores.

Examples:

- Credits ledger -> Economy owner;
- Robot structural state -> Robot owner;
- Ship module state -> Spacecraft owner;
- physical item ownership -> Inventory/Resource authority;
- Mission objective state -> Mission owner;
- faction reputation -> Faction/Reputation owner.

Cross-domain derived values are recomputed/read through contracts instead of becoming duplicate authorities.

## 4. Store API Boundary

A domain store exposes three conceptual surfaces:

1. read-only query/view API;
2. command/transaction participant API;
3. serialization/export/import API.

Direct arbitrary mutable access from another domain is prohibited.

## 5. Store Lifetime

Domain stores are created:

- when a new session is created; or
- while loading a save into staging state.

They live for the duration of the loaded session and are destroyed on session replacement/shutdown.

They are not hidden process-global singletons.

## 6. Persistent Record Lifecycle

A persistent record can be:

- Created;
- Existing/Dormant;
- Active-Leased;
- Retired/Tombstoned where applicable;
- Removed only where the owning GDS permits true erasure.

The exact gameplay state names remain domain-specific.

## 7. Deterministic Enumeration

Authoritative domain processing must use deterministic enumeration.

If underlying storage is hash-based, the store must expose an explicitly ordered view before behavior depends on iteration.

Default deterministic key:

1. primary owning schedule/order key where defined;
2. persistent ID raw value as tie-breaker.

Hash-container bucket order is never gameplay authority.

## 8. StateRevision

Every mutable domain store maintains a monotonically increasing `StateRevision`.

A record that is independently targeted by asynchronous jobs/optimistic validation also maintains a record-level revision or equivalent generation where required.

A successful authoritative commit that changes a store increments its relevant revision exactly once per committed transaction participation.

## 9. Revision Uses

Revisions support:

- stale worker-result rejection;
- prepared-transaction preconditions;
- read-model versioning;
- diagnostics;
- activation/deactivation validation.

Revisions are not player-facing progression values.

## 10. Persistent vs Active State Problem

Active entities need high-frequency state without duplicating authority inside a dormant persistent record.

The architecture therefore uses an explicit **Activation Lease**.

## 11. Activation Lease

When a persistent object becomes active, the owning domain grants the Active Local Context an `ActivationLease` for the fields designated as high-frequency active state.

The lease contains at minimum:

- typed Persistent ID;
- ActivationEpoch;
- source record revision;
- list/schema of leased state facets;
- RuntimeEntityHandle after activation.

## 12. Authority During Activation

While a facet is leased:

- the active runtime facet is authoritative for that high-frequency field;
- the persistent store retains identity, invariant metadata, and non-leased fields;
- off-screen logic may not independently mutate the leased field;
- cross-domain commands targeting that field route to the active owner/facet through the owning domain boundary.

There is never a dormant copy and active copy that can both change independently.

## 13. Examples of Leased Facets

Depending on later TA phases, leased facets can include:

- local transform/velocity;
- current active combat Health/shield state;
- active robot locomotion state;
- active ship rigid-body state;
- current local weapon/projectile state;
- detailed AI tactical state.

Persistent strategic fields such as identity, long-term assignments, unlocked Blueprints, or mission history remain store-owned.

## 14. Activation Sequence

Canonical activation sequence:

1. resolve Persistent ID and ContentId;
2. validate record is eligible for activation;
3. increment/allocate new ActivationEpoch;
4. prepare activation snapshot/facets;
5. allocate RuntimeEntityHandle;
6. create runtime components;
7. create physics/render/audio proxies where needed;
8. register PersistentId <-> RuntimeEntityHandle mapping;
9. commit ActivationLease;
10. runtime behavior begins at next valid simulation phase.

If preparation fails before lease commit, the persistent record remains Dormant and authoritative.

## 15. No Half-Activated State

The transition to Active-Leased is atomic at a Stable Simulation Boundary.

A failed activation cannot leave:

- allocated persistent identity duplicated;
- domain record marked active without a runtime handle;
- runtime entity active without an owning persistent record where persistence is required.

## 16. Deactivation Sequence

Canonical deactivation:

1. mark runtime entity Closing; stop accepting new local commands that require continued presence;
2. resolve/abort local in-flight operations under owning rules;
3. prepare returned persistent facet state;
4. validate lease Persistent ID + ActivationEpoch;
5. atomically return leased fields to owning domain record;
6. increment relevant persistent revisions;
7. remove active mapping;
8. destroy physics/render/audio proxies;
9. reclaim runtime components/handle during deferred cleanup;
10. record returns to Dormant or its committed retired state.

## 17. Save Capture While Active

Save capture does not require forcibly deactivating every active entity.

At a Stable Save Boundary:

- dormant persistent records export directly from domain stores;
- active-leased entities export their current authoritative leased facets from the active runtime snapshot;
- the persistence exporter combines them into one versioned domain DTO without mutating gameplay merely to save.

Thus the save contains one coherent logical record for each persistent identity.

## 18. Off-Screen Horizon Rule

When Horizon is not the Active Local Context:

- high-detail leases are returned/deactivated unless a specific simultaneous event requires active local detail;
- authoritative station/crew/robot/resource state remains in domain stores;
- lower-frequency strategic simulation mutates those stores through normal domain commands/transactions.

No alternate simplified save model is created.

## 19. Simultaneous Horizon Event

A Horizon Defense Event can run while another local context is active.

This does not imply two full player-local runtime worlds.

The defense architecture may create bounded tactical/off-screen working state later, but it must source and commit through the same persistent stores and revisions.

## 20. Read Consistency

Cross-domain read-only queries execute against a stable phase-defined view.

A domain must not expose an iterator/reference that remains writable across a later transaction commit.

Long-lived consumers use immutable read models rather than borrowed mutable store references.

## 21. Mutation Entry Points

Authoritative domain mutation occurs only through:

- owning-domain simulation update explicitly scheduled for the current phase;
- accepted command;
- transaction participant commit;
- deterministic load/migration staging before session activation.

UI, renderer, audio, worker jobs, and debug widgets never write stores directly.

## 22. Domain-Local Transactions

A domain may execute a local atomic operation without invoking the cross-domain coordinator when every authoritative mutation belongs to that one domain.

It still uses:

- validation;
- prepared state;
- atomic commit;
- revision increment;
- typed result/event.

## 23. Cross-Domain Transactions

If authoritative fields in two or more owning domains must change atomically, `simulation` transaction coordination is mandatory.

No domain obtains temporary writable ownership of another store merely to simplify code.

## 24. Derived Caches

A domain can maintain non-authoritative caches/indexes for performance.

Examples:

- ID lookup index;
- sorted work queue;
- spatial summary;
- aggregate capacity cache.

Rules:

- cache is reconstructible from authoritative state;
- cache never serializes as sole truth;
- cache invalidation follows revisions;
- cache corruption cannot legally alter gameplay outcome.

## 25. Referential Integrity

Stores declare required/optional/historical references according to `05_identity_and_reference_model.md`.

After load/migration and before session activation, cross-store references are validated in staging.

Missing Strong Required references fail load/invariant validation.

## 26. Deletion and Tombstones

True record deletion is allowed only when the owning domain no longer requires identity/history and no Strong/Historical reference remains.

Where history matters, the store retains a tombstone/retired record with enough data to satisfy the contract.

## 27. Session Replacement on Load

Loading a save constructs a completely separate staging `GameState`.

The currently running session is not mutated piece by piece.

Only after:

- all domain imports;
- migrations;
- reference validation;
- invariants;
- required content resolution

pass does `app` atomically replace the current session root.

## 28. Error Handling

Expected command/state errors return typed domain errors.

Invariant violations indicate programmer/data corruption and do not trigger heuristic state repair during live gameplay.

Load-time repair is allowed only through an explicit versioned migration/recovery rule.

## 29. Diagnostics

Development tools can inspect:

- store revision;
- record ID/state;
- activation status;
- ActivationEpoch;
- RuntimeEntityHandle;
- leased facets;
- outstanding prepared transactions;
- reference graph.

Diagnostics remain read-only unless using explicit debug commands that pass normal mutation boundaries.

## 30. Test Requirements

Tests must cover:

- deterministic store enumeration;
- active/dormant authority exclusivity;
- activation failure rollback;
- stale ActivationEpoch rejection;
- deactivation returning latest active state;
- save export of active and dormant records;
- off-screen mutation after deactivation;
- cross-domain pointer/reference prohibition at API boundaries;
- revision increment behavior;
- load staging not touching live session on failure.

## 31. Performance Constraints

The design permits storage optimization later, but domain boundaries must not require copying entire stores every 60 Hz tick.

Read models/snapshots are selective and generated for actual consumers.

## 32. Explicit Non-Goals

This architecture does not require:

- one universal ECS for persistent state;
- every persistent entity active simultaneously;
- direct mutable cross-store references;
- store serialization by raw memory dump;
- full domain-store cloning every tick;
- runtime entity handles as persistent identity.

## 33. Dependencies

This specification depends on:

- TA-1 System Context and Runtime Model;
- TA-2 Identity;
- TA-2 Transactions;
- TA-2 Serialization;
- GDS persistent/off-screen behavior.

## 34. Open Questions

None in the TA-2 domain-state/activation contract.
