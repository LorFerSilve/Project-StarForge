# Audio Direction

> **Status:** Design Complete  
> **Authority:** Project-wide audio identity, ambience, machinery, weapons, UI feedback, spatial audio, vacuum handling, Pilot Telemetry Mix, protagonist-voice boundary, music behavior, dialogue intelligibility, and audio accessibility

## 1. Purpose

Audio communicates physical space, machine operation, danger, interaction, faction/location identity, and player action while remaining a truthful companion to—not a replacement for—visual/system feedback.

## 2. Core Audio Pillars

1. **Mechanical Causality** — audible state corresponds to a real source/system/event.
2. **Spatial Readability** — localizable cues remain spatially meaningful when physics/knowledge permits them.
3. **Dynamic State** — power, pressure, damage, combat, and station state change the mix.
4. **Restraint** — silence and low-density ambience are intentional tools.
5. **Redundancy** — critical audio always has a non-audio presentation path.
6. **Knowledge Integrity** — music/captions/UI audio cannot reveal hidden state beyond legitimate player knowledge.

## 3. Horizon Station Ambience

When corresponding physical sources are present/operational, the Horizon sound palette includes:

- ventilation and pumps;
- reactor/transformer/electrical hum;
- machinery cycles;
- cargo/logistics motion;
- player/crew footsteps;
- robots;
- localized crew activity;
- docking/mechanical impacts;
- PA/alarm announcements.

The ambience is source-driven. A missing/disabled source contributes no normal operating loop.

## 4. System-State Audio

System sounds change with authoritative state. Examples:

- unpowered machinery loses its powered loop;
- degraded/faulted equipment uses its documented degraded signature;
- atmosphere loss reduces air-carried local sound;
- emergency power has a distinct limited-operation signature;
- thermal stress can alter machinery audio before protective shutdown where the owning system exposes that state.

## 5. Vacuum

Open vacuum carries no ordinary distant air sound.

The player may hear only valid mediated/conducted cues such as:

- own suit breathing/life support;
- boot/tool/impact conduction through contacted structure;
- internal weapon/tool mechanism conducted through the player/equipment;
- radio/telemetry;
- suit-generated warning tones;
- structure-borne vibration while physically coupled.

A distant external explosion or engine does not create ordinary atmospheric sound for an EVA player.

## 6. Spacecraft Interior / Cockpit Mix

Inside a pressurized ship/cockpit, the mix includes valid:

- structure-transmitted engine/thruster vibration;
- life support;
- power/electrical systems;
- weapon mechanism/recoil/launch conduction;
- impacts;
- docking capture;
- alarms/comms.

Pressure/compartment changes affect air-carried interior sound where the ship simulation supports them.

## 7. Third-Person Pilot Telemetry Mix

The baseline third-person spacecraft camera **uses a Pilot Telemetry Mix**.

This is a deliberate synthesized pilot-feedback layer, not literal sound propagating through vacuum.

It may represent legitimately known ship/sensor/structure events through restrained:

- thruster/engine feedback;
- own weapon firing;
- detected weapon impacts;
- shield/hull impacts;
- detected nearby explosions;
- warning/target telemetry.

The mix must:

- never expose an undetected external event;
- never imply atmosphere-dependent occlusion/reverb in open space;
- remain more restrained/synthetic than pressurized cockpit audio;
- preserve the same mechanical capability in first- and third-person views.

## 8. Footsteps and Contact

Footsteps/contact vary by:

- surface material;
- footwear/Suit;
- gravity;
- movement state;
- atmosphere/exterior context.

Where GDS-9 AI hearing uses them, a corresponding gameplay sound event has an authored audibility/propagation profile.

## 9. AI Hearing Boundary

Player audio-volume/mix accessibility settings never change AI hearing.

AI hearing uses gameplay sound events and their physical/abstract propagation data, independent of how loud the user mixes them.

## 10. Weapon Audio Families

Canonical presentation differentiates at least:

- ballistic;
- shotgun/heavy ballistic;
- rail/electromagnetic;
- laser;
- plasma;
- explosive launcher;
- melee/contact.

Each weapon model's audio follows its actual architecture and firing/reload state rather than a generic gunshot layer.

## 11. Distance, Enclosure and Medium

Where atmosphere exists, weapon/impact sound may respond to:

- distance;
- enclosure/reverb;
- occlusion;
- medium/pressure;
- source architecture.

