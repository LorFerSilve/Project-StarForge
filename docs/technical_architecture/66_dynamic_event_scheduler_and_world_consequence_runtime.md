# TA-9 — Dynamic Event Scheduler and World Consequence Runtime

> **Status:** Architecture Complete  
> **Authority:** DynamicEventStore, deterministic eligibility/generation scheduling, event lifecycle, concurrency, Mission/DefenseEvent handoff, strategic timers/cooldowns/grace, and typed world/economic consequence routing

## 1. Purpose

Dynamic Events make the persistent galaxy react during active Simulation Time. They must remain deterministic, sourced from plausible world state, bounded by concurrency/recovery rules, and separate from the specialized Mission/DefenseEvent engines they may create.

## 2. DynamicEventStore

`DynamicEventStore` owns persistent `DynamicEventId` records.

Each `DynamicEventRecord` contains at minimum:

- DynamicEventId;
- event family/type;
- stable event seed/generation version;
- source/location/faction references;
- eligibility snapshot key/revisions;
- lifecycle state;
- strategic start/expiry/arrival deadlines;
- linked MissionId / DefenseEventId / market/world consequence IDs where applicable;
- committed knowledge/delivery state references;
- resolution/cancellation reason;
- applied TransactionIds;
- event revision.

## 3. Canonical Lifecycle

TA-9 implements the GDS states:

```text
Candidate
 -> Offered | Scheduled
 -> Active
 -> Resolved | Expired | Cancelled
```

Candidate is normally staging state and need not receive DynamicEventId until the candidate is ready to commit as Offered/Scheduled/Active.

Rejected candidates do not become persistent events.

## 4. Strategic Event Scheduler

A project-owned `StrategicEventScheduler` advances only on global Simulation Time and at deterministic strategic evaluation/deadline boundaries.

It owns persistent scheduling cursors such as:

- per-family generation ordinal;
- next evaluation deadline;
- family cooldown state;
- optional-offer capacity state;
- Recovery Grace state/source;
- critical-Horizon-event slot state.

Wall-clock time, render frames, and application shutdown do not advance it.

## 5. Deterministic Candidate Evaluation

At an evaluation boundary:

1. snapshot eligible strategic/world/economy/station state and revisions;
2. enumerate eligible event families/locations in stable order;
3. derive scoped TA-2 PCG32 streams from persistent generation cursor data;
4. construct deterministic candidates;
5. apply hard eligibility/concurrency/softlock/grace filters;
6. apply permitted weighting/selection in deterministic stream order;
7. prepare chosen event creation/consequence transaction;
8. revalidate source revisions;
9. commit scheduler cursor + DynamicEvent record/effect atomically.

Worker execution order is not selection authority.

## 6. Event Families

The scheduler supports the Design Complete families without giving them one universal execution model:

- Distress / Survivor Rescue;
- Salvage Opportunity;
- Resource Discovery / Extraction Window;
- Trader / Convoy Arrival;
- Market Shortage / Surplus;
- Faction Conflict / Patrol;
- Route Disruption / Restoration;
- Anomaly / Research;
- Station Emergency;
- Horizon Station Defense;
- NPC Raid / Conflict Opportunity;
- Recovery / Follow-Up.

Each family owns typed eligibility/effect adapters.

## 7. Event Source Requirement

Every event candidate must identify a plausible source/context.

Examples:

- a faction station/patrol for an attack/convoy;
- an actual route/location for disruption;
- a physical wreck/site for salvage;
- a station fault/world incident for emergency;
- a faction/world force and reachable route for Horizon attack.

A free-floating punishment timer cannot create a force or resource event from nothing.

## 8. Concurrency Contracts

The scheduler enforces the GDS baseline:

- maximum 3 concurrently Offered optional external Dynamic Events;
- maximum 1 active/scheduled Critical Horizon Station Event;
- canonical story missions do not consume the optional-offer cap;
- only one external player MissionInstance may be deployed, enforced separately by `61`.

Excess candidates wait/reject according to deterministic family policy rather than silently replacing committed events.

