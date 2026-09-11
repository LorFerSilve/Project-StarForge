# Collision Damage and Structural Contact Architecture

> **Status:** Architecture Complete  
> **Authority:** Physics-to-gameplay collision consequence routing, impact severity facts, ship/character/object collision damage candidates, structural-contact handling, break/strain reporting, and damage-authority separation

## 1. Purpose

Physics determines whether bodies contact and with what physical severity. Gameplay systems determine whether that contact causes damage, disablement, breakage, mission consequences, or no gameplay effect.

## 2. Core Rule

A physics contact is never itself Health/Integrity damage.

Canonical flow:

```text
Physics contact
↓
normalized Impact/Contact Fact
↓
Collision Consequence Resolver
↓
owning Damage/Health/Structure system validates
↓
Damage/Break/No-effect transaction
```

## 3. Impact Fact

A normalized impact fact can contain:

- participants/owner references;
- contact position;
- contact normal;
- relative normal/tangential velocity;
- mass/effective-mass context where available;
- solver impulse or project-owned impulse estimate;
- CollisionMaterialIds;
- body classes/layers;
- tick index;
- SceneGeneration.

## 4. Gameplay Severity Metric

The collision consequence layer computes a project-owned `ImpactSeverity` from stable physical inputs.

The exact formula/profile can vary by collision class but must be deterministic and data-driven.

Examples of inputs:

- relative impact speed;
- effective mass;
- contact impulse;
- protected area/material;
- configured safe thresholds.

## 5. No Backend-Damage Coupling

Jolt solver impulse values can contribute to severity but are not directly interpreted as hit points.

A backend tuning change must not automatically redefine game damage without passing through project-owned profiles.

## 6. Collision Consequence Profiles

Each gameplay-relevant physical actor class references a profile defining:

- minimum report threshold;
- safe-impact threshold;
- damage curve/thresholds;
- impulse transfer policy;
- material modifiers;
- maximum per-contact/per-event handling;
- cooldown/debouncing for persistent resting contacts.

## 7. Resting Contact

A ship resting against a dock wall or character standing on a moving platform must not receive repeated collision damage every tick.

Damage requires an impact/strain event that exceeds configured conditions rather than continuous ordinary support contact.

## 8. Character Landing

Character landing impact is produced by CharacterMotor support transition.

Player Health resolves:

- fall-protection equipment;
- gravity context;
- safe-impact threshold;
- resulting damage/injury.

TA-5 only supplies physical facts.

## 9. Character-vs-World Impact

High-speed character collision in Zero-G/EVA can produce an impact candidate using relative contact velocity and character/equipment profile.

Ordinary walking into a wall remains below gameplay damage threshold.

## 10. Character-vs-Ship/Heavy Body

A massive moving body can produce severe impact candidates against a character.

The consequence resolver uses relative motion/mass semantics; physics callback does not directly set the character dead/incapacitated.

## 11. Ship-vs-World Collision

Ship contact with:

- station;
- asteroid;
- terrain;
- large debris;

can generate a ship collision-damage candidate.

Damage routing can include:

- hull/armor zone;
- impacted subsystem region;
- docking equipment;
- cargo/internal consequences where explicitly defined.

GDS-6/GDS-9 own actual ship damage semantics.

## 12. Ship-vs-Ship Collision

Each ship receives its own collision consequence candidate based on shared contact facts and each ship's profile/mass/protection.

Damage need not be symmetric when mass/protection differ.

## 13. Safe Docking Contact

Contacts inside a valid low-speed docking envelope can be classified as docking/soft contact and remain below ordinary collision-damage thresholds.

Excessive approach velocity/orientation can instead generate damage/strain candidates and prevent capture.

## 14. Dock Constraint Strain

Hard Dock constraints can expose accumulated/peak physical strain facts.

Gameplay dock/structure systems determine whether the connector:

- remains intact;
- becomes damaged;
- jams;
- breaks/is destroyed.

## 15. Kinematic Machinery Contact

Doors/elevators/clamps are gameplay-driven kinematic bodies.

Their motion controllers must use collision-aware safety rules where the design expects machinery not to intentionally crush actors.

If a mechanism is explicitly hazardous/damaged and crushing is allowed, physics reports contact/severity; gameplay applies the consequence.

## 16. Crushing Boundary

The baseline does not infer lethal crush damage merely from overlap correction.

A crush condition requires project-owned detection of sustained opposing constraints/contact forces or an authored hazardous mechanism profile.

## 17. Dynamic Prop Impact

Gameplay-relevant props can produce impact candidates when their mass/speed exceed thresholds.

