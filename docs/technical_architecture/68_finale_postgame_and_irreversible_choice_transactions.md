# TA-9 — Finale, Postgame, and Irreversible Choice Transactions

> **Status:** Architecture Complete  
> **Authority:** MS-F02 final-choice staging/commit, capability validation, exactly-once irreversible story transition, postgame resolution state, route/faction/narrative consequences, and retry boundary

## 1. Purpose

The main-campaign finale must allow ordinary tactical failure and retry until the player explicitly commits one of the canonical choices:

- Stabilize;
- Sever;
- Contain.

The choice cannot become irreversible because a UI option was highlighted, a dialogue line played, or an animation began. It becomes irreversible only through one prepared cross-domain Final Resolution transaction.

## 2. Finale Mission Ownership

MS-F02 remains a normal canonical `MissionId` and deploys through the same `MissionInstanceId` architecture as other external missions.

Its authored objective phases remain:

```text
A — Reach Crownless
B — Gain Nexus Access
C — Stabilize Control Path
D — Final Resolution Choice
E — Survive / Complete Resolution
```

Raid/combat/AI systems can execute phases B/C/E, but none of them owns the irreversible campaign choice.

## 3. Choice Eligibility

The final decision interface becomes eligible only when the Objective graph has committed the prerequisites for Phase D.

Choice-specific requirements can include the Design Complete capability state such as:

- required Research / evidence;
- Continuance understanding/authentication;
- coalition commitments;
- Reach IV infrastructure;
- functioning final control interfaces.

Eligibility is checked from authoritative persistent state at commit preparation, not inferred from UI availability.

## 4. FinalChoiceIntent

Player selection first creates a typed `FinalChoiceIntent` containing:

- MissionId / MissionInstanceId;
- selected canonical choice;
- source objective/interface identity;
- source mission/objective revisions;
- player confirmation intent when required by presentation policy;
- CommandId.

The intent itself changes no world state.

## 5. Prepared Final Resolution

The finale resolver prepares a `FinalResolutionTransaction` before commit.

Preparation validates:

- the finale instance is still current/eligible;
- Phase D has not already committed;
- selected choice is one of the three authored canonical choices;
- choice-specific hard capability requirements are satisfied;
- required persistent world/control targets still exist in a valid state;
- no conflicting terminal Mission/failure transition has already committed;
- all participant consequence deltas can be prepared.

## 6. Participating Domains

The prepared transaction can contain typed deltas for:

- MissionStore / Objective graph;
- Narrative/Postgame state;
- World route/topology state;
- faction/reputation/coalition consequence records;
- Continuance relationship/state;
- strategic Dynamic Event / world scheduling state;
- persistent final-site target state;
- knowledge/progression flags;
- any explicit physical/control state change at Crownless.

Each domain remains owner of its fields. The finale transaction coordinates their atomic semantic commit.

## 7. Final Choice Transaction ID

The transaction receives a persistent TA-2 `TransactionId`.

The MissionRecord/Postgame state stores this ID as the unique irreversible choice commit marker.

Any later replay/duplicate command with the same or conflicting finale context is rejected because a terminal choice already exists.

## 8. Atomic Commit Boundary

The infallible final commit occurs on the authoritative main simulation thread at a Stable Simulation Boundary-compatible point.

During commit there is:

- no worker wait;
- no asset loading;
- no disk write dependency;
- no UI interaction;
- no network/service call;
- no fallible procedural generation.

Either all required prepared domain deltas commit or none do.

## 9. Canonical Choice State

Persistent `PostgameResolutionState` stores at minimum:

- selected ending: Stabilize / Sever / Contain;
- FinalResolution TransactionId;
- commit Simulation Time;
- relevant coalition/faction outcome snapshot references;
- postgame world-route policy/state references;
- main-campaign completion marker;
- postgame schema/version.

It is never reconstructed only from current route flags because those can evolve later in postgame.

## 10. Stabilize Consequence Contract

The Stabilize transaction prepares the authored controlled-anomaly route/infrastructure outcome and associated faction/narrative state.

It does not create a generic `good ending` score.

Required safeguards/capabilities are validated explicitly.

## 11. Sever Consequence Contract

The Sever transaction prepares the authored permanent disablement of anomaly-linked Transit Lattice architecture in the playable region and corresponding route/world/faction effects.

Conventional/home access required for postgame remains valid under the GDS postgame-softlock requirement.

## 12. Contain Consequence Contract

The Contain transaction prepares the authored isolation/guarded research state, route restrictions, and relevant Asterion/Continuance/world consequences.

Contain does not secretly behave as Stabilize or Sever; its persistent world policy is separately represented.

## 13. Phase E After Choice Commit

