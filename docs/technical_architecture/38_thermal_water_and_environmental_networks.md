# Thermal, Water, and Environmental Networks

> **Status:** Architecture Complete  
> **Authority:** Station heat-energy accounting, coolant-loop transport, radiator rejection, compartment temperature coupling, fresh/wastewater network allocation, leaks, recycling, and deterministic environmental stepping

## 1. Purpose

Thermal and Water are separate conserved-state systems that share station topology, Power dependencies, damage, automation, and off-screen scheduling.

This document defines their runtime architecture while preserving the GDS distinction between:

- heat versus electrical power;
- coolant versus water;
- fresh water versus wastewater;
- equipment temperature versus compartment ambient temperature;
- quantity shortage versus network-throughput shortage.

## 2. Separate Domain Stores

Persistent state is owned by:

```text
StationThermalStore
StationWaterStore
```

They share TA-6 topology facts but never combine their conserved quantities into one generic “utility resource.”

## 3. Thermal State Representation

Major thermal entities use project-owned records such as:

```text
ThermalNodeId
CoolantLoopId
CoolantEdgeId
RadiatorId
HeatStorageId
```

A thermal node tracks at minimum:

- temperature;
- effective heat capacity;
- current heat-energy state or equivalent deterministic accumulator;
- authored safe/operating thresholds;
- current generated heat rate;
- current transfer interfaces.

## 4. Temperature and Energy

TA-6 treats temperature as derived from stored thermal energy and effective heat capacity where practical.

For a simple node:

```text
DeltaTemperature = NetHeatEnergy / HeatCapacity
```

This provides deterministic thermal inertia without per-polygon conduction.

Exact unit scaling is implementation-lock data; conservation behavior is architectural.

## 5. Thermal Domains

The runtime preserves GDS thermal domains:

- equipment temperature;
- coolant-loop working-fluid temperature/energy;
- compartment ambient temperature;
- optional finite heat-storage state.

Transfer between domains occurs only through explicit authored couplings.

## 6. Heat Sources

Power/device systems publish `HeatGenerationRequest` values from actual operating state.

Examples:

- reactor electrical generation;
- manufacturing processing;
- research;
- weapons;
- electronics;
- fires;
- ship service.

Heat is added during the thermal step exactly once.

## 7. Coolant Loop Topology

A `CoolantLoopRuntime` is derived from:

- coolant-capable utility links;
- pumps;
- valves;
- heat exchangers;
- equipment interfaces;
- radiators;
- current damage/leak state.

Disconnected loop components are solved independently.

## 8. Coolant Inventory

Each loop owns finite coolant quantity.

Quantity affects effective transport capability according to authored thresholds.

A leak removes coolant from the loop over Simulation Time. Lost coolant is not automatically returned.

## 9. Coolant Flow

Flow requires a valid path and operating pump capability unless an authored passive system explicitly supports circulation.

Each edge/pump has finite throughput.

The solver computes deliverable flow through the connected loop rather than granting every connected device full cooling automatically.

## 10. Thermal Transfer

Equipment-to-coolant, coolant-to-radiator, coolant-to-compartment, and climate-control transfers use bounded conductance/throughput parameters.

A transfer is computed as an energy delta and applied conservatively:

```text
SourceHeatEnergy -= transferred
DestinationHeatEnergy += transferred
```

Radiator rejection is an explicit sink to exterior space and is tracked separately.

## 11. Radiators

Radiators publish rejection capacity based on:

- deployed/operational state;
- connected coolant flow;
- damage;
- current radiator/coolant temperature;
- authored environment/context modifier.

Rejected heat is removed from station thermal ownership through this explicit sink.

## 12. Thermal Protection

Devices consume thermal read models and can request:

- output reduction;
- additional cooling priority;
- shutdown;
- restart when safe.

Thermal does not directly change reactor output/manufacturing speed; it publishes temperature/cooling facts used by the owning device state machine.

## 13. Emergency Shutdown

A device crossing its authored emergency threshold can commit a protective shutdown through its owning system.

Restart requires the real temperature/fault conditions to pass.

No “reset temperature on restart” exists.

## 14. Thermal Runaway

Runaway is represented only for authored device profiles.

It is a feedback relation where current state can increase future heat generation or reduce safe operating margin.

The loop remains Simulation-Time driven and deterministic.

## 15. Compartment Ambient Temperature

Each pressure cell/derived compartment has an ambient thermal node or mapped ambient-state record.

Ambient heat changes from:

- equipment transfer;
- occupants;
- fire;
- climate control;
- adjacent authored exchange;
- exterior abstraction.

Atmosphere consumes this temperature for pressure and habitability calculations.

## 16. Climate Control

Climate-control devices move heat between compartment ambient state and a valid thermal rejection path.

They cannot lower room temperature indefinitely if the destination loop/radiator cannot accept/reject the energy.

## 17. Thermal Step

A normal thermal step follows:

1. latch device heat-generation facts;
2. resolve coolant topology/available flow;
3. calculate equipment↔coolant and ambient↔control transfers;
4. calculate radiator/exterior rejection;
5. apply simultaneous energy deltas;
6. update temperatures/bands;
7. publish thermal facts/protection threshold crossings.

## 18. Water State Representation

Water uses typed project-owned records such as:

