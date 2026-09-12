# TA-11 — Management, Planning, Tutorial, and System UI

> **Status:** Architecture Complete  
> **Authority:** Major management screens, preview/commit flows, maps/overlays, onboarding/tutorial presentation, pause/settings/loading/failure/system screens, and cross-domain composite Read Model use

## 1. Purpose

StarForge has many non-HUD interfaces that expose complex authoritative state without replacing physical gameplay: inventory, mission planning, station management, construction, research, manufacturing, crew/robots, spacecraft configuration, trade, raids, settings, save/load, tutorials, and failure/recovery.

This document defines how those screens consume state and issue commands without creating shadow gameplay models.

## 2. Major Screen Families

Baseline screen families include:

- Pause/System;
- Character/Loadout;
- Inventory;
- Mission Log/Preparation;
- Galaxy Map;
- Station Overview;
- Construction;
- Station Management;
- Research;
- Manufacturing;
- Crew Management;
- Robot/Squad Management;
- Spacecraft Configuration;
- Trade/Market;
- Raid Planning;
- Settings/Controls/Accessibility;
- Codex/Logs where content provides them;
- Save/Load;
- Loading/Transition;
- Failure/Recovery;
- Tutorial/Onboarding overlays/history.

## 3. Composite Read Models

Complex screens consume dedicated immutable composite Read Models assembled at a Stable Simulation Boundary.

A composite view records all source revisions used to build it and contains only knowledge-filtered player-facing state.

Examples:

- `MissionPreparationReadModel`;
- `StationOverviewReadModel`;
- `ShipConfigurationReadModel`;
- `RaidPlanningReadModel`;
- `FinaleReadinessReadModel`.

The UI never joins mutable DomainStore references itself.

## 4. Preview State

Screens may maintain transient nonauthoritative preview state, such as:

- proposed station module placement;
- proposed ship loadout;
- proposed trade quantity;
- selected route;
- proposed squad composition;
- proposed research/work order choice.

Preview state is clearly distinct from committed gameplay state.

## 5. Preview Validation

The owning gameplay/domain service evaluates consequential previews against current authoritative state and returns a typed preview result containing:

- valid/warning/invalid;
- known blockers;
- known consequences;
- projected resource/capability effects where the GDS permits them;
- source revisions/preconditions.

The UI does not reproduce business rules to predict legality independently.

## 6. Confirm/Commit

A consequential operation uses:

`Preview -> User Confirm -> Typed Command/Transaction -> Accepted/Rejected -> Committed Result`.

The screen updates from the next Read Model/event after commit.

Closing the screen can discard preview state but cannot undo a committed operation.

## 7. Inventory and Equipment

Inventory UI uses authoritative ownership/security data and displays:

- item/resource identity;
- quantity;
- Mass;
- Volume;
- owner/location;
- reservation/security state;
- equipment compatibility;
- valid actions/blockers.

Sorting/filtering is presentation-only and commands target stable IDs.

## 8. Station Overview

Station Overview consumes layered views for:

- structure/modules;
- Power;
- Atmosphere;
- Thermal;
- Water;
- Logistics;
- Manufacturing;
- Security/Defense;
- Crew/robot work;
- alarms/automation response.

Only one dominant diagnostic overlay is emphasized at a time to control cognitive load.

## 9. Network Overlays

Network diagrams visualize existing TA-6 graph/read data:

- sources/producers;
- consumers/sinks;
- connections;
- disconnected islands;
- bottlenecks;
- warnings;
- redundancy.

Unknown consequences remain unknown when sensors/control/communication do not provide them.

UI graph visualization cannot alter topology.

## 10. Construction UI

Construction maintains a noncommitted placement proposal containing selected module, transform/socket and requested orientation.

The Station/Construction domain returns known:

- snap/attachment validity;
- collision/clearance;
- structural compatibility;
- pressure/utility implications;
- resource requirements;
- exact blockers/warnings.

Confirm creates the actual construction command/transaction.

## 11. Manufacturing and Research

Manufacturing UI displays real WorkOrder stages/reservations/blockers/output destination.

Research UI distinctly represents:

- Technology Research;
- persistent Research Evidence;
- Blueprint ownership/provider state.

Presentation cannot imply Evidence is consumed or a Blueprint/item is granted before the authoritative rule says so.

## 12. Crew and Robot Management

Crew/robot screens use persistent identities and known status/task/location information.

UI does not create a generic rarity/quality score.

Robot/squad composition preview exposes the real constraints relevant to the GDS, including TCC, transport/rack/Heavy Bay and environment compatibility.

## 13. Spacecraft Configuration

