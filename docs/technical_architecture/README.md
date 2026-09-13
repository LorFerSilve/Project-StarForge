# Project StarForge — Technical Architecture Specification

> **Status:** Technical Architecture Complete / Implementation Locked  
> **Implementation baseline:** `TA16-V1`  
> **Authority:** Technical realization of the Design Complete GDS and implementation handoff

This directory translates the authoritative Game Design Specification under `docs/game_design/` into the locked C++/OpenGL software architecture used by implementation.

## Current Phase

The Game Design Specification is **Design Complete**.

Technical Architecture **TA-0 through TA-16 is complete**. TA-15 finished the integration audit at **260/260 PASS** with 0 blockers. TA-16 then fixed the concrete implementation baseline, target DAG, repository/build/test/CI contracts, reference runner, implementation roadmap, vertical slice, milestone/branch policy, and change-control boundary.

The next project dependency is no longer another TA phase:

> **IMP-1 — Core, Identity, Deterministic Simulation, and Transactions**

TA-16's checked-in CMake/headless/unit-test/CI bootstrap is infrastructure proof only; it does not claim gameplay implementation.

## Authority Relationship

The technical architecture implements the Design Complete GDS, may choose software/data/runtime realization where explicitly free, may not silently change gameplay behavior, and must escalate infeasibility through formal change control.

At `TA16-V1`, TA-0 through TA-16 are **Implementation Locked**. Code is subordinate to those contracts.

## Architectural Direction

Project StarForge is a **purpose-built modular monolith** in C++23 with a custom OpenGL renderer. It deliberately avoids a general-purpose game engine, runtime plugin architecture, mandatory gameplay scripting VM, baseline networking architecture, one universal persistent ECS, and hidden framework ownership of the game loop.

## TA16-V1 Toolchain / Dependency Lock

- Visual Studio 2022 / Build Tools 17.14.39 (installation build 17.14.37614.0), MSVC v143 14.44 x64;
- primary hosted bootstrap certification image `windows-2022`;
- CMake 4.3.3;
- LLVM/Clang 23.1.1 independent compiler/tooling line;
- vcpkg baseline `a1cae005c39be7b18ba319fced856b68d7276271`;
- OpenGL 4.6 Core;
- GLFW 3.5.1;
- glad2 v2.0.8 generated source, explicitly not the vcpkg glad1 port;
- GLM 1.0.3;
- Jolt Physics 5.6.0#1;
- Recast/Detour 1.6.0#1;
- miniaudio 0.11.25;
- FreeType 2.14.3;
- HarfBuzz 14.4.0;
- fastgltf 0.9.0;
- KTX-Software 4.4.2;
- meshoptimizer 1.2;
- glslang 16.4.0;
- Dear ImGui 1.92.9 development-only;
- Catch2 3.16.0;
- simdjson 4.6.8 where selected by content tools.

Exact ownership/features/license-notice policy: `131_toolchain_dependency_and_license_lock.md`.

## Architecture Baselines

### TA-1 — Runtime Foundation

Purpose-built modular monolith, main-thread simulation/OpenGL authority, fixed **60 Hz** authoritative simulation, deterministic bounded worker handoff, Stable Simulation Boundaries, True Pause and save-snapshot capture boundary.

### TA-2 — Identity / State / Transactions / Serialization

Typed persistent IDs, generation-checked runtime handles, domain stores, Activation Leases, typed commands/results/events, prepared atomic transactions, immutable Read Models, one-owner physical transfers, Save DTO/container contracts, staged migration/load and deterministic PCG32 RNG.

### TA-3 — World / Scene / Streaming

One authoritative player-local SceneInstance, scene lifecycle/generation, residency separate from gameplay activation, Hard Streaming Hold, double-precision Context Space + floating origin, Horizon active/off-screen handoff, persistent world projection and staged atomic transitions.

### TA-4 — Rendering

Main-thread OpenGL 4.6 renderer, immutable RenderSnapshot, hybrid deferred/forward pipeline, reversed-Z, linear HDR → SDR, glTF-compatible PBR, lighting/shadows, cameras, culling/LOD/instancing/transparency, VFX/particles, GPU lifetime and presentation-only quality tiers.

### TA-5 — Physics / Character / Spaceflight

One StarForge PhysicsWorld per active scene, Jolt encapsulation, collision/query facts, kinematic CharacterMotor, swept projectiles, Dynamic 6DoF spacecraft, physical docking, Zero-G/EVA/Magnetic Boots, collision consequence routing, deferred backend mutation and origin/load invariants.

### TA-6 — Station Simulation

Canonical Horizon topology with separate Structural/Traversal/Pressure/Power/Thermal/Water/Logistics/ControlData views, deterministic Power, conserved utilities, one-owner logistics, persistent WorkOrders, topology mutation, automation/control, chronological off-screen simulation and persistent-state-first projection.

