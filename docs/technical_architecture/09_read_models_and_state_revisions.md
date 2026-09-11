# Read Models and State Revisions

> **Status:** Architecture Complete  
> **Authority:** Immutable consumer-facing state, revision/generation tracking, presentation knowledge filtering, debug views, and stale-read handling

## 1. Purpose

Renderer, UI, audio, tools, and worker jobs need access to game state without receiving writable authority.

This document defines how authoritative state is projected into immutable read models and how consumers detect staleness.

## 2. Read Model Principle

A **Read Model** is an immutable consumer-specific projection derived from authoritative state at a defined stable simulation point.

A Read Model is not:

- a second gameplay authority;
- a mutable cache exposed to callers;
- a serialized replacement for the owning domain store unless an explicit save DTO says so.

## 3. Consumer-Specific Views

The architecture does not require one giant universal snapshot.

Typical read models include:

- RenderSnapshot;
- HudReadModel;
- InventoryReadModel;
- MissionReadModel;
- StationManagementReadModel;
- RobotSquadReadModel;
- NavigationReadModel;
- AudioSemanticState;
- DeveloperInspectionView.

Each contains only data required by its consumer.

## 4. State Revision

Every mutable authoritative domain exposes a monotonic `StateRevision`.

A Read Model records the relevant source revision(s) used to create it.

This supports:

- stale UI command rejection;
- job result validation;
- diagnostics;
- incremental rebuild.

## 5. ReadModelGeneration

Every published consumer read model receives a monotonic `ReadModelGeneration`.

This is process/session-local metadata and is not gameplay identity.

Consumers can detect that a newer read model superseded an older one.

## 6. Stable Publication Point

Gameplay/presentation read models are published only after a Stable Simulation Boundary or another explicitly safe phase-defined boundary.

They never expose half-committed cross-domain transactions.

## 7. Immutable Lifetime

Once published, a read model's contents never change.

A later update publishes a new generation.

Consumers may retain the old immutable object for a bounded period required for rendering/interpolation, but cannot mutate it.

## 8. Renderer Boundary

Renderer consumes RenderSnapshot data such as:

- transform;
- visual asset/material handles;
- light/camera state;
- visibility flags;
- animation/VFX presentation data.

It does not receive writable gameplay components.

## 9. Simulation Interpolation

Where rendering interpolates between two simulation states:

- both input snapshots are immutable;
- interpolation affects presentation only;
- interpolated transform is not fed back into authoritative simulation;
- non-interpolable state transitions use committed state rules.

## 10. UI Boundary

UI reads domain-specific Read Models and emits typed Commands.

Example:

1. Inventory UI displays ReadModel at revision 42;
2. player requests transfer;
3. Command carries relevant expected revision/precondition;
4. inventory changed to revision 43 before command commit;
5. command rejects/refreshes rather than applying against stale assumptions.

## 11. Presentation Knowledge Filter

Player-facing Read Models are created through a **Knowledge Filter** appropriate to the owning GDS presentation rules.

They contain only information the player legitimately knows.

Examples:

- unknown enemy subsystem omitted;
- suspected raid defense marked suspected rather than confirmed;
- undiscovered objective position omitted;
- unavailable strategic communication prevents current remote telemetry.

## 12. Debug Privilege

Development tools may request privileged inspection views that bypass player Knowledge Filter for diagnostics.

Such views:

- exist only in development/tooling compositions;
- are clearly marked privileged;
- cannot be used by shipping gameplay UI to gain authority/knowledge.

## 13. Read Model Builders

The owning gameplay domain defines the semantic projection rules for its data.

`ui`/`render` can define presentation-specific composition, but cannot reinterpret hidden gameplay state into new facts.

## 14. Cross-Domain Composite Read Models

A screen such as Mission Preparation may require data from multiple domains.

A composite Read Model:

- is assembled at a stable boundary;
- records source revisions;
- uses read-only domain queries/snapshots;
- never obtains cross-domain writable references.

## 15. Atomic Visibility

If a Cross-Domain Transaction changes inventory + Credits + mission state, a published Read Model generation sees either:

