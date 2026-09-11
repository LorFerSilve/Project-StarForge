# Technical Architecture Roadmap

> **Status:** Active  
> **Authority:** Dependency-driven technical architecture sequencing

This roadmap defines the technical architecture work that must be completed before and during implementation handoff.

## TA-0 — Architecture Governance and Constraints

**Status:** Complete

Defines:

- architecture authority and precedence;
- Architecture Complete / Implementation Locked status model;
- no-general-engine principle;
- GDS traceability;
- third-party dependency policy;
- implementation gate.

Artifacts:

- `00_architecture_authority.md`;
- `01_architecture_principles.md`.

## TA-1 — System Context, Toolchain, and Runtime Foundation

**Status:** Architecture Complete

Defines:

- modular-monolith process model;
- top-level module partition;
- dependency direction;
- active/off-screen context split;
- C++23/CMake/vcpkg/OpenGL baseline;
- third-party library roles/boundaries;
- 60 Hz fixed authoritative simulation;
- command/transaction/event model;
- worker-job authority;
- Stable Simulation Boundary;
- save snapshot capture boundary.

Artifacts:

- `02_system_context.md`;
- `03_toolchain_and_dependencies.md`;
- `04_runtime_and_simulation_model.md`.

## TA-2 — Identity, Domain State, Transactions, and Serialization Contracts

**Status:** Architecture Complete

Defines:

- strongly typed non-zero 64-bit `PersistentId<Tag>` representation;
- index+generation `RuntimeEntityHandle` representation;
- canonical UTF-8 `ContentId` references;
- persistent domain-store ownership and deterministic enumeration;
- `StateRevision` and `ActivationEpoch` stale-state protection;
- Activation Lease authority transfer between persistent and active state;
- typed Command / Result / Event conventions;
- deterministic command/event sequencing;
- cross-domain TransactionCoordinator;
- physical ownership and Credit transaction contracts;
- immutable revisioned consumer Read Models;
- per-domain versioned Save DTO ownership;
- StarForge-owned little-endian chunked binary save container;
- CRC32C section integrity and initial `None` compression codec;
- staged all-or-nothing loading/migration;
- project-owned PCG32 deterministic RNG and scoped stream derivation;
- generation-version and integrity validation contracts.

Artifacts:

- `05_identity_and_reference_model.md`;
- `06_domain_state_and_activation_contracts.md`;
- `07_command_result_and_event_contracts.md`;
- `08_transaction_and_ownership_contracts.md`;
- `09_read_models_and_state_revisions.md`;
- `10_serialization_and_save_contracts.md`;
- `11_rng_migration_and_integrity_contracts.md`;
- `TA2_CROSS_VALIDATION.md`.

## TA-3 — World, Scene, Zone, and Streaming Architecture

**Status:** Architecture Complete

Defines:

- one authoritative player-local SceneInstance at a time;
- strategic galaxy graph separated from local 3D scene coordinates;
- ActiveLocalContext ownership kinds separated from physical SceneProfile kinds;
- Scene lifecycle and SceneGeneration stale-result protection;
- Mission/World Zone Instance and stable local-key projection;
- stream-cell residency separated from simulation activation;
- deterministic activation sets and non-authoritative predictive prefetch;
- Hard Streaming Hold with Simulation Time freeze;
- persistent double-precision Context Space positions;
- origin-relative runtime coordinates and `RuntimeOrigin64`/`OriginEpoch` rebasing;
- loose hashed gameplay spatial index distinct from streaming/physics/render structures;
- explicit Horizon active/off-screen handoff;
- off-screen Horizon Defense → active-scene continuity;
- staged destination scene preparation and atomic context-switch transitions;
- Mission/World persistent-state projection over immutable content;
- Interior, Surface, EVA, LocalSpaceflight, Horizon, and Mixed scene-composition profiles.

Artifacts:

- `12_active_local_context_and_scene_model.md`;
- `13_zone_streaming_and_residency_model.md`;
- `14_coordinate_precision_and_spatial_partitioning.md`;
- `15_horizon_active_offscreen_handoff.md`;
- `16_scene_transition_and_loading_state_machine.md`;
- `17_zone_state_and_persistent_world_projection.md`;
- `18_scene_composition_profiles.md`;
- `TA3_CROSS_VALIDATION.md`.

## TA-4 — Rendering Architecture

**Status:** Architecture Complete

Defines:

- main-thread OpenGL 4.6 Core context and thin StarForge `RenderDevice` ownership;
- immutable RenderSnapshot frame input and fixed-simulation/variable-render interpolation;
- linear HDR scene rendering with SDR sRGB output and native-resolution shipping UI composition;
- reversed-Z zero-to-one floating-point depth;
- hybrid deferred opaque/alpha-mask rendering plus forward transparency/special/VFX;
- fixed-purpose render-pass/resource dependency schedule;
- four-cascade primary directional shadows and bounded local-light shadow budgets;
- compute-assisted tiled deferred local-light culling;
- glTF-compatible metallic-roughness PBR, IBL/environment probes, and explicit direct lights;
- bounded GLSL 4.60 shader families/variant keys and interface validation;
- FirstPerson, SpacecraftChase, Cockpit, Management/Strategic, and Debug view contracts;
- renderer-only frustum/LOD/optional conservative occlusion culling;
- stable draw ordering, batching, hardware instancing, and transparent sorting;
- renderer-side GPU particle/VFX execution with no gameplay authority;
- Reduced Effects/Photosensitivity critical-representation guarantees;
- main-thread GPU upload/deletion queues, generation-checked RenderResourceHandles, caching, and fence-safe retirement;
- Low/Medium/High/Ultra/Custom graphics presets whose changes are presentation-only;
- Off/FXAA initial anti-aliasing and no baseline motion-blur implementation;
- resize/minimize/context-creation/context-loss failure policy.

Artifacts:

- `19_renderer_ownership_and_frame_pipeline.md`;
- `20_render_graph_and_passes.md`;
- `21_camera_and_view_system.md`;
- `22_material_shader_and_lighting_model.md`;
- `23_visibility_culling_batching_and_transparency.md`;
- `24_vfx_particles_and_debug_rendering.md`;
- `25_gpu_resource_upload_and_lifecycle.md`;
- `26_graphics_settings_resize_and_failure_recovery.md`;
- `TA4_CROSS_VALIDATION.md`.

## TA-5 — Physics, Collision, Character, and Spaceflight Integration

**Status:** Next

Must define:

- Jolt adapter API;
- collision layers/filter matrix;
- character controller architecture;
- triggers/interactions;
- projectile/query integration;
- ship rigid-body/6DoF integration;
- docking contacts;
- zero-g player movement;
- collision damage interface;
- station static/kinematic geometry updates;
- fixed-step synchronization.

## TA-6 — Station Simulation and Graph Architecture

**Status:** Planned

Must define technical representations for:

- structural graph;
- compartment graph;
- utility networks;
- power graph;
- atmosphere/pressure volumes;
- thermal system;
- water;
- logistics;
- manufacturing;
- construction transactions;
- damage/repair topology invalidation;
- off-screen coarse scheduling.

## TA-7 — Gameplay Runtime Entity Architecture

**Status:** Planned

Must define:

- runtime entity registry;
- component pools;
- actor lifecycle;
- player runtime state;
- item/equipment runtime state;
- combat-facing entities;
- projectiles/status effects;
- interactive objects;
- persistent/runtime synchronization;
- deferred destruction.

## TA-8 — AI and Navigation Architecture

**Status:** Planned

Must define:

- navigation representation for interiors/surfaces;
- dynamic path invalidation;
- heavy/light robot traversal constraints;
- crew task navigation;
- enemy perception/memory;
- robot command AI;
- station automation scheduling;
- asynchronous pathfinding job model;
- off-screen behavior abstraction;
- debugging/validation.

