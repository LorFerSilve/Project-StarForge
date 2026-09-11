# Architecture Decisions

> **Status:** Active  
> **Purpose:** Preserve major technical architecture decisions and rationale

Detailed technical mechanics remain authoritative in their owning architecture specifications.

---

## AD-001 — StarForge Is a Purpose-Built Modular Monolith

**Status:** Accepted

### Decision

The baseline game runs as one desktop process with explicit internal modules rather than microservices, runtime plugins, or a general-purpose engine host.

### Rationale

The Design Complete GDS is single-player and does not require distributed authority. A modular monolith keeps ownership explicit while avoiding unnecessary infrastructure.

---

## AD-002 — C++23 and OpenGL 4.6 Core Are the Baseline

**Status:** Accepted

### Decision

Project code targets C++23 and the renderer targets desktop OpenGL 4.6 Core Profile.

### Rationale

This matches the project's explicit C++/OpenGL learning and implementation goal while allowing modern C++ language/library structure and modern core-profile rendering.

---

## AD-003 — Main Thread Owns Authoritative Simulation and OpenGL

**Status:** Accepted

### Decision

The initial runtime keeps OS/window handling, authoritative simulation commits, and OpenGL context/render execution on the main thread.

Worker threads may prepare immutable results but cannot mutate authoritative gameplay state or issue OpenGL commands.

### Rationale

This minimizes synchronization complexity during the initial custom-engine/game implementation and directly supports deterministic transaction ordering and Stable Save Boundaries.

---

## AD-004 — Authoritative Simulation Uses a Fixed 60 Hz Tick

**Status:** Accepted

### Decision

Real-time authoritative gameplay uses a fixed 60 Hz base simulation tick. Rendering remains variable-rate and may interpolate presentation state.

### Rationale

Movement, physics, combat, ship control, and timers require frame-rate-independent behavior. A single fixed base step gives predictable update ordering without forcing low-frequency strategic systems to update every tick.

---

## AD-005 — Strategic Systems Use Scheduled Lower-Frequency Updates

**Status:** Accepted

### Decision

Station, economy, long timers, production, and other low-frequency systems can use deterministic lower-rate/deadline scheduling derived from the same Simulation Time instead of running every 60 Hz tick.

### Rationale

The GDS explicitly permits coarser off-screen implementation while requiring equivalent Simulation Time and causal outcomes.

---

## AD-006 — StarForge Uses a Hybrid Entity Architecture

**Status:** Accepted

### Decision

High-frequency active local entities use a runtime entity registry/component storage, while strategic/persistent systems retain domain-owned stores.

There is no universal ECS containing all persistent game state.

### Rationale

Combat/movement/rendering benefit from compact active entity data, while Horizon topology, crew, missions, economy, research, events, and long-term assets have stronger explicit ownership/persistence requirements than a single generic ECS provides.

---

## AD-007 — Persistent Identity Is Separate From Runtime Handles

**Status:** Accepted

### Decision

Persistent identities such as CrewId, RobotId, ShipId, MissionId, and DynamicEventId survive save/load and scene activation. Runtime entity handles are process-local index/generation references and are never serialized as identity.

### Rationale

The GDS requires unique persistent identity across loading, off-screen simulation, mission transitions, and recovery.

---

## AD-008 — Authoritative Mutation Uses Command/Validate/Commit/Event

**Status:** Accepted

### Decision

Cross-domain and externally requested gameplay mutation follows Command/Intent → Validation → Atomic Commit → Committed Event/Result.

### Rationale

This directly implements the GDS ownership, extraction, trade, crafting, robot commissioning, raid theft, and Stable Save Boundary requirements without half-applied state.

---

## AD-009 — No Global Untyped Event Bus

**Status:** Accepted

### Decision

Cross-domain facts use typed event contracts with known producers/consumers. A stringly typed global gameplay event bus is not part of the baseline.

### Rationale

Typed events preserve ownership, traceability, compiler validation, and deterministic ordering in a large systemic game.

---

## AD-010 — Jolt Physics Is an Adapter, Not Gameplay Authority

**Status:** Accepted

### Decision

Jolt Physics supplies collision/rigid-body functionality behind a StarForge-owned physics interface. Jolt objects do not own damage, docking rules, health, mission outcomes, station structure, or persistent identity.

### Rationale

Physics is commodity infrastructure; GDS semantics must remain under project control and testable independently.

---

