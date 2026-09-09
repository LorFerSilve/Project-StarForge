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


---

## DD-010 — Single-Player Is the Complete Core Game

**Status:** Accepted

### Decision

Project StarForge is designed so that all core gameplay and progression function completely in single-player.

Synchronous multiplayer is not a dependency of the initial product.

### Rationale

The project remains technically achievable for a custom C++/OpenGL codebase while preserving the option to explore asynchronous multiplayer later.

---

## DD-011 — On-Foot Gameplay Is First-Person

**Status:** Accepted

### Decision

Normal on-foot gameplay uses a first-person perspective.

Special construction, navigation, or strategic interfaces may use dedicated cameras.

### Rationale

First-person supports direct station presence, exploration, combat, engineering interaction, and a manageable animation/content scope.

---

## DD-012 — No Real-World Offline Progression

**Status:** Accepted

### Decision

Game-world production, research, farming, attacks, and other simulations do not continue simply because real-world time passes while the game is closed.

### Rationale

Progression should result from active play and in-game systems rather than mobile-style waiting mechanics.

---

## DD-013 — Recruited Crew Do Not Routinely Die Permanently

**Status:** Accepted

### Decision

Ordinary failures and automated combat do not permanently delete recruited crew.

Crew may be injured or incapacitated. Permanent death requires an explicitly authored narrative case or a later accepted design change.

### Rationale

Crew members are persistent progression assets intended to create attachment and station identity rather than function as expendable units.

---

## DD-014 — Automation Removes Solved Repetition

**Status:** Accepted

### Decision

Routine station operations can become progressively automated, while strategic choices remain under player control.

### Rationale

The player's role should evolve from hands-on operator to commander without turning progression into repeated maintenance of already-solved tasks.

---

## DD-015 — The Galaxy Does Not Universally Scale to the Player

**Status:** Accepted

### Decision

Locations and enemies have intended threat ranges rather than automatically matching player power everywhere.

### Rationale

Fixed and bounded threat structure preserves meaningful danger, progression, preparation, and the feeling that previously difficult content has become easier through advancement.


---

## DD-016 — Crew Quality Is Not Represented by Loot Rarity

**Status:** Accepted

### Decision

Recruited crew does not use generic Common, Rare, Epic, or Legendary tiers as the primary measure of character quality.

Crew value comes from profession, skills, traits, background, special knowledge, and earned progression.

### Rationale

Crew members are intended to feel like persistent people and specialists rather than collectible loot cards.

---

## DD-017 — Crew Has Finite Assignment Workload

**Status:** Accepted

### Decision

A crew member has one Primary Assignment at a time and finite workload capacity.

Higher skill can improve supervision capacity, but no specialist can provide unlimited coverage.

### Rationale

This preserves meaningful staffing, redundancy, specialization, and emergency tradeoffs as the station grows.

---

## DD-018 — Crew Needs Are Station-Level, Not Life-Sim Micromanagement

**Status:** Accepted

### Decision

Crew requires habitation, food, water, breathable atmosphere, safe temperature, and medical support, but the game does not require individual hunger, thirst, happiness, or minute-by-minute sleep management.

### Rationale

Crew support should reinforce station engineering and population planning without turning Project StarForge into a detailed social-survival simulator.

---

## DD-019 — Crew Progresses Through Skills and Training, Not a Generic Crew Level

**Status:** Accepted

### Decision

Crew competence progresses through skill-specific experience, training, mentorship, and special knowledge.

There is no generic crew level that replaces professional skill.

### Rationale

Skill-specific progression preserves meaningful specialization and makes a crew member's history of work mechanically legible.
