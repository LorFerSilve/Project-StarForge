# TA-8 Cross-Validation — AI and Navigation Architecture

> **Status:** PASS
> **Scope:** TA-8 files `52`–`60` against TA-0 through TA-7 and Design Complete Crew, Combat, Robots, Station, Missions, Player, and Systems contracts

## 1. Navigation Representation

| # | Check | Result |
|---|---|---|
| 1 | Ground navigation is bounded/local rather than galaxy-scale | PASS |
| 2 | Recast/Detour is behind StarForge navigation adapter | PASS |
| 3 | Raw Detour refs are not persistent/gameplay identity | PASS |
| 4 | Ground navigation uses tiled 2.5D navmesh | PASS |
| 5 | Free-flight/Zero-G navigation is not projected onto floor navmesh | PASS |
| 6 | Free-flight navigation uses bounded 3D graph/volume | PASS |
| 7 | Small/Standard/Heavy navigation classes exist | PASS |
| 8 | Actor-specific capability remains in traversal profile | PASS |
| 9 | Navigation does not grant door/security authority | PASS |
| 10 | Navigation does not complete mission/task outcomes | PASS |
| 11 | Destination projection cannot cross forbidden geometry/boundary | PASS |
| 12 | Local avoidance is distinct from global pathfinding | PASS |

## 2. Traversal Profiles and Links

| # | Check | Result |
|---|---|---|
| 13 | Traversal profile includes body clearance/step/slope | PASS |
| 14 | Traversal profile includes mobility type | PASS |
| 15 | Traversal profile includes door/airlock capability | PASS |
| 16 | Traversal profile includes environmental protection inputs | PASS |
| 17 | Heavy Mech can be excluded from narrow/light routes | PASS |
| 18 | Heavy pathing can require authored load-bearing compatibility | PASS |
| 19 | Door traversal is a typed link, not instant polygon crossing | PASS |
| 20 | Airlock traversal is multi-step physical gameplay | PASS |
| 21 | Elevator traversal physically waits/rides | PASS |
| 22 | Ladder traversal requires actor capability | PASS |
| 23 | Dock/boarding traversal requires actual committed connection | PASS |
| 24 | Breach link enables only after owning breach state commits | PASS |

## 3. Dynamic Topology / Invalidation

| # | Check | Result |
|---|---|---|
| 25 | Persistent gameplay topology commits before navigation mutation | PASS |
| 26 | Door/security changes can use dynamic overlays without recook | PASS |
| 27 | Geometry-changing construction invalidates bounded tiles | PASS |
| 28 | Destruction/repair invalidates/restores appropriate navigation | PASS |
| 29 | Navigation tile identity is project-owned, not memory address | PASS |
| 30 | Async rebuild consumes immutable geometry snapshot | PASS |
| 31 | Stale SceneGeneration rebuild is rejected | PASS |
| 32 | Stale GeometryRevision rebuild is rejected | PASS |
| 33 | Multi-tile correctness update can commit atomically | PASS |
| 34 | Existing paths revalidate after NavigationRevision change | PASS |
| 35 | Old planned path cannot cross newly locked door | PASS |
| 36 | Streaming residency does not itself grant traversal | PASS |

## 4. Async Pathfinding

| # | Check | Result |
|---|---|---|
| 37 | Path requests carry actor generation | PASS |
| 38 | Path requests carry SceneGeneration | PASS |
| 39 | Path requests carry NavigationRevision | PASS |
| 40 | Path requests carry command/task generation | PASS |
| 41 | Worker path jobs are read-only/advisory | PASS |
| 42 | Worker cannot move actor | PASS |
| 43 | Worker cannot open door/commit task | PASS |
| 44 | Ready results are consumed in deterministic actor/request order | PASS |
| 45 | Worker completion order cannot decide gameplay | PASS |
| 46 | Request replacement invalidates old result | PASS |
| 47 | Path following emits locomotion intent rather than teleport | PASS |
| 48 | Moving targets do not force full replan every tick | PASS |
| 49 | Traversal-link execution returns to corridor only after physical result | PASS |
| 50 | NoPath and temporary Blocked are distinct | PASS |

## 5. Local Avoidance / Stuck Recovery

