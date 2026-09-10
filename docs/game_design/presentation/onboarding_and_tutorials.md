# Onboarding and Tutorials

> **Status:** Draft  
> **Authority:** How the player learns controls, systemic rules, station operation, missions, extraction, resources, crew, robots, spacecraft, combat, raids, economy, progression, failure, and advanced systems

## 1. Purpose

StarForge contains many interacting systems. Onboarding must teach the player how to reason about them without creating a separate tutorial ruleset.

## 2. Core Principle — Teach the Real Game

Tutorials use the same authoritative mechanics as normal play.

They do not introduce fake:

- infinite ammunition;
- nonpersistent resources;
- temporary crafting rules;
- immortal robots;
- different damage formulas;
- special tutorial-only inventory ownership.

If a mechanic is shown, its real rule should apply unless the tutorial explicitly labels a temporary safety constraint.

## 3. Campaign-Integrated Onboarding

Primary onboarding occurs through P0/P1 and the canonical early story missions rather than through an unrelated simulation level.

The player learns by restoring and operating Horizon Station.

## 4. Teaching Philosophy

Use the sequence:

1. Context;
2. Demonstration/Prompt;
3. Player Action;
4. Immediate Result;
5. Short Explanation;
6. Independent Reuse later.

A system is not considered learned because the player read a large text page once.

## 5. Progressive Disclosure

Only expose information relevant to the current mechanic unless the player deliberately opens reference/codex material.

Advanced systems are introduced after their prerequisites become meaningful.

## 6. No Forced Repetition Grind

A tutorial can require one or a few representative uses.

It must not force repetitive low-value actions solely to prove mastery.

## 7. Tutorial Persistence

Tutorial completion state persists.

Reloading a save does not repeatedly replay an already committed tutorial unless:

- save is from before completion;
- player requests replay;
- context-sensitive hint remains necessary.

## 8. Tutorial Replay

Completed tutorials are available through a reference/tutorial menu containing:

- concise rule summary;
- current control binding;
- optional illustrated/animated explanation;
- replay where practical.

## 9. Contextual Hints

Hints can appear when:

- mechanic is first available;
- player encounters a known blocker repeatedly;
- player has not used a required capability after a reasonable interval;
- failure indicates a likely misunderstood rule.

Hints should not trigger every time the state occurs.

## 10. Hint Escalation

A contextual help sequence can escalate:

1. subtle reminder;
2. explicit action hint;
3. detailed explanation/reference link.

The player can disable contextual hints.

## 11. No Hidden Tutorial Assumptions

Mandatory progression cannot assume the player learned information that was only shown once in optional transient text.

Critical rules remain inspectable later.

## 12. Opening Control Teaching

Early P0 teaches:

- look/move;
- sprint/crouch/jump as applicable;
- interact;
- flashlight/scan where available;
- inventory;
- objective tracking;
- pause/settings.

Controls are shown using the player's current bindings.

## 13. First-Person Interaction

The first Horizon sequence teaches:

- focus an interactable;
- read available action;
- understand Blocked state;
- identify required tool/resource;
- commit action;
- observe resulting world state.

## 14. Power Tutorial

The opening restoration sequence teaches power through a real small network:

- source;
- connection;
- consumer;
- breaker/control;
- unavailable/unpowered state;
- restored state.

The tutorial should make cause/effect visible before exposing larger power-management overlays.

## 15. Atmosphere Tutorial

Early Horizon restoration teaches:

- pressure boundary;
- sealed door/airlock;
- unsafe compartment;
- oxygen/atmosphere restoration;
- pressure warning;
- safe entry.

It should demonstrate why a door cannot simply open under unsafe pressure conditions.

## 16. Repair Tutorial

Player repairs one meaningful fault using real:

- diagnostics;
- tool;
- part/resource;
- work time;
- repair result.

The distinction between temporary stabilization and full repair is introduced when first relevant.

## 17. Inventory Tutorial

The player learns:

- backpack ownership;
- Mass;
- Volume;
- equipment;
- item transfer;
- capacity blockers.

The UI demonstrates that dense and bulky objects can hit different limits.

## 18. Resource Tutorial

First resource acquisition teaches:

- physical source;
- extraction/pickup;
- inventory ownership;
- storage transfer;
- processing/crafting relationship.

No generic resource points are introduced.

## 19. Crafting/Manufacturing Tutorial

The first Work Order teaches:

- Blueprint/recipe;
- required inputs;
- reservation;
- machine/facility;
- work progress;
- output destination;
- blocked state.

If inputs are reserved, the tutorial explicitly shows they still exist but are committed to the Work Order.

## 20. Blueprint vs Research Tutorial

When both concepts become available, the player is explicitly taught:

