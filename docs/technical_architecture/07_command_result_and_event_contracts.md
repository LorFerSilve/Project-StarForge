# Command, Result, and Event Contracts

> **Status:** Architecture Complete  
> **Authority:** Typed mutation requests, validation results, committed events, deterministic sequencing, correlation, delayed work, and producer/consumer boundaries

## 1. Purpose

StarForge has many systems that need to request changes or react to committed facts without sharing writable state.

This document defines the common contract shape for Commands, Results, and Events.

## 2. Core Rule

The three concepts are distinct:

- **Command** — a request/intention to change authoritative state;
- **Result** — the authoritative outcome of processing a specific command/transaction;
- **Event** — an immutable fact describing something that has already committed.

An Event is never a request.

A Command is never proof that something happened.

## 3. Typed Contracts

All gameplay commands, results, and events are strongly typed C++ contracts.

The baseline does not use:

- stringly typed event names;
- untyped property bags for core gameplay;
- a global `void*` message bus;
- reflection-dependent dynamic dispatch for normal domain behavior.

## 4. Command Envelope

Every authoritative command has conceptual metadata:

- `CommandId`;
- `CommandType` known statically by the handler;
- originating `SimulationTick`;
- source/issuer context;
- target IDs/data where relevant;
- immutable payload;
- optional parent/correlation `TransactionId`;
- optional expected StateRevision/precondition token.

Domain command structs own their exact payload fields.

## 5. CommandId

`CommandId` is a non-zero 64-bit monotonic operation ID scoped to the loaded session/process timeline.

It exists for:

- diagnostics;
- correlation;
- deterministic local ordering;
- duplicate-submission detection inside the live session where needed.

It is not a persistent gameplay identity and does not need to survive a normal save/load unless an explicitly persistent pending operation stores it as data.

## 6. Command Immutability

After enqueue, command payload is immutable.

A caller that changes its local UI/object state cannot modify a queued authoritative command.

## 7. Command Ownership

Every Command Type has exactly one owning handler/domain.

Examples:

- `TransferInventoryCommand` -> inventory/resource transaction owner;
- `CommissionRobotCommand` -> robot/manufacturing transaction path;
- `AcceptMissionCommand` -> mission domain;
- `SetRobotRoeCommand` -> robot domain;
- `InitiateStrategicTravelCommand` -> spacecraft/navigation domain.

Routing tables are explicit at composition/startup and not discovered through global string names.

## 8. Command Sources

Commands may originate from:

- player input/action mapping;
- UI;
- AI;
- automation;
- mission/raid state machine;
- recovery system;
- deterministic scheduled timer;
- development test/debug tooling.

Source does not grant permission to bypass normal validation.

## 9. Validation Result

Command validation returns one of the contractually allowed categories:

- `Accepted`;
- `Rejected` with typed reason;
- `Deferred`/`Blocked` only when the owning GDS/domain explicitly supports a non-final waiting state.

A rejected command produces no authoritative partial mutation.

## 10. Typed Rejection Reasons

Rejections use domain enums/data, not human-readable strings as authority.

Presentation converts typed reason to localized/readable output.

Examples conceptually:

- `InsufficientCapacity`;
- `MissingRequiredItem`;
- `InvalidTargetState`;
- `OutOfRange`;
- `PermissionDenied`;
- `StaleRevision`;
- `NoValidPath`.

## 11. Command Processing Order

Within a simulation phase, commands are processed by deterministic ordering.

Baseline ordering key:

1. owning phase/domain priority defined by runtime architecture;
2. originating SimulationTick;
3. command sequence/CommandId;
4. explicit domain tie-breaker if a command type requires one.

Queue/container insertion race from worker threads cannot define authoritative order.

## 12. Worker-Originated Requests

Worker threads do not enqueue authoritative commands by racing directly into the simulation mutation queue.

A worker returns an immutable `JobResult` carrying:

- JobId;
- source request identity;
- source state revision/generation;
- prepared data.

The main thread validates the result and, if still valid, creates/accepts the next authoritative command in deterministic order.

## 13. Result Contract

A processed Command yields a typed Result.

Result contains as applicable:

- CommandId;
- TransactionId;
- accepted/rejected/committed status;
- typed error/rejection reason;
- created Persistent IDs;
- committed quantities/values;
- resulting state version/revision;
- follow-up identifiers.

UI and tests consume Results rather than assuming success from button/input acceptance.

## 14. Result Timing

A synchronous same-tick command can return/queue its Result at the owning deterministic phase.

A long-running gameplay operation does not hold an open atomic transaction across many ticks.

Instead it creates persistent state such as:

- Work Order;
- Mission state;
- Repair task;
- Travel state;
- Strategic Timer.

Later completion is a new command/transaction with its own result.

## 15. Event Contract

A committed Event contains:

- statically known Event Type;
- `EventSequence`;
- commit `SimulationTick`;
- producing domain;
- originating TransactionId where applicable;
- typed payload containing only facts after commit.

## 16. EventSequence

`EventSequence` is a monotonically increasing 64-bit sequence assigned on the authoritative simulation thread when events are committed for dispatch.

It defines deterministic dispatch order inside the live session.

