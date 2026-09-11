# Scene Composition Profiles

> **Status:** Architecture Complete  
> **Authority:** Technical composition profiles for Horizon/interior, surface, EVA, local spaceflight, and mixed local contexts

## 1. Purpose

GDS Mission Zones define several bounded gameplay categories with materially different spatial and streaming behavior.

TA-3 therefore defines explicit scene-composition profiles instead of forcing every local context through one generic scene recipe.

## 2. Common Scene Contract

Every profile shares:

- one ActiveLocalContextDescriptor;
- one SceneInstance;
- one Context Space;
- one RuntimeOrigin64/OriginEpoch;
- one gameplay spatial-index owner;
- one root Zone/Location identity;
- streamed content residency;
- TA-2 activation leases;
- immutable render/UI read extraction;
- explicit entry/exit points.

Profiles differ only in composition strategy and technical tuning.

## 3. Horizon / Large Interior Profile

Used for:

- Horizon interior/exterior complex;
- large stations;
- colonies;
- authored facility interiors.

Primary partitioning:

- structural/room/corridor adjacency;
- stream cells aligned to meaningful station sections;
- portal/door-aware prefetch;
- persistent station topology as source of truth.

## 4. Horizon Geometry Source

Horizon runtime geometry is assembled from current Station structural state plus authored module definitions.

The SceneInstance never owns station structural connectivity.

Construction/damage changes invalidate affected runtime/static representations through station-domain events/revisions.

## 5. Interior Cell Composition

Interior cells can include:

- room/corridor static mesh chunks;
- collision;
- doors/airlocks;
- environment/pressure volumes;
- local lights/audio zones;
- spawn/interactable descriptors;
- nav tiles/links;
- station-system visual endpoints.

## 6. Interior Visibility Optimization

TA-4 may use room/portal/occlusion structures for rendering.

These structures are technical visibility aids and cannot control gameplay actor existence or security knowledge.

## 7. Surface Profile

Used for bounded planetary/moon surfaces.

Primary partitioning:

- authored/procedural terrain tiles;
- structure/interior pockets;
- vegetation/props/resource placement;
- environment/weather volumes;
- landing/extraction regions.

## 8. Surface Terrain

Surface terrain is bounded to the Mission Zone extent.

TA-10 later defines terrain source/cooked representation.

TA-3 requires:

- deterministic tile identity/layout;
- stable collision geometry for authoritative play;
- explicit zone boundary;
- no infinite procedural terrain continuation.

## 9. Surface Structures

Buildings/facilities embedded in a Surface Zone can be:

- part of the same stream-cell graph; or
- nested interior sub-zone groups under the same SceneInstance.

Entering a building does not require creating a second Mission Instance.

## 10. Surface Distant Presentation

Renderer can retain lower-detail distant terrain/landmarks beyond simulation-active cells when needed for visual continuity.

Distant representation is non-authoritative and cannot expose hidden POIs beyond player knowledge rules.

## 11. EVA Exterior Profile

Used around:

- spacecraft;
- stations;
- asteroids;
- structures;
- wrecks.

Primary composition:

- bounded free-movement volume;
- large static/moving anchor structures;
- explicit airlock/breach/docking transition points;
- vacuum/environment state;
- three-dimensional stream interest.

## 12. EVA Reference Frames

EVA can occur near moving ships/structures.

The scene tracks explicit reference-frame transforms rather than treating every exterior as a global fixed coordinate space.

TA-5 defines detailed character/physics attachment and relative-motion handling.

## 13. Local Spaceflight Profile

Used for bounded spacecraft flight around:

- stations;
- asteroid fields;
- wreck fields;
- orbital facilities;
- combat encounters.

Primary composition:

- large 3D context bounds;
- coarse stream/interest cells;
- floating origin centered around player ship;
- high-speed predictive prefetch;
- persistent ships/major objects;
- authored navigation/exit volumes.

## 14. Local Spaceflight Object Classes

Scene composition can include:

- player ship;
- NPC ships;
- stations/installations;
- large asteroids/wrecks;
- missiles/projectiles;
- environmental hazards;
- docking/approach volumes;
- transition beacons/exit regions.

Object behavior/physics remains owned by later TA domains.

## 15. Spaceflight Background

Stars, distant planets, nebulae, and noninteractive astronomical imagery are presentation layers, not physics objects at literal astronomical coordinates.

TA-4 owns their render representation.

## 16. No Astronomical Precision Burden

The local spaceflight profile does not place distant star systems millions/billions of kilometers from the active ship inside the same coordinate space.

Strategic galaxy identity remains separate.

## 17. Mixed Profile

