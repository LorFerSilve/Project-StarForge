# Spacecraft Systems

> **Status:** Draft  
> **Authority:** Onboard power, thermal, life-support, control, sensors, shield support, system states, dependencies, and ship-system prioritization

## 1. Purpose

Ship systems provide the functional infrastructure that makes a spacecraft more than a flying hit-point container.

The system should create readable dependencies between:

- power;
- propulsion;
- thermal management;
- life support;
- sensors;
- shields;
- weapons;
- cargo;
- docking;
- control.

## 2. System Architecture

A ship uses compact integrated onboard networks rather than full station-scale routing graphs.

The baseline distinguishes:

- Power Bus;
- Thermal Loop;
- Control/Data Network;
- Life-Support System;
- Propulsion System;
- Sensor/Avionics System;
- Shield System where equipped;
- Weapon/Utility Systems;
- Cargo System.

## 3. Power Bus

The ship Power Bus connects:

- generators;
- batteries;
- consumers.

Power values use the same conceptual units as station power.

The ship is smaller, so routing is more abstract but still finite.

## 4. Power Sources

Ship power sources can include:

- battery bank;
- fission reactor;
- fusion reactor;
- specialized advanced source.

A source defines:

- generation;
- startup requirement;
- fuel;
- heat;
- condition;
- ramp behavior.

## 5. Batteries

Batteries provide:

- startup power;
- burst power;
- emergency reserve;
- short-duration silent operation where later systems support it.

Battery energy is finite.

## 6. Essential Flight Loads

Baseline essential categories:

1. Flight Control / Avionics;
2. Life Support when occupied;
3. Maneuvering Control;
4. Thermal Safety;
5. Propulsion according to active maneuver;
6. Sensors;
7. Shields;
8. Weapons/Utilities;
9. Cargo/secondary systems.

Exact priority is configurable within safety limits.

## 7. Power Shedding

If demand exceeds available supply:

- batteries discharge where policy permits;
- lower-priority loads shed;
- systems enter reduced/offline state;
- UI reports the limiting cause.

A ship does not silently operate every system at full output during shortage.

## 8. Emergency Reserve

The player can reserve a fraction of battery capacity for:

- life support;
- control;
- distress communication;
- emergency maneuvering.

Automation respects reserve policy unless explicit emergency override is authorized.

## 9. Blackout

If no usable power remains:

- active propulsion stops;
- active thermal systems degrade;
- powered life support may stop;
- shields/weapons go offline;
- passive structure remains.

Emergency recovery depends on local backup and restart capability.

## 10. Black Start

A ship must have a recoverable black-start path.

Possible sources:

- protected emergency battery;
- external dock power;
- portable emergency power;
- tow/rescue assistance.

A total blackout must not create an unrecoverable save solely because all normal controls are dark.

## 11. Thermal Loop

Ships use one or more compact coolant/thermal loops.

A loop contains:

- coolant;
- pump;
- heat exchangers;
- radiator/rejection hardware;
- connected heat-producing systems.

## 12. Heat Sources

Major heat sources include:

- reactor;
- main drive;
- thrusters;
- shields;
- weapons;
- high-output sensors;
- utility equipment.

## 13. Radiators and Rejection

Heat must eventually be rejected.

Ship radiator systems may be:

- fixed;
- deployable;
- hull-integrated.

A combat configuration can trade radiator exposure for reduced sustained output, but heat cannot disappear.

## 14. Thermal States

Relevant systems can be:

- Nominal;
- Elevated;
- Degraded;
- Critical;
- Shutdown.

The thermal system can throttle propulsion/weapons/reactor output to protect the ship.

## 15. Heat Storage

Some ships can use finite heat storage for short high-output bursts.

Stored heat must later be rejected.

## 16. Coolant Loss

Damage can leak coolant.

Loss reduces thermal capacity.

Coolant is a physical GDS-4 resource and must be repaired/replenished.

## 17. Control/Data Network

The Control Network carries:

- pilot commands;
- navigation;
- sensor data;
- subsystem control;
- targeting information.

It is not modeled as packet-level networking.

Damage can create:

- local control loss;
- sensor loss;
- module isolation.

## 18. Avionics

Avionics provides:

- attitude control;
- flight stabilization;
- navigation solution execution;
- engine/thruster command;
- docking assistance.

