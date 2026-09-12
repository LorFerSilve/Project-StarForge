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

The project directly owns render passes, shaders, resource lifetime, materials, framebuffers, draw submission, lighting/shadows, and GPU diagnostics.

## 7. GLFW

**GLFW** provides only the platform-facing window/context/input-device foundation: desktop window, OpenGL context creation, keyboard/mouse events, controller discovery/input, and framebuffer/window events.

GLFW does not own gameplay bindings or UI action semantics.

## 8. glad2

**glad2** provides generated OpenGL function loading for the selected OpenGL 4.6 Core API.

Generated loader code is third-party/generated infrastructure. No gameplay module includes glad headers.

## 9. GLM

**GLM** is the foundational runtime math library for graphics-compatible vectors, matrices, quaternions, transforms, and common math operations.

Project-specific semantic types can wrap GLM values where units/meaning require stronger distinction, including SimulationDuration, TickIndex, MassKg, PersistentId, and world/local coordinate abstractions.

## 10. Jolt Physics

**Jolt Physics** supplies commodity collision and rigid-body physics behind the StarForge `physics` adapter.

Direct Jolt types do not become persistent/gameplay domain types.

StarForge retains authority for character movement rules, damage, collision consequences, docking validity, mission boundaries, station structure, and ship disable/destroy semantics.

## 11. Recast / Detour

**Recast/Detour** supplies commodity grounded navigation-mesh cooking and path-corridor queries behind the StarForge `navigation` adapter.

The upstream `recastnavigation/recastnavigation` project is not archived at TA-8 selection time; the exact dependency version is pinned in the implementation manifest under the normal version/license gate.

Direct Recast/Detour types such as navmesh/query objects and polygon refs do not become gameplay or persistent identity.

StarForge retains authority for:

- actor traversal profiles;
- security/access rules;
- door/airlock/elevator gameplay;
- hazard knowledge/cost policy;
- Heavy/light robot constraints;
- 3D free-flight/Zero-G navigation;
- AI decisions;
- path-result freshness/deterministic consumption;
- dynamic navigation invalidation semantics.

Recast/Detour does not host the game loop or AI object model.

## 12. miniaudio

**miniaudio** supplies the audio device/backend and decoding/mixing facilities used by the StarForge audio layer.

Gameplay code emits semantic audio intents/events rather than calling miniaudio directly.

## 13. fastgltf

**fastgltf** is the canonical build-pipeline parser for **glTF 2.0** mesh/scene asset import.

The project uses glTF as the primary authored 3D asset exchange format. Runtime representation is StarForge-owned and shipping runtime does not depend on raw glTF DOM objects.

## 14. KTX-Software / KTX2

TA-10 selects **KTX2** as the canonical cooked texture container and **Khronos KTX-Software** as the pinned texture-tool/library boundary.

Canonical source textures use approved authoring formats such as PNG/EXR; the content pipeline performs semantic-aware mip/compression processing and emits validated KTX2.

StarForge ContentId remains texture identity. KTX filenames/objects do not become gameplay identity.

The initial Windows x64 OpenGL 4.6 profile can use GPU block-compressed formats such as BC7/BC5/BC4/BC6H where the TA-10 semantic profile selects them.

Basis Universal remains optional profile technology rather than mandatory desktop runtime semantics.

## 14.1 meshoptimizer

TA-10 selects pinned **meshoptimizer** for offline mesh optimization and declared generated-LOD processing behind the StarForge content tool.

Permitted use includes vertex-cache/fetch optimization, overdraw-oriented reordering and deterministic simplification.

meshoptimizer does not own runtime mesh identity, scene semantics, collision, or LOD gameplay behavior.

## 14.2 glslang

TA-10 selects pinned Khronos **glslang** as the offline/reference validator for declared shipping GLSL 4.60 shader variants.

The StarForge renderer still performs final OpenGL driver compile/link and interface validation at runtime. glslang does not provide persistent driver binaries or own renderer shader-program lifetime.

## 14.3 Source Image Decode Boundary

