# Docking Contacts and Constraints

> **Status:** Architecture Complete  
> **Authority:** Physics-facing docking envelopes, alignment/relative-velocity validation, capture commit, hard-dock constraint ownership, undocking, emergency separation, and dock-damage interaction

## 1. Purpose

Docking must remain a physical process that uses actual ship control/contact while preserving the GDS docking state machine and transaction semantics.

## 2. Authority Split

Spacecraft/Docking gameplay owns:

- docking request;
- berth assignment;
- authorization;
- compatibility;
- Docking state;
- Capture eligibility;
- Hard Dock commit;
- service-link state;
- launch blockers;
- emergency-undock consequences.

Physics owns:

- relative transform;
- relative velocity;
- collision/clearance;
- physical constraints after authorized capture;
- contact response.

## 3. Docking Interface Runtime Data

Each active docking interface exposes runtime data derived from authored content/persistent state:

- connector transform;
- docking axis/orientation frame;
- size/connector class;
- capture position tolerance;
- capture angular tolerance;
- maximum capture relative speed;
- approach/capture sensor volumes;
- clearance volume;
- operational/damaged state.

## 4. Approach Sensors

Approach/capture volumes are QueryOnly/Sensor bodies.

Overlap only informs the Docking system that the ship is physically in the relevant region.

Overlap alone never commits docking.

## 5. Final Alignment Validation

The Docking system receives physics observations for:

- connector-to-connector positional error;
- angular alignment error;
- linear relative velocity;
- angular relative velocity;
- clearance/obstruction;
- connector operability.

Capture is eligible only when all GDS hard conditions pass.

## 6. Relative Frame

Docking validation uses motion relative to the target docking body/platform, not absolute world speed.

A ship can dock to a moving target if relative position/orientation/velocity are inside the authored envelope and gameplay allows it.

## 7. Docking Assist

Docking Assist produces bounded flight intents supplied to `ShipFlightController`.

It may request:

- relative-velocity damping;
- translation correction;
- angular alignment;
- controlled final approach.

It cannot:

- teleport the ship;
- ignore missing thruster authority;
- phase through obstacles;
- exceed capture tolerances.

## 8. Manual Docking

Manual docking uses exactly the same capture validation.

Only control input source changes.

## 9. Capture Transaction

When capture conditions pass, Docking prepares a short authoritative transaction that validates:

- current Docking state;
- berth still assigned/available;
- connectors still compatible/operational;
- physics capture observations still within tolerance/revision;
- no conflicting constraint/occupancy;
- mission/security authorization still valid.

Only after commit may physics create the hard-dock constraint.

## 10. Constraint Creation Ordering

Canonical ordering:

1. capture validation prepared;
2. Docking state commits `Capture`/constraint authorization;
3. post-commit `PhysicsMutationBatch` creates the docking constraint;
4. constraint creation is verified;
5. Docking finalizes `Hard Docked` at the next allowed stable boundary;
6. service links may then activate according to their own state.

If physical constraint creation fails technically before Hard Dock finalization, the docking attempt enters a controlled technical failure/recovery path rather than pretending the ship is attached.

## 11. Hard-Dock Constraint

The baseline uses a rigid six-degree attachment equivalent to a fixed docking relationship after successful capture.

The constraint preserves the authored relative connector pose between ship and target.

Backend constraint type is adapter-private.

## 12. Docked Moving Target

If the target docking structure is Kinematic/Dynamic and allowed to move, the constrained ship follows it physically through the backend constraint.

Gameplay services remain separate from the physical attachment.

## 13. No Inventory Merge

Creating a hard-dock constraint does not merge inventories or resources.

Cargo, fuel, atmosphere, shore power, passenger, and repair transfers remain explicit domain transactions.

## 14. Dock Collision During Approach

Before capture, normal physical collision applies.

A ship can strike:

- docking arms;
- station hull;
- other ships;
- berth structure.

Physics emits impact observations and Docking/Combat may fail/damage the attempt.

## 15. Docking Clearance

`DockingClearance` queries validate that the required ship/connector sweep volume is unobstructed.

