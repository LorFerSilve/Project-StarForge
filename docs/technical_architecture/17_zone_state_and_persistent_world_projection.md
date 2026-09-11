# Zone State and Persistent World Projection

> **Status:** Architecture Complete  
> **Authority:** Persistent zone/world state projection, runtime reconstruction, transient-vs-persistent object policy, mission cleanup, revisit behavior, and post-resolution world-state handoff

## 1. Purpose

StarForge repeatedly constructs bounded local scenes from persistent world and mission state.

The architecture must distinguish:

- authored immutable definitions;
- persistent world/location facts;
- Mission Instance state;
- runtime-only scene objects;
- reconstructible technical caches.

Without that distinction, scene unload/reload could accidentally respawn resources, duplicate ships, restore destroyed objects, or erase discoveries.

## 2. Four State Layers

A local zone is reconstructed from four layers:

1. **Content Definition** — immutable authored data;
2. **Persistent World/Location State** — long-lived changes beyond one visit;
3. **Mission/Zone Instance State** — persistent state scoped to the active/owned Mission Instance;
4. **Runtime Projection** — active high-frequency scene representation.

Only layers 2 and 3 are mutable persistence authorities.

## 3. Content Definition

Content Definition can specify:

- default geometry;
- spawn definitions;
- resource deposit definitions;
- doors/ports;
- environment defaults;
- entry/exit points;
- POI definitions;
- authored destruction/repair states;
- stream-cell layout.

Content does not record whether a particular save already harvested/opened/destroyed something.

## 4. Persistent World State

World/Location persistence records long-lived facts such as:

- location discovered;
- faction control;
- installation restored/destroyed;
- unique survivor resolved;
- unique loot removed;
- route unlocked;
- persistent door/system damage where design requires;
- post-story location state.

## 5. Mission Instance State

Mission/Zone Instance state records visit-specific persistent facts such as:

- generated layout seed/version;
- objective states;
- spawned/defeated mission actors;
- opened containers;
- harvested deposits;
- dropped mission items;
- local destruction/repair;
- discoveries;
- temporary mission hazards/events;
- extraction state.

## 6. Runtime Projection

Runtime Projection contains only active technical realization:

- RuntimeEntityHandles;
- origin-relative transforms;
- physics bodies;
- AI working state;
- render/audio proxies;
- active triggers;
- spatial-index entries.

It is rebuilt from authoritative layers and TA-2 leases.

## 7. Projection Precedence

When reconstructing an object/state:

1. persistent World/Location override applies where it owns the fact;
2. active Mission Instance override applies where mission-scoped state owns the fact;
3. otherwise immutable Content default applies.

Runtime state never overrides persistence simply because its mesh/component was created last.

## 8. Stable Local Keys

Authored/procedural local objects that need persistent override state use stable keys scoped to their owner.

Examples:

- DoorKey;
- DepositKey;
- ContainerKey;
- SpawnKey;
- POIKey;
- DestructibleKey.

Keys are stable across unload/reload for the same owning instance.

## 9. Local Key vs PersistentId

A local object receives a global typed PersistentId only when GDS/ownership/history requires independent identity.

A door inside one one-shot mission can use `MissionId + DoorKey` rather than consuming a global persistent DoorId.

A persistent Ship, Robot, Crew member, unique item, Mission, Dynamic Event, etc. continues to use its TA-2 PersistentId.

## 10. Spawn State

A spawn definition and a spawned persistent actor are different concepts.

The owning Mission/World state determines whether a SpawnKey is:

- NotRealized;
- Active/Realized;
- Resolved/Defeated;
- Moved/Transferred;
- Retired.

Cell load alone never resets this state.

## 11. Persistent Actor Activation

If a SpawnKey created a persistent actor with its own PersistentId, future runtime reconstruction resolves that existing PersistentId rather than allocating a replacement.

## 12. Nonpersistent Actor Policy

Purely disposable actors may be represented by mission-scoped spawn state without permanent global identity only if:

- GDS does not require later individual history;
- no persistent inventory/item/relationship refers to them;
- save/load can reproduce equivalent active state.

TA-7 finalizes exact categories.

## 13. Containers

A container is projected from:

- authored capacity/location;
- persistent ownership/inventory state;
- opened/lock/security state.

Reloading a cell cannot restore its original authored contents after those contents moved elsewhere.

## 14. Resource Deposits

A deposit stores remaining yield/resolution under Mission/World state where persistence is required.

Runtime resource nodes are projections.

Harvested quantity cannot regenerate because its stream cell unloaded.

## 15. Dropped Items

A dropped persistent item keeps its unique/stack ownership and persistent local placement state while the mission/location preserves recovery.

Streaming can deactivate its runtime representation without changing owner/location semantics.

## 16. Doors and Access State

Door runtime state is reconstructed from:

- authored door definition;
- persistent damage/open/locked/security overrides;
- current power/security system outputs.

Animation pose is runtime presentation; open/locked/damaged meaning belongs to gameplay/domain state.

## 17. Destructibles

A destructible with persistent consequence stores its committed state/damage result outside the render/physics object.

