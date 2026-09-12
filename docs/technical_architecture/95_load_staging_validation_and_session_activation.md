# TA-12 — Load Staging, Validation, and Session Activation

> **Status:** Architecture Complete  
> **Authority:** Deterministic save-load pipeline, staged decode/import, validation order, compatibility preflight, active-context preparation, live-session replacement, runtime reconstruction, and failure semantics

## 1. Purpose

Loading is a potentially destructive operation only if implemented incorrectly. StarForge therefore treats the selected save as untrusted input and constructs an entirely separate staged session before the live session can change.

The canonical rule is:

> decode -> migrate -> import -> validate -> prepare -> **one session-root swap**.

## 2. Load Input

A normal load targets one concrete committed `.sfgsave` generation selected by the SaveCatalog.

The load operation never combines:

- sections from different save files;
- newest domain sections from multiple generations;
- current live-session inventory with loaded progression;
- catalog/index cache state with gameplay state.

## 3. Load Pipeline

Canonical stages:

1. `OpenFile`;
2. `ValidateContainerHeader`;
3. `ValidateDirectory`;
4. `ReadAndCRCSections`;
5. `DecodeHistoricalDTOs`;
6. `MigrateToCurrentDTOs`;
7. `ResolveContentCompatibility`;
8. `ImportDomainStagingStores`;
9. `ValidateDomainLocalInvariants`;
10. `ValidateIdentityAndAllocatorIntegrity`;
11. `ValidateCrossDomainReferences`;
12. `ValidateOwnershipAndExactlyOnceIntegrity`;
13. `ValidateTimerRNGAndEventIntegrity`;
14. `ValidateActiveContextContinuation`;
15. `PrepareRuntimeActivationPlan`;
16. `FinalStagingConsistencyGate`;
17. `ReplaceLiveSessionRoot`;
18. `InstantiateRuntimeBackendsAndActiveScene`;
19. `PublishInitialReadModels`;
20. `ResumeFromSavedSimulationTick`.

Each stage returns a typed result and does not silently continue after a Required failure.

## 4. No Live Mutation Before Stage 17

Stages 1–16 operate on:

- immutable file bytes;
- historical/current DTOs;
- migration workspace;
- staged DomainStores;
- staged content-resolution tables;
- staged activation descriptors.

They never mutate the live SessionRoot.

## 5. Container Preflight

Before payload allocation, validation uses TA-12 container rules:

- magic/version/header size;
- endian marker;
- header CRC;
- physical file size;
- section count/directory bounds;
- directory CRC;
- SectionKind order/uniqueness;
- payload interval bounds/non-overlap;
- codec support;
- Required section presence.

Failure here is `ContainerInvalid`/`UnsupportedContainer`, not a domain error.

## 6. Application Compatibility Gate

`application_save_compat_version` is evaluated after safe container parsing.

Policy:

- same supported compatibility generation -> continue;
- older supported generation -> migration path required;
- newer unsupported generation -> reject clearly;
- unknown/incompatible lineage -> reject; do not guess by application marketing version string.

The compatibility version is not a substitute for per-domain SchemaVersion migration.

## 7. Section Read/CRC

Required sections are read and decoded independently into immutable byte buffers or bounded streams.

For each:

- codec decode succeeds;
- decoded size matches directory;
- CRC32C matches decoded bytes;
- payload prologue kind/schema echoes match directory.

No semantic decoder runs on corrupt bytes.

## 8. Decode Order Does Not Create Semantic Order

Sections can be physically decoded in stable SectionKind order, but cross-domain reference resolution waits until all Required sections have decoded/migrated.

A Player record can reference a ShipId whose Spacecraft section has not yet been semantically validated without making file order authoritative.

## 9. Historical DTO Decode

Stored SchemaVersion selects exactly one supported historical decoder.

Decoder output is a historical-version DTO representation, not the current DomainStore.

Unsupported historical version fails with:

- SectionKind;
- stored version;
- current version;
- supported migration range.

## 10. Migration Stage

Migration is completed before current-store import unless a documented cross-domain migration requires coordinated current DTO workspace.

Migration obeys `96_save_migration_and_content_compatibility.md` and never uses live world state, current wall clock, fresh RNG, or worker completion order.

## 11. Content Compatibility Preflight

The header ContentBuildId is compared with the currently loaded TA-10 registry.

If equal, normal ContentId resolution still validates required references.

If different:

- load is **not** rejected solely because the hash differs;
- saved required ContentIds are resolved against the current registry/migration map;
- removed/renamed content follows explicit migration;
- missing required content without migration fails;
- presentation-only optional fallback may degrade only where the owning content contract permits it.

