# TA-14 — Backend and Subsystem Smoke / Integration Testing

> **Status:** Architecture Complete  
> **Authority:** Adapter/backend smoke coverage, subsystem integration boundaries, graphical/audio/platform smoke environments, and cross-backend correctness expectations

## 1. Purpose

Headless tests prove most gameplay semantics, but StarForge also depends on real third-party/backend integrations whose failures can only be detected when the adapter is exercised.

This specification defines bounded smoke/integration coverage without turning backend tests into the primary gameplay oracle.

## 2. Backend Principle

A backend smoke test answers:

- can the adapter initialize correctly;
- can project-owned data cross the boundary;
- are lifetime/thread/handle contracts respected;
- does teardown/recreate safely work;
- do unsupported capabilities fail explicitly.

It does not duplicate every gameplay rule through the backend.

## 3. Jolt Physics Smoke

Required Jolt adapter smoke coverage includes:

- world create/destroy;
- static/kinematic/dynamic bodies;
- project-owned PhysicsBodyHandle generation/staleness;
- collision layer/filter expectations;
- ray/sweep/overlap semantic result conversion;
- contact fact extraction;
- constraints/docking setup/teardown;
- CharacterMotor support queries;
- floating-origin shift without synthetic impacts;
- deterministic fixed-step integration fixture within documented tolerances.

Jolt callbacks never directly mutate gameplay state in the test harness either.

## 4. Recast/Detour Smoke

Ground-navigation adapter tests include:

- cook/load tiny nav fixture;
- traversal profile filtering;
- path query;
- dynamic link enable/disable;
- stale generation/revision rejection;
- tile replacement/rebuild handoff;
- teardown/reload.

The test verifies project-owned route results rather than exposing raw backend references as authority.

## 5. 3D Navigation Smoke

The project-owned free-flight navigation layer requires fixtures for:

- bounded graph/volume construction;
- obstacle update;
- route query;
- actor-size/profile filtering;
- stale-result rejection;
- deterministic tie breaking.

## 6. OpenGL Renderer Smoke

A graphical smoke environment initializes a real OpenGL 4.6 Core context and verifies:

- required capability gate;
- glad loading;
- shader compile/link;
- basic buffer/texture/VAO creation;
- G-buffer/depth/HDR framebuffer creation;
- reversed-Z state;
- minimal opaque draw;
- forward transparent draw;
- UI pass draw;
- resize/recreate;
- safe resource retirement;
- clean shutdown.

This test need not certify final image quality.

## 7. Shader Family Smoke

Every mandatory ShaderFamily/required variant must compile/link against the configured OpenGL baseline on supported CI/reference environments where a real driver is available.

Offline glslang success is necessary but not sufficient for final driver compile/link coverage.

## 8. Render Image Tests

A small number of deterministic image tests may validate broad renderer invariants such as:

- depth convention;
- material channel wiring;
- alpha-mask behavior;
- tone-map ordering;
- UI after tone mapping;
- critical accessibility presentation.

Image comparison uses documented tolerance/perceptual metrics and stable controlled fixtures.

Screenshot goldens never become gameplay correctness evidence.

## 9. Driver Variability

Pixel-exact comparison across unrelated GPU vendors/drivers is not required.

Tests distinguish:

- strict project data/state assertions;
- tolerant visual image assertions;
- performance measurements tied to known reference hardware.

## 10. GLFW / Platform Smoke

Platform adapter smoke covers:

- window/context creation;
- framebuffer resize/minimize/restore;
- input event ingestion;
- cursor modes where used;
- clean close/shutdown;
- unsupported OpenGL version failure path.

It does not test gameplay through raw keycodes; semantic input mapping has separate deterministic tests.

## 11. Input Adapter Smoke

Physical-device adapter fixtures verify normalization into project-owned device state:

- key/button edge ingestion;
- mouse motion/wheel;
- controller axes/buttons;
- deadzone/calibration boundaries;
- device connect/disconnect;
- stable semantic mapping handoff.

## 12. FreeType / HarfBuzz Smoke

Text integration tests cover:

- UTF-8 decode path;
- shaping of representative Latin/Arabic-like complex-script fixtures where fonts are licensed/available to tests;
- glyph rasterization;
- fallback/missing glyph diagnostics;
- glyph-atlas insertion/eviction contract;
- scale/DPI changes.

The test asserts layout/shaping invariants without distributing restricted font files.

