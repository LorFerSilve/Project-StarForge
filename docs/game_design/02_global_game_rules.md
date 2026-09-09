# Global Game Rules

> **Status:** Under Review  
> **Authority:** Project-wide gameplay invariants and scope rules

This document defines rules that apply across multiple gameplay domains.

Subsystem documents may refine these rules for their own context but may not contradict them.

## 1. Primary Game Mode

Project StarForge is designed first and foremost as a **single-player game**.

All core progression, missions, station operation, raids, crafting, crew systems, and narrative must function completely without another human player.

No core progression reward may require multiplayer participation.

## 2. Multiplayer Scope

Synchronous multiplayer is not part of the initial authoritative product scope.

The design may later support asynchronous player-station raiding, but the complete first implementation of raiding is against AI-controlled stations.

Future multiplayer must adapt to the established single-player rules rather than forcing the core game to depend on network availability.

## 3. On-Foot Perspective

Normal on-foot gameplay uses a **first-person perspective**.

This includes:

- station traversal;
- planetary exploration;
- interior exploration;
- mining;
- direct combat;
- repairs;
- interaction;
- boarding actions.

A third-person on-foot mode is not an initial design requirement.

## 4. Spacecraft Perspective

Normal spacecraft piloting supports:

- a third-person chase camera as the primary flight view;
- an optional first-person cockpit view where the current ship has a cockpit representation.

Both views control the same ship simulation and must not provide different mechanical capabilities.

## 5. Strategic and Construction Cameras

Interfaces that require spatial planning may temporarily use non-character cameras.

Examples include:

- station construction mode;
- galaxy navigation;
- tactical station overview;
- selected raid-planning interfaces.

These camera modes do not change the physical existence of the player character or station.

## 6. World Structure

Project StarForge does not simulate one seamless continuous galaxy.

The game world is divided into strategic locations and bounded playable environments.

Space travel connects locations through an explicit navigation and travel system.

Planetary surfaces are represented by mission zones rather than complete planet-scale spherical worlds.

## 7. Home Station Persistence

The home station is persistent.

Its authoritative persistent state includes, subject to the dedicated persistence specification:

- built structure;
- installed modules;
- infrastructure configuration;
- stored resources;
- installed equipment;
- damage;
- repairs;
- crew;
- assignments;
- production configuration;
- research progress;
- defenses;
- automation settings.

Leaving the station does not reset its state.

## 8. Active-Time Simulation

Station systems continue to simulate while the player is away from the station **during active gameplay**.

The station therefore remains operational while the player is on:

- missions;
- raids;
- space travel;
- other active locations.

Off-screen station simulation may use a lower-detail technical implementation, but it must produce gameplay-equivalent outcomes to the rules of the station systems.

## 9. No Real-World Offline Progression

Closing the game stops game-world simulation.

Project StarForge does not use mobile-style real-world timers that continue production, research, attacks, farming, or resource consumption while the application is not running.

Save/load restores the recorded game state rather than calculating arbitrary hours of offline progression.

## 10. Pause Semantics

Because the core game is single-player, opening the true pause menu pauses gameplay simulation.

Interfaces explicitly classified as gameplay interfaces rather than pause interfaces do not automatically pause the game.

Examples that may remain live include tactical or in-world interfaces when their subsystem specification says so.

## 11. No Mandatory Real-Time Waiting

Long-term progression may use in-game processing time, but the game must not require the player to wait real-world hours or days before continuing meaningful gameplay.

Production, research, farming, repairs, and similar processes advance through active game time and can be improved through progression and automation.

## 12. Player Failure Philosophy

Routine player defeat must be consequential without deleting major long-term progression.

When the player is defeated during an external mission:

1. the active mission is failed unless its specific rules define a recoverable incapacitation state;
2. the player returns to the home station through the game's recovery fiction;
3. persistent equipped gear is not permanently deleted by ordinary defeat;
4. equipped gear may receive damage or durability consequences where defined;
5. mission resources that were not secured before defeat are subject to mission-loss rules;
6. permanent station construction, unlocked research, recruited crew, and previously banked resources are not rolled back.

The exact recovery presentation and resource-loss model are owned by the relevant player and mission specifications.

## 13. No Routine Permanent Crew Death

Recruited station crew are long-term progression assets and persistent characters.

Routine station failures, automated combat resolution, or ordinary accidents do **not** permanently delete recruited crew.

Crew can become:

- injured;
- critically injured;
- temporarily incapacitated;
- unavailable for work.

Permanent crew death is reserved for explicitly authored narrative cases or a future explicitly accepted design change.

## 14. Human Crew Are Not Disposable Troops

Recruited human crew are not used as mass-consumable combat units.

The player's repeatable tactical assault force is primarily robotic.

Security personnel may defend the station and specialists may participate in authored or specially defined field situations, but the game does not treat recruited survivors as expendable equivalents of Clash of Clans troops.

## 15. Survival Scope

Survival mechanics exist where they create engineering or environmental decisions.

The game does **not** use constant personal hunger and thirst bars as the primary on-foot gameplay loop.

Food, water, oxygen, pressure, and temperature are important mainly through:

- station sustainability;
- crew support;
- environmental hazards;
- suit requirements;
- mission preparation;
- infrastructure failure.

## 16. Station Resource Philosophy

Common station necessities should progressively become automatable.

The player may manage them directly in the early game, but late-game progression should reduce routine maintenance burden.

Automation exists to remove solved repetition, not to remove strategic decisions.

## 17. Automation Ceiling

