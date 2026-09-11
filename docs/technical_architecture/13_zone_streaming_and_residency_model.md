# Zone Streaming and Residency Model

> **Status:** Architecture Complete  
> **Authority:** Stream-cell partitioning, content residency, simulation activation, prefetch, eviction, streaming holds, sub-zone transitions, and streaming correctness invariants

## 1. Purpose

Large bounded local contexts can exceed the amount of geometry, collision, AI, audio, and asset data that should remain fully active at all times.

Streaming must reduce runtime cost without changing gameplay truth.

## 2. Streaming Is Technical, Not Gameplay Authority

A cell being unloaded does not mean its gameplay state ceased to exist.

Persistent Mission/World/domain state remains authoritative regardless of residency.

Streaming controls technical representations only.

## 3. Stream Cell

A `StreamCell` is the coarse residency unit inside a SceneInstance.

A cell can contain references to:

- static geometry chunks;
- collision data;
- authored spawn definitions;
- environment volumes;
- audio regions;
- navigation tile data;
- render assets;
- persistent-object activation descriptors.

## 4. Stream Cell Identity

A cell uses a stable key scoped to its owning Zone/Location.

It is not a globally persistent gameplay identity unless a separate GDS object actually requires one.

## 5. Residency and Gameplay Activation Are Separate Axes

The architecture separates:

### Content Residency

- Unloaded;
- Requested;
- Loading;
- Resident;
- EvictPending.

### Simulation Activation

- Inactive;
- Activating;
- Active;
- Deactivating.

A Resident cell can remain simulation-Inactive as a prefetched neighbor.

## 6. Why Separation Is Required

Prefetching assets early must not:

- spawn enemies early;
- advance hazards;
- reveal unknown POIs;
- trigger objectives;
- consume resources;
- start combat AI.

Residency is therefore not equivalent to activation.

## 7. Deterministic Required Activation Set

The Simulation thread determines the logical set of cells that must be Active using authoritative state such as:

- player/ship position;
- zone traversal topology;
- active objective requirements;
- explicitly active local hazards/events;
- gameplay interaction ranges.

The decision cannot depend on worker completion order or render-frame timing.

## 8. Predictive Prefetch Set

A non-authoritative prefetch system can request additional Resident cells based on:

- velocity/direction;
- camera direction;
- route connectivity;
- nearby doors/portals;
- likely mission objective path;
- ship speed.

Prefetch may vary with hardware/performance because it does not activate gameplay.

## 9. Resident-Set Profiles

Each zone/content profile defines tuneable streaming parameters such as:

- activation radius;
- prefetch radius;
- minimum neighbor depth;
- eviction delay;
- memory-priority class.

Surface/interior/spaceflight contexts can use different values.

Exact values are content/performance tuning under TA-10/TA-13.

## 10. Interior Portal/Adjacency Streaming

Interior cells can use room/corridor/portal adjacency for prefetch and activation.

A closed door can still cause adjacent content to be Resident before opening while its actors remain Inactive until the activation contract requires them.

## 11. Surface Streaming

Surface zones can use authored/procedural tile cells.

Terrain/geometry residency can extend beyond gameplay activation distance to avoid visible pop-in.

## 12. Local Spaceflight Streaming

Spaceflight zones use coarser 3D stream cells/interest volumes appropriate to higher traversal speeds.

Important stations, ships, wrecks, asteroids, hazards, and transition volumes can force residency ahead of ordinary distance rules.

## 13. Mixed Zone Streaming

A Mixed Zone can combine different cell profiles.

Example:

- Local Spaceflight exterior cells;
- station exterior EVA cells;
- boarding connector;
- interior room cells.

All remain under one SceneInstance/Mission state.

## 14. Cell Activation

A cell can enter Active only when:

- required content/collision is Resident;
- persistent state needed to instantiate it is available;
- cross-references are valid;
- activation dependencies are satisfied.

Activation occurs at Stable Simulation Boundary.

## 15. Activation Effects

Cell activation can:

- grant TA-2 Activation Leases;
- create runtime entities;
- register physics bodies/triggers;
- register local environment providers;
- register gameplay spatial entries;
- activate AI behavior on the next valid phase.

No actor receives hidden extra simulation time because its cell loaded late.

## 16. Cell Deactivation

A cell becomes eligible for deactivation only when:

- no required active participant remains inside;
- no noninterruptible local transaction depends on its runtime state;
- persistent consequences are synchronized;
- transition/path rules allow eviction.

## 17. Deactivation Does Not Resolve Gameplay

Deactivation cannot automatically:

- kill actors;
- fail/complete objectives;
- collect loot;
- reset doors;
- restore resources;
- heal/repair entities.

Only owning gameplay systems can do those things.

## 18. Cross-Cell Actor Movement

Before an authoritative actor crosses into a destination cell:

- destination must be Resident and logically Active or become Active at the boundary;
- relevant collision/navigation representation must exist;
- the actor retains the same persistent/runtime identity as appropriate.

## 19. Hard Streaming Hold

