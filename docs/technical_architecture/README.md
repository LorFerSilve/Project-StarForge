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
- **TA-7 — Gameplay Runtime Entity Architecture**;
- **TA-8 — AI and Navigation Architecture**;
- **TA-9 — Missions, Raids, Dynamic Events, and Strategic State Machines**;
- **TA-10 — Content and Asset Pipeline**;
- **TA-11 — Input, UI, Audio, and Presentation Integration**;
- **TA-12 — Persistence Implementation Architecture**;
- **TA-13 — Concurrency, Performance, Memory, and Streaming Budgets**.

The next dependency is **TA-14 — Testing, Diagnostics, and CI Architecture**.

Gameplay implementation and repository scaffolding have not started. Technical contracts are defined first so implementation does not invent architecture ad hoc.

## Authority Relationship

The technical architecture must implement the Design Complete GDS, may choose software/data/runtime realization, may not silently change gameplay behavior, and must escalate technical infeasibility through explicit design/architecture change control.

## Architectural Direction

Project StarForge is a **purpose-built modular monolith** in C++23 with a custom OpenGL renderer. It deliberately avoids a general-purpose game engine, runtime plugin architecture, mandatory gameplay scripting VM, baseline networking architecture, one universal persistent ECS, and hidden framework ownership of the game loop.

## Initial Technical Baseline

- C++23;
- CMake;
- vcpkg manifest mode;
- OpenGL 4.6 Core Profile;
- GLFW;
- glad2;
- GLM;
- Jolt Physics behind a StarForge adapter;
- Recast/Detour behind a StarForge navigation adapter for grounded navigation;
- project-owned bounded 3D navigation for flying/Zero-G AI;
- miniaudio behind a StarForge audio layer;
- FreeType + HarfBuzz for shipping text;
- fastgltf / glTF 2.0 for canonical 3D exchange import;
- KTX2 + Khronos KTX-Software for cooked textures;
- meshoptimizer for offline mesh optimization/generated LODs;
- glslang for offline GLSL validation;
- Dear ImGui for development tooling only;
- Catch2;
- GitHub + GitHub Actions.

Exact dependency versions and concrete reference hardware remain TA-16 implementation-roadmap data and must be pinned before scaffolding.

## Architecture Baselines

### TA-1 — Runtime Foundation

Purpose-built modular monolith, main-thread simulation/OpenGL authority, fixed **60 Hz** authoritative simulation, bounded workers, deterministic handoff, Stable Simulation Boundaries, True Pause, and save-snapshot capture boundary.

### TA-2 — Identity / State / Transactions / Serialization

Typed persistent IDs, generation-checked runtime handles, domain stores, Activation Leases, revisions, typed commands/results/events, atomic prepared transactions, immutable Read Models, one-owner physical transfers, versioned Save DTOs, staged migration/load, and deterministic PCG32 RNG.

### TA-3 — World / Scene / Streaming

One authoritative player-local SceneInstance, strategic-vs-local coordinates, scene lifecycle/generation, residency separate from gameplay activation, Hard Streaming Hold, double-precision Context Space + floating origin, Horizon active/off-screen handoff, persistent world projection, and staged atomic transitions.

### TA-4 — Rendering

Main-thread OpenGL 4.6 renderer, immutable RenderSnapshot, hybrid deferred/forward pipeline, reversed-Z, linear HDR → SDR, glTF-compatible PBR, lights/shadows, cameras, culling/LOD/instancing/transparency, VFX/particles, GPU lifetime, and presentation-only quality tiers.

### TA-5 — Physics / Character / Spaceflight

One StarForge PhysicsWorld per active scene, Jolt encapsulation, collision/query facts, kinematic CharacterMotor, swept projectiles, Dynamic 6DoF spacecraft, physical docking, Zero-G/EVA/Magnetic Boots, collision consequence routing, deferred safe backend mutation, and origin/load invariants.

### TA-6 — Station Simulation

Canonical Horizon topology with separate Structural/Traversal/Pressure/Power/Thermal/Water/Logistics/ControlData views, deterministic Power, conserved utilities, one-owner logistics, persistent WorkOrders, topology mutation, automation/control, chronological off-screen station simulation, and persistent-state-first physics projection.

### TA-7 — Gameplay Runtime Entities

Generation-checked RuntimeEntityRegistry, typed component pools, persistent actor Activation Lease bridges, player/equipment/inventory runtime references, combat/status/projectile/interactable projections, runtime↔persistent synchronization, deferred destruction, deterministic runtime phases, and headless execution.

