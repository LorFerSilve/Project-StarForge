# Design Decisions

> **Status:** Draft  
> **Purpose:** Record major accepted design decisions and their rationale.

This file records decisions whose rationale is important to preserve.

Detailed mechanics remain authoritative in their subsystem documents.

---

## DD-001 — The Home Station Is a Physical 3D Location

**Status:** Accepted

### Decision

The player's home station is physically traversable in 3D and is not represented solely as a strategic menu.

### Rationale

The station is intended to create ownership, spatial memory, engineering gameplay, visible growth, and direct interaction with crew and infrastructure.

---

## DD-002 — Station Infrastructure Is Functionally Connected

**Status:** Accepted

### Decision

Critical station functions such as power, atmosphere, thermal management, logistics, production, and defenses operate through connected systems.

Station modules are not designed as isolated buildings that function independently simply because they exist.

### Rationale

This enables meaningful engineering, redundancy, sabotage, automation, defensive layout, and emergent failure behavior.

---

## DD-003 — Survivors Become Functional Persistent Crew

**Status:** Accepted

### Decision

Survivors encountered during expeditions can become recruitable persistent station crew.

Crew members can perform specialized work and progressively automate station operations.

### Rationale

This connects exploration directly to home-station development and gives rescued characters lasting mechanical and spatial presence.

---

## DD-004 — Planetary Gameplay Uses Bounded Mission Zones

**Status:** Accepted

### Decision

Planetary and lunar gameplay uses bounded mission environments rather than requiring seamless full-scale planetary simulation.

### Rationale

The core gameplay benefits from authored or selectively procedural mission spaces, while seamless planetary simulation would impose disproportionate technical and content costs.

---

## DD-005 — Robot Combat Uses Tactical Squads, Not Massive RTS Armies

**Status:** Accepted

### Decision

The player deploys limited robot squads with understandable tactical commands while personally participating in combat.

### Rationale

This preserves direct action gameplay, makes AI complexity manageable, and provides strategic composition without turning the project into a full large-scale RTS.

---

## DD-006 — Propulsion Unlocks Strategic Access

**Status:** Accepted

### Decision

Spacecraft propulsion progression affects which regions and mission opportunities the player can reach.

Engine upgrades are therefore progression-enabling technology rather than only movement-speed upgrades.

### Rationale

This ties spacecraft engineering directly to exploration and long-term progression.

---

## DD-007 — The Project Does Not Build a General-Purpose Engine First

**Status:** Accepted

### Decision

Technical systems are implemented when approved game requirements need them.

The project does not attempt to complete a reusable general-purpose engine before building the game.

### Rationale

This reduces speculative complexity and keeps C++/OpenGL development aligned with actual gameplay needs.

---

## DD-008 — Design Precedes Gameplay Implementation

**Status:** Accepted

### Decision

Gameplay implementation begins only after the relevant design specification is sufficiently complete to prevent implementation-time invention of intended behavior.

### Rationale

The project aims to resolve gameplay rules, interactions, edge cases, presentation requirements, and system relationships deliberately before implementation.

---

## DD-009 — Numerical Balance Is Not Required to Be Final Before Implementation

**Status:** Accepted

### Decision

Fixed gameplay rules must be specified before implementation, but values explicitly classified as tuneable balancing parameters may change during testing.

### Rationale

Some values cannot be validated without playable feedback. Separating rules from parameters preserves design authority without preventing iteration.
