# Environment Art Direction

> **Status:** Design Complete  
> **Authority:** Visual construction of planetary, orbital, derelict, colony, anomaly, and space mission environments; biome readability; environmental storytelling; gameplay-space legibility; and art-state persistence

## 1. Purpose

Environment art must communicate where the player is, what environmental risks apply, which spaces are navigable, what faction or history shaped the location, and which elements are mechanically relevant.

## 2. Environment Categories

Baseline environment families:

- Horizon Station;
- inhabited human stations/colonies;
- abandoned/derelict facilities;
- planetary/moon surfaces;
- asteroid/mining sites;
- local-space orbital zones;
- Ancient/Anomaly structures.

Each family uses the same gameplay rules while retaining distinct visual identity.

## 3. Bounded-Zone Honesty

Mission environments are bounded authored/procedural zones.

Visual composition must make boundaries believable through world logic such as:

- terrain impassability;
- radiation/storm fronts;
- damaged bulkheads;
- locked pressure sections;
- cliffs/chasm;
- mission-approved landing perimeter;
- debris fields;
- navigation hazard.

Invisible walls should be a last-resort enforcement layer, not the primary visual explanation.

## 4. Traversal Readability

Routes that are intentionally traversable should differ visually from decorative dead ends through:

- clearance;
- lighting;
- structural continuity;
- floor treatment;
- signage;
- door/access geometry;
- authored landmarks.

The environment should not require yellow-paint-on-every-ledge as the default traversal language.

## 5. Gameplay Object Separation

Interactive machinery, loot containers, resource deposits, breach targets, consoles, cover, hazards, and extraction interfaces must be visually separable from pure decoration.

This separation can come from:

- silhouette;
- material;
- functional mounting;
- lighting/state indicators;
- contextual UI focus.

## 6. Environmental Storytelling

Locations communicate history through plausible physical evidence:

- repairs;
- emergency barricades;
- abandoned tools;
- rerouted power;
- evacuation markings;
- faction occupation changes;
- salvaged panels;
- damaged habitation;
- logs/terminals where authored.

Storytelling props must not contradict persistent world state.

## 7. Biome Identity

Canonical surface biome families can include:

- arid/desert;
- frozen;
- volcanic/geothermal;
- toxic/chemical;
- overgrown/bio-rich;
- barren moon/regolith;
- ruined colony/urban-industrial;
- crystalline/anomalous;
- irradiated wasteland.

Each biome should differ through terrain form, sky/lighting, material palette, weather, flora/geology, hazard cues, and resource context.

## 8. Hazard Readability

Known environmental hazards must have at least one valid presentation path.

Examples:

- radiation: instrumentation + warning signage/visual contamination cues;
- heat: shimmer, material glow, steam, thermal readout;
- cold: frost, condensation, brittle surfaces, suit readout;
- toxic atmosphere: haze/corrosion/industrial warning + suit readout;
- vacuum: sound/atmosphere change, exposed space, pressure indicators;
- electrical hazard: arcing, damaged insulation, warning audio;
- anomaly: dedicated Fracture visual language + scanner feedback.

Color alone is insufficient.

## 9. Unknown Hazards

An unknown hazard may initially lack explicit explanation, but must still be discoverable through at least one gameplay-valid method such as:

- scanner;
- environmental observation;
- equipment behavior;
- faction intel;
- warning records.

Unknown does not mean visually arbitrary instant punishment.

## 10. Surface Scale

Surface zones should imply a larger planet/moon beyond playable boundaries through:

- horizon composition;
- distant terrain;
- celestial bodies;
- weather;
- distant structures;
- ship approach/departure presentation.

Playable space remains intentionally bounded.

## 11. Landmarking

Each substantial mission zone should contain distinct orientation landmarks.

Examples:

- relay tower;
- crashed ship;
- refinery stack;
- mountain silhouette;
- shield dome;
- large antenna;
- excavation pit;
- Ancient structure.

Repeated modular rooms require local variation/signage to prevent navigation ambiguity.

## 12. Interior Navigation

Human interiors use consistent wayfinding:

- deck numbers;
- room labels;
- directional arrows;
- functional color bands as secondary cues;
- distinctive major machinery;
- emergency route symbols.

Faction styles can reinterpret these while preserving readability.

## 13. Derelicts

Derelicts should show systemic failure:

