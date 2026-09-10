# Crew Assignments

> **Status:** Design Complete  
> **Authority:** Crew job assignment, duty ownership, workload, reassignment, substitution, and assignment validity

## 1. Purpose

Assignments connect persistent crew members to concrete station responsibilities.

A crew member does not provide full professional automation merely by existing on the roster. Their expertise becomes operational when they are assigned to a compatible responsibility.

## 2. Assignment Model

Every available crew member has exactly one **Primary Assignment State**:

- Unassigned;
- Assigned;
- Medical Recovery;
- Temporarily Unavailable;
- Narrative Duty.

A crew member cannot hold two simultaneous full-capacity Primary Assignments.

## 3. Primary Assignment

A Primary Assignment defines the crew member's normal responsibility.

An assignment specifies:

- responsible crew member;
- profession/capability requirements;
- station domain or facility;
- optional physical work zone;
- workload demand;
- priority;
- current validity;
- current coverage state.

Examples:

- Engineer → Engineering Zone A;
- Farmer → Hydroponics Sector;
- Scientist → Research Lab 2;
- Logistics Officer → Station Logistics Network;
- Security Officer → Internal Security Command.

## 4. Assignment Targets

An assignment can target one of these authored scopes:

### Facility Assignment

One specific facility or room.

Example: Scientist assigned to Xenotech Laboratory.

### Zone Assignment

A group of compatible station modules.

Example: Engineer responsible for Engineering Zone A.

### Network Assignment

A system-wide connected network.

Example: Logistics Officer supervising Cargo Network 1.

### Station-Wide Command Assignment

Reserved for professions whose responsibility is naturally global.

Example: Security Officer commanding internal security.

A profession's authoritative responsibility specification determines which scopes it supports.

## 5. Assignment Requirements

An assignment is valid only when:

1. the crew member is available;
2. required minimum skills are satisfied;
3. the target exists;
4. the target is accessible or remotely supervisable as required;
5. required workstation/control infrastructure exists where applicable;
6. the assignment does not conflict with another Primary Assignment.

## 6. Workload

Every assignment generates a **Workload Demand**.

Every crew member has finite **Workload Capacity** derived from:

- profession;
- relevant skill;
- traits;
- installed automation/control infrastructure;
- current health state.

Workload units are tuneable data, but the capacity rule is fixed.

## 7. Coverage Ratio

For a valid assignment:

`Coverage = Available Workload Capacity / Assigned Workload Demand`

Coverage is capped for normal operation at the amount the assignment can use.

The gameplay interpretation is:

- Coverage >= 1.0: fully supervised;
- Coverage < 1.0: under-covered;
- Coverage = 0: no crew supervision.

Exact performance effects are owned by the relevant automation responsibility.

## 8. No Infinite Supervision

Adding more modules to a crew member's assigned scope can increase workload.

A single Master Engineer cannot supervise an unlimited station merely because their skill is high.

## 9. Shared Coverage

A responsibility may accept several compatible crew members when the target specification supports team supervision.

Their usable workload capacity combines subject to diminishing or authored coordination rules.

Team assignment does not allow the same person to be counted twice.

## 10. Lead Specialist

Where multiple crew members cover one responsibility, one may be designated Lead.

The Lead:

- provides the primary policy authority;
- is used for profession-specific decisions;
- can mentor lower-skill workers where supported.

Lead designation does not multiply capacity by itself.

## 11. Secondary Competence

A crew member may perform occasional tasks outside their Primary Assignment if:

- they possess the required skill;
- the task is emergency or temporary;
- their current Primary Assignment permits temporary interruption;
- automation policy allows reassignment.

This does not create a second permanent full-capacity assignment.

## 12. Emergency Reassignment

During a critical event, automation may temporarily reassign eligible crew if emergency policy allows it.

Emergency reassignment follows:

1. protect immediate life/safety;
2. preserve critical infrastructure;
3. restore essential operation;
4. return to normal assignment after resolution.

The player is notified when reassignment creates coverage loss elsewhere.

## 13. Player Reassignment

The player can change a crew member's assignment when:

