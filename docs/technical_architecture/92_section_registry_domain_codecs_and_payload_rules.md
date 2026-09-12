# TA-12 — Section Registry, Domain Codecs, and Payload Rules

> **Status:** Architecture Complete  
> **Authority:** Stable SectionKind registry, required/optional classification, domain serializer descriptors, payload framing, deterministic collection encoding, nested DTO rules, and schema ownership

## 1. Purpose

Container layout alone is insufficient. TA-12 needs one stable registry that maps each save section to exactly one semantic owner and one versioned codec contract.

The persistence layer encodes bytes; gameplay domains define what those bytes mean.

## 2. Stable SectionKind Values

Version-1 reserves the following numeric values:

| Value | SectionKind | Classification | Semantic owner |
|---:|---|---|---|
| 1 | `SaveMetadata` | Optional | Persistence/UI metadata |
| 100 | `SessionCore` | Required | Session/infrastructure root |
| 110 | `Player` | Required | Player domain |
| 120 | `Station` | Required | Horizon/Station domain |
| 130 | `Crew` | Required | Crew domain |
| 140 | `InventoryResources` | Required | Inventory/Resource authority |
| 150 | `Spacecraft` | Required | Spacecraft domain |
| 160 | `Robots` | Required | Robot domain |
| 170 | `Missions` | Required | Mission domain |
| 180 | `RaidsDefense` | Required | Raid/Horizon Defense domain |
| 190 | `WorldFactions` | Required | World/Faction/Reputation domain |
| 200 | `EconomyMarkets` | Required | Economy/Market/Credits domain |
| 210 | `ProgressionKnowledge` | Required | Progression/Research/Blueprint/knowledge domain |
| 220 | `DynamicEvents` | Required | Dynamic Event domain |
| 230 | `RecoveryCommunications` | Required | Recovery/strategic communications domain |
| 240 | `SimulationInfrastructure` | Required | Persistent timers/RNG/allocator infrastructure |
| 250 | `LocalContextContinuation` | Required | Consequential active-local continuation state |

Values 2–99 and unassigned values above 250 are reserved for future explicit architecture changes. Existing assigned values are never renumbered.

## 3. Why `LocalContextContinuation` Exists

Some consequential active-local state must survive a save even though it is not itself a long-lived strategic record.

Examples include eligible:

- mission-local hostile continuation state;
- active projectiles whose next result is consequential;
- Status instances not naturally embedded in another persistent record;
- local tactical search/awareness state;
- interaction/action continuation state explicitly required by gameplay rules.

This section is **not** a runtime registry dump. It contains project-owned, versioned continuation DTOs keyed by stable owning/context identity.

## 4. SessionCore Responsibilities

`SessionCore` includes only cross-domain session-root state that has no better semantic owner, such as:

- SaveSeed;
- current ApplicationSaveCompatibilityVersion semantic state if required;
- current active strategic/local location identity;
- campaign creation/version lineage metadata required by gameplay compatibility;
- save-owned Difficulty/custom gameplay axes where no dedicated domain owns them.

It does not duplicate records already owned by other sections.

## 5. SimulationInfrastructure Responsibilities

`SimulationInfrastructure` owns persistent infrastructure state such as:

- typed persistent ID allocator NextValue values;
- TransactionId allocator state;
- persistent timer scheduler state that is not embedded in owning domain DTOs;
- persistent PCG32 stream state;
- generation algorithm version/cursor infrastructure where centralized;
- persistent exactly-once operation markers whose owner is infrastructure rather than a gameplay domain.

A domain-owned timer/RNG cursor remains in that domain section when the owning architecture already places it there. There is no duplicate infrastructure copy.

## 6. Serializer Registry Descriptor

Every section registers one immutable descriptor:

```text
SectionSerializerDescriptor {
  SectionKind kind;
  uint32 current_schema_version;
  SectionRequirement requirement;
  ExportFunction export_fn;
  EncodeFunction encode_fn;
  DecodeFunction decode_fn;
  MigrationChain migrations;
  DomainValidateFunction validate_fn;
  ImportToStagingFunction import_fn;
  ValidationDependencySet validation_dependencies;
}
```

The registry is assembled explicitly in the application composition root.

## 7. Registry Validation at Startup

Application startup/tests reject:

- duplicate SectionKind descriptors;
- `SchemaVersion == 0`;
- missing codec/import/export functions for Required sections;
- descriptor Required/Optional mismatch with the project registry;
- validation dependency cycles that cannot be ordered explicitly;
- unsupported migration gaps within the declared support window.

## 8. Required Sections

All v1 Required SectionKinds must exist in every committed gameplay save.

A domain with no records still emits a valid empty/current-version payload rather than omitting the section.

This makes absence distinguishable from valid empty state.

## 9. Optional SaveMetadata

`SaveMetadata` is the only baseline Optional section.

Its contents are non-authoritative presentation/catalog data such as:

- display label;
- save creation wall-clock timestamp;
- active playtime;
- location display key;
- story/progression summary key;
- build/application display version;
- autosave trigger reason(s).

Corruption/loss of this section cannot alter world state.

## 10. Payload Framing

Each decoded section payload begins with a project-owned payload prologue:

| Field | Type |
|---|---|
| `payload_format_version` | `uint32` |
| `section_kind_echo` | `uint32` |
| `schema_version_echo` | `uint32` |
| `payload_flags` | `uint32` |

For v1:

- `payload_format_version = 1`;
- echoed kind/version must match the directory entry;
- unknown non-zero payload flags reject unless explicitly compatible.

The prologue is part of the payload bytes covered by payload CRC32C.

## 11. Why Echo Kind/Version

Directory metadata and payload metadata are checked against each other so accidental section-swapping/corrupt directory entries fail before semantic import.

The echo is not a second authority; disagreement is corruption.

## 12. Integer Encoding

Payload primitives use explicit little-endian widths:

- `u8/u16/u32/u64`;
- signed integers only where semantically required and encoded as explicit two's-complement fixed width;
- booleans as `u8` values `0` or `1` only.

Other boolean byte values reject.

## 13. Enum Encoding

Every serialized enum has:

- explicit fixed underlying width;
- documented stable numeric values;
- validation that the stored value is recognized or explicitly supported as historical.

C++ compiler-assigned enum numbering is never save authority.

## 14. Persistent ID Encoding

Typed Persistent IDs encode their raw non-zero `uint64` value.

Type comes from the schema field itself, not from an untyped runtime tag stored beside every ID.

`0` is allowed only for fields explicitly typed as optional/null identity.

## 15. ContentId Encoding

ContentIds encode as:

```text
uint32 byte_length
byte[byte_length] UTF-8
```

Validation applies the canonical TA-10 grammar after decode.

The save never serializes runtime ContentHandle/index in place of ContentId.

## 16. String Encoding

General strings use `uint32 byte_length + UTF-8 bytes`.

Rules:

- no null terminator in schema;
- byte length validated before allocation;
- UTF-8 validity required for textual fields;
- normalization policy is field-specific; persistence does not silently normalize identity strings.

## 17. Collection Encoding

Arrays use:

```text
uint32 element_count
encoded elements...
```

Maps/sets are never written in hash-bucket order.

For logically unordered collections, the domain codec first sorts by the schema-defined canonical key, usually:

- Persistent ID;
- ContentId bytes;
- stable composite key.

## 18. Optional Fields

Binary v1 payload schemas are positional/versioned rather than self-describing tagged-field maps.

Optional semantic values use explicit presence representation, for example:

```text
u8 present
[value when present]
```

Adding/removing/reordering fields requires SchemaVersion change unless the previous schema explicitly reserved bytes/structure for that purpose.

## 19. Variant/Union Encoding

Closed variants encode:

```text
discriminator fixed-width enum
variant payload
```

Unknown discriminators reject unless the current schema explicitly defines a preserved opaque optional branch. Required gameplay unions do not use best-effort unknown skipping.

## 20. Floating-Point Encoding

Where a DTO requires floating point:

- `float` persists as explicit IEEE-754 binary32 bits;
- `double` persists as explicit IEEE-754 binary64 bits;
- non-finite values reject unless the field schema explicitly permits them, which baseline gameplay schemas generally do not.

Persistence does not serialize locale-formatted decimal strings for authoritative numeric state.

## 21. SimulationTick and Durations

Simulation ticks/deadlines use explicit integer tick representations, normally `uint64`.

Wall-clock timestamps are prohibited in Required gameplay sections unless they are informational historical metadata with no progression semantics.

## 22. DTO Evolution

For each SectionKind:

- current writer emits exactly one current SchemaVersion;
- reader supports current plus the explicit migration support window;
- old payload decodes into an old-version DTO type or neutral migration representation;
- migration advances deterministically to current DTO;
- only current DTO imports into current staging DomainStore.

Current runtime classes are never asked to interpret arbitrary historical binary layouts directly.

## 23. No Generic Reflection Serializer

The baseline does not use a runtime reflection system to walk arbitrary C++ fields.

Project-owned codecs are explicit and testable.

Helper primitives for integers, strings, arrays, IDs, enums, and DTO composition are permitted, but schema ownership remains visible in code.

## 24. Deterministic Encoding Contract

A Required section encoded from semantically identical current DTO state must produce identical bytes across supported builds that claim the same schema/compatibility implementation.

Permitted differences are limited to explicitly non-authoritative metadata in `SaveMetadata`.

## 25. Decode Safety

Every decoder uses a bounded `BinaryReader`/equivalent with:

- immutable byte span;
- current cursor;
- remaining-length checks;
- overflow-safe count/length math;
- nesting/collection limit hooks;
- typed error with section/path/offset.

Reading past the payload end is impossible by API contract.

## 26. Full Consumption Rule

After a section decoder finishes, it must have consumed exactly the decoded payload length defined by its schema.

Unexpected trailing bytes in a v1 current/historical known schema reject rather than being silently ignored.

## 27. Import Boundary

Decode/migration creates neutral/staging DTOs only.

Import into staging domain state happens afterward through the owning descriptor.

A binary decoder does not directly allocate live gameplay objects, runtime handles, Jolt objects, OpenGL resources, or audio voices.

## 28. Cross-Section References

Payload codecs may store typed IDs referencing another section, but they do not resolve them during byte decode.

Resolution occurs during staged cross-domain integrity validation after all Required sections have decoded/migrated/imported.

This prevents section order from changing semantic results.

## 29. Section Schema Diagnostics

Errors identify:

- SectionKind;
- stored/current SchemaVersion;
- payload byte offset;
- DTO field path when known;
- error category;
- migration step where applicable.

The user receives safe summary text; development logs retain technical detail.

## 30. Tests

TA-14 must cover every current section with:

- empty/minimal payload fixture;
- representative round-trip;
- deterministic re-encode;
- truncated field rejection;
- invalid enum/bool/string/count rejection;
- trailing-byte rejection;
- migration fixture(s) for supported old versions;
- cross-section Strong reference validation.

## 31. Explicit Non-Goals

No baseline:

- protobuf/FlatBuffers/Boost.Serialization ownership of save schema;
- arbitrary self-describing reflection graph;
- hash iteration as encode order;
- runtime ContentHandle serialization;
- backend object serialization;
- missing Required sections interpreted as defaults;
- duplicate SectionKinds inside one file.

## 32. Dependencies

Depends on TA-2 IDs/domain stores/save DTOs, TA-7 continuation export, TA-9 strategic state, TA-10 ContentIds, and TA-12 container/load/migration architecture.

## 33. Open Questions

None.
