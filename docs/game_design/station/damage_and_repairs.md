# Station Damage and Repairs

> **Status:** Draft  
> **Authority:** Persistent station damage, faults, breaches, destruction states, stabilization, repair, reconstruction, and repair automation

## 1. Purpose

Damage makes attacks, accidents, and infrastructure failures consequential without allowing routine events to erase the player's complete long-term base.

Repair turns those consequences into engineering, logistics, crew, and automation gameplay.

## 2. Damage Layers

The baseline distinguishes:

- Structural Integrity: whether structure and hull remain physically intact.
- Functional Condition: whether installed machinery can perform its purpose.
- Fault State: discrete failures such as a broken relay, coolant leak, jammed door, failed sensor, or short circuit.

A room can remain structurally intact while its machinery is disabled.

## 3. Structural Integrity States

Structural elements use:

- Intact
- Damaged
- Critical
- Broken or Destroyed

Exact thresholds are tuneable.

## 4. Functional Condition States

Functional equipment uses:

- Operational
- Degraded
- Disabled
- Destroyed

Subsystem faults can further describe the reason.

## 5. Damage Sources

Damage may originate from:

- hostile weapons;
- boarding combat;
- explosions;
- fire;
- overheating;
- structural failure;
- decompression consequences;
- coolant or fluid failure;
- debris impact;
- authored accidents.

## 6. Explicit Damage Resolution

Damage is applied to authored gameplay targets rather than arbitrary mesh triangles.

A target definition determines:

- integrity loss;
- armor/protection;
- functional degradation;
- resulting faults;
- breach creation;
- topology changes.

## 7. No Automatic Cascading Damage

Damage propagates only through explicit mechanics such as:

- explosion radius;
- fire spread;
- thermal runaway;
- pressure breach;
- structural disconnection.

A damaged module does not harm neighbors simply because it is damaged.

## 8. Breaches

Pressure-boundary damage can create a breach with a defined severity class.

The atmosphere system owns decompression behavior.

Repairing general module condition does not silently remove an active breach unless the repair action explicitly includes sealing it.

## 9. Structural Breaks

When a structural connection breaks:

- the structural graph recalculates;
- dependent utility paths recalculate;
- branches can become disconnected.

Utilities do not remain magically connected across destroyed structure.

## 10. Destroyed Modules

A destroyed permanent module remains as a persistent damaged or wrecked state until it is:

- reconstructed;
- salvaged;
- removed through valid deconstruction.

It does not instantly disappear during combat.

## 11. Command Core Rule

The Command Core can be heavily damaged and lose normal capability.

Routine gameplay cannot permanently delete it or remove the protected recovery path defined by the station overview.

## 12. Persistence

Damage persists across:

- leaving the station;
- missions;
- travel;
- save/load.

There is no automatic full heal on scene transition.

## 13. Emergency Stabilization

Some failures can be stabilized before full repair.

Examples:

- patching a hull breach;
- isolating a coolant leak;
- opening a bypass power route;
- bracing a critical structural link;
- extinguishing fire.

Stabilization prevents or slows escalation but does not count as full restoration.

## 14. Repair Requirements

A repair job may require:

- physical or remote access;
- repair materials;
- replacement components;
- tools;
- labor;
- safe environmental conditions;
- shutdown or isolation of the target.

The target definition declares which requirements apply.

## 15. Physical Repair Materials

Repair materials come from actual inventories through logistics.

A repair cannot spend inaccessible resources.

Emergency repair kits can provide limited local supplies.

## 16. Repair Labor

Eligible labor can come from:

- player;
- Engineer or other qualified crew;
- repair drone;
- specialized automatic equipment.

Different fault classes can require different capabilities.

## 17. Player Repair

The player can perform compatible repairs with appropriate tools.

Manual repair is especially important early-game and during emergencies.

It never bypasses missing parts or impossible access.

## 18. Crew Repair

Qualified crew can diagnose, coordinate, and perform repairs according to crew rules.

Crew cannot create required materials or work in conditions their safety rules prohibit.