Ship fitting preview shows actual known constraints including:

- Mass;
- power;
- thermal;
- Fuel;
- Reaction Propellant;
- cargo;
- life support;
- hardpoints/slots;
- robot transport;
- Strategic Reach.

Invalid configuration results come from the Spacecraft domain rather than duplicated UI formulae.

## 14. Galaxy Map

The Galaxy Map consumes strategic knowledge, route and event Read Models.

It can present:

- known locations/routes;
- route verification;
- Reach requirements;
- ship compatibility;
- known faction control;
- known missions/events;
- known risk/intel;
- travel estimates.

Unknown geography/hazards/access remain unknown.

## 15. Map Interaction

Map pan/zoom/selection is presentation state.

Selecting a route does not commit travel.

Travel commit is a typed command using stable StrategicLocation/Route identity plus expected state revisions and follows the owning travel transaction.

## 16. Mission Preparation

Mission Preparation combines Mission, ship, inventory, squad, passenger/cargo, route, environment and capability Read Models.

Hard Blocks and Warnings are presented using the already defined TA-9 preparation semantics.

UI cannot relax a Hard Block to allow deployment.

## 17. Threat Presentation

Mission Threat shows all four canonical axes individually:

- Hostile;
- Environmental;
- Operational Complexity;
- Extraction Risk.

Overall Threat can be emphasized but cannot replace/average away the individual axes.

## 18. Raid Planning

Raid Planning separates known:

- Intel;
- Approach;
- Access;
- Interior Objective;
- Withdrawal;
- Loadout/Squad;
- Cargo capacity;
- reinforcement/escalation risk.

Suspected/stale/unknown intel remains visibly uncertain.

## 19. Trade/Market

Trade preview displays known:

- quantity;
- unit price;
- total Credits;
- market stock/liquidity where applicable;
- physical source/destination;
- cargo Mass/Volume effect;
- known access/reputation factors.

Confirm submits the actual trade transaction; the UI never modifies Credits or inventory directly.

## 20. Finale Readiness

MS-F01/Finale readiness uses the complete GDS categories and exact known blockers/provider paths.

No generic readiness percentage replaces the category list.

The irreversible MS-F02 choice remains TA-9 FinalResolution authority and uses an explicit high-consequence confirmation flow.

## 21. Pause Menu

The Pause Menu requests/operates under global True Pause and provides the GDS baseline surfaces:

- Resume;
- Save;
- Load;
- Settings;
- Controls;
- Accessibility;
- Return to Main Menu.

It does not create its own local simulation freeze.

## 22. Live Management Screens

Management interfaces opened outside True Pause are live unless their GDS contract explicitly requests global pause.

A visible `Simulation Live`/equivalent presentation state is available where ambiguity would be harmful.

The UI cannot silently freeze a local hazard while allowing other Simulation-Time systems to continue.

## 23. Settings Screens

Settings screens operate on the TA-11 profile/settings service.

Categories include:

- Controls;
- Accessibility;
- HUD/UI;
- Audio;
- Display/Graphics;
- subtitles/captions;
- difficulty links where the owning GDS/TA permits.

Reset-to-default is category scoped and never resets campaign progression.

## 24. First-Launch Accessibility Setup

Before new-game start the system can present the GDS minimum first-launch choices including:

- subtitles/Closed Captions;
- UI/text scale;
- color/readability;
- Reduced Motion;
- Photosensitivity Safe;
- remapping;
- Aim Assist baseline;
- Master/Dialogue audio.

Completing/skipping this screen changes preferences only.

## 25. Tutorial Architecture

Tutorial/onboarding uses a project-owned `TutorialPresentationController` driven by authored tutorial definitions and legitimate gameplay/read events.

A tutorial definition can specify:

- eligibility trigger;
- prerequisite tutorial flags;
- presentation content;
- semantic action references for effective binding display;
- optional global True Pause request;
- completion/dismiss/revisit policy.

It cannot directly grant gameplay rewards, resources, access or progression unless a separate owning gameplay command explicitly exists.

## 26. Tutorial Trigger Boundary

Tutorial triggers are derived from committed gameplay/read facts such as:

- first legitimate interaction opportunity;
- first Field-Unsecured pickup;
- first station network fault;
- first robot command context;
- first valid extraction state.

A tutorial overlay never fabricates the underlying event.

## 27. Tutorial Timing

Tutorial timing is presentation/accessibility state.

Required explanations support:

- extended duration;
- manual dismiss;
- global True Pause where invoked;
- hints on/off;
- history/revisit.

A required lesson is not permanently lost merely because a short display timer expired.

## 28. Tutorial Persistence Boundary

