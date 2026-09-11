# TA-8 — Async Pathfinding and Path Following

> **Status:** Architecture Complete
> **Authority:** Path-request lifecycle, worker-job contract, deterministic acceptance, corridor following, local avoidance, replanning, blocked-state reporting, and path persistence boundary

## 1. Purpose

Pathfinding is computationally expensive enough to benefit from worker threads, but AI behavior must remain deterministic and authoritative on the main simulation thread.

This specification defines that boundary.

## 2. Path Request

A path request contains at minimum:

- `PathRequestId` / monotonically increasing request sequence scoped to actor;
- `RuntimeEntityHandle` + generation;
- persistent actor ID when one exists;
- `SceneGeneration`;
- `NavigationSpace` key;
- `NavigationRevision`;
- start position;
- destination specification;
- `NavTraversalProfile`/profile revision;
- current known access/hazard filter revision;
- requested route policy/cost profile;
- purpose/task identifier.

## 3. Request Lifecycle

Canonical states:

```text
Requested
→ Queued
→ Computing
→ ResultReady
→ Accepted | RejectedStale | NoPath | Failed
→ Following
→ Arrived | Blocked | Cancelled | ReplanRequested
```

A path request is not a long-running gameplay transaction.

## 4. Worker Authority

Worker threads may:

- read immutable navigation snapshots;
- execute Recast/Detour or project-owned 3D graph queries;
- build candidate path corridors;
- return diagnostic/path-cost data.

Workers may not:

- move actors;
- mutate navigation topology;
- open doors;
- commit command completion;
- change AI state directly;
- choose mission consequences.

## 5. Result Freshness

Every result must match all required freshness tokens:

- same SceneGeneration;
- same actor handle generation;
- same or explicitly compatible NavigationRevision;
- same active PathRequest sequence;
- same task/command generation;
- compatible current traversal/access profile.

A stale result is discarded, never heuristically patched into current state.

## 6. Deterministic Consumption

Worker completion timing cannot decide gameplay.

At the AI/path-result consumption phase, ready results are considered in stable order by:

1. actor stable identity/runtime sort key;
2. active request sequence;
3. path-purpose priority.

An actor consumes only its currently active request result.

## 7. Path Query Determinism

Path queries use fixed area costs, stable traversal-link IDs, stable query filters, and deterministic project-side tie-breaking where multiple equivalent authored candidates are exposed.

No AI decision is based on pointer/hash iteration order or which worker finished first.

## 8. Destination Kinds

A path request can target:

- exact reachable point;
- bounded destination area/radius;
- target actor approach region;
- work/interaction anchor;
- cover candidate;
- patrol/muster/rally anchor;
- transport/rack/dock approach point;
- traversal-link entry;
- free-flight observation volume.

The target specification owns acceptable arrival criteria.

## 9. Corridor Representation

Accepted grounded paths are represented as project-owned path corridors/waypoint portals derived from Detour results.

Raw Detour refs are technical references and can be discarded/rebuilt.

The runtime corridor stores enough project-owned metadata to:

- follow the route;
- identify required traversal links;
- detect stale/invalid segments;
- expose remaining path distance/cost;
- request replan when necessary.

## 10. Path Following

Path following produces desired locomotion intent before TA-5 physics.

It does not directly set world position.

Canonical flow:

```text
Path corridor
→ next steering target
→ local avoidance / traversal intent
→ TA-5 CharacterMotor / rigid movement
→ actual physical result
→ path progress update
```

## 11. Local Avoidance

Short-range avoidance is project-owned and consumes nearby actor/obstacle data from TA-7 spatial/runtime views.

Baseline goals:

- avoid simple actor collisions;
- yield at narrow passages;
- preserve formation spacing;
- avoid permanent corridor deadlocks;
- never bypass world collision or forbidden nav areas.

NPC-vs-NPC avoidance may temporarily soften mutual blocking according to GDS crew rules, but world/static collision remains authoritative.

## 12. Avoidance Priority

Local avoidance uses deterministic yielding priority based on semantic role plus stable identity tie-break.

