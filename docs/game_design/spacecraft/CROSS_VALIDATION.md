# Spacecraft Cross-Validation

> **Status:** Active Audit  
> **Authority:** GDS-6 consistency review only  
> **Purpose:** Validate spacecraft identity, configuration, systems, flight, propulsion, navigation, docking, cargo, damage, and combat integration against established global, station, crew, resource, and player rules.

## 1. Scope

This audit checks GDS-6 against:

- Global Game Rules;
- GDS-4 Resource Model, Catalog, Crafting, Research, and Production Chains;
- GDS-5 Player Character, Inventory, Equipment, Movement, Interaction, and Field Survival;
- GDS-3 Crew professions and assignments;
- Station Power, Thermal, Atmosphere, Logistics, Manufacturing, Damage, and Automation.

## 2. Spacecraft Camera Rule — Result: PASS

GDS-6 preserves the global rule:

- third-person chase camera is the primary flight view;
- cockpit view is optional where supported;
- both control the same ship simulation.

No camera-exclusive mechanical advantage was introduced.

## 3. World Structure / Strategic Travel — Result: PASS

Local flight is real-time and bounded.

Strategic travel connects discrete galaxy locations through routes.

GDS-6 does not require seamless manual galaxy traversal.

This matches Global Game Rule 6.

## 4. Propulsion Progression — Result: PASS

DD-006 requires propulsion to unlock strategic access.

GDS-6 implements this through Strategic Reach Classes:

- Reach I;
- Reach II;
- Reach III;
- Reach IV.

Routes declare minimum Reach Class.

Progression therefore changes where the player can go, not only local top speed.

## 5. Resource Ownership — Result: PASS

Ship cargo, fuel tanks, propellant tanks, coolant, and installed modules remain physical owners/items under GDS-4.

Transfers between:

- station;
- player inventory;
- ship cargo;
- mission world

use one authoritative ownership transfer.

No spacecraft system creates duplicate resources.

## 6. Fuel / Propellant — Result: PASS

GDS-6 explicitly distinguishes:

- Fuel = energy source;
- Propellant = reaction mass.

Canonical operating inputs are:

- Reaction Propellant;
- Fusion Fuel Pellet;
- electrical energy;
- canonical advanced components.

Reach IV propulsion does not introduce an undefined exotic consumable.

## 7. Reach IV Chain — Result: PASS

Reach IV propulsion uses:

- Fusion Fuel Pellets;
- Reaction Propellant;
- very high electrical power;
- Quantum Processor control;
- Quantum Substrate-derived hardware.

These all exist in GDS-4.

No undefined material remains.

## 8. Power Architecture — Result: PASS

Spacecraft power uses the same design principles as station power:

- finite generation;
- finite battery energy;
- priority shedding;
- startup requirements;
- blackout;
- black-start recovery.

GDS-6 uses a more compact integrated bus and does not redefine station power topology.

## 9. Thermal Architecture — Result: PASS

Ship thermal systems preserve:

- finite coolant;
- heat generation;
- radiator/heat rejection;
- thermal throttling;
- emergency shutdown;
- no arbitrary heat deletion.

This is consistent with station thermal philosophy.

## 10. Atmosphere and Life Support — Result: PASS

Ship cabin atmosphere is owned by spacecraft systems.

GDS-5 Field Survival owns player suit response.

The ship can refill player Life-Support Reserve only through finite compatible service.

No infinite oxygen/life-support source is introduced.

## 11. Player Inventory vs Ship Cargo — Result: PASS

GDS-5 Player Inventory and GDS-6 Cargo remain separate physical owners.

Player-to-ship transfer requires an actual cargo interface.

Mass/Volume limitations apply on both sides.

## 12. Mission Extraction Security — Result: PASS

GDS-5 defines:

- Secured Loadout;
- Field-Unsecured;
- Vehicle/Extraction-Secured;
- Station-Secured.

GDS-6 provides the physical ship-cargo commit mechanism for Vehicle/Extraction-Secured state.

GDS-8 Missions still owns the exact mission extraction success/failure transaction.

## 13. EVA — Result: PASS

GDS-5 owns:

- suit seal;
- Life-Support Reserve;
- zero-g player movement capability.

GDS-6 owns:

- ship exterior;
- docking context;
- disabled/drifting ship state;
- repair targets.

EVA ship repair therefore has a clean authority boundary.

## 14. Player Controls / Flight — Result: PASS

GDS-5 uses action-based input contexts.

GDS-6 introduces Spacecraft context actions without hard-coding engine-specific key behavior.

This remains implementable in the custom C++/OpenGL input architecture.

## 15. Flight Model — Result: PASS

Local flight uses:

- 6DoF;
- inertia;
- thrust/mass relationship;
- flight assistance;
- finite maneuver authority.

The model intentionally avoids full orbital mechanics.

This is consistent with selective-simulation and realism philosophy.

## 16. Cargo Mass — Result: PASS

Cargo, modules, fuel, propellant, ammunition, and occupants all contribute to ship mass.

Mass affects flight/strategic cost.

There is no hidden weightless cargo system.

## 17. Ship Construction vs GDS-4 — Result: PASS

Hull/module construction requires:

- Blueprint;
- Research prerequisites;
- canonical materials/components;
- station shipyard/manufacturing capability.

No generic undefined "Ship Parts" resource was introduced.

