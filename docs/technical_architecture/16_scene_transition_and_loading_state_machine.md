# Scene Transition and Loading State Machine

> **Status:** Architecture Complete  
> **Authority:** Inter-context transition state machine, destination staging, loading holds, atomic context switching, transition failure, scene replacement, and Simulation Time behavior during blocking loads

## 1. Purpose

Moving between Horizon, missions, persistent locations, local spaceflight zones, and recovery destinations must not become an ad-hoc sequence of unload/load calls.

Transitions can change:

- player location;
- ship location;
- Mission state;
- extraction/security state;
- Active Local Context;
- persistent/runtime authority leases.

The transition architecture therefore stages technical work before committing gameplay location changes.

## 2. Transition Types

Baseline transition classes:

- Horizon Departure/Arrival;
- Mission Deployment;
- Mission Extraction;
- Strategic Travel Arrival/Departure Local Context;
- Local Spaceflight Entry/Exit;
- Persistent Location Visit;
- Recovery Arrival;
- Intra-Context Sub-Zone Streaming Transition.

Intra-context cell streaming uses TA-3 streaming rules and does not replace the whole Active Local Context.

## 3. ContextTransitionId

Every inter-context transition receives a process/session-unique technical `ContextTransitionId` for diagnostics and correlation.

It is not a gameplay asset identity and does not replace Mission/Ship/Transaction IDs.

A save only needs transition identity/state when the GDS/TA save boundary allows capture during that transition phase.

## 4. Canonical State Machine

Inter-context transitions use:

1. `Idle`;
2. `Requested`;
3. `Validating`;
4. `PreloadingDestination`;
5. `DestinationReady`;
6. `QuiescingOrigin`;
7. `TransitionHold`;
8. `CommittingContextSwitch`;
9. `ActivatingDestination`;
10. `Completed`;
11. `Failed`.

Not every presentation frame sees every state, but the logical boundaries are preserved.

## 5. Requested

A gameplay/system command requests transition with:

- transition type;
- origin descriptor;
- destination descriptor;
- involved persistent identities;
- reason/mission/extraction context;
- expected StateRevisions where relevant.

No location changes yet.

## 6. Validating

Owning domains validate GDS requirements such as:

- route/reach;
- extraction readiness;
- docking/landing compatibility;
- mission deployment requirements;
- player/ship/robot state;
- destination availability;
- Recovery destination validity.

Validation is side-effect free except for explicitly committed preparatory gameplay transactions defined by the owning system.

## 7. PreloadingDestination

Technical destination data is staged while origin can remain active when safe.

Preloading can request:

- content definitions;
- zone layout reconstruction/generation;
- blocking assets;
- collision/nav data;
- static scene structures;
- runtime activation plans.

The staging scene is non-authoritative.

## 8. DestinationReady

DestinationReady means:

- required blocking content exists;
- zone/layout validation passed;
- staging scene/runtime plan can activate;
- required entry point exists;
- required cross-references resolve;
- no technical blocker is known.

It does not yet mean the player has arrived.

## 9. Revision Revalidation

Before quiescing origin, the transition revalidates all persistent StateRevisions that could invalidate:

- destination;
- transport;
- player/ship ownership;
- mission/extraction permission;
- entry-point state.

Stale transition plans are refreshed or rejected.

## 10. QuiescingOrigin

At a Stable Simulation Boundary:

- new local commands incompatible with departure stop;
- atomic local transactions finish;
- required mission/extraction/ownership effects prepare;
- active leases prepare return state;
- deferred destruction is drained.

The origin scene remains authoritative until context switch commit.

## 11. TransitionHold

After origin can no longer continue normal gameplay and before the destination is authoritative, the session enters `TransitionHold`.

During TransitionHold:

- no authoritative Simulation Time advances;
- platform/UI/loading presentation continues in real time;
- asset/background jobs continue;
- gameplay commands other than allowed loading/cancel/recovery commands are rejected/queued by contract.

This matches the GDS rule that blocking loading does not progress gameplay.

## 12. No Wall-Clock Compensation

Time spent on a loading screen is never added back into Simulation Time.

A slower disk or computer cannot cause:

- more station production;
- more healing;
- reinforcement arrival;
- market changes;
- hazard damage;
- travel completion.

## 13. Committing Context Switch

The context switch is an atomic authoritative transaction where required participants commit together.

Depending on transition type, this can include:

- player strategic/local location;
- ship location/state;
- Mission deployed/extracted state;
- inventory security transition;
- robot transport/deployment state;
- ActiveLocalContextDescriptor.

A partial switch is prohibited.

## 14. Destination Activation Ordering

Destination technical scene is staged before commit, but TA-2 Activation Leases become authoritative only as part of/after the successful context switch boundary.

The architecture must never expose a destination runtime entity as active while the persistent location still claims it is at the origin.

## 15. ActivatingDestination

After context switch commit:

- destination SceneGeneration becomes current;
- required Activation Leases commit;
- runtime entities/physics/spatial entries are enabled;
- player/ship is placed at validated Entry Point;
- render/read snapshots publish destination state;
- local gameplay resumes on the next ordinary simulation tick.

## 16. Completed

Completed transition means:

- destination is the sole Active Local Context;
- origin runtime scene is no longer authoritative;
- persistent state reflects the transition;
- transition-owned temporary resources can be released.

## 17. Origin Eviction

Origin scene resources can be evicted after context switch according to cache policy.

The eviction timing is technical and cannot change persistent gameplay outcomes.

## 18. Failure Before Context Commit

