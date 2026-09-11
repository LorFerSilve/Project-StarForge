# Project StarForge — Technical Architecture Specification

> **Status:** Active  
> **Authority:** Technical realization of the Design Complete GDS

This directory translates the authoritative Game Design Specification under `docs/game_design/` into an implementable C++/OpenGL software architecture.

## Current Phase

The Game Design Specification is **Design Complete**.

Technical Architecture has completed:

- **TA-0 — Architecture Governance and Constraints**;
- **TA-1 — System Context, Toolchain, and Runtime Foundation**;
- **TA-2 — Identity, Domain State, Transactions, and Serialization Contracts**;
- **TA-3 — World, Scene, Zone, and Streaming Architecture**;
- **TA-4 — Rendering Architecture**;
- **TA-5 — Physics, Collision, Character, and Spaceflight Integration**;
- **TA-6 — Station Simulation and Graph Architecture**.

The next dependency is **TA-7 — Gameplay Runtime Entity Architecture**.

Gameplay implementation and repository scaffolding have not started. Technical contracts are defined first so implementation does not invent architecture ad hoc.

## Authority Relationship

The technical architecture:

- **must implement** the behavior defined by the Design Complete GDS;
- may choose software structure, data layout, libraries, update strategy, serialization representation, and module boundaries;
- may not silently change gameplay behavior;
- must escalate a technical infeasibility into an explicit design/architecture decision rather than altering the GDS implicitly.

When a technical document conflicts with the GDS, the GDS wins until a formal design change is accepted.

## Architectural Direction

Project StarForge is a **purpose-built modular monolith** in modern C++ with an OpenGL renderer.

The architecture deliberately avoids a general-purpose game engine, runtime plugin architecture, mandatory scripting VM, baseline networking/server architecture, one universal ECS for every persistent system, and hidden framework ownership of the game loop or renderer.

Commodity libraries are allowed when they remove non-differentiating infrastructure while leaving StarForge gameplay/rendering architecture under project control.

## Initial Technical Baseline

- Language: **C++23**;
- build system: **CMake**;
- dependency management: **vcpkg manifest mode** with pinned baseline/versions;
- graphics API: **OpenGL 4.6 Core Profile**;
- platform/window/input: **GLFW**;
- OpenGL loader: **glad2**;
- math: **GLM**;
- physics/collision: **Jolt Physics** behind a StarForge-owned adapter;
- audio backend: **miniaudio** behind a StarForge-owned audio layer;
- glTF 2.0 import: **fastgltf**;
- developer/debug UI: **Dear ImGui**, tooling only;
- tests: **Catch2**;
- build automation: GitHub + GitHub Actions.

Exact dependency versions remain implementation-roadmap/toolchain data and must be pinned before scaffolding.

## Architecture Baselines

### TA-1 Runtime Foundation

TA-1 establishes the modular-monolith process model, main-thread simulation/OpenGL authority, fixed **60 Hz** authoritative simulation, deterministic worker handoff, Stable Simulation Boundaries, and save-snapshot capture boundary.

### TA-2 Data / State

TA-2 establishes typed persistent IDs, nonserialized generation-checked runtime handles, Content IDs, domain stores, Activation Leases, StateRevision/ActivationEpoch, typed Command/Result/Event contracts, prepared atomic transactions, immutable Read Models, one-owner physical transfers, integer Credits, versioned Save DTOs, StarForge's chunked binary save family, staged all-or-nothing load/migration, and project-owned deterministic PCG32 RNG.

### TA-3 World / Scene

TA-3 establishes one authoritative player-local `SceneInstance`, strategic-vs-local coordinates, explicit Scene lifecycle/generation, streaming residency separated from simulation activation, Hard Streaming Hold, double-precision Context Space + floating origin, Horizon active/off-screen handoff, staged atomic context transitions, persistent world projection, and scene-composition profiles.

### TA-4 Rendering

TA-4 establishes one main-thread OpenGL 4.6 Core renderer, immutable `RenderSnapshot`, reversed-Z, HDR linear world rendering with SDR output, hybrid deferred/forward passes, glTF-compatible metallic-roughness PBR, bounded shadows/lights, view/camera contracts, culling/LOD/instancing/transparency, VFX/particles, GPU resource lifecycle, graphics/accessibility constraints, and resize/context-failure behavior.

