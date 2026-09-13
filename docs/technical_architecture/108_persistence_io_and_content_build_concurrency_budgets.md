# TA-13 — Persistence I/O and Content-Build Concurrency Budgets

> **Status:** Architecture Complete  
> **Authority:** Save snapshot memory/time, background encode/write limits, load/migration staging budgets, catalog scan behavior, profile persistence, content-build concurrency and memory policy

## 1. Purpose

TA-12 and TA-10 define correct persistence/content pipelines. TA-13 constrains their CPU, memory, I/O and concurrency so saving, loading and content cooking cannot destabilize gameplay or grow without bound.

## 2. Save Snapshot Capture Budget

At a Stable Save Boundary:

- snapshot capture main-thread p50 target: **<= 2.0 ms**;
- p95: **<= 4.0 ms**;
- p99: **<= 8.0 ms**;
- capture > **16.0 ms** is a performance regression requiring domain-level profiling.

The snapshot remains complete; data is never omitted to hit the timing target.

## 3. Immutable Snapshot Memory

One in-flight `SaveSnapshot` targets:

- ordinary immutable snapshot bytes: **<= 256 MiB**;
- soft red threshold: **512 MiB**;
- hard implementation safety cap: **1.0 GiB**.

A campaign whose required snapshot would exceed the hard cap fails a development/validation gate rather than truncating sections.

## 4. Save File Size Envelope

Container v1 target committed file size:

- ordinary: **<= 512 MiB**;
- warning: **> 1.0 GiB**;
- hard supported v1 safety cap: **2.0 GiB**.

The binary format uses explicit wide offsets, but TA-13 constrains initial implementation resource use. Raising the hard cap is an explicit architecture/performance change.

## 5. Save Concurrency

Per campaign:

- active encode/write generation: **1**;
- one coalesced pending periodic autosave request is allowed;
- a user-requested Manual/Quick Save retains its explicit completion/result semantics;
- no two workers write competing generations to the same logical slot concurrently.

## 6. Save Worker CPU

With codec `None`, save worker work primarily includes DTO encoding, CRC32C, file I/O and reopen validation.

Target:

- one primary persistence worker job chain;
- at most **2 general workers** consumed simultaneously by non-I/O save processing;
- background save work yields to RequiredStreaming/BlockingLoad priority according to TA-13 worker policy.

## 7. Background Save Commit Latency

On TA-16 reference storage/hardware for a <=256 MiB benchmark snapshot:

- p95 request-boundary-to-committed-file target: **<= 3 seconds**;
- <=512 MiB stress snapshot target: **<= 5 seconds**.

Gameplay can continue after immutable capture; UI shows `Saving` until atomic commit completes.

These are performance targets, not reasons to weaken flush/reopen/validation safety.

## 8. Pending-File Disk Space

A save commit can temporarily require both prior committed generation and new pending/new committed generation.

Before write, storage policy checks conservative free-space requirement:

```text
estimated new file bytes + 64 MiB safety margin
```

Failure produces typed insufficient-space result without deleting the prior valid save.

## 9. Load Staging Memory

Full load/migration can temporarily hold:

- encoded/decoded section buffers;
- migrated DTOs;
- staging DomainStores;
- activation plan;
- current live SessionRoot until swap.

Load-mode process memory soft target: **<= 6.0 GiB** on High reference configuration, with hard safety ceiling **8.0 GiB**.

If required staging cannot fit, load fails safely before replacing the session.

## 10. Load Parallelism

Container directory parsing and semantic activation remain deterministically orchestrated.

Independent section I/O/CRC/decode may use up to **4 workers** where implementation preserves deterministic diagnostics and bounded memory.

Domain migrations/validation that have explicit dependencies run in declared order rather than racing for semantic precedence.

## 11. Persistence Load-Time Targets

Excluding TA-10 scene asset loading/backend construction unrelated to save bytes:

- container read + CRC + decode + migration + domain validation for <=256 MiB save: **<= 2 seconds p95**;
- <=512 MiB stress save: **<= 4 seconds p95**;
- full time-to-playable with required scene/content target remains TA-13 streaming's **<= 10 seconds p95** reference benchmark.

Simulation Time remains stopped regardless of real duration.

## 12. Migration Memory

A migration should avoid retaining every historical intermediate representation simultaneously.

Per-section migration can pipeline old DTO -> next/current DTO while preserving cross-domain staging needs.

Migration-specific temporary memory target: **<= 512 MiB** beyond ordinary load staging.

## 13. CRC/Hash Work

CRC32C and content/save fingerprint work executes off the authoritative tick after immutable capture or during blocking load.

