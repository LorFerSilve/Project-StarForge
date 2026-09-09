# Spacecraft Construction and Configuration

> **Status:** Draft  
> **Authority:** Hull acquisition/building, module fitting, slot compatibility, mass/power/thermal validation, configuration changes, blueprint/resource requirements, and ship loadout persistence

## 1. Purpose

Ship construction/configuration lets the player specialize spacecraft while keeping every build physically and technologically valid.

## 2. Hull Acquisition

A player can obtain a hull through future systems such as:

- station construction;
- faction purchase;
- mission reward;
- recovered/restored derelict.

The acquisition source does not change hull mechanics.

## 3. Building a Hull

Station-built hulls require:

- Hull Blueprint;
- technology prerequisites;
- compatible shipyard/hangar facility;
- canonical GDS-4 resources/components;
- construction work/time.

The exact material quantities are tuneable content data.

## 4. Hull Recipe Philosophy

Hull recipes primarily consume canonical chains:

- Structural Steel;
- Aluminum;
- Titanium;
- Carbon Composite;
- Structural Frames;
- Pressure Seal Assemblies;
- circuits;
- power regulators;
- sensor/thermal components.

Future hull content must not invent undefined generic "Ship Parts".

## 5. Configuration Slots

A Hull Variant defines authored slots.

A slot has:

- Slot ID;
- category;
- size/class;
- location;
- supported module tags;
- optional exclusivity rules;
- physical exterior/interior representation.

## 6. Core Required Systems

A flight-capable ship normally requires:

- Command/Avionics;
- onboard power source or sufficient stored power;
- propulsion;
- maneuvering/control thrusters;
- thermal control;
- structural hull.

Occupied sealed ships additionally require life support.

## 7. Optional Systems

Optional configurable systems can include:

- shields;
- weapons;
- advanced sensors;
- expanded cargo;
- utility equipment;
- robot bay;
- enhanced life support;
- additional batteries;
- specialized mission modules.

## 8. Module Identity

Every module has a persistent Item/Module ID and definition including:

- slot compatibility;
- mass;
- power behavior;
- heat behavior;
- condition;
- technology/blueprint;
- resource recipe;
- system-specific stats.

## 9. Fitting Validation

A configuration is flight-valid only when:

1. all mandatory systems exist;
2. modules fit compatible slots;
3. actual mass <= Maximum Safe Operating Mass;
4. required control connectivity exists;
5. propulsion/control can operate the hull;
6. occupied spaces have required life support;
7. no mutually exclusive module rules are violated.

## 10. Power Margin

The fitting interface calculates expected power behavior.

A ship can be valid even if peak simultaneous demand exceeds generation when:

- batteries/storage can support bursts; or
- player power management can shed loads.

However, no ship can be considered operational if essential flight/control systems have no viable power source.

## 11. Thermal Margin

The fitting interface estimates:

- steady thermal rejection;
- burst thermal capacity;
- expected high-load limitations.

A ship may intentionally be burst-limited, but it cannot ignore eventual heat accumulation.

## 12. Mass and Performance

Actual mass affects:

- acceleration;
- maneuvering;
- strategic propulsion efficiency/range where propulsion defines it;
- landing capability margin.

Mass does not change merely because cargo is hidden from UI.

## 13. Installed Cargo

Cargo inventory contributes mass.

Cargo module volume defines storage capacity.

## 14. Installed Fuel

Fuel/propellant also contributes mass.

Launching with more range can therefore reduce acceleration/cargo margin.

## 15. Weapon Fitting

Weapons require:

- compatible hardpoint;
- sufficient structure;
- power/thermal support;
- ammunition/feed system where applicable.

Combat owns firing behavior.

## 16. External Geometry

External modules must use authored mount positions.

The player cannot place a turret where it physically clips through the hull or blocks required docking geometry.

## 17. Interior Modules

For walkable interiors, installed modules with interior representation occupy authored spaces/visual sockets.

Configuration cannot create impossible overlapping rooms.

## 18. Configuration Location

Major refitting normally requires a compatible:

- station hangar;
- shipyard;
- maintenance berth.

Minor portable items/ammunition can be changed outside full refit.

## 19. Field Module Swap

Large ship modules are not swapped instantly during missions.

A field swap requires an explicitly designed mobile service capability.

## 20. Module Removal

Removing a module transfers it to valid station/hangar storage if capacity exists.

If storage cannot accept it, removal is blocked.

## 21. Damaged Module Removal

A damaged module can be removed if:

- safe shutdown is possible;
- physical access/service facility exists.

Critical damaged components may require stabilization first.

## 22. Blueprint Requirements

Installing/building a newly manufactured module requires its Blueprint/Research rules.

A salvaged intact module can potentially be installed without owning its manufacturing blueprint if:

- hull compatibility exists;
- technology is sufficiently understood to operate it where module definition requires;
- module is not faction/security locked.

Owning an item and knowing how to manufacture it are distinct.

## 23. Reverse Engineering

Recovered modules can enter GDS-4 reverse-engineering flows if supported.

Fitting them does not automatically unlock their blueprint.

## 24. Configuration Profiles

The player can save named loadout profiles as plans.

A profile stores references/configuration intentions, not duplicate items.

Applying a profile:

- checks available modules;
- transfers/installs actual items;
- reports missing components.

## 25. No Free Profile Swapping

A profile does not teleport installed modules between ships.

Physical inventory and refit location rules remain authoritative.

## 26. Multiple Owned Ships

Each ship keeps its own installed configuration.

The same physical module cannot be installed on two ships.

## 27. Construction Queue

Ship/hull/module production uses station manufacturing/construction capability.

GDS-6 does not create a separate resource-generation system.

## 28. Commissioning

A newly built ship must complete Commissioning before normal use.

Commissioning verifies:

- essential systems;
- power;
- control;
- propulsion;
- thermal;
- seal/life support where required.

Commissioning prevents spawning a logically incomplete "working" ship.

## 29. Decommissioning/Salvage

Owned ships can eventually be decommissioned.

Recovered materials follow salvage rules and never equal a full free refund by default.

## 30. UI

Ship fitting UI shows:

- slot map;
- installed modules;
- actual/max mass;
- cargo/fuel contribution;
- power generation/demand;
- thermal capacity;
- propulsion performance;
- life support;
- warnings/blockers.

## 31. Persistence

Hull configuration, module identity, installed location, module condition, loadout profiles, cargo, fuel, and commissioning state persist.

## 32. Edge Cases

If removing a reactor leaves no viable power source, the configuration can be edited while docked but cannot be commissioned/undocked.

If cargo pushes the ship above Maximum Safe Operating Mass, launch/undock is blocked until mass is reduced.

If the same profile references a module currently installed on another ship, the application reports it unavailable rather than cloning it.

If a module becomes incompatible after a design migration, the save migration places it in safe storage and marks the configuration invalid for review.

## 33. Explicit Non-Goals

The system does not provide:

- freeform voxel hull building;
- duplicate modules from saved profiles;
- instant large-module swapping in combat;
- hidden mass;
- generic "ship parts" resource replacing GDS-4 materials.

## 34. Tuneable Parameters

Tuneable values include construction costs, build duration, slot counts, module mass, refit time, and service capacity.

## 35. Dependencies

This specification depends on Ship Classes, Ship Systems, GDS-4 Resources/Blueprints/Crafting, station Manufacturing/Logistics, and future shipyard content.

## 36. Open Questions

None in the hull/configuration baseline.
