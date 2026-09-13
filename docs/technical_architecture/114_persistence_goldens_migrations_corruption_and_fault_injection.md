# TA-14 — Persistence Goldens, Migrations, Corruption, and Fault Injection

> **Status:** Architecture Complete  
> **Authority:** Save-format golden fixtures, migration compatibility suites, corruption matrices, crash/fault injection, catalog recovery tests, profile persistence tests, and exactly-once persistence verification

## 1. Purpose

TA-12 persistence is only trustworthy if binary format, migrations, recovery, crash safety, content compatibility, and staged load behavior are verified against stable fixtures and controlled failures.

## 2. Golden Fixture Families

The repository/test assets maintain bounded fixture families for:

- current valid minimal save;
- current representative populated save;
- historical supported application/container/domain versions;
- active mission save;
- active Horizon Defense save;
- active/local-context continuation save;
- profile/settings file;
- catalog/index examples;
- intentionally corrupt variants.

## 3. Exact V1 Container Golden

At least one minimal v1 `.sfgsave` fixture is byte-exact golden evidence for:

- `SFGSAVE\0` magic;
- exact 128-byte header offsets;
- 64-byte section entries;
- little-endian encoding;
- section order;
- 8-byte payload alignment;
- CRC32C fields;
- ContentBuildId field placement;
- codec None baseline.

Unexpected byte drift fails.

## 4. Semantic Round Trip

Every current SectionKind codec requires tests for:

```text
current DTO
-> encode
-> decode
-> current DTO
```

The semantic result must be equivalent and deterministic.

## 5. Whole-Snapshot Round Trip

Representative SessionRoot fixtures export a SaveSnapshot, encode/write/read/decode/stage and compare canonical authoritative state after reconstruction.

Runtime/backend handles are expected to differ and are excluded from semantic identity.

## 6. Deterministic Encoding

Given identical current DTO state and schema/tool version, section bytes and container layout are deterministic.

Encoding must not depend on unordered-container iteration, runtime handle values, worker order, filesystem timestamps or wall time except explicit metadata fields where allowed.

## 7. Historical Migration Chain

For every supported historical version boundary, tests include:

- direct fixture at old version;
- explicit migration step(s);
- current DTO result;
- invariant validation;
- source fixture remains unchanged.

Skipping unsupported intermediate semantics is prohibited unless a reviewed direct migration is explicitly defined.

## 8. Migration Identity Preservation

Migration tests assert Persistent IDs remain unchanged unless a formal migration split/conversion creates new records using the deterministic allocation rule.

Exactly-once markers, MissionInstance history, rewards and ownership cannot reset merely because schema changed.

## 9. Content Rename Compatibility

Fixtures cover:

- ContentBuildId differs but all ContentIds still resolve -> load succeeds;
- explicit old ContentId -> new ContentId migration -> succeeds and preserves entity/item identity;
- required ContentId removed without migration -> load rejects;
- fuzzy/path-based accidental alias -> rejected.

## 10. Missing Optional Section

Optional SaveMetadata absence/corruption follows its defined optional behavior without allowing missing required gameplay sections.

## 11. Required Section Matrix

For each required SectionKind, at least one corruption/omission test proves load fails before live SessionRoot mutation.

## 12. CRC Corruption Matrix

Tests mutate independently:

- header bytes;
- header CRC;
- directory bytes;
- directory CRC;
- payload bytes;
- payload CRC;
- section bounds/offset;
- duplicate SectionKind;
- impossible payload size/alignment.

The reader reports deterministic primary rejection classification.

## 13. Truncation Matrix

Fixtures truncate the save at representative boundaries:

- inside header;
- inside directory;
- between sections;
- inside payload;
- before final payload.

No truncated file partially loads.

## 14. Unknown/Newer Versions

Tests verify unsupported newer container/application/domain versions are rejected without rewriting the source file.

## 15. Size / Count Safety

Malicious/corrupt declared sizes/counts beyond TA-12/TA-13 safety limits fail before unbounded allocation.

Integer overflow/offset-wrap cases are explicit negative fixtures.

## 16. Staged Load Isolation

A failure at each major load stage is injected and asserts the existing live SessionRoot remains unchanged:

```text
container read
migration
ContentId resolution
domain import
domain validation
cross-domain validation
runtime activation planning
```

## 17. Session Swap Exactly Once

Successful load tests instrument application ownership and prove there is one SessionRoot replacement only after staging reaches ReadyToActivate.

## 18. Resume Tick

Golden/runtime tests prove:

```text
saved tick = N
first ordinary post-load tick = N + 1
```

No due event from tick N is applied twice.

## 19. No Load-Time Progression

Injected artificial worker/storage delays during load must not change:

