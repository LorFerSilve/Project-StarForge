# Graphics Settings, Resize, and Failure Recovery

> **Status:** Architecture Complete  
> **Authority:** Shipping graphics-quality configuration, render scale, resolution/vsync, feature tiers, accessibility interaction, resize/minimize behavior, shader/framebuffer failure, context failure, and renderer recovery policy

## 1. Purpose

Graphics settings must scale presentation cost without changing gameplay rules, visibility knowledge, collision, threat, loot, or simulation timing.

## 2. Quality Presets

Baseline presets:

- Low;
- Medium;
- High;
- Ultra;
- Custom.

A preset is only a group of documented renderer settings. Selecting one does not create a different gameplay mode.

## 3. Preset Baseline

`High` is the visual reference configuration for content validation.

`Medium` is the performance-oriented default candidate for unknown hardware until an initial capability/benchmark policy is defined during implementation planning.

The implementation may select another first-launch preset from measured hardware capability, but all presets preserve the same gameplay semantics.

## 4. User-Exposed Core Settings

Baseline graphics/display settings include:

- display mode (Windowed / Borderless / Fullscreen where platform supports);
- output resolution;
- VSync Off/On;
- render scale;
- shadow quality;
- texture quality/residency budget;
- anisotropic filtering where supported;
- SSAO Off/Low/High;
- bloom amount/enable;
- effect/particle quality;
- LOD/view-distance quality;
- anti-aliasing Off/FXAA;
- gamma/brightness calibration;
- FOV through camera/accessibility settings;
- Reduced Effects / Photosensitivity integration through accessibility.

## 5. Render Scale

3D world rendering supports a user/configuration render scale independent of shipping UI resolution.

Baseline supported target range is 50%-100% of framebuffer dimensions.

UI remains at native output resolution.

Values outside that range require later validation/architecture amendment.

## 6. Dynamic Resolution

Automatic dynamic-resolution scaling is not part of the baseline.

Frame pacing is therefore not allowed to silently change render scale during gameplay.

It can be added later through an explicit architecture decision if profiling demonstrates need.

## 7. Texture Quality

Texture quality controls permitted mip/residency budget and initial loaded detail; it does not substitute different gameplay geometry or alter scanner/interaction knowledge.

Missing high-detail mip levels must degrade visual detail gracefully.

## 8. LOD / View Distance

LOD/view-distance settings adjust thresholds for approved render-only LOD/decorative distance policy.

They cannot hide collision-relevant/gameplay-critical structures at distances where those objects remain relevant.

## 9. Shadow Quality

Shadow quality can control:

- directional cascade resolution;
- cascade distance distribution;
- local shadow atlas/cubemap resolution;
- number/budget of shadowed local lights;
- filtering radius/quality.

Lower shadow quality does not disable the actual light or gameplay visibility state.

## 10. SSAO Quality

SSAO is presentation-only and can be disabled completely.

No gameplay affordance may rely on SSAO to distinguish geometry or hazards.

## 11. Bloom

Bloom can be disabled/reduced.

The renderer cannot encode a critical state solely as bloom.

## 12. Particle / Effect Quality

Effect quality controls decorative density and optional detail.

It obeys the TA-4 critical-effect floor and GDS-13 Reduced Effects/Photosensitivity requirements.

## 13. Anti-Aliasing

Initial settings:

- Off;
- FXAA.

No save/gameplay state depends on the selection.

## 14. Motion Blur

The baseline renderer has no motion-blur implementation.

The settings UI therefore does not expose a nonfunctional motion-blur toggle in the first implementation.

## 15. Gamma / Brightness

Output calibration adjusts presentation mapping only.

It cannot modify actual light gameplay/system state.

Calibration UI uses reference images/patterns consistent with GDS-13 accessibility.

## 16. Accessibility Precedence

Accessibility modes can override/reduce unsafe presentation even if the selected graphics preset would otherwise enable it.

Examples:

- Photosensitivity Safe limits flash intensity/rate;
- Reduced Effects lowers effect density/distortion;
- Camera Shake/Head Bob belong to camera/presentation settings and can be 0%;
- High Contrast/interaction highlights remain knowledge-safe.

Quality preset never disables an accessibility invariant.

## 17. Difficulty Separation

Graphics settings are independent of GDS-12 Difficulty.

Changing graphics quality does not change:

- enemy AI;
- incoming damage;
- resource yields;
- rewards;
- progression;
- hazard timing.

## 18. Framebuffer Resize Event

GLFW framebuffer-size changes are consumed by the render/platform boundary.

When width/height are valid and changed:

1. increment framebuffer/render-target generation;
2. wait only for required safe GPU retirement boundary;
3. invalidate incompatible frame targets;
4. recreate targets for new internal render extent;
5. update camera aspect/viewport;
6. reset incompatible temporal/history resources;
7. resume normal rendering.

## 19. Minimized / Zero Extent

When framebuffer extent is zero:

- no render target allocation occurs;
- render execution/present can be suspended;
- input/window events continue as platform permits;
- simulation pause policy is not implicitly changed by renderer.

When non-zero extent returns, resources are rebuilt.

## 20. Fullscreen / Display Mode Change

Display-mode changes are treated as controlled platform/render reconfiguration.