### TA-7 — Gameplay Runtime Entities

Generation-checked RuntimeEntityRegistry, typed component pools, persistent actor Activation Lease bridges, player/equipment/inventory runtime references, combat/status/projectile/interactable projections, runtime↔persistent synchronization, deferred destruction, deterministic runtime phases and headless execution.

### TA-8 — AI and Navigation

Recast/Detour grounded navigation plus project-owned bounded 3D free-flight navigation, traversal profiles/links, nav invalidation, revision-checked async paths, knowledge-limited perception, enemy tactical AI, robot command/squad AI, crew tasks, off-screen logical AI and deterministic scheduling.

### TA-9 — Missions / Raids / Strategic State Machines

Persistent MissionId vs per-attempt MissionInstanceId, exactly-once objective DAGs, deterministic anti-reroll generation, offensive raids, finite reinforcements, persistent Horizon DefenseEvents, Dynamic Events/Recovery Grace, communication-separated knowledge, causal Recovery Transit and exactly-once finale resolution.

### TA-10 — Content and Asset Pipeline

Canonical source/cooked separation, path-independent ContentId, closed versioned schemas, fingerprints/ContentBuildId, fastgltf import, meshoptimizer, KTX2, glslang, separately cooked collision/nav/terrain, immutable Content Registry/cache, deterministic builds, classified hot reload, provenance/license checks and registry-first runtime loading.

### TA-11 — Input / UI / Audio / Presentation

Fixed-tick semantic ActionId sampling/remapping, explicit focus contexts, StarForge-owned retained shipping UI, HarfBuzz/FreeType text, knowledge-filtered HUD/management/tutorials, miniaudio presentation audio separated from AI hearing, subtitles/captions/alarms/accessibility and presentation-only animation/camera/VFX.

### TA-12 — Persistence Implementation

Exact v1 `SFGSAVE` bytes/SectionKind registry, Stable Save Boundary orchestration, immutable save generations, crash-safe commit, staged all-or-nothing load, deterministic source-preserving migrations, ContentId compatibility, fail-soft profile settings, recovery tooling and resume at `saved_simulation_tick + 1`.

### TA-13 — Concurrency / Performance / Memory / Streaming

Bounded worker pool/backpressure, no Simulation-Time skipping, entity/physics/station/AI/strategic scale envelopes, CPU/GPU/cache/streaming limits, renderer/UI/audio/persistence/content-build budgets, percentile thresholds and representative benchmark scenarios.

### TA-14 — Testing / Diagnostics / CI

CMake/CTest/Catch2 layers, deterministic headless scenarios, backend smoke, content determinism, persistence goldens/migration/corruption/fault injection, diagnostics/assertions/debug tools, warnings/static analysis/sanitizers, least-privilege Actions, controlled performance runner, exact-SHA evidence and zero-test protection.

### TA-15 — Architecture Integration Audit

Final cross-system audit covering authority, dependencies, timing, lifecycle, active/off-screen state, content, persistence, presentation, failure/performance and testability, with complete GDS traceability and a formal **260/260 PASS**.

### TA-16 — Implementation Roadmap and Contract Locking

TA-16 fixes:

- `TA16-V1` reproducible environment;
- concrete CMake target/source DAG;
- repository-owned presets/CTest labels;
- nonzero test discovery;
- real bootstrap targets/tests;
- executable CI with immutable Action pins;
- stable check/ruleset target state;
- `SF-PERF-WIN-01` reference hardware;
- IMP-0 through IMP-16 dependency roadmap;
- Horizon Test Cell V0 acceptance;
- C0–C3 change control;
- short-lived branch/squash-merge/evidence milestones;
- architecture decision continuity AD-001 through AD-141.

## TA-16 Artifacts

| Artifact | Purpose |
|---|---|
| `130_ta16_scope_lock_authority_and_exit_criteria.md` | final scope/lock/exit authority |
| `131_toolchain_dependency_and_license_lock.md` | exact toolchain/dependency/license evidence |
| `132_cmake_target_dependency_and_source_layout_lock.md` | target DAG and source layout |
| `133_build_presets_configurations_and_ctest_lock.md` | presets/configurations/test labels |
| `134_ci_workflows_actions_and_required_check_lock.md` | CI/action/check/ruleset contract |
| `135_reference_hardware_and_performance_runner_lock.md` | controlled performance reference |
| `136_implementation_roadmap_and_vertical_slice.md` | IMP roadmap + Horizon Test Cell V0 |
| `137_implementation_lock_matrix_and_change_control.md` | per-contract locks / C0–C3 |
| `138_branch_merge_milestone_and_release_strategy.md` | Git integration + M0–M9 |
| `139_ta16_final_implementation_handoff.md` | final transition to implementation |
| `TA16_ARCHITECTURE_DECISIONS.md` | AD-133 through AD-141 |
| `ARCHITECTURE_DECISION_REGISTRY.md` | contiguous AD-001 through AD-141 registry |
| `TA16_CROSS_VALIDATION.md` | final implementation-readiness audit |

