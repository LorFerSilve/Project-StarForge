# TA-12 — Persistence Service and Snapshot Orchestration

> **Status:** Architecture Complete  
> **Authority:** Persistence service ownership, save requests, Stable Save Boundary capture, immutable snapshots, save-operation lifecycle, queue/coalescing rules, thread boundaries, and session-replacement coordination

## 1. Purpose

TA-2 established what a valid save snapshot means. TA-12 now defines the concrete runtime service that requests, captures, writes, loads, validates, migrates, and activates persistence state without becoming gameplay authority.

The persistence implementation must preserve three invariants:

1. gameplay mutation remains owned by gameplay domains;
2. save capture occurs only at a Stable Save Boundary;
3. file I/O and migration never partially mutate the live session.

## 2. PersistenceService Ownership

`PersistenceService` is a project-owned application service composed by `app`.

It owns:

- save/load operation state;
- save-request queue/coalescing;
- snapshot-capture orchestration;
- domain serializer registry access;
- storage backend access;
- file/container encode/decode orchestration;
- migration pipeline orchestration;
- integrity-validation orchestration;
- save catalog/index rebuild;
- persistence diagnostics.

It does **not** own semantic gameplay fields inside Player, Station, Inventory, Mission, Robot, Economy, or other domain stores.

## 3. Persistence Threading Boundary

The authoritative main simulation thread owns:

- deciding when a Stable Save Boundary exists;
- freezing the logical capture view for one boundary;
- asking each domain exporter for immutable Save DTO data;
- final consistency validation required before releasing capture;
- atomic live-session replacement after successful load staging.

Worker threads may own:

- binary encoding/decoding;
- CRC32C calculation;
- file reads/writes;
- immutable migration steps that are declared worker-safe;
- save catalog metadata extraction;
- diagnostics generation.

Workers never mutate live DomainStores.

## 4. Save Request Types

Canonical request kinds are:

- `ManualSaveRequest`;
- `QuickSaveRequest`;
- `AutosaveRequest`.

Every request contains at minimum:

- request sequence ID;
- save kind;
- logical slot target where applicable;
- optional user display label;
- request reason/trigger;
- originating Presentation Time timestamp for diagnostics only;
- current session identity token;
- request status.

The request does not contain writable gameplay references.

## 5. Save Request State Machine

Canonical state machine:

```text
Requested
  -> WaitingForStableBoundary
  -> CapturingSnapshot
  -> SnapshotCaptured
  -> Encoding
  -> WritingPendingFile
  -> ValidatingPendingFile
  -> CommittingFile
  -> Committed
```

Failure can move any post-request state to:

`Failed(reason)`.

Cancellation is permitted only before snapshot capture begins unless the operation explicitly supports safe cancellation of background work. A cancelled operation never deletes a previously committed save.

## 6. Stable Save Boundary Handshake

When a save request is pending, simulation does not stop at an arbitrary mid-tick point.

At the next TA-1/TA-7 Stable Simulation Boundary:

1. persistence receives the current authoritative `SimulationTick`;
2. all cross-domain transactions for the tick are settled;
3. deferred gameplay ownership/lifecycle consequences required for persistence are settled;
4. active Activation Leases expose save-export facets;
5. each registered required domain exporter produces immutable DTO data;
6. snapshot-level invariants run;
7. an immutable `SaveSnapshot` is published to the persistence worker;
8. normal simulation can continue.

No background file work holds the simulation paused after immutable capture is complete.

## 7. Snapshot Capture Is Not Session Cloning

`SaveSnapshot` is a purpose-built immutable persistence representation.

It is not:

- a raw clone of `GameState`;
- a retained set of mutable DomainStore references;
- a runtime entity-registry memory dump;
- a physics/render/audio snapshot.

Domains can allocate/copy only fields required by their Save DTO contracts.

## 8. SaveSnapshot Root

Conceptually:

```text
SaveSnapshot {
  SimulationTick saved_tick;
  SnapshotSequence snapshot_sequence;
  SaveKind save_kind;
  LogicalSaveSlotId logical_slot_id;
  ContentBuildId content_build_id;
  ApplicationSaveCompatibilityVersion app_compat_version;
  DomainSnapshotSection sections[];
  SavePresentationMetadata metadata;
}
```

`SnapshotSequence`, `SaveKind`, and `LogicalSaveSlotId` are persistence-storage metadata, not gameplay progression state.

