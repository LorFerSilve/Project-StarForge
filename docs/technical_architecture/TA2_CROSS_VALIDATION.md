# TA-2 Cross-Validation — Identity, State, Transactions, and Serialization

> **Status:** PASS  
> **Authority:** TA-2 consistency audit against TA-0/TA-1 and the Design Complete GDS

## 1. Scope

This audit validates:

- `05_identity_and_reference_model.md`;
- `06_domain_state_and_activation_contracts.md`;
- `07_command_result_and_event_contracts.md`;
- `08_transaction_and_ownership_contracts.md`;
- `09_read_models_and_state_revisions.md`;
- `10_serialization_and_save_contracts.md`;
- `11_rng_migration_and_integrity_contracts.md`.

against:

- Technical Architecture Authority;
- TA-1 Runtime/Simulation Model;
- Design Complete GDS identity, physical ownership, mission, robot, ship, event, Research/Blueprint, economy, recovery, and save/persistence rules.

## 2. Persistent Identity vs Runtime Handles — PASS

Persistent gameplay objects use typed `PersistentId<Tag>` values.

Active runtime entities use index+generation `RuntimeEntityHandle`.

Runtime handles are explicitly nonserialized and cannot become save identity.

## 3. Typed Identity — PASS

`CrewId`, `RobotId`, `ShipId`, `MissionId`, etc. are distinct compile-time types even when their raw 64-bit values match.

This prevents accidental cross-domain ID substitution.

## 4. Persistent ID Allocation — PASS

Persistent gameplay IDs:

- are non-zero;
- are monotonic per type;
- are allocated in the same authoritative creation transaction as the record;
- are not reused after committed creation;
- do not use wall-clock time or random collision avoidance.

This is compatible with save/load timeline semantics.

## 5. Transaction Identity — PASS

Transactions use a separate persistent monotonic `TransactionId` sequence for correlation/idempotency where required.

This does not turn Commands or runtime handles into persistent gameplay identities.

## 6. Content Identity — PASS

Authored definitions use stable `ContentId`, separate from runtime/persistent object identity.

Save v1 stores ContentId as canonical UTF-8 string.

TA-10 remains owner of content registry/file layout without needing to redesign persistence identity.

## 7. Domain Ownership — PASS

Every authoritative mutable field has one owning domain/store.

No universal mutable ECS/store is introduced.

This preserves TA-1 hybrid architecture and GDS authority ownership.

## 8. Cross-Domain References — PASS

Domains use typed IDs/read contracts/commands rather than writable cross-store pointers.

Strong/Optional/Historical reference semantics are explicit and load-validatable.

## 9. Deterministic Store Iteration — PASS

Authoritative behavior cannot depend on hash-map bucket order, allocation address, or worker timing.

Persistent IDs provide deterministic tie-breaking where no stronger domain order exists.

## 10. Activation Authority — PASS

Activation Lease makes active high-frequency state and dormant persistent state mutually exclusive authorities for leased facets.

This closes the dual-authority risk inherent in keeping persistent actors active in local simulation.

## 11. Save While Active — PASS

Active entities do not need artificial deactivation before saving.

The save exporter combines store-owned fields and currently authoritative leased facets into one logical DTO at Stable Save Boundary.

No duplicate active+dormant identity is serialized.

## 12. Off-Screen Horizon — PASS

When Horizon is off-screen, detailed runtime leases can return to persistent stores while strategic systems continue through the same domain state.

No alternate simplified ownership/save model is introduced.

## 13. StateRevision — PASS

Domain/record revisions provide a common stale-state mechanism for:

- worker results;
- UI commands;
- prepared transactions;
- activation;
- read models.

They are technical versions, not gameplay progression.

## 14. Command Semantics — PASS

Commands are immutable typed requests with one owning handler.

Rejected commands do not mutate state.

UI, AI, automation, scheduled timers, and tests all use the same authoritative validation boundary.

## 15. Result Semantics — PASS

Results explicitly communicate accepted/rejected/committed outcome and typed reasons/created IDs where required.

Presentation no longer needs to infer success from input submission.

