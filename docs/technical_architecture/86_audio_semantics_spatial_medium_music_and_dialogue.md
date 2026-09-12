# TA-11 — Audio Semantics, Spatial Medium, Music, and Dialogue

> **Status:** Architecture Complete  
> **Authority:** Semantic audio event routing, player-audibility boundary, vacuum/conduction/telemetry rules, occlusion/reverb, AI-hearing separation, adaptive music, dialogue/radio playback, and commit-synchronized audio feedback

## 1. Purpose

Audio must be physically/systemically truthful enough to communicate space and machinery while also respecting the GDS Knowledge Boundary, vacuum rules, Pilot Telemetry Mix, dialogue intelligibility, and non-omniscient adaptive music.

This document defines the semantic layer above the backend/resource contracts in `85_audio_backend_voice_bus_and_resource_lifecycle.md`.

## 2. Gameplay Sound vs Player Audio

StarForge separates two concepts:

1. **GameplaySoundEvent** — authoritative/semantic event used by gameplay systems such as AI hearing;
2. **PresentationAudioEvent** — player-facing audible/presentation request.

They may originate from the same physical action but are not the same object or authority.

User volume/mute settings affect only PresentationAudioEvent playback.

## 3. AI Hearing Independence

AI hearing uses TA-8 gameplay sound propagation/knowledge contracts.

It never samples:

- miniaudio output;
- user bus gain;
- headphones/speaker configuration;
- dynamic-range preset;
- dialogue ducking;
- accessibility mute settings.

Muting footsteps does not make the player silent to AI.

## 4. Presentation Audibility Feed

The active scene publishes an immutable `PlayerAudibilityReadModel`/equivalent containing only sound-producing events/sources eligible for player presentation.

Eligibility can be established by:

- actual local physical audibility;
- suit/structure conduction;
- radio/telemetry reception;
- known UI/system feedback;
- authored narrative/music presentation state.

The audio subsystem does not query every hidden entity to discover sounds.

## 5. Audibility Can Create Knowledge

A legitimately audible sound does not require the source to have been visually identified first.

For example, audible footsteps can become a legitimate player cue and can update presentation/perception knowledge through the owning gameplay perception path.

The important rule is that the sound must be physically/informationally eligible, not that it was already visually known.

## 6. Medium Profile

Each spatial source/listener relation resolves an `AudioMediumProfile` from authoritative environment state, conceptually including:

- pressurized atmosphere/air;
- thin atmosphere;
- vacuum;
- structure-contact/conduction;
- suit/internal;
- radio/telemetry.

The audio subsystem cannot invent pressure or communication connectivity.

## 7. Vacuum Rule

Open vacuum carries no ordinary distant air sound.

A source in vacuum is audible only through a valid mediated path such as:

- player suit/internal mechanism;
- direct/structure conduction while coupled;
- radio/communications;
- Pilot Telemetry Mix for eligible ship events;
- explicit instrument/sensor warning.

A visually large explosion does not automatically create an atmospheric boom in EVA.

## 8. Pressure Transition

When local pressure changes, air-carried sound parameters respond to the authoritative environment state.

A pressure leak/fire loop can diminish/stop as atmosphere is lost while conduction, alarm or suit telemetry remains according to its own source path.

Audio state does not itself change pressure/fire simulation.

## 9. Structure Conduction

Structure-conducted cues require a valid physical/contact/coupling relation exposed by gameplay/physics presentation data.

Examples:

- Magnetic Boots/contact footsteps;
- tool impacts on held/touched structure;
- ship hull impacts;
- docking capture;
- machinery vibration transmitted through the occupied structure.

The presentation layer may simplify acoustic transfer but cannot invent a conductive path across disconnected space.

## 10. First-Person Listener

For ordinary on-foot first-person play, listener position/orientation follows the presentation camera/head reference in current origin-relative space.

Gameplay hit/interaction origins remain their owning TA-5/TA-7 authorities and are not derived back from audio listener state.

## 11. Ship Cockpit Mix

Cockpit/interior presentation combines valid:

- engine/thruster structure vibration;
- life support/power;
- weapon mechanism/recoil/launch conduction;
- impacts;
- docking;
- alarms/comms;
- pressurized interior ambience.

Pressure and subsystem state can modify layers when those states are legitimately known/represented.

## 12. Third-Person Pilot Telemetry Mix

Third-person spacecraft view uses the GDS **Pilot Telemetry Mix**.