Examples of higher priority can include:

- emergency responder carrying out life-critical task;
- actor already inside narrow traversal link;
- evacuation flow;
- player-controlled path safety.

Exact priority values are tuneable content/system data, but stable ordering is fixed.

## 13. Stuck Detection

Path follower tracks progress over Simulation Time.

Potential stuck conditions include:

- insufficient progress toward corridor goal;
- repeated collision/avoidance oscillation;
- traversal link not becoming usable;
- dynamic obstruction;
- actor displaced away from corridor.

Stuck detection cannot teleport the actor.

## 14. Stuck Recovery

Recovery escalation:

1. refresh local steering target;
2. request short local replan;
3. request full path replan;
4. yield/back off where safe;
5. report Blocked when no valid route remains;
6. technical safe reposition only under the strict GDS technical recovery rule and within the same authoritative logical region.

Technical recovery cannot cross security/hazard/topology boundaries.

## 15. Dynamic Target Following

For moving targets, the AI does not recompute a full path every tick.

A route is replanned when:

- target moved beyond authored replan threshold;
- current corridor no longer approaches valid interception/formation area;
- NavigationRevision invalidates remaining route;
- target crosses a traversal/security boundary;
- tactical intent changes.

## 16. Path to Cover / Tactical Position

Tactical systems can score candidate destinations first, then issue bounded path queries to top candidates.

Pathfinder itself does not decide which enemy to attack or which cover is tactically desirable.

## 17. Traversal-Link Execution

When a path reaches a typed link, path following yields control to the matching gameplay traversal executor.

Examples:

- door use;
- airlock cycle;
- elevator call/ride;
- ladder climb;
- boarding connector;
- authored jump/mantle.

On successful link completion, corridor following resumes from the resulting physical location.

On failure/block, path is revalidated/replanned.

## 18. Command Blocked vs NoPath

`NoPath` means the current navigation/query constraints cannot produce a route.

`Blocked` can mean a previously valid route is temporarily unable to progress, for example:

- lift occupied/unavailable;
- door temporarily closed pending access;
- friendly congestion;
- temporary hazard;
- repair/hack dependency.

Robot tactical commands expose the semantic blocker when known.

## 19. Path Cancellation

A path request/follower is cancelled when:

- owning command/task is replaced/cancelled;
- actor is Disabled/Incapacitated;
- actor leaves scene/deactivates;
- destination becomes semantically irrelevant;
- scene transition begins.

Pending worker results for cancelled requests are discarded by sequence validation.

## 20. Save / Load

Runtime path corridors and backend refs are reconstructible and need not be serialized.

Persistent AI state stores the semantic task/command, logical destination, and any gameplay-significant progress.

On load/activation, navigation requests reconstruct a valid current route.

Loading cannot teleport an actor to the old path's next waypoint.

## 21. Off-Screen Boundary

Off-screen actor travel does not require detailed navmesh following every 60 Hz tick.

TA-8 off-screen abstraction uses logical route/travel segments and the same access/traversal constraints. When reactivated, a valid physical position/path is reconstructed.

## 22. Diagnostics

Development diagnostics expose:

- current PathRequestId/state;
- source NavigationRevision;
- computed cost/length;
- current corridor/waypoints;
- current steering target;
- blocked reason;
- replan count/reason;
- worker latency;
- stale-result rejects.

Numeric performance budgets belong to TA-13.

## 23. Test Requirements

Tests cover stale SceneGeneration, stale actor generation, stale NavigationRevision, request replacement, worker completion reordering, moving target replans, blocked doors, stuck recovery, local avoidance deadlock, link execution failure, load reconstruction, and deterministic result consumption.

## 24. Explicit Non-Goals

No worker-side AI mutation, no direct transform teleport, no full replan every frame, no saved raw nav refs, no completion-order gameplay race, and no pathfinder-owned door/security permissions.

## 25. Dependencies

Depends on `52`, `53`, TA-1 workers, TA-5 locomotion, TA-7 runtime phases, and later TA-13 job budgets.

## 26. Open Questions

None.