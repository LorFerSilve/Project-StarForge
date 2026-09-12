# TA-11 — Subtitles, Captions, Alarms, and Accessibility Presentation

> **Status:** Architecture Complete  
> **Authority:** Subtitle/caption timing and eligibility, typed alarm presentation, acknowledgement, accessibility presentation transforms, visual-direction alternatives, photosensitivity/reduced-motion handling, and critical-channel redundancy

## 1. Purpose

StarForge must keep critical information perceivable when audio is muted, color differentiation is difficult, motion/flash is reduced, text/UI is enlarged, or controller/accessibility assistance is enabled—without revealing hidden information or changing gameplay difficulty/authority.

This document defines the shared presentation architecture for subtitles, Closed Captions, alarms, accessibility cues, and redundant warning channels.

## 2. Accessibility Invariant

Accessibility changes presentation/input support only unless a setting is explicitly part of GDS Difficulty.

It never directly changes:

- rewards;
- loot ownership;
- Research/Blueprint access;
- faction logic;
- mission state;
- save validity;
- hidden knowledge;
- physical hit resolution;
- AI hearing.

## 3. Subtitle Presentation Event

Gameplay-relevant spoken dialogue exposes a `SubtitlePresentationEvent` aligned with its `DialoguePresentationEvent` and containing conceptually:

- dialogue/content identity;
- speaker display identity where known;
- subtitle text/text reference;
- authored timing segments;
- language/text metadata;
- source/communication context;
- source SceneGeneration/event correlation;
- accessibility style role.

Subtitle text remains available independently of the user's Dialogue/Master volume.

## 4. Subtitle Timing Source

Subtitle timing is authored/cooked presentation metadata rather than guessed from frame count.

For ordinary voiced dialogue it may use:

- line start/end timing;
- phrase/segment timing;
- authored duration metadata;
- active playback clock when available.

If playback is unavailable/muted, subtitle timing uses the same presentation timeline metadata and does not disappear merely because no audio samples are emitted.

## 5. True Pause and Subtitles

When gameplay dialogue is paused by global True Pause, its subtitle timeline pauses with it.

Pause/menu UI text remains interactive through Presentation Time.

Resuming continues the existing subtitle line/segment rather than restarting gameplay dialogue unless the owning dialogue controller explicitly restarts it.

## 6. Subtitle Queue/Overlap

Subtitle presentation follows dialogue priority/correlation.

The system can:

- replace obsolete ambient chatter;
- prioritize mandatory dialogue;
- present a bounded number of concurrent speakers when authored;
- preserve required lines in a communication/log history where the GDS/content supports replay.

Numeric concurrency limits belong to TA-13.

## 7. Subtitle Options

Profile settings include at minimum:

- subtitles On/Off;
- text size;
- background opacity;
- speaker name visibility;
- speaker color as secondary cue;
- line-width/readability preference;
- Closed Captions On/Off;
- source/direction cue where legitimately known.

Recommended/default baseline remains subtitles On according to GDS-13.

## 8. Speaker Identity

Speaker name/color is shown only to the identity precision legitimately known to the player.

An unknown radio source can remain `Unknown Signal`/equivalent rather than leaking a hidden character identity.

Color is never the sole speaker distinction when identity matters.

## 9. Closed Caption Event

Closed Captions derive from an explicit `CaptionEligibleAudioEvent`/descriptor, never from arbitrary hidden audio emitters.

A caption descriptor contains:

- semantic caption text/token;
- source category;
- current audibility/knowledge eligibility;
- direction/source precision when allowed;
- priority;
- correlation/deduplication key.

## 10. Caption Eligibility

A non-speech event can be captioned only when the player could legitimately receive/know that event through the same physical/information path.

Examples:

- hull breach alarm;
- pressure leak nearby;
- footsteps to the right when actually audible;
- robot command rejected;
- ship warning tone.

An inaudible hidden enemy/event cannot become visible through captions.

## 11. Vacuum Caption Rule

Closed Captions obey TA-11/GDS vacuum rules.

A distant external explosion in open vacuum receives no atmospheric-sound caption merely because it is visible.

Valid suit/internal/radio/conduction/Pilot Telemetry cues can be captioned because those are legitimate received channels.

## 12. Directional Caption Cue

When a legitimately audible source has meaningful direction, the caption system may expose:

- Left/Right;
- Front/Behind;
- approximate directional icon;
- source/location label if already known.

The precision cannot exceed the actual audibility/knowledge source.

## 13. Caption Deduplication

Repeated continuous sounds use bounded caption repetition/aggregation.

A persistent leak does not spam a new caption every render frame.

