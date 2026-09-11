# Identity and Reference Model

> **Status:** Architecture Complete  
> **Authority:** Persistent identity representation, runtime-handle representation, content references, generic references, allocator state, identity lifetime, and reference validity

## 1. Purpose

Project StarForge contains entities that outlive local scenes, runtime activation, save/load, and individual process sessions.

This document defines exactly how those identities are represented and how they differ from temporary runtime handles.

## 2. Identity Classes

The architecture distinguishes four reference classes:

1. **Persistent Gameplay ID** — identity of a save-persistent gameplay object;
2. **Runtime Entity Handle** — process-local handle to an active runtime representation;
3. **Content ID** — immutable authored-definition reference;
4. **Ephemeral Operation ID** — command/job/read-model identifiers that need ordering/correlation but are not gameplay identity.

These classes are not interchangeable.

## 3. Persistent Gameplay ID Representation

Every persistent gameplay identity uses the conceptual type:

`PersistentId<Tag>`

with:

- one unsigned 64-bit integer payload;
- value `0` reserved as Invalid/Null;
- domain-specific tag type at compile time.

Examples:

- `CrewId`;
- `RobotId`;
- `ShipId`;
- `MissionId`;
- `RaidId`;
- `DefenseEventId`;
- `DynamicEventId`;
- `UniqueItemId`;
- `RewardTransactionId`;
- `StrategicLocationId` where runtime-created persistent location identity is required.

A `CrewId{17}` and `RobotId{17}` are different types and cannot be compared or substituted without an explicit conversion layer.

## 4. Persistent ID Allocation

Each persistent ID type has an allocator state owned by the persistent session state.

Allocator rule:

1. allocator stores `NextValue`;
2. allocation returns current non-zero `NextValue`;
3. `NextValue` increments exactly once in the same authoritative transaction that creates the persistent record;
4. allocator overflow is treated as a fatal invariant violation rather than wrapping/reusing an ID.

Persistent ID allocation never depends on:

- wall-clock time;
- memory address;
- process ID;
- frame timing;
- random collision avoidance.

## 5. Allocation Atomicity

An ID is not considered created merely because a candidate number was inspected.

The authoritative creation transaction commits both:

- allocator advancement; and
- new persistent record insertion.

If preparation/validation fails, no ID is consumed.

This preserves deterministic save/load behavior and prevents orphan allocated identities.

## 6. ID Reuse

Persistent IDs are never reused inside one save timeline after a committed identity existed.

Destroying a robot, resolving a mission, retiring a ship, or deleting a temporary item record does not return its ID to an allocator.

Loading an older save legitimately restores that older timeline and its older allocator state.

## 7. Runtime Entity Handle Representation

Active runtime entities use:

`RuntimeEntityHandle = { Index:uint32, Generation:uint32 }`

Conceptual semantics:

- `Index` selects a runtime registry slot;
- `Generation` invalidates stale references when a slot is recycled;
- one reserved invalid value represents Null.

Runtime handles are:

- process-local;
- Active-Local-Context-local in meaning;
- nonpersistent;
- never serialized as gameplay identity.

## 8. Runtime Handle Lifetime

A RuntimeEntityHandle is valid only while:

- its registry slot is occupied; and
- its generation matches.

Destroy/deactivation increments/replaces the slot generation before future reuse.

Holding an old handle after deactivation must fail validation rather than accidentally referring to another entity.

## 9. Persistent-to-Runtime Mapping

`world` owns a bidirectional active mapping where applicable:

`PersistentId<Tag> -> RuntimeEntityHandle`

and:

`RuntimeEntityHandle -> typed persistent identity/reference metadata`.

Not every runtime entity has a persistent ID.

Examples without persistent identity:

- transient projectile;
- short-lived VFX gameplay proxy;
- local debris with no persistence requirement.

Not every persistent identity has an active runtime handle.

Examples:

- off-screen crew member;
- robot stored at Horizon while player is elsewhere;
- resolved mission record;
- derelict ship at another Strategic Location.

## 10. No Identity From Asset Loading

Loading a mesh, prefab/content definition, audio asset, or physics shape never creates a Persistent Gameplay ID.

Persistent identity is created only by the owning gameplay transaction.

## 11. Content ID

Authored definitions use a separate `ContentId` concept.

Version-1 architecture represents Content IDs as canonical case-sensitive UTF-8 logical strings such as:

`weapon.ballistic.rifle_mk1`

or:

`station.module.life_support_basic`.

Rules:

- Content IDs are stable authored identifiers;
- they are not runtime object IDs;
- they are unique within the content registry;
- changing display name does not change ContentId;
- save files serialize ContentId as UTF-8 text in v1;
- TA-10 owns final content registry/file layout and validation.

## 12. Content ID Compatibility

A save can reference authored content by ContentId.

If a required ContentId no longer exists after a game update:

- migration must explicitly translate it; or
- load fails integrity validation with a diagnostic.

The loader does not silently substitute an arbitrary content definition.

## 13. Generic Persistent Reference

Cross-cutting systems that truly need heterogeneous references use a tagged value:

`PersistentRef = { Kind, RawId }`

where `Kind` is a closed project-owned enum of persistent identity categories.

Typed domain code should prefer `CrewId`, `RobotId`, `ShipId`, etc.

`PersistentRef` is reserved for infrastructure such as:

- diagnostics;
- generic ownership metadata;
- save manifest relations;
- transaction tracing;
- cross-domain notifications.

## 14. Strong, Optional, and Historical References

Persistent references are classified by schema as:

- **Strong Required** — referenced record must exist for state to be valid;
- **Optional** — explicit null is valid;
- **Historical** — target may no longer be active/available but identity remains meaningful for history/logging.

The reference class is part of the owning DTO/store schema and cannot be guessed during load.

## 15. No Raw Cross-Domain Pointers

Persistent gameplay domains do not retain writable raw pointers/references into another domain store.

Cross-domain relations use:

- typed persistent IDs;
- immutable read/query interfaces;
- commands;
- committed events.

This prevents invalidation and hidden authority sharing.

## 16. Persistent Record Tombstones

A domain may retain a compact tombstone/history record when the GDS requires historical identity after removal.

Examples:

- Destroyed Robot ID;
- completed unique Mission ID;
- retired Dynamic Event ID.

Tombstone use is domain-specific; the identity system does not require tombstones for every transient object.

## 17. Identity and Destruction

Runtime destruction and persistent identity deletion are separate operations.

Example:

1. robot chassis reaches Destroyed state;
2. `RobotDestroyed` transaction commits persistent robot outcome/tombstone;
3. runtime entity enters deferred destruction;
4. RuntimeEntityHandle becomes invalid at cleanup;
5. RobotId remains historically meaningful and is not reused.

## 18. Identity and Activation Epoch

Each persistent actor/object that can activate locally maintains an `ActivationEpoch` or equivalent monotonically increasing generation in activation metadata.

Worker jobs and delayed results can carry:

- Persistent ID;
- ActivationEpoch;
- relevant state revision.

A result produced for an older activation epoch is rejected if the entity has since deactivated/reactivated.

## 19. Ephemeral Operation IDs

Commands, jobs, and read models use separate monotonic operation identifiers such as:

- `CommandId`;
- `JobId`;
- `ReadModelGeneration`;
- `EventSequence`.

These are not gameplay entity identities.

Whether each is serialized depends on whether its owning operation survives save/load.

## 20. Transaction ID

Authoritative transactions use a project-wide `TransactionId` with a persistent 64-bit monotonic allocator stored in the session save state.

Reasons:

- idempotency for one-time rewards/commits;
- deterministic audit tracing;
- cross-domain correlation;
- save/load proof that a committed operation already occurred.

`TransactionId{0}` is invalid and IDs are never reused in a save timeline.

## 21. Short-Lived vs Persistent Transaction Records

Every authoritative transaction receives a TransactionId.

The complete transaction trace does not need to remain forever in gameplay state.

Owning domains persist the TransactionId only where later idempotency/history requires it, for example:

- one-time reward;
- irreversible story choice;
- unique commissioning/output;
- committed claim.

Development traces may retain a bounded diagnostics history separately.

## 22. Deterministic Ordering

Persistent ID numerical value may be used as a deterministic secondary ordering key.

Authoritative behavior must never depend on:

- memory address;
- hash-table iteration order;
- allocation address;
- worker completion order.

## 23. Serialization

Persistent IDs serialize as unsigned 64-bit values under their typed field/schema.

RuntimeEntityHandle never appears in a save DTO.

ContentId serializes as canonical UTF-8 string in save schema version 1.

## 24. Diagnostics

Development diagnostics print typed identity in qualified form, for example:

`RobotId(42)`

not only:

`42`.

Transaction/event logs likewise retain type qualification.

## 25. Test Requirements

TA-2 implementation tests must prove:

- null ID cannot be allocated;
- committed IDs are unique per type;
- failed creation does not consume ID;
- destroyed persistent IDs are not reused;
- runtime generation rejects stale handles;
- runtime handles do not serialize;
- required references fail validation when missing;
- optional references preserve null;
- save/load preserves allocator state;
- same snapshot produces same subsequent allocated IDs for the same command sequence.

## 26. Performance Constraints

ID lookup must support large persistent collections without requiring object-pointer coupling.

The architecture does not mandate one physical container implementation here; deterministic enumeration rules are defined in `06_domain_state_and_activation_contracts.md`.

## 27. Explicit Non-Goals

The identity architecture does not require:

- UUID strings for every entity;
- wall-clock-generated IDs;
- globally unique IDs across unrelated save files;
- serialized memory addresses;
- runtime handle stability across load;
- one universal untyped integer ID API;
- identity creation by asset loading.

## 28. Dependencies

This document depends on:

- GDS physical ownership and persistent IDs;
- GDS save/persistence rules;
- TA-1 runtime activation/deactivation;
- TA-2 domain state, transactions, and serialization.

## 29. Open Questions

None in the TA-2 identity/reference contract.