```text
WaterNodeId
WaterEdgeId
FreshWaterTankId
WastewaterTankId
WaterPumpId
WaterRecyclerId
```

Fresh Water and Wastewater are distinct conserved inventories.

## 19. Water Topology

The runtime Water Graph is derived from:

- fresh-water/wastewater utility links;
- tanks;
- pumps;
- valves;
- consumers;
- recyclers;
- producers/external input interfaces;
- current damage/isolation state.

Disconnected components do not share inventory.

## 20. Water Demand Request

A consumer publishes a request containing:

- required water category;
- requested quantity/rate;
- minimum viable quantity/rate where applicable;
- priority;
- local input-buffer capacity;
- destination endpoint;
- operating interval/deadline.

Water decides deliverability. Owning systems decide effect of under-supply.

## 21. Water Priority

Water allocation preserves the GDS order by default:

1. critical life-support process;
2. crew habitability;
3. essential farming;
4. important industry;
5. discretionary process.

Eligible player policy can reorder configurable consumers while protected safety constraints remain enforced.

## 22. Water Distribution Solver

Within each connected network island, TA-6 performs deterministic capacity-constrained allocation from eligible storage/production nodes to demand nodes.

Constraints include:

- source inventory;
- pump capability;
- edge throughput;
- reserve policy;
- consumer priority;
- destination capacity.

A network can have enough total water while still failing a local demand because of pump/edge bottlenecks.

## 23. Water Transfer Commit

Unlike Power flow, water changes persistent quantity ownership.

A transfer uses TA-2 conservation semantics:

```text
Source quantity - delivered quantity
Destination/local buffer + delivered quantity
```

Both changes commit together.

## 24. Crew Consumption Aggregation

Routine crew water use is aggregated over Simulation Time by occupied station population/state.

It creates demand against reachable Fresh Water. No per-person drinking item transaction is required for routine station accounting.

## 25. Wastewater Production

Processes that generate wastewater explicitly add quantity to valid local/output wastewater ownership.

If output capacity is unavailable, the owning process follows its GDS blocked/degraded behavior rather than silently deleting wastewater.

## 26. Recycling

A recycler executes a bounded process:

```text
Wastewater consumed
+ Power
+ valid machine state
→ Fresh Water output
+ explicit unrecovered loss
```

Recovery efficiency is `<= 100%` unless a future explicit external-source technology changes the process definition.

## 27. Water Leaks

A damaged tank/edge can own a `WaterLeakState` with finite loss rate.

Leak stepping:

- removes only actually available water;
- can lower local throughput;
- stops when source is depleted or isolated/repaired;
- never becomes an invisible negative inventory.

Water released to vacuum is an explicit external loss.

## 28. Reserve Policy

Fresh-water storage can define protected reserve.

Normal lower-priority allocation cannot consume below reserve unless policy explicitly authorizes the relevant emergency use.

## 29. Farming Integration

Farms request:

- Fresh Water;
- Power;
- valid atmosphere;
- valid temperature;
- nutrients from Logistics.

TA-6 Water supplies only the water fact. Agriculture state/growth is handled in the station work-scheduling specification.

## 30. Atmosphere Integration

Oxygen generators or other life-support devices can consume Fresh Water through normal water transfer/work semantics.

Atmosphere receives only the resulting gas-production effect after required input/power/device conditions pass.

## 31. Thermal Make-Up Integration

Where an authored cooling process consumes water, it does so through a normal Water demand/transaction.

Coolant remains a distinct thermal working fluid unless a specific content definition explicitly uses water as that fluid.

## 32. Off-Screen Environmental Integration

Thermal and Water support deterministic larger off-screen steps only when the scheduler subdivides at relevant boundaries such as:

- tank empty/full;
- protected reserve reached;
- leak isolation;
- temperature threshold;
- device shutdown/restart threshold;
- crop exposure tolerance boundary;
- crew habitability threshold.

## 33. Persistence

Thermal persistence includes:

- temperatures/heat-energy state;
- coolant quantities;
- valve/pump configuration;
- leaks;
- heat storage;
- faults.

Water persistence includes:

- Fresh/ Wastewater quantities;
- tank state;
- reserve policy;
- valve/pump state;
- leaks;
- recycler state/configuration.

Derived routing/flow workspaces are reconstructed.

## 34. Debugging

Thermal diagnostics expose:

- node temperature/energy;
- generated/transferred/rejected heat;
- coolant quantity/flow;
- bottlenecks;
- radiator utilization;
- threshold crossings.

Water diagnostics expose:

- source/destination quantities;
- network islands;
- pump/edge utilization;
- demand allocation;
- reserves;
- recycler mass balance;
- leak losses.

Both systems include conservation accounting with explicit external sources/sinks.

## 35. Explicit Non-Goals

TA-6 does not implement:

- per-triangle conduction;
- full fluid hydraulics;
- molecular thermodynamics;
- perfect lossless water recycling by default;
- hidden water teleportation;
- coolant/water as one interchangeable inventory unless content explicitly says so.

## 36. Dependencies

Depends on TA-6 topology/Power/Atmosphere, TA-2 transactions, TA-3 off-screen simulation, GDS Thermal, Water, Farming, Damage/Repair, Resources, and Automation.

## 37. Open Questions

None within TA-6 thermal/water architecture.