It is a synthesized pilot-feedback representation, not sound emitted into vacuum at the camera location.

Eligible layers can include known/detected:

- own propulsion;
- own weapons;
- shield/hull impacts;
- detected nearby explosions;
- target/warning telemetry.

Undetected external events cannot enter this mix.

## 13. Camera Switch

Switching cockpit/third-person presentation can change mix/profile/filtering immediately or through a short transition.

It cannot change:

- ship simulation;
- sensor detection;
- weapon behavior;
- communication state;
- event existence.

## 14. Distance Attenuation

Each spatial cue family has an authored attenuation profile appropriate to its source class.

Distance falloff is presentation tuning and must remain consistent with the already-determined player-audibility eligibility.

An audio gain curve cannot extend knowledge beyond the semantic audibility boundary.

## 15. Occlusion

Presentation occlusion is computed from bounded main-thread/worker-safe scene query data, never inside the real-time audio callback.

A cue can receive attenuation/filtering/reverb adjustments based on known blocking geometry/portals.

Occlusion is a presentation refinement; it does not alter AI hearing unless TA-8 independently applies corresponding gameplay propagation rules.

## 16. Occlusion Query Staleness

Occlusion results carry source/listener/SceneGeneration and where needed OriginEpoch/revision.

Stale results are discarded.

A stale worker result cannot suddenly reveal/suppress a cue from an unrelated new scene.

## 17. Reverb/Acoustic Zones

Authored/cooked scene presentation data can expose acoustic zones such as:

- small/large interior;
- metallic corridor;
- hangar;
- exterior atmosphere;
- vacuum/no-air-reverb;
- ship cockpit/interior.

Zone/reverb transitions are presentation only and do not define pressure topology.

TA-6 atmosphere remains authoritative.

## 18. Source-Driven Machinery

Machinery loops reflect actual known/active source state.

Examples:

- unpowered equipment loses powered loop;
- degraded equipment can use a defined degraded layer;
- emergency power has its own presentation;
- inactive/destroyed source does not continue normal operating audio.

The audio system consumes source state; it does not decide operating state.

## 19. Footsteps

Footstep presentation can select variants by legitimately known:

- collision surface material;
- footwear/Suit;
- gravity/movement state;
- atmosphere/exterior context.

Animation timing may align the audible footstep, but an animation marker never becomes AI-hearing or locomotion authority.

## 20. Weapon/Impact Audio

Weapon and impact cues are driven by actual weapon/action/commit facts.

They can distinguish weapon architecture and material/shield/armor interactions where legitimately confirmed.

A hit/penetration/weak-point success cue cannot play before the corresponding gameplay result exists.

## 21. Reload Audio

Reload sounds align with the authoritative reload stages.

A ready/reload-complete cue occurs only after the weapon state actually commits to fire-ready.

Interrupted reload does not play a false completion cue.

## 22. Interaction/UI Semantic Audio

Semantic UI/action cues include:

- navigation/focus;
- confirm;
- cancel;
- blocked/invalid;
- warning;
- transaction committed;
- save queued;
- save committed;
- save failed;
- objective update;
- robot command accepted/rejected.

Success cues are emitted from typed committed results, not button-down alone.

## 23. Robot Command Audio

Accepted group commands produce one concise aggregate acknowledgement tone plus HUD state.

Optional authored voice-codes are deduplicated to avoid squad spam.

Rejected/blocked/disconnected state uses distinct feedback only after the owning command system reports it.

## 24. Alarm/PA Audio

Alarm/PA presentation consumes the TA-11 alarm projection.

P0/P1 incidents can request distinct warning/PA cues appropriate to known scope/family.

Routine P3 work-completion notifications do not automatically produce station-wide PA announcements.

## 25. Dialogue Event

Spoken content is represented by a `DialoguePresentationEvent` containing conceptually:

- dialogue line/content identity;
- speaker identity/display data where known;
- local/radio channel kind;
- audio ContentId;
- subtitle track/text reference;
- expected timing metadata;
- source/mission/narrative correlation;
- knowledge/communication delivery state;
- priority/interrupt policy.

The audio file itself does not own story progression.

## 26. Local Dialogue

Local speech requires the speaker/source to be legitimately present/audible under the scene/medium rules.

If speech is gameplay-relevant, subtitle presentation is available independently of user dialogue volume.

## 27. Radio Dialogue

Radio/communication audio requires a valid delivered communication event from the owning communication system.

Filtering/dropout/delay is derived from actual communication quality/state when provided.

