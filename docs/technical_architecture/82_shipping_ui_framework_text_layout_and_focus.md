# TA-11 — Shipping UI Framework, Text Layout, and Focus

> **Status:** Architecture Complete  
> **Authority:** Shipping UI ownership, retained widget/layout model, text shaping/rasterization, focus/navigation, modal hierarchy, screen lifetime, rendering boundary, and UI-to-domain command flow

## 1. Purpose

StarForge requires a shipping UI capable of HUD, inventory, station management, galaxy map, research, manufacturing, crew/robot/ship management, settings, subtitles, tutorials, confirmations, and failure/loading screens.

Dear ImGui remains development tooling only. Shipping UI uses a purpose-built StarForge UI layer integrated with TA-4 rendering and TA-2 Read Models.

## 2. UI Ownership Boundary

The shipping `ui` module owns:

- widget/screen tree;
- layout;
- pointer/controller/keyboard focus;
- styling/presentation;
- text shaping/layout;
- transient UI state;
- command construction;
- screen transitions;
- local preview state;
- accessibility presentation transforms.

It does **not** own:

- gameplay truth;
- inventory contents;
- mission/objective truth;
- Credits;
- station topology;
- ship configuration authority;
- save success;
- hidden world knowledge.

UI consumes immutable Read Models and emits typed requests/Commands.

## 3. Retained UI Tree

The baseline shipping UI uses a small StarForge-owned **retained widget tree**.

A `UIScreenInstance` owns a hierarchy of `UIElement`s with stable process-local element IDs for focus/lifetime, not persistent gameplay identity.

The tree is rebuilt/updated from screen controller state and Read Models at presentation boundaries, not mutated by gameplay domains.

## 4. Purpose-Built Widget Set

The baseline provides only project-needed primitives such as:

- Panel/Container;
- Stack/Row/Column;
- Grid;
- Text;
- Image/Icon;
- Button;
- Toggle/Checkbox;
- Slider;
- Progress/Status display;
- Scroll container;
- List/virtualized list;
- Tab/section selector;
- Tooltip;
- Text field where required;
- Input-binding capture row;
- modal dialog;
- map/diagram custom canvas host.

This is not intended to become a general-purpose third-party UI engine.

## 5. Screen Controllers

Each major screen has a project-owned controller/presenter responsible for:

- selecting the correct Read Model;
- maintaining transient selection/filter/sort/focus state;
- mapping user UI intent to typed Commands/settings requests;
- interpreting typed command results into presentation feedback;
- never bypassing owning-domain validation.

A widget does not call DomainStore mutation directly.

## 6. UI Read Model Flow

Conceptual flow:

```text
Authoritative Domains
  -> Stable Simulation Boundary
  -> Knowledge-Filtered Read Models
  -> UIScreenController
  -> Retained UI Tree
  -> TA-4 Shipping UI Render Pass
```

Commands flow in the opposite direction through typed domain APIs.

## 7. UI State Classes

UI state is classified as:

- `Ephemeral` — pointer hover, animation phase, tooltip timer;
- `SessionTransient` — open screen, selected tab, current filter/sort, scroll location;
- `ProfilePreference` — UI scale, HUD mode, subtitle preferences;
- `GameplayState` — never owned by UI; referenced through Read Models only.

Save/load must not accidentally serialize ephemeral widget objects as world state.

## 8. Screen Stack

The shipping interface maintains a `UIScreenStack`.

Layer classes include:

1. system/fatal/modal layer;
2. pause/settings layer;
3. major gameplay management screen;
4. contextual panel/overlay;
5. HUD;
6. passive notifications/captions.

Higher modal layers can capture focus while lower layers remain visible but noninteractive.

## 9. Single Primary Focus Owner

At any moment exactly one shipping UI layer owns primary navigation/pointer focus.

The input context stack from TA-11 input architecture reflects that owner.

A single button press cannot both activate UI and fire/interact in the world unless a specific combined interaction has an authored contract.

## 10. Focus Model

Each focusable UI element exposes:

- enabled/disabled state;
- tab/navigation order or explicit directional neighbors where needed;
- semantic role;
- accessible label/value/state;
- activation/cancel behavior;
- parent screen/lifetime generation.

Focus never points to an element from a destroyed/rebuilt screen generation.

## 11. Controller/Keyboard Navigation

Shipping UI supports complete navigation without pointer-only interaction.

Directional navigation uses:

- explicit neighbors for authored high-value layouts where necessary;
- deterministic spatial/navigation-order resolution for ordinary layouts;
- wrap behavior only when intentionally enabled;
- predictable entry/default focus.

Lists/scroll views reveal the focused item automatically.

