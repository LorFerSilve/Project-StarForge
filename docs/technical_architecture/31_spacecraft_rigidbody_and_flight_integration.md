# Spacecraft Rigid-Body and Flight Integration

> **Status:** Architecture Complete  
> **Authority:** Dynamic spacecraft body model, 6DoF thrust/torque application, mass/inertia integration, flight assist, speed envelope, propulsion failure, local gravity boundary, and render/state handoff

## 1. Purpose

Spacecraft must obey the GDS six-degree-of-freedom inertial flight model using real dynamic rigid-body motion while preserving project-owned gameplay rules for propulsion, power, damage, navigation, and docking.

## 2. Body Model

An active spacecraft in local flight uses one primary Dynamic rigid body representing the ship hull mass/inertial frame.

Installed modules, cargo, fuel, ammunition, and occupants contribute to gameplay-computed mass/inertia inputs but are not modeled as independent rigid bodies by default.

## 3. Hull Collision

Ship collision geometry uses authored compound/convex/static-compatible hull proxies rather than relying on render triangles.

Large concave ship geometry can use multiple convex/compound collision parts.

## 4. Persistent Identity

The Dynamic physics body is mapped to the existing persistent `ShipId` through runtime/activation state.

The Jolt body identity is never serialized and can be recreated after scene activation/load.

## 5. Physical State

High-frequency active flight state includes at minimum:

- origin-relative position;
- orientation quaternion;
- linear velocity;
- angular velocity;
- current mass;
- effective inertia representation;
- sleep/awake state where relevant.

Ship domain remains authority for modules, fuel, power, damage, and configuration.

## 6. Mass Recalculation

Current ship mass is recomputed from authoritative ship configuration/state whenever a committed change can affect mass, including:

- cargo transfer;
- fuel/propellant change;
- ammunition change;
- module installation/removal;
- occupants/robots where modeled into operating mass.

Mass changes commit at stable simulation boundaries before subsequent thrust integration.

## 7. Inertia Model

The rigid body uses a project-computed effective inertia tensor/approximation derived from hull class and current configuration.

The baseline does not require simulating every module as a separate rigid mass.

The inertia model must still preserve the GDS principle that heavier/larger configurations rotate less aggressively for equal torque.

## 8. Thruster Model

Ship propulsion exposes project-owned `ThrusterAuthority` for each axis/direction, derived from:

- installed propulsion modules;
- functioning maneuver thrusters;
- main drive;
- available power;
- propellant/fuel requirements;
- damage;
- thermal limits;
- boost state.

Physics receives only currently authorized force/torque commands.

## 9. Translational Thrust

Player/autopilot translational intent resolves into body-space force requests for:

- forward/back;
- left/right;
- up/down.

For each axis, applied force cannot exceed current available thrust authority.

Resulting acceleration naturally follows force/mass through rigid-body integration.

## 10. Rotational Thrust

Pitch/yaw/roll intent resolves into torque requests bounded by current attitude-control authority.

The gameplay flight controller can use tuned response curves but cannot directly set arbitrary angular velocity while claiming physical thruster motion.

## 11. Main Drive

Main Drive primarily contributes forward force.

It can also participate in braking/other directions only when the configured propulsion design explicitly supports that capability.

## 12. Maneuver Thrusters

Maneuvering thrusters provide the six-axis control envelope needed for:

- lateral translation;
- vertical translation;
- reverse/braking;
- pitch/yaw/roll;
- docking precision.

Damage can reduce selected axes independently when the ship model exposes that granularity.

## 13. Flight Assist ON

Flight Assist ON is a project-owned control loop that converts desired local velocity/angular-state behavior into physically bounded forces/torques.

When player releases translational input, assist requests counter-thrust toward the configured target drift state, normally zero local commanded drift.

It cannot exceed actual available thruster authority.

## 14. Flight Assist OFF

With Flight Assist OFF:

- no automatic translational drift cancellation is applied except the local gameplay speed-envelope controller;
- angular stabilization follows the GDS setting/intent contract;
- current linear/angular momentum persists absent external force/torque.

## 15. Brake/Stabilize

Brake/Stabilize computes bounded opposing force/torque against current relative velocity/angular velocity using available thrusters.

Stopping distance therefore emerges from current mass, velocity, and available authority.

## 16. Avionics Speed Envelope

The GDS local maximum speed is not atmospheric drag.

TA-5 implements it as an avionics/control constraint:

- player thrust that would increase speed beyond the allowed local envelope is limited;
- flight-assist/controller can apply physically available opposing thrust;
- external collision/forces can temporarily produce exceptional velocity that the controller then attempts to recover within limits.

Physics itself does not apply fake universal drag to space.

## 17. Flight Assist OFF and Speed Limit

Disabling Flight Assist does not disable the avionics safe-speed envelope.

The controller can suppress further acceleration along excessive velocity and command bounded recovery thrust without instant velocity clamping.

