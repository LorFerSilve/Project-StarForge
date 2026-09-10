# Robot Squad Composition

> **Status:** Design Complete  
> **Authority:** Tactical squad identity, membership, Command Load/TCC limits, transport/support constraints, deployment, reserve units, squad preparation, and mission extraction of robots

## 1. Purpose

Squads give the player strategic force composition while keeping direct action readable and technically manageable.

## 2. Tactical Squad

A Tactical Squad is a persistent pre-deployment grouping of player-owned robots.

A squad records:

- Squad ID;
- name;
- member Robot IDs;
- preferred formation;
- Rules of Engagement;
- default Fallback Order;
- command groups.

## 3. One Active Field Squad

The player controls one primary Tactical Squad in a normal external mission.

Additional allied/mission robots can exist, but they are not automatically part of the player's command roster.

## 4. Squad Size Constraints

A deployed squad must satisfy all of:

1. Tactical Control Capacity;
2. transport footprint;
3. compatible transport bays/racks;
4. charging/support capacity;
5. mission-specific deployment restrictions;
6. operational robot state.

## 5. Command Load

Each Robot Class has a Command Load.

Total Squad Command Load:

Sum(Command Load of all active commanded robots)

must be <= Available TCC.

## 6. TCC Is Not Robot Count

Example with TCC 8:

- 8 Recon Drones = load 8;
- 4 Assault Units = load 8;
- 2 Heavy Mechs = load 8;
- 1 Heavy + 2 Assault = load 8.

This creates meaningful composition tradeoffs.

## 7. Baseline Command Loads

- Recon Drone = 1;
- Repair Drone = 1;
- Hacker Unit = 1;
- Assault Unit = 2;
- Shield Unit = 2;
- Breaching Unit = 2;
- Demolition Unit = 2;
- Heavy Mech = 4.

## 8. TCC Sources

Available field TCC is calculated from:

- player's command equipment baseline;
- deployed ship command/sensor support where compatible;
- researched command upgrades;
- temporary authored mission support.

GDS-12 finalizes progression values.

## 9. No TCC Overdeployment

The player cannot deploy above TCC and simply accept "reduced AI quality."

Over-capacity deployment is blocked.

This avoids undefined partial command ownership.

## 10. Transport Footprint

Robot Rack Units:

- Light = 1;
- Medium = 2;
- Heavy = Heavy Robot Bay + 4-equivalent support.

Transport capacity is checked separately from TCC.

## 11. Robot Rack

A spacecraft Robot Rack:

- secures robots during travel;
- provides charging/data connection;
- supports deployment/recovery.

Rack capacity is a GDS-6 ship-module capability.

## 12. Heavy Robot Bay

Heavy Mech deployment requires a Heavy Robot Bay or authored equivalent transport.

An ordinary passenger seat/cargo crate cannot substitute.

## 13. Ship Support

A mission ship must provide enough:

- rack/bay space;
- charging/energy support;
- deployment access.

A robot may be transported powered down to reduce support demand where the rack permits it.

## 14. Squad Preparation

Before mission deployment, the player selects:

- member Robot IDs;
- installed loadout;
- ammunition;
- energy charge;
- ROE;
- fallback;
- command groups;
- optional spare parts.

No supplies are created by the preparation UI.

## 15. Deployment Readiness

A Robot ID is deployable only if:

- not Destroyed;
- operational enough for class;
- valid configuration;
- sufficient minimum energy;
- command link compatible;
- transport valid;
- mission environment compatible or risk override permitted.

## 16. Deployment Location

Robots deploy from a physical point such as:

- landed ship ramp/bay;
- docked ship airlock/rack;
- mission transport;
- station defense storage.

They do not appear at player's crosshair.

## 17. Powered-Down Transport

A robot can travel powered down.

On deployment:
- rack/ship can bring it to required startup energy;
- boot/activation takes authored time;
- no free full charge is granted.

## 18. Mid-Mission Deployment

Additional robots can enter only if:

- they were physically transported in reserve;
- a valid deployment route exists;
- TCC becomes available;
- mission state permits release.

