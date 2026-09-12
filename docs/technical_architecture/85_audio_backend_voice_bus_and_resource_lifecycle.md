# TA-11 — Audio Backend, Voice, Bus, and Resource Lifecycle

> **Status:** Architecture Complete  
> **Authority:** miniaudio adapter boundary, audio thread ownership, voice handles, buses, resource/decode/streaming classes, listener/emitter state, device changes, pause behavior, and audio failure degradation

## 1. Purpose

StarForge requires source-driven spatial audio, UI feedback, dialogue, music, radio, alarms, ship telemetry, ambience, and accessibility controls without allowing an audio callback, device timing, or user volume to become gameplay authority.

The baseline uses **miniaudio** behind a StarForge-owned `audio` module.

## 2. Backend Boundary

miniaudio provides commodity facilities for:

- device output;
- low-level mixing/render callback;
- decoding/stream support where selected;
- channel/sample conversion;
- spatialization primitives where useful.

Gameplay/UI code never calls miniaudio directly.

All public runtime audio APIs use StarForge-owned types.

## 3. Audio Authority Rule

Audio is presentation.

The audio subsystem never owns:

- AI hearing;
- target detection;
- weapon hit resolution;
- damage;
- equipment state;
- station faults;
- mission/objective state;
- Strategic Communication availability;
- resource ownership.

It represents authoritative/legitimately perceivable state supplied by owning systems.

## 4. Thread Model

TA-1 main thread owns semantic audio submission and presentation-state updates.

miniaudio may invoke a real-time audio callback on a backend/device thread.

The callback:

- never mutates gameplay state;
- never queries DomainStores/physics/UI;
- never waits for simulation workers;
- avoids unbounded allocation/locking/filesystem I/O;
- consumes prebuilt/prepared audio state through the audio adapter.

## 5. Main-Thread Audio Update

Each presentation frame the main thread can submit immutable updates such as:

- listener transform/profile;
- emitter transform/state;
- play/stop/update voice requests;
- bus/settings snapshot;
- music/dialogue state;
- pause snapshot;
- occlusion/medium parameters prepared outside the audio callback.

Backend timing changes playback only, never authoritative state.

## 6. AudioVoiceHandle

Active playback uses generation-checked `AudioVoiceHandle` values.

A voice handle is:

- process/session-local;
- nonpersistent;
- invalid after the voice slot is recycled;
- never a gameplay entity ID.

Persistent/scene sources are correlated through stable entity/content/event identities outside the backend handle.

## 7. Voice Classes

Baseline playback classes:

- short one-shot;
- looping world emitter;
- UI one-shot;
- dialogue/voice line;
- radio/communication voice;
- ambience bed/loop;
- music stem/track;
- long-form streamed source;
- synthetic/procedural tone where explicitly implemented.

The class controls resource/mixing behavior, not gameplay semantics.

## 8. Audio Content Resolution

Audio assets resolve through TA-10 `ContentId`/Content Registry.

TA-10 canonical WAV/FLAC source files are offline inputs; normal shipping playback consumes validated cooked audio products/profile metadata.

The runtime never scans source directories to find a similar sound when a required ContentId is missing.

## 9. Cooked Audio Runtime Profile

TA-11 defines two primary runtime residency classes:

- `ResidentClip` — short latency-sensitive cues decoded/prepared into memory;
- `StreamedClip` — longer dialogue/music/ambience content read/decompressed incrementally.

The exact encoded codec/container and quality parameters are pinned as TA-10/TA-16 implementation-profile data after compatibility/license testing.

Changing codec must not change ContentId or semantic cue identity.

## 10. Decode Boundary

Decode work can occur on bounded workers or miniaudio-backed resource workers if the implementation satisfies TA-1/TA-13 contracts.

Decoded PCM/result buffers are presentation resources only.

Decode completion order cannot decide which gameplay event occurred.

## 11. Audio Resource State

Canonical resource lifecycle:

`Unloaded -> Requested -> Preparing -> Ready -> InUse -> Retiring -> Released`.

Failure is typed and does not expose half-valid audio data as Ready.

