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
- **TA-12 — Persistence Implementation Architecture**.

The next dependency is **TA-13 — Concurrency, Performance, Memory, and Streaming Budgets**.

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
- Recast/Detour behind a StarForge navigation adapter for grounded navigation;
- project-owned bounded 3D navigation graph/volume for flying/Zero-G AI;
- miniaudio behind a StarForge audio layer;
- FreeType + HarfBuzz for shipping text shaping/rasterization;
- fastgltf / glTF 2.0 for canonical 3D exchange import;
- KTX2 + Khronos KTX-Software for cooked textures;
- meshoptimizer for offline mesh optimization/generated LODs;
- glslang for offline GLSL validation;
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

Canonical Horizon topology with separate Structural/Traversal/Pressure/Power/Thermal/Water/Logistics/ControlData views; deterministic Power; conserved atmosphere/thermal/water; one-owner logistics; persistent Simulation-Time WorkOrders; topology mutation; automation/control; chronological off-screen station simulation; and persistent-state-first projection into physics.

### TA-7 Gameplay Runtime Entities

One generation-checked runtime registry per active scene, typed component pools, persistent-actor Activation Lease bridges, actor lifecycle, player/inventory/equipment runtime references, combat/status/projectile/interactable projections, persistent↔runtime synchronization, deferred destruction, deterministic 60 Hz runtime phase ordering, and headless execution.

### TA-8 AI and Navigation

Recast/Detour-backed tiled grounded navmeshes, separate bounded 3D free-flight navigation, typed traversal profiles/links, persistent-state-first navigation invalidation, revision-validated asynchronous paths, project-owned following/avoidance, explicit world-truth vs AI-knowledge separation, enemy tactical AI, robot command/squad AI, crew task/emergency navigation, off-screen logical AI, deterministic AI scheduling, and headless diagnostics/tests.

### TA-9 Missions, Raids, Dynamic Events, and Strategic State Machines

Persistent `MissionId` separated from per-attempt `MissionInstanceId`, typed exactly-once objective DAGs, deterministic anti-reroll procedural generation, offensive raids as specialized missions, finite reinforcement state, one persistent Horizon `DefenseEventId` across active/off-screen modes, DynamicEvent scheduling/Recovery Grace, communication-separated event knowledge, causal Recovery Transit, typed strategic consequences, and exactly-once `Stabilize | Sever | Contain` finale commit.

### TA-10 Content and Asset Pipeline

TA-10 establishes:

- DCC working files, canonical repository source content, and generated cooked runtime content as separate layers;
- a canonical `content/` source layout and UTF-8 `*.sfdef.json` closed schema definitions;
- stable path-independent lowercase dotted `ContentId` plus explicit `ContentKind`, schema versioning, typed references, SHA-256 fingerprints, and complete-build `ContentBuildId`;
- glTF 2.0 import through fastgltf into StarForge-owned mesh/skeleton/animation formats, with deterministic transform/vertex/index processing, sockets, skinning, and meshoptimizer offline optimization/LOD;
- KTX2/KTX-Software cooked texture workflow with semantic color-space, mip and desktop BC-format policies;
- material/environment/IBL cooking and bounded ShaderFamily/variant bundles with glslang offline GLSL validation while final OpenGL compilation remains renderer authority;
- explicit separate collision, collision-material, grounded navigation, free-flight navigation, terrain, traversal-link and mission-feasibility cooked products;
- closed authored gameplay/template/procedural-module schemas without general-purpose gameplay scripting or content-side persistent-ID/resource/reward authority;
- versioned loose cooked assets plus one immutable Content Registry, generation-checked nonpersistent ContentHandles, immutable CPU ContentCache, and backend-resource ownership retained by renderer/physics/navigation/audio consumers;
- deterministic dependency graph and SHA-256 fingerprint-driven incremental cooking, atomic output/registry publication, source provenance/license validation, and clean-build reproducibility;
- hot-reload safety classes `PresentationSafe`, `SceneReactivationRequired`, and `SessionRestartRequired` with stale-generation rejection;
- headless content validation/cook/build tooling integrated through CMake and designed for later TA-14 CI enforcement;
- registry-first runtime loading, required asset readiness, Hard Streaming Hold integration, no raw-source fallback in shipping, and no gameplay/procedural changes from I/O/cache/worker timing.

