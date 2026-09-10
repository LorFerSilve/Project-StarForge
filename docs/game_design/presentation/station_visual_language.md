# Station Visual Language

> **Status:** Draft  
> **Authority:** Visual grammar for Horizon Station modules, infrastructure, operational states, construction readability, damage, repairs, automation, defenses, crew presence, and progression

## 1. Purpose

Horizon Station must be readable as a functioning machine and as the player's evolving home.

Its appearance should reveal how the station works without requiring every system to be understood through menus.

## 2. Horizon Baseline Identity

Horizon is a recovered pre-Fracture industrial/research station that becomes progressively repaired, expanded, inhabited, and customized.

Its baseline language is:

- practical;
- modular;
- serviceable;
- layered with old infrastructure and newer player-built additions.

## 3. Module Readability

Every major module class must have a distinct visual identity through geometry and functional equipment.

Examples:

- Reactor: shielding, heavy conduits, thermal infrastructure;
- Life Support: air-handling equipment, ducting, scrubber geometry;
- Farming: grow beds, lighting, irrigation, plant mass;
- Manufacturing: machine cells, robotic tooling, input/output handling;
- Research: instrumentation, controlled workstations, sample containment;
- Storage: standardized racks/containers/handling lanes;
- Hangar/Dock: large access geometry, service equipment, traffic markings;
- Security: hardened access, sensors, weapon/control interfaces.

## 4. Module Function Before Label

Room labels support identification, but major module purpose should be inferable from geometry and equipment before reading a wall sign.

## 5. Structural vs Functional Pieces

Structural station pieces use a quieter visual language than functional machinery.

Structural:
- corridor;
- wall;
- bulkhead;
- frame;
- junction;
- shaft;
- airlock shell.

Functional:
- reactor;
- relay;
- battery;
- pump;
- fabricator;
- farm;
- turret;
- sensor;
- terminal.

This prevents infrastructure from becoming unreadable decorative noise.

## 6. Connection Readability

Where a gameplay network is physically represented, the player should be able to identify its connection class.

Examples:

- high-power trunks;
- local electrical distribution;
- atmosphere ducting;
- coolant/thermal loops;
- water/fluid piping;
- cargo/logistics interfaces;
- data/security interfaces where physically exposed.

These networks may use different shape, scale, labeling, connector geometry, and secondary color coding.

## 7. Color Is Secondary

Network identification cannot rely only on pipe color.

Examples:

- power uses connector/insulator geometry;
- atmosphere uses broad duct forms;
- coolant uses paired/insulated piping;
- water uses fluid-service fittings;
- cargo uses rails/conveyance interfaces.

## 8. Construction Mode

Station construction mode may temporarily simplify presentation to expose:

- valid sockets;
- structural grid;
- connection points;
- footprint;
- pressure boundary;
- power/utility interfaces;
- collision/clearance;
- invalid placement reasons.

Construction overlays are planning tools and do not imply the physical station normally glows with placement grids.

## 9. Placement States

Construction preview must distinguish:

- Valid;
- Valid with warning;
- Invalid;
- Requires prerequisite/connection;
- Blocked by collision/clearance;
- Blocked by structural/pressure rule.

Color, icon, shape, and text reason are combined.

## 10. Operational States

Functional station equipment has readable states:

- Operational;
- Standby;
- Offline/Player Disabled;
- Unpowered;
- Degraded;
- Critical;
- Damaged;
- Destroyed where applicable.

Presentation must not collapse these into one generic red light.

## 11. Power State

Powered equipment has subtle operational activity such as:

- indicator light;
- display activity;
- motor/fan motion;
- localized sound.

Unpowered equipment becomes visually/sound-wise quiet while preserving emergency/manual identification.

## 12. Load and Priority

Power-management interfaces can visualize:

- generation;
- load;
- battery state;
- disconnected segments;
- overloaded/degraded paths;
- priority shedding.

Normal station traversal does not require permanent floating wattage numbers.

## 13. Atmosphere State

Pressure/atmosphere is communicated through:

- airlock/door indicators;
- emergency lighting/signaling;
- visible leak effects where physically present;
- local terminal/HUD readout;
- audible atmosphere changes;
- pressure-map overlay when requested.

## 14. Thermal State

Thermal presentation can use:

- coolant flow/readouts;
- fan/pump activity;
- heat shimmer;
- warning indicators;
- thermal diagnostic overlay;
- steam/vapor where physically appropriate.

## 15. Logistics State

Storage/logistics spaces show actual occupancy.

Examples:

- empty rack is visually empty;
- cargo containers occupy real positions;
- logistics robots physically move cargo;
- blocked output accumulates at defined output/storage interfaces.

Aggregated UI may summarize stock but cannot contradict physical ownership.

## 16. Manufacturing State

Manufacturing equipment visually distinguishes:

