# Deterministic RNG, Migration, and Integrity Contracts

> **Status:** Architecture Complete  
> **Authority:** Deterministic random stream algorithm, scoped stream derivation, persistent RNG state, generation-version contracts, domain/schema migration, cross-domain validation, and load integrity

## 1. Purpose

Procedural missions, events, loot, AI/content variation, and save/load must not depend on wall-clock time, thread races, library-specific random distributions, or changing implementation details.

This document defines the deterministic RNG and migration/integrity architecture used by persistent systems.

## 2. RNG Algorithm

StarForge uses a project-owned implementation of **PCG32** as the baseline deterministic pseudo-random generator.

The serialized generator state consists of:

- `State:uint64`;
- `Increment:uint64`, always odd.

The output primitive is one deterministic `uint32` sequence defined by the project's locked PCG32 implementation/tests.

## 3. Why Project-Owned RNG

Authoritative procedural behavior must remain stable across:

- operating systems;
- compiler/STL versions;
- save/load;
- render FPS;
- worker scheduling.

The architecture therefore does not use implementation-dependent standard-library distribution behavior as authoritative procedural logic.

## 4. RNG Primitive API

The authoritative RNG primitive exposes only a small fixed set of deterministic operations:

- `NextU32()`;
- `NextU64()` composed from defined U32 calls;
- `UniformBoundedU32(bound)` using rejection sampling;
- `Uniform01Float()` using a project-defined bit/scale conversion;
- `Uniform01Double()` where required;
- deterministic shuffle using project-defined Fisher-Yates + bounded integer sampling.

Gameplay code does not directly use `std::uniform_*_distribution` for persistent authoritative outcomes.

## 5. Root Save Seed

Every new save/session timeline receives a non-zero 64-bit `SaveSeed`.

SaveSeed is generated at new-game creation through platform entropy or an explicit deterministic development/test seed.

Once created, SaveSeed is persisted and never regenerated on load.

Wall-clock time may contribute to entropy at new-game creation, but is never consulted again for authoritative random outcomes in that timeline.

## 6. Scoped Stream Derivation

Named/scoped streams are derived deterministically from:

- SaveSeed;
- stable StreamKind identifier;
- owning Persistent ID or stable scope key;
- GenerationAlgorithmVersion where applicable.

Derivation uses a project-owned fixed 64-bit mixing function based on **SplitMix64 finalization/sequence rules** documented in code/tests.

Stream derivation never uses `std::hash` because its implementation/stability is not a persistence contract.

## 7. StreamKind

Every persistent procedural consumer has a statically defined `StreamKind`/domain tag.

Examples:

- MissionGeneration;
- DynamicEventGeneration;
- RaidTargetGeneration;
- LootGeneration;
- EncounterComposition;
- FactionMarketVariation.

Human-readable string names can exist for diagnostics, but serialized identity uses stable explicit numeric enum values.

## 8. Independent Streams

Different systems do not consume one giant global random sequence.

A mission's layout RNG cannot change merely because an unrelated market calculation consumed random numbers first.

Scoped streams isolate deterministic domains.

## 9. Persistent Stream State

A stream must serialize its current PCG32 state when future authoritative outcomes depend on continuing the same sequential stream after save/load.

Examples:

- a persistent Dynamic Event whose future phases consume additional random choices;
- a long-lived procedural strategic actor/state machine if its contract requires sequential randomness.

## 10. Seed-Only Persistent Generation

If procedural generation is completed atomically and all resulting authoritative content/state is persisted, the owning record can store:

- original seed/scope;
- GenerationAlgorithmVersion;
- generated committed state.

It does not need to retain a live RNG cursor unless future generation continues from that stream.

## 11. No Reload Reroll

Loading restores:

- committed generated state; and
- required stream cursor state.

The same save snapshot cannot receive different major procedural outcomes merely because it was loaded again.

## 12. Worker RNG

A worker may perform procedural candidate generation only from an explicitly assigned immutable RNG input:

- seed/stream state;
- stream kind;
- generation version;
- candidate ordinal/range.

Workers never race on one shared mutable authoritative generator.

## 13. Candidate Selection

If multiple workers produce candidates, authoritative selection order is deterministic and independent of completion order.

Selection uses an explicit ordinal/score/tie-breaker rule on the simulation thread.

The first worker to finish never wins simply because it finished first.

## 14. Algorithm Version

Every persistent generated-content family has a `GenerationAlgorithmVersion`.

This version is separate from save Domain SchemaVersion.

Reason:

- DTO layout can remain unchanged while generation logic changes;
- old persistent content must not silently regenerate differently.

## 15. Existing Generated Content

After generation has committed, existing generated content is normally restored from its persisted state rather than regenerated from current algorithms.

The stored seed/version remains useful for:

- validation;
- deterministic deferred portions;
- diagnostics;
- migration where explicitly designed.

## 16. New Content After Game Update

A newer executable can use a newer GenerationAlgorithmVersion for newly created missions/events while preserving old committed records under their stored version/state.

A migration must explicitly define any intended conversion of old procedural state.

## 17. Deterministic Float Caveat

Persistent RNG output itself is integer-defined.

When random floats are required, project conversion code is fixed/tested.

The architecture does not assume that arbitrary floating-point simulation across all platforms becomes bit-identical solely because RNG is deterministic.

TA-13 later defines broader determinism/performance constraints.

## 18. Migration Layers

Save migration has three layers:

1. **Container Migration** — only when save container structure changes;
2. **Domain DTO Migration** — SchemaVersion N -> N+1 within an owning domain;
3. **Cross-Domain Migration/Reconciliation** — explicit staged migration when a schema change affects references/ownership across domains.

## 19. Domain Migration Rule

Each supported old Domain SchemaVersion has an explicit deterministic migration chain.

Preferred pattern:

`V1 -> V2 -> V3 -> Current`

rather than one unmaintainable function for every old-to-current pair.

## 20. Migration Purity

A migration uses only:

- decoded old save data;
- explicit migration constants/tables;
- current compatible ContentId migration maps where specified.

It must not depend on:

- wall clock;
- player input;
- random fresh entropy;
- network/web service;
- current frame timing.

## 21. Migration Atomicity

Migration operates on staging DTO/state.

The active live session is never partly migrated.

If migration fails:

- staged state is discarded;
- original save file remains unchanged;
- current live session remains unchanged.

## 22. No Silent Data Loss

Removing/renaming a field/content definition requires an explicit migration decision.

Migration does not silently drop:

- persistent identity;
- owned physical resources/items;
- committed knowledge;
- completed story state;
- crew/ship/robot assets;
- transaction markers

unless a formally accepted design/migration rule specifies the semantic replacement.

## 23. ContentId Migration

Renaming a ContentId uses a versioned migration map such as:

`old.id -> new.id`.

The mapping is explicit and tested.

Missing required content without a valid migration path fails load.

## 24. Persistent ID Preservation

Migrations preserve Persistent IDs whenever the logical entity remains the same.

They do not assign fresh IDs merely because the schema changed.

If one old entity must split into several new persistent records, migration defines exactly:

- which retains old ID;
- how new IDs are allocated deterministically;
- how allocator state advances;
- how references are rewritten.

## 25. Deterministic Migration ID Allocation

New IDs created by migration use the staged saved allocator state in a fixed ordering.

Iteration order is schema-defined and deterministic.

No hash-container order may affect newly allocated migration IDs.

## 26. Transaction Marker Preservation

Persisted TransactionIds used for idempotency remain preserved/mapped so migration cannot accidentally allow a one-time reward/action to execute again.

## 27. Integrity Validation Stages

Load validation executes in layers:

