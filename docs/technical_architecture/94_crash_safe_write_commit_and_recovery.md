# TA-12 — Crash-Safe Write, Commit, and Recovery

> **Status:** Architecture Complete  
> **Authority:** Pending-file protocol, durable write sequence, commit point, interrupted-write recovery, superseded-generation cleanup, quarantine, fault handling, and platform storage guarantees

## 1. Purpose

A save operation must never destroy the last known-good snapshot because the process crashes, power is lost, storage fills, or an I/O call fails.

TA-12 therefore treats every committed save generation as immutable and defines an explicit pending -> validate -> commit protocol.

## 2. Storage Backend Contract

`IPersistenceStorageBackend` is project-owned and provides bounded operations for:

- create unique pending file in target campaign directory;
- sequential write;
- seek/read for validation;
- flush file contents/metadata according to supported platform durability semantics;
- atomic same-directory rename from pending name to unique committed generation name;
- enumerate files;
- delete superseded/pending/quarantined files;
- query errors/space/capability diagnostics.

Gameplay/domain code never calls operating-system file APIs directly.

## 3. Unique Committed Target

The baseline does not overwrite an existing committed `.sfgsave` path.

Every save generation receives a unique committed filename containing its SnapshotSequence.

This reduces the required atomic primitive to:

> atomically make one fully written validated pending file visible under a previously unused committed filename in the same directory.

Logical slot replacement is handled by catalog supersession after commit.

## 4. Pending File Creation

The writer creates the pending file in the same directory/filesystem as its final committed generation.

Rules:

- create-new/exclusive semantics; never truncate a committed save;
- pending name includes enough operation uniqueness to avoid collision;
- permissions/access are user-local according to platform policy;
- pending extension/name is not recognized as loadable committed save.

## 5. Canonical Write Sequence

For one immutable SaveSnapshot:

1. encode header placeholder/directory/payload bytes or stream deterministically;
2. create unique pending file;
3. write the complete container;
4. verify every write returned expected byte count;
5. flush pending file through storage backend durability operation;
6. close or transition handle as required;
7. reopen pending file through the normal container reader;
8. validate header CRC, directory CRC, file bounds, Required sections and payload CRCs;
9. verify header SnapshotSequence/SaveKind/LogicalSlotId/ContentBuildId match the operation;
10. atomically rename pending file to its unique final `.sfgsave` name;
11. verify committed locator is discoverable/readable;
12. update/rebuild catalog/index cache;
13. only now emit `SaveCommitted`/`Save Complete`;
14. perform superseded-generation cleanup later/best effort.

## 6. Commit Point

The semantic storage commit point is the successful atomic rename into the committed `.sfgsave` namespace.

Before that point:

- previous saves remain authoritative catalog choices;
- UI cannot say Save Complete;
- pending file can be deleted/recovered as temporary debris.

After that point:

- the new file is a committed generation;
- catalog/index failure cannot uncommit it;
- cleanup failure cannot uncommit it.

## 7. Why Catalog Is Not Commit Authority

`catalog.sfindex` is rebuildable.

If the process crashes after the `.sfgsave` rename but before catalog update, next startup scans committed files and discovers the new generation by header/SnapshotSequence.

Therefore catalog corruption never rolls back or creates gameplay saves.

## 8. Post-Rename Verification

A rename success is followed by a lightweight existence/header verification before UI success.

If verification fails because storage reports an ambiguous state:

- operation returns a typed `CommitStateUnknown` diagnostic;
- next catalog rebuild determines whether the generation exists;
- prior known-good generation remains untouched.

The service never resolves ambiguity by overwriting/deleting both candidates.

## 9. Platform Durability

The initial Windows implementation must use storage primitives that provide the strongest practical local-file durability available without writing in-place.

`IPersistenceStorageBackend` reports capabilities so tests/diagnostics know whether:

- file data flush is supported;
- metadata/directory durability can be requested;
- same-directory rename is atomic.

If the selected filesystem cannot provide the required atomic visibility guarantee, StarForge refuses unsafe in-place replacement behavior and reports a persistence-storage capability failure.

## 10. Power-Loss Model

TA-12 guarantees application-level crash safety and uses platform durability calls to reduce power-loss risk.

It does not claim impossible absolute guarantees against hardware/firmware failure that violates acknowledged durable writes.

The architecture ensures at least that no valid previous generation is deliberately removed before a replacement is committed.

## 11. Startup Recovery Scan

At application startup or campaign catalog open, persistence scans for:

- committed `.sfgsave` files;
- pending files;
- superseded generations;
- quarantine entries;
- stale catalog index.

Recovery is storage-state reconstruction, not gameplay mutation.

## 12. Pending File Recovery

A pending file is never automatically treated as committed solely because its bytes look valid.

Canonical policy:

- if there is no recorded active operation after process restart, pending files are considered interrupted operations;
- validate them only for diagnostics/recovery tooling;
- by default delete or move them to quarantine after confirming a valid committed prior generation remains;
- they are not shown as ordinary selectable saves.

A future explicit recovery UI could offer import of a fully valid orphan pending file, but that is not baseline behavior.

## 13. Why Pending Files Are Not Auto-Promoted

Automatic promotion could commit a save that the user never received as completed and whose final rename/slot semantics were interrupted.

The baseline favors clear committed-generation semantics over speculative salvage.

