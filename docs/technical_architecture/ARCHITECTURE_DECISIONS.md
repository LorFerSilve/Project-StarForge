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

---

## AD-021 — The Strategic Galaxy Is Topological, Not One Continuous Physics Coordinate Space

**Status:** Accepted

### Decision

Strategic galaxy navigation is represented through persistent location identities and route topology. Only the current bounded local context receives a physical 3D coordinate space.

### Rationale

The Design Complete GDS explicitly separates strategic travel from bounded local flight/mission zones. This avoids meaningless astronomical coordinate precision and prevents a speculative seamless-galaxy engine architecture.

---

## AD-022 — Exactly One Player-Local Scene Is Authoritative

**Status:** Accepted

### Decision

StarForge runs at most one authoritative player-local `SceneInstance` at a time. Horizon can continue off-screen through persistent scheduled state and bounded tactical working data, but not through a second fully active physics/render/locomotion scene.

### Rationale

This preserves the GDS one-deployed-local-context model, simplifies authority and memory ownership, and prevents duplicate crew/robot/station state.

---

## AD-023 — Streaming Residency Is Separate From Gameplay Activation

**Status:** Accepted

### Decision

Stream-cell content can be loaded/resident without its gameplay actors, hazards, objectives, discoveries, or simulation being active. Logical activation is decided deterministically by the simulation thread; prefetch and eviction are technical optimizations only.

### Rationale

Asset-I/O timing must not change gameplay, reveal hidden information, or cause resources/enemies to spawn differently across machines.

---

## AD-024 — Persistent Local Space Uses Double Precision With an Origin-Relative Runtime Frame

**Status:** Accepted

### Decision

Persistent local positions are stored as double-precision meter coordinates scoped to their Zone/Location. Active physics/render/audio operate in an origin-relative runtime frame controlled by `RuntimeOrigin64` and `OriginEpoch`, with rebasing only at Stable Simulation Boundaries.

### Rationale

This supports player-scale precision inside large bounded spaceflight contexts without requiring double precision in every backend or one galaxy-scale coordinate system.

---

## AD-025 — Required Streaming Failure Freezes Simulation Rather Than Changing Outcomes

**Status:** Accepted

### Decision

If required gameplay content is not ready before the player/actor would cross into it, the session enters a Hard Streaming Hold at a Stable Simulation Boundary and authoritative Simulation Time stops until the content is ready or a technical failure is declared.

### Rationale

A slow disk, CPU, or worker schedule must not let enemies move, hazards tick, production advance, or the player suffer outcomes while required world geometry is unavailable.

---

## AD-026 — Inter-Context Transitions Stage Destination Before Atomic Context Commit

**Status:** Accepted

### Decision

Destination scene/content is staged and validated before the authoritative player/ship/mission/context-location switch. The context change then commits atomically, after which destination Activation Leases become authoritative.

### Rationale

This avoids unload-first failure modes and prevents half-transitions where gameplay ownership says the player is at one location while runtime representation exists at another.

---

## AD-027 — Runtime Scenes Are Projections, Not Persistent Gameplay History

**Status:** Accepted

### Decision

Local scenes are reconstructed from immutable Content definitions plus persistent World/Location and Mission/Zone state. Scene presence, streamed residency, render visibility, or runtime object destruction is never the sole record of persistent gameplay consequence.

### Rationale

This guarantees that unload/reload cannot respawn harvested resources, duplicate ships/robots, repair destroyed objects, reset doors, or erase discoveries/objectives.

---

## AD-028 — StarForge Uses a Hybrid Deferred Renderer

**Status:** Accepted

### Decision

Opaque and alpha-masked world geometry render through a deferred G-buffer/lighting path. Transparent glass, shields, special energy surfaces, and VFX render through forward HDR passes after deferred lighting.

### Rationale