If a required destination cell cannot become ready before gameplay would cross into unavailable space, StarForge enters a **Hard Streaming Hold** at a Stable Simulation Boundary.

During a Hard Streaming Hold:

- authoritative Simulation Time does not advance;
- presentation/loading feedback can continue using real time;
- workers continue loading;
- no actor can gain tactical advantage/disadvantage because another machine loads slower.

The hold exits only when required state is ready or a technical failure is declared.

## 20. Hard Hold Is Exceptional

Normal play should avoid Hard Streaming Holds through prefetching and memory budgets.

Frequency/duration are performance metrics and later acceptance gates.

## 21. No Invisible Simulation Beyond Residency

High-frequency actors do not keep full 60 Hz local AI/physics after their cell deactivates.

If their gameplay state must continue, it returns to an owning persistent/off-screen representation whose cadence is explicitly defined elsewhere.

## 22. Persistent Mission Actors

A persistent mission actor can deactivate and later reactivate from its committed persistent state.

A nonpersistent disposable actor can be retired only if its owning mission/combat rules say its continued individual identity no longer matters.

## 23. Loot/Resource Persistence

Opened containers, harvested deposits, dropped persistent items, and recovered/abandoned robots are represented by persistent Mission/World/Inventory state.

Cell eviction cannot regenerate them.

## 24. Spawn Definitions

Authored spawn definitions are immutable content.

A spawn's runtime realization has separate persistent/mission state recording whether it:

- has not spawned;
- is active;
- was defeated/removed;
- moved elsewhere;
- remains eligible according to explicit mission rules.

Loading a cell does not blindly replay spawn definitions.

## 25. Procedural Cell Reconstruction

Procedural cells reconstruct from:

- stable owning instance identity;
- generation algorithm version;
- stable seed;
- persisted resolved differences.

Worker thread generation must match TA-2 RNG/version contracts.

## 26. Streaming Request Versioning

Every asynchronous streaming request carries:

- SceneGeneration;
- owning cell key;
- content generation/revision where applicable;
- request generation.

Stale results are discarded.

## 27. Asset Dependency Failure

If a blocking cell asset is missing/corrupt:

- the cell never becomes Ready/Active;
- transition remains safely held or fails;
- no placeholder collision can silently change gameplay geometry.

Presentation-only missing assets can use explicit fallback only when collision/gameplay semantics remain identical.

## 28. Eviction

Eviction releases technical resources after:

- cell is simulation-Inactive;
- no runtime proxy references remain;
- no in-flight worker/upload uses the resource unsafely;
- cache policy permits release.

## 29. Eviction Is Nondeterministic but Harmless

The exact moment a Resident-but-Inactive cell is evicted can depend on memory pressure/performance.

Because gameplay authority is not attached to residency, different eviction timing cannot change gameplay outcomes.

## 30. Stream Cell vs Spatial Grid

Stream cells are not required to equal the gameplay spatial-query grid.

Streaming partitions optimize residency.

Spatial-query partitions optimize runtime lookup/culling/query behavior.

The two can use different sizes/topologies.

## 31. Visibility Is Not Authority

Renderer visibility/culling does not control simulation activation.

An actor can remain Active while outside the camera view.

Likewise a prefetched visible static mesh can be Resident without activating hidden gameplay entities.

## 32. Save Capture

At Stable Save Boundary:

- streamed-cell residency is not required to serialize as gameplay state;
- authoritative zone/entity state does serialize through owning domains;
- optional technical cache hints can be omitted/rebuilt.

Load can choose a different residency set while restoring equivalent gameplay state.

## 33. Debugging

Development tools show per cell:

- residency state;
- activation state;
- memory estimate;
- blocking dependencies;
- last request generation;
- active persistent IDs;
- reason for activation/prefetch;
- eviction eligibility.

## 34. Test Requirements

Tests cover:

- residency without activation produces no gameplay effects;
- activation set independent from worker completion order;
- unload/reload preserves mission/world state;
- cross-cell actor movement retains identity;
- stale streaming results are rejected;
- Hard Streaming Hold freezes Simulation Time;
- cell eviction cannot alter ownership/objectives/spawn state;
- procedural reconstruction is seed/version stable.

## 35. Performance Requirements

Streaming must permit later TA-13 budgets for:

- CPU resident memory;
- GPU resident memory;
- blocking I/O latency;
- cell activation cost;
- Hard Streaming Hold frequency.

Architecture does not yet fix those numeric budgets.

## 36. Explicit Non-Goals

The streaming system does not provide:

- seamless galaxy traversal;
- gameplay state stored only in loaded cells;
- render visibility driving actor existence;
- worker-thread gameplay activation;
- infinite world terrain;
- guaranteed zero loading boundaries on every supported machine.

## 37. Dependencies

Depends on TA-1 scheduling/workers, TA-2 identity/activation/revisions/RNG, TA-3 scene model, GDS Mission Zones, World persistence, and later TA-10/TA-13.

## 38. Open Questions

None in the TA-3 streaming/residency model.
