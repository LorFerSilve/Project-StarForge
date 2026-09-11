# Runtime and Simulation Model

> **Status:** Architecture Complete  
> **Authority:** Main loop, fixed-tick simulation, pause, domain scheduling, worker jobs, transactions, active/off-screen contexts, and simulation boundaries

## 1. Main Loop Ownership

`App` owns the process main loop.

The baseline main thread owns:

- OS event pumping;
- input event ingestion;
- authoritative simulation stepping;
- OpenGL context and rendering submission/execution;
- presentation/UI update;
- final frame presentation.

Worker threads never own the OpenGL context or authoritative gameplay mutation.

## 2. Fixed Simulation Frequency

The authoritative base simulation uses a fixed timestep:

`1 / 60 second`

represented through a fixed `SimulationTick`/duration type rather than frame delta.

This rate is a technical architecture decision, not a GDS gameplay speed multiplier.

## 3. Variable Render Frequency

Rendering is decoupled from fixed simulation updates.

Each rendered frame:

- samples the two most recent stable simulation presentation states where interpolation is appropriate;
- uses an interpolation alpha derived from the fixed-step accumulator;
- never advances authoritative gameplay by itself.

## 4. Accumulator Loop

Conceptual loop:

1. poll platform events;
2. calculate real frame duration for scheduling only;
3. clamp pathological frame duration to a safety ceiling for accumulator stability;
4. if not True Paused, add eligible elapsed real frame duration to the simulation accumulator at baseline 1.0x;
5. execute zero or more fixed simulation ticks while accumulator >= fixed tick;
6. update presentation/read models;
7. render interpolated frame;
8. present/swap;
9. perform non-authoritative background/service work.

No domain receives arbitrary raw render-frame delta as its gameplay clock.

## 5. Spiral-of-Death Protection

The application enforces a maximum number of catch-up simulation ticks per rendered frame.

If the process falls catastrophically behind:

- gameplay Simulation Time is **not** silently skipped;
- the renderer may temporarily reduce frame output while simulation catches up;
- diagnostics report sustained backlog;
- if required, the session can enter a controlled performance-failure state rather than corrupt timing.

The exact catch-up cap is tuneable implementation data.

## 6. True Pause

When True Pause is active:

- no authoritative simulation ticks execute;
- the simulation accumulator does not advance from wall time;
- UI/pause presentation can still render/update using non-gameplay real time;
- audio can transition to pause presentation according to GDS;
- background file/asset jobs may continue if they do not advance gameplay.

## 7. Queued Pause

If pause is requested during an indivisible authoritative transaction:

- `PauseRequested = true` is retained;
- transaction completes inside the current tick;
- the tick reaches Stable Simulation Boundary;
- True Pause begins before the next ordinary fixed tick.

## 8. Simulation Phases Per Tick

Each fixed tick executes in a stable phase order.

Initial canonical phase order:

1. **Input Command Sampling**;
2. **Pre-Simulation Command Validation**;
3. **Movement / Character / Ship Control Intent**;
4. **Physics Step Preparation**;
5. **Physics Fixed Step**;
6. **Physics Result Collection**;
7. **Combat / Interaction Resolution**;
8. **Local Actor AI / Robot / Crew Active Logic**;
9. **Mission / Raid / Objective Logic**;
10. **Station / Strategic / Economy / Timers Due This Tick**;
11. **Cross-Domain Transaction Commit**;
12. **Committed Event Dispatch**;
13. **Cleanup / Deferred Destruction**;
14. **Persistent-State Synchronization for Active Entities**;
15. **Stable Simulation Boundary**.

The exact sub-order inside a domain is defined in later architecture specs, but cross-domain ordering must remain deterministic.

## 9. Commands

A `Command` is a request to change authoritative state.

Commands contain:

- typed command kind;
- issuer/source identity where relevant;
- target identity/data;
- payload;
- originating simulation tick;
- optional transaction correlation ID.

Commands are immutable after enqueue.

## 10. Command Sources

Commands can originate from:

- player action mapping;
- UI;
- AI;
- station automation;
- robot tactical orders;
- mission/narrative state machine;
- system recovery logic;
- debug/test tooling in development builds.

All use the same validation/commit principles.

## 11. Validation

Owning domain validates a command against the state at the defined validation phase.

Validation returns typed result:

- Accepted;
- Rejected with reason;
- Deferred/Blocked where the owning GDS supports that state.

Validation does not partially mutate authoritative state.

## 12. Transactions

