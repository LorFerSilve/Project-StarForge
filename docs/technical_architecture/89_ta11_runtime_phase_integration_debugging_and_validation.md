# TA-11 — Runtime Phase Integration, Debugging, and Validation

> **Status:** Architecture Complete  
> **Authority:** End-to-end input/presentation phase ordering, Simulation Time versus Presentation Time, Read Model/event publication and consumption, True Pause behavior, stale-result rejection, presentation failure isolation, headless testability, and TA-11 diagnostics

## 1. Purpose

TA-11 spans several systems that run at different frequencies:

- platform input arrives with OS/render-frame timing;
- authoritative gameplay advances at the fixed 60 Hz Simulation tick;
- Read Models publish only at stable boundaries;
- UI/HUD/animation/VFX can update every rendered frame;
- audio output has a real-time backend callback;
- subtitles, fades and presentation transitions can use non-gameplay Presentation Time.

This document fixes the ordering and authority boundaries so none of those frequencies becomes hidden gameplay authority.

## 2. Core Authority Invariant

Only the authoritative simulation/domain phases may mutate gameplay state.

Input, UI, HUD, audio, captions, alarms, animation, camera, VFX and haptics can:

- produce semantic player requests;
- consume immutable state;
- present committed facts;
- maintain transient presentation state.

They cannot commit gameplay merely because a presentation callback, animation marker, audio completion, render frame, or UI transition occurred.

## 3. End-to-End Main-Loop Order

The baseline main-thread frame flow is conceptually:

```text
1. Pump platform/window/input events
2. Update RawInputState and presentation device state
3. If simulation tick(s) are due:
     a. sample semantic ActionSample for tick
     b. execute TA-1/TA-7 authoritative fixed-tick phases
     c. reach Stable Simulation Boundary
     d. publish immutable Read Models + committed PresentationEvents
4. Ingest newest presentation snapshots/events
5. Update shipping UI/HUD/tutorial/subtitle/alarm presentation
6. Resolve animation/camera/VFX presentation state
7. Submit audio semantic/listener/emitter/mix updates
8. Build TA-4 render snapshot/UI draw data
9. Render and present frame
10. Perform bounded nonauthoritative service/background work
```

The exact internal ordering of independent presentation-only updates can be optimized later provided the observable authority/causal rules remain unchanged.

## 4. Fixed-Tick Input Placement

TA-1 simulation phase 1 remains `Input Command Sampling`.

The TA-11 input resolver samples:

- current `InputContextStack`;
- effective profile bindings;
- current held/raw device state;
- latched press/release edges not yet consumed;
- accessibility input transforms.

It emits semantic action state/intents for exactly that simulation tick.

No render-frame UI update executes a gameplay action directly.

## 5. UI Commands During Presentation Frames

A UI interaction that occurs between simulation ticks can construct/enqueue an immutable typed Command/request for the next defined authoritative validation point.

The UI may immediately show nonauthoritative button/focus acknowledgement, but final success/failure is driven by the typed authoritative result.

A queued UI Command is not silently retried after rejection unless its owning gameplay contract explicitly provides retry semantics.

## 6. Command Correlation

Presentation-originated Commands carry a project-owned correlation/request ID suitable for matching:

- input/UI request;
- validation/rejection;
- committed transaction result;
- resulting presentation feedback.

Correlation IDs are diagnostic/presentation routing metadata unless another owning TA explicitly defines persistence.

They do not replace persistent gameplay IDs.

## 7. Stable Boundary Publication

At each relevant Stable Simulation Boundary, the main thread can publish a coherent presentation package containing conceptually:

- Read Model generations/revisions;
- committed presentation event batch;
- SceneGeneration/OriginEpoch where relevant;
- active settings snapshot generation;
- current global pause/session presentation status.

The package represents one coherent post-commit view.

Consumers never receive a partially committed cross-domain state.

## 8. Read Model Consumption

Presentation consumers use the newest complete compatible Read Model generation.

Examples:

- shipping UI -> management/HUD Read Models;
- HUD -> HudReadModel/AlarmReadModel/marker descriptors;
- audio -> AudioSemanticState/PlayerAudibilityReadModel;
- animation/VFX -> actor/action/presentation state;
- subtitles -> dialogue/caption events and settings.

Consumers cannot retain writable references to domain state.

## 9. PresentationEvent Batch

