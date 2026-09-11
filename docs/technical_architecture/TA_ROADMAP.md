# Technical Architecture Roadmap

> **Status:** Active  
> **Authority:** Dependency-driven technical architecture sequencing

This roadmap defines the technical architecture work that must be completed before and during implementation handoff.

## TA-0 — Architecture Governance and Constraints

**Status:** Complete

Defines architecture authority, precedence, status model, no-general-engine principle, GDS traceability, third-party dependency policy, and implementation gate.

## TA-1 — System Context, Toolchain, and Runtime Foundation

**Status:** Architecture Complete

Defines the modular monolith, C++23/CMake/vcpkg/OpenGL baseline, dependency boundaries, 60 Hz authoritative simulation, worker authority, Stable Simulation Boundary, and save snapshot capture boundary.

## TA-2 — Identity, Domain State, Transactions, and Serialization Contracts

**Status:** Architecture Complete

Defines typed persistent/runtime identities, domain stores, Activation Leases, revisions/epochs, typed commands/results/events, transactions, read models, physical ownership, Save DTO/container, migration/integrity, and deterministic PCG32 RNG.

## TA-3 — World, Scene, Zone, and Streaming Architecture

**Status:** Architecture Complete

Defines one authoritative local SceneInstance, scene lifecycle/generation, streaming residency vs activation, Hard Streaming Hold, double-precision Context Space/floating origin, Horizon handoff, atomic transitions, persistent world projection, and scene profiles.

## TA-4 — Rendering Architecture

**Status:** Architecture Complete

Defines main-thread OpenGL ownership, RenderSnapshot interpolation, hybrid deferred/forward rendering, reversed-Z, HDR/SDR, passes, shadows/lights, PBR, cameras, culling/LOD/instancing/transparency, VFX, GPU lifetime, and graphics tiers.

## TA-5 — Physics, Collision, Character, and Spaceflight Integration

**Status:** Architecture Complete

Defines Jolt adapter/world ownership, collision/query contracts, CharacterMotor, projectiles, Dynamic 6DoF spacecraft, docking, Zero-G/EVA, impact/contact facts, fixed physics ordering, deferred mutation, and floating-origin/load behavior.

Artifacts: `27_physics_adapter_and_world_lifecycle.md` through `34_physics_tick_origin_shift_and_snapshot_sync.md`, plus `TA5_CROSS_VALIDATION.md`.

## TA-6 — Station Simulation and Graph Architecture

**Status:** Architecture Complete

Defines Horizon topology, typed utility graph views, deterministic Power, conserved atmosphere/thermal/water, one-owner logistics, WorkOrders, topology mutation, automation/control, chronological off-screen simulation, and persistent-state-first projection.

Artifacts: `35_station_graph_foundation_and_topology.md` through `43_station_runtime_projection_and_physics_handoff.md`, plus `TA6_CROSS_VALIDATION.md`.

## TA-7 — Gameplay Runtime Entity Architecture

**Status:** Architecture Complete

Defines one runtime registry per active scene, generation-checked handles, typed component pools, persistent-actor Activation Lease bridges, actor lifecycle, player/inventory/equipment runtime, combat/status/projectile/interactable projections, persistent/runtime synchronization, deferred destruction, deterministic runtime phases, and headless execution.

Artifacts: `44_runtime_entity_registry_and_handles.md` through `51_runtime_update_phases_and_system_boundaries.md`, plus `TA7_CROSS_VALIDATION.md`.

## TA-8 — AI and Navigation Architecture

**Status:** Architecture Complete

Defines Recast/Detour-backed grounded navigation, separate bounded 3D free-flight navigation, typed traversal profiles/links, persistent-state-first navigation invalidation, revision-validated asynchronous pathfinding, project-owned path following/avoidance, knowledge-limited perception/memory, enemy tactical AI, robot command/squad AI, crew task/emergency navigation, deterministic AI scheduling, active/off-screen logical AI equivalence, and headless diagnostics/validation.

Artifacts: `52_navigation_representation_and_traversal_profiles.md` through `60_ai_runtime_phase_integration_debugging_and_validation.md`, plus `TA8_CROSS_VALIDATION.md`.

## TA-9 — Missions, Raids, Dynamic Events, and Strategic State Machines

**Status:** Architecture Complete

Defines:

