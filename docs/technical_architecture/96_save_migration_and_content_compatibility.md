# TA-12 — Save Migration and Content Compatibility

> **Status:** Architecture Complete  
> **Authority:** Container/application/domain migration policy, migration support window, deterministic migration execution, ContentId remaps/removals, migration-created IDs, compatibility diagnostics, and source-save preservation

## 1. Purpose

Project StarForge must evolve without silently reinterpreting old save data or turning content updates into undefined state.

TA-12 therefore defines migration as explicit, deterministic staged transformation from historical save representations to the current Save DTO contracts.

## 2. Migration Layers

Three migration layers exist:

1. **Container migration** — file/header/directory structure changes;
2. **Application compatibility migration** — broad save-generation compatibility changes spanning several domains;
3. **Domain Schema migration** — one SectionKind DTO version to the next.

Content compatibility participates through explicit TA-10 ContentId migration metadata.

## 3. Current v1 Container Policy

Container v1 is parsed directly by the v1 reader.

If a future ContainerVersion changes byte structure:

- a dedicated historical container reader decodes the old container into historical section bytes/metadata;
- the original file is never modified in place;
- migration continues in staging;
- successful load can later be saved as a new current-format generation.

## 4. ApplicationSaveCompatibilityVersion

The project owns a monotonically managed `ApplicationSaveCompatibilityVersion:uint32`.

It changes only when broad compatibility policy requires it; ordinary domain schema changes do not automatically increment it.

The application maintains an explicit table:

```text
stored compatibility version -> Supported / MigrationRequired / Unsupported
```

Marketing/application version strings are diagnostics only.

## 5. Domain Migration Chain

Each SectionKind owns a linear explicit chain where supported:

```text
Schema V1 -> V2 -> V3 -> Current
```

Rules:

- no implicit field-name matching;
- no reflection-based guess migration;
- every step has deterministic input/output;
- missing intermediate step means the historical version is unsupported;
- one domain cannot secretly mutate another domain's DTO outside a declared cross-domain migration stage.

## 6. Migration Registry

The Persistence serializer registry exposes, per SectionKind:

- current SchemaVersion;
- oldest directly supported historical version;
- ordered migration functions;
- any required cross-domain migration tag;
- migration test fixture identifiers;
- content-compatibility dependencies.

Startup validation ensures the declared support chain has no gaps.

## 7. Migration Purity

Migration may read only:

- decoded historical DTOs;
- other staged historical/current DTOs explicitly participating in the same cross-domain migration;
- saved allocator/RNG/timer state;
- versioned migration constants/tables;
- current TA-10 compatibility/remap metadata;
- current content definitions only where the migration contract explicitly requires semantic replacement data and the referenced content is itself versioned/stable enough for that rule.

Migration may not depend on:

- wall-clock time;
- network/web service;
- live-session state;
- player input;
- render/audio state;
- fresh entropy;
- worker completion order.

## 8. Original Save Preservation

Loading/migrating an old save never rewrites the selected source file.

On successful migration:

- migrated state becomes staging/current session state;
- source file remains intact;
- first subsequent save writes a new current-version immutable generation.

This preserves rollback and support evidence.

## 9. Migration Workspace

All migration operates inside an isolated `MigrationWorkspace` containing:

- source container/version metadata;
- per-section historical/current DTOs;
- staged allocator state;
- ContentId remap decisions;
- migration-created record plan;
- diagnostics/change ledger.

The workspace is discarded completely on failure.

## 10. Cross-Domain Migration

A cross-domain migration is required when one historical fact changes ownership or representation across SectionKinds.

Examples:

- a field moves from SessionCore to ProgressionKnowledge;
- old combined World/Economy record is split;
- ownership representation changes from embedded inventory to explicit OwnerRef;
- one old persistent entity record splits into multiple current records.

Such migration has an explicit ordered coordinator rather than two independent migrations both guessing about the other domain.

## 11. Persistent ID Preservation

If a logical entity still exists, its Persistent ID is preserved.

Migration never allocates a replacement ID merely because:

- record layout changed;
- content definition renamed;
- domain serializer changed;
- active/dormant representation changed.

References are rewritten only when the semantic entity truly changes under an explicit migration rule.

## 12. Migration-Created IDs

When one old record must create additional persistent records:

1. preserve the original ID on the explicitly designated successor;
2. gather all new-record creation requests;
3. sort them by a schema-defined stable key;
4. allocate from staged saved allocator state in that order;
5. rewrite references deterministically;
6. advance allocator state;
7. validate no collision/zero ID.

Hash iteration or thread order cannot influence new IDs.

## 13. Transaction/Idempotency Preservation

Migration preserves or explicitly transforms:

- Reward Transaction IDs;
- finale/major choice commit markers;
- mission objective exactly-once markers;
- reinforcement committed state;
- manufacturing completion markers;
- other one-time operation identity.

A migration cannot make an already committed one-time reward/action eligible again merely because the schema changed.

## 14. ContentBuildId Difference

A differing saved/current ContentBuildId is a compatibility signal, not an automatic failure.

The loader evaluates actual required saved ContentIds and schema expectations.

Possible outcomes:

- all required IDs still resolve -> compatible;
- explicit rename/remap exists -> migrate;
- explicit removal-with-semantic-rule exists -> apply that rule;
- required ID missing with no rule -> fail;
- only optional presentation content missing with approved fallback -> load with presentation fallback.

## 15. ContentId Rename

A TA-10 compatibility record can define:

```text
old.content.id -> new.content.id
```

Migration verifies:

- expected old ContentKind/schema;
- expected new ContentKind/schema;
- no remap cycle;
- remap is valid for the save/application compatibility range.

Then the historical DTO is rewritten to the new canonical ContentId before current import.

Ordinary runtime lookup does not permanently follow the alias.

## 16. Content Removal With Semantic Replacement

A removed required ContentId needs an explicit rule, for example:

- replace old weapon definition with a named successor while preserving UniqueItemId and condition/ammo state;
- replace removed station module definition with a known compatible module state;
- convert deprecated research node to a designated current knowledge state.

The rule must define ownership, identity, quantities, and any lost/changed capability explicitly.

If no acceptable semantic replacement exists, load is unsupported rather than silently deleting the asset.

## 17. No Silent Persistent Asset Deletion

Migration must not silently discard:

- crew;
- ships;
- robots;
- unique items;
- physical resource quantities;
- completed research/Blueprints;
- discovered routes/knowledge;
- mission/finale history;
- committed transaction state.

Any intentional removal/conversion requires an accepted migration rule and test fixture.

## 18. Content Definition Semantic Change

A ContentId can remain the same while its current definition changes.

If that change makes old persistent state invalid, one of these must occur:

- current domain validator accepts the old state under the new definition;
- schema/application migration transforms the state explicitly;
- the content definition retains compatibility support;
- load is rejected as incompatible.

Persistence never clamps/repairs persistent state solely because the current content happens to have different limits unless the migration explicitly says so.

## 19. Generated Content Versions

Persistent generated records retain `GenerationAlgorithmVersion` where required.

Migration policy:

- committed generated outcomes normally remain exactly stored;
- unfinished/future procedural generation requiring an old algorithm must keep a supported implementation or migrate its cursor/state explicitly;
- current executable does not silently regenerate old content using newest generator.

## 20. RNG Migration

If RNG representation/algorithm ever changes:

- it is an explicit architecture compatibility change;
- old stream state has a tested deterministic mapping or the affected save generation becomes unsupported;
- fresh random entropy is never substituted for an unmigratable cursor.

## 21. Timer Migration

Timer/deadline migration operates in SimulationTick units.

It cannot translate an old gameplay timer into a wall-clock expiry.

If tick-rate representation ever changes, conversion semantics must be explicitly defined to preserve remaining Simulation Time as closely/exactly as the architecture permits.

## 22. Migration Validation

After every migration layer, validation runs at the appropriate granularity:

- DTO structural validity;
- IDs/references introduced by migration;
- ownership conservation;
- transaction/idempotency markers;
- ContentId validity;
- allocator consistency;
- generation/timer/RNG invariants.

A later final cross-domain validation still runs after all sections reach current form.

## 23. Migration Change Ledger

For diagnostics, migration records a non-gameplay `MigrationChangeLedger` including:

- source/current container/app/domain versions;
- each migration step ID;
- ContentId remaps;
- records created/split/merged;
- persistent IDs preserved/newly allocated;
- explicit semantic conversion codes;
- warnings.

This ledger is not automatically written back into the source save.

## 24. User-Facing Compatibility Result

The UI receives a concise safe result:

- `Compatible`;
- `Migration Required`;
- `Unsupported Newer Save`;
- `Unsupported Historical Save`;
- `Required Content Missing`;
- `Migration Failed`;
- `Save Corrupt`.

Technical logs provide SectionKind/schema/ContentId details.

## 25. Migration Support Window

The architecture does not promise infinite historical compatibility.

Release/implementation policy must explicitly declare which historical Application/Domain versions remain supported.

Dropping support requires:

- deliberate compatibility decision;
- removal/update of migration fixtures/tests;
- release-note/support policy as appropriate;
- no reinterpretation of the dropped version as another schema.

TA-16/ongoing release governance owns operational support windows.

## 26. Forward Compatibility

An older executable is not guaranteed to load a save created by a newer executable.

Unknown newer:

- ContainerVersion;
- ApplicationSaveCompatibilityVersion;
- Required SectionKind;
- Required schema version;
- required codec/flags

cause explicit rejection.

No heuristic "try current decoder" path exists.

## 27. Mods/DLC Boundary

Third-party mod registry/load-order save compatibility is not part of the baseline.

Future DLC/mod architecture must add explicit registry namespace/availability semantics rather than overloading TA-12 base-game migration rules.

## 28. Migration Tests

Every supported migration path requires golden fixtures asserting at minimum:

- persistent IDs;
- physical ownership/quantities;
- Credits/transactions;
- progression/knowledge;
- mission/raid/event state;
- timers;
- RNG/generation version;
- content remaps;
- allocator next values;
- expected current DTO bytes/state after migration.

## 29. Deterministic Re-Migration

Migrating the same historical save under the same current build/content registry must produce semantically identical current staging state and deterministic Required-section DTO encoding.

Worker count/order cannot change the result.

## 30. Failure Semantics

On any migration failure:

- no live SessionRoot changes;
- source save remains unchanged;
- migration workspace is discarded;
- no partial current-format save is committed automatically;
- user can select another save/update version.

## 31. Explicit Non-Goals

No baseline:

- fuzzy ContentId matching;
- data-loss-by-default migration;
- live-session migration in place;
- fresh RNG substitution;
- auto write-back of old saves;
- hidden alias chains at normal runtime;
- infinite backward compatibility promise;
- older-build forward compatibility guarantee.

## 32. Dependencies

Depends on TA-2 RNG/migration/integrity, TA-10 content compatibility, TA-12 container/section/load architecture, and every domain owning a persistent DTO schema.

## 33. Open Questions

None.