A `PresentationEventBatch` contains one-shot committed presentation facts produced since the prior publication point.

Events are ordered by:

1. simulation tick;
2. deterministic authoritative/domain event order;
3. event sequence/correlation.

A render frame may consume zero, one, or multiple batches, but cannot reorder them causally.

## 10. Exactly-Once Presentation Consumption

Each presentation consumer tracks the latest consumed event sequence/generation relevant to it.

A one-shot committed event such as:

- muzzle flash;
- hit confirmation;
- transaction confirmation;
- objective update;
- robot acknowledgement;
- docking capture cue;
- save committed notification

must not replay merely because the renderer presents the same stable Read Model for several frames.

## 11. Persistent State vs One-Shot Event

Presentation chooses its source by semantics:

- **persistent/continuous state** reconstructs from Read Models every frame/load;
- **one-shot transitions** come from committed event batches.

Examples:

- active fire/leak -> persistent Read Model state;
- impact spark -> one-shot event;
- current Health/Shield -> Read Model;
- shield-hit flash -> one-shot event;
- active P1 alarm -> AlarmReadModel;
- escalation banner/chime -> event derived from priority transition.

This prevents both missing persistent state and duplicate transient effects.

## 12. Simulation Time

Simulation Time is authoritative for:

- gameplay durations;
- interaction progress;
- mission timers;
- status durations;
- reload/action stages;
- strategic travel/events;
- station work;
- all other gameplay progression.

Presentation never extrapolates gameplay progress beyond the latest authoritative/interpolable state using wall time.

## 13. Presentation Time

A monotonic non-gameplay `PresentationTime` may drive presentation-only behavior including:

- cursor/focus animation;
- menu fades/transitions;
- tooltip delay;
- notification fade duration;
- UI hover effects;
- cosmetic crossfades;
- audio fades/ducking where independent of gameplay progress;
- optional animation blend smoothing that does not affect action authority.

Presentation Time is not serialized as gameplay progression and cannot advance Simulation Time.

## 14. Render Interpolation

TA-4 interpolation between stable simulation snapshots may smooth:

- actor/world transforms;
- camera presentation;
- animation pose parameters;
- other explicitly interpolable presentation data.

It cannot interpolate a discrete authoritative transition into an earlier gameplay fact.

For example, the renderer may interpolate ship position but cannot display `Hard Docked` before the committed docking state.

## 15. True Pause Entry

When True Pause is requested and activated at a Stable Simulation Boundary:

- no further authoritative simulation ticks run;
- gameplay action sampling/Commands do not advance world state;
- transient one-shot gameplay press edges are discarded as defined by TA-11 input policy;
- the newest stable Read Models remain visible;
- UI/system navigation continues using Presentation Time/input routing;
- gameplay-coupled animation/dialogue/subtitle/progress freezes;
- world audio enters the pause mix/snapshot;
- menu/UI presentation can continue.

## 16. True Pause Exit

On Resume:

- no stale one-shot gameplay input is replayed;
- stale impact/fire/haptic/VFX events are not regenerated;
- simulation resumes from the unchanged authoritative state;
- held-state semantics are re-evaluated by the input resolver;
- gameplay-coupled presentation continues from the resumed authoritative state.

## 17. Pause While Viewing

Any TA-11 `Pause While Viewing` feature is implemented only by the same global True Pause request/activation path.

There is no separate UI-local simulation clock or partial pause authority.

## 18. Scene Transition Integration

During TA-3 inter-context transition:

- system/loading UI can remain active;
- source SceneGeneration presentation objects retire when invalidated;
- destination presentation waits for required TA-10 content/readiness;
- old scene marker/audio/VFX/animation results cannot bind to the destination scene;
- destination gameplay becomes authoritative only after the TA-3 atomic context commit.

Presentation never unloads the source gameplay state first merely to show a loading screen.

## 19. SceneGeneration Validation

Scene-local presentation requests/results carry `SceneGeneration` where applicable.

A mismatch invalidates:

- runtime actor presentation references;
- marker projections;
- emitter/occlusion updates;
- one-shot scene VFX;
- animation markers;
- scene-local pointer/interaction targets.

The request/result is discarded rather than heuristically retargeted.

## 20. Runtime Entity Generation Validation

Presentation state tied to active runtime actors can additionally carry the generation-checked `RuntimeEntityHandle`/actor presentation generation.

