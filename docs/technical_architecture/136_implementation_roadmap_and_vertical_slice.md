# TA-16 — Dependency-Ordered Implementation Roadmap and First Vertical Slice

> **Status:** Implementation Locked  
> **Baseline:** `TA16-V1`  
> **Authority:** Order in which Architecture Complete contracts become implemented and depended upon

## 1. Principle

Implementation follows dependency order, not feature excitement. A later phase may prototype against a narrow fake only when the fake is explicitly test-owned and does not become production authority.

A phase is not complete because code exists; its exit evidence must pass.

## 2. Phase Naming

Post-TA-16 implementation phases use `IMP-N`.

`IMP-0` is the TA-16 bootstrap materialization itself. Real game-system implementation begins at `IMP-1`.

## 3. IMP-0 — Build / Verification Bootstrap

**Status at TA-16 closure:** Complete

Owns:

- pinned toolchain/dependency manifest;
- root CMake/presets;
- strict warnings;
- `sf_core` bootstrap target;
- `starforge-headless` bootstrap executable;
- `starforge_tests_unit`;
- nonzero-test guard;
- first executable CI gate.

**Exit:** configure/build/test succeeds on locked primary CI and emits exact-SHA evidence.

No gameplay is implemented here.

## 4. IMP-1 — Core, Identity, Deterministic Simulation, Transactions

Implement first:

- foundational Result/error/assert/log contracts;
- strong typed IDs and allocators;
- `SimulationTick`, durations and 60 Hz `SimulationClock`;
- deterministic tick pipeline shell;
- command/result/event envelopes;
- revisions/generations;
- PCG32/SplitMix64 scoped RNG;
- prepared transaction coordinator;
- immutable read-model foundation;
- headless composition-root foundation.

Primary targets:

```text
sf_core
sf_diagnostics
sf_simulation
sf_transactions
sf_read_models
starforge-headless
starforge_tests_unit
starforge_tests_domain
```

**Exit:** deterministic headless tick/RNG/ID/transaction tests pass across repeated runs and selected worker counts; no platform/render dependency is required.

## 5. IMP-2 — Content Model and Persistence Primitives

Implement:

- `ContentId`/ContentKind/registry value model;
- closed definition schema foundation;
- binary primitive codecs;
- Save DTO/version primitives;
- exact save v1 header/directory codecs;
- CRC32C/hash helpers;
- staged load/export interfaces;
- minimal current-version save round trip;
- fixture/golden infrastructure.

Targets:

```text
sf_content_model
sf_persistence_primitives
sf_persistence
starforge_tests_content
starforge_tests_persistence
```

**Exit:** exact binary golden + corruption rejection + ContentId resolution tests pass headlessly. No world/backend object is serialized.

## 6. IMP-3 — Platform and Rendering Foundation

Implement:

- GLFW platform/window/context adapter;
- glad2 v2.0.8 generated loader;
- OpenGL 4.6 Core capability validation;
- render-owned resource lifetime;
- immutable `RenderSnapshot` handoff shell;
- reversed-Z setup;
- minimal shader/mesh path;
- debug callback and backend diagnostics.

Targets:

```text
sf_platform
sf_platform_glfw
sf_render
sf_render_opengl
starforge
starforge_tests_backend
```

**Exit:** Windows app opens an OpenGL 4.6 Core context and renders a deterministic test scene/triangle through project-owned renderer boundaries; backend smoke and resource-lifetime tests pass; no gameplay domain includes OpenGL/glad/GLFW directly.

## 7. IMP-4 — Active World, Streaming Shell, Physics, Character

Implement:

- SceneInstance/ActiveLocalContext lifecycle;
- RuntimeEntityRegistry/handles minimal foundation;
- Context Space/runtime origin foundation;
- required-content readiness/Hard Streaming Hold shell;
- Jolt PhysicsWorld adapter;
- collision layers/queries;
- CharacterMotor baseline;
- active-scene activation/deactivation;
- fixed-tick physics integration;
- input sampling sufficient for locomotion test.

Targets include:

```text
sf_world
sf_physics
sf_physics_jolt
sf_player
```

**Exit:** a headless physics scene and graphical test room produce consistent character/collision state, scene teardown leaks no owned backend object, and frame cadence does not change authoritative position checkpoints.

## 8. IMP-5 — Vertical Slice V0: Horizon Test Cell

This is the first end-to-end playable engineering slice. It intentionally proves architecture before broad feature development.

### 8.1 Player Experience

The player can:

1. launch `starforge`;
2. enter a small authored/cooked Horizon test compartment;
3. look/move in first person;
4. collide with room geometry;
5. interact with one physical persistent test item;
6. transfer that item between a world owner/container and player inventory through a real typed transaction;
7. see committed state reflected in a minimal shipping-style read-model UI/debug presentation;
8. save at a Stable Save Boundary;
9. exit/relaunch;
10. load the save;
11. observe that ownership/location/state are reconstructed exactly once and the item did not respawn/duplicate;
12. leave/re-enter the local context and obtain the same persistent truth.

### 8.2 Systems Deliberately Included

- C++23 build/CI;
- content identity + minimal cooked test content;
- platform/window/input;
- renderer;
- fixed simulation;
- world activation;
- Jolt collision/CharacterMotor;
- RuntimeEntityHandle vs PersistentId separation;
- one-owner physical transfer transaction;
- read model/presentation boundary;
- save/load staging;
- headless scenario equivalence.

### 8.3 Systems Deliberately Not Expanded Yet

- full station utilities;
- enemy combat breadth;
- AI/nav breadth;
- missions/procedural generation;
- spacecraft;
- raids;
- production/economy/campaign breadth;
- polished art/audio/UI.

