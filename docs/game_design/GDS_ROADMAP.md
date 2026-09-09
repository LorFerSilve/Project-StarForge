# Game Design Specification Roadmap

> **Status:** Active  
> **Authority:** Game-design documentation sequencing

This roadmap defines the order in which Project StarForge's authoritative Game Design Specification is developed.

The sequence is dependency-driven: foundational rules are specified before systems that depend on them.

## GDS-0 — Governance, Structure, and Historical Baseline

**Status:** Complete

Deliverables:

- design authority;
- specification template;
- glossary;
- design decision log;
- thematic documentation hierarchy;
- preserved original concept baseline;
- structure-completeness audit.

Exit criteria:

- the original concept is archived;
- every major design concern has an authoritative planned home;
- documentation ownership rules are established.

## GDS-1 — Global Game Rules

**Status:** In Progress

Define project-wide rules that constrain multiple systems, including:

- gameplay perspectives;
- primary game mode;
- time and simulation;
- pause behavior;
- player failure philosophy;
- crew permanence;
- survival-system scope;
- automation philosophy;
- mission instancing;
- progression philosophy;
- save/persistence philosophy;
- difficulty philosophy;
- multiplayer scope.

Exit criteria:

- downstream subsystem designers do not need to invent conflicting global assumptions.

## GDS-2 — Home Space Station

**Status:** First-Pass Complete — Cross-Validation Pending

Authoritative specifications for:

- station identity and lifecycle;
- construction;
- structural rules;
- power;
- atmosphere and oxygen;
- thermal management;
- water;
- farming and food;
- storage and logistics;
- manufacturing;
- damage and repairs;
- automation;
- defenses;
- security;
- station events.

## GDS-3 — Survivors and Crew

**Status:** First-Pass Complete — Cross-Validation Pending

Authoritative specifications for:

- survivor encounters;
- recruitment;
- professions;
- skills and traits;
- assignments;
- automation responsibilities;
- needs;
- progression;
- injury and incapacitation;
- physical station presence.

## GDS-4 — Resources, Processing, Crafting, and Research

**Status:** First-Pass Complete — Cross-Validation Pending

Define:

- resource taxonomy;
- acquisition;
- mining;
- salvage;
- processing;
- production chains;
- blueprints;
- crafting;
- research.

## GDS-5 — Player Character

**Status:** First-Pass Complete — Cross-Validation Pending

Define:

- movement;
- interaction;
- controls;
- health;
- shields;
- damage;
- incapacitation;
- inventory;
- equipment;
- tools;
- field survival.

## GDS-6 — Spacecraft

**Status:** First-Pass Complete — Cross-Validation Pending

Define:

- ship classes;
- modular configuration;
- ship systems;
- flight;
- propulsion;
- navigation;
- docking;
- ship combat;
- damage;
- repairs;
- cargo.

## GDS-7 — World, Galaxy, Narrative, and Factions

**Status:** First-Pass Complete — Cross-Validation Pending

Define:

- setting;
- galaxy topology;
- star systems;
- planetary locations;
- space locations;
- factions;
- reputation;
- historical lore;
- narrative structure;
- main progression narrative;
- major characters.

## GDS-8 — Missions and Exploration

**Status:** First-Pass Complete — Cross-Validation Pending

Define:

- mission lifecycle;
- mission generation;
- mission zones;
- objectives;
- environmental hazards;
- extraction;
- rewards;
- failure;
- abandonment.

## GDS-9 — Combat

**Status:** First-Pass Complete — Cross-Validation Pending

Define:

- combat loop;
- damage model;
- weapons;
- armor;
- shields;
- status effects;
- enemy combat behavior.

## GDS-10 — Robots

**Status:** First-Pass Complete — Cross-Validation Pending

Define:

- manufacturing;
- robot classes;
- components;
- AI;
- squads;
- commands;
- damage;
- repair;
- station robots.

## GDS-11 — Raids and Station Defense

**Status:** Next

Define:

- intelligence;
- raid preparation;
- external attack;
- boarding;
- interior attack;
- sabotage;
- looting;
- escalation;
- extraction;
- defensive resolution.

## GDS-12 — Economy, Progression, Difficulty, and Cross-Cutting Systems

**Status:** Planned

Define:

- trading;
- economy;
- loot;
- cross-domain progression;
- difficulty;
- scaling;
- dynamic events;
- persistence;
- failure/recovery rules.

## GDS-13 — Presentation, Onboarding, and Accessibility

**Status:** Planned

Define:

- visual direction;
- station visual language;
- faction visual language;
- UI/UX;
- HUD;
- interaction feedback;
- VFX;
- audio;
- alarms;
- onboarding;
- tutorials;
- accessibility.

## GDS-14 — Cross-System Consistency Audit

**Status:** Planned

The complete specification is tested against complex scenarios involving several systems simultaneously.

Example audit scenario:

> The player is away on a mission when the home station suffers an attack, a primary power route is damaged, thermal capacity falls, a critical crew specialist is incapacitated, and automated repair resources are limited.

The specification must determine every consequential game state without requiring an implementation-time gameplay decision.

Exit criteria:

- no unresolved cross-system contradictions;
- no duplicated authoritative rules;
- no orphan mechanics;
- no dependency cycle with undefined ownership;
- all implementation-critical open questions resolved.

## After GDS Completion

Only after the relevant design is sufficiently complete does the project proceed to:

1. technical architecture;
2. implementation roadmap;
3. C++/OpenGL project scaffolding;
4. gameplay implementation.
