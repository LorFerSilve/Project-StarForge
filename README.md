# Project StarForge

Project StarForge is a custom-built 3D science-fiction action, exploration, base-building, strategy, and light-simulation game developed in C++ and OpenGL without a general-purpose game engine.

The game centers on a physically traversable and expandable player-owned space station. The player undertakes expeditions, gathers resources and technology, recruits survivors, develops spacecraft and robot squads, automates station operations, and attacks or defends space stations.

## Current Project Phase

The authoritative Game Design Specification completed **GDS-0 through GDS-14** and is formally **Design Complete**.

Technical Architecture completed **TA-0 through TA-16**. TA-15's final integration audit ended **260/260 PASS** with 0 blockers, and TA-16 has now converted the architecture into the first **Implementation Locked** baseline: `TA16-V1`.

The project is therefore leaving Technical Architecture and entering **dependency-ordered implementation**.

The next formal dependency is:

> **IMP-1 — Core, Identity, Deterministic Simulation, and Transactions**

TA-16 materialized only the minimum non-gameplay bootstrap required to make the implementation contract executable: pinned toolchain/dependencies, root CMake/presets, `sf_core`, `starforge-headless`, Catch2 unit tests, zero-test protection, and a real GitHub Actions Build & Unit / CI Gate. Broad gameplay implementation has not started yet.

## Architecture State

The locked architecture includes:

- purpose-built C++23 modular monolith;
- fixed 60 Hz authoritative Simulation Time;
- typed persistent identity separate from runtime handles;
- prepared atomic cross-domain transactions and immutable read models;
- one authoritative player-local SceneInstance plus persistent off-screen world state;
- custom OpenGL 4.6 renderer with project-owned resource lifetime;
- Jolt and Recast/Detour behind project adapters;
- persistent-state-first Horizon station simulation;
- generation-checked active runtime entities and Activation Leases;
- knowledge-limited deterministic AI/navigation;
- stable Mission/MissionInstance/raid/DynamicEvent state machines;
- deterministic cooked content pipeline with path-independent ContentIds;
- StarForge-owned shipping UI/input/audio/presentation boundaries;
- exact versioned staged/crash-safe persistence;
- bounded concurrency/performance/memory policies that may never rewrite gameplay semantics;
- headless deterministic verification, backend smoke, content/persistence compatibility and controlled performance certification.

## TA16-V1 Implementation Baseline

Initial locked environment:

- C++23;
- Visual Studio 2022 / Build Tools 17.14.39 (installation build 17.14.37614.0), MSVC v143 14.44 x64;
- primary hosted bootstrap certification image `windows-2022`;
- CMake 4.3.3;
- LLVM/Clang 23.1.1 independent tooling/compiler line;
- vcpkg manifest baseline `a1cae005c39be7b18ba319fced856b68d7276271`;
- OpenGL 4.6 Core Profile;
- GLFW 3.5.1;
- glad2 v2.0.8 generated-source contract;
- GLM 1.0.3;
- Jolt Physics 5.6.0#1;
- Recast/Detour 1.6.0#1;
- miniaudio 0.11.25;
- FreeType 2.14.3 + HarfBuzz 14.4.0;
- fastgltf 0.9.0;
- KTX-Software 4.4.2;
- meshoptimizer 1.2;
- glslang 16.4.0;
- Dear ImGui 1.92.9 for development tooling only;
- Catch2 3.16.0;
- simdjson 4.6.8 where selected by content tooling.

## First Implementation Roadmap

The dependency-ordered plan is defined in [`docs/technical_architecture/136_implementation_roadmap_and_vertical_slice.md`](docs/technical_architecture/136_implementation_roadmap_and_vertical_slice.md).

The first major end-to-end proof is **IMP-5 — Horizon Test Cell V0**: a small authored room in which first-person movement/collision, persistent physical item ownership, typed transfer, read-model feedback, Stable Save Boundary, save/quit/load, and runtime reconstruction are proven together before broad station/AI/mission/spacecraft development.

## Documentation

Authoritative game design:

[`docs/game_design/`](docs/game_design/)

Technical architecture and Implementation Locked contracts:

[`docs/technical_architecture/`](docs/technical_architecture/)

Technical Architecture roadmap / implementation handoff:

[`docs/technical_architecture/TA_ROADMAP.md`](docs/technical_architecture/TA_ROADMAP.md)

TA-16 final handoff:

[`docs/technical_architecture/139_ta16_final_implementation_handoff.md`](docs/technical_architecture/139_ta16_final_implementation_handoff.md)

TA-16 final cross-validation:

[`docs/technical_architecture/TA16_CROSS_VALIDATION.md`](docs/technical_architecture/TA16_CROSS_VALIDATION.md)

Consolidated architecture decision registry:

[`docs/technical_architecture/ARCHITECTURE_DECISION_REGISTRY.md`](docs/technical_architecture/ARCHITECTURE_DECISION_REGISTRY.md)

Final GDS-14 maturity/promotion evidence:

[`docs/game_design/audit/`](docs/game_design/audit/)

## Bootstrap Build

The TA-16 bootstrap is intentionally small and is not gameplay implementation.

With the locked vcpkg checkout exposed as `VCPKG_ROOT`, the primary Windows loop is:

```text
cmake --preset windows-msvc-debug
cmake --build --preset windows-msvc-debug
cmake -DSTARFORGE_TEST_BUILD_DIR=out/build/windows-msvc-debug -DSTARFORGE_TEST_LABEL=unit -DSTARFORGE_TEST_CONFIG=Debug -P cmake/RequireTests.cmake
ctest --preset windows-unit
```

## Core Development Principle

Project StarForge implements only the engine and gameplay functionality required by approved game design. Code remains subordinate to the Design Complete GDS and Implementation Locked architecture; implementation difficulty is not permission to silently change gameplay, ownership, persistence, determinism, or dependency boundaries.

## Working Title

**Project StarForge**

This is currently a working project title and may be replaced later without changing the project's history or design identity.
