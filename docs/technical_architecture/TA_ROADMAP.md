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

Defines one authoritative player-local SceneInstance, strategic-vs-local coordinates, ActiveLocalContext and SceneProfile kinds, scene lifecycle/generation, streaming residency versus activation, Hard Streaming Hold, double-precision Context Space, floating origin, local spatial indexing, Horizon active/off-screen handoff, atomic context transitions, persistent world projection, and scene-composition profiles.

## TA-4 — Rendering Architecture

**Status:** Architecture Complete

Defines main-thread OpenGL ownership, RenderSnapshot interpolation, hybrid deferred/forward rendering, reversed-Z, HDR/SDR output, render passes, shadows, tiled lights, glTF-compatible PBR, shader families, cameras/views, culling/LOD/instancing/transparency, particles/VFX/debug rendering, GPU uploads/resource lifetime, resize/context-failure handling, and presentation-only graphics tiers.

## TA-5 — Physics, Collision, Character, and Spaceflight Integration

**Status:** Architecture Complete

Defines:

- StarForge-owned Jolt adapter and one PhysicsWorld per active SceneInstance;
- generation-checked physics handles and no serialized backend identity;
- semantic collision layers, pair filtering, CollisionMaterialId, and typed query families;
- locomotion bodies separated from Combat hit-zone/query shapes;
- kinematic CharacterMotor for grounded movement, crouch, jump, slopes, steps, platforms, mantle, ladders, and safe-position recovery;
- trigger/interaction physical facts that never directly commit gameplay;
- swept project-owned fast projectiles plus Dynamic grenade/bounce bodies where appropriate;
- melee sweep and explosion-candidate query architecture;
- Dynamic spacecraft rigid bodies with mass/inertia and bounded 6DoF force/torque;
- Flight Assist and local safe-speed envelope through actual avionics/thruster authority;
- physical Hard Dock capture constraints and explicit undock/separation;
- Zero-G/EVA momentum, stabilization thrusters, and Magnetic Boot support frames;
- physics-to-gameplay Impact/Contact Facts and collision-damage routing;
- fixed 60 Hz physics phase ordering;
- deferred safe body/constraint mutation after gameplay commit;
- floating-origin rebase synchronization with no synthetic impact/trigger events;
- coherent save/activation/RenderSnapshot synchronization.

Artifacts:

- `27_physics_adapter_and_world_lifecycle.md`;
- `28_collision_layers_filters_and_queries.md`;
- `29_character_controller_and_ground_movement.md`;
- `30_triggers_interactions_projectiles_and_sweeps.md`;
- `31_spacecraft_rigidbody_and_flight_integration.md`;
- `32_docking_constraints_and_zero_g.md`;
- `33_collision_damage_and_structural_contacts.md`;
- `34_physics_tick_origin_shift_and_snapshot_sync.md`;
- `TA5_CROSS_VALIDATION.md`.

## TA-6 — Station Simulation and Graph Architecture

**Status:** Architecture Complete

Defines:

- one canonical persistent Horizon topology foundation with stable station-local typed IDs;
- separate Structural and Traversal graphs plus stable Pressure Cells/Portals;
- typed utility-link channels and independent topology revisions;
- deterministic dependency invalidation after construction, damage, repair, door/valve/breaker, breach, and docking changes;
- deterministic capacity-constrained Power allocation per connected island with `PowerLoadPriority`, producer dispatch, finite storage, breakers, protection, blackout, and black-start behavior;
- conserved Atmosphere species quantities in stable pressure cells with portal/ventilation exchange, breaches, life-support processing, airlock pressure facts, and bounded decompression-force handoff;
- finite Thermal energy/coolant state, coolant-loop throughput, radiator rejection, ambient-temperature coupling, and equipment protection facts;
- separate Fresh Water/Wastewater inventories with finite storage, distribution, reserve policy, recycling loss, and leaks;
- one-owner Logistics requests/reservations, deterministic source/routing selection, finite throughput, explicit `TransferCargoOwner`, local buffers, and docking cargo links;
- persistent Simulation-Time WorkOrders for Manufacturing, Farming, Construction, Repair, and compatible station work;
- exactly-once material consumption/output/harvest/completion milestones;
- construction/deconstruction transactions, structural-completion/commissioning boundaries, persistent damage/fault/breach state, stabilization, repair, and topology restoration;
- explicit ControlData connectivity, sensor knowledge, local safety controllers, automation tasks/policies/permissions, finite automation capacity, and escalation;
- chronological event/deadline-based active/off-screen station scheduling using the same persistent Horizon state;
- persistent-state-first station runtime projection with `StationGeometryDelta` handoff to TA-5 deferred safe physics mutation;
- active/off-screen/docking/defense continuity without duplicate station state.

Artifacts:

- `35_station_graph_foundation_and_topology.md`;
- `36_power_network_solver_and_allocation.md`;
- `37_atmosphere_compartments_and_pressure_simulation.md`;
- `38_thermal_water_and_environmental_networks.md`;
- `39_logistics_reservations_and_transfer_runtime.md`;
- `40_manufacturing_farming_and_work_scheduling.md`;
- `41_construction_repair_and_topology_mutation.md`;
- `42_station_automation_control_and_offscreen_simulation.md`;
- `43_station_runtime_projection_and_physics_handoff.md`;
- `TA6_CROSS_VALIDATION.md`.

## TA-7 — Gameplay Runtime Entity Architecture

**Status:** Next

Must define:

- runtime entity registry;
- component pools;
- actor lifecycle;
- player runtime state;
- item/equipment runtime state;
- combat-facing entities;
- projectiles/status effects;
- interactive objects;
- persistent/runtime synchronization;
- deferred destruction.

## TA-8 — AI and Navigation Architecture

**Status:** Planned

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

Must define worker-pool model, job categories/priorities, immutable snapshot/versioning, asset streaming budgets, CPU/GPU frame budgets, memory budgets, simulation backlog policy, profiling counters, performance test scenes, Horizon/off-screen scalability, TA-3 streaming/origin/grid/cache numeric budgets, TA-4 rendering budgets, TA-5 physics/query/body/contact budgets, and TA-6 station graph/solver/environment/off-screen-scheduler budgets.

## TA-14 — Testing, Diagnostics, and CI Architecture

**Status:** Planned

Must define CMake target tests, Catch2 layers, headless simulation tests, transaction/persistence tests, deterministic-seed regressions, content validation, renderer/physics/station-simulation smoke tests, formatting/tidy/warnings, sanitizers where supported, CI gates, and debug-tool requirements.

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
→ **TA-7 next**  
→ TA-8 ... TA-15  
→ TA-16 implementation roadmap/locking  
→ C++/OpenGL scaffolding  
→ gameplay implementation.