The game preserves authoritative session state throughout.

Failure to apply a requested display mode falls back to the last valid supported mode or a safe windowed configuration with a clear diagnostic/user notification.

## 21. VSync

Baseline VSync supports:

- Off (`swap interval 0` where supported);
- On (`swap interval 1`).

Adaptive/driver-specific modes are not required.

VSync changes presentation cadence only; fixed simulation remains 60 Hz.

## 22. Frame Limiting Boundary

A presentation frame limiter can be added in App/platform runtime without changing authoritative tick frequency.

Exact frame-cap options are implementation-roadmap policy rather than TA-4 gameplay architecture.

## 23. Shader Compile/Link Failure

For mandatory startup/base-path shader failure:

- renderer reports full diagnostic;
- affected render configuration is invalid;
- game does not continue into invisible/undefined mandatory scene rendering.

For development hot reload failure:

- keep previous valid program;
- show diagnostic;
- do not interrupt authoritative gameplay.

## 24. Optional Feature Failure

If an optional pass/resource such as SSAO or bloom fails to initialize while the base render path remains valid:

- disable that feature for the session/configuration;
- record diagnostic;
- notify user only when necessary;
- do not alter gameplay state.

## 25. Mandatory Render Target Failure

Failure to create required depth/G-buffer/HDR/output resources triggers a typed renderer failure.

The application may attempt one safe lower-cost fallback configuration if predeclared and semantically equivalent, such as reduced render scale/resolution.

If mandatory rendering still cannot initialize, session does not enter active gameplay.

## 26. Safe Fallback Configuration

Baseline emergency render fallback can reduce only presentation cost:

- render scale;
- optional SSAO;
- bloom;
- shadow resolution/budget;
- decorative effects.

It cannot:

- remove mandatory geometry;
- disable UI;
- alter gameplay collision;
- reveal/hide knowledge;
- change simulation.

## 27. GPU/Driver Debug Errors

Development builds enable OpenGL debug output and classify:

- notification/performance;
- warning;
- error;
- fatal/invariant-breaking.

Known benign driver notifications can be filtered at the logging layer, not by globally disabling debug output.

## 28. Context Creation Failure

If OpenGL 4.6 Core context creation fails at startup:

- show a platform-level actionable error;
- do not silently fall back to deprecated compatibility OpenGL;
- do not enter gameplay.

Required GPU/driver capability is explicit project baseline.

## 29. Runtime Context Failure

Irrecoverable runtime context loss/driver reset is treated as render-fatal in the initial architecture.

The application attempts a controlled save only if a valid Stable Save Boundary and functioning persistence path are available without relying on broken graphics state.

No guarantee of transparent same-session context reconstruction is made.

## 30. Graphics Settings Persistence

Graphics/display/accessibility presentation preferences are profile/application configuration, separate from authoritative campaign Save DTO state where practical.

Changing them cannot migrate or invalidate a campaign save.

## 31. Startup Validation

Renderer startup validates at minimum:

- OpenGL version/profile;
- required core capabilities/limits;
- required framebuffer formats;
- GLSL shader baseline;
- required UBO/SSBO/image/compute limits for configured renderer path;
- debug support in development where available.

Insufficient mandatory capability fails clearly before gameplay.

## 32. Hardware Limits

Architecture queries runtime OpenGL limits rather than assuming desktop-maximum values.

Renderer validates configured:

- texture sizes;
- UBO/SSBO sizes;
- texture units;
- compute workgroup limits;
- framebuffer attachments;
- array/cubemap limits.

Content/configuration that exceeds required minimum baseline is rejected or downconfigured through explicit safe policy.

## 33. Performance Targets Boundary

TA-4 defines scalable rendering mechanisms but does not yet freeze a shipping minimum GPU or universal FPS target.

TA-13 Performance/Concurrency and implementation roadmap own measured budgets/hardware targets after representative scenes exist.

## 34. Benchmark Scene Requirement

Before graphics tiers are Implementation Locked, representative benchmark scenes must include at least:

- dense Horizon/interior lighting;
- surface mission exterior;
- local spaceflight with ship/VFX;
- combat/raid effect density.

Preset budgets are tuned against those cases rather than empty test rooms.

## 35. Test Requirements

Validation must cover:

- quality preset changes preserve gameplay read-state;
- render scale changes keep UI native resolution;
- resize/minimize restoration;
- VSync does not change simulation rate;
- optional pass failure fallback;
- mandatory target failure prevents invalid gameplay presentation;
- accessibility overrides quality safely;
- Low preset preserves critical effect/geometry readability;
- context startup capability gate.

## 36. Explicit Non-Goals

No baseline requirement for:

- automatic dynamic resolution;
- native HDR display output;
- adaptive VSync;
- motion blur;
- fallback to deprecated OpenGL;
- transparent runtime context-loss recovery;
- graphics settings affecting gameplay/difficulty.

## 37. Dependencies

Depends on TA-1 platform/runtime, TA-3 scene/profile/streaming, all TA-4 renderer specs, GDS-13 Accessibility/Visual Direction, and later TA-10 content budgets/TA-13 performance measurement/TA-11 shipping settings UI.

## 38. Open Questions

None in the graphics settings/resize/failure architecture.
