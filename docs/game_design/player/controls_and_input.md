# Controls and Input

> **Status:** Design Complete  
> **Authority:** Direct player input model, default PC/controller mappings, rebinding rules, context ownership, hold/toggle behavior, and input conflict resolution

## 1. Purpose

Controls remain predictable across exploration, station operation, combat, mining, salvage, interaction, construction, and spacecraft transitions.

The input layer exposes named gameplay actions. Physical keys/buttons are bindings to those actions and never own gameplay behavior directly.

## 2. Core On-Foot Actions

Canonical on-foot actions are:

- Move Forward / Backward / Left / Right;
- Look;
- Jump;
- Sprint;
- Crouch;
- Interact;
- Alternate Interact;
- Primary Action;
- Secondary Action / Aim;
- Reload;
- Switch Held Item;
- Quick Slot 1–4;
- Inventory;
- Mission / Objectives;
- Pause.

Subsystems may expose additional named context actions, but they must register them through the same binding/conflict system.

## 3. Default Keyboard and Mouse Mapping

Baseline defaults:

| Action | Default |
| --- | --- |
| Move | W / A / S / D |
| Look | Mouse |
| Jump | Space |
| Sprint | Left Shift |
| Crouch | Left Ctrl |
| Interact | E |
| Alternate Interact | F |
| Primary Action | Left Mouse |
| Secondary Action / Aim | Right Mouse |
| Reload | R |
| Switch Held Item | Mouse Wheel |
| Quick Slot 1–4 | 1 / 2 / 3 / 4 |
| Inventory | Tab |
| Mission / Objectives | M |
| Pause | Escape |

## 4. Default Controller Mapping

Controller defaults use Xbox-style position names; platform glyphs substitute their local symbols.

| Action | Default |
| --- | --- |
| Move | Left Stick |
| Look | Right Stick |
| Jump | South Face Button / A |
| Crouch | East Face Button / B |
| Interact | West Face Button / X |
| Alternate Interact | North Face Button / Y |
| Sprint | Left Stick Click |
| Primary Action | Right Trigger |
| Secondary Action / Aim | Left Trigger |
| Reload | Right Bumper |
| Switch Held Item | Left Bumper |
| Quick Slot 1–4 | D-Pad Up / Right / Down / Left |
| Inventory | View / Back |
| Mission / Objectives | Hold View / Back |
| Pause | Menu / Start |

`Inventory` is the short press of View/Back. `Mission / Objectives` is a hold of the same control using a tuneable hold threshold. The UI always communicates this distinction.

## 5. Rebinding

Every normal gameplay action is rebindable for keyboard/mouse and controller.

The binding UI:

- detects hard conflicts inside the same simultaneously active context;
- permits deliberate contextual reuse where contexts are mutually exclusive;
- warns when an essential action becomes unbound;
- shows the currently effective binding in prompts/tutorials;
- supports reset-to-default per device family.

## 6. Input Contexts

Canonical contexts are:

- On Foot;
- UI / Menu;
- Construction;
- Spacecraft;
- Console / Terminal;
- Incapacitated / Restricted.

Only the current context receives context-exclusive input.

## 7. Context Priority

Input ownership priority is:

1. modal/system confirmation;
2. active UI/terminal;
3. specialized control mode;
4. normal gameplay.

A higher context consumes conflicting input before lower contexts.

## 8. Device Switching

Keyboard/mouse and controller may be switched during gameplay without restarting.

Prompt glyphs update to the most recently active device family after a short tuneable anti-flicker delay.

## 9. Look Input

Mouse look is direct and continuous.

Mouse acceleration is disabled by default.

Separate sensitivity is exposed for:

- first-person look;
- ADS/scoped look;
- spacecraft control/camera;
- construction/strategic camera.

Controller sensitivity and deadzones follow GDS-13 Accessibility.

## 10. Hold / Toggle Preferences

The baseline supports user-selectable Hold or Toggle for:

- Sprint;
- Crouch;
- Aim Down Sights;
- Scan Mode where a tool exposes continuous scan mode.

Changing Hold/Toggle changes input ergonomics only; it does not bypass state, energy, movement, or interruption rules.

## 11. Accessible Sustained Interaction Alternative