Vacuum follows Section 5/7 instead.

## 12. Impact Audio

Impact presentation differentiates, when legitimately observed:

- shield absorption;
- armor/metal impact;
- structural hull;
- rock/geology;
- glass;
- biological target;
- energy/electronics;
- explosion/debris.

Audio never confirms penetration/damage before the gameplay result commits.

## 13. Reload / Ready Audio

Reload and weapon-state audio aligns with the authoritative GDS-9 reload transaction stages.

A `ready`/reload-complete cue occurs only when the weapon has committed to the fire-ready state.

Empty/blocked/overheated firing has distinct feedback from a valid shot.

## 14. Player Health and Shield Audio

Audio may communicate actual:

- shield hit/collapse;
- armor/equipment impact;
- biological damage;
- Critical Health;
- Incapacitation;
- treatment/recovery;
- explicit Status Effects.

There is no hidden low-Health audio mechanic that changes gameplay. Heartbeat/tinnitus/distortion are presentation only and individually reducible/disableable under Accessibility.

## 15. Environmental Hazard Audio

Physically/audio-plausible hazards use source-driven cues such as:

- arcing;
- atmospheric leak;
- structural stress;
- radiation instrument telemetry;
- thermal alarms;
- corrosive machinery reaction;
- storms/weather.

A silent hazard remains mechanically valid and must rely on sensors/visual/UI where available.

## 16. Pressure Leak and Fire

A pressure leak produces air-flow sound only while local atmosphere supports it; the air-carried component diminishes toward vacuum while alarms/conduction may remain.

Fire sound/intensity follows actual fuel/atmosphere state and does not continue as unchanged atmospheric crackle in vacuum.

## 17. Robot Audio

Robot audio communicates actual class/body/system state through:

- locomotion mass/type;
- tool/weapon operation;
- energy warning;
- damage/Disabled state;
- communication state;
- command acknowledgement.

Heavy units use materially heavier mechanical presentation than Light units, without giving hidden combat information.

## 18. Robot Command Acknowledgement

Baseline command acknowledgement uses:

- one concise synthetic acknowledgement tone per accepted group command; and
- HUD confirmation.

A robot model may additionally use a short authored voice-code, but group commands aggregate such presentation so multiple units do not create simultaneous voice spam.

Rejected/blocked commands use a distinct failure tone and textual/icon reason where known.

## 19. Faction Audio Language

Faction content may distinguish itself through authored:

- UI/terminal tones;
- PA/radio processing;
- machinery maintenance character;
- ship/weapon technology sound;
- ambient/music instrumentation.

Shared identical technology remains recognizably related; faction identity cannot contradict physical source state.

## 20. Continuance and Anomaly Language

Continuance favors sparse machine-control rhythms, legacy infrastructure tones, synthetic signaling, and unusual nonhuman timing.

Fracture/Anomaly presentation uses restrained unstable harmonics/filtering/spatial interference tied to actual anomaly state.

Neither language defaults to painful high-frequency content or horror noise unrelated to gameplay state.

## 21. Player Protagonist Voice

The baseline has **no voiced conversational player dialogue**.

The player avatar may produce non-linguistic sounds:

- breathing;
- exertion;
- pain;
- impact reaction;
- Suit-mediated nonverbal cues.

These are presentation/embodiment only and do not speak dialogue choices or establish fixed narrative wording.

## 22. Music States

Music supports authored states including:

- Horizon calm/recovery;
- exploration;
- tension;
- combat;
- raid escalation;
- major discovery;
- narrative/finale.

The baseline does not run maximum-intensity score continuously.

## 23. Adaptive Music Knowledge Boundary

Music transitions may react only to state the presentation is permitted to telegraph.

Combat music does not begin solely because an undiscovered hostile exists nearby.

A mission may intentionally use music as an authored warning only when that warning itself is a legitimate player-facing signal.

## 24. Horizon Musical Development

Horizon ambience/music can gain additional authored warmth/complexity as the station becomes inhabited/stable, but this is a presentation response to known station/world state—not a hidden numeric Base Level meter.

The baseline implementation must support state-layering; individual musical assets/layers are content production.

## 25. Dialogue Intelligibility

Critical spoken dialogue remains intelligible through:

- dialogue-priority ducking;
- subtitles;
- communication log/replay where applicable;
- restrained radio processing.

Dialogue volume and subtitle availability are independent.

## 26. Radio / Communication

