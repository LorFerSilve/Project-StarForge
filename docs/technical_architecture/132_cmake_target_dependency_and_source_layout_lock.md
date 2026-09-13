# TA-16 — CMake Target, Dependency, and Source Layout Lock

> **Status:** Implementation Locked  
> **Baseline:** `TA16-V1`  
> **Authority:** Concrete build-target names, source ownership, link direction, and initial repository layout

## 1. Purpose

TA-15 proved that the architecture is realizable as a directed build graph. TA-16 fixes the concrete names and layering that implementation must preserve.

Targets may be split further for build-time reasons only when the new graph preserves the same authority/dependency direction. Merging targets across forbidden boundaries requires architecture change control.

## 2. Naming Rules

Project CMake targets use lowercase `sf_` names for libraries and `starforge-*` for executable products. Public alias targets use `StarForge::<Name>` where useful.

Third-party targets are linked only from their owning adapter/tool target.

## 3. Locked Production Target Set

### Foundation

```text
sf_core
sf_diagnostics
sf_platform
sf_platform_glfw
sf_content_model
sf_content_runtime
sf_simulation
sf_transactions
sf_read_models
sf_persistence_primitives
```

### Backend interfaces and adapters

```text
sf_physics
sf_physics_jolt
sf_navigation
sf_navigation_recast
sf_navigation_3d
sf_render
sf_render_opengl
sf_audio
sf_audio_miniaudio
sf_text
```

### World and gameplay domains

```text
sf_world
sf_station
sf_gameplay_common
sf_player
sf_combat
sf_crew
sf_robots
sf_spacecraft
sf_economy
sf_progression
sf_ai
sf_missions
sf_strategic
```

### Presentation / persistence / tools

```text
sf_ui
sf_presentation
sf_persistence
sf_devtools
sf_content_pipeline
```

### Executables

```text
starforge
starforge-headless
starforge-content
```

The full target set is locked now even though targets are created incrementally by their implementation phase. A name becoming listed here does not mean empty placeholder source should be created immediately.

## 4. Locked Test Target Set

Canonical test executable groups are:

```text
starforge_tests_unit
starforge_tests_domain
starforge_tests_headless
starforge_tests_scenarios
starforge_tests_backend
starforge_tests_content
starforge_tests_persistence
starforge_tests_performance
```

Sanitizers are build/preset properties applied to real test targets, not a fake standalone test executable.

## 5. Foundational Dependency Direction

The production graph follows this lower-to-higher direction:

```text
sf_core
  ↓
sf_diagnostics / sf_platform / sf_content_model /
sf_persistence_primitives / backend interfaces
  ↓
sf_simulation / sf_transactions / sf_content_runtime
  ↓
sf_world
  ↓
gameplay domains
  ↓
sf_read_models
  ↓
sf_ui / sf_presentation / backend submission
  ↓
application composition roots
```

A downward target may not link back to a higher target.

## 6. Concrete Backend Edges

Allowed backend implementation edges include:

```text
sf_platform_glfw      -> sf_platform + GLFW
sf_physics_jolt       -> sf_physics + Jolt
sf_navigation_recast  -> sf_navigation + Recast/Detour
sf_navigation_3d      -> sf_navigation + project-owned algorithms
sf_render_opengl      -> sf_render + sf_platform + glad2 + OpenGL + GLM
sf_audio_miniaudio    -> sf_audio + miniaudio
sf_text               -> FreeType + HarfBuzz + project text contracts
sf_content_pipeline   -> sf_content_model + fastgltf + KTX + meshoptimizer + glslang + Recast cooking
sf_devtools           -> project dev interfaces + Dear ImGui
```

No gameplay-domain target links raw Jolt/OpenGL/GLFW/Recast/miniaudio/ImGui solely for convenience.

## 7. World / Gameplay Edge Rules

`sf_world` owns active-scene/runtime representation and may consume project physics/navigation/content interfaces. It does not own persistent gameplay truth.

Gameplay domain targets may depend on:

- `sf_core`;
- `sf_simulation`;
- `sf_transactions`;
- `sf_content_model`/approved runtime definition interfaces;
- narrow world/backend interfaces;
- explicit shared domain contracts.

They may not expose writable stores as a cycle-breaking mechanism.

## 8. Domain Dependency Rule

Cross-domain mutation is not implemented by mutual target linkage plus writable references. It uses the TA-2 Command/Validate/Prepared Transaction/Atomic Commit/Committed Event contracts.

