# Docking Constraints and Zero-G Architecture

> **Status:** Architecture Complete  
> **Authority:** Docking capture geometry/constraints, hard-dock attachment, undock/separation physics, player Zero-G locomotion, EVA thrusters, stabilization, magnetic boots, and gravity-frame transitions

## 1. Purpose

TA-5 must bridge two physical behaviors that rely heavily on relative motion and local frames:

- spacecraft docking;
- player Zero-G/EVA movement.

Both require exact physical state transitions rather than teleportation or animation-only approximation.

# Part I — Docking

## 2. Docking Interface Representation

A runtime docking interface exposes:

- owning Ship/Station/World object reference;
- connector transform;
- connector normal/forward axes;
- size/connector compatibility metadata;
- capture position tolerance;
- orientation tolerance;
- relative-speed tolerance;
- obstruction/clearance volume;
- docking trigger/capture envelope;
- physical operability state.

Authorization/service compatibility remain gameplay-domain data.

## 3. Docking State Boundary

The GDS docking states remain authoritative:

- Free Flight;
- Docking Requested;
- Approach;
- Final Alignment;
- Capture;
- Hard Docked;
- Undocking;
- Launching.

TA-5 provides the physical predicates/transitions needed by these states but does not redefine mission/security authorization.

## 4. Relative Frame

Docking validation uses the relative transform between ship connector and target connector:

- relative position;
- relative orientation;
- relative linear velocity;
- relative angular velocity where required.

World-space speed alone is insufficient when docking target itself moves.

## 5. Approach Guidance

Physics exposes relative kinematic data to the docking controller/UI.

Docking Assist may convert desired alignment into ship thrust/torque intents, but those intents remain bounded by actual spacecraft authority defined in TA-5 flight integration.

## 6. Capture Envelope

Physical capture is eligible only when all required predicates pass simultaneously:

- compatible connector geometry;
- ship connector within position envelope;
- orientation within tolerance;
- relative linear speed below threshold;
- relative angular state within allowed threshold;
- capture path/connector volume unobstructed;
- docking mechanism physically operational.

Gameplay authorization is an additional nonphysics prerequisite.

## 7. Capture Commit

Entering the trigger/envelope does not hard-dock automatically.

Canonical process:

```text
physical envelope valid
+
gameplay docking validation valid
↓
DockCaptureCommand
↓
atomic docking-state commit
↓
physics capture constraint/attachment created
↓
Hard Docked physical state
```

If constraint creation fails, Hard Dock cannot commit.

## 8. Capture Constraint

Hard Dock uses a project-owned docking constraint/attachment built through Jolt constraint facilities or equivalent kinematic attachment strategy.

The constraint:

- fixes relative connector transform within required tolerance;
- prevents ordinary free-flight thrust from separating the ship;
- preserves target-platform movement if the dock target moves;
- remains a physical attachment until released/damaged.

## 9. Hard-Docked Body State

A docked ship does not become a teleported static prop.

Depending on docking target:

- dynamic ship can be constrained to static/kinematic station geometry;
- dynamic-to-dynamic docking can be supported only for authored cases with a stable constraint configuration.

Service/transfer state remains domain authority.

## 10. Thruster Behavior While Docked

Ordinary piloting thrust cannot overcome an intact Hard Dock constraint.

Ship controls can be inhibited by docking state, while emergency separation/damage follows explicit dock rules.

Physics cannot silently allow continuous force to explode the solver/connector without the gameplay system resolving connector limits/damage.

## 11. Dock Damage Boundary

Docking connector/mechanism gameplay condition is owned by ship/station Damage systems.

Physics can report excessive contact/constraint force candidates.

Damage logic decides whether the connector becomes:

- Damaged;
- Jammed;
- Disabled;
- Destroyed.

## 12. Emergency Separation

Emergency Undock is an explicit command/state transition.

It can:

- release/break the docking constraint;
- apply a controlled separation impulse where mechanism supports it;
- emit physical strain/contact facts;
- leave damaged/jammed connector state as committed by owning domain.

It is not an arbitrary teleport offset.

## 13. Normal Undock

Normal undock sequence physically:

1. gameplay/service state closes transfer paths;
2. clamps/attachment become releasable;
3. docking constraint is released at a physics-safe boundary;
4. ship enters controlled separation state;
5. maneuvering thrusters move ship clear.

## 14. Jammed Dock

A jammed/damaged connector can prevent constraint release.

Physics represents attachment until gameplay recovery/manual release/emergency-damage action changes the state.

## 15. Docked Save/Load

Persistence stores logical dock state/berth/interface identities and ship physical state, not raw constraint object IDs.

On load:

- both docking participants are reconstructed;
- compatibility/state is validated;
- required docking constraint is recreated;
- failure to reconstruct a required Hard Dock is a load/staging error, not a silent Free Flight fallback.

## 16. Origin Rebase While Docked

All attached participants and the docking constraint rebase together at the same Stable Simulation Boundary.

Relative transform must remain invariant.

# Part II — Zero-G and EVA

## 17. Zero-G State

When local gravity is zero/negligible and the player is not attached to supported/magnetic ground, character locomotion enters `Zero-G Free Movement`.

Grounded CharacterMotor semantics no longer apply as ordinary walking.

## 18. Zero-G Physical Representation

The player remains represented by a controlled character collision volume rather than becoming an unrestricted Dynamic ragdoll.

A Zero-G movement controller integrates:

- suit-thruster acceleration;
- external velocity;
- collision sweeps/slide response;
- stabilization counter-thrust;
- optional magnetic attachment transitions.

## 19. Zero-G Velocity

Zero-G locomotion preserves linear momentum.

Input commands acceleration/deceleration through available EVA-thruster authority rather than directly setting velocity.

## 20. EVA Thruster Authority

Suit/equipment state provides current EVA authority based on:

- installed/compatible thruster system;
- available suit energy;
- damage/operability;
- equipment restrictions.

TA-5 consumes authorized acceleration limits; it does not create free thrust/energy.

## 21. Zero-G Translation

Baseline player control can request thrust along camera/body-relative axes:

- forward/back;
- left/right;
- up/down.

Applied acceleration is bounded by current equipment authority.

## 22. Zero-G Rotation

First-person orientation remains primarily camera/input controlled for readability, while the collision capsule/body frame follows the movement model sufficiently to keep physical collision coherent.

The baseline does not require full free-tumbling humanoid rigid-body orientation for ordinary EVA control.

## 23. Stabilization Mode

When enabled, Stabilization Mode computes counter-thrust toward zero relative linear velocity in the current local frame.

It:

- uses real available thrust;
- consumes suit energy through owning equipment systems;
- cannot instant-stop;
- cannot cancel external forces beyond available authority.

## 24. Energy Failure

If suit energy/thrusters become unavailable:

- active Zero-G thrust stops;
- Stabilization stops;
- current momentum remains;
- collisions/environment continue normally.

## 25. Zero-G Collision Response

Character sweep/contact resolution prevents passing through solid geometry.

On collision, the controlled player can:

- slide along surfaces;
- lose velocity normal to contact according to controller profile;
- retain tangential motion;
- receive collision-impact facts when severity warrants.

The controller does not become a pinball-rigid-body by default.

## 26. Push-Off

An authored/support-valid push-off action can add a bounded velocity impulse away from contacted surface if GDS/control content exposes such an action.

TA-5 does not require a universal push-off control beyond the Design Complete locomotion set.

## 27. Magnetic Boots

Magnetic Boots require:

- equipment capability enabled;
- compatible physical surface metadata;
- surface within attachment range/alignment envelope;
- no blocker preventing attachment.

On successful attach, the controller establishes a local support frame with the surface.

## 28. Magnetic Support Frame

With Magnetic Boots attached:

- local `Up` derives from supported surface normal;
- grounded locomotion operates in the tangent frame;
- support motion can be inherited;
- player remains constrained within magnetic attachment rules.

The surface can be station/ship geometry that moves.

## 29. Magnetic Detach

Detaching:

- removes magnetic support;
- preserves appropriate support/actor velocity;
- transitions to local gravity Grounded/Airborne or Zero-G Free Movement according to environment.

If supporting surface is destroyed/invalidated, detach is immediate.

## 30. Gravity Transition

Transition between gravity contexts occurs at a fixed simulation boundary using environment-provided gravity state.

Examples:

- artificial-gravity zone entered/exited;
- local planetary gravity volume;
- Zero-G compartment;
- magnetic attachment.

Velocity is preserved; the controller changes acceleration/support rules rather than teleporting/reorienting position.

## 31. Reduced Gravity

Reduced gravity uses the normal CharacterMotor with different gravity magnitude and resulting grounded/jump/fall dynamics.

It does not require a separate controller family unless no support exists, in which case ordinary Airborne/Zero-G rules apply according to gravity threshold/content profile.

## 32. EVA Hazards

Pressure, oxygen, thermal, radiation, and other survival effects are outside physics authority.

Physics reports location/contact/volume facts; Field Survival applies consequences.

## 33. EVA Save/Load

Persistent physical state includes the logical Zero-G/EVA locomotion-compatible values needed to reconstruct:

- position;
- velocity;
- orientation/view-relevant state;
- Magnetic Boot attachment identity/state where persistent and valid.

Raw physics handles are reconstructed.

## 34. Render/Camera Boundary

TA-4 camera presentation can smooth/chase/adjust viewpoint but cannot modify physical docking or Zero-G collision state.

Camera obstruction queries use TA-5 query architecture.

## 35. Debugging

Development tooling visualizes:

- docking connector frames;
- capture envelope;
- relative velocity/orientation error;
- active docking constraint;
- constraint force/strain;
- Zero-G velocity;
- requested/applied EVA acceleration;
- stabilization vector;
- magnetic support frame;
- gravity vector.

## 36. Headless Tests

Required tests include:

- capture rejected above relative-speed threshold;
- capture rejected on blocked connector;
- Hard Dock preserves relative transform;
- undock releases constraint before separation;
- docked origin rebase invariance;
- Zero-G momentum preservation;
- bounded stabilization deceleration;
- thruster-energy loss preserves drift;
- magnetic attachment/detachment velocity inheritance;
- gravity transition continuity.

## 37. Explicit Non-Goals

No teleport docking, constraint-free Hard Dock, infinite docking-assist authority, full humanoid rigid-body tumbling, free EVA thrust, instant Zero-G stop, or magnetic attachment to arbitrary surfaces exists in the baseline.

## 38. Dependencies

Depends on Physics Adapter, Character/Ship integration, Collision Queries, TA-1 fixed time, TA-2 state/transactions, TA-3 origin/context, GDS Spacecraft Docking/Flight, Player Movement/Equipment/Field Survival, and TA-4 camera presentation.

## 39. Open Questions

None in docking/Zero-G architecture baseline.
