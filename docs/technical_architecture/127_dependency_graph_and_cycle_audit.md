# TA-15 — Dependency Graph and Cycle Audit

> **Status:** Architecture Complete  
> **Authority:** Final dependency-direction, interface-boundary, compile-time/runtime cycle, and initialization/shutdown-order audit before TA-16 target locking

## 1. Purpose

TA-15 must prove that the architecture can be decomposed into build targets/modules without requiring circular ownership or broad backend leakage.

This document defines the dependency constraints TA-16 must preserve when it chooses exact CMake target names.

## 2. Dependency Classes

Dependencies are classified as:

- **foundational value dependency** — IDs, math, Result/error, ticks, immutable DTO/value types;
- **interface dependency** — a project-owned abstract/narrow service contract;
- **domain dependency** — one gameplay domain consumes another domain's public query/command/event contract;
- **composition dependency** — `app` constructs/wires concrete implementations;
- **dev/test dependency** — tools/tests consume public/dev interfaces;
- **backend implementation dependency** — adapter target depends on a third-party library.

A runtime callback/event relationship does not automatically imply compile-time mutual ownership.

## 3. Canonical Build-Level Shape

TA-16 should preserve a shape equivalent to:

```text
core
├── platform_interface / platform_glfw
├── content_model / content_runtime / content_tools
├── simulation
├── physics_interface / physics_jolt
├── navigation_interface / navigation_recast / navigation_3d
├── render_interface / render_opengl
├── audio_interface / audio_miniaudio
├── text / ui_primitives
└── persistence_primitives

simulation + content_model + project interfaces
        ↓
world
        ↓
gameplay domain targets
        ↓
read-model / presentation contracts
        ↓
ui / audio-presentation / render-submission

persistence
↔ domain Save DTO codec registration through narrow interfaces

app
└── concrete composition root over runtime targets

tools/tests
└── depend outward on project interfaces/runtime modules; shipping runtime never depends on tools/tests
```

Exact target splitting/spelling remains TA-16-owned.

## 4. Core Rule

`core` must not depend on gameplay, renderer, physics backend, UI, persistence service, or content tooling.

**PASS:** current architecture only places foundational primitives there.

## 5. Platform Rule

Platform owns GLFW/OS interaction and profiling clock/path services.

Gameplay/UI may consume project-owned platform/input abstractions, not GLFW objects directly.

**PASS.**

## 6. Content Rule

Runtime content data/types may be consumed by world/gameplay/render/physics/navigation/audio adapters through project-owned content structures/IDs.

Offline content tooling may depend on fastgltf/KTX/meshoptimizer/glslang/Recast cooking tools; shipping gameplay must not depend on authoring-tool representations.

**PASS.**

## 7. Simulation Rule

Simulation infrastructure may know generic command/transaction/tick/event machinery but not domain-specific game rules.

Gameplay domains register/participate through narrow interfaces.

**PASS:** avoids `simulation ↔ gameplay` rule ownership cycle.

## 8. World Rule

`world` owns active local representation/registry/scene activation and can depend on project interfaces for physics/navigation/content.

It does not own persistent domain semantics.

Gameplay may reference runtime handles/world query services without transferring persistent ownership.

**PASS.**

## 9. Gameplay Domain Rule

Gameplay domains can depend on shared domain contracts but must not expose writable stores to each other.

Cross-domain mutation uses Commands/Transactions/Events rather than mutual mutable references.

**PASS:** semantic cycles are broken by transaction coordination and typed facts.

## 10. Physics Rule

`physics_jolt` depends on `physics_interface` + Jolt.

Gameplay/world depend on project physics contracts/value facts, never `physics_jolt` internals.

**PASS.**

## 11. Navigation Rule

Recast/Detour and project 3D-navigation backends implement StarForge navigation contracts.

AI/gameplay own authorization/knowledge/action logic and consume route results.

**PASS:** `navigation backend ↔ AI policy` ownership cycle is not required.

## 12. Renderer Rule

`render_opengl` depends on render contracts/content GPU-ready products/OpenGL platform context boundary.

Gameplay/world/UI submit immutable presentation data and do not depend on raw renderer resources for authoritative behavior.

**PASS.**

## 13. Audio Rule

`audio_miniaudio` implements audio presentation services.

Gameplay emits semantic cues and AI hearing consumes GameplaySoundEvents separately.

**PASS:** gameplay does not depend on mixed voice output.

