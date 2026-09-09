# Asteroids and Space Locations

> **Status:** Draft  
> **Authority:** Non-planetary strategic location classes, asteroid fields, derelict zones, stations, wreck fields, deep-space points, anomalies, and resource/site plausibility

## 1. Purpose

Space locations provide the majority of non-surface exploration, salvage, ship flight, and boarding contexts.

## 2. Canonical Space Location Classes

Baseline classes:

- Asteroid Field;
- Mining Cluster;
- Derelict Ship;
- Derelict Station;
- Wreck Field;
- Active Station/Colony;
- Shipyard;
- Research Installation;
- Military Installation;
- Pirate Base;
- Deep-Space Relay;
- Anomaly Site;
- Ancient/Unknown Structure.

## 3. Asteroid Field

Supports:

- mining;
- navigation hazards;
- hidden bases;
- resource scanning;
- ship combat.

Asteroids use explicit deposits rather than unrestricted voxel destruction.

## 4. Mining Cluster

Contains one or more:

- resource deposits;
- temporary extraction sites;
- industrial machinery;
- faction infrastructure.

## 5. Derelict Ship

Can support:

- EVA;
- boarding;
- salvage;
- survivor rescue;
- data recovery;
- environmental hazards.

## 6. Derelict Station

Larger persistent/mission site with:

- compartments;
- infrastructure;
- salvage;
- survivors;
- hostile systems;
- story records.

## 7. Wreck Field

Represents historical battle/disaster remains.

Contains:

- salvage nodes;
- debris hazards;
- data;
- possible hostile scavengers.

## 8. Active Station

A functioning faction/neutral site.

May provide:

- docking;
- trade;
- missions;
- services;
- faction interaction.

## 9. Shipyard

Specialized active station with stronger:

- ship construction;
- refit;
- repair;
- module access.

## 10. Research Installation

Can provide:

- Research Evidence;
- samples;
- unique blueprints;
- restricted/anomaly content.

## 11. Military Installation

Fortified location with:

- defenses;
- restricted access;
- combat/raid relevance;
- faction intelligence.

## 12. Pirate Base

Hostile/conditionally neutral fortified station.

Used for:

- raids;
- black-market/economy content later;
- captive survivors;
- stolen cargo.

## 13. Deep-Space Relay

Pre-Fracture navigation/communication infrastructure.

Can unlock:

- routes;
- historical records;
- map data;
- strategic shortcuts.

## 14. Anomaly Site

A location where standard environment/navigation assumptions are disrupted.

Requires authored mechanics.

It can produce:

- Anomaly Physics evidence;
- Voltaic Crystal;
- Exotic Matter Sample;
- Reach IV relevance.

## 15. Ancient/Unknown Structure

A rare non-standard location with uncertain origin.

Can provide:

- Ancient Technology Core;
- unique evidence;
- main-story progression.

## 16. Local Flight Zones

Space locations can instantiate bounded local flight volumes.

Boundaries transition through Navigation/Mission rules.

## 17. Boarding Locations

Ships/stations expose explicit docking/breach/entry points.

The player does not enter arbitrary geometry seams.

## 18. Resource Plausibility

Each location class defines canonical resource weights.

No new Resource IDs are introduced here.

## 19. Procedural Generation

Procedural space locations can vary:

- layout;
- deposit positions;
- wreck arrangement;
- hazards;
- enemies;
- salvage.

Persistent unique story sites use fixed identity.

## 20. Persistence

Unique site state persists.

Generic repeatable mission sites follow GDS-8 regeneration rules.

## 21. Explicit Non-Goals

Space locations do not require infinite fields, arbitrary full-geometry salvage, or seamless travel between all nodes.

## 22. Tuneable Parameters

Tuneable values include zone size, asteroid density, salvage density, resource profiles, and encounter density.

## 23. Dependencies

This specification depends on Galaxy, Systems, Resources, Spacecraft, Missions, Combat, Raids, and Narrative.

## 24. Open Questions

None in the space-location baseline.
