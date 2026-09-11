# Triggers, Interactions, Projectiles, and Sweeps

> **Status:** Architecture Complete  
> **Authority:** Sensor/trigger execution, interaction-volume physics, projectile physical-state integration, melee sweeps, explosion candidate queries, and gameplay-query handoff

## 1. Purpose

Physics must expose local physical facts for gameplay without silently converting overlaps/hits into authoritative gameplay outcomes.

## 2. Trigger Principle

A trigger overlap means only:

> an eligible physical/query shape overlaps a sensor volume.

It does **not** itself mean:

- interaction succeeded;
- objective completed;
- extraction committed;
- docking completed;
- hazard damage applied.

Owning gameplay systems validate and commit consequences.

## 3. Trigger Types

Baseline semantic trigger families:

- InteractionTrigger;
- MissionTrigger;
- HazardTrigger;
- DockingTrigger;
- ExtractionTrigger;
- TraversalTrigger;
- Detection/ScannerTrigger where explicit overlap geometry is appropriate.

Each trigger references an owning runtime/local key and a typed trigger profile.

## 4. Trigger State

Physics reports overlap transitions:

- Enter;
- Stay where required;
- Exit.

Long-lived `Stay` events are not emitted every tick unless a consumer explicitly needs them; the adapter can maintain overlap state and expose current membership to reduce event spam.

## 5. Trigger Determinism

Enter/Exit transitions are normalized and ordered using stable owner/body keys rather than backend callback order.

SceneGeneration and physics-handle generations are validated before consumption.

## 6. Interaction Physics

An interactable can use:

- a world collider;
- an InteractionTrigger;
- a dedicated interaction anchor/point;
- line-of-sight/shape query metadata.

TA-5 determines physical reach/occlusion facts; TA-7/TA-11 own focus priority, prompts, and interaction commands.

## 7. Interaction Validation Inputs

A physical interaction candidate can expose:

- target owner/key;
- closest valid interaction point;
- distance;
- line-of-sight status;
- relevant blocking surface;
- interaction-side/orientation metadata where authored.

## 8. No Through-Wall Interaction

A valid interaction command requiring direct access must fail physical validation when opaque/solid collision blocks the allowed path.

Accessibility focus tolerance can expand selection tolerance only within the owning design envelope and cannot bypass this rule.

## 9. Physical Projectiles

Projectiles whose GDS class is `Physical Projectile` receive a runtime projectile object with project-owned state:

- ProjectileRuntimeId/RuntimeEntityHandle;
- owner/source reference;
- projectile profile/content ID;
- position;
- velocity;
- lifetime/fuse state;
- payload reference;
- collision shape/radius;
- guidance state where applicable.

## 10. Projectile Integration Modes

TA-5 supports two physical-projectile implementation modes selected by projectile profile:

1. **Query-integrated projectile** — project-owned kinematic trajectory advanced each fixed tick with swept collision;
2. **Dynamic rigid projectile** — Jolt Dynamic body when true rigid-body interactions/bounce are required.

Most plasma/rocket combat projectiles should prefer query-integrated swept motion for controllable gameplay behavior; thrown grenades can use dynamic rigid physics where bounce/rolling matters.

## 11. Swept Motion Requirement

Fast projectiles cannot rely solely on discrete end-position overlap.

Each fixed-step movement uses a sweep/raycast from previous to proposed position sufficient to avoid tunneling through valid collision.

## 12. Projectile Spawn

Projectile spawn validates:

- muzzle/source position;
- initial owner-ignore rule;
- initial geometry overlap;
- SceneGeneration;
- projectile profile.

A projectile cannot spawn on the far side of a muzzle blocker because the camera had visibility.

## 13. Owner Ignore Window

A projectile can temporarily ignore the firing weapon/owner collider only as required to leave the muzzle safely.

The ignore rule is bounded by:

- explicit owner handle;
- short spawn distance/time/profile condition.

It does not create friendly-fire phasing through allies.

## 14. Projectile Hit Fact

On first/eligible physical impact, physics produces a normalized `ProjectileImpactFact` containing:

- projectile runtime reference;
- target owner/body;
- hit position/normal;
- hit-zone metadata where applicable;
- collision material;
- relative velocity;
- tick index.

Combat owns Damage Packet generation and detonation semantics.

## 15. Projectile Termination

Projectile physics can request terminal physical state such as:

- stopped/embedded;
- bounced;
- penetrated/continued;
- detonated-requested;
- lifetime expired;
- destroyed/intercepted.

The projectile profile/Combat system determines which response is valid.

## 16. Penetration Boundary

Physics supplies ordered hit/material geometry.

GDS-9 combat resolver owns penetration budget, damage reduction, and continuation decision.

If continuation is authorized, the next trace/sweep begins beyond the resolved surface with the remaining profile state.

## 17. Ricochet Boundary

Physics supplies incidence geometry/material facts.

