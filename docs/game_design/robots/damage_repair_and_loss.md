# Robot Damage, Repair and Loss

> **Status:** Draft  
> **Authority:** Robot structural/subsystem damage states, hit zones, disablement, destruction, field repair, station repair, wreck recovery, loss, salvage, and mission-failure persistence

## 1. Purpose

Robots must be meaningfully expendable without becoming disposable abstractions.

Damage should create recoverable engineering problems before total destruction where possible.

## 2. Core Robot State

A player robot tracks:

- Chassis Integrity;
- component Condition;
- subsystem operational state;
- Internal Energy Reserve;
- ammunition;
- active Status Effects.

## 3. Chassis Integrity

Chassis Integrity represents primary structural survivability.

At Chassis Integrity > 0:
- robot can remain repairable, subject to subsystem state.

At Chassis Integrity = 0:
- robot enters Destroyed state;
- original Robot ID becomes nonrepairable.

## 4. Canonical Robot Hit Zones

Where chassis geometry supports them:

- Core/Control;
- Power;
- Mobility;
- Sensors/Communication;
- Weapon/Tool;
- Chassis/Armor.

Heavy/specialized units can expose additional authored zones.

## 5. Hit Zone Routing

GDS-9 resolves:

- hit;
- shield;
- armor;
- remaining damage.

Remaining machine damage routes to:

- impacted component;
- local chassis;
- both according to chassis/component definition.

## 6. Operational State

Canonical whole-unit states:

- Operational;
- Degraded;
- Disabled;
- Destroyed.

## 7. Operational

Robot can execute its mandatory class capabilities.

Minor damage may exist.

## 8. Degraded

Robot remains deployable/controllable but one or more capabilities are reduced or lost.

Examples:

- secondary sensor damaged;
- one weapon disabled;
- armor degraded;
- reduced mobility.

## 9. Disabled

Robot remains physically recoverable but cannot perform its primary autonomous role.

A robot becomes Disabled when any mandatory condition occurs, such as:

- Control Core nonfunctional;
- Power System cannot provide minimum operating power;
- all valid mobility disabled for a mobile chassis;
- chassis-specific critical control path disabled.

A class-specific tool loss alone can make the unit **Class-Incapable** without necessarily whole-unit Disabled.

## 10. Class-Incapable

A robot may remain mobile/controllable while no longer capable of its primary class function.

Examples:

- Shield Unit loses Shield Projector;
- Assault Unit loses all weapons;
- Repair Drone loses Repair Tool.

It can:
- retreat;
- follow;
- perform remaining secondary capabilities.

Mission/class validation reports the missing capability.

## 11. Destroyed

At Chassis Integrity = 0:

- active AI stops;
- commands stop;
- energy/ammo no longer function as operational unit;
- Robot ID is retired as Destroyed;
- a Wreck entity can remain.

Destroyed cannot be repaired back into same Robot ID.

## 12. Component Destruction

A component can become Disabled/Destroyed while chassis survives.

If field-replaceable:
- spare replacement can restore capability.

If not:
- station/ship service is required.

## 13. Control Core Damage

Disabled Control Core causes whole-unit Disabled.

Replacing/repairing it preserves Robot ID if chassis remains >0 and security validation succeeds.

## 14. Power Damage

Power System failure disables powered operation.

Stored charge is not automatically lost unless power/battery component damage explicitly destroys it.

## 15. Mobility Damage

Mobility consequences depend on architecture.

Examples:

- wheeled unit with one damaged wheel may be slowed;
- tracked unit may lose steering/movement;
- legged unit can lose speed/stability;
- flying drone losing required lift/thruster can crash/fall.

Exact chassis thresholds are data.

## 16. Weapon/Tool Damage

A destroyed weapon/tool stops that capability.

It does not necessarily Destroy the robot.

## 17. Sensor/Communication Damage

Possible effects:

- reduced perception;
- loss of shared squad data;
- Disconnected communication state;
- inability to receive commands.

Fallback AI then applies.

## 18. EMP

GDS-9 EMP/Disrupted status can temporarily disable:

- communication;
- sensors;
- shield;
- control;
- powered components

according to resistance.

Temporary EMP does not equal Destroyed.

## 19. Repair Levels

Robot repair has three levels:

1. Field Stabilization;
2. Field Component Repair/Replacement;
3. Full Service Repair.

## 20. Field Stabilization

Purpose:
- stop worsening state;
- restore minimum mobility/control;
- recover Disabled robot.

Can include:
- reconnect power;
- patch damaged wiring;
- temporary mobility fix;
- emergency reboot.

It does not restore full condition by default.

## 21. Field Component Repair/Replacement

Requires:

- Repair-capable actor;
- physical access;
- field-serviceable component;
- spare component/material;
- time;
- safe enough conditions.

Consumes actual inputs.

## 22. Full Service Repair

At compatible ship/station facility, repair can restore:

- chassis integrity;
- component condition;
- calibration;
- full diagnostics.

Requires canonical materials/components/power/work.

## 23. Repair Ownership

Repairing a robot does not create replacement components.

Installed replacement transfers from actual inventory/storage into robot ownership.