### TA-8 — AI and Navigation

Recast/Detour grounded navigation plus project-owned bounded 3D free-flight navigation, traversal profiles/links, nav invalidation, revision-checked asynchronous paths, knowledge-limited perception, enemy tactical AI, robot command/squad AI, crew task behavior, off-screen logical AI, and deterministic scheduling.

### TA-9 — Missions / Raids / Strategic State Machines

Persistent MissionId vs per-attempt MissionInstanceId, exactly-once objective DAGs, deterministic anti-reroll generation, offensive raids as specialized missions, finite reinforcements, persistent Horizon DefenseEvents, Dynamic Events/Recovery Grace, communication-separated knowledge, causal Recovery Transit, and exactly-once finale resolution.

### TA-10 — Content and Asset Pipeline

Canonical source/cooked separation, path-independent ContentId, closed versioned schemas, SHA-256 fingerprints/ContentBuildId, fastgltf import, meshoptimizer, KTX2 texture cooking, glslang shader validation, explicit collision/nav/terrain products, no general gameplay scripting VM, immutable Content Registry/cache, deterministic dependency builds, classified hot reload, and registry-first runtime loading.

### TA-11 — Input / UI / Audio / Presentation

Fixed-tick semantic ActionId sampling/remapping, explicit input/focus contexts, StarForge-owned retained shipping UI, HarfBuzz/FreeType text, knowledge-filtered HUD/management/tutorial flows, miniaudio-backed audio separated from AI hearing, subtitles/captions/typed alarms/accessibility, presentation-only animation/camera/VFX, and stable Read Model/PresentationEvent handoff.

### TA-12 — Persistence Implementation

Exact v1 `SFGSAVE` bytes and SectionKind registry, Stable Save Boundary orchestration, immutable save generations, Manual/Quick/Autosave catalogs, crash-safe pending-file validation + atomic commit, all-or-nothing staged load/session replacement, deterministic source-preserving migrations, explicit ContentId compatibility, fail-soft profile settings persistence, diagnostics/recovery tooling, and resume at `saved_simulation_tick + 1`.

### TA-13 — Concurrency / Performance / Memory / Streaming Budgets

TA-13 establishes:

- reference software target of fixed 60 Hz authoritative simulation plus 60 FPS at 1920×1080 High, with TA-16 pinning the concrete reference PC;
- simulation p95 <= **4.0 ms**, total main-thread work p95 <= **12.0 ms**, GPU p95 <= **13.5 ms**, GPU p99 <= **16.0 ms** in representative reference scenes;
- one shared runtime worker pool `clamp(HardwareConcurrency - 2, 2, 12)`, bounded priority queues/mailboxes, fairness, cancellation, immutable job data, and completion-order non-authority;
- four catch-up ticks/render frame maximum, explicit backlog pressure states, and CriticalPerformanceRecovery without skipping Simulation Time;
- active-local envelopes for runtime entities, physics bodies/contacts/queries, actors, projectiles, CharacterMotors, spacecraft and spatial indexing;
- numeric Horizon graph/logistics/WorkOrder/automation, AI/perception/pathfinding, nav rebuild, objective, reinforcement and strategic scheduler envelopes while preserving chronological semantics;
- process/ContentCache/streaming-staging CPU memory targets, Low/Medium/High/Ultra GPU residency targets, bounded file/decode/upload concurrency, prefetch lead times, cache eviction, and Hard Streaming Hold acceptance targets;
- renderer budgets for visible instances/triangles/draws/lights/shadows/transparency/particles/skinning/render targets plus presentation-only degradation;
- bounded input/UI/text/glyph/marker/notification/presentation/audio/animation/haptic work with accessibility and GameplaySoundEvent invariants preserved;
- bounded save snapshot/file/load/catalog memory/I/O/concurrency and offline content-build worker/RAM/I/O/hot-reload envelopes;
- standardized profiler/telemetry, 30-second warm-up + 120-second repeated benchmark runs, ten representative benchmark scenarios, percentile regression thresholds, bounded trace overhead, hitch captures, leak plateau tests, worker-count/frame-rate equivalence matrices, and a strict degradation order that never reaches authoritative gameplay semantics.

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

### TA-8

