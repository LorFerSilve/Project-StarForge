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

---

## DD-020 — Physical Resources Have Single Authoritative Ownership

**Status:** Accepted

### Decision

Every physical resource quantity exists at exactly one authoritative physical owner/location at any moment.

Reservation and UI aggregation do not duplicate ownership.

### Rationale

This prevents resource duplication and keeps mission loot, ship cargo, station storage, manufacturing, and logistics mechanically consistent.

---

## DD-021 — Materials Do Not Use Loot-Rarity Quality Tiers

**Status:** Accepted

### Decision

Canonical resources do not have Common, Rare, Epic, or Legendary quality variants.

Scarcity comes from deposit richness, location, acquisition difficulty, processing requirements, and technology access.

### Rationale

This avoids inventory clutter and preserves readable industrial production chains.

---

## DD-022 — Research Uses Persistent Evidence, Not Spendable Science Points

**Status:** Accepted

### Decision

Research progression is gated by persistent non-consumable Research Evidence, unique discoveries, laboratory work, and prerequisites rather than a universal spendable Science Point currency.

### Rationale

Exploration discoveries remain meaningful and research feels like accumulated knowledge rather than another generic resource bar.

---

## DD-023 — Blueprint Knowledge Is Distinct From Technology Research

**Status:** Accepted

### Decision

Research determines whether the player understands an underlying technology, while Blueprints determine whether the player possesses a specific design.

Possessing one does not automatically imply possession of the other.

### Rationale

This creates clearer progression and supports discovery, reverse engineering, faction rewards, and research without collapsing every unlock into one tree.

---

## DD-024 — Production Chains Must Prevent Infinite Material Loops

**Status:** Accepted

### Decision

Recycling and transformation chains may recover resources but cannot create net free physical material through closed loops.

### Rationale

Station automation should create efficiency and self-sufficiency, not exploitable resource duplication.

---

## DD-025 — Player Progression Is Capability-Based, Not Level-Based

**Status:** Accepted

### Decision

The player has no generic Player Level or universal RPG attribute tree.

Direct player progression comes primarily from equipment, tools, research, station capability, spacecraft, robots, and preparation.

### Rationale

StarForge's progression should unlock new capabilities and strategic options rather than rely on invisible percentage inflation.

---

## DD-026 — Normal Sprinting Does Not Use a Stamina Bar

**Status:** Accepted

### Decision

Ordinary on-foot sprinting is not limited by a generic regenerating stamina resource.

Movement limitations come from environment, carried load, health, equipment, gravity, and explicit combat effects.

### Rationale

Routine traversal should remain responsive; survival pressure belongs in meaningful engineering and environmental systems.

---

## DD-027 — Player Inventory Uses Both Mass and Volume Capacity

**Status:** Accepted

### Decision

Portable backpack storage is constrained by both carried Mass and occupied Volume.

Equipped gear contributes to carried Mass, and a Heavy Load state can begin below hard maximum capacity.

### Rationale

This prevents unrealistic ore hauling while distinguishing dense and bulky cargo without requiring a grid-placement minigame.

---

## DD-028 — Ordinary Mission Defeat Uses a Deterministic Loadout/Loot Transaction

**Status:** Accepted

### Decision

On ordinary external-mission defeat, recovered Secured Loadout equipment and unconsumed pre-mission supplies return with the player, consumed supplies remain consumed, Field-Unsecured mission loot is lost, and already Vehicle/Extraction-Secured cargo remains secured.

### Rationale

This creates meaningful extraction risk without deleting the player's established equipment and prevents duplication/refund exploits.

---

## DD-029 — Suit Energy and Life-Support Reserve Are Separate Resources

**Status:** Accepted

### Decision

Player environmental equipment distinguishes electrical Suit Energy from finite Life-Support Reserve.

Loss of Suit Energy disables active systems according to priority but does not automatically erase all remaining breathable-atmosphere endurance if the suit supports passive emergency life support.

### Rationale

Separating electrical energy from atmosphere endurance creates clearer engineering decisions and avoids arbitrary instant death at battery depletion.

---

## DD-030 — Spacecraft Use Authored Hull Frames With Modular Fitting

**Status:** Accepted

### Decision

Player spacecraft use authored hull variants with defined internal bays, external hardpoints, docking geometry, mass envelopes, and modular equipment slots.

The baseline does not use unrestricted voxel/block ship construction.

### Rationale

This preserves meaningful customization while keeping flight collision, interiors, damage zones, docking, AI, and presentation tractable for the custom C++/OpenGL project.

---

## DD-031 — Local Flight and Strategic Travel Are Separate Simulation Scales

**Status:** Accepted

### Decision