## 14. UI/Text Rule

UI consumes immutable read models/content/text services and emits Commands/intents.

Gameplay does not depend on concrete widgets/layout tree.

**PASS:** no UI↔gameplay mutable dependency cycle.

## 15. Persistence Rule

Persistence service depends on serialization primitives and registered domain DTO/codecs.

Gameplay domains own DTO meaning but do not call filesystem/container internals to mutate themselves.

Composition can register codecs/exporters/importers without making persistence a parent of all gameplay types.

**PASS.**

## 16. Tools Rule

`tools` may depend on many project interfaces in development builds.

Shipping/gameplay modules never depend on Dear ImGui or tooling implementations.

**PASS.**

## 17. Tests Rule

Tests may depend on targets under test plus fake/null adapters.

Production runtime cannot depend on Catch2/test fixtures.

**PASS.**

## 18. App Rule

`app` sits at the top of the dependency graph and performs concrete wiring/startup/shutdown/main-loop orchestration.

No lower module may depend on `app` to access another service.

**PASS.**

## 19. Initialization Order

A dependency-safe initial startup order is realizable:

1. low-level diagnostics/config/platform paths;
2. content registry/toolchain-compatible runtime metadata;
3. platform/window/context where applicable;
4. project service interfaces/workers;
5. renderer/physics/navigation/audio backends as required by selected application mode;
6. persistence/catalog services;
7. session/domain stores;
8. world/scene/runtime systems;
9. UI/presentation;
10. gameplay session activation;
11. developer tooling.

Headless test composition omits presentation/platform backends as defined.

**PASS.**

## 20. Shutdown Order

Shutdown reverses ownership safely:

1. stop accepting new gameplay/application work;
2. finish/cancel bounded async jobs according to owner policy;
3. reach valid session/save decision boundary if applicable;
4. retire active scene/runtime entities;
5. destroy physics/navigation/render/audio proxies/resources while backends still exist;
6. stop persistence/content workers/services;
7. destroy backends/context/window/platform;
8. flush final diagnostics.

No backend must be destroyed before objects it owns are retired.

**PASS.**

## 21. Runtime Event Cycles

Runtime event chains may form logical feedback over **later deterministic phases/ticks**, for example damage → station fault → automation task → repair command.

This is not a synchronous recursive ownership cycle because:

- committed facts cannot be vetoed retroactively;
- follow-up commands execute through defined phase boundaries;
- recursive unbounded same-tick chains are prohibited/diagnosed.

**PASS.**

## 22. Save/Load Dependency Cycle

Save/load does not require active backend memory as serialized truth.

Backends reconstruct from staged persistent state after load.

**PASS:** persistence is not cyclically dependent on live renderer/physics/audio object graphs.

## 23. Content/Runtime Dependency Cycle

Content Registry resolves immutable definitions/products; runtime may request residency but does not mutate canonical content identity.

Development hot reload publishes a new validated generation rather than runtime mutating source definitions in place.

**PASS.**

## 24. Performance/Diagnostics Dependency Cycle

Profiler/logging/CI instrumentation consumes runtime metrics but does not decide gameplay outcomes.

**PASS:** observability cannot create a semantic cycle back into simulation.

## 25. Forbidden Dependency Examples

TA-16 must reject target/include relationships equivalent to:

- `core -> gameplay`;
- `gameplay -> app`;
- `gameplay -> Dear ImGui`;
- `gameplay -> raw OpenGL/Jolt/miniaudio/Recast types`;
- `physics_jolt -> gameplay domain stores`;
- `render_opengl -> gameplay mutable stores`;
- `ui -> gameplay mutable containers`;
- `persistence filesystem writer -> runtime entity handles`;
- `shipping runtime -> tests`;
- `content authoring tool internals -> campaign save identity`.

## 26. TA-14 Mechanical Enforcement

TA-14 allows dependency/include hygiene checks and forbidden-backend-leak linting to encode this graph in CI once source targets exist.

TA-16 must map these rules to concrete target/link/include boundaries.

## 27. Verdict

```text
Build-level DAG realizable: PASS
Semantic ownership DAG realizable: PASS
Initialization ordering: PASS
Shutdown ordering: PASS
Backend encapsulation: PASS
Persistence reconstruction direction: PASS
Runtime event feedback bounded: PASS
Required compile-time circular dependency: NONE
Required semantic ownership cycle: NONE
Blocking finding: NONE
```