Optional audio may degrade to silence while diagnostics record the failure; required dialogue/caption metadata must still preserve a non-audio route.

## 12. Voice Lifecycle

Canonical voice lifecycle:

`Pending -> Starting -> Playing -> Stopping/Fading -> Finished -> Reclaimed`.

A voice can additionally be:

- Paused by global pause snapshot;
- Virtualized when inaudible/nonessential and the semantic class permits it;
- Rejected/culled by resource/voice budget policy.

Presentation lifecycle never changes the underlying gameplay incident.

## 13. Voice Priority

Semantic voice priority follows GDS loudness importance:

1. critical warning/mandatory dialogue;
2. immediate threat/weapon/impact;
3. interaction/action feedback;
4. local machinery/footsteps;
5. ambience;
6. decorative detail.

TA-13 defines numeric voice budgets and virtualization thresholds.

Critical warnings/dialogue must not be silently stolen by decorative voices.

## 14. Bus Graph

The StarForge mixer exposes stable semantic buses:

- `Master`;
- `Music`;
- `Dialogue`;
- `SFX`;
- `UI`;
- `Ambience`;
- `VoiceRadio`;
- `Warnings` where critical/accessibility warning routing benefits from separation.

Sub-buses may exist internally, but settings/UI depend only on documented semantic buses.

## 15. User Volume

Each exposed user category applies gain to its bus under Master.

Changing bus gain:

- cannot alter AI hearing;
- cannot change knowledge/detection;
- cannot suppress gameplay incidents;
- cannot affect damage/weapon behavior.

Critical state always has a non-audio route.

## 16. Dynamic Range Profiles

Baseline profiles:

- Full;
- Standard;
- Night/Reduced Range.

They alter compression/limiting/mix headroom only.

Dynamic range selection does not change audibility for AI or the existence/range of gameplay sound events.

## 17. Dialogue Boost

Dialogue Boost applies a bounded presentation gain/ducking policy to dialogue relative to other buses.

It cannot amplify hidden/unreceived dialogue or radio traffic into existence.

## 18. Ducking

Project-owned mix snapshots can implement sidechain-like ducking for:

- critical dialogue;
- critical warnings;
- pause/menu presentation;
- major narrative sequences.

Music/ambience normally yield to higher-priority information.

Ducking is presentation only.

## 19. Listener

There is one primary local presentation listener for the baseline single-player process.

Listener state includes:

- origin-relative transform;
- velocity where useful;
- presentation profile;
- environment/medium context;
- output configuration.

It is reconstructed from the active scene/camera/control state and never serialized as gameplay identity.

## 20. Listener Profiles

Baseline profiles include:

- `FirstPerson`;
- `ShipCockpit`;
- `ShipThirdPersonTelemetry`;
- `UI/Menu` presentation.

The third-person ship profile follows GDS Pilot Telemetry Mix and is not literal vacuum camera sound.

## 21. Emitters

A world audio emitter references:

- source ContentId/cue;
- stable source entity/incident correlation where relevant;
- current origin-relative transform;
- source velocity if used;
- semantic category;
- spatialization/attenuation profile;
- medium/conduction context;
- loop/state parameters.

It cannot infer gameplay meaning from a filename.

## 22. SceneGeneration and OriginEpoch

Emitter/listener updates carry SceneGeneration and, where coordinate-sensitive, OriginEpoch.

Stale updates from an unloaded scene or prior origin frame are rejected.

Floating-origin rebases update presentation coordinates without restarting semantic loops or generating synthetic impacts/sounds.

## 23. Spatialization Baseline

Baseline world audio supports:

- distance attenuation;
- stereo directional spatialization/panning;
- doppler only where physically/presentationally appropriate and bounded;
- occlusion/filter parameters prepared by main-thread presentation queries;
- environment/reverb sends where authored.

A mandatory binaural/HRTF mode is not required by the baseline, but the adapter must not prevent adding one later.

## 24. Mono Output

Mono output is supported through final bus/output fold-down.

Direction-dependent critical gameplay cues also have visual/caption alternatives where legitimately known.

