# TA-13 — Station, AI, Mission, and Strategic Scalability

> **Status:** Architecture Complete  
> **Authority:** Numeric scalability envelopes for Horizon graphs/work, active/off-screen AI/navigation, missions/objectives/events, strategic deadlines, and chronological scheduler work

## 1. Purpose

StarForge has systemic persistent simulation that must scale without becoming one second full-detail scene. TA-13 fixes measurable envelopes for TA-6, TA-8, and TA-9 while preserving exact ownership and chronological semantics.

## 2. Horizon Structural Scale

One player Horizon station targets support for:

- station modules: **<= 2,048**;
- structural/traversal connection records: **<= 8,192**;
- pressure cells: **<= 4,096**;
- aggregate subsystem graph nodes across Power/Thermal/Water/Logistics/ControlData: **<= 8,192 per subsystem**;
- aggregate graph edges: **<= 16,384 per subsystem**.

These are architecture scale envelopes, not player-facing build limits. Content/design can choose lower limits.

## 3. Station Solver Time

On representative maximum-density Horizon scenarios:

- routine station due-work p95 contribution: **<= 1.5 ms/base tick**;
- ordinary off-screen station average: **<= 0.75 ms/base tick**;
- single significant-boundary burst target: **<= 4.0 ms**;
- topology full-view rebuild target: **<= 8.0 ms** under an explicit mutation/loading boundary, not every tick.

Incremental invalidation is expected for ordinary localized changes.

## 4. Logistics Scale

Persistent Horizon targets:

- active/pending TransferJobs: **<= 4,096**;
- active reservations: **<= 8,192**;
- concurrently moving physical cargo representations in an active scene: governed by TA-7 entity budget, not all TransferJobs;
- logistics routing recomputation should be incremental by affected topology/revision.

A TransferJob is never dropped for performance.

## 5. WorkOrder Scale

Persistent station work targets:

- total active/queued WorkOrders: **<= 2,048**;
- work completions due in one base tick normal target: **<= 128**;
- semantic safety cap due at same tick: **4,096**.

If an authored/configured station produces pathological synchronized completion bursts, the runtime diagnoses the content/state rather than deleting completions.

## 6. Automation Task Scale

`StationAutomationStore` targets:

- unresolved tasks: **<= 4,096**;
- simultaneously assigned mobile tasks: **<= 512**;
- new-task detection/assignment ordinary target: **<= 256 operations/base tick**.

Task deduplication remains semantic and prevents repeated sensors from creating unlimited duplicate jobs.

## 7. Chronological Boundary Iterations

For one base 60 Hz advancement interval, station/strategic schedulers normally target **<= 128 significant chronological boundaries** across all coarse systems.

A hard diagnostic safety cap of **2,048 boundaries per base tick** exists. Crossing it indicates a pathological zero/near-zero reschedule loop or invalid content and is not resolved by skipping boundaries.

## 8. Off-Screen Mobile Actor Scale

Logical off-screen navigation/task state targets:

- crew + robot mobile actors under station strategic scheduler: **<= 1,024**;
- route-segment/deadline updates due per base tick normal target: **<= 256**.

Actors outside the active scene do not retain full Jolt/CharacterMotor/renderer state.

## 9. Active Detailed AI Scale

One active local context targets:

- detailed AI actors: **<= 128**;
- active perception-enabled combat actors: **<= 96**;
- high-level decision evaluations due in one tick: **<= 64 ordinary target**;
- emergency/interrupt decisions can exceed that target but are never dropped.

The local runtime entity cap remains larger because not every entity runs full AI.

## 10. AI CPU Targets

On benchmark combat scenes:

- high-level AI decision + scheduler p95: **<= 1.0 ms/tick**;
- perception fact ingestion/query preparation p95: **<= 0.75 ms/tick**;
- path-result acceptance/application p95: **<= 0.5 ms/tick**;
- local avoidance/following is included in movement/AI profiling and should remain bounded by actor count.

## 11. Perception Candidate Budget

Spatially generated perception candidates target **<= 8,192 actor-sensor candidate pairs/tick** before visibility/hearing/sensor filters.

The implementation must use spatial/knowledge filtering rather than O(N²) all-actor perception.

## 12. Pathfinding Capacity

TA-13 worker limits already cap **256 outstanding path requests**.

Additional targets:

