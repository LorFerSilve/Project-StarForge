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
- **TA-6 — Station Simulation and Graph Architecture**;
- **TA-7 — Gameplay Runtime Entity Architecture**.

The next dependency is **TA-8 — AI and Navigation Architecture**.

Gameplay implementation and repository scaffolding have not started. Technical contracts are defined first so implementation does not invent architecture ad hoc.

## Authority Relationship

The technical architecture must implement the Design Complete GDS, may choose software/data/runtime realization, may not silently change gameplay behavior, and must escalate technical infeasibility through explicit design/architecture change control.

## Architectural Direction

Project StarForge is a **purpose-built modular monolith** in C++23 with a custom OpenGL renderer. It deliberately avoids a general-purpose game engine, runtime plugin architecture, mandatory scripting VM, baseline networking architecture, one universal persistent ECS, and hidden framework ownership of the game loop.

## Initial Technical Baseline

- C++23;
- CMake;
- vcpkg manifest mode;
- OpenGL 4.6 Core Profile;
- GLFW;
- glad2;
- GLM;
- Jolt Physics behind a StarForge adapter;
- miniaudio behind a StarForge audio layer;
- fastgltf / glTF 2.0;
- Dear ImGui for development tooling only;
- Catch2;
- GitHub + GitHub Actions.

Exact dependency versions remain implementation-roadmap/toolchain data and must be pinned before scaffolding.

## Architecture Baselines

### TA-1 Runtime Foundation

Modular-monolith process model, main-thread simulation/OpenGL authority, fixed **60 Hz** authoritative simulation, deterministic worker handoff, Stable Simulation Boundaries, and save-snapshot capture boundary.

### TA-2 Data / State

Typed persistent IDs, generation-checked runtime handles, Content IDs, domain stores, Activation Leases, revisions/epochs, typed commands/results/events, prepared atomic transactions, immutable Read Models, one-owner physical transfers, Credits ledger, Save DTO/container contracts, staged load/migration, and deterministic PCG32 RNG.

### TA-3 World / Scene

One authoritative player-local `SceneInstance`, strategic-vs-local coordinates, Scene lifecycle/generation, streaming residency separate from simulation activation, Hard Streaming Hold, double-precision Context Space + floating origin, Horizon active/off-screen handoff, staged atomic transitions, persistent world projection, and scene profiles.

### TA-4 Rendering

Main-thread OpenGL 4.6 renderer, immutable `RenderSnapshot`, reversed-Z, linear HDR → SDR, hybrid deferred/forward rendering, glTF-compatible PBR, shadows/lights, camera contracts, culling/LOD/instancing/transparency, VFX/particles, GPU resource lifetime, and presentation-only graphics tiers.

### TA-5 Physics

One StarForge `PhysicsWorld` per active SceneInstance, Jolt encapsulation, semantic collision/query contracts, kinematic `CharacterMotor`, swept projectiles, Dynamic 6DoF spacecraft, physical docking constraints, Zero-G/EVA/Magnetic Boots, collision fact routing, deferred safe physics mutation, and no synthetic load/rebase impacts.

### TA-6 Station Simulation

Canonical Horizon topology with separate Structural/Traversal/Pressure/Power/Thermal/Water/Logistics/ControlData views; deterministic capacity-constrained Power; conserved atmosphere/thermal/water; one-owner logistics; persistent Simulation-Time WorkOrders; construction/damage/repair topology mutation; automation/control; chronological off-screen station simulation; and persistent-state-first projection into TA-5 physics.

### TA-7 Gameplay Runtime Entities

TA-7 establishes:

- exactly one `RuntimeEntityRegistry` per active SceneInstance;
- generation-checked `RuntimeEntityHandle` plus SceneGeneration validation;
- explicit separation between runtime handles and persistent IDs;
- typed sparse/packed component pools rather than a universal polymorphic `GameObject` hierarchy;
- buffered structural component/entity mutation;
- typed persistent-actor activation adapters over TA-2 Activation Leases;
- atomic actor activation/deactivation with Closing/PendingDestroy lifecycle states;
- one locally controlled Player runtime entity;
- Inventory/Equipment runtime references that never duplicate physical ownership;
- sustained player actions and equipment use driven by Simulation Time;
- combat actor, weapon, hit-zone, shield, Health and deterministic Status runtime contracts;
- TA-5 physical facts → GDS-9 Combat → owning-domain consequence routing;
- swept/dynamic projectile runtime aligned with TA-5;
- typed Interactable, WorldItem, Container, Hazard, MissionObject and Station projection entities;
- persistent/runtime synchronization via ActivationEpoch and revisions;
- logical removal before deferred backend/component reclamation;
- one deterministic 60 Hz runtime phase order from intents through physics, consequences, commits, cleanup and immutable snapshot publication;
- headless gameplay-runtime execution independent of OpenGL/audio/UI.

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

### TA-7

- [`44_runtime_entity_registry_and_handles.md`](44_runtime_entity_registry_and_handles.md)
- [`45_component_storage_and_query_model.md`](45_component_storage_and_query_model.md)
- [`46_actor_lifecycle_activation_and_deactivation.md`](46_actor_lifecycle_activation_and_deactivation.md)
- [`47_player_inventory_and_equipment_runtime.md`](47_player_inventory_and_equipment_runtime.md)
- [`48_combat_actor_weapon_damage_and_status_runtime.md`](48_combat_actor_weapon_damage_and_status_runtime.md)
- [`49_projectiles_interactables_and_world_runtime_objects.md`](49_projectiles_interactables_and_world_runtime_objects.md)
- [`50_persistent_runtime_sync_and_deferred_destruction.md`](50_persistent_runtime_sync_and_deferred_destruction.md)
- [`51_runtime_update_phases_and_system_boundaries.md`](51_runtime_update_phases_and_system_boundaries.md)
- [`TA7_CROSS_VALIDATION.md`](TA7_CROSS_VALIDATION.md)

### Governance

- [`ARCHITECTURE_DECISIONS.md`](ARCHITECTURE_DECISIONS.md)
- [`TA_ROADMAP.md`](TA_ROADMAP.md)

Later TA phases define AI/navigation, missions/raids, content pipeline, input/UI/audio integration, persistence implementation details, performance/concurrency, testing/CI, integration audit, and implementation handoff.

## Implementation Gate

Technical subsystems reach code only after the relevant Design Complete GDS, Architecture Complete technical contract, explicit ownership/lifetime/threading/persistence boundaries, dependency/toolchain decisions, validation expectations, and implementation-roadmap approval exist.

`Implementation Locked` remains a later per-contract handoff state. **TA-7 Architecture Complete does not authorize C++/OpenGL scaffolding yet.**