- [`52_navigation_representation_and_traversal_profiles.md`](52_navigation_representation_and_traversal_profiles.md)
- [`53_navigation_topology_invalidation_and_links.md`](53_navigation_topology_invalidation_and_links.md)
- [`54_async_pathfinding_and_path_following.md`](54_async_pathfinding_and_path_following.md)
- [`55_perception_memory_and_shared_knowledge.md`](55_perception_memory_and_shared_knowledge.md)
- [`56_decision_layers_enemy_combat_and_tactical_ai.md`](56_decision_layers_enemy_combat_and_tactical_ai.md)
- [`57_robot_command_ai_and_squad_coordination.md`](57_robot_command_ai_and_squad_coordination.md)
- [`58_crew_task_navigation_and_station_behavior.md`](58_crew_task_navigation_and_station_behavior.md)
- [`59_ai_scheduling_and_offscreen_behavior_abstraction.md`](59_ai_scheduling_and_offscreen_behavior_abstraction.md)
- [`60_ai_runtime_phase_integration_debugging_and_validation.md`](60_ai_runtime_phase_integration_debugging_and_validation.md)
- [`TA8_CROSS_VALIDATION.md`](TA8_CROSS_VALIDATION.md)

### TA-9

- [`61_mission_store_instance_and_deployment_state.md`](61_mission_store_instance_and_deployment_state.md)
- [`62_objective_graph_runtime_and_commit_semantics.md`](62_objective_graph_runtime_and_commit_semantics.md)
- [`63_procedural_mission_generation_and_offer_runtime.md`](63_procedural_mission_generation_and_offer_runtime.md)
- [`64_raid_state_machine_and_target_runtime.md`](64_raid_state_machine_and_target_runtime.md)
- [`65_reinforcements_escalation_and_defense_events.md`](65_reinforcements_escalation_and_defense_events.md)
- [`66_dynamic_event_scheduler_and_world_consequence_runtime.md`](66_dynamic_event_scheduler_and_world_consequence_runtime.md)
- [`67_communications_recovery_and_strategic_consequence_routing.md`](67_communications_recovery_and_strategic_consequence_routing.md)
- [`68_finale_postgame_and_irreversible_choice_transactions.md`](68_finale_postgame_and_irreversible_choice_transactions.md)
- [`69_ta9_runtime_phase_integration_debugging_and_validation.md`](69_ta9_runtime_phase_integration_debugging_and_validation.md)
- [`TA9_CROSS_VALIDATION.md`](TA9_CROSS_VALIDATION.md)

### TA-10

- [`70_content_repository_layout_and_source_formats.md`](70_content_repository_layout_and_source_formats.md)
- [`71_content_ids_schemas_registry_and_compatibility.md`](71_content_ids_schemas_registry_and_compatibility.md)
- [`72_gltf_mesh_scene_skeleton_and_animation_import.md`](72_gltf_mesh_scene_skeleton_and_animation_import.md)
- [`73_texture_material_environment_and_shader_pipeline.md`](73_texture_material_environment_and_shader_pipeline.md)
- [`74_collision_navigation_and_terrain_cooking.md`](74_collision_navigation_and_terrain_cooking.md)
- [`75_gameplay_content_definitions_and_procedural_modules.md`](75_gameplay_content_definitions_and_procedural_modules.md)
- [`76_cooked_asset_formats_registry_runtime_cache.md`](76_cooked_asset_formats_registry_runtime_cache.md)
- [`77_content_dependency_graph_incremental_build_and_hot_reload.md`](77_content_dependency_graph_incremental_build_and_hot_reload.md)
- [`78_content_validation_diagnostics_and_build_integration.md`](78_content_validation_diagnostics_and_build_integration.md)
- [`79_content_runtime_loading_failure_and_scene_integration.md`](79_content_runtime_loading_failure_and_scene_integration.md)
- [`TA10_CROSS_VALIDATION.md`](TA10_CROSS_VALIDATION.md)

### TA-11

