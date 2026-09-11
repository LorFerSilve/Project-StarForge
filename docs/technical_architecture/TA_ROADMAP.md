# Technical Architecture Roadmap

> **Status:** Active  
> **Authority:** Dependency-driven technical architecture sequencing

This roadmap defines the technical architecture work that must be completed before and during implementation handoff.

## TA-0 — Architecture Governance and Constraints

**Status:** Complete

Defines architecture authority, precedence, status model, no-general-engine principle, GDS traceability, third-party dependency policy, and implementation gate.

## TA-1 — System Context, Toolchain, and Runtime Foundation

**Status:** Architecture Complete

Defines the modular monolith, C++23/CMake/vcpkg/OpenGL baseline, dependency boundaries, 60 Hz authoritative simulation, command/transaction/event model, worker authority, Stable Simulation Boundary, and save snapshot capture boundary.

## TA-2 — Identity, Domain State, Transactions, and Serialization Contracts

**Status:** Architecture Complete

Defines typed persistent/runtime identities, ContentId, domain stores, Activation Leases, revisions/epochs, typed commands/results/events, deterministic transaction commit, immutable read models, physical ownership, Credit ledger transactions, versioned Save DTOs, the StarForge save container, migrations/integrity, and deterministic PCG32 RNG.

## TA-3 — World, Scene, Zone, and Streaming Architecture

**Status:** Architecture Complete

Defines one authoritative player-local SceneInstance, strategic-vs-local coordinates, scene lifecycle/generation, streaming residency versus activation, Hard Streaming Hold, double-precision Context Space, floating origin, local spatial indexing, Horizon active/off-screen handoff, atomic context transitions, persistent world projection, and scene-composition profiles.

## TA-4 — Rendering Architecture

**Status:** Architecture Complete

Defines main-thread OpenGL ownership, RenderSnapshot interpolation, hybrid deferred/forward rendering, reversed-Z, HDR/SDR output, render passes, shadows, tiled lights, glTF-compatible PBR, shader families, cameras/views, culling/LOD/instancing/transparency, particles/VFX/debug rendering, GPU uploads/resource lifetime, resize/context-failure handling, and presentation-only graphics tiers.

## TA-5 — Physics, Collision, Character, and Spaceflight Integration

**Status:** Architecture Complete

Defines Jolt adapter/world ownership, semantic collision/query contracts, kinematic CharacterMotor, swept/dynamic projectile physics, Dynamic 6DoF spacecraft, physical Hard Dock constraints, Zero-G/EVA/Magnetic Boots, normalized impact/contact facts, collision-damage routing, fixed 60 Hz physics phase ordering, deferred safe backend mutation, and floating-origin/load behavior without synthetic impacts.

Artifacts: `27_physics_adapter_and_world_lifecycle.md` through `34_physics_tick_origin_shift_and_snapshot_sync.md`, plus `TA5_CROSS_VALIDATION.md`.

## TA-6 — Station Simulation and Graph Architecture

**Status:** Architecture Complete

Defines canonical Horizon topology, Structural/Traversal/Pressure/Power/Thermal/Water/Logistics/ControlData graph views, deterministic capacity-constrained Power, conserved atmosphere/thermal/water state, one-owner logistics, persistent Simulation-Time WorkOrders, construction/damage/repair topology mutation, automation/control, chronological off-screen simulation, and persistent-state-first station projection into TA-5 physics.

Artifacts: `35_station_graph_foundation_and_topology.md` through `43_station_runtime_projection_and_physics_handoff.md`, plus `TA6_CROSS_VALIDATION.md`.

## TA-7 — Gameplay Runtime Entity Architecture

**Status:** Architecture Complete

Defines:

- one `RuntimeEntityRegistry` per active SceneInstance;
- generation-checked RuntimeEntityHandle plus SceneGeneration scoping;
- explicit persistent-ID/runtime-handle separation;
- typed component pools with no universal polymorphic GameObject hierarchy;
- deterministic query ordering where gameplay results depend on order;
- buffered runtime structural mutation;
- typed persistent actor activation/deactivation through TA-2 Activation Leases;
- actor lifecycle states `Preparing -> Active -> Closing -> PendingDestroy -> Reclaimed`;
- one locally controlled player runtime entity;
- player Health/control/action runtime state without hidden penalties;
- Inventory/Equipment runtime references that preserve one physical owner;
- world-item pickup/partial-stack/Auto Pickup transaction ordering;
- combat actor, weapon, shield, hit-zone, damage and deterministic Status runtime state;
- TA-5 physical facts routed through GDS-9 Combat to owning gameplay domains;
- swept/dynamic projectile runtime contracts;
- Interactable, Container, Hazard, MissionObject, Door and Station projection runtime entities;
- persistent/runtime synchronization with ActivationEpoch/revision validation;
- logical removal before deferred entity/component/physics/render/audio reclamation;
- a single deterministic 60 Hz runtime phase order from intents through immutable snapshot publication;
- headless runtime execution for testing.

