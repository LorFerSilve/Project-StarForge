# Presentation Design

> **Domain Status:** First-Pass Complete — Cross-Validation Pending

Authoritative design domain for how authoritative game state is communicated, taught, perceived, and made accessible.

## Specifications

- [`visual_direction.md`](visual_direction.md) — **Draft**
- [`environment_art_direction.md`](environment_art_direction.md) — **Draft**
- [`station_visual_language.md`](station_visual_language.md) — **Draft**
- [`faction_visual_language.md`](faction_visual_language.md) — **Draft**
- [`ui_ux.md`](ui_ux.md) — **Draft**
- [`hud.md`](hud.md) — **Draft**
- [`interaction_feedback.md`](interaction_feedback.md) — **Draft**
- [`vfx.md`](vfx.md) — **Draft**
- [`audio_direction.md`](audio_direction.md) — **Draft**
- [`alarms_and_signaling.md`](alarms_and_signaling.md) — **Draft**
- [`onboarding_and_tutorials.md`](onboarding_and_tutorials.md) — **Draft**
- [`accessibility.md`](accessibility.md) — **Draft**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — first-pass consistency audit

## Authority Boundary

GDS-13 owns presentation, communication, teaching, and accessibility behavior.

It owns:

- project-wide visual direction;
- environment/station/faction visual language;
- UI information architecture;
- HUD composition and marker rules;
- interaction/state/transaction feedback;
- VFX presentation;
- audio presentation;
- alarms and signaling;
- onboarding/tutorial sequencing;
- accessibility options and invariants.

It does **not** redefine the underlying gameplay state owned by GDS-1 through GDS-12.

Examples:

- GDS-2 owns station power state; GDS-13 owns how that state is shown/heard.
- GDS-8 owns Mission objective/extraction state; GDS-13 owns markers, HUD and teaching.
- GDS-9 owns combat hit/damage; GDS-13 owns reticle, hit feedback, VFX/audio and accessibility.
- GDS-12 owns Credits, Difficulty, Simulation Time and persistence; GDS-13 owns their interface and feedback.

## Core Presentation Model

Project StarForge uses a **hybrid diegetic/non-diegetic presentation model**.

Physical world state should be readable through geometry, materials, animation, lighting, sound and actor behavior where practical.

Critical systemic information may additionally use conventional HUD, panels, maps, overlays, captions and alerts.

Diegetic purity never justifies hiding a gameplay-critical rule.

## Core Visual Baseline

The canonical visual target is **stylized grounded science fiction**.

Presentation prioritizes:

- functional engineering forms;
- strong silhouettes;
- modular readability;
- lived-in physical spaces;
- restrained effects;
- persistent damage/repair fidelity;
- faction identity through form/material/signage rather than color alone;
- cosmic scale contrast between industrial interiors and space vistas.

## Core UX Baseline

GDS-13 establishes:

- adaptive HUD rather than permanent information overload;
- no omniscient minimap or hidden-information leakage;
- exact known blocker messaging instead of `level too low`/generic failure;
- Preview vs Commit separation for consequential actions;
- truthful transaction feedback: presentation cannot report success before the gameplay commit;
- explicit Field-Unsecured / Vehicle-Secured / Station-Secured communication;
- four-axis Mission Threat presentation;
- TCC and physical robot transport shown as separate constraints;
- physical ship fitting/Reach/route blockers shown directly;
- raid intelligence preserving Unknown/Suspected/Confirmed/Stale;
- explicit Stable Save Boundary feedback;
- capability-based progression/readiness UI rather than Player Level/Gear Score.

## Audio/VFX/Alarm Baseline

Effects and audio follow actual physical/system state.

The baseline establishes:

- no false operating effects for disabled systems;
- vacuum-aware sound presentation;
- restrained combat/environment VFX;
- no rarity loot beams as default;
- alarm priorities P0–P3;
- distinct alarm families;
- acknowledgement distinct from resolution;
- root-cause/child-alert aggregation during cascading failures;
- no audio-only, color-only, or strobe-only critical warnings.

## Onboarding Baseline

Tutorials teach the **real authoritative game**, integrated into P0/P1 and normal campaign progression.

Critical rules explicitly taught before meaningful exposure include:

- Mass + Volume inventory;
- power/atmosphere/repair basics;
- Resource ownership;
- Blueprint vs Research vs Research Evidence;
- 6DoF flight and docking;
- Strategic Reach;
- Field-Unsecured extraction semantics;
- mission failure consequences;
- Crew and Automation;
- Robot TCC/transport and permanent robot destruction;
- raid planning and non-brute-force alternatives;
- Horizon defense;
- Credits/economy;
- Difficulty;
- Stable Save Boundaries;
- capability-based progression and finale readiness.

## Accessibility Baseline

Accessibility is independent from Difficulty and never carries a reward/progression penalty.

The first-pass baseline includes:

- full core input remapping;
- hold/toggle alternatives;
- adjustable controller deadzones/sensitivity;
- adjustable FOV;
- camera shake and head bob down to 0%;
- motion blur Off by default;
- Photosensitivity Safe and Reduced Effects modes;
- color-independent semantic states and palette customization;
- UI/text scaling;
- subtitles and closed captions;
- mono audio and visual direction cues;
- dynamic-range options;
- navigation/objective assistance limited to already-known information;
- bounded visible-target aim assistance without bullet magnetism, wallhacks, hidden hit chance or auto weak-point targeting;
- single-player true pause and queued save feedback.

## Cross-Validation Result

[`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) performs 107 first-pass checks against GDS-1 through GDS-12.

Result:

- no blocking contradiction found;
- no implementation-critical open question remains inside GDS-13 scope;
- all gameplay-critical information classes have at least one presentation path;
- accessibility options preserve authoritative world/knowledge/progression semantics.

## Completion State

GDS-13 is:

**First-Pass Complete — Cross-Validation Pending**

It is not yet Design Complete.

The remaining dependency is **GDS-14 — Cross-System Consistency Audit**, which must test the complete specification under combined multi-system scenarios and close any remaining contradictions/orphan mechanics before technical architecture or implementation begins.
