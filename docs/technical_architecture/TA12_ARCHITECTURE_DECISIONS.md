# TA-12 — Architecture Decision Addendum

> **Status:** Accepted  
> **Authority:** TA-12 persistence implementation decisions  
> **Relationship:** Extends `ARCHITECTURE_DECISIONS.md` after AD-100 without rewriting prior accepted history

This addendum records the accepted architecture decisions introduced by TA-12. Decision numbering continues the central architecture sequence.

---

## AD-101 — Save History Uses Immutable Committed Generations and Logical Slot Supersession

**Status:** Accepted

### Decision

Every committed `.sfgsave` generation is immutable. Manual overwrite, Quick Save replacement, and Autosave rotation create a new `SnapshotSequence` generation first; older generations become superseded or cleanup candidates only after the new generation commits.

### Rationale

This removes in-place corruption risk and makes crash recovery and catalog reconstruction deterministic.

---

## AD-102 — Save Container V1 Has a Fixed 128-Byte Header and 64-Byte Section Entries

**Status:** Accepted

### Decision

Container v1 is little-endian with exact project-owned field offsets, `SFGSAVE\0` magic, header/directory CRC32C, 8-byte-aligned payloads, and 32-byte SHA-256 `ContentBuildId` metadata. Directory entries are exactly 64 bytes and payloads are ordered by stable `SectionKind`.

### Rationale

Exact byte contracts prevent implementation-defined padding, offsets, and compatibility behavior.

---

## AD-103 — Persistence Uses a Stable SectionKind Registry and Explicit Domain Codecs

**Status:** Accepted

### Decision

Required gameplay state is partitioned into fixed `SectionKind`s with explicit owning serializers and schema versions. `SaveMetadata` is the only baseline Optional section; `LocalContextContinuation` is a project-owned DTO section rather than a runtime-memory dump.

### Rationale

Persistence remains traceable to domain ownership while still capturing consequential active-local continuation state.

---

## AD-104 — Save Commit Is Validated Pending File to Unique Atomic Rename

**Status:** Accepted

### Decision

A save writes a unique same-directory pending file, flushes it, reopens it through the normal reader, validates container/section integrity, then atomically renames it into a previously unused committed `.sfgsave` generation name. Catalog update and old-generation cleanup happen after this commit point.

### Rationale

A failed write or cleanup cannot destroy the last known-good generation, and the catalog never becomes commit authority.

---

## AD-105 — Load Is Fully Staged and Replaces the SessionRoot Exactly Once

**Status:** Accepted

### Decision

Container decode, migrations, content resolution, domain import, local/cross-domain integrity validation, and active-context preparation all occur in isolated staging state. Only a fully valid staged session can replace the live `SessionRoot`, and the replacement is one application-level ownership swap.

### Rationale

No corrupt or incompatible save can partially mutate the running campaign or merge with its current inventory/economy.

---

## AD-106 — Save Migration Is Deterministic, Source-Preserving, and Identity-Preserving

**Status:** Accepted

### Decision

Supported historical saves migrate through explicit container/application/domain chains in staging. Source files are never rewritten during load; logical Persistent IDs and exactly-once transaction markers are preserved, with deterministic allocator-based creation only when a formal split/conversion rule requires new records.

### Rationale

Version upgrades must not create reload exploits, data loss, fresh identities, or migration outcomes dependent on timing or randomness.

---

## AD-107 — ContentBuildId Is Compatibility Metadata; Actual ContentId Resolution Is Authority

**Status:** Accepted

### Decision

A differing saved/current `ContentBuildId` does not by itself reject a save. Required saved `ContentId`s must resolve under the current registry or an explicit versioned rename/removal migration rule; missing required content without such a rule fails load.

### Rationale

Content builds can change without invalidating stable logical identity, while silent fuzzy aliases or deletions remain prohibited.

---

## AD-108 — Save Catalog and Campaign Manifests Are Rebuildable Storage Caches, Not Gameplay Authority

**Status:** Accepted

### Decision

Logical slot selection and menu metadata can use rebuildable index/manifest files, but committed `.sfgsave` headers and sections remain the authoritative storage evidence. Filesystem timestamps never decide save ordering; `SnapshotSequence` and validity do.

### Rationale

A crash between file commit and catalog update must not lose a successfully committed save.

---

## AD-109 — Profile/Application Settings Use Separate Versioned Fail-Soft Persistence

**Status:** Accepted

### Decision

Controls, accessibility, HUD, audio, display, and other non-gameplay settings persist outside campaign saves in a versioned project-owned profile document. Missing or invalid settings can safely default/clamp, and profile corruption cannot invalidate campaign world state.

### Rationale

Presentation/input preferences require different recovery semantics from authoritative gameplay saves and must be portable across campaigns.

---

## AD-110 — Persistence Diagnostics Share the Shipping Parsers and Do Not Heuristically Repair Gameplay State

**Status:** Accepted

### Decision

Save inspection/recovery tooling reuses the same container reader, codecs, migrations, and validators as the game. Recovery may rebuild catalogs or quarantine storage debris, but there is no general automatic repair that invents resources, IDs, objective state, or `ContentId`s.

### Rationale

A permissive second loader or heuristic repair path would undermine the exact integrity guarantees of normal loading.

---

## AD-111 — Loading Resumes After the Saved Stable Tick Without Advancing Gameplay During Load

**Status:** Accepted

### Decision

A save represents complete state after `saved_simulation_tick` reaches its Stable Simulation Boundary. Loading, migration, and backend reconstruction advance no gameplay; the next ordinary authoritative tick is `saved_simulation_tick + 1`.

### Rationale

This prevents duplicate timer/action processing and guarantees wall-clock load duration cannot progress the simulation.

---

## Consolidation Rule

TA-16 contract locking may mechanically consolidate AD-101 through AD-111 into the main `ARCHITECTURE_DECISIONS.md` sequence. Such consolidation is editorial only and must not change the accepted semantics recorded here.
