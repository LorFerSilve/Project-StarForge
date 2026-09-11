# Serialization and Save Contracts

> **Status:** Architecture Complete  
> **Authority:** Save DTO structure, binary save container, schema/versioning, section ownership, integrity checks, snapshot assembly, write strategy, and load staging

## 1. Purpose

This document translates the Design Complete save/persistence rules into an exact technical representation that avoids raw-memory serialization, partial-domain loads, duplicate ownership, and accidental coupling to runtime layout.

## 2. Serialization Principle

Authoritative runtime/domain objects are never serialized by dumping C++ object memory.

Every persistent domain exports a versioned **Save DTO** containing only explicit persistence fields.

Runtime-only caches, pointers, handles, backend objects, and derived data are reconstructed after load.

## 3. SaveSnapshot

At a Stable Save Boundary, persistence assembles one immutable `SaveSnapshot` containing:

- SaveContainer metadata;
- authoritative SimulationTick;
- global/session allocator state;
- per-domain versioned Save DTO sections;
- deterministic RNG state required by persistent systems;
- difficulty/gameplay settings that are save-owned;
- integrity/reference metadata required for validation.

The snapshot is immutable after capture.

## 4. Domain Save DTO Ownership

Each owning gameplay domain defines:

- its Save DTO schema;
- its current domain schema version;
- export from authoritative state;
- import into staging state;
- migrations from supported previous schema versions;
- validation of domain-local invariants.

`persistence` owns the container/encoding, not the semantic meaning of fields.

## 5. DTO Field Rules

Save DTOs may contain:

- fixed-width integers;
- booleans;
- explicitly defined floating-point values where required;
- canonical UTF-8 strings;
- enums with stable serialized numeric values;
- typed Persistent IDs as fixed-width integers;
- ContentId strings;
- arrays/maps with deterministic encoded ordering;
- nested versioned DTO structs.

They may not contain:

- raw pointers;
- references;
- `RuntimeEntityHandle`;
- Jolt/OpenGL/GLFW/miniaudio backend types;
- virtual-function/object-layout serialization;
- hash-map bucket state;
- function pointers/callbacks;
- mutex/thread primitives.

## 6. Binary Save Container

Save file format v1 is a StarForge-owned chunked binary container.

The v1 container is explicitly little-endian.

It uses a header plus a section directory followed by domain payloads.

## 7. File Header v1

The logical header contains at minimum:

- 8-byte magic: `SFGSAVE\0`;
- `ContainerVersion:uint32` = 1;
- endian marker/version marker;
- game/application save compatibility version;
- saved `SimulationTick:uint64`;
- section count;
- section-directory offset/size;
- whole-file integrity metadata where used;
- save metadata block reference.

Exact byte offsets are implementation-contract detail to be locked in TA-12 before serializer code begins, but these fields and semantics are fixed here.

## 8. Section Directory

Every domain section directory entry contains at minimum:

- stable `SectionKind` enum;
- domain `SchemaVersion:uint32`;
- payload offset;
- payload stored size;
- payload uncompressed size;
- codec enum;
- payload CRC32C;
- required/optional compatibility flag.

## 9. Compression Codec

Container v1 supports a codec field but the initial authoritative codec is:

`None`.

Compression is not required for the first implementation.

A later codec can be added without changing gameplay semantics if it preserves identical decoded DTO bytes.

## 10. Integrity Algorithm

Each section uses CRC32C over the decoded/uncompressed payload bytes.

Purpose:

- detect truncation/corruption;
- reject damaged sections before domain import.

CRC32C is an integrity/error-detection mechanism, not an anti-cheat/security signature.

## 11. Deterministic Encoding

Equivalent SaveSnapshot state must encode deterministically under the same schema version.

Rules include:

- little-endian fixed-width primitives;
- stable enum values;
- deterministic container ordering;
- canonical string encoding;
- no serialization of padding bytes;
- no hash-map iteration order;
- no platform pointer-size dependence.

Byte-for-byte equality is desirable for tests but may exclude metadata fields intentionally containing save creation timestamp or user-facing label; gameplay payload sections remain deterministic.

## 12. Floating-Point Serialization

Where exact floating-point gameplay state must persist, v1 stores IEEE-754 binary representation of the project-selected type explicitly.

