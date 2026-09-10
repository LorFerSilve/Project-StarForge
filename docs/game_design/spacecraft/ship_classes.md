# Spacecraft Ship Classes

> **Status:** Design Complete  
> **Authority:** Canonical player-ship hull classes, intended roles, capacity envelopes, interior expectations, and progression boundaries

## 1. Purpose

Ship classes provide understandable structural categories without locking every craft to one exact build.

A hull class defines the feasible configuration envelope.

## 2. Class Philosophy

Classes are not RPG character classes.

They define:

- hull scale;
- module capacity;
- mass envelope;
- hardpoints;
- cargo;
- occupancy;
- docking size;
- intended maneuverability range.

Different designs within one class can specialize differently.

## 3. Canonical Player Hull Classes

The baseline player-accessible classes are:

1. Shuttle;
2. Scout;
3. Utility Cutter;
4. Corvette;
5. Frigate.

Larger capital ships are not required as directly player-owned baseline craft.

## 4. Shuttle

Role:

- local transport;
- early mission deployment;
- personnel transfer.

Characteristics:

- very small hull;
- high maneuverability;
- low cargo;
- limited range;
- minimal weapons;
- cockpit or tiny cabin;
- low operating cost.

Typical capacity:

- pilot;
- small passenger count;
- limited backpack-scale cargo.

## 5. Scout

Role:

- exploration;
- reconnaissance;
- long-range light missions.

Characteristics:

- small hull;
- strong sensors;
- good strategic range;
- low/medium cargo;
- modest weapons;
- efficient propulsion;
- limited survivability compared with combat ships.

## 6. Utility Cutter

Role:

- mining support;
- salvage;
- cargo extraction;
- repair/service missions.

Characteristics:

- small-to-medium hull;
- larger cargo;
- utility mounts;
- moderate maneuverability;
- limited-to-moderate combat capability;
- deployable equipment support.

This is the primary non-combat industrial expedition hull.

## 7. Corvette

Role:

- combat;
- escort;
- dangerous mission deployment;
- boarding support.

Characteristics:

- medium hull;
- multiple weapon hardpoints;
- stronger shields/armor;
- moderate cargo;
- stronger reactor/thermal capacity;
- lower agility than small craft.

## 8. Frigate

Role:

- late-game long-range multipurpose operations;
- heavy combat;
- major raids;
- large cargo/robot deployment.

Characteristics:

- largest baseline player-owned hull;
- walkable interior expected;
- high power/thermal capacity;
- many system slots;
- substantial cargo;
- significant weapon/defense capacity;
- slower maneuvering;
- high build/service cost.

## 9. Capital Ships

Destroyer/cruiser/carrier-scale player ownership is outside the first authoritative baseline.

Such ships may exist as:

- faction ships;
- raid targets;
- narrative assets;
- future expansion.

This prevents GDS-6 from forcing capital-ship simulation scope.

## 10. Hull Variant

A specific craft design is a Hull Variant within a class.

A variant defines:

- exact geometry;
- slot layout;
- hardpoints;
- mass limit;
- base structure;
- cockpit/interior;
- cargo geometry;
- docking connector.

## 11. Hull Technology

More advanced hull variants can use:

- Titanium;
- Carbon Composite;
- advanced armor;
- better structural efficiency.

Progression can improve capacity without changing class.

## 12. Role Specialization

A class can support multiple builds.

Example Corvette:

- interceptor;
- missile platform;
- boarding corvette;
- shield-heavy escort.

Specialization comes from configuration.

## 13. Mass Envelope

Every hull defines:

- Dry Hull Mass;
- Recommended Operating Mass;
- Maximum Safe Operating Mass.

Installed modules, cargo, fuel, ammunition, and equipment all contribute to actual mass.

## 14. Overweight Configuration

The fitting UI prevents undocking/flight if Maximum Safe Operating Mass is exceeded.

A configuration above Recommended Operating Mass but below Maximum can remain valid with worse performance.

## 15. Structural Capacity

Hull variants expose module capacity through authored slots/bays rather than one abstract unrestricted capacity pool.

## 16. External Hardpoints

Hardpoint categories can include:

- Light;
- Medium;
- Heavy;
- Utility;
- Defensive.

A weapon/module must match hardpoint compatibility.

## 17. Internal Bays

Internal bays can include:

- Reactor;
- Power Storage;
- Propulsion Core;
- Thermal;
- Cargo;
- Utility;
- Sensor/Avionics;
- Crew/Life Support.

Not every hull has every bay type.

## 18. Occupancy

Every hull specifies maximum:

- seated active crew;
- passengers;
- robot transport;
- temporary rescued persons.

Occupancy also depends on life-support capacity.

## 19. Interior Scope by Class

Typical baseline:

- Shuttle: cockpit/tiny cabin;
- Scout: cockpit + compact cabin optional;
- Utility Cutter: cockpit + cargo/utility interior;
- Corvette: cockpit/bridge + selected interior spaces;
- Frigate: substantial walkable interior.

Exact variant can differ.

## 20. Docking Class

Hull variants use a Docking Size Class:

- S;
- M;
- L.

Baseline mapping:

- Shuttle/Scout: S;
- Utility Cutter/Corvette: M;
- Frigate: L.

Specific compact variants may differ only if explicitly authored.

## 21. Landing Capability

Not every spacecraft must land on planetary surfaces.

Hull variants define:

- Planetary Landing Capable;
- Space/Docking Only.

Large/heavy variants can require orbital/space docking and shuttle transfer.

## 22. Atmospheric Flight

Full realistic aerodynamic flight is not a baseline requirement.

Planetary landing-capable craft use a simplified controlled descent/landing model where mission zones require it.

Detailed atmospheric handling can be added only if World/Missions justify it.

## 23. Starting Craft

The starting player craft should be a low-capability Shuttle or Scout-class variant sufficient for:

- station access;
- nearby mission deployment;
- limited cargo;
- early strategic travel.

The exact narrative model belongs to GDS-7.

## 24. Frigate Progression

Frigate access is late-game and should require:

- advanced blueprint;
- large resource investment;
- advanced reactor/propulsion technology;
- station construction/service capability.

## 25. No Class-Level Stat Scaling

A Frigate is not stronger because "Frigate level = 10".

Its capability follows actual hull and installed systems.

## 26. UI

Hull comparison shows:

- class;
- dry mass;
- max mass;
- slot layout;
- hardpoints;
- cargo;
- occupants;
- docking class;
- landing capability;
- baseline structural integrity.

## 27. Explicit Non-Goals

Classes do not provide:

- arbitrary character-like levels;
- unrestricted modules;
- guaranteed role lock;
- required capital-ship ownership.

## 28. Tuneable Parameters

Tuneable values include mass envelopes, slot counts, hardpoint counts, cargo volumes, occupancy, and hull integrity.

## 29. Dependencies

This specification depends on ship configuration, systems, propulsion, cargo, docking, resources, combat, and future hull content.

## 30. Open Questions

None in the canonical class baseline.
