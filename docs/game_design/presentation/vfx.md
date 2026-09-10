# VFX

> **Status:** Draft  
> **Authority:** Visual-effects language for weapons, impacts, shields, environmental hazards, station systems, spacecraft, anomalies, damage, and effect readability/accessibility

## 1. Purpose

VFX must communicate physical/systemic events clearly while reinforcing the stylized-grounded science-fiction identity.

Effects are presentation of authoritative state, not an independent gameplay system.

## 2. VFX Principles

Effects should be:

- readable;
- causal;
- spatially anchored;
- restrained enough not to hide gameplay;
- distinguishable by event type;
- reducible for accessibility.

## 3. Effect Truthfulness

A VFX event must not imply a gameplay consequence that did not occur.

Examples:

- shield-break effect only when shield reaches relevant break/depletion state;
- breach venting only when pressure differential/path exists;
- persistent fire only while Burning/fire state exists;
- destroyed-system sparks cannot continue forever after a fully repaired state.

## 4. Weapon Muzzle Effects

Weapon families use distinct but restrained signatures:

- ballistic: muzzle flash, gas/smoke where appropriate;
- rail: brief electromagnetic discharge/tracer-like energy signature;
- laser: coherent beam/pulse presentation appropriate to firing mode;
- plasma: luminous projectile/thermal trail;
- explosive launcher: propellant/backblast/projectile cue;
- melee: impact/contact effects only where physically relevant.

## 5. Projectile Readability

Physical projectiles that the player can react to must remain visually trackable at intended combat distance/speed.

Projectile trails do not need to reveal every high-speed hitscan shot as a slow visible projectile.

## 6. Impact Language

Impacts distinguish broad surface/result classes:

- shield;
- metal/armor;
- rock/terrain;
- glass/transparent surface;
- biological target;
- electrical/electronic system;
- water/fluid/ice where relevant.

Impact effects must not exceed the physical energy scale so dramatically that small-arms fire looks like artillery.

## 7. Shield Effects

Shield feedback uses:

- localized impact ripple;
- brief surface reveal;
- channel-dependent variation where readable;
- depletion/break transition;
- recharge/return cue.

Personal shields should not permanently surround actors with an opaque bubble that obscures aim.

## 8. Armor Feedback

Armor impact emphasizes:

- spark/debris/material response;
- ricochet/deflection where appropriate;
- penetration effect when relevant.

The effect itself must not expose exact armor values unless sensors/UI legitimately provide them.

## 9. Weak-Point Effects

Weak-point hits can receive a distinct concise effect, especially on machines/ships where a component reacts.

No oversized fantasy critical explosion is required.

## 10. Explosion Scale

Explosions are scaled by actual payload and environment.

They communicate:

- blast origin;
- pressure/thermal effect;
- fragmentation/debris where applicable;
- environmental interaction.

One visual explosion template cannot represent every grenade, reactor failure, missile, and capital-scale detonation.

## 11. Vacuum Explosions

Vacuum effects avoid persistent atmospheric fireballs/smoke behavior unless the event supplies its own material/oxidizer.

Use expanding debris, brief luminous gas, venting material, and local energy signatures as appropriate.

## 12. Fire

Fire presentation depends on:

- atmosphere;
- fuel source;
- local airflow/pressure;
- suppression state.

Station fire is a localized hazard, not a decorative effect pasted onto arbitrary surfaces.

## 13. Smoke and Visibility

Smoke can obstruct vision when physically justified.

It must be adjustable/simplifiable for accessibility while preserving hazard state through alternate cues.

## 14. Electrical Effects

Electrical/EMP effects use:

- arcing;
- short equipment flicker;
- sensor/display disruption;
- localized discharge;
- brief interference overlays where appropriate.

Full-screen strobing is not baseline.

## 15. Corrosive Effects

Corrosive damage can use:

- material pitting;
- vapor/fume;
- surface discoloration;
- dripping/chemical reaction;
- damaged-seal cues.

It must remain visually distinct from Burning.

## 16. Player Damage Overlays

Player-screen effects are minimal and short-lived.

Possible:

- shield edge distortion;
- directional impact flash;
- brief low-health vignette;
- status-specific subtle overlay.

Long opaque blood overlays are avoided.

## 17. Healing/Recovery Effects

Medical feedback should remain grounded:

- tool animation;
- injector/patch feedback;
- suit medical indicator;
- restrained local visual cue.

No magical healing aura is baseline.

## 18. Station Power

Power state can create VFX through:

- equipment emissive activity;
- relay arcing under fault;
- overload sparks;
- emergency lighting transition;
- disabled equipment darkness.

Diagnostic network overlays belong to UI rather than permanent VFX.

## 19. Atmosphere and Pressure

Pressure events use:

- vapor condensation where temperature/humidity permits;
- dust/debris movement;
- escaping gas;
- loose-object motion;
- suit/airlock visual indicators.

Vacuum itself is not represented by generic blue fog.