Spacecraft use real-time local flight inside bounded gameplay zones and explicit strategic travel between galaxy locations.

The game does not require seamless manual flight across astronomical distances.

### Rationale

This preserves direct piloting while keeping galaxy scale technically and mechanically manageable.

---

## DD-032 — Strategic Reach Classes Gate Galaxy Access

**Status:** Accepted

### Decision

Propulsion systems provide Strategic Reach Classes I–IV, and galaxy routes declare minimum Reach requirements.

### Rationale

Propulsion progression directly unlocks new locations and mission opportunities instead of functioning only as a speed upgrade.

---

## DD-033 — Fuel and Propellant Are Distinct Spacecraft Resources

**Status:** Accepted

### Decision

Spacecraft explicitly distinguish energy-producing Fuel from momentum-producing Propellant whenever a drive architecture requires both.

### Rationale

The distinction creates clearer propulsion engineering, refueling decisions, cargo/mass tradeoffs, and avoids an ambiguous universal fuel bar.

---

## DD-034 — Established Player Ships Are Persistent Assets

**Status:** Accepted

### Decision

Ordinary mission defeat does not permanently delete the player's established Primary Ship.

Ships may be damaged, disabled, stranded, or recovered. Permanent ship loss requires an explicit high-stakes rule or player-authorized scrapping/abandonment.

### Rationale

Ships are long-term configurable progression assets analogous to the home station and recruited crew, not disposable lives.

---

## DD-035 — The Game Begins 67 Years After The Fracture

**Status:** Accepted

### Decision

Project StarForge begins in 67 AF (After Fracture), in a fragmented but recovering interstellar human civilization.

### Rationale

A fixed historical era provides consistent technological loss, faction emergence, survivor context, and infrastructure decay without requiring total civilizational extinction.

---

## DD-036 — The Fracture Has a Multi-Causal Canonical Explanation

**Status:** Accepted

### Decision

The Fracture resulted from interacting anomalous route physics, Transit Lattice coupling, human emergency intervention, conflicting autonomous containment directives, and subsequent institutional suppression.

No single villain or faction caused the entire event.

### Rationale

This preserves the mystery while supporting faction disagreement, technical investigation, and a more credible systemic disaster.

---

## DD-037 — The Campaign Uses a Fixed Geographic Backbone

**Status:** Accepted

### Decision

The base campaign uses fixed named sectors, systems, faction hubs, story locations, and Reach I–IV routes defined in `world/canonical_region_map.md`.

Procedural side content may expand around that backbone but cannot randomize away mandatory campaign geography.

### Rationale

Mission design should not have to invent or relocate core world structure during implementation.

---

## DD-038 — Major Human Factions Use Independent Reputation

**Status:** Accepted

### Decision

Helios Directorate, Meridian Compact, Asterion Institute, Freehold Coalition, and Blackwake Corsairs use separate -100 to +100 player reputation values.

The Continuance uses node/authentication states instead of human-style reputation.

### Rationale

Faction relations should represent demonstrated history with each group rather than one global morality score.

---

## DD-039 — The Main Campaign Uses Convergent Branching

**Status:** Accepted

### Decision

Faction choices, mission approaches, relationships, and local outcomes may branch, while the core Fracture investigation converges on a shared late-game crisis.

### Rationale

This preserves meaningful agency while keeping campaign scope achievable and coherent.

---

## DD-040 — The Finale Offers Stabilize, Sever, or Contain

**Status:** Accepted

### Decision

The Second Cascade is resolved through one of three strategic outcomes: Stabilize, Sever, or Contain.

There is no hidden fourth ending that removes all tradeoffs.

### Rationale

The ending should reflect the game's themes of connectivity, safety, autonomy, and knowledge rather than collapse into a single objectively perfect answer.

---

## DD-041 — Only One External Mission Instance Can Be Deployed at a Time

**Status:** Accepted

### Decision

The player may accept multiple missions but only one external Deployed Mission Instance can be active at a time.

### Rationale

This keeps player location, deployed ship, mission inventory security, zone persistence, and extraction ownership unambiguous.

---

## DD-042 — Procedural Missions Use Persistent IDs and Stable Seeds

**Status:** Accepted

### Decision

Once a procedural mission is created, its Mission ID, seed, major objectives, survivor identities, major loot, hazard schedule, and generated layout remain stable for that mission.

Reloading or reopening the offer does not reroll them.

### Rationale

Mission generation should create replayability, not save-scumming or survivor/loot reroll exploits.

---

## DD-043 — Mission Threat Uses Four Independent 0–5 Axes

**Status:** Accepted

### Decision

