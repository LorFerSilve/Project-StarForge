# Technical Architecture Roadmap

> **Status:** Technical Architecture Complete / Implementation Handoff Active  
> **Authority:** Dependency-driven technical architecture sequencing

This roadmap defines the completed Technical Architecture and its handoff into dependency-ordered implementation.

## TA-0 — Architecture Governance and Constraints

**Status:** Complete

Defines architecture authority, precedence, status model, no-general-engine principle, GDS traceability, third-party dependency policy, and implementation gate.

## TA-1 — System Context, Toolchain, and Runtime Foundation

**Status:** Architecture Complete / Implementation Locked

Defines the modular monolith, C++23/CMake/vcpkg/OpenGL baseline, dependency boundaries, 60 Hz authoritative simulation, worker authority, Stable Simulation Boundary, and save snapshot capture boundary.

## TA-2 — Identity, Domain State, Transactions, and Serialization Contracts

**Status:** Architecture Complete / Implementation Locked

Defines typed persistent/runtime identities, domain stores, Activation Leases, revisions/epochs, typed commands/results/events, transactions, read models, physical ownership, Save DTO/container, migration/integrity, and deterministic PCG32 RNG.

## TA-3 — World, Scene, Zone, and Streaming Architecture

**Status:** Architecture Complete / Implementation Locked

Defines one authoritative local SceneInstance, scene lifecycle/generation, streaming residency vs activation, Hard Streaming Hold, double-precision Context Space/floating origin, Horizon handoff, atomic transitions, persistent world projection, and scene profiles.

## TA-4 — Rendering Architecture

**Status:** Architecture Complete / Implementation Locked

Defines main-thread OpenGL ownership, RenderSnapshot interpolation, hybrid deferred/forward rendering, reversed-Z, HDR/SDR, passes, shadows/lights, PBR, cameras, culling/LOD/instancing/transparency, VFX, GPU lifetime, and graphics tiers.

## TA-5 — Physics, Collision, Character, and Spaceflight Integration

**Status:** Architecture Complete / Implementation Locked

Defines Jolt adapter/world ownership, collision/query contracts, CharacterMotor, projectiles, Dynamic 6DoF spacecraft, docking, Zero-G/EVA, impact/contact facts, fixed physics ordering, deferred mutation, and floating-origin/load behavior.

Artifacts: `27_physics_adapter_and_world_lifecycle.md` through `34_physics_tick_origin_shift_and_snapshot_sync.md`, plus `TA5_CROSS_VALIDATION.md`.

## TA-6 — Station Simulation and Graph Architecture

**Status:** Architecture Complete / Implementation Locked

Defines Horizon topology, typed utility graph views, deterministic Power, conserved atmosphere/thermal/water, one-owner logistics, WorkOrders, topology mutation, automation/control, chronological off-screen simulation, and persistent-state-first projection.

Artifacts: `35_station_graph_foundation_and_topology.md` through `43_station_runtime_projection_and_physics_handoff.md`, plus `TA6_CROSS_VALIDATION.md`.

## TA-7 — Gameplay Runtime Entity Architecture

**Status:** Architecture Complete / Implementation Locked

Defines one runtime registry per active scene, generation-checked handles, typed component pools, persistent-actor Activation Lease bridges, actor lifecycle, player/inventory/equipment runtime, combat/status/projectile/interactable projections, persistent/runtime synchronization, deferred destruction, deterministic runtime phases, and headless execution.

Artifacts: `44_runtime_entity_registry_and_handles.md` through `51_runtime_update_phases_and_system_boundaries.md`, plus `TA7_CROSS_VALIDATION.md`.

## TA-8 — AI and Navigation Architecture

**Status:** Architecture Complete / Implementation Locked

