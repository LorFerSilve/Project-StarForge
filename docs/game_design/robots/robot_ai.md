# Robot AI

> **Status:** Draft  
> **Authority:** Player-owned robot perception, command execution, autonomous behavior, task priorities, combat logic, communications loss, pathfinding, self-preservation, and deterministic fallback behavior

## 1. Purpose

Robot AI must make a small squad useful without taking strategic control away from the player.

## 2. AI Layers

Player robots use three behavioral layers:

1. Safety / Survival Constraints;
2. Player Command;
3. Role Autonomy.

Higher layers override lower layers only according to explicit rules below.

## 3. Safety Constraints

A robot will not intentionally:

- enter geometry it cannot traverse;
- perform an action without required capability;
- fire through a known friendly;
- detonate demolition payload inside prohibited safety radius unless explicitly overridden;
- spend protected strategic resources without permission.

Safety does not grant immunity from hazards/enemy action.

## 4. Player Command Authority

Valid direct commands have higher priority than routine role autonomy.

The robot attempts the command until:

- completed;
- cancelled;
- impossible;
- superseded;
- communications lost and fallback changes behavior.

## 5. No Physics Bypass

A command cannot cause:

- teleportation;
- impossible door access;
- infinite tool range;
- shooting through cover;
- repair without parts;
- hacking without interface/path.

## 6. Role Autonomy

When not executing a higher-priority direct command, the robot follows its class/Rules of Engagement behavior.

Examples:

- Assault Unit engages authorized threats;
- Repair Drone seeks assigned damaged friendly;
- Recon Drone seeks sensor vantage;
- Shield Unit positions projection defensively.

## 7. Perception

Robot perception can use:

- visual sensors;
- range sensors;
- thermal/electronic sensors where equipped;
- squad-shared information;
- player target designation;
- mission intel.

## 8. No Omniscience

Robots know only:

- directly sensed data;
- received squad/player data;
- persistent known mission information.

They do not know hidden enemy positions through walls without suitable sensor information.

## 9. Shared Squad Information

A functioning communication network can share:

- hostile contact;
- Last Known Position;
- target ID;
- hazard warning;
- objective marker;
- ally damage state.

Shared data cannot exceed the information actually acquired by at least one valid source.

## 10. Communications

Each robot requires a valid command/communication connection for live player orders.

Communication can degrade because of:

- range;
- geometry/interference;
- EMP/jamming;
- damaged communication module;
- mission anomaly.

## 11. Communication States

Canonical:

- Connected;
- Degraded;
- Disconnected.

## 12. Connected

Normal command and squad data flow.

## 13. Degraded

Commands/data can experience:

- delayed update;
- reduced shared sensor information;
- lower coordination.

Exact latency/quality is tuneable.

## 14. Disconnected

Robot cannot receive new live commands.

It follows its **Fallback Order**.

## 15. Fallback Orders

Every deployed robot has one configured fallback:

- Hold Position;
- Continue Current Objective;
- Return to Rally Point;
- Return to Transport;
- Self-Preserve / Seek Safe Position.

Fallback cannot initiate new strategic objectives.

## 16. Reconnection

When communication restores:

- robot reports current state;
- pending obsolete commands are not blindly replayed;
- player/squad controller can issue fresh order.

## 17. Pathfinding

Robot movement must use valid navigation for its mobility type.

Examples:

- wheeled cannot climb arbitrary stairs/vertical walls;
- flying drone cannot pass sealed bulkhead;
- Heavy Mech requires sufficient clearance/load-bearing path where authored.

## 18. Path Failure

If no valid path exists:

- robot stops/chooses safe reachable position;
- command becomes Blocked;
- player receives reason when known.

The robot does not teleport.

## 19. Combat AI

Tactical robots reuse GDS-9 combat principles:

- physical hit rules;
- finite ammunition;
- reload;
- cover;
- perception;
- non-omniscient targeting;
- friendly-fire avoidance.

## 20. Role Combat Behavior

### Assault

- maintain effective weapon range;
- use cover;
- attack commanded/prioritized threats;
- protect squad objective.

### Heavy

- hold/advance along viable heavy path;
- prioritize high-threat/armored targets;
- avoid excessive chase.

### Shield

- maintain protective geometry;
- orient toward threat;
- manage energy reserve;
- avoid projecting when no useful protected target exists unless ordered.

### Recon

- avoid direct sustained combat;
- maintain sensor coverage;
- disengage from superior threats.

### Repair