## 12. Pointer Navigation

Pointer hit testing uses UI layout rectangles in output/safe-area coordinates.

Pointer capture is explicit for operations such as:

- slider dragging;
- map panning;
- scrollbar drag;
- binding capture.

Capture is released on pointer-up, screen destruction, focus loss, or device/context invalidation.

## 13. Back/Cancel Contract

A canonical semantic `UIBack`/Cancel action:

1. closes the current modal/subpanel;
2. cancels uncommitted preview state;
3. returns one hierarchy level;
4. never reverses an already committed gameplay transaction unless that gameplay operation itself exposes a reversal command.

Every screen provides a safe Back path.

## 14. Confirm/Commit Boundary

Consequential UI flows distinguish:

- Preview;
- Confirm request;
- Authoritative Accepted/Rejected result;
- Committed result.

The UI cannot display final success merely because the player clicked Confirm.

Confirmation dialogs are reserved for material irreversible/hard-to-reverse consequences according to GDS-13.

## 15. Stale Command Protection

Commands generated from UI include stable typed target IDs and relevant source revision/precondition data.

If the displayed Read Model becomes stale before commit:

- owning domain rejects/revalidates;
- UI refreshes from a newer Read Model;
- row index/visible order is never used as target identity.

## 16. UI Rendering Boundary

Shipping UI is submitted through the TA-4 UI/composite path after world tone mapping at native output resolution.

UI rendering uses:

- orthographic logical UI coordinates;
- scissor/clip rectangles;
- batched quads/icons/glyphs;
- explicit blend state;
- renderer-owned GPU resources;
- ContentId-resolved textures/fonts/icons from TA-10.

The UI module does not issue raw OpenGL calls directly.

## 17. Logical UI Units

Layout occurs in resolution-independent logical UI units derived from:

- framebuffer size;
- global UI scale;
- safe area;
- platform DPI information where reliable;
- text-scale/accessibility settings.

A 100% baseline layout remains readable across supported desktop resolutions without hard-coded pixel-only positioning.

## 18. Global UI Scale

The architecture supports the GDS target 80%–150% global UI scale range subject to final technical validation.

Critical layouts reflow, scroll, or change responsive arrangement rather than clip mandatory blockers/confirmations at the supported upper bound.

## 19. Safe Area

A `UISafeArea` constrains critical HUD and menu content.

The player can adjust safe-area margins within validated bounds.

Decorative backgrounds may extend outside the safe area; required text/actions may not.

## 20. Layout System

Baseline layout supports:

- fixed/min/max dimensions;
- content/intrinsic sizing;
- flex-like stack growth/shrink;
- grid tracks;
- alignment;
- margins/padding/gaps;
- aspect constraints where needed;
- scroll overflow;
- responsive breakpoints based on available logical extent.

Layout order is deterministic for identical inputs.

## 21. Text Architecture

Shipping text uses project-owned `TextLayout` over:

- UTF-8 source strings;
- HarfBuzz for glyph shaping/positioning;
- FreeType for scalable font face access and glyph rasterization;
- project-owned line breaking/wrapping/alignment;
- renderer-owned glyph atlas textures.

Raw HarfBuzz/FreeType objects do not become gameplay/content identity.

## 22. Why HarfBuzz + FreeType

HarfBuzz handles Unicode/script shaping and converts text runs into correctly ordered/positioned glyphs.

FreeType provides high-quality scalable glyph rasterization/font access.

This combination keeps text rendering robust without importing a general UI framework.

## 23. Font Content

Fonts are content assets referenced by ContentId.

TA-10 remains responsible for source/cooked discovery/versioning; TA-11 owns runtime text-role mapping such as:

- functional UI body;
- heading;
- subtitle/caption;
- monospace/diagnostic where needed;
- optional high-legibility accessibility alternative.

Decorative faction fonts cannot replace critical long-form functional text.

## 24. Glyph Atlas

The text renderer uses one or more renderer-owned dynamic glyph atlases keyed by:

- font ContentId;
- face/style;
- effective pixel size;
- render mode.

Glyph atlas handles are nonpersistent and generation-checked through renderer resource lifetime rules.

TA-13 defines numeric atlas/memory budgets.

## 25. Text Reflow

Changing UI/text scale invalidates affected layout/text runs and triggers deterministic reflow.

Text clipping is never used to hide irreversible-action labels, blockers, subtitle content, or accessibility-critical information when scrolling/wrapping can preserve it.

## 26. Text Direction and Shaping

The text subsystem is Unicode/shaping-capable and stores explicit run direction/script/language metadata when supplied.