## 14. Quarantine

Files with ambiguous/corrupt storage metadata can be moved into a `quarantine/` directory when safe.

Quarantine:

- preserves bytes for diagnostics/user support;
- removes the file from automatic logical-slot selection;
- never repairs or rewrites gameplay state;
- never replaces a valid committed generation.

Failure to move into quarantine does not justify deleting a potentially recoverable file.

## 15. Corrupt Committed Generation

If a committed generation later fails integrity validation:

- it is marked invalid in catalog;
- it is never partially loaded;
- another older valid generation in the same logical slot/history may be selected by the user or automatic convenience fallback only when the UI makes that fallback explicit;
- the corrupt file can be quarantined rather than silently deleted.

## 16. Manual Slot Recovery

If the newest generation for one Manual LogicalSaveSlotId is corrupt but an older generation remains valid:

- catalog marks newest invalid;
- older valid generation can be offered as `Previous valid generation`;
- the logical slot is not silently shown as if the old generation were the new one without indicating recovery/fallback.

## 17. Quick/Autosave Recovery

Quick/Autosave menus choose the newest **valid** committed generation after validation/preflight.

If the newest file is corrupt, UI can expose the next valid generation with an explicit warning that the latest save was unusable.

No corrupt file is merged with an older one.

## 18. Superseded Generation Cleanup

Cleanup is separate from save commit.

Rules:

- only files that are no longer retained by logical-slot/history policy are eligible;
- deletion order is oldest first;
- never delete the only valid generation of a logical manual slot as automatic cleanup;
- Quick/Autosave cleanup begins only after newest generation is committed;
- cleanup I/O failure is non-fatal to gameplay/save commit and is retried later/bounded.

## 19. Interrupted Cleanup

If the process crashes during cleanup, extra old generations can remain.

Next catalog rebuild deterministically classifies them as superseded/retained/cleanup candidates by header metadata and SnapshotSequence.

No user gameplay state changes because cleanup stopped midway.

## 20. Out-of-Space Failure

If storage fills during pending write:

- write fails;
- pending file is closed and removed/quarantined best effort;
- no committed generation is created;
- previous saves remain;
- UI reports Save Failed / insufficient storage when known.

Persistence never frees retained manual saves automatically to rescue the operation.

## 21. Permission/Read-Only Failure

If user-data storage becomes read-only or permission denied:

- gameplay can continue if safe;
- save request fails explicitly;
- repeated autosave spam is rate-limited by presentation/service policy while state remains visible;
- no fallback writes into the install/content directory.

## 22. Device Removal / I/O Fault

Storage backend converts OS-specific errors into typed persistence errors.

A device/removal fault does not trigger alternate gameplay snapshot capture or partial success.

The same captured snapshot may be retried to a valid target only through an explicit service retry operation that preserves its SnapshotSequence/operation identity rules, or the user can request a new save later.

## 23. Atomic Rename Failure

If final rename fails:

- pending file remains noncommitted;
- prior saves remain untouched;
- UI reports failure;
- recovery scan later cleans/quarantines pending debris.

Persistence never copies over an existing committed target as an unsafe fallback.

## 24. Fault Injection

The storage test backend supports deterministic failure injection at each step:

- create;
- write N;
- flush;
- close/reopen;
- validation read;
- rename;
- catalog update;
- cleanup delete.

Every failure point must prove prior committed generations remain valid/selectable.

## 25. Process-Crash Test Matrix

TA-14 must simulate/reconstruct process termination after each major write step:

- before pending creation;
- during header/directory/payload writes;
- after full write before flush;
- after flush before validation;
- after validation before rename;
- immediately after rename;
- during catalog update;
- during old-generation cleanup.

Expected catalog/recovery outcome is fixed for every point.

## 26. No Write-Back Repair During Load

Loading/migrating an old save does not overwrite the original file automatically.

If migration succeeds:

- session loads from staged migrated state;
- original save remains byte-for-byte intact;
- a future save writes a new current-version generation.

This preserves rollback/support evidence and prevents load-time crash from destroying source data.

## 27. Save File Permissions and Secrets

Saves contain local game state, not secrets requiring encryption in the baseline.

Storage permissions should follow normal per-user application-data defaults.

The project must not place credentials/tokens in save containers.

## 28. Diagnostics

Crash/recovery diagnostics include:

- operation ID;
- pending/final locator;
- last completed write stage;
- platform error code + normalized persistence error;
- bytes written;
- flush capability/result;
- rename result;
- post-rename discovery result;
- cleanup/quarantine action.

## 29. Tests

Tests verify:

- prior generation survives every injected pre-commit failure;
- committed file remains valid without catalog index;
- crash after rename discovers new generation;
- crash before rename does not select pending generation;
- old cleanup only after replacement commit;
- out-of-space cannot corrupt prior save;
- load migration never rewrites source save;
- ambiguous duplicate sequence is quarantined rather than timestamp-selected.

## 30. Explicit Non-Goals

No baseline:

- in-place save mutation;
- append journal replay;
- automatic corrupt-state repair;
- automatic pending-file promotion;
- delete-before-write slot rotation;
- cloud conflict resolution;
- encryption/authenticity signing.

## 31. Dependencies

Depends on TA-12 container/catalog/service architecture and later TA-13 operational limits/TA-14 fault-injection automation.

## 32. Open Questions

None.