Shipping runtime does not require ordinary PNG/EXR decode for world textures because textures are cooked to KTX2.

Build-time source decode/conversion occurs inside the content-tool boundary through KTX-Software and any small explicitly selected source-image support required by the implementation.

Adding such a helper decoder remains subject to the normal vcpkg/version/license gate and cannot change texture ContentId or semantic color-space authority.

## 15. Dear ImGui

**Dear ImGui** is permitted only for developer/debug tooling such as inspectors, profilers, render/physics/navigation diagnostics, content-pipeline diagnostics, graph visualization, and debug commands.

It is not the baseline player-facing HUD/menu/accessibility UI.

## 16. Catch2

**Catch2** is the unit/component test framework.

Tests must be runnable without launching the full game window unless they specifically exercise platform/render integration.

TA-10 content-schema/cooker/dependency tests are headless by default.

## 17. Logging

The architecture uses a StarForge-owned logging facade.

The underlying logging implementation may use a lightweight dependency selected during scaffolding, but game/domain code depends only on the project facade.

Required properties include severity, category/channel, structured context such as IDs/tick, development assertions/fatal diagnostics, and file/console sinks in development.

## 18. Serialization Dependencies

Persistent save serialization is project-owned and versioned.

A third-party encoding utility may be used, but it does not define save schema ownership; raw C++ object graphs/pointers are never serialized and migrations remain explicit StarForge logic.

Cooked asset serialization is likewise project-owned/versioned except where TA-10 explicitly adopts a standard container such as KTX2.

## 19. No General-Purpose Engine Libraries

The dependency manifest must not pull in a general-purpose engine/framework that takes ownership of scene/game loop, rendering architecture, gameplay object model, editor/runtime lifecycle, or mission/world systems.

Unity, Unreal, Godot, O3DE, or another engine are out of scope as host runtime.

## 20. Dependency Version Policy

Every direct third-party dependency is pinned through the reproducible toolchain/manifest or explicitly pinned build-tool bootstrap.

Upgrades are deliberate commits with relevant release review, clean configure/build, automated tests, adapter/content-tool smoke tests, affected asset recooking where required, and no floating `latest` dependency in CI.

## 21. Transitive Dependency Policy

Avoid adding a large dependency solely for a small convenience utility.

Prefer compatible-license, actively maintained, small-footprint libraries with simple CMake/vcpkg integration, clear replacement boundaries, and no hidden engine ownership.

## 22. License Gate

Before a dependency is first committed to the implementation manifest:

- license is recorded;
- redistribution obligations are understood;
- required notices are tracked;
- incompatible/copyleft constraints are rejected or explicitly reviewed.

TA-10 applies a parallel provenance/license gate to externally sourced game assets.

## 23. Build Configurations

Initial CMake architecture provides at least `Debug`, `RelWithDebInfo`, and `Release`.

Development-only diagnostics/tools compile conditionally rather than changing gameplay semantics.

Content semantics/fingerprints do not depend on C++ optimization mode except where a distinct explicit content target profile is intentionally selected.

## 24. Warnings

Project-owned code is compiled with a strict warning baseline.

Third-party warnings are isolated at dependency boundaries; warnings-as-errors is enabled in CI for project-owned targets after the initial toolchain stabilizes.

## 25. Static Analysis and Formatting

The implementation phase standardizes repository-owned `clang-format`, selected `clang-tidy` checks, supported sanitizers, and useful MSVC runtime/debug diagnostics.

## 26. No Precompiled Binary Dependency Assumption

A clean supported development machine must restore/build dependencies and content tools from the documented manifest/toolchain process without undocumented local DLL/lib/tool copies.

Content compilation cannot depend on a random utility discovered first on the user's shell PATH.

## 27. Explicit Non-Goals

This toolchain does not require a custom compiler, custom build system, vendoring every dependency manually, a game-engine package manager, a general gameplay scripting-language dependency, baseline networking dependencies, player-facing Dear ImGui UI, raw authoring asset parsing as the normal shipping content path, or unpinned external content tools.