## 13. miniaudio Smoke

Audio adapter smoke uses a null/test device where possible and a real device only in explicit local/manual environments.

Coverage includes:

- engine/device init/shutdown;
- generation-checked voice lifetime;
- resident/streamed clips;
- bus gain routing;
- device failure -> safe silent degradation;
- callback isolation;
- no gameplay access from callback;
- streamed voice underrun diagnostics.

## 14. AI Hearing Independence

Audio backend tests explicitly prove that muting/virtualizing/presentation voice failure does not alter `GameplaySoundEvent` delivery to AI tests.

## 15. fastgltf Smoke

Importer fixtures verify:

- valid minimal glTF;
- transforms;
- mesh attributes/indices;
- skeleton/skin fixture;
- animation fixture;
- socket metadata;
- invalid/unsupported feature diagnostics;
- deterministic StarForge-owned output ordering.

## 16. KTX2 / Texture Smoke

Texture pipeline tests cover:

- source semantic -> expected color space;
- mip generation path;
- KTX2 output/readback;
- selected desktop compression profile;
- corrupt/unsupported texture diagnostics;
- content fingerprint stability under identical inputs.

## 17. meshoptimizer Smoke

Offline mesh processing fixtures verify deterministic/index-valid optimized output and LOD generation properties required by TA-10.

Optimization output is version-sensitive fixture data and is updated only through intentional dependency/version changes.

## 18. glslang Smoke

Every shader source required by baseline content passes pinned glslang offline validation.

Invalid fixture shaders must fail with useful diagnostics.

## 19. Content Runtime Smoke

A minimal cooked registry/scene fixture verifies:

```text
registry load
-> ContentId resolution
-> CPU asset load
-> physics/nav preparation
-> render preparation
-> Scene Ready
-> activation
```

Required missing content must block/fail according to TA-3/TA-10 rather than silently substitute gameplay geometry.

## 20. Scene Transition Smoke

Integration fixture exercises:

```text
Horizon/interior -> staged destination -> atomic context switch -> destination active
```

and verifies:

- old context remains until destination ready;
- Simulation Time hold semantics;
- identity retention;
- no duplicate actors/resources;
- backend teardown in safe order.

## 21. Persistence Runtime Reconstruction Smoke

A save/load integration test with backend adapters verifies:

- runtime/backend handles absent from save;
- new handles reconstructed;
- same persistent IDs/state restored;
- no synthetic physics impact;
- no duplicate audio/VFX one-shot replay;
- first normal tick is saved tick + 1.

## 22. Station Runtime Projection Smoke

Horizon fixture verifies persistent topology/state projects to:

- collision;
- doors/elevators;
- lights;
- pressure areas;
- docking interfaces;
- interactables.

A persistent topology mutation then updates backend representation only after authoritative commit/deferred phase.

## 23. Backend Failure Injection

Adapters expose test-only controlled failure points for cases such as:

- shader compile failure;
- framebuffer allocation failure;
- missing/corrupt content;
- audio device unavailable;
- nav cook failure;
- physics resource creation failure where recoverable.

Tests assert defined typed failure/fallback behavior and no partial gameplay commit.

## 24. Environment Classification

Backend tests are classified by environment requirement:

- `headless-backend`: can run on ordinary CI without display/audio device;
- `graphics-software`: can run with CI software/virtual display where valid;
- `graphics-reference`: requires supported physical/reference GPU runner;
- `audio-device-manual`: optional hardware-specific diagnostic suite.

TA-14 CI architecture must not pretend a software OpenGL renderer certifies reference GPU performance.

## 25. No Backend Golden Lock-In

Backend output fixtures must not expose internal Jolt/Recast/OpenGL handle values as expected semantic output.

The StarForge adapter contract is the testing boundary.

## 26. Smoke Runtime

Ordinary backend PR smoke suites remain intentionally small and bounded.

Large image/performance/systemic scenarios belong to scheduled/reference workflows.

## 27. Explicit Non-Goals

Backend smoke tests do not require:

- exhaustive GPU-vendor certification on every PR;
- actual speaker output in cloud CI;
- gameplay rules duplicated through render screenshots;
- exposing backend IDs in save/golden contracts.

## 28. Dependencies

Depends on TA-3 through TA-11 adapters/content/runtime architecture, TA-13 budgets, and TA-16's final supported build/reference-runner matrix.

## 29. Open Questions

None within backend/subsystem smoke scope.