## AD-011 — Dear ImGui Is Development Tooling Only

**Status:** Accepted

### Decision

Dear ImGui is used for development/debug tools but not as the canonical shipping player-facing UI.

### Rationale

The GDS has explicit presentation/accessibility contracts; shipping UI must be owned by StarForge rather than constrained by a debug-tool UI model.

---

## AD-012 — vcpkg Manifest Mode Owns Reproducible Third-Party Dependencies

**Status:** Accepted

### Decision

CMake is the authoritative build description and vcpkg manifest mode provides pinned third-party dependencies.

### Rationale

The repository must build reproducibly without undocumented machine-global library installations while avoiding manually vendoring all dependency sources.

---

## AD-013 — Gameplay Logic Must Be Headlessly Testable

**Status:** Accepted

### Decision

Core simulation/gameplay/persistence tests must run without requiring an OpenGL context, window, or audio device.

### Rationale

The largest correctness risks are transactions, persistence, system graphs, missions, economy, AI rules, and deterministic state—not rendering. Headless tests make those systems fast and CI-friendly.

---

## AD-014 — Persistent Gameplay IDs Are Strongly Typed 64-Bit Monotonic Values

**Status:** Accepted

### Decision

Persistent gameplay identities use domain-tagged non-zero 64-bit monotonic IDs. Allocator state is persisted, committed identities are never reused within a save timeline, and creation commits ID allocation with record creation.

### Rationale

This is compact, deterministic, serialization-friendly, testable, and prevents accidental cross-domain ID substitution without introducing wall-clock/random UUID infrastructure.

---

## AD-015 — Active Runtime State Uses Explicit Activation Leases

**Status:** Accepted

### Decision

When a persistent entity becomes high-frequency active, designated state facets are transferred to the active runtime representation through an Activation Lease. The dormant store and active runtime never simultaneously mutate the same leased field.

### Rationale

This removes dual-authority ambiguity while allowing performant local simulation and coherent off-screen persistence/save export.

---

## AD-016 — Cross-Domain Transactions Prepare Fully Before an Infallible Commit Window

**Status:** Accepted

### Decision

Cross-domain mutations validate and prepare all participant deltas first. Final commit executes on the authoritative simulation thread in deterministic domain order and performs no fallible external I/O, asset loading, worker waits, or user interaction.

### Rationale

The GDS requires exactly-once ownership and reward/extraction/trade semantics. A fully prepared single-thread commit avoids partial gameplay transactions and complex rollback infrastructure.

---

## AD-017 — Consumer State Is Exposed Through Immutable Revisioned Read Models

**Status:** Accepted

### Decision

UI, renderer, audio, worker jobs, and diagnostics consume immutable purpose-specific read models/snapshots tagged with source revisions/generations. Player-facing views apply the Presentation Knowledge Boundary.

### Rationale

This prevents presentation/worker code from becoming hidden gameplay authority and provides explicit stale-state detection.

---

## AD-018 — Save Format V1 Is a Project-Owned Chunked Little-Endian Binary Container

**Status:** Accepted

### Decision

Shipping save format v1 uses explicit versioned per-domain Save DTO sections in a StarForge-owned little-endian binary container with a section directory and CRC32C integrity. Initial compression codec is `None`.

### Rationale

The format is deterministic, backend-independent, migration-friendly, and avoids coupling persistence to raw C++/STL/Jolt/OpenGL memory layout or adding a general serialization framework before it is needed.

---

## AD-019 — Persistent Procedural Randomness Uses Project-Owned PCG32 Scoped Streams

**Status:** Accepted

### Decision

Authoritative persistent procedural randomness uses a locked project-owned PCG32 implementation. Scoped streams are derived deterministically from SaveSeed, stable StreamKind/scope identity, and generation version using project-owned SplitMix64-based derivation. Standard-library distributions and `std::hash` are not persistence authority.

### Rationale

This keeps persistent procedural outcomes stable across save/load, thread scheduling, compiler/STL implementations, and unrelated random consumers.

---

## AD-020 — Save Loading and Migration Are Staged and All-or-Nothing

**Status:** Accepted

### Decision

Save decode, migration, domain import, reference/ownership validation, ContentId resolution, and active-context preparation occur in staging state. The live session root is replaced only after all required validation succeeds.

### Rationale

A corrupt/incompatible save must never leave the current session partially replaced or heuristically repaired into an undefined state.
