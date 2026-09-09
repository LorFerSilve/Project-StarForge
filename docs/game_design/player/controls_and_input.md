# Controls and Input

> **Status:** Draft  
> **Authority:** Direct player input model, default PC mappings, rebinding rules, context ownership, hold/toggle behavior, and input conflict resolution

## 1. Purpose

Controls remain predictable across exploration, station operation, combat, mining, salvage, and interaction.

The baseline targets keyboard/mouse first while using named actions so controller mapping can provide equivalent capability later.

## 2. Core Actions

Core actions include:

- Move Forward/Backward/Left/Right;
- Look;
- Jump;
- Sprint;
- Crouch;
- Interact;
- Alternate Interact;
- Primary Action;
- Secondary Action;
- Reload;
- Switch Held Item;
- Quick Slot 1–4;
- Inventory;
- Mission/Objectives;
- Pause.

## 3. Default Keyboard/Mouse Mapping

- Move: W/A/S/D
- Look: Mouse
- Jump: Space
- Sprint: Left Shift
- Crouch: Left Ctrl
- Interact: E
- Alternate Interact: F
- Primary Action: Left Mouse
- Secondary Action/Aim: Right Mouse
- Reload: R
- Switch Held Item: Mouse Wheel
- Quick Slots: 1–4
- Inventory: Tab
- Mission/Objectives: M
- Pause: Escape

Future tool/weapon actions can add context bindings without redefining these core actions.

## 4. Rebinding

Every normal gameplay action is rebindable.

The game warns when two actions conflict in the same active context or when an essential action becomes unbound.

Contextual reuse can be accepted when the contexts cannot be active simultaneously.

## 5. Input Contexts

Contexts include:

- On Foot;
- UI/Menu;
- Construction;
- Spacecraft;
- Console/Terminal;
- Incapacitated/Restricted.

Only the active context receives context-exclusive actions.

## 6. Context Priority

Priority order is:

1. modal/system confirmation;
2. active UI/terminal;
3. specialized control mode;
4. normal gameplay.

## 7. Look Input

Mouse look is direct and continuous.

Sensitivity is separately configurable for normal look, aiming, spacecraft, and scoped views.

Mouse acceleration is disabled by default.

## 8. Toggle/Hold

Configurable where appropriate:

- Sprint: Hold or Toggle;
- Crouch: Hold or Toggle;
- Aim: Hold or Toggle.

Timed interactions remain sustained actions when interruption is mechanically meaningful.

## 9. Interaction Input

A short Interact press activates the highest-priority valid target.

A hold interaction is used only where the target defines sustained progress.

## 10. Primary and Secondary Action

Held tools/weapons own Primary and Secondary Action behavior.

The input system does not define the tool effect.

## 11. Input Buffering

Short buffering may improve responsiveness, but a buffered action cannot execute after its context has become invalid.

## 12. No Hidden Double-Tap Requirement

Core movement does not require undocumented double taps.

Dash/dodge is not part of baseline movement.

## 13. Controller Boundary

Future controller mapping must expose equivalent core actions.

Exact controller button defaults can be finalized in accessibility/presentation design without changing action semantics.

## 14. Accessibility Hooks

The input layer supports future remapping, toggle/hold preferences, sensitivity, inversion, and reduced repeated-input requirements.

## 15. Pause

True Pause stops gameplay simulation and routes input to pause UI.

No world action continues because a held key remains down.

## 16. Construction Context

Construction Mode reinterprets movement/look as construction-camera navigation.

The physical player stays at the pre-construction location while simulation is paused according to construction rules.

## 17. Console Context

A terminal can capture movement/look for UI navigation.

Exiting returns On-Foot context.

## 18. Conflict Resolution

When multiple world targets overlap, Interaction rules determine target priority rather than raw input order.

## 19. Persistence

Bindings/preferences are profile/settings state, not world progression.

Transient input state is not persisted across load.

## 20. Explicit Non-Goals

The baseline does not require fixed bindings, mandatory acceleration, undocumented combos, double-tap movement, or gameplay rules tied directly to physical key codes.

## 21. Dependencies

This specification depends on movement, interaction, tools, combat, spacecraft, construction, UI, and accessibility.

## 22. Open Questions

None in the core input-action baseline.
