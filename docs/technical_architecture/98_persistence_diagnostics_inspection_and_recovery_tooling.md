# TA-12 — Persistence Diagnostics, Inspection, and Recovery Tooling

> **Status:** Architecture Complete  
> **Authority:** Persistence diagnostics, save inspection, corruption reporting, recovery tooling boundaries, deterministic dumps, golden fixtures, support data, and non-authoritative repair policy

## 1. Purpose

Persistence failures are high-cost failures. StarForge therefore requires first-class diagnostics and inspection tooling that can explain why a save failed without becoming a second loader or silently mutating gameplay state.

## 2. Persistence Diagnostic Event

Every save/load/migration operation emits structured diagnostic records containing conceptually:

- operation ID;
- operation kind;
- campaign/storage locator identifier;
- SnapshotSequence/logical slot;
- stage;
- SectionKind/schema version where applicable;
- byte offset/size where applicable;
- typed error code;
- relevant PersistentId/ContentId when safe;
- migration step ID;
- platform/storage error;
- elapsed diagnostic timing.

Diagnostics are not gameplay events.

## 3. Stable Error Codes

Persistence uses project-owned stable error categories/codes rather than only free-text exceptions.

Baseline families include:

- `PERSIST_REQUEST_*`;
- `PERSIST_STORAGE_*`;
- `PERSIST_CONTAINER_*`;
- `PERSIST_SECTION_*`;
- `PERSIST_SCHEMA_*`;
- `PERSIST_MIGRATION_*`;
- `PERSIST_CONTENT_*`;
- `PERSIST_IDENTITY_*`;
- `PERSIST_OWNERSHIP_*`;
- `PERSIST_TIMER_RNG_*`;
- `PERSIST_ACTIVATION_*`;
- `PERSIST_INTERNAL_*`.

The exact numeric/string code table is implementation-locked in TA-16 but category semantics are fixed.

## 4. User-Facing vs Technical Detail

TA-11 receives concise safe summaries such as:

- Save failed: storage unavailable;
- Save is corrupt;
- Save was created by a newer unsupported version;
- Required game content is unavailable;
- Migration failed;
- Profile settings could not be saved.

Development logs/tools retain section, offset, ID, content, and migration detail.

A user-facing error never exposes raw memory addresses or irrelevant backend internals.

## 5. Save Inspector

Development builds provide a project-owned `starforge-save-inspect` capability, either as a command-line tool or headless executable target.

It can:

- print header fields;
- list directory entries;
- verify header/directory/payload CRCs;
- show SectionKind/schema/size/required status;
- decode supported sections into human-readable diagnostic output;
- show ContentBuildId/application compatibility;
- run migrations in dry-run workspace;
- run domain/cross-domain validators;
- print migration change ledger;
- compare two save generations structurally/semantically where supported.

It does not activate gameplay or require renderer/audio/window.

## 6. Inspector Is Not a Second Schema Authority

The inspector reuses the same:

- container reader;
- serializer registry;
- decoders;
- migration functions;
- validators

as the game persistence module.

It cannot maintain a separate permissive parser that accepts files the shipping loader rejects.

## 7. Human-Readable Diagnostic Dump

The tool may emit a canonical text/JSON-like diagnostic dump of current DTO state.

This dump is:

- read-only diagnostic output;
- not loadable shipping save authority;
- allowed to redact/omit large binary/presentation data;
- deterministically ordered by SectionKind and schema-defined record key.

## 8. Save Comparison

Development comparison can report:

- changed SectionKinds;
- schema/version differences;
- Persistent IDs added/removed/changed;
- ContentId changes;
- ownership quantity changes;
- transaction/timer/RNG changes;
- active context differences.

Comparison does not infer that every difference is a bug.

## 9. Corruption Localization

When CRC/bounds failure occurs, tooling identifies the smallest safe scope possible:

- header;
- directory;
- specific SectionKind;
- specific decoded field/offset where decoder reached before failure.

It never continues decoding beyond a violated bounds/CRC contract merely to produce more output.

## 10. Recovery Tool Boundary

Baseline recovery tooling can:

- rebuild catalog index from committed files;
- identify newest valid generation;
- move invalid files to quarantine;
- delete stale pending files after policy checks;
- restore a previous valid profile backup;
- export/copy diagnostic bytes.

It cannot:

- invent missing resources;
- duplicate inventory to fix ownership;
- assign replacement persistent IDs heuristically;
- mark objectives completed;
- reset story choice;
- recreate a missing required ContentId by fuzzy matching.

## 11. No Automatic Save Repair

There is no general-purpose "repair save" button that edits broken gameplay state until validators pass.

Only two transformations are allowed:

1. explicit versioned migration;
2. explicit accepted deterministic recovery transform with the same governance/testing standard as migration.