The baseline project language set can remain content/product policy, but the architecture does not assume every string is simple ASCII/Latin glyph-by-glyph rendering.

## 27. Localization Boundary

TA-11 does not define a full localization production pipeline unless required by later product scope.

UI controllers consume resolved display strings or localization-ready text references from the content/presentation layer.

Gameplay identifiers and save identity never depend on translated display text.

## 28. Style Tokens

Shipping UI style uses project-owned immutable style data such as:

- spacing;
- type roles;
- corner/border metrics;
- semantic colors;
- icon sizes;
- focus/disabled/critical states.

Style can be content-configured through closed TA-10 schemas, but it cannot execute screen logic or gameplay code.

## 29. Color Semantics

Critical meaning never depends solely on color.

Widgets expose icon/shape/text/pattern/state alongside semantic color for warning, hostile, friendly, neutral, interactive, objective, and critical meanings.

Color-vision presets transform presentation tokens only.

## 30. Reduced Motion

UI transitions/animations consume non-gameplay Presentation Time and have a reduced-motion path.

Reduced Motion can:

- shorten/remove decorative transitions;
- replace motion with fades/instant state;
- remove repeated pulsing.

It cannot delay or hide committed state.

## 31. True Pause

Pause/settings UI continues updating with presentation real time while authoritative Simulation Time is frozen according to TA-1.

Opening a screen that requests `Pause While Viewing` sends the normal global True Pause request. No UI screen can create a local partial pause.

## 32. Large Lists

Crew, robots, resources, Blueprints, Work Orders, logs, and similar large views use:

- stable typed IDs;
- filter/search/sort over immutable Read Models;
- row virtualization when scale warrants it;
- no row-number authority.

The currently selected item is revalidated when a new Read Model generation arrives.

## 33. Tooltips

Tooltips are presentation-only expansions of already available/legitimate information.

They may explain units, sources/modifiers, constraints, terminology and known consequences.

They cannot reveal hidden state and cannot be the sole carrier of immediate lethal warnings.

## 34. Modal Policy

A modal dialog is appropriate only when:

- a material irreversible/hard-to-reverse action requires confirmation;
- a fatal/technical condition blocks continuation;
- the user explicitly opened a system modal such as binding capture.

Routine information uses nonblocking presentation instead of modal spam.

## 35. Error/Blocker Presentation

Rejected requests display the typed known blocker supplied by the owning domain/presentation read model.

Unknown causes remain generic/uncertain according to the Knowledge Filter.

UI never introspects privileged state to produce a more revealing error string.

## 36. Loading and Failure Screens

Loading/failure screens display staged progress/state from TA-3/TA-10/TA-12 read models.

They cannot claim:

- destination Ready before required content is ready;
- save complete before persistence succeeds;
- mission success before commit;
- resource restoration that did not occur.

## 37. Accessibility Semantics

Every focusable/meaningful widget carries internal semantic metadata sufficient for:

- consistent keyboard/controller navigation;
- readable labels/values/states;
- high-contrast transformation;
- scalable text/layout;
- later platform accessibility integration if scheduled.

TA-11 does not require an operating-system screen-reader integration in the baseline unless later product scope adds it.

## 38. Development Tooling

Dear ImGui may inspect:

- UI tree;
- focus path;
- layout rectangles;
- clipping;
- draw batches;
- Read Model generation;
- command results;
- glyph atlases.

ImGui remains outside shipping player-facing navigation/state.

## 39. Validation Requirements

Tests must cover:

- one primary focus owner;
- controller-only traversal of major screens;
- UI focus consumes gameplay fire/interact input;
- Back cancels preview without undoing committed state;
- stale row/selection cannot retarget a command;
- UI scale upper/lower supported range preserves critical controls;
- text shaping/layout deterministic for fixture inputs;
- screen destruction invalidates focus/pointer capture;
- Reduced Motion changes presentation only;
- hidden knowledge never appears through tooltip/blocker generation.

## 40. Explicit Non-Goals

No baseline requirement for:

- Dear ImGui as shipping UI;
- HTML/CSS/browser embedding;
- a general UI scripting language;
- gameplay mutation from widget callbacks;
- pixel-perfect single-resolution layout;
- color-only state;
- persistent gameplay identity for UI elements;
- screen-reader platform integration unless separately scheduled.

## 41. Dependencies

Depends on TA-2 Read Models/Commands/Knowledge Filter, TA-4 native-resolution UI composite, TA-10 content/assets, TA-11 input routing/settings, GDS UI/UX and Accessibility, and later TA-12/13/14 persistence/budget/testing phases.

## 42. Open Questions

None in the shipping UI framework/text/focus architecture.