A Mixed Zone combines two or more scene-composition profiles inside one SceneInstance.

Example:

Local Spaceflight  
→ EVA near disabled ship  
→ breach connector  
→ interior boarding cells  
→ return to EVA  
→ board player ship.

## 18. Profile Region

Each sub-zone group declares a `SceneProfileKind`:

- Interior;
- Surface;
- EVA;
- LocalSpaceflight;
- HorizonInterior/Station.

Runtime systems query the profile associated with the player's/current object's region when profile-specific technical behavior is required.

## 19. Profile Transition Inside One Scene

Crossing from one profile region to another can change:

- gravity/environment provider;
- streaming parameters;
- nav representation;
- camera/presentation mode;
- physics control mode.

It does not automatically change persistent Mission/Context identity.

## 20. Environment Transition

Environment changes are authoritative gameplay transitions owned by environment/door/airlock systems.

Scene profile labels cannot themselves create pressure, gravity, temperature, or vacuum state.

## 21. Player Ship and Interior Coexistence

In a Mixed Zone, the same persistent player ShipId can have:

- a runtime exterior rigid-body representation; and
- accessible authored interior geometry/interaction representation

only under a later TA-5/TA-7 contract that prevents duplicate transform/physics authority.

TA-3 therefore treats ship exterior/interior composition as one identity with multiple technical projections, not two ships.

## 22. Nested Moving Interiors

A moving-ship interior uses an explicit local reference frame under the parent ship transform.

The nested interior remains bounded.

TA-5 decides whether detailed physical simulation uses parent-relative mechanics, inertial approximation, or constrained switching while preserving GDS behavior.

## 23. Station Exterior + Interior

Horizon can include exterior EVA/local-space cells and interior cells within one context.

Airlocks/docking connectors provide explicit transitions between environment/profile regions.

## 24. Composition Validation

Before a Zone can become Ready, validation checks applicable requirements:

- all root cells exist;
- entry point maps to a valid profile region;
- required objective/exit regions are connected by at least one valid traversal path;
- profile transitions have explicit connectors;
- stream-cell references resolve;
- generated geometry lies within context bounds;
- required collision/environment definitions exist.

## 25. Runtime Profile Switching

Changing the player's active region/profile is processed at a deterministic simulation boundary after the crossing/connector condition is validated.

Presentation can interpolate camera/audio changes, but gameplay authority uses the committed region/environment state.

## 26. Profile-Specific Streaming

Profiles select tuneable streaming policy classes, not hardcoded gameplay values.

For example:

- interior: portal/adjacency-heavy prefetch;
- surface: tile/radius prefetch;
- EVA: 3D radius/anchor prefetch;
- spaceflight: velocity-weighted 3D prefetch.

## 27. Profile-Specific Spatial Index

GameplaySpatialIndex remains one scene service but can use profile-specific cell-size/configuration data.

A profile switch can change lookup policy/configuration without changing Persistent IDs.

## 28. Headless Construction

Scene-profile validation and non-render runtime composition must work without OpenGL.

Renderer-specific mesh/material availability is checked through content/resource contracts, not by requiring a live GPU during gameplay logic tests.

## 29. Save/Load

The save stores authoritative location/zone/profile-relevant state, not a rendered scene graph.

Scene profiles are reconstructed from:

- ContentId/profile definitions;
- current Zone/Location identity;
- persistent overrides;
- current player/ship local state.

## 30. Debugging

Development tools expose:

- profile kind by region;
- active player profile;
- nested reference-frame hierarchy;
- stream-policy profile;
- entry/exit connectors;
- environment provider;
- profile transition history.

## 31. Test Requirements

Tests cover:

- Surface to Interior remains same Mission Instance;
- LocalSpaceflight/EVA/Interior Mixed Zone retains one Active Local Context;
- profile switch changes no persistent identity;
- distant presentation cannot become gameplay authority;
- local spaceflight contains no literal strategic galaxy geometry;
- Horizon interior/exterior preserves one Station identity;
- entry/exit connectivity validation catches broken profile composition.

## 32. Explicit Non-Goals

Profiles do not introduce:

- separate engine scene systems per biome;
- automatic environment state from a label alone;
- literal astronomical rendering coordinates;
- duplicate persistent ships for exterior/interior;
- separate Mission Instances merely to cross an airlock.

## 33. Dependencies

Depends on TA-3 scene/streaming/coordinates/transitions, GDS Mission Zone categories, Horizon, Spacecraft, EVA/Player, and later TA-4/TA-5/TA-10.

## 34. Open Questions

None in the TA-3 scene-composition profile contract.
