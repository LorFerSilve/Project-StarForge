# Project StarForge

Project StarForge is a custom-built 3D science-fiction action, exploration, base-building, strategy, and light-simulation game developed in C++ and OpenGL without a general-purpose game engine.

The game centers on a physically traversable and expandable player-owned space station. The player undertakes expeditions, gathers resources and technology, recruits survivors, develops spacecraft and robot squads, automates station operations, and attacks or defends space stations.

## Current Project Phase

The authoritative Game Design Specification has completed **GDS-0 through GDS-14** and is formally **Design Complete**.

The project has now entered the **Technical Architecture** phase.

Technical Architecture stages **TA-0** and **TA-1** are complete/Architecture Complete. The next dependency is **TA-2 — Identity, Domain State, Transactions, and Serialization Contracts**.

Gameplay implementation and C++/OpenGL scaffolding have **not** begun yet. They remain gated behind the remaining Technical Architecture work, architecture integration audit, and implementation roadmap/contract locking.

## Documentation

Authoritative game design:

[`docs/game_design/`](docs/game_design/)

Technical architecture:

[`docs/technical_architecture/`](docs/technical_architecture/)

Technical Architecture roadmap:

[`docs/technical_architecture/TA_ROADMAP.md`](docs/technical_architecture/TA_ROADMAP.md)

Final GDS-14 maturity/promotion evidence:

[`docs/game_design/audit/`](docs/game_design/audit/)

## Initial Technical Baseline

- C++23
- CMake
- vcpkg manifest mode
- OpenGL 4.6 Core Profile
- GLFW
- glad2
- GLM
- Jolt Physics behind a StarForge adapter
- miniaudio behind a StarForge audio layer
- fastgltf / glTF 2.0 asset pipeline
- Dear ImGui for development tooling only
- Catch2 for automated C++ tests

## Core Development Principle

Project StarForge implements only the engine and gameplay functionality required by approved game design. Technical architecture follows the Design Complete gameplay requirements rather than speculative engine development.

## Working Title

**Project StarForge**

This is currently a working project title and may be replaced later without changing the project's history or design identity.
