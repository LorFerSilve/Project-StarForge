# Crew Injuries, Incapacitation and Death

> **Status:** Design Complete  
> **Authority:** Crew physical-health states, injury consequences, stabilization, medical treatment, recovery, availability, and permanent-death constraints

## 1. Purpose

Crew health creates consequences for station hazards and boarding attacks while protecting long-term character investment from routine random deletion.

The system must support:

- injury;
- critical injury;
- incapacitation;
- treatment;
- recovery;
- temporary staffing loss;
- authored permanent-death exceptions.

## 2. Global Permanent-Death Rule

Routine gameplay does **not** permanently kill recruited crew.

Permanent death can occur only when:

- an explicitly authored narrative event defines it; or
- a later accepted global design decision changes the rule.

Dynamic accidents, normal shortages, routine boarding combat, and off-screen simulation do not permanently delete recruited crew.

## 3. Health States

Crew uses these authoritative health/availability states:

- Healthy;
- Minor Injury;
- Serious Injury;
- Critical;
- Incapacitated;
- Stabilized;
- Recovering;
- Permanent Narrative Death.

Permanent Narrative Death is unavailable to generic systemic resolution.

## 4. Healthy

Healthy crew has no injury-based work restriction.

Other availability rules can still prevent assignment.

## 5. Minor Injury

Minor Injury can:

- reduce workload capacity;
- limit hazardous physical tasks;
- benefit from treatment.

Minor Injury does not automatically remove the crew member from assignment.

## 6. Serious Injury

Serious Injury:

- substantially reduces capacity;
- blocks hazardous/manual work;
- normally requires medical treatment;
- can remove the crew member from their assignment depending on task.

## 7. Critical

Critical crew requires stabilization.

While Critical:

- normal work stops;
- movement may be impossible or emergency-only;
- medical priority is high;
- untreated deterioration can lead to Incapacitated.

Routine Critical state does not progress to permanent death.

## 8. Incapacitated

An Incapacitated crew member:

- cannot work;
- cannot self-evacuate unless a later recovery rule permits;
- requires rescue/transport if in danger;
- consumes medical response capacity.

Incapacitation is a gameplay loss of availability, not character deletion.

## 9. Stabilized

A Stabilized crew member is no longer worsening from the immediate critical event but still requires treatment/recovery.

Stabilization can occur:

- on-site;
- in a medbay;
- through emergency medical equipment.

## 10. Recovering

Recovering crew has entered formal recovery.

Recovery can restore health through stages.

The crew member normally cannot provide full assignment capacity until medically cleared.

## 11. Injury Sources

Crew injuries may result from:

- hostile combat;
- explosions;
- fire;
- decompression exposure;
- unsafe temperature;
- toxic atmosphere;
- radiation where defined;
- structural accidents;
- prolonged critical food/water failure;
- authored events.

## 12. Severity Resolution

Injury severity depends on actual event parameters such as:

- damage magnitude;
- protection;
- exposure duration;
- environmental severity;
- medical response.

The system does not randomly declare permanent death.

## 13. Protective Equipment

Protective gear can reduce or prevent injury from compatible hazards.

Examples:

- pressure suit;
- thermal protection;
- radiation protection;
- armor;
- respirator.

Equipment effects belong to the player/equipment or crew-equipment specification when created.

## 14. Crew Combat Exposure

Security personnel and other crew can be injured during internal station attacks.

Because human crew is not the disposable troop system, defensive automation should prefer robots and protected tactics where available.

## 15. First Aid

Eligible nearby crew or the player can perform first aid if they have:

- required skill;
- equipment;
- access.

First aid can stabilize but does not necessarily fully heal serious injury.

## 16. Medic Role

A Medic can:

- triage;
- stabilize;
- diagnose;
- treat;
- manage recovery;
- allocate medical resources.

Higher skill can improve treatment speed/capacity according to tuneable rules.

## 17. Medbay

Advanced treatment uses station medical facilities.

A medbay requires:

- safe environment;
- power;
- medical supplies;
- treatment capacity;
- compatible medical equipment.

A Medic can improve capability but cannot make a destroyed unpowered medbay fully operational.

## 18. Treatment Queue