No per-frame gameplay path repeatedly hashes full save/content payloads.

## 14. Save Catalog Scale

Manual saves are storage-limited rather than a small authored slot count.

The catalog therefore supports paging/lazy enumeration:

- initial visible metadata batch: **256 generations**;
- metadata header/preflight processing target: **>= 5,000 generations/second** on reference SSD after OS cache warm-up;
- UI materializes only visible rows;
- catalog/index remains rebuildable cache.

Filesystem timestamps do not determine semantic order.

## 15. Autosave Rotation Cost

The rolling autosave count remains **10**. Cleanup executes after a new generation commits and is low-priority I/O.

Failure to delete an old superseded autosave creates storage cleanup debt/diagnostic, not failure of the already committed new save.

## 16. Profile Settings Budget

Profile/application settings document targets:

- ordinary size: **< 256 KiB**;
- hard safety cap: **4 MiB**;
- parse/apply target: **< 10 ms p95** on reference hardware.

A profile file is never allowed to consume arbitrary memory through corrupt counts/strings.

## 17. Save Inspector Budget

Developer inspection defaults to metadata/directory parsing without decoding all sections.

Full decode/validation is an explicit operation and follows ordinary load memory limits. The inspector does not mmap/allocate a corrupt declared size before bounds validation.

## 18. Content Build Worker Count

Offline `starforge-content` baseline:

```text
BuildWorkers = clamp(HardwareConcurrency - 1, 1, 16)
```

Worker count affects build duration only. Artifact ordering/fingerprints remain deterministic.

## 19. Content Build RAM Cap

The content tool must respect:

```text
BuildMemoryCap = min(70% of detected physical RAM, 12 GiB)
```

with a minimum viable cap of **2 GiB** for supported development operation.

If a build graph cannot execute within the configured cap, jobs serialize/defer; the tool does not exceed memory unchecked.

## 20. Content Job Memory Reservation

Heavy cook tasks declare estimated scratch/resident bytes before dispatch where feasible.

The scheduler only dispatches a set whose reservations fit `BuildMemoryCap`, allowing large texture/mesh/nav jobs to serialize instead of causing system-wide thrashing.

## 21. Content Build I/O

Offline build permits up to:

- **16 concurrent small metadata/source reads**;
- **8 heavy source read/decode tasks**;
- output publication remains atomic per artifact/registry according to TA-10.

Storage completion order cannot affect ContentRegistry order.

## 22. Incremental Build Targets

On TA-16 development reference machine after initial cache:

- no-op validation/build graph scan target: **<= 2 seconds** for baseline project scale;
- single ordinary definition/material change to validated cooked publication target: **<= 5 seconds**;
- single ordinary texture/mesh recook target: **<= 10 seconds**, excluding intentionally large source assets.

Full clean build duration is measured but not architecture-gated until representative content volume exists.

## 23. Content Build Artifact Size Diagnostics

Individual cooked product warning thresholds:

- ordinary texture product > **64 MiB**;
- ordinary mesh product > **128 MiB**;
- ordinary stream-cell aggregate unique payload > TA-13 cell targets;
- any single cooked asset > **512 MiB** requires explicit review.

Warnings do not change ContentId or automatically downsample gameplay-important content.

## 24. Hot Reload Concurrency

Development hot reload:

- at most **8 cook candidates** concurrently published/validated;
- publication to runtime ContentGeneration remains ordered/atomic;
- repeated edits coalesce obsolete candidate generations;
- failed candidate keeps prior valid content.

## 25. I/O Priority Interaction

Runtime RequiredStreaming and blocking load I/O outrank background save cleanup/diagnostics.

Save commit writes in progress receive fairness and are not starved forever, but speculative prefetch cannot indefinitely block a user-visible save commit.

## 26. Fault Behavior

Performance pressure may slow save/load/build work but cannot:

- skip a save section;
- omit migration;
- bypass CRC/content validation;
- partially activate load;
- publish partially cooked registry;
- use filesystem timestamp to pick authority.

## 27. Metrics

Persistence/content telemetry records:

- snapshot capture bytes/time;
- section bytes/encode/CRC/write times;
- save commit latency;
- pending/free-space state;
- load read/CRC/decode/migrate/validate times;
- staging memory peak;
- catalog scan throughput;
- content workers/queue/memory reservations;
- cache hits/misses;
- cook duration/artifact bytes;
- hot-reload generations/stale rejects.

## 28. Non-Goals

No save compression requirement, no parallel semantic migration race, no unlimited manual-save UI materialization, no content-build worker count as determinism, no persistence shortcuts for frame rate.

## 29. Open Questions

None in the TA-13 persistence/content-build budget architecture.