- avoid combat;
- repair/stabilize assigned eligible targets;
- seek cover while not repairing.

### Hacker

- reach valid access point;
- maintain hack interaction;
- defend/retreat according to ROE.

### Breacher

- reach designated breach target;
- secure safe approach;
- execute breach when authorized.

### Demolition

- place only designated charges;
- respect blast safety;
- withdraw before detonation when required.

## 21. Rules of Engagement

Each squad/robot can use one ROE:

- Hold Fire;
- Defensive;
- Engage Hostiles;
- Engage Designated Target Only.

## 22. Hold Fire

Robot does not initiate weapon fire.

It can move/evade and perform nonweapon commands.

## 23. Defensive

Robot fires only against actors currently presenting a credible threat to:

- player;
- squad;
- protected objective;
- itself.

## 24. Engage Hostiles

Robot can engage identified hostile actors inside mission/command constraints.

## 25. Designated Target Only

Robot attacks only explicitly designated target/group.

It can still perform immediate collision/self-preservation behavior.

## 26. Target Priority

AI can rank valid targets using:

- player designation;
- immediate threat;
- mission objective relevance;
- range;
- target vulnerability;
- class doctrine.

Player designation overrides routine priority when target remains valid/reachable.

## 27. Friendly Fire Avoidance

Robot checks firing line.

If a friendly blocks the shot, it:

- delays;
- repositions;
- chooses another valid target

rather than firing through the ally by default.

If damage occurs anyway, GDS-9 friendly-fire rules apply.

## 28. Ammunition

Robot combat uses finite ammunition/energy.

When low/empty:

- reload;
- switch compatible weapon;
- request resupply;
- fall back to available capability.

No infinite robot ammo.

## 29. Energy Management

Robot AI tracks Internal Energy Reserve.

Default priority:

1. control core/essential mobility;
2. communication/sensors needed for safety;
3. mission-critical tool;
4. defense/shield;
5. weapons;
6. discretionary utility.

Class/software can modify eligible lower priorities.

## 30. Low Energy

At a warning threshold the robot:

- reports low energy;
- reduces discretionary consumption where policy allows;
- can return to recharge if commanded/fallback says so.

## 31. Damage Self-Preservation

Robot can withdraw/reposition when critically damaged unless:

- Hold Position/explicit sacrifice command overrides;
- mission-specific rule requires continued action.

## 32. Sacrifice Command Boundary

The player can order a robot to maintain a dangerous position.

This does not make the robot indestructible.

No generic self-destruct exists unless a class/component explicitly provides it.

## 33. Repair AI

A Repair-capable robot can autonomously service only tasks:

- permitted by assignment;
- physically reachable;
- within repair capability;
- with required spare parts/resources;
- safe enough under policy.

## 34. Hacking AI

Hacker AI cannot autonomously choose diplomatic/strategic intrusion.

It can execute an authorized target/action.

## 35. Demolition AI

Demolition Unit requires explicit target authorization before placing destructive charges on friendly/neutral strategic infrastructure.

## 36. Mission Objective AI

Robots can execute bounded objective sub-actions.

The player remains mission authority.

A robot does not independently choose to:

- abandon mission;
- extract squad;
- trigger irreversible story choice;
- initiate raid.

## 37. Off-Screen AI

When robot is simulated off-camera during active gameplay:

- behavior can use lower-detail simulation;
- results must remain equivalent to physical capabilities, time, damage, ammo, energy, access, and orders.

## 38. Save/Load

AI state persists where gameplay relevant:

- current command;
- target;
- ROE;
- fallback;
- communication state;
- ammo;
- energy;
- damage;
- path/task state.

Loading does not re-roll tactical decisions or refill resources.

## 39. Difficulty Boundary

GDS-12 can tune:

- reaction speed;
- targeting precision;
- command latency assistance;
- self-preservation thresholds.

It cannot grant robots omniscience or break physical rules.

## 40. Explicit Non-Goals

Player Robot AI does not use:

- omniscient sensors;
- teleport path recovery;
- infinite ammo/energy;
- autonomous strategic decisions;
- software-only physical capability;
- guaranteed friendly-fire immunity.

## 41. Tuneable Parameters

Perception ranges, reaction time, cover preference, communication range, low-energy thresholds, retreat thresholds, and role priorities are tuneable.

## 42. Dependencies

This specification depends on Robot Classes/Components, Tactical Commands, GDS-9 Combat AI, Missions, Station Automation, Communications, and future Difficulty/Raids.

## 43. Open Questions

None in the player-robot AI baseline.