StarForge contains dense station/interior lighting plus meaningful transparent and effect-heavy surfaces. A hybrid deferred path gives predictable many-light opaque shading without forcing transparency into an unsuitable G-buffer or adopting a larger engine renderer.

---

## AD-029 — Gameplay Views Use Reversed-Z Zero-to-One Depth

**Status:** Accepted

### Decision

The OpenGL renderer uses `glClipControl` zero-to-one clip depth, reversed-Z floating-point depth, and effectively infinite far perspective projection where compatible with the view profile.

### Rationale

TA-3 permits large bounded local spaceflight contexts while also requiring precise player-scale scenes. Reversed-Z provides substantially better depth precision without creating a galaxy-scale coordinate system.

---

## AD-030 — World Materials Use glTF-Compatible Metallic-Roughness PBR

**Status:** Accepted

### Decision

Baseline world shading uses metallic-roughness PBR with GGX/Smith/Schlick direct lighting, image-based environment/probe lighting, and explicit direct lights. Real-time global illumination is not a baseline requirement.

### Rationale

This maps directly to the selected glTF 2.0 asset exchange format, supports the stylized-grounded art direction, and keeps the custom renderer achievable without speculative GI/ray-tracing infrastructure.

---

## AD-031 — Rendering Uses Linear HDR Internally and SDR sRGB Output Baseline

**Status:** Accepted

### Decision

Scene lighting is accumulated in linear HDR, tone mapped to SDR sRGB, and shipping UI is composited afterward at native output resolution. Native HDR-display output is outside the initial baseline.

### Rationale

This provides lighting/effect headroom while keeping the first shipping display path predictable and preserving UI color/readability independently of world exposure and 3D render scale.

---

## AD-032 — Initial Anti-Aliasing Is Off or FXAA and Motion Blur Is Omitted

**Status:** Accepted

### Decision

The initial renderer supports no anti-aliasing or FXAA as the shipping AA options. TAA/MSAA are not baseline requirements, and motion blur is not implemented in the initial renderer.

### Rationale

FXAA is simple and stable with the deferred/floating-origin architecture, while avoiding temporal-history complexity before representative scenes exist. GDS-13 already defines motion blur as optional and Off by default.

---

## AD-033 — GPU Resources Are Render-Owned and Uploaded on the Main OpenGL Thread

**Status:** Accepted

### Decision

Workers prepare immutable CPU-side asset payloads. Only the main/context-owning render thread creates, mutates, uploads, or destroys OpenGL resources. Runtime GPU references are generation-checked and never persistent gameplay identity.

### Rationale

This preserves TA-1 thread ownership, avoids multi-context synchronization complexity, and gives scene streaming a clear resource-lifetime boundary.

---

## AD-034 — Graphics Quality Can Change Presentation Cost but Never Gameplay Semantics

**Status:** Accepted

### Decision

Low/Medium/High/Ultra/Custom presets can change render scale, shadows, SSAO, texture/LOD residency, bloom, and decorative effects only within the critical-readability floor. Automatic dynamic resolution is not baseline.

### Rationale

Hardware/performance variability must never change collision, simulation timing, world knowledge, enemy behavior, rewards, or required hazard/objective readability.

---

## AD-035 — Jolt Physics Is Accessed Only Through StarForge-Owned Runtime Contracts

**Status:** Accepted

### Decision

The active player-local SceneInstance owns one StarForge `PhysicsWorld`; gameplay accesses it only through typed project handles, descriptors, queries, and normalized physical facts. Raw Jolt body/constraint identities are neither gameplay authority nor persistent data.

### Rationale

This keeps backend replacement/tuning isolated and preserves the GDS/TA ownership model for damage, missions, docking, structure, and persistence.

---

## AD-036 — Humanoid Locomotion Uses a Kinematic CharacterMotor, Spacecraft Use Dynamic Rigid Bodies

**Status:** Accepted

### Decision