- SimulationTick;
- WorkOrder progress;
- statuses;
- Recovery Transit;
- Dynamic Events;
- market/timer state;
- fuel/ammo.

## 20. Crash-Safe Write Fault Points

The persistence writer exposes test-only deterministic fault points around:

1. pending-file create;
2. partial header write;
3. directory write;
4. payload write;
5. flush;
6. reopen/validate;
7. before atomic rename;
8. after atomic rename but before catalog update;
9. during post-commit cleanup.

## 21. Pre-Commit Failure Rule

For every injected failure before atomic committed rename:

- prior committed generation remains valid;
- new generation is not selected as committed;
- pending debris is recoverable/quarantinable;
- no old valid generation is deleted.

## 22. Post-Commit Failure Rule

Failure after atomic rename but before catalog/index update must still allow startup scan to discover the valid newer SnapshotSequence.

Catalog is proven rebuildable and non-authoritative.

## 23. Autosave Rotation

Tests create >10 autosaves and verify:

- newest 10 valid committed generations remain logically retained;
- cleanup occurs only after new commit;
- crash during 11th write cannot reduce history below prior valid set;
- SnapshotSequence, not filesystem timestamp, determines ordering.

## 24. Quick Save Supersession

Repeated Quick Save creates immutable generations; tests prove old file is not overwritten in place before new generation commits.

## 25. Manual Slot Supersession

Manual logical slot replacement follows the same immutable-generation rule.

The UI/catalog can show one logical slot while storage history remains crash-safe until cleanup policy applies.

## 26. Catalog Recovery

Tests cover:

- missing catalog;
- corrupt catalog;
- stale catalog missing newest committed generation;
- catalog referencing missing generation;
- orphan pending file.

Filesystem scan + header validation reconstructs safe catalog state without guessing gameplay data.

## 27. Quarantine

Corrupt/unrecognized files can be quarantined by tooling, but quarantine tests prove valid files are not modified and no corrupt file is silently repaired into playable state.

## 28. No Heuristic Gameplay Repair

Negative tests ensure inspector/recovery code cannot invent:

- resources;
- IDs;
- objective completion;
- owner mappings;
- missing ContentIds;
- RNG state.

## 29. Active Lease Save

Saving an active persistent actor tests that leased runtime facets and persistent facets export into exactly one logical record without duplication.

## 30. Extraction / Ownership Boundary Save

A save request injected during an indivisible extraction/resource transaction waits until the stable boundary and captures only pre- or post-commit coherent ownership according to timing—never both.

## 31. Finale Save / Load

After FinalResolution commit, save/load tests prove no second ending choice becomes available and all cross-domain consequences remain committed.

## 32. RNG Continuation

Save/load tests compare deterministic RNG streams before/after and verify future procedural outcome is unchanged by reload.

## 33. Profile Persistence

Profile tests differ intentionally from world saves:

- valid settings round trip;
- missing field defaults;
- invalid bounded field clamps/defaults according to schema;
- corrupt active profile falls back to prior valid/default policy;
- campaign save remains usable;
- unsupported newer profile is not silently overwritten.

## 34. Display Confirmation Persistence

A risky display setting preview is persisted only after confirmation. Timeout/revert tests ensure invalid preview does not become durable configuration.

## 35. Golden Update Review

Save goldens can change only with an intentional container/schema/migration decision.

A current writer change that breaks old goldens without corresponding migration/version architecture is a test failure, not a reason to regenerate fixtures automatically.

## 36. Fuzz Boundary

TA-14 supports parser fuzzing architecture for save/container/profile decoders.

Fuzzing inputs are untrusted bytes; success criteria are:

- no crash/UB/out-of-bounds/unbounded allocation;
- valid files still decode;
- invalid files return typed rejection.

The exact fuzz engine/tool invocation is TA-16 implementation/toolchain data.

## 37. Fixture Retention

Historical golden fixtures required for supported migration compatibility remain version controlled or otherwise immutably retained under repository policy.

They are not deleted merely to make the suite smaller.

## 38. CI Partitioning

PR gate:

- current round trips;
- key historical migrations;
- core corruption matrix;
- write fault smoke.

Scheduled/release gate:

- full historical migration matrix;
- extended corruption/fuzz corpus;
- long fault sequence and autosave recovery suites.

## 39. Explicit Non-Goals

TA-14 does not:

- promise recovery of arbitrary corrupted gameplay state;
- allow tests to modify historical source fixtures in place;
- use filesystem timestamps as golden authority;
- require byte-identical runtime handles after load.

## 40. Dependencies

Depends on TA-2 save DTO/migration rules, TA-9 exactly-once strategic state, TA-10 content compatibility, TA-12 concrete persistence implementation architecture and TA-13 memory/I/O safety budgets.

## 41. Open Questions

None within persistence verification scope.