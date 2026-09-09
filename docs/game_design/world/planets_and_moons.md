# Planets and Moons

> **Status:** Draft  
> **Authority:** Planetary/lunar strategic representation, biome classes, landing/mission-zone relationship, environmental metadata, and resource plausibility

## 1. Purpose

Planets and moons provide bounded surface mission contexts without requiring seamless full-world simulation.

## 2. Strategic Body

A Planet/Moon definition contains:

- Body ID;
- name;
- system;
- body class;
- biome/environment tags;
- atmosphere;
- gravity band;
- temperature band;
- radiation/hazard tags;
- canonical resource profile;
- known settlements/sites;
- landing restrictions.

## 3. Mission-Zone Rule

Surface gameplay always occurs in bounded Mission Zones.

The strategic body represents the larger world.

The player does not circumnavigate an entire planet in real scale.

## 4. Canonical Biome Classes

Baseline biome classes:

- Barren/Rocky;
- Frozen;
- Desert;
- Volcanic;
- Toxic;
- Temperate/Overgrown;
- Ruined Urban/Colony;
- Crystal/Anomalous;
- Irradiated.

A body can combine compatible tags.

## 5. Barren/Rocky

Typical:

- low/no atmosphere;
- mineral extraction;
- regolith;
- exposed installations;
- EVA hazards.

## 6. Frozen

Typical:

- Water Ice;
- low temperature;
- buried facilities;
- cryogenic hazards.

## 7. Desert

Typical:

- heat;
- dust;
- sparse settlements;
- exposed mineral operations.

## 8. Volcanic

Typical:

- extreme heat;
- unstable terrain;
- high-value minerals;
- thermal hazards.

## 9. Toxic

Typical:

- unsafe atmosphere;
- contamination;
- specialized suit requirement;
- biological/chemical research.

## 10. Temperate/Overgrown

Typical:

- breathable or partially breathable areas;
- native biology;
- abandoned colonies;
- agriculture/biological research.

## 11. Ruined Urban/Colony

Represents dense remains of human settlement.

Typical:

- survivors;
- salvage;
- faction conflict;
- infrastructure restoration.

## 12. Crystal/Anomalous

Rare.

Typical:

- Voltaic Crystal;
- anomaly phenomena;
- high research value;
- unusual route/story relevance.

## 13. Irradiated

Typical:

- radiation;
- damaged reactors/facilities;
- Uraninite;
- salvage/research risk.

## 14. Gravity Bands

World exposes gravity to GDS-5 as:

- Low;
- Reduced;
- Standard;
- High.

Exact acceleration is body/location data.

## 15. Atmosphere

Atmosphere metadata reports to player survival:

- pressure class;
- breathable status;
- contamination;
- temperature.

Detailed local atmosphere can vary by mission zone.

## 16. Resources

Resource profiles use only GDS-4 resources.

Examples:

- Water Ice → Frozen bodies;
- Uraninite → Irradiated/mineral bodies;
- Titanium/Tungsten → high-value rocky bodies;
- Alien Biomaterial → biological sites;
- Voltaic Crystal → anomalous worlds.

## 17. Settlements

A body can contain multiple distinct settlements/locations with different faction control.

## 18. Landing

Only landing-capable ships can perform direct surface deployment where the mission supports it.

Other ships require:

- shuttle;
- orbital transfer;
- mission transport.

## 19. Landing Site

Mission zones define authored/procedural landing areas.

The player does not freely land at every arbitrary surface coordinate.

## 20. Weather

Weather can exist when it creates gameplay value.

Examples:

- dust storm;
- electrical storm;
- blizzard;
- toxic rain.

GDS-8 owns mission-level hazard behavior.

## 21. Native Life

Biological life can exist where compatible with body design.

It does not imply intelligent civilization.

## 22. Discovery

Scanning can reveal partial body data before landing.

Detailed resources/sites require exploration/intelligence.

## 23. Persistence

Major discovered sites and story/faction outcomes persist at body level.

Procedural mission regeneration is owned by GDS-8.

## 24. Explicit Non-Goals

Planet design does not require seamless spheres, full terrain deformation, free landing anywhere, or physically simulated global weather.

## 25. Tuneable Parameters

Tuneable values include gravity, temperature, resource occurrence, mission-zone count, and hazard intensity.

## 26. Dependencies

This specification depends on Galaxy/Systems, GDS-4 Resources, GDS-5 Survival, GDS-6 Landing/Navigation, Missions, and Presentation.

## 27. Open Questions

None in the planetary/lunar baseline.