All routine, repeatable station operations can eventually be automated if the player has sufficient:

- infrastructure;
- technology;
- crew expertise;
- robots;
- resources.

Automation does not automatically make strategic decisions such as:

- choosing major construction expansion;
- selecting research direction;
- choosing missions;
- initiating raids;
- spending unique strategic resources;
- changing diplomatic policy.

Those remain player decisions.

## 18. Manual Override

Where a station process is automated, the player retains explicit control over its configuration.

Automation may react to faults according to configured rules but does not silently override deliberate player shutdowns, locked priorities, or prohibited resource usage unless an explicit emergency rule says otherwise.

## 19. Progression Philosophy

Progression should primarily unlock **new capability, reach, specialization, efficiency, and strategic options**.

Pure numerical inflation may support balancing but should not be the main reason a later system is more valuable.

Examples of preferred progression:

- a propulsion technology opens a new region;
- a new reactor architecture supports a more demanding station;
- an engineer enables automatic fault response;
- a robot class enables breaching;
- a research breakthrough unlocks a new production chain.

## 20. No Universal Level Scaling

The entire galaxy does not automatically scale to the player's current power.

Regions, factions, mission categories, and strategic targets have intended threat ranges.

Dynamic encounter systems may vary composition within those ranges, but early weak enemies do not become arbitrarily powerful merely because the player progressed.

This preserves the feeling of becoming stronger and creates dangerous areas that can initially exceed player capability.

## 21. Resource Scarcity Philosophy

Basic resources become increasingly easy to acquire or automate.

Advanced progression depends more on:

- specialized resources;
- dangerous locations;
- strategic targets;
- research requirements;
- rare components;
- faction access;
- difficult missions.

Late-game challenge should not primarily consist of manually collecting enormous quantities of trivial early-game materials.

## 22. Mission Instancing

External missions occur in bounded mission instances or mission zones.

Entering a mission establishes a mission state with:

- location;
- objectives;
- threat configuration;
- environmental state;
- mission inventory state where applicable;
- success/failure state.

Mission-specific persistence and regeneration rules are defined by the mission domain.

## 23. Extraction Principle

A mission does not automatically convert all collected field resources into permanently stored station resources at the moment of pickup.

The mission and resource specifications must distinguish between resources currently carried in the field and resources successfully secured.

This makes extraction mechanically meaningful.

## 24. Direct Participation

The player remains an active participant in core action gameplay.

Strategic systems do not replace:

- exploration;
- combat;
- boarding;
- resource acquisition;
- emergency intervention.

The intended experience combines direct action with long-term command rather than evolving into a pure management game.

## 25. Base Defense Philosophy

Station defense must reward:

- layout;
- redundancy;
- infrastructure protection;
- defensive specialization;
- power management;
- security design.

A defense should not be reducible to one global station-defense number.

## 26. Raid Philosophy

Raids combine systemic sabotage with combat.

The most effective target is not always the enemy with the highest health.

Players can gain advantage by identifying and attacking functional dependencies such as:

- power;
- shields;
- communications;
- security control;
- production;
- storage;
- access routes.

## 27. Destruction Scope

The game supports destruction where it has defined gameplay meaning.

It does not require unrestricted destruction of arbitrary world geometry.

Destructible targets include only objects or structures whose destruction rules are explicitly authored or systemically specified.

## 28. Simulation Scope

Project StarForge uses selective simulation.

A system is simulated when its state creates meaningful player decisions or cross-system consequences.

The project does not attempt to simulate every physical, biological, economic, or social process merely for realism.

## 29. Realism Philosophy

The game uses internally consistent science-fiction rules rather than strict real-world simulation.

Systems should feel mechanically credible and understandable.

Gameplay clarity and meaningful tradeoffs take precedence over exact real-world engineering where the two conflict.

## 30. Player Knowledge

Important consequences must be reasonably learnable.

The game may hide:

- unexplored locations;
- enemy intelligence;
- narrative mysteries;
- unknown technology.

It must not hide essential operational rules that the player is expected to reason about.

## 31. Failure Readability

Critical failures must communicate:

- what failed;
- the immediate consequence;
- what downstream systems are affected where the player has sufficient information;
- what recovery actions are available.

Complexity is acceptable; unexplained arbitrariness is not.

## 32. Save Philosophy

The game supports persistent single-player saves.

The final persistence specification must support:

- reliable restoration of station state;
- player progression;
- crew state;
- research;
- world progression;
- faction state;
- ship state.

Autosaves must occur at safe logical transitions.

Manual saving is supported unless a later subsystem requires a narrowly defined restriction during an unsafe transient state.

## 33. No Monetization-Driven Design Requirements

The authoritative game design does not assume:

- microtransactions;
- paid timers;
- loot boxes;
- energy systems;
- premium currencies;
- pay-to-skip progression.

Economic distribution decisions made much later must not corrupt the core progression design.

## 34. Tunable Values

Exact numerical values such as:

- damage;
- health;
- movement speed;
- production duration;
- resource yield;
- power output;
- turret range;
- mission reward quantity;

are balancing parameters unless their exact value is itself structurally meaningful.

Subsystem specifications define the rules and valid relationships; playtesting can tune values.

## 35. Technical Independence

No gameplay rule may assume the presence of a commercial game engine.

The intended game remains implementable through the project's custom C++/OpenGL technology stack and purpose-built supporting systems.

## 36. Open Questions

None at the global-rule level for the current design baseline.

Subsystems may reveal contradictions that require an explicit revision to this document during review.
