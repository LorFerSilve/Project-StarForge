# Crew Automation Responsibilities

> **Status:** Draft  
> **Authority:** What crew professions may automate, supervision boundaries, authority limits, and profession-specific failure escalation

## 1. Purpose

This document defines how crew expertise plugs into the station automation framework.

Station automation owns generic task scheduling and policies.

This document owns **which professional responsibilities crew can provide** and the limits of that responsibility.

## 2. General Rule

A crew member can automate a professional task only when:

- their current assignment covers the task;
- required skill is satisfied;
- workload capacity is available;
- necessary infrastructure exists;
- required information is available;
- player policy permits the action.

Crew never bypasses physical requirements.

## 3. Supervision vs Execution

Crew responsibility can be:

### Supervisory

The crew member diagnoses, prioritizes, configures, or authorizes work while machinery/robots execute it.

### Hands-On

The crew member physically performs the work.

### Hybrid

The crew member supervises routine work and performs exceptional local intervention when needed.

Each task defines which form applies.

## 4. Farmer Responsibility

A Farmer can automate:

- crop-plan execution within player targets;
- planting;
- harvest scheduling;
- nutrient allocation;
- water-use monitoring;
- crop-health response;
- agricultural output handling requests.

A Farmer cannot:

- create water;
- create nutrients;
- ignore power or atmosphere requirements;
- expand farming infrastructure without player construction decisions.

## 5. Engineer Responsibility

An Engineer can automate or supervise:

- power-network fault diagnosis;
- breaker isolation and eligible rerouting;
- atmosphere-system diagnosis;
- thermal-loop diagnosis;
- water-utility diagnosis;
- emergency system stabilization;
- infrastructure repair queues;
- repair-drone dispatch;
- startup/restart procedures within policy.

An Engineer cannot:

- override deliberate player shutdown without permission;
- spend protected strategic repair parts without authorization;
- restore destroyed hardware without resources/labor.

## 6. Mechanic Responsibility

A Mechanic can automate or supervise:

- docking machinery maintenance;
- spacecraft mechanical servicing;
- moving industrial machinery repair;
- mechanical inspection;
- mechanical replacement tasks.

A Mechanic does not replace Engineer authority over electrical networks unless their Engineering skill separately qualifies them.

## 7. Robotics Specialist Responsibility

A Robotics Specialist can automate or supervise:

- robot diagnostics;
- repair;
- maintenance;
- readiness;
- robot-production support;
- service-drone allocation;
- firmware/control restoration where defined.

They cannot command strategic raid objectives independently.

## 8. Scientist Responsibility

A Scientist can automate or supervise:

- active research execution;
- laboratory operation;
- artifact analysis;
- experiment monitoring;
- research fault diagnosis;
- data processing.

A Scientist cannot choose the player's research direction unless the player enables a specific queue policy.

## 9. Medic Responsibility

A Medic can automate or supervise:

- triage;
- treatment priority;
- medical stabilization;
- recovery care;
- medical resource requests;
- medbay operation.

A Medic cannot permanently resurrect a dead narrative character or ignore unavailable medical resources.

## 10. Security Officer Responsibility

A Security Officer can automate:

- access-policy enforcement;
- alert assessment;
- lockdown coordination;
- boarding-response allocation;
- internal-security robot deployment;
- protected-zone defense.

They cannot override physical pressure-safety rules or detect enemies without information.

## 11. Weapons Specialist Responsibility

A Weapons Specialist can automate:

- target-priority policy;
- ammunition-conservation policy;
- weapon-readiness supervision;
- shield/weapon tactical allocation where supported;
- defensive coverage coordination.

They cannot create ammunition, power, cooling, or firing arcs.

## 12. Logistics Officer Responsibility

A Logistics Officer can automate:

- storage policy;
- cargo routing;
- shortage response;
- reserve enforcement;
- supply priority;
- blocked-flow diagnosis;
- stock-balancing.

They cannot teleport cargo across disconnected networks.

## 13. Navigator Responsibility

A Navigator can automate or supervise:

- route computation;
- hazard-aware path planning;
- travel preparation;
- navigation-data validation;
- course optimization.

They cannot unlock unreachable regions without required propulsion/technology.

## 14. Intelligence Officer Responsibility