- the crew member is not incapacitated;
- the new assignment is valid;
- no authored emergency lock prevents reassignment.

Reassignment is normally immediate at the management level, but the crew member must physically travel to a workplace before tasks requiring local presence begin.

## 14. Remote Supervision

Some assignments support remote supervision from a control room.

Remote supervision requires:

- suitable control infrastructure;
- functioning data/control connection;
- adequate sensor information.

Loss of remote connectivity can reduce or remove coverage until the specialist reaches a valid local control point or connectivity returns.

## 15. Local Presence

Tasks that explicitly require hands-on work need local physical presence.

Examples:

- manual repair;
- medical treatment;
- physical inspection;
- certain research procedures.

A valid high-level assignment alone does not teleport the crew member to the target.

## 16. Unassigned Crew

Unassigned crew:

- remains part of population;
- consumes station support;
- can respond to general emergency orders;
- can train;
- does not provide full assigned-domain automation.

## 17. Medical Recovery

Crew in Medical Recovery cannot hold an active normal assignment.

Their previous assignment becomes uncovered or eligible for substitution.

The system retains the previous assignment reference for optional restoration after recovery.

## 18. Temporarily Unavailable

Temporary unavailability can result from:

- authored story participation;
- transfer;
- quarantine;
- emergency evacuation;
- other explicitly defined states.

Unavailable crew does not contribute workload capacity.

## 19. Assignment Priority

Assignments can have a player-configured priority.

Priority influences:

- emergency substitution;
- staffing recommendations;
- automation resource attention.

Priority cannot make an unqualified crew member qualified.

## 20. Auto-Assignment

The player may enable optional auto-assignment for routine staffing.

Auto-assignment can:

- fill uncovered eligible roles;
- prefer relevant profession and skill;
- respect locked assignments;
- respect player priority.

It cannot:

- move a locked crew member;
- change a person's primary profession;
- consume protected resources;
- assign an unqualified person to a hard-required role.

## 21. Locked Assignment

The player may lock a crew assignment.

Routine automation will not reassign that crew member.

Emergency policy can only override a locked assignment if the player has explicitly enabled that category of emergency override.

## 22. Assignment Failure States

An assignment can become invalid because:

- target is destroyed;
- target becomes inaccessible;
- required control system fails;
- crew member becomes incapacitated;
- skill requirement changes;
- station branch disconnects.

Invalid assignments stop providing unavailable capability and report the cause.

## 23. Assignment UI

The crew management interface shows:

- crew member;
- profession;
- relevant skills;
- current assignment;
- workload capacity;
- workload demand;
- coverage;
- assignment priority;
- lock state;
- blocking conditions.

## 24. Station Coverage View

The station can display responsibility coverage by:

- facility;
- zone;
- network;
- station-wide domain.

Coverage gaps are visible before they become emergencies.

## 25. Persistence

Assignments, priorities, locks, Lead status, target scope, and previous-assignment recovery references persist through save/load.

## 26. Edge Cases

If an assigned target is deconstructed, the assignment becomes invalid rather than silently moving elsewhere.

If a crew member's path becomes unavailable, remote-capable tasks may continue only if remote requirements are met.

If two specialists are both assigned and one becomes incapacitated, remaining coverage recalculates immediately.

If a recovered crew member's previous target no longer exists, they return Unassigned.

## 27. Explicit Non-Goals

The assignment system does not require:

- minute-by-minute shift planning;
- simultaneous full-time jobs for one person;
- automatic profession changes;
- hidden workload;
- unlimited supervision.

## 28. Tuneable Parameters

Tuneable values include workload demand, capacity per skill level, teamwork modifiers, reassignment response time, and remote-supervision efficiency.

## 29. Dependencies

This specification depends on:

- [Crew Overview](overview.md);
- [Professions](professions.md);
- [Skills and Traits](skills_and_traits.md);
- [Automation Responsibilities](automation_responsibilities.md);
- [Crew Needs](crew_needs.md);
- [Injuries, Incapacitation and Death](injuries_incapacitation_and_death.md);
- `../station/automation.md`;
- station structural and pathfinding rules.

## 30. Open Questions

None in the current assignment baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
