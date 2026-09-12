# TA-12 — Save Catalog, Slots, Manual Save, Quick Save, and Autosave

> **Status:** Architecture Complete  
> **Authority:** Local save-directory organization, logical save slots, immutable file generations, save catalog reconstruction, manual/quick/autosave semantics, rolling autosave retention, metadata, overwrite semantics, and storage identity

## 1. Purpose

The GDS distinguishes Manual Save, Quick Save, and Autosave while requiring crash safety and no arbitrary small manual-slot limit.

TA-12 therefore separates:

- **logical save intent** — Manual/Quick/Autosave and logical slot identity;
- **committed file generation** — one immutable `.sfgsave` file;
- **save catalog** — rebuildable presentation/index state over committed files.

The catalog is not gameplay authority.

## 2. User Data Root

All local persistence lives below a platform-resolved StarForge user-data root supplied by `platform`.

Conceptual layout:

```text
<UserData>/StarForge/
  profiles/
  saves/
    <CampaignStorageId>/
      manual/
      quick/
      autosave/
      quarantine/
      catalog.sfindex
```

Exact operating-system path resolution is a platform adapter concern and is locked during TA-16 implementation planning.

No gameplay system constructs absolute save paths directly.

## 3. CampaignStorageId

Each campaign storage directory has one opaque persistence-only `CampaignStorageId`.

Requirements:

- unique with overwhelming practical certainty on the local installation;
- generated once when the campaign storage root is created;
- not used for gameplay RNG, world identity, or persistent entity identity;
- not serialized as a substitute for gameplay IDs;
- safe to use in directory/file metadata.

The Windows implementation may use a platform-generated 128-bit random identifier or equivalent project-owned opaque token.

A deterministic development fixture can inject a fixed value for tests.

## 4. Campaign Storage Manifest

Each campaign directory contains a small versioned `campaign` storage manifest recording persistence-only catalog information such as:

- CampaignStorageId;
- display name where applicable;
- next Manual `LogicalSaveSlotId`;
- next SnapshotSequence hint;
- last-known committed save reference;
- manifest schema version.

The manifest is **not gameplay authority**.

If missing or corrupt, the persistence service reconstructs safe counters/catalog state by scanning valid committed save files and allocates future values above observed maxima.

## 5. Immutable Committed Generations

A committed `.sfgsave` file is never modified in place.

Every new save produces a new immutable generation with a unique SnapshotSequence.

Replacement behavior is logical:

- create and commit new generation first;
- update/rebuild catalog view;
- only then garbage-collect superseded generations according to save-kind policy.

This is stronger than overwriting the only previous file.

## 6. Canonical Filename Convention

Filenames are locators, not authority. Canonical version-1 naming:

```text
manual/m_<slotid>_<sequence>.sfgsave
quick/q_<sequence>.sfgsave
autosave/a_<sequence>.sfgsave
```

`slotid` and `sequence` are fixed-width lowercase hexadecimal or another project-defined stable ASCII encoding chosen in implementation and validated against the header values.

If a filename disagrees with the valid header, header metadata wins for semantic cataloging and the mismatch is diagnosed/quarantined as appropriate.

## 7. Pending Filenames

In-progress files use a non-loadable pending suffix/name, conceptually:

```text
.<target-name>.pending-<operation-id>
```

Only a successfully validated file renamed into the `.sfgsave` committed namespace is considered committed.

Normal catalog scans ignore pending files except during crash recovery.

## 8. Logical Manual Slots

A Manual Save creates or replaces a logical manual slot identified by non-zero `LogicalSaveSlotId:uint64`.

New Manual Save:

1. allocate new LogicalSaveSlotId;
2. capture new snapshot;
3. commit immutable file generation;
4. catalog slot points to newest valid generation.

Overwrite existing Manual Save:

1. retain same LogicalSaveSlotId;
2. create newer immutable generation;
3. commit it fully;
4. newest valid SnapshotSequence becomes active generation;
5. older superseded generation can be deleted later.

A crash between steps 3 and 5 leaves at least one valid generation.

## 9. Manual Save Capacity

No authored small gameplay limit exists.

The UI can create additional Manual Save slots until:

- storage is unavailable/full;
- platform/filesystem limit is reached;
- configured safety/storage policy rejects the operation.

Storage failure is reported explicitly and never deletes another manual save to make room without user-approved policy.

## 10. Manual Save Labels

User labels live in the optional SaveMetadata section/catalog metadata and are not file identity.

Changing a display label can be implemented by creating/updating catalog/profile metadata without mutating gameplay payloads; if labels are embedded only in immutable save metadata, editing a label creates a metadata/catalog update rather than rewriting gameplay authority.

TA-16 may choose the smallest compliant implementation.

## 11. Quick Save Logical Semantics

Quick Save is one logical history class rather than one mutable file.

Header rules:

- `save_kind = Quick`;
- `logical_slot_id = 0` is the reserved Quick Save slot value.

On successful new Quick Save:

- newest valid Quick generation becomes the UI's current Quick Save;
- previous Quick generation becomes superseded;
- cleanup may remove older Quick generations only after the new one is committed.

Baseline retention target is the newest **one** committed Quick generation after cleanup, while crash recovery may temporarily observe multiple valid generations.

## 12. Autosave Logical Semantics

Autosaves use:

- `save_kind = Autosave`;
- `logical_slot_id = 0` reserved for the rolling Autosave history class;
- SnapshotSequence to order committed generations.

Autosaves are independent full snapshots, never deltas from an earlier autosave.

## 13. Autosave Triggers

GDS baseline triggers remain authoritative:

- before external mission/raid deployment;
- after mission/raid stable resolution;
- after strategic travel arrival;
- after major story/faction decision commit;
- after significant Horizon Defense resolution/recovery transition;
- periodic protection every 10 minutes of Active Game Time when a Stable Save Boundary exists.

The persistence service receives these as typed trigger reasons; it does not decide gameplay milestones independently.

## 14. Periodic Autosave Clock

The 10-minute periodic protection interval uses **Active Game Time** as defined by the GDS for playtime/UX autosave scheduling.

It does not:

- advance Simulation Time;
- progress world systems while paused;
- use elapsed wall-clock time while the application is closed.

If the periodic trigger becomes due during an indivisible transaction, the request waits for the next Stable Save Boundary.

## 15. Rolling Autosave Retention

Baseline committed retention is the latest **10** valid autosave generations.

Rotation sequence:

1. write/validate/commit new autosave generation;
2. rebuild/update catalog;
3. sort valid autosaves by SnapshotSequence descending;
4. retain newest 10;
5. delete older generations as best-effort cleanup.

Cleanup failure does not invalidate the newly committed autosave; it produces diagnostics and may temporarily leave more than 10 files.

## 16. Never Delete Before Replacement Commits

For Quick and Autosave histories, persistence never deletes the previous known-good generation before the new generation reaches `Committed`.

This is a hard crash-safety invariant.

## 17. Save Catalog

`SaveCatalog` is an immutable/revisioned read model over local committed save generations.

Each entry contains conceptually:

- storage locator;
- CampaignStorageId;
- SaveKind;
- LogicalSaveSlotId;
- SnapshotSequence;
- saved SimulationTick;
- ApplicationSaveCompatibilityVersion;
- ContentBuildId;
- metadata summary/status;
- integrity/preflight status;
- file size;
- whether generation is active/superseded/quarantined.

The catalog never stores world state not already in the save.

## 18. Catalog Index File

`catalog.sfindex` is a rebuildable cache that accelerates menus.

It can store:

- last scan fingerprint/stat metadata;
- decoded header fields;
- optional SaveMetadata summaries;
- supersession relationships;
- validation status cache.

It is not required to load a save.

If missing, stale, corrupt, or inconsistent, it is discarded/rebuilt from committed files.

## 19. Catalog Rebuild

A full rebuild:

1. enumerate only recognized committed save roots/files;
2. read fixed headers with bounded I/O;
3. validate header CRC/basic bounds;
4. optionally validate directory/SaveMetadata for richer UI;
5. classify generation by campaign/kind/slot/sequence;
6. choose newest valid generation for each logical Manual slot and Quick history;
7. list newest valid Autosaves;
8. mark invalid/superseded files without loading gameplay.