Removed damaged part becomes:

- repairable component;
- salvage;
- scrap/waste state

according to component definition.

## 24. Robotics Specialist

Robotics Specialist can:

- diagnose complex faults;
- supervise repair automation;
- enable advanced control-core/component service;
- improve throughput.

They do not bypass missing parts/tools/access.

## 25. Repair Drone Self-Repair

A Repair Drone cannot fully repair itself when the required damaged component/tool makes that physically impossible.

Limited self-diagnostic/minor service is allowed where hardware supports it.

## 26. Towing / Carrying

Disabled robot recovery requires a capability appropriate to its mass.

Possible methods:

- another robot manipulator/tow;
- player-compatible light-drone carry interaction;
- ship recovery equipment;
- Heavy recovery system.

A Heavy Mech cannot be placed in backpack.

## 27. Wreck State

A Destroyed robot can leave a Wreck with:

- salvageable components;
- materials;
- remaining ammunition/energy components where safe;
- Robot ID historical reference.

The Wreck is not an operational robot.

## 28. Wreck Recovery

Recovering wreck consumes:

- cargo/recovery capacity;
- physical loading time/access.

At station it can be dismantled/salvaged.

## 29. Salvage Yield

GDS-4 owns material salvage.

Robot destruction cannot create more components/material than physically existed.

Damaged/destroyed components can reduce recoverable yield.

## 30. Abandoned Operational Robot

A player can intentionally leave an Operational robot behind.

It follows fallback/world persistence.

If the mission instance closes without persistent recovery path:
- the robot can become Lost.

The player is warned before extraction.

## 31. Lost State

**Lost** is an ownership/recoverability state, not a damage state.

A Lost robot:

- may still physically exist;
- is not currently available to player roster/deployment;
- can become recoverable if a later world/mission rule preserves its location.

If no persistent world recovery is defined, Lost resolves to permanently unavailable.

## 32. Mission Failure

GDS-8 failure uses actual robot state.

By default:

- secured in transport → preserved;
- field Operational/Disabled → recovery fiction determines Lost/recoverable;
- Destroyed → remains Destroyed;
- Wreck → recovered only if physically secured or recovery system explicitly includes it.

## 33. Station Defense

During station defense:

- Disabled robots remain physically in Horizon Station;
- Destroyed robots produce wrecks where valid;
- station automation can create repair/recovery tasks.

They are not restored after battle for free.

## 34. No Robot Permadeath Protection

Unlike human crew, robots have no global protection from ordinary permanent destruction.

This is one reason they are the repeatable assault force.

## 35. Friendly Repair During Combat

Repair can be attempted during combat if:

- physical access;
- capability;
- parts;
- action not interrupted.

No global combat repair ban exists.

## 36. Repair Priority

Automation can prioritize:

- mobility recovery;
- control recovery;
- weapon repair;
- full condition

according to policy.

Strategic/rare component spending can require authorization.

## 37. Energy Recharge vs Repair

Recharging energy is not repair.

A fully charged robot can remain mechanically Disabled.

A repaired robot with zero charge can remain Power-Depleted.

## 38. Ammunition Resupply vs Repair

Resupplying ammo does not repair damage.

Repairing weapon hardware does not create ammunition.

## 39. Dismantling

Operational/Disabled robot dismantling:

- retires Robot ID;
- yields authorized salvage;
- requires service facility.

Destroyed wreck salvage does not require treating the unit as repairable.

## 40. Save/Load

Damage, components, energy, ammo, Lost/Destroyed state, Wreck state, and repair work persist.

A committed Destroyed state cannot be undone by reloading a later save snapshot outside normal user save selection.

## 41. Failure Readability

UI must report:

- which subsystem failed;
- Operational/Degraded/Disabled/Destroyed;
- repair requirements;
- recoverability;
- destruction/loss warning.

## 42. Edge Cases

If Chassis reaches zero in same event that a component repair completes, deterministic event ordering decides whether repair committed before destruction; once Chassis zero commits, Robot ID is Destroyed.

If robot is EMP-Disabled but structurally intact, waiting/reboot can restore it if status expires and energy/control remain valid.

If abandoned robot remains in a persistent Strategic Location, later recovery uses same Robot ID and current condition.

If a Destroyed robot's wreck is later salvaged, a new replacement robot still receives a new Robot ID.

## 43. Explicit Non-Goals

Robot damage does not provide:

- free post-mission restoration;
- Destroyed-to-repaired resurrection;
- generic Robot HP only with no subsystem consequence;
- replacement parts from nothing;
- backpack storage for heavy robots;
- automatic wreck recovery.

## 44. Tuneable Parameters

Integrity, component thresholds, repair quantities/time, salvage yield, tow capacity, field-repair effectiveness, crash damage, and maintenance rates are tuneable.

## 45. Dependencies

This specification depends on GDS-9 Damage/Status, Components, Manufacturing, Squad Extraction, GDS-4 Salvage/Resources, GDS-2 Automation/Repair, Crew Robotics Specialist, and GDS-12 Persistence.

## 46. Open Questions

None in the damage/repair/loss baseline.