The ephemeral event stream itself is not the save-state authority.

## 17. Events Are Not Event Sourcing

StarForge does not reconstruct the complete save by replaying all historical events.

Persistent domain stores/DTOs remain authoritative save state.

Events are used for:

- cross-domain reactions;
- presentation;
- audio/VFX;
- diagnostics;
- creation of later commands.

If an event consequence must survive save/load, the resulting authoritative state is persisted by the owning domain.

## 18. Known Producer / Consumer Registration

Every cross-domain Event Type documents:

- producer(s);
- intended consumer(s);
- allowed reaction phase/timing.

Subscription wiring is explicit in the composition root or typed module setup.

There is no unconstrained global event listener that can mutate arbitrary state.

## 19. Event Payload References

Event payloads use:

- typed Persistent IDs;
- RuntimeEntityHandle only when event is strictly local/runtime and lifetime is guaranteed for the consumption window;
- value snapshots for quantities/positions/state when later lookup could become ambiguous.

Persistent gameplay events should prefer persistent identity.

## 20. Same-Tick Event Reactions

Default rule:

- committed events are dispatched after the Cross-Domain Transaction Commit phase;
- consumers may update presentation/read caches immediately;
- gameplay reactions normally enqueue a Command for the next applicable deterministic phase/tick.

Same-tick authoritative follow-up is permitted only when explicitly registered as a bounded internal reaction in the owning architecture.

## 21. No Recursive Event Mutation

An event handler cannot recursively perform arbitrary cross-domain mutation while dispatching an event.

This prevents:

- unbounded loops;
- commit-order ambiguity;
- half-visible state.

## 22. Domain-Local Immediate Facts

Inside one owning domain operation, local helper calls do not need to become global Events.

Events exist at meaningful module/domain boundaries, not for every internal function call.

## 23. Event Coalescing

Presentation-only high-frequency events may be coalesced where semantic meaning is preserved.

Example:

- multiple tiny damage visual cues in one frame.

Authoritative events that drive gameplay transactions are not dropped/coalesced unless their contract explicitly defines aggregation.

## 24. Persistent Notifications

A player notification that must still exist after save/load is not implemented merely as an ephemeral Event.

The owning gameplay/presentation-notification domain stores a persistent notification/message record, and an Event may announce its creation.

## 25. Command Idempotency

Commands that can be retried because of UI/network-like asynchronous boundaries inside the process must define idempotency behavior.

Baseline single-player UI normally submits once, but persistence-critical operations use TransactionId/owning state to prevent duplicate commit.

## 26. Transaction Correlation

For cross-domain mutation:

- originating Command has a CommandId;
- coordinator allocates/uses TransactionId;
- prepared participant operations reference TransactionId;
- final Result references both where useful;
- committed Events reference TransactionId.

This provides one trace across domains.

## 27. StateRevision Preconditions

A command prepared from an old read model can include an expected revision/version.

If the relevant state changed before commit:

- validation rejects with `StaleRevision` or equivalent;
- caller refreshes and reissues if appropriate.

The engine does not silently apply the command to semantically different state.

## 28. Presentation Knowledge Boundary

UI-facing command results/events contain only information that the player is allowed to know.

Internal diagnostics can consume privileged events/read models in development builds.

A rejected command cannot leak hidden enemy/world data through an overly specific error reason.

## 29. Error Separation

Architecture distinguishes:

- expected gameplay rejection;
- recoverable technical error;
- invariant violation.

Gameplay rejection is normal typed Result data.

Technical/invariant failures are diagnostics/runtime error paths and are not presented as fictional gameplay outcomes.

## 30. Serialization Boundary

Ephemeral Commands and Events are not serialized by default.

Long-lived operations are represented by persistent domain state.

If a pending operation truly must survive save/load, its owning persistent DTO stores the minimal semantic state needed to resume; it does not serialize function pointers, queues, callbacks, or RuntimeEntityHandles.

## 31. Diagnostics

Development tools provide a bounded trace containing:

- CommandId/type/source;
- validation outcome;
- TransactionId;
- committed EventSequence/type;
- affected typed IDs;
- store revisions;
- SimulationTick.

Trace retention is debug data and not gameplay authority.

## 32. Test Requirements

Tests must prove:

- rejected command causes no mutation;
- identical command sequence yields deterministic ordering;
- typed rejection reasons map correctly;
- committed event occurs only after commit;
- event consumer cannot veto committed fact;
- stale revision command rejects;
- worker completion order cannot reorder authoritative command outcome;
- recursive event mutation is blocked by API structure;
- long-running operations persist as domain state, not open transactions.

## 33. Explicit Non-Goals

This contract does not provide:

- global untyped event bus;
- event-sourced save reconstruction;
- open transactions spanning arbitrary gameplay time;
- command mutation after enqueue;
- worker-thread gameplay commits;
- hidden callbacks with cross-domain write access.

## 34. Dependencies

This specification depends on TA-1 Runtime/Simulation, TA-2 Identity, Domain State, Transactions, Read Models, and GDS Presentation Knowledge rules.

## 35. Open Questions

None in the TA-2 command/result/event contract.
