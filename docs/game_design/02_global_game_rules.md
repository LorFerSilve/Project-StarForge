# Global Game Rules

> **Status:** Under Review — GDS-14 Corrected  
> **Authority:** Project-wide gameplay invariants and scope rules

This document defines rules that apply across multiple gameplay domains.

Subsystem documents may refine these rules for their own context but may not contradict them.

## 1. Primary Game Mode

Project StarForge is a **single-player game** in the authoritative baseline.

All core progression, missions, station operation, raids, crafting, crew systems, economy, and narrative function completely without another human player.

No core progression reward requires multiplayer participation.

## 2. Multiplayer Scope

The baseline contains no synchronous or asynchronous PvP station-raiding system and no shared player economy.

Any future multiplayer design is a later explicit scope addition. It must adapt to the established single-player rules rather than becoming a hidden dependency of the core game.

## 3. On-Foot Perspective

Normal on-foot gameplay is first-person, including:

- station traversal;
- planetary/interior exploration;
- mining;
- direct combat;
- repairs;
- interaction;
- boarding.

A third-person on-foot mode is not a baseline requirement.

## 4. Spacecraft Perspective

Normal spacecraft piloting supports:

- third-person chase camera as the primary flight view;
- optional first-person cockpit view where the hull has an authored cockpit representation.

Both control the same ship simulation and grant no different mechanical capability.

## 5. Strategic and Construction Cameras

Spatial planning interfaces can use dedicated non-character cameras, including:

- station Construction Mode;
- galaxy navigation;
- tactical station overview;
- selected raid-planning interfaces.

These cameras do not change physical player/station existence.

## 6. World Structure

The game does not simulate one seamless continuous galaxy.

It uses:

- Strategic Locations and routes;
- bounded mission/local-flight environments;
- explicit navigation/travel transitions.

Planetary surfaces are bounded mission zones rather than full seamless planetary spheres.

## 7. Horizon Station Persistence

Horizon Station is a persistent physical home base.

Persistent state includes as applicable:

- built structure/modules;
- infrastructure topology/configuration;
- stored physical resources;
- installed equipment;
- damage/faults/repairs;
- crew/assignments;
- robots;
- production/automation policies;
- Research/Blueprint state;
- defenses/security;
- atmosphere/thermal/power/water state.

Leaving Horizon never resets it.

## 8. Off-Screen Active Simulation

Horizon continues to simulate while the player is away during unpaused gameplay.

Off-screen implementation may use lower detail, but outcomes must preserve the same:

- Simulation Time;
- physical ownership;
- topology;
- capacities;
- resources;
- damage;
- policies;
- deterministic event state.

## 9. Time Authority

**Simulation Time** is the sole gameplay-progression clock.

Real-world time while the application is closed never advances gameplay.

Active Game Time is player-facing unpaused playtime/UX cadence and does not independently decide gameplay state transitions.

See `systems/time_and_simulation.md`.

## 10. True Pause

True Pause freezes the complete baseline gameplay simulation, including:

- local actors/combat;
- Horizon off-screen simulation;
- production/Research;
- travel;
- markets;
- Dynamic Events;
- raids/Defense Events;
- recovery;
- hazards/timers.

Construction Mode is a specialized global True-Pause state under GDS-2.

No normal interface secretly pauses only one gameplay subsystem.

## 11. No Mandatory Real-World Waiting

Processes may require Simulation Time but never real-world hours/days while the application is closed.

The game should provide meaningful parallel activity rather than requiring passive waiting for long mandatory progression.

## 12. Player Failure Philosophy

Routine player defeat is consequential without deleting the player identity or unrelated long-term progression.

For external defeat:

1. GDS-8 resolves mission objective/result and physical ownership/security exactly once;
2. consumed ammunition/fuel/consumables remain consumed;
3. Field-Unsecured loot follows mission-loss rules;
4. established equipped gear is not routinely deleted, though condition consequences can remain;
5. the player enters **Recovery Transit** when no local rescue keeps the mission active;
6. `systems/recovery_transit_and_destination.md` selects a valid recovery destination from actual current world capability;
7. Horizon is the normal preferred long-term recovery hub only when a valid safe recovery ingress/location exists;
8. player recovery does not automatically teleport the Primary Ship, robots, cargo, crew, or Temporary Passengers.

Intentional defeat cannot be used as zero-time strategic fast travel.

## 13. No Routine Permanent Crew Death

Routine station failures, ordinary combat, raids, and off-screen simulation do not permanently delete recruited Crew IDs.

Crew can be:

- injured;
- Critical;
- Incapacitated;
- Stabilized;
- Recovering;
- temporarily unavailable.

Permanent Narrative Death requires explicit authored narrative authority or a later accepted global design change.

## 14. Human Crew Are Not Disposable Troops

Human crew are persistent specialists, not mass-consumable assault units.

The repeatable tactical assault force is primarily robotic.

Security crew can defend Horizon and specialists can participate in authored field contexts, but they are not interchangeable with disposable robot troops.

## 15. Robot Loss

Player robots are persistent physical manufactured units but **can** be permanently Destroyed through ordinary gameplay.

Destroyed Robot IDs are not resurrected for free.

Disabled robots can be repaired/recovered where physically possible.

## 16. Primary Ship Persistence

Established player ships are persistent progression assets.

Ordinary defeat can leave them:

- damaged;
- Disabled;
- Derelict;
- stranded;
- requiring tow/recovery.

Routine failure does not automatically delete the established Primary Ship or teleport it with the player.

## 17. Survival Scope

Survival mechanics exist where they create engineering/environmental decisions.

The game does not use constant personal hunger/thirst as its primary on-foot loop.

Food, water, oxygen, pressure, temperature, radiation, contamination, and life support matter through:

- station sustainability;
- crew support;
- mission environments;
- suit capability;
- infrastructure failure;
- preparation.

## 18. Automation Philosophy

Common solved repetition should become increasingly automatable.

Automation can perform repeatable work when actual:

- infrastructure;
- technology;
- qualified crew/robots;
- access;
- energy;
- resources;
- permissions

exist.

Automation never creates missing physical capability.

## 19. Automation Ceiling

Automation does not independently choose:

- major station expansion;
- Research direction;
- missions;
- raids;
- diplomacy;
- final story choices;
- protected strategic-resource spending outside authorized policy.

These remain player decisions.

## 20. Manual Override

Deliberate player shutdowns, lockouts, priorities, and protected-resource policies override routine automation unless an explicitly configured emergency authority permits the exact exception.

## 21. Progression Philosophy

Progression primarily unlocks:

- capability;
- strategic Reach;
- specialization;
- automation;
- resilience;
- efficiency;
- information/access;
- strategic options.

The game has no universal Player Level/Gear Score/Base Level/Ship Level/Robot Level replacing actual capability.

P0-P5 Capability Phases summarize state but grant nothing by themselves.

## 22. No Universal Level Scaling

The galaxy does not automatically scale to player power.

Locations, factions, missions, and targets have intended threat envelopes.

Dynamic composition can vary inside valid world ranges, but early enemies do not receive arbitrary late-game inflation solely because the player progressed.

## 23. Resource Scarcity Philosophy

Basic resources become easier to acquire/automate over progression.

Later challenge shifts toward:

- specialized resources;
- dangerous locations;
- difficult access;
- strategic targets;
- advanced components;
- Research/Blueprint requirements;
- faction/world conditions.

Late game is not primarily manual grinding of enormous trivial early-game quantities.

## 24. Physical Ownership

Every physical resource/item quantity has exactly one authoritative physical owner/location at any moment.

UI aggregation, reservation, mission state, trade, extraction, raids, manufacturing, and save/load must not duplicate ownership.

Credits are non-physical ledger currency and are not a substitute for crafting matter.

## 25. Mission Instancing

External missions use persistent Mission IDs and bounded Mission Instances/Zones.

Only one external Deployed Mission Instance can be active at a time.

A simultaneous Horizon Defense Event is a Home Station Event, not a second external player deployment.

## 26. Extraction Principle

Field pickup is not permanent security.

The design distinguishes:

- Field-Unsecured;
- Vehicle/Extraction-Secured;
- Station-Secured.

Successful extraction is an explicit transaction and does not automatically teleport all mission loot to station storage.

## 27. Direct Participation

The player remains a direct participant in core action gameplay.

Strategic/automation systems do not fully replace:

- exploration;
- combat;
- boarding;
- resource acquisition;
- emergency intervention.

The experience evolves toward command without becoming a pure management game.

## 28. Station Defense Philosophy

Horizon defense rewards actual:

- layout;
- redundancy;
- infrastructure protection;
- sensors;
- shields;
- power/thermal design;
- ammunition/logistics;
- crew/robot response;
- security segmentation.

Outcome is never authoritative from one opaque Defense Score.

## 29. Raid Philosophy

Raids combine direct combat with systemic access/sabotage/theft.

Players can gain advantage by targeting real dependencies such as:

- power;
- shields;
- sensors;
- communications;
- security;
- production/logistics;
- storage;
- access routes.

