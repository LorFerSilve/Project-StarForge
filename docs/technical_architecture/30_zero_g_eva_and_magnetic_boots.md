# Zero-G, EVA, and Magnetic Boots Physics

> **Status:** Architecture Complete  
> **Authority:** Zero-gravity player locomotion, suit-thruster integration, stabilization, magnetic boots, local gravity transitions, EVA contact behavior, and zero-g physics boundaries

## 1. Purpose

This contract implements the GDS Zero-G/EVA movement model while preserving inertia, finite thruster authority, suit energy use, and explicit magnetic attachment.

## 2. Zero-G Locomotion State

When the player has no valid grounded/magnetic support and local gravity is below the grounded threshold, Movement enters `Zero-G Free Movement`.

The player remains represented by the humanoid CharacterMotor collision volume.

The controller does not switch to unrestricted rigid-body ragdoll movement.

## 3. Suit Thruster Intent

Controls produce a six-axis EVA thrust intent:

- forward/back;
- strafe left/right;
- strafe up/down;
- rotational/look orientation through player-view rules.

The Movement/Equipment domain validates that compatible EVA thrusters are installed, powered, and not disabled before thrust is authorized.

## 4. Translational Thrust

Authorized EVA movement applies bounded acceleration to current velocity.

Conceptually:

```text
DeltaVelocity = AvailableSuitAcceleration * Input * FixedDt
```

Velocity is not set directly to a target value.

## 5. Energy Boundary

The physics controller receives available thrust authority from the owning suit/equipment system.

It does not consume Suit Energy directly.

The suit/equipment system commits energy consumption and returns the thrust budget that physics may realize for the tick.

If energy is exhausted, acceleration stops and existing momentum remains.

## 6. Stabilization Mode

When GDS Stabilization Mode is enabled, the Movement controller requests counter-thrust against current relative linear velocity.

The control law:

- targets zero relative velocity;
- is clamped by current available EVA thrust;
- consumes the same real energy/thruster capability as manual thrust;
- never snaps velocity to zero.

## 7. Stabilization Priority

Manual input takes priority over stabilization on the commanded axes.

Uncommanded axes may still receive stabilization counter-thrust when enabled.

## 8. Relative Reference Frame

Default zero-G stabilization targets velocity relative to the local inertial Context Space.

When attached to/inside an authored moving spacecraft or platform context, Movement may use the owning support/reference-frame velocity provided by that scene profile so that "zero relative velocity" means zero relative drift to the intended local operational frame.

The reference frame is explicit state, never inferred from the renderer camera.

## 9. Collision in Zero-G

Zero-G character motion uses swept CharacterMotor collision resolution.

On impact with solid geometry:

- penetration is prevented;
- physically appropriate tangential/normal velocity response is computed;
- impact facts can be reported to Health/Combat;
- locomotion does not automatically become Grounded unless a valid support/magnetic state is established.

## 10. Surface Push-Off

The baseline allows deliberate push-off only through the normal EVA movement/thruster/contact rules defined by equipped capability.

There is no universal superhuman wall-jump mechanic.

A future dedicated handhold/push interaction would require explicit gameplay design.

## 11. Local Gravity

Local environments provide an authoritative gravity vector/field from World/Scene state.

TA-5 supports:

- ordinary constant/local gravity;
- reduced gravity;
- zero gravity;
- bounded authored gravity volumes/fields.

The baseline does not implement galaxy-wide N-body gravity.

## 12. Gravity Transition

When crossing between gravity volumes, the new gravity vector becomes active at a fixed-step boundary.

Existing velocity is preserved.

The controller does not rotate/teleport momentum merely because local Up changed.

## 13. Magnetic Boots Capability

Magnetic Boots require an explicit equipped capability and a physics surface with `MagneticAttachable` metadata.

They do not attach to arbitrary material simply because it visually looks metallic.

## 14. Attachment Probe

An attach request uses `MagneticBootProbe` against nearby candidate surfaces.

A valid candidate requires:

- attachable metadata;
- distance inside authored attachment range;
- unobstructed contact path;
- sufficient surface area/normal validity;
- boot system operational and powered as required.

## 15. Magnetic Attachment Commit

When attachment is accepted:

- Movement enters magnetic-supported locomotion;
- support surface/body is recorded through runtime physics linkage;
- local Up becomes the chosen surface normal/tangent frame;
- residual normal separation velocity is resolved through bounded boot adhesion behavior;
- no position teleport beyond collision-safe contact correction occurs.

## 16. Magnetic Ground Frame

While attached:

- ordinary grounded walk/crouch rules apply in the surface tangent plane;
- support motion is inherited like a moving platform;
- jump/detach behavior follows Movement/equipment rules;
- gravity can still exert force, but magnetic adhesion provides authored support authority while within capability.

## 17. Curved / Changing Surface

Magnetic locomotion may transition across adjacent compatible surfaces only when continuous support probes find a valid walkable magnetic path.

The controller updates its support frame gradually/at fixed steps without allowing discontinuous wall-to-wall teleportation.

## 18. Detachment

Magnetic support ends when:

- player explicitly detaches;
- surface becomes invalid/destroyed;
- boot system loses required capability/power;
- separation exceeds support limits;
- authored movement action detaches.

Current world velocity, including inherited support velocity, is preserved appropriately.

The player then transitions to Grounded/Airborne/Zero-G according to current gravity/support.

## 19. Surface Destruction

If the attached collider is destroyed/replaced:

- physics invalidates support linkage at the next safe topology boundary;
- Movement receives support loss;
- player transitions immediately to the appropriate free state;
- no stale backend body handle is retained.

## 20. Moving Spacecraft Exterior

An EVA player attached to a moving spacecraft exterior inherits the support body's local surface motion.

On detach, the player retains the corresponding linear/tangential velocity rather than dropping to global zero.

## 21. EVA and Spacecraft Collision

A free EVA character physically collides with spacecraft hull collision.

At high relative impact speed, physics emits impact observations for Health/Combat evaluation.

A spacecraft cannot pass through an EVA actor because the actor is player-controlled; collision response may use safety limits to avoid unstable solver behavior but does not grant gameplay phasing.

## 22. Tether Boundary

A universal EVA tether is not part of the baseline GDS.

TA-5 therefore does not introduce rope/tether physics.

A future tether tool requires explicit design and architecture amendment.

## 23. Airlocks and Pressure

Airlock pressure/life-support state belongs to Station/Ship simulation.

Physics only represents door solids, traversal volume, and local movement.

Entering vacuum does not itself grant EVA capability.

## 24. Zero-G Objects

Dynamic props/debris in zero gravity retain momentum through rigid-body physics.

The player cannot manipulate them remotely without an explicit interaction/tool capability.

## 25. Rotation Boundary

The baseline first-person player does not become a fully tumbling six-DOF rigid body under ordinary EVA input.

View orientation remains controlled by Player/Camera input while translation preserves inertial physics.

This keeps first-person control readable while retaining meaningful drift.

## 26. Incapacitation

If the player becomes Incapacitated in Zero-G:

- direct thrust/input stops;
- current momentum remains unless an authored rescue/suit stabilization capability acts;
- mission/recovery rules own the outcome.

Physics does not auto-return the player to safety.

## 27. Save / Load

Save export stores persistent/logical position, orientation, velocity, locomotion context, and magnetic-support identity only when representable through persistent/scoped gameplay identity.

Backend PhysicsBodyHandles are reconstructed on load.

If the saved magnetic support no longer exists after migration/content validation, load staging falls back to physically valid free movement at the same legitimate location.

## 28. Floating Origin

Zero-G velocity and relative support state survive TA-3 origin rebases unchanged.

Rebasing shifts runtime coordinates only.

## 29. Debugging

Development diagnostics display:

- gravity vector;
- EVA thrust request/realized acceleration;
- stabilization vector;
- energy-limited thrust authority input;
- magnetic probe;
- magnetic support normal;
- support body velocity;
- relative velocity.

## 30. Explicit Non-Goals

TA-5 zero-G physics does not introduce:

- infinite suit thrust;
- velocity snapping;
- automatic wall attachment;
- universal wall-running;
- N-body orbital gravity;
- rope/tether physics;
- free six-axis uncontrolled player tumbling as baseline locomotion.

## 31. Dependencies

Depends on Character Motor, Collision Queries, GDS Player Movement/Equipment/Field Survival, TA-3 Context Space/floating origin, TA-4 camera, and Spacecraft physics for moving exterior support.

## 32. Open Questions

None within Zero-G/EVA/Magnetic Boots architecture.
