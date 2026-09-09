# Station Construction

> **Status:** Draft  
> **Authority:** Player-facing creation, modification, construction lifecycle, and deconstruction of home-station modules

## 1. Purpose

The construction system allows the player to expand and reorganize the persistent home station while preserving:

- readable spatial design;
- functional infrastructure;
- meaningful resource cost;
- strategic layout;
- technical feasibility for a custom engine.

Station construction is modular rather than unrestricted free-form geometry creation.

## 2. Player Experience

The intended flow is:

```text
Identify station need
    ↓
Enter Construction Mode
    ↓
Choose module or structural element
    ↓
Preview placement
    ↓
Validate structural / spatial requirements
    ↓
Place blueprint
    ↓
Reserve / deliver required materials
    ↓
Construction work occurs
    ↓
Module reaches structural completion
    ↓
Required systems are commissioned
    ↓
Module becomes usable
```

Construction should make station growth visible rather than instantly replacing one menu icon with another.

## 3. Construction Mode

Permanent station construction is performed through **Construction Mode**.

Construction Mode:

- is available only while the player has access to the home station's construction-control capability;
- uses a dedicated free spatial camera;
- provides module previews, connection visualization, occupancy visualization, and relevant infrastructure overlays;
- pauses active game-world simulation while open;
- cannot be entered during an active station combat state;
- cannot be used to directly modify external mission environments.

Pausing prevents geometry edits from racing against combat, decompression, AI movement, or active machinery state transitions.

## 4. Construction Space

The home station uses an orthogonal three-dimensional construction lattice.

Modules occupy one or more discrete construction cells.

A module definition specifies:

- occupied cell dimensions;
- valid orientations;
- structural connection ports;
- traversal ports;
- utility interfaces;
- exterior-only or interior-capable status;
- construction requirements.

The player does not manually manipulate arbitrary vertices or meshes.

## 5. Orientation

Standard structural modules rotate in 90-degree increments around supported station axes.

Special modules may restrict rotation where their authored geometry or function requires a specific orientation.

A module never visually accepts an orientation that its collision, traversal, structural, or service definitions cannot support.

## 6. Module Categories

Construction content is classified into:

### Structural Elements

Examples:

- corridors;
- junctions;
- shafts;
- bulkhead sections;
- airlocks;
- frames;
- adapter sections.

### Functional Modules

Examples:

- reactors;
- hydroponics;
- research rooms;
- refineries;
- fabrication rooms;
- crew quarters;
- medical rooms;
- robot facilities.

### External Functional Modules

Examples:

- radiators;
- sensors;
- antennas;
- shield emitters;
- turrets;
- missile batteries.

### Docking Structures

Examples:

- docking collars;
- docking arms;
- shuttle bays;
- hangars.

### Interior Equipment

Smaller equipment placed inside compatible modules.

Interior-equipment rules may use a finer placement grid but may not violate access, collision, or safety requirements.

## 7. Blueprint Placement

Confirming a valid preview creates a **Construction Blueprint**.

A blueprint is persistent planned construction.

It records:

- module type;
- position;
- orientation;
- required resources;
- delivered resources;
- construction progress;
- priority;
- dependencies;
- current construction state.

Blueprint geometry is not treated as a completed solid module for normal station function.

## 8. Blueprint Placement Validation

A permanent module blueprint is valid only when all applicable conditions pass:

1. every occupied construction cell is available;
2. module geometry does not overlap incompatible existing geometry;
3. at least one required structural connection can attach to the existing station or to an already-valid planned dependency;
4. connector types are compatible;
5. authored orientation restrictions are satisfied;
6. required clearance volumes remain unobstructed;
7. docking clearance is preserved where applicable;
8. the placement does not overlap a protected no-build volume;
9. special module prerequisites are satisfied.

The UI must state which validation rule failed.

## 9. Planned Construction Chains

The player may plan several connected modules before the first one is built.

A blueprint may therefore depend structurally on another blueprint.

Example:

```text
Existing Station
    ↓
Corridor Blueprint A
    ↓
Junction Blueprint B
    ↓
Research Module Blueprint C
```

B and C remain blocked until their required upstream structural dependency becomes valid.