## 20. Thermal Systems

High heat can use:

- glow on genuinely hot materials;
- heat shimmer in atmosphere;
- steam/coolant vapor;
- thermal scanner overlay.

Cooling failure does not automatically produce visible flames.

## 21. Fluid/Leak Effects

Water/coolant/fuel leaks reflect actual source and gravity state.

Zero-G fluids may form droplets/streams rather than falling downward.

## 22. Manufacturing Effects

Fabrication/processing effects show real machine work:

- cutting;
- welding;
- deposition;
- machining;
- heating;
- material movement.

A generic glowing crafting beam is not the baseline presentation for all manufacturing.

## 23. Robot Effects

Robot classes communicate capability through actual tool/weapon effects.

Examples:

- Repair Drone: welding/diagnostic light;
- Hacker: interface/data-link visualization localized to valid connection;
- Shield Unit: shield projector geometry;
- Breacher: cutter/charge;
- Recon: sensor pulse/scan visualization when scan is actually active.

## 24. Scanner Effects

Scanning uses a contextual visualization that distinguishes:

- scanning in progress;
- detected;
- identified;
- unresolved;
- inaccessible/occluded where relevant.

It cannot visually reveal content the scanner did not detect.

## 25. Spacecraft Thrusters

Thruster VFX must match actual active thrusters and thrust direction.

If a thruster is disabled, it cannot visually fire.

This is important for 6DoF flight readability.

## 26. Spacecraft Drives

Drive families receive distinct visual identity consistent with propulsion technology:

- chemical/electrothermal;
- ion/plasma;
- fusion torch;
- exotic/quantum Reach technology.

Higher Reach does not simply mean a larger recolored flame.

## 27. Ship Damage

Ship effects map to actual subsystem condition:

- engine failure;
- coolant vent;
- shield collapse;
- weapon damage;
- hull breach;
- reactor instability;
- electrical fault.

## 28. Docking Effects

Docking uses physical cues:

- port lighting;
- alignment markers via HUD;
- thruster response;
- capture clamp animation;
- seal/service-link indicators.

No teleport/fade effect substitutes for Hard Dock capture.

## 29. Raid Effects

Raid VFX distinguish:

- shield suppression;
- turret damage;
- breach creation;
- atmosphere venting;
- sabotage;
- lockdown;
- reinforcement arrival;
- extraction departure.

Escalation is reinforced by environment/defense activity, not only a UI meter.

## 30. Anomaly Effects

Fracture/Anomaly VFX use:

- localized geometric distortion;
- spectral violet/magenta accent;
- coherent wave/interference structures;
- spatial refraction;
- electronics/material reaction.

Effects must avoid seizure-risk strobing and uncontrolled full-screen chromatic flicker.

## 31. Teleportation Boundary

The baseline game has no universal player/robot teleportation.

Anomaly visual distortion must not imply objects teleport unless gameplay explicitly performs such a transition.

## 32. Loot Effects

Normal loot does not require a vertical rarity beam.

Mission-critical/unique physical objects may receive subtle contextual highlight/marker when legitimately identified.

## 33. Objective Effects

Objective VFX should be world-plausible when used:

- beacon;
- signal light;
- scanner response;
- active console.

The HUD remains responsible for explicit objective semantics.

## 34. Effect Priority

When effect density is high, priority order is:

1. lethal hazard telegraph;
2. incoming/active combat threat;
3. interaction/objective feedback;
4. system state;
5. decorative ambience.

Lower-priority effects may be reduced first.

## 35. Effect Culling

Presentation may reduce distant/noncritical particles/effects for performance without changing gameplay visibility of required hazards or events.

Technical culling implementation is not specified here.

## 36. Accessibility — Reduced Effects

Reduced Effects mode can lower:

- particles;
- smoke density;
- bloom;
- distortion;
- screen overlays;
- sparks;
- debris quantity.

It must retain alternate hazard/impact communication.

## 37. Accessibility — Photosensitivity

A Photosensitivity Safe option reduces/removes:

- rapid flashes;
- strobing lights;
- high-contrast repeated pulses;
- lightning frequency/intensity;
- full-screen flicker.

Critical state remains communicated through steady icons/text/audio/haptics.

## 38. Effect Persistence

Persistent world damage and effects use actual state.

Transient particles may end while persistent scorch/breach/damage remains.

## 39. Explicit Non-Goals

No universal loot beams, permanent shield bubbles, excessive bloom, full-screen strobe alarms, magic repair/healing auras, generic blue vacuum fog, or effects that misreport system state.

## 40. Tuneable Parameters

Particle count, lifetime, opacity, light intensity, shake coupling, distortion, smoke density, effect distance, debris amount, and decal lifetime are tuneable.

## 41. Dependencies

References Combat, Station, Spacecraft, Robots, Missions, Raids, Environment, Interaction Feedback, Audio, and Accessibility.

## 42. Open Questions

None in the VFX baseline.