- [`80_input_device_action_and_context_routing.md`](80_input_device_action_and_context_routing.md)
- [`81_input_bindings_remapping_settings_and_haptics.md`](81_input_bindings_remapping_settings_and_haptics.md)
- [`82_shipping_ui_framework_text_layout_and_focus.md`](82_shipping_ui_framework_text_layout_and_focus.md)
- [`83_hud_markers_notifications_and_interaction_presentation.md`](83_hud_markers_notifications_and_interaction_presentation.md)
- [`84_management_planning_tutorial_and_system_ui.md`](84_management_planning_tutorial_and_system_ui.md)
- [`85_audio_backend_voice_bus_and_resource_lifecycle.md`](85_audio_backend_voice_bus_and_resource_lifecycle.md)
- [`86_audio_semantics_spatial_medium_music_and_dialogue.md`](86_audio_semantics_spatial_medium_music_and_dialogue.md)
- [`87_subtitles_captions_alarms_and_accessibility_presentation.md`](87_subtitles_captions_alarms_and_accessibility_presentation.md)
- [`88_animation_camera_vfx_and_presentation_state_integration.md`](88_animation_camera_vfx_and_presentation_state_integration.md)
- [`89_ta11_runtime_phase_integration_debugging_and_validation.md`](89_ta11_runtime_phase_integration_debugging_and_validation.md)
- [`TA11_CROSS_VALIDATION.md`](TA11_CROSS_VALIDATION.md)

### TA-12

- [`90_persistence_service_and_snapshot_orchestration.md`](90_persistence_service_and_snapshot_orchestration.md)
- [`91_save_container_v1_byte_layout.md`](91_save_container_v1_byte_layout.md)
- [`92_section_registry_domain_codecs_and_payload_rules.md`](92_section_registry_domain_codecs_and_payload_rules.md)
- [`93_save_catalog_slots_manual_quick_and_autosave.md`](93_save_catalog_slots_manual_quick_and_autosave.md)
- [`94_crash_safe_write_commit_and_recovery.md`](94_crash_safe_write_commit_and_recovery.md)
- [`95_load_staging_validation_and_session_activation.md`](95_load_staging_validation_and_session_activation.md)
- [`96_save_migration_and_content_compatibility.md`](96_save_migration_and_content_compatibility.md)
- [`97_profile_application_settings_persistence.md`](97_profile_application_settings_persistence.md)
- [`98_persistence_diagnostics_inspection_and_recovery_tooling.md`](98_persistence_diagnostics_inspection_and_recovery_tooling.md)
- [`99_ta12_runtime_integration_debugging_and_validation.md`](99_ta12_runtime_integration_debugging_and_validation.md)
- [`TA12_CROSS_VALIDATION.md`](TA12_CROSS_VALIDATION.md)

### TA-13

- [`100_performance_target_and_budget_framework.md`](100_performance_target_and_budget_framework.md)
- [`101_worker_pool_job_system_and_backpressure.md`](101_worker_pool_job_system_and_backpressure.md)
- [`102_main_thread_tick_frame_and_backlog_budgets.md`](102_main_thread_tick_frame_and_backlog_budgets.md)
- [`103_runtime_entities_physics_and_spatial_budgets.md`](103_runtime_entities_physics_and_spatial_budgets.md)
- [`104_station_ai_mission_and_strategic_scalability.md`](104_station_ai_mission_and_strategic_scalability.md)
- [`105_streaming_content_cache_and_memory_budgets.md`](105_streaming_content_cache_and_memory_budgets.md)
- [`106_renderer_gpu_and_vfx_performance_budgets.md`](106_renderer_gpu_and_vfx_performance_budgets.md)
- [`107_ui_audio_input_and_presentation_budgets.md`](107_ui_audio_input_and_presentation_budgets.md)
- [`108_persistence_io_and_content_build_concurrency_budgets.md`](108_persistence_io_and_content_build_concurrency_budgets.md)
- [`109_profiling_telemetry_benchmark_scenes_and_degradation.md`](109_profiling_telemetry_benchmark_scenes_and_degradation.md)
- [`TA13_CROSS_VALIDATION.md`](TA13_CROSS_VALIDATION.md)

### Governance

- [`ARCHITECTURE_DECISIONS.md`](ARCHITECTURE_DECISIONS.md)
- [`TA12_ARCHITECTURE_DECISIONS.md`](TA12_ARCHITECTURE_DECISIONS.md)
- [`TA13_ARCHITECTURE_DECISIONS.md`](TA13_ARCHITECTURE_DECISIONS.md)
- [`TA_ROADMAP.md`](TA_ROADMAP.md)

Later TA phases define automated testing/CI, final architecture integration audit, and implementation handoff/locking.

## Implementation Gate

Technical subsystems reach code only after the relevant Design Complete GDS, Architecture Complete technical contract, explicit ownership/lifetime/threading/persistence/content/performance boundaries, dependency/toolchain decisions, validation expectations, and implementation-roadmap approval exist.

`Implementation Locked` remains a later per-contract handoff state. **TA-13 Architecture Complete does not authorize implementation scaffolding yet.**