### 8.4 V0 Acceptance Criteria

V0 passes only if:

- graphical and headless authoritative checkpoints agree for the scripted interaction;
- running at different render cadences does not change final authoritative state;
- world item has exactly one owner before/after transfer;
- save/load does not duplicate/delete the committed item;
- runtime handles differ safely after reload while PersistentId remains stable;
- load duration advances zero Simulation Time;
- OpenGL/Jolt objects are reconstructed rather than serialized;
- required CTest/CI/backend/persistence evidence is green;
- ASan/UBSan-supported slice run has no blocking finding;
- no vertical-slice code violates the locked target DAG.

V0 is the architectural proof that broad gameplay can safely build on the foundation.

## 9. IMP-6 — Runtime Gameplay, Inventory, Equipment, Combat

Implement mature runtime entity/component pools, inventory/equipment, interaction families, Health/shield/status, weapons/ammo, hitscan/swept/dynamic projectiles, combat consequences and persistent/runtime synchronization.

**Exit:** player combat/inventory scenarios, ownership conservation, deferred destruction and backend smoke all pass.

## 10. IMP-7 — Horizon Station Systems

Implement structural/traversal topology, Power, atmosphere, thermal, water, logistics, WorkOrders, manufacturing/farming, construction/repair and automation/control.

**Exit:** conservation/topology/transaction tests pass active and off-screen; representative station benchmark becomes executable.

## 11. IMP-8 — Navigation, AI, Crew, Robots

Implement grounded Recast/Detour adapter, bounded 3D navigation, invalidation, async pathfinding, perception/memory, enemy tactical AI, crew tasks, robot command/ROE/squads and off-screen logical behavior.

**Exit:** knowledge/path determinism, worker-order equivalence and active↔off-screen actor tests pass.

## 12. IMP-9 — Missions, Exploration, and Procedural Runtime

Implement Mission/MissionInstance stores, objective DAG, exploration/hazards, extraction/security, stable procedural generation, rewards/failure and story-mission execution framework.

**Exit:** anti-reroll, retry, extraction, objective exactly-once and save/load scenario matrix passes.

## 13. IMP-10 — Spacecraft and Local Spaceflight

Implement ship configuration/mass/inertia, 6DoF controls, Flight Assist, docking, cargo, ship systems/damage and local-space scene profile; strategic Reach/travel integrates with persistent state.

**Exit:** ship dynamics/docking/cargo ownership/reload/backend smoke and reference local-flight performance scenario pass.

## 14. IMP-11 — Raids, Horizon Defense, Dynamic Events, Recovery

Implement offensive raid specialization, persistent target consequences, reinforcements/escalation, DefenseEvent active/off-screen resolution, Dynamic Events/Recovery Grace, communications/remote orders and Recovery Transit.

**Exit:** theft exactly-once, finite reinforcement, defense active↔off-screen equivalence and defeat/recovery chronology pass.

## 15. IMP-12 — Shipping UI, Audio, Accessibility, Presentation

Expand retained UI/text/HUD/management/tutorials, miniaudio presentation, subtitles/captions/alarms, accessibility, animation/camera/VFX integration and dev tooling.

**Exit:** input focus/knowledge boundaries, mute-vs-AI-hearing, accessibility semantic equivalence and presentation budgets pass.

## 16. IMP-13 — Economy, Research, Progression, Narrative Integration

Implement Credits/markets/trade, research/evidence/blueprints, capability progression, factions/reputation, authored campaign state, finale transaction and postgame integration.

**Exit:** economy conservation, progression prerequisites, reputation/knowledge and exactly-once finale save/load scenarios pass.

## 17. IMP-14 — Content Pipeline and Production Content Scale-Up

Move from minimal fixtures to full deterministic authoring/cook workflow for meshes/materials/textures/shaders/collision/nav/terrain/gameplay definitions, plus incremental build/hot reload and provenance/license reports.

This phase can overlap content creation after its underlying contracts are stable, but runtime cannot depend on unvalidated ad-hoc source assets.

**Exit:** clean/incremental/no-op/worker-count cook determinism passes on representative content and shipping runtime uses cooked registry only.

## 18. IMP-15 — Scale, Performance, Persistence Compatibility, Hardening

Execute TA-13 scale envelopes, reference benchmarks, profiling, long scenarios, historical save migrations, fault injection, leak/race testing, recovery tests and quality-tier degradation rules.

**Exit:** controlled reference runner meets required percentile/memory/Hard-Hold gates for implemented representative scenes; no correctness gate is waived for performance.

## 19. IMP-16 — Content Complete / Release Certification Track

After approved GDS content is implemented, move through content-complete, alpha/beta stabilization and release certification using TA-14 exact-SHA evidence.

This phase does not permit new baseline systems merely because release is approaching; scope changes return through governance.

## 20. Parallelism Rule

Work may proceed in parallel only when dependencies are real and stable. Examples:

- renderer resource internals can advance while headless domain work advances, because they meet through locked snapshots/interfaces;
- content authoring can scale after schemas/cook contracts exist;
- UI screens can use read-model fakes in UI tests, but production cannot depend on fake stores;
- mission implementation cannot invent unfinished ownership semantics to bypass IMP-6/7 prerequisites.

## 21. Phase Exit Rule

A dependent phase may begin exploratory work before the previous phase fully closes, but it may not declare its own locked dependency satisfied until upstream required tests/interfaces are green and stable.

## 22. Immediate Next Dependency

After TA-16 closes, the project's next formal dependency is:

```text
IMP-1 — Core, Identity, Deterministic Simulation, and Transactions
```