Where two domain targets need shared value contracts, those values move into the narrowest lower common contract target instead of introducing a target cycle.

## 9. Persistence Direction

`sf_persistence` owns storage/container/catalog/staging orchestration and depends on persistence primitives plus registered project-owned DTO codec interfaces.

Domain targets own Save DTO meaning. They do not link filesystem/container internals merely to save themselves.

No persistence target serializes runtime/backend handles.

## 10. App Composition Roots

`starforge` is the shipping graphical composition root.

`starforge-headless` is the headless composition root used by deterministic scenarios, tools and CI where no window/OpenGL/audio device is required.

`starforge-content` is the offline content validation/cook/build executable.

Lower targets never link to an application target as a service locator.

## 11. Source Layout Lock

The implementation source tree is:

```text
/
├── CMakeLists.txt
├── CMakePresets.json
├── vcpkg.json
├── cmake/
│   ├── StarForgeProjectOptions.cmake
│   ├── StarForgeWarnings.cmake
│   └── ... narrowly scoped build modules
├── include/starforge/
│   ├── core/
│   ├── diagnostics/
│   ├── platform/
│   ├── content/
│   ├── simulation/
│   ├── physics/
│   ├── navigation/
│   ├── world/
│   ├── gameplay/
│   ├── render/
│   ├── audio/
│   ├── text/
│   ├── ui/
│   └── persistence/
├── src/
│   └── <matching owned implementations>/
├── apps/
│   ├── game/
│   └── headless/
├── tools/
│   └── content/
├── tests/
│   ├── unit/
│   ├── domain/
│   ├── headless/
│   ├── scenarios/
│   ├── backend/
│   ├── content/
│   ├── persistence/
│   ├── performance/
│   └── fixtures/
├── content/
├── shaders/
├── third_party/generated/glad/
├── docs/
└── .github/workflows/
```

Directories are created when they receive real owned files; the repository does not need empty-directory scaffolding.

## 12. Include Boundary Rule

Public headers live under `include/starforge/<module>/` only when another target needs the contract. Private implementation headers stay beside/in the owning `src` subtree.

`target_include_directories` visibility must reflect that distinction. `PUBLIC` is not a convenience default.

## 13. CMake Ownership Rule

Every production/test executable or library has exactly one owning `CMakeLists.txt` near its source subtree. Root CMake composes; it does not accumulate every source file in one monolithic list.

Cross-target compile definitions/options are centralized in narrow project option functions, not copied inconsistently.

## 14. Link Visibility Rule

- `PRIVATE` is the default for backend/implementation dependencies;
- `PUBLIC` is used only when a dependency's project-owned public type appears in the consumer-visible contract;
- `INTERFACE` is used only for genuine header-only/policy contracts, never to hide an unimplemented subsystem behind an empty target.

## 15. Forbidden Edges

The locked graph rejects relationships equivalent to:

```text
sf_core -> gameplay
sf_simulation -> domain-specific rule implementation
sf_player/sf_combat/... -> sf_render_opengl
sf_gameplay_* -> Jolt/GLFW/OpenGL/miniaudio/Recast raw targets
sf_ui -> writable gameplay stores
sf_persistence -> RuntimeEntityHandle/Jolt/OpenGL objects
sf_physics_jolt -> gameplay stores
shipping target -> Catch2
shipping target -> sf_devtools in Release
lower target -> starforge app executable
```

## 16. Bootstrap Materialization

TA-16 materializes only these real initial targets:

```text
sf_core
starforge-headless
starforge_tests_unit
```

They prove C++23 compilation, target linkage, Catch2 discovery, headless execution and CI integration. All later targets enter when their implementation phase starts and must conform to this locked target registry.

## 17. Architecture Boundary Enforcement

Implementation adds mechanical checks over time for:

- forbidden third-party includes outside adapter owners;
- target dependency direction;
- shipping target dependency on tests/devtools;
- public-header leakage;
- OpenGL calls outside renderer/resource code;
- persistent DTO references to runtime/backend types.

Until automated lint covers an edge, code review enforces the same lock.

## 18. Lock Verdict

```text
Concrete target registry: LOCKED
Source tree: LOCKED
Application roots: LOCKED
Backend ownership: LOCKED
Dependency direction: LOCKED
Forbidden cycles/leaks: LOCKED
Initial materialized targets: sf_core + starforge-headless + starforge_tests_unit
Empty placeholder targets: PROHIBITED
```