Raids remain specialized GDS-8 Missions rather than a parallel mission system.

## 30. Destruction Scope

Destruction exists only where authored/systemically meaningful.

The baseline does not require unrestricted destruction of arbitrary world geometry.

## 31. Selective Simulation

A system is simulated when its state creates meaningful player decisions or cross-system consequences.

The project does not simulate every physical, economic, biological, or social process for realism alone.

## 32. Realism Philosophy

The game uses internally consistent science-fiction rules rather than exact real-world simulation.

Mechanical credibility, consistency, and readable tradeoffs take precedence over unnecessary engineering detail.

## 33. Player Knowledge

The game may hide:

- unexplored locations;
- enemy intelligence;
- narrative mysteries;
- unknown technology;
- events not delivered through valid information paths.

It must not hide essential operational rules the player is expected to reason about.

Presentation and accessibility never reveal information the player does not legitimately know.

## 34. Communications

Strategic live information/orders require a valid **Strategic Communication Link** under `systems/communications_and_remote_control.md`.

There is no universal interstellar telemetry.

A communication blackout does not stop remote systems from continuing to simulate.

## 35. Failure Readability

Critical failure presentation must communicate, subject to player knowledge:

- what failed;
- where;
- immediate consequence;
- downstream consequences;
- active response;
- blockers;
- available recovery actions.

Complexity is acceptable; unexplained arbitrariness is not.

## 36. Save Philosophy

The game supports Manual Save, Quick Save, and Autosave.

Saving during combat/missions/raids is allowed when a **Stable Save Boundary** exists.

A request during a short atomic transaction queues until the next stable boundary.

Save/load preserves authoritative ownership, identity, event seeds, timers, damage, world state, and committed choices without offline progression or reroll duplication.

## 37. Dynamic Events

Dynamic Events have persistent IDs/stable seeds and use Simulation Time.

They require plausible world sources/eligibility and cannot create arbitrary campaign softlocks.

Procedural Horizon attacks obey Recovery Grace and Horizon Recovery State rules.

## 38. Horizon Recovery Protection

Horizon Recovery State is derived from actual subsystem conditions, not a global station-health number.

Every resolved ordinary procedural Horizon Defense Event starts Recovery Grace.

While Horizon is in Critical Recovery, ordinary procedural follow-up raids are blocked and the grace countdown is held according to `systems/horizon_recovery_state.md`.

## 39. Difficulty

Difficulty changes only documented execution-pressure axes.

It does not change:

- story access;
- Reach;
- Research/Blueprint prerequisites;
- loot/reward eligibility;
- TCC rules;
- permanent-loss classification;
- world threat ownership;
- AI knowledge rules.

## 40. Accessibility

Accessibility is independent from Difficulty/reward validity.

Accessibility can change presentation/control support but cannot create:

- hidden hit chance;
- bullet magnetism;
- wallhacks;
- new world knowledge;
- free resources;
- changed progression semantics.

Critical information cannot depend solely on color, audio, haptics, or camera effects.

## 41. Presentation Truthfulness

UI, HUD, VFX, audio, alarms, captions, markers, and interaction feedback must follow authoritative gameplay state.

They cannot present an uncommitted transaction as completed or leak unknown information.

## 42. Priority Namespace

Generic priority labels are domain-qualified.

Examples:

- `PowerLoadPriority::P0` is not `AlarmPriority::P0`;
- their numeric/text labels cannot be treated as one shared severity enum.

Likewise generic states such as `Critical` and `Disabled` remain qualified by their owning domain/type.

## 43. No Monetization-Driven Design Requirements

The authoritative design assumes no:

- microtransactions;
- loot boxes;
- premium gameplay currency;
- paid timers;
- pay-to-skip progression;
- daily-login mechanics.

Later commercial/distribution decisions must not silently corrupt core design rules.

## 44. Tuneable Values

Exact numerical values such as:

- damage;
- Health;
- speed;
- capacities;
- processing duration;
- resource yield;
- power output;
- weapon range;
- market price;
- recovery duration

are tuneable unless the specification explicitly classifies an exact value as structural.

Tuneable values can change in balancing without redefining fixed rule semantics.

## 45. Technical Independence

No gameplay rule assumes a commercial game engine.

The intended game remains implementable through the project's custom C++/OpenGL technology stack and purpose-built systems.

This is a design constraint, not permission to begin implementation before design closure.

## 46. Open Questions

None after the GDS-14 global-rule reconciliation.

GDS-14 still owns the final whole-project consistency verdict and status promotion.