A recycled runtime slot cannot receive stale:

- hit flash;
- animation notify;
- audio emitter update;
- health/status HUD attachment;
- world marker.

Persistent player-facing identity remains the typed persistent ID where appropriate.

## 21. OriginEpoch Validation

Spatial presentation jobs/results that depend on origin-relative coordinates carry `OriginEpoch` or use a stable coordinate representation explicitly convertible across rebases.

Old-epoch camera/audio/VFX/marker data is rejected or transformed only under a documented safe path.

A floating-origin rebase cannot produce a synthetic gameplay/presentation event.

## 22. ContentGeneration Validation

Hot-reloaded presentation content uses TA-10 `ContentGeneration`/fingerprint rules.

A stale worker or decode result cannot overwrite a newer:

- UI skin/icon/font;
- shader/material;
- animation asset;
- audio clip;
- presentation definition.

Gameplay/persistence-sensitive changes remain subject to TA-10 reactivation/restart classifications.

## 23. ReadModelGeneration Validation

UI controllers and presentation jobs tag requests with relevant Read Model/source revisions.

When a result/selection depends on stale state:

- pure presentation output may be dropped/rebuilt;
- a gameplay Command must still be revalidated by its owning domain;
- UI never assumes stale state is authoritative merely because the player saw it.

## 24. SettingsGeneration

Profile/settings publication uses a monotonic `SettingsGeneration`.

Presentation consumers can atomically adopt one coherent settings snapshot for:

- input transforms/bindings;
- UI scale/theme;
- HUD options;
- subtitle/caption options;
- audio bus gains/dynamic range;
- accessibility effects;
- haptics.

A frame/audio update does not combine arbitrarily half-applied old/new settings.

## 25. Main-to-Audio Handoff

The main thread prepares bounded immutable audio commands/state for the audio module.

The real-time callback consumes only backend-safe prepared state.

No callback can:

- issue gameplay Commands;
- query physics;
- inspect hidden DomainStore state;
- advance dialogue/mission authority;
- request save/load.

Audio thread failure/underrun is presentation diagnostics only.

## 26. Audio Callback-to-Main Notifications

The backend may publish bounded technical notifications such as:

- voice finished;
- stream underrun;
- device lost;
- backend error.

These enter a presentation/service queue.

They are never directly treated as gameplay facts.

A narrative/gameplay system cannot progress solely because a backend voice-finished notification arrived.

## 27. UI-to-Renderer Handoff

The shipping UI builds immutable/native-frame draw data from the retained UI tree after layout.

TA-4 renderer consumes:

- glyph/icon/quad draw descriptors;
- clip/scissor rectangles;
- transforms;
- material/resource handles;
- z/layer order.

Renderer cannot invoke widget/gameplay callbacks while executing draw commands.

## 28. Animation/VFX Handoff

Presentation resolvers consume current state/event batches and produce renderer/runtime presentation descriptors.

Animation marker/VFX completion notifications remain presentation-local unless an owning gameplay system already has an independent authoritative action timeline.

No renderer callback commits gameplay.

## 29. Headless Architecture

Core TA-11 logic must be testable without an OpenGL context or physical audio device.

Headless-testable units include:

- input binding/action/context resolver;
- conflict detection;
- settings validation;
- UI screen/controller logic against fixture Read Models;
- layout/focus/navigation calculations;
- marker eligibility/knowledge logic;
- notification/alarm grouping;
- subtitle/caption eligibility/timing logic;
- audio semantic routing and bus policy;
- music/dialogue state decisions;
- presentation-event deduplication;
- animation/VFX semantic trigger routing.

Backend-specific rendering/audio integration gets separate adapter smoke tests.

## 30. No Hidden Frame-Rate Semantics

The following cannot change authoritative gameplay based on render/audio frame rate:

- UI animation duration;
- marker update frequency;
- particle update frequency;
- audio callback frequency;
- controller prompt switching;
- subtitle render frame count;
- animation interpolation/blend rate.

Gameplay Commands and Simulation-Time state remain fixed-tick authoritative.

## 31. Presentation Failure Classes

TA-11 presentation failures are classified conceptually as:

- `OptionalPresentationDegraded` — e.g. cosmetic VFX/audio/icon missing but safe fallback exists;
- `RequiredPresentationUnavailable` — required readable UI/text/critical feedback resource unavailable;
- `BackendUnavailable` — e.g. audio device missing;
- `FatalInterfaceFailure` — player cannot safely continue/control the application/session.

