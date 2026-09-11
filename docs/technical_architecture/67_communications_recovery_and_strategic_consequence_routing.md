# TA-9 — Communications, Recovery, and Strategic Consequence Routing

> **Status:** Architecture Complete  
> **Authority:** Event existence versus delivered knowledge, queued strategic messages/orders, external-defeat Recovery Transit, recovery-destination selection, strategic consequence orchestration, and cross-event handoff without physical bypass

## 1. Purpose

Missions, Horizon attacks, Dynamic Events, recovery, and world consequences can all continue while the player is elsewhere or unaware.

TA-9 therefore separates:

- what has actually happened in authoritative state;
- what information has reached the player;
- what remote command has actually been delivered/accepted;
- where the player/ship/robots/resources physically remain after failure;
- which owning domain commits each resulting consequence.

## 2. Event Existence Is Not Player Knowledge

A `MissionId`, `DefenseEventId`, or `DynamicEventId` can exist and progress without being known to the player.

The strategic layer never exposes existence merely because a persistent record exists.

Knowledge requires an actual source/delivery path defined by the communication authority.

## 3. Strategic Message Record

Gameplay-relevant delayed/queued communication uses a persistent or save-reconstructable `StrategicMessageRecord` containing conceptually:

- source persistent reference/event;
- source Simulation-Time timestamp;
- message type;
- knowledge payload/fidelity class;
- sender/recipient/context;
- communication-path requirement;
- state: Queued / InTransit if delayed / Delivered / Invalidated;
- delivery Simulation Time where relevant;
- staleness/source revision metadata;
- correlation TransactionId.

The record is information transfer, not a second copy of the source event.

## 4. Strategic Link Evaluation

Communication availability is read from the GDS states:

- Available;
- Degraded;
- Unavailable.

TA-9 requests delivery through the communication layer. It does not assume universal connectivity.

A Degraded link can explicitly support only a subset such as summary telemetry, delayed messages, or no remote orders.

## 5. Horizon Attack Notification

A remote Horizon Defense alert requires both:

1. Horizon actually detecting/knowing enough about the attack through station sensors/security;
2. a valid strategic communication path to the player.

If either is absent, the DefenseEvent continues but no live warning is generated.

A later queued delivery retains the original event timestamp and reports stale/current status accurately.

## 6. Dynamic Event Offers

`66` can commit an Offered DynamicEvent before the player receives its information.

The player-facing offer appears only after delivery commits.

Expiration continues according to the event's Simulation-Time deadline even while the player is unaware if that is the authored world behavior.

The UI later distinguishes current versus delayed information; TA-11 owns presentation.

## 7. Remote Orders

Remote order execution follows:

```text
Player strategic intent
 -> communication-path validation
 -> message/order delivery
 -> receiving automation/domain validation
 -> owning-domain command
 -> commit or explicit rejection
```

A UI click while the link is unavailable does not directly mutate Horizon.

Orders can never:

- repair without actor/resources;
- close a destroyed door;
- create sensor knowledge;
- teleport robots/resources;
- fire first-person systems across strategic distance without an explicit supported control system.

## 8. Remote Order Identity / Idempotency

A gameplay-significant remote order carries CommandId/Transaction correlation and delivery state.

If delivery is delayed/retried, the receiving system can recognize a previously committed order rather than applying it twice.

Obsolete orders are rejected according to target revision/precondition rather than blindly replayed when contact returns.

## 9. External Mission Defeat Boundary

When player incapacitation produces a terminal external-mission defeat and no local rescue succeeds, TA-9 coordinates:

1. Mission failure transaction under `61`/GDS-8;
2. authoritative ownership/security snapshot of carried/vehicle cargo;
3. robot/ship/crew/passenger outcomes;
4. creation of `RecoveryTransitState`;
5. selection/staging of a valid recovery destination process.

Recovery Transit is **not** successful extraction and cannot satisfy success-only objectives.

## 10. RecoveryTransitState

Persistent recovery state contains at minimum:

- source MissionInstanceId/failure TransactionId;
- player recovery status;
- origin Strategic Location/context;
- candidate/selected destination reference;
- recovery method/service source;
- start Simulation Time;
- minimum allowed arrival Simulation Time;
- actual planned arrival Simulation Time;
- included moved assets, if any;
- explicitly stranded/excluded assets;
- committed costs/obligations;
- state/revision.

## 11. Anti-Teleport Timing

Recovery arrival cannot occur sooner than both:

- the authored minimum recovery-service duration; and
- the shortest valid strategic return/recovery route duration required by the selected method/context.

Additional service overhead may extend it.

The recovery system does not use defeat as free fast travel.

## 12. Recovery Destination Selection

Destination resolution uses the GDS precedence and actual validity:

1. Primary Ship / mission support craft when valid and safe;
2. Horizon when a valid safe ingress/recovery point exists;
3. already-available allied/faction recovery facility;
4. authored neutral/emergency recovery location;
5. protected Horizon emergency path once safe ingress is established.

No destination is invented ad hoc at defeat time.

## 13. Destination Revalidation

Destination validity is rechecked before arrival commit.

If Horizon becomes unsafe because a DefenseEvent progresses during Recovery Transit, the system can reroute only to another legitimate recovery path.

Rerouting cannot reduce the already-required anti-teleport travel/service duration and can add time/cost.

## 14. Horizon Attack During Recovery Transit

