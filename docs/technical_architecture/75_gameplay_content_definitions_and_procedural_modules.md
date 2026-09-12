# TA-10 — Gameplay Content Definitions and Procedural Modules

> **Status:** Architecture Complete  
> **Authority:** Authored gameplay-definition schemas, template-vs-instance separation, procedural module metadata, closed data-driven behavior boundaries, cross-content semantic validation, and TA-9 generation compatibility

## 1. Purpose

Project StarForge is highly systemic, but data-driven content must not become an unbounded scripting language or duplicate persistent gameplay state.

TA-10 defines how immutable authored definitions describe weapons, items, actors, ships, station modules, missions, events and procedural building blocks while TA-2/TA-6/TA-7/TA-8/TA-9 continue to own mutable runtime/persistent state.

## 2. Definition vs Instance

A **Content Definition** describes what a kind/template is.

A **Persistent Gameplay Record** describes what one actual object/event/mission is doing in a save.

Examples:

```text
weapon.ballistic.rifle_mk1   -> immutable weapon definition
UniqueItemId(81)             -> one persistent physical item where unique identity exists

robot.chassis.heavy_mk1      -> immutable chassis definition
RobotId(42)                  -> one persistent robot

mission.story.ms_a204        -> immutable story mission template
MissionId(17)                -> one persistent mission record
MissionInstanceId(33)        -> one deployed attempt
```

Content reload/build never allocates persistent gameplay IDs.

## 3. Gameplay Definition Families

Baseline schema families include at minimum:

- Resource/Item definitions;
- Equipment/Tool definitions;
- Weapon/Ammo definitions;
- Armor/Shield definitions;
- Actor/NPC archetypes;
- Robot chassis/modules;
- Ship hull/module definitions;
- Station module/fixture definitions;
- Collision materials;
- Environment/hazard archetypes;
- World/Strategic Location definitions;
- Faction-authored data;
- Mission templates;
- Objective templates;
- Raid templates/target profiles;
- Dynamic Event templates;
- reward definition/profile references;
- Scene/Zone definitions;
- procedural modules;
- presentation references where later TA-11 consumes them.

The exact field sets remain aligned with their owning GDS/TA domains.

## 4. Closed Schema Principle

Content can select/configure **implemented capabilities**.

It cannot invent arbitrary new behavior by embedding executable code.

For example a Weapon definition can select implemented fire/action models and parameters, but cannot include arbitrary C++/Lua/Python expressions that execute during combat.

New behavior requires code/architecture plus an explicit schema extension.

## 5. No Baseline Scripting VM

TA-10 does not add a general-purpose gameplay scripting runtime.

Reasons:

- GDS/TA contracts are already explicit;
- deterministic ownership/transactions must remain inspectable;
- arbitrary scripts would create a second hidden behavior authority;
- current project scope does not require mod scripting.

Specialized data-driven state machines are allowed only where an owning TA defines their closed semantics.

## 6. Definition Header

Every gameplay definition uses the common TA-10 envelope:

```text
SchemaId
SchemaVersion
ContentId
Data
```

References to other definitions use typed ContentIds validated by expected ContentKind/schema.

## 7. Enumerations

Gameplay enums in content use canonical symbolic strings mapped to closed project enums.

Unknown values fail validation.

The runtime does not preserve unknown enum strings and hope another subsystem interprets them later.

## 8. Defaults

Schema defaults are explicit and versioned.

A default can be applied only when:

- the field is documented optional;
- the schema version defines the value;
- applying the default preserves unambiguous semantics.

Critical fields such as damage type, physical resource identity, mission extraction policy or collision material are not silently guessed from neighboring content.

## 9. Tuning Data

Balance/tuning values belong in content when GDS marks them tuneable.

Examples:

- weapon base damage/recoil timings;
- production duration;
- detection range;
- generation weights;
- reward budget parameters;
- event cooldown target values;
- LOD thresholds.

Moving a value to content does not permit it to violate fixed architecture/GDS invariants.

## 10. Formula Boundary

Where the GDS/TA defines a mathematical formula, content can provide its permitted parameters/curves/tables.

Content does not replace that formula with an arbitrary expression language.

If a curve is supported, its interpolation/extrapolation semantics are closed/versioned by the owning schema.

## 11. Curves and Tables

Reusable data curves/tables are explicit content assets or embedded typed structures with:

- sorted domain keys;
- finite values;
- declared interpolation mode;
- defined out-of-range behavior;
- deterministic evaluation.

Duplicate/unsorted keys fail validation where order is required.

## 12. World Definitions

Authored world content maps canonical GDS locations/routes/factions into stable ContentIds.

