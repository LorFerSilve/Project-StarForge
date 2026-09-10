# Audio Direction

> **Status:** Draft  
> **Authority:** Project-wide audio identity, ambience, machinery, weapons, UI feedback, spatial audio, vacuum handling, music behavior, dialogue intelligibility, and audio accessibility

## 1. Purpose

Audio must communicate physical space, machine operation, danger, interaction, faction/location identity, and player action while supporting—not replacing—visual feedback.

## 2. Core Audio Pillars

1. **Mechanical Causality:** sounds come from understandable physical/system events.
2. **Spatial Readability:** the player can localize relevant threats and machinery.
3. **Dynamic State:** ambience changes when power, pressure, combat, or station state changes.
4. **Restraint:** silence and low-density ambience are used deliberately.
5. **Redundancy:** gameplay-critical audio has a non-audio presentation route.

## 3. Station Ambience

Horizon Station baseline ambience can include:

- ventilation;
- pumps;
- transformer/reactor hum;
- machinery cycles;
- distant cargo movement;
- footsteps;
- robots;
- crew activity;
- docking/mechanical impacts;
- public-address announcements.

The soundscape should make the station feel inhabited and operational.

## 4. System-State Audio

Station sounds respond to actual state.

Examples:

- unpowered module loses machinery hum;
- degraded pump develops unstable mechanical rhythm;
- atmosphere loss reduces air-carried sound locally;
- emergency power has a distinct lower-capacity electrical signature;
- overloaded cooling/industrial equipment becomes audibly stressed.

## 5. No False Machinery

A disabled reactor, fan, pump, or turret cannot continue playing a normal operating loop purely for ambience.

## 6. Vacuum Audio

External vacuum does not carry ordinary environmental sound.

The player can still hear physically transmitted or suit-mediated cues such as:

- own suit/foot contact;
- internal breathing/life support;
- structure-borne vibration when physically coupled;
- radio communication;
- weapon/internal mechanism where conducted to the player;
- intentionally synthesized suit warning cues.

No normal distant explosion/engine sound propagates through open vacuum as air sound.

## 7. Spacecraft Interior Audio

Inside a pressurized ship, the player hears:

- engine/drive structure vibration;
- thrusters through hull conduction;
- life support;
- power systems;
- weapon recoil/launch mechanisms;
- docking capture;
- alarms;
- impacts transmitted through structure.

## 8. Spacecraft Exterior Camera Audio

Third-person ship view may use a stylized **pilot telemetry mix** rather than literal external-vacuum silence.

This mix represents information available to the pilot through ship sensors and structure and can include restrained synthesized weapon/thruster/impact cues.

It must remain clearly less like atmospheric cinema sound than interior/cockpit audio.

## 9. Footsteps

Footsteps vary by:

- surface material;
- suit/footwear;
- gravity;
- movement speed;
- interior/exterior state.

They are useful for both player embodiment and AI hearing where GDS-9 uses audible cues.

## 10. AI Hearing Boundary

A sound used by enemy AI hearing must correspond to a gameplay sound event with an authored audibility radius/propagation rule.

Audio mix volume settings do not change AI hearing.

## 11. Weapon Families

Weapon audio uses distinct mechanical/energy signatures:

- ballistic: pressure/transient/mechanical action;
- shotgun: heavier transient and action;
- rail: electromagnetic charge/discharge + impact character;
- laser: sharp coherent energy signature, not generic gunpowder report;
- plasma: charged thermal/energy pulse;
- explosive launcher: launch/backblast + delayed impact;
- melee: material/contact-driven.

## 12. Suppression and Distance

Weapon sound changes with:

- distance;
- enclosure;
- atmosphere;
- occlusion;
- weapon architecture.

No single sample at constant loudness represents every range.

## 13. Impact Audio

Impacts differ across:

- shield;
- armor/metal;
- rock;
- glass;
- biological target;
- energy/electronics;
- hull/structure.

Shield hit confirmation should sound distinct from armor penetration or unshielded impact.

## 14. Reload and Weapon-State Audio

Reload timing audio aligns with actual transaction stages where meaningful.

Examples:

- magazine/round manipulation;
- chamber/bolt action;
- energy-cell insertion;
- capacitor ready tone;
- overheat warning.

A completed-ready sound cannot occur before the gameplay state is ready.

## 15. Low/Empty Ammo

Low ammo can use restrained UI/weapon cues.

Empty/failed fire must clearly differ from normal firing.

## 16. Player Health and Shield

Player feedback uses distinct audio for:

- shield hit;
- shield depleted;
- health damage;
- critical health;
- medical stabilization;
- status effects where useful.

Persistent heartbeat/tinnitus effects are optional and must be reducible/disableable.

## 17. Environmental Hazards

Hazards can use characteristic audio where physically plausible:

- electrical arcing;
- pressure leak;
- structural stress;
- radiation detector telemetry;
- thermal system warning;
- chemical corrosion/failure;
- storm/weather.

Silent hazards require visual/instrument feedback.

## 18. Pressure Leaks

In atmosphere, pressure leaks can produce directional rushing/hissing sound.

As local atmosphere approaches vacuum, air-carried leak sound reduces while suit/structure alarms remain.

## 19. Fire

Fire audio depends on atmosphere and fuel.

No ordinary crackling fire loop continues unchanged in vacuum.

## 20. Robots

Robot class audio communicates:

- locomotion type/weight;
- tool operation;
- weapon system;
- low energy;
- damage;
- command acknowledgement;
- communication loss/reconnect.

Heavy Mech should sound materially heavier than a Recon Drone.