Anything else remains diagnostic/manual-support territory and must not be presented as authoritative safe recovery.

## 12. Quarantine Metadata

When a file is quarantined, persistence can store a small sidecar diagnostic record containing:

- original locator;
- quarantine reason/error code;
- detection timestamp for user support only;
- original header summary if safely readable;
- no modified gameplay payload.

Wall-clock quarantine timestamp has no gameplay semantics.

## 13. Golden Save Fixtures

Repository test data includes synthetic golden fixtures for:

- current minimal valid save;
- representative current save;
- each supported historical application/domain migration generation;
- specific corruption cases;
- specific ownership/reference invariant failures;
- content rename/removal migration cases;
- active mission/raid/local-context continuation state.

Fixtures contain no user data.

## 14. Golden Fixture Stability

Current-format exact-byte fixtures are updated only when:

- container/schema compatibility intentionally changes;
- deterministic metadata fixture inputs intentionally change.

A random refactor that changes bytes under the same schema is treated as a regression until explained.

## 15. Corruption Fixtures

Purpose-built invalid fixtures include:

- bad magic;
- bad header CRC;
- overflowing directory size;
- overlapping payloads;
- bad directory CRC;
- payload CRC mismatch;
- duplicate SectionKind;
- missing Required section;
- invalid UTF-8;
- invalid enum/bool;
- excessive count/length;
- duplicate Persistent ID;
- missing Strong reference;
- duplicate Unique Item ownership;
- invalid allocator NextValue;
- invalid RNG increment;
- missing required ContentId.

The expected rejection stage/error family is locked by tests.

## 16. Fuzzing Boundary

TA-14 may add fuzzing/property tests around:

- fixed header parser;
- section directory parser;
- bounded BinaryReader primitives;
- selected domain decoders.

Fuzz input never reaches live gameplay state without the full staging/validation pipeline.

## 17. Sensitive Information

Persistence diagnostics should avoid collecting unrelated personal information.

Normal save files can contain user-entered save labels/callsigns. Diagnostic bundles should expose such strings only when needed and can provide a redaction mode.

No credentials/tokens are collected.

## 18. Support Bundle

A future/optional support bundle can include:

- application version;
- content build ID;
- persistence diagnostic log;
- save header/directory summary;
- migration ledger;
- redacted validation report.

Uploading/transmitting bundles is not a baseline requirement and always remains user-controlled.

## 19. Performance Diagnostics

Persistence records non-gameplay performance metrics such as:

- capture duration;
- per-section export/encode/decode duration;
- encoded size;
- CRC duration;
- write/flush duration;
- migration duration;
- validation duration;
- activation preparation duration;
- catalog scan duration.

Numeric budgets/threshold gates belong to TA-13/TA-14.

## 20. Deterministic Validation Report

For the same invalid fixture/current build, the primary rejection stage/code should be deterministic.

If multiple independent invariant violations exist, validation uses a documented stable validation order so logs/tests do not depend on hash/thread order.

Additional secondary errors may be reported only when doing so is safe and does not change the primary rejection semantics.

## 21. Development Assertions

Persistence invariants that indicate programmer error can assert/fail-fast in development after diagnostic capture, for example:

- duplicate registered SectionKind;
- writer emitted non-zero reserved bytes;
- writer generated directory overlapping payload;
- Required serializer missing;
- live DomainStore pointer captured by background writer.

Shipping builds convert user-data corruption into controlled typed failures rather than crashing intentionally.

## 22. Telemetry Boundary

No online telemetry service is required.

If telemetry is introduced later, persistence diagnostics are opt-in/product-policy governed and must not become required to save/load locally.

## 23. Inspector Write Operations

Baseline inspector is read-only for gameplay saves.

Commands that delete/quarantine storage debris operate at file/catalog level and never rewrite payload bytes.

A future migration-conversion export can write a **new** save generation/file from a validated staged migration, but it must never modify the source in place.

## 24. Test Requirements

Tests cover:

- inspector and game loader agree on validity;
- catalog rebuild requires no gameplay activation;
- quarantine does not alter source bytes before move;
- deterministic invalid fixture -> deterministic primary error;
- diagnostic dump ordering independent of hash order;
- redaction mode removes user-entered strings where configured;
- source save remains unchanged after dry-run migration;
- corrupt payload is never passed to semantic import.

## 25. Explicit Non-Goals

No baseline:

- heuristic gameplay repair;
- hidden automatic save editing;
- separate permissive inspector parser;
- credential collection;
- mandatory online support upload;
- gameplay decisions based on diagnostic timing/performance data.

## 26. Dependencies

Depends on all TA-12 persistence contracts and feeds TA-14 testing/CI architecture.

## 27. Open Questions

None.
