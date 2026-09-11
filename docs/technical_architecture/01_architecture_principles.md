# Technical Architecture Principles

> **Status:** Architecture Complete  
> **Authority:** Project-wide software architecture principles

## 1. Purpose-Built Modular Monolith

StarForge is one executable/game application composed of explicit internal modules.

The baseline does not use microservices, runtime plugins, or a distributed game-server architecture.

Modules communicate through narrow interfaces and typed data/contracts rather than uncontrolled global access.

## 2. Gameplay State Is Not Rendering State

Authoritative gameplay state is independent from OpenGL objects, render meshes, animation presentation, audio voices, and UI widgets.

A gameplay entity can continue to exist and simulate when:

- not rendered;
- outside the active local scene;
- represented through coarse off-screen simulation;
- temporarily missing presentation assets.

## 3. One Authoritative Simulation Owner

All gameplay-authoritative mutation occurs on the **Simulation Thread**.

Worker threads can calculate or prepare results, but they may not commit gameplay-authoritative state directly.

This supports:

- deterministic transaction ordering;
- Stable Save Boundaries;
- reproducible debugging;
- no data races over authoritative gameplay state.

## 4. Main/Simulation Thread Baseline

For the first implementation baseline, the OS/window thread and authoritative Simulation Thread are the same main thread.

OpenGL context ownership also remains on this thread.

Worker threads are used only for bounded asynchronous jobs such as:

- file I/O;
- asset decoding/transcoding;
- procedural-generation preparation;
- pathfinding queries;
- expensive read-only analysis;
- save compression/write staging after an immutable snapshot exists.

The architecture can later split rendering from simulation only through an explicit architecture revision.

## 5. Fixed Simulation, Variable Rendering

Authoritative real-time simulation advances at a fixed **60 Hz** base tick.

Rendering runs at the display/frame rate and interpolates presentation state between completed simulation ticks where appropriate.

Systems that do not require 60 Hz may run at lower deterministic cadences derived from Simulation Time.

No gameplay result is derived from rendered frame count.

## 6. Simulation Time Is the Only Gameplay Clock

Every gameplay timer consumes the GDS-defined Simulation Time.

Technical timers use integer tick/deadline representations wherever practical to avoid divergent floating-point accumulation.

Real/wall time is restricted to:

- profiling;
- frame pacing;
- logs;
- file timestamps;
- user-facing metadata explicitly permitted by GDS.

## 7. Stable Simulation Boundary

A **Stable Simulation Boundary** exists after one authoritative fixed tick has:

- completed all scheduled domain updates;
- committed all atomic transactions for that tick;
- drained deterministic gameplay event queues for the tick;
- produced a coherent next-state snapshot.

Stable Save Boundaries are taken only from a Stable Simulation Boundary that also satisfies persistence-specific conditions.

## 8. Command → Validate → Commit → Event

Player input, AI, automation, UI, and scripted systems do not mutate arbitrary state directly.

State-changing operations follow the conceptual pattern:

1. Command/Intent;
2. validation against authoritative state;
3. atomic commit;
4. typed domain event/result;
5. downstream consumers update from the committed result.

This is the core anti-duplication/anti-half-state pattern.

## 9. Typed Identity

Persistent identities use strongly typed persistent IDs.

Examples:

- CrewId;
- RobotId;
- ShipId;
- MissionId;
- ZoneId;
- DynamicEventId;
- DefenseEventId;
- ItemInstanceId;
- TransactionId.

An ID type is not implicitly interchangeable with another ID type.

## 10. Persistent ID vs Runtime Handle

Persistent identity and runtime storage handle are separate concepts.

- **PersistentId:** stable across save/load and scene unload/reload;
- **RuntimeEntityHandle:** process-local index/generation handle used for efficient active-scene access.

Persistent state must never serialize raw pointers or process-local handles as identity.

## 11. Hybrid Entity Architecture

StarForge does **not** put every game concept into one universal ECS.

Use:

### Runtime Entity Registry

For high-frequency active-world entities such as:

- player;
- enemies;
- local robots;
- projectiles;
- interactive props;
- local ship bodies;
- temporary mission actors.

### Domain-Owned Persistent Stores

For strategic/persistent state such as:

- Horizon station graph/state;
- crew roster;
- robot roster/configuration;
- ship registry/configuration;
- missions/events;
- economy/markets;
- research/Blueprint knowledge;
- galaxy/routes;
- progression;
- save metadata.

The two layers synchronize through explicit activation/deactivation boundaries.

## 12. No God Object

There is no writable global `GameState` object exposed to every subsystem.

The application has an orchestration root, but each domain owns its data and exposes narrow commands/queries/events.

## 13. No Global Untyped Event Bus

Typed event channels are used with known producers/consumers.

A universal stringly-typed event bus is prohibited.

Cross-domain events are intentionally small facts such as:

- `StructuralConnectionBroken`;
- `InventoryTransferCommitted`;
- `RobotDestroyed`;
- `MissionResolved`;
- `StrategicLinkStateChanged`.

Consumers cannot mutate producer-owned data through event references.

## 14. Query vs Command Separation

Read-only queries return views/value data.

Commands request mutation.