1. binary/container bounds;
2. CRC/section integrity;
3. schema decode validity;
4. domain-local invariants;
5. typed Persistent ID uniqueness;
6. allocator consistency;
7. cross-domain Strong reference resolution;
8. physical ownership conservation/invariants;
9. transaction/idempotency invariants;
10. timer/RNG validity;
11. ContentId resolution;
12. Active Local Context consistency;
13. domain-specific project invariants.

Session activation occurs only after all required stages pass.

## 28. Persistent ID Integrity

For every persistent ID type:

- `0` cannot identify a real record;
- no duplicate ID exists in its domain;
- allocator `NextValue` must be greater than every committed allocated raw ID in that allocator's timeline;
- required references must point to correct ID type/domain.

## 29. Ownership Integrity

Physical ownership checks include:

- UniqueItemId appears under exactly one current physical owner;
- stack quantities are non-negative and within representation bounds;
- WIP/reservations do not duplicate source inventory;
- extracted/raid cargo security state matches actual owner/location semantics;
- destroyed/consumed quantities are not simultaneously present elsewhere.

Exact resource-domain invariants are implemented by owning domains.

## 30. Timer Integrity

Persistent timers validate:

- owner reference;
- timer kind;
- due SimulationTick/remaining duration range;
- state compatibility;
- transaction marker where required.

No timer is reconstructed from wall-clock deadline.

## 31. RNG Integrity

Persistent RNG streams validate:

- recognized StreamKind;
- supported GenerationAlgorithmVersion;
- non-invalid PCG state representation;
- odd increment;
- owning persistent scope validity.

Unsupported generation version required for unfinished future generation fails compatibility unless migrated.

## 32. Cross-Domain Validation Ownership

`persistence` orchestrates validation order, but gameplay domains own semantic validators.

A generic persistence layer does not decide what a valid robot, mission, station topology, or Research state means.

## 33. Repair Policy

Automatic repair of corrupt saves is not a baseline behavior.

Only deterministic, explicitly versioned migrations/recovery transforms are allowed.

If current-state invariants fail outside such a rule, load fails and the user can select another save.

## 34. Development Validation

Development builds can run stronger/expensive invariant scans at:

- session creation;
- save capture;
- after load/migration;
- selected transaction commits;
- explicit debug command.

Shipping builds keep required safety/integrity checks without necessarily running every expensive diagnostic continuously.

## 35. Golden RNG Tests

The repository must contain locked test vectors for:

- PCG32 initial state -> first N outputs;
- SplitMix64-based scope derivation;
- bounded integer sampling;
- float conversion;
- deterministic shuffle;
- stream serialization/resume.

Changing these outputs is an architecture compatibility change, not a casual refactor.

## 36. Golden Migration Tests

Every supported old schema fixture must have a migration test asserting:

- expected persistent IDs;
- ownership quantities;
- transaction markers;
- ContentId mapping;
- timers;
- generated-content versions;
- domain invariants.

## 37. Diagnostics

Load/migration diagnostics report:

- section/schema version;
- migration steps executed;
- generation versions encountered;
- unresolved references/content IDs;
- duplicate IDs;
- allocator violations;
- ownership invariant failure;
- RNG stream scope/version;
- exact validation stage that rejected the save.

## 38. Security Scope

Save files are local user data and not server-trusted competitive state.

Validation protects correctness and crash safety, not anti-cheat.

The loader still treats file lengths/counts as untrusted input to prevent crashes/excessive allocation from corruption.

## 39. Explicit Non-Goals

The RNG/migration architecture does not require:

- cryptographic RNG for gameplay outcomes;
- global single random stream;
- `std::hash` as persistence hash;
- `std::uniform_*_distribution` as authoritative persistent distribution;
- wall-clock reseeding on load;
- silent corrupt-save repair;
- arbitrary best-effort field dropping;
- bit-identical entire physics simulation solely from deterministic RNG.

## 40. Dependencies

This specification depends on TA-2 Identity, Domain State, Serialization, Transactions, TA-1 worker rules, and GDS deterministic generation/persistence.

## 41. Open Questions

None in the TA-2 RNG/migration/integrity contract.