Player/humanoid locomotion uses a collision-aware kinematic CharacterMotor over Jolt character/query facilities, while local spacecraft use true Dynamic rigid bodies with project-computed mass/inertia and bounded forces/torques.

### Rationale

Responsive authored first-person locomotion and inertial 6DoF spacecraft have materially different control requirements. Forcing both into one physical-body model would either make characters unstable or ships nonphysical.

---

## AD-037 — Physics Contacts Produce Facts; Gameplay Domains Commit Consequences

**Status:** Accepted

### Decision

Jolt contact/trigger callbacks are normalized into project-owned physical observations. Health damage, ship damage, structural breakage, docking state, mission progress, and hazard effects are committed only by their owning gameplay domains after validation.

### Rationale

Solver callbacks are not a safe or semantically correct location for cross-domain gameplay mutation, and backend contact details must not become hidden gameplay rules.

---

## AD-038 — Ship Speed Limits Are Avionics Constraints, Not Fake Vacuum Drag or Instant Clamps

**Status:** Accepted

### Decision

The GDS local-space speed envelope is implemented through project-owned flight-control/avionics authority. Thrust beyond the safe envelope is limited and physically available counter-thrust can recover excessive velocity; the rigid body is not subjected to gameplay-significant universal vacuum drag or instantaneous velocity clamping.

### Rationale

This preserves inertial spaceflight while maintaining the bounded controllable local-flight envelope defined by the GDS.

---

## AD-039 — Hard Dock Is a Real Physical Attachment Reconstructed From Logical Dock State

**Status:** Accepted

### Decision

A committed Hard Dock creates an explicit project-owned physics constraint/attachment after physical capture validation. Save/load stores logical docking identities/state and reconstructs the physical constraint rather than serializing backend constraint IDs.

### Rationale

Docking must remain a physical operational attachment without coupling persistence to Jolt runtime objects or allowing trigger overlap to equal docking success.

---

## AD-040 — Floating-Origin Rebases Occur Only at Stable Simulation Boundaries and Cannot Create Gameplay Events

**Status:** Accepted

### Decision

Physics bodies, character state, projectiles, constraints, and related spatial structures rebase together only at a TA-1 Stable Simulation Boundary. Relative state is preserved and no impact, damage, trigger, or movement consequence is generated solely by the coordinate-frame change.

### Rationale

TA-3's origin-relative runtime model must remain an invisible precision mechanism rather than becoming a source of synthetic physics or gameplay.

---

## AD-041 — Physical Projectiles Prefer Swept Project-Owned Motion; Dynamic Rigid Bodies Are Used When Bounce/Rigid Interaction Matters

**Status:** Accepted

### Decision

Fast plasma, rockets, and similar gameplay projectiles use deterministic fixed-tick swept motion unless their physical behavior specifically requires a Dynamic rigid body. Grenades and comparable bouncing/rolling objects can use Dynamic physics.

### Rationale

Swept project-owned motion avoids tunneling and excessive rigid-body overhead while preserving true rigid interactions for projectile classes that visibly depend on them.

---

## AD-042 — Horizon Uses Shared Topology Facts but Separate Typed Subsystem Solvers

**Status:** Accepted

### Decision

Horizon has one canonical persistent module/port/topology foundation, but Power, Atmosphere, Thermal, Water, and Logistics build separate typed graph/solver views. StarForge does not implement one universal utility-flow solver.

### Rationale

The station systems share physical connectivity while conserving materially different state and obeying different failure/transfer semantics. A universal graph solver would blur authority and make conservation/error behavior ambiguous.

---

## AD-043 — Stable Pressure Cells Own Atmosphere; Portals Transfer Conserved Gas

**Status:** Accepted

### Decision

Station atmosphere is stored as conserved gas-species quantities in stable authored Pressure Cells. Doors, breaches, ducts, and airlocks are Pressure Portals that transfer gas; opening/closing topology does not recreate gas ownership through temporary compartment objects.

### Rationale

