# TA-12 — Save Container v1 Byte Layout

> **Status:** Architecture Complete  
> **Authority:** Exact binary file layout, header fields, section directory entries, alignment, byte order, CRC coverage, reserved fields, and deterministic payload placement

## 1. Purpose

TA-2 fixed a project-owned chunked little-endian save format. TA-12 now locks the version-1 byte layout so serializer code cannot invent offsets, field sizes, padding, or validation behavior.

## 2. Primitive Encoding

Container v1 uses:

- little-endian integer encoding;
- fixed-width unsigned integers unless a schema says otherwise;
- IEEE-754 binary floating point only inside domain payload schemas that explicitly permit it;
- UTF-8 strings inside payloads with explicit byte length;
- zero-filled structural padding/reserved bytes;
- no native C++ struct dumps.

All offset/size arithmetic is checked for overflow before use.

## 3. File Structure

Canonical layout:

```text
+-------------------------------+ 0
| SaveHeaderV1 (128 bytes)      |
+-------------------------------+ 128
| SectionDirectory              | count * 64 bytes
+-------------------------------+
| zero padding to 8-byte align  |
+-------------------------------+
| Section payload 1             |
+-------------------------------+
| zero padding to 8-byte align  |
+-------------------------------+
| Section payload 2 ...         |
+-------------------------------+
| ...                           |
+-------------------------------+ file_size
```

Directory entries and payloads are ordered by ascending numeric `SectionKind`.

## 4. SaveHeaderV1

The v1 header is exactly **128 bytes**.

| Offset | Size | Field | v1 rule |
|---:|---:|---|---|
| 0 | 8 | `magic[8]` | ASCII bytes `SFGSAVE\0` |
| 8 | 4 | `container_version` | `1` |
| 12 | 4 | `header_size` | `128` |
| 16 | 4 | `endian_marker` | `0x01020304` encoded little-endian |
| 20 | 4 | `header_flags` | defined bits only; unknown required bits reject |
| 24 | 4 | `application_save_compat_version` | project compatibility gate |
| 28 | 4 | `section_entry_size` | `64` |
| 32 | 8 | `saved_simulation_tick` | authoritative snapshot tick |
| 40 | 8 | `snapshot_sequence` | persistence history sequence |
| 48 | 8 | `logical_slot_id` | persistence-only logical slot; reserved semantics for Quick/Auto |
| 56 | 4 | `save_kind` | stable enum: Manual/Quick/Autosave |
| 60 | 4 | `section_count` | number of directory entries |
| 64 | 8 | `directory_offset` | v1 canonical value `128` |
| 72 | 8 | `directory_size` | `section_count * 64` |
| 80 | 8 | `file_size` | exact committed file size |
| 88 | 4 | `directory_crc32c` | CRC32C over directory bytes only |
| 92 | 4 | `header_crc32c` | CRC32C over all 128 header bytes with this field zeroed |
| 96 | 32 | `content_build_id_sha256` | loaded TA-10 ContentBuildId digest |

No compiler padding is serialized. The writer emits fields explicitly by offset.

## 5. Header Flags

Version-1 defines these bits:

- bit 0: `HasOptionalMetadataSection`;
- bits 1–31: reserved, must be zero in v1 writers.

A v1 reader rejects an unknown flag whose future semantics are marked required by a later container version. For v1 input, any non-zero reserved bit is treated as unsupported/incompatible rather than guessed.

## 6. SaveKind Serialized Values

Stable v1 values:

- `1` = Manual;
- `2` = Quick;
- `3` = Autosave.

`0` is invalid. Unknown values fail catalog/load validation.

## 7. Header CRC32C

To compute `header_crc32c`:

1. encode the complete 128-byte header;
2. set bytes 92–95 to zero;
3. compute CRC32C over bytes 0–127;
4. write the resulting uint32 at bytes 92–95.

The reader repeats the same procedure before trusting offsets/counts beyond safety-minimum parsing.

## 8. SectionDirectoryEntryV1

Each directory entry is exactly **64 bytes**.

| Offset within entry | Size | Field | Rule |
|---:|---:|---|---|
| 0 | 4 | `section_kind` | stable SectionKind numeric value |
| 4 | 4 | `schema_version` | owning domain DTO schema version |
| 8 | 4 | `section_flags` | Required/Optional + defined compatibility bits |
| 12 | 4 | `codec` | v1 writer uses `0 = None` |
| 16 | 8 | `payload_offset` | absolute file offset, 8-byte aligned |
| 24 | 8 | `stored_size` | encoded bytes in file |
| 32 | 8 | `decoded_size` | bytes after codec decode |
| 40 | 4 | `payload_crc32c` | CRC32C over decoded payload bytes |
| 44 | 4 | `reserved0` | zero |
| 48 | 8 | `reserved1` | zero |
| 56 | 8 | `reserved2` | zero |

Any non-zero reserved field in a v1 entry is rejected as unsupported unless a later container version defines it.

## 9. Section Flags

Version-1 section flags:

- bit 0: `Required`;
- bit 1: `Optional`;
- bits 2–31: zero.

Exactly one of Required or Optional must be set.

