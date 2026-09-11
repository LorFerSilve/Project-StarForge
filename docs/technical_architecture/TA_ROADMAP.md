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

**Status:** Next

Must define:

- strongly typed PersistentId representation;
- RuntimeEntityHandle representation;
- persistent domain stores;
- activation/deactivation data contracts;
- command/result/event type conventions;
- cross-domain transaction coordinator;
- immutable read models;
- save DTO ownership;
- schema/version conventions;
- deterministic RNG stream storage;
- migration/integrity boundaries.

Primary GDS dependencies:

- physical ownership;
- unique persistent identities;
- Stable Save Boundaries;
- mission/event/robot/ship IDs;
- transactions and anti-duplication.

## TA-3 — World, Scene, Zone, and Streaming Architecture

**Status:** Planned

Must define:

- Active Local Context;
- Zone/Scene lifecycle;
- mission sub-zone streaming;
- persistent location activation;
- Horizon active/off-screen transitions;
- spatial partitioning;
- local/global coordinate strategy;
- origin/precision strategy;
- world bounds;
- terrain/interior/space scene composition;
- transition/loading state machine.

## TA-4 — Rendering Architecture

**Status:** Planned

Must define:

- OpenGL object/resource ownership;
- render graph/pass structure;
- material/shader model;
- camera/view system;
- lighting/shadows;
- opaque/transparent ordering;
- instancing/batching;
- particles/VFX;
- debug rendering;
- GPU asset upload lifecycle;
- resize/device/context failure behavior;
- graphics settings/performance tiers.

## TA-5 — Physics, Collision, Character, and Spaceflight Integration

**Status:** Planned

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

- concrete save container/encoding;
- snapshot manifest;
- domain serialization registry;
- checksums/integrity;
- write-new-then-commit;
- autosave/manual/quicksave file layout;
- migration pipeline;
- crash recovery;
- corruption diagnostics;
- deterministic load validation.

TA-2 establishes persistence contracts; TA-12 selects/defines their concrete storage implementation.

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
- scalability strategy for Horizon/off-screen state.

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
→ **TA-2 next**  
→ TA-3 ... TA-15  
→ TA-16 implementation roadmap/locking  
→ C++/OpenGL scaffolding  
→ gameplay implementation.
