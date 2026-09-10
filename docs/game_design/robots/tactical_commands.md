# Robot Tactical Commands

> **Status:** Design Complete  
> **Authority:** Player-issued robot commands, targeting requirements, command states, group execution, role-specific orders, Rules of Engagement interaction, cancellation, blocked-state reporting, and communications behavior

## 1. Purpose

Tactical Commands let the player direct a small robot force without turning Project StarForge into a full RTS.

Commands are concise intent, not frame-by-frame remote piloting.

## 2. Command Recipients

A command can target:

- one Robot ID;
- one Command Group;
- entire active Tactical Squad.

Only currently player-commandable robots receive it.

## 3. Command State Machine

Every issued command uses:

1. Issued;
2. Accepted;
3. Executing;
4. Completed;
5. Blocked;
6. Failed;
7. Cancelled.

## 4. Issued

The player has created the command intent.

The command has not yet been accepted by every target.

## 5. Accepted

The receiving robot has:

- valid communications;
- compatible capability;
- valid basic target data.

Acceptance does not guarantee path/action success.

## 6. Executing

Robot is actively attempting the command.

## 7. Completed

The explicit completion condition has been reached.

## 8. Blocked

The command cannot currently progress but can become possible.

Examples:

- door closed;
- path temporarily obstructed;
- missing line of fire;
- repair part not currently accessible;
- target temporarily shielded/inaccessible.

Robot reports the blocker when known.

## 9. Failed

The command is no longer executable.

Examples:

- target destroyed when capture was required;
- no path exists;
- required tool destroyed;
- target ID invalidated;
- robot becomes Disabled.

## 10. Cancelled

Player or higher-priority system explicitly cancels the command.

The robot returns to Role Autonomy or new order.

## 11. Command Replacement

A robot normally has one active primary tactical command.

Issuing a new primary command replaces/cancels the previous one unless the new command is an immediate sub-action explicitly compatible with it.

The baseline does not require long RTS-style arbitrary command queues.

## 12. Canonical General Commands

Baseline general commands:

- Follow;
- Move To;
- Hold Position;
- Defend Area;
- Attack Target;
- Focus Fire;
- Retreat / Disengage;
- Return to Transport;
- Resume Role Autonomy.

## 13. Follow

Target:

- Player;
- selected allied actor.

Behavior:

- maintain formation-relative position;
- navigate around obstacles;
- obey ROE.

Completion:
- continuous command until replaced/cancelled.

## 14. Move To

Target:
- valid world position or known waypoint.

Behavior:
- physically navigate to reachable target area.

Completion:
- robot reaches acceptable destination radius.

If unreachable:
- Blocked/Failed with path reason.

## 15. Hold Position

Target:
- current position or selected position.

Behavior:
- robot remains within authored hold radius;
- can rotate/reposition locally for cover/line of fire;
- does not pursue beyond hold boundary unless self-preservation override or new command.

## 16. Defend Area

Target:
- area center + radius;
- protected actor/object.

Behavior:
- remain in/near area;
- engage threats according to ROE;
- prioritize threats to protected target.

This is not a magical aggro aura.

## 17. Attack Target

Target:
- identified hostile Actor/System ID.

Requirements:
- robot capable of attack;
- target known/valid.

Behavior:
- maneuver to effective firing position;
- attack target while valid;
- obey weapon/ammunition/physics.

Completion:
- target reaches objective-compatible combat end state or becomes invalid.

## 18. Focus Fire

Target:
- identified hostile Actor/System ID.

Behavior:
- selected multiple combat robots prioritize the same target.

It does not increase raw damage or accuracy.

## 19. Retreat / Disengage

Target:
- rally point, player, or safe direction.

Behavior:
- break offensive pursuit;
- move toward requested safe position;
- fire defensively under ROE where permitted.

## 20. Return to Transport

Target:
- current valid robot rack/bay/extraction transport.

Behavior:
- navigate to transport;
- physically board/secure;
- transition out of active commanded squad after secured.

Completion:
- robot is physically secured.

## 21. Resume Role Autonomy

Cancels primary tactical order.

Robot resumes its class/ROE behavior while maintaining squad membership.

## 22. Canonical Role-Specific Commands

Baseline role commands:

- Scout Area;
- Repair Target;
- Hack Target;
- Project Shield;
- Breach Target;
- Demolish Target.

## 23. Scout Area

Eligible:
- Recon-capable robot.

Target:
- bounded area/route.

Behavior:
- traverse valid observation points;
- collect supported sensor data;
- report discoveries;
- avoid unnecessary combat according to ROE.

Completion:
- required area survey threshold/objective state reached.

## 24. Repair Target

Eligible:
- Repair-capable robot.

Target:
- valid damaged robot/equipment/system.

Requirements:
- compatible repair capability;
- actual required spare materials/components;
- physical access.

Completion:
- target reaches commanded repair state.