### TA-11 Input, UI, Audio, and Presentation

TA-11 establishes:

- GLFW raw-device ingestion mapped to typed semantic `ActionId` samples at the fixed simulation boundary, with a one-tick edge latch that is explicitly not a gameplay input buffer;
- deterministic InputContextStack priority/consumption, device switching, controller calibration, Hold/Toggle, sustained-interaction support, Auto-Sprint and bounded knowledge-safe Aim Assist;
- fully remappable keyboard/mouse/controller profiles, conflict validation, effective prompt glyph resolution, settings snapshots and optional haptics behind a StarForge `IHapticsBackend`;
- a purpose-built retained shipping UI tree with one focus owner, keyboard/controller/pointer navigation, safe Back/Cancel, preview-versus-commit flows, responsive UI scaling/safe area and TA-4 native-resolution rendering;
- UTF-8 text shaping with HarfBuzz plus FreeType glyph/font rasterization behind StarForge-owned text layout and renderer-owned glyph atlases;
- knowledge-filtered HUD, interaction prompts, marker precision, notifications, station/ship/mission/raid presentation, and commit-synchronized success feedback;
- management/planning/tutorial/system screens built from immutable composite Read Models and typed Commands, with no UI-side business-rule or partial-pause authority;
- miniaudio behind a project audio adapter with isolated real-time callback, generation-checked voices, semantic buses, resident/streamed clip classes, device-failure degradation and explicit audio lifetime;
- separate GameplaySoundEvent and PresentationAudioEvent pipelines so user mix never alters AI hearing;
- atmosphere/vacuum/conduction/radio/Pilot Telemetry audio semantics, knowledge-safe occlusion, dialogue/radio, and non-omniscient adaptive music;
- subtitles/Closed Captions, typed `AlarmPriority::P0–P3`, acknowledgement/escalation/deduplication, High Contrast, Reduced Motion/Effects, Photosensitivity Safe and critical multi-channel redundancy;
- animation/camera/VFX/viewmodel projection where notifies, particles and camera effects never become movement/combat/resource/gameplay authority;
- one explicit runtime handoff from fixed-tick commits to immutable Read Models + ordered exactly-once PresentationEvents, with Simulation Time separated from Presentation Time and stale generation/revision rejection.

### TA-12 Persistence Implementation

TA-12 establishes:

- Stable Save Boundary persistence-service orchestration with immutable snapshots and background encoding/writing;
- an exact v1 `SFGSAVE` binary container with a 128-byte header, 64-byte section entries, deterministic little-endian encoding, CRC32C and explicit ContentBuildId metadata;
- a stable required SectionKind/domain-codec registry including consequential LocalContextContinuation without serializing runtime/backend memory;
- immutable save generations, logical manual/quick/autosave slots, SnapshotSequence ordering, rebuildable catalog caches and 10-generation rolling autosave retention;
- crash-safe pending-file validation and atomic unique rename commit with post-commit cleanup only;
- staged all-or-nothing load/migration/content validation followed by exactly one SessionRoot replacement and fresh runtime/backend reconstruction;
- deterministic source-preserving migrations with Persistent-ID/exactly-once preservation and explicit ContentId compatibility rules;
- separate fail-soft versioned profile persistence for non-gameplay controls/accessibility/HUD/audio/display settings;
- save inspection, corruption/quarantine diagnostics, golden fixtures and no heuristic gameplay-state repair;
- fixed load-resume semantics at `saved_simulation_tick + 1` with zero gameplay progression during file/migration/backend work.

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

### Governance

- [`ARCHITECTURE_DECISIONS.md`](ARCHITECTURE_DECISIONS.md)
- [`TA_ROADMAP.md`](TA_ROADMAP.md)

Later TA phases define performance/concurrency/memory/streaming budgets, testing/CI, final architecture integration audit, and implementation handoff/locking.

## Implementation Gate

Technical subsystems reach code only after the relevant Design Complete GDS, Architecture Complete technical contract, explicit ownership/lifetime/threading/persistence/content boundaries, dependency/toolchain decisions, validation expectations, and implementation-roadmap approval exist.

`Implementation Locked` remains a later per-contract handoff state. **TA-12 Architecture Complete does not authorize implementation scaffolding yet.**
