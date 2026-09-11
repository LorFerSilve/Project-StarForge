# Collision Layers, Filters, and Queries

> **Status:** Architecture Complete  
> **Authority:** Physics collision taxonomy, pair-filter rules, query masks, sensor filtering, hit-zone routing, material metadata, and query contracts

## 1. Purpose

Collision behavior must be explicit, data-driven, and testable. StarForge does not rely on ad-hoc per-call `if` logic to decide whether two physical objects should collide or whether a trace should hit them.

## 2. Layer Model

Every physics shape/body belongs to one primary `CollisionLayer` and can additionally expose query/category metadata.

Baseline physical layers:

- WorldStatic;
- WorldKinematic;
- PlayerCharacter;
- HumanoidCharacter;
- RobotCharacter;
- ShipDynamic;
- DynamicProp;
- GameplayProjectile;
- Debris;
- SensorTrigger;
- InteractionTrigger;
- DockingTrigger;
- HazardTrigger;
- CombatHitZone;
- NoPhysicalResponse.

Exact backend layer indices are implementation details; semantic names are project authority.

## 3. Collision Pair Matrix

The physics adapter owns one canonical pair matrix.

A pair is either:

- solid/contact-enabled;
- overlap/sensor-only;
- ignored.

Gameplay code cannot silently override the matrix for one isolated call without using an explicit query filter contract.

## 4. WorldStatic

`WorldStatic` collides with:

- PlayerCharacter;
- HumanoidCharacter;
- RobotCharacter;
- ShipDynamic where local zone geometry permits ships;
- DynamicProp;
- GameplayProjectile where the projectile profile enables world collision;
- Debris.

It is query-visible to movement, combat, camera obstruction, interaction line-of-sight, and navigation-support queries as appropriate.

## 5. WorldKinematic

`WorldKinematic` behaves as solid moving world geometry for relevant actors.

Examples:

- doors;
- elevators;
- moving platforms;
- docking machinery.

Characters inherit/support motion according to the character-controller contract rather than treating a moving platform as teleporting floor geometry.

## 6. Character Layers

Player/Humanoid/Robot character physical volumes block ordinary solid traversal against relevant world/character geometry.

Combat hit zones are separate query shapes so movement collision and anatomical hit resolution do not require identical geometry.

## 7. Character-vs-Character Policy

Baseline:

- characters have solid body presence for ordinary movement;
- they cannot freely phase through each other;
- character-controller depenetration avoids explosive solver impulses;
- crowd pressure must not launch actors unrealistically.

Exact crowd locomotion behavior is finalized in TA-7/TA-8 without changing collision solidity.

## 8. Ships

`ShipDynamic` collides with:

- stations/terrain/asteroids/large authored world collision;
- other ships;
- relevant large dynamic debris/props;
- docking geometry.

Tiny decorative particles/debris do not become expensive rigid collision merely because they are visible.

## 9. Gameplay Projectiles

Physical projectile collision uses a projectile-specific query/filter profile.

Projectile owner/team is not encoded solely in the physics layer because GDS-9 requires friendly bodies to remain hittable.

A projectile can ignore its own firing collider for a bounded spawn-safe rule while still becoming capable of hitting allies/hostiles according to actual geometry.

## 10. Sensors and Triggers

Sensor layers do not generate ordinary rigid contact response.

Overlap is reported only for filter-eligible objects.

A trigger volume can specify semantic filters such as:

- player only;
- any character;
- ship only;
- robot only;
- physical item/cargo body;
- explicit owner/tag classes.

Overlap alone never commits mission/interact/docking success.

## 11. Combat Hit Zones

Combat actors can expose separate `CombatHitZone` shapes associated with:

- owning RuntimeEntityHandle;
- authored HitZoneId/HitZoneKind;
- armor/component routing metadata.

These shapes participate in combat traces/sweeps but need not participate in locomotion collision response.

## 12. Hit-Zone Geometry

Humanoid baseline zones remain:

- Head;
- Torso;
- Left Arm;
- Right Arm;
- Left Leg;
- Right Leg.

Machines/ships can expose authored subsystem/weak-point shapes.

TA-5 only provides physical query routing; GDS-9/TA-7 own damage meaning.

## 13. Collision Materials

Physical geometry references a stable `CollisionMaterialId` resolving properties such as:

- penetration resistance;
- ricochet eligibility/response data;
- friction/restitution defaults;
- footstep/presentation surface category;
- hazard/contact metadata where applicable.

Material IDs are content data, not raw backend material pointers.

## 14. Physics Material vs Render Material

Render material and collision material are separate references.

A visually painted metal panel can share one physical collision material while using many rendered appearances.

Gameplay penetration/hazard behavior never derives from shader texture names.

## 15. Query API Families

The StarForge physics adapter exposes explicit query families:

- RayCast;
- ShapeCast/Sweep;
- Overlap;
- GroundProbe;
- LineOfSight;
- CameraObstruction;
- InteractionProbe;
- CombatTrace;
- DockingEnvelope/clearance queries.

These can share lower-level implementation but remain semantic APIs so callers do not invent inconsistent masks.

## 16. Query Filter

Every query defines:

- included semantic layers;
- excluded owner/body handles where allowed;
- sensor inclusion policy;
- backface policy where relevant;
- maximum hits/result mode;
- optional material/owner predicates;
- SceneGeneration.

