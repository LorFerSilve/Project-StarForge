# TA-7 — Runtime Update Phases and System Boundaries

> **Status:** Architecture Complete
> **Authority:** Per-tick ordering of local runtime systems, structural command application, combat/interaction fact handoff, and immutable consumer snapshot publication

## 1. Purpose

TA-7 needs one deterministic high-frequency execution order so actor state, physics, combat, interaction, persistent-domain commits, deferred destruction, and presentation snapshots cannot race each other.

## 2. Base Cadence

The active local gameplay runtime executes on TA-1's fixed 60 Hz authoritative Simulation Time tick.

Not every subsystem must perform expensive work every tick, but any high-frequency state mutation participates in this ordered frame.

## 3. Canonical Runtime Tick

The baseline order is:

```text
1. Tick Begin / Stable State Latch
2. Apply accepted external/player gameplay intents
3. Validate queued activation/spawn/despawn requests
4. Update active action timers / status deadlines due at this tick
5. Build movement / weapon / interaction intents
6. Pre-physics gameplay queries and kinematic targets
7. TA-5 Physics Step
8. Normalize contacts, overlaps, trigger and projectile facts
9. Resolve movement/support/physical state
10. Resolve combat hits, damage/status intents, interaction eligibility
11. Prepare and commit owning-domain transactions/outcomes
12. Reflect committed consequences into active leased components
13. Run post-consequence actor/object lifecycle transitions
14. Apply deferred runtime structural mutations
15. Apply deferred safe physics proxy mutations
16. Reach Stable Simulation Boundary
17. Publish immutable runtime/read/render/audio snapshots
18. Tick End
```

TA-8 may insert AI intent production into the defined intent phases without changing commit authority.

## 4. Input Boundary

TA-11 later maps raw device input to gameplay actions. TA-7 consumes only normalized gameplay intents valid for the current input/context state.

Render frames do not directly execute movement, attacks or interactions.

## 5. Action and Status Deadlines

Finite actions/status effects use Simulation-Time deadlines. When multiple deadlines are due in one tick, explicit semantic priority plus stable identity ordering resolves them.

## 6. Movement Intent

Movement systems produce desired character/ship control inputs before the physics step. Physics then determines supported/collided physical result under TA-5.

No post-physics gameplay system rewinds the physics step to invent a different collision result.

## 7. Combat Fact Phase

Physical queries/contacts become normalized facts after physics. Combat consumes those facts and produces typed consequence intents/results.

Damage/status is not applied from inside TA-5 callbacks.

## 8. Interaction Phase

Interaction targeting may use current pre/post-physics spatial facts as defined by the action, but success commits only through the target's owning command/transaction after final validation.

A prompt or focused target is not success authority.

## 9. Persistent Domain Commit Phase

Cross-domain gameplay mutation uses TA-2 prepared transactions.

Examples:

- pickup ownership transfer;
- ammunition/resource consumption where persistent owner participates;
- mission objective result;
- faction/reputation consequence;
- station damage;
- persistent actor outcome.

The runtime never holds a transaction open into a later tick.

## 10. Runtime Reflection Phase

After persistent/domain consequence commit, leased runtime components are updated to the resulting valid state in deterministic order.

Example:

```text
Pickup commit succeeds
-> Inventory now owns item
-> source WorldItem proxy quantity becomes zero
-> source runtime entity enters Closing
```

not the inverse.

## 11. Lifecycle Phase

Entities reaching Incapacitated, Disabled, Destroyed, Completed, Extracted or otherwise no-longer-normal states enter their explicit runtime lifecycle transition after the owning gameplay result has committed.

## 12. Structural Mutation Phase

Buffered entity/component structural commands are applied only after systems using the previous stable component topology have completed.

This includes spawn finalization, component add/remove, Closing -> PendingDestroy and entity reclamation eligibility.

## 13. Physics Mutation Phase

TA-5 backend body/constraint changes occur in its designated deferred-safe phase after gameplay commits. Runtime registry/component mutation cannot call Jolt mutation from arbitrary systems.

## 14. Snapshot Publication

Only after all authoritative consequences and structural changes for the tick are settled are immutable consumer snapshots generated:

- TA-4 RenderSnapshot;
- UI/read-model inputs;
- audio event/state snapshots;
- worker-job inputs where applicable.

Presentation never sees a half-committed gameplay transaction as final state.

## 15. Event/Facts Lifetime

Per-tick normalized facts are owned by bounded typed queues/arenas whose lifetime ends after all designated consumers finish the tick unless explicitly promoted to persistent/longer-lived state.

A global untyped event bus is prohibited.

## 16. Command Buffers

Separate typed buffers are preferred for semantically different mutation categories, for example:

- runtime structural commands;
- physics proxy mutations;
- activation/deactivation requests;
- combat consequence intents;
- interaction/domain commands.

This avoids one opaque queue with undefined ordering.

## 17. Same-Tick Conflicts

Conflicts such as extraction versus Health zero, pickup versus source destruction, or two actors claiming one object are resolved by owning-domain deterministic event/transaction ordering.

Whichever valid transaction commits first changes the preconditions; later conflicting commands fail/re-evaluate rather than duplicating state.

## 18. Worker Results

Asynchronous results can be consumed only at defined phases and must revalidate:

- SceneGeneration;
- RuntimeEntityHandle generation;
- relevant component/runtime revision;
- persistent source revision where applicable.

Stale results are discarded, not merged heuristically.

## 19. Headless Execution

The full TA-7 authoritative runtime tick can run without OpenGL, audio output or shipping UI. Render/audio snapshot production may be replaced by test sinks.

## 20. Diagnostics

Per-tick diagnostics may record:

- phase durations;
- entity/component counts;
- spawn/destroy queues;
- activation/deactivation counts;
- hit/damage/status facts;
- domain transaction counts/failures;
- stale worker-result rejects;
- snapshot revisions.

Numeric budget thresholds belong to TA-13.

## 21. Test Requirements

Tests cover canonical phase ordering, no callback-side destruction, persistent commit before runtime disappearance, same-tick ownership conflict, status periodic tick determinism, stale worker rejection, Stable Simulation Boundary integrity, and headless execution.

## 22. Explicit Non-Goals

No variable-delta authoritative gameplay loop, no render-thread gameplay mutation, no one untyped global event queue, no arbitrary system execution order and no transaction spanning multiple simulation ticks.

## 23. Dependencies

Depends on TA-1 runtime phases, TA-2 transactions/read models, TA-5 physics phase ordering, and `44`–`50`.

## 24. Open Questions

None. TA-8 may refine where AI intent generation runs while preserving this phase/authority contract.
