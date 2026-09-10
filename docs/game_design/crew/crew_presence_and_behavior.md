# Crew Presence and Behavior

> **Status:** Design Complete  
> **Authority:** Physical crew representation on the home station, movement, activity states, workplace presence, emergency behavior, off-screen abstraction, and AI scope

## 1. Purpose

Crew should make the station feel inhabited and operational without requiring a full life-simulation game.

Physical presence must correspond to assignments, emergencies, health, and accessibility.

## 2. Physical Representation

When a station area containing crew is actively simulated/rendered, crew members are represented as physical NPCs with persistent identity.

They have:

- current station location;
- movement state;
- activity state;
- destination;
- availability;
- assignment context.

## 3. Activity States

The baseline crew AI uses high-level activity states:

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

These are operational states, not minute-by-minute lifestyle simulation.

## 4. No Detailed Shift Simulator

The baseline does not require the player to create daily work schedules.

Crew automatically alternates between plausible work and off-duty behavior while preserving the effective workload/capacity defined by assignments.

The simulation model accounts for sustainable work capacity without requiring hourly shift editing.

## 5. Assignment Presence

A crew member assigned to a local facility normally travels to an appropriate work position before local work begins.

A crew member assigned to remote supervision can work from an authorized control location if connection requirements are satisfied.

## 6. Work Positions

Functional station modules can expose authored crew work points such as:

- console;
- maintenance station;
- lab bench;
- medical station;
- security desk;
- agricultural control station.

Work points provide animation/navigation anchors.

They do not themselves grant skill or automation capability.

## 7. Travel

Crew uses station pathfinding through currently traversable station geometry.

Pathfinding must respect:

- doors;
- security permissions;
- destroyed connections;
- airlocks;
- hazardous zones;
- temporary lockdown;
- vertical traversal.

Crew does not walk through walls or disconnected geometry.

## 8. Path Unavailable

If no valid route exists:

- local-presence tasks become Blocked;
- remote-compatible work may continue if its remote requirements remain valid;
- the crew member selects a safe waiting location;
- critical task blockage is escalated.

## 9. Door Interaction

Authorized crew can use normal doors.

Crew follows pressure and security interlocks.

They do not open a door that station safety logic identifies as immediately unsafe unless an explicit emergency behavior requires it and their role/equipment permits it.

## 10. Airlock Use

Crew uses airlocks only when:

- task requires passage;
- destination is safe or appropriate protective equipment is available;
- airlock is operational.

Ordinary off-duty wandering does not send unprotected crew into vacuum.

## 11. Hazard Avoidance

Crew pathfinding treats known severe hazards as blocked or high-cost based on role/equipment.

Examples:

- vacuum;
- fire;
- toxic atmosphere;
- extreme temperature;
- active hostile combat.

Emergency responders can enter hazardous areas only with sufficient protection or explicit acceptable-risk rules.

## 12. Off-Duty Behavior

When not required for active work, crew may visually:

- return to quarters;
- use common areas;
- walk between relevant safe spaces;
- interact with simple environmental points.

Off-duty behavior is cosmetic/immersive unless another specification explicitly assigns gameplay impact.

## 13. No Social-Life Simulation Requirement

The game does not need to model:

- friendship networks;
- romantic schedules;
- detailed conversations between every pair;
- personal entertainment satisfaction.

Authored dialogue can occur independently of generic AI behavior.

## 14. Player Interaction

The player can interact with crew for:

- identity/status;
- assignment shortcut;
- relevant dialogue;
- current task;
- injury/status information;
- authored narrative interaction.

The player does not need to physically speak to each crew member to configure routine station assignments if management UI is available.

## 15. Emergency Response

When an emergency occurs, crew behavior depends on:

- current location;
- profession;
- assignment;
- emergency policy;
- hazard knowledge;
- available equipment;
- safe routes.

Possible actions:

- continue critical duty;
- respond to task;
- evacuate;
- shelter;
- assist injured;
- move to muster area.

## 16. Muster Areas

Station design can designate safe muster or emergency assembly locations.

Crew not needed for emergency response can prefer reachable safe muster areas.

A muster designation does not make an unsafe compartment safe.

## 17. Boarding Attack

During confirmed boarding:

- non-security crew prioritizes safety and critical assigned emergency duties;
- Security Officers/security robots coordinate response;
- crew avoids known hostile paths where possible;
- combat-capable crew can defend themselves according to combat rules.