Stable cells make dynamic compartment split/merge deterministic and prevent pressure-boundary changes from duplicating or deleting atmosphere.

---

## AD-044 — Station Power Uses Deterministic Capacity-Constrained Allocation Per Connected Island

**Status:** Accepted

### Decision

Power is allocated separately inside each connected electrical island using deterministic priority/order rules and finite producer, storage, edge, and consumer capacities. Total station generation does not bypass a local distribution bottleneck.

### Rationale

This directly implements the GDS network, redundancy, load-shedding, and throughput behavior without simulating unnecessary AC electrical detail.

---

## AD-045 — Logistics Preserves One Physical Owner Through Explicit In-Transfer Ownership

**Status:** Accepted

### Decision

Station logistics never creates a network-global inventory copy. Cargo removed from a source but not yet delivered is owned by a typed TransferCargoOwner correlated to one TransferJob until destination delivery or explicit loss/recovery.

### Rationale

This makes interrupted routes and save/load unambiguous while preserving TA-2 exactly-one-owner resource semantics.

---

## AD-046 — Long-Running Station Work Uses Persistent WorkOrders and Exactly-Once Completion

**Status:** Accepted

### Decision

Manufacturing, farming, construction, repair, and compatible station work progress through persistent WorkOrders driven only by Simulation Time. Resource consumption and output/harvest/completion occur at explicit transactional milestones rather than through an open long-lived transaction.

### Rationale

This supports pausing, saving, interruption, off-screen execution, and deterministic failure without duplicated outputs or refunded consumed inputs.

---

## AD-047 — Station Topology Commits Before Physics/Render Projection Changes

**Status:** Accepted

### Decision

Construction, damage, repair, door, breach, and related persistent station mutations commit in their owning stores first. Resulting StationGeometryDelta records are applied only later at TA-5's safe deferred physics-mutation boundary; renderer/navigation consume derived state afterward.

### Rationale

Persistent station state must never chase a physics/render mutation that happened first, and Jolt callbacks must not become topology authority.

---

## AD-048 — Off-Screen Horizon Advances Chronologically by Simulation-Time Boundaries

**Status:** Accepted

### Decision

Horizon off-screen simulation uses the same persistent systems and advances through deterministic event/deadline boundaries. It may take larger mathematical steps but must stop at earlier depletion, threshold, completion, damage, or policy events before advancing further.

### Rationale

This achieves performance without a hidden second full scene and prevents coarse stepping from granting production/cooling/growth after a failure that should already have occurred.

---

## AD-049 — Thermal and Water Networks Conserve Explicit Stored Quantities

**Status:** Accepted

### Decision

Thermal simulation tracks finite heat/coolant state with explicit transfer/rejection, while Water tracks separate Fresh Water and Wastewater inventories with explicit transfer, recycling loss, reserve, and leakage. Neither system uses hidden global availability.

### Rationale

Explicit conserved state makes damage, isolation, leaks, storage limits, and off-screen stepping testable and prevents utility duplication/deletion.

---

## AD-050 — Control/Data Connectivity Governs Observability and Automation, Not Physical Existence

**Status:** Accepted

### Decision

Central station control and automation depend on explicit ControlData connectivity and sensor knowledge. Losing control/sensors can degrade commands/diagnostics, while the underlying physical Power, Atmosphere, Thermal, Water, equipment, and local safety state continue according to their own rules.

### Rationale

This preserves systemic failures and prevents both omniscient automation and the opposite error where losing a network connection makes physical infrastructure cease to exist.

---

## AD-051 — Active Scenes Own a Generation-Checked Runtime Entity Registry

**Status:** Accepted

### Decision

Each active SceneInstance owns exactly one RuntimeEntityRegistry. RuntimeEntityHandle uses index + generation and is additionally validated against SceneGeneration. Runtime handles are never serialized or treated as persistent identity.

### Rationale