Mission threat is represented through Hostile Threat, Environmental Threat, Operational Complexity, and Extraction Risk on a fixed 0–5 scale.

Overall Threat equals the highest known axis rather than an average.

### Rationale

A single extreme danger must remain visible instead of being hidden inside an averaged difficulty score.

---

## DD-044 — Extraction Is an Explicit Security Transaction

**Status:** Accepted

### Decision

Mission-acquired field resources are not permanently secured on pickup.

Successful extraction explicitly transitions eligible Field-Unsecured holdings to Vehicle/Extraction-Secured, while Station-Secured still requires actual station ownership/transfer.

### Rationale

This makes extraction mechanically meaningful while preserving the single-authoritative-owner resource model.

---

## DD-045 — Field Acquisition and Mission Resolution Rewards Are Separate

**Status:** Accepted

### Decision

Resources and items physically acquired during a mission are governed by field ownership/extraction.

Mission completion compensation such as reputation, Blueprint unlocks, Research Evidence, access, economic entitlements, or sponsor-provided physical rewards is resolved separately.

### Rationale

This prevents duplicated loot, weightless reward insertion, and ambiguity between what the player found and what a sponsor grants.

---

## DD-046 — Main-Story Missions Use the Same Mission Framework as Other Expeditions

**Status:** Accepted

### Decision

The canonical GDS-7 main-story missions use GDS-8 mission lifecycle, zones, objectives, extraction, failure, and persistence rules.

Narrative importance does not create an unrelated special mission engine.

### Rationale

A single mission framework reduces bespoke exceptions and keeps story gameplay consistent with the systemic game.

---

## DD-047 — Combat Uses Physical Hit Resolution, Not RPG Hit Rolls

**Status:** Accepted

### Decision

Ranged and melee attacks resolve through traces, projectiles, collision volumes, cover, and actual target hit zones.

There is no secondary hidden hit-chance roll after physical intersection.

### Rationale

StarForge combat should reward aim, positioning, movement, and physical world understanding rather than invisible probability.

---

## DD-048 — Critical Damage Comes From Authored Weak Points

**Status:** Accepted

### Decision

The baseline has no random critical-hit chance.

Critical damage comes from hitting a defined head, weak point, exposed component, or vulnerable subsystem.

### Rationale

Critical performance becomes skill- and knowledge-based rather than luck-based.

---

## DD-049 — Combat Uses Explicit Damage Channels and Deterministic Protection

**Status:** Accepted

### Decision

Combat damage uses Kinetic, Thermal, Explosive, Electrical/EMP, and Corrosive channels.

Protection resolves in deterministic order through Shield, Armor/Protection, and target-specific Health/System/Structure.

### Rationale

This creates understandable weapon/protection tradeoffs and reusable rules for people, machines, ships, and stations.

---

## DD-050 — Ammunition Is Finite and Physically Conserved

**Status:** Accepted

### Decision

Weapons consume finite ammunition or finite energy.

Reloading transfers authoritative ammunition ownership into weapon state and cannot create or refund rounds.

### Rationale

Ammunition preparation, logistics, crafting, and mission failure should remain materially connected to the resource system.

---

## DD-051 — Weapons Progress Through Technology and Configuration, Not Loot Rarity

**Status:** Accepted

### Decision

Weapons are differentiated through model, technology, firing behavior, ammunition, modifications, condition, and manufacturing access.

The baseline does not use random Common/Rare/Epic/Legendary weapon stat tiers.

### Rationale

This preserves Project StarForge's capability-based engineering progression and avoids generic looter-shooter inflation.

---

## DD-052 — Enemy Combat AI Cannot Use Omniscient Knowledge

**Status:** Accepted

### Decision

Enemy combat decisions use perceived or communicated information through vision, hearing, sensors, alerts, and last-known-position memory.

Enemies do not continuously know the player's hidden current position.

### Rationale

Combat difficulty should come from credible tactics and capability rather than AI cheating.

---

## DD-053 — Player Robots Are Physical Persistent Manufactured Units

**Status:** Accepted

### Decision

Player-owned robots have persistent Robot IDs, physical locations, components, energy, condition, and deployment state.

They are not abstract troop cards or instant-summon effects.

### Rationale

This keeps robots consistent with the station, resource, spacecraft, mission, and physical-world simulation.

---

## DD-054 — Robots Can Be Permanently Destroyed Through Ordinary Gameplay

**Status:** Accepted

### Decision

Unlike recruited human crew, robots do not receive routine permanent-loss protection.

A Destroyed robot's original Robot ID cannot be repaired back into service; replacement requires a new manufactured/acquired unit.

### Rationale

Robots are the repeatable tactical force and must carry meaningful manufacturing, recovery, and attrition consequences without making human survivors disposable.