An Intelligence Officer can automate or supervise:

- threat report analysis;
- hostile-station intelligence consolidation;
- faction-information processing;
- raid reconnaissance interpretation;
- uncertainty classification.

They cannot reveal information that has never been acquired through sensors, missions, factions, or other legitimate sources.

## 15. Skill Thresholds

Professional tasks are classified by required skill.

A task may require:

- Novice (1);
- Competent (2);
- Skilled (3);
- Expert (4);
- Master (5).

Untrained (0) does not satisfy a specialist task unless that task is explicitly general labor.

## 16. Capacity Consumption

Every automated responsibility consumes workload capacity while active or supervised.

Routine steady-state supervision can consume low continuous workload.

Emergency incidents can temporarily consume substantially more.

This creates meaningful overload during simultaneous failures.

## 17. Over-Capacity Behavior

When assignment demand exceeds available crew capacity:

1. tasks are ordered by policy priority;
2. highest-priority tasks receive coverage first;
3. lower-priority tasks can experience delayed response or loss of specialist supervision;
4. critical uncovered tasks escalate to the player.

Capacity shortfall does not randomly choose tasks.

## 18. Delegation to Robots

A specialist may supervise several physical robots if:

- robot-control infrastructure exists;
- their workload capacity permits it;
- robots have task capability.

Crew expertise plus robots scales automation without implying that crew performs every physical action personally.

## 19. Local Controllers Without Crew

Some routine systems can operate through fixed automation without specialist crew.

Crew improves capability for:

- diagnosis;
- exception handling;
- complex tasks;
- optimization;
- recovery.

This avoids hard-locking basic station survival behind finding a specific profession.

## 20. Specialist-Required Capability

Advanced systems may require a specialist to commission, research, diagnose, or run advanced modes.

Such a hard requirement must be explicit in that subsystem.

The game never hides a mandatory profession requirement.

## 21. Escalation Reasons

Crew automation escalates when:

- skill is insufficient;
- workload capacity is exhausted;
- required resources are missing;
- access is blocked;
- sensor data is unavailable;
- authority is insufficient;
- equipment is destroyed;
- no execution robot/labor exists.

## 22. Protected Actions

The following categories require explicit player policy before crew may perform them automatically:

- consume protected strategic resources;
- vent inhabited compartments;
- intentionally disable critical life support;
- abandon a station section;
- fire on non-confirmed hostiles;
- destroy player-owned equipment.

Local safety shutdowns defined by equipment remain separate protective logic.

## 23. Decision Logging

Major automated emergency actions are recorded in the station event log with:

- initiating actor;
- reason;
- action;
- resources consumed;
- affected systems;
- outcome.

## 24. No Hidden Autonomous Strategy

Crew automation cannot independently change the player's strategic plan because a crew member has a high skill or special trait.

Traits can alter performance only as explicitly specified.

## 25. UI

The automation/crew interface shows:

- domain coverage;
- responsible specialist;
- workload;
- active tasks;
- queued tasks;
- blocked/escalated tasks;
- permission limits;
- robots under supervision.

## 26. Persistence

Responsibility configuration, coverage, permissions, and active crew-linked task assignments persist.

## 27. Edge Cases

If the specialist is incapacitated during an emergency, their active task is reassigned or escalated.

If automation hardware fails but the specialist can physically reach the equipment, hands-on response may continue.

If crew has expertise but no required robot/tool exists, the task remains blocked.

If two professions both qualify, the scheduler uses assignment and priority rather than double-counting work.

## 28. Explicit Non-Goals

Crew automation does not provide:

- infinite remote control;
- unexplained passive stat buffs;
- resource creation;
- strategic autonomy;
- omniscient information;
- unlimited multi-tasking.

## 29. Tuneable Parameters

Tuneable values include workload cost, response time, supervision capacity, skill modifiers, robot-to-supervisor ratios, and emergency workload multipliers.

## 30. Dependencies

This specification depends on:

- [Assignments](assignments.md);
- [Professions](professions.md);
- [Skills and Traits](skills_and_traits.md);
- `../station/automation.md`;
- all station systems using crew automation;
- spacecraft, research, robots, raids, and missions.

## 31. Open Questions

None in the current baseline.

The document remains Draft until every consuming domain is cross-validated.