## 19. Repair Drones

Repair drones have finite:

- number;
- task compatibility;
- access;
- speed;
- availability.

One drone cannot service unlimited simultaneous failures.

## 20. Repair Queue

Automated repair tasks record:

- target;
- fault;
- severity;
- required resources;
- assigned actor;
- access state;
- progress;
- priority;
- blocking reason.

## 21. Default Repair Priority

Default priority is:

1. immediate occupant safety;
2. Command Core recovery;
3. life support;
4. structural integrity;
5. thermal and power stability;
6. active defense;
7. logistics and industry;
8. noncritical damage.

Eligible priorities can be reconfigured by the player.

## 22. Manual Lockout

The player can exclude equipment from automatic repair.

Automation respects the lock unless an explicitly enabled emergency policy authorizes the specific override.

## 23. Repair Progress

Progress advances only while:

- labor remains available;
- required materials remain available;
- access remains valid;
- the target remains repairable.

Interruptible repairs preserve completed progress.

## 24. Reconstruction

A destroyed module can require reconstruction rather than ordinary repair.

Reconstruction uses construction-like material and labor rules.

If reconstruction is impossible, the wreck must be salvaged/deconstructed and rebuilt.

## 25. Salvage

Destroyed modules can yield salvage based on:

- original material;
- remaining condition;
- recovery method.

Salvage cannot exceed physically recoverable material and cannot duplicate inventory.

## 26. Unsafe Areas

Automation does not send unprotected crew into immediately lethal environments.

Possible alternatives are:

- protected crew;
- repair drones;
- remote isolation;
- stabilization before entry.

## 27. Diagnosis

Remote information may show:

- exact fault;
- suspected fault;
- generic malfunction;
- no useful data.

Sensor and control capability determine diagnosis quality.

Local inspection can reveal more.

## 28. Damage During Repair

A target can take new damage during live gameplay while being repaired.

Repair work never grants invulnerability.

## 29. Multiple Faults

One module can contain several independent faults.

Repairing one does not clear unrelated failures.

## 30. Repair UI

The station interface shows:

- damaged modules;
- severity;
- breaches;
- faults;
- disconnected systems;
- repair requirements;
- assigned labor;
- missing resources;
- progress;
- blocking cause.

## 31. Visual Feedback

Persistent damage is represented where practical through:

- broken panels;
- scorch marks;
- sparks;
- leaks;
- emergency lighting;
- disabled machinery;
- hull damage.

Visual state must correspond to actual gameplay state when it communicates functionality.

## 32. Audio Feedback

Relevant audio includes:

- impacts;
- alarms;
- gas or coolant leaks;
- electrical faults;
- repair tools;
- stabilization confirmation.

Critical state is also communicated visually.

## 33. Edge Cases

If repair materials run out, the repair pauses without recreating consumed resources.

If the assigned worker becomes unavailable, the task pauses or is reassigned.

If the target is destroyed mid-repair, the old repair task converts to the appropriate destruction/reconstruction state.

If the final broken structural connection is restored, all dependent topologies recalculate before systems decide whether they can operate.

## 34. Progression

Early game emphasizes manual repair and limited diagnostics.

Mid game introduces Engineers, repair queues, drones, spares, and automatic isolation.

Late game supports distributed repair capacity, advanced diagnostics, autonomous stabilization, and rapid reconstruction.

## 35. Explicit Non-Goals

The damage system does not provide:

- automatic free healing;
- unrestricted per-polygon destruction;
- routine permanent total-station loss;
- repairs without required inputs;
- hidden mechanical damage represented only cosmetically.

## 36. Tuneable Parameters

Tuneable values include integrity, armor, repair duration, material cost, salvage return, stabilization duration, and repair-drone rate.

## 37. Dependencies

This document depends on the authoritative structural, power, atmosphere, thermal, water, logistics, automation, resource, crew, combat, and event specifications.

## 38. Open Questions

None in the current baseline.

The document remains Draft until dependent domains are cross-validated.
