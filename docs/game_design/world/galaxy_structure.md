# Galaxy Structure

> **Status:** Draft  
> **Authority:** Strategic galaxy topology, sectors, systems, locations, routes, discovery state, territory mapping, and reach-band structure

## 1. Purpose

The galaxy structure turns the setting into a navigable strategic graph compatible with GDS-6 Navigation and bounded mission zones.

## 2. Hierarchy

The canonical hierarchy is:

Galaxy Region  
→ Sector  
→ Star System / Deep-Space Cluster  
→ Strategic Location  
→ Local Space Zone / Mission Zone

## 3. Galaxy Region

The playable game occurs inside one bounded post-Fracture recovery region.

The region contains multiple Sectors connected by strategic routes.

## 4. Sector

A Sector is a strategic subdivision used for:

- navigation;
- faction territory;
- threat;
- progression;
- route readability.

A sector contains a manageable set of important systems/locations rather than every astronomical body.

## 5. Canonical Sector Bands

The baseline progression uses five broad bands:

1. **Recovery Sector** — starting region;
2. **Inner Frontier** — early expansion;
3. **Contested Reach** — mid-game faction conflict;
4. **Outer Expanse** — advanced exploration;
5. **Fracture Verge** — late-game anomalous region.

These are structural progression bands, not universal difficulty scaling.

## 6. Recovery Sector

Characteristics:

- player's home station;
- damaged nearby infrastructure;
- common resources;
- weak/local hostile groups;
- early survivor opportunities;
- Reach I routes.

## 7. Inner Frontier

Characteristics:

- established settlements;
- first major factions;
- industrial routes;
- research sites;
- mixed Reach I/II routes.

## 8. Contested Reach

Characteristics:

- overlapping faction claims;
- fortified stations;
- dangerous raids;
- rare resources;
- Reach II/III routes.

## 9. Outer Expanse

Characteristics:

- sparse settlements;
- pre-Fracture ruins;
- dangerous environments;
- exotic discoveries;
- high logistical risk;
- Reach III routes.

## 10. Fracture Verge

Characteristics:

- anomalous routes;
- ancient/unknown structures;
- severe navigation hazards;
- endgame research;
- Reach IV access.

## 11. Strategic Location

A Strategic Location is a selectable navigation node.

Examples:

- station;
- colony;
- planet orbit;
- moon;
- asteroid field;
- derelict zone;
- shipyard;
- research site;
- pirate base;
- anomaly;
- deep-space relay.

## 12. Local Zone

A Strategic Location can contain one or more bounded gameplay zones.

Examples:

Planet Orbit node:
- local orbital flight;
- landing mission;
- derelict satellite mission.

## 13. Routes

Routes are graph edges owned jointly with GDS-6 Navigation.

World defines:

- origin/destination;
- discovery;
- Reach requirement;
- known hazards;
- territorial context.

Navigation owns travel-state execution and cost calculation.

## 14. Reach Mapping

Baseline:

- Recovery Sector: Reach I;
- Inner Frontier: Reach I–II;
- Contested Reach: Reach II–III;
- Outer Expanse: Reach III;
- Fracture Verge: Reach IV.

Exceptions can exist for special difficult local routes.

## 15. No Universal Linear Corridor

The galaxy is not one straight progression line.

Each sector contains:

- primary progression routes;
- optional branches;
- faction routes;
- resource locations;
- exploration dead ends;
- shortcuts unlocked later.

## 16. Route Discovery

Routes can be:

- Unknown;
- Rumored;
- Mapped;
- Verified;
- Temporarily Blocked.

Navigation can only select Mapped/Verified routes according to its rules.

## 17. Rumored Routes

Rumored routes identify possible connectivity but do not yet provide a valid navigation solution.

They can be converted to Mapped through:

- exploration;
- faction data;
- research;
- recovered navigation records.

## 18. Shortcuts

Some later discoveries can unlock routes connecting previously distant nodes.

This makes exploration change logistics, not only add content.

## 19. Territory

A Strategic Location can have:

- controlling faction;
- claimed faction;
- contested status;
- neutral/independent status;
- unclaimed status.

Territory affects missions, trade, security, and reputation context.

## 20. Dynamic Control Boundary

GDS-7 allows selected locations to change faction control.

GDS-12/GDS-11 later define exact dynamic-control rules.

Core narrative hubs may be protected from random control changes when story consistency requires it.

## 21. Threat

Locations have intended threat bands.

Threat does not automatically scale to player power.

## 22. Resource Distribution

World defines which canonical resources are plausible at locations.

It does not create new Resource IDs.

## 23. Location Persistence

Important locations maintain persistent world-state flags such as:

- discovered;
- faction owner;
- major objective completed;
- destroyed/restored installation;
- unique survivor resolved;
- route unlocked.

Mission instance details are owned by GDS-8.

## 24. Procedural Content

Procedural locations can populate defined sector templates.

Procedural generation must respect:

- sector resource profiles;
- faction presence;
- Reach;
- biome/environment;
- threat range.

## 25. Authored Content

Main-story and major faction locations are authored Strategic Locations.

They can still use procedural sub-layouts only where narrative requirements remain preserved.

## 26. Map Readability

The galaxy map displays:

- sectors;
- known locations;
- routes;
- Reach;
- faction control;
- threat information where known;
- route blockers.

## 27. Discovery Fog

Unknown locations are not globally revealed.

Player knowledge expands through exploration/intelligence.

## 28. Explicit Non-Goals

Galaxy structure does not require:

- seamless astronomical scale;
- millions of procedural stars;
- global level scaling;
- every astronomical object becoming a mission;
- one fixed linear path.

## 29. Tuneable Parameters

Tuneable values include sector node counts, route density, optional-branch density, threat bands, and discovery frequency.

## 30. Dependencies

This specification depends on GDS-6 Navigation/Propulsion, Star Systems, Factions, World locations, Missions, and Progression.

## 31. Open Questions

None in the topology baseline.