## TA-9 — Missions, Raids, Dynamic Events, and Strategic State Machines

**Status:** Planned

Must define:

- MissionInstance runtime/persistent representation;
- objective state machine infrastructure;
- deterministic procedural generation pipeline;
- raid phase orchestration;
- reinforcement scheduling;
- Dynamic Event scheduler;
- world-location persistent consequences;
- recovery/communication integration;
- finale transaction state.

## TA-10 — Content and Asset Pipeline

**Status:** Planned

Must define:

- canonical asset source/exchange formats;
- glTF import;
- textures/materials;
- shader source/build pipeline;
- content IDs/references;
- content schemas;
- validation tooling;
- cooked/runtime asset formats if needed;
- asset registry/cache;
- dependency tracking;
- development hot reload boundaries.

## TA-11 — Input, UI, Audio, and Presentation Integration

**Status:** Planned

Must define:

- raw input → action mapping;
- context/action routing;
- remapping persistence;
- controller/aim-assist boundary;
- shipping UI architecture;
- HUD/read models;
- captions/subtitles;
- alarm presentation;
- miniaudio adapter/buses/spatialization;
- accessibility presentation contracts.

## TA-12 — Persistence Implementation Architecture

**Status:** Planned

Must define:

- exact byte layout of the TA-2 StarForge save container;
- snapshot manifest/section directory representation;
- domain serialization registry;
- CRC32C implementation and integrity handling;
- write-new-then-commit storage flow;
- autosave/manual/quicksave file layout;
- migration pipeline implementation;
- crash recovery;
- corruption diagnostics;
- deterministic load validation.

TA-2 already fixes the persistence semantics and container family; TA-12 specifies the concrete storage implementation without redesigning those contracts.

## TA-13 — Concurrency, Performance, Memory, and Streaming Budgets

**Status:** Planned

Must define:

- worker pool implementation model;
- job categories/priorities;
- immutable snapshot/versioning rules;
- asset streaming budgets;
- CPU/GPU frame budgets;
- memory budgets;
- simulation backlog behavior;
- profiling counters;
- performance test scenes;
- scalability strategy for Horizon/off-screen state;
- TA-3 streaming radius/origin/grid/cache numeric budgets;
- TA-4 render-quality/shadow/texture/effect numeric budgets.

## TA-14 — Testing, Diagnostics, and CI Architecture

**Status:** Planned

Must define:

- CMake target graph tests;
- Catch2 test layers;
- headless simulation tests;
- transaction/persistence tests;
- deterministic-seed regression tests;
- content validation;
- renderer/physics smoke tests;
- clang-format/tidy/warnings;
- sanitizer jobs where supported;
- CI gates;
- debug tooling requirements.

## TA-15 — Architecture Integration Audit

**Status:** Planned

Cross-validates TA-0 through TA-14 against:

- all Design Complete GDS contracts;
- ownership/lifetime boundaries;
- threading;
- persistence;
- world/scene transition consistency;
- performance assumptions;
- subsystem dependency cycles;
- failure recovery;
- testability.

No implementation phase is authorized to invent unresolved architecture after this audit.

## TA-16 — Implementation Roadmap and Contract Locking

**Status:** Planned

Produces:

- dependency-ordered implementation phases;
- vertical-slice definition;
- scaffolding plan;
- exact initial CMake/vcpkg targets;
- first test gates;
- per-contract `Implementation Locked` handoff;
- merge/branch strategy;
- milestone completion criteria.

Only after TA-16 may the project begin the planned C++/OpenGL scaffolding phase.

## Current Sequence

GDS Design Complete  
→ TA-0 complete  
→ TA-1 Architecture Complete  
→ TA-2 Architecture Complete  
→ TA-3 Architecture Complete  
→ TA-4 Architecture Complete  
→ **TA-5 next**  
→ TA-6 ... TA-15  
→ TA-16 implementation roadmap/locking  
→ C++/OpenGL scaffolding  
→ gameplay implementation.
