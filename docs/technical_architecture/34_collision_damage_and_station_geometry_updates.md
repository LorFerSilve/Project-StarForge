# Collision Damage and Station Geometry Updates

> **Status:** Architecture Complete  
> **Authority:** Physical impact observations, collision-damage handoff, contact aggregation, crush/scrape boundaries, destructible/station collider updates, door/breach topology changes, and collision-state synchronization

## 1. Purpose

Physics must report physically meaningful impacts without becoming the owner of Health, hull/module condition, structural state, atmosphere, mission consequences, or repair.

TA-5 also defines how committed world/station geometry changes safely propagate into the active PhysicsWorld.

## 2. Collision Damage Authority Split

Physics owns observations such as:

- bodies involved;
- contact point/normal;
- relative contact velocity;
- normal/tangential impulse estimates;
- effective mass information;
- material/contact classification.

Combat/Health/Ship/Station domains own:

- whether damage occurs;
- Damage Packet construction;
- protection/armor/shield processing;
- subsystem routing;
- destruction/disablement;
- mission/faction consequences.

## 3. Impact Observation

The adapter emits a gameplay-facing `ImpactObservation` containing at minimum:

```text
ImpactObservation {
    BodyOwnerA;
    BodyOwnerB;
    ContactPoint;
    ContactNormal;
    RelativeNormalSpeed;
    RelativeTangentialSpeed;
    SolverImpulseMagnitude;
    EffectiveMassEstimate;
    MaterialA;
    MaterialB;
    SceneGeneration;
    TickIndex;
}
```

Exact C++ representation is implementation data; the semantics are fixed.

## 4. Contact Aggregation

Backend solvers may produce multiple manifold points between the same logical body pair in one tick.

StarForge aggregates them into a deterministic pair-level impact record before gameplay damage evaluation.

Aggregation uses:

- ordered logical body pair key;
- maximum closing normal speed;
- total/representative normal impulse;
- impulse-weighted representative contact point/normal where meaningful;
- stable deterministic ordering.

This prevents one physical collision from dealing duplicate damage because the manifold had several contact points.

## 5. New Impact vs Resting Contact

A resting/support contact is not repeatedly treated as a fresh high-energy impact every fixed tick.

The adapter tracks a runtime **Contact Episode** per relevant body pair with states conceptually:

- New;
- Continuing;
- Separated.

High-energy impact observations are emitted on new/re-impact conditions; sustained force observations are separate when crushing/load mechanics need them.

## 6. Re-Impact

A Continuing contact can become a new impact event again when the pair first separates sufficiently and then closes again, or when a significant new closing impulse/velocity event occurs according to the contact profile.

Exact numeric thresholds are tuneable physics/gameplay data.

## 7. Crush / Sustained Load

Sustained crushing is not modeled by replaying impact damage every tick.

Where gameplay requires crush damage, Physics emits a `SustainedLoadObservation` with bounded cadence containing contact force/load facts.

The owning Damage/Health/Station system decides consequence.

## 8. Scraping

Tangential scraping/friction does not automatically cause Health damage.

Specific gameplay profiles may use sustained tangential speed/contact information for:

- hull scraping;
- machinery damage;
- environmental hazard effects.

This remains explicit content/system behavior.

## 9. Fall Damage

Player landing uses the Character Motor `LandingImpactObservation` rather than generic body-pair hull collision damage.

Player Health/Equipment applies fall thresholds/protection.

## 10. Spacecraft Collision Damage

Ship collision evaluation can use physical facts including:

- relative normal speed;
- ship/current mass;
- impacted zone;
- other body's mass/static classification;
- collision material/profile.

Combat/Ship Damage then constructs deterministic Kinetic/impact Damage Packets or direct structural-impact requests according to GDS rules.

Physics never subtracts Hull Integrity directly.

## 11. EVA / Character Impact

High-relative-speed character/EVA impacts emit physical observations to Player Health/Combat.

No universal damage threshold is hidden inside Jolt callbacks.

## 12. Dynamic Prop Impact

Movable gameplay props can produce impact observations only when their collision profile authorizes gameplay impact consequence.

A lightweight decorative canister does not necessarily damage a Heavy Mech simply because the backend registered contact.

## 13. Damage Impulse Feedback

Combat may return a physically meaningful reaction impulse/knockback request after Damage resolution.

That request is applied through the next legal physics mutation/force phase.

This avoids recursively mutating body velocity inside the original contact callback.

## 14. Explosion Impulse

Explosion Damage and explosion physical impulse are distinct outputs from Combat.

A target can receive:

- Damage only;
- impulse only;
- both;
- neither,

according to exposure/target profile.

Physics applies the committed impulse but does not infer Damage from the visual explosion.

## 15. Structural/Module Damage Commit

When Combat damages/destroys a station/ship/world component:

1. Damage transaction commits owning domain state;
2. owning domain determines required collision/topology change;
3. a typed collision mutation request is produced;
4. TA-5 applies the change at the next allowed PhysicsMutation boundary.