Cosmetic debris can be configured non-damaging even when it has presentation collision.

Visible object size alone does not define damage.

## 18. Projectile Direct Impact

For physical projectiles, direct-hit damage is primarily Combat payload authority.

Physics supplies impact velocity/material/hit-zone facts.

A projectile profile can use physical impact energy as one parameter, but physics does not invent weapon damage independently.

## 19. Explosion Impulse

Combat/explosion systems can request physical impulses after calculating an explosion event.

TA-5 applies those impulses to eligible bodies.

Resulting secondary impacts can create new collision facts if they later physically strike objects.

## 20. Chain Reactions

Secondary collision events are valid only when they physically occur in later solver/contact resolution.

One explosion does not recursively apply arbitrary collision damage to every nearby object without contact or explicit explosion-area rules.

## 21. Structural Contact Boundary

TA-6 will own station structural graph integrity and destruction decisions.

TA-5 supplies physical contact/impact/constraint facts to TA-6.

A rendered/physics mesh breaking apart cannot independently delete a structural graph edge.

## 22. Breakable Physical Attachments

Where a gameplay-owned connection can physically break (for example docking attachment or authored destructible mechanism), the owning domain defines:

- strength thresholds;
- damage state;
- break transaction;
- persistence.

Physics may provide a break/strain observation, but the project domain commits the break and then updates/removes the physical constraint.

## 23. Damage Contact Deduplication

Multiple solver points belonging to one physical impact event are grouped/debounced so a four-point contact manifold does not automatically count as four independent gameplay hits.

The adapter/consequence resolver creates a stable `ImpactEventKey` per relevant body-pair/time/contact episode.

## 24. Repeated Impacts

Distinct separation-and-reimpact events can produce distinct damage candidates.

Continuous contact without a new impact transition does not repeatedly reapply impact damage unless an authored crush/abrasion hazard explicitly requires continuous consequence.

## 25. Relative Velocity Source

Damage uses relative motion between participants/support frames, not raw world velocity.

A ship moving quickly while docked inside an equally moving carrier/station frame can have near-zero relative docking impact.

## 26. Material Interaction

CollisionMaterialId can modify severity/consequence through authored physics/gameplay profiles.

Examples:

- padded landing surface;
- brittle panel;
- armored hull plating;
- hazardous energized machinery.

Render material is not authority.

## 27. Friendly/Hostile Neutrality

Physics impact facts are faction-neutral.

Friendly status does not make bodies intangible or immune to collisions unless a specific gameplay rule says damage should be suppressed after physical contact.

## 28. Difficulty Boundary

Difficulty settings can modify authorized GDS-12 incoming-damage/recovery axes but cannot alter whether a physical collision occurred.

Any collision-damage difficulty modifier is applied by Damage systems after the physical fact, not by making physics ignore contacts.

## 29. Persistence

Physics contact facts are transient.

Committed damage, destroyed components, broken connections, and structural consequences persist through their owning domain states.

Save/load does not serialize contact manifolds merely to preserve already-committed damage.

## 30. Origin Rebase

A floating-origin rebase cannot create an impact event.

The rebase process preserves relative transforms/velocities and suppresses artificial contact-delta interpretation caused solely by coordinate-frame translation.

## 31. Scene Activation

Initial body overlap/contact during scene reconstruction is not automatically treated as a damaging impact.

Spawn validation resolves illegal overlaps before active gameplay. Existing valid resting contacts reconstruct without synthetic collision-damage events.

## 32. Debugging

Development tools expose:

- contact pair;
- relative velocity;
- impulse/effective mass;
- computed ImpactSeverity;
- profile thresholds;
- grouped ImpactEventKey;
- resulting gameplay event/ignored reason.

## 33. Tests

Required tests include:

- resting contact produces no repeated damage;
- landing impact uses relative support velocity;
- ship collision severity scales with relative speed/mass profile;
- safe docking contact stays non-damaging;
- manifold points deduplicate;
- separation/reimpact creates a new event;
- origin rebase creates no synthetic impact;
- load reconstruction creates no synthetic impact;
- physics callback alone cannot mutate Health.

## 34. Explicit Non-Goals

No solver-impulse-to-HP direct conversion, continuous resting-contact damage, render-material damage authority, faction-based physical phasing, physics-owned structural destruction, or rebase-generated impact exists in the baseline.

## 35. Dependencies

Depends on Physics Adapter, Collision Materials, Character Controller, Spacecraft Rigid Body, Docking, GDS Combat/Player Health/Spacecraft Damage, and future TA-6 structural topology/TA-7 runtime damage entities.

## 36. Open Questions

None in collision-damage/structural-contact architecture.