## 9. Offered Events

Offered means the event/opportunity exists and is player-visible **only if its information has actually been delivered**.

The event can be committed before player knowledge reaches the player.

Therefore TA-9 stores separately:

- event existence/lifecycle;
- communication/message delivery state;
- player-known read-model state.

## 10. Scheduled Events

Scheduled represents a committed future event or arrival, such as:

- force en route;
- convoy dispatched;
- recovery operation scheduled;
- authored delayed incident.

The stored deadline advances in Simulation Time and cannot be reset by save/load.

Destroying an origin communication node after a dispatch transaction does not retroactively unschedule the dispatched object unless a valid modeled recall/cancellation occurs.

## 11. Mission-Producing Event Handoff

A mission-producing DynamicEvent uses an atomic cross-domain transaction that:

1. validates event still owns the opportunity;
2. invokes/prepares `63` mission candidate/package with EventId-scoped source context;
3. allocates/creates MissionId and MissionRecord;
4. writes the MissionId link into DynamicEventRecord;
5. marks the event's appropriate handoff state.

After commit:

- MissionStore owns objectives/acceptance/deployment/extraction/resolution;
- DynamicEventStore does not mirror Mission state;
- the event can observe the mission result only through typed committed outcome facts when needed for its own resolution.

## 12. Horizon DefenseEvent Handoff

For a procedural Horizon attack, the scheduler validates all GDS eligibility:

- plausible hostile/motivated attacker;
- valid world source and route/reach;
- Horizon meaningful/eligible;
- no Recovery Grace blocker;
- no authored campaign prohibition;
- no existing active/scheduled Critical Horizon event.

The creation transaction atomically:

- allocates DefenseEventId;
- creates finite attacker/approach/objective package;
- creates `65` DefenseEventRecord;
- links it from DynamicEventRecord;
- occupies the Critical Horizon event slot.

Afterward `65` owns execution.

## 13. Recovery Grace

Recovery Grace is persistent strategic scheduler state with:

- source DefenseEventId;
- source resolution TransactionId;
- remaining SimulationDuration;
- paused-for-Critical-Recovery flag/state;
- revision.

Every resolved ordinary procedural Horizon Defense Event refreshes/starts grace exactly once.

While Critical Recovery is active, remaining grace does not decrease.

Procedural attack generation cannot override grace.

An explicitly authored story crisis can bypass it only through declared content policy validated at event creation.

## 14. Cooldowns and Repetition Control

Family/location cooldowns use Simulation Time and persistent state.

Anti-frustration weighting can consider:

- unresolved severe setbacks;
- recent same-family/location history;
- current reachability;
- recovery state;
- world plausibility.

It affects eligibility/weight only; it cannot secretly reduce enemy HP/damage or fabricate resources.

## 15. Optional Event Expiration

An optional Offered event can store an explicit expiry deadline.

At deadline, if still eligible for expiration:

- lifecycle transitions to Expired exactly once;
- any authored local/world natural consequence is prepared/committed;
- no generic global punishment is added;
- linked accepted Mission state is not silently deleted contrary to Mission rules.

## 16. Cancellation

Cancellation requires a legitimate committed invalidating state, such as:

- source destroyed before dispatch;
- target no longer exists/eligible;
- authored story state supersedes event;
- route/context becomes permanently invalid under the event contract.

Cancellation records reason and cannot be used as a silent reroll mechanism.

## 17. World Consequence Batch

Dynamic Events coordinate consequences through a typed prepared `StrategicConsequenceBatch` rather than writing directly into unrelated stores.

Possible participants include:

- World/route state;
- Faction/reputation;
- Economy/market finite stock/modifiers;
- MissionStore;
- DefenseEventStore;
- Horizon station-event/recovery state;
- Narrative/progression flags;
- physical resource owners where a real transfer occurs.

TA-2 prepared-transaction rules apply: all fallible validation occurs before the infallible commit window.

## 18. No Event-Owned Physical Duplication

DynamicEventRecord can reference physical assets/stock/cargo but does not duplicate ownership.

