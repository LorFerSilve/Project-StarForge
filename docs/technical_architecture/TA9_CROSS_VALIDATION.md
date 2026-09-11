# TA-9 — Cross-System Architecture Validation

> **Status:** PASS  
> **Scope:** `61`–`69` against TA-0..TA-8 and the Design Complete Missions, Raids, Dynamic Events, Communications, Failure/Recovery, World, Narrative, Economy/Progression, Resources, Crew, Robots, Spacecraft, Station, and Persistence contracts

## 1. Validation Method

TA-9 was checked for:

- single authoritative owner per state;
- typed persistent identity and lifetime;
- deterministic ordering/RNG;
- exactly-once transitions and rewards/consequences;
- physical ownership conservation;
- active/off-screen equivalence;
- Simulation-Time-only progression;
- player-knowledge/communication boundaries;
- mission/raid/event state-machine compatibility;
- retry/failure/recovery behavior;
- campaign softlock protection;
- Stable Save Boundary compatibility;
- headless testability;
- future-phase authority boundaries.

A PASS means no implementation-critical contradiction remains for the checked contract.

---

## 2. Identity, Authority, and Store Boundaries

| # | Check | Result |
|---:|---|:---:|
| 1 | MissionId remains persistent mission identity | PASS |
| 2 | MissionInstanceId is separate persistent attempt identity | PASS |
| 3 | MissionInstanceId is not RuntimeEntityHandle | PASS |
| 4 | Retry never reuses a prior MissionInstanceId | PASS |
| 5 | RaidId remains separate from MissionId while referencing it | PASS |
| 6 | Offensive Raid does not create a second mission engine | PASS |
| 7 | DefenseEventId is independent Home Station Event identity | PASS |
| 8 | DynamicEventId owns strategic event identity only | PASS |
| 9 | DynamicEvent does not mirror linked Mission objective/deployment state | PASS |
| 10 | DynamicEvent does not mirror DefenseEvent tactical phase state | PASS |
| 11 | Persistent target/world state remains owned by target/world domains | PASS |
| 12 | RaidRecord does not duplicate target system authority | PASS |
| 13 | Mission runtime Scene is projection, not mission persistence authority | PASS |
| 14 | Objective UI is not objective authority | PASS |
| 15 | Event notification UI is not event authority | PASS |
| 16 | Reinforcement response cannot be created by AI callback alone | PASS |
| 17 | Strategic consequence router coordinates but does not own participant domains | PASS |
| 18 | PostgameResolutionState is persistent and separate from presentation | PASS |
| 19 | Persistent IDs follow TA-2 nonreuse/atomic-allocation rules | PASS |
| 20 | Runtime handles/raw backend references are excluded from persistent strategic identity | PASS |

## 3. Mission Lifecycle and Deployment

| # | Check | Result |
|---:|---|:---:|
| 21 | Canonical GDS Mission lifecycle is preserved | PASS |
| 22 | Multiple Accepted missions can coexist | PASS |
| 23 | At most one external MissionInstance can be deployed | PASS |
| 24 | External deployment exclusivity is transactionally enforced | PASS |
| 25 | Horizon DefenseEvent does not consume external deployment slot | PASS |
| 26 | Deployment allocates MissionInstanceId atomically with record creation | PASS |
| 27 | Failed deployment creates no orphan MissionInstanceId | PASS |
| 28 | Prepared deployment is revalidated before commit | PASS |
| 29 | Hard Block cannot be overridden by UI | PASS |
| 30 | Warning can be accepted without changing physical feasibility | PASS |
| 31 | En Route uses owning strategic travel authority | PASS |
| 32 | Mission state cannot teleport player/ship during travel | PASS |
| 33 | MissionInstance persists zone/objective/hazard/POI state | PASS |
| 34 | Multi-zone transition preserves MissionInstance identity | PASS |
| 35 | Scene unload cannot resolve or erase MissionRecord | PASS |
| 36 | Success requires authored objective/extraction conditions | PASS |
| 37 | Loot pickup alone cannot imply mission success | PASS |
| 38 | Killing all enemies is not universal success condition | PASS |
| 39 | Abandonment does not teleport player to safety | PASS |
| 40 | Abandonment preserves consumed/committed physical consequences | PASS |
| 41 | Resolution settles mandatory consequences before Resolved | PASS |
| 42 | Resolution uses idempotency marker/TransactionId | PASS |
| 43 | Resolved procedural MissionId cannot reopen as same opportunity | PASS |
| 44 | Accepted mission has no hidden expiry unless explicit contract permits it | PASS |
| 45 | Stable Save Boundary cannot hold dangling active external instance reference | PASS |