The command cannot manufacture missing parts.

## 25. Hack Target

Eligible:
- Hacker-capable robot.

Target:
- known valid access/interface point.

Requirements:
- hacking path exists;
- required authorization to attempt;
- physical/data link available.

Completion:
- owning hacking/objective system reports success.

The robot cannot remotely hack arbitrary sealed systems with no link.

## 26. Project Shield

Eligible:
- Shield-capable robot.

Target modes:
- Protect Player;
- Protect Group;
- Hold Shield Direction/Area.

Behavior:
- position/orient projection geometry;
- maintain while power/energy permits.

The shield remains physical/geometry-limited.

## 27. Breach Target

Eligible:
- Breacher.

Target:
- explicit breach-capable door/obstacle/access point.

Requirements:
- target marked breachable;
- required tool/payload available;
- safe enough placement.

Completion:
- passage reaches authored Open/Breached state.

It does not destroy arbitrary walls.

## 28. Demolish Target

Eligible:
- Demolition Unit.

Target:
- explicit destructible objective/system.

Sequence:

1. reach placement point;
2. validate target;
3. place/arm charge;
4. withdraw if required;
5. detonate according to authorization/fuse;
6. report target state.

## 29. Demolition Confirmation

For friendly/neutral/strategic targets, destructive charge arming requires explicit confirmation unless mission policy has pre-authorized that exact target class.

This prevents automation from destroying infrastructure through target-selection mistakes.

## 30. Command Target Knowledge

The player can command Attack/Hack/Breach/etc. only against:

- directly visible/known target;
- scanner/squad-known target;
- mission/intelligence-known target.

UI cannot select an undiscovered actor through walls.

## 31. Command Range

Live commands require valid communication connection.

Out-of-range/disconnected robots do not receive new commands.

## 32. Partial Group Acceptance

When a group command is issued:

- compatible connected members accept;
- incompatible/disconnected members reject with explicit reason.

A group command does not silently pretend every member is executing.

## 33. Capability Filtering

UI can show which selected robots are eligible for the chosen command.

Ineligible robot receives no impossible action.

## 34. ROE Interaction

Direct Attack Target/Focus Fire counts as explicit engagement authorization for that target, even under Hold Fire.

Other targets still obey current ROE.

## 35. Hold Fire Interaction

Hold Fire does not prevent:

- movement;
- repair;
- hacking;
- shield projection;
- breach tool use where tool is nonweapon;
- retreat.

Destructive Breach/Demolition remains separate explicit authorization.

## 36. Defensive ROE and Commands

Defend Area under Defensive ROE attacks only qualifying threats.

Under Engage Hostiles it can proactively engage identified hostile actors inside permitted defense envelope.

## 37. Command Interruptions

Commands can be interrupted by:

- robot Disabled/Destroyed;
- communications loss;
- target invalidation;
- path failure;
- insufficient energy/ammo;
- higher-priority safety condition.

The resulting state is Blocked/Failed/Fallback, not undefined.

## 38. Communications Loss

If command is already executing and connection is lost:

- robot follows configured Fallback Order;
- Continue Current Objective can preserve current command when fallback explicitly says so.

No new command can arrive until connection restores.

## 39. Manual Direct Control

The baseline does not require player possession/direct first-person control of a robot.

The player commands from their own character perspective.

A future special drone-camera mode may be added but is not required by GDS-10.

## 40. Command Feedback

The system must communicate:

- accepted;
- executing;
- completed;
- blocked reason;
- failed reason;
- disconnected.

Presentation details are GDS-13 authority.

## 41. Command Persistence

Current command, target, ROE, fallback, and relevant command state persist in allowed saves.

Loading does not auto-complete blocked tasks.

## 42. Edge Cases

If a target is destroyed by another actor during Attack Target, command completes if destruction satisfies requested outcome; otherwise target becomes invalid and command ends accordingly.

If Breach Target opens normally before robot arrives, command detects already-satisfied state and completes without using charge/tool unnecessarily.

If Repair Target becomes fully repaired by another worker, robot stops and does not consume duplicate parts.

If Return to Transport loses transport access, command becomes Blocked and robot follows safety fallback.

## 43. Explicit Non-Goals

Tactical Commands do not provide:

- infinite command queues;
- teleport movement;
- hidden-target selection;
- physical capability from software alone;
- direct damage bonuses from Focus Fire;
- arbitrary structure destruction;
- robot first-person possession as baseline.

## 44. Tuneable Parameters

Destination radius, hold radius, defend radius, formation tolerances, reaction delay, command transmission delay, and destructive-confirmation UX are tuneable.

## 45. Dependencies

This specification depends on Robot AI, Classes, Squad Composition, GDS-9 Combat, GDS-8 Objectives, Station Security, Communications, and Raids/Presentation.

## 46. Open Questions

None in the tactical-command baseline.