Horizon DefenseEvent, station simulation, Dynamic Events, markets, target recovery, and work continue during player Recovery Transit because they share Simulation Time.

If Horizon remains a valid destination when transit completes, the player activates into the **current** persistent defense/recovery state; the attack is not restarted or rolled back.

## 15. Ship Location Preservation

Player recovery does not automatically relocate the Primary Ship.

The ship moves only if the same prepared recovery transaction includes a valid method such as:

- tow;
- rescue-carrier transport;
- valid autopilot recovery;
- station retrieval.

Otherwise ShipId, cargo, damage, fuel/propellant and world location remain where they actually are.

## 16. Robot / Crew / Passenger Preservation

Recovery Transit does not recall other actors by association with the player.

- secured robots remain with their actual transport;
- field Operational/Disabled robots follow GDS recoverability/lost rules;
- Destroyed robots stay Destroyed;
- crew/survivors remain at actual secured owner/location;
- Temporary Passengers are not duplicated into the recovery destination.

## 17. Field and Vehicle Cargo

Field-Unsecured acquired loot resolves through Mission failure rules and is not upgraded to secured merely because Recovery Transit begins.

Vehicle/Extraction-Secured cargo remains with its actual vehicle/owner if that vehicle is stranded elsewhere.

## 18. Strategic Consequence Router

TA-9 uses a project-owned `StrategicConsequenceRouter` to coordinate prepared typed mutations across domains after mission/event outcomes.

It does not directly own those domains.

Potential participants include:

- MissionStore;
- World/route state;
- Faction/reputation;
- Economy/Credits/market;
- DynamicEventStore;
- DefenseEventStore;
- Horizon recovery/automation;
- Narrative/progression;
- Resource/Inventory owners.

## 19. Prepared Consequence Batch

A batch contains:

- source event/mission/outcome ID;
- source revision;
- TransactionId;
- typed participant commands/deltas;
- explicit required/optional participants;
- deterministic commit order;
- idempotency markers.

All fallible validation/preparation occurs before the TA-2 infallible commit window.

No consequence batch waits for UI, disk I/O, worker completion, or asset loading during commit.

## 20. Consequence Ordering

Semantic ordering is explicit. A typical mission/raid result may commit in an order such as:

1. final physical ownership/security consequences;
2. persistent target/world consequences;
3. actor/ship/robot persistent outcomes;
4. Mission/Defense terminal state;
5. faction/economy/narrative/progression effects;
6. Dynamic Event follow-up/grace scheduling;
7. read-model/message publication.

Exact participants depend on outcome, but hash/container iteration never defines order.

## 21. World Change Versus Knowledge Change

A world route/faction/target state can commit before the player learns about it.

World consequence and player knowledge delivery are separate transactions linked by source IDs/timestamps.

This allows, for example, a distant route closure to exist without omniscient UI revelation.

## 22. Failure Preservation

The consequence router never adds generic failure taxes/refunds.

It preserves actual:

- consumed resources;
- world damage;
- robot destruction;
- station theft;
- knowledge already committed;
- reputation/economic consequence only when explicitly justified.

## 23. Recovery Softlock Protection

If no immediate recovery destination is valid, player remains in a pending Recovery Transit/rescue state while Simulation Time advances through legitimate recovery mechanisms.

Ordinary failure must still expose at least one eventual authored recovery path under the GDS softlock rules.

The path may be costly/delayed; it is not free restoration.

## 24. Save / Load

Persist where relevant:

- queued/delayed strategic messages;
- delivered knowledge markers;
- remote order state;
- RecoveryTransitState and deadlines;
- destination/method/cost;
- consequence TransactionIds/idempotency markers;
- source-event relations.

Reload never retroactively delivers an alert that had no path or moves assets not included in recovery.

## 25. Stable Save Boundary

A save cannot capture:

- mission failed but RecoveryTransitState not yet created when required;
- physical ownership both before and after defeat transfer;
- a remote order both uncommitted and reflected as applied;
- world consequence committed without its source transaction marker.

## 26. Debugging

Developer tooling exposes separate timelines for:

- event occurrence;
- station/source detection;
- communication availability;
- message queue/delivery;
- player-known time;
- remote order send/delivery/commit;
- defeat transaction;
- Recovery Transit route/deadline;
- consequence-batch participants/results.

## 27. Test Requirements

Tests prove:

- event can progress while unknown to player;
- unavailable communication prevents live alert/order but not simulation;
- delayed message keeps original timestamp;
- duplicate remote-order delivery cannot duplicate commit;
- defeat recovery is not mission extraction;
- Recovery Transit advances Simulation Time and respects minimum route time;
- Horizon attack can change recovery destination validity;
- player recovery does not teleport ship/robots/cargo;
- consequence batch is all-or-nothing at Stable Save Boundary;
- world change does not automatically reveal knowledge.

## 28. Explicit Non-Goals

No universal interstellar omniscience, defeat fast-travel exploit, recovery-based ship/robot recall, event pause because player is uninformed, UI-owned remote command mutation, or generic percentage loss/refund.

## 29. Dependencies

Depends on `61`–`66`, TA-2 transactions/read models, TA-3 transitions, TA-6 Horizon state, GDS Communications, Failure/Recovery, Recovery Transit, Missions, Raids, Dynamic Events, Economy/World/Narrative.

## 30. Open Questions

None in the communication/recovery/consequence-routing baseline.