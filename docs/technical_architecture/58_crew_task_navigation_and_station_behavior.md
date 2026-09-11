# TA-8 — Crew Task Navigation and Station Behavior

> **Status:** Architecture Complete
> **Authority:** Crew physical task execution, assignment-to-task translation, local travel/work behavior, emergency response, safe waiting/muster behavior, auto-assignment interaction, and station automation actor handoff

## 1. Purpose

Crew assignments and station automation define what work should happen; TA-8 defines how a persistent crew member physically reaches and performs work without turning Horizon into a detailed life simulator.

## 2. Authority Boundary

TA-8 crew AI consumes:

- persistent Crew assignment/availability;
- TA-6 `AutomationTask` / WorkOrder needs;
- current station topology/hazards/control state;
- navigation/perception state;
- health/equipment state.

It does not own:

- assignment policy;
- profession/skill validity;
- repair/manufacturing completion;
- resource ownership;
- station physical system state.

## 3. Crew Runtime Activity

Active crew uses GDS activity states:

- Off Duty;
- Traveling;
- At Work;
- Performing Task;
- Responding to Emergency;
- Evacuating;
- In Medical Treatment;
- Recovering;
- Waiting/Blocked;
- Narrative Activity.

Runtime behavior is a projection of the persistent semantic activity/task.

## 4. Task Offer

TA-6 or another owning domain exposes a typed `CrewTaskOffer` with:

- task/source ID;
- target/zone/facility;
- required profession/skills/capability;
- local vs remote presence requirement;
- priority/urgency;
- equipment/protection requirements;
- physical interaction anchor(s);
- resource prerequisites;
- current blocker/revision;
- completion owner.

Crew AI may accept/execute only when assignment/policy allows it.

## 5. Eligibility

A crew member is eligible when:

- available and not Incapacitated;
- skill/profession requirements pass;
- assignment/emergency policy permits task;
- required equipment/protection is available or can be legitimately acquired;
- target still exists;
- local/remote access requirements can be satisfied.

Eligibility does not mean path exists.

## 6. Task Selection

When several eligible tasks compete, selection uses stable deterministic factors:

1. emergency/life-safety priority;
2. player/assignment priority;
3. current assignment responsibility;
4. task urgency/deadline;
5. travel/access cost;
6. capability/skill fit;
7. current-task interruption cost;
8. stable task ID tie-break.

Routine task selection never overrides a locked assignment unless the configured emergency override permits it.

## 7. Assignment Travel

For local-presence work:

```text
Assigned/Accepted Task
→ choose compatible work/interaction anchor
→ path request
→ Travel
→ physically arrive
→ validate task still current
→ perform task interaction/work
```

The assignment does not teleport crew to the workplace.

## 8. Remote Supervision

A remote-capable assignment can execute from an authorized control point when:

- ControlData connectivity is valid;
- sensor confidence is sufficient;
- required console/control capability exists;
- physical hands-on presence is not required.

Loss of remote connectivity causes re-evaluation: travel locally, select another control point, or become Blocked.

## 9. Work Points

Authored work points are navigation/animation anchors only.

A work point can define:

- compatible task tags;
- actor clearance;
- approach position/facing;
- concurrent-user capacity;
- interaction target/interface;
- local hazard requirements.

Occupying a work point does not itself grant profession/skill or complete work.

## 10. Equipment Acquisition

If a task requires equipment, AI may obtain it only through an authorized crew/equipment workflow defined by owning systems.

It cannot spawn a tool, pressure suit, medical kit, weapon, or repair part.

Failure to obtain required equipment blocks or reassigns the task.

## 11. Door / Airlock / Lift Use

Crew route execution obeys typed traversal links.

Crew:

- operates only authorized doors;
- respects pressure/security interlocks;
- uses airlocks as actual sequences;
- uses lifts physically;
- does not route unprotected personnel through vacuum just because the geometric path is shortest.

## 12. Hazard-Aware Routing

Known hazards modify path eligibility/cost based on:

- crew protection/equipment;
- profession/emergency role;
- task urgency;
- acceptable-risk policy.

Ordinary off-duty or routine workers treat lethal hazards as forbidden.

Qualified emergency responders may enter only when their protection/policy permits it.

## 13. Blocked Task

If no valid route/access exists:

- task becomes locally Blocked for that actor;
- the crew member moves to a reachable safe waiting location if appropriate;
- remote supervision may continue only if valid;
- TA-6 automation receives the actual blocker;
- task can be offered to another eligible actor or escalated.

## 14. Safe Waiting Location

Safe waiting candidates are navigation annotations filtered by:

- reachable current topology;
- known hazard state;
- pressure/temperature safety;
- security/access;
- current emergency policy.