| # | Check | Result |
|---|---|---|
| 51 | Local avoidance preserves world collision | PASS |
| 52 | Local avoidance cannot enter forbidden nav area | PASS |
| 53 | Narrow-passage yielding is deterministic | PASS |
| 54 | Crew collision rules can soften NPC-NPC blocking only | PASS |
| 55 | Stuck detection uses Simulation-Time progress | PASS |
| 56 | First stuck response is steering/replan, not teleport | PASS |
| 57 | Technical recovery cannot bypass security | PASS |
| 58 | Technical recovery cannot bypass hazards/topology | PASS |
| 59 | Dynamic obstacle can produce replan/Blocked state | PASS |
| 60 | Path/load reconstruction does not snap actor to stale waypoint | PASS |

## 6. Perception / Knowledge

| # | Check | Result |
|---|---|---|
| 61 | Authoritative world truth is separated from AI knowledge | PASS |
| 62 | Target selection uses legitimate knowledge | PASS |
| 63 | Vision uses FOV/distance/TA-5 occlusion | PASS |
| 64 | Renderer visibility/culling is not AI perception authority | PASS |
| 65 | Hearing uses semantic NoiseFacts, not final audio mix | PASS |
| 66 | Hearing need not reveal exact source identity/location | PASS |
| 67 | Specialized sensors require explicit equipment semantics | PASS |
| 68 | Damage does not automatically perfectly reveal attacker | PASS |
| 69 | Memory stores Last Known Position and confidence | PASS |
| 70 | Confidence decays on Simulation Time | PASS |
| 71 | Unseen moving target does not update Last Known Position | PASS |
| 72 | Search area derives from known evidence, not hidden target | PASS |
| 73 | Shared knowledge retains source/timestamp/provenance | PASS |
| 74 | Shared knowledge cannot improve precision beyond source | PASS |
| 75 | Communication degradation/disconnection limits sharing | PASS |
| 76 | Save/load grants no free perception scan | PASS |

## 7. Enemy Tactical AI

| # | Check | Result |
|---|---|---|
| 77 | GDS awareness states map to explicit runtime states | PASS |
| 78 | Unaware AI does not move toward hidden player | PASS |
| 79 | Suspicious investigation uses evidence location | PASS |
| 80 | Alert can prepare/guard without current exact target | PASS |
| 81 | Engaged tactics use current knowledge/capability | PASS |
| 82 | Searching uses Last Known Position and confidence | PASS |
| 83 | Retreat is physical navigation, not disappearance | PASS |
| 84 | Tactical goal selection is deterministic for equal input | PASS |
| 85 | Hysteresis prevents frame-by-frame goal oscillation | PASS |
| 86 | Reaction delay is Simulation-Time based | PASS |
| 87 | AI aim is modeled, never guaranteed hit | PASS |
| 88 | Cover selection uses real physical protection queries | PASS |
| 89 | Flanking requires valid navigable route | PASS |
| 90 | Suppression consumes real ammo | PASS |
| 91 | Grenade use validates path/ally safety/ammo | PASS |
| 92 | Difficulty cannot grant omniscience/physics bypass | PASS |

## 8. Robot Command AI

| # | Check | Result |
|---|---|---|
| 93 | Robot safety/capability constraints precede commands | PASS |
| 94 | Valid player command precedes routine role autonomy | PASS |
| 95 | Command states preserve GDS Issued→Accepted→Executing outcomes | PASS |
| 96 | New primary command invalidates old command generation | PASS |
| 97 | Follow formation physically navigates | PASS |
| 98 | Formation can collapse in narrow corridor | PASS |
| 99 | Hold Position respects hold envelope | PASS |
| 100 | Focus Fire changes priority, not damage/accuracy | PASS |
| 101 | Return to Transport completes only after physical securing | PASS |
| 102 | Repair requires real parts/capability/access | PASS |
| 103 | Hack requires valid physical/data interface | PASS |
| 104 | Shield remains geometry/energy limited | PASS |
| 105 | Breach/demolition require explicit authorized target | PASS |
| 106 | ROE constrains firing without changing knowledge truth | PASS |
| 107 | Disconnected robot accepts no new live command | PASS |
| 108 | Disconnected robot uses configured fallback only | PASS |
| 109 | Reconnection does not replay obsolete commands | PASS |
| 110 | Robot AI cannot initiate mission extraction/raid/story choice | PASS |

## 9. Crew / Station Task AI