Deduplication uses explicit cue/correlation identity and does not merge unrelated sources solely by text equality.

## 14. Alarm Projection

Gameplay incidents map to a presentation `AlarmReadModel` through authored deterministic mappings owned by the relevant domain/presentation builder.

The canonical namespace is exactly:

- `AlarmPriority::P0`;
- `AlarmPriority::P1`;
- `AlarmPriority::P2`;
- `AlarmPriority::P3`.

`AlarmPriority` remains a distinct type from `PowerLoadPriority`, `AutomationReportSeverity`, `NotificationPriority`, mission threat, or Health state.

## 15. Alarm Descriptor

An active alarm presentation descriptor contains conceptually:

- stable incident correlation/source identity;
- AlarmPriority;
- alarm family;
- scope/location precision;
- known cause/symptom text data;
- active/cleared state;
- acknowledgement state;
- parent/group key where explicitly supplied;
- known response/automation summary;
- source revision.

The descriptor is not a duplicate gameplay incident.

## 16. Alarm Families

Baseline alarm presentation families remain:

- Fire/Heat;
- Atmosphere/Pressure;
- Power/Electrical;
- Reactor/Critical Energy;
- Security/Intrusion;
- Structural/Hull;
- Medical/Crew;
- Ship Navigation/Flight;
- Environmental Exposure;
- Automation/Production;
- Communication/Signal.

Each family has icon/pattern/text/audio identity independent of color.

## 17. Alarm Scope

Scope is represented explicitly as:

- Local;
- Section/Deck;
- Station/Ship Global;
- Strategic/Remote.

Remote/global scope exists only when the underlying detection/control/communication path actually delivered that information.

## 18. Alarm Knowledge Boundary

Alarm detail is built from legitimately known diagnostics.

Known cause can produce specific text such as a failed coolant pump.

Unknown cause remains symptom-level such as `Thermal fault detected — source unresolved`.

UI/audio/caption code never queries privileged debug data to improve player alarm detail.

## 19. Alarm Acknowledgement

Acknowledgement is presentation state only.

It means the player has seen/acknowledged a specific alarm presentation instance/group.

It does not:

- repair/clear the incident;
- lower gameplay severity;
- cancel automation;
- change AlarmPriority itself;
- change mission/state consequences.

Acknowledgement may suppress repeated non-escalated banner/audio repetition according to policy.

## 20. Alarm Clear

An alarm clears only when the owning gameplay condition is resolved or no longer maps to an alarm-worthy state.

Closing a panel or acknowledging it cannot clear the condition.

If physical aftermath remains, a lower-priority advisory may continue as a separate mapped state.

## 21. Escalation/De-escalation

When authoritative source state changes, the next AlarmReadModel can change priority deterministically.

Escalation can re-arm attention even if an earlier lower-priority alarm was acknowledged.

De-escalation does not imply repair if the remaining source state is still degraded.

## 22. Alarm Deduplication

Related alarms group only when an authored grouping/parent key or owning-system relationship says they share one incident.

Presentation does not infer causality from timestamp coincidence.

A critical child consequence can remain separately visible even under a grouped parent.

## 23. Critical Redundancy

P0/P1 conditions use at least two suitable presentation channels, including a non-audio route.

Available routes include:

- HUD/banner;
- icon/text/pattern;
- map/overview marker;
- local/world indicator;
- audio/PA;
- Closed Caption;
- optional haptics.

No critical state relies solely on color, audio, flash, haptics, or camera motion.

## 24. Visual Direction Alternative

Direction-dependent important audio can emit a visual `DirectionalCue` only when the sound/event itself is legitimately audible/known.

The cue represents approximate source direction and cannot track an unknown actor after audibility ends unless another legitimate knowledge source maintains it.

## 25. High Contrast

High Contrast presentation can strengthen eligible:

- interactables;
- squad members;
- identified actors;
- objective-relevant targets;
- hazards;
- critical UI.

Eligibility is determined before the contrast transform; High Contrast cannot discover hidden targets/routes/items.

## 26. Interaction Highlight

Highlight strength supports Subtle/Standard/Strong plus the documented in-focus-range persistence behavior.

Highlighting consumes an already-valid `InteractionFocusView` and cannot extend interaction through opaque barriers or beyond the accessibility envelope supplied by the owning interaction system.

## 27. Color-Vision Support

Semantic color themes/presets are style-token transforms.

Critical meaning retains:

- icon;
- shape;
- pattern;
- label/state;
- optional audio/caption/haptic.

Changing theme does not change faction/target classification.

## 28. UI/Text Scale