## 18. Blueprint / Salvaged Module — Result: PASS

A salvaged physical module can be installed if technically compatible even when the player does not own its manufacturing Blueprint, subject to technology/operation restrictions.

Installing the module does not automatically unlock its Blueprint.

This preserves GDS-4's distinction between item ownership and design knowledge.

## 19. Station Logistics / Docking — Result: PASS

Hard Docking enables finite service links.

Docking does not merge inventories.

Cargo/refuel/service flows require:

- valid connector;
- throughput;
- source;
- destination capacity.

This matches station logistics.

## 20. Station Manufacturing / Refit — Result: PASS

Major ship construction/refit uses station facilities.

GDS-6 defines fitting/commissioning rules.

Station Manufacturing remains owner of physical production queues and processing.

No second independent manufacturing economy exists.

## 21. Crew Professions — Result: PASS

Crew roles integrate cleanly:

- Navigator → route/navigation supervision;
- Mechanic → mechanical ship service;
- Engineer → power/thermal/system diagnosis;
- Weapons Specialist → combat coordination;
- Logistics Officer → cargo/service support.

Baseline ship operation does not hard-lock the player behind mandatory multi-crew.

## 22. Player-Owned Ship Persistence — Result: PASS

Routine mission defeat does not permanently delete the player's established Primary Ship.

The ship can become:

- damaged;
- disabled;
- derelict;
- recovered.

This is consistent with global failure philosophy.

## 23. Ship Damage — Result: PASS

GDS-6 owns:

- module condition;
- subsystem consequences;
- leaks;
- breaches;
- disabled/derelict state;
- repair/recovery.

GDS-9 now owns:

- attack resolution;
- combat damage channels;
- penetration;
- shield overflow;
- weapon formulas;
- subsystem targeting semantics;
- missile/point-defense combat behavior.

**Result: PASS**

No combat formula is duplicated.

## 24. Ship Combat Boundary — Result: PASS

GDS-6 defines combat integration:

- hardpoints;
- targeting dependency;
- shields;
- power/thermal tradeoffs;
- disable-vs-destroy;
- boarding preconditions.

GDS-9 remains authoritative for the actual combat model.

## 25. No Infinite Resource Loops — Result: PASS

Reviewed spacecraft loops:

### Refueling

Consumes actual station/field resources.

### Suit Refill

Transfers finite ship/station life-support capacity.

### Cargo

Ownership transfer only.

### Repair

Consumes canonical parts/materials.

### Salvage

Returns less than full recipe by default.

### Batteries

Recharge from actual electrical power.

No free closed material/energy loop was introduced.

## 26. Internal GDS-6 Consistency — Result: PASS

### Class vs Configuration

Hull class defines envelope; variant defines exact slots; modules define capability.

### Configuration vs Flight

Installed mass and propulsion directly affect flight.

### Propulsion vs Navigation

Drive Reach enables routes; Navigation validates and executes route transitions.

### Docking vs Cargo

Docking creates physical service access; cargo transfer remains separate.

### Systems vs Damage

Damaged systems reduce the actual functions they support.

### Cargo vs Mission Security

Cargo can provide secure extraction ownership without automatically completing a mission.

## 27. Dependencies Preventing Design Complete

GDS-6 remains first-pass pending:

### GDS-7 World / Galaxy / Factions — First-Pass Resolved

**Result: PASS**

GDS-7 now defines the canonical campaign route graph, Strategic Locations, sector Reach bands, faction ship identity, major shipyard/service hubs, and planetary/space location contexts.

Exact economy acquisition prices remain GDS-12 authority.

### GDS-8 Missions — First-Pass Resolved

**Result: PASS**

GDS-8 now defines ship mission deployment, landing/docking use within Mission Zones, extraction commit/failure, ship-secured cargo behavior, rescue/tow mission contexts, and stranded-state mission recovery.

GDS-6 remains authoritative for the ship's actual flight, cargo, docking, damage, and recovery systems.

### GDS-9 Combat — First-Pass Resolved

**Result: PASS**

GDS-9 now defines ship weapon families, shield/armor resolution, subsystem targeting, missile/point-defense behavior, collision-damage authority, and generic enemy ship-combat behavior.

GDS-6 remains authoritative for flight, power, thermal state, module condition, Disabled/Derelict state, and recovery.

### GDS-10 Robots — First-Pass Resolved

**Result: PASS**

GDS-10 now defines Light/Medium/Heavy robot footprints, Tactical Squads, reserve/deployment semantics, charging/support needs, and robot extraction/recovery.

GDS-6 Cargo/Utility now explicitly owns Robot Rack Units, Robot Racks, and Heavy Robot Bays as physical spacecraft capabilities.

### GDS-11 Raids

Required for:
- hostile boarding;
- breaching;
- sabotage;
- capture.

### GDS-12 Economy / Progression / Persistence

Required for:
- prices;
- service costs;
- final progression pacing;
- permanent save transaction details.

### GDS-13 Presentation

Required for:
- final cockpit/chase HUD;
- flight feedback;
- docking UI;
- damage alarms.

## 28. First-Pass Conclusion

No blocking contradiction was found.

GDS-6 can be marked:

**First-Pass Complete — Cross-Validation Pending**

The next dependency-driven phase is GDS-7 World, Galaxy, Narrative, and Factions.
