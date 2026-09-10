# Accessibility

> **Status:** Design Complete  
> **Authority:** Accessibility options and invariants for controls, motion, vision, color, text, audio, captions, interaction, navigation, cognitive load, aiming, pause access, and difficulty separation

## 1. Purpose

Accessibility makes the same authoritative game state perceivable and controllable by more players without silently replacing world, economy, progression, ownership, knowledge, or simulation rules.

## 2. Core Invariant

Accessibility settings do not:

- reduce rewards;
- disable progression;
- alter story/Research/Blueprint access;
- alter loot ownership;
- change faction-reputation logic;
- invalidate saves;
- reveal information the player does not legitimately know.

Accessibility and Difficulty are separate systems.

## 3. Accessibility vs Difficulty

Any accessibility configuration can be used with Assisted, Standard, Veteran, Extreme, or Custom Difficulty.

Gameplay-pressure changes belong to Difficulty. Accessibility changes input/presentation/support unless a setting is explicitly listed as an Assisted/Custom Difficulty axis.

## 4. First-Launch Access

Before starting a new game, the player can configure at minimum:

- subtitles/Closed Captions;
- UI/text scale;
- color/readability preset;
- motion reduction;
- photosensitivity mode;
- key/controller remapping;
- aim-assistance baseline;
- master/dialogue volume.

The setup can be reopened later without restarting progression.

## 5. Full Input Remapping

All normal gameplay actions defined by `../player/controls_and_input.md` are remappable for keyboard/mouse and controller.

The UI warns on same-context hard conflicts and essential unbound actions. Tutorials/prompts always show the effective binding.

## 6. Input Device Switching

Keyboard/mouse and controller may switch during play without restart. Presentation updates glyphs according to the current input-family rule.

## 7. Hold / Toggle Alternatives

User-selectable Hold/Toggle is supported for:

- Aim Down Sights;
- Sprint;
- Crouch;
- Scan Mode;
- other continuous actions whose owning mechanic declares toggle-safe operation.

For non-precision sustained interactions, a press-to-start / press-to-cancel alternative is available while keeping the same Simulation-Time duration and interruption rules.

## 8. No Mandatory Button Mashing

The baseline contains no mandatory rapid-button-mashing/QTE mechanic.

Any later authored repeated-input interaction requires a hold/automatic equivalent and formal design update.

## 9. Auto-Sprint

Optional Auto-Sprint can request Sprint after sustained valid forward movement.

It obeys all Movement restrictions including Heavy Load, Critical Health, crouch, invalid terrain, and explicit equipment/status blockers.

## 10. Controller Deadzones and Sensitivity

Settings expose:

- left-stick inner deadzone;
- left-stick outer response;
- right-stick inner deadzone;
- right-stick outer response;
- trigger threshold where relevant;
- first-person sensitivity;
- ADS/zoom multiplier;
- spacecraft sensitivity;
- construction/strategic camera sensitivity.

Values are user-adjustable within technically safe bounds.

## 11. Axis Inversion

Horizontal and vertical look inversion are independently configurable where the camera/control mode supports them, including spacecraft camera/aim.

## 12. First-Person FOV

Initial target:

- default vertical FOV: **75°**;
- supported vertical adjustment target: **60°–100°**.

Technical validation may narrow/extend safe bounds without changing the requirement that first-person FOV be broadly adjustable.

## 13. Camera Shake / Head Bob / Motion Blur

- Camera Shake: 0–100%, with **0% fully supported**.
- Head Bob: 0–100%, with **0% fully supported**.
- Motion Blur: optional and **Off by default**.

At zero/reduced motion, impact/state remains communicated through other channels.

## 14. Cosmetic Weapon Sway

Purely cosmetic first-person weapon sway may be reduced.

Mechanically authoritative spread/recoil/aim displacement remains represented by reticle/weapon behavior and is not silently removed by a presentation setting.

## 15. Screen Distortion

Chromatic aberration, strong full-screen refraction, damage warping, and anomaly distortion have a reduction setting.

Any critical state they represent also has non-distortion cues.

## 16. Photosensitivity Safe Mode

This preset/mode reduces or removes:

- rapid flashing/strobing;
- high-contrast repetitive pulses;
- aggressive full-screen flicker;
- high-frequency lightning/emissive flashes.

Equivalent semantics use steady/slow visual cues, icon/text, audio, and optional haptics.

## 17. Reduced Effects Mode

Reduced Effects lowers nonessential:

- particles;
- smoke density;
- debris;
- sparks;
- bloom;
- distortion;
- decorative screen overlays.

Gameplay-critical hazard boundaries, attack telegraphs, objective state, and interaction cues remain legible.

## 18. Brightness / Gamma

Brightness/gamma calibration uses reference imagery so dark environments retain intended contrast while supported displays do not unintentionally crush gameplay-relevant shadow detail.

## 19. High Contrast Gameplay

Optional High Contrast strengthens presentation of already known/eligible:

- interactables;
- player squad;
- identified hostile/friendly/neutral actors;
- objective-relevant targets;
- hazards;
- critical UI.

It never reveals unknown targets/routes/items.

## 20. Interaction Highlight

Highlight strength supports:

- Subtle;
- Standard;
- Strong.

An optional persistent-in-focus-range behavior may keep an already valid interactable highlighted while it remains legitimately targetable. No highlight passes through opaque barriers or reveals undiscovered targets.

## 21. Color Independence

No gameplay-critical semantic relies solely on hue.

Every critical color meaning also uses at least one of:

- icon;
- shape;
- pattern;
- text;
- animation/state;
- audio/caption.

## 22. Color-Vision Support

Semantic color presets support common red/green and blue/yellow differentiation difficulties plus a low-saturation/monochrome-readable configuration.

Where practical, key semantic colors are independently customizable for Friendly, Hostile, Neutral, Interactive, Objective, Warning, and Critical.

Icons/shapes remain authoritative secondary cues.

## 23. UI and Text Scale

Initial supported target:

- global UI scale: **80%–150%**;
- default: **100%**.

Body/subtitle text provides an additional scale control where feasible.

Layouts must reflow rather than clip critical information throughout the supported range.

## 24. Font Readability

Functional UI/body text uses a high-legibility sans-serif family.

Decorative faction fonts are not used for long critical text.

A high-legibility/dyslexia-friendly alternative may replace functional text without changing terminology/layout semantics.

## 25. Subtitles

Gameplay-relevant spoken dialogue supports subtitles.

First-launch setup asks for subtitle preference; baseline recommended/default configuration is **On**.

Configurable options include:

- text size;
- background opacity;
- speaker name;
- speaker color as secondary cue;
- line width;
- Closed Captions;
- direction/source cue where legitimately known.

## 26. Closed Captions

Closed Captions can describe relevant non-speech audio only when that event is legitimately audible/known.

Examples:

- `[Hull breach alarm — Deck B]`;
- `[Pressure leak nearby]`;
- `[Hostile footsteps: right]` when actually audible;
- `[Robot command rejected]`.

Captions do not reveal inaudible hidden enemies/events.

## 27. Dialogue Intelligibility / Audio

Critical dialogue can duck Music/SFX.

A Dialogue Boost setting raises dialogue relative to other categories.

Major audio categories are independently adjustable; critical state always has a non-audio route.

## 28. Mono Audio / Directional Alternative

Mono output is supported.

Direction-dependent gameplay audio has an optional visual direction indicator when the sound/event is legitimately known.

## 29. Dynamic Range / Tinnitus

Dynamic Range presets:

- Full;
- Standard;
- Night / Reduced Range.

Tinnitus/high-frequency damage effects can be fully disabled independently.

## 30. Haptics

Haptics have:

- master enable;
- 0–100% intensity;
- optional category separation where practical.

No mechanic requires haptics.

## 31. Notification Duration / Tutorial Timing

Noncritical notification duration is adjustable.

Tutorial explanations support:

- global True Pause while reading when the tutorial invokes pause;
- extended duration;
- manual dismiss;
- contextual hints on/off;
- revisit/history.

No required tutorial permanently disappears because reading took longer than a fixed short timer.

## 32. Pause Semantics

Accessibility does **not** create any partial-pause state.

The explicit Pause action invokes the same global `True Pause` defined by `../systems/time_and_simulation.md` during ordinary gameplay, combat, missions, raids, station events, and management.

A complex planning/accessibility screen may provide a `Pause While Viewing` convenience only by invoking **global True Pause**. It cannot freeze a local hazard while allowing production, travel, Dynamic Events, or Horizon attacks to continue.

If an atomic transaction briefly cannot be interrupted, the Pause request is retained and activates immediately after the stable boundary.

## 33. Navigation Assistance

Navigation Assistance may strengthen legitimate known-route guidance through:

- tracked-objective direction;
- breadcrumb/path hints in explored/known traversable areas;
- stronger eligible door/route highlighting;
- distance display.