## Earlier Authoritative Artifact Ranges

| Phase | Authoritative artifacts |
|---|---|
| TA-0 / TA-1 | `00_architecture_authority.md` through `04_runtime_and_simulation_model.md`, `TA1_CROSS_VALIDATION.md` |
| TA-2 | `05_identity_and_reference_model.md` through `11_rng_migration_and_integrity_contracts.md`, `TA2_CROSS_VALIDATION.md` |
| TA-3 | `12_active_local_context_and_scene_model.md` through `18_scene_composition_profiles.md`, `TA3_CROSS_VALIDATION.md` |
| TA-4 | `19_renderer_ownership_and_frame_pipeline.md` through `26_graphics_settings_resize_and_failure_recovery.md`, `TA4_CROSS_VALIDATION.md` |
| TA-5 | `27_physics_adapter_and_world_lifecycle.md` through `34_physics_tick_origin_shift_and_snapshot_sync.md`, `TA5_CROSS_VALIDATION.md` |
| TA-6 | `35_station_graph_foundation_and_topology.md` through `43_station_runtime_projection_and_physics_handoff.md`, `TA6_CROSS_VALIDATION.md` |
| TA-7 | `44_runtime_entity_registry_and_handles.md` through `51_runtime_update_phases_and_system_boundaries.md`, `TA7_CROSS_VALIDATION.md` |
| TA-8 | `52_navigation_representation_and_traversal_profiles.md` through `60_ai_runtime_phase_integration_debugging_and_validation.md`, `TA8_CROSS_VALIDATION.md` |
| TA-9 | `61_mission_store_instance_and_deployment_state.md` through `69_ta9_runtime_phase_integration_debugging_and_validation.md`, `TA9_CROSS_VALIDATION.md` |
| TA-10 | `70_content_repository_layout_and_source_formats.md` through `79_content_runtime_loading_failure_and_scene_integration.md`, `TA10_CROSS_VALIDATION.md` |
| TA-11 | `80_input_device_action_and_context_routing.md` through `89_ta11_runtime_phase_integration_debugging_and_validation.md`, `TA11_CROSS_VALIDATION.md` |
| TA-12 | `90_persistence_service_and_snapshot_orchestration.md` through `99_ta12_runtime_integration_debugging_and_validation.md`, `TA12_CROSS_VALIDATION.md` |
| TA-13 | `100_performance_target_and_budget_framework.md` through `109_profiling_telemetry_benchmark_scenes_and_degradation.md`, `TA13_CROSS_VALIDATION.md` |
| TA-14 | `110_test_architecture_layers_targets_and_conventions.md` through `119_ci_evidence_artifacts_quarantine_and_release_certification.md`, `TA14_CROSS_VALIDATION.md` |
| TA-15 | `120_ta15_audit_scope_method_and_evidence.md` through `129_ta15_final_integration_verdict.md`, `TA15_CROSS_VALIDATION.md` |

## Governance

- [`ARCHITECTURE_DECISIONS.md`](ARCHITECTURE_DECISIONS.md) — AD-001 through AD-100 detailed source;
- [`TA12_ARCHITECTURE_DECISIONS.md`](TA12_ARCHITECTURE_DECISIONS.md) — AD-101 through AD-111;
- [`TA13_ARCHITECTURE_DECISIONS.md`](TA13_ARCHITECTURE_DECISIONS.md) — AD-112 through AD-121;
- [`TA14_ARCHITECTURE_DECISIONS.md`](TA14_ARCHITECTURE_DECISIONS.md) — AD-122 through AD-132;
- [`TA16_ARCHITECTURE_DECISIONS.md`](TA16_ARCHITECTURE_DECISIONS.md) — AD-133 through AD-141;
- [`ARCHITECTURE_DECISION_REGISTRY.md`](ARCHITECTURE_DECISION_REGISTRY.md) — canonical consolidated numbering/source registry;
- [`TA_ROADMAP.md`](TA_ROADMAP.md).

## Implementation Gate

The Technical Architecture gate is now open. Implementation proceeds only in the dependency order and milestone rules defined by TA-16.

Immediate next dependency:

```text
IMP-1 — Core, Identity, Deterministic Simulation, and Transactions
```

No implementation phase may use difficulty, performance pressure, backend convenience, presentation state, worker completion order, or existing code as permission to contradict Design Complete / Implementation Locked authority.
