# System Context and Top-Level Modules

> **Status:** Architecture Complete  
> **Authority:** Top-level runtime/module partition and dependency direction

## 1. Purpose

This document defines the main software regions of Project StarForge and which responsibilities belong where.

The goal is to prevent architecture drift into either:

- one giant coupled executable where every subsystem can touch everything; or
- an over-engineered general game engine with abstractions unrelated to StarForge.

## 2. Process Model

Baseline StarForge runs as one desktop process:

`starforge`

The process contains:

- platform/window layer;
- application/runtime orchestration;
- authoritative simulation;
- active local world representation;
- off-screen strategic simulation;
- renderer;
- physics adapter/world;
- audio;
- UI/presentation;
- asset/content services;
- persistence;
- developer diagnostics.

No gameplay server or companion service is required.

## 3. Top-Level Source Modules

The intended source partition is conceptually:

- `core`;
- `platform`;
- `content`;
- `simulation`;
- `world`;
- `gameplay`;
- `physics`;
- `render`;
- `audio`;
- `ui`;
- `persistence`;
- `tools`;
- `app`.

Exact CMake target names will be finalized in the implementation roadmap, but dependency direction follows this structure.

## 4. Core

`core` owns low-level project primitives with no gameplay/render dependency:

- typed IDs;
- result/error types;
- assertions/contracts;
- logging facade;
- deterministic RNG primitives;
- time/tick value types;
- math aliases/helpers;
- serialization primitives/interfaces;
- generic small utilities actually required by the game.

`core` must remain small.

## 5. Platform

`platform` owns OS/window/device interaction:

- GLFW initialization;
- window creation;
- OpenGL context creation;
- raw keyboard/mouse/controller events;
- clipboard/file-dialog hooks if later required;
- high-resolution real-time clock for profiling/frame pacing;
- filesystem/platform paths.

It does not translate raw input into gameplay actions.

## 6. Content

`content` owns authored data definitions and validation:

- resource/item definitions;
- station module definitions;
- ship hull/module definitions;
- robot definitions/components;
- weapon/ammunition definitions;
- recipes/research definitions;
- mission templates;
- faction/world data;
- tuning/configuration data;
- asset references.

Content data is immutable during normal gameplay after load, except development hot-reload functionality explicitly supported later.

## 7. Simulation

`simulation` owns cross-cutting authoritative runtime infrastructure:

- SimulationClock;
- fixed-tick scheduler;
- stable event ordering;
- command/transaction execution;
- deterministic RNG stream ownership;
- domain update ordering;
- Stable Simulation Boundary;
- pause state;
- strategic timer scheduling.

It does not own domain-specific gameplay rules.

## 8. World

`world` owns active local-world runtime representation:

- runtime entity registry;
- transforms;
- local scene/zone activation;
- spatial queries/culling-facing scene data;
- spawn/despawn activation from persistent state;
- local environment state bridge;
- runtime handles;
- scene transition lifecycle.

Persistent domain identity remains owned by gameplay/persistence stores.

## 9. Gameplay

`gameplay` is a modular collection of domain targets/namespaces, not one monolithic class.

Submodules mirror GDS authority where useful:

- player;
- station;
- crew;
- resources;
- spacecraft;
- missions;
- combat;
- robots;
- raids;
- world-state/factions;
- economy;
- progression;
- dynamic events;
- communications/recovery.

Each gameplay domain owns its persistent/authoritative data and commands.

## 10. Physics

`physics` owns the StarForge-facing physics interface and Jolt adapter:

- rigid body creation/destruction;
- character collision support;
- collision layers/filters;
- ray/shape queries;
- contacts;
- trigger volumes;
- kinematic/static/dynamic bodies;
- fixed-step integration.

It does not own GDS damage, mission, docking, structural, or health semantics.

## 11. Render

`render` owns all OpenGL work:

- context-visible GPU resources;
- shaders/programs;
- buffers/VAOs;
- textures/samplers;
- framebuffers;
- render passes;
- cameras;
- materials;
- lighting/shadows;
- instancing;
- particles/VFX rendering;
- debug rendering;
- GPU profiling.

The renderer consumes immutable/submitted render data from gameplay/world/presentation.

## 12. Audio

`audio` owns:

- miniaudio backend;
- sound asset handles;
- emitters/listener;
- buses/categories;
- spatialization parameters;
- music/state transitions;
- accessibility mixes;
- voice/subtitle timing bridge where required.