## 4. Retry, Failure, and Attempt History

| # | Check | Result |
|---:|---|:---:|
| 46 | Story tactical failure remains retryable/recoverable | PASS |
| 47 | Retry uses same canonical story MissionId | PASS |
| 48 | Retry creates new MissionInstanceId | PASS |
| 49 | Failed instance remains historical rather than overwritten | PASS |
| 50 | Retry does not reroll forbidden major content | PASS |
| 51 | Retry does not refund ammunition | PASS |
| 52 | Retry does not refund fuel/propellant | PASS |
| 53 | Retry does not resurrect Destroyed robots | PASS |
| 54 | Retry does not undo persistent target damage | PASS |
| 55 | Retry does not undo committed physical transfers | PASS |
| 56 | Explicit authored tutorial/reset exception requires bounded declared transaction | PASS |
| 57 | Mission failure preserves independent committed world changes | PASS |
| 58 | Failure screen itself cannot remove resources | PASS |
| 59 | Mandatory campaign content retains recovery/alternate path | PASS |
| 60 | Unique mandatory object cannot respawn as duplicate | PASS |

## 5. Objective Graph and Exactly-Once Semantics

| # | Check | Result |
|---:|---|:---:|
| 61 | Objective IDs are unique within Mission | PASS |
| 62 | Objective definitions are separate from runtime records | PASS |
| 63 | Objective graph is DAG | PASS |
| 64 | Direct/indirect cycles are rejected | PASS |
| 65 | AND dependencies are explicit | PASS |
| 66 | OR dependencies are explicit | PASS |
| 67 | Exclusive branch selection is transactionally recorded | PASS |
| 68 | Branch choice is not worker/UI/hash-order dependent | PASS |
| 69 | Locked objectives unlock only after prerequisites commit | PASS |
| 70 | Completion consumes committed facts/read state | PASS |
| 71 | Raw physics callback cannot complete objective | PASS |
| 72 | UI progress cannot complete objective | PASS |
| 73 | Source-domain state is preferred over duplicate shadow counters | PASS |
| 74 | Objective-owned progress is used only where intrinsically required | PASS |
| 75 | Completion is exactly once | PASS |
| 76 | Failure is exactly once | PASS |
| 77 | Terminal objective consequence cannot replay after load | PASS |
| 78 | Optional failure does not fail mission by itself | PASS |
| 79 | Primary failure checks alternate branch before mission failure | PASS |
| 80 | Cancelled is distinct from Failed | PASS |
| 81 | Combat objective specifies Kill/Destroy/Disable/DriveOff/Capture | PASS |
| 82 | Resource objective specifies security state | PASS |
| 83 | Rescue distinguishes Boarded/Secured from proximity | PASS |
| 84 | Repair completes only at required operational state | PASS |
| 85 | Scan completes only after evidence/data commit | PASS |
| 86 | Timed objectives use Simulation Time | PASS |
| 87 | True Pause freezes objective timer | PASS |
| 88 | Mandatory active timer cannot be hidden from permitted player knowledge | PASS |
| 89 | Hidden objective does not leak marker/target via read model | PASS |
| 90 | Extraction objective requires authoritative extraction commit | PASS |

## 6. Procedural Mission Generation

