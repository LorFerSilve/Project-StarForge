# Exploration

> **Status:** Draft  
> **Authority:** Local discovery states, Points of Interest, scanner/map revelation, exploration rewards, persistent discovery, route/location intelligence, and exploration anti-omniscience rules

## 1. Purpose

Exploration makes unknown locations mechanically valuable through discovery of:

- resources;
- hazards;
- routes;
- survivors;
- salvage;
- research;
- faction activity;
- optional objectives.

## 2. Discovery States

A discoverable local object/site can be:

- Unknown;
- Detected;
- Identified;
- Surveyed;
- Resolved.

## 3. Unknown

No actionable player knowledge exists.

The map does not display the object.

## 4. Detected

The player knows that something is present.

Information can be incomplete.

Example:

- unknown signal;
- mineral signature;
- heat source.

## 5. Identified

The broad type is known.

Examples:

- derelict ship;
- Titanium deposit;
- survivor signal;
- hostile installation.

## 6. Surveyed

Relevant detailed information has been acquired.

Examples:

- deposit Yield Grade;
- hazard type;
- salvage components;
- entrance points;
- faction ownership;
- estimated reserve.

## 7. Resolved

The site's primary discovery interaction has been completed.

Examples:

- unique archive recovered;
- survivor rescued;
- deposit exhausted;
- route relay mapped.

Resolved does not necessarily mean physically destroyed.

## 8. Point of Interest

A **POI** is a local discoverable site.

POI classes can include:

- resource deposit;
- wreck;
- structure;
- signal;
- survivor location;
- anomaly;
- cache;
- environmental feature;
- route/navigation object.

## 9. POI Identity

Persistent/unique POIs use stable IDs.

Procedural mission POIs derive from Mission/Zone ID and seed.

## 10. Discovery Methods

Discovery can occur through:

- direct line-of-sight/proximity;
- Survey Scanner;
- ship sensors;
- mission intelligence;
- NPC information;
- recovered data;
- environmental clues.

## 11. Scanner Boundary

GDS-5 Survey Scanner reveals only data supported by:

- scanner mode;
- range;
- sensor quality;
- line/occlusion rules;
- target detectability.

It is not universal wallhack vision.

## 12. Ship Sensor Boundary

Ship sensors can detect larger/longer-range objects.

Detailed on-foot information can still require local scan.

## 13. Local Map

The local map records discovered:

- major terrain/rooms where supported;
- POIs;
- hazards;
- extraction points;
- objective areas;
- known paths.

It does not automatically expose all entities.

## 14. Map Revelation

Map revelation can come from:

- traversal;
- downloaded facility map;
- sensor scan;
- faction intelligence.

Downloaded data can be outdated/incomplete if authored.

## 15. No Universal Fog Reset

Leaving/re-entering a persistent location does not erase known discoveries.

## 16. Mission-Instance Discovery

For one-shot procedural missions, discovered state persists for that active Mission Instance.

On resolution, only discovery designated as world-persistent is retained globally.

## 17. World-Persistent Discoveries

Examples:

- new Strategic Location;
- route;
- faction hub;
- unique site;
- major resource field;
- Ancient Structure;
- persistent hazard.

## 18. Resource Exploration

Finding a resource can reveal progressively:

Detected signature  
→ Resource identity  
→ Yield Grade estimate  
→ reserve estimate.

GDS-4 owns actual deposit values.

## 19. Salvage Exploration

Scanning a wreck can reveal:

- salvageable zones;
- intact modules;
- hazards;
- structural access points.

## 20. Survivor Exploration

A distress signal can be Detected before the survivor's exact identity/location is known.

GDS-3 owns survivor state.

## 21. Hazard Discovery

Hazards can be:

- Known;
- Detectable;
- Hidden until triggered.

A hidden hazard must still follow plausible sensor/visual/world logic.

## 22. Exploration Rewards

Exploration can directly provide:

- route/location data;
- Research Evidence;
- objective discovery;
- persistent map knowledge;
- access to resources/salvage.

Physical loot remains governed by ownership/extraction.

## 23. Optional POIs

Mission zones can contain optional POIs unrelated to Primary Objectives.

They create risk/reward choices without being required.

## 24. Exploration Completion

The game does not require "100% map completion" for normal mission success unless a Survey mission explicitly defines it.

## 25. Search Pressure

Exploration can become harder because of:

- hazards;
- enemies;
- limited suit endurance;
- distance from extraction;
- unknown terrain.

The baseline does not use an arbitrary exploration-energy bar.

## 26. Route Discovery

A mission can discover Rumored/Mapped/Verified routes.

GDS-6 Navigation defines which state permits travel.

## 27. Stable Discoveries

Reloading after a committed discovery cannot reroll:

- POI type;
- survivor identity;
- resource identity;
- unique reward.

## 28. Environmental Storytelling

Exploration can expose narrative evidence through GDS-7 environmental storytelling.

Required story evidence uses clear discovery paths.

## 29. Exploration UI

The player can inspect:

- known POIs;
- discovery state;
- known hazards;
- map annotations;
- mission relevance.

The player may add manual map markers where UI later supports it.

## 30. Edge Cases

If a POI becomes inaccessible after a systemic event, its state remains known but accessibility changes.

If a route is Detected but not fully mapped, it appears as Rumored rather than travel-selectable.

If a resource deposit is exhausted, it remains known/resolved rather than disappearing from history.

## 31. Explicit Non-Goals

Exploration does not provide:

- omniscient maps;
- scanner wallhacks by default;
- mandatory 100% completion;
- arbitrary exploration stamina;
- discovery reset on reload.

## 32. Tuneable Parameters

Tuneable values include detection ranges, scan times, POI density, map reveal radius, information precision, and optional-POI counts.

## 33. Dependencies

This specification depends on World, Mission Zones, Generation, Objectives, GDS-5 Scanner, GDS-6 Sensors, Resources, Narrative, and Presentation.

## 34. Open Questions

None in the exploration-framework baseline.