After the Final Resolution transaction commits, Phase E can still contain physical survival/escape/safe-resolution gameplay.

However the canonical final choice is already immutable in that save timeline.

A later tactical setback after the choice does not permit selecting a second ending within the same post-choice state.

Recovery behavior must preserve the committed ending and provide the authored completion/recovery path.

## 14. Failure Before Choice Commit

Ordinary tactical defeat before the final resolution transaction remains retryable under Story Mission recovery.

No ending is recorded.

Spent resources, destroyed robots, committed site damage, and other legitimate consequences remain under normal failure rules.

A new attempt gets a new MissionInstanceId while retaining the canonical MS-F02 MissionId and current persistent world state.

## 15. Failure During Choice Preparation

If capability/world validation fails before commit:

- no partial ending consequence applies;
- no FinalResolution TransactionId is persisted as committed;
- the objective/interface reports the actual blocker;
- the player can correct the blocker where the authored mission permits.

## 16. User Confirmation Boundary

TA-11 can provide a confirmation screen for an irreversible choice.

Confirmation UI still only emits the typed intent.

Closing/opening UI, focus changes, button animation, or dialogue state cannot commit the ending by themselves.

## 17. No Hidden Fourth Ending

The technical schema accepts the canonical closed enum only:

```text
Stabilize
Sever
Contain
```

Adding another ending requires accepted design/architecture change control and schema migration; content cannot smuggle an undocumented `Perfect` branch through arbitrary strings.

## 18. Main Campaign Completion

Campaign completion commits only after the final choice transaction and required Phase E safe-resolution condition satisfy the authored Mission success graph.

This distinction allows:

- choice immutable at D;
- physical resolution still ongoing at E;
- postgame transition only after mission success completion.

## 19. Postgame Transition

When MS-F02 resolves successfully:

- main campaign is marked complete;
- Horizon remains the same persistent player-owned station;
- current built station state is preserved;
- surviving player ships/robots/crew/resources remain the same identities/state;
- world route/faction state comes from committed final-choice consequences;
- remaining missions/events/research/raids can continue.

There is no automatic New Game+ or save reset.

## 20. Postgame Route Safety

Final consequence validation must prove that the selected authored route changes do not eliminate required access to Horizon or make baseline postgame impossible.

Closed anomaly shortcuts are explicit world-route state changes, not deletion of the strategic world graph.

## 21. Epilogue Inputs

Character/faction epilogues read persistent state such as:

- final choice;
- faction reputation/arcs;
- rescued specialists;
- restored routes/settlements;
- station development;
- evidence disclosure;
- relevant major-character state.

The epilogue presentation does not write these results back as new historical truth unless an explicit authored consequence transaction says so.

## 22. Save / Load

Before final choice commit, loading the same current save preserves no ending selection beyond any nonauthoritative UI draft.

After commit, loading preserves:

- selected choice;
- TransactionId;
- consequence state;
- current Phase E/postgame progress.

The same post-choice save cannot use the finale interface to commit another choice.

Returning to a user-selected older pre-choice save is a legitimate timeline rollback chosen by the player/persistence system, not an in-world undo transaction.

## 23. Idempotency

Every finale consequence adapter accepts the FinalResolution TransactionId/source and records enough history to reject duplicate application.

Examples:

- route change is not applied twice;
- reputation consequence is not doubled;
- Dynamic Event follow-up is not emitted twice;
- PostgameResolutionState is not recreated with a different choice.

## 24. Diagnostics

Development tools expose:

- current finale Mission/Instance;
- phase/objective prerequisites;
- available choice eligibility and blockers;
- prepared participant deltas;
- FinalResolution TransactionId;
- committed PostgameResolutionState;
- postgame route/faction consequence revisions.

Developer mutation requires explicit typed debug commands and cannot occur through a read-only inspector.

## 25. Test Requirements

Tests prove:

- final UI selection does not commit state;
- invalid capability prevents transaction preparation/commit;
- failure before commit records no ending;
- exactly one of three choices can commit;
- all required participant consequences are atomic;
- reload after commit cannot choose another ending;
- post-choice tactical failure does not erase/change ending;
- postgame preserves Horizon and persistent assets;
- route consequences retain required postgame access;
- duplicate FinalResolution command is idempotently rejected.

## 26. Explicit Non-Goals

No hidden perfect ending, UI-owned irreversible state, partial cross-domain ending commit, automatic New Game+, station reset, second ending from one post-choice save, or ending reroll after commit.

## 27. Dependencies

Depends on `61`/`62`/`67`, TA-2 prepared transactions/TransactionId, GDS MS-F02 execution, Endgame Narrative, World/Factions/Progression/Persistence, and later TA-11 presentation confirmation.

## 28. Open Questions

None in the finale/postgame transaction baseline.