## 16. Event Semantics — PASS

Events are immutable committed facts and cannot be retroactively vetoed.

The event stream is not used as the sole save authority, avoiding an unintended event-sourcing architecture.

## 17. Deterministic Event/Command Order — PASS

SimulationTick + sequence + explicit domain order provide deterministic processing independent of worker completion and hash iteration.

## 18. Long-Running Operations — PASS

Research, manufacturing, travel, missions, recovery, and reinforcements are persistent state machines/timers rather than atomic transactions held open over Simulation Time.

This is compatible with True Pause and Stable Save Boundaries.

## 19. Cross-Domain Atomicity — PASS

Cross-domain TransactionCoordinator validates/prepares all participants before committing any participant.

Participant commit order is fixed and deterministic.

Nested arbitrary cross-domain commits are prohibited.

## 20. Commit Failure Boundary — PASS

Commit phase contains only already-prepared deterministic in-memory changes and no fallible external I/O/content loading/worker wait.

Expected failure therefore occurs before commit.

Invariant failure is treated as software-integrity failure rather than fictional partial success.

## 21. Physical Ownership — PASS

Physical transfer commits source and destination ownership under one transaction.

Unique items have exactly one owner; stack transfer conserves quantity except explicit consumption/transformation.

This realizes GDS Physical Ownership.

## 22. Consumption — PASS

Consumed ammo/fuel/materials use an explicit sink transaction rather than disappearing outside ownership accounting.

This supports failure persistence and diagnostics.

## 23. Credits — PASS

Credits remain a non-physical Economy ledger and use integer smallest-unit representation.

Trade with physical items is one Cross-Domain Transaction.

No floating-point money or resource-like Credit object was introduced.

## 24. Trade — PASS

Credit debit/liquidity/stock and item transfer commit together.

A save cannot represent payment without goods or goods without payment for an immediate normal trade.

## 25. Manufacturing — PASS

Physical inputs transition through persistent WorkInProgress rather than disappearing while a long open transaction waits.

Robot commissioning creates RobotId only at final valid commit.

## 26. Mission Extraction — PASS

Mission state and ownership-security transition can participate in one transaction.

`Field-Unsecured -> Vehicle/Extraction-Secured` cannot double-commit.

## 27. Raid Theft — PASS

Raid theft uses the same physical ownership transaction model and GDS extraction finalization.

No separate percentage loot ledger is introduced.

## 28. One-Time Rewards — PASS

Persistent transaction/completion markers allow exactly-once reward semantics across save/load.

The design does not require retaining an unbounded global event log.

## 29. Story Choice — PASS

Irreversible narrative/world/faction/progression changes can commit atomically and persist TransactionId/choice state where required.

## 30. Stable Save Boundary — PASS

No save captures Prepared/Committing short transaction state.

All transaction participants are fully committed or aborted before snapshot capture.

This directly realizes the GDS Stable Save Boundary.

## 31. Immutable Read Models — PASS

Render/UI/audio/workers consume immutable projections rather than writable domain state.

Cross-domain transactions become visible atomically as complete pre- or post-state.

## 32. Presentation Knowledge Boundary — PASS

Player-facing Read Models apply knowledge filtering.

UI rejection/results cannot leak hidden enemy/world data through technical state access.

Development privileged inspection remains isolated to tools.

## 33. Stale UI/Worker State — PASS

Expected StateRevision/ActivationEpoch/generation validation prevents stale commands and worker outputs from overwriting newer state.

## 34. Save DTO Boundary — PASS

Domain Save DTOs contain explicit persistence data only.

They do not serialize:

- RuntimeEntityHandle;
- raw pointers;
- STL memory layout;
- OpenGL/Jolt/GLFW/miniaudio objects;
- callbacks/mutexes.

## 35. Save Container — PASS

Save v1 has a concrete StarForge-owned little-endian chunked binary container with:

- fixed header;
- section directory;
- per-domain SchemaVersion;
- explicit codec field;
- CRC32C section integrity.

Initial codec is explicitly `None`, so implementation does not need to invent compression policy.

## 36. Deterministic Encoding — PASS