This allows meaningful planning without requiring the player to wait after every individual module.

## 10. Resource Reservation

When a blueprint is placed, the game calculates its material requirements.

Resource handling distinguishes:

- **required** resources;
- **reserved** resources;
- **delivered** resources;
- **consumed** resources.

Reservation prevents the same stored item from being promised to multiple construction jobs.

Resources are not considered consumed merely because a blueprint exists.

Exact inventory-routing behavior belongs to storage/logistics and resources specifications.

## 11. Material Availability

A blueprint may be placed even when every required resource is not currently available.

Such a blueprint enters **Awaiting Materials**.

This allows the player to plan future expansion.

Construction cannot consume resources that do not exist.

## 12. Construction Labor

Physical construction requires construction labor.

Labor can be provided by systems such as:

- the starting construction capability;
- construction drones;
- engineering crew;
- advanced automated construction machinery.

The exact labor rates are tuneable.

A blueprint with all materials but no available construction labor remains queued.

## 13. Player Assistance

The player may manually assist compatible construction jobs using the appropriate engineering/construction tool.

Manual assistance:

- accelerates the active job;
- does not bypass missing materials;
- does not bypass structural validation;
- does not replace specialized commissioning requirements.

This gives early-game hands-on construction value without making manual building mandatory forever.

## 14. Construction Priority

The player can assign construction priority.

The station construction manager allocates available automated labor according to priority and eligibility.

A higher priority does not permit an invalid or resource-starved job to consume unavailable inputs.

## 15. Construction States

Every blueprint follows the relevant subset of these states:

```text
Planned
↓
Awaiting Materials
↓
Ready
↓
Under Construction
↓
Structural Complete
↓
Commissioning
↓
Completed
```

Additional terminal state:

```text
Cancelled
```

A constructed module may later be:

- operational;
- inactive;
- unpowered;
- isolated;
- faulted;
- damaged;

but those are operational states rather than construction states.

## 16. Planned

The blueprint exists but has not yet met all requirements for work.

## 17. Awaiting Materials

At least one required construction input is unavailable or undelivered.

## 18. Ready

All required inputs for the next construction stage are available and valid construction labor can begin.

## 19. Under Construction

Labor is actively converting delivered resources into the module.

An incomplete module provides no normal functional output unless a later subsystem explicitly defines a staged capability.

## 20. Structural Complete

The module's physical structure exists.

At this state it can participate in structural topology according to `structural_rules.md`.

A functional module is not necessarily operational yet.

## 21. Commissioning

Commissioning verifies the completed module's operational requirements.

Depending on module type, commissioning can require:

- valid power connection;
- valid control/data connection;
- atmosphere compatibility;
- cooling;
- fluid connection;
- logistics access;
- specialist setup.

The owning subsystem defines its requirements.

## 22. Completed

Construction work is finished.

Completion does not guarantee the module is currently operational.

Example:

> A completed refinery with no electrical power is a completed but unpowered refinery.

## 23. Cancellation Before Work

Cancelling a blueprint before any resources have been consumed:

- releases all reservations;
- removes the blueprint;
- loses no construction materials.

## 24. Cancellation During Construction

If construction has consumed materials:

- all unconsumed reserved/delivered materials are returned where physically recoverable;
- consumed materials produce a salvage return rather than a guaranteed full refund;
- exact salvage fractions are tuneable;
- destroyed or irrecoverable material is not duplicated.

## 25. Deconstruction of Completed Modules

Completed modules cannot be magically repositioned.

To relocate a completed permanent module, the player must:

1. deconstruct it;
2. recover applicable materials/components;
3. construct the replacement location.

Deconstruction is itself a job that requires time/labor.

## 26. Deconstruction Restrictions

A module cannot begin deconstruction when:

- it contains the player;
- it contains crew who have no valid evacuation path;
- it is the Command Core;
- it would immediately create an invalid unsupported structural configuration that is not part of the same approved deconstruction plan;
- a docked spacecraft requires that exact structure for its current connection;
- the station is in active combat;
- another explicit subsystem safety rule blocks removal.

The interface must explain the blocking condition.

## 27. Cascading Deconstruction Plan

