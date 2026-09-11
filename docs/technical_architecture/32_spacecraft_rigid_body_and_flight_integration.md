# Spacecraft Rigid Body and Flight Integration

> **Status:** Architecture Complete  
> **Authority:** Local spacecraft rigid-body representation, force/torque application, mass/inertia abstraction, Flight Assist, speed-envelope enforcement, thruster authority, boost/braking, disabled-flight behavior, and physics handoff

## 1. Purpose

This contract realizes the GDS six-degree-of-freedom inertial local-spaceflight model through the StarForge physics adapter.

## 2. Dynamic Rigid Body

An active local-flight spacecraft uses one primary Dynamic rigid body representing its physical hull motion.

The body stores runtime physics state for:

- position;
- orientation;
- linear velocity;
- angular velocity;
- mass properties;
- collision shape;
- active forces/torques.

Persistent `ShipId` and ship-system state remain outside Jolt.

## 3. Ship Physics Profile

Each hull frame exposes a `ShipPhysicsProfile` containing at minimum:

- base collision shape/reference;
- dry-hull mass contribution;
- authored center-of-mass reference;
- authored inertia coefficients/shape abstraction;
- maximum local safe-speed envelope baseline;
- docking collision reference points;
- physical size class.

Installed module/cargo/fuel state supplies runtime mass and capability data.

## 4. Current Mass

Before physics activation and whenever committed ship mass changes, the owning spacecraft system computes `CurrentMass` from:

- hull;
- modules;
- cargo;
- fuel/propellant;
- ammunition;
- occupants;
- physically carried robots/equipment where represented by ship mass rules.

The physics adapter receives the committed result.

## 5. Inertia Abstraction

The baseline does not recompute a high-fidelity rigid-body inertia tensor from every cargo crate/module transform.

Instead, each hull uses authored normalized inertia coefficients scaled by current mass and hull dimensions/configuration modifiers.

This preserves meaningful class/mass differences without turning cargo inventory layout into an unplanned rigid-body simulator.

## 6. Center of Mass

Baseline center of mass follows the authored hull/profile plus explicit module/configuration offsets where a ship definition exposes them.

Ordinary inventory item placement inside abstract cargo storage does not continuously shift center of mass.

## 7. Flight Controller

A StarForge `ShipFlightController` converts player/AI flight intent into a requested six-axis wrench:

- translational force X/Y/Z;
- rotational torque pitch/yaw/roll.

The requested wrench is then clamped by actual propulsion/control authority supplied by ship systems.

## 8. Thruster Authority Input

The physics controller does not decide whether a thruster is powered, damaged, out of propellant, overheated, or otherwise unavailable.

Spacecraft systems provide a per-tick `ThrusterAuthority` describing available:

- forward thrust;
- reverse thrust;
- lateral thrust;
- vertical thrust;
- pitch torque;
- yaw torque;
- roll torque;
- boost capability.

## 9. Force Application

Authorized translational thrust is applied as physical force to the ship rigid body.

Conceptually:

```text
Acceleration = RealizedForce / CurrentMass
```

The controller never directly assigns the desired ship velocity as ordinary flight behavior.

## 10. Torque Application

Authorized rotation input applies bounded torque against the ship's runtime inertia representation.

Angular velocity therefore changes over time rather than snapping to the requested orientation/rate.

## 11. Flight Assist On

With Flight Assist enabled, the controller computes bounded feedback forces/torques to reduce uncommanded relative velocity/angular velocity.

The assist:

- uses only current available thruster authority;
- respects mass/inertia;
- respects power/propellant/damage limits provided by ship systems;
- cannot instant-stop the ship.

## 12. Flight Assist Off

With Flight Assist disabled:

- no automatic translational drift cancellation is requested except speed-envelope safety behavior;
- no automatic angular stabilization is requested except explicitly separate safety/docking behavior;
- existing momentum persists under physical forces/contact.

## 13. Brake/Stabilize Command

Brake/Stabilize is an explicit control mode that requests counter-thrust against current relative velocity/angular drift.

It uses the same finite thruster authority as normal maneuvering.

Stopping distance remains physical.

## 14. Speed Envelope

The GDS `Avionics-Controlled Cruise Speed Limit` is implemented as a control-system envelope, not drag and not a hard per-frame velocity clamp.

When below the allowed envelope, commanded thrust behaves normally.

When at/above the envelope in a direction:

- further player-commanded acceleration that increases excessive speed is suppressed;
- Flight Assist/safety control may request bounded deceleration;
- external collision/explosion/gravity impulses may temporarily exceed the envelope.

The ship is then physically brought back toward the safe envelope as control authority permits.

## 15. No Velocity Clamping

Physics never executes:

```text
if speed > max: velocity = normalize(velocity) * max
```

for ordinary flight.

Such a clamp would destroy collision impulses and violate inertial behavior.

## 16. Boost

When ship systems authorize Boost, `ThrusterAuthority` exposes increased force and an optional temporary speed-envelope increase.

Spacecraft systems remain authoritative for:

- fuel/propellant consumption;
- power;
- thermal consequences;
- damage restrictions.

Physics consumes only the committed authority profile.

## 17. Main Drive and Maneuvering Thrusters

