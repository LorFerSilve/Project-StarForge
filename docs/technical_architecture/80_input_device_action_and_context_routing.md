# TA-11 — Input Device, Action, and Context Routing

> **Status:** Architecture Complete  
> **Authority:** Raw platform input ingestion, device state, semantic actions, fixed-tick input sampling, context ownership, action consumption, device switching, and the boundary between input intent and gameplay authority

## 1. Purpose

StarForge must accept keyboard, mouse, and controller input across first-person play, spacecraft, construction, terminals, menus, tactical interfaces, restricted states, and True Pause without allowing physical key codes, frame rate, or UI focus to become gameplay authority.

The input architecture therefore converts **physical device state** into **named semantic actions** before any gameplay domain receives intent.

## 2. Authority Boundary

The input module owns:

- device discovery/state;
- raw button/key/axis ingestion;
- binding resolution;
- deadzone/response/inversion transforms;
- semantic action state;
- input-context ownership and consumption;
- prompt-device family selection.

It does **not** own:

- target selection;
- movement legality;
- weapon behavior;
- interaction validity;
- docking validity;
- construction validity;
- robot command validity;
- pause transaction boundaries;
- gameplay state mutation.

Those remain in their owning domains.

## 3. Platform Ingestion

GLFW remains the baseline platform input source for:

- keyboard keys;
- mouse buttons;
- mouse movement;
- scroll wheel;
- connected gamepads/controllers;
- controller buttons and analog axes.

Platform-specific haptics are handled separately under TA-11 presentation/haptics and do not make GLFW responsible for rumble.

## 4. Raw Input State

The platform layer publishes a process-local `RawInputState` containing conceptually:

- current key/button down state;
- press/release edges since the previous simulation consumption point;
- accumulated mouse delta;
- accumulated wheel delta;
- controller axes/buttons;
- device connection/disconnection events;
- monotonic presentation timestamp for device-family UX only.

Raw platform codes never enter gameplay/domain APIs.

## 5. Fixed-Tick Sampling

TA-1 `Input Command Sampling` is the only point at which authoritative gameplay consumes player input.

At the beginning of each simulation tick the input resolver produces an immutable `ActionSample` from the latest raw state and current context stack.

Render frames may ingest input even when they execute no simulation tick, but gameplay commands are emitted only when the next fixed tick samples the action state.

## 6. Edge Latch Is Not a Gameplay Input Buffer

Physical press/release edges that arrive between fixed ticks are retained only until the next `Input Command Sampling` so a short press is not lost merely because render/platform polling ran faster than simulation.

This **InputEdgeLatch**:

- is a low-level sampling bridge;
- is cleared when consumed by the next eligible sampling point;
- is invalidated when device/context loss makes the edge inapplicable;
- does not retry a rejected gameplay action later;
- does not survive a context transition for an action owned by the old context;
- does not queue arbitrary semantic actions until they become legal.

It therefore does not violate the GDS no-general-input-intent-buffer rule.

## 7. Multiple Catch-Up Ticks

If one rendered frame executes multiple fixed simulation ticks:

- a `Pressed` edge is exposed only to the first eligible tick;
- a `Released` edge is exposed only once;
- held digital state remains held across subsequent ticks;
- accumulated mouse/wheel delta is consumed once unless new platform events arrive;
- analog controller state is sampled from the latest known device state for each tick.

A catch-up loop cannot multiply one physical click into multiple gameplay presses.

## 8. Semantic Action Types

Each registered action declares one input value kind:

- `Digital`;
- `Axis1D`;
- `Axis2D`.

Digital samples expose:

- `Pressed`;
- `Held`;
- `Released`.

Analog samples expose a normalized value after device/profile transforms.

Actions may additionally declare whether continuous hold state is meaningful to the owning mechanic.

## 9. Canonical Action Registry

The project maintains a closed typed `ActionId` registry for shipping actions.

It includes the GDS canonical on-foot actions and context-specific actions for:

- UI navigation/confirm/cancel;
- construction;
- spacecraft flight/camera/docking;
- terminals/consoles;
- tactical robot/squad control;
- pause/system functions;
- accessibility-supported equivalents.

A physical key cannot directly call a gameplay function.

## 10. Input Context Stack

The active session owns one ordered `InputContextStack`.

