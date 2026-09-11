# Character Controller and Ground Movement

> **Status:** Architecture Complete  
> **Authority:** Humanoid character-controller architecture, grounded locomotion, crouch, jump, slope/step handling, moving platforms, mantle/ladder traversal, technical unstuck, and physics interaction boundaries

## 1. Purpose

The Character Controller translates GDS Player Movement into deterministic fixed-step physical movement without treating the player as an unrestricted rigid body.

## 2. Backend Strategy

StarForge uses a project-owned `CharacterMotor` built over Jolt's virtual/character-collision functionality rather than representing the player as a normal Dynamic rigid body.

The backend provides collision/support queries and safe movement resolution.

The CharacterMotor owns gameplay locomotion semantics.

## 3. Character Representation

The baseline humanoid controller uses a capsule-like collision representation with separate validated profiles for:

- Standing;
- Crouched.

Exact radius/height/skin/step values are tuneable `CharacterControllerProfile` data.

Cosmetic character appearance never changes gameplay collision dimensions.

## 4. CharacterControllerProfile

Profile data contains at minimum:

- standing shape dimensions;
- crouched shape dimensions;
- collision margin/skin;
- walk speed;
- crouch speed;
- sprint speed;
- Heavy Load modifiers;
- ground acceleration/deceleration;
- air-control coefficients;
- jump impulse/target vertical velocity;
- step height;
- maximum walkable slope;
- ground probe distance;
- mantle bounds;
- support detach thresholds.

TA-5 fixes semantics; numeric values remain tuneable content/balance data.

## 5. Fixed-Step Intent

Raw input never directly writes position.

Controls produce a `CharacterMoveIntent` for the next fixed simulation tick containing normalized semantic actions such as:

- desired planar move vector;
- sprint requested;
- crouch requested;
- jump requested;
- mantle request;
- traversal-state actions.

Movement validity is then resolved against gameplay state and physics.

## 6. Locomotion State Authority

GDS Player Movement remains authoritative for primary locomotion states.

The CharacterMotor maintains the runtime representation required to execute those states but does not invent additional hidden movement modes.

## 7. Ground Frame

Grounded movement is expressed in the local gravity/support tangent frame.

For ordinary gravity:

- `Up = -normalize(Gravity)`;
- planar input is projected onto the support plane;
- camera-relative forward/right are reprojected into that plane.

This same abstraction supports reduced gravity and later magnetic-support frames.

## 8. Ground Detection

Ground support requires a valid `GroundSupport` query result that satisfies:

- support surface exists within grounded tolerance;
- contact normal is within walkable-angle limit;
- support is not explicitly non-walkable;
- character is not moving away beyond detach threshold.

A nearby wall does not count as ground.

## 9. Ground State Output

The CharacterMotor exposes runtime support data:

- `Supported` / `Unsupported`;
- support body handle;
- contact point;
- contact normal;
- support linear velocity;
- support angular contribution where relevant;
- surface metadata.

Gameplay Movement maps this into Grounded/Airborne state semantics.

## 10. Grounded Acceleration

Ground movement accelerates toward desired planar velocity rather than teleport-setting position.

Exact acceleration/deceleration is profile data.

Diagonal input is normalized before desired speed is applied.

## 11. Sprint

Sprint changes target grounded speed only when the GDS validity conditions pass.

The CharacterMotor receives the already-authorized sprint state; it does not override:

- Heavy Load;
- Critical Health;
- crouch;
- locomotion incompatibility;
- explicit equipment/status restrictions.

## 12. Crouch

Crouch transitions from Standing collision shape toward the Crouched profile.

Standing up requires an explicit shape-clearance test for the complete standing volume.

If clearance fails:

- crouch state remains;
- no upward teleport occurs.

## 13. Crouch Transition

Presentation can interpolate camera/body height over a short duration.

Authoritative collision shape changes only through validated intermediate/final shapes that do not overlap solid geometry.

The visual camera is not collision authority.

## 14. Jump

Jump is accepted only while validly grounded and not blocked by GDS Movement state.

On accepted jump:

- vertical/support-normal velocity receives the authored jump impulse/velocity;
- support state detaches;
- inherited support velocity is preserved appropriately.

There is no coyote-time/input-buffer requirement in the baseline unless later explicitly added through design amendment.

## 15. Airborne Movement

Airborne control modifies lateral velocity through weaker acceleration than grounded control.

It does not instantly set velocity or cancel existing momentum.

Gravity is integrated every fixed step.

## 16. Step Handling

Small obstacles at or below `StepHeight` can be automatically traversed when a deterministic step test passes:

1. forward movement is obstructed;
2. candidate top surface is within allowed step height;
3. standing/crouched collision shape can occupy the elevated position;
4. top surface is walkable;
5. forward clearance exists.

Failed step validation leaves the obstacle blocking.

## 17. Slope Handling

Walkable slopes support grounded movement.

Surfaces steeper than the maximum walkable angle cannot provide ordinary grounded support.

The CharacterMotor resolves gravity/tangent motion into slide/fall behavior rather than forcing the character to stick to the surface.

## 18. Moving Platforms

Kinematic/Dynamic support bodies expose support velocity/motion to the CharacterMotor.