An atomic transaction groups all state mutations that must commit together.

A transaction contains:

- TransactionId;
- participating domain operations;
- preconditions;
- prepared mutation set;
- commit result;
- emitted committed events.

## 13. Transaction Rule

Transaction sequence:

1. gather authoritative current state;
2. validate all required participants;
3. prepare mutation data without changing public authoritative state;
4. if any mandatory participant rejects, abort with no partial commit;
5. commit all prepared changes in deterministic order;
6. emit one committed transaction result and domain events.

## 14. Nested Transactions

Arbitrary nested cross-domain transactions are prohibited.

A domain-local operation may be part of a parent transaction, but only the parent coordinator owns final commit/abort.

This avoids recursive partial ownership.

## 15. Event Model

Committed events represent facts that already happened.

Examples:

- `InventoryTransferCommitted`;
- `DamageApplied`;
- `RobotDestroyed`;
- `StructuralConnectionBroken`;
- `MissionObjectiveChanged`;
- `MissionResolved`;
- `StrategicLinkStateChanged`.

Event consumers may issue new commands for a later deterministic processing phase/tick but do not retroactively veto the committed fact.

## 16. Same-Tick Follow-Up

Only explicitly defined safe follow-up events/transactions may execute later in the same tick.

Recursive unbounded event chains are prohibited.

A configurable hard diagnostic limit detects accidental event loops in development builds.

## 17. Deferred Destruction

Runtime entities/bodies/resources marked for destruction are not immediately invalidated while current-phase iterators/references may still exist.

They enter a deferred-destruction queue and are physically released at the Cleanup phase.

Persistent destruction semantics commit before runtime storage reclamation.

## 18. Runtime Entity Activation

When a persistent actor/object enters the Active Local Context:

1. persistent record is read;
2. content definition is resolved;
3. RuntimeEntityHandle allocated;
4. high-frequency components created;
5. physics proxy created if required;
6. render/audio proxies registered if required;
7. active behavior begins at next valid simulation phase.

## 19. Runtime Entity Deactivation

Deactivation:

1. stop accepting new local behavior commands;
2. finish/abort eligible pending local operations under owning rules;
3. synchronize persistent fields;
4. remove physics proxy;
5. remove render/audio proxies;
6. release runtime components/handle.

Persistent identity survives.

## 20. Active Local Simulation

The Active Local Context runs high-detail real-time systems:

- movement;
- rigid-body physics;
- hit detection;
- combat;
- local sensors;
- detailed AI;
- interactions;
- local hazards;
- mission objective triggers;
- ship piloting.

## 21. Off-Screen Strategic Simulation

Off-screen systems do not need 60 Hz updates when their GDS behavior does not require them.

The scheduler supports deterministic lower-frequency jobs, for example:

- 10 Hz system-control updates where responsive enough;
- 1 Hz station production/needs/economy-style evaluation;
- timer-deadline/event-driven processing for long strategic timers.

These frequencies are technical/tuneable and may be optimized after profiling if gameplay-equivalent outcomes are preserved.

## 22. Horizon Off-Screen

When Horizon is not the Active Local Context:

- station topology/state remains authoritative;
- power/thermal/atmosphere/water/logistics/production/crew/robots/events continue through scheduled simulation;
- detailed render/physics representations are not required unless a simultaneous Home Station event needs a specific local physical calculation;
- no alternate simplified ownership or resource model is allowed.

## 23. Home Station Defense Off-Screen

Off-screen Horizon Defense resolves through real defense/system data.

Architecture may use aggregate spatial/ballistic approximations specifically defined later for off-screen resolution, but inputs and consequences remain derived from actual:

- geometry/topology;
- active defenses;
- firing arcs;
- ammunition;
- power/thermal state;
- security;
- crew;
- robots;
- attacker capabilities.

No opaque `DefensePower` scalar becomes authoritative.

## 24. Strategic Timer Scheduler

Long-duration timers are not decremented by touching every timer every frame.

A priority/deadline scheduler stores deterministic due ticks/deadlines.

When SimulationTick reaches a deadline, the owner receives a due event/command.

Examples:

- research completion;
- work orders;
- market replenishment;
- Recovery Grace;
- reinforcement arrival;
- travel arrival;
- status expiry when high-frequency tracking is unnecessary.

## 25. Timer Identity

Persistent strategic timers serialize:

- owner ID/type;
- timer type;
- due SimulationTick or remaining canonical duration;
- state;
- transaction correlation where required.

