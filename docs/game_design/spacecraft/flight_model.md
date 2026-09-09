# Spacecraft Flight Model

> **Status:** Draft  
> **Authority:** Real-time local-space piloting, translational/rotational control, inertia, flight assist, camera-equivalent mechanics, speed envelope, collision boundary, and disabled-flight behavior

## 1. Purpose

Local flight should feel responsive enough for action gameplay while preserving meaningful spacecraft mass, inertia, thrust, and system failure.

## 2. Scope

This document governs real-time piloting inside bounded space gameplay zones.

It does not govern strategic inter-location travel.

## 3. Flight Style

The baseline uses a **six-degree-of-freedom inertial spaceflight model with configurable flight assistance**.

The ship has:

- position;
- linear velocity;
- orientation;
- angular velocity;
- mass;
- translational thrust;
- rotational thrust.

## 4. Primary Controls

Flight actions include:

- thrust forward/back;
- strafe left/right;
- strafe up/down;
- pitch;
- yaw;
- roll;
- boost/high-output thrust where equipped;
- brake/stabilize;
- flight-assist toggle;
- target/docking actions.

Exact default bindings are finalized with controls/presentation.

## 5. Flight Assist On

With Flight Assist enabled:

- releasing translational input requests controlled velocity stabilization;
- rotational input stabilizes toward commanded angular state;
- avionics use available thrusters to reduce unwanted drift.

Assistance consumes real thruster authority and cannot violate damage/power limitations.

## 6. Flight Assist Off

With Flight Assist disabled:

- ship preserves inertial linear/angular motion except for environmental forces;
- the player directly commands thrust/torque.

This enables advanced maneuvering.

## 7. Mass

Actual ship mass includes:

- hull;
- installed modules;
- cargo;
- fuel/propellant;
- ammunition;
- occupants.

Mass directly affects acceleration.

## 8. Translational Acceleration

Conceptually:

`Acceleration = Available Thrust / Current Mass`

Exact response can include gameplay smoothing but may not ignore mass.

## 9. Rotational Acceleration

Rotational response depends on:

- available attitude-control thrust;
- hull class/inertia abstraction;
- current mass distribution abstraction.

The baseline does not require full rigid-body tensor simulation for every module.

## 10. Speed Envelope

Space has no real drag, but gameplay zones require a practical controllable speed envelope.

The ship therefore uses an **Avionics-Controlled Cruise Speed Limit** during normal flight.

This represents control/safety doctrine rather than atmospheric drag.

## 11. Speed Limit and Assist Off

Flight Assist Off does not grant infinite velocity.

The avionics/control architecture still enforces the local gameplay-zone maximum safe relative speed.

Special scripted/high-speed travel belongs to strategic travel.

## 12. Boost

A compatible propulsion system can provide temporary Boost.

Boost consumes increased:

- power;
- propellant/fuel where applicable;
- thermal capacity.

Boost raises acceleration and may raise the temporary safe-speed envelope.

## 13. Braking

Brake/Stabilize commands opposing thrust.

Stopping distance therefore depends on:

- current velocity;
- available reverse/maneuver thrust;
- mass;
- power;
- damage.

## 14. No Instant Stop

The ship cannot instantly stop from high speed merely because input is released.

Flight Assist applies actual deceleration.

## 15. Thruster Authority

Maneuvering requires functioning control thrusters.

Damaged thrusters can reduce:

- strafe;
- rotation;
- braking;
- docking precision.

## 16. Main Drive vs Maneuvering Thrusters

Main propulsion primarily provides high forward thrust.

Maneuvering thrusters provide:

- lateral movement;
- reverse/braking;
- attitude control.

Some advanced drives can provide stronger multi-axis thrust where defined.

## 17. Gravity

Local space zones can include gravity sources only when World design explicitly requires it.

The baseline does not simulate full N-body orbital mechanics.

## 18. Atmospheric Flight Boundary

Planetary landing sequences can use simplified gravity/descent handling for compatible craft.

Full aerodynamic lift/stall simulation is outside baseline.

## 19. Camera Equivalence

Third-person chase and cockpit view operate the same:

- position;
- thrust;
- target state;
- weapons;
- sensors;
- docking logic.

Camera choice never changes ship stats.

## 20. Chase Camera

Primary flight view follows ship motion with configurable distance/sensitivity.

Camera smoothing cannot conceal collision or materially alter input latency.

## 21. Cockpit View

Cockpit view uses an authored cockpit camera for ships that support it.

HUD may be spatially integrated, but all required information remains accessible.

## 22. Collision

Physical collision can occur with authored:

- ships;
- stations;
- asteroids;
- large debris;
- terrain/structures.

Collision consequence belongs to Damage/Combat integration.

## 23. Collision Avoidance Assistance

Docking/low-speed assist can warn or limit unsafe approach.

It does not make collision impossible.

The player can override assistance where allowed.

## 24. World Bounds

A local flight zone has gameplay boundaries.

Approaching a boundary triggers navigation/mission behavior rather than allowing endless empty-space flight.

The boundary should be explained through the location/travel context.

## 25. Disabled Propulsion

If main propulsion fails:

- current momentum remains;
- maneuver thrusters can continue if operational;
- the ship may be unable to leave the zone strategically.

## 26. Disabled Maneuvering

If attitude/translation thrusters fail:

- the ship can retain momentum;
- main drive may still provide forward thrust;
- precise control/docking becomes impossible.

## 27. Power Loss

During power loss:

- active thrusters stop if unpowered;
- momentum remains;
- passive collision still applies.

Emergency battery may restore limited control.

## 28. Pilot Incapacitation

If the player becomes unable to pilot while in local flight, automation can enter Safe Drift/Stabilize mode if avionics/power remain.

Future combat/mission rules own failure outcome.

## 29. Docking Integration

Docking requires low relative speed and valid orientation/position envelope.

Docking Assist can provide guidance or automatic final alignment.

## 30. Travel Transition

Strategic travel can begin only when Navigation/Propulsion conditions are met.

Transition from local flight does not require manually flying astronomical distances.

## 31. HUD

Flight UI communicates:

- velocity;
- relative velocity where relevant;
- thrust;
- flight-assist state;
- boost;
- power/thermal warning;
- propulsion condition;
- target/docking information.

## 32. Persistence

If a save is permitted in local flight, position, velocity, orientation, system state, and zone identity persist.

## 33. Edge Cases

If mass changes by cargo transfer while docked, flight performance recalculates before launch.

If a thruster fails mid-brake, stopping performance updates immediately.

If Flight Assist is toggled off while drifting, current velocity is preserved.

If cockpit camera is unavailable/damaged visually, third-person remains mechanically equivalent unless the ship's actual avionics/sensors are also damaged.

## 34. Explicit Non-Goals

The baseline does not require:

- full orbital mechanics;
- atmospheric fighter aerodynamics;
- instant-stop arcade movement;
- infinite local velocity;
- camera-dependent flight capability.

## 35. Tuneable Parameters

Tuneable values include thrust, angular acceleration, cruise-speed envelope, boost multiplier, camera smoothing, braking assistance, and docking-speed thresholds.

## 36. Dependencies

This specification depends on Ship Systems, Propulsion, Navigation, Docking, Damage, Combat, World, and Player Controls.

## 37. Open Questions

None in the local-flight baseline.
