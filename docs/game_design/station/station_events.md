# Station Events

> **Status:** Design Complete  
> **Authority:** Dynamic home-station incidents, event escalation, resolution, fairness constraints, and interaction with station automation

## 1. Purpose

Station events create operational pressure and make the home station feel alive.

They test redundancy, crew, automation, reserves, emergency response, and station design.

They must create decisions rather than arbitrary punishment.

## 2. Event Categories

### Technical

Examples: equipment fault, breaker failure, pump failure, sensor failure, logistics blockage.

### Environmental

Examples: hull leak, contamination, thermal excursion, coolant leak, fire.

### External Natural

Examples: debris impact, micro-meteor damage, location-appropriate radiation event.

### Operational

Examples: docking incident, industrial accident, medical emergency, production failure.

### Hostile

Examples: attack warning, boarding, sabotage, hostile probe.

Hostile raid mechanics remain authoritative in raid, defense, and security documents.

## 3. Event Sources

An event can be:

- a direct systemic consequence;
- authored story event;
- mission consequence;
- dynamic event selected by the event system;
- hostile action.

## 4. Systemic vs Dynamic

A reactor overheating because cooling failed is a systemic consequence, not arbitrary randomness.

Dynamic incidents are additional events and still update real subsystem state when they trigger.

## 5. Event Lifecycle

Every event follows the applicable stages:

Trigger → Detection → Notification → Response → Possible Escalation → Resolution → Aftermath.

## 6. Trigger

The trigger creates the underlying gameplay state.

A warning popup never substitutes for actual subsystem state.

## 7. Detection

Detection depends on available:

- sensors;
- crew;
- local controllers;
- alarms.

A hidden fault can exist before remote detection if the owning subsystem supports it.

## 8. Notification Levels

Events use:

- Information
- Warning
- Critical
- Immediate Player Decision

Critical events use redundant visual and audio communication.

## 9. Response

Response comes from:

- local protection;
- automation;
- crew;
- robots;
- player.

The event system does not perform repairs directly; it creates actual conditions and tasks for owning systems.

## 10. Escalation

Escalation follows explicit physical/system consequences.

Examples:

- coolant leak → coolant loss → insufficient cooling → overheating;
- fire → heat and contamination → damage;
- hull breach → pressure loss → decompression.

There is no arbitrary escalation timer that ignores state.

## 11. Resolution

An event resolves only when its underlying condition resolves.

Closing a notification does not resolve it.

## 12. Aftermath

Aftermath may include:

- persistent damage;
- spent resources;
- repairs;
- crew injury;
- temporary capacity loss;
- salvage;
- narrative consequences.

## 13. Fairness Rule

Random dynamic events cannot cause unavoidable catastrophic permanent loss without a reasonable prevention, response, or recovery path.

Poor preparation can be punished; untelegraphed dice rolls should not erase major progression.

## 14. Severe Random Event Budget

Under normal settings, one unresolved severe random event suppresses additional unrelated severe random events.

This restriction does not suppress genuine systemic cascades or authored multi-stage events.

## 15. Frequency

Dynamic-event frequency may depend on:

- difficulty;
- station condition;
- location;
- faction threat;
- player actions;
- narrative state.

Constant emergency spam is not the intended difficulty model.

## 16. Preventability

Some incidents can be made less likely or less severe through:

- maintenance;
- redundancy;
- shielding;
- better technology;
- crew coverage;
- security;
- reserves.

## 17. Fire

Fire changes real state:

- consumes oxygen;
- creates heat;
- creates smoke/contamination;
- damages equipment/structure.

Valid responses include suppression, isolation, crew/robot action, and controlled depressurization where appropriate.

## 18. Hull Breach

A breach creates an actual atmosphere leak and damage state.

Response uses isolation, emergency patching, and full repair.

## 19. Coolant Leak

A coolant leak changes actual thermal-network inventory and can be isolated/repaired.

## 20. Power Fault

A power fault changes actual power equipment or topology.

Response uses breaker isolation, rerouting, backup, and repair.

## 21. Debris Impact

An impact targets a valid external area.

Protection can come from shields, armor, and eligible point defense.

Damage uses the station damage model.

## 22. Medical Emergency

A medical emergency creates crew-health tasks.

Routine dynamic events do not permanently kill recruited crew.

## 23. Hostile Warning

A hostile warning hands operational control to defense, security, and attack/raid rules.

## 24. Player Away From Station

Station events continue during active gameplay while the player is away.

Local automation and crew can respond.

Remote information depends on communications.

Events do not progress because real-world offline time passes.

## 25. Communication Loss

If remote communication is unavailable:

- station simulation continues;
- local automation continues;
- player information can be delayed or incomplete.

## 26. Decision Events

Some events can present real subsystem choices.

Example: cooling critically low; shut down industry, reduce shields, or authorize emergency coolant expenditure.

Each choice maps to actual system actions.

## 27. Automation Resolution

If automation has sufficient capability, resources, access, and authority, routine incidents can resolve without direct player action.

That is an intended progression reward.

## 28. Event History

The station maintains a recent incident log with:

- type;
- time;
- affected area;
- actions;
- outcome;
- major losses/repairs.

## 29. Persistence

Active events persist through save/load.

Saving does not extinguish fires, seal leaks, or cancel attacks.

## 30. Edge Cases

If an untriggered event target becomes invalid, the event is cancelled or validly retargeted.

If local protection resolves a minor event before the player sees it, only an informational log may remain.

Systemic failures can coincide naturally even when the severe random-event budget blocks unrelated random incidents.

## 31. Difficulty

Difficulty may tune frequency, warning time, tolerance, and hostile pressure.

It does not silently contradict core physical rules.

## 32. Progression

Early game: manual response to simple incidents.

Mid game: specialists, diagnostics, automated containment.

Late game: complex incidents but strong redundancy and high automation, making response strategic rather than repetitive.

## 33. Explicit Non-Goals

The event system does not provide:

- arbitrary unavoidable base deletion;
- offline real-world incidents;
- popup-only fictional failures;
- random escalation disconnected from subsystem state;
- event spam as the main challenge.

## 34. Tuneable Parameters

Tuneable values include frequency, warning time, severity distribution, cooldown, event budget, and escalation timing.

## 35. Dependencies

This specification depends on all station systems, automation, damage, defense, security, crew, cross-cutting dynamic events, difficulty, raids, and narrative.

## 36. Open Questions

None in the current baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