## 17. Query Result

A normalized hit result can include:

- hit fraction/distance;
- runtime/local hit position;
- surface normal;
- PhysicsBodyHandle;
- owner reference;
- CollisionLayer;
- CollisionMaterialId;
- optional CombatHitZone metadata;
- triangle/feature metadata only when a gameplay/debug consumer actually requires it.

## 18. Single vs Multi Hit

Queries explicitly choose:

- Closest;
- Any;
- AllSorted;
- PenetrationChain/continuation where supported by combat resolver.

No caller infers ordering from backend iteration.

## 19. Stable Result Ordering

Multi-hit query results are sorted deterministically by:

1. hit fraction/distance;
2. stable body/owner tie-break key;
3. hit-zone/subshape tie-break where required.

This prevents callback ordering from becoming gameplay authority.

## 20. Hitscan Combat

GDS-9 hitscan uses `CombatTrace`.

Process:

1. camera/weapon system computes final intended ray;
2. muzzle reconciliation determines physical origin/direction;
3. CombatTrace returns physical intersections;
4. penetration/ricochet resolver consumes material/hit-zone metadata;
5. Damage system receives committed physical hit facts.

There is no hidden hit chance in physics.

## 21. Shotgun Multi-Trace

Each deterministic pellet direction issues a normal CombatTrace using the same filter contract.

Physics does not generate pellet dispersion randomness.

## 22. Melee Sweeps

Melee uses swept authored shape(s) through the attack window.

The melee resolver tracks already-hit target IDs per attack instance where the GDS requires one hit per swing.

Physics reports overlap/hit candidates; combat owns deduplication/damage.

## 23. Explosion Queries

Area/explosion damage uses an explicit overlap plus occlusion/line-of-sight strategy owned by Combat.

Physics supplies:

- candidate bodies;
- distances/closest points where needed;
- obstruction queries.

Physics does not itself calculate Damage Packets.

## 24. Interaction Probe

Interaction targeting uses a bounded camera/player-origin probe with actual range and occlusion.

The query may identify candidate interactable owners but TA-7/TA-11 own focus priority and presentation.

Accessibility focus tolerance cannot pass through opaque blockers or exceed allowed interaction reach.

## 25. Camera Obstruction

Spacecraft chase-camera and other camera obstruction use dedicated query masks.

Camera queries never alter gameplay collision state.

A camera being forced closer does not move the player/ship physical body.

## 26. Ground Probe

Character grounding uses a dedicated support query that distinguishes:

- valid walkable support;
- steep/nonwalkable surface;
- moving/kinematic support;
- no support.

Grounding cannot be inferred solely from vertical velocity.

## 27. Sensor Query Knowledge Boundary

Physics can detect geometry that exists in the local world, but presentation/gameplay systems still enforce player knowledge.

A debug physics query result does not automatically make a hidden enemy known to HUD/scanner systems.

## 28. Query and Streaming Boundary

Only collision that is required/resident for the current TA-3 activation set can be authoritative for local physical queries.

If mandatory collision needed for valid traversal is not resident, TA-3 Hard Streaming Hold applies rather than allowing queries through missing geometry.

## 29. Origin Rebase

All query inputs/results use current origin-relative runtime coordinates and carry/validate `OriginEpoch` where stale worker/query results could cross a rebase boundary.

Synchronous main-thread queries performed inside one tick use the current epoch implicitly through PhysicsWorld.

## 30. Sleep and Query Visibility

Sleeping dynamic bodies remain query-visible/collidable.

Sleeping is a solver optimization, not deactivation or disappearance.

## 31. Collision Disablement

Temporarily disabling collision is an explicit physics-state change tied to a valid gameplay/state reason.

Rendering visibility or animation state cannot silently disable collision.

## 32. Spawn Overlap Validation

Spawning/activating a solid actor/body validates that its required collision volume is not illegally embedded in solid geometry.

If invalid:

- scene activation can fail for required actors;
- technical safe-position recovery can apply where GDS permits;
- gameplay cannot simply disable collision to hide the error.

## 33. World Boundary Collision

Mission/local-zone world bounds use explicit physical/nav/mission boundary behavior where required.

Invisible blocking collision is not introduced arbitrarily when the design expects a contextual boundary response.

## 34. Debugging

Development tools can visualize:

- collision layers;
- pair matrix;
- hit zones;
- query ray/sweep/overlap geometry;
- query hits/normals;
- collision materials;
- sensors/triggers;
- ignored pairs.

## 35. Tests

Required component tests include:

- canonical layer pair matrix;
- friendly bodies blocking combat traces;
- camera/muzzle blocker behavior;
- hit-zone routing;
- trigger overlap without solid response;
- deterministic multi-hit sorting;
- interaction occlusion;
- moving-platform support query;
- stale SceneGeneration rejection.

## 36. Explicit Non-Goals

No universal all-layer raycast, friendly-fire phasing, render-material-driven damage behavior, backend callback ordering authority, hidden enemy reveal through debug query, or missing-stream collision bypass exists in the baseline.

## 37. Dependencies

Depends on Physics Adapter, TA-2 runtime identity, TA-3 streaming/origin, GDS-5 Interaction/Movement, GDS-9 Hit Resolution, and future TA-7 gameplay runtime entities.

## 38. Open Questions

None in the collision/filter/query architecture baseline.