## 12. Staging SessionRoot

Import constructs one isolated staged SessionRoot containing the same conceptual persistent stores as a live session.

Staging stores:

- accept only current-version DTOs;
- allocate no random replacement IDs;
- do not run ordinary gameplay ticks;
- do not emit player-facing gameplay events;
- do not create OpenGL/Jolt/miniaudio backend resources.

## 13. Domain-Local Validation

Each owning domain validates its own invariants before cross-domain validation.

Examples:

- Health/state enum ranges;
- station graph internal record consistency;
- robot configuration structural validity;
- mission objective DAG validity/state coherence;
- market quantities/prices representation validity;
- no invalid internal duplicate IDs.

Persistence orchestrates but does not redefine these semantics.

## 14. Persistent ID Validation

For every typed ID allocator/domain:

- real record ID != 0;
- no duplicate ID;
- typed reference representation is valid;
- saved allocator `NextValue` exceeds every committed ID it owns;
- migration-created IDs were allocated deterministically and reflected in allocator state.

`RuntimeEntityHandle` must not exist in save DTOs.

## 15. Cross-Domain Reference Validation

After all stores exist, validation resolves:

- Strong Required references -> target must exist and type match;
- Optional references -> null/absence allowed per schema;
- Historical references -> referenced tombstone/history record contract must pass;
- owner/source/target relationships -> allowed owner kinds only.

Missing Strong references fail load. They are never repaired by picking a similarly named record.

## 16. Physical Ownership Validation

Before activation, Inventory/Resource authority plus relevant participant domains prove:

- each Unique Item has exactly one physical owner;
- stack quantities are non-negative/in-range;
- reservations/WIP/in-transfer ownership do not duplicate source holdings;
- mission security state matches owner/location;
- raid theft/extraction state matches actual transferred owner;
- consumed/destroyed quantities do not coexist as inventory.

Any violation rejects the staged session.

## 17. Credits and Transaction Integrity

Validation checks:

- Credits representation/ledger invariants;
- persisted one-time transaction/idempotency markers;
- reward completion state paired with committed reward ownership/ledger results;
- trade transaction end states are internally complete;
- next TransactionId is beyond required persisted markers.

Persistence never reruns a one-time reward to "repair" mismatched state.

## 18. Timers and Simulation Time

Persistent timers validate against the saved SimulationTick.

Rules:

- no gameplay timer is reconstructed from wall-clock elapsed time since file creation;
- due/remaining tick arithmetic is overflow checked;
- owner reference/state compatibility must pass;
- True Pause/Active Game Time presentation metadata does not alter Simulation Time.

## 19. RNG Integrity

Persistent PCG32/scoped RNG state validates:

- recognized StreamKind;
- supported GenerationAlgorithmVersion;
- odd increment/valid representation;
- owner/scope identity;
- no missing cursor for a system whose future deterministic sequence requires it.

Loading the same snapshot cannot reseed from current time.

## 20. Mission/Raid/Event Integrity

TA-9 state validates at least:

- at most one external Deployed MissionInstance;
- MissionId vs MissionInstanceId relationships;
- objective graph/state consistency;
- reinforcement committed/reservation semantics;
- DefenseEvent identity/state;
- DynamicEvent handoff links;
- finale Postgame resolution exactly-once state;
- Recovery Transit causal destination/timing.

## 21. Active Local Context Validation

The staged save identifies exactly one valid player-local context state.

Validation checks:

- strategic/current location exists;
- local context kind matches mission/Horizon/flight/recovery state;
- required persistent player/ship/mission IDs exist;
- `LocalContextContinuation` belongs to that context and saved tick;
- transient continuation records reference valid ContentIds/owners;
- no stale SceneGeneration/RuntimeEntityHandle is serialized as authority.

## 22. Runtime Activation Plan

Before replacing the live session, staging creates an immutable `RuntimeActivationPlan` describing what must be reconstructed after swap:

- target Scene/Zone/Location content;
- persistent actors to activate;
- activation descriptors/facets;
- local continuation objects;
- physics/nav/render/audio resource requirements;
- required content readiness set;
- saved spatial/origin-independent authoritative state.

The plan contains persistent IDs/ContentIds, never old backend handles.

## 23. Required Content Readiness

TA-10 required content for the activation plan must resolve before the live root is swapped unless the existing application transition can guarantee a post-swap Hard Streaming Hold without losing the old session on content failure.