The serializer does not round-trip through locale-dependent decimal text.

Later architecture must ensure calculations that require cross-platform determinism are constrained appropriately; this file guarantees faithful bit persistence of stored values.

## 13. String Encoding

All serialized text identifiers/strings use UTF-8.

Strings store explicit byte length and are not null-terminated as a schema requirement.

Invalid UTF-8 in fields declared textual is rejected during validation.

## 14. Enum Stability

Serialized enums have explicit underlying fixed-width integer values.

Existing serialized values are never renumbered casually.

Removed values require migration/compatibility handling.

## 15. Collection Ordering

If collection order has gameplay meaning, that order is serialized explicitly.

If collection order has no gameplay meaning, encoding still uses deterministic ordering, normally by:

- typed Persistent ID;
- ContentId;
- stable schema-defined key.

## 16. Allocator State

SaveSnapshot stores:

- NextValue for every persistent ID allocator;
- next TransactionId;
- persistent timer/operation allocators where their IDs must survive load.

RuntimeEntityHandle allocators are not serialized.

## 17. Active Entities in Save DTOs

Active persistent entities export one logical persistent record.

Save exporter merges:

- store-owned dormant/non-leased fields;
- current authoritative active-leased facets.

The DTO does not preserve duplicate active/dormant copies.

## 18. Runtime-Only State

The following are rebuilt after load unless an owning domain explicitly requires semantic persistence:

- renderer GPU objects;
- physics backend handles;
- spatial acceleration structures;
- UI widgets;
- audio voices;
- cached path searches;
- derived capacity caches;
- RuntimeEntityHandles;
- debug traces.

## 19. Persistent Active-Tactical State

Gameplay state needed to continue equivalently after loading must be serialized even if represented by active runtime components, for example:

- player transform/velocity where applicable;
- local ship transform/velocity;
- actor Health/shield/ammo;
- robot active command/ROE state;
- enemy awareness/Last Known Position where GDS requires mission persistence;
- projectile/status state where necessary to reconstruct exact consequential next state;
- Mission/Raid state.

TA-7/TA-9 define exact per-entity DTOs later.

## 20. Save Metadata

User-facing metadata is separate from authoritative gameplay sections and may include:

- display save name;
- save type;
- real-world creation timestamp;
- active playtime;
- current location label;
- progression phase;
- application version.

Real-world timestamp never advances gameplay.

## 21. Snapshot Capture

Canonical capture:

1. queued save waits for Stable Save Boundary;
2. simulation enters snapshot-capture phase at stable boundary;
3. each domain exports immutable DTO data and schema version;
4. persistence assembles SaveSnapshot + manifest;
5. snapshot consistency checks run;
6. simulation releases snapshot capture and can continue;
7. background worker encodes/writes the immutable snapshot.

## 22. Snapshot Memory Ownership

After capture, the serializer worker owns or shares immutable snapshot memory independent of live domain stores.

Live simulation may mutate newer state without changing the captured snapshot.

## 23. Write-New-Then-Commit

A save write never overwrites the only known-good file in place.

Storage sequence:

1. create new temporary file in same target filesystem/directory where atomic replace semantics are available;
2. encode/write all sections;
3. flush/close according to platform durability policy;
4. reopen/validate header/directory/checksums as required;
5. atomically replace/rename target save entry;
6. only then report save committed to UI.

On failure, prior valid save remains untouched.

## 24. Autosave Rotation

Rolling autosave rotation manipulates complete committed files/slots only.

It never deletes the last known valid autosave before a replacement has committed successfully.

## 25. Load Pipeline

Canonical load sequence:

1. open selected save;
2. validate header/magic/container version;
3. read section directory;
4. validate bounds/duplicates/required sections;
5. verify section CRC32C;
6. decode sections;
7. migrate DTOs to current domain versions;
8. build staging DomainStores;
9. validate domain-local invariants;
10. validate cross-domain references/ownership/IDs;
11. validate allocator state and deterministic RNG state;
12. resolve required ContentIds;
13. construct required Active Local Context staging data;
14. atomically replace live session root;
15. create runtime/backend proxies;
16. resume Simulation Time from saved tick.

## 26. Partial Load Prohibition

Normal save loading is all-or-nothing.

If a required section is corrupt/incompatible and cannot be migrated:

- current live session remains unchanged;
- staging state is discarded;
- load fails with diagnostic/user-facing error.

## 27. Required vs Optional Sections

Baseline core gameplay sections are Required.

An Optional section may be skipped only if its schema explicitly defines a safe absent/default meaning that does not alter required gameplay authority.

Unknown Required SectionKind is a compatibility failure.

## 28. Duplicate Sections

Container v1 rejects duplicate singleton SectionKinds unless a particular section schema explicitly defines a multi-section structure.

The loader never guesses which duplicate copy is authoritative.

## 29. Bounds and Size Validation

Before allocating from untrusted/corrupt save lengths, loader validates:

- section offset inside file;
- size arithmetic overflow;
- configured safety maxima;
- collection counts;
- string lengths;
- recursion/nesting limits where applicable.

A corrupt save cannot request arbitrary unbounded allocation.

## 30. Reference Validation

After import/migration, staging validation checks:

- Strong Required IDs resolve;
- Unique IDs are not duplicated;
- OwnerRef targets exist and are valid owner types;
- physical quantities have one authoritative owner;
- ContentIds resolve or have migration;
- active-local context identity exists;
- allocator NextValue exceeds all committed IDs for its type;
- TransactionId allocator exceeds all persisted committed transaction IDs that require comparison.

## 31. Save Schema Versions

Versioning is layered:

- ContainerVersion — file structure/codec directory format;
- ApplicationSaveCompatibilityVersion — broad game compatibility gate;
- Domain SchemaVersion — individual domain DTO schema.

A domain schema change does not require bumping ContainerVersion unless the file-container structure itself changes.

## 32. Forward Compatibility

The baseline guarantees migration from supported older save schemas to current code.

It does not guarantee that an older executable can load saves written by a newer executable.

Newer unsupported Required sections/versions cause a clear incompatibility error rather than heuristic load.

## 33. Unknown Fields

The v1 binary schema is explicit rather than self-describing field-tag storage.

Therefore forward addition of fields requires Domain SchemaVersion increment and migration/default logic.

This choice favors compact deterministic project-owned schemas over a general reflection format.

## 34. Save DTO Testing

Every domain Save DTO requires tests for:

- encode/decode round trip;
- current schema invariant validation;
- supported migration fixtures;
- corrupt/truncated payload rejection;
- deterministic collection ordering;
- missing Strong reference failure when cross-validation runs.

## 35. Golden Save Fixtures

Persistence tests maintain small versioned golden save fixtures for supported historical schema versions.

They test migration and compatibility without depending only on current writer output.

Golden fixtures contain synthetic/non-sensitive game state.

## 36. Debug Serialization

Development builds may provide a human-readable diagnostic dump of DTO/read state for inspection.

This diagnostic format is not the authoritative shipping save format and need not be loadable.

## 37. No Backend Type Leakage

A save schema never depends on the binary layout/version of:

- Jolt;
- OpenGL;
- GLFW;
- miniaudio;
- Dear ImGui;
- STL container implementation.

Third-party/runtime upgrades therefore do not automatically invalidate saves.

## 38. Performance Constraints

Snapshot capture should minimize time holding the main simulation boundary.

Expensive:

- compression;
- checksumming large payloads;
- file writes

occur after immutable snapshot capture on worker threads where safe.

The initial codec `None` keeps first implementation simple; profiling can justify later compression.

## 39. Diagnostics

Save/load diagnostics report:

- container version;
- section kinds/schema versions/sizes;
- CRC status;
- migration path;
- load-stage failure;
- unresolved typed ID/ContentId;
- allocator inconsistencies;
- snapshot capture duration;
- encoding/write duration.

## 40. Explicit Non-Goals

The serialization architecture does not use:

- raw C++ memory dumps;
- runtime handle serialization;
- JSON as authoritative shipping save format;
- server/cloud authority;
- wall-clock progression;
- partial best-effort load of corrupt required gameplay domains;
- first-version compression requirement;
- third-party backend object serialization.

## 41. Dependencies

This specification depends on TA-2 Identity, Domain State, Transactions, RNG/Migration, TA-1 Stable Save Boundary, and Design Complete GDS Save/Persistence.

## 42. Open Questions

None in the TA-2 serialization/save contract.