Failure class determines presentation/system recovery, never a fabricated mission/gameplay outcome.

## 32. Optional Presentation Degradation

Optional failures can fall back to validated alternatives such as:

- silence for noncritical cosmetic audio;
- diagnostic/fallback texture/icon;
- omitted decorative animation/VFX.

The fallback cannot hide collision/gameplay geometry or required critical information.

## 33. Required Presentation Failure

If a required shipping UI/font/critical interaction presentation resource cannot be represented safely:

- the affected screen/context does not silently continue unreadable;
- a validated fallback resource/path is used if available;
- otherwise a controlled technical error is raised;
- gameplay is frozen/kept at a safe boundary when continuation would be unsafe.

It is never converted into character/mission damage/failure.

## 34. Audio Backend Failure

Audio device/backend failure follows TA-11 audio rules:

- switch to silent/degraded presentation where possible;
- preserve subtitles/captions/visual warnings;
- report user-facing technical status;
- gameplay and AI hearing remain unchanged.

## 35. UI Command Failure

A rejected gameplay Command is represented with the typed known reason from the owning domain/knowledge-filtered result.

The UI does not auto-adjust world state to make the request succeed.

## 36. Diagnostics Root

Development builds expose a consolidated TA-11 diagnostic root with sections for:

- Input;
- Effective Bindings/Settings;
- UI tree/layout/focus;
- HUD/markers/notifications;
- Tutorials/system screens;
- Audio backend/voices/buses;
- Player audibility/medium/occlusion;
- Dialogue/music;
- Subtitles/captions;
- Alarms/accessibility;
- Animation/camera/VFX;
- Presentation event queues/generations.

Dear ImGui can host these inspectors because it remains development-only tooling.

## 37. Input Diagnostics

Input inspector includes:

- connected devices;
- raw/transformed axes;
- edge latch;
- active InputContextStack;
- effective ActionId bindings;
- current ActionSample;
- context consumption;
- prompt-device family;
- aim-assist eligible candidates/output.

Privileged target information is clearly marked and never routed into shipping HUD.

## 38. UI Diagnostics

UI inspector includes:

- active screen stack;
- focus owner/path;
- pointer capture;
- layout rectangles/clipping;
- UI/text scale and safe area;
- glyph atlas stats;
- source Read Model revisions;
- pending Command correlation/results;
- preview vs committed state markers.

## 39. HUD/Alarm Diagnostics

Inspector includes:

- marker knowledge/precision/source;
- through-wall eligibility reason;
- notification grouping keys;
- active AlarmPriority/scope/family;
- acknowledgement/escalation transitions;
- rejected hidden-information presentation attempts.

## 40. Audio Diagnostics

Audio inspector includes:

- backend device/format/state;
- active/virtualized/rejected voices;
- buses/gains/mix snapshot;
- listener profile;
- PlayerAudibility reasons;
- medium/occlusion/reverb data;
- dialogue queue;
- music state/transition reason;
- underrun/failure counters.

## 41. Animation/VFX Diagnostics

Inspector includes:

- actor animation presentation state;
- action correlation/generation;
- stale marker/notify rejection;
- camera effect stack/accessibility scaling;
- active persistent/one-shot VFX;
- event-consumption sequence;
- ContentGeneration/SceneGeneration/OriginEpoch tags.

## 42. Shipping Logging

Important TA-11 diagnostics route through the project logging facade with typed categories and stable context IDs/generations where useful.

Shipping builds avoid high-volume per-frame logs but retain actionable errors for:

- device/backend failure;
- missing required presentation content;
- invalid settings/bindings;
- content/interface mismatch;
- fatal UI state;
- repeated stale-result anomalies.

## 43. Deterministic Fixture Tests

TA-14 later defines CI jobs, but TA-11 requires deterministic fixtures for:

- input sequences over fixed ticks/context transitions;
- Read Model -> UI command scenarios;
- marker knowledge/precision;
- alarm grouping/escalation;
- subtitle/caption audibility;
- audio semantic state transitions;
- music hidden-hostile rejection;
- animation/VFX committed-event routing.

Identical fixture input/state must produce the same semantic output independent of render/audio timing.

## 44. Integration Tests

