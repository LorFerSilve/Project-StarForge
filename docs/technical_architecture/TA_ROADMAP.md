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

Defines:

- Recast/Detour-backed tiled grounded navmesh behind a StarForge navigation adapter;
- Small/Standard/Heavy grounded navigation classes and actor-specific traversal profiles;
- separate bounded 3D navigation graph/volume for flying/Zero-G autonomous actors;
- typed traversal links for doors, airlocks, lifts, ladders, breaches, docking and free-flight portals;
- dynamic navigation overlays plus persistent-state-first tile/link invalidation and atomic NavigationRevision updates;
- asynchronous path jobs with SceneGeneration/NavigationRevision/actor/task/request freshness validation;
- deterministic worker-result consumption independent of completion order;
- project-owned path following, local avoidance and no-teleport stuck recovery;
- explicit world-truth vs AI-knowledge separation;
- vision, semantic hearing, equipment sensors, Last Known Position/confidence and provenance-preserving shared knowledge;
- generic enemy awareness/tactical AI with reaction timing, cover, search, flank, suppression, retreat and no omniscience;
- player robot command AI, ROE, communication degradation, fallback and squad coordination;
- crew assignment/task travel, emergency response, evacuation and TA-6 automation execution handoff;
- ActiveLocal/ReducedLocal/OffScreenLogical AI modes with chronological Simulation-Time travel/task boundaries;
- deterministic AI scheduler and exact integration into TA-7 fixed runtime phases;
- read-only AI diagnostics and headless validation.

Artifacts:

- `52_navigation_representation_and_traversal_profiles.md`;
- `53_navigation_topology_invalidation_and_links.md`;
- `54_async_pathfinding_and_path_following.md`;
- `55_perception_memory_and_shared_knowledge.md`;
- `56_decision_layers_enemy_combat_and_tactical_ai.md`;
- `57_robot_command_ai_and_squad_coordination.md`;
- `58_crew_task_navigation_and_station_behavior.md`;
- `59_ai_scheduling_and_offscreen_behavior_abstraction.md`;
- `60_ai_runtime_phase_integration_debugging_and_validation.md`;
- `TA8_CROSS_VALIDATION.md`.

## TA-9 — Missions, Raids, Dynamic Events, and Strategic State Machines

**Status:** Next

Must define MissionInstance runtime/persistent representation, objective state-machine infrastructure, deterministic procedural generation, raid phases, reinforcements, Dynamic Events, world-location consequences, recovery/communication integration, and finale transaction state.

## TA-10 — Content and Asset Pipeline

**Status:** Planned

Must define canonical source/exchange formats, glTF import, textures/materials, collision/navigation-content cooking, shader build pipeline, content IDs/schemas, validation tooling, runtime/cooked assets, registry/cache, dependency tracking, and hot-reload boundaries.

## TA-11 — Input, UI, Audio, and Presentation Integration

**Status:** Planned

Must define raw input → actions, context routing, remapping, controller/aim-assist boundary, shipping UI/HUD/read models, subtitles/captions, alarms, miniaudio adapter/buses/spatialization, and accessibility integration.

## TA-12 — Persistence Implementation Architecture

**Status:** Planned

Must define exact TA-2 save-container byte layout, section directory/manifest, domain serialization registry, CRC32C, write-new-then-commit, manual/quick/autosave layout, migrations, crash recovery, diagnostics, and deterministic load validation.

## TA-13 — Concurrency, Performance, Memory, and Streaming Budgets

**Status:** Planned

Must define worker-pool model, job priorities, snapshot/versioning, streaming/CPU/GPU/memory budgets, simulation backlog policy, profiling, performance scenes, station scalability, and numeric TA-3 through TA-8 budgets including entity counts, physics, station solvers, nav tile rebuilds, path jobs, perception queries, AI decision cadence, AI backlog, and off-screen actor advancement.

## TA-14 — Testing, Diagnostics, and CI Architecture

**Status:** Planned

Must define CMake/Catch2 layers, headless simulation tests, transaction/persistence tests, deterministic-seed regressions, content validation, renderer/physics/station/runtime-entity/AI/navigation smoke tests, formatting/tidy/warnings, sanitizers, CI gates, and debug-tool requirements.

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
→ **TA-9 next**  
→ TA-10 ... TA-15  
→ TA-16 implementation roadmap/locking  
→ C++/OpenGL scaffolding  
→ gameplay implementation.
