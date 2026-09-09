# Spacecraft Docking and Hangars

> **Status:** Draft  
> **Authority:** Docking compatibility, approach/alignment, hard-dock state, station service connections, hangar use, launch/undock validation, and docking failure behavior

## 1. Purpose

Docking connects spacecraft physically and operationally to stations, ships, and mission infrastructure.

It enables:

- boarding;
- cargo transfer;
- refueling;
- repairs;
- module refit;
- atmosphere transfer;
- power service;
- passenger transfer.

## 2. Docking Types

Baseline docking types:

- External Hard Dock;
- Hangar Berth;
- Temporary Service Dock;
- Mission/Boarding Dock.

## 3. Docking Compatibility

Both sides define a Docking Interface.

Compatibility checks:

- docking size class;
- connector type;
- geometry clearance;
- pressure compatibility where crew transfer is expected;
- current damage/operability;
- authorization.

## 4. Size Classes

Baseline size classes:

- S;
- M;
- L.

A berth supports one or more explicit classes.

A larger ship does not fit a smaller berth merely because collision could technically overlap.

## 5. Docking States

A ship can be:

- Free Flight;
- Docking Requested;
- Approach;
- Final Alignment;
- Capture;
- Hard Docked;
- Undocking;
- Launching.

## 6. Docking Request

Normal docking begins with a request to the target.

Request validation can check:

- berth availability;
- authorization;
- hostile alert;
- size compatibility;
- target operability;
- mission state.

## 7. Approach

The player flies toward the assigned berth/corridor.

Docking guidance provides:

- relative position;
- relative velocity;
- alignment;
- safe approach envelope.

## 8. Docking Assist

Docking Assist can:

- display guidance;
- damp relative velocity;
- align orientation;
- perform final approach automatically if enabled.

It uses actual maneuvering systems and cannot dock a ship with insufficient control authority.

## 9. Manual Docking

The player may perform the full approach manually.

Final capture still requires valid docking envelope.

## 10. Capture Envelope

Capture requires:

- position within tolerance;
- orientation within tolerance;
- relative velocity below threshold;
- compatible connector unobstructed.

## 11. Hard Dock

On successful capture:

- ship local-flight motion is constrained to docking target;
- docking connector becomes authoritative physical attachment;
- eligible service links can become available;
- boarding/transfer becomes possible.

## 12. No Instant Teleport Docking

Normal docking does not teleport a distant ship into a berth.

Mission transitions may spawn the ship at an authored already-docked state only when the transition fiction explicitly says so.

## 13. Service Connections

A hard dock can provide compatible:

- electrical shore power;
- fuel/propellant transfer;
- coolant transfer;
- cargo transfer;
- atmosphere/life-support service;
- data/control;
- repair access.

Each connection is finite and must exist on both sides.

## 14. Shore Power

When connected, the ship can:

- recharge batteries;
- power onboard systems;
- perform black start.

Shore power is limited by berth/service throughput and station power availability.

## 15. Refueling

Fuel/propellant transfer moves actual physical resources from station/ship storage.

Transfer stops when:

- destination full;
- source empty;
- connection broken;
- policy stops transfer.

## 16. Life-Support Service

Compatible service can:

- replenish ship atmosphere/life-support consumables;
- support cabin processing;
- refill player suit through ship/station interfaces.

No resource is created for free.

## 17. Cargo Transfer

Cargo transfer uses station logistics and ship cargo ownership.

A dock alone does not instantly merge station and ship inventory.

## 18. Passenger Transfer

Crew/survivors can physically transfer through a valid pressurized route or other explicitly safe transfer method.

Passenger capacity updates after transfer.

## 19. Hangar Berth

A Hangar Berth can provide additional capabilities:

- major repair;
- module refit;
- hull construction/commissioning;
- protected loading;
- detailed service access.

Not every external dock supports refit.

## 20. Hangar Capacity

A hangar defines:

- supported size class;
- occupied berth count;
- service capacity;
- launch clearance.

A hangar cannot service unlimited ships simultaneously.

## 21. Refit Lock

While major refit is active:

- ship is not flight-ready;
- affected modules can be removed/installed;
- launch is blocked until commissioning/validation completes.

## 22. Launch Validation

Before launch/undock, checks include:

- configuration flight-valid;
- mass <= Maximum Safe Operating Mass;
- docking clamps releasable;
- essential control/power available;
- no active refit lock;
- required pressure boundaries safe;
- berth exit clear.

## 23. Dangerous Override

Selected noncritical launch warnings can be overridden.

Hard blockers such as physically attached maintenance equipment or impossible mass limit cannot be ignored.

## 24. Undocking

Normal undocking:

1. stop service transfers;
2. close/seal pressure transfer path;
3. verify clamps/connectors;
4. release hard dock;
5. move to controlled separation.

## 25. Emergency Undock

Emergency Undock can skip selected normal service steps but cannot violate impossible physical state.

It may cause:

- connector damage;
- lost transfer hose;
- atmosphere loss;
- incomplete service.

## 26. Dock Damage

A docking connector can be:

- Damaged;
- Jammed;
- Disabled;
- Destroyed.

Damage can prevent capture or release.

## 27. Jammed Dock Recovery

Recovery can use:

- repair;
- manual release;
- emergency separation;
- station machinery.

The result follows physical damage rules.

## 28. Hostile Docking / Boarding

Hostile boarding is not normal docking authorization.

Combat/Raid systems may use:

- forced docking;
- breaching pod;
- hull breach;
- captured airlock.

Docking provides the physical interface rule but does not define raid success.

## 29. Multiple Docked Ships

Each berth holds its own ship state.

Shared station service capacity can bottleneck simultaneous refuel/repair operations.

## 30. Docking UI

The interface shows:

- assigned berth;
- compatibility;
- alignment;
- relative velocity;
- capture readiness;
- service connections;
- launch blockers.

## 31. Persistence

Docked state, berth assignment, active transfers, service connection state, and refit locks persist or reconstruct deterministically.

## 32. Edge Cases

If station power fails while docked, physical attachment remains; powered services can stop.

If atmosphere service disconnects, onboard cabin atmosphere remains its own finite state.

If the berth is damaged while ship is docked, recovery uses damage/emergency undock rules.

If cargo transfer is interrupted, already-transferred quantities remain at their new owner and untransferred quantities remain at source.

## 33. Explicit Non-Goals

Docking does not provide:

- global inventory merging;
- infinite service throughput;
- teleport docking;
- automatic repair simply from touching a station;
- docking large ships into incompatible bays.

## 34. Tuneable Parameters

Tuneable values include capture tolerance, approach velocity, service throughput, berth count, docking-assist strength, and refit time.

## 35. Dependencies

This specification depends on Flight, Ship Systems, Cargo, Configuration, station Logistics/Power/Atmosphere, Player Interaction, Missions, Combat/Raids, and Persistence.

## 36. Open Questions

None in the docking/hangar baseline.