Defines Recast/Detour-backed grounded navigation, separate bounded 3D free-flight navigation, typed traversal profiles/links, persistent-state-first navigation invalidation, revision-validated asynchronous pathfinding, project-owned path following/avoidance, knowledge-limited perception/memory, enemy tactical AI, robot command/squad AI, crew task/emergency navigation, deterministic AI scheduling, active/off-screen logical AI equivalence, and headless diagnostics/validation.

Artifacts: `52_navigation_representation_and_traversal_profiles.md` through `60_ai_runtime_phase_integration_debugging_and_validation.md`, plus `TA8_CROSS_VALIDATION.md`.

## TA-9 — Missions, Raids, Dynamic Events, and Strategic State Machines

**Status:** Architecture Complete / Implementation Locked

Defines persistent MissionId versus per-deployment MissionInstanceId, transactional single-external-deployment authority, exactly-once objective graphs, deterministic anti-reroll procedural generation, offensive raids, finite reinforcements, persistent Horizon DefenseEvents, Dynamic Events/Recovery Grace, communication-separated knowledge, causal Recovery Transit, exactly-once finale resolution, and deterministic local/strategic phase integration.

Artifacts: `61_mission_store_instance_and_deployment_state.md` through `69_ta9_runtime_phase_integration_debugging_and_validation.md`, plus `TA9_CROSS_VALIDATION.md`.

## TA-10 — Content and Asset Pipeline

**Status:** Architecture Complete / Implementation Locked

Defines canonical source/cooked separation, path-independent ContentId, closed versioned schemas, fingerprints/ContentBuildId, fastgltf import, meshoptimizer, KTX2 cooking, glslang validation, explicit collision/navigation/terrain products, no general gameplay scripting VM, immutable Content Registry/cache, deterministic dependency builds, classified hot reload, provenance/license validation, and registry-first runtime loading.

Artifacts: `70_content_repository_layout_and_source_formats.md` through `79_content_runtime_loading_failure_and_scene_integration.md`, plus `TA10_CROSS_VALIDATION.md`.

## TA-11 — Input, UI, Audio, and Presentation Integration

**Status:** Architecture Complete / Implementation Locked

Defines fixed-tick semantic input/remapping, explicit input/focus contexts, retained shipping UI with HarfBuzz/FreeType text, knowledge-filtered HUD/management/tutorial flows, miniaudio-backed presentation audio separated from AI hearing, subtitles/captions/typed alarms/accessibility, presentation-only animation/camera/VFX, and deterministic stable-state/event handoff.

Artifacts: `80_input_device_action_and_context_routing.md` through `89_ta11_runtime_phase_integration_debugging_and_validation.md`, plus `TA11_CROSS_VALIDATION.md`.

## TA-12 — Persistence Implementation Architecture

**Status:** Architecture Complete / Implementation Locked

Defines exact save-container bytes/SectionKind codecs, Stable Save Boundary snapshot capture, immutable Manual/Quick/Autosave generations, crash-safe atomic commit, staged all-or-nothing load/session replacement, deterministic migrations/content compatibility, separate profile persistence, recovery/inspection tooling, and saved-tick resume semantics.

Artifacts: `90_persistence_service_and_snapshot_orchestration.md` through `99_ta12_runtime_integration_debugging_and_validation.md`, plus `TA12_CROSS_VALIDATION.md` and `TA12_ARCHITECTURE_DECISIONS.md`.

## TA-13 — Concurrency, Performance, Memory, and Streaming Budgets

**Status:** Architecture Complete / Implementation Locked

Defines percentile-based 60 Hz / 1080p High reference targets, bounded worker queues/backpressure, fixed-tick backlog recovery without Simulation-Time skipping, active-runtime/station/AI/strategic scale envelopes, CPU/GPU/content/streaming budgets, renderer/UI/audio/persistence/content-build limits, standardized profiler telemetry, and representative benchmark scenarios.

Artifacts: `100_performance_target_and_budget_framework.md` through `109_profiling_telemetry_benchmark_scenes_and_degradation.md`, plus `TA13_CROSS_VALIDATION.md` and `TA13_ARCHITECTURE_DECISIONS.md`.

