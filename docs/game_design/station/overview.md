# Home Station Overview

> **Status:** Design Complete  
> **Authority:** Persistent home-station identity, lifecycle, topology, and project-wide station invariants

This document defines what the player's home station fundamentally is.

Detailed construction, infrastructure, defense, automation, and damage behavior is owned by the corresponding station subsystem specifications.

## 1. Purpose

The home station is the central persistent location and primary long-term expression of player progression.

It serves simultaneously as:

- the player's home;
- operational headquarters;
- resource storage;
- industrial center;
- research center;
- crew habitat;
- automation hub;
- robot production and maintenance center;
- ship docking and maintenance facility;
- defensive fortress;
- strategic command center.

The station must remain relevant throughout the complete game.

## 2. Core Player Experience

The player should experience the station as a place they personally built and understand spatially.

The player physically traverses station interiors in first person.

Station growth must be visible and spatial rather than represented only through abstract upgrade menus.

Returning from an expedition should create a clear transition from field operation to long-term development.

## 3. One Primary Home Station

The authoritative baseline contains exactly **one primary player-owned home station**.

The player may later own or interact with:

- spacecraft;
- temporary mission structures;
- remote industrial equipment;
- allied installations;
- captured or discovered locations.

These do not replace the persistent home station unless a future accepted design decision explicitly changes this rule.

## 4. Station Origin

The game begins with a small damaged but recoverable station core.

The starting station contains the minimum infrastructure required to establish the main progression loop:

- a Command Core;
- emergency electrical capability;
- basic life support;
- minimal storage;
- a primitive fabrication capability;
- one basic docking interface.

The initial station is intentionally inadequate for long-term operation.

Its limitations create early progression goals.

## 5. Command Core

Every valid home station contains exactly one **Command Core**.

The Command Core is the station's root structural and logical identity.

It contains or provides:

- station identity;
- central command interfaces;
- emergency control;
- core navigation reference for construction;
- emergency communication capability;
- protected recovery functionality.

The Command Core is not intended to produce enough power, oxygen, storage, or industry for a developed station.

## 6. Anti-Hard-Lock Recovery Rule

Normal gameplay must not allow the player to permanently destroy the save by losing every means of restoring essential station operation.

The Command Core therefore retains a minimal protected emergency capability sufficient to support recovery from total infrastructure failure.

Emergency capability:

- does not substitute for normal reactors;
- does not run normal industry;
- does not sustain a large crew indefinitely;
- does not make defensive failure irrelevant;
- exists only to preserve recoverability.

Exact emergency power, atmosphere, and repair rules are defined by their owning subsystem specifications.

## 7. Physical Station Structure

The station consists of physically connected construction elements.

The high-level hierarchy is:

```text
Home Station
├── Command Core
├── Structural Modules
│   ├── Corridors
│   ├── Junctions
│   ├── Shafts
│   ├── Airlocks
│   └── Structural Frames
├── Functional Modules
│   ├── Engineering
│   ├── Industry
│   ├── Research
│   ├── Habitation
│   ├── Logistics
│   ├── Robotics
│   ├── Hangars
│   └── Defense
└── Infrastructure Networks
    ├── Power
    ├── Atmosphere
    ├── Thermal
    ├── Water
    ├── Logistics
    └── Control / Data
```

A module's existence does not guarantee its operation.

Functional modules must satisfy their subsystem requirements.

## 8. Interior and Exterior Space

The station has meaningful interior and exterior geometry.

### Interior

Interior space contains:

- player traversal;
- crew activity;
- equipment;
- machinery;
- service routes;
- doors;
- compartments;
- operational consoles.

### Exterior

Exterior space can contain:

- hull geometry;
- docking structures;
- radiators;
- antennas;
- sensors;
- shield hardware;
- defensive weapons;
- exposed infrastructure where explicitly allowed.

External placement has tactical and operational consequences.

## 9. Compartments

Station interiors are divided into compartments.

A compartment is a physically bounded interior volume that can participate in:

- atmosphere simulation;
- pressure isolation;
- fire containment;
- access control;
- damage containment.

Compartments are connected through controlled openings such as doors, hatches, airlocks, or breaches.

Exact compartment-generation and structural rules belong to `structural_rules.md`.

## 10. Connectivity Principle

Station expansion must remain structurally connected to the home station.

A normal permanent station module cannot exist as an arbitrary floating disconnected component.

Construction rules determine legal structural attachment.

Remote external devices are permitted only when their owning specification explicitly defines them as independent deployables.

## 11. Infrastructure Principle

Important station functions operate through explicit infrastructure.

A module that requires electricity must be connected to a valid electrical network.

