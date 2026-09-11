# Project StarForge — Technical Architecture Specification

> **Status:** Active  
> **Authority:** Technical realization of the Design Complete GDS

This directory translates the authoritative Game Design Specification under `docs/game_design/` into an implementable C++/OpenGL software architecture.

## Current Phase

The Game Design Specification is **Design Complete**.

Technical Architecture has completed:

- **TA-0 — Architecture Governance and Constraints**;
- **TA-1 — System Context, Toolchain, and Runtime Foundation**;
- **TA-2 — Identity, Domain State, Transactions, and Serialization Contracts**.

The next dependency is **TA-3 — World, Scene, Zone, and Streaming Architecture**.

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

### Governance

- [`ARCHITECTURE_DECISIONS.md`](ARCHITECTURE_DECISIONS.md)
- [`TA_ROADMAP.md`](TA_ROADMAP.md)

Later TA phases define world/scene ownership, rendering, physics, station graphs, runtime entities, AI/navigation, missions/raids, asset/content pipeline, audio/input/UI boundaries, persistence storage details, concurrency, observability/testing, and implementation handoff.

## Implementation Gate

A technical subsystem is ready for code only when:

1. its relevant GDS contracts are Design Complete;
2. its technical architecture contract is sufficiently complete;
3. ownership/lifetime/threading boundaries are explicit;
4. data and persistence boundaries are explicit where applicable;
5. dependencies are selected and pinned where required;
6. tests/validation expectations are defined;
7. the implementation roadmap places it in an approved phase.

`Implementation Locked` remains a later per-contract handoff state. TA-2 Architecture Complete does **not** authorize C++/OpenGL scaffolding yet.