Baseline context classes are:

1. `SystemModal`;
2. `UI/Menu`;
3. `Terminal/Console`;
4. `SpecializedGameplay` such as Construction or Spacecraft;
5. `Restricted` where incapacitation or authored state suppresses ordinary actions;
6. `OnFoot` base gameplay.

The stack records the exact active context instances rather than only one global enum so nested modal/menu states remain explicit.

## 11. Context Priority

Resolution follows GDS priority:

1. modal/system confirmation;
2. active UI/terminal;
3. specialized control mode;
4. normal gameplay.

A higher-priority context receives first opportunity to consume a conflicting action.

No lower context sees a consumed action.

## 12. Action Capture Modes

A context binding declares one of:

- `Exclusive` — consume when matched;
- `Conditional` — consume only if the context currently handles that action;
- `GlobalSystem` — route to the explicit system owner regardless of lower gameplay focus, subject to modal safety.

Pass-through is explicit. There is no accidental fallthrough from a UI click to weapon fire.

## 13. Pause Action

`Pause` is a named system action.

Its semantic result is `PauseRequest`, not direct mutation of `TruePause`.

TA-1 queued-pause rules remain authoritative: if a transaction is indivisible, the request is retained until the next Stable Simulation Boundary and then global True Pause starts before the next ordinary tick.

## 14. UI Focus and Pointer Capture

When UI owns pointer/navigation focus:

- pointer clicks do not also fire weapons/interactions;
- mouse movement used by a pointer does not also rotate the player/ship camera unless the active UI explicitly uses a combined authored interaction;
- controller navigation actions are consumed by the focused UI layer;
- closing the UI restores the prior valid gameplay context.

TA-11 UI specs define detailed focus behavior.

## 15. Mouse Input

Mouse look uses raw/high-precision relative movement where supported by the platform path.

Project settings expose separate sensitivity for:

- first-person look;
- ADS/scoped look;
- spacecraft control/camera;
- construction/strategic camera.

Mouse acceleration is Off by default.

Mouse delta is never treated as a Simulation-Time velocity source without the owning control system interpreting it.

## 16. Controller Input

Controller input uses normalized semantic axes/buttons after mapping.

The resolver applies in deterministic order:

1. device normalization;
2. inner deadzone;
3. outer normalization;
4. response curve;
5. axis inversion;
6. sensitivity multiplier;
7. action-specific clamp.

The transformed value remains intent; spacecraft/character/camera systems decide physical effect.

## 17. Deadzones

Left stick, right stick, and relevant triggers expose user-adjustable deadzone/threshold values within validated safe ranges.

Deadzone settings cannot create movement outside the resulting action value and cannot modify game physics directly.

## 18. Device Switching

Keyboard/mouse and controller can both remain connected and can both generate valid input.

`PromptDeviceFamily` is presentation state selected from the most recently meaningful user input after a tuneable anti-flicker interval.

Noise below deadzone and insignificant mouse/controller jitter does not flip prompt glyph family.

## 19. Disconnect/Reconnect

Controller disconnect:

- releases its held semantic actions;
- cannot leave movement/fire permanently held;
- emits a presentation notification if the disconnected device was actively used;
- does not pause automatically unless a later explicit platform policy requires it.

Reconnect restores device availability but does not resurrect stale press edges.

## 20. Hold/Toggle Transform

Hold/Toggle accessibility preferences are implemented at the semantic action layer for actions explicitly declared toggle-safe by the GDS.

Examples include Sprint, Crouch, ADS, and compatible Scan Mode.

A toggle changes action-state ergonomics only. It cannot bypass movement/equipment/energy/interruption legality.

## 21. Sustained Interaction Alternative

For eligible non-precision sustained interactions the input layer can emit a latched **interaction intent** using:

`Press start -> Press cancel/stop`.

The Interaction domain still owns:

- range;
- target validity;
- duration;
- tool/capability requirement;
- movement/damage interruption;
- completion commit.

## 22. Auto-Sprint

Auto-Sprint is an input-assistance policy that may request Sprint after sustained eligible forward intent.

It never changes the Movement domain's authoritative sprint restrictions.

When a blocker becomes active, the request ceases or is rejected normally.

## 23. Aim-Assist Boundary