Persistent world state references the authored identity plus save-owned mutable state.

Procedural content cannot replace the canonical region backbone or move story-critical sites to arbitrary locations.

## 13. Scene / Zone Definitions

A Scene/Zone definition references:

- scene profile;
- bounds;
- geometry/collision/nav/terrain products;
- environment;
- stream-cell composition;
- entry/exit anchors;
- approved spawn/POI sockets;
- traversal/interaction anchors;
- required/optional content dependencies.

Mutable doors/resources/enemies are runtime/persistent instances, not baked state hidden in the Scene definition.

## 14. Mission Template Definition

A MissionTemplate contains authored immutable structure such as:

- mission source/type/class;
- hard requirement templates;
- threat envelope inputs;
- objective graph definition/slots;
- zone/module requirements;
- extraction policy;
- reward profile reference;
- faction/world/story compatibility;
- optional procedural variation slots;
- resolution consequence definitions that are executed only by TA-9 transactions.

The template does not own Mission lifecycle state.

## 15. Story Mission Definition

Canonical story missions use fixed ContentIds, for example:

```text
mission.story.ms_p01
mission.story.ms_a204
mission.story.ms_f02
```

Their authored mandatory geography/characters/evidence/choice structure is fixed by the Design Complete GDS and TA-9.

Procedural slots are explicitly marked and cannot replace mandatory story content.

## 16. Objective Template Definition

An objective template describes the predicate/transition contract that TA-9 ObjectiveGraph materializes into a MissionRecord/Instance.

It includes explicit semantic type such as:

- acquire/secure/deliver physical owner state;
- destroy/disable/capture target state;
- repair operational state;
- rescue state;
- survey/scan evidence state;
- timed state;
- extraction state;
- authored narrative/interaction commit.

The schema does not accept a free-form string expression evaluated against arbitrary game memory.

## 17. Raid Template Definition

Raid content defines immutable orchestration data such as:

- target profile compatibility;
- phase composition;
- approach/access/breach possibilities;
- escalation/reinforcement package references;
- loot/sabotage objective slots;
- withdrawal/extraction rules;
- authored consequence profiles.

TA-9 owns actual RaidId/MissionInstance state and physical target consequences.

## 18. Dynamic Event Template

A DynamicEventTemplate defines:

- event family;
- eligibility constraints;
- source/world plausibility requirements;
- optional mission/defense creation profile;
- offer/schedule timing parameters;
- cooldown family;
- allowed local/economic/world outcome profile;
- communication presentation source metadata.

TA-9 scheduler owns DynamicEventId lifecycle, seed and commit.

## 19. Procedural Module

A ProceduralModule is an authored reusable local geometry/gameplay composition building block.

It has:

- ContentId;
- module class;
- compatible SceneProfileKind;
- local bounds;
- entrance/exit/socket set;
- collision/nav products;
- environment compatibility;
- allowed faction/theme tags;
- POI/objective anchor capabilities;
- extraction/landing/docking capability where applicable;
- actor/cargo traversal compatibility;
- generation weight/constraints;
- required neighboring socket rules.

## 20. Module Sockets

A procedural module socket has typed semantics such as:

- Corridor;
- Doorway;
- Airlock;
- ExteriorPortal;
- DockingConnector;
- TerrainEdge;
- FreeFlightPortal;
- ObjectiveAnchor;
- POIAnchor;
- ExtractionAnchor.

Connection validation requires compatible type, transform/orientation, clearance and environment/security rules.

A simple spatial overlap is not enough.

## 21. Module Geometry Validity

The cooker validates each module independently for:

- closed/valid declared bounds;
- collision/nav coherence;
- required socket reachability;
- no mandatory anchor embedded in solid geometry;
- compatible traversal classes;
- extraction/docking interface geometry where advertised;
- environment boundary correctness where relevant.

## 22. Composition Validation

TA-9 procedural generation can compose modules only when TA-10 metadata proves the static compatibility relation.

The candidate validator additionally checks current world/runtime requirements before mission commit.

TA-10 therefore supplies deterministic static feasibility; TA-9 supplies dynamic/world feasibility.

## 23. Procedural Module Set Version

Every generator-visible module set resolves through the Content Registry and has a deterministic ordered content set/fingerprint.

The generation snapshot records the relevant generator/content schema version and selected module ContentIds/choices.

Reloading a committed MissionId never silently substitutes newly added modules into its already committed major layout.

## 24. Generation Weights

Weights are content tuning values and must be:

- finite;
- nonnegative;
- deterministic under the TA-2 RNG/distribution implementation;
- scoped to explicit candidate pools;
- independent from filesystem order.