High-frequency local objects need compact references and safe slot reuse, while TA-2 persistent identities must survive scene destruction/reconstruction independently.

---

## AD-052 — Runtime Composition Uses Typed Component Pools, Not a Universal GameObject Hierarchy

**Status:** Accepted

### Decision

Runtime gameplay state is composed from typed component pools under the active scene. StarForge does not use a universal polymorphic GameObject inheritance tree or make a generic archetype ECS a baseline requirement.

### Rationale

Typed pools give efficient active iteration and explicit state ownership without turning the project into a general engine or forcing persistent strategic state into an ECS.

---

## AD-053 — Persistent Actors Activate Through Typed Activation Adapters and Leases

**Status:** Accepted

### Decision

Persistent player, crew, robot, ship and other eligible actor records enter the active runtime through typed activation descriptors and TA-2 Activation Leases. Only designated high-frequency facets transfer to runtime authority; persistent identity and non-leased strategic fields remain store-owned.

### Rationale

This preserves exactly one mutable authority per field while allowing performant local simulation and coherent save/deactivation handoff.

---

## AD-054 — Runtime Inventory and Equipment State References Physical Persistent Ownership

**Status:** Accepted

### Decision

Equipped items, quick slots, world-item proxies and runtime equipment state reference the actual Inventory/Resource-owned objects or stacks. Runtime components never create a second physical ownership copy. Pickup/removal presentation follows ownership commit.

### Rationale

This keeps mass/volume, security state, reservations, ammo and defeat behavior transactionally correct and prevents scene/runtime reconstruction from duplicating items.

---

## AD-055 — Combat Runtime Consumes Physics Facts; Owning Domains Commit Consequences

**Status:** Accepted

### Decision

Weapons/projectiles/hit zones produce typed attack and physical facts. Combat resolves shield/armor/status semantics, then Biological Health, Robot, Spacecraft, Station or other owning domains/components commit the authoritative consequence. Health zero does not itself free the runtime entity.

### Rationale

This preserves TA-5's backend boundary and GDS distinctions between Incapacitation, Disablement, Destruction and persistent aftermath.

---

## AD-056 — Gameplay-Relevant Status Effects Use Deterministic Simulation-Time Runtime Instances

**Status:** Accepted

### Decision

Active StatusInstance state carries explicit definition, source, stacking/intensity, duration/deadline and periodic tick state. Status progression uses Simulation Time and authored deterministic stacking/application rules; persistent statuses return/export with persistent actor state.

### Rationale

This makes status effects frame-rate independent, save/deactivation safe and consistent with the GDS prohibition on undefined/random baseline proc behavior.

---

## AD-057 — Runtime Entity Removal Is Logical First and Physically Reclaimed Later

**Status:** Accepted

### Decision

Gameplay outcomes first move entities through Closing/PendingDestroy. Entity/component/physics/render/audio reclamation occurs only in deferred safe mutation phases after consequences and lease returns settle. Callbacks cannot immediately free authoritative runtime objects.

### Rationale

Deferred destruction prevents use-after-free, invalid iteration and backend callback races while preserving persistent/corpse/wreck outcome semantics.

---

## AD-058 — The Active Gameplay Runtime Uses One Explicit Deterministic Phase Order

**Status:** Accepted

### Decision

Each 60 Hz runtime tick orders intents, action/status deadlines, movement/weapon preparation, physics, normalized facts, combat/interaction resolution, owning-domain commits, runtime reflection, lifecycle transitions, deferred structural/physics mutation and immutable snapshot publication.

### Rationale

A fixed phase contract prevents render order, callback timing or container iteration from deciding gameplay and ensures persistent commit precedes visual/runtime disappearance.

---

## AD-059 — Grounded Navigation Uses Recast/Detour Behind a StarForge Navigation Adapter

**Status:** Accepted

### Decision

