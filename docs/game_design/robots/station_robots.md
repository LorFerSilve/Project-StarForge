# Station Robots

> **Status:** Design Complete  
> **Authority:** Canonical station robot roles, automation integration, physical task execution, charging/storage, service territories, security behavior, crew supervision, and station-side deployment

## 1. Purpose

Station robots turn solved repetitive physical work into configurable automation without making station logistics or crew expertise fictional.

## 2. Station Robot Philosophy

Station robots are real physical robots assigned to Horizon Station.

They:

- move through actual paths;
- require power/charge;
- use tools;
- consume/transport real resources;
- can be blocked/damaged;
- have finite throughput.

They do not provide passive percentage bonuses merely because they exist.

## 3. Canonical Station Robot Roles

Baseline station-side roles:

1. Maintenance / Repair Drone;
2. Logistics Carrier;
3. Agriculture Service Unit;
4. Security Sentinel;
5. General Utility Unit.

These are role configurations and can share compatible chassis/components with field classes.

## 4. Maintenance / Repair Drone

Primary tasks:

- routine inspection;
- repair;
- fault response;
- component replacement;
- emergency stabilization.

Requires:

- repair tool;
- diagnostics;
- manipulator;
- access;
- spare materials/components.

## 5. Logistics Carrier

Primary tasks:

- move resources;
- move components;
- deliver production inputs;
- remove outputs;
- resupply ammunition/repair stores.

Requires:

- cargo capacity;
- manipulator/loading interface;
- route access.

It cannot teleport inventory between containers.

## 6. Agriculture Service Unit

Primary tasks:

- seed/plant handling where automated farm supports it;
- harvest;
- nutrient/consumable delivery;
- cleaning/service;
- moving produce to output.

Farmer profession still owns agricultural supervision/optimization where crew is assigned.

## 7. Security Sentinel

Primary tasks:

- patrol;
- guard;
- respond to intrusion;
- support lockdown;
- defend assigned area.

Uses GDS-9 combat and GDS-2 Security permissions.

Security Officer can supervise policy/response.

## 8. General Utility Unit

Primary tasks:

- simple physical labor;
- move light equipment;
- inspections;
- routine station service;
- assist other automation domains.

It cannot substitute for specialized tool/capability requirements.

## 9. Station vs Field Mode

A compatible robot can be reassigned from station duty to field duty if:

- its class/configuration is field-valid;
- removed from active station assignment;
- transported physically;
- squad/TCC requirements satisfied.

A robot cannot be simultaneously working at station and deployed off-station.

## 10. Automation Task Integration

GDS-2 Station Automation remains owner of generic task lifecycle:

Detected  
→ Queued  
→ Assigned  
→ In Progress  
→ Blocked/Escalated  
→ Completed.

Station robot is an eligible physical task actor.

## 11. Eligibility

Scheduler selects robot only if it has:

- required tool/capability;
- sufficient operational state;
- physical access;
- adequate energy;
- authorization;
- cargo/part capacity where needed.

## 12. No Infinite Worker Capacity

One robot performs finite work.

More robots, better tools, better routing, or better automation can increase throughput.

No robot supervises infinite tasks.

## 13. Crew Supervision

Robotics Specialist can improve:

- diagnostics;
- task allocation;
- robot fleet maintenance;
- advanced robot availability.

Domain specialists still matter.

Examples:

- Engineer defines/diagnoses complex utility response;
- Logistics Officer manages routing policy;
- Security Officer manages security policy;
- Farmer manages agriculture policy.

Robots execute bounded physical work.

## 14. Autonomous Routine Work

Once policy is configured, robots can execute routine tasks without individual player confirmation.

Examples:

- move output;
- repair normal low-risk fault;
- harvest mature crop;
- recharge;
- patrol.

## 15. Strategic Boundary

Station robots cannot independently:

- spend protected strategic resources without permission;
- redesign station layout;
- choose research;
- initiate raid;
- change faction policy;
- dismantle critical infrastructure outside authorized maintenance policy.

## 16. Service Territory

A station robot can be assigned to:

- whole eligible station;
- station zone;
- department;
- specific module group.

Territory limits scheduler assignment.

## 17. Physical Navigation

Robots obey:

- doors;
- access control;
- pressure;
- gravity;
- corridor size;
- lifts;
- damaged paths.

A robot cannot pass a sealed bulkhead because task exists beyond it.

## 18. Hazard Compatibility

Robots can work in hazards only when their configuration supports them.