- persistent MissionId versus per-deployment MissionInstanceId and transactional single-external-deployment authority;
- persistent multi-zone mission attempts, retry history, no-reroll/no-refund failure semantics, and exactly-once resolution;
- typed acyclic objective graphs driven by committed owning-domain facts/state;
- explicit objective branch, combat-end-state, resource-security, rescue, repair, scan, timer, hidden-objective and extraction semantics;
- deterministic staged procedural mission generation using persistent cursors, scoped PCG32 streams, bounded validation and stable major content;
- offensive Raids as specialized MissionInstances against persistent targets;
- independent raid phase/escalation state with immediate persistent target damage, sabotage and physical loot consequences;
- finite reinforcement response calls with a pre-commit interruption boundary, persistent post-commit force manifest, strategic ETA and physical arrival;
- persistent Horizon DefenseEvent execution in active/off-screen modes without an authoritative single Defense Score;
- physical hostile theft/extraction and causal station recovery;
- DynamicEventStore/StrategicEventScheduler with source plausibility, concurrency limits, cooldowns, Recovery Grace and softlock validation;
- atomic DynamicEvent handoff to MissionId/DefenseEventId without duplicated execution state machines;
- strategic communication/knowledge delivery separate from event existence and remote-command physical feasibility;
- external-defeat Recovery Transit with Simulation-Time anti-teleport destination logic and real asset-location preservation;
- typed cross-domain strategic consequence batches;
- exactly-once MS-F02 Stabilize/Sever/Contain FinalResolution transaction and persistent PostgameResolutionState;
- deterministic local/strategic phase integration, chronological cross-scheduler boundaries, Stable Save Boundary invariants and headless diagnostics/testing contracts.

Artifacts:

- `61_mission_store_instance_and_deployment_state.md`;
- `62_objective_graph_runtime_and_commit_semantics.md`;
- `63_procedural_mission_generation_and_offer_runtime.md`;
- `64_raid_state_machine_and_target_runtime.md`;
- `65_reinforcements_escalation_and_defense_events.md`;
- `66_dynamic_event_scheduler_and_world_consequence_runtime.md`;
- `67_communications_recovery_and_strategic_consequence_routing.md`;
- `68_finale_postgame_and_irreversible_choice_transactions.md`;
- `69_ta9_runtime_phase_integration_debugging_and_validation.md`;
- `TA9_CROSS_VALIDATION.md`.

## TA-10 — Content and Asset Pipeline

**Status:** Next

Must define canonical source/exchange formats, glTF import, textures/materials, collision/navigation-content cooking, shader build pipeline, content IDs/schemas, validation tooling, runtime/cooked assets, registry/cache, dependency tracking, and hot-reload boundaries.

## TA-11 — Input, UI, Audio, and Presentation Integration

**Status:** Planned

Must define raw input → actions, context routing, remapping, controller/aim-assist boundary, shipping UI/HUD/read models, subtitles/captions, alarms, miniaudio adapter/buses/spatialization, and accessibility integration.

## TA-12 — Persistence Implementation Architecture

**Status:** Planned

Must define exact TA-2 save-container byte layout, section directory/manifest, domain serialization registry, CRC32C, write-new-then-commit, manual/quick/autosave layout, migrations, crash recovery, diagnostics, and deterministic load validation.

## TA-13 — Concurrency, Performance, Memory, and Streaming Budgets

**Status:** Planned

Must define worker-pool model, job priorities, snapshot/versioning, streaming/CPU/GPU/memory budgets, simulation backlog policy, profiling, performance scenes, station scalability, and numeric TA-3 through TA-9 budgets including entities, physics, station solvers, navigation/path/perception/AI, objective evaluation, procedural-generation jobs, strategic-event scheduling, off-screen DefenseEvent advancement, and strategic backlog limits.

## TA-14 — Testing, Diagnostics, and CI Architecture

**Status:** Planned

Must define CMake/Catch2 layers, headless simulation tests, transaction/persistence tests, deterministic-seed regressions, content validation, renderer/physics/station/runtime-entity/AI/navigation/mission/raid/event smoke and deterministic tests, formatting/tidy/warnings, sanitizers, CI gates, and debug-tool requirements.

## TA-15 — Architecture Integration Audit

**Status:** Planned

Cross-validates TA-0 through TA-14 against all Design Complete GDS contracts, ownership/lifetime, threading, persistence, world/scene transitions, performance assumptions, dependency cycles, failure recovery, and testability.

No implementation phase is authorized to invent unresolved architecture after this audit.

## TA-16 — Implementation Roadmap and Contract Locking

**Status:** Planned

Produces dependency-ordered implementation phases, vertical-slice definition, scaffolding plan, exact initial CMake/vcpkg targets, first test gates, per-contract `Implementation Locked` handoff, merge/branch strategy, and milestone criteria.

Only after TA-16 may planned C++/OpenGL scaffolding begin.

## Current Sequence

GDS Design Complete  
→ TA-0 complete  
→ TA-1 Architecture Complete  
→ TA-2 Architecture Complete  
→ TA-3 Architecture Complete  
→ TA-4 Architecture Complete  
→ TA-5 Architecture Complete  
→ TA-6 Architecture Complete  
→ TA-7 Architecture Complete  
→ TA-8 Architecture Complete  
→ TA-9 Architecture Complete  
→ **TA-10 next**  
→ TA-11 ... TA-15  
→ TA-16 implementation roadmap/locking  
→ C++/OpenGL scaffolding  
→ gameplay implementation.