Filesystem enumeration order never determines which generation wins; SnapshotSequence and validity do.

## 20. Duplicate Sequence Handling

Two committed files in the same campaign root claiming the same SnapshotSequence are an integrity/storage anomaly.

Persistence does not choose by modification time.

Policy:

- if byte-identical and semantically identical, one can be treated as duplicate storage debris after diagnostic verification;
- if different, both are quarantined from automatic selection and the catalog reports the conflict;
- no gameplay load is attempted automatically from an ambiguous generation.

## 21. Save Selection

UI selection always resolves to one concrete committed generation locator.

A logical Manual slot display usually selects its newest valid generation, but development/recovery UI may expose older retained superseded generations when they still physically exist.

Loading never means "merge latest pieces from multiple generations."

## 22. Delete Manual Save

Deleting a manual logical slot is a user storage action, not gameplay mutation.

The service:

- confirms according to TA-11 irreversible-action policy;
- removes all committed generations associated with that logical slot as best effort;
- updates catalog;
- never touches other logical slots/campaigns.

If deletion partially fails, remaining files remain catalog-visible/diagnosed rather than being silently treated as deleted.

## 23. Copy/Backup Behavior

Because each `.sfgsave` is self-contained, copying a committed file outside the game is sufficient to preserve that exact snapshot.

On reintroduction/import, the game validates the file normally and does not trust filename/path identity.

Formal import/export UX is not required by the baseline.

## 24. Metadata Read Without Gameplay Load

Save menus may read:

- fixed header;
- directory;
- optional SaveMetadata section;

without decoding all Required gameplay sections.

This is presentation/catalog inspection only and does not claim the save is fully load-valid until full load validation passes.

## 25. Compatibility Status in Catalog

Preflight status can be:

- `LikelyCompatible`;
- `MigrationRequired`;
- `UnsupportedNewerVersion`;
- `MissingRequiredContent` where determinable cheaply;
- `CorruptHeaderOrDirectory`;
- `MetadataDamagedGameplayUnknown`;
- `FullValidationRequired`.

Only the actual TA-12 load pipeline can produce final `LoadValid`.

## 26. Storage Space Estimation

Before writing a snapshot, storage backend can report available-space diagnostics/estimates.

Failure to predict space accurately is not correctness-critical: the actual write path must still handle out-of-space without corrupting prior generations.

## 27. Cross-Campaign Separation

A save generation belongs to exactly one campaign storage root.

The filesystem catalog cannot merge physical resources, IDs, or progression from different campaign directories.

Loading an older save in the same campaign legitimately creates an older descendant timeline state in memory, but storage histories remain separate immutable generations.

## 28. Last-Played Pointer

The application/profile may remember the last selected campaign/save locator as convenience.

It is non-authoritative. If missing or invalid, menus rebuild from catalog rather than creating/repairing gameplay state.

## 29. Crash-Recovery Interaction

Pending files, superseded generations, orphaned catalog entries, and interrupted cleanup are handled by `94_crash_safe_write_commit_and_recovery.md`.

The catalog only exposes files that satisfy committed-generation criteria.

## 30. Tests

Tests must cover:

- unlimited/new manual slot allocation behavior;
- overwrite creates newer immutable generation before old cleanup;
- Quick Save newest-generation selection;
- 11th autosave commits before oldest deletion;
- cleanup failure leaves new save valid;
- missing/corrupt catalog index rebuilds from files;
- filesystem enumeration permutation yields same catalog;
- duplicate SnapshotSequence conflict is not resolved by timestamp;
- optional metadata corruption does not imply gameplay corruption;
- filename/header disagreement is diagnosed;
- deleting one manual slot cannot remove another.

## 31. Explicit Non-Goals

No baseline requirement for:

- cloud sync;
- Steam/platform save APIs;
- tiny fixed manual-slot count;
- one mutable quick/autosave file;
- delta autosaves;
- wall-clock offline progression;
- filesystem timestamp as save ordering authority;
- catalog index as gameplay authority.

## 32. Dependencies

Depends on GDS Save/Persistence, TA-12 PersistenceService/container/crash recovery, TA-11 save UI, and later TA-13 storage budgets/TA-14 tests.

## 33. Open Questions

None.