- all pre-transaction state; or
- all post-transaction state.

It never publishes a mixed partial state.

## 16. Incremental Rebuild

Read Model builders may reuse unchanged immutable substructures keyed by StateRevision for performance.

This is an optimization only.

Consumer semantics are identical to rebuilding from authoritative state.

## 17. Large Lists

Large management views may support paging/filtering/sorting.

Sorting intended for user presentation is presentation data and does not change authoritative processing order.

## 18. Commands From Sorted Views

UI commands target stable typed IDs, never visible list row numbers.

Reordering/filtering a list cannot cause an action to target a different entity.

## 19. Runtime Handle Exposure

Player-facing Read Models prefer persistent typed IDs for persistent entities.

RuntimeEntityHandle may be included only for strictly local rendering/tooling use where lifetime is bounded and the consumer never serializes it.

## 20. Content References

Read Models can contain ContentId or resolved lightweight content presentation handles.

They do not expose mutable content definitions.

## 21. Worker Snapshot Inputs

Worker jobs receive immutable, purpose-specific input snapshots.

Each carries:

- source generation/revision;
- relevant Persistent IDs/Content IDs;
- required geometry/data copy or immutable shared structure.

Worker does not retain mutable access to a DomainStore.

## 22. Worker Result Validation

When a worker result returns:

- main thread checks request ID;
- source revision/generation/ActivationEpoch is validated;
- stale result is discarded or explicitly recomputed.

No stale worker output silently overwrites newer state.

## 23. Event vs Read Model

Event answers:

> What committed just now?

Read Model answers:

> What state is currently visible to this consumer?

UI/audio may use both.

Reloaded sessions need not replay old ephemeral Events because current Read Models reconstruct current state.

## 24. Notification Persistence

If a notification must survive save/load, its authoritative notification/message record is part of domain/presentation persistent state.

The read model displays that record.

An ephemeral Event alone is not sufficient.

## 25. Read Model Error Handling

A builder encountering impossible authoritative state triggers invariant diagnostics.

It does not silently fabricate placeholder gameplay facts.

Missing purely cosmetic presentation assets are handled by later presentation/content fallback rules without mutating gameplay state.

## 26. Threading

Authoritative Read Model assembly is initiated from the simulation/main thread at stable boundaries.

Large pure transformation work may later be offloaded from immutable snapshot inputs, but publication occurs only after generation/staleness validation.

## 27. Persistence Boundary

Read Models are not serialized as canonical save state by default.

They are rebuilt after load from authoritative persistent state.

Exceptions require explicit owning specification and must never replace more authoritative domain data.

## 28. Diagnostics

Development tools can display:

- ReadModelGeneration;
- source StateRevisions;
- publication SimulationTick;
- consumer name;
- stale/rebuild counts;
- Knowledge Filter mode;
- worker snapshot generations.

## 29. Test Requirements

Tests must prove:

- published Read Model cannot observe partial transaction;
- stale expected revision rejects mutable command;
- list sorting cannot retarget ID-based actions;
- player Knowledge Filter hides unknown information;
- debug privileged view does not alter authoritative state;
- stale worker result is rejected;
- interpolation cannot mutate simulation;
- active/dormant data produce equivalent semantic views where expected.

## 30. Performance Constraints

Read Models should be consumer-specific and avoid unconditional whole-world copies each frame.

The architecture allows:

- structural sharing of immutable data;
- dirty/revision-based rebuild;
- sparse updates;
- double/triple buffering for render snapshots where later profiling requires it.

## 31. Explicit Non-Goals

Read models do not provide:

- writable gameplay state to UI/render/audio;
- one universal reflection dump;
- save authority;
- hidden player knowledge;
- row-index identity;
- worker-thread direct mutation.

## 32. Dependencies

This specification depends on TA-2 Domain State, Identity, Commands/Events, Transactions, TA-1 Stable Simulation Boundary, and GDS Presentation Knowledge Boundary.

## 33. Open Questions

None in the TA-2 read-model/revision contract.