| # | Check | Result |
|---|---|---|
| 111 | TA-6 remains owner of AutomationTask/WorkOrder | PASS |
| 112 | Crew AI cannot permanently change assignment by itself | PASS |
| 113 | Local-presence work requires physical travel | PASS |
| 114 | Remote work requires valid ControlData/sensors | PASS |
| 115 | Work points are anchors, not skill authority | PASS |
| 116 | Required equipment/resources cannot be spawned by AI | PASS |
| 117 | Crew respects door/airlock/security interlocks | PASS |
| 118 | Known lethal hazards block routine unprotected crew | PASS |
| 119 | Emergency responders require actual protection/policy | PASS |
| 120 | Blocked local task reports actual blocker to TA-6 | PASS |
| 121 | Muster point must still be currently safe/reachable | PASS |
| 122 | Boarding does not turn generic crew into assault troops | PASS |
| 123 | Casualty rescue is physical/reachable | PASS |
| 124 | Player body blocking cannot create permanent automation softlock | PASS |

## 10. Scheduling / Off-Screen AI

| # | Check | Result |
|---|---|---|
| 125 | AI detail modes keep one authority per facet | PASS |
| 126 | ReducedLocal changes cadence, not gameplay rules | PASS |
| 127 | OffScreenLogical stores logical location/task/progress | PASS |
| 128 | Off-screen travel has real Simulation-Time duration | PASS |
| 129 | Off-screen routes respect doors/access/hazards/Heavy compatibility | PASS |
| 130 | Off-screen advance stops at earlier significant event boundary | PASS |
| 131 | TA-6 task ownership remains persistent off-screen | PASS |
| 132 | Active↔off-screen uses Activation Lease handoff | PASS |
| 133 | Reactivation position cannot provide tactical shortcut | PASS |
| 134 | TA-8 does not invent generic hidden combat resolver | PASS |
| 135 | AIScheduler schedules due work but does not own AI state | PASS |
| 136 | Same-tick AI scheduling uses stable actor order | PASS |
| 137 | Urgent updates cannot be silently dropped under budget pressure | PASS |
| 138 | Worker results can finish during pause but not advance gameplay | PASS |
| 139 | True Pause/Hard Streaming Hold freeze AI Simulation-Time progress | PASS |
| 140 | Save/load adds no wall-clock AI/travel progress | PASS |

## 11. Runtime Phase / Authority Integration

| # | Check | Result |
|---|---|---|
| 141 | Async results are consumed before due AI decisions with freshness checks | PASS |
| 142 | AI decisions occur before movement/weapon intent construction | PASS |
| 143 | AI intents use same TA-5/Combat execution paths as player intents | PASS |
| 144 | AI cannot mutate from physics callbacks | PASS |
| 145 | Current-tick perception facts do not rewind completed physics | PASS |
| 146 | Consequence commit precedes AI assumption of success | PASS |
| 147 | Urgent interrupts are queued, not callback-executed immediately | PASS |
| 148 | Navigation mutation occurs at deferred safe boundary | PASS |
| 149 | Activation gives no free omniscient perception scan | PASS |
| 150 | Deactivation invalidates live async requests | PASS |
| 151 | Candidate tie resolution uses stable keys | PASS |
| 152 | Random AI variation, if authored, uses scoped TA-2 RNG | PASS |

## 12. Persistence / Debug / Future Boundaries

| # | Check | Result |
|---|---|---|
| 153 | Raw nav/path/backend refs are excluded from saves | PASS |
| 154 | Gameplay-relevant AI memory/command/task state can persist | PASS |
| 155 | Headless AI/navigation execution requires no OpenGL/audio | PASS |
| 156 | Debug rendering is read-only consumer of AIDebugSnapshot | PASS |
| 157 | Debug mutation uses typed simulation commands | PASS |
| 158 | TA-9 remains owner of mission/raid/dynamic-event strategic state | PASS |
| 159 | TA-10 remains owner of final nav-content cooking/build pipeline | PASS |
| 160 | TA-11 remains owner of raw input/shipping UI/audio presentation | PASS |
| 161 | TA-13 remains owner of numeric AI/path/perception budgets | PASS |
| 162 | TA-14 remains owner of final CI/test gate implementation | PASS |

---

## 13. Final Result

```text
TA-8 — AI and Navigation Architecture
Status: ARCHITECTURE COMPLETE
Cross-validation: PASS
Checks: 162 / 162 PASS
Blocking contradictions: 0
Implementation-critical open questions: 0
```

TA-9 — Missions, Raids, Dynamic Events, and Strategic State Machines is the next dependency.