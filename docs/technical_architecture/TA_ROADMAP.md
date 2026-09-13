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

Artifacts: `70_content_repository_layout_and_source_formats.md` through `79_content_runtime_loading_failure_and_scene_integration.md`, plus `TA10_CROSS_VALIDATION.md`.

## TA-11 — Input, UI, Audio, and Presentation Integration

**Status:** Architecture Complete

Defines fixed-tick semantic input/remapping, explicit input/focus contexts, retained shipping UI with HarfBuzz/FreeType text, knowledge-filtered HUD/management/tutorial flows, miniaudio-backed presentation audio separated from AI hearing, subtitles/captions/typed alarms/accessibility, presentation-only animation/camera/VFX, and deterministic stable-state/event handoff.

Artifacts: `80_input_device_action_and_context_routing.md` through `89_ta11_runtime_phase_integration_debugging_and_validation.md`, plus `TA11_CROSS_VALIDATION.md`.

## TA-12 — Persistence Implementation Architecture

**Status:** Architecture Complete

Defines exact save-container bytes/SectionKind codecs, Stable Save Boundary snapshot capture, immutable Manual/Quick/Autosave generations, crash-safe atomic commit, staged all-or-nothing load/session replacement, deterministic migrations/content compatibility, separate profile persistence, recovery/inspection tooling, and saved-tick resume semantics.

Artifacts: `90_persistence_service_and_snapshot_orchestration.md` through `99_ta12_runtime_integration_debugging_and_validation.md`, plus `TA12_CROSS_VALIDATION.md` and `TA12_ARCHITECTURE_DECISIONS.md`.

## TA-13 — Concurrency, Performance, Memory, and Streaming Budgets

**Status:** Architecture Complete

Defines percentile-based 60 Hz / 1080p High reference software targets, bounded worker queues/backpressure, fixed-tick backlog recovery without Simulation-Time skipping, active-runtime/station/AI/strategic scale envelopes, CPU/GPU/content/streaming budgets, renderer/UI/audio/persistence/content-build limits, standardized profiler telemetry and ten representative benchmark scenarios.

Artifacts: `100_performance_target_and_budget_framework.md` through `109_profiling_telemetry_benchmark_scenes_and_degradation.md`, plus `TA13_CROSS_VALIDATION.md` and `TA13_ARCHITECTURE_DECISIONS.md`.

## TA-14 — Testing, Diagnostics, and CI Architecture

**Status:** Architecture Complete

Defines:

- layered CMake/CTest/Catch2 verification with unit/domain, headless integration, deterministic scenario, backend smoke, golden/compatibility, fault/corruption and performance suites;
- fixed-seed/tick scripted headless scenarios, semantic checkpoint/state-hash diagnostics, transaction/ownership/Activation Lease tests, worker-count and frame-rate equivalence, active/off-screen station equivalence, AI knowledge constraints, mission/raid/event/finale exactly-once tests;
- adapter-level Jolt/Recast/OpenGL/GLFW/miniaudio/FreeType/HarfBuzz/fastgltf/KTX2/meshoptimizer/glslang smoke tests without backend identity becoming gameplay authority;
- closed-schema/content reference/provenance validation, all required GLSL variant validation, deterministic clean/incremental/no-op content cooks, atomic registry publication and hot-reload safety tests;
- exact persistence v1 binary goldens, current/historical migration fixtures, CRC/truncation/size corruption matrices, staged-load isolation, save-write crash fault injection, autosave/catalog recovery, RNG continuation and profile fail-soft tests;
- typed bounded diagnostics, development assertions, crash context, deterministic traces, Dear ImGui inspectors, debug commands, state/graph dumps, stall/memory/leak diagnostics and privacy-safe failure artifacts;
- warnings-as-errors, deterministic formatting, pinned static analysis, architecture-boundary linting, ASan/UBSan blocking gates, leak checking and scheduled/release TSan where supported;
- GitHub Actions PR/main/nightly/reference workflow architecture, stable aggregate required checks, zero-test protection, least-privilege/untrusted-PR security, bounded caches, artifact/report semantics and final `StarForge / CI Gate`;
- TA-13 reference-runner performance regression gates using controlled hardware, percentile/memory/Hard-Hold thresholds, reviewed versioned baselines and no rerun-until-green policy;
- exact-SHA certification evidence, bounded artifact retention classes, strict flaky/quarantine issue-owner-expiry policy, and release/Implementation Locked certification requirements;
- explicit deferral of executable `.github/workflows/*.yml` until TA-16 creates real CMake/CTest targets/presets and pins toolchain/runner versions.

Artifacts: `110_test_architecture_layers_targets_and_conventions.md` through `119_ci_evidence_artifacts_quarantine_and_release_certification.md`, plus `TA14_CROSS_VALIDATION.md` and `TA14_ARCHITECTURE_DECISIONS.md`.

## TA-15 — Architecture Integration Audit

**Status:** Architecture Complete

Defines and verifies:

- one final evidence/severity method for cross-validating TA-0 through TA-14 against the Design Complete GDS;
- authority, persistent/runtime identity, physical ownership, mutation rights, transactions, backend encapsulation and dependency direction;
- Simulation Time, fixed-tick ordering, main-thread commit ownership, worker non-authority, Stable Boundaries, lifecycle/deferred destruction and save/load ordering;
- one active local SceneInstance, residency/activation separation, streaming holds, physics/navigation/gameplay boundaries, station conservation and active↔off-screen equivalence;
- mission/objective/raid/reinforcement/DynamicEvent/communications/recovery/finale exactly-once semantics;
- ContentId/cooked-content/runtime compatibility, staged persistence/migrations and presentation/input/UI/audio/accessibility non-authority;
- failure/recovery/performance degradation rules and complete TA-14 verification paths;
- complete GDS-0 through GDS-14 → Technical Architecture traceability;
- a realizable module/build dependency DAG with no required compile-time or semantic ownership cycle;
- an implementation-readiness/risk register separating normal engineering risk and TA-16 lock items from architecture blockers;
- a final **260/260 PASS** cross-validation with **0 blockers, 0 required corrections and 0 implementation-critical open architecture questions**.

TA-15 introduces no new AD number; it certifies already accepted architecture rather than changing it during audit.

Artifacts: `120_ta15_audit_scope_method_and_evidence.md` through `129_ta15_final_integration_verdict.md`, plus `TA15_CROSS_VALIDATION.md`.

## TA-16 — Implementation Roadmap and Contract Locking

**Status:** Next

Must produce the dependency-ordered implementation phases, initial vertical slice, source/scaffolding plan, exact initial CMake targets/presets, pinned compiler/vcpkg/dependency/action versions, concrete reference hardware/runners, CTest labels/fixtures, executable GitHub Actions workflows/required checks, architecture-decision addendum consolidation, branch/merge strategy, milestone/exit criteria, and per-contract `Implementation Locked` handoff.

Only after TA-16 completes the relevant lock/scaffolding gate may planned C++/OpenGL scaffolding begin.

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
→ TA-11 Architecture Complete  
→ TA-12 Architecture Complete  
→ TA-13 Architecture Complete  
→ TA-14 Architecture Complete  
→ TA-15 Architecture Complete  
→ **TA-16 next — implementation roadmap/contract locking**  
→ per-contract Implementation Locked  
→ C++/OpenGL scaffolding  
→ dependency-ordered implementation.