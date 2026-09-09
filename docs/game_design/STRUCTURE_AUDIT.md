# Game Design Structure Audit

> **Status:** Design Complete  
> **Audit Scope:** Authoritative GDS hierarchy before subsystem expansion  
> **Audit Date:** 2026-09-09

## 1. Objective

Verify that the documentation hierarchy provides an explicit authoritative home for every major category of game-design decision before detailed subsystem specifications are created.

## 2. Result

The initial hierarchy was strong but incomplete for an authoritative specification.

The following gaps were identified and corrected or assigned:

### Narrative

**Gap:** World lore alone is insufficient for main story structure, characters, dialogue policy, narrative progression, and authored story missions.

**Resolution:** Add a dedicated `narrative/` domain.

### Controls and Input

**Gap:** Player movement did not explicitly own input mapping and control behavior.

**Resolution:** Add `player/controls_and_input.md` to the planned player domain.

### Onboarding and Tutorials

**Gap:** No authoritative home existed for teaching complex station, crafting, mission, and combat systems.

**Resolution:** Add `presentation/onboarding_and_tutorials.md`.

### Accessibility

Already planned under `presentation/`; retained as authoritative presentation-level accessibility design.

### Save and Persistence

Already planned under `systems/save_and_persistence.md`; retained as the cross-cutting authority for persistent state.

### Time and Off-Screen Simulation

Already planned under `systems/time_and_simulation.md`; global invariants are additionally defined in `02_global_game_rules.md`.

### Player Failure and Recovery

Already planned through player, mission, and cross-cutting failure documents. Global loss philosophy is defined centrally before subsystem-specific details.

### Station Persistence

Owned jointly through the station lifecycle specification for station-specific state and the cross-cutting persistence specification for save semantics.

### Economy and Trading

Already assigned to `systems/economy_and_trading.md`.

### Difficulty and Scaling

Already assigned to `systems/difficulty_and_scaling.md`.

### Multiplayer Position

Already assigned to `systems/multiplayer_position.md`; global scope is fixed in `02_global_game_rules.md`.

### Endgame

Already assigned under `progression/endgame.md`.

## 3. Domain Ownership After Audit

The authoritative domains are:

- core governance;
- world;
- narrative;
- player;
- combat;
- station;
- crew;
- spacecraft;
- resources;
- missions;
- robots;
- raids;
- progression;
- cross-cutting systems;
- presentation;
- historical archive.

## 4. Rule Ownership Principle

No new domain should be created solely because a feature is large.

A new domain is justified only when the feature owns a distinct set of authoritative rules that would otherwise be duplicated or ambiguously distributed.

## 5. Audit Conclusion

The hierarchy is sufficiently complete to begin detailed design work.

Future audits may still discover missing documents within a domain, but no currently known top-level design category lacks an authoritative home.