The audio layer cannot fabricate a communications outage for drama.

## 28. Protagonist Voice Boundary

The baseline player protagonist has no voiced conversational dialogue.

Nonverbal breathing/exertion/pain cues are presentation events only and cannot establish dialogue choices or narrative text.

## 29. Dialogue Interruption

Dialogue can declare presentation interrupt policy such as:

- noninterruptible critical line;
- queue/duck lower-priority dialogue;
- replace obsolete low-priority chatter;
- stop because source/communication legitimately ended.

Narrative consequences do not depend solely on whether the audio voice reached its final sample.

## 30. Dialogue Queue

The dialogue presentation queue is priority/correlation aware and bounded.

It can suppress obsolete ambient chatter but cannot drop a required narrative line without an alternate delivery path such as subtitle/log/replay according to content design.

TA-13 defines numeric queue/voice limits.

## 31. Music State

Music uses a project-owned `MusicPresentationState`/state machine with authored states such as:

- Horizon calm/recovery;
- exploration;
- tension;
- combat;
- raid escalation;
- major discovery;
- narrative/finale.

The music system owns crossfades/stem presentation, not the gameplay conditions themselves.

## 32. Music Knowledge Filter

Music transitions can react only to presentation-approved/known state.

An undiscovered hostile cannot start combat music solely because it exists nearby.

If an authored mission intentionally uses music as a warning, the mission must expose that warning as legitimate player-facing presentation state.

## 33. Music Transition Timing

Crossfades and stem envelopes use non-gameplay presentation timing while respecting True Pause/presentation snapshots.

Their timing cannot advance Simulation Time or decide combat/mission state.

## 34. Horizon Music Development

Horizon music/ambience can layer additional authored warmth/complexity in response to known station recovery/inhabitation state.

This uses explicit source state and does not create a hidden generic Base Level.

## 35. Audio and Save/Load

Gameplay incidents that drive audio persist in their owning systems.

After load, audio reconstructs appropriate loops/music/dialogue availability from loaded state/read models.

Transient one-shot sounds are not serialized as gameplay history solely to replay them.

A bounded presentation checkpoint may preserve music/dialogue continuity where safe, but it cannot alter gameplay.

## 36. True Pause

When paused:

- Simulation-Time source state is static;
- world one-shots are not generated repeatedly;
- gameplay dialogue playback normally pauses to preserve subtitle/audio sync;
- pause UI sounds remain available;
- music can use an authored pause snapshot without telegraphing new hidden state.

## 37. Failure/Recovery Audio

Routine mission/setback feedback is restrained and follows committed outcome.

No punitive/casino-like stinger is required.

Major narrative/finale score/stingers are permitted only for committed authored outcomes.

## 38. Reward Audio

Ordinary Credits/item/reward transactions use restrained confirmation.

The baseline does not use loot-box/casino escalation for normal rewards/purchases.

## 39. Diagnostics

Development diagnostics expose:

- GameplaySoundEvent vs PresentationAudioEvent traces;
- player-audibility reason/path;
- medium profile;
- occlusion query/result generation;
- listener profile;
- current music state and transition reason;
- dialogue queue/priority/correlation;
- rejected hidden/invalid audio events.

## 40. Validation Requirements

Tests must verify:

- user volume does not alter AI hearing;
- vacuum rejects ordinary distant atmospheric cues;
- valid conduction/radio/telemetry can remain in vacuum;
- third-person ship mix cannot expose undetected event;
- audio hit/reload success follows committed result;
- stale occlusion result is rejected;
- radio dialogue requires delivered communication;
- combat music cannot trigger from hidden hostile truth;
- playback completion cannot independently progress gameplay;
- protagonist conversational voice remains absent by baseline.

## 41. Explicit Non-Goals

No baseline requirement for:

- physically exact wave simulation;
- audio samples as AI-hearing authority;
- atmospheric sound in open vacuum;
- omniscient adaptive music;
- voiced conversational protagonist;
- gameplay progression from raw audio-file completion;
- audio-side station/pressure simulation.

## 42. Dependencies

Depends on TA-2 Knowledge Filter/events, TA-3 scene/origin, TA-4 camera, TA-5 collision/query facts, TA-6 atmosphere, TA-8 AI hearing, TA-9 communications/missions, TA-11 audio backend/HUD, and GDS Audio Direction/Accessibility.

## 43. Open Questions

None in the audio semantic/spatial/music/dialogue architecture.
