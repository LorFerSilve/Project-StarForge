# Project StarForge — Technical Architecture Specification

> **Status:** Active  
> **Authority:** Technical realization of the Design Complete GDS

This directory translates the authoritative Game Design Specification under `docs/game_design/` into an implementable C++/OpenGL software architecture.

## Current Phase

The Game Design Specification is **Design Complete**.

The project is now in **Technical Architecture**.

Gameplay implementation and repository scaffolding are not started by this document set. Technical contracts are defined first so implementation does not invent architecture ad hoc.

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

## Architecture Documents

- [`00_architecture_authority.md`](00_architecture_authority.md)
- [`01_architecture_principles.md`](01_architecture_principles.md)
- [`02_system_context.md`](02_system_context.md)
- [`03_toolchain_and_dependencies.md`](03_toolchain_and_dependencies.md)
- [`04_runtime_and_simulation_model.md`](04_runtime_and_simulation_model.md)
- [`TA_ROADMAP.md`](TA_ROADMAP.md)

Later TA phases will define rendering, world/scene ownership, gameplay state, physics, AI/navigation, persistence, asset/content pipeline, audio/input/UI boundaries, concurrency, observability/testing, and implementation handoff.

## Implementation Gate

A technical subsystem is ready for code only when:

1. its relevant GDS contracts are Design Complete;
2. its technical architecture contract is sufficiently complete;
3. ownership/lifetime/threading boundaries are explicit;
4. data and persistence boundaries are explicit where applicable;
5. dependencies are selected and pinned where required;
6. tests/validation expectations are defined;
7. the implementation roadmap places it in an approved phase.

`Implementation Locked` remains a later per-contract handoff state; creating this architecture does not automatically lock every GDS file.