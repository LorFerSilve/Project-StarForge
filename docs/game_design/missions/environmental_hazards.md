# Environmental Hazards

> **Status:** Draft  
> **Authority:** Mission-level environmental hazard classes, hazard states, telegraphing, activation patterns, interaction with World/Player/Ship systems, and hazard persistence

## 1. Purpose

Environmental hazards create preparation and navigation decisions using the world/survival systems already defined.

## 2. Authority Boundary

World defines the location's possible environmental conditions.

Mission Hazards defines local hazard behavior/patterns.

GDS-5 Field Survival owns player protection/exposure.

GDS-6 owns spacecraft environmental response.

GDS-9 owns combat damage integration where required.

## 3. Canonical Hazard Classes

Baseline:

- Vacuum / Low Pressure;
- Toxic / Contaminated Atmosphere;
- Extreme Cold;
- Extreme Heat;
- Radiation;
- Fire;
- Electrical Hazard;
- Storm / Weather;
- Debris / Impact Hazard;
- Unstable Terrain / Structural Collapse;
- Gravity Hazard;
- Anomaly Hazard.

## 4. Hazard State

A hazard can be:

- Dormant;
- Active;
- Escalating;
- Receding;
- Resolved/Disabled.

Not every hazard uses all states.

## 5. Hazard Pattern

A hazard can be:

- Static;
- Cyclic;
- Triggered;
- Progressive;
- Event-Driven.

## 6. Static Hazard

Always present in the relevant area.

Example:
- vacuum exterior.

## 7. Cyclic Hazard

Alternates according to a known/detectable active-time cycle.

Example:
- electrical storm windows.

Cycle state persists through save/load.

## 8. Triggered Hazard

Begins because a clear system/event condition occurs.

Example:
- reactor room fire after damage.

## 9. Progressive Hazard

Worsens over active game time or system deterioration.

Example:
- spreading contamination;
- failing structural pressure.

## 10. Event-Driven Hazard

Owned by a mission/world event.

Example:
- meteor/debris wave.

## 11. Hazard Telegraphing

Hazards should be communicated through one or more:

- sensors;
- visible effects;
- audio;
- environment;
- mission intelligence;
- warning systems.

Unknown hazards can surprise the player, but cannot be arbitrary untelegraphed instant death.

## 12. Vacuum / Low Pressure

Uses actual pressure/environment state.

Protection requires GDS-5 pressure-safe equipment.

## 13. Toxic / Contaminated Atmosphere

Uses contaminant classification.

Valid sealed protection or filtration is required according to hazard type.

## 14. Extreme Temperature

Environment exposes temperature beyond normal safety.

Suit/ship thermal systems determine protection.

## 15. Radiation

Hazard defines local radiation intensity/source/field.

Player accumulates Radiation Dose under GDS-5.

## 16. Fire

Fire can:

- damage;
- add heat;
- affect atmosphere;
- block routes.

In systemic interiors, station/ship fire behavior should use the relevant infrastructure rules.

## 17. Electrical Hazard

Examples:

- exposed power conductor;
- damaged machinery arc;
- anomaly discharge.

The hazard requires a physical/system source.

## 18. Storm / Weather

Weather hazards can include:

- blizzard;
- dust storm;
- toxic rain;
- electrical storm.

Effects can modify:

- visibility;
- sensors;
- movement;
- exposure;
- flight/landing.

## 19. Debris / Impact Hazard

In space zones, debris can create moving collision threats.

It follows local-flight/collision rules rather than cosmetic screen effects only.

## 20. Unstable Terrain / Structural Collapse

Specific authored/procedural geometry can become unsafe.

Collapse/destruction must be explicitly supported and cannot imply unrestricted terrain destruction.

## 21. Gravity Hazard

Examples:

- unusually low/high gravity;
- intermittent artificial gravity failure;
- anomaly-induced local disturbance.

GDS-5 Movement consumes gravity state.

## 22. Anomaly Hazard

Anomaly hazards are late-game/specialized and can affect:

- navigation;
- sensors;
- power;
- movement;
- radiation-like exposure;
- route stability.

Each anomaly effect must be explicitly authored.

"Anomaly" is not permission for arbitrary rule-breaking.

## 23. Hazard Zones

A mission can partition local areas into different hazard states.

Crossing a boundary updates actual environmental input.

## 24. Hazard Mitigation

Mitigation can include:

- appropriate suit;
- shelter;
- repair;
- shutdown;
- route choice;
- timing;
- deployed gadget;
- ship system;
- mission-specific engineering action.

## 25. Hazard Resolution

Some hazards are permanent environmental facts.

Others can be resolved through objectives.

Example:
- restore ventilation;
- shut down exposed reactor;
- seal breach.

## 26. No Generic Hazard Immunity

No single suit automatically protects against every class.

Protection is rating/capability-specific.

## 27. Hazard and Objectives

Mission objectives can deliberately route through hazards.

Known unavoidable hazards become briefing requirements/warnings.

## 28. Dynamic Hazard Persistence

A cyclic/progressive hazard's phase/progress persists.

Reload cannot reset a storm timer or radiation leak to farm safe windows.

## 29. Ship Hazards

Space/planetary hazards can affect:

- sensors;
- thermal;
- hull;
- propulsion;
- docking.

GDS-6 owns ship-system consequences.

## 30. Failure Readability

Hazard-caused failure identifies the primary environmental cause when known.

## 31. Edge Cases

If a player disables a hazard source, all dependent hazard zones update according to actual system state.

If a mission generator creates a mandatory path through an unavoidable lethal hazard, deployment validation requires a valid survival solution.

If a hazard blocks one route but an alternate remains, mission does not fail automatically.

## 32. Explicit Non-Goals

Hazards do not provide:

- arbitrary instant death;
- reset-on-reload safe windows;
- universal suit immunity;
- "anomaly" as undefined magic;
- unrestricted destruction.

## 33. Tuneable Parameters

Tuneable values include intensity, cycle duration, progression rate, warning lead time, area size, and sensor detectability.

## 34. Dependencies

This specification depends on World environment, Mission Zones, Threat, GDS-5 Field Survival, GDS-6 Ship Systems/Flight, Combat, and Presentation.

## 35. Open Questions

None in the environmental-hazard baseline.