It cannot reveal secret routes, unexplored objective locations, hidden enemies, or unavailable access knowledge.

## 34. Objective Guidance

Three levels:

- Minimal;
- Standard;
- Detailed.

Detailed rewrites/expands already known instructions and blockers; it does not add world knowledge.

## 35. Interaction Assistance

Options may:

- strengthen highlights;
- keep valid prompts visible longer;
- modestly increase target-focus tolerance within the authoritative maximum interaction range;
- provide focus snapping for small noncombat controls when line/range validity already passes.

They cannot interact through walls or extend physical reach beyond the owning interaction's allowed accessibility envelope.

## 36. Aim Assistance

Aim Assist supports Off, Low, Standard, High.

Permitted assistance:

- reticle slowdown/friction near a legitimately visible eligible target;
- mild camera rotation toward a target already under/near the reticle.

Forbidden:

- bullet/projectile magnetism;
- hidden hit chance;
- snap through walls;
- detection of unknown enemies;
- automatic weak-point selection.

GDS-9 physical hit resolution remains authoritative.

Mouse Aim Assist is Off by default but may use the same bounded assistance when explicitly enabled.

## 37. Reticle and Damage Presentation

Reticle options include size, thickness, opacity, center dot, outline, and semantic color.

They do not change spread.

Players can reduce damage vignette, screen flash, camera kick, and low-Health pulse while Health/status remains available through HUD/icon/text/audio alternatives.

## 38. Cognitive Load

Noncritical notification categories are filterable.

P0/P1 AlarmPriority safety warnings cannot be completely hidden without an explicit high-risk warning and a remaining accessible route to the state.

HUD information-density presets:

- Minimal;
- Standard;
- Expanded;
- Custom.

Mandatory critical state remains accessible in every preset.

## 39. Management-View Readability

Complex interfaces provide consistent hierarchy plus appropriate:

- filtering;
- search where lists warrant it;
- explicit blocker lists;
- one dominant diagnostic overlay at a time;
- expanded terminology/tooltips for abbreviations.

## 40. Save Accessibility

Manual/Quick Save follow GDS-12 Stable Save Boundary rules.

If the snapshot cannot commit immediately, the request is queued and its waiting state is clearly communicated.

This allows users to stop unexpectedly without creating an unsafe half-transaction save.

## 41. Presets

Baseline accessibility presets include:

- Reduced Motion;
- High Readability;
- Hearing Support;
- Photosensitivity Safe.

Presets modify documented individual settings only. Every setting remains independently editable afterward.

## 42. Persistence / Reset

Accessibility/input/subtitle/display/audio preferences persist at profile/application level where appropriate.

Difficulty persists according to GDS-12.

Each category supports reset-to-default. Resetting accessibility never resets campaign progression.

## 43. Testing Invariants

Every implementation regression pass must verify critical gameplay with combinations including:

- color-independent presentation;
- subtitles/Closed Captions with reduced/muted audio;
- Camera Shake 0%;
- Head Bob 0%;
- Reduced Effects;
- Photosensitivity Safe Mode;
- remapped inputs;
- Detailed Objective Guidance;
- UI scale at supported extremes;
- Aim Assist at High without altered hit resolution.

## 44. Edge Cases

- Muting audio never makes a critical known alarm inaccessible.
- High Contrast/Aim Assist never highlights or tracks an unknown actor.
- Setting Camera Shake to 0 never removes authoritative recoil/spread—only viewpoint displacement presentation.
- A Pause While Viewing option freezes the whole Simulation, including Horizon Defense and strategic timers.
- UI at 150% must provide reflow/scroll rather than clipping an irreversible confirmation or blocker.
- Closed Captions do not caption events outside valid hearing/information state.

## 45. Tuneable Parameters

Tuneable/technical-validation values include safe FOV/UI ranges, deadzone ranges, aim-assist coefficients, caption timing, highlight tolerance, and reduced-effect intensity.

The feature semantics, knowledge boundary, no-reward-penalty rule, global pause semantics, and physical-hit boundary are fixed.

## 46. Explicit Non-Goals

No accessibility reward penalty, wallhack, bullet magnetism, hidden hit chance, mandatory button mash, unremappable core action, audio-only/color-only critical state, partial-pause exploit, or save invalidation exists in the baseline.

## 47. Dependencies

References GDS-1 Pause/Knowledge, GDS-5 Controls/Movement/Interaction, GDS-9 Combat, GDS-12 Difficulty/Time/Save, and all GDS-13 presentation specifications.

## 48. Open Questions

None.