They cannot be summoned from Horizon Station.

## 19. Reserve Robots

Robots physically aboard the ship but not active can be designated Reserve.

Reserve units:
- occupy transport space;
- can recharge/service if rack supports it;
- consume no active TCC until activated.

## 20. Swapping Active Robot

To activate a Reserve robot when TCC is full:

- another active robot must be returned/deactivated/released from commanded squad;
- TCC recalculates;
- new robot physically deploys.

## 21. Command Groups

Squad members can be assigned to command groups.

Example:

- Group 1 Assault;
- Group 2 Support;
- Group 3 Breach.

Groups are UI/control organization, not separate AI ownership.

## 22. Formation

Baseline formation modes:

- Follow / Adaptive;
- Tight;
- Spread;
- Column.

Formation affects preferred relative positioning but never overrides path safety.

## 23. Heavy Pathing

Heavy units can force squad route choice because they require larger clearances.

The squad does not teleport a Heavy Mech through a doorway it cannot fit.

## 24. Robot Casualties

If a robot becomes Disabled/Destroyed:

- it immediately stops contributing active TCC commands;
- its physical body remains;
- other squad members do not respawn it.

TCC becomes available for another physically available reserve unit.

## 25. Disabled Robot Recovery

A disabled robot can be:

- field-repaired;
- carried/towed if capability exists;
- loaded into transport;
- abandoned.

Recovery capacity must physically support its size/mass.

## 26. Destroyed Robot Wreck Recovery

A wreck can occupy transport/cargo/recovery capacity.

Recovering wreck is optional unless mission/objective requires it.

## 27. Robot Extraction

A robot is safely recovered from a mission when:

- physically aboard/secured in valid extraction transport;
- or explicitly transitioned through another authorized robot extraction method.

Robot proximity to extraction is insufficient.

## 28. Player Extraction Without Robots

The player may extract while robots remain behind if mission/extraction rules permit.

Result:

- secured robots persist;
- abandoned robots remain at location or are lost according to mission/world persistence;
- Destroyed robots remain destroyed.

The game warns before knowingly abandoning recoverable owned robots.

## 29. Mission Failure

On ordinary player mission failure, robots resolve from actual physical state:

- secured aboard ship → preserved with ship;
- active operational/disabled in field → recoverability depends on recovery fiction/location;
- destroyed → lost;
- authored emergency recovery can recover eligible units only if its capability says so.

There is no automatic squad refund.

## 30. Squad Presets

The player can save squad configuration presets.

A preset references desired Robot IDs/classes/loadouts but does not duplicate units.

If referenced unit is unavailable, preset reports missing member.

## 31. No Disposable Human Mixing

Human crew are not counted as robotic squad Command Load and are not treated as interchangeable troop slots.

Authored human companions, if any, use their own mission rules.

## 32. Example Compositions

### Balanced Load 8

- 2 Assault = 4;
- 1 Shield = 2;
- 1 Repair = 1;
- 1 Recon = 1.

Total = 8.

### Breach Load 8

- 1 Heavy = 4;
- 1 Breacher = 2;
- 1 Hacker = 1;
- 1 Repair = 1.

Total = 8.

### Recon/Utility Load 8

- 2 Assault = 4;
- 1 Recon = 1;
- 1 Hacker = 1;
- 1 Repair = 1;
- 1 additional Light = 1.

Total = 8.

These are examples, not mandatory caps.

## 33. Explicit Non-Goals

Squads do not use:

- one fixed robot-count cap independent of class;
- summoning from station;
- over-TCC degraded control;
- free casualty replacement;
- passenger seats as Heavy Mech bays;
- troop presets that duplicate units.

## 34. Tuneable Parameters

TCC progression, Rack capacity, minimum startup energy, formation spacing, reserve support, recovery capacity, and transport-module values are tuneable.

## 35. Dependencies

This specification depends on Overview, Classes, AI, Tactical Commands, GDS-6 Spacecraft, GDS-8 Mission deployment/extraction, Damage/Repair, and Raids/Progression.

## 36. Open Questions

None in the squad/deployment baseline.
