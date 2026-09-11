# TA-8 — Decision Layers, Enemy Combat, and Tactical AI

> **Status:** Architecture Complete
> **Authority:** Generic active AI decision architecture, awareness-state execution, tactical goal/action selection, combat role doctrine, cover/search/retreat behavior, deterministic hysteresis, and difficulty boundary

## 1. Purpose

StarForge needs purposeful combat AI without building a general AI framework or allowing behavior code to bypass perception, navigation, combat, or mission authority.

## 2. Baseline Decision Architecture

Active AI uses explicit project-owned layered decision logic rather than a mandatory generic Behavior Tree/GOAP engine.

Canonical layers:

1. **Hard Safety / Capability Constraints**;
2. **Owning Command / Mission Task**;
3. **Awareness and Tactical Goal**;
4. **Role/Doctrine Action Selection**;
5. **Action Execution** through TA-7/TA-5/Combat contracts.

Higher layers constrain lower layers; action execution never silently changes the strategic task.

## 3. No Hidden AI Authority

AI can request/attempt actions, but owning systems still validate and commit:

- firing/ammunition;
- damage/status;
- doors/hacking/breaching;
- mission/objective completion;
- surrender/prisoner outcomes;
- reinforcement spawning;
- extraction;
- station/resource transactions.

## 4. Awareness State Machine

Enemy generic awareness states follow GDS-9:

```text
Unaware
→ Suspicious
→ Alert
→ Engaged
→ Searching
→ Unaware/Alert/Engaged

Any eligible state
→ Retreating
→ Disabled/Defeated
```

Transitions require explicit perception, memory, doctrine, combat, or mission facts.

## 5. Unaware Behavior

Unaware actors execute authored noncombat behavior such as:

- patrol;
- guard station;
- work/idle anchor;
- territorial movement.

They do not path toward the player's hidden location.

## 6. Suspicious Behavior

Suspicion is driven by incomplete evidence.

Typical actions:

- orient toward stimulus;
- move to bounded investigation point;
- check nearby route/cover;
- communicate suspicion where possible;
- escalate only when evidence threshold is reached.

The investigation target is derived from observation memory, not hidden world truth.

## 7. Alert Behavior

Alert actors know a credible threat exists but may not know its current location.

They can:

- ready weapon;
- move to defensive positions;
- raise local alarm if capable;
- share alert;
- guard likely routes;
- respond toward Last Known Position.

## 8. Engaged Behavior

An actor enters Engaged when it has sufficient actionable target information under its doctrine.

It can select tactical goals such as:

- attack current target;
- take/hold cover;
- advance;
- flank;
- defend objective;
- suppress Last Known Position;
- reload/resupply;
- reposition;
- retreat.

## 9. Searching Behavior

When current contact is lost:

- exact target tracking stops;
- Last Known Position/confidence drives search;
- candidate search positions come from known navigable routes/cover/observation points;
- search radius/priority evolves with Simulation Time and new evidence;
- failed search can decay back toward Alert/Unaware according to doctrine.

## 10. Retreating Behavior

Retreat can be selected by doctrine/morale/objective/damage facts.

A retreat goal includes a known/reachable fallback region or exit route.

AI does not disappear when retreating; it physically moves through valid navigation until owning mission/world rules resolve departure.

## 11. Tactical Role Profile

Each combat-capable actor definition references a `TacticalRoleProfile` describing preferences such as:

- preferred engagement distance;
- cover preference;
- aggression;
- flank preference;
- pursuit leash;
- objective-defense weight;
- target-class preferences;
- grenade/support willingness;
- retreat/morale policy;
- communication behavior.

This is authored content, not a universal class-level power value.

## 12. Tactical Goal Selection

Goal candidates are generated only from currently valid knowledge and capability.

Candidate scoring can consider:

- owning command/mission priority;
- immediate threat;
- current target knowledge confidence;
- distance/range fit;
- health/ammo/status;
- cover availability;
- objective relevance;
- ally state;
- doctrine.

The selected goal is deterministic for equal input using stable tie-breaking.

## 13. Hysteresis

AI does not recompute a new high-level goal every render frame.

Current goals have:

- minimum commitment where physically safe;
- switch threshold/hysteresis;
- explicit urgent interrupt conditions;
- cooldown where appropriate.

This prevents frame-by-frame target/cover oscillation.

## 14. Urgent Interrupts

Examples that can interrupt a normal decision interval:

- current target becomes invalid/defeated;
- imminent explosion/hazard recognized;
- actor becomes critically damaged;
- command changes;
- route becomes impossible;
- friendly crosses firing line;
- weapon becomes empty/jammed;
- objective state changes materially.

An interrupt still runs through ordinary validation/commit boundaries.

## 15. Action Execution

A selected tactical goal decomposes into typed runtime actions, for example:

- `MoveToAction`;
- `TakeCoverAction`;
- `AimAction`;
- `FireAction`;
- `ReloadAction`;
- `ThrowGrenadeAction`;
- `RaiseAlarmAction`;
- `SearchAreaAction`;
- `RetreatAction`;
- `WaitObserveAction`.

Actions have explicit start/continue/complete/block/fail conditions.

## 16. Firing Discipline

Before firing, AI validates:

- current target knowledge supports shot;
- physical line of fire;
- weapon readiness/ammo/energy;
- range/mode validity;
- friendly obstruction;
- doctrine/ROE;
- reaction/aim requirements.