- Idle;
- Inputs Reserved;
- Working;
- Blocked;
- Output Ready;
- Damaged/Offline.

Work-in-progress should visibly exist where feasible rather than appearing only as a progress bar.

## 17. Farming State

Farms visibly reflect crop lifecycle and failure:

- planted;
- growing;
- mature;
- harvested;
- degraded/failed.

UI confirms exact state and blockers.

## 18. Crew Presence

Crew quarters and workplaces become visibly inhabited through:

- personal-use props;
- occupied stations;
- lockers/bunks;
- work equipment;
- moving crew;
- small authored personalization.

The station should not look empty while the roster says dozens of people live there.

## 19. Robot Presence

Station robots visibly:

- dock/charge;
- patrol;
- carry cargo;
- repair;
- work agriculture;
- move between tasks.

Off-screen abstraction may be used technically, but local visual simulation must match current task/state.

## 20. Repair Language

Repairs are visible as repairs.

Temporary repair:
- patch plate;
- bypass cable;
- temporary seal;
- service brace.

Full repair:
- restored functional surface/component;
- possibly a subtle replacement-panel distinction.

The station's history can remain visible without falsely indicating active damage.

## 21. Damage Language

Station damage uses causal local presentation:

- ballistic impacts;
- scorch;
- exposed wiring;
- structural breach;
- broken display;
- coolant leak;
- decompression;
- disabled light;
- destroyed turret/component.

Damage decals/effects must follow actual persistent state.

## 22. Emergency Lighting

Emergency lighting uses localized state, not one global "everything red" filter.

Examples:

- decompressed compartment;
- fire zone;
- security lockdown;
- critical reactor area;
- evacuation route.

Different emergency categories need distinct icon/audio support.

## 23. Doors

Station doors communicate:

- access status;
- pressure status;
- power status;
- lockdown status;
- damage/jam;
- opening cycle.

A locked door should not be visually indistinguishable from an unpowered door once inspected.

## 24. Security Zones

Restricted/security areas use:

- stronger frame/door construction;
- sensor/reader hardware;
- signage;
- surveillance/defense placement;
- access indicators.

## 25. Defense Visuals

Turrets, missile batteries, point-defense systems, shields, sensors, and related infrastructure expose:

- firing/coverage orientation;
- active/disabled state;
- power/ammo/service state when inspected;
- damage.

Coverage overlays are available in defense planning interfaces rather than permanently rendered.

## 26. Redundancy Readability

The station-network overlays must make redundant routes and single points of failure legible to the player.

The physical art should also provide visible separate trunk/routes where space allows.

## 27. Station Progression

Horizon growth is additive and physical.

The player sees:

- more modules;
- better equipment;
- larger docking/industry;
- increased crew activity;
- more automation;
- stronger defenses;
- repaired legacy sections;
- more organized infrastructure.

There is no global visual "Base Level 3" transformation.

## 28. Old vs New Technology

Recovered pre-Fracture systems and newly manufactured player systems can differ in age/wear while sharing compatible interface standards where gameplay requires them.

Advanced technology should look functionally different, not merely recolored.

## 29. Station Overview Mode

A tactical/engineering station overview may present layered overlays for:

- structure;
- pressure;
- power;
- thermal;
- water;
- logistics;
- security;
- defense coverage;
- crew/robot tasks.

Only one primary diagnostic overlay is emphasized at a time to avoid unreadable stacking.

## 30. Alerts in Space

World-space indicators may mark exact failed modules only when:

- player has valid sensor/diagnostic knowledge;
- relevant alert is active;
- UI setting permits markers.

No omniscient failure markers through unexplored/sealed areas.

## 31. Signage Standard

Horizon uses a coherent signage standard:

- deck/section code;
- module/function name;
- directional arrows;
- pressure/security symbol;
- emergency route marks.

Player-renamed modules may display the custom name in UI and selected signage locations.

## 32. Personalization

Cosmetic personalization may alter nonfunctional surfaces, signage, lighting accents, and selected decoration.

It cannot hide mandatory safety indicators, connection types, collision, door state, or faction/security information.

## 33. Accessibility

Station overlays and states support:

- color-independent icons/patterns;
- scalable labels;
- stronger outlines/highlights;
- reduced flicker;
- adjustable warning intensity;
- optional route guidance.

## 34. Explicit Non-Goals

No global base-level reskin, permanent diagnostic overlay, color-only network identification, magically empty/filled storage art, or emergency-state full-screen tint as the sole signal.

## 35. Tuneable Parameters

Signage density, wear, emergency-light intensity, overlay opacity, network highlight thickness, repair-patch visibility, and ambient activity density are tuneable.

## 36. Dependencies

References GDS-2 Station, GDS-3 Crew, GDS-10 Station Robots, GDS-11 Defense, GDS-12 Station Progression, Visual Direction, UI/HUD, VFX, and Alarms.

## 37. Open Questions

None in the station visual-language baseline.
