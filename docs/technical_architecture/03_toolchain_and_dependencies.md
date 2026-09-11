# Toolchain and Dependency Architecture

> **Status:** Architecture Complete  
> **Authority:** Language, build system, platform baseline, third-party dependency roles, pinning, and dependency boundaries

## 1. Language Standard

Project StarForge uses **C++23**.

The project may use C++23 standard-library functionality when it is supported by the pinned compiler baseline.

The codebase does not maintain a C++17/C++20 compatibility mode.

## 2. Build System

The project uses **CMake** as its only authoritative build-description system.

IDE project files are generated from CMake and are never maintained as independent build truth.

## 3. Dependency Management

Third-party C/C++ dependencies use **vcpkg manifest mode**.

The repository stores:

- `vcpkg.json`;
- a pinned vcpkg baseline/commit;
- required feature selections;
- platform-specific dependency conditions when necessary.

Developers do not rely on globally installed ad-hoc library versions for reproducible builds.

## 4. Primary Development Platform

Initial development target:

- Windows x64;
- MSVC toolchain;
- desktop OpenGL 4.6-capable GPU/driver.

The architecture remains portable enough for a later Linux x64 build, but Linux parity is not allowed to delay the first executable foundation unless the implementation roadmap explicitly schedules it.

## 5. Compiler Policy

Primary compiler:

- current supported MSVC for C++23 project baseline.

CI adds at least one independent compiler family once source scaffolding exists, preferably Clang on Linux or clang-cl on Windows, to catch nonportable assumptions.

Exact compiler versions are pinned/documented in the implementation phase.

## 6. Graphics API

The renderer targets **OpenGL 4.6 Core Profile**.

Compatibility-profile/deprecated fixed-function OpenGL is prohibited.

The project directly owns:

- render passes;
- shaders;
- resource lifetime;
- material system;
- framebuffers;
- draw submission;
- lighting/shadows;
- GPU debugging/profiling.

## 7. GLFW

**GLFW** provides only the platform-facing window/context/input-device foundation:

- desktop window;
- OpenGL context creation;
- keyboard/mouse events;
- gamepad/controller discovery/input where supported;
- framebuffer/window events.

GLFW does not own gameplay bindings or UI action semantics.

## 8. glad2

**glad2** provides generated OpenGL function loading for the selected OpenGL 4.6 Core API.

Generated loader code is treated as third-party/generated infrastructure.

No gameplay module includes glad headers.

## 9. GLM

**GLM** is adopted as the foundational runtime math library for graphics-compatible vectors, matrices, quaternions, transforms, and common math operations.

Project-specific semantic types can wrap GLM values where units/meaning require stronger distinction.

Examples:

- SimulationDuration;
- TickIndex;
- MassKg;
- PersistentId;
- world/local coordinate abstractions where later required.

## 10. Jolt Physics

**Jolt Physics** supplies commodity collision and rigid-body physics.

It is wrapped by the StarForge `physics` module.

Direct Jolt types do not become persistent/gameplay domain types.

The project retains authority for:

- character movement rules;
- damage;
- collision gameplay consequences;
- docking validity;
- mission boundaries;
- station structural graph;
- ship disable/destroy semantics.

## 11. miniaudio

**miniaudio** supplies the audio device/backend, decoding/mixing facilities used by the StarForge audio layer.

Gameplay code emits semantic audio intents/events rather than calling miniaudio directly.

## 12. fastgltf

**fastgltf** is the canonical runtime/build-pipeline parser for **glTF 2.0** mesh/scene asset import.

The project uses glTF as the primary authored 3D asset exchange format.

Runtime representation is StarForge-owned and does not keep parser objects as gameplay state.

## 13. Texture/Image Support

Texture decoding/transcoding is kept behind the content/render-resource pipeline.

The initial implementation can use a small decoder dependency suitable for common source formats, but shipping texture representation should support later GPU-friendly compression without changing gameplay/content IDs.

The exact decoder/compression package is selected during the asset-pipeline architecture phase rather than prematurely fixed here.

## 14. Dear ImGui

**Dear ImGui** is permitted only for developer/debug tooling:

- inspectors;
- profilers;
- render/physics diagnostics;
- graph visualization;
- debug commands.

It is **not** the baseline implementation of the player-facing HUD/menu/accessibility UI because the GDS requires shipping presentation behavior that should remain project-owned and accessibility-aware.

## 15. Catch2

**Catch2** is the unit/component test framework.

Tests must be runnable without launching the full game window unless they specifically exercise platform/render integration.

## 16. Logging

The architecture uses a StarForge-owned logging facade.

The underlying logging implementation may use a lightweight dependency selected during scaffolding, but game/domain code depends only on the project facade.

Required properties:

- severity;
- category/channel;
- structured context such as IDs/tick;
- development assertions/fatal diagnostics;
- file/console sinks in development.

## 17. Serialization Dependencies

Persistent save serialization is project-owned and versioned.

A third-party JSON/binary library may be used as an encoding utility, but:

- it does not define save schema ownership;
- raw C++ object graphs/pointers are never serialized directly;
- migrations remain explicit StarForge logic.

Exact encoding library/format is decided in the persistence architecture phase.

## 18. No General-Purpose Engine Libraries

The dependency manifest must not pull in a general-purpose engine/framework that takes ownership of:

- scene/game loop;
- rendering architecture;
- gameplay object model;
- editor/runtime lifecycle;
- mission/world systems.

Examples of out-of-scope dependency categories include integrating Unity, Unreal, Godot, O3DE, or another engine as the host runtime.

## 19. Dependency Version Policy

Every direct third-party dependency is pinned through the reproducible toolchain/manifest.

Upgrades are deliberate commits with:

- changelog/release review when relevant;
- clean configure/build;
- automated tests;
- smoke test for affected adapter;
- no silent floating `latest` dependency in CI.

## 20. Transitive Dependency Policy

Avoid adding a large dependency solely for a small convenience utility.

When two libraries solve the same need, prefer the option with:

- smaller conceptual/runtime footprint;
- compatible license;
- active maintenance;
- simple CMake/vcpkg integration;
- clear replacement boundary;
- no hidden engine ownership.

## 21. License Gate

Before a dependency is first committed to the implementation manifest:

- license is recorded;
- redistribution obligations are understood;
- required notices are tracked;
- incompatible/copyleft constraints that conflict with planned distribution are rejected or explicitly reviewed.

## 22. Build Configurations

Initial CMake architecture provides at least:

- `Debug`;
- `RelWithDebInfo`;
- `Release`.

Development-only diagnostics/tools compile conditionally rather than changing gameplay semantics.

## 23. Warnings

Project-owned code is compiled with a strict warning baseline.

Warnings from third-party headers are isolated/suppressed at dependency boundaries rather than weakening project warnings globally.

Warnings-as-errors is enabled in CI for project-owned targets after the initial toolchain stabilizes.

## 24. Static Analysis and Formatting

The implementation phase will standardize:

- `clang-format`;
- `clang-tidy` on selected project targets/checks;
- compiler sanitizers where supported;
- MSVC runtime/debug diagnostics where useful.

Configuration files are repository-owned.

## 25. No Precompiled Binary Dependency Assumption

A clean supported development machine must be able to restore/build dependencies from the documented manifest and toolchain process without relying on undocumented local DLL/lib copies.

## 26. Explicit Non-Goals

This toolchain does not require:

- a custom compiler;
- custom build system;
- vendoring every dependency source manually;
- a game engine package manager;
- a scripting-language dependency;
- networking dependencies in baseline;
- player-facing Dear ImGui UI.