A pressurized occupied space must satisfy atmosphere requirements.

A heat-producing machine must interact with thermal capacity.

A production machine must receive required inputs and have valid output handling.

The station is therefore a functioning system rather than a collection of independent bonuses.

## 12. Station Zones

The player may conceptually organize the station into functional zones.

Typical examples include:

- command;
- engineering;
- industrial;
- habitation;
- research;
- logistics;
- military;
- docking.

Zones are organizational groupings, not magical buffs.

Any zone-specific automation, permissions, or UI behavior must be explicitly defined in its owning specification.

## 13. Vertical Construction

The station supports three-dimensional expansion.

Construction is not limited to one horizontal plane.

The station can expand:

- forward;
- backward;
- left;
- right;
- upward;
- downward;

subject to construction and structural rules.

This allows stacked decks, shafts, vertical industrial areas, defensive layers, and large hangar structures.

## 14. Player Traversal

The player can physically travel between accessible station areas.

Normal traversal uses the same first-person movement framework as other on-foot gameplay.

Station traversal may include:

- corridors;
- stairs;
- ladders where defined;
- lifts;
- pressurized doors;
- airlocks;
- maintenance access.

Fast internal traversal systems may be introduced for sufficiently large stations, but they must not replace the physical station layout.

## 15. Crew Presence

Recruited crew physically inhabit the station.

Crew are assigned to workplaces and use station spaces according to the crew specifications.

The station must provide enough operational capacity to support its crew population.

Detailed crew requirements and scheduling are owned by `../crew/`.

## 16. Population Capacity

Station crew capacity is not an arbitrary account-level number.

Maximum sustainable population is constrained by station infrastructure.

Relevant systems include:

- habitation;
- oxygen;
- water;
- food;
- medical support;
- life-support capacity.

Exact calculations are defined by the respective station and crew specifications.

## 17. Station Construction Cost

Permanent station expansion consumes resources.

A module may require:

- structural materials;
- manufactured components;
- specialist components;
- construction capacity.

Exact recipes belong to the resource and construction specifications.

Construction is never free simply because sufficient geometric space exists.

## 18. Station Operation

The station continues operating during active gameplay while the player is away.

This includes, subject to subsystem rules:

- power generation;
- resource consumption;
- farming;
- manufacturing;
- research;
- repairs;
- automation;
- crew work;
- defensive readiness.

This follows the Simulation-Time simulation rules in `../02_global_game_rules.md`.

## 19. Station State Persistence

The station is persistent across:

- missions;
- travel;
- game sessions;
- save/load.

Persistent state includes all long-term player-relevant station state.

Temporary purely visual effects do not need to persist unless they communicate a persistent gameplay state.

The exact serialization contract belongs to `../systems/save_and_persistence.md`.

## 20. Station Damage

Station components can be damaged where their subsystem defines damage behavior.

Damage can:

- reduce performance;
- disable functionality;
- sever connectivity;
- create hazards;
- expose compartments;
- require repairs.

Damage does not automatically disappear when the player leaves and returns.

## 21. No Routine Permanent Station Deletion

Ordinary hostile attacks do not permanently erase the entire home station or delete dozens of hours of construction.

Attacks can cause meaningful persistent consequences such as:

- damaged modules;
- disabled systems;
- lost unsecured resources;
- repair costs;
- temporary operational disruption.

The Command Core and recovery rules prevent routine attacks from converting into irreversible total-save destruction.

## 22. Station Defense

The station can defend itself through physically placed systems.

Defense may include:

- shields;
- point defense;
- laser weapons;
- kinetic weapons;
- missile systems;
- defensive drones;
- security systems;
- compartment control;
- internal robots.

Defense effectiveness depends on infrastructure and layout.

A turret that lacks required power, targeting data, ammunition, cooling, or structural connection cannot behave as fully operational unless its own specification explicitly provides internal reserves.

## 23. Redundancy

The station design supports redundant infrastructure.

The player can intentionally create alternate:

- power paths;
- power sources;
- cooling routes;
- atmosphere support;
- access routes;
- storage distribution;
- defensive coverage.

Redundancy consumes additional space and resources but reduces single points of failure.

## 24. Strategic Layout

Layout decisions have real consequences.

Examples:

- an exposed reactor is easier to attack;
- centralized infrastructure is efficient but vulnerable;
- distributed infrastructure costs more but improves resilience;
- placing storage near docks improves logistics but increases exposure;
- compartmentalization can contain hazards but increases structural complexity.

The game should not reduce these choices to cosmetic placement.

## 25. Industry

The station is the primary location for scalable long-term processing and manufacturing.

Industry can transform expedition resources into:

- construction materials;
- player equipment;
- ship components;
- robot components;
- ammunition;
- infrastructure parts.

Industrial capacity becomes increasingly automated as the station develops.

## 26. Research

Research is conducted through station infrastructure and qualified crew or automated technology as defined by the research specification.

Research can unlock:

- blueprints;
- new infrastructure;
- automation;
- ship technology;
- robot technology;
- new production methods;
- new strategic access.

## 27. Hangars and Docking

The station supports spacecraft through dedicated docking infrastructure.

A ship cannot use station services that require a physical dock if no compatible docking connection is available.

Large ships may require correspondingly larger docking or hangar infrastructure.

Exact compatibility rules belong to the spacecraft and station construction specifications.

## 28. Visual Growth

Station progression must be visible from both inside and outside.

A late-game station should not look like the starting station with larger numeric values.

Growth should manifest through:

- increased physical size;
- new architectural sectors;
- larger industrial machinery;
- additional docking structures;
- visible defensive systems;
- expanded crew activity;
- more sophisticated lighting and control infrastructure.

## 29. Station Identity

The player's construction choices should produce recognizably different stations.

The game does not require one fixed optimal layout.

Different valid designs may emphasize:

- industry;
- defense;
- research;
- logistics;
- fleet support;
- automation;
- balanced operation.

## 30. Station Progression Stages

The station progresses conceptually through four operational stages.

### Stage I — Survival Outpost

Characteristics:

- minimal space;
- manual operation;
- fragile infrastructure;
- small crew;
- limited production.

### Stage II — Functional Base

Characteristics:

- specialized rooms;
- initial automation;
- reliable life support;
- basic defenses;
- growing industry;
- multiple crew roles.

### Stage III — Orbital Stronghold

Characteristics:

- redundant infrastructure;
- advanced production;
- research specialization;
- significant defenses;
- multiple ships;
- mature automation.

### Stage IV — Interstellar Command Station

Characteristics:

- large multi-sector structure;
- advanced energy;
- deep-space support;
- high automation;
- powerful defenses;
- extensive industrial and fleet capability.

These stages describe progression identity rather than fixed numeric levels.

## 31. Player Control Philosophy

The station should become easier to operate as it becomes larger.

Increased scale must not require proportional growth in repetitive player maintenance.

The progression pattern is:

```text
Manual operation
    ↓
Assisted operation
    ↓
Crew-managed operation
    ↓
Automated routine operation
    ↓
Player strategic supervision
```

The player remains responsible for strategic decisions and exceptional situations.

## 32. UI Principle

The player must be able to understand station health at multiple scales.

The station interface eventually requires:

- local machine feedback;
- compartment-level status;
- network/system overview;
- station-wide alerts;
- construction visualization;
- automation configuration.

Detailed UI behavior belongs to presentation and subsystem documents.

## 33. Visual Feedback Principle

Operational state must be observable where practical.

Examples:

- powered equipment visibly active;
- unpowered sections use emergency lighting;
- damaged systems show physical or VFX indicators;
- pressure emergencies use warning lighting;
- high reactor load changes reactor presentation;
- active defenses visibly track or fire.

Visual feedback supplements UI; it does not need to expose hidden information the player has not legitimately acquired.

## 34. Audio Principle

The station uses audio to communicate operational state.

Examples include:

- reactor hum;
- ventilation;
- machinery;
- alarms;
- pressure warnings;
- impact sounds;
- announcements;
- defense fire;
- docking sounds.

Important critical alerts require more than one communication channel where accessibility permits.

## 35. Explicit Non-Goals

The home-station baseline does not require:

- unrestricted free-form mesh modeling by the player;
- arbitrary voxel construction;
- physically simulating every pipe molecule or electrical electron;
- permanently losing the complete station from a routine attack;
- multiple independent player home stations;
- full crew social-life simulation;
- decorative placement having hidden gameplay effects unless explicitly defined.

## 36. Dependencies

This overview inherits project-wide rules from:

- `../00_design_authority.md`;
- `../02_global_game_rules.md`.

Detailed station behavior will reference:

- `../crew/`;
- `../resources/`;
- `../spacecraft/`;
- `../robots/`;
- `../raids/`;
- `../systems/`;
- `../presentation/`.

## 37. Tuneable Parameters

No exact numeric balance values are fixed by this overview.

Potential tuneable values owned by later subsystem specifications include:

- module costs;
- station capacities;
- emergency reserve duration;
- structural limits;
- damage thresholds;
- production rates;
- crew support capacity;
- repair rates.

## 38. Open Questions

None at the station-overview level.

Detailed subsystem questions are resolved in their owning station specification before that subsystem reaches Design Complete.