Artifacts:

- `44_runtime_entity_registry_and_handles.md`;
- `45_component_storage_and_query_model.md`;
- `46_actor_lifecycle_activation_and_deactivation.md`;
- `47_player_inventory_and_equipment_runtime.md`;
- `48_combat_actor_weapon_damage_and_status_runtime.md`;
- `49_projectiles_interactables_and_world_runtime_objects.md`;
- `50_persistent_runtime_sync_and_deferred_destruction.md`;
- `51_runtime_update_phases_and_system_boundaries.md`;
- `TA7_CROSS_VALIDATION.md`.

## TA-8 — AI and Navigation Architecture

**Status:** Next

Must define:

- navigation representation for interiors/surfaces;
- dynamic path invalidation;
- heavy/light robot traversal constraints;
- crew task navigation;
- enemy perception/memory;
- robot command AI;
- station automation scheduling;
- asynchronous pathfinding job model;
- off-screen behavior abstraction;
- debugging/validation.

## TA-9 — Missions, Raids, Dynamic Events, and Strategic State Machines

**Status:** Planned

Must define MissionInstance runtime/persistent representation, objective state-machine infrastructure, deterministic procedural generation, raid phases, reinforcements, Dynamic Events, world-location consequences, recovery/communication integration, and finale transaction state.

## TA-10 — Content and Asset Pipeline

**Status:** Planned

Must define canonical source/exchange formats, glTF import, textures/materials, collision-content cooking, shader build pipeline, content IDs/schemas, validation tooling, runtime/cooked assets where needed, asset registry/cache, dependency tracking, and hot-reload boundaries.

## TA-11 — Input, UI, Audio, and Presentation Integration

**Status:** Planned

Must define raw input → actions, context routing, remapping, controller/aim-assist boundary, shipping UI/HUD/read models, subtitles/captions, alarms, miniaudio adapter/buses/spatialization, and accessibility integration.

## TA-12 — Persistence Implementation Architecture

**Status:** Planned

Must define exact TA-2 save-container byte layout, section directory/manifest, domain serialization registry, CRC32C implementation, write-new-then-commit flow, manual/quick/autosave layout, migration implementation, crash recovery, diagnostics, and deterministic load validation.

## TA-13 — Concurrency, Performance, Memory, and Streaming Budgets

**Status:** Planned

Must define worker-pool model, job categories/priorities, immutable snapshot/versioning, asset streaming budgets, CPU/GPU frame budgets, memory budgets, simulation backlog policy, profiling counters, performance test scenes, Horizon/off-screen scalability, TA-3 streaming/origin/grid/cache budgets, TA-4 rendering budgets, TA-5 physics/query/body/contact budgets, TA-6 station graph/solver/off-screen budgets, and TA-7 entity/component/query/projectile/status/runtime-structural-mutation budgets.

## TA-14 — Testing, Diagnostics, and CI Architecture

**Status:** Planned

Must define CMake target tests, Catch2 layers, headless simulation tests, transaction/persistence tests, deterministic-seed regressions, content validation, renderer/physics/station/runtime-entity smoke tests, formatting/tidy/warnings, sanitizers where supported, CI gates, and debug-tool requirements.

## TA-15 — Architecture Integration Audit

**Status:** Planned

Cross-validates TA-0 through TA-14 against all Design Complete GDS contracts, ownership/lifetime, threading, persistence, world/scene transition consistency, performance assumptions, subsystem dependency cycles, failure recovery, and testability.

No implementation phase is authorized to invent unresolved architecture after this audit.

## TA-16 — Implementation Roadmap and Contract Locking

**Status:** Planned

Produces dependency-ordered implementation phases, vertical-slice definition, scaffolding plan, exact initial CMake/vcpkg targets, first test gates, per-contract `Implementation Locked` handoff, merge/branch strategy, and milestone criteria.

Only after TA-16 may the planned C++/OpenGL scaffolding phase begin.

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
→ **TA-8 next**  
→ TA-9 ... TA-15  
→ TA-16 implementation roadmap/locking  
→ C++/OpenGL scaffolding  
→ gameplay implementation.