A ship without functioning avionics can retain limited manual/emergency control only if the hull/system explicitly supports it.

## 19. Sensors

Ship sensors provide information on:

- nearby craft;
- navigation references;
- hazards;
- docking targets;
- mission contacts;
- target tracks.

Sensor information is not omniscient.

Range/quality depend on actual systems and future World/Combat rules.

## 20. Life Support

An occupied sealed ship tracks at minimum:

- cabin pressure;
- breathable atmosphere state;
- CO2/contaminant handling;
- temperature;
- life-support reserve/capacity.

For compact craft, this can use one cabin atmosphere volume.

Larger walkable ships can have several authored compartments.

## 21. Life-Support Inputs

Life support can require:

- power;
- Fresh Water;
- stored atmosphere;
- functioning scrubber/processor.

Occupied endurance is finite if regeneration/resupply cannot continue.

## 22. Player Suit Integration

A docked/pressurized ship can refill the player's Life-Support Reserve if it has:

- sufficient ship atmospheric capacity;
- compatible refill interface;
- power/control.

The transfer is finite and cannot create atmosphere from nothing.

## 23. Crew/Passenger Capacity

Life-support capacity must be sufficient for actual occupancy.

A ship may physically contain extra seats but cannot safely support unlimited passengers.

## 24. Shield Support

Personal/ship shields are distinct.

Ship shields depend on:

- shield module;
- power;
- thermal support;
- control.

Combat owns damage interaction.

## 25. Utility Systems

Utility modules can include:

- tractor/tow interface;
- mining support;
- repair systems;
- drone/robot bay;
- enhanced scanner;
- field refinery support where explicitly allowed.

Each module uses real power/heat/cargo/slot capacity.

## 26. System States

A module can be:

- Offline;
- Standby;
- Active;
- Degraded;
- Overheated;
- Faulted;
- Disabled;
- Destroyed.

Owning subsystem can add more specific state.

## 27. Startup

Some systems have startup duration/energy.

Starting a main reactor/drive is not assumed instantaneous.

## 28. Shutdown

Systems can use:

- Normal Shutdown;
- Emergency Shutdown.

Emergency shutdown can protect hardware but may interrupt flight/combat capability.

## 29. Automation

Onboard automation can:

- maintain power priority;
- protect thermal limits;
- maintain cabin environment;
- manage batteries;
- stabilize flight;
- execute docking assistance;
- report faults.

It does not choose the player's destination or mission.

## 30. Manual Override

Player can manually:

- disable systems;
- change priority;
- reserve battery;
- throttle output;
- deploy/retract eligible thermal hardware;
- authorize emergency restart.

Safety-critical overrides require explicit warning.

## 31. Crew Support

Crew specialists can later improve:

- diagnosis;
- repair;
- navigation;
- weapon coordination;
- engineering response.

The ship can still be operated by the player without mandatory multi-crew for baseline craft.

## 32. System Dashboard

Ship UI communicates:

- generation/demand;
- battery charge;
- fuel;
- temperatures;
- coolant;
- life support;
- shield;
- sensor state;
- faults;
- offline systems.

## 33. Persistence

All gameplay-relevant system state persists:

- power;
- battery;
- fuel;
- coolant;
- temperatures;
- life-support state;
- faults;
- configuration.

## 34. Edge Cases

If reactor fails but batteries remain, the ship can continue temporarily within available power.

If thermal capacity fails under thrust, propulsion can throttle/shutdown before destruction unless protection is overridden.

If cabin life support fails, existing atmosphere degrades over time rather than vanishing instantly.

If control network is partially damaged, unaffected local systems continue where their architecture supports it.

## 35. Explicit Non-Goals

The system does not simulate:

- AC phase;
- per-wire electrical circuits;
- molecular thermodynamics;
- packet-level avionics;
- arbitrary heat deletion;
- infinite batteries/life support.

## 36. Tuneable Parameters

Tuneable values include power, energy, heat, coolant, life-support throughput, startup time, sensor range, and protection thresholds.

## 37. Dependencies

This specification depends on Ship Configuration, Propulsion, Flight, Cargo, GDS-4 resources, GDS-5 field survival, station power/thermal philosophy, Combat, and Damage/Repairs.

## 38. Open Questions

None in the onboard-system baseline.