While supported, the player inherits enough platform transform/velocity to remain spatially stable.

On leaving support, physically appropriate inherited velocity is retained.

The player is not parented to the render transform as the authority.

## 19. Dynamic Body Interaction

Ordinary player movement may push eligible small Dynamic props through explicit CharacterMotor impulse policy.

The policy uses bounded transfer based on:

- player movement intent;
- target mass;
- contact normal;
- configurable push limit.

The player cannot launch arbitrarily massive spacecraft/heavy machinery by walking into it.

## 20. Character-to-Character Contact

Humanoid/light-character overlaps use controlled separation.

The controller:

- prevents stable interpenetration;
- avoids unrestricted rigid-body knockback loops;
- preserves combat hit/damage semantics separately.

Intentional knockback/explosion impulses arrive from Combat/Physics consequence APIs rather than ordinary crowd separation.

## 21. External Impulses

Combat/explosion/collision systems can submit a bounded external velocity/impulse contribution.

The CharacterMotor combines it with locomotion and gravity while preserving the owning Damage/Status semantics.

External impulse does not automatically equal damage.

## 22. Fall Impact Reporting

When unsupported downward motion becomes supported, the motor records the pre-contact relative support-normal velocity and gravity context.

It emits a `LandingImpactObservation` containing physical facts such as:

- relative normal speed;
- support body/material;
- gravity magnitude;
- contact position.

Player Health/Combat evaluates fall protection and damage thresholds.

Physics does not directly subtract Health.

## 23. Mantle Detection

Mantling is not inferred from every wall collision.

A mantle request performs deterministic `MantleProbe` tests for:

- front obstruction;
- reachable ledge top;
- mantle-compatible metadata;
- ledge height/reach bounds;
- destination capsule clearance;
- swept path clearance.

Only a passing probe can enter Mantling state.

## 24. Mantle Execution

Mantling uses a Movement-owned fixed-duration/trajectory traversal, not an animation-only teleport.

Each fixed step:

- computes the next authored trajectory point;
- sweeps the character volume;
- commits only physically valid progression.

If the path becomes blocked before completion, mantle cancels to the nearest physically valid fallback state.

## 25. Ladders / Shafts

Ladder traversal requires an authored ladder/traversal volume and valid entry point.

While in the Ladder/Shaft state:

- motion is constrained to the authored traversal frame/path;
- gravity influence follows the traversal profile;
- ordinary grounded free movement is suspended.

Leaving the valid path transitions to Grounded, Airborne, or Zero-G as appropriate.

## 26. Elevators

The player remains physically supported by an elevator Kinematic body.

Elevators are not scene teleports.

Transition elevators, if ever introduced by explicit design, would use TA-3 scene transition rules rather than pretending a physics platform moved through unloaded space.

## 27. Hazardous Surfaces

Character contact reports material/trigger facts to owning Hazard systems.

The CharacterMotor never invents Damage from a surface texture/material name.

## 28. Health / Load Restrictions

Before motor intent is generated, Movement resolves state restrictions from:

- Health;
- Heavy Load;
- equipment;
- statuses;
- environment.

The motor receives allowed target motion rather than querying unrelated domain stores directly.

## 29. Technical Unstuck

The controller maintains a small history of recently valid safe transforms inside the current legitimate gameplay region.

A technical Unstuck request can search:

1. recent safe transforms newest-to-oldest;
2. nearby valid positions inside the same allowed region.

A candidate must pass:

- collision clearance;
- mission/security boundary validity;
- no illegitimate locked-boundary crossing;
- no exploitative hazard escape.

## 30. Load Recovery

If saved/migrated character placement is invalid after load, staging uses the same safe-position validation before session activation.

This does not change inventory or mission state.

## 31. Runtime/Persistence Boundary

High-frequency character pose/velocity/support state is runtime leased under TA-2.

Save export captures the logical persistent location/orientation/velocity/locomotion-compatible state required by GDS.

Backend support-body IDs are never serialized.

## 32. Camera Boundary

First-person camera follows Character state through TA-4 view logic.

Camera collision/offset never writes Character physics state except through an explicit gameplay command such as crouch.

## 33. Deterministic Update

Given the same:

- fixed-step duration;
- movement intent;
- CharacterControllerProfile;
- world collision state;
- external impulses;
- gravity;

the CharacterMotor must produce deterministic gameplay-facing movement results within supported platform precision tolerances.

## 34. Debugging

Development visualization exposes:

- standing/crouched capsule;
- support point/normal;
- desired vs actual velocity;
- slope classification;
- step probe;
- mantle probes/path;
- safe-position history;
- external impulse contribution.

## 35. Explicit Non-Goals

The CharacterMotor does not implement:

- prone;
- swimming;
- wall-running;
- universal climbing;
- dash/dodge;
- rigid-body ragdoll as player locomotion;
- teleport step climbing;
- collision dimensions derived from cosmetics.

## 36. Dependencies

Depends on Physics Adapter, Collision Queries, TA-1 fixed step, TA-2 Activation Leases, TA-3 local coordinate space, GDS Player Movement/Health/Inventory/Equipment, Combat, and TA-4 camera presentation.

## 37. Open Questions

None within grounded character-controller architecture.