Fixed-width primitives, stable enum values, explicit UTF-8 strings, deterministic collection order, and no raw padding/hash order remove platform-layout dependence from saves.

## 37. Active Entity Serialization — PASS

Active leased state exports into the same logical persistent DTO as dormant state.

This satisfies the GDS requirement to save combat/mission/ship/robot state without serializing runtime backend handles.

## 38. Crash-Safe Write — PASS

Save file writing uses immutable snapshot + write-new + validate + atomic replace.

The last known valid save is not overwritten before replacement succeeds.

## 39. All-or-Nothing Load — PASS

Load uses staging state and replaces the live session only after all required migration, reference, ownership, content, and invariant validation passes.

A corrupt required section cannot partially mutate the running session.

## 40. Save Versioning — PASS

ContainerVersion, ApplicationSaveCompatibilityVersion, and per-domain SchemaVersion are distinct.

A domain schema change does not unnecessarily redefine the binary container.

## 41. Migration — PASS

Migrations are deterministic staging transforms and cannot use wall clock, fresh entropy, network state, or player input.

Persistent IDs and exactly-once markers are preserved unless an explicit migration contract says otherwise.

## 42. Content Migration — PASS

Renamed ContentIds require explicit migration maps.

Missing required content does not silently substitute unrelated content.

## 43. Deterministic RNG — PASS

Authoritative persistent RNG uses project-owned PCG32 primitives with SplitMix64-based deterministic stream derivation.

No `std::hash`, wall-clock reseeding, or implementation-dependent standard distribution is authoritative.

## 44. RNG Stream Isolation — PASS

Named/scoped streams prevent unrelated random consumption from perturbing mission/event/loot outcomes.

Worker threads cannot race on one shared generator.

## 45. Reload Anti-Reroll — PASS

Persistent generated state and required RNG cursor/version are saved.

Reloading the same snapshot cannot reroll major procedural outcomes.

## 46. Generation Algorithm Version — PASS

GenerationAlgorithmVersion is separate from Save SchemaVersion.

Existing generated content can persist unchanged even when newer game versions generate future content differently.

## 47. Referential Integrity — PASS

Strong required references, allocator bounds, unique IDs, ContentIds, timers, ownership, TransactionIds, and local-context identity are all validated before session activation.

## 48. Save Corruption Safety — PASS

Bounds/count/string checks plus CRC prevent malformed save lengths from becoming unchecked allocation or partial gameplay state.

This is correctness/crash protection, not multiplayer anti-cheat.

## 49. Single-Player Scope — PASS

TA-2 introduces no server/database/distributed transaction/anti-cheat architecture.

All contracts remain appropriate for the authoritative local single-player process.

## 50. Headless Testing — PASS

Identity, stores, transactions, RNG, serialization, migration, and validation can be tested without OpenGL, GLFW, Jolt world, or audio device.

This preserves AD-013.

## 51. TA-3 Compatibility — PASS

TA-3 can now define local/world/zone streaming using:

- typed persistent identity;
- runtime handles;
- Activation Lease;
- ContentId;
- staged activation/deactivation;
- StateRevision;
- Save DTO ownership.

It does not need to invent persistence or identity rules.

## 52. Remaining Dependencies

TA-2 does not authorize scaffolding yet.

Later architecture still needs to define:

- world/scene/zone/streaming;
- renderer;
- physics/character/spaceflight;
- station graphs;
- runtime entity component layout;
- AI/navigation;
- mission/raid runtime state;
- content/asset pipeline;
- UI/input/audio integration;
- concrete save schema/domain DTO field layouts;
- concurrency/performance budgets;
- testing/CI;
- final architecture integration audit;
- implementation roadmap and lock.

## 53. Verdict

**TA-2: ARCHITECTURE COMPLETE**  
**Blocking contradiction with TA-0/TA-1: NONE**  
**Blocking contradiction with Design Complete GDS: NONE**  
**Implementation-critical open question in TA-2 scope: NONE**  
**C++/OpenGL scaffolding authorized: NO**  
**Next dependency: TA-3 — World, Scene, Zone, and Streaming Architecture**