Collision never leads the authoritative state transition.

## 16. Geometry Mutation Categories

TA-5 supports these local collision changes:

- body enable/disable;
- Static/Kinematic collider creation/removal;
- collision-shape replacement;
- door/machinery transform change;
- breach/opening collider replacement;
- destroyed prop removal;
- construction collider activation;
- stream-cell collision chunk activation/eviction.

## 17. Station Construction

GDS Construction Mode globally pauses Simulation Time while permanent geometry is edited.

For a completed/structurally committed construction change:

1. Station Construction commits the module/structural state;
2. TA-6 structural authority will validate final topology;
3. collision payload for affected module/chunk is prepared;
4. the active PhysicsWorld updates at a safe mutation boundary before Simulation resumes.

An unbuilt blueprint does not become a solid normal gameplay collider.

## 18. Structural Complete State

A module may become physically solid at its GDS `Structural Complete` state even if its functional systems are not yet commissioned.

The owning station construction/topology system explicitly emits the collider activation state.

Physics does not infer construction progress from visuals.

## 19. Deconstruction

Deconstruction removal follows the same ordering:

1. authoritative deconstruction stage permits physical removal;
2. station state commits;
3. collision shape/body is removed/replaced at a safe physics boundary.

The collider is not removed merely when deconstruction starts.

## 20. Doors

Door state provides a collision-driving runtime state such as:

- Closed/Blocking;
- Opening;
- Open/Clear;
- Closing;
- Jammed/Blocked.

The owning door/system logic determines when the passage is physically clear.

Physics follows that state through a Kinematic body or validated collider switch.

## 21. Door Safety

A closing door uses actual collision/sweep checks.

It does not phase through the player/robot/ship.

Owning door behavior decides whether to:

- stop;
- reverse;
- apply crush behavior;
- continue under a hazardous/security profile.

Physics only reports obstruction/contact.

## 22. Breaches

When a breach/destruction state commits:

- original blocking collider is replaced/removed;
- authored remaining hull/edge collision stays;
- new traversal/pressure-path metadata is sent to owning Station/World systems;
- atmosphere consequences are not calculated by physics.

The visual hole and collision opening must correspond to the same committed breach state.

## 23. Hull Penetration vs Structural Breach

A bullet material-penetration query passing through a wall does not automatically create a persistent station breach.

Persistent breach requires the owning structural/damage rule to commit a Breached/Destroyed state.

## 24. Destroyed Objects

When an object's gameplay state becomes Destroyed:

- its old collider can be removed/replaced;
- a wreck/debris collider can be created if the owning content/state defines one;
- persistent identity/state remains according to TA-2/GDS.

Physics object destruction is not gameplay object deletion.

## 25. Static Collision Rebuild Scope

The architecture prefers localized collision chunk replacement rather than rebuilding the entire active level for one door/breach/module change.

TA-13 sets exact chunk-size/performance budgets.

## 26. Stream Residency Interaction

If a collider mutation targets a currently nonresident Stream Cell:

- persistent world/station state commits normally;
- no active backend body needs immediate mutation;
- the next cell activation reconstructs collision from the latest authoritative state.

## 27. Scene Transition Interaction

During Scene quiesce/staging, outstanding collision mutation batches are either:

- committed before final quiesce when required;
- folded into the persistent state used to construct the destination/current scene later.

No stale mutation is applied to a newer SceneGeneration.

## 28. Floating Origin

Impact/contact episodes and pending geometry mutations use owner identity plus current epoch-safe data.

Origin rebase does not create collision-damage events.

## 29. Physics Failure During Geometry Update

If a required collider update cannot be created/replaced during an active critical transition:

- the system must not resume Simulation with known missing mandatory collision;
- it enters a controlled technical hold/failure path;
- authoritative gameplay state is not silently rolled back by physics.

TA-14 defines final diagnostics; TA-3 Hard Streaming/technical holds supply compatible behavior.

## 30. Deterministic Ordering

Post-step impact observations are sorted by stable logical keys before gameplay consumption.

Geometry mutations are applied in deterministic category/owner order at the defined mutation boundary.

Backend callback ordering is never gameplay authority.

## 31. Debugging

Required diagnostics include:

- contact episodes;
- impact relative speed/impulse;
- aggregated contact point/normal;
- resulting Damage request ID where created;
- pending collider mutations;
- before/after collision shapes;
- door/breach collision state;
- scene/origin generation.

## 32. Explicit Non-Goals

TA-5 does not implement:

- solver-contact = automatic damage;
- repeated resting-contact damage spam;
- physics-owned structural destruction;
- visual-only breach collision changes;
- whole-level rebuild for every small mutation;
- atmosphere simulation in the physics module.

## 33. Dependencies

Depends on Physics Adapter, Collision Filters, Character/Ship physics, GDS Combat Damage, Player Health, Spacecraft Damage, Station Construction/Damage/Structure, TA-2 transactions, and TA-3 scene/stream state.

## 34. Open Questions

None within collision-damage and geometry-update architecture.