Radio audio is distinct from local world speech.

Filtering/dropouts/delay occur only when the actual Strategic Communication Link or local radio state provides that degradation.

Audio effects cannot fabricate a communication failure that gameplay does not have.

## 27. PA / Alarm Audio

Horizon PA can communicate actual:

- P0/P1 AlarmPriority incidents;
- boarding/lockdown;
- evacuation;
- docking where relevant;
- major system restoration/failure.

Routine low-priority work completion stays in notification/UI channels rather than constant PA announcements.

## 28. UI Audio

Canonical semantic UI cues exist for:

- navigation/focus;
- confirm;
- cancel;
- blocked/invalid;
- warning;
- transaction committed;
- save queued;
- save committed;
- objective update.

A success cue never precedes the authoritative commit.

## 29. Economy and Reward Audio

Ordinary Credits/item transactions use restrained confirmation.

The baseline avoids casino/loot-box-style audiovisual reward escalation for normal purchases, loot, or mission rewards.

## 30. Failure Audio

Routine setbacks use clear restrained feedback rather than humiliating/exaggerated punitive stingers.

Major story/finale scenes may use authored score/stingers consistent with the committed outcome.

## 31. Loudness Priority

Mix priority during contention is:

1. critical warnings and mandatory dialogue;
2. immediate threat/weapon/impact cues;
3. interaction/action feedback;
4. local machinery/footsteps;
5. ambience;
6. decorative distant detail.

Music yields dynamically to higher priorities.

## 32. Dynamic Range and Categories

The game provides dynamic-range presets:

- Full;
- Standard;
- Night / Reduced Range.

Independent user volume categories include:

- Master;
- Music;
- Dialogue;
- SFX;
- UI;
- Ambience;
- Voice/Radio where separated;
- Accessibility warning cues where technically separated.

## 33. Subtitles / Closed Captions

All gameplay-relevant speech supports subtitles.

Closed Captions may describe important audible non-speech events only when they are legitimately audible/known, e.g. hull-breach alarm, nearby footsteps, pressure leak, robot command rejection.

Captions never reveal an event outside actual player information range.

## 34. Spatial-Audio Accessibility

Important directional sound can have optional visual direction indicators where the event is legitimately audible/known.

This preserves event range/existence and changes only accessibility presentation.

## 35. Tinnitus / Damage Distortion

Tinnitus, muffling, heartbeat, and damage distortion are:

- bounded in duration/intensity;
- reducible/disableable;
- never the sole communication channel;
- never allowed to obscure mandatory dialogue/warnings irrecoverably.

## 36. Haptics

Audio may coordinate with haptics, but haptics are optional and never the sole carrier of critical state.

## 37. Persistence

Audio settings persist at profile/application level.

Gameplay incidents that drive audio persist in their owning systems; audio state reconstructs from those incidents after load rather than becoming a separate gameplay authority.

Music may persist enough presentation state to avoid jarring restart, but cannot alter gameplay.

## 38. Edge Cases

- Muting Master audio does not hide critical state because GDS-13 supplies visual/text alternatives.
- In vacuum, closed captions do not caption distant air sounds that the player could not receive; suit/radio/conduction cues may still be captioned.
- Switching spacecraft camera changes the mix representation but not ship simulation or detection.
- If a robot command is issued while communication is lost, no accepted-command cue plays unless/when the robot actually accepts it.
- If a transaction fails after confirmation input but before commit, a blocked/failure cue replaces success.

## 39. Tuneable Parameters

Tuneable values include mix levels, propagation/occlusion, reverb, sample variation, music transition timing, telemetry intensity, ducking, dynamic range, radio filtering, and nonverbal avatar vocalization frequency.

Vacuum behavior, Pilot Telemetry Mix existence, no-voiced-protagonist baseline, knowledge/commit boundaries, and critical redundancy are fixed.

## 40. Explicit Non-Goals

The baseline does not include:

- ordinary atmospheric sound propagation through open vacuum;
- omniscient combat music;
- audio-only critical mechanics;
- voiced player conversational lines;
- casino-style ordinary reward audio;
- false machinery loops from disabled equipment;
- mandatory tinnitus;
- constant PA spam.

## 41. Dependencies

References Station, Combat, Spacecraft, Robots, World/Factions, Narrative Dialogue, Raids, Strategic Communications, Interaction Feedback, Alarms/Signaling, and Accessibility.

## 42. Open Questions

None.