The controller preserves distinct force envelopes for:

- main forward drive;
- reverse/braking authority;
- lateral/vertical maneuvering;
- rotational authority.

A powerful main engine therefore does not imply equally powerful reverse/strafe/rotation.

## 18. Disabled Thrusters

Thruster/module damage updates the next valid ThrusterAuthority snapshot.

A destroyed lateral thruster group can reduce a specific axis without requiring physics to know the module-damage model.

## 19. Power Loss

If powered thrust authority becomes zero:

- active thrust forces stop;
- rigid-body momentum remains;
- gravity/contact/external impulses continue.

Physics never auto-stops an unpowered ship.

## 20. Pilot Incapacitation

When piloting input disappears because the player is Incapacitated, spacecraft automation may supply a Safe Drift/Stabilize intent if avionics/power permit.

That decision belongs to ship/mission gameplay.

Physics simply realizes the provided bounded control intent.

## 21. Local Gravity

A local spaceflight scene can supply an authored gravity field/vector.

Ship physics applies it when the location requires it.

The baseline does not perform N-body orbital simulation.

## 22. Atmospheric Boundary

TA-5 does not implement full aerodynamics.

Simplified authored descent/gravity behavior can still use rigid-body forces/controllers for designated landing sequences.

Lift, stall, drag polar, atmospheric combat-flight simulation are outside baseline.

## 23. Collision Shape

Ship collision uses authored simplified hull collision suitable for navigation/contact/docking.

Rendering mesh triangles are not mandatory collision authority.

Critical docking connector/hit-zone query helpers can be separate compound subshapes/sensors.

## 24. Collision Response

Ordinary ship collisions are resolved by rigid-body physics.

Physics emits physical impact observations; Combat/Ship Damage decides damage/subsystem consequences.

Contact impulse does not directly modify hull Health/Condition.

## 25. CCD

Spacecraft profiles can enable continuous collision/motion quality appropriate to their local speed/size envelope where necessary to prevent tunneling through significant geometry.

TA-13 validates the performance cost and thresholds.

## 26. Small Debris Boundary

Not every decorative particle/debris object becomes collision-active against spacecraft.

Only gameplay-physical debris participates in ship collision/damage queries.

Renderer particles remain nonphysical presentation.

## 27. Flight Zone Boundary

The GDS local-flight boundary is represented through mission/world boundary sensors and navigation state, not an invisible physics wall by default.

Approaching/crossing the allowed limit triggers the owning navigation/mission behavior.

An authored physical barrier remains possible only when the world fiction actually contains one.

## 28. Targeting Boundary

Physics can provide ray/shape/line-of-sight queries for ship weapons/sensors.

It does not decide target knowledge, lock acquisition, faction hostility, or subsystem visibility.

## 29. Docking Handoff

When the ship enters docking approach/final alignment, the Flight Controller continues rigid-body control with optional Docking Assist intents.

The ship remains Dynamic/free until the docking Capture transaction authorizes the physical docking constraint defined in the docking contract.

## 30. Hard Docked State

Once Hard Docked:

- free-flight thrust intent is disabled;
- the docking constraint owns physical attachment;
- ship persistent state remains under Spacecraft authority;
- service links are gameplay/domain state, not Jolt constraint properties.

## 31. Mass Change While Docked

Committed cargo/refuel/module changes can update CurrentMass while docked.

Before release/launch, mass/inertia is refreshed and flight validation re-runs.

No in-flight mass change occurs before its ownership transaction commits.

## 32. Floating Origin

Ship rigid-body state is runtime-origin-relative.

TA-3 origin rebase shifts position only while preserving:

- velocity;
- angular velocity;
- orientation;
- docking/contact semantics;
- Context Space position after conversion.

## 33. Activation / Persistence

On scene activation, persistent ship state reconstructs the Dynamic body from:

- Context Space transform;
- velocity;
- orientation/angular velocity;
- committed mass/configuration;
- collision profile.

On deactivation, runtime physics state is committed back through the TA-2 Activation Lease.

Backend body IDs are discarded.

## 34. Determinism / Fixed Step

Ship control/physics updates occur at the fixed 60 Hz simulation step.

Rendering interpolates the committed ship state but never feeds interpolated transforms back into physics.

## 35. Debugging

Development tools expose:

- mass/inertia;
- center of mass;
- requested vs realized force/torque;
- available ThrusterAuthority per axis;
- Flight Assist corrections;
- speed-envelope state;
- rigid-body velocity/angular velocity;
- contact/CCD diagnostics.

## 36. Explicit Non-Goals

This architecture does not introduce:

- direct velocity-setting arcade flight;
- automatic drag in vacuum;
- full module-by-module rigid-body mass tensor simulation;
- full orbital mechanics;
- full atmospheric aerodynamics;
- camera-dependent ship physics;
- physics-owned damage.

## 37. Dependencies

Depends on Physics Adapter, TA-1 fixed tick, TA-2 Activation Leases, TA-3 Context Space/floating origin, GDS Spacecraft Flight/Systems/Propulsion, Combat/Ship Damage, and TA-4 camera/render snapshots.

## 38. Open Questions

None within spacecraft local-flight physics architecture.