- accepted path results processed: **<= 32/tick ordinary**;
- grounded path query CPU worker p95: **<= 4 ms/request** for representative routes;
- 3D free-flight path query CPU worker p95: **<= 4 ms/request**;
- path lengths/results use explicit safety maxima in implementation, with failures returning typed `NoPath/TooComplex` rather than allocating without bound.

Worker timing cannot choose actor priority.

## 13. Navigation Tile Rebuild

Dynamic grounded-nav invalidation targets:

- ordinary localized rebuild: **<= 8 tiles/request**;
- no more than **32 tiles** published in one live-gameplay simulation boundary under normal changes;
- larger reconstruction occurs during explicit scene staging/loading or is spread as advisory work until safe publication.

Gameplay continues on the last valid nav generation where allowed; actors never use stale routes through newly invalid physical topology.

## 14. Mission Objective Scale

Per MissionInstance:

- objective DAG nodes soft target: **<= 256**;
- hard semantic cap: **1,024 nodes**;
- objective transitions due from one committed fact batch normal target: **<= 64**.

Objective evaluation remains event/read-state driven rather than polling every objective every tick.

## 15. Mission/Strategic Persistent Scale

A campaign can retain historical MissionIds/attempts beyond active-scene budgets, but active scheduling targets:

- external deployed MissionInstance: fixed GDS maximum **1**;
- optional offered Dynamic Events: fixed GDS maximum **3**;
- active/scheduled Critical Horizon Event: fixed GDS maximum **1**;
- concurrent unresolved Horizon DefenseEvent: content/state normally **<= 1**, with any exception requiring explicit TA/GDS rule rather than accidental stacking.

Historical records are cold persistent data and need not execute every tick.

## 16. Reinforcement and Strategic Route Scale

One raid/defense target normally supports:

- committed reinforcement waves/manifests: **<= 32**;
- active strategic transit records associated with that event: **<= 64**.

These are implementation soft envelopes. Finite response identity/state is never merged or deleted merely to fit the target.

## 17. Dynamic Event Scheduler

Dynamic-event candidate generation is deadline/cursor driven.

Ordinary runtime target:

- event-generation candidates evaluated in one scheduling pass: **<= 64**;
- committed event creations in one base tick: **<= 16**;
- worker-generated candidates are bounded and selected by deterministic ordinal.

Recovery Grace/concurrency rules usually keep player-facing active counts much lower.

## 18. Strategic Backlog

Strategic due work uses the same global Simulation Time. It cannot lag indefinitely behind the current tick.

If strategic work cannot complete within normal soft budget:

1. optional candidate generation is deferred;
2. routine non-due reevaluation is deferred;
3. due chronological consequences continue;
4. main tick may exceed soft target and global backlog policy takes over.

No second asynchronous strategic timeline is allowed to trail behind gameplay authority.

## 19. Active vs Off-Screen Equivalence Testing

Performance optimization must preserve equivalence for representative traces:

- power loss stopping production;
- water depletion stopping crops;
- repair interrupted by actor loss;
- reinforcement arrival timing;
- Horizon Defense damage/resource theft;
- communication loss;
- player return mid-event.

Coarse scheduling is accepted only if the same committed state results within the defined fixed-tick/time semantics.

## 20. Memory Expectations

Station/AI/strategic persistent state should target **<= 512 MiB combined** at the scale envelopes in this document, excluding shared authored ContentCache and active scene render/physics assets.

A single runaway store approaching hundreds of MiB is a profiling red state.

## 21. Content Validation

Authored/procedural definitions that can deterministically exceed a semantic hard cap must be rejected during content validation/generation before deployment where possible.

Procedural generation cannot choose a cheaper/different mission because the current machine is slow.

## 22. Diagnostics

Per-domain telemetry includes:

- graph nodes/edges and rebuild size/time;
- scheduler boundary count/time;
- WorkOrders/TransferJobs/AutomationTasks;
- active/off-screen actor counts;
- AI decision/perception counts/time;
- path request queue/result latency;
- nav rebuild tiles/time;
- objective/event transitions;
- strategic deadline backlog.

## 23. Non-Goals

No DefenseScore shortcut, no off-screen teleport, no hardware-dependent AI quality, no skipped chronological thresholds, no polling every persistent record every 60 Hz tick.

## 24. Open Questions

None in the TA-13 station/AI/strategic scalability architecture.