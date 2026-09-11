# Active Local Context and Scene Model

> **Status:** Architecture Complete  
> **Authority:** Active Local Context identity, scene ownership, local-context composition, runtime scene lifecycle, player-local authority, and off-screen-context boundary

## 1. Purpose

Project StarForge does not simulate one seamless astronomical 3D world.

The runtime therefore needs a precise technical boundary between:

- the strategic galaxy/world state;
- the one local 3D space currently occupied by the player;
- streamed sub-regions inside that local space;
- persistent systems such as Horizon that can continue off-screen.

This document defines that boundary.

## 2. One Player-Local Context

At any ordinary gameplay moment there is at most one **Active Local Context** containing the player's directly simulated 3D environment.

Examples:

- Horizon interior/exterior/local orbital space;
- a planetary Surface Zone;
- an Interior Zone;
- an EVA zone;
- a Local Spaceflight Zone;
- a raid target;
- a recovery location.

A Mixed Mission remains one Active Local Context even when it contains multiple connected sub-zones.

## 3. Strategic World Is Not a Scene

The galaxy graph is strategic data, not one enormous 3D scene.

A Strategic Location is addressed by identity and route topology.

Interstellar travel changes strategic state through GDS/TA mission-navigation contracts; it does not move one physics object through a galaxy-sized Cartesian coordinate system.

## 4. ActiveLocalContext Descriptor

The session stores one authoritative `ActiveLocalContextDescriptor` containing at minimum:

- `ActiveLocalContextKind`;
- owning persistent identity/identities;
- root Zone Instance identity where applicable;
- current Scene generation;
- current coordinate-origin generation;
- transition state;
- active scene-profile/environment identifier.

`ActiveLocalContextKind` describes **ownership/lifecycle**, not physical scene shape.

Baseline values are:

- Horizon;
- Mission;
- PersistentLocation;
- Recovery.

Physical scene shape is separately described by `SceneProfileKind` under `18_scene_composition_profiles.md`, including Interior, Surface, EVA, LocalSpaceflight, and Horizon/Station profiles.

Thus a Mission can contain a LocalSpaceflight scene profile without becoming a different ActiveLocalContextKind.

The descriptor is persistent session state where required by save/load.

## 5. No Redundant Context Identity

The Active Local Context does not invent a second persistent identity for an object already identified by GDS/TA-2.

Examples:

- Horizon is keyed by its Station identity;
- an external mission is keyed by `MissionId` plus its Zone Instance identity;
- a persistent world location is keyed by its World Location identity.

Runtime scene generations are technical epochs, not gameplay identity.

## 6. SceneInstance

A `SceneInstance` is the runtime realization of the Active Local Context.

It owns or coordinates:

- runtime entity registry membership for the context;
- local transform representations;
- streamed cell residency;
- active zone/sub-zone runtime state;
- spatial query index;
- physics-world representation through the physics adapter;
- render-facing scene extraction;
- environment providers;
- local interaction/query registries.

A SceneInstance is not serialized as a raw object graph.

## 7. Scene Generation

Each SceneInstance has a monotonically increasing process-local `SceneGeneration`.

Asynchronous jobs and runtime handles that depend on a particular scene carry this generation.

A result from an older SceneGeneration is rejected rather than applied to a new scene that reused similar addresses/content.

## 8. Scene State Sources

Scene construction consumes:

- immutable authored Content definitions;
- persistent domain records;
- procedural layout/state identified by stable seed/version;
- current Mission/World state;
- TA-2 activation snapshots/leases.

The scene does not become a second long-term source of truth.

## 9. Runtime Scene vs Persistent Domain Authority

Persistent strategic/domain state remains in its owning stores.

High-frequency active facets can be leased to the SceneInstance through TA-2 Activation Leases.

Static or slowly changing facts do not need to be leased merely because they are visually represented.

Example:

- station structural graph stays Station-owned;
- a crew member's active transform/velocity can be leased;
- a rendered wall mesh is a projection of station/world state, not its authority.

