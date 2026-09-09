# Station Automation

> **Status:** Draft  
> **Authority:** Delegation of routine station work, task execution, policy, permission, escalation, and coordination between crew, robots, machinery, and player control

## 1. Purpose

Automation is a central progression system.

It moves the player from performing routine station work personally toward configuring and supervising a functioning interstellar base.

Automation removes solved repetition without removing strategic responsibility.

## 2. Strategic Boundary

Automation may execute defined repeatable work.

It does not independently choose:

- major construction;
- research direction;
- missions;
- raids;
- diplomacy;
- spending of protected strategic resources;
- long-term station strategy.

Those remain player decisions.

## 3. Automation Actors

Automated work can be performed by:

- fixed machinery;
- local controllers;
- crew specialists;
- service robots;
- repair drones;
- advanced station-control systems.

Each actor has finite capability and capacity.

## 4. Automation Levels

The station evolves through:

### Manual

Player performs or explicitly starts most work.

### Assisted

Diagnostics, queues, warnings, and limited automatic safety behavior exist.

### Crew Managed

Assigned specialists handle routine work in their profession.

### Automated

Robots and machinery perform most repetitive physical operations according to policy.

### Advanced Supervision

Distributed systems coordinate routine work; the player focuses on exceptions and strategy.

These are maturity descriptions, not one global XP bar.

## 5. Task Model

An automation task records:

- type;
- target;
- required capability;
- required resources;
- required access;
- priority;
- urgency;
- assigned actor;
- current state.

## 6. Task States

Tasks can be:

- Detected
- Queued
- Assigned
- In Progress
- Blocked
- Escalated
- Completed
- Cancelled

## 7. Task Creation

Tasks may be created by:

- player order;
- schedule;
- sensor-detected fault;
- resource threshold;
- production target;
- emergency;
- crew observation.

Creating a task does not guarantee the station can resolve it.

## 8. Assignment

The scheduler chooses an eligible actor using:

- capability;
- availability;
- priority;
- access;
- safety;
- required equipment;
- player policy.

## 9. Physical Work Rule

Automation does not teleport repairs, harvests, cargo, or other physical work.

Off-screen technical optimization is allowed, but results must still respect time, access, capacity, resources, and topology.

## 10. Player Policies

The player configures policies such as:

- reserve thresholds;
- system priority;
- repair priority;
- crop targets;
- production targets;
- emergency isolation;
- defense engagement;
- protected-resource permissions.

Automation follows these policies.

## 11. Protected Resources

Rare or protected resources can require player authorization.

Automation can escalate a task when the only available solution would spend a protected resource.

## 12. Manual Override

A deliberate manual command supersedes routine automation.

Examples:

- lock device off;
- isolate branch;
- halt production;
- preserve battery reserve;
- prohibit repair.

Automation cannot instantly undo the command unless the player enabled a specific emergency override rule.

## 13. Crew Specializations

Crew professions expose automation capability.

Examples:

- Farmer: agricultural scheduling and fault response.
- Engineer: power, atmosphere, thermal, and utility management.
- Mechanic: mechanical and ship servicing.
- Robotics Specialist: robot and drone systems.
- Scientist: research operation.
- Medic: medical response.
- Security Officer: internal security.
- Weapons Specialist: defensive coordination.
- Logistics Officer: routing and storage.
- Navigator: navigation.
- Intelligence Officer: threat and raid information.

Detailed skill and capacity values belong to the crew domain.

## 14. No Omnipotent Specialist

One specialist cannot supervise unlimited work.

Capacity scales through:

- additional crew;
- skill;
- better controllers;
- robots;
- distributed automation.

## 15. Responsibility Assignment

The player can assign responsibility by domain or station zone where supported.

A responsibility assignment does not change physical infrastructure.

## 16. Scheduled and Target-Based Work

Automation can maintain player-defined targets such as:

- minimum food reserve;
- battery reserve;
- ammunition stock;
- repair backlog threshold;
- selected production quantity.

It does not create an infinite production order unless the player configured one.

## 17. Event-Driven Work

Automation can react to:

- breaker trips;
- leaks;
- low oxygen;
- harvest readiness;
- output blockage;
- hostile boarding;
- low reserve.

The response is limited by installed capability and permission.

## 18. Engineer Response Example

A relay fault can produce this sequence:

Fault detected → diagnosis → branch isolation → alternate route attempt → repair task → material reservation → repair-drone dispatch → verification → restoration.

If a step cannot be completed, the task becomes Blocked or Escalated.

## 19. Farmer Response Example

A mature crop can produce this sequence:

Harvest detected → output capacity checked → harvest executed → produce enters buffer → logistics request created → next crop scheduled if inputs and policy permit.

## 20. Escalation

A task escalates when:

- no qualified actor exists;
- resources are missing;
- access is impossible;
- authority is insufficient;
- repeated automatic attempts fail;
- severity exceeds configured authority.

The player is told the actual reason.

## 21. Emergency Permissions

Configurable emergency authority can allow:

- bulkhead closure;
- power isolation;
- emergency battery use;
- standard repair material use;
- defense activation.

High-cost strategic actions may remain approval-gated.

## 22. Local Safety Controllers

Critical equipment retains authored local protection even if central automation fails.

Examples:

- reactor emergency shutdown;
- breaker trip protection;
- airlock interlock.

## 23. Automation Failure

Automation can degrade because of:

- missing crew;
- damaged control systems;
- failed sensors;
- power loss;
- network isolation;
- destroyed robots;
- blocked logistics.

Underlying physical systems continue to obey their own rules.

## 24. No Hidden Stat Multipliers

Automation primarily provides:

- capability;
- capacity;
- response speed;
- efficiency;
- fault management.

Unrelated hidden bonuses are not allowed unless explicitly specified.

## 25. Dashboard

The automation interface shows:

- automated domains;
- responsible crew;
- task queues;
- blocked tasks;
- priority;
- robot availability;
- resource permissions;
- escalations;
- coverage gaps.

## 26. Notification Levels

Reports are categorized as:

- Information
- Warning
- Critical
- Decision Required

Routine success does not spam the player.

Critical events cannot be completely hidden.

## 27. Off-Screen Operation

Automation continues during active gameplay while the player is away.

It does not receive instant success simply because the station is not rendered.

## 28. No Offline Progression

Automation stops advancing when the game application is not running.

## 29. Persistence

Policies, assignments, queues, task state, permissions, schedules, and coverage persist through save/load.

## 30. Edge Cases

Two workers cannot independently complete the same single-worker task.

If assigned crew becomes incapacitated, the task pauses or is reassigned.

Reservations remain respected.

If the player manually resolves a problem, the pending automation task detects completion and does not duplicate work.

If required sensor data disappears, automation becomes limited rather than assuming success.

## 31. Progression

Early game: alarms, queues, direct player response.

Mid game: specialists, repair/farm/logistics automation, configurable policy.

Late game: distributed controllers, robot workers, automatic isolation, target-driven production, high-level supervision.

## 32. Explicit Non-Goals

Automation does not provide:

- autonomous diplomacy;
- autonomous mission selection;
- unlimited worker capacity;
- free resources;
- teleporting physical work;
- opaque rule-breaking AI;
- real-world offline production.

## 33. Tuneable Parameters

Tuneable values include response time, worker capacity, scheduling weights, specialist efficiency, controller capacity, and notification thresholds.

## 34. Dependencies

This framework depends on every station operational system and especially crew, robots, time/simulation, UI, logistics, farming, manufacturing, and repairs.

## 35. Open Questions

None in the current baseline.

The document remains Draft until dependent domains are cross-validated.