## 25. Output Device Changes

The audio adapter handles device initialization, default-device change, temporary loss, and explicit user device selection where the platform path supports it.

A device loss:

- never pauses/mutates gameplay by itself;
- enters a recoverable silent/degraded presentation state if possible;
- keeps semantic event processing bounded;
- reports diagnostics/UI status;
- reinitializes the backend safely when supported.

## 26. Audio Failure

If audio output cannot be initialized:

- the game can continue silently if no other technical dependency requires audio;
- subtitles/captions/visual warnings remain functional;
- a clear nonmodal/system warning is presented;
- gameplay/AI semantics remain unchanged.

Audio failure is not a mission failure.

## 27. True Pause

On global True Pause:

- ordinary gameplay SFX/ambience/world loops enter the project pause snapshot;
- gameplay-local dialogue is paused unless an authored/system line is explicitly part of pause UI;
- UI/menu sounds can continue;
- music may transition/duck/freeze according to presentation policy without advancing narrative/gameplay state;
- no Simulation-Time-driven audio envelope progresses as if gameplay were unpaused.

Resume does not replay stale one-shot impact/fire cues.

## 28. Scene Transition

During TA-3 scene transition:

- old scene-local emitters retire;
- cross-scene UI/music/dialogue resources survive only if their owner/lifetime says so;
- destination emitters activate after their legitimate scene/content state is ready;
- stale old-scene voice updates are rejected by generation.

## 29. Audio Cache Ownership

TA-11 audio module owns decoded/streaming CPU audio resource state.

TA-10 owns content identity/cooked asset discovery; miniaudio owns no persistent game content identity.

Audio cache eviction cannot remove gameplay state or affect procedural choices.

## 30. Streaming Under Load

If a long-form stream under-runs:

- presentation may glitch/fade/fail diagnostically;
- gameplay Simulation Time/state is not advanced/skipped to compensate;
- mandatory dialogue subtitle text remains available from independent content metadata;
- the system must not synthesize a successful narrative/gameplay event merely because playback ended/failed.

## 31. Audio Completion Events

Playback completion can notify presentation controllers for nonauthoritative sequencing.

Gameplay/story progression must not depend solely on an audio backend callback saying a file ended unless the owning narrative mechanic explicitly converted that playback timeline into an authoritative Simulation-Time contract outside the backend.

## 32. Shutdown Order

Controlled shutdown ensures:

1. semantic audio submissions stop;
2. scene/presentation voices retire;
3. streaming/decode work quiesces;
4. miniaudio device/callback stops;
5. audio resources/backends are destroyed;
6. process teardown continues.

No callback may access already-destroyed gameplay/UI state.

## 33. Diagnostics

Development diagnostics expose:

- output device/format;
- active voices by semantic priority/bus;
- virtualized/stolen voices;
- decode/stream state;
- bus gains/mix snapshot;
- listener profile;
- scene/origin generations;
- underruns/backend errors;
- audio memory estimates.

## 34. Validation Requirements

Tests must verify:

- audio callback never mutates gameplay;
- stale voice handle/generation rejection;
- scene unload retires scene-local voices;
- origin rebase does not create synthetic audio event;
- Master mute leaves gameplay identical;
- critical dialogue/warning priority cannot be stolen by decorative voice under test budget;
- output-device failure degrades without world mutation;
- True Pause does not replay queued world one-shots on resume;
- streamed playback failure cannot advance gameplay.

## 35. Explicit Non-Goals

No baseline requirement for:

- gameplay running on audio callback timing;
- AI hearing from mixed audio samples;
- raw miniaudio handles outside the audio module;
- mandatory HRTF;
- audio device failure as gameplay failure;
- runtime source-directory audio discovery;
- unlimited simultaneous voices.

## 36. Dependencies

Depends on TA-1 main/presentation threading, TA-2 Read Models/events, TA-3 scene/origin lifetime, TA-4 camera, TA-10 cooked content, GDS Audio/Accessibility, and later TA-13 numeric audio budgets.

## 37. Open Questions

None in the audio backend/voice/bus/resource architecture.