## TA-14 — Testing, Diagnostics, and CI Architecture

**Status:** Architecture Complete / Implementation Locked

Defines layered CMake/CTest/Catch2 verification, deterministic headless scenarios, backend-adapter smoke tests, content/shader/cook determinism, persistence goldens/migrations/corruption/fault injection, typed diagnostics, warnings/static-analysis/sanitizer gates, stable least-privilege GitHub Actions checks, controlled performance certification, exact-SHA evidence, and zero-test protection.

Artifacts: `110_test_architecture_layers_targets_and_conventions.md` through `119_ci_evidence_artifacts_quarantine_and_release_certification.md`, plus `TA14_CROSS_VALIDATION.md` and `TA14_ARCHITECTURE_DECISIONS.md`.

## TA-15 — Architecture Integration Audit

**Status:** Architecture Complete / Implementation Locked

Defines and verifies the final TA-0 through TA-14 integration, including authority/ownership/dependency/timing/lifecycle/active-off-screen/content/persistence/presentation/failure/performance/testability traceability. The formal audit ends **260/260 PASS** with **0 blockers, 0 required corrections and 0 implementation-critical open architecture questions**.

Artifacts: `120_ta15_audit_scope_method_and_evidence.md` through `129_ta15_final_integration_verdict.md`, plus `TA15_CROSS_VALIDATION.md`.

## TA-16 — Implementation Roadmap and Contract Locking

**Status:** Implementation Locked / Final Technical Architecture Phase Complete

TA-16 closes the final pre-implementation lock by fixing:

- `TA16-V1` reproducible compiler/CMake/vcpkg/dependency baseline;
- glad2 v2.0.8 generated-source boundary instead of the vcpkg glad1 port;
- concrete CMake target registry and dependency DAG;
- source/scaffolding layout;
- configure/build/test presets and stable CTest labels;
- zero-test discovery protection;
- executable non-placeholder bootstrap CI with immutable Action SHAs;
- stable required-check names and branch-ruleset target state;
- controlled reference runner `SF-PERF-WIN-01`;
- dependency-ordered IMP-0 through IMP-16 roadmap;
- first end-to-end Horizon Test Cell V0 acceptance matrix;
- per-contract Implementation Locked/change-control rules;
- short-lived branch/squash-PR/evidence-milestone policy;
- architecture-decision continuity through AD-141.

TA-16 materializes a minimal real bootstrap (`sf_core`, `starforge-headless`, `starforge_tests_unit`) solely so build/test/CI contracts are executable rather than placeholder-green. Gameplay implementation begins after TA-16 at IMP-1.

Artifacts: `130_ta16_scope_lock_authority_and_exit_criteria.md` through `139_ta16_final_implementation_handoff.md`, `TA16_ARCHITECTURE_DECISIONS.md`, `ARCHITECTURE_DECISION_REGISTRY.md`, and `TA16_CROSS_VALIDATION.md`.

## Implementation Roadmap Handoff

The implementation sequence is owned in detail by `136_implementation_roadmap_and_vertical_slice.md`.

Immediate state:

```text
IMP-0 — Build / Verification Bootstrap
Status: Complete as TA-16 materialization

IMP-1 — Core, Identity, Deterministic Simulation, and Transactions
Status: NEXT
```

## Current Sequence

GDS Design Complete  
→ TA-0 complete  
→ TA-1 through TA-14 Architecture Complete  
→ TA-15 Architecture Integration Audit 260/260 PASS  
→ **TA-16 Implementation Roadmap and Contract Locking — Implementation Locked**  
→ IMP-0 bootstrap materialized  
→ **IMP-1 next — Core / Identity / Deterministic Simulation / Transactions**  
→ IMP-2 data/persistence foundation  
→ IMP-3/4 local runtime foundation  
→ IMP-5 Horizon Test Cell V0  
→ dependency-ordered systemic gameplay implementation.
