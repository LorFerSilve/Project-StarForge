# Projectiles, Hitscan, Triggers, and Interaction Queries

> **Status:** Architecture Complete  
> **Authority:** Physics-facing implementation of hitscan, projectile motion/collision, melee sweeps, explosion occlusion, triggers, interaction targeting, and query/result ownership

## 1. Purpose

TA-5 provides the physical-query substrate required by GDS Combat and Interaction while keeping Combat/Interaction as gameplay authority.

## 2. Query Phase

Authoritative combat/interaction physics queries execute during the fixed-step query phase against the current active PhysicsWorld after movement/rigid-body integration for that tick.

The query result itself does not commit damage, interaction, mission progress, or ownership.

## 3. Query Result Contract

A StarForge query result can contain:

- owning RuntimeEntityHandle/scoped object key;
- PhysicsBodyHandle;
- hit fraction/distance;
- hit point;
- normal;
- logical collision layer;
- material metadata;
- HitLocation/WeakPoint/subsystem metadata where applicable;
- SceneGeneration;
- OriginEpoch.

Backend subshape/body IDs do not escape as gameplay identity.

## 4. Hitscan Pipeline

Canonical on-foot hitscan pipeline:

1. Combat computes final deterministic shot direction after aim/recoil/dispersion;
2. camera query determines intended aim line/point when needed;
3. weapon muzzle transform is resolved from authoritative runtime pose;
4. `CombatHitscan` trace starts from muzzle/weapon origin;
5. Physics returns ordered intersections;
6. Combat applies penetration/material continuation rules;
7. each accepted target intersection produces a Hit Resolution input;
8. Damage Model resolves consequences.

## 5. Camera-to-Muzzle Boundary

The camera ray is not the damage ray.

If the muzzle path is blocked before reaching the camera-intended target, the blocker is the first physical result.

TA-4 camera clipping cannot alter this.

## 6. Deterministic Multi-Trace

Shotgun/pellet attacks generate deterministic seeded directions in Combat before physics querying.

Physics resolves each trace independently.

Backend iteration order does not alter pellet direction or damage roll because no random damage roll exists.

## 7. Material Penetration Loop

For a penetrating trace, Combat repeatedly requests the next geometric intersection while maintaining an explicit remaining penetration budget.

Physics supplies geometry/material/thickness facts; Combat owns whether the trace continues and how much damage/penetration remains.

## 8. Ricochet

If the weapon/material profile authorizes ricochet, Combat computes the continued direction/energy using returned normal/material data and deterministic rules.

Physics does not randomly ricochet bullets.

## 9. Laser

Baseline laser weapons use immediate hitscan queries.

No persistent physical projectile body is created for the gameplay hit.

Renderer beam effects are presentation only.

## 10. Physical Projectile Runtime Entity

A physical combat projectile is a runtime gameplay entity with explicit state such as:

- owner/instigator;
- attack profile;
- current Context Space/runtime position;
- velocity;
- remaining lifetime;
- guidance state where applicable;
- payload/fuse;
- collision shape/profile;
- SceneGeneration.

Persistent identity is not required for ordinary short-lived projectiles.

## 11. Plasma / Rocket Motion

Plasma bolts, rockets, and selected heavy ordnance use gameplay-owned projectile motion with mandatory swept collision between previous and proposed next transform.

The sweep prevents simple discrete-position tunneling.

The projectile controller commits movement only up to the first accepted impact/time-of-impact.

## 12. Projectile Integration

Per fixed step:

1. apply guidance/acceleration if any;
2. integrate proposed velocity/position;
3. perform `CombatProjectile` sweep from old to proposed transform;
4. if no hit, commit proposed transform;
5. if hit, commit impact transform;
6. resolve direct-hit/detonation policy;
7. destroy/continue projectile according to Combat definition.

## 13. Guided Rockets

Guidance logic belongs to the weapon/projectile gameplay system.

Physics provides:

- current transform/velocity;
- collision sweeps;
- optional line-of-sight query.

Guidance cannot steer through solid geometry or create target lock knowledge.

## 14. Grenades

Thrown/bouncing grenades use Dynamic rigid-body physics when their authored behavior requires physical bounce/rolling.

The grenade gameplay entity owns:

- fuse;
- payload;
- armed state;
- detonation policy.

Jolt owns physical contact response only.

## 15. Grenade CCD

Grenade/fast-ordnance profiles can enable continuous collision to prevent tunneling at intended speeds.