If transition fails before `CommittingContextSwitch`:

- origin remains authoritative where it still exists;
- no persistent location/extraction switch occurs;
- destination staging is discarded/diagnosed;
- player receives an appropriate technical/gameplay result.

## 19. Failure During TransitionHold

If the origin was quiesced but the context switch has not committed, the system attempts to restore/resume the valid origin scene from its still-authoritative state.

If that is technically impossible due to unrecoverable content/device failure, the session enters controlled technical failure handling rather than inventing gameplay consequences.

## 20. Failure After Context Commit

The context switch commit is only allowed once destination technical activation has been validated to a no-fail commit set.

No filesystem/worker/backend operation that can ordinarily fail belongs inside the commit.

If an unexpected invariant/backend failure occurs after commit, the session stops into controlled fatal/recovery handling; it does not roll gameplay back through ad-hoc inverse mutations.

## 21. Mission Deployment

Mission Deployment transition requires:

- Mission prepared/deployable;
- required ship/player/robot state;
- valid destination Zone Instance/Entry Point;
- generated layout stable/validated;
- loadout ownership committed according to Mission rules.

The Mission Instance identity survives transition.

## 22. Mission Extraction

Extraction commit remains GDS-8 authority.

Technical transition stages the destination before or during extraction preparation where possible.

At extraction commit, the transaction can update:

- mission state;
- eligible security states;
- passenger/robot/ship state;
- player context location.

The loading screen does not itself secure loot.

## 23. Strategic Travel

Strategic travel is a persistent gameplay state, not one continuous local scene.

Local departure scene transitions into travel state under Spacecraft/Navigation authority.

At due Simulation Time arrival:

- destination local context can be staged;
- travel arrival transaction commits;
- destination scene activates.

Loading duration after travel completion adds no extra Simulation Time.

## 24. Recovery Transit

Recovery Transit remains persistent strategic state during its Simulation-Time duration.

When its arrival becomes due, the destination is revalidated and staged.

The local Recovery/Station scene begins only after arrival commit.

## 25. Save Boundaries During Transition

Normal save capture is allowed only in transition states explicitly classified as stable by Persistence architecture.

Baseline:

- Requested/Validating/PreloadingDestination can save the unchanged authoritative origin if no gameplay commit is pending;
- Quiescing/CommittingContextSwitch are not Stable Save Boundaries;
- TransitionHold after origin handoff but before context commit is not saved as a half-transition in v1;
- Completed can save normally.

TA-12 can define exact file-level handling but cannot serialize a half-committed context switch.

## 26. Autosave Placement

An autosave request arriving during an unstable transition state is queued until the next valid Stable Save Boundary.

It is not silently dropped unless shutdown/fatal error prevents completion.

## 27. Intra-Context Cell Transition

Crossing streamed cells/sub-zones within one Active Local Context does not change persistent player strategic location by default.

The streaming system ensures destination residency/activation.

If loading cannot keep up, a Hard Streaming Hold freezes Simulation Time at the local boundary.

## 28. Entry Points

Entry Points are explicit authored/generated records containing:

- stable local key;
- transform/reference frame;
- compatible actor/ship class;
- environment/safety requirements;
- optional linked Exit Point;
- activation prerequisites.

Entry selection is deterministic given authoritative transition data.

## 29. Exit Points

Exit Points are gameplay-owned routes such as:

- ship departure;
- airlock/dock;
- mission extraction area;
- strategic local-flight boundary.

Technical scene transition cannot create an exit not authorized by gameplay state.

## 30. Entry Collision Safety

Before activation, destination verifies that required spawn/entry geometry is valid enough to avoid embedding the player/ship in blocked collision.

A technical fallback transform is allowed only if the Zone definition explicitly supplies a valid alternate Entry Point.

No arbitrary nearest-free-space teleport changes authored mission access.

## 31. Presentation

UI receives a transition read model containing only:

- transition state relevant to presentation;
- destination known name/identity;
- blocking/failure reason safe to reveal;
- loading progress estimates that do not imply gameplay progression.

Technical loading cannot leak hidden mission content.

## 32. Threading

Transition state ownership remains main/simulation-thread authoritative.

Workers can preload/decode/build staging data and return immutable versioned results.

## 33. Diagnostics

Development tools expose:

- ContextTransitionId;
- origin/destination descriptors;
- state;
- validation revisions;
- staging dependencies;
- loading/streaming blockers;
- TransitionHold duration in real time;
- context commit TransactionId;
- destination SceneGeneration.

## 34. Test Requirements

Tests cover:

- destination staging cannot mutate gameplay;
- failed preload leaves origin valid;
- context switch commits atomically;
- Simulation Time freezes during TransitionHold;
- extraction security changes only on extraction transaction commit;
- no save captured in half-commit states;
- mission/ship/robot IDs survive transition;
- stale destination revision causes revalidation;
- recovery/strategic-travel loading time does not advance gameplay.

## 35. Explicit Non-Goals

Transitions do not use:

- unload-origin-first and hope destination works;
- gameplay progression during loading screens;
- raw scene serialization as travel state;
- loading UI as extraction/ownership authority;
- arbitrary fallback teleport through blocked geometry;
- worker-thread context switching.

## 36. Dependencies

Depends on TA-1 runtime, TA-2 transactions/activation/save boundaries, TA-3 scene/streaming/coordinates, GDS Missions/Extraction/Navigation/Recovery, and later TA-10/TA-12.

## 37. Open Questions

None in the TA-3 scene-transition/loading contract.