Examples:

- convoy stock belongs to the convoy/market physical owner;
- salvage belongs to world/wreck owners;
- attacker cargo belongs to actual force owners;
- mission reward remains entitlement/claim until delivered through its owning reward rules.

## 19. World / Route Events

A route disruption/restoration changes only explicit route/world records.

Procedural events cannot permanently remove the sole campaign route/capability without an authored recovery/alternate path.

Route changes use committed world consequences and player knowledge remains subject to communication/discovery.

## 20. Market / Convoy Events

Market effects operate on finite economy/stock state, not arbitrary menu-price magic.

A convoy event can create/associate a finite persistent convoy/world supply record and arrival/departure schedule.

TA-9 can schedule the strategic event; economy authority owns price/ledger/stock consequences.

## 21. Persistent Raid-Target Recovery

DynamicEvent scheduling can create a target-recovery process for a damaged/depleted NPC raid target.

Recovery records:

- target identity;
- recovery source/support;
- declared repair/replenishment scope;
- Simulation-Time deadlines;
- required world/economy support where modeled;
- irrecoverable/unique exclusions.

Recovery cannot duplicate stolen unique items or erase protected persistent story consequences.

## 22. Event Knowledge and Delivery

An event existing does not imply the player knows it exists.

Information delivery uses `67` communication routing and records:

- source event/fact;
- source timestamp;
- delivery state/time;
- fidelity/staleness;
- recipient knowledge revision.

Event progression never pauses solely because the player has not received the message.

## 23. Campaign Softlock Validation

Before committing procedural destructive/route events, a strategic validator checks that ordinary systemic events cannot eliminate all paths for mandatory:

- item;
- route;
- character;
- ship capability;
- station capability;
- committed Knowledge Asset.

A costly recovery path is allowed; no path is not.

## 24. Save / Load

Persist:

- DynamicEvent IDs/seeds/lifecycle;
- strategic deadlines;
- generation ordinals/cursors;
- cooldowns;
- concurrency slot ownership;
- MissionId/DefenseEventId links;
- Recovery Grace;
- applied consequence TransactionIds;
- delivered/queued knowledge references.

Reload does not invoke fresh generation for existing events.

## 25. Stable Save Boundary

A strategic event commit never leaves a state where:

- DynamicEvent links to an uncreated Mission/Defense record;
- Critical Horizon slot is occupied by no event;
- world consequence is applied but event remains pre-commit;
- one-time resolution effect lacks idempotency marker.

## 26. Diagnostics

Developer tooling exposes:

- event generation cursor/next deadline;
- eligibility gates and rejection reasons;
- active/offered concurrency counts;
- event seed/version;
- source/world refs;
- lifecycle/deadline;
- Mission/Defense links;
- Recovery Grace;
- consequence transaction history;
- existence versus delivered-player-knowledge state.

## 27. Test Requirements

Tests prove:

- no wall-clock progression;
- deterministic generation for same scheduler state;
- optional-offer and Critical Horizon caps;
- Recovery Grace blocks procedural attack creation and pauses in Critical Recovery;
- Mission handoff creates one MissionId and no duplicate mission state;
- Defense handoff creates one finite DefenseEvent and occupies slot atomically;
- dispatched Scheduled event is not unsent by later comm loss;
- expired optional event applies only authored effects;
- procedural event cannot create campaign softlock;
- save/load cannot reroll existing events or reset deadlines;
- physical assets are referenced, not duplicated by EventStore.

## 28. Explicit Non-Goals

No real-world daily events, offline attacks, endless spam, gear-matched world scaling, duplicate Mission/Defense state machines, threats without source, percent resource magic, or event existence equaling player knowledge.

## 29. Dependencies

Depends on `61`–`65`, TA-2 deterministic RNG/transactions, TA-6 Horizon/recovery inputs, TA-8 knowledge boundaries, GDS-12 Dynamic Events, Economy/World/Faction systems, and `67` communication/consequence routing.

## 30. Open Questions

None in the DynamicEvent scheduling baseline.