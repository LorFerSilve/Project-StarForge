# Game Design Specification Roadmap

> **Status:** Active  
> **Authority:** Game-design documentation sequencing

This roadmap defines the dependency-driven order in which Project StarForge's authoritative Game Design Specification is developed and matured.

## GDS-0 — Governance, Structure, and Historical Baseline

**Status:** Complete

Established:

- design authority;
- specification template;
- glossary;
- design decision log;
- thematic documentation hierarchy;
- preserved original concept baseline;
- structure-completeness audit.

## GDS-1 — Global Game Rules

**Status:** Design Complete — GDS-14 Promoted

Defines project-wide invariants including:

- perspectives;
- primary game mode;
- simulation/pause;
- failure philosophy;
- crew/ship/station persistence;
- survival scope;
- automation;
- mission instancing;
- progression;
- save/persistence;
- difficulty;
- multiplayer scope;
- communication/recovery constraints.

GDS-14 corrected the time and defeat-recovery wording without changing the project's single-player/no-development scope.

## GDS-2 — Home Space Station

**Status:** Design Complete — GDS-14 Promoted

Authoritative domain for:

- station identity/lifecycle;
- construction;
- structure;
- power;
- atmosphere/oxygen;
- thermal management;
- water;
- farming/food;
- storage/logistics;
- manufacturing;
- damage/repair;
- automation;
- defenses;
- security;
- station events.

## GDS-3 — Survivors and Crew

**Status:** Design Complete — GDS-14 Promoted

Authoritative domain for:

- survivor encounters/recruitment;
- professions;
- skills/traits;
- assignments/workload;
- automation responsibilities;
- needs;
- progression;
- injury/incapacitation;
- physical station presence.

## GDS-4 — Resources, Processing, Crafting, and Research

**Status:** Design Complete — GDS-14 Promoted

Defines:

- physical resource taxonomy/ownership;
- acquisition/mining/salvage;
- processing;
- production chains;
- Blueprints;
- crafting;
- Research/Evidence.

## GDS-5 — Player Character

**Status:** Design Complete — GDS-14 Promoted

Defines:

- movement;
- interaction;
- controls;
- health/incapacitation;
- inventory;
- equipment;
- tools;
- field survival.

GDS-14 added a cross-cutting Recovery Transit/destination authority and reconciled Player Health to it.

## GDS-6 — Spacecraft

**Status:** Design Complete — GDS-14 Promoted

Defines:

- ship classes/configuration;
- ship systems;
- flight;
- propulsion/Reach;
- navigation;
- docking/hangars;
- cargo/utility;
- combat integration;
- damage/repairs/recovery.

## GDS-7 — World, Galaxy, Narrative, and Factions

**Status:** Design Complete — GDS-14 Promoted

Defines:

- setting/geography;
- star systems/locations;
- factions/reputation;
- Fracture lore;
- narrative structure;
- main story;
- characters;
- Stabilize/Sever/Contain endings.

## GDS-8 — Missions and Exploration

**Status:** Design Complete — GDS-14 Promoted

Defines:

- mission lifecycle/generation;
- zones;
- objectives;
- exploration/hazards;
- extraction;
- rewards;
- failure/abandonment;
- story mission execution.

## GDS-9 — Combat

**Status:** Design Complete — GDS-14 Promoted

Defines:

- physical hit resolution;
- Damage Packets/channels;
- weapons/ammunition;
- armor/shields;
- status effects;
- on-foot combat;
- enemy combat behavior;
- spacecraft combat resolution;
- combat-objective semantics.

## GDS-10 — Robots

**Status:** Design Complete — GDS-14 Promoted

Defines:

- manufacturing;
- classes/components;
- AI;
- TCC/squads;
- commands/ROE;
- damage/repair/loss;
- station robots.

## GDS-11 — Raids and Station Defense

**Status:** Design Complete — GDS-14 Promoted

Defines:

- intelligence/preparation;
- external assault;
- boarding/breaching;
- interior attack;
- sabotage;
- physical looting/theft;
- reinforcements/escalation;
- withdrawal/extraction;
- Horizon Station defense resolution.

## GDS-12 — Economy, Progression, Difficulty, and Cross-Cutting Systems

**Status:** Design Complete — GDS-14 Promoted

Defines:

- Credits/trading/economy;
- loot/reward coordination;
- faction-reputation cross-system consequences;
- P0-P5 capability progression;
- early/mid/late/endgame pacing;
- TCC progression;
- finale readiness;
- Difficulty;
- Dynamic Events;
- Simulation Time;
- persistence/Stable Save Boundaries;
- failure/recovery;
- multiplayer position.

### GDS-14 Gap-Closure Additions

GDS-14 identified and created authoritative homes for:

- Horizon Recovery State / Critical Recovery / Recovery Grace;
- strategic communication and remote control;
- external-defeat Recovery Transit and destination resolution.

It also reconciled Time/Dynamic Event semantics so Simulation Time is the sole gameplay-progression clock.

## GDS-13 — Presentation, Onboarding, and Accessibility

**Status:** Design Complete — GDS-14 Promoted

Defines:

- visual/environment/station/faction language;
- UI/UX;
- adaptive HUD;
- interaction feedback;
- VFX;
- audio;
- alarms/signaling;
- onboarding/tutorials;
- accessibility.

GDS-14 validates that critical cross-system states remain readable under color-independent, audio-reduced, reduced-motion/effects, subtitle/caption, UI-scale, remapped-input, navigation-assistance, and bounded aim-assist configurations.

## GDS-14 — Cross-System Consistency Audit

**Status:** Complete — Design Complete Promotion Passed

GDS-14 audited the complete first-pass specification across domains.

### Audit Artifacts

See [`audit/`](audit/):

- authority/namespace audit;
- 25 compound scenario audit;
- transaction/persistence audit;
- presentation/accessibility audit;
- specification-maturity audit;
- final audit report.

### Blocking Findings Discovered

GDS-14 found five implementation-relevant cross-domain issues:

1. mixed Active Game Time / Simulation Time gameplay authority;
2. undefined Horizon Critical Recovery / Recovery Grace trigger;
3. missing unified strategic communication / remote-control authority;
4. external-defeat recovery ambiguity and potential defeat-as-fast-travel exploit;
5. unqualified repeated priority/state labels such as P0/P1, Critical, and Disabled.

All five cross-system problems now have authoritative resolutions.

### Cross-System Result

- authority ownership: PASS;
- compound scenarios: PASS;
- transaction/persistence: PASS;
- failure/softlock: PASS;
- economy/progression/difficulty: PASS;
- presentation/accessibility: PASS;
- cross-system orphan mechanics: PASS after corrections.

### Formal Maturity Result

**PASS.**

All **156 authoritative owning specifications** were traversed, normalized against current cross-domain contracts, checked for implementation-critical open questions and stale dependencies, and promoted to `Design Complete` only after the hard maturity gate passed.

### GDS-14 Closure Action — Design Complete Promotion Sweep

**COMPLETE.**

The sweep performed the required per-spec traversal, gameplay-time and dependency normalization, semantic ambiguity review, status promotion, domain synchronization, and regression validation. Evidence is recorded in [`audit/DESIGN_COMPLETE_PROMOTION_REPORT.md`](audit/DESIGN_COMPLETE_PROMOTION_REPORT.md).

## Current Project Gate

The complete authoritative GDS is **Design Complete**. The design-governance blocker that previously prohibited technical handoff is closed.

The next project action is **technical architecture**, followed by an implementation roadmap. This status does not claim that gameplay implementation has already started.

## After Formal GDS Completion

The project may now proceed, in order, to:

1. technical architecture;
2. implementation roadmap;
3. C++/OpenGL project scaffolding;
4. gameplay implementation.

`Implementation Locked` remains a later governance status applied when a Design Complete contract is explicitly handed to implementation and change-control expectations are active.