Unknown Required `SectionKind` fails load. Unknown Optional sections may be skipped only when the current container reader understands the optional-skip contract and all remaining required invariants still pass.

## 10. Codec Values

Stable codec values begin with:

- `0` = None.

The v1 writer emits only `None`.

A future reader may support additional codecs without changing ContainerVersion if the directory semantics remain identical, but an unsupported codec on a Required section fails load.

## 11. Directory Ordering

Entries must be strictly ascending by `section_kind`.

Version-1 baseline uses singleton sections, so duplicate SectionKinds are invalid.

The loader rejects:

- out-of-order entries;
- duplicate SectionKinds;
- directory size/count disagreement.

This gives deterministic encoding and simple corruption checks.

## 12. Directory CRC32C

`directory_crc32c` covers exactly `directory_size` bytes beginning at `directory_offset`.

It validates directory metadata before payload allocation/reads.

Payload CRCs remain independent and cover decoded payload bytes.

## 13. Payload Placement

Canonical writer rules:

1. directory immediately follows the 128-byte header;
2. first payload begins at `align_up(128 + directory_size, 8)`;
3. every payload begins at an 8-byte-aligned absolute offset;
4. payloads appear in the same ascending SectionKind order as the directory;
5. alignment bytes are zero;
6. there are no hidden trailers in v1;
7. `file_size` equals the end of the final payload exactly.

## 14. Deterministic Structural Bytes

Given identical:

- snapshot gameplay DTO bytes;
- schema versions;
- metadata payload;
- SnapshotSequence/slot/save kind;
- ContentBuildId;

the structural encoding is byte deterministic.

No filesystem timestamp, memory address, hash iteration order, or uninitialized padding is included.

Real-world save creation time, when present, exists only in the metadata section and is intentionally allowed to differ.

## 15. ContentBuildId in Header

The 32-byte TA-10 SHA-256 ContentBuildId is copied into the header for fast compatibility preflight and diagnostics.

It is **not** sufficient by itself to determine compatibility.

A differing ContentBuildId can still load if required saved ContentIds and schema expectations resolve or migrate correctly.

## 16. File Size Validation

Before reading payloads the loader validates:

- physical file length equals `file_size`;
- `directory_offset == 128` for v1;
- `directory_size == section_count * 64` without overflow;
- directory lies wholly inside file;
- every payload lies wholly inside file;
- payload intervals do not overlap;
- payload offsets are 8-byte aligned;
- payloads do not overlap the header/directory;
- `stored_size`/`decoded_size` satisfy codec-specific maxima.

## 17. Size Safety Limits

Implementation uses configured hard safety ceilings for:

- total save file bytes;
- section count;
- individual stored/decoded section size;
- decoded collection counts and string lengths.

Exact numeric operational budgets are finalized in TA-13, but validation hooks and failure semantics are fixed here.

A file exceeding a safety ceiling fails cleanly before unbounded allocation.

## 18. CRC Validation Order

Canonical order:

1. read fixed header bytes;
2. validate magic/basic fixed constants;
3. validate header CRC32C;
4. validate file/directory bounds;
5. read directory;
6. validate directory CRC32C;
7. validate entries/bounds/order;
8. read/decode each required payload;
9. validate each decoded payload CRC32C;
10. pass bytes to schema decoder.

Schema decoding never receives a payload that failed CRC/bounds validation.

## 19. Metadata Section Corruption

The optional SaveMetadata section is presentation-only.

If its payload alone is corrupt while all required gameplay sections remain valid:

- the file can remain gameplay-loadable;
- catalog presentation marks metadata unavailable/corrupt;
- the loader does not fabricate user-facing metadata from gameplay sections unless a documented fallback read model exists.

Required gameplay sections never receive this fail-soft treatment.

## 20. No Whole-File Cryptographic Signature

Version-1 has no anti-cheat signature, encryption, or cryptographic authenticity requirement.

CRC32C detects accidental corruption, not malicious modification.

ContentBuildId is a content-set identifier, not a save signature.

## 21. Container Version Handling

Reader behavior:

- version 1 -> parse exactly this contract;
- version 0/unknown future version -> do not reinterpret as v1;
- supported future container versions require explicit container migration/reader code.

An ApplicationSaveCompatibilityVersion mismatch is evaluated only after the container itself can be safely parsed.

## 22. Golden Byte Fixture

TA-14 tests must include at least one tiny canonical v1 save fixture asserting exact bytes for:

- header offsets;
- endian marker;
- directory entry layout;
- CRC calculations;
- zero padding/reserved fields;
- payload alignment;
- deterministic re-encode.

Any intentional byte change requires an explicit compatibility decision.

## 23. Explicit Non-Goals

Container v1 does not include:

- native struct serialization;
- JSON gameplay payload framing;
- compression by default;
- encryption;
- anti-cheat signing;
- arbitrary duplicate section generations inside one file;
- append-only transaction journal;
- delta-save chains.

## 24. Dependencies

Depends on TA-2 serialization contracts, TA-10 ContentBuildId, TA-12 persistence service/section registry, and later TA-13 safety budgets/TA-14 golden fixtures.

## 25. Open Questions

None.