A valid sensor overlap cannot override a blocked clearance path.

## 16. Dock Damage While Attached

If ship or station connector becomes Damaged/Jammed/Disabled/Destroyed while hard docked:

- gameplay Docking state changes according to GDS damage rules;
- physics constraint remains until the owning rule authorizes release/break or structural failure forces a physical separation;
- service links can stop independently.

A power failure alone does not dissolve the physical attachment.

## 17. Constraint Breakage

A docking interface/profile may define a structural break threshold for extraordinary physical loads.

If solver/contact force exceeds the authored threshold:

- Physics emits a `DockConstraintStressObservation`;
- Docking/Combat determines whether connector damage/break should commit;
- after committed break, physics releases the constraint.

Jolt does not independently decide permanent dock destruction.

## 18. Jammed Dock

A Jammed dock can remain physically constrained while normal release is blocked.

Repair/manual release/emergency separation gameplay decides how to clear it.

## 19. Normal Undock

Normal undock follows the GDS sequence:

1. stop/settle service transfers;
2. close/seal transfer paths;
3. verify release eligibility;
4. commit Docking transition to release;
5. queue physics constraint removal;
6. confirm free-flight separation state;
7. apply bounded separation flight intent where required.

## 20. Constraint Removal

Constraint removal happens only at an allowed physics mutation boundary.

The ship keeps the physically correct target/support velocity at the instant of release.

It is not reset to zero velocity.

## 21. Controlled Separation

After normal release, Docking Assist may command low thrust to reach a safe separation distance.

This uses actual maneuvering authority and does not teleport the ship away from the dock.

## 22. Emergency Undock

Emergency Undock can authorize a faster constraint/service release path.

The Docking gameplay system prepares explicit consequences such as:

- connector damage;
- hose/service loss;
- atmosphere loss;
- incomplete transfer.

Physics then performs the authorized constraint release and any specified physical separation impulse/force.

## 23. Forced Docking / Boarding

Raid/boarding mechanics can use the same physical connector/constraint substrate when the design specifies forced docking.

They do not bypass:

- geometry;
- relative-motion constraints appropriate to the device;
- physical attachment;
- subsequent breach/access rules.

Hostile authorization is owned by Raid/Docking gameplay, not physics layers.

## 24. Hangar Berth

A Hangar Berth may transition from free rigid-body flight into a physically parked/secured state after valid landing/capture.

The same rule holds: parking/berthing state commits before physical constraints/kinematic securing are treated as authoritative.

## 25. Multiple Ships

Each ship/berth constraint is independently owned.

Physics does not allow one ship to occupy two incompatible berth constraints unless an explicitly authored multi-point docking system exists.

## 26. Save / Load

Save stores gameplay Docking state and persistent berth/connector identity, not the backend constraint object.

On load staging:

1. both physical owners are reconstructed;
2. compatibility/state is validated;
3. hard-dock constraint is recreated;
4. session activation fails if a required persisted Hard Dock cannot be reconstructed consistently.

## 27. Scene Transition

A docked ship cannot be silently transferred to another Scene unless the owning transition transaction explicitly moves both required attachment state/owners or releases docking first.

## 28. Floating Origin

Docking constraints remain invariant under TA-3 origin rebase because all attached bodies shift by the same delta.

No capture/release event is generated due solely to rebase.

## 29. Debugging

Development tooling visualizes:

- approach/capture volumes;
- connector frames;
- position/angular error;
- relative velocity;
- clearance volume;
- active constraint;
- stress observation;
- release state.

## 30. Explicit Non-Goals

Docking physics does not provide:

- teleport capture;
- inventory merging;
- authorization bypass;
- free repairs/services;
- automatic success from sensor overlap;
- power-dependent disappearance of physical clamps.

## 31. Dependencies

Depends on Spacecraft Flight Physics, Physics Adapter, Collision Queries, TA-2 transactions, TA-3 scene transitions, GDS Spacecraft Docking, Station docking structures, Combat/Raids, and future station simulation.

## 32. Open Questions

None within docking/contact/constraint architecture.