A vacuum-rated repair drone can work outside where human crew may not safely operate.

Radiation/heat/EMP can still damage it according to ratings.

## 19. Charging

Station robots use Charging/Service Docks.

A dock provides:

- electrical recharge;
- diagnostics;
- optional ammo/tool resupply;
- storage/standby location.

## 20. Charging Capacity

Each dock has finite simultaneous charging/service slots and power draw.

More robots than docks can queue.

## 21. Automatic Charging

Robot AI can return to charge when:

- energy below policy threshold;
- no higher-priority emergency task requires remaining endurance;
- dock is reachable/available.

## 22. Low-Energy Emergency

If energy becomes insufficient away from dock:

- robot reports condition;
- attempts safe return if possible;
- can become Power-Depleted/Disabled.

It is not teleported to charger.

## 23. Robot Storage

Inactive robots occupy physical storage/parking/rack space.

The station roster UI can aggregate them but does not erase physical location.

## 24. Maintenance Queue

Damaged station robots can create automation repair tasks.

A repair-capable robot can repair another robot if:
- capability/parts/access exist.

Fleet maintenance can therefore itself depend on healthy service robots/crew.

## 25. Self-Sustaining Boundary

Automation can become highly self-maintaining but must still conserve:

- energy;
- parts;
- materials;
- labor;
- physical access.

No closed free repair loop exists.

## 26. Logistics Carrier Transaction

When a carrier moves cargo:

Source Inventory  
→ Carrier Cargo  
→ Destination Inventory.

The resource quantity has one authoritative owner throughout.

## 27. Blocked Destination

If destination is full/inaccessible:

- carrier retains cargo or returns it according to policy;
- task becomes Blocked/Escalated;
- resources are not deleted.

## 28. Security Patrol

Security Sentinel patrol uses:

- actual route;
- sensors;
- access permissions;
- ROE.

No omniscient intruder detection.

## 29. Boarding Response

During hostile boarding, Security system/Station Defense can assign:

- Security Sentinels;
- available tactical robots;
- doors/lockdowns;
- crew Security Officer response.

GDS-11 owns full event orchestration.

## 30. Repair During Station Attack

Repair robots can respond during attack if:

- safe enough under policy;
- route exists;
- resources available.

The player can configure risk/priority.

## 31. Off-Screen Station Simulation

While player is away during active gameplay:

- robot task progress can be simulated at lower detail;
- output must remain equivalent to physical time, energy, resources, access, damage, and assignment.

## 32. No Offline Progression

Robot work/charging stops when game simulation is not running.

## 33. Station Robot Loss

Destroyed station robots remain destroyed.

Wrecks/disabled units remain physically at station and can generate recovery/salvage/repair tasks.

No post-event automatic restoration occurs.

## 34. Reserve Security Force

Player can keep combat robots stationed as Security Reserve.

They:
- consume station storage/maintenance/charging;
- can respond to defense;
- are unavailable for external mission if simultaneously assigned/deployed.

## 35. Ammo Resupply

Armed station robots consume actual ammunition.

Logistics/charging stations can resupply from station inventory according to policy.

No infinite security ammo.

## 36. Alarm Integration

Critical robot failures/attacks can generate station alerts through GDS-2.

Routine charging does not spam the player.

## 37. Persistence

Assignments, service territory, task state, cargo, energy, dock state, patrol state, damage, and physical location persist.

## 38. Edge Cases

If a logistics robot becomes Disabled while carrying resources, those resources remain owned by its cargo until physically recovered/transferred.

If charging dock loses power, connected robots stop charging but retain existing energy.

If station decompression isolates a robot that is vacuum-capable, it may continue work if path/policy allows; noncompatible robot becomes blocked/damaged.

If robot is reassigned to mission while holding station cargo, cargo must be unloaded/transferred before deployment validation unless explicitly part of mission loadout.

## 39. Explicit Non-Goals

Station robots do not provide:

- teleporting logistics;
- infinite task capacity;
- free energy;
- free repairs;
- omniscient security;
- simultaneous station and field presence;
- automatic strategic decision-making.

## 40. Tuneable Parameters

Task speed, cargo capacity, charging rate, dock capacity, patrol spacing, energy thresholds, service territories, and maintenance demand are tuneable.

## 41. Dependencies

This specification depends on GDS-2 Station Automation/Logistics/Security/Power, GDS-3 Crew, Robot AI/Components/Damage, GDS-4 Resources, GDS-9 Combat, and Raids/Progression.

## 42. Open Questions

None in the station-robot baseline.