## 18. Boost

Boost modifies authorized propulsion limits according to the owning propulsion/power/thermal system.

Physics only receives the increased permitted force and temporary envelope when those systems have committed Boost availability.

## 19. No Hidden Drag

Baseline local-space rigid bodies use no atmospheric drag in vacuum except explicit system-defined damping needed for numerical stability below a negligible threshold.

Any gameplay-significant damping must correspond to Flight Assist, atmosphere, docking, tractor/service equipment, or another explicit physical system.

## 20. Gravity Sources

Local flight scenes can provide explicit gravity fields only when the world profile requires them.

TA-5 supports project-provided gravity acceleration fields/vectors but does not implement full N-body orbital simulation.

## 21. Atmospheric Boundary

Full aerodynamic flight is outside baseline.

If a landing/descent sequence uses local gravity/atmospheric assistance, it is an authored simplified control/force profile rather than a full lift/stall simulation.

## 22. External Forces

Ships can receive forces/impulses from:

- collisions;
- explosions where gameplay authorizes impulse;
- docking constraints/machinery;
- authored gravity;
- explicit environmental/anomaly effects.

The source system provides bounded physical intent; TA-5 applies it through the rigid body.

## 23. Power Failure

If powered thrusters lose power:

- new thrust/torque authority drops accordingly;
- current momentum remains;
- rigid body continues physical motion/collision.

Emergency power can restore only the capability actually authorized by ship systems.

## 24. Main Propulsion Failure

Main-drive failure removes/reduces the relevant thrust contribution.

Maneuver thrusters continue if operational.

No physics reset/velocity deletion occurs.

## 25. Maneuvering Failure

If maneuver thrusters are lost:

- unavailable axes lose control authority;
- existing angular/linear momentum persists;
- docking can become physically impossible;
- main drive can still act where operational.

## 26. Pilot Incapacitation

If pilot input becomes unavailable, the avionics gameplay layer can request Safe Drift/Stabilize if valid.

Physics does not independently choose the recovery mode.

## 27. Ship Sleep

A local-flight player ship is normally kept awake while actively piloted/controlled.

Noncritical distant dynamic bodies can sleep as a solver optimization without losing physical state.

Sleep never means despawn/deactivation.

## 28. Collision Shape and Damage Zones

Ship physical hull collision and combat subsystem/weak-point hit zones can coexist as separate shape/query representations.

A hull contact can produce collision facts; combat hit-zone queries route weapon damage to authored subsystems.

## 29. Ship-vs-Character

A moving ship is a massive dynamic body relative to humanoids.

Contact can generate physical collision consequence candidates, but physics callbacks do not directly kill/damage actors.

Collision damage routing is defined separately in TA-5.

## 30. World Bounds

When ship approaches a local-space mission/world boundary, the owning mission/navigation system receives boundary physical state and applies the authored behavior.

Physics does not simply wrap or teleport the ship.

## 31. Origin Rebase

Ship rigid-body position participates in TA-3 floating-origin rebase.

Rebase preserves exactly:

- Context Space physical position;
- linear velocity;
- orientation;
- angular velocity;
- contacts/constraint semantics after rebuild/update.

Rebase occurs only at a Stable Simulation Boundary, never mid-solver-step.

## 32. Save/Load

Save persistence contains project-owned ship physical state required by the GDS:

- local context/position;
- velocity;
- orientation;
- angular velocity;
- relevant ship-system state in owning domains.

Raw backend body/constraint state is recreated.

## 33. Render Handoff

Previous/current committed ship transforms/velocity vectors are published into TA-4 RenderSnapshot.

Rendering/camera smoothing cannot feed back into rigid-body authority.

## 34. Debugging

Development tooling visualizes:

- center of mass;
- collision hull;
- linear/angular velocity;
- body axes;
- requested/applied force;
- requested/applied torque;
- per-axis thruster authority;
- Flight Assist corrections;
- safe-speed envelope;
- sleep/awake state.

## 35. Headless Tests

Required tests include:

- force/mass acceleration relationship;
- Flight Assist bounded braking;
- Assist Off momentum preservation;
- speed-envelope no-instant-clamp behavior;
- thruster-axis failure;
- power-loss momentum preservation;
- mass change after cargo transfer;
- origin rebase invariance;
- save/load physical-state reconstruction.

## 36. Explicit Non-Goals

No full orbital mechanics, aerodynamic fighter simulation, universal fake space drag, instant stop, infinite Flight Assist authority, render-camera-owned ship physics, or per-module rigid body requirement exists in the baseline.

## 37. Dependencies

Depends on Physics Adapter, Collision architecture, TA-1 fixed simulation, TA-2 Ship/Activation state, TA-3 local space/origin, GDS Spacecraft Flight/Propulsion/Systems, Combat Damage, and TA-4 spacecraft camera/readout needs.

## 38. Open Questions

None in spacecraft rigid-body/flight integration architecture.