Generic scientists/farmers do not behave as disposable assault troops.

## 18. Injured Behavior

Minor/Serious injury modifies movement/work according to health rules.

Critical/Incapacitated crew cannot perform normal navigation.

Rescue actors must reach them physically.

## 19. Remote Station Simulation

When crew is in an unloaded station area or the player is away, the game may simulate crew at reduced detail.

Reduced-detail simulation must preserve:

- assignment availability;
- travel/access constraints at relevant granularity;
- task duration;
- hazard state;
- injury;
- location ownership sufficient for emergencies.

## 20. No Off-Screen Teleport Advantage

Reduced-detail simulation cannot complete a task faster simply because the NPC is not rendered.

Travel and task timing remain gameplay-equivalent.

## 21. Location Abstraction

For off-screen simulation, exact centimeter-level position is unnecessary.

The authoritative logical location can be:

- compartment;
- module;
- transit connection;
- docked ship;
- medical facility.

When the area loads, a valid physical position is reconstructed.

## 22. Streaming/Loading Rule

Crew cannot duplicate or disappear when station sectors stream in/out.

Each crew member has exactly one authoritative existence/location state.

## 23. Blocked NPC Recovery

If technical navigation error leaves an NPC in invalid geometry, implementation may safely reposition them to the nearest valid location within their authoritative logical compartment/path context.

This is a technical recovery mechanism, not a gameplay teleport ability.

It must not bypass locked/security/hazard boundaries for gameplay advantage.

## 24. Crew Collision

Crew has physical collision sufficient to appear present and avoid walking through the player/geometry.

Crowd collision should not allow crew to permanently soft-lock narrow corridors.

Local avoidance may temporarily relax NPC-vs-NPC blocking while preserving world collision.

## 25. Player Blocking

Crew should avoid trapping the player in doorways.

AI can yield, step aside, or temporarily reduce mutual blocking.

The player cannot use body-blocking to permanently prevent critical automated station operation.

## 26. Work Animation

Task animations communicate activity but are subordinate to actual task state.

An NPC does not appear to repair a device after the repair has completed.

## 27. Idle Animation

Idle/off-duty animations have no hidden productivity effect.

## 28. Dialogue Timing

Authored dialogue can temporarily claim a crew member's Narrative Activity state.

Critical emergency behavior can interrupt noncritical dialogue where narrative design permits.

## 29. Crew Identification

The player can identify crew through readable UI/name display at appropriate range or interaction.

Important roles can have consistent visual cues such as uniform/equipment elements without requiring rigid class costumes.

## 30. Equipment for Work

Tasks can require visible profession equipment.

Examples:

- Engineer tool kit;
- Medic kit;
- pressure suit;
- security weapon;
- scanner.

Equipment requirements must match actual task capability.

## 31. Save/Load

Crew location, activity, destination where needed, assignment state, health, and narrative state persist or reconstruct deterministically.

Loading does not reset every crew member to quarters.

## 32. Edge Cases

If a door locks while crew is traversing, the NPC completes only the physically valid side transition and then recalculates.

If a compartment depressurizes mid-route, crew reroutes/evacuates according to safety.

If their workstation is destroyed, the assignment becomes invalid and the crew moves to a safe state.

If a crew member is off-screen when boarding begins, logical position is used for security/hazard resolution rather than teleporting them to safety.

## 33. Performance Philosophy

Crew AI detail scales with relevance.

Full local animation/pathfinding is used where the player can observe it.

Distant/unloaded behavior uses lower-cost logical simulation.

Gameplay outcomes must remain consistent.

## 34. Explicit Non-Goals

The presence system does not require:

- full social simulation;
- detailed daily schedule editing;
- exact physics simulation of every off-screen NPC;
- NPC teleportation for normal task completion;
- thousands of fully simulated crew in one local space.

## 35. Tuneable Parameters

Tuneable values include walking speed, local avoidance radius, off-duty duration ranges, task-position timing, path cost, emergency-response delay, and simulation-detail thresholds.

## 36. Dependencies

This specification depends on:

- [Assignments](assignments.md);
- [Automation Responsibilities](automation_responsibilities.md);
- [Crew Needs](crew_needs.md);
- [Injuries, Incapacitation and Death](injuries_incapacitation_and_death.md);
- station structure, atmosphere, security, automation;
- future player movement, AI/pathfinding, and presentation specifications.

## 37. Open Questions

None in the current presence baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