Gameplay emits semantic audio cues; audio decides playback representation.

## 13. UI

`ui` owns player-facing presentation state and input routing for UI contexts:

- HUD;
- menus;
- station management;
- inventory;
- mission/raid planning;
- maps;
- subtitles/captions;
- accessibility presentation;
- interaction prompts.

Gameplay state is queried through read models/views.

UI sends commands/intents; it does not mutate gameplay stores directly.

Dear ImGui belongs only to `tools`/development UI, not the canonical shipping gameplay UI.

## 14. Persistence

`persistence` owns:

- save snapshot assembly;
- schema versions;
- serialization/deserialization;
- integrity checks;
- write-new-then-commit file strategy;
- save-slot metadata;
- migrations;
- deterministic restoration;
- crash-safe file replacement.

Gameplay domains remain owners of the meaning of their data and provide explicit serialization adapters/state DTOs.

## 15. Tools

`tools` exists only in development/editor-enabled builds and owns:

- Dear ImGui diagnostics;
- entity/state inspector;
- station graph visualizer;
- physics debug draw;
- render stats;
- transaction/event trace;
- content validation reports;
- deterministic seed controls for test/dev contexts.

Tools cannot be required for normal game progression.

## 16. App

`app` is the composition root.

It owns:

- startup/shutdown ordering;
- module construction;
- main loop;
- high-level scene/session state;
- dependency wiring;
- command-line/dev launch configuration.

It must not absorb domain rules simply because it can access every subsystem.

## 17. Dependency Direction

Intended direction:

`core`

→ `platform`, `content`

→ `simulation`, `physics`, `render`, `audio`

→ `world`

→ `gameplay`

→ `ui`

→ `app`

This is conceptual, not permission for every layer to depend on every lower layer.

## 18. Render/Physics Independence

`gameplay` may depend on StarForge-owned render/physics-facing interfaces/value types where required, but may not include backend-specific OpenGL/Jolt types.

Backend implementation targets depend inward toward their interfaces rather than leaking outward.

## 19. Active Local Context

At runtime there is at most one **Active Local Context** containing the directly simulated/rendered local zone:

- Horizon interior/exterior/local space;
- mission zone;
- raid target zone;
- local spacecraft flight zone;
- recovery/local authored context.

This follows the GDS single external deployed Mission Instance rule.

## 20. Off-Screen Horizon Context

When the Active Local Context is external, Horizon remains represented by its authoritative persistent strategic/system state.

High-detail runtime entities can be deactivated while station systems continue on lower-frequency deterministic schedules.

## 21. Activation Boundary

Persistent data becomes an active runtime representation through explicit activation:

Persistent Domain Record
→ validate
→ create RuntimeEntityHandle(s)
→ physics/render/audio proxies created as required
→ active simulation.

Deactivation performs the reverse:

active runtime state
→ commit relevant persistent state
→ destroy transient proxies
→ release RuntimeEntityHandle.

No persistent identity is created merely by loading a mesh.

## 22. Domain Communication

Preferred communication forms:

- direct read-only query through stable interface;
- typed command to owning domain;
- typed committed event to known consumers;
- immutable snapshot/read model for UI/render/debug.

Avoid circular callbacks and mutable cross-domain references.

## 23. Cross-Domain Transaction Coordinator

Transactions touching multiple authoritative owners use a small explicit coordinator/transaction object.

Examples:

- trade: Credits ledger + physical inventory;
- extraction: mission state + inventory security + zone transition;
- robot commissioning: manufacturing + resource ownership + RobotId creation;
- raid theft: station inventory + raider cargo + extraction state.

The coordinator validates all participants before commit and emits one transaction result.

## 24. Service Locator Rule

A global mutable service locator is prohibited for gameplay systems.

Dependencies are passed through constructors/setup interfaces or narrow context objects owned by the composition root.

## 25. Singleton Rule

Process-unique technical services can have one owned instance, but not hidden global lifetime.

Examples:

- Renderer;
- AudioSystem;
- PhysicsSystem;
- AssetManager;
- PersistenceService.

The `App` composition root owns them explicitly.

## 26. Explicit Non-Goals

This module structure does not create:

- a separate engine repository;
- dynamically loadable game modules;
- arbitrary game-editor plugin support;
- a server/client split;
- gameplay rules inside render/physics backends;
- persistence through raw memory dumps.