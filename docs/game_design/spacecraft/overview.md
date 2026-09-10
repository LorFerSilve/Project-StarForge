# Spacecraft Overview

> **Status:** Design Complete  
> **Authority:** Persistent player-owned spacecraft identity, role, global ship invariants, operational lifecycle, and boundaries with station/player/combat systems

## 1. Purpose

Spacecraft are the player's mobile operational platforms connecting the home station to the galaxy.

They enable:

- strategic travel;
- mission deployment;
- cargo extraction;
- EVA support;
- boarding;
- ship combat;
- mobile sensing;
- resource hauling;
- specialist mission roles.

## 2. Core Ship Fantasy

A spacecraft is not a disposable menu vehicle.

It is a persistent machine that the player:

- acquires;
- configures;
- equips;
- fuels;
- repairs;
- upgrades;
- flies;
- docks;
- uses to carry resources and equipment.

## 3. Persistent Ship Identity

Each owned spacecraft has persistent identity including:

- Ship ID;
- name;
- hull class;
- installed modules;
- installed weapons/utilities;
- cargo;
- fuel/propellant;
- power-system state;
- thermal state;
- structural/functional damage;
- configuration;
- cosmetic presentation where supported.

## 4. Primary Player Ship

The player can designate one spacecraft as the current Primary Ship.

The Primary Ship is the craft used for normal deployment/travel unless a mission or explicit choice selects another owned craft.

## 5. Fleet Scope

The baseline supports ownership of multiple spacecraft over progression.

However, GDS-6 does not require real-time command of a large fleet in tactical combat.

The player normally directly pilots one ship at a time.

Future automation/crew can support parked, docked, or strategically assigned ships where later systems define it.

## 6. Physical Interior

Not every spacecraft requires a fully traversable multi-room interior.

The baseline distinguishes:

- cockpit-only craft;
- cockpit + compact cabin/cargo interior;
- larger walkable interior craft.

A ship definition declares its interior scope.

This keeps content/technical cost proportional to ship class.

## 7. Camera

Normal piloting supports:

- third-person chase camera as primary;
- optional first-person cockpit view where cockpit representation exists.

Both views control the same flight simulation.

Neither provides exclusive mechanical capability.

## 8. Ship Systems

Spacecraft can contain:

- structure/hull;
- power generation/storage;
- propulsion;
- maneuvering thrusters;
- thermal control;
- shields;
- sensors;
- weapons;
- cargo;
- docking equipment;
- life support where crew/player occupancy requires it;
- utility modules.

## 9. No Single Ship Power Score

Ship capability is not reduced to one universal Power Rating.

Performance emerges from actual installed systems such as:

- thrust;
- mass;
- power;
- thermal capacity;
- shield capacity;
- weapon fit;
- sensors;
- cargo;
- range.

A UI summary can show derived comparisons without replacing system state.

## 10. Configuration Philosophy

Ships are modular within hull-defined constraints.

The player can change loadout and systems, but hull class determines:

- physical size envelope;
- module slots/mounts;
- mass limits;
- reactor/power architecture limits;
- cargo envelope;
- weapon hardpoints;
- crew/passenger capacity;
- docking compatibility.

## 11. No Freeform Voxel Ship Building Baseline

The baseline does not require arbitrary block-by-block spacecraft construction.

Ships use authored hull frames with modular internal/external configuration.

This keeps:

- flight collision;
- interiors;
- animation;
- damage zones;
- docking;
- camera;
- AI

manageable in a custom C++/OpenGL project.

## 12. Strategic Progression

Spacecraft progression should unlock:

- farther reachable regions;
- greater mission endurance;
- safer environmental access;
- heavier cargo;
- more powerful ship systems;
- stronger combat capability;
- specialized mission roles.

It should not be only linear stat inflation.

## 13. Strategic Travel vs Local Flight

Spacecraft operation has two distinct scales:

### Local Flight

Real-time piloting inside bounded space gameplay zones.

### Strategic Travel

Travel between strategic galaxy locations through Navigation and Propulsion rules.

Strategic travel is not continuous real-time traversal of the full galaxy.

## 14. Player Transition

The player can move between:

- on-foot station;
- ship interior/cockpit;
- docked ship;
- local flight;
- EVA;
- mission deployment.

Transitions must preserve player and inventory ownership.

## 15. Cargo Role

Ship cargo is a distinct physical inventory owner under GDS-4.

Mission-acquired resources can become Vehicle/Extraction-Secured when successfully committed to valid ship cargo under mission rules.

Ship cargo is not the same object as player backpack inventory.

## 16. Life Support

Occupied ships require compatible safe environment.

Ships with sealed crew spaces can provide:

- pressure;
- breathable atmosphere;
- thermal control;
- life-support reserve.

Detailed ship-environment behavior is defined in ship systems.

## 17. Power and Thermal

Spacecraft use dedicated onboard power and thermal systems.

They follow the same design philosophy as station infrastructure:

- no free power;
- no arbitrary heat deletion;
- failure propagates through actual dependencies.

Spacecraft systems are smaller and more integrated than station networks.

## 18. Damage Persistence

Ship damage persists after:

- combat;
- mission;
- travel;
- docking;
- save/load.

Docking does not automatically fully repair the ship.

## 19. Routine Ship Loss Philosophy

The baseline does not make ordinary mission defeat permanently delete the player's established primary spacecraft.

Ships can be:

- damaged;
- disabled;
- stranded;
- recovered.

Permanent loss of major owned ships is reserved for explicitly designed high-stakes cases or later accepted design change.

## 20. Derelict State

A ship that cannot operate can become Derelict/Disabled.

Recovery can require:

- towing;
- repair;
- emergency power;
- rescue;
- station service.

## 21. Crew and Passengers

Ship definitions can provide:

- pilot seat;
- crew stations;
- passenger capacity;
- robot capacity.

The player remains the default direct pilot.

Crew participation is not assumed until specific ship/mission roles require it.

## 22. Ship Automation

Onboard control systems can automate routine:

- power balancing;
- thermal protection;
- flight stabilization;
- docking assistance;
- navigation computation;
- damage alerts.

Automation does not choose strategic destination, mission, or combat objective without player command.

## 23. Ship Naming

Owned ships can be renamed by the player.

Renaming changes display identity only, not Ship ID or mechanics.

## 24. Save State

Persistent ship state includes all gameplay-relevant configuration, inventory, damage, fuel, energy, and location.

No ship system refills or repairs merely because the game was closed.

## 25. Explicit Non-Goals

GDS-6 does not require:

- seamless galaxy-scale manual flight;
- massive fleet RTS control;
- freeform voxel ship building;
- disposable primary ships on routine defeat;
- one global ship-power stat;
- every small craft having a fully walkable interior.

## 26. Dependencies

This overview depends on GDS-4 Resources, GDS-5 Player, station docking/manufacturing, and World, Missions, Combat, Robots, Economy, and Persistence.

## 27. Open Questions

None at spacecraft overview level.
