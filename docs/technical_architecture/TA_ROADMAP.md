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
- deterministic staged procedural mission generation using persistent cursors, scoped PCG32 streams, bounded validation and stable major content;
- offensive Raids as specialized MissionInstances against persistent targets;
- finite reinforcement responses and persistent Horizon DefenseEvents;
- DynamicEvent scheduling/concurrency/Recovery Grace with plausible world sources;
- communication-separated event knowledge and causal Recovery Transit;
- exactly-once MS-F02 Stabilize/Sever/Contain FinalResolution transaction;
- deterministic local/strategic phase integration and headless validation.

Artifacts: `61_mission_store_instance_and_deployment_state.md` through `69_ta9_runtime_phase_integration_debugging_and_validation.md`, plus `TA9_CROSS_VALIDATION.md`.

## TA-10 — Content and Asset Pipeline

**Status:** Architecture Complete

Defines:

- explicit separation of DCC working sources, canonical repository source content, and generated cooked runtime content;
- canonical source layout under `content/`, UTF-8 `*.sfdef.json` structured definitions, glTF 2.0 3D exchange, PNG/EXR texture sources, GLSL 4.60 shader sources, WAV/FLAC audio sources, and bounded terrain manifests;
- path-independent lowercase dotted `ContentId` identity, explicit `ContentKind`, closed versioned schemas, typed content references, and deterministic generated Content Registry;
- SHA-256 per-entry `ContentFingerprint` plus complete-build `ContentBuildId`, with explicit rename/removal compatibility rather than silent aliases;
- fastgltf-based glTF import into StarForge-owned mesh/skeleton/animation products, deterministic vertex/index packing, sockets, skinning, and offline meshoptimizer LOD/mesh optimization;
- KTX2 as the canonical cooked texture container through pinned KTX-Software, semantic color-space/compression/mip rules, material/environment/IBL cooking, and bounded shader families;
- pinned glslang offline GLSL validation while final OpenGL compile/link remains TA-4 renderer authority;
- separately cooked collision, CollisionMaterial, Recast/Detour grounded navigation, project-owned 3D free-flight navigation, terrain, traversal-link, and feasibility metadata products;
- closed gameplay-definition schemas and authored procedural modules without a general-purpose scripting VM or content-side persistent-ID/resource/reward authority;
- versioned loose cooked asset files plus one immutable runtime Content Registry, nonpersistent generation-checked ContentHandles, immutable CPU ContentCache, and consumer-owned GPU/Jolt/navigation resources;
- explicit dependency graph, fingerprint-driven incremental rebuilding, atomic cooked/registry publication, deterministic build scheduling, and tool/schema/profile invalidation;
- development hot reload classified as `PresentationSafe`, `SceneReactivationRequired`, or `SessionRestartRequired`, with stale-generation rejection and previous-valid-state preservation on failure;
- headless `starforge-content` validation/cook/build architecture, stable diagnostics, CMake integration, provenance/license checks, deterministic fixtures and future CI contracts;
- registry-first runtime loading, required-content readiness, TA-3 Hard Streaming Hold integration, no raw-source fallback in shipping, and the invariant that I/O/cache/worker timing cannot alter gameplay or reroll committed procedural content.

Artifacts:

- `70_content_repository_layout_and_source_formats.md`;
- `71_content_ids_schemas_registry_and_compatibility.md`;
- `72_gltf_mesh_scene_skeleton_and_animation_import.md`;
- `73_texture_material_environment_and_shader_pipeline.md`;
- `74_collision_navigation_and_terrain_cooking.md`;
- `75_gameplay_content_definitions_and_procedural_modules.md`;
- `76_cooked_asset_formats_registry_runtime_cache.md`;
- `77_content_dependency_graph_incremental_build_and_hot_reload.md`;
- `78_content_validation_diagnostics_and_build_integration.md`;
- `79_content_runtime_loading_failure_and_scene_integration.md`;
- `TA10_CROSS_VALIDATION.md`.

## TA-11 — Input, UI, Audio, and Presentation Integration

**Status:** Next

Must define raw input → semantic actions, input contexts/focus/routing, keyboard/mouse/controller remapping, controller deadzones/aim-assist boundary, shipping UI/HUD architecture and read models, menus/settings, loading/failure/mission/strategic presentation, subtitles/captions, alarms/notifications, miniaudio adapter/resource lifetime/buses/spatialization/streaming, animation/presentation integration, and accessibility behavior without crossing gameplay knowledge/authority boundaries.

## TA-12 — Persistence Implementation Architecture

**Status:** Planned

Must define exact TA-2 save-container byte layout, section directory/manifest, domain serialization registry, CRC32C, write-new-then-commit, manual/quick/autosave layout, migrations, crash recovery, diagnostics, content-compatibility integration, and deterministic load validation.

## TA-13 — Concurrency, Performance, Memory, and Streaming Budgets

**Status:** Planned

Must define worker-pool model, job priorities, snapshot/versioning, streaming/CPU/GPU/memory budgets, simulation backlog policy, profiling, performance scenes, station scalability, and numeric TA-3 through TA-10 budgets including entities, physics, station solvers, navigation/path/perception/AI, objective/procedural/event scheduling, content I/O/decode/upload, ContentCache residency, texture/mesh/nav/terrain memory, content-build concurrency, and strategic backlog limits.

## TA-14 — Testing, Diagnostics, and CI Architecture

**Status:** Planned

Must define CMake/Catch2 layers, headless simulation tests, transaction/persistence tests, deterministic-seed regressions, clean/incremental content validation/cook determinism, schema/reference/provenance checks, required GLSL validation, renderer/physics/station/runtime-entity/AI/navigation/mission/raid/event/content smoke and deterministic tests, formatting/tidy/warnings, sanitizers, CI gates, and debug-tool requirements.

## TA-15 — Architecture Integration Audit

**Status:** Planned

Cross-validates TA-0 through TA-14 against all Design Complete GDS contracts, ownership/lifetime, threading, persistence, content compatibility, world/scene transitions, performance assumptions, dependency cycles, failure recovery, and testability.

No implementation phase is authorized to invent unresolved architecture after this audit.

## TA-16 — Implementation Roadmap and Contract Locking

**Status:** Planned

Produces dependency-ordered implementation phases, vertical-slice definition, scaffolding plan, exact initial CMake/vcpkg targets and pinned dependency versions, first test/content gates, per-contract `Implementation Locked` handoff, merge/branch strategy, and milestone criteria.

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
→ TA-10 Architecture Complete  
→ **TA-11 next**  
→ TA-12 ... TA-15  
→ TA-16 implementation roadmap/locking  
→ C++/OpenGL scaffolding  
→ gameplay implementation.