For sustained interactions that do not require continuous precision aiming, accessibility may convert physical button holding into:

`Press once to start → Press again/Cancel to stop`.

The authoritative interaction duration, movement/range checks, damage interruption, tool requirement, and target-validity checks remain unchanged.

Precision continuous actions whose physical aim is itself gameplay keep continuous directional control while still avoiding mandatory rapid button pressing.

## 12. Interaction Input

A short `Interact` press requests the highest-priority valid target from the Player Interaction system.

`Alternate Interact` opens/executes the target's secondary contextual action where one exists.

The input layer does not choose target priority itself.

## 13. Primary and Secondary Action

The currently held tool/weapon owns the semantic effect of `Primary Action` and `Secondary Action`.

The input system supplies intent only.

## 14. No General Input-Intent Buffer

The baseline has **no global gameplay input buffer** that stores arbitrary action presses for later execution.

An input either reaches its currently valid owning subsystem or is rejected for that moment.

A subsystem may define an explicit local queue/window when that mechanic requires it, but such a queue must be documented by that subsystem and cannot survive a context change that invalidates the action.

This prevents implementation from inventing hidden cross-system buffering behavior.

## 15. No Hidden Double-Tap or Mash Requirement

Core movement has no undocumented double-tap action.

The baseline contains no mandatory rapid-button-mashing/QTE mechanic.

Dash/dodge is not part of baseline movement.

## 16. Pause

`Pause` requests global True Pause under `../systems/time_and_simulation.md`.

If an atomic transition temporarily prevents pausing, the request is retained and True Pause begins immediately after that transaction reaches its stable boundary.

Held gameplay inputs do not execute world actions while paused or on resume unless a fresh/current input state legitimately requests them.

## 17. Construction Context

Construction Mode maps movement/look inputs to its free construction camera while the physical player remains at the pre-construction location.

Construction Mode invokes the global pause semantics defined by the Station/Time authorities.

## 18. Console and UI Context

A terminal/menu may capture movement/look/navigation bindings for UI control.

Closing the interface restores the previous valid gameplay context.

A stale UI command cannot execute after its target/context has become invalid.

## 19. Incapacitated Context

While Incapacitated, direct movement/combat/tool actions are disabled.

Only recovery/pause/menu actions explicitly permitted by Player Health and UI remain available.

## 20. Accessibility

GDS-13 Accessibility is authoritative for:

- full remapping;
- controller deadzones;
- sensitivity;
- axis inversion;
- hold/toggle alternatives;
- aim-assistance settings;
- repeated-input alternatives;
- prompt glyph accessibility.

These features use the same named actions and do not create separate gameplay semantics.

## 21. Persistence

Bindings and input preferences are profile/application settings.

Transient held/pressed input is not serialized as world progression.

Gameplay-state settings whose effects belong to the save follow GDS-12 persistence rules.

## 22. Edge Cases

- If the active context changes while an action is held, the old context loses ownership immediately.
- If a binding conflict leaves an essential action unbound, the UI warns before leaving the binding screen and provides a reset/rebind path.
- If keyboard and controller are used nearly simultaneously, gameplay uses both valid inputs but presentation chooses the most recent device family after anti-flicker filtering.
- If the View/Back hold crosses the Mission/Objectives threshold, the short-press Inventory action is not also fired.
- A remapped accessibility control never bypasses an interaction, combat, movement, or equipment legality check.

## 23. Tuneable Parameters

Tuneable values include:

- sensitivity defaults/ranges;
- stick deadzones;
- trigger thresholds;
- View/Back hold threshold;
- prompt device-switch anti-flicker delay.

Action semantics, context priority, full rebinding, and the no-global-input-buffer rule are fixed.

## 24. Explicit Non-Goals

The baseline does not require:

- fixed non-rebindable gameplay controls;
- mouse acceleration;
- undocumented combos;
- mandatory button mashing;
- global delayed input buffering;
- gameplay rules tied directly to key codes;
- separate mechanics for controller users.

## 25. Dependencies

Depends on Movement, Interaction, Equipment/Tools, Combat, Spacecraft, Station Construction, UI/UX, Accessibility, and Time/Pause.

## 26. Open Questions

None.