## 9. SnapshotSequence

Each campaign storage root maintains a monotonically increasing non-zero `SnapshotSequence:uint64`.

It is allocated when snapshot capture begins and recorded in the committed file header.

Rules:

- sequence values are never reused within one campaign storage root;
- a failed save may consume a sequence value;
- sequence order is storage/history order only;
- gameplay simulation never branches on SnapshotSequence.

The next sequence is recovered as `max(valid committed and pending historical sequence) + 1`, with overflow treated as a persistence-fatal condition.

## 10. LogicalSaveSlotId

Manual save slots have a persistence-only non-zero `LogicalSaveSlotId:uint64`.

Quick Save and Autosave use reserved logical slot semantics defined in `93_save_catalog_slots_manual_quick_and_autosave.md`.

A logical slot identifies replacement/history intent, while every committed file generation remains individually immutable until cleanup.

## 11. Domain Export Registration

Every required persistent domain registers one serializer/exporter descriptor during application composition.

Descriptor contains:

- stable `SectionKind`;
- current `SchemaVersion`;
- required/optional classification;
- export function;
- decode/import function;
- migration chain;
- domain-local validator;
- cross-domain validation dependencies.

Registration is static/project-owned; save files cannot inject executable serializers.

## 12. Capture Ordering

Capture iterates serializers in stable ascending `SectionKind` order.

Ordering must not depend on:

- static initialization order;
- map/hash iteration;
- worker timing;
- filesystem order.

A duplicate SectionKind registration is a startup/test failure.

## 13. Active Lease Export

When a persistent record is Active-Leased, the owning domain exporter obtains its currently authoritative leased facets through the TA-2/TA-7 save-export contract.

The resulting DTO contains one coherent logical record.

The save process does not deactivate actors solely to serialize them.

## 14. Transient Local Continuation

Gameplay-relevant transient local state that must survive save/load but is not a long-lived persistent actor is exported through the owning active-context continuation contract.

Examples can include:

- mission-local transient hostile state;
- consequential projectiles;
- active Status instances;
- tactical awareness/search state;
- local interaction progress where persistence rules require it.

Such state is serialized only because it affects consequential next state; decorative particles, UI focus, audio voices, camera blends, and backend handles are excluded.

## 15. Snapshot Consistency Gate

Before releasing snapshot capture, persistence verifies at minimum:

- exactly one section per registered singleton SectionKind;
- every Required section is present;
- saved tick is identical across snapshot-level exporters that expose it;
- persistent ID allocator state is coherent with exported records;
- active-context identity exists;
- no exporter reports an unresolved lease or half-transaction;
- snapshot does not contain runtime handles/backend objects.

Domain semantic validation remains owned by each domain.

## 16. Save Request Coalescing

Persistence avoids an unbounded queue of redundant save requests.

Rules:

- a Manual Save request is never silently replaced by an Autosave request;
- multiple periodic Autosave requests waiting before capture may coalesce to the newest equivalent periodic request;
- milestone Autosave requests with distinct semantic reason may be represented by one resulting snapshot if they become due before the same Stable Save Boundary, while all triggers are recorded in diagnostics/metadata;
- repeated Quick Save presses before capture may coalesce to one newest Quick Save request;
- a save request arriving after snapshot capture begins belongs to a later snapshot and cannot mutate the already captured one.

## 17. One Write Operation Per Logical Target

Two concurrent writes cannot race for the same logical slot/history class.

The service serializes commit operations per campaign storage root while still allowing CPU encoding/checksum work on immutable snapshots where TA-13 later permits concurrency.

The initial implementation can conservatively allow one active save write per campaign.

## 18. Load Request State Machine

Canonical load states:

```text
Requested
 -> ReadingContainer
 -> ContainerValidated
 -> Decoding
 -> Migrating
 -> ImportingToStaging
 -> ValidatingStaging
 -> PreparingActiveContext
 -> ReadyToActivate
 -> ReplacingSession
 -> Activated
```

Any pre-activation failure discards staging and leaves the current live session unchanged.

## 19. Load During Existing Session

Loading does not merge with the current session.

Before final replacement:

- current simulation is placed under the existing loading/True-Pause application transition policy;
- staging completes independently;
- the old session remains intact until the replacement commit;
- old backend/runtime resources are torn down only after the new staged session is accepted or through a controlled transition that can still fail without corrupting gameplay state.