Zero weight excludes a candidate from ordinary weighted selection but does not delete its ContentId.

## 25. Content Ordering

Whenever authored list order is semantically meaningful, the schema uses an ordered JSON array.

Whenever order is not meaningful, the cooker normalizes a stable order before hashing/cooking.

Map/object iteration order is never used as random or gameplay order.

## 26. Cross-Definition Validation

The content compiler performs semantic checks such as:

- weapon references compatible ammo/energy model;
- station module ports match supported utility types;
- ship module fits declared slot/class constraints;
- actor archetype references existing equipment/AI doctrine/profile;
- robot chassis references compatible modules/nav class;
- mission objective target/profile exists;
- Dynamic Event mission source references a compatible MissionTemplate;
- procedural module advertises only anchors/interfaces actually present in cooked scene metadata.

## 27. Persistent Unique Content Boundary

A content definition can declare that a generated/mission element is unique, but it does not itself claim the unique persistent identity.

TA-9 final generation/mission transaction owns unique claims and persistent ID allocation.

Rejected procedural candidates never consume persistent identities.

## 28. Reward Definition Boundary

Reward content can describe a profile/package template.

It cannot grant inventory, Credits, Blueprint, Research Evidence or reputation when parsed/loaded.

Actual reward realization is an exactly-once gameplay/economy/resource transaction.

## 29. Resource Conservation Boundary

Content recipes/process definitions can declare input/output quantities allowed by GDS-4, but loading a recipe never creates physical Resources.

Production/crafting/manufacturing domains perform physical ownership transactions.

## 30. AI Content Boundary

Actor/robot definitions can select TA-8 capability/role/doctrine parameters.

They cannot embed omniscient target queries, arbitrary code or navigation bypass.

Perception/traversal rules remain TA-8 authority.

## 31. Difficulty Boundary

Content can expose permitted difficulty tuning values/profiles where GDS-12 allows them.

It cannot change permanent-loss classifications, mission identity, world truth or other fixed GDS invariants based on player difficulty.

## 32. Presentation References

Gameplay definitions can contain references such as:

- icon ContentId;
- model/material ContentId;
- audio cue key/reference;
- localization/display string key.

Those references are presentation dependencies and cannot carry hidden gameplay values not represented in the gameplay schema.

TA-11 owns player-facing presentation behavior.

## 33. Hot Reload Boundary

Gameplay definitions are **not generally live-hot-applied** to an existing normal save/session.

Changing weapon/mission/world/station/ship/progression definitions can alter invariants already materialized into persistent records.

Development tooling may:

- validate/recook them;
- inspect them;
- apply them in dedicated resettable test scenes;
- require session restart/reload for ordinary gameplay.

Presentation-only referenced content can follow the safer hot-reload path defined later in TA-10.

## 34. Save Compatibility

A saved persistent record stores ContentIds needed to interpret it.

Changing/removing a gameplay definition must follow TA-10 compatibility metadata + TA-12 migration policy.

A new content build cannot silently reinterpret an old ContentId as an unrelated item/mission/ship just because the file path is reused.

## 35. Diagnostics

Semantic validation reports:

- ContentId/schema field;
- referenced ContentId;
- expected kind/capability;
- actual kind/capability;
- cross-definition dependency chain;
- GDS/TA invariant violated where identifiable.

## 36. Tests

Required tests cover:

- definition/instance separation;
- unknown behavior/enum rejection;
- closed-schema defaulting;
- cross-reference type mismatch;
- mission template cannot create MissionId on load;
- procedural module socket compatibility;
- advertised anchor matches cooked scene/nav metadata;
- generator-visible module ordering is deterministic;
- added future module does not mutate committed mission package;
- reward definition cannot grant on load;
- recipe definition cannot create resource on load;
- AI content cannot bypass capability/perception schema;
- gameplay-definition hot reload is rejected/deferred in a live normal session;
- ContentId removal triggers compatibility/migration validation.

## 37. Explicit Non-Goals

No baseline requirement for:

- Lua/Python/general gameplay scripts;
- content-defined arbitrary expressions into C++ memory;
- content loading allocating persistent gameplay identities;
- source file order controlling generator outcomes;
- procedurally replacing canonical story geography;
- live mutation of already-materialized save semantics after editing a definition;
- gameplay values hidden only in textures/model node names.

## 38. Dependencies

Depends on Design Complete GDS, TA-2 stores/transactions, TA-6 station systems, TA-7 runtime actors/combat, TA-8 AI/navigation, TA-9 missions/raids/events, and TA-10 registry/cooked scene products.

## 39. Open Questions

None in the gameplay-definition/procedural-module content contract.
