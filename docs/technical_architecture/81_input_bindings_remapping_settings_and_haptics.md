# TA-11 — Input Bindings, Remapping, Settings, and Haptics

> **Status:** Architecture Complete  
> **Authority:** Binding profiles, conflict validation, controller calibration, input/accessibility preferences, settings ownership, profile persistence boundary, prompt glyph resolution, and haptic output contracts

## 1. Purpose

StarForge requires fully remappable normal gameplay controls and accessibility/input preferences that survive application restarts without being confused with campaign/world progression.

This specification defines the settings/data model around the semantic action system from `80_input_device_action_and_context_routing.md`.

## 2. Settings Ownership Classes

Settings are separated by authority:

1. **Application/Profile Presentation Settings** — controls, accessibility, HUD, audio, display and related preferences;
2. **Session-Transient Presentation State** — current focus, current prompt device, open panels, pointer position, temporary previews;
3. **Save-Owned Gameplay Settings** — only settings explicitly defined by the GDS as world/save gameplay state, such as Difficulty where applicable.

TA-11 does not serialize any category itself; TA-12 defines exact persistence implementation.

## 3. InputProfile

A profile-level `InputProfile` conceptually contains:

- keyboard/mouse bindings;
- controller bindings;
- mouse sensitivities;
- controller sensitivities;
- stick deadzones/response parameters;
- trigger thresholds;
- axis inversion;
- Hold/Toggle preferences;
- Auto-Sprint preference;
- Aim Assist preference/intensity where allowed;
- sustained-interaction accessibility preference;
- haptic enable/intensity;
- prompt/glyph presentation preferences where applicable.

No world entity identity is stored in this profile.

## 4. Binding Record

A binding record contains:

- `ActionId`;
- device family;
- physical input code/axis;
- optional modifier/chord only where the action registry explicitly permits one;
- applicable input context set;
- analog transform metadata when relevant.

A binding cannot embed executable gameplay logic.

## 5. Default Bindings

Project defaults implement the GDS keyboard/mouse and Xbox-position controller mapping.

Defaults are versioned project data so a future default change does not silently overwrite a player's customized profile.

`Reset to Default` intentionally replaces the selected device family's user bindings with the current supported default set.

## 6. Rebinding Flow

The shipping controls screen uses an explicit capture state:

`Idle -> AwaitingInput -> Candidate -> ConflictReview/Accepted -> Applied`.

During `AwaitingInput`:

- gameplay action routing is suppressed by UI context;
- the next eligible physical input becomes a candidate;
- reserved platform/system combinations can be rejected with explanation;
- cancel exits without changing the prior binding.

## 7. Conflict Classification

Binding validation classifies conflicts as:

- `HardConflict` — same physical input maps to incompatible actions in simultaneously active contexts;
- `ContextualReuse` — same physical input is reused only across mutually exclusive contexts;
- `EssentialActionUnbound` — a required action would become unavailable for that device family;
- `AllowedSharedBinding` — explicitly authored shared semantic behavior.

Hard conflicts require resolution before the new binding becomes effective unless the GDS explicitly permits the exact coexistence.

## 8. Essential Actions

Essential action sets are context/device aware.

They include enough controls to:

- navigate/cancel/confirm UI;
- pause/return from gameplay;
- move/look/interact in ordinary gameplay;
- safely leave specialized contexts where their GDS requires direct control.

The binding UI cannot trap the player with no valid Back/Cancel path.

## 9. Apply Semantics

A validated binding change applies atomically to the `InputProfile` presentation/settings state.

If a change would invalidate the current UI navigation path, the controls screen keeps a temporary guaranteed fallback for Confirm/Cancel until the screen leaves capture/review state.

The new mapping never retroactively reinterprets an already sampled simulation-tick action.

## 10. Prompt Glyph Resolution

Prompts use semantic `ActionId`, never hard-coded key labels.

The prompt service resolves:

`ActionId + current InputContext + PromptDeviceFamily -> display glyph/text`.

Therefore tutorials, interaction prompts, and HUD always show the currently effective binding.