- partial power;
- pressure loss;
- debris;
- damaged doors;
- emergency lighting;
- exposed hull;
- improvised repair;
- scavenging history.

A pristine abandoned station is not the default visual state.

## 14. Inhabited Locations

Inhabited facilities show current operation:

- moving personnel/robots;
- active cargo;
- maintained lighting;
- occupied workspaces;
- operational signage;
- faction-specific security;
- localized wear rather than universal ruin.

## 15. Resource Deposits

Canonical resource deposits must have a plausible visual source.

Examples:

- exposed ore vein;
- drilled seam;
- ice pocket;
- salvage wreckage;
- industrial tank;
- crystal formation;
- Ancient component.

The resource UI can identify exact material after valid detection, but art should communicate broad category.

## 16. Loot Placement

Physical loot should appear where its presence makes sense:

- lockers;
- cargo racks;
- workshops;
- wrecks;
- bodies only where authored and physically plausible;
- secure storage;
- industrial output.

Random glowing crates scattered without context are not the baseline.

## 17. Cover Language

Combat spaces use physically plausible cover:

- cargo;
- machinery;
- structural columns;
- barriers;
- terrain;
- vehicle wreckage.

Cover does not need a special universal paint color.

## 18. Destructible/Breachable Targets

Breachable targets need authored construction cues such as:

- service seams;
- damaged plating;
- breaching frame;
- cutting markers;
- scanner result;
- exposed structural weakness.

Not every visually thin wall is therefore destructible.

## 19. Doors and Access

Door visual state should distinguish:

- open;
- closed/unlocked;
- locked/security-restricted;
- unpowered;
- jammed/damaged;
- sealed/pressure isolation;
- breachable when identified.

Exact status text appears through interaction UI when focused.

## 20. Spaceflight Zones

Local-space environments use:

- celestial anchor;
- station/ship silhouettes;
- debris lanes;
- beacons;
- navigation markers;
- clear relative-depth cues.

Background stars cannot be so visually dense that targets disappear.

## 21. Debris Fields

Debris must communicate actual collision scale.

Gameplay-relevant debris should differ from background-only particles through size, lighting, parallax, and sensor response.

## 22. Ancient Structures

Ancient/unknown structures avoid direct fantasy ornament.

Visual identity uses:

- unfamiliar but precise geometry;
- low-visible fasteners/joints;
- unusual material continuity;
- non-human scale patterns;
- controlled anomaly interactions;
- sparse readable active elements.

They should feel engineered, not mystical temples with unexplained magic symbols.

## 23. Weather

Weather supports atmosphere and hazard state but cannot make mandatory gameplay unreadable without appropriate instruments/mitigation.

Examples:

- dust storm;
- snow;
- rain/chemical precipitation;
- ash;
- electrical storm.

## 24. Environmental State Persistence

Committed changes such as:

- opened breach;
- destroyed defense;
- depleted storage;
- repaired relay;
- persistent fire damage;
- placed beacon

must have matching environment presentation when revisited.

Art cannot silently revert while gameplay state remains changed.

## 25. Procedural Composition

Procedural side locations may recombine authored modules/terrain pieces but must preserve:

- navigable connectivity;
- faction visual rules;
- hazard logic;
- objective readability;
- extraction readability;
- stable seed/persistence.

## 26. Repetition Control

Repeated modules should vary through legitimate contextual layers:

- damage;
- faction occupation;
- cargo/use state;
- lighting;
- signage;
- environmental contamination;
- prop population.

Variation must not change door size or collision meaning unpredictably.

## 27. Mission Objective Presentation Boundary

Environment art can make likely objective structures recognizable, but GDS-8/GDS-13 HUD owns explicit objective state.

A visually important object is not automatically an objective.

## 28. Accessibility

Environment readability must remain viable with:

- reduced motion;
- color-vision filters;
- stronger interaction highlight;
- adjustable brightness/contrast;
- simplified effects.

## 29. Explicit Non-Goals

No requirement for seamless planets, dense decorative clutter everywhere, universal glowing loot, universal painted climbing markers, invisible boundary-first design, or photoreal environmental simulation.

## 30. Tuneable Parameters

Prop density, fog, weather intensity, landmark density, decal wear, vegetation density, distant-detail amount, and highlight strength are tuneable.

## 31. Dependencies

References World geography, Missions/Zones/Hazards, Resources, Raids, Combat, and Visual Direction.

## 32. Open Questions

None in the environment-art baseline.
