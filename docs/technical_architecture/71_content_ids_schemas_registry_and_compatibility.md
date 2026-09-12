# TA-10 — Content IDs, Schemas, Registry, and Compatibility

> **Status:** Architecture Complete  
> **Authority:** ContentId grammar, content kinds, schema envelope/versioning, reference classes, generated registry, runtime lookup, compatibility metadata, and source/cooked identity invariants

## 1. Purpose

TA-2 established `ContentId` as the stable authored-definition identity used by saves and runtime systems.

TA-10 now fixes how ContentIds are named, declared, validated, registered, resolved, versioned, and kept compatible across content builds.

## 2. ContentId Is Logical Identity

`ContentId` is not:

- a file path;
- an asset filename;
- a runtime pointer;
- a registry array index;
- a persistent gameplay object ID;
- a hash alone.

It is a stable logical authored identity.

## 3. Canonical ContentId Grammar

Version-1 ContentIds use lowercase ASCII logical names with dot-separated namespaces.

Canonical grammar:

```text
segment      := [a-z0-9][a-z0-9_-]*
content_id   := segment ('.' segment)+
```

Examples:

```text
weapon.ballistic.rifle_mk1
station.module.life_support_basic
mission.story.ms_a204
world.location.nacre_echo_lab
texture.material.horizon_hull_basecolor
shader.family.pbr_deferred
```

Rules:

- at least two segments;
- lowercase only;
- no whitespace;
- no slash/backslash;
- no leading/trailing dot;
- no empty segment;
- maximum byte length is validated by tooling and chosen conservatively for diagnostics/serialization;
- comparison is byte-exact/case-sensitive, but uppercase is invalid canonical source.

This refines TA-2's case-sensitive UTF-8 logical-string representation without changing save representation.

## 4. Namespace Ownership

Top-level namespaces are project-owned and schema-associated.

Typical namespaces include:

- `actor.*`;
- `audio.*`;
- `collision_material.*`;
- `effect.*`;
- `environment.*`;
- `item.*`;
- `material.*`;
- `mission.*`;
- `module.*`;
- `nav.*`;
- `robot.*`;
- `scene.*`;
- `shader.*`;
- `ship.*`;
- `station.*`;
- `terrain.*`;
- `texture.*`;
- `weapon.*`;
- `world.*`.

A namespace prefix improves organization but does not replace explicit `ContentKind` validation.

## 5. Content Kind

Every registered content entry has one closed project-owned `ContentKind`.

Baseline kinds include:

- Definition;
- Mesh;
- Skeleton;
- AnimationClip;
- Texture;
- Material;
- Environment;
- ShaderFamily;
- ShaderBundle;
- Audio;
- Collision;
- GroundNavigation;
- FreeFlightNavigation;
- Terrain;
- Scene;
- ProceduralModule;
- MissionTemplate;
- RaidTemplate;
- DynamicEventTemplate;
- WorldDefinition;
- other explicitly introduced project kinds.

A `ContentId` cannot resolve to two kinds in the same registry.

## 6. Definition Envelope

Every `.sfdef.json` uses a common top-level envelope:

```json
{
  "schema": "starforge.weapon",
  "schema_version": 1,
  "content_id": "weapon.ballistic.rifle_mk1",
  "data": {}
}
```

Optional envelope fields can include schema-approved:

- `display_name_key`;
- `dependencies`;
- `provenance`;
- development notes metadata;
- build/cook profile hints that do not alter gameplay authority outside the schema.

## 7. Schema Identity

Each definition schema has:

- stable `SchemaId`, e.g. `starforge.weapon`;
- positive integer `SchemaVersion`;
- closed field contract;
- semantic validation rules;
- reference-kind constraints;
- migration/compatibility policy.

Changing a schema incompatibly requires a schema-version increment.

## 8. Schema Files

Machine-readable schema descriptions live under:

```text
content/schemas/
```

The exact schema-description representation can be StarForge-owned JSON schema metadata; the authoritative semantics remain the TA/GDS contract plus project validators.