CCD choice is explicit per projectile profile.

## 16. Projectile vs Projectile

Projectile-to-projectile collision is not universally enabled.

Specific interceptable projectiles/missiles can expose collision/query eligibility when required by point defense or authored behavior.

Ordinary bullets/hitscan do not collide with each other.

## 17. Melee Sweep

Melee attacks perform an authored shape/sweep over the active attack window.

Combat owns the per-swing damaged-target set so one target is normally damaged once per swing unless the attack explicitly supports multi-hit.

Physics only returns intersected candidate hit zones.

## 18. Explosion Geometry

When an explosion commits, Combat identifies candidate targets through a bounded spatial/physics overlap query.

For each target, Combat can request `ExplosionOcclusion` samples against solid geometry.

Physics supplies exposure geometry; Damage owns blast falloff and final damage.

## 19. No Explosion Through Arbitrary Walls

Sealed/solid collision blocks or reduces exposure according to Combat's occlusion rule.

A visual particle effect passing through a wall has no damage authority.

## 20. Interaction Focus Query

Player interaction targeting is a two-stage process:

1. physics focus ray/sweep identifies nearby candidate interaction colliders;
2. Interaction validates target capability/state/range/authorization/line of sight.

The query never directly invokes the interaction action.

## 21. Interaction Distance

Physics uses the maximum permitted interaction envelope supplied by the owning Interaction profile.

Accessibility focus tolerance can alter targeting assistance only within the GDS-authorized envelope; it cannot extend physical reach through geometry.

## 22. Line of Sight

Interaction/AI/mission systems request semantic `LineOfSight` queries.

The query uses appropriate world-solid blockers and never uses render visibility as authority.

## 23. Trigger State Table

The physics adapter maintains one logical overlap state per `(TriggerOwner, OtherOwner)` pair.

At each fixed step it derives deterministic logical transitions:

- Enter;
- Stay if subscriber requests it;
- Exit.

Duplicate backend contact points are coalesced.

## 24. Trigger Consumers

Trigger outputs can feed typed candidate events for:

- hazards;
- missions;
- ladders/traversal;
- local gravity volumes;
- docking approach/capture volumes;
- extraction zones;
- world boundaries;
- interaction proximity.

Each owning system validates its own semantics after receiving the physical overlap fact.

## 25. Trigger Destruction

If a trigger or overlapping body is destroyed/unloaded, the adapter generates the logically required Exit/invalidation during the safe teardown boundary where subscribers require it.

It does not leave a permanent phantom overlap.

## 26. Stream/Scene Boundary

Queries never traverse into nonresident required geometry as though it were empty.

TA-3 Hard Streaming Hold protects gameplay when required collision is unavailable.

Query results from obsolete SceneGeneration are invalid.

## 27. Floating Origin

Projectile/runtime query positions use current origin-relative physics space while gameplay Context Space is preserved by TA-3 conversion.

A floating-origin rebase does not change projectile velocity, lifetime, guidance target identity, or hit eligibility.

## 28. Query Budgeting

The architecture permits batching/caching broad candidate collection for performance, but final gameplay-affecting hit/interaction decisions must use the authoritative fixed-step query contract.

TA-13 owns numeric budgets.

## 29. Async Query Boundary

Worker-thread queries can be introduced only for non-immediate planning/data tasks using immutable collision snapshots.

They cannot resolve the current tick's player shot, melee hit, interaction, or collision consequence.

## 30. Debugging

Required debug tools include visualization/logging of:

- hitscan ray and ordered hits;
- muzzle obstruction;
- penetration continuation;
- projectile sweep;
- melee volume;
- explosion samples;
- trigger pair state;
- interaction focus ray;
- filtered/ignored colliders.

## 31. Explicit Non-Goals

This layer does not introduce:

- hidden hit chance;
- camera-origin bullets;
- frame-rate-dependent projectile motion;
- renderer-mesh hit authority;
- interaction-through-wall shortcuts;
- trigger-overlap-as-automatic-mission-success;
- GPU particles as damage projectiles.

## 32. Dependencies

Depends on Physics Adapter, Collision Filters, Character/Ship runtime transforms, TA-1 fixed simulation, TA-3 scene space, GDS Combat Hit/Damage, Player Interaction, Missions/Hazards, and TA-4 presentation boundary.

## 33. Open Questions

None within combat/interaction physics-query architecture.