## 26. Random Streams

A deterministic RNG service owns named/scoped streams.

Persistent events store enough stream state/seed to resume without load-based rerolling.

Worker threads may generate random candidates only from an explicitly assigned deterministic stream/range and may not race to decide which random values become authoritative.

## 27. Worker Job System

A bounded worker pool executes asynchronous jobs.

Allowed examples:

- file reads;
- image/mesh decoding;
- procedural layout candidate generation;
- navigation path search;
- visibility/spatial analysis prepared from immutable snapshots;
- save encoding/compression after snapshot capture.

## 28. Worker Result Rule

Worker results are immutable messages/data blobs.

They return to the main/simulation thread and are:

- checked for staleness/version;
- validated against current state;
- committed or discarded.

Workers cannot write gameplay stores directly.

## 29. Snapshot Versioning for Jobs

Long-running jobs carry an input-state version/generation.

Example:

- pathfinding request references NavWorldGeneration 41;
- station geometry changes to generation 42;
- result from generation 41 arrives;
- consumer rejects/replans if path is no longer valid.

## 30. Render Snapshot

At Stable Simulation Boundary, presentation-facing state is copied/assembled into a render/read snapshot.

Renderer receives only data needed for the frame, such as:

- transforms;
- mesh/material handles;
- lights;
- cameras;
- particle/VFX commands/state;
- debug primitives.

Renderer never holds mutable references into gameplay stores across ticks.

## 31. UI Read Model

UI reads immutable/domain-owned view models refreshed from authoritative state.

UI interactions generate commands.

A widget cannot retain a writable pointer to inventory, mission, station, or robot data.

## 32. Audio Event Timing

Gameplay emits semantic audio events stamped with SimulationTick/context.

Audio presentation may use real-time mixing/playback, but it cannot feed back uncommitted gameplay state.

## 33. Save Snapshot Capture

At a Stable Save Boundary:

1. simulation reaches stable boundary;
2. each persistent domain exports immutable versioned save DTO/state;
3. persistence service assembles one coherent snapshot manifest;
4. simulation can resume after capture;
5. serialization/compression/file writing can continue on worker thread from immutable copied snapshot data;
6. write-new-then-atomic-replace completes on storage layer.

The file-writing duration does not freeze gameplay unless memory/platform constraints later require an explicitly handled fallback.

## 34. Loading

Loading occurs outside ordinary live simulation ticks.

Sequence:

1. enter loading/session transition state;
2. validate save header/version/checksums;
3. migrate if required;
4. construct domain stores in staging state;
5. cross-reference/invariant validation;
6. replace current session state atomically at application/session level;
7. activate required local context;
8. resume from saved SimulationTick/state.

Partial domain loads are prohibited for a normal save restore.

## 35. Scene Transition

A local-context transition is an explicit state machine, not ad-hoc unload/load.

Conceptual states:

- Request;
- Validate;
- Freeze transition participants at stable boundary;
- Persist/deactivate origin representation;
- Load/prepare destination content;
- Activate destination runtime entities;
- Commit location/context ownership transition;
- Resume simulation.

Transition can display loading presentation while gameplay Simulation Time is held according to the transition contract.

## 36. Loading-Time Simulation

Normal blocking loading screens do not advance Simulation Time.

Background streaming that occurs during live gameplay does not pause unless the owning transition explicitly enters a hold.

## 37. Error Handling

Recoverable runtime errors are classified:

- content/data error;
- resource/asset unavailable;
- save/integrity error;
- platform/device error;
- invariant violation.

Development builds provide exact diagnostics.

Gameplay state is not partially committed to compensate for an unexpected technical failure.

## 38. Headless Simulation

The simulation/gameplay/persistence architecture must support tests without:

- GLFW window;
- OpenGL context;
- audio device.

A test composition root substitutes/nulls presentation/platform adapters.

## 39. Performance Metrics

Development runtime records at minimum:

- simulation tick duration;
- catch-up tick count/backlog;
- domain update timings;
- worker queue depth/latency;
- active runtime entity count;
- physics body/contact/query counts;
- render submission/draw/GPU timings later;
- save snapshot/serialization time.

## 40. Explicit Non-Goals

The runtime architecture does not require:

- variable-delta authoritative simulation;
- worker-thread direct gameplay mutation;
- one thread per subsystem;
- gameplay progression from render frames;
- global untyped events;
- scene load resetting persistent state;
- all station systems updating at 60 Hz;
- offline/wall-clock simulation.