Later implementation tests must include representative flows such as:

- keyboard/controller switching while interacting/UI is open;
- pause during combat and queued pause around transaction boundary;
- remapped controls shown in tutorial/interaction prompts;
- inventory transfer commit followed by correct ownership feedback;
- mission marker changing SearchArea -> Exact only after legitimate discovery;
- EVA vacuum with valid/invalid audio/captions;
- controller disconnect/reconnect;
- alarm escalation while station UI is open;
- audio device loss during gameplay;
- scene transition with stale presentation jobs;
- Reduced Motion/Photosensitivity/High Contrast combinations.

## 45. Accessibility Regression Matrix

Required regression combinations include at minimum the GDS cases:

- color-independent presentation;
- subtitles/Closed Captions with reduced/muted audio;
- Camera Shake 0%;
- Head Bob 0%;
- Reduced Effects;
- Photosensitivity Safe Mode;
- remapped inputs;
- Detailed Objective Guidance;
- UI scale at supported extremes;
- Aim Assist High without altered hit resolution.

## 46. Security/Knowledge Regression

Automated/headless tests must specifically ensure presentation cannot leak:

- hidden enemies through Aim Assist, markers, captions or audio;
- unknown mission/objective coordinates;
- undelivered strategic events/alarms;
- hidden diagnostic causes;
- exact state through a tooltip when the Read Model exposes only uncertainty.

## 47. Commit-Timing Regression

Tests verify success presentation occurs only after commit for:

- pickup/ownership transfer;
- reload completion;
- repair completion;
- docking Hard Dock;
- robot command acceptance;
- trade/Credits transfer;
- objective completion;
- save completion.

## 48. Performance Boundary

TA-11 defines semantic architecture, not final numeric budgets.

TA-13 must establish measurable budgets for:

- UI layout/text shaping;
- glyph atlases;
- marker/notification counts;
- audio voices/streams/decode;
- occlusion queries;
- presentation-event queues;
- animation/VFX work;
- input/controller processing.

Budget pressure may degrade optional presentation only and cannot change gameplay outcomes.

## 49. Persistence Boundary

TA-11 identifies which state is profile/session/presentation/gameplay authority but does not define exact on-disk bytes.

TA-12 owns:

- profile settings persistence format;
- optional alarm acknowledgement continuity representation;
- tutorial preference/history representation;
- save/load status integration;
- any safe presentation continuity checkpoint data.

Runtime handles, widget IDs, voice handles and raw input state remain nonpersistent.

## 50. CI Boundary

TA-14 owns concrete CI enforcement, including:

- headless TA-11 tests;
- text/layout fixtures;
- content/interface checks;
- backend smoke tests where feasible;
- accessibility/knowledge/commit-timing regressions;
- warnings/static analysis/sanitizers.

TA-11 provides the invariants those gates must test.

## 51. Implementation-Lock Boundary

No C++/OpenGL/audio/UI scaffolding is authorized merely because TA-11 is Architecture Complete.

TA-15 must perform the full architecture integration audit and TA-16 must produce implementation sequencing/contract locking before implementation begins.

## 52. Validation Summary

TA-11 is internally complete when all owning specs guarantee:

- named semantic input only;
- deterministic fixed-tick input consumption;
- one explicit UI focus/context owner;
- knowledge-filtered immutable presentation state;
- commit-synchronized success feedback;
- no audio/render/presentation callback authority;
- AI-hearing/user-audio separation;
- vacuum/telemetry/communication correctness;
- typed AlarmPriority and accessibility redundancy;
- animation/VFX/camera presentation-only boundaries;
- global True Pause semantics;
- headless semantic testability;
- stale generation/revision rejection.

## 53. Explicit Non-Goals

No baseline requirement for:

- presentation running authoritative gameplay;
- render/audio-frame-dependent mechanics;
- serialized widget/voice/runtime presentation handles;
- partial pause;
- direct OpenGL/miniaudio calls from gameplay/UI widgets;
- animation/audio/VFX completion as generic gameplay trigger;
- privileged debug state in shipping player presentation.

## 54. Dependencies

Depends on TA-1 through TA-10, every TA-11 owning specification `80` through `88`, and the Design Complete GDS Player Controls + Presentation/Accessibility domain.

## 55. Open Questions

None in the TA-11 runtime integration/debugging/validation architecture.
