# Collision Layers, Queries, and Filters

> **Status:** Architecture Complete  
> **Authority:** Logical collision layers, broadphase grouping, pair filtering, query masks, hit-zone metadata, trigger/query behavior, and collision-information boundaries

## 1. Purpose

The collision model must answer two different questions correctly:

1. **Should these bodies physically collide?**
2. **Should this query consider this body?**

These are separate policies.

## 2. Logical Object Layers

The StarForge adapter defines project-owned logical object layers rather than exposing backend layer integers throughout gameplay code.

Baseline layers:

- `StaticWorld`;
- `KinematicWorld`;
- `DynamicProp`;
- `PlayerCharacter`;
- `HumanoidCharacter`;
- `RobotLightMedium`;
- `RobotHeavy`;
- `Spacecraft`;
- `PhysicalProjectile`;
- `Debris`;
- `SensorTrigger`;
- `InteractionQueryOnly`.

More specialized categories may be expressed through body/query tags rather than multiplying broadphase layers unnecessarily.

## 3. Broadphase Groups

Backend broadphase grouping may combine logical layers when that improves performance.

The logical StarForge layer remains the authority exposed to gameplay/query filters.

Initial broadphase families:

- Static;
- Dynamic/Character;
- Spacecraft/Large Dynamic;
- Sensor/Query.

Exact backend mapping is an implementation detail behind the adapter.

## 4. Pair Filtering Principle

Pair filtering exists only for physical/contact response.

It must not encode faction diplomacy or mission hostility.

Friendly actors remain physically solid to one another where their body classes require it.

## 5. Baseline Physical Collision Matrix

`Y` means normal contact generation/response is eligible. `S` means sensor-only overlap. `-` means no physical pair response.

| A \ B | Static | Kinematic | Dynamic Prop | Character | Robot Hvy | Spacecraft | Projectile | Debris | Sensor |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Static | - | - | Y | Y | Y | Y | Y | Y | S |
| Kinematic | - | - | Y | Y | Y | Y | Y | Y | S |
| Dynamic Prop | Y | Y | Y | Y | Y | Y | Y | Y | S |
| Character | Y | Y | Y | controlled | Y | Y | query/impact | Y | S |
| Robot Heavy | Y | Y | Y | Y | Y | Y | query/impact | Y | S |
| Spacecraft | Y | Y | Y | Y | Y | Y | query/impact | Y | S |
| Projectile | Y | Y | query/impact | query/impact | query/impact | query/impact | policy | query/impact | S |
| Debris | Y | Y | Y | Y | Y | Y | query/impact | Y | S |
| Sensor | S | S | S | S | S | S | S | S | - |

`controlled` means character-character push/overlap response is mediated by Character Motor rules rather than unrestricted rigid-body impulse exchange.

## 6. Character Separation

Player, crew, ordinary humanoid enemies, and Light/Medium robots use controlled separation.

The character system prevents stable interpenetration while avoiding solver-driven launches or crowd explosions.

Heavy robots and spacecraft remain physically substantial obstacles.

## 7. Query Masks

Gameplay queries use explicit named masks.

Baseline query masks:

- `WorldSolid`;
- `CharacterMovement`;
- `GroundSupport`;
- `CombatHitscan`;
- `CombatProjectile`;
- `MeleeSweep`;
- `ExplosionOcclusion`;
- `InteractionFocus`;
- `MantleProbe`;
- `MagneticBootProbe`;
- `DockingClearance`;
- `CameraObstruction`;
- `LineOfSight`;
- `SensorDetection`.

A caller requests one of these semantic masks rather than constructing ad-hoc bitmasks in gameplay modules.

## 8. WorldSolid

Includes geometry that physically blocks ordinary world traversal/visibility as appropriate:

- StaticWorld;
- KinematicWorld;
- blocking DynamicProp;
- Heavy Robot/Spacecraft where relevant.

It does not automatically include sensors.

## 9. CharacterMovement Query

Movement sweeps include:

- StaticWorld;
- KinematicWorld;
- blocking props;
- characters/robots according to controlled-separation policy;
- spacecraft where physically present.

Movement ignores purely interaction/query-only helpers.

## 10. GroundSupport Query

Ground/support detection evaluates surfaces that can physically support the character.

It returns metadata including:

- contact point;
- normal;
- body handle;
- surface material;
- support velocity;
- walkable/attachable flags.

## 11. CombatHitscan Query

Hitscan considers:

- solid world geometry;
- combat actor hit colliders;
- robot/ship/station hit-zone colliders;
- physical props that can block/penetrate shots.

Faction does not remove a target from the query.

## 12. Camera vs Combat

