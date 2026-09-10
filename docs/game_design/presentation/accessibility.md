# Accessibility

> **Status:** Draft  
> **Authority:** Accessibility options and invariants for controls, motion, vision, color, text, audio, subtitles, captions, timing, interaction, aiming, navigation, cognitive load, and difficulty separation

## 1. Purpose

Accessibility options make the same authoritative game state perceivable and controllable by more players without silently replacing the underlying world, economy, progression, or simulation rules.

## 2. Core Principle

Accessibility is not a reward penalty and is not a separate campaign mode.

Using accessibility settings does not:

- reduce rewards;
- disable achievements by default;
- alter story access;
- alter Research/Blueprint access;
- alter loot ownership;
- change faction reputation rules;
- invalidate saves.

## 3. Accessibility vs Difficulty

Accessibility settings and Difficulty profiles are independent.

A player can use:

- Extreme difficulty + extensive accessibility support;
- Assisted difficulty + minimal accessibility changes;
- any other combination.

Difficulty changes permitted gameplay pressure. Accessibility changes control/presentation/support unless a setting explicitly overlaps an Assisted gameplay aid.

## 4. Settings Organization

Accessibility options are grouped into:

- Controls/Input;
- Camera/Motion;
- Visual/Color;
- Text/UI;
- Audio/Dialogue;
- Interaction/Timing;
- Navigation/Orientation;
- Combat/Aiming;
- Cognitive/Notification;
- Presets.

## 5. First-Launch Access

Accessibility and subtitle settings are reachable before starting a new game.

The first-launch flow provides a short accessible setup without forcing the player through gameplay first.

## 6. Full Input Remapping

Keyboard/mouse and controller gameplay actions are remappable.

The system detects hard conflicts and warns when essential actions become unbound.

Tutorial prompts always display the current binding.

## 7. Input Device Switching

Where supported, the game can switch between keyboard/mouse and controller without restarting.

Prompts update to the currently active input family.

## 8. Toggle/Hold Options

Actions that normally require sustained input provide toggle alternatives where mechanically safe, including:

- Aim Down Sights;
- Sprint;
- Crouch;
- Scan mode;
- selected continuous tools;
- hold interactions.

## 9. Hold Interaction Alternative

For non-precision hold interactions, accessibility can convert `hold input for duration` into:

Press once to start
→ Press again/Cancel to stop.

The actual interaction time and interruption rules remain unchanged.

## 10. Rapid Pressing

The baseline design avoids mandatory rapid-button-mashing/QTE mechanics.

If a later authored interaction uses repeated input, it must support a hold/automatic alternative.

## 11. Auto-Sprint

Optional Auto-Sprint can engage ordinary sprint from sustained forward movement according to current movement legality.

It does not bypass Heavy Load/environmental restrictions.

## 12. Controller Deadzones

Controller settings expose:

- left-stick inner deadzone;
- left-stick outer response;
- right-stick inner deadzone;
- right-stick outer response;
- trigger threshold where needed.

Values are user-adjustable within safe valid ranges.

## 13. Sensitivity

Separate sensitivity controls exist for:

- first-person look;
- ADS/zoom multiplier;
- spacecraft flight camera/aim;
- construction/strategic camera;
- controller and mouse where appropriate.

## 14. Axis Inversion

Horizontal/vertical look inversion is independently configurable where relevant, including spacecraft camera controls.

## 15. Camera FOV

First-person Field of View is adjustable over a broad safe range.

Initial design target:

- default vertical FOV: **75°**;
- adjustable vertical FOV: **60°–100°**.

Exact final safe bounds can be technically validated without changing the accessibility requirement.

## 16. Camera Shake

Camera shake uses a global **0–100% intensity** control.

At 0%, gameplay state/impact remains communicated through HUD/audio/haptics/animation without mandatory viewpoint displacement.

## 17. Head Bob

Head bob has a separate **0–100% intensity** control.

It may be fully disabled.

## 18. Weapon Sway

Cosmetic first-person weapon sway can be reduced without changing actual aim/spread mechanics.

If a sway is mechanically authoritative, the presentation cannot simply hide its gameplay effect; it must be represented by reticle/aim feedback.

## 19. Motion Blur

Motion blur is optional and **Off by default** in the baseline.

If implemented, it can be independently disabled.

## 20. Screen Distortion

Chromatic aberration, strong refraction overlays, damage warping, and anomaly distortion can be reduced through a Screen Distortion setting.

Critical anomaly state remains visible through alternate cues.

## 21. Photosensitivity Safe Mode

A dedicated mode reduces or removes:

- rapid flashing;
- strobing alarms;
- repeated high-contrast pulses;
- lightning frequency/intensity;
- full-screen flicker;
- aggressive emissive flashes.

It preserves warning semantics through steady light, icon, text, audio, and haptic alternatives.

## 22. Reduced Effects Mode

Reduced Effects decreases nonessential:

- particles;
- smoke density;
- debris;
- sparks;
- bloom;
- distortion;
- screen overlays.

Gameplay-critical hazard boundaries/telegraphs remain perceivable.

## 23. Brightness/Gamma

Display settings provide brightness/gamma calibration using reference imagery so dark environments remain intentional while shadow detail is not unintentionally crushed.

## 24. High Contrast

Optional High Contrast Gameplay presentation strengthens separation of:

- interactables;
- player squad;
- hostile identified actors;
- objective-relevant targets;
- hazards;
- critical UI.

It does not reveal unknown actors/objects.

## 25. Interaction Highlight

Interaction highlight strength is adjustable.

Options can include:

- Subtle;
- Standard;
- Strong;
- Persistent-on-focus-range where valid.

The setting never highlights undiscovered/inaccessible objects without a legitimate detection path.

## 26. Color Independence

No gameplay-critical state relies only on hue.

States combine at least one additional channel such as:

- icon;
- shape;
- pattern;
- text;
- animation;
- audio.

## 27. Color-Vision Presets

The game provides semantic color palette presets suitable for common color-vision differences, including support for:

- red/green differentiation difficulty;
- blue/yellow differentiation difficulty;
- low-saturation/monochrome readability.

The design favors remapping semantic UI colors rather than merely applying a full-screen simulation filter.

## 28. Custom Semantic Colors

Where practical, players can customize key HUD semantic colors such as:

- Friendly;
- Hostile;
- Neutral;
- Interactive;
- Objective;
- Warning;
- Critical.

Shape/icon semantics remain intact.

## 29. UI Scale

UI supports scalable presentation.

Initial target:

- global UI scale **80%–150%**;
- default **100%**.

Layouts must reflow/avoid clipping across supported range.

## 30. Text Scale

Body/subtitle text has an additional readable scale control independent from some icon/HUD elements where feasible.

## 31. Minimum Legibility

Critical text cannot be made so small by UI customization that it becomes unreadable at the supported reference resolution.

## 32. Font Readability

Default UI uses a high-legibility sans-serif family for functional text.

Decorative/faction typography is not used for long critical UI text.

## 33. Dyslexia-Friendly Option

An alternate high-legibility/dyslexia-friendly font option can replace functional UI/body text while preserving layout and terminology.

## 34. Subtitle Defaults

Spoken critical story/gameplay dialogue always supports subtitles.

First-launch setup explicitly asks whether subtitles should be enabled.

Recommended baseline default is **On**.

## 35. Subtitle Options

Configurable:

- text size;
- background opacity;
- speaker name;
- speaker color as secondary cue;
- maximum line width;
- closed captions;
- directional/source indicator where useful.

## 36. Closed Captions

Optional captions communicate relevant non-speech sounds without revealing inaudible/unknown events.

Examples:

- `[Hull breach alarm — Deck B]`;
- `[Pressure leak nearby]`;
- `[Hostile footsteps: right]` when actually audible;
- `[Robot command rejected]`.

## 37. Dialogue Priority

Critical dialogue can automatically duck Music/SFX to improve intelligibility.

A Dialogue Boost setting increases dialogue relative to noncritical audio.

## 38. Audio Categories

Players can independently adjust major audio categories defined in Audio Direction.

No critical state becomes inaccessible when one category is muted because visual alternatives exist.

## 39. Mono Audio

Mono audio output is supported.

Direction-dependent gameplay cues gain optional visual direction indicators.

## 40. Dynamic Range Presets

At minimum:

- Full;
- Standard;
- Night/Reduced Range.

This changes mix dynamics, not gameplay audibility to AI.

## 41. Tinnitus Reduction

Tinnitus/high-frequency damage effects can be disabled independently.

## 42. Haptics

Haptics have:

- master enable;
- intensity control 0–100%;
- optional separation of camera/impact/alert haptics where feasible.

No mechanic requires haptics.

## 43. Notification Duration

Noncritical notification duration can be increased.

Critical state remains available through persistent alert panels until resolved/acknowledged according to Alarm rules.

## 44. Tutorial Timing

Tutorial messages can use:

- pause-on-explanation;
- extended display duration;
- manual dismiss;
- contextual hints on/off.

No important tutorial disappears permanently because reading speed was slower than a fixed short timer.

## 45. Menu Time Behavior

Interfaces clearly indicate whether simulation is:

- Paused;
- Running.

Accessibility can optionally make selected complex planning screens pause in single-player **only where doing so does not create a gameplay exploit or contradict the owning system**.

True Pause always remains available.

## 46. Navigation Assistance

Optional Navigation Assistance can strengthen legitimate route guidance through:

- clearer tracked objective direction;
- breadcrumb/path hints in explored/known areas;
- stronger door/route highlighting;
- distance indication.

It cannot reveal undiscovered objectives, secret routes, or hidden enemies.

## 47. Objective Detail

Players can choose Objective Guidance:

- Minimal;
- Standard;
- Detailed.