Recreating intact geometry from Content when state says Destroyed is an invariant violation.

## 18. Environment Overrides

Persistent/mission events can override authored environment defaults such as:

- pressure;
- fire;
- radiation;
- temperature;
- power;
- local hazard activation.

Scene activation assembles the current environment from owning systems rather than resetting to content defaults.

## 19. POI Discovery

Discovery states remain persistent under Exploration/World/Mission authority.

Streaming/renderer visibility cannot directly upgrade Unknown/Detected/Identified/Surveyed/Resolved.

## 20. Player Knowledge Projection

Map/UI presentation receives a read model filtered by discovery/intelligence state.

Resident but undiscovered cells/objects are not automatically disclosed.

## 21. Mission Resolution Cleanup

When a one-shot mission resolves and its runtime scene closes:

- transient runtime state is discarded;
- Field-Unsecured unresolved mission-world loot is removed unless an explicit persistence/recovery rule preserves it;
- Vehicle/Extraction-Secured ownership remains with its real owner;
- unique persistent outcomes commit to World/other owning domains;
- Mission history/result persists as required;
- no later scene rebuild reopens the same active Mission Instance.

## 22. Promotion to World State

Mission-local changes become persistent world facts only through explicit outcome transactions/events.

Examples:

- relay restored;
- faction control changed;
- unique site destroyed;
- route verified;
- survivor recruited/resolved.

The architecture does not copy every local broken crate into global world history unless GDS needs it.

## 23. Repeatable Strategic Locations

A persistent Strategic Location can host multiple Mission Instances over time.

Each new mission combines:

- current persistent World/Location state;
- new stable Mission identity/seed;
- allowed reusable authored content;
- explicit duplicate-protection rules.

A resolved prior Mission Instance is not reactivated as if unfinished.

## 24. Story Locations

Canonical story-location projection preserves required authored landmarks/identity while applying committed story/world-state changes.

Procedural reconstruction cannot delete or replace story-critical keys.

## 25. Player Ship Projection

The player's physical ship is resolved by persistent ShipId and current location.

A Mission scene never instantiates a new persistent ship from an authored `PlayerShipSpawn` definition.

The spawn/entry point only places the existing ShipId runtime representation.

## 26. Robot/Crew Projection

Likewise deployed RobotId/CrewId records are activated according to persistent transport/location state.

Scene content cannot create duplicate copies because an authored placeholder exists.

## 27. Runtime-to-Persistent Sync

Persistent consequences generated during active play commit as they happen according to owning transactions/events, or at explicit deactivation sync for leased high-frequency facets.

The system does not wait until scene unload to record all important gameplay outcomes.

## 28. Crash/Save Safety

Because persistent effects commit during gameplay and saves snapshot owning state at Stable Save Boundary, scene technical caches are never the only copy of critical state.

## 29. Procedural Layout Persistence

Procedural Mission layout identity uses:

- Mission/Zone identity;
- generation algorithm version;
- stable seed;
- deterministic generation inputs;
- persisted overrides.

A save can reconstruct rather than serialize every static generated vertex/object if deterministic equivalence is guaranteed.

## 30. Generated Content Version Compatibility

If a future generation algorithm changes, an existing instance continues to use its persisted `GenerationAlgorithmVersion` and required compatibility path.

If exact reconstruction cannot be guaranteed from old generation inputs alone, the save must persist sufficient resolved layout data before that version ships.

## 31. Cleanup Ownership

Scene cleanup may release:

- RuntimeEntityHandles;
- backend bodies/resources;
- stream-cell data;
- derived spatial/nav/render caches.

It may not erase persistent Mission/World/Inventory/Ship/Robot/Crew state unless a gameplay transaction already committed that change.

## 32. Tombstones/Historical State

Where future references/history require knowledge that an object existed and was destroyed/resolved, the owning domain retains a tombstone/historical record rather than relying on absence from the current scene.

## 33. Diagnostics

Tools can inspect for any local key/object:

- Content default;
- World override;
- Mission override;
- runtime projected state;
- owning authority;
- PersistentId if present;
- stream cell;
- last revision/commit.

This is essential for debugging apparent scene-reset errors.

## 34. Test Requirements

Tests cover:

- opened container stays open/empty after cell reload;
- harvested deposit does not regenerate;
- existing ShipId/RobotId/CrewId is reused;
- destructible state overrides intact content default;
- mission resolution promotes only explicit world outcomes;
- repeat visit creates new Mission Instance without resurrecting old one;
- procedural reconstruction with same seed/version is stable;
- cleanup cannot erase persistent state.

## 35. Explicit Non-Goals

The projection system does not:

- serialize an entire runtime scene graph;
- make every prop globally persistent;
- persist every cosmetic physics disturbance;
- use scene presence/absence as gameplay history;
- allocate global IDs for every static decoration;
- regenerate authoritative mission state from current frame visibility.

## 36. Dependencies

Depends on TA-2 persistent identity/domain state/save contracts, TA-3 scene/streaming/transitions, GDS World/Missions/Exploration/Extraction, and later TA-7/TA-9/TA-10.

## 37. Open Questions

None in the TA-3 zone/world-state projection contract.