`CameraObstruction` and `CombatHitscan` are distinct query contracts.

The camera may use simplified obstruction geometry for presentation, but actual weapon hit authority uses combat/world collision and the muzzle path.

## 13. Hit-Zone Metadata

Combat-capable collision shapes can expose immutable/query metadata such as:

- owning RuntimeEntityHandle;
- HitLocation/WeakPoint key;
- armor/coverage surface reference;
- material penetration profile;
- subsystem key for machine/ship components.

Jolt shape identity itself is not a gameplay HitLocation ID.

## 14. Compound Shapes

A combat actor may use a compound collider containing multiple simplified hit-zone subshapes.

The query result resolves subshape metadata through the StarForge adapter.

Visual triangle geometry is not required for actor hit zones.

## 15. Material Penetration Queries

Combat owns penetration semantics.

Physics supplies ordered intersections and geometric thickness/entry/exit information where supported.

The Combat system applies:

- weapon penetration budget;
- material resistance;
- damage loss;
- continuation/termination.

Physics never decides that an attack should deal reduced Health damage.

## 16. Explosion Occlusion

Explosion exposure uses `ExplosionOcclusion` queries against solid geometry.

The Damage system owns exposure sampling count/layout and damage attenuation.

Sensors and nonblocking helpers do not block blast merely because they have physics representations.

## 17. Interaction Focus

Interaction queries can detect eligible world/actor interaction colliders.

A hit/overlap only returns an **interaction candidate**.

The Interaction system must still validate:

- range;
- line of sight;
- player state;
- authorization;
- target state;
- mission/security rules.

## 18. Trigger Volumes

Trigger overlap uses sensors and reports:

- Enter;
- Stay where requested;
- Exit.

Canonical users include:

- mission volumes;
- hazards;
- ladders/shafts;
- docking approach/capture helpers;
- environmental gravity volumes;
- local-space boundaries.

## 19. Trigger Identity

Trigger observations reference stable runtime/scoped owner metadata, not backend sensor IDs as gameplay identity.

## 20. Trigger Event Coalescing

Multiple backend contact points for the same logical trigger/body pair in one tick are coalesced into one logical overlap state transition.

## 21. Query Determinism

When a query returns multiple equally eligible hits, StarForge applies deterministic ordering using:

1. normalized distance/time-of-impact;
2. stable owning/runtime key;
3. stable subshape key.

Backend iteration order is not gameplay authority.

## 22. Query Snapshot

Synchronous gameplay queries run against the authoritative current PhysicsWorld at the defined fixed-step query phase.

Async worker queries, if added later, operate only on immutable query snapshots and cannot commit gameplay state.

## 23. Continuous Collision

Fast physical objects can request continuous/swept collision according to body/projectile profile.

CCD is used selectively rather than globally.

Typical eligible objects:

- fast grenades/ordnance;
- fast debris where gameplay relevant;
- spacecraft at collision-relevant velocities.

Hitscan does not need CCD because it is already a trace query.

## 24. One-Way / Special Surfaces

The baseline does not introduce generic one-way platform behavior.

Any authored special collision rule requires an explicit profile and cannot be inferred from rendering material alone.

## 25. Destructible Collision

Destruction changes collision only after the owning gameplay state commits the destroyed/breached/open state.

The resulting collider mutation is then queued through `PhysicsMutationBatch`.

Visual disappearance never removes collision before authoritative state permits it.

## 26. Doors

Doors use Kinematic/Static state according to implementation profile.

Collision opening/closing follows the authoritative door state.

A visually opening door cannot become nonblocking before its physical clearance state is valid.

## 27. Stream Cells

TA-3 residency may add/remove collision chunks.

A required solid cell must be resident before simulation permits an actor to enter it.

Hard Streaming Hold prevents missing collision from becoming traversable empty space.

## 28. Floating Origin

Queries use current origin-relative runtime coordinates.

Query results exposed to systems that require persistent location convert back using current `RuntimeOrigin64`/`OriginEpoch` before persistence.

## 29. Debugging

Development tools can display:

- logical layer;
- broadphase group;
- query mask membership;
- sensor status;
- hit-zone metadata;
- material class;
- ignored/accepted pair rationale.

## 30. Explicit Non-Goals

Collision filtering does not encode:

- faction hostility;
- player knowledge;
- damage immunity;
- mission success;
- loot ownership;
- render visibility.

## 31. Dependencies

Depends on Physics Adapter, TA-3 scene/streaming, GDS Hit Resolution, Player Movement/Interaction, Spacecraft Docking, Missions/Hazards, and future runtime-entity architecture.

## 32. Open Questions

None within collision/filter/query architecture.