A generic third-party JSON Schema engine is not required as gameplay/runtime authority.

## 9. Unknown Fields

Unknown fields fail validation by default.

This prevents a typo such as:

```text
penetraton_power
```

from silently becoming ignored content.

A schema may explicitly provide a namespaced extension object only when a real extensibility use case exists.

## 10. Units Are Typed by Schema

Numeric data must declare meaning through schema fields, not comments or unit suffix guesses.

Examples:

- meters;
- kilograms;
- seconds of Simulation Time;
- degrees/radians according to field contract;
- joule-like energy units where a gameplay spec defines them;
- normalized 0..1 ranges.

The cooker validates ranges and unit expectations.

## 11. Content References

Structured content references another content definition through typed `ContentId` fields.

Reference categories:

- **RequiredContentRef** — target must exist and match expected ContentKind/schema;
- **OptionalContentRef** — explicit null/absence is valid;
- **PresentationFallbackRef** — optional presentation-only target may use a declared fallback;
- **Historical/MigrationRef** — used only in compatibility/migration metadata, not ordinary runtime lookup.

There is no generic unchecked string reference.

## 12. Runtime Semantic References vs Build Dependencies

TA-10 distinguishes:

1. **Semantic Content Reference** — logical relation in data, which can legitimately participate in cycles;
2. **Build Dependency** — artifact A must be cooked before/with artifact B, which must form a valid dependency graph for the content compiler.

Example:

- Faction definition can reference a mission family while mission content references that faction semantically;
- runtime registry resolution handles the logical cycle;
- their cooked build products must not require recursively embedding each other.

The content compiler must not confuse semantic reference cycles with impossible build cycles.

## 13. Registry Generation

A validated content build generates one immutable **Content Registry**.

Each entry contains conceptually:

```text
ContentRegistryEntry {
  ContentId
  ContentKind
  SchemaId / SchemaVersion where applicable
  CookedFormatVersion
  ContentFingerprint
  CookedLocator
  RequiredBuildDependencies[]
  OptionalDependencies[]
  CompatibilityFlags
  DebugSourceLocator (development only)
}
```

## 14. Duplicate Identity

Two canonical source records declaring the same ContentId are a hard content-build error.

The compiler does not choose one based on:

- source path order;
- filesystem enumeration order;
- timestamp;
- latest modification;
- build-thread completion.

## 15. Registry Ordering

Registry serialization uses deterministic ordering by canonical ContentId bytes, with ContentKind/schema as validation metadata rather than an unstable container iteration order.

A different filesystem traversal order must produce the same logical registry.

## 16. Runtime Lookup

Runtime public lookup is conceptually:

```text
ContentRegistry::resolve(ContentId, ExpectedKind)
```

Resolution returns an immutable registry/content handle or a typed failure.

Gameplay code does not scan the filesystem or search by display name.

## 17. Runtime Registry Index

The runtime may assign compact registry indices/handles for performance.

Rules:

- index is nonpersistent;
- index is valid only for the loaded ContentBuildId/registry generation;
- saves never serialize the compact index instead of ContentId;
- hot reload/content-registry rebuild invalidates generation-sensitive cached indices.

## 18. Content Fingerprint

Each entry has a deterministic **ContentFingerprint** computed from:

- canonical source bytes/normalized semantic input;
- transitive build dependency fingerprints;
- relevant schema versions;
- relevant cooker/tool version;
- target cook profile/options.

Version-1 fingerprint uses **SHA-256**.

Wall-clock timestamp and absolute source path are excluded.

## 19. ContentBuildId

A complete validated content build has a `ContentBuildId` derived from the deterministically ordered registry entries and their fingerprints plus content-toolchain version metadata.

Version-1 uses SHA-256 and exposes the digest in development/build diagnostics.

`ContentBuildId` identifies one complete compatible cooked content set; it is not a gameplay save ID.

## 20. ContentBuildId and Saves

Save files continue storing stable ContentIds as specified by TA-2.

A save may record ContentBuildId as diagnostic/load-compatibility metadata, but the build hash is not used to avoid explicit migration.