## 10. Scene Lifecycle

Canonical SceneInstance lifecycle:

1. `Absent`;
2. `Staging`;
3. `Ready`;
4. `Activating`;
5. `Active`;
6. `Quiescing`;
7. `Deactivating`;
8. `Destroyed`.

A failed staging scene never becomes gameplay-authoritative.

## 11. Staging

`Staging` prepares technical runtime state from immutable/persistent inputs.

Allowed work includes:

- content resolution;
- zone-layout reconstruction;
- CPU asset preparation requests;
- static scene records;
- spatial partitions;
- initial runtime-entity creation plans;
- physics-body creation plans.

No persistent gameplay ownership changes merely because staging started.

## 12. Ready

A scene becomes `Ready` only when every blocking resource/structure required for safe activation is available and validation has passed.

Nonblocking presentation assets can still stream according to TA-10 policies if their absence cannot change gameplay semantics.

## 13. Activation

At a Stable Simulation Boundary:

1. validate destination/context still matches current authoritative state;
2. allocate a new SceneGeneration;
3. grant required Activation Leases;
4. instantiate/enable runtime entities and physics representations;
5. register spatial state;
6. establish player spawn/entry transform;
7. publish the new active read/render snapshot;
8. permit local simulation beginning on the next valid simulation phase.

## 14. Quiescing

A scene enters `Quiescing` before deactivation.

During quiescing:

- new local-only actions that cannot survive transition are rejected/deferred;
- currently atomic transactions finish;
- deferred destruction is drained;
- required persistent results are committed;
- TA-2 activation facets prepare return state.

## 15. Deactivation

Deactivation:

- returns leased state to owning domains;
- unregisters local spatial entries;
- destroys/removes physics proxies;
- releases runtime entities through deferred cleanup;
- releases streamed cells/resources according to cache policy;
- invalidates SceneGeneration-bound jobs/handles.

No persistent gameplay identity is deleted simply because its scene unloaded.

## 16. Zone Composition

One SceneInstance contains one root Zone Instance plus zero or more connected sub-zone instances/cells required by the GDS.

A Mixed Zone can therefore represent:

Local Spaceflight  
→ Dock/Breach  
→ Interior  
→ EVA Exterior

without creating separate missions or separate player-local worlds.

## 17. ZoneInstance Identity

A mission/generated zone whose state must survive save/load uses its GDS Zone Instance identity represented as a TA-2 persistent typed ID.

Authored persistent world locations use their existing persistent location identity and stable authored sub-zone keys rather than manufacturing duplicate global identities for every room.

## 18. Sub-Zone Keys

Sub-zones/cells that do not require independent global identity use a stable local key scoped under their owning Zone Instance or World Location.

The key is derived from authored content or deterministic generation output and is stable for the lifetime of that owning instance.

## 19. Environment Ownership

A SceneInstance exposes a local `EnvironmentSnapshot` assembled from authoritative owners.

It can include:

- gravity vector/profile;
- atmosphere/vacuum state;
- temperature;
- radiation;
- contamination;
- weather;
- local hazard state.

The snapshot is consumable by player/AI/physics/presentation but does not replace the owning gameplay systems.

## 20. Active Local Context Invariant

Exactly one of the following is true during normal session operation:

- one SceneInstance is Active; or
- the session is inside an explicit loading/transition hold with no player-local scene accepting gameplay commands.

Two full player-controllable scenes are never authoritative simultaneously.

## 21. Horizon Off-Screen Is Not a Second Scene

When the player is away from Horizon, Horizon continues through its persistent strategic/system state and scheduled simulation.

It does not keep a hidden second fully active:

- physics world;
- render scene;
- crew locomotion scene;
- runtime entity registry equivalent.

Off-screen defense/tactical calculations use bounded derived working state owned by their later architecture domains.

## 22. Player Presence

The Player domain owns player identity/progression/persistent state.