## 11. Controller Glyph Families

Controller prompts use position/semantic mapping rather than assuming one brand label.

The baseline supports an Xbox-style position family because GDS defaults use it, while the presentation layer can supply additional glyph packs later without changing action semantics.

If no dedicated glyph exists, a readable textual fallback is mandatory.

## 12. Device Family Anti-Flicker

`PromptDeviceFamily` changes only after a meaningful input from another family passes the tuneable anti-flicker threshold/time policy.

Stick noise below configured deadzone, controller telemetry jitter, or incidental mouse movement below a small presentation threshold does not replace prompts.

## 13. Mouse Sensitivity

Separate profile values exist for:

- first-person look;
- ADS/scoped look;
- spacecraft control/camera;
- construction/strategic camera.

Values are transformed through documented units/curves so the displayed settings remain stable across frame rate.

Mouse acceleration remains Off by default.

## 14. Controller Calibration

Controller calibration exposes:

- left-stick inner deadzone;
- left-stick outer normalization;
- right-stick inner deadzone;
- right-stick outer normalization;
- trigger threshold;
- per-context sensitivity;
- horizontal/vertical inversion where supported.

Settings UI can display live raw and transformed axis diagnostics without becoming gameplay authority.

## 15. Safe Parameter Bounds

Every numeric input setting has technical safe bounds.

The UI clamps/rejects values outside those bounds and exposes the actual effective value.

TA-13 may tune default ranges/performance implications but may not remove required user configurability without architecture/design change control.

## 16. Hold/Toggle Preferences

The profile stores per-action Hold/Toggle choices only for actions declared toggle-safe.

At minimum the baseline supports:

- Sprint;
- Crouch;
- Aim Down Sights;
- Scan Mode where applicable.

The preference changes semantic input state production, not owning gameplay restrictions.

## 17. Repeated/Sustained Input Accessibility

The profile stores the GDS press-to-start/press-to-stop alternative for eligible sustained interactions.

No profile option can transform precision aiming/movement into automated gameplay beyond the documented accessibility envelope.

## 18. Auto-Sprint Preference

Auto-Sprint is configurable independently of difficulty.

The setting controls whether input assistance emits Sprint intent after valid sustained forward input. It cannot change Movement restrictions or consume resources differently.

## 19. Aim Assist Settings

Aim Assist exposes:

- Off;
- Low;
- Standard;
- High.

Controller may default to an approved non-Off value according to later tuning; mouse defaults to Off.

The setting controls bounded input-camera assistance coefficients only. Hit resolution remains TA-5/GDS-9 authority.

## 20. Accessibility Presets

Accessibility presets such as Reduced Motion, High Readability, Hearing Support, and Photosensitivity Safe are implemented as named bundles of normal individual settings.

Applying a preset:

- writes the documented member settings;
- does not lock them;
- does not alter Difficulty;
- does not reset campaign state.

Every setting remains independently editable afterward.

## 21. Settings Transaction Model

Settings changes use a small project-owned settings service rather than direct widget mutation of global variables.

Conceptual flow:

`Settings UI -> SettingsChangeRequest -> validate -> apply to candidate -> publish new SettingsSnapshot`.

Settings that need confirmation, such as disruptive display changes, use an explicit preview/revert transaction owned by the relevant subsystem.

## 22. Immediate vs Deferred Settings

Examples of settings that can normally apply immediately:

- UI scale;
- subtitle size/background;
- audio volumes;
- reticle presentation;
- input sensitivity;
- haptic intensity.

Settings requiring subsystem recreation or safe-boundary transition expose an explicit `ApplyRequiresRestart` or `ApplyAtSafeBoundary` result rather than silently applying half-state.

## 23. Profile Persistence Boundary

TA-11 defines logical profile settings but not on-disk format.

TA-12 must persist profile/application preferences separately from world-save domain sections where appropriate.

A campaign save can be loaded under a different control/accessibility profile without changing world state.

## 24. Profile Compatibility

Profile settings are versioned separately from save gameplay schemas.

If a setting is added in a later build:

- missing profile field receives project default;
- invalid/out-of-range values are replaced with validated defaults and diagnosed;
- unknown future fields follow TA-12 profile-version policy;
- world save is not invalidated merely because a presentation setting changed.

## 25. Haptics Boundary

Haptics are presentation only.

Gameplay/interaction systems emit semantic `HapticCue` requests after appropriate authoritative facts/commits, containing conceptually:

- cue kind;
- intensity envelope;
- duration;
- optional left/right/low/high-frequency emphasis where backend supports it;
- priority;
- correlation/source identity for deduplication.

No gameplay mechanic reads haptic output state.

## 26. Haptic Categories

Baseline semantic categories include:

- weapon/action;
- impact/damage;
- machinery/environment;
- docking/vehicle;
- warning/alarm;
- UI confirmation where deliberately used.

The profile has master enable and 0–100% intensity. Optional category controls can be added without changing gameplay semantics.

## 27. Windows Baseline Haptics Adapter

The initial Windows x64 implementation uses a StarForge-owned `IHapticsBackend` with a platform adapter capable of controller vibration on supported devices.

For XInput-compatible controllers, Windows XInput vibration is an acceptable baseline backend behind that interface.

The input path remains GLFW-based; haptics do not take over device/action discovery.

Unsupported controllers simply report no haptic capability.

## 28. Haptic Capability Discovery

Each active controller exposes presentation capability flags such as:

- rumble supported;
- independent motors/channels supported;
- trigger haptics supported where a future backend provides them.

Game code requests semantic cues and never assumes a specific motor layout.

## 29. Haptic Mixer

The haptic subsystem combines simultaneous cues by explicit priority/envelope policy.

Critical warning haptics may pre-empt decorative cues, but haptics are never the only critical channel.

Numeric simultaneous-cue limits belong to TA-13.

## 30. True Pause and Haptics

When True Pause begins:

- gameplay-loop haptic cues stop/fade promptly;
- no repeating gameplay haptic timeline continues to imply advancing incidents;
- optional menu/UI confirmation haptics may continue because they are presentation interaction;
- resume does not replay stale impact/fire pulses.

## 31. Mute/Disable Semantics

Disabling haptics or setting intensity to zero has no effect on:

- damage;
- recoil/spread;
- AI sensing;
- docking;
- alarms;
- objective state;
- rewards.

All critical information has a non-haptic route.

## 32. Settings Read Models

UI consumes immutable:

- `InputSettingsReadModel`;
- `AccessibilitySettingsReadModel`;
- `HapticsCapabilityReadModel`;
- `EffectiveBindingsReadModel`.

Widgets never retain writable references to the live settings store.

## 33. Diagnostics

Development tools expose:

- default vs overridden bindings;
- conflict classification;
- effective context-specific mapping;
- raw/transformed controller axes;
- active accessibility preset expansion;
- haptic device capabilities;
- active haptic cues/mixer output.

## 34. Validation Requirements

Tests must cover:

- same-context hard conflict detection;
- mutually exclusive contextual reuse;
- essential action cannot be silently lost;
- reset-to-default affects only selected device family/category;
- prompt glyph follows effective remap;
- profile change cannot mutate world state;
- controller setting transforms are bounded;
- Aim Assist setting cannot change hit tests;
- haptics disabled leaves gameplay identical;
- unsupported haptic device degrades safely.

## 35. Explicit Non-Goals

No baseline requirement for:

- cloud profile synchronization;
- platform-account settings ownership;
- controller-only gameplay rules;
- unremappable ordinary gameplay controls;
- haptics as gameplay input;
- haptic-only critical state;
- arbitrary macros that execute multiple gameplay actions from one binding;
- profile settings embedded in every campaign save.

## 36. Dependencies

Depends on TA-11 input routing, TA-1 platform/runtime, TA-2 read models/commands, TA-5 hit authority, TA-10 content/glyph assets, GDS Controls and Accessibility, and later TA-12 persistence implementation.

## 37. Open Questions

None in the binding/settings/haptics architecture.