Detailed provides more explicit wording based on already known information, not new world knowledge.

## 48. Interaction Assistance

Optional settings can:

- increase interactable focus range modestly;
- strengthen highlight;
- keep prompt visible longer;
- enable aim/focus snapping for noncombat small controls.

They cannot activate objects through walls or beyond physical interaction range.

## 49. Controller Aim Assist

Controller users can enable bounded aim assistance.

Permitted baseline assistance:

- reticle slowdown/friction near a legitimately visible hostile target;
- mild camera rotational assistance while tracking a target already under/near the reticle.

It does not:

- bend bullets;
- create hidden hit chance;
- snap through walls;
- reveal unknown enemies;
- auto-target weak points.

## 50. Mouse Aim Assist

Mouse aim assistance is Off by default.

Accessibility may allow the same bounded visible-target assistance if explicitly enabled by the player, without changing physical hit resolution.

## 51. Aim Assist Strength

Aim Assist strength supports:

- Off;
- Low;
- Standard;
- High.

The exact slowdown/rotation coefficients are tuneable.

## 52. Reticle Options

Reticle supports:

- size;
- thickness;
- opacity;
- center dot;
- high-contrast outline;
- semantic color preset/custom color.

Reticle changes do not modify spread.

## 53. Target Identification Assistance

An accessibility setting can display clearer labels/icons for already identified actors.

It cannot identify unknown actors earlier than sensors/perception allow.

## 54. Combat Damage Feedback

Players can reduce:

- damage vignette;
- screen flash;
- camera kick;
- low-health pulse.

Health/status remains visible through HUD/icon/audio alternatives.

## 55. Difficulty-Assisted Combat

If the player wants reduced gameplay pressure rather than presentation support, use Assisted Difficulty or Custom Difficulty axes.

Accessibility UI should explain the difference rather than hiding combat modifiers inside visual options.

## 56. Pause Anywhere

Because the baseline is single-player, true pause is available during ordinary gameplay, combat, missions, and raids except during unavoidable technical transitions where input is not active.

Pause does not advance Simulation Time.

## 57. Save Accessibility

Manual Save/Quick Save remain available under GDS-12 rules.

If a Stable Save Boundary is temporarily unavailable, the game queues the request and communicates that state.

This supports players who need to stop unexpectedly without inventing an unsafe mid-transaction snapshot.

## 58. Cognitive Load — Alert Filtering

Players can adjust noncritical notification categories.

P0/P1 safety-critical warnings cannot be completely hidden without an explicit high-risk warning in settings.

## 59. Cognitive Load — Information Density

HUD presets:

- Minimal;
- Standard;
- Expanded;
- Custom

allow players to reduce or increase routine visible information while mandatory critical state remains accessible.

## 60. Cognitive Load — Management Views

Complex station/ship/robot interfaces use:

- consistent hierarchy;
- filtering;
- search;
- one dominant diagnostic overlay at a time;
- explicit blocker lists;
- no unexplained abbreviations where avoidable.

## 61. Reading Support

Codex/log/tutorial text supports:

- text scaling;
- scroll speed independent from simulation;
- pause context;
- revisit/history.

## 62. Failure Readability

Failure summaries use direct causal language and can remain on screen until dismissed.

They do not require reading during live simulation.

## 63. Accessibility Presets

Optional presets provide starting configurations such as:

- Reduced Motion;
- High Readability;
- Hearing Support;
- Photosensitivity Safe.

Presets only modify the documented settings and remain individually editable afterward.

## 64. No Forced Accessibility Labeling

The game does not characterize players using accessibility settings as playing an invalid or lesser version.

## 65. Persistence

Accessibility, controls, subtitle, and display preferences persist at profile/application level where appropriate and apply across saves.

Difficulty remains save/campaign state according to GDS-12.

## 66. Reset

Every accessibility category has a reset-to-default action; presets can be reapplied without resetting campaign progression.

## 67. Testing Requirement

GDS-14 must include cross-system cases using:

- color-independent presentation;
- subtitles/captions only;
- camera shake 0%;
- reduced effects;
- remapped inputs;
- Detailed Objective Guidance;
- aim assist;
- UI scaling.

The specification must still communicate all required gameplay state.

## 68. Explicit Non-Goals

No reward penalties for accessibility, no bullet magnetism requirement, no accessibility wallhacks, no mandatory rapid presses, no unremappable core controls, no audio-only/color-only critical information, and no inaccessible save restriction during ordinary play.

## 69. Tuneable Parameters

Exact UI-scale bounds, FOV safe limits, aim-assist coefficients, deadzone ranges, caption timing, highlight strength, and reduced-effect intensity can be tuned after technical validation while preserving the fixed option semantics.

## 70. Dependencies

References GDS-1 Pause/Difficulty philosophy, GDS-5 controls/movement, GDS-9 combat, GDS-12 Difficulty/Save, and all GDS-13 presentation specifications.

## 71. Open Questions

None in the accessibility baseline.
