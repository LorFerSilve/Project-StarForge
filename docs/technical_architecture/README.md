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
- **TA-5 — Physics, Collision, Character, and Spaceflight Integration**.

The next dependency is **TA-6 — Station Simulation and Graph Architecture**.

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

The architecture deliberately avoids:

- a general-purpose game engine project;
- runtime plugin architecture;
- mandatory scripting-language VM;
- networking/server architecture in the baseline;
- one universal ECS owning every strategic and persistent state;
- hidden framework ownership of the game loop or renderer.

Commodity libraries are allowed when they remove non-differentiating infrastructure while leaving StarForge gameplay/rendering architecture under project control.

## Initial Technical Baseline

- Language: **C++23**;
- build system: **CMake**;
- dependency management: **vcpkg manifest mode** with pinned baseline/versions;
- graphics API: **OpenGL 4.6 Core Profile**;
- window/context/input platform layer: **GLFW**;
- OpenGL function loader: **glad2**;
- math: **GLM**;
- physics/collision: **Jolt Physics** behind a StarForge-owned adapter;
- audio device/mixing backend: **miniaudio** behind a StarForge-owned audio layer;
- glTF 2.0 asset import: **fastgltf**;
- developer/debug UI: **Dear ImGui**, development tooling only;
- automated C++ tests: **Catch2**;
- version control/build automation: GitHub + GitHub Actions.

Exact dependency versions are implementation-roadmap/toolchain data and must be pinned before scaffolding.

## TA-2 Data/State Baseline

TA-2 establishes:

- strongly typed non-zero 64-bit persistent IDs;
- nonserialized index+generation runtime handles;
- stable UTF-8 Content IDs;
- persistent domain stores with deterministic enumeration;
- explicit Activation Leases for active high-frequency state;
- StateRevision/ActivationEpoch stale-state protection;
- typed Command / Result / committed Event contracts;
- prepared single-thread atomic cross-domain transactions;
- immutable consumer Read Models;
- one-owner physical transfer semantics;
- integer Credit ledger transactions;
- versioned per-domain Save DTOs;
- StarForge-owned little-endian chunked binary save container;
- CRC32C section integrity and initial `None` compression codec;
- staged all-or-nothing loading/migration;
- project-owned PCG32 deterministic RNG with scoped SplitMix64-based derivation.

## TA-3 World/Scene Baseline

TA-3 establishes:

- one authoritative player-local `SceneInstance` at a time;
- strategic galaxy identity/topology separated from local 3D coordinates;
- `ActiveLocalContextKind` separated from physical `SceneProfileKind`;
- explicit Scene lifecycle and SceneGeneration stale-result protection;
- stream-cell content residency separated from simulation activation;
- deterministic required activation plus non-authoritative predictive prefetch;
- Hard Streaming Hold that freezes Simulation Time when required content is unavailable;
- persistent double-precision Context Space positions with origin-relative runtime coordinates;
- floating-origin `RuntimeOrigin64` + `OriginEpoch` rebasing;
- loose hashed gameplay spatial-index architecture distinct from streaming/physics/render partitions;
- Horizon active/off-screen state handoff without a second full scene;
- off-screen Horizon Defense handoff preserving the same persistent event/actors/resources;
- staged destination loading and atomic context-switch transitions;
- persistent World/Mission state projected over immutable Content definitions;
- explicit Interior, Surface, EVA, LocalSpaceflight, Horizon, and Mixed scene-composition profiles.

## TA-4 Rendering Baseline

TA-4 establishes:

- one main-thread OpenGL 4.6 Core rendering context;
- a thin StarForge-owned `RenderDevice` and move-safe/deferred-lifetime GPU wrappers;
- immutable `RenderSnapshot` input with fixed-simulation/variable-render interpolation;
- origin/scene-generation discontinuity handling;
- linear HDR rendering with SDR sRGB output and native-resolution shipping UI composition;
- reversed-Z zero-to-one floating-point depth;
- a hybrid deferred renderer: G-buffer/deferred opaque lighting plus forward transparent/special/VFX passes;
- four-cascade primary directional shadows and bounded prioritized local-light shadows;
- compute-assisted tiled deferred local-light culling;
- glTF-compatible metallic-roughness PBR with GGX/Smith/Schlick direct lighting and IBL/probes;
- bounded shader families and explicit shader variant keys;
- explicit FirstPerson, SpacecraftChase, Cockpit, Management/Strategic, and Debug view contracts;
- render-only frustum/LOD/optional conservative occlusion culling and hardware instancing;
- stable transparent ordering without mandatory OIT;
- GPU-compute presentation particles that never become gameplay authority;
- VFX priority/accessibility filtering with critical-representation floor;
- main-thread GPU upload/deletion queue, generation-checked RenderResourceHandles, caches, and fence-safe retirement;
- Low/Medium/High/Ultra/Custom presentation presets that cannot alter gameplay semantics;
- Off/FXAA initial anti-aliasing and no baseline motion-blur implementation;
- explicit resize/minimize/context-failure policies.

## TA-5 Physics Baseline

TA-5 establishes:

- one StarForge-owned `PhysicsWorld` per active SceneInstance with Jolt isolated behind typed adapters;
- generation-checked physics runtime handles and no serialized/backend gameplay identity;
- one canonical semantic collision layer/filter/query architecture;
- separate locomotion colliders and Combat hit-zone query shapes;
- a kinematic `CharacterMotor` for humanoid grounded movement, crouch, slopes, steps, jumping, platforms, mantling, ladders, and technical safe-position recovery;
- project-owned physical projectile/sweep architecture with bounded owner-ignore, deterministic melee sweeps, and explosion candidate/occlusion queries;
- Dynamic spacecraft rigid bodies with project-computed mass/inertia and bounded 6DoF forces/torques;
- Flight Assist and safe-speed envelope implemented through avionics/thruster authority rather than fake vacuum drag or velocity teleport/clamping;
- real Hard Dock capture constraints plus explicit release/separation behavior;
- Zero-G/EVA momentum, suit-thruster stabilization, and Magnetic Boot support frames;
- normalized Impact/Contact Facts routed to gameplay Damage/Health/Structure owners rather than physics-owned damage;
- one fixed 60 Hz physics phase schedule integrated with TA-2 transactions, TA-3 origin rebasing, TA-4 snapshots, streaming holds, and save boundaries;
- no synthetic impacts/triggers caused by load reconstruction or floating-origin rebases.

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

### Governance

- [`ARCHITECTURE_DECISIONS.md`](ARCHITECTURE_DECISIONS.md)
- [`TA_ROADMAP.md`](TA_ROADMAP.md)

Later TA phases define station graphs, runtime entities, AI/navigation, missions/raids, asset/content pipeline, audio/input/UI boundaries, persistence storage details, concurrency, observability/testing, and implementation handoff.

## Implementation Gate

A technical subsystem is ready for code only when:

1. its relevant GDS contracts are Design Complete;
2. its technical architecture contract is sufficiently complete;
3. ownership/lifetime/threading boundaries are explicit;
4. data and persistence boundaries are explicit where applicable;
5. dependencies are selected and pinned where required;
6. tests/validation expectations are defined;
7. the implementation roadmap places it in an approved phase.

`Implementation Locked` remains a later per-contract handoff state. TA-5 Architecture Complete does **not** authorize C++/OpenGL scaffolding yet.