AI never uses an unsafe authored waiting point merely because it is closest.

## 15. Off-Duty Behavior

Off-duty behavior is low priority and cosmetic unless a GDS task says otherwise.

It can select among safe anchors such as:

- quarters;
- common area;
- observation/common facilities;
- nearby idle/work-support locations.

It is preempted by assignment/emergency needs.

No hidden productivity modifier is generated by wandering/idle animations.

## 16. Emergency Response

An emergency produces typed response tasks and/or global station policy state.

Crew behavior can become:

- continue critical duty;
- respond to repair/medical/security task;
- evacuate;
- shelter;
- move to muster;
- assist casualty.

Selection uses actual profession, assignment, equipment, hazard knowledge, and reachable routes.

## 17. Evacuation

Evacuation computes reachable safe destination candidates such as:

- designated muster area;
- protected compartment;
- evacuation craft/dock where explicitly ordered;
- medical facility for casualty movement.

A designated muster point is not assumed safe if current atmosphere/fire/security state says otherwise.

## 18. Boarding Attack

During confirmed boarding:

- non-security crew prioritize evacuation/shelter plus critical duties;
- Security crew can receive defensive tasks;
- combat-capable crew use GDS-9 AI/combat constraints;
- known hostile routes are high-cost/forbidden for noncombat crew where alternatives exist;
- generic workers do not become assault AI.

## 19. Casualty Rescue

Rescue is a physical task requiring:

- known casualty location;
- reachable path;
- rescuer hazard protection;
- manipulation/medical/transport capability;
- valid destination.

The rescued actor is not teleported to medbay.

## 20. Player Body Blocking / Congestion

Crew local avoidance yields/steps aside to reduce softlocks.

Critical automated work cannot be permanently defeated solely by player body-blocking in a narrow corridor; after bounded retries, the AI can wait, choose alternate route, or use the GDS technical recovery rule only where no gameplay boundary is bypassed.

World/static collision and locked/security boundaries remain intact.

## 21. Auto-Assignment Boundary

Crew AI does not decide permanent Primary Assignment changes.

Optional auto-assignment is an owning crew/automation policy that commits assignment state first.

TA-8 then executes the resulting travel/work behavior.

Emergency temporary reassignment likewise requires an authorized persistent policy/result.

## 22. Task Handoff

If a task is reassigned while one crew member is traveling/working:

- owning task generation/revision changes;
- old actor stops at the next safe interruption boundary;
- stale path/work results are rejected;
- consumed resources/work progress follow the owning task's explicit retention semantics;
- new actor starts from current committed task state.

## 23. Narrative Activity

Narrative content can claim `Narrative Activity` through an authored task/state.

Critical emergency policy may interrupt it only where narrative rules permit.

TA-8 does not invent dialogue interruption rules.

## 24. Crew AI Cadence

High-level task selection can run below 60 Hz; movement/action execution remains integrated with TA-7/TA-5 fixed ticks.

Urgent emergency/health/path invalidation can force immediate re-evaluation on the next valid AI phase.

Numeric cadence belongs to TA-13.

## 25. Persistence

Persistent/reconstructible crew AI state includes as needed:

- semantic activity;
- current task/assignment reference;
- logical destination/work target;
- blocked reason;
- emergency response state;
- authoritative logical location;
- health/equipment via owning domains.

Raw path corridor/local avoidance/animation anchor state is reconstructible.

## 26. Off-Screen Handoff

When a station area/Horizon is off-screen:

- detailed locomotion leases return;
- crew retains logical location/task/travel state;
- TA-8 off-screen model advances travel/task timing under same access/hazard constraints;
- reactivation reconstructs a valid physical position consistent with the logical location and progress.

## 27. Diagnostics

Debug tools expose:

- assignment and current task;
- eligibility filters;
- chosen destination/work point;
- path/blocker;
- emergency policy;
- protection/hazard assessment;
- task priority score;
- off-screen logical location/progress;
- last task transition reason.

## 28. Test Requirements

Tests cover assignment travel, remote supervision loss, inaccessible work point, emergency reassignment, pressure hazard routing, airlock sequence, muster invalidation, casualty rescue, body-block congestion, task reassignment stale-result rejection, auto-assignment boundary, and off-screen timing equivalence.

## 29. Explicit Non-Goals

No daily shift simulator, no detailed social-life AI, no permanent assignment changes from local AI, no teleport to jobs/medbay, no unsafe airlock shortcut, and no infinite worker capacity.

## 30. Dependencies

Depends on Crew Assignments/Presence, `52`–`57`, TA-6 Automation/WorkOrders, TA-5 movement, TA-7 actors, and Station hazard/control state.

## 31. Open Questions

None.