The same GDS-9 weapon/hit/damage rules apply after the attack is issued.

## 17. Reaction Time

Newly acquired targets use authored/tuneable reaction and aim-acquisition timing.

Zero-delay firing is allowed only when the actor was already prepared/aimed at the relevant attack lane or a specific authored capability states otherwise.

Difficulty can tune approved timing but cannot bypass perception.

## 18. AI Aim

AI produces an aim intent with modeled error/tracking/recoil constraints.

It never submits a guaranteed-hit request.

TA-5/GDS-9 physical trajectory/hit resolution remains authoritative.

## 19. Cover Selection

Cover selection pipeline:

1. collect cover candidates near tactically relevant region;
2. filter by actor size/navigation reachability;
3. evaluate physical protection/peek geometry using TA-5 queries and current target knowledge;
4. account for flank/explosive exposure;
5. include route cost and role preference;
6. choose deterministically with hysteresis.

Cover data does not override actual projectile collision.

## 20. Flanking

A flank exists only if a valid navigable route reaches a position meaningfully different from the current engagement line.

AI cannot spawn/teleport behind the player.

Pursuit/flank routes respect mission leash, security, hazards, and actor traversal profile.

## 21. Suppression

Suppressive fire can target a credible Last Known Position/route under doctrine.

It:

- consumes real ammo;
- uses ordinary physical shots;
- does not track unseen movement;
- stops/replans when the tactical information becomes obsolete.

## 22. Grenades / Area Weapons

AI area-weapon use validates:

- available real ammunition/payload;
- target position confidence;
- throw/flight path;
- ally/self safety;
- blast geometry/range;
- tactical value;
- authored doctrine.

It cannot place an explosive directly at hidden player coordinates.

## 23. Target Selection

Target candidates come only from legitimate knowledge.

Scoring can include:

- immediate threat;
- mission/objective relevance;
- recent known damage source;
- current range/visibility;
- vulnerability known to the actor;
- doctrine.

Target hysteresis prevents rapid oscillation.

## 24. Morale / Surrender Boundary

TA-8 supports authored morale state for eligible human enemies.

Morale can drive Retreat/Surrender candidate goals when mission/content permits.

There is no universal random surrender roll and no blanket machine morale.

The owning mission/combat system validates the actual surrender outcome.

## 25. Wildlife

Wildlife uses a simplified subset:

- perception;
- territory/home region;
- approach/avoid;
- chase/attack;
- flee/return.

It does not need humanoid cover/firearm logic.

## 26. Machines

Machine enemies can use sensor-driven perception and role-specific tactics without biological morale.

EMP/system damage can change available sensors/actions through the same capability checks rather than a separate cheating AI mode.

## 27. Bosses

Boss AI can define authored state/action logic above this foundation, but remains bound by:

- perception/knowledge where relevant;
- physical attack telegraph/trajectory;
- navigation/movement;
- status/damage contracts;
- no unexplained perfect tracking.

## 28. Decision Cadence

High-level tactical selection can run at a lower cadence than 60 Hz.

Low-level action execution, aiming/movement intent, urgent interrupts, and safety validation participate in the appropriate TA-7 phases.

Scheduling cadence and budgets belong numerically to TA-13.

## 29. Deterministic Decision Ordering

Actors scheduled for decision on one tick are processed in stable actor order.

Candidate score ties use explicit semantic tie-break and stable target/action/position keys.

Random tactical choice is not required by the baseline. Any authored random variation must use TA-2 scoped deterministic RNG and may not depend on frame/worker timing.

## 30. Difficulty Boundary

Difficulty may tune GDS-approved parameters such as:

- reaction time;
- aim error;
- aggression;
- coordination;
- cover preference;
- search persistence.

Difficulty cannot:

- add omniscience;
- guarantee hits;
- ignore ammo;
- ignore navigation;
- ignore physical cover;
- bypass friendly-fire rules.

## 31. Persistence

Gameplay-significant active AI state can persist/reconstruct:

- awareness state;
- current semantic goal/task;
- target identity if legitimately known;
- Last Known Position/confidence;
- morale state where relevant;
- current action milestone if saveable;
- combat ammo/health/status via owning domains.

Raw behavior function pointers, path refs, candidate arrays and temporary scores are reconstructible.

## 32. Diagnostics

Debug tools expose:

- awareness state;
- current goal/action;
- candidate goals and scores;
- target knowledge source/confidence;
- selected cover/path;
- reaction timer;
- blocker/failure cause;
- doctrine profile;
- decision/interrupt history.

## 33. Test Requirements

Tests cover no hidden target use, awareness transitions, Last Known Position search, goal hysteresis, reaction delay, friendly-fire avoidance, ammo exhaustion, cover physically blocked, flank reachability, retreat, difficulty no-cheat invariants, worker/path failure, and deterministic tie resolution.

## 34. Explicit Non-Goals

No mandatory generic behavior-tree editor, GOAP planner, machine-learning runtime, omniscient blackboard, guaranteed-hit AI, cover teleport, hidden-player flank, or render-frame decision authority.

## 35. Dependencies

Depends on `52`–`55`, TA-7 combat/runtime phases, GDS-9 Enemy Combat Behavior, GDS-12 Difficulty, and TA-9 mission/raid context for later strategic objectives.

## 36. Open Questions

None.