Loading against a different ContentBuildId is allowed only when all required saved ContentIds/schema expectations remain resolvable or TA-12 migration explicitly handles changes.

## 21. Rename / Removal Compatibility

A ContentId rename/removal is a compatibility event.

It cannot be implemented as silent runtime fallback.

TA-10 provides an authored compatibility map for migration tooling, conceptually:

```text
Old ContentId -> New ContentId / RemovedWithMigrationRule
```

TA-12 owns actual save migration execution.

Ordinary registry lookup does not automatically follow hidden aliases indefinitely.

## 22. Deprecated Content

A content entry can be marked deprecated for authoring validation.

Deprecated content:

- remains resolvable while compatibility requires it;
- produces warnings when newly referenced;
- cannot disappear from shipped builds while supported saves still require it unless migration exists.

## 23. Schema Version Compatibility

Cooked records store the schema version they were produced from.

The content cooker either:

- accepts and explicitly upgrades supported old source schema to current normalized form during cooking; or
- rejects it with a migration diagnostic.

Runtime does not guess missing fields from arbitrary historical versions.

## 24. Cross-Registry References

Version-1 ships one logical project Content Registry for the current build.

Separate DLC/mod registries and third-party override precedence are not baseline requirements.

This avoids defining load-order/mod conflict semantics before the base game requires them.

## 25. No Display-Name Identity

Localized/display names can change freely without changing ContentId.

UI/localization keys are separate presentation content and TA-11 owns their player-facing use.

## 26. No Path Identity

Source path and cooked locator are registry implementation details.

A save that references `weapon.ballistic.rifle_mk1` remains valid if its source file moves from one subdirectory to another, provided content compatibility is maintained.

## 27. Content Ownership vs Gameplay Ownership

A Content Definition is immutable authored data.

It does not own runtime/persistent mutable state.

Examples:

- `robot.chassis.heavy_mk1` describes a chassis definition;
- `RobotId(42)` owns one persistent robot's actual state;
- `mission.story.ms_a204` describes the authored mission template;
- `MissionId(17)` owns the save-persistent mission record.

The registry never substitutes for a gameplay domain store.

## 28. Registry Integrity

Registry load validates:

- magic/version;
- ContentBuildId;
- unique ContentIds;
- valid ContentKind values;
- locator validity;
- fingerprint field integrity;
- expected cooked-format versions;
- required registry-level references.

A malformed registry is a content-load failure, not an opportunity to guess entries from directories.

## 29. Diagnostics

Content diagnostics identify at minimum:

- ContentId;
- source file/field where known;
- SchemaId/version;
- expected/actual ContentKind;
- dependency/reference chain;
- error code/message.

A generic `asset failed` message is insufficient.

## 30. Test Requirements

Tests must prove:

- invalid ContentId grammar is rejected;
- uppercase ContentId is rejected in canonical source;
- duplicate ContentId is rejected deterministically;
- expected-kind mismatch fails;
- unknown JSON fields fail;
- required vs optional reference semantics differ correctly;
- semantic cycles do not automatically become build cycles;
- registry order is filesystem-order independent;
- path move does not change ContentId;
- source byte/schema/cooker change changes fingerprint;
- timestamp-only change does not change fingerprint;
- save-facing lookup uses ContentId, not runtime index;
- missing saved required ContentId requires migration/fails clearly;
- migration metadata is not ordinary runtime alias lookup.

## 31. Explicit Non-Goals

No baseline requirement for:

- GUID/UUID authoring IDs;
- path-as-ID;
- display-name lookup;
- fuzzy lookup of misspelled IDs;
- global mutable content registry during normal shipping play;
- third-party mod override/load-order semantics;
- schema-free arbitrary JSON blobs;
- registry indices in saves.

## 32. Dependencies

Depends on TA-2 identity/save compatibility, TA-10 source layout, later TA-12 migrations, and every gameplay/render/runtime domain that consumes authored content.

## 33. Open Questions

None in the ContentId/schema/registry contract.