Tutorial viewed/dismissed/history preferences are profile/presentation state unless an owning gameplay design explicitly makes one part of campaign progression.

TA-12 defines persistence representation.

Resetting tutorials does not reset world progression.

## 29. Save UI

Save UI displays state from the persistence service:

- Save Requested;
- Waiting for Stable Save Boundary;
- Saving;
- Save Complete;
- Save Failed.

TA-11 presents these states; TA-12 owns actual writing/integrity/crash-safe save implementation.

## 30. Load UI

Load selection exposes validated save metadata and compatibility status from TA-12.

The UI cannot force-load a save that persistence validation marks incompatible/corrupt without an explicit supported recovery path.

Loading presentation never mutates the current live session before TA-12 staged load commit.

## 31. Scene/Content Loading UI

TA-3/TA-10 expose loading-stage Read Models including destination/stage/required readiness/failure.

Loading UI may show progress categories but must not use fake percentages when exact aggregate progress is unavailable.

Hard Streaming Hold is presented as a technical loading state while Simulation Time is frozen; it is not a gameplay penalty.

## 32. Failure/Recovery UI

Failure summary is causal and consumes committed Mission/Resource/Robot/Ship/Station/Faction state.

It communicates where applicable:

- mission outcome;
- Field-Unsecured loss;
- secured holdings preserved;
- consumed ammo/fuel/resources;
- robot/ship/station consequences;
- reputation/Credits consequences;
- valid retry/recovery path.

It cannot invent a rollback/refund.

## 33. Fatal Technical Error UI

A controlled technical-fatal screen can present errors such as:

- required content incompatibility;
- renderer/context fatal condition;
- persistence write/load fatal state.

It provides safe exit/retry options only when the owning subsystem says they are valid.

A technical failure is not converted into mission/gameplay failure.

## 34. Search/Filter

Large lists use purpose-specific search/filter/sort over their Read Models.

Search text affects presentation only.

A filtered-out entity continues to exist and simulate normally.

## 35. Units and Formatting

A shared presentation formatting service renders physical values consistently using canonical units/precision policy.

Examples include kg/t, L/m³, kW/MW, kPa, °C, m/km and Simulation-Time durations.

Formatting never changes underlying numeric gameplay values.

## 36. Known vs Unknown Blockers

Screens display exact blocker reasons only when the player legitimately knows them.

Otherwise the controller uses the owning Read Model's uncertainty representation such as:

- Unknown Risk;
- Signal Unresolved;
- Requirement Unknown;
- Insufficient Intel.

Privileged developer reasons never leak into shipping strings.

## 37. Notification/History Links

A notification may link to a relevant management screen only when the player has access to that screen/information.

Following the link changes UI navigation, not gameplay state.

## 38. Screen Revalidation

Every new composite Read Model generation revalidates:

- selected persistent IDs;
- preview preconditions;
- focus target availability;
- visible blocker/consequence information.

If a selected target disappeared/changed, the UI falls back safely and cannot commit against a stale visible row.

## 39. Accessibility

All major management/system screens support:

- UI/text scaling;
- controller/keyboard navigation;
- remapped effective action prompts;
- color-independent state;
- Reduced Motion;
- high contrast/readability;
- scroll/reflow at supported scale extremes.

## 40. Diagnostics

Development tooling exposes for each screen:

- source Read Model revisions;
- selected stable IDs;
- preview state/preconditions;
- pending typed command correlation;
- last accepted/rejected result;
- focus path;
- live/paused time mode.

## 41. Validation Requirements

Tests must cover:

- preview never mutates gameplay;
- confirm success follows domain commit;
- stale preview is rejected/refreshed;
- map cannot show unknown routes/targets;
- live management screen does not create partial pause;
- tutorial prompts reflect remapped controls;
- tutorial reset leaves world state unchanged;
- save UI does not claim success before persistence result;
- failure summary matches committed ownership/security outcomes;
- 150% UI scaling preserves irreversible confirmation controls.

## 42. Explicit Non-Goals

No baseline requirement for:

- management UI as gameplay authority;
- UI-side station/network solver;
- fake loading percentages;
- partial-pause management screens;
- generic player/base level UI;
- tutorial state granting hidden gameplay capability;
- direct save-file parsing by widgets.

## 43. Dependencies

Depends on all prior TA domain Read Models, TA-11 shipping UI/input/HUD, TA-3 scene transitions, TA-9 mission/finale state, TA-10 content runtime, GDS UI/UX/Onboarding/Accessibility, and later TA-12 persistence implementation.

## 44. Open Questions

None in the management/planning/tutorial/system UI architecture.