---

## DD-055 — Robot Deployment Uses Separate Command and Transport Constraints

**Status:** Accepted

### Decision

Active field robots are limited by both Tactical Control Capacity and actual spacecraft transport/support capacity.

A robot can fit physically but exceed command capacity, or fit command capacity but lack transport.

### Rationale

This creates strategic squad composition and ship-design tradeoffs without one arbitrary global troop-count cap.

---

## DD-056 — Tactical Control Capacity Is Capability, Not Currency

**Status:** Accepted

### Decision

Each robot has Command Load and the active squad must remain within available Tactical Control Capacity.

TCC is a command-system capability and is not spent or traded as a physical/economic resource.

### Rationale

Command complexity should limit simultaneous deployment without creating another artificial progression currency.

---

## DD-057 — Robot AI Uses Bounded Autonomy and Explicit Fallback Orders

**Status:** Accepted

### Decision

Player robots execute direct commands and role autonomy using only sensed/communicated information.

When disconnected, they follow a preconfigured fallback rather than receiving magical live orders.

### Rationale

This makes communication, EMP, positioning, and control infrastructure mechanically meaningful while preventing omniscient companion AI.

---

## DD-058 — Robot Progression Is Component-Based, Not Level/Rarity-Based

**Status:** Accepted

### Decision

Robot capability advances through chassis, hardware components, software packages, weapons/tools, Research, Blueprints, and configuration.

The baseline does not use generic Robot Levels or Common/Rare/Epic/Legendary robot tiers.

### Rationale

This preserves StarForge's engineering-oriented capability progression and keeps robot strengths explainable through physical design.

---

## DD-059 — Raids Are Specialized Missions, Not a Second Mission System

**Status:** Accepted

### Decision

Offensive raids use GDS-8 Mission identity, objectives, persistence, extraction, failure, and reward transactions.

GDS-11 adds fortified-target phases without creating a parallel raid-only mission engine.

### Rationale

One mission framework keeps story, persistence, extraction, and objective semantics coherent.

---

## DD-060 — Raid Targets Preserve Committed Physical State

**Status:** Accepted

### Decision

Committed raid damage, breaches, depleted storage, stolen unique objects, and persistent sabotage remain part of the target's world state until an explicit recovery/replenishment process changes them.

A resolved target does not immediately reset to a pristine farmable copy.

### Rationale

Persistent consequences make sabotage and looting meaningful and prevent reload/re-entry farming exploits.

---

## DD-061 — Raid Loot and Theft Are Physical Ownership Transfers

**Status:** Accepted

### Decision

The player and hostile raiders must physically access, carry, transfer, and extract loot.

Horizon Station resources are permanently stolen only when the hostile extraction owner successfully departs with them.

### Rationale

This preserves the single-authoritative-owner resource model and turns logistics/extraction into real raid gameplay.

---

## DD-062 — Raid Reinforcements Are Finite and Require a Valid Source

**Status:** Accepted

### Decision

Reinforcements require an actual local/world source, successful call/activation, valid route, and finite force budget.

The raid system does not create endless punishment waves simply because an operation takes a long time.

### Rationale

Escalation should reward intelligence and sabotage while remaining physically credible and deterministic.

---

## DD-063 — Off-Screen Horizon Defense Uses Real Systems, Not a Defense Score

**Status:** Accepted

### Decision

When Horizon Station is attacked while the player is away during active gameplay, lower-detail resolution must still derive from actual defenses, shields, power, cooling, ammunition, security, crew, robots, geometry, and attacker capabilities.

No single opaque Defense Score determines the outcome.

### Rationale

Station construction and redundancy must remain mechanically meaningful whether the player is physically present or not.

---

## DD-064 — Routine Station Raids Cannot Delete the Core Campaign

**Status:** Accepted

### Decision

A normal dynamic raid can seriously damage Horizon Station, steal physical resources, destroy robots, compromise security, and incapacitate crew, but it cannot permanently delete Horizon Station/Command Core, erase committed Knowledge Assets, or routinely kill recruited crew.

### Rationale

Station defense should create consequential setbacks without allowing an ordinary dynamic event to hard-brick long-term campaign progression.

---

## DD-065 — Baseline Raid Gameplay Is Single-Player PvE

**Status:** Accepted

### Decision

The base game contains no synchronous or asynchronous player-versus-player station-raiding system.

Core raid and station-defense gameplay is fully implemented against systemic NPC/faction targets and attackers.

### Rationale

This preserves the achievable custom C++/OpenGL scope and avoids networking, anti-cheat, snapshot, matchmaking, and griefing requirements becoming dependencies of the core game.
