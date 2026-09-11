# Project StarForge

Project StarForge is a custom-built 3D science-fiction action, exploration, base-building, strategy, and light-simulation game developed in C++ and OpenGL without a general-purpose game engine.

The game centers on a physically traversable and expandable player-owned space station. The player undertakes expeditions, gathers resources and technology, recruits survivors, develops spacecraft and robot squads, automates station operations, and attacks or defends space stations.

## Current Project Phase

The authoritative Game Design Specification has completed **GDS-0 through GDS-14** and is formally **Design Complete**.

The project is now in **Technical Architecture**.

Technical Architecture stages **TA-0 through TA-7** are complete/Architecture Complete. The next dependency is **TA-8 — AI and Navigation Architecture**.

TA-2 fixed persistent identity, domain-state ownership, Activation Leases, typed command/result/event contracts, cross-domain transaction semantics, immutable read models, save DTO/container contracts, migration/integrity rules, and deterministic procedural RNG.

TA-3 fixed the one-player-local-scene model, bounded Zone/Scene lifecycle, streaming residency versus simulation activation, double-precision Context Space plus floating-origin runtime coordinates, Horizon active/off-screen handoff, persistent world projection, scene profiles, and staged atomic context transitions/loading.

TA-4 fixed the OpenGL renderer architecture: immutable render snapshots, hybrid deferred/forward rendering, reversed-Z depth, glTF-compatible PBR, lighting/shadows, cameras, visibility/LOD/instancing/transparency, VFX/particles, GPU resource lifetime, resize/failure behavior, and presentation-only graphics tiers.

TA-5 fixed Jolt integration, collision/query layers, kinematic CharacterMotor, physical projectiles/sweeps, Dynamic 6DoF spacecraft, docking constraints, Zero-G/EVA/Magnetic Boots, collision-damage fact routing, origin synchronization, and fixed 60 Hz physics ordering.

TA-6 fixed Horizon's structural/traversal/utility topology, deterministic Power allocation, conserved Atmosphere/Thermal/Water state, one-owner logistics, manufacturing/farming/work scheduling, construction/damage/repair topology mutation, ControlData/automation, chronological off-screen simulation, and persistent-state-first physics projection.

TA-7 now fixes the active gameplay runtime: one generation-checked RuntimeEntityRegistry per SceneInstance, typed component pools, persistent-actor Activation Lease bridges, actor lifecycle, player/equipment/inventory runtime references, combat/weapon/status runtime state, projectile/interactable/world-object projections, persistent↔runtime synchronization, deferred destruction, and a deterministic 60 Hz runtime phase order.

Gameplay implementation and C++/OpenGL scaffolding have **not** begun yet. They remain gated behind the remaining Technical Architecture work, TA-15 integration audit, and TA-16 implementation roadmap/contract locking.

## Documentation

Authoritative game design:

[`docs/game_design/`](docs/game_design/)

Technical architecture:

[`docs/technical_architecture/`](docs/technical_architecture/)

Technical Architecture roadmap:

[`docs/technical_architecture/TA_ROADMAP.md`](docs/technical_architecture/TA_ROADMAP.md)

Final GDS-14 maturity/promotion evidence:

[`docs/game_design/audit/`](docs/game_design/audit/)

## Initial Technical Baseline

- C++23
- CMake
- vcpkg manifest mode
- OpenGL 4.6 Core Profile
- GLFW
- glad2
- GLM
- Jolt Physics behind a StarForge adapter
- miniaudio behind a StarForge audio layer
- fastgltf / glTF 2.0 asset pipeline
- Dear ImGui for development tooling only
- Catch2 for automated C++ tests

## Core Development Principle

Project StarForge implements only the engine and gameplay functionality required by approved game design. Technical architecture follows the Design Complete gameplay requirements rather than speculative engine development.

## Working Title

**Project StarForge**

This is currently a working project title and may be replaced later without changing the project's history or design identity.