Combat/projectile profile decides whether an authored ricochet occurs and computes the new velocity/direction under fixed rules.

The backend does not randomly ricochet every bullet.

## 18. Rockets

Guided rockets use project-owned guidance to produce thrust/steering intent, then TA-5 advances their physical projectile state.

Target lock/knowledge is owned by gameplay targeting systems.

Physics cannot acquire hidden targets.

## 19. Grenades

Thrown grenades can use Dynamic rigid bodies with:

- finite fuse/impact trigger;
- collision material restitution/friction;
- owner/source identity;
- gameplay payload.

Fuse time advances only in Simulation Time.

## 20. Hitscan

Hitscan attacks are not simulated as tiny rigid-body bullets.

They use the TA-5 CombatTrace query pipeline defined in collision/query architecture.

## 21. Melee Sweep Architecture

Melee uses authored sweep shape(s) attached to an attack instance.

Per active attack window:

- previous/current attack-shape transforms are swept;
- hit candidates are normalized/sorted;
- an attack-local set prevents duplicate hits where the attack profile is single-hit-per-target.

## 22. Melee Movement Boundary

A melee attack cannot move the attacker through geometry.

Any authored attack step/lunge must be a real CharacterMotor/actor movement request validated by collision.

## 23. Explosion Candidate Query

Explosion resolution uses:

1. bounded overlap to collect candidates;
2. stable candidate ordering;
3. distance/shape relation calculation;
4. one or more occlusion traces according to Combat profile;
5. Combat-owned damage/exposure calculation.

Physics never directly applies area damage.

## 24. Shield/Barrier Collision

Physical shield/barrier representations can expose query/collision surfaces when gameplay requires interception.

The shield's energy/damage outcome remains Combat/System authority.

A shield VFX without physical/gameplay shield state creates no collision.

## 25. Mission Boundaries

Mission/world trigger volumes can report player/ship boundary crossing attempts.

Owning mission/navigation logic decides warning, redirection, transition, or hard boundary behavior.

The physics layer does not invent arbitrary invisible walls.

## 26. Extraction Volumes

An ExtractionTrigger can confirm that required actor/ship is physically within the authorized area.

Extraction success still requires GDS/mission validation and atomic extraction commit.

## 27. Docking Capture Trigger

DockingTrigger overlap is only one prerequisite.

Final capture also validates:

- relative position;
- orientation;
- relative velocity;
- connector compatibility;
- obstruction;
- authorization/state.

TA-5 docking architecture owns the physical capture transition.

## 28. Hazard Triggers

HazardTrigger can report exposure geometry for:

- radiation volume;
- toxic atmosphere region;
- fire/electrical zone;
- anomaly region;
- other authored hazards.

The Hazard/Survival system computes gameplay exposure/effects.

## 29. Continuous Hazard Contact

For continuous hazards, physics exposes current overlap membership plus entry/exit timing in Simulation Time.

Hazard systems integrate exposure; physics does not accumulate status itself.

## 30. Scanner Boundary

Scanner queries/volumes can use physics for occlusion/range candidate checks.

Detection/identification state is not stored in the PhysicsWorld.

## 31. Trigger Persistence

Transient overlap membership is reconstructed from physical state on scene/load activation.

Persistent mission/objective states are stored in owning domains, never inferred solely from whether a trigger currently overlaps after loading.

## 32. Origin Rebase

Active projectile/trigger geometry rebases with the PhysicsWorld at TA-3 Stable Simulation Boundary.

Projectile Context Space continuity/velocity/lifetime is preserved.

## 33. Streaming Boundary

A projectile cannot travel into required unloaded collision space.

If its authoritative trajectory reaches a streaming boundary lacking mandatory content, TA-3 content requirements must have loaded/held simulation before collision could become undefined.

## 34. Debugging

Development views support:

- trigger volumes/membership;
- interaction probes;
- projectile sweep segment/shape;
- projectile owner-ignore state;
- melee sweep volumes;
- explosion candidate/occlusion queries;
- extraction/docking trigger geometry.

## 35. Headless Tests

Required tests include:

- trigger Enter/Exit ordering;
- no automatic gameplay commit from overlap;
- high-speed projectile tunneling prevention;
- muzzle-blocker spawn behavior;
- owner-ignore boundedness;
- grenade fuse Simulation Time;
- single-hit melee deduplication;
- explosion occlusion;
- extraction/docking overlap not equaling success.

## 36. Explicit Non-Goals

No trigger-owned mission completion, physics-owned damage, camera-origin bullets through walls, all-projectiles-as-rigid-bodies requirement, unlimited owner collision immunity, or VFX-only collision authority exists in the baseline.

## 37. Dependencies

Depends on Physics Adapter, Collision Queries, Character/Ship physical state, GDS Interaction/Missions/Combat, TA-2 commands/transactions, TA-3 streaming/origin, and future TA-7/TA-9 runtime state machines.

## 38. Open Questions

None in the trigger/projectile/sweep architecture baseline.