Controller/accessibility Aim Assist can transform **camera/aim input intent** only after receiving a filtered set of legitimately visible/eligible target presentation candidates.

Permitted support:

- reticle friction/slowdown;
- bounded camera rotation toward a target already near the reticle.

Forbidden:

- projectile/bullet magnetism;
- modifying hit queries;
- hidden target discovery;
- through-wall snapping;
- automatic weak-point selection.

Mouse Aim Assist is Off by default and uses the same boundary if explicitly enabled.

## 24. Aim-Assist Candidate Feed

Candidate production is owned by gameplay/perception/physics visibility systems and is knowledge-filtered.

The input module receives only candidate direction/bounds and eligibility needed for assist calculations.

It cannot query all hidden runtime entities to search for targets.

## 25. Text Entry

Text-entry mode is distinct from action binding.

When a shipping UI text field has focus:

- Unicode character/text events route to the UI text input owner;
- navigation/editing keys are interpreted by that field/context;
- gameplay bindings are suppressed according to UI context priority.

Text characters are not inferred from gameplay action bindings.

## 26. Rebinding Boundary

Bindings map `DeviceInput` -> `ActionId` within one or more declared contexts.

Rebinding cannot create a new gameplay action or redefine action semantics.

Detailed conflict detection/persistence is defined in the next TA-11 input-settings specification.

## 27. Context Change While Held

When context ownership changes while an input is held:

- the old context receives a semantic release/cancel where required for cleanup;
- the held physical state is re-evaluated under the new context;
- no old Pressed edge is replayed in the new context;
- the new context can observe current hold only for actions whose semantics allow entering while already held.

Potentially destructive/commit actions require a fresh press after entering their context unless the owning screen/mechanic explicitly defines otherwise.

## 28. True Pause Input Behavior

While True Paused:

- simulation/gameplay actions do not commit;
- pause/menu UI actions remain active through presentation update;
- transient gameplay press edges are not saved for post-resume execution;
- held gameplay input on resume is treated according to current hold state, but one-shot world actions require a fresh valid press.

## 29. Input and Saves

Transient raw/semantic input is never serialized as world progression.

Profile-level bindings/preferences are settings data whose persistence implementation belongs to TA-12.

Save/load never restores a key as `Pressed`.

## 30. Threading

Platform event pumping and authoritative action sampling occur on the TA-1 main thread.

No worker thread mutates live device/context state.

Haptic/audio device callbacks remain separate presentation backends and cannot inject gameplay commands directly.

## 31. Determinism Boundary

Player input is inherently an external real-time source, but once sampled for a simulation tick its semantic command ordering is deterministic for that tick.

Gameplay outcomes cannot depend on:

- render frame count;
- hash iteration order;
- callback arrival ordering between equivalent same-frame inputs;
- audio thread timing;
- prompt-device presentation selection.

## 32. Diagnostics

Development inspection exposes:

- connected devices;
- raw normalized axes;
- active context stack;
- effective bindings;
- action samples;
- consumed-by context;
- prompt-device family;
- deadzone/response transformed values;
- aim-assist candidate/result diagnostics without hidden shipping knowledge.

Privileged debug views must be clearly separated from shipping UI.

## 33. Validation Requirements

Tests must cover:

- short press between fixed ticks is sampled once;
- one press is not multiplied across catch-up ticks;
- context priority prevents fire-through-UI;
- context transition does not replay stale presses;
- controller disconnect releases held actions;
- prompt-device switching ignores analog noise;
- hold/toggle alternatives preserve gameplay legality;
- True Pause discards one-shot gameplay presses;
- Aim Assist cannot change physical hit resolution or target unknown actors.

## 34. Explicit Non-Goals

No baseline requirement for:

- raw key codes in gameplay systems;
- global delayed gameplay input buffering;
- mandatory combo/mash detection;
- platform-specific gameplay semantics;
- a separate controller-only game ruleset;
- input-thread ownership of gameplay mutation;
- aim magnetism.

## 35. Dependencies

Depends on TA-1 main loop/pause, TA-2 commands/read models, TA-5 physics visibility, TA-7 player/runtime command phase, GDS Player Controls, GDS Accessibility, and the remaining TA-11 UI/settings specifications.

## 36. Open Questions

None in the input device/action/context routing architecture.