Public APIs do not expose mutable containers belonging to another domain.

## 15. Data-Oriented Where It Matters

High-frequency systems should favor cache-friendly contiguous data where measurable benefit exists:

- transforms;
- physics proxies;
- active render instances;
- projectiles;
- AI sensory candidates.

Low-frequency strategic state prioritizes clarity, explicit ownership, and serialization correctness over forced data-oriented layout.

## 16. OpenGL Is a Renderer Detail

OpenGL calls occur only inside renderer/render-resource implementation.

Gameplay, AI, physics, persistence, missions, station simulation, and UI model code do not depend on OpenGL.

## 17. Renderer Is Submit-Based

Gameplay/presentation systems submit render descriptions to the renderer.

They do not directly:

- bind shaders;
- create VAOs;
- upload textures;
- change OpenGL state.

## 18. Physics Is an Adapter Boundary

Jolt Physics is used for collision/rigid-body commodity functionality behind a StarForge-owned physics interface.

Gameplay owns:

- damage semantics;
- mission outcomes;
- movement permissions;
- docking rules;
- structural graph rules.

The physics library reports physical contacts/query results; it does not become the gameplay authority.

## 19. Determinism Scope

The baseline does not require bit-identical cross-platform deterministic physics.

It **does** require deterministic authoritative transaction/event ordering from the same committed gameplay state and saved deterministic random-stream state.

Persistent procedural outcomes must not depend on:

- hash-map iteration order;
- render FPS;
- wall-clock timing;
- thread completion race.

## 20. Randomness Is Explicit

Gameplay randomness uses named deterministic streams/scopes.

Examples:

- MissionGenerationRng;
- LootRng;
- DynamicEventRng;
- CombatSpreadRng where the GDS permits dispersion randomness.

No gameplay code uses an untracked global RNG.

## 21. Scene Loading Cannot Own Persistent Truth

Loading or unloading a local scene changes runtime representation, not long-term ownership.

Unloading a Mission Zone cannot silently:

- refund ammunition;
- resurrect enemies;
- recreate deposits;
- duplicate cargo;
- reset persistent target damage.

## 22. Off-Screen Simulation Uses the Same Domain Rules

Horizon may use coarse technical cadence when not rendered, but it still consumes the same:

- resource ownership;
- topology;
- subsystem state;
- timers;
- task rules;
- damage/event outcomes.

There is no alternate opaque off-screen Defense Score or economy model.

## 23. Content Is Data, Rules Are Code/Contracts

Authored game content should be externalized into validated data where variation is expected:

- item definitions;
- weapons;
- resources;
- robot components;
- ship modules;
- station modules;
- mission templates;
- factions;
- recipes;
- tuning values.

Core rule semantics remain explicit in code and architecture contracts.

## 24. Validation Before Runtime Failure

Content and configuration should be validated as early as possible.

Invalid references, duplicate IDs, incompatible components, impossible recipe/resource references, or malformed mission templates should fail tooling/build/content-validation rather than becoming silent runtime behavior.

## 25. Debuggability Is Architectural

Every authoritative transaction and persistent identity must be inspectable in development builds.

Required developer facilities eventually include:

- structured logging;
- entity/state inspector;
- transaction/event trace;
- simulation clock/tick display;
- deterministic seed display;
- save integrity checker;
- station graph/network visualization;
- AI/pathfinding diagnostics;
- renderer/physics statistics.

## 26. Failure Is Explicit

Recoverable technical failures return typed errors/results.

Fatal invariant violations in development builds fail loudly with diagnostics rather than silently corrupting state.

Player-facing recoverable failures are translated through presentation/UI, not raw exceptions.

## 27. Exceptions Boundary

C++ exceptions are not used as ordinary gameplay control flow.

Third-party/library exceptions, where possible, are caught at adapter/load boundaries and converted to typed project errors.

## 28. Ownership Uses RAII

C++ resource lifetime uses RAII.

Raw owning pointers are prohibited.

Use value ownership and `std::unique_ptr` by default; `std::shared_ptr` requires a real shared-lifetime case rather than convenience.

## 29. Dependency Direction

High-level gameplay domains may depend on stable foundation interfaces.

Foundation modules may not include or call higher-level gameplay modules.

Acyclic dependency direction is a hard architectural goal and will be CI-validated where practical.

## 30. Performance Philosophy

Correctness and clear authority come before premature optimization.

Performance-sensitive areas are designed so they can be measured and optimized without changing GDS semantics.

The baseline targets responsive 3D gameplay on contemporary desktop hardware; exact minimum/recommended hardware belongs to a later performance-validation phase.

## 31. Testability

Simulation logic should be executable without a live OpenGL context whenever the tested behavior does not require rendering.

Core gameplay, persistence, transactions, mission rules, station networks, economy, and deterministic simulation should therefore be testable headlessly.

## 32. Explicit Non-Goals

The architecture does not require:

- a reusable public engine SDK;
- runtime mod/plugin ABI in baseline;
- multiplayer replication;
- server authority;
- a scripting VM;
- a universal object hierarchy;
- a universal ECS for strategic state;
- cross-platform bit-identical rigid-body physics;
- OpenGL dependencies in gameplay code.