UI/text scaling is applied through TA-11 layout/text systems.

Layouts reflow/scroll throughout the supported range rather than clipping:

- critical warnings;
- blocker reasons;
- subtitles/captions;
- irreversible confirmations;
- mandatory settings controls.

## 29. Reduced Motion

Reduced Motion can reduce/disable:

- decorative UI motion;
- camera shake;
- head bob;
- presentation-only weapon sway;
- large screen transitions;
- repeated pulsing.

It cannot remove authoritative recoil/spread/movement or hide state.

## 30. Photosensitivity Safe Mode

Photosensitivity Safe transforms/removes:

- rapid flashing/strobing;
- high-contrast repetitive pulses;
- aggressive full-screen flicker;
- unsafe high-frequency lightning/emissive effects.

Equivalent state remains through steady/slow visual, icon/text, audio/caption, and optional haptic routes.

## 31. Reduced Effects

Reduced Effects can lower nonessential particle/smoke/debris/spark/bloom/distortion presentation.

Gameplay-critical hazard boundaries, telegraphs, interaction cues, and objective readability remain available.

The setting cannot delete authoritative hazards/projectiles.

## 32. Camera/Screen Effect Reduction

Profile settings can reduce:

- damage vignette;
- screen flash;
- camera kick presentation;
- low-Health pulse;
- chromatic aberration/refraction/distortion;
- tinnitus/muffling presentation.

Any gameplay-relevant state stays visible/audible through other channels.

## 33. Tinnitus

Tinnitus/high-frequency damage effects can be completely disabled.

Disabling them does not alter damage, hearing gameplay events, AI, or status duration.

## 34. Objective/Navigation Assistance

Navigation/Objective Guidance consumes only known route/objective information.

Detailed guidance can expand instructions, legitimate route hints, known blockers and distances, but cannot reveal secret routes, unknown objective positions, hidden enemies, or unavailable access state.

## 35. Accessibility and Aim Assist

Aim-assistance UI/settings use the TA-11 input boundary.

Presentation can show enabled mode/eligible feedback but cannot fabricate hit confirmation or weak-point knowledge.

## 36. Alarm/Notification Filtering

Noncritical notifications are filterable.

P0/P1 alarms cannot be silently hidden through ordinary notification filtering.

If the product permits high-risk suppression, the user receives an explicit warning and at least one remaining accessible route to the condition.

## 37. Pause While Viewing

Any accessibility screen option labeled `Pause While Viewing` invokes global TA-1 True Pause.

It cannot create a local/partial pause.

## 38. Persistence Boundary

Subtitle/caption/accessibility/alarm-audio preferences are profile settings.

Alarm acknowledgement continuity may require persisted presentation metadata correlated to underlying persistent incidents; TA-12 defines representation.

Loading never clears/resurrects the underlying incident merely because presentation state is restored/rebuilt.

## 39. Accessibility Preset Expansion

Named presets modify explicit member settings and are reversible/inspectable.

There is no opaque accessibility mode that secretly changes gameplay rules.

## 40. Diagnostics

Development tools expose:

- subtitle/dialogue correlation/timing;
- caption eligibility/audibility reason;
- alarm source mapping/priority/scope/grouping;
- acknowledgement/escalation history;
- active accessibility transforms;
- directional cue source/precision;
- rejected hidden caption/marker attempts.

## 41. Validation Requirements

Tests must verify:

- muted audio leaves critical alarm state accessible;
- caption cannot expose inaudible hidden event;
- vacuum blocks invalid atmospheric caption;
- unknown speaker/cause remains unknown;
- alarm acknowledgement cannot clear source incident;
- AlarmPriority remains typed separately from other priority/severity namespaces;
- High Contrast cannot reveal unknown target;
- Photosensitivity Safe preserves semantics without rapid flash;
- UI scale extremes preserve critical content;
- Aim Assist/Reduced Motion do not change physical hit resolution;
- `Pause While Viewing` freezes whole Simulation.

## 42. Explicit Non-Goals

No baseline requirement for:

- audio-only/color-only/haptic-only critical state;
- omniscient captions;
- acknowledgement-as-repair;
- accessibility reward penalty;
- hidden target highlighting;
- bullet magnetism;
- partial pause;
- mandatory tinnitus/flash/camera shake.

## 43. Dependencies

Depends on TA-2 Knowledge Filter/read models, TA-6 alarms/station state, TA-8 perception, TA-9 strategic communications, TA-11 UI/HUD/audio/input settings, and GDS Accessibility/Alarms/Audio/HUD.

## 44. Open Questions

None in the subtitles/captions/alarms/accessibility presentation architecture.