## 21. Robot Command Acknowledgement

Acknowledgement is concise and nonspammy.

Possible channels:

- synthetic tone;
- short voice-code;
- HUD sound.

Repeated group orders aggregate rather than producing eight simultaneous full voice lines.

## 22. Faction Audio Identity

Factions can differ through:

- UI/terminal sound palette;
- PA style;
- machinery condition;
- weapon/ship technology;
- music/ambient instrumentation;
- radio processing.

Audio identity cannot contradict shared human technology where systems are genuinely identical.

## 23. Continuance Audio

Continuance presentation favors:

- machine-control rhythms;
- sparse synthesized signaling;
- legacy system tones;
- distributed infrastructure activity;
- nonhuman timing/phrasing.

It should not default to horror screeches for every machine interaction.

## 24. Anomaly Audio

Fracture/Anomaly phenomena use a distinct restrained language:

- unstable harmonic relationships;
- phase-like filtering;
- spatially unusual but localizable textures;
- sub/ultrasonic implication through audible design;
- interference with electronics/radio where gameplay state supports it.

Avoid painful high-frequency tones and uncontrolled loudness.

## 25. Music Philosophy

Music supports:

- exploration;
- station calm;
- tension;
- combat;
- raid escalation;
- major discovery;
- narrative/finale.

It should not run at maximum intensity continuously.

## 26. Adaptive Music

Music state can react to:

- exploration vs combat;
- raid escalation;
- critical Horizon defense;
- anomaly proximity;
- story state.

Transitions should avoid revealing hidden enemy presence earlier than gameplay knowledge allows unless the music itself is intentionally a permitted dramatic telegraph.

## 27. Combat Music Boundary

Combat music should not function as an omniscient detector for an enemy the player/AI state has not yet legitimately engaged/recognized.

## 28. Station Music

Horizon uses sparse ambient musical layers rather than constant heroic score.

As the station becomes stable/inhabited, musical warmth/complexity may subtly increase without acting as a literal progression meter.

## 29. Dialogue

Critical dialogue remains intelligible over machinery/combat through:

- priority ducking;
- subtitles;
- repeat/log support where appropriate;
- radio processing kept intelligible.

## 30. Radio Communication

Radio/helmet communication is spatially distinct from world speech.

Signal degradation can be communicated with filtering/dropout only when communication gameplay state supports degradation.

## 31. PA Announcements

Horizon station announcements can communicate:

- critical alarms;
- docking events;
- major system restoration/failure;
- security lockdown;
- evacuation.

Routine low-priority work completion should not generate constant PA spam.

## 32. UI Audio

UI sounds are concise and semantically consistent:

- focus/navigation;
- confirm;
- cancel;
- invalid/block;
- warning;
- transaction complete;
- save complete;
- objective update.

A confirmation tone cannot play on a failed transaction.

## 33. Economy Audio

Credits/item trade uses a restrained transaction confirmation.

No casino-like celebratory sound is baseline for ordinary purchases/rewards.

## 34. Save Audio

Save complete has a subtle confirmation sound.

Queued/waiting save is distinct from successful save.

## 35. Failure Audio

Failure presentation avoids exaggerated punitive stingers for routine setbacks.

Major story/finale outcomes can use authored music/stingers.

## 36. Loudness Hierarchy

Priority during normal play:

1. critical warnings/essential dialogue;
2. direct threat/weapon/impact cues;
3. interaction feedback;
4. local machinery/footsteps;
5. ambience;
6. decorative distant detail.

Music dynamically yields to critical information when needed.

## 37. Dynamic Range

Baseline supports meaningful dynamic range while providing a reduced-dynamic-range option for players/environments where quiet/loud extremes are undesirable.

## 38. Audio Categories

Independent volume categories:

- Master;
- Music;
- Dialogue;
- SFX;
- UI;
- Ambience;
- Voice/Radio where separated;
- Accessibility warning cues where practical.

## 39. Subtitles

All gameplay-relevant spoken dialogue has subtitles.

Subtitles can identify speaker and radio/source context.

## 40. Closed Captions

Optional closed captions can communicate important non-speech sounds such as:

- `[Alarm: Hull Breach]`;
- `[Hostile footsteps nearby]` where legitimately audible;
- `[Pressure leak hissing]`;
- `[Robot: command rejected]`.

Captions cannot reveal sounds outside actual audible/sensor state.

## 41. Spatial Audio Accessibility

Important direction-dependent audio can have optional visual indicators where gameplay permits.

This provides alternate access without changing event existence/range.

## 42. Tinnitus/Low-Health Effects

Any tinnitus, muffling, heartbeat, or auditory distortion used for damage is:

- short;
- bounded;
- separately reducible/disableable.

It cannot prevent understanding mandatory dialogue/warnings.

## 43. Haptics Boundary

Audio can coordinate with haptics but haptics remain optional.

Neither system is the sole carrier of critical information.

## 44. Explicit Non-Goals

No normal atmospheric sound propagation in open vacuum, no casino-style reward audio, no omniscient combat-music enemy detector, no constant station PA spam, no mandatory tinnitus, and no audio-only critical mechanics.

## 45. Tuneable Parameters

Mix levels, propagation/occlusion values, reverb, music transition timing, sample variation, PA frequency, ducking strength, dynamic range, and radio filtering are tuneable.

## 46. Dependencies

References Station, Combat, Spacecraft, Robots, World/Factions, Raids, Interaction Feedback, Alarms, and Accessibility.

## 47. Open Questions

None in the audio-direction baseline.