## 20. Load Replacement Boundary

The final live-session replacement is one application-level atomic ownership switch:

```text
old SessionRoot
        |
        +-- remains current until staging PASS

staged SessionRoot -> validation PASS -> app swaps root -> activate local runtime
```

No domain is imported directly into the old root.

## 21. Save During Load / Load During Save

A load request never begins session replacement while a snapshot capture is in progress.

A completed immutable save worker may finish file I/O while a later load is being prepared, but:

- its snapshot remains tied to the session token captured at request time;
- completion cannot mutate the newly loaded session;
- UI result is associated with the original save operation ID.

A new save request is not accepted for a staging session until that session becomes live.

## 22. New Game Initialization

New game creation constructs a valid initial SessionRoot through the same domain invariants expected after load.

Initial SaveSeed, persistent allocators, content references, difficulty/gameplay settings, and required world records are established before the first save can commit.

New-game initialization does not use a fake save/load round-trip as gameplay authority.

## 23. UI Integration

TA-11 UI consumes an immutable `PersistenceReadModel` containing:

- active operation state;
- save kind/slot;
- queued/waiting status;
- current load stage;
- typed failure code and safe user message;
- last committed save metadata;
- catalog revision.

UI cannot mark a save as complete before `Committed`.

## 24. Save Feedback Semantics

Canonical presentation states include:

- Save Requested;
- Waiting for Stable Save Boundary;
- Saving;
- Save Complete;
- Save Failed.

`Save Complete` is emitted only after the new committed file generation is durable according to the storage contract and passes post-write validation.

## 25. True Pause

A save request does not automatically invoke True Pause.

Snapshot capture occurs at a stable boundary and is intended to be short.

Loading uses the application/loading transition and does not advance Simulation Time.

## 26. Headless Operation

Persistence service logic can run without OpenGL, audio, GLFW window creation, or shipping UI.

Tests can provide:

- in-memory storage backend;
- fault-injecting storage backend;
- synthetic domain serializer registry;
- deterministic content registry fixture.

## 27. Error Classification

Persistence errors are typed into at least:

- Request/State error;
- Storage I/O error;
- Container corruption;
- Unsupported container/application version;
- Section/schema incompatibility;
- Migration failure;
- Content compatibility failure;
- Domain invariant failure;
- Cross-domain integrity failure;
- Activation preparation failure;
- Internal persistence invariant failure.

User-facing wording is TA-11 presentation; diagnostics retain precise codes/context.

## 28. No Hidden Retry That Changes Semantics

Storage operations may retry an idempotent low-level read/write only under a documented backend policy.

Persistence never retries by:

- capturing a different gameplay snapshot without a new save operation;
- dropping a Required section;
- regenerating procedural content;
- allocating substitute Persistent IDs;
- resetting timers/RNG.

## 29. Diagnostics

Persistence diagnostics include:

- operation ID;
- session token;
- save/load kind;
- logical slot;
- SnapshotSequence;
- requested/captured SimulationTick;
- capture duration;
- section count/schema versions;
- encoded sizes;
- validation stage;
- migration steps;
- storage path identifier;
- commit/recovery result.

## 30. Test Requirements

Tests must cover:

- save request queued through a transaction then captured at next stable boundary;
- active-lease export without deactivation;
- autosave/quick-save coalescing;
- manual request not lost behind autosave;
- immutable snapshot unaffected by later simulation changes;
- background save completion cannot mutate a newly loaded session;
- staging load failure leaves live session unchanged;
- successful load performs exactly one SessionRoot replacement;
- duplicate serializer registration fails;
- required exporter missing fails snapshot capture.

## 31. Explicit Non-Goals

No baseline requirement for:

- cloud synchronization;
- server-authoritative saves;
- incremental/delta saves;
- copy-on-write cloning of all DomainStores;
- save-state mutation from file workers;
- checkpoint-only saving;
- synchronous whole-file writing while simulation is blocked;
- arbitrary best-effort partial loads.

## 32. Dependencies

Depends on TA-1 Stable Simulation Boundaries, TA-2 domain stores/leases/save DTOs/transactions, TA-7 runtime synchronization, TA-9 persistent mission/event state, TA-11 save/load presentation, and GDS Save/Persistence.

## 33. Open Questions

None.