The player may select a structurally dependent branch for joint deconstruction.

The planner shows all modules that would lose their only valid structural path.

The player must explicitly confirm the full affected set.

No hidden cascade deletion occurs.

## 28. Interior Equipment Relocation

Non-structural interior equipment may be repositionable without full deconstruction when:

- it is offline;
- its placement target is compatible;
- required clearance exists;
- no connected resource or safety state forbids movement.

This rule does not apply to permanent structural modules.

## 29. Collision and Clearance

Module definitions include:

- solid occupied volume;
- traversal clearance;
- door clearance;
- maintenance clearance where required;
- docking or weapon clearance where required.

A preview that would create impossible player access or physically intersect incompatible geometry is invalid.

## 30. Access Requirement

Crewed or player-usable interior modules must have a valid intended traversal connection before commissioning.

Uncrewed external devices may be serviced through authored maintenance rules instead.

The exact accessibility graph is owned by `structural_rules.md`.

## 31. Utility Connections

Structural attachment does not automatically imply every utility is available.

Compatible adjacency may create connection opportunities for:

- power;
- control/data;
- atmosphere;
- thermal transfer;
- water;
- logistics.

Each network defines whether connection is automatic, configurable, or requires explicit components.

## 32. Construction and Atmosphere

New interior construction does not instantly create safe breathable volume.

Pressure and atmosphere behavior follows the atmosphere specification.

A newly created compartment may remain:

- depressurized;
- isolated;
- uninhabitable;

until its sealing and life-support requirements are satisfied.

## 33. Construction and Damage

Blueprints and incomplete structures can be damaged if an explicitly defined gameplay event reaches them outside Construction Mode.

Damage does not generate resources.

A damaged incomplete build may require replacement material before continuing.

## 34. Research Locks

A module can appear in the construction catalog only when its blueprint/technology has been unlocked.

Locked technology cannot be bypassed through placement tricks.

## 35. Module Upgrades

A higher-tier function is not assumed to replace a module invisibly.

An upgrade definition must specify whether it is:

- an installed component upgrade;
- an in-place reconstruction;
- a replacement module;
- an attached extension.

This prevents ambiguous "level-up building" behavior.

## 36. Build Catalog

The build interface organizes available content by functional category.

Every catalog entry exposes, at minimum:

- name;
- purpose;
- physical footprint;
- resource requirements;
- prerequisite technology;
- major system requirements;
- connector requirements.

The player should understand why a module cannot be built or operated.

## 37. Construction Preview Feedback

The preview uses distinct states for:

- valid placement;
- invalid placement;
- planned dependency;
- missing prerequisite;
- clearance conflict;
- structural conflict.

Color cannot be the only indicator; iconography/text or patterns must provide redundant accessibility cues.

## 38. Undo Behavior

Construction planning may support undo for blueprint-placement actions while no physical construction progress has occurred.

Undo must not reverse already completed in-world construction, consumed resource history, combat damage, or other persistent gameplay events.

## 39. Save and Load

Construction blueprints, reservations, delivered resources, job progress, and priorities are persistent.

Loading a save must restore the same construction state rather than silently completing or cancelling jobs.

## 40. Explicit Non-Goals

The construction system does not provide:

- arbitrary vertex editing;
- unrestricted voxel building;
- instant free relocation of completed modules;
- construction during active station combat;
- free placement without structural connection;
- hidden automatic destruction of dependent modules;
- guaranteed full refund from completed structures.

## 41. Tuneable Parameters

Tuneable values include:

- construction duration;
- labor rate;
- salvage fractions;
- priority weights;
- manual-assistance rate;
- module costs;
- construction-drone capacity.

## 42. Dependencies

Authoritative dependencies include:

- [Home Station Overview](overview.md);
- `structural_rules.md`;
- `storage_and_logistics.md`;
- `damage_and_repairs.md`;
- `../resources/`;
- `../crew/`;
- `../systems/save_and_persistence.md`;
- `../presentation/ui_ux.md`.

## 43. Open Questions

None in the current construction-rule baseline.

The specification remains Draft until its dependent resource, logistics, structural, and presentation rules are cross-validated.