When patients exceed medical capacity, treatment uses triage.

Default order:

1. unstabilized Critical;
2. Incapacitated/Serious requiring urgent treatment;
3. Stabilized Critical;
4. Serious;
5. Minor.

The system does not choose randomly.

## 19. Medical Supplies

Treatment can consume physical medical resources.

If supplies are missing:

- treatment can be limited;
- stabilization may still be possible with basic capability;
- recovery slows or blocks according to treatment definition.

Medical scarcity does not silently kill generic persistent crew.

## 20. Recovery Time

Recovery requires Simulation Time.

Exact duration depends on:

- injury severity;
- treatment;
- Medic skill;
- medical facility;
- traits.

No real-world offline recovery occurs.

## 21. Assignment Consequences

When a crew member becomes unable to work:

- their assignment coverage recalculates;
- automation attempts valid substitution;
- uncovered critical responsibilities escalate.

This makes crew injury operationally meaningful.

## 22. Medical Evacuation

An injured crew member in an unsafe area may require transport to medical care.

Evacuation can use:

- another crew member;
- security/rescue robot;
- emergency stretcher/drone where available.

The system does not teleport patients through blocked structure.

## 23. Environmental Rescue

If a crew member is incapacitated in vacuum, fire, toxic atmosphere, or other lethal space:

- they remain at risk of worsening to the system's maximum non-permanent state;
- rescue requires appropriate protection;
- routine simulation does not delete them permanently.

Severe failed rescue can produce long recovery and resource cost.

## 24. Narrative Death

If an authored story uses permanent death:

- the event must be explicit;
- narrative and gameplay consequences are authored;
- the roster permanently records the outcome;
- the player is not surprised by a generic random roll disguised as narrative.

## 25. No Revival Mechanic Needed for Routine Play

Because routine gameplay does not permanently kill crew, the baseline does not require cloning, resurrection, or magical revival as a systemic workaround.

Such technologies may exist in lore only if later designed.

## 26. Player Away From Station

Crew injury can occur while the player is away during active station simulation.

Local medical/security automation responds according to actual capability.

Off-screen resolution cannot permanently kill generic crew.

## 27. Save/Load

Health state, treatment state, recovery progress, medical assignment, and location persist.

Reloading does not reset injury.

## 28. UI

Crew health UI shows:

- health state;
- injury type where known;
- work restriction;
- treatment requirement;
- current treatment;
- recovery estimate;
- assignment coverage impact.

## 29. Notification

Critical injury to a crew member generates a high-priority notification.

Minor injury does not interrupt gameplay unnecessarily unless it affects critical operation.

## 30. Edge Cases

If a medbay loses power during treatment, treatment pauses or falls back to available local emergency capability.

If the Medic is injured, medical capacity recalculates.

If two Critical patients arrive with one treatment slot, triage order resolves deterministically.

If a Recovering crew member's previous assignment was removed, they return Unassigned after clearance.

If an injured crew member is on an authored story mission, the story mission owns extraction/continuation consequences while respecting global permanent-death rules unless explicitly marked as a narrative-death case.

## 31. Progression

Early game:

- basic first aid;
- limited medical space;
- longer recovery;
- high staffing impact.

Mid game:

- dedicated Medic;
- medbay;
- better supplies;
- rescue robots.

Late game:

- advanced treatment;
- multiple beds;
- distributed emergency response;
- faster stabilization;
- strong medical redundancy.

## 32. Explicit Non-Goals

The health system does not provide:

- routine permanent crew death;
- random permadeath rolls;
- instant full healing from a menu;
- teleporting patients;
- medical treatment without resources/capacity where required;
- cloning as a required core mechanic.

## 33. Tuneable Parameters

Tuneable values include injury thresholds, workload penalties, deterioration timing, stabilization time, treatment duration, supply cost, bed capacity, and medical modifiers.

## 34. Dependencies

This specification depends on:

- [Crew Needs](crew_needs.md);
- [Assignments](assignments.md);
- [Professions](professions.md);
- [Skills and Traits](skills_and_traits.md);
- station atmosphere, thermal, security, damage, automation;
- combat and medical-resource specifications.

## 35. Open Questions

None in the current health baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
