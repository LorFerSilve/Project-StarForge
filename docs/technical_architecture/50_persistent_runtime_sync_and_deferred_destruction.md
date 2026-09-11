# TA-7 — Persistent/Runtime Synchronization and Deferred Destruction

> **Status:** Architecture Complete
> **Authority:** Runtime-to-persistent lease synchronization, active save export, logical removal, closing/pending-destroy semantics, and backend/component reclamation

## 1. Purpose

TA-7 must permit high-frequency local mutation without creating two authoritative copies of the same persistent state or destroying runtime objects while systems still reference them.

## 2. Synchronization Rule

For every persistent facet there is exactly one current mutable authority:

```text
Dormant persistent record
OR
Active leased runtime facet
```

never both.

## 3. Activation Epoch

Every persistent activation uses the TA-2 `ActivationEpoch`.

Runtime-to-persistent return/export is accepted only when:

- persistent ID matches;
- ActivationEpoch matches current lease;
- scene generation is valid;
- required source revision/preconditions still hold.

Stale entities cannot overwrite newer persistent state.

## 4. Runtime Commit to Persistent Domain

Cross-domain consequences generated while active are not accumulated indefinitely in components when the owning domain requires an immediate committed result.

Examples:

- inventory pickup/consumption;
- mission objective result;
- faction consequence;
- station structural damage;
- world-object ownership transfer.

These use normal TA-2 transactions immediately. Only explicitly leased high-frequency facets remain runtime-authoritative until deactivation/save export.

## 5. Active Save Export

At a Stable Save Boundary the exporter asks each active lease adapter for an immutable export snapshot.

The owning persistent domain builds its Save DTO from:

- dormant record fields;
- currently leased runtime facets;
- committed cross-domain state already present in stores.

The live lease remains active after save capture.

## 6. Runtime Revision

Runtime components that receive asynchronous/optimistic results maintain appropriate local revision/generation tokens. Worker results must validate entity generation, scene generation and component/source revision before application.

## 7. Logical Removal Before Physical Reclamation

Runtime destruction is two-stage:

```text
Gameplay outcome commits
        ↓
Logical removal / Closing
        ↓
PendingDestroy
        ↓
Safe structural cleanup
        ↓
Reclaimed slot
```

This prevents callbacks/iteration from observing freed storage mid-phase.

## 8. Closing Semantics

A Closing entity:

- remains resolvable for settlement/read-only consequence processing;
- rejects ordinary new actions that require active normal presence;
- can still be referenced by already-created committed facts until their owning phase finishes;
- does not immediately lose persistent identity mapping if deactivation return is still pending.

## 9. PendingDestroy Semantics

`PendingDestroy` means semantic removal is settled and only technical cleanup remains.

At this stage:

- no new gameplay commands target the entity;
- systems skip it for normal simulation queries;
- deferred backend cleanup is queued;
- component memory remains valid until cleanup boundary where required by phase contracts.

## 10. Cleanup Order

Canonical cleanup order for a persistent-backed entity:

1. settle/return leased state where required;
2. remove persistent/runtime active mapping;
3. detach gameplay query registrations;
4. queue/remove TA-5 physics proxy at safe physics mutation phase;
5. retire TA-4 render/audio proxies through their lifecycle owners;
6. remove runtime components in dependency-safe order;
7. invalidate entity slot by generation increment/reclamation.

Transient entities omit persistent return/mapping steps.

## 11. No Immediate Destroy in Callbacks

Physics contacts, combat hit resolution, interaction callbacks, AI notifications and render/audio callbacks cannot directly free an entity/component.

They may only emit a typed destroy/close request into the appropriate structural command buffer.

## 12. Entity Replacement

If one semantic object changes runtime form, for example intact device -> wreck proxy, the owning domain commits the state change first. Runtime may then:

- mutate components in place; or
- close old entity and spawn replacement projection.

Either implementation preserves the same persistent identity/outcome and cannot duplicate ownership.

## 13. Death/Corpse/Wreck

Defeat does not imply immediate reclamation.

A persistent or gameplay-relevant corpse/wreck remains as an entity/proxy for as long as World/Mission/Inventory/Repair rules require.

Only when those rules permit removal does runtime cleanup proceed.

## 14. Scene Teardown

On scene teardown:

1. all persistent-backed active leases are prepared for return/export;
2. required domain commits finish at a Stable Simulation Boundary;
3. transient local state is resolved/discarded according to owning rules;
4. physics/render/audio/backend resources are retired;
5. component pools/registry are destroyed.

Scene teardown never discards unresolved persistent leased state.

## 15. Hard Streaming Hold / True Pause

These freeze Simulation Time but do not invalidate runtime entities. Technical asset/background preparation may continue where allowed; gameplay synchronization/expiry/status/action progress does not advance.

## 16. Error Handling

A stale epoch/generation return is rejected as an invariant violation/stale result; it is never merged heuristically.

Failure to return required persistent leased state during orderly scene transition is transition-fatal rather than silently dropping state.

## 17. Tests

Tests cover stale epoch return rejection, active save export, immediate cross-domain commit versus leased facet distinction, logical-removal-before-free, callback destroy buffering, corpse/wreck retention, scene teardown lease return and backend cleanup ordering.

## 18. Non-Goals

No reference-counted universal gameplay object ownership, no immediate free from callbacks, no hidden merge of competing active/persistent copies, no runtime-handle persistence and no scene teardown that resets persistent actor state.

## 19. Dependencies

Depends on TA-2 leases/revisions/transactions, TA-3 scene lifecycle, TA-4/5 backend lifetimes and `44`–`49`.

## 20. Open Questions

None.