- Research = understanding/technology;
- Blueprint = specific design;
- Research Evidence = persistent non-spendable knowledge requirement.

This distinction must not remain discoverable only through tooltips.

## 21. First Ship Tutorial

Before/within first repeatable expedition, teach:

- enter/pilot ship;
- ship systems summary;
- thrust/orientation;
- Flight Assist;
- nose vs velocity vector;
- docking;
- cargo;
- Fuel vs Propellant when current drive actually uses both;
- strategic navigation.

## 22. 6DoF Flight Teaching

Flight tutorial deliberately demonstrates:

- rotate without changing velocity;
- thrust changes velocity;
- Flight Assist dampens unwanted motion using real thrusters;
- damaged/limited thrusters affect control.

It must not teach atmospheric airplane assumptions as universal spaceflight rules.

## 23. Docking Tutorial

The player learns the distinction:

Alignment
→ Capture Envelope
→ Physical Capture
→ Hard Dock
→ optional service links.

A tutorial assist can guide alignment, but the actual dock transition remains real.

## 24. Strategic Travel Tutorial

First route selection teaches:

- route known/verified state;
- Strategic Reach;
- ship readiness;
- fuel/propellant estimate;
- travel commit;
- arrival/local flight transition.

## 25. First Mission Tutorial

The first external mission teaches:

- Accepted vs Deployed Mission;
- objective type;
- Mission Zone boundary;
- Threat summary;
- field inventory state;
- extraction requirement;
- failure consequence.

## 26. Extraction Tutorial Is Mandatory

Before the game expects the player to risk meaningful loot, it explicitly teaches:

Field-Unsecured
→ valid extraction transfer
→ Vehicle/Extraction-Secured
→ later Station-Secured.

This is a core game rule and must not be left implicit.

## 27. Failure Teaching

The first meaningful mission deployment presents the failure contract before departure in concise form:

- consumed supplies stay consumed;
- unsecured loot can be lost;
- secured cargo persists;
- player gear follows recovery rules;
- robots can have different loss rules once introduced.

## 28. Combat Tutorial

Early combat teaches:

- aim/fire;
- ammo/reload;
- shield vs armor feedback;
- cover/positioning;
- weak points when introduced;
- healing;
- extraction under threat.

No RPG hit chance or random critical language is used.

## 29. Damage Channel Teaching

Damage channels are introduced gradually through actual equipment/enemy encounters.

When first relevant, UI/reference explains:

- Kinetic;
- Thermal;
- Explosive;
- Electrical/EMP;
- Corrosive.

The player is not asked to memorize all channels during the opening minutes.

## 30. Crew Tutorial

First survivor/recruitment sequence teaches:

- rescue is not recruitment;
- Temporary Passenger/Guest state;
- recruitment requirements;
- Profession;
- Primary Assignment;
- finite workload;
- crew needs at station scale.

## 31. Automation Tutorial

The player first performs representative work manually.

Only afterward does onboarding introduce automation for that solved task.

Sequence:

Manual task
→ assign specialist/robot/policy
→ observe automated task
→ inspect exception/blocker.

This reinforces DD-014.

## 32. Robot Manufacturing Tutorial

First robot commissioning teaches:

- real resources/components;
- Robot Assembly Bay;
- Work Order;
- commissioning;
- Robot ID;
- charging;
- assignment.

The robot does not appear as a troop-card counter.

## 33. Robot Squad Tutorial

Before first required tactical robot use, teach:

- Class;
- Command Load;
- TCC;
- Robot Rack/transport footprint;
- active vs reserve;
- basic commands;
- ROE;
- communication/fallback;
- Disabled vs Destroyed.

## 34. Robot Loss Warning

Before the first deployment where permanent robot destruction is possible, onboarding explicitly warns that Destroyed Robot IDs are not automatically restored.

This warning is not repeated intrusively forever.

## 35. Station Management Tutorial

As Horizon expands, introduce diagnostic overlays individually:

1. Power;
2. Atmosphere;
3. Thermal;
4. Water/Food;
5. Logistics;
6. Manufacturing;
7. Automation;
8. Security/Defense.

Do not open every overlay simultaneously during the opening.

## 36. Station Construction Tutorial

First construction sequence teaches:

- module preview;
- socket/grid;
- structural validity;
- utility connection;
- resource requirement;
- confirm/commit;
- relocation/dismantling consequences where available.

## 37. Emergency Tutorial

The first authored station emergency demonstrates:

Alarm
→ identify affected area
→ inspect root cause
→ assign/respond
→ stabilize
→ repair
→ post-event summary.

It uses a bounded recoverable incident rather than random catastrophic punishment.

## 38. Economy Tutorial

First market contact teaches:

- Credits;
- finite stock;
- buy/sell prices;
- cargo destination;
- market liquidity where relevant;
- reputation/access effects;
- no real-world market timers.

## 39. Reputation Tutorial

First meaningful reputation change explains:

- faction-specific relation;
- numeric/tier movement;
- concrete access consequence;
- no global morality score.

## 40. Raid Tutorial Sequence

Full raids are introduced only after mission, combat, robot, ship, and extraction fundamentals are established.

The first raid teaches in stages:

1. Target Intelligence;
2. Preparation;
3. Approach;
4. Access/External Defense;
5. Boarding/Bridgehead;
6. Interior objective;
7. Sabotage/loot;
8. escalation;
9. withdrawal/extraction.

## 41. Raid Alternatives

The tutorial demonstrates that raid success is not necessarily `kill everything`.

At least one early raid/tutorialized fortified target should offer a meaningful alternative such as:

- hacked access;
- power sabotage;
- sensor avoidance;
- targeted disablement.

## 42. Horizon Defense Tutorial

Before severe procedural Horizon raids are allowed, the player is introduced to:

- detection;
- defense coverage;
- security zones;
- reserve robots;
- ammunition/power support;
- boarding response;
- repair/recovery.

This satisfies the early-game protection contract.

## 43. Dynamic Events Tutorial

The first optional event teaches the difference between:

- world information;
- event offer;
- accepted mission;
- urgent Horizon threat.

## 44. Difficulty Tutorial

Difficulty selection explains that profiles alter pressure/accessibility—not progression rewards or hidden enemy leveling.

Changing difficulty is not presented as cheating or invalidating progression.

## 45. Save Tutorial

Early onboarding explains:

- Manual Save;
- Quick Save;
- Autosave;
- Stable Save Boundary.

If saving is queued, UI explains why rather than implying failure.

## 46. Progression Tutorial

The player is taught that progression uses real capability rather than character level.

When a blocker occurs, the tutorial/reference points to the missing capability chain.

## 47. Reach Progression

When a higher Reach route is discovered before the player can use it, UI uses this as a teaching moment:

`Route known — current ship lacks required Reach.`

This reinforces that knowledge and capability are separate.

## 48. Finale Readiness Tutorial

MS-F01 does not reveal a giant checklist without prior context.

Support Channels and readiness categories are foreshadowed during P3/P4 through normal faction, ship, station, research, and robot progression.

At MS-F01 the player receives the complete explicit readiness interface.

## 49. Tutorial Safety

Tutorial assistance may temporarily:

- suppress unrelated severe Dynamic Events;
- guarantee a valid teaching target;
- delay procedural Horizon raids;
- provide authored starter resources.

It may not alter the demonstrated mechanic's core rules.

## 50. No Tutorial Softlocks

Mandatory teaching sequences provide at least one valid recovery path if the player spends/loses ordinary required consumables.

They do not require an irreplaceable item that could already have been permanently lost through ordinary play.

## 51. Skipping

Experienced players can skip most explanatory prompts.

Canonical story objectives themselves are not skipped unless a separate campaign skip/new-game option is explicitly designed later.

Critical one-time warnings for irreversible consequences can remain even with tutorials disabled.

## 52. Tutorial Reset

Player can reset tutorial/hint completion independently from world progression, causing explanations to reappear without resetting the game state.

## 53. Controls and Accessibility

Every tutorial uses current input bindings and respects:

- hold/toggle preferences;
- UI scale;
- subtitle settings;
- motion reduction;
- color settings;
- controller/keyboard input.

## 54. Text Load

Tutorial text is concise in the moment.

Longer conceptual explanations live in optional reference pages.

## 55. No Time Pressure While Reading

Mandatory tutorial explanation is not presented in a live lethal context unless the game pauses or the prompt can be safely revisited.

## 56. Objective Guidance

Early tutorial objectives can use stronger guidance.

As the game progresses, guidance reduces to normal knowledge-based mission markers unless Accessibility settings request additional navigation assistance.

## 57. Mastery Verification

The game does not require formal quizzes.

Understanding is verified through successful use of the real mechanic in later independent gameplay.

## 58. Explicit Non-Goals

No separate fake-rules tutorial universe, no mandatory tutorial grind, no irreversible progression item with no recovery path, no wall-of-text onboarding, and no requirement to memorize all systems before first mission.

## 59. Tuneable Parameters

Hint delay, repeat thresholds, tutorial prompt duration, early event suppression window, initial resource support, and guidance strength are tuneable.

## 60. Dependencies

References GDS-1 through GDS-12, especially Early Game, Missions, Station, Resources, Player, Spacecraft, Crew, Robots, Raids, Economy, Difficulty, Save/Persistence, UI/HUD, and Accessibility.

## 61. Open Questions

None in the onboarding/tutorial baseline.
