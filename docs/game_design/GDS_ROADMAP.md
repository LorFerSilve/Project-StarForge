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

**Status:** First-Pass Complete — Cross-Validation Pending

Defines project-wide rules that constrain multiple systems, including:

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

First-pass closure:

- global baseline has no open gameplay question;
- GDS-2 through GDS-13 have been designed under these constraints;
- GDS-14 must perform the final whole-project validation before Design Complete status.

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

Defines:

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

Defines:

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

Defines:

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

Defines:

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

Defines:

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

Defines:

- combat loop;
- damage model;
- weapons;
- armor;
- shields;
- status effects;
- enemy combat behavior.

## GDS-10 — Robots

**Status:** First-Pass Complete — Cross-Validation Pending

Defines:

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

**Status:** First-Pass Complete — Cross-Validation Pending

Defines:

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

**Status:** First-Pass Complete — Cross-Validation Pending

Defines:

- trading;
- economy;
- loot and reward coordination;
- faction-reputation cross-system consequences;
- cross-domain capability progression;
- early-, mid-, late-, and endgame pacing;
- Tactical Control Capacity progression;
- finale readiness;
- difficulty profiles and scaling constraints;
- dynamic events and Horizon Recovery Grace;
- time and simulation authority;
- persistence and Stable Save Boundaries;
- failure/recovery rules;
- multiplayer position.

First-pass closure establishes:

- all planned cross-cutting system specifications;
- all planned progression specifications;
- explicit P0–P5 capability phases;
- exact first-pass MS-F01 finale-readiness rules;
- a combined GDS-12 cross-validation against GDS-1 through GDS-11;
- no remaining implementation-critical open gameplay question inside GDS-12 scope.

Remaining dependency:

- GDS-13 must define/cross-validate presentation, onboarding, feedback, and accessibility behavior;
- GDS-14 then performs the final whole-project cross-system audit.

## GDS-13 — Presentation, Onboarding, and Accessibility

**Status:** First-Pass Complete — Cross-Validation Pending

Defines:

- project-wide visual direction;
- environment art direction;
- Horizon Station visual language;
- faction visual language;
- UI/UX information architecture;
- adaptive HUD;
- interaction and transaction feedback;
- VFX language;
- audio direction;
- alarms and signaling;
- onboarding/tutorial sequencing;
- accessibility.

First-pass closure establishes:

- stylized grounded science-fiction visual direction;
- hybrid diegetic/non-diegetic presentation;
- functional/silhouette readability before color-only identification;
- presentation knowledge boundaries that prevent hidden-information leakage;
- adaptive HUD without an omniscient minimap or generic XP/gear-score layer;
- truthful Preview/Commit and transaction feedback;
- visual/audio/VFX state tied to authoritative gameplay state;
- P0–P3 alarm priority and category signaling;
- campaign-integrated tutorials using the real game rules;
- explicit onboarding for extraction ownership, robot attrition, raids, station defense, economy, saves, and capability progression;
- accessibility independent from Difficulty/rewards;
- remappable controls, motion reduction, photosensitivity protection, UI/text scaling, subtitle/caption/audio support, navigation support, and bounded aim assistance;
- a combined GDS-13 cross-validation with 107 checks against GDS-1 through GDS-12;
- no remaining implementation-critical open gameplay/presentation question inside GDS-13 scope.

Remaining dependency:

- GDS-14 must perform the final whole-project cross-system consistency audit and resolve any contradictions/orphan states before implementation planning.

## GDS-14 — Cross-System Consistency Audit

**Status:** Next

The complete specification is tested against complex scenarios involving several systems simultaneously.

The audit must include at minimum:

- simultaneous station power/thermal/atmosphere/security failures;
- Horizon defense while the player is deployed elsewhere;
- robot/crew response under damaged logistics and communication;
- mission failure with mixed secured/unsecured ownership;
- spacecraft damage during extraction/docking/travel boundaries;
- raid sabotage, theft, reinforcements, robot loss and withdrawal;
- economy/reputation/progression consequences after persistent world changes;
- save/load at transactional boundaries;
- Dynamic Event overlap/recovery grace;
- finale readiness and all three finale resolutions;
- presentation/readability of simultaneous failures;
- accessibility configurations including reduced motion/effects, color-independent presentation, subtitles/captions, remapped inputs, UI scaling and aim assistance.

Example audit scenario:

> The player is away on a mission when the home station suffers an attack, a primary power route is damaged, thermal capacity falls, a critical crew specialist is incapacitated, and automated repair resources are limited.

The specification must determine every consequential game state without requiring an implementation-time gameplay decision.

Exit criteria:

- no unresolved cross-system contradictions;
- no duplicated authoritative rules;
- no orphan mechanics;
- no gameplay-critical state without a valid presentation path;
- no accessibility configuration that silently changes knowledge/progression semantics;
- no dependency cycle with undefined ownership;
- all implementation-critical open questions resolved;
- relevant domain statuses can be promoted according to `00_design_authority.md`.

## After GDS Completion

Only after GDS-14 passes and the relevant design is sufficiently complete does the project proceed to:

1. technical architecture;
2. implementation roadmap;
3. C++/OpenGL project scaffolding;
4. gameplay implementation.

**No gameplay development begins before that gate is satisfied.**