StarForge uses Recast/Detour for commodity grounded navmesh cooking/query operations. Project-owned traversal profiles, dynamic links, hazard/access filters, path-result validation and AI decisions remain outside the library, and raw backend refs are never persistent identity.

### Rationale

Navigation-mesh generation and path corridors are mature commodity problems, while StarForge-specific security, hazard, robot-size, task and persistence semantics must remain under project control.

---

## AD-060 — Grounded and True 3D Navigation Use Separate Representations

**Status:** Accepted

### Decision

Grounded humanoids/robots use tiled 2.5D navigation meshes grouped into Small/Standard/Heavy classes. Flying/Zero-G autonomous actors use a bounded project-owned 3D free-space graph/volume representation rather than being projected onto a floor navmesh.

### Rationale

The two movement domains have materially different topology and clearance semantics. One representation would either overcomplicate grounded navigation or make free-flight behavior physically incorrect.

---

## AD-061 — Async Pathfinding Is Advisory and Revision-Validated

**Status:** Accepted

### Decision

Pathfinding workers read immutable navigation snapshots and return candidate routes tagged with SceneGeneration, NavigationRevision, actor generation, task/command generation, and request sequence. The main simulation accepts results only after freshness validation and consumes them in deterministic actor/request order.

### Rationale

Worker completion timing must improve performance without becoming a gameplay race or allowing stale paths to cross changed topology.

---

## AD-062 — AI Knowledge Is Explicitly Separate From World Truth

**Status:** Accepted

### Decision

Knowledge-limited decisions consume PerceptionMemory/SharedKnowledge built from legitimate vision, hearing, sensors, combat facts, commands, and mission intel. AI may not query hidden world state for targeting/search convenience, and shared information retains source, timestamp, confidence and precision.

### Rationale

The GDS requires non-omniscient enemy/robot behavior. Encoding that as an architecture boundary prevents accidental wallhacks through otherwise convenient gameplay queries.

---

## AD-063 — Active AI Uses Layered Project-Owned Decisions Rather Than a Mandatory Generic Behavior Framework

**Status:** Accepted

### Decision

The baseline AI architecture uses explicit safety/capability constraints, owning commands/tasks, awareness/tactical goals, role/doctrine selection, and typed action execution. A generic Behavior Tree, GOAP, scripting VM, or ML runtime is not a baseline architectural requirement.

### Rationale

The game needs deterministic, testable behaviors tied closely to established GDS contracts, not a speculative general AI engine.

---

## AD-064 — Robot Orders Are Persistent Intent; Local AI Executes Them Through Physical Sub-Actions

**Status:** Accepted

### Decision

Player robot commands keep their GDS state/generation and decompose into navigation, positioning, interaction and combat sub-actions. Communication/ROE/fallback constrain execution; command AI cannot create resources, bypass access, or initiate strategic mission outcomes.

### Rationale

This preserves concise player authority while preventing tactical orders from becoming hidden teleport/permission/resource shortcuts.

---

## AD-065 — Off-Screen Mobile AI Uses Logical Routes and Simulation-Time Boundaries

**Status:** Accepted

### Decision

Crew/robots outside high-detail simulation retain authoritative logical locations, route segments, tasks, resources, hazards and deadlines. They advance chronologically through the same Simulation Time/access/capability constraints and reconstruct physical runtime state through Activation Lease handoff.

### Rationale

This avoids a second hidden full Horizon scene while preventing off-screen teleport, free work, skipped hazards, or different gameplay outcomes merely because the player is away.

---

## AD-066 — AI Scheduling and Worker Timing Are Never Semantic Authority

**Status:** Accepted

### Decision

An `AIScheduler` deterministically selects due high-level/perception work, while urgent interrupts cannot be silently dropped. Worker jobs remain advisory; result completion order never determines target choice, task assignment, path priority, or combat outcome.

### Rationale

Performance scaling must not make gameplay depend on CPU timing, thread scheduling, hash iteration, or render rate.