# Robot Manufacturing

> **Status:** Design Complete  
> **Authority:** Robot production prerequisites, Blueprint/research requirements, assembly process, ownership creation, configuration commit, dismantling, repair-vs-rebuild boundary, and manufacturing queues

## 1. Purpose

Robots enter the player's force through actual industrial production/acquisition rather than abstract training timers.

## 2. Production Philosophy

"Training troops" from the original strategic inspiration becomes:

Research/Blueprint  
→ Manufacture Components  
→ Assemble Robot  
→ Configure Software/Loadout  
→ Test/Commission  
→ Assign/Deploy.

## 3. Manufacturing Facility

Player-built robot production requires a compatible **Robot Fabrication/Assembly capability** at Horizon Station.

The exact station module is owned by GDS-2 Manufacturing content but must provide:

- assembly workspace;
- power;
- logistics access;
- component input;
- output space;
- software/configuration interface.

## 4. Research Requirement

A robot chassis/component can require:

- Technology Research;
- Blueprint;
- both.

GDS-4 remains authoritative for Research/Blueprint distinction.

## 5. Physical Inputs

Production consumes canonical GDS-4 resources/components.

Typical inputs include:

- Structural Frame;
- Servo Assembly;
- Basic/Advanced Circuits;
- Sensor Package;
- Power Regulator;
- armor materials;
- weapon components;
- specialized advanced components.

## 6. No Generic Robot Currency

There is no "Robot Point", "Training Elixir", or universal troop currency.

Production consumes actual materials, components, station power, capacity, and Simulation Time.

## 7. Production Recipe

Every robot chassis/model has a manufacturing recipe defining:

- required Blueprint/Research;
- component inputs;
- structural materials;
- electronics;
- assembly work;
- power consumption;
- facility class;
- optional specialist requirement.

## 8. Manufacturing Queue

Robot production uses normal GDS-2/GDS-4 manufacturing queue semantics.

Starting production:

- reserves valid inputs;
- does not duplicate them;
- creates one Work Order.

## 9. Assembly Stages

Canonical logical stages:

1. Parts Reserved;
2. Chassis Fabrication;
3. Component Installation;
4. Software/Control Initialization;
5. Diagnostics;
6. Commissioning;
7. Output Ready.

Implementation can combine stages visually while preserving transaction meaning.

## 10. Robot ID Creation

A new persistent Robot ID is created at **Commissioning commit**, not at queue creation.

Before Commissioning, production is a Work Order/unfinished assembly.

## 11. Commissioning Requirements

Commissioning requires:

- required assembly complete;
- mandatory components present;
- control core operational;
- no blocking diagnostic failure;
- valid output/storage location.

## 12. Output Full

If the output area cannot accept the commissioned robot:

- production pauses before final output commit;
- no robot is deleted;
- no duplicate Robot ID is created.

## 13. Initial Energy

A newly commissioned robot must receive a defined initial charge from station electrical power.

It does not spawn with free infinite energy.

## 14. Initial Ammunition

Combat robots do not spawn with free ammunition unless ammunition is explicitly included/consumed by the manufacturing/loadout transaction.

Normally:

Robot production  
+ separate ammunition stock  
→ loadout preparation.

## 15. Configuration Before Commissioning

The Work Order can specify initial compatible components/software.

Changing configuration before installation updates required inputs only before relevant commit points.

## 16. Post-Commission Modification

After commissioning, components are changed through Robot Component service/maintenance rather than re-running full manufacturing.

## 17. Robotics Specialist

A Robotics Specialist can improve:

- throughput;
- diagnostics;
- complex-model eligibility;
- repair supervision;
- automated production management.

The Specialist does not replace missing Blueprint, components, facility, or power.

## 18. Unstaffed Production

Basic researched robot production can be possible through sufficiently capable automated fabrication.

Advanced/experimental models can require qualified Robotics supervision.

This follows Crew/Automation progression rather than making one human mandatory forever.

## 19. Interrupted Production

If power/logistics/facility fails:

- Work Order pauses/blocks;
- committed inputs remain in authoritative work-in-progress state;
- production does not reset to zero unless physical damage destroys work.

## 20. Facility Damage

Damage to an unfinished robot assembly can:

- damage installed components;
- destroy work-in-progress;
- require repair/replacement.

Station Damage owns the physical damage event.

## 21. Cancellation

Cancelling before irreversible assembly returns unconsumed reserved inputs.

Already transformed/installed materials follow the manufacturing cancellation/salvage rule.

No full free refund after irreversible work.

## 22. Dismantling Operational Robot

The player can deliberately dismantle a robot at a valid service facility.

Dismantling:

- permanently retires the Robot ID;
- recovers an authored fraction of reusable components/materials;
- does not yield more than original physical content;
- preserves no magical copy.

## 23. Rebuild vs Repair

If Robot ID is Disabled but repairable:
- repair preserves the same Robot ID.

If Destroyed:
- the Robot ID cannot be repaired;
- a replacement requires a new manufacturing Work Order and new Robot ID.

## 24. Captured/Recovered Robots

A mission can yield a non-player robot chassis.

Conversion to player ownership requires explicit:

- compatibility;
- control/security authorization;
- repair;
- reprogramming/research where applicable.

Finding an enemy robot does not automatically add it to the player's roster.

## 25. Unique Robots

An authored unique robot can have a unique Blueprint/configuration.

Unique status does not bypass physical manufacturing/repair rules unless the narrative explicitly grants the physical unit.

## 26. Production Automation

The player can configure robot-production targets such as:

- maintain 4 Repair Drones;
- maintain 6 Assault Units in reserve.

Automation can queue replacements only if:

- policy authorizes it;
- required resources are unprotected/authorized;
- fabrication capacity exists.

Strategic resources remain approval-gated.

## 27. No Offline Production

Manufacturing advances only during Simulation Time.

## 28. Persistence

Work Orders, reserved inputs, WIP stage, diagnostics, completed output, and newly created Robot IDs persist.

## 29. Edge Cases

If production finishes while the player is away, the robot remains physically at its output/storage location.

If a required component is removed/destroyed from WIP, commissioning blocks.

If the player changes automation target after one replacement has already reached irreversible assembly, that unit completes or must be explicitly cancelled under normal rules.

## 30. Explicit Non-Goals

Robot manufacturing does not use:

- abstract troop training currency;
- instant troop creation;
- free ammunition;
- free full-charge energy;
- full refunds after irreversible work;
- repair of Destroyed Robot IDs.

## 31. Tuneable Parameters

Recipe quantities, assembly times, electrical work, facility throughput, specialist bonuses, salvage fractions, and initial charge target are tuneable.

## 32. Dependencies

This specification depends on GDS-4 Manufacturing/Resources/Blueprints/Research, GDS-2 Station Manufacturing/Automation/Power/Logistics, Crew Robotics Specialist, Components, Damage/Repair, and GDS-12 Progression/Economy.

## 33. Open Questions

None in the robot-manufacturing baseline.