The active SceneInstance owns leased high-frequency local presentation/locomotion facets while the player is locally active.

The player cannot exist as two active runtime entities in two scenes.

## 23. Persistent Ship Presence

A player ship represented in the scene maps to the same persistent `ShipId` used by SpacecraftStore.

Landing, docking, boarding, scene streaming, or mission transitions never create a duplicate persistent ship.

## 24. Persistent Crew/Robot Presence

Crew/robots active in the scene use the same TA-2 ID-to-runtime mapping rules.

Off-screen records cannot simultaneously mutate leased local locomotion/combat facets.

## 25. Runtime-Only Objects

Purely transient runtime objects can exist without persistent IDs where the GDS does not require persistence across unload/save.

Examples can include:

- ephemeral particles;
- short-lived projectile runtime instances when their save contract stores equivalent combat state elsewhere;
- temporary debug primitives.

TA-7 owns the exact runtime-entity categories.

## 26. Persistent Consequence Rule

Any local action with a persistent GDS consequence must commit that consequence into its owning domain before the relevant runtime representation can be discarded.

Examples:

- opened persistent container;
- harvested deposit;
- destroyed unique system;
- dropped persistent item;
- resolved survivor;
- door/access state;
- ship damage.

## 27. No Scene-Reset Gameplay

Unloading/reloading a scene cannot reset:

- harvested resources;
- unique loot;
- defeated persistent actors where state must persist;
- objective state;
- discovered POIs;
- damage;
- ownership;
- mission seed/layout.

Those states come from persistent Mission/World/domain data.

## 28. Initialization/Shutdown

`App` owns scene-manager construction/shutdown.

Shutdown sequence first quiesces authoritative local state, then destroys scene/runtime resources before destroying the persistent session root and technical backends.

## 29. Thread Affinity

Scene authority/lifecycle transitions execute on the main/simulation thread at Stable Simulation Boundaries.

Workers can prepare immutable:

- cell data;
- asset decode results;
- procedural layout candidates;
- spatial build data.

Worker results cannot activate/deactivate gameplay entities directly.

## 30. Error Handling

A technical scene staging failure:

- produces typed transition/content error;
- does not partially switch persistent location ownership;
- does not destroy the current valid scene if it can remain active;
- preserves enough diagnostic context to reproduce the failure.

If failure occurs while restoring a save before a live scene exists, load activation fails rather than entering a half-valid session.

## 31. Diagnostics

Development tooling exposes:

- ActiveLocalContextDescriptor;
- SceneGeneration;
- root Zone identity;
- resident/active cells;
- leased persistent entities;
- spatial counts;
- pending streaming requests;
- transition state;
- origin epoch.

## 32. Test Requirements

Tests must cover:

- never more than one active player-local scene;
- activation failure leaves persistent state unchanged;
- deactivation returns leased facets;
- stale SceneGeneration worker result rejection;
- same ShipId/RobotId/CrewId survives scene reload;
- Mixed Zone sub-zones remain one Mission Instance;
- unload/reload does not regenerate persistent local outcomes;
- off-screen Horizon does not own duplicate leased state;
- Mission context with LocalSpaceflight profile does not change context ownership kind.

## 33. Performance Constraints

Scene architecture must permit:

- bounded resident data;
- selective activation;
- streaming without cloning entire persistent stores;
- headless scene/transition validation without OpenGL.

## 34. Explicit Non-Goals

The scene model does not create:

- a seamless galaxy scene;
- simultaneous fully simulated player worlds;
- a persistent scene-graph serialization format;
- a general-purpose editor scene model;
- scene-local duplicate gameplay identity;
- scene unloading as gameplay reset.

## 35. Dependencies

Depends on TA-1 runtime, TA-2 identity/state/activation/serialization, GDS World, Missions, Station, Spacecraft, Player, Robots, and Persistence.

## 36. Open Questions

None in the TA-3 Active Local Context/Scene model.