Baseline conservative policy:

> all Required activation content is resolved/validated during staging; expensive GPU/Jolt instantiation occurs after swap from already validated inputs.

Missing required asset => load failure before session replacement.

## 24. Final Staging Consistency Gate

Immediately before swap, persistence rechecks immutable staging result tokens:

- all Required validation stages PASS;
- content registry generation used for staging is still current;
- load operation was not superseded/cancelled;
- target save locator still refers to the validated bytes or immutable open-file identity;
- runtime activation plan matches staged session revision/generation.

If content hot reload invalidates assumptions, staging is rebuilt/revalidated rather than partially activated.

## 25. SessionRoot Swap

The application performs one ownership transfer:

```text
live_root = move(staged_root)
```

conceptually at a controlled application boundary.

After the swap, the old session is no longer gameplay authority.

No individual DomainStore swaps independently.

## 26. Backend Reconstruction

After session replacement:

- TA-3 constructs the active SceneInstance;
- TA-7 allocates new RuntimeEntityHandles;
- TA-5 reconstructs Jolt bodies/constraints;
- TA-4 reconstructs render proxies/resources;
- TA-8 reconstructs navigation/query runtime state;
- TA-11 reconstructs audio/UI/presentation from committed state/read models.

Backend generation IDs are new process/runtime state and are not expected to match the saved session.

## 27. Floating Origin on Load

Persistent local positions remain TA-3 double-precision Context Space state.

Runtime `RuntimeOrigin64` is chosen/reconstructed according to current activation policy. A saved runtime origin is not required as gameplay identity.

Rebasing/initial origin choice must not synthesize impacts, velocity, trigger, or gameplay changes.

## 28. First Simulation Tick After Load

Before ordinary gameplay resumes:

1. staged persistent state is live;
2. active runtime/physics proxies exist;
3. read models/presentation initial state are published;
4. no committed one-shot event is replayed merely because UI/audio reconstructed;
5. accumulator starts from a clean application scheduling state;
6. next authoritative tick is `saved_simulation_tick + 1` unless the saved state contract explicitly represents work due at the saved tick and the scheduler defines otherwise.

The implementation uses one documented tick-resume rule consistently across all timers.

## 29. Presentation Reconstruction

UI/audio/VFX rebuild persistent presentation from current state.

They do **not** replay historical transient events such as:

- old weapon-fire sound;
- already-shown pickup toast;
- past hit flash;
- prior save-success chime.

Persistent alarms/current incidents reappear because their owning state remains active.

## 30. Failure After Session Swap

The architecture minimizes fallible work after the swap.

If backend reconstruction encounters an unexpected internal failure after validated inputs were accepted:

- session enters a controlled load/technical-failure state;
- it must not resume partial gameplay;
- diagnostics preserve failure stage;
- the source save remains untouched;
- application can return to menu/retry load rather than heuristically mutating state.

TA-15 later audits these cross-subsystem failure paths.

## 31. User Cancellation

Load cancellation is permitted during pre-activation I/O/migration/staging if operations can stop safely.

Once SessionRoot replacement begins, cancellation is disabled until the controlled transition reaches either Activated or technical failure state.

## 32. Diagnostics

Load diagnostics expose:

- selected file/SnapshotSequence;
- container/app/domain versions;
- ContentBuildId saved/current;
- per-section CRC/decode status;
- migration chain;
- content remaps;
- domain/cross-domain validation stage;
- offending typed IDs/ContentIds;
- activation-plan content set;
- final swap/activation status.

## 33. Tests

Tests must cover:

- corruption rejected before semantic decode;
- old supported schema migrates then imports;
- newer unsupported version leaves live session unchanged;
- missing Strong ID fails staging;
- duplicate ownership fails staging;
- differing but compatible ContentBuildId loads;
- missing required ContentId fails before root swap;
- active mission save reconstructs equivalent consequential continuation;
- backend handles are newly allocated after load;
- no old presentation one-shot events replay;
- current active alarms reconstruct;
- failed staging never mutates live session.

## 34. Explicit Non-Goals

No baseline:

- partial-domain load;
- cross-file section merge;
- corrupt-save auto repair;
- save file rewrite during migration;
- runtime-handle preservation;
- wall-clock catch-up after load;
- renderer/audio/UI as load validation authority.

## 35. Dependencies

Depends on TA-2 load/migration/integrity contracts, TA-3/7 activation, TA-9 strategic state, TA-10 registry/content compatibility, and the other TA-12 persistence specs.

## 36. Open Questions

None.
