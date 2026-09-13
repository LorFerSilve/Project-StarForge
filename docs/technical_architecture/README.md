# Project StarForge — Technical Architecture Specification

> **Status:** Active  
> **Authority:** Technical realization of the Design Complete GDS

This directory translates the authoritative Game Design Specification under `docs/game_design/` into an implementable C++/OpenGL software architecture.

## Current Phase

The Game Design Specification is **Design Complete**.

Technical Architecture has completed **TA-0 through TA-15**. The next and final pre-implementation dependency is **TA-16 — Implementation Roadmap and Contract Locking**.

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

Exact dependency versions, compiler/runner versions and concrete reference hardware remain TA-16 implementation-roadmap data and must be pinned before scaffolding.

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

Recast/Detour grounded navigation plus project-owned bounded 3D free-flight navigation, traversal profiles/links, nav invalidation, revision-checked asynchronous paths, knowledge-limited perception, enemy tactical AI, robot command/squad AI, crew task behavior, off-screen logical AI and deterministic scheduling.

### TA-9 — Missions / Raids / Strategic State Machines

Persistent MissionId vs per-attempt MissionInstanceId, exactly-once objective DAGs, deterministic anti-reroll generation, offensive raids as specialized missions, finite reinforcements, persistent Horizon DefenseEvents, Dynamic Events/Recovery Grace, communication-separated knowledge, causal Recovery Transit and exactly-once finale resolution.

### TA-10 — Content and Asset Pipeline

Canonical source/cooked separation, path-independent ContentId, closed versioned schemas, fingerprints/ContentBuildId, fastgltf import, meshoptimizer, KTX2 cooking, glslang validation, separate collision/nav/terrain products, no general gameplay scripting VM, immutable Content Registry/cache, deterministic dependency builds, classified hot reload and registry-first runtime loading.

### TA-11 — Input / UI / Audio / Presentation

Fixed-tick semantic ActionId sampling/remapping, explicit input/focus contexts, StarForge-owned retained shipping UI, HarfBuzz/FreeType text, knowledge-filtered HUD/management/tutorial flows, miniaudio-backed audio separated from AI hearing, subtitles/captions/typed alarms/accessibility, presentation-only animation/camera/VFX and stable Read Model/PresentationEvent handoff.

### TA-12 — Persistence Implementation

Exact v1 `SFGSAVE` bytes/SectionKind registry, Stable Save Boundary orchestration, immutable save generations, Manual/Quick/Autosave catalogs, crash-safe pending-file validation + atomic commit, staged all-or-nothing load/session replacement, deterministic source-preserving migrations, explicit ContentId compatibility, fail-soft profile settings, diagnostics/recovery tooling and resume at `saved_simulation_tick + 1`.

### TA-13 — Concurrency / Performance / Memory / Streaming Budgets

Fixed 60 Hz + 1080p High reference software target, bounded shared worker pool/backpressure, no Simulation-Time skipping, active entity/physics/station/AI/strategic scale envelopes, CPU/GPU/cache/streaming budgets, renderer/UI/audio/persistence/content-build envelopes, percentile regression thresholds, profiler/trace contracts and ten representative benchmark scenarios.

### TA-14 — Testing / Diagnostics / CI

TA-14 establishes:

- CMake/CTest/Catch2 layers for unit/domain, headless integration, deterministic scenarios, backend smoke, golden/compatibility, fault/corruption and performance verification;
- fixed-seed SimulationTick-scripted scenarios with semantic checkpoints, transaction/ownership/Activation Lease tests and worker-count/frame-rate equivalence;
- Jolt/Recast/OpenGL/GLFW/miniaudio/FreeType/HarfBuzz/fastgltf/KTX2/meshoptimizer/glslang adapter smoke coverage without backend identity becoming gameplay authority;
- source/schema/reference/provenance validation, all baseline GLSL variant validation and deterministic clean/incremental/no-op content cook tests;
- byte-exact persistence v1 goldens, historical migration fixtures, corruption/truncation/overflow matrices, crash fault injection, catalog/autosave recovery, RNG continuation and profile persistence tests;
- typed bounded diagnostics, strong development assertions, crash context, deterministic trace correlation, Dear ImGui inspectors, debug commands and leak/stall/memory diagnostics;
- warnings-as-errors, deterministic formatting, pinned static analysis, architecture-boundary linting, blocking ASan/UBSan and scheduled/release leak/TSan certification where supported;
- least-privilege GitHub Actions architecture with stable aggregate checks (`Build & Unit`, `Headless Determinism`, `Content Validation`, `Persistence Compatibility`, `Static Analysis`, `Sanitizers`, `Backend Smoke` where stable, and final `CI Gate`);
- controlled-reference-runner TA-13 percentile/memory/Hard-Hold performance regression gates with reviewed baselines and no rerun-until-green policy;
- bounded privacy-safe artifacts, explicit flaky/quarantine issue-owner-expiry governance, exact-SHA certification evidence and zero-test required-gate protection;
- executable CI workflow YAML deliberately deferred to TA-16 so workflows are created together with real CMake/CTest targets/presets and pinned toolchain/runner versions.

### TA-15 — Architecture Integration Audit

TA-15 establishes:

- one final audit method/severity/evidence contract over TA-0 through TA-14;
- a single-owner authority audit covering persistent identity, runtime handles, transactions, backend encapsulation and mutation rights;
- fixed-tick/Simulation-Time/threading/lifecycle verification including worker non-authority, deferred destruction, save/load ordering and backlog semantics;
- active local scene, streaming, physics, station, navigation, AI and active↔off-screen equivalence validation;
- mission/raid/event/content/persistence/input/UI/audio/accessibility exactly-once and knowledge-boundary validation;
- failure/recovery/performance/testability validation proving technical pressure/failure never fabricates or simplifies authoritative gameplay;
- complete GDS-0 through GDS-14 → TA realization traceability;
- a realizable build/module dependency DAG with no required compile-time or semantic ownership cycle;
- an explicit implementation-readiness/risk register separating 0 blockers/0 required corrections from normal engineering risks and TA-16 lock items;
- a formal **260/260 PASS** integration matrix and final verdict authorizing TA-16, not direct coding.

TA-15 deliberately introduces no new Architecture Decision number: the audit certifies already accepted contracts instead of redesigning them during certification.

## Architecture Documents

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

- [`ARCHITECTURE_DECISIONS.md`](ARCHITECTURE_DECISIONS.md)
- [`TA12_ARCHITECTURE_DECISIONS.md`](TA12_ARCHITECTURE_DECISIONS.md)
- [`TA13_ARCHITECTURE_DECISIONS.md`](TA13_ARCHITECTURE_DECISIONS.md)
- [`TA14_ARCHITECTURE_DECISIONS.md`](TA14_ARCHITECTURE_DECISIONS.md)
- [`TA_ROADMAP.md`](TA_ROADMAP.md)

TA-16 now owns the dependency-ordered implementation roadmap, exact toolchain/dependency pins, concrete target/preset/runner selection, CI workflow materialization, reference hardware, vertical slice, milestone exits, decision-log consolidation and per-contract implementation locking.

## Implementation Gate

Technical subsystems reach code only after the relevant Design Complete GDS, Architecture Complete technical contract, explicit ownership/lifetime/threading/persistence/content/performance/testing boundaries, dependency/toolchain decisions, validation expectations, and implementation-roadmap approval exist.

`Implementation Locked` remains a TA-16 per-contract handoff state. **TA-15 Architecture Complete authorizes TA-16 only; it does not authorize C++/OpenGL scaffolding by itself.**