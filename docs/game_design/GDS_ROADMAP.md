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

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

Defines:

- physical resource taxonomy/ownership;
- acquisition/mining/salvage;
- processing;
- production chains;
- Blueprints;
- crafting;
- Research/Evidence.

## GDS-5 — Player Character

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

Defines:

- manufacturing;
- classes/components;
- AI;
- TCC/squads;
- commands/ROE;
- damage/repair/loss;
- station robots.

## GDS-11 — Raids and Station Defense

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** First-Pass Complete — GDS-14 Audited

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

**Status:** Audit Complete — Design Complete Promotion Pending

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

**Pending.**

Many authoritative child specifications remain explicitly `Draft` or newly `Under Review`.

Under [`00_design_authority.md`](00_design_authority.md), those files cannot silently be treated as `Design Complete`.

### Required GDS-14 Closure Action — Design Complete Promotion Sweep

Before technical architecture or implementation planning:

1. traverse every authoritative child specification;
2. resolve remaining implementation-relevant ambiguity in Draft wording;
3. remove stale future-dependency language;
4. normalize gameplay timing terminology to Simulation Time;
5. verify fixed rules vs tuneable values;
6. promote each passing authoritative spec individually to `Design Complete`;
7. run targeted regression checks after promotion edits;
8. update final domain/root status only when the child specs agree.

This is still **game-design documentation/governance**, not development.

## Current Project Gate

The project has completed the planned GDS-14 **cross-system audit**, but the complete GDS has **not yet passed formal Design Complete promotion**.

Therefore the next project action is the GDS-14 Design Complete Promotion Sweep.

## After Formal GDS Completion

Only after the promotion sweep succeeds and the relevant design is formally Design Complete may the project proceed to:

1. technical architecture;
2. implementation roadmap;
3. C++/OpenGL project scaffolding;
4. gameplay implementation.

**No gameplay development begins before that gate is satisfied.**