| # | Check | Result |
|---:|---|:---:|
| 91 | Procedural generation uses TA-2 PCG32 scoped streams | PASS |
| 92 | Wall-clock time is not generation seed | PASS |
| 93 | Worker completion order cannot select candidate | PASS |
| 94 | Candidate attempt order is deterministic | PASS |
| 95 | Generation input uses immutable revisioned snapshot | PASS |
| 96 | Commit revalidates generation snapshot revisions | PASS |
| 97 | Invalid world/biome/faction/Reach combination is rejected | PASS |
| 98 | Invalid objective DAG is rejected | PASS |
| 99 | Impossible access/objective/extraction candidate is rejected | PASS |
| 100 | Required survivor/cargo extraction feasibility is validated | PASS |
| 101 | Rejected candidate consumes no persistent gameplay IDs | PASS |
| 102 | Persistent child IDs allocate atomically with valid mission commit | PASS |
| 103 | Unique named/story/knowledge content cannot duplicate | PASS |
| 104 | Story mandatory location/character/evidence cannot be randomized away | PASS |
| 105 | Available procedural offer major content is stable | PASS |
| 106 | Save/load cannot reroll Available mission | PASS |
| 107 | Decline does not regenerate same MissionId | PASS |
| 108 | Generator does not universally scale to player equipment | PASS |
| 109 | Bounded generation failure shows no broken offer | PASS |
| 110 | DynamicEvent-to-Mission link commits atomically with MissionId | PASS |

## 7. Offensive Raid Architecture

| # | Check | Result |
|---:|---|:---:|
| 111 | Raid is specialized MissionRecord/MissionInstance | PASS |
| 112 | Generic Mission lifecycle remains authoritative around raid phases | PASS |
| 113 | Raid phase is orthogonal to escalation state | PASS |
| 114 | Raid phase graph supports legitimate phase skipping | PASS |
| 115 | Phase skip still requires real authored access route | PASS |
| 116 | Raid phase transition cannot teleport player/robots | PASS |
| 117 | Raid target state comes from persistent target/world owner | PASS |
| 118 | Destroyed turret remains destroyed after raid failure | PASS |
| 119 | Depleted/stolen target storage remains depleted according to ownership | PASS |
| 120 | Sabotage commits explicit subsystem state | PASS |
| 121 | Raid phase alone cannot grant sabotage success | PASS |
| 122 | Raid phase alone cannot grant physical loot | PASS |
| 123 | No global Raid HP bar is authoritative | PASS |
| 124 | Defender assets are finite/persistent | PASS |
| 125 | Zone reload cannot respawn defeated defenders | PASS |
| 126 | Multi-zone raid preserves objective/escalation/loot state | PASS |
| 127 | Withdrawal is distinct from extraction | PASS |
| 128 | Raid failure preserves spent resources/robot loss | PASS |
| 129 | New retry observes current damaged/recovered target state | PASS |
| 130 | Target recovery uses later Simulation-Time world process, not raid reset | PASS |

## 8. Reinforcement and Escalation

| # | Check | Result |
|---:|---|:---:|
| 131 | Escalation uses canonical GDS states | PASS |
| 132 | Escalation is distinct from local AI awareness | PASS |
| 133 | Escalation is distinct from station Security Alarm | PASS |
| 134 | Observed escalation trigger has deterministic authored effect | PASS |
| 135 | Reinforcement requires plausible source | PASS |
| 136 | Reinforcement requires valid communication/call authority | PASS |
| 137 | Reinforcement requires valid route/reach | PASS |
| 138 | Calling can be interrupted before commit | PASS |
| 139 | Call commit atomically reserves/creates finite response manifest | PASS |
| 140 | Same source capacity cannot dispatch twice | PASS |
| 141 | Post-commit communication loss does not unsend force | PASS |
| 142 | Valid external cause may cancel response before/according to authored transit state | PASS |
| 143 | Response ETA uses Simulation Time | PASS |
| 144 | True Pause stops reinforcement ETA | PASS |
| 145 | Reload cannot reset ETA | PASS |
| 146 | Arrival requires valid local entry route | PASS |
| 147 | Reinforcement cannot spawn in sealed room | PASS |
| 148 | Wave/response budget is finite | PASS |
| 149 | Full Response cannot invent endless stronger waves | PASS |
| 150 | Committed procedural response composition cannot reroll on load | PASS |

## 9. Horizon DefenseEvent

| # | Check | Result |
|---:|---|:---:|
| 151 | Dynamic Events owns DefenseEvent creation eligibility | PASS |
| 152 | DefenseEvent owns execution after DefenseEventId commit | PASS |
| 153 | Defense phases match GDS-11 baseline | PASS |
| 154 | DefenseEvent can coexist with one external deployed MissionInstance | PASS |
| 155 | Player-away defense uses same persistent Horizon stores | PASS |
| 156 | No second hidden full Horizon scene is required | PASS |
| 157 | Off-screen defense consumes actual power/ammo/cooling/sensors/topology | PASS |
| 158 | No single Defense Score is authoritative | PASS |
| 159 | Coarse defense outcomes map to concrete GDS state | PASS |
| 160 | Off-screen resolution is deterministic for same state/seed/policies/time | PASS |
| 161 | Attackers obey knowledge boundary | PASS |
| 162 | Boarding requires physical/access route | PASS |
| 163 | Hostile loot uses physical owner chain | PASS |
| 164 | Stolen cargo remains recoverable before hostile extraction | PASS |
| 165 | Permanent theft commits only at valid hostile extraction | PASS |
| 166 | Routine defense cannot delete Horizon identity/Command Core | PASS |
| 167 | Routine defense cannot permanently kill recruited crew | PASS |
| 168 | Routine defense cannot delete committed Knowledge Assets | PASS |
| 169 | Player return joins current persistent phase, no restart | PASS |
| 170 | Defense resolution emits Recovery Grace trigger exactly once | PASS |

## 10. Dynamic Events, Communications, Recovery, Finale, and Future Boundaries

| # | Check | Result |
|---:|---|:---:|
| 171 | Dynamic Event lifecycle uses Candidate/Offered/Scheduled/Active/terminal states | PASS |
| 172 | Rejected candidate creates no persistent player-visible event | PASS |
| 173 | Dynamic scheduler uses Simulation Time only | PASS |
| 174 | Maximum three optional external offers is enforceable | PASS |
| 175 | Maximum one active/scheduled Critical Horizon event is enforceable | PASS |
| 176 | Recovery Grace blocks ordinary procedural Horizon attacks | PASS |
| 177 | Recovery Grace pauses during Critical Recovery | PASS |
| 178 | Event source/world plausibility is mandatory | PASS |
| 179 | Procedural event cannot permanently softlock campaign | PASS |
| 180 | Event existence is separate from player knowledge | PASS |
| 181 | Communication loss does not pause event/Defense simulation | PASS |
| 182 | Delayed message retains original source timestamp | PASS |
| 183 | Remote order requires valid link + receiving capability | PASS |
| 184 | Duplicate delayed remote order cannot commit twice | PASS |
| 185 | External defeat Recovery Transit is not successful extraction | PASS |
| 186 | Recovery Transit respects strategic anti-teleport timing | PASS |
| 187 | Recovery does not teleport Primary Ship/robots/cargo by association | PASS |
| 188 | Final choice intent alone is not irreversible commit | PASS |
| 189 | Final Stabilize/Sever/Contain transaction is atomic/exactly once | PASS |
| 190 | Post-choice save cannot commit another ending; TA-10/11/12/13/14 retain their future authorities | PASS |

---

## 11. Authority Boundary Review

TA-9 leaves the following work to later phases without ambiguity:

- **TA-10 Content and Asset Pipeline:** final content schemas, mission/raid/event authored data files, procedural module packaging, nav/collision content cooking, asset registry/dependency validation;
- **TA-11 Input/UI/Audio/Presentation:** mission log/HUD, choice confirmation UI, event notifications, communication-state presentation, recovery/failure screens, alarm/audio feedback;
- **TA-12 Persistence Implementation:** exact binary encoding/container implementation, registry, crash recovery, migrations and load diagnostics for these TA-9 DTOs;
- **TA-13 Performance/Concurrency:** numeric strategic scheduler cadence, procedural worker budgets, objective-evaluation budgets, off-screen DefenseEvent granularity/backlog limits;
- **TA-14 Testing/CI:** implementation of the required deterministic/headless test suites and CI gates;
- **TA-15:** final cross-architecture integration audit;
- **TA-16:** implementation locking/roadmap.

No later phase is permitted to change TA-9 gameplay semantics merely as an implementation convenience.

## 12. Final Result

```text
TA-9 — Missions, Raids, Dynamic Events, and Strategic State Machines
Status: ARCHITECTURE COMPLETE
Cross-validation: PASS
Checks: 190 / 190 PASS
Blocking contradictions: 0
Implementation-critical open questions: 0
```

**Next dependency:** TA-10 — Content and Asset Pipeline.