### TA-5 Physics

TA-5 establishes one StarForge `PhysicsWorld` per active SceneInstance, Jolt encapsulation, semantic collision/filter/query contracts, kinematic `CharacterMotor`, swept project-owned projectiles, Dynamic 6DoF spacecraft, physical Hard Dock constraints, Zero-G/EVA/Magnetic Boots, collision-impact fact routing, fixed 60 Hz physics ordering, deferred safe physics mutation, and origin-rebase/load behavior without synthetic impacts.

### TA-6 Station Simulation

TA-6 establishes:

- canonical persistent Horizon module/port topology;
- separate Structural, Traversal, Pressure, Power, Thermal, Water, Logistics, and ControlData views/revisions;
- no universal station utility solver;
- deterministic capacity-constrained Power allocation with physical islands, breakers, storage, priorities, and black-start behavior;
- stable pressure cells with conserved Oxygen/CO2/Contaminant/Inert gas and portal-based pressure exchange;
- finite Thermal energy/coolant networks, radiator rejection, ambient temperature, and protection thresholds;
- separate Fresh Water/Wastewater inventories, distribution, reserve, recycling, and leaks;
- one-owner Logistics reservations, transfer jobs, in-transfer ownership, local buffers, and docking cargo links;
- persistent Simulation-Time `WorkOrder` execution for manufacturing, farming, construction, repair, and compatible station work;
- exactly-once manufacturing/harvest/completion milestones;
- construction/deconstruction/damage/repair topology mutation with dependent graph invalidation;
- ControlData/sensor-aware automation with finite capacity and policy/permission enforcement;
- chronological event/deadline-based off-screen Horizon simulation preserving active-scene outcomes;
- station runtime projection where persistent commits precede `StationGeometryDelta` application at TA-5 safe mutation boundaries.

## Architecture Documents

### TA-0 / TA-1

- [`00_architecture_authority.md`](00_architecture_authority.md)
- [`01_architecture_principles.md`](01_architecture_principles.md)
- [`02_system_context.md`](02_system_context.md)
- [`03_toolchain_and_dependencies.md`](03_toolchain_and_dependencies.md)
- [`04_runtime_and_simulation_model.md`](04_runtime_and_simulation_model.md)
- [`TA1_CROSS_VALIDATION.md`](TA1_CROSS_VALIDATION.md)

### TA-2

- [`05_identity_and_reference_model.md`](05_identity_and_reference_model.md)
- [`06_domain_state_and_activation_contracts.md`](06_domain_state_and_activation_contracts.md)
- [`07_command_result_and_event_contracts.md`](07_command_result_and_event_contracts.md)
- [`08_transaction_and_ownership_contracts.md`](08_transaction_and_ownership_contracts.md)
- [`09_read_models_and_state_revisions.md`](09_read_models_and_state_revisions.md)
- [`10_serialization_and_save_contracts.md`](10_serialization_and_save_contracts.md)
- [`11_rng_migration_and_integrity_contracts.md`](11_rng_migration_and_integrity_contracts.md)
- [`TA2_CROSS_VALIDATION.md`](TA2_CROSS_VALIDATION.md)

### TA-3

- [`12_active_local_context_and_scene_model.md`](12_active_local_context_and_scene_model.md)
- [`13_zone_streaming_and_residency_model.md`](13_zone_streaming_and_residency_model.md)
- [`14_coordinate_precision_and_spatial_partitioning.md`](14_coordinate_precision_and_spatial_partitioning.md)
- [`15_horizon_active_offscreen_handoff.md`](15_horizon_active_offscreen_handoff.md)
- [`16_scene_transition_and_loading_state_machine.md`](16_scene_transition_and_loading_state_machine.md)
- [`17_zone_state_and_persistent_world_projection.md`](17_zone_state_and_persistent_world_projection.md)
- [`18_scene_composition_profiles.md`](18_scene_composition_profiles.md)
- [`TA3_CROSS_VALIDATION.md`](TA3_CROSS_VALIDATION.md)

### TA-4

- [`19_renderer_ownership_and_frame_pipeline.md`](19_renderer_ownership_and_frame_pipeline.md)
- [`20_render_graph_and_passes.md`](20_render_graph_and_passes.md)
- [`21_camera_and_view_system.md`](21_camera_and_view_system.md)
- [`22_material_shader_and_lighting_model.md`](22_material_shader_and_lighting_model.md)
- [`23_visibility_culling_batching_and_transparency.md`](23_visibility_culling_batching_and_transparency.md)
- [`24_vfx_particles_and_debug_rendering.md`](24_vfx_particles_and_debug_rendering.md)
- [`25_gpu_resource_upload_and_lifecycle.md`](25_gpu_resource_upload_and_lifecycle.md)
- [`26_graphics_settings_resize_and_failure_recovery.md`](26_graphics_settings_resize_and_failure_recovery.md)
- [`TA4_CROSS_VALIDATION.md`](TA4_CROSS_VALIDATION.md)

### TA-5

- [`27_physics_adapter_and_world_lifecycle.md`](27_physics_adapter_and_world_lifecycle.md)
- [`28_collision_layers_filters_and_queries.md`](28_collision_layers_filters_and_queries.md)
- [`29_character_controller_and_ground_movement.md`](29_character_controller_and_ground_movement.md)
- [`30_triggers_interactions_projectiles_and_sweeps.md`](30_triggers_interactions_projectiles_and_sweeps.md)
- [`31_spacecraft_rigidbody_and_flight_integration.md`](31_spacecraft_rigidbody_and_flight_integration.md)
- [`32_docking_constraints_and_zero_g.md`](32_docking_constraints_and_zero_g.md)
- [`33_collision_damage_and_structural_contacts.md`](33_collision_damage_and_structural_contacts.md)
- [`34_physics_tick_origin_shift_and_snapshot_sync.md`](34_physics_tick_origin_shift_and_snapshot_sync.md)
- [`TA5_CROSS_VALIDATION.md`](TA5_CROSS_VALIDATION.md)

### TA-6

- [`35_station_graph_foundation_and_topology.md`](35_station_graph_foundation_and_topology.md)
- [`36_power_network_solver_and_allocation.md`](36_power_network_solver_and_allocation.md)
- [`37_atmosphere_compartments_and_pressure_simulation.md`](37_atmosphere_compartments_and_pressure_simulation.md)
- [`38_thermal_water_and_environmental_networks.md`](38_thermal_water_and_environmental_networks.md)
- [`39_logistics_reservations_and_transfer_runtime.md`](39_logistics_reservations_and_transfer_runtime.md)
- [`40_manufacturing_farming_and_work_scheduling.md`](40_manufacturing_farming_and_work_scheduling.md)
- [`41_construction_repair_and_topology_mutation.md`](41_construction_repair_and_topology_mutation.md)
- [`42_station_automation_control_and_offscreen_simulation.md`](42_station_automation_control_and_offscreen_simulation.md)
- [`43_station_runtime_projection_and_physics_handoff.md`](43_station_runtime_projection_and_physics_handoff.md)
- [`TA6_CROSS_VALIDATION.md`](TA6_CROSS_VALIDATION.md)

### Governance

- [`ARCHITECTURE_DECISIONS.md`](ARCHITECTURE_DECISIONS.md)
- [`TA_ROADMAP.md`](TA_ROADMAP.md)

Later TA phases define runtime entities, AI/navigation, missions/raids, content pipeline, input/UI/audio integration, persistence storage details, performance/concurrency, testing/CI, integration audit, and implementation handoff.

## Implementation Gate

A technical subsystem is ready for code only when:

1. its relevant GDS contracts are Design Complete;
2. its technical architecture contract is sufficiently complete;
3. ownership/lifetime/threading boundaries are explicit;
4. data and persistence boundaries are explicit where applicable;
5. dependencies are selected and pinned where required;
6. tests/validation expectations are defined;
7. the implementation roadmap places it in an approved phase.

`Implementation Locked` remains a later per-contract handoff state. **TA-6 Architecture Complete does not authorize C++/OpenGL scaffolding yet.**
