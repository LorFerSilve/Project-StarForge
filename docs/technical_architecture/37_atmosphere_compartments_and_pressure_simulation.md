# Atmosphere Compartments and Pressure Simulation

> **Status:** Architecture Complete  
> **Authority:** Pressure-cell runtime model, gas conservation, portal flow, ventilation, breaches, airlocks, compartment derivation, life-support coupling, and deterministic atmosphere stepping

## 1. Purpose

This specification realizes the GDS Atmosphere/Oxygen model without full fluid dynamics while preserving:

- finite atmosphere;
- pressure and composition;
- compartment isolation;
- decompression;
- ventilation;
- oxygen/CO2/contaminant processing;
- airlock behavior;
- deterministic split/merge outcomes;
- active/off-screen equivalence.

## 2. Stable Pressure Cells

Canonical atmosphere state is stored per stable `PressureCellId` contributed by TA-6 module topology.

Each cell contains:

- authored geometric volume;
- gas inventory;
- linked thermal temperature reference;
- sensor/visibility metadata;
- persistent atmosphere-related faults where applicable.

Cells remain stable when doors open/close. Gas is not re-owned by temporary UI compartment objects.

## 3. Gas Inventory

The baseline internal gas state tracks conserved amounts for:

```text
Oxygen
CarbonDioxide
Contaminant
Other/Inert Gas
```

Internal units are project-owned fixed/float simulation quantities chosen for numeric stability. Player-facing presentation derives kPa and composition percentages.

The system never stores only percentages because percentages alone cannot conserve atmosphere during transfer.

## 4. Pressure Derivation

Cell total pressure is derived from:

- total gas amount;
- pressure-cell volume;
- current atmosphere temperature supplied by Thermal.

A simplified ideal-gas-equivalent relationship is acceptable and becomes the canonical TA-6 model.

Numeric constants/units are fixed in implementation contracts later; the conservation relation is architectural.

## 5. Composition

For a non-vacuum cell:

```text
GasFraction(species) = SpeciesAmount / TotalGasAmount
```

Breathability is not decided by percentage alone; GDS thresholds consume total pressure, oxygen availability, CO2, contaminant, and temperature facts.

## 6. Pressure Portals

Gas transfer occurs through typed `PressurePortalRuntime` records.

Portal kinds include:

- door/hatch opening;
- airlock door;
- breach/hull opening;
- authored service penetration;
- ventilation connection;
- deliberate vent path.

A portal contains:

- endpoint cell IDs or exterior/vacuum endpoint;
- opening/conductance class;
- current open fraction/state;
- damage state;
- one-way/controlled behavior where authored;
- stable ID/order.

## 7. Exterior / Vacuum

Vacuum is represented as an external sink boundary, not a giant gas cell with negative/finite capacity.

Gas transferred through an open exterior portal is removed from station atmosphere ownership.

The loss is recorded once and cannot return unless another gameplay source explicitly supplies gas.

## 8. Deterministic Flow Model

For every atmosphere step, each open portal computes candidate transfer from pressure differential and authored conductance/maximum-throughput parameters.

The architecture requires:

- transfer direction from higher to lower pressure;
- bounded quantity per step;
- proportional species transfer from source composition;
- exact source decrement and destination increment;
- stable portal iteration/order or simultaneous-delta accumulation;
- no negative gas inventory.

A simple deterministic orifice-flow approximation is sufficient; full CFD is not required.

## 9. Simultaneous Delta Application

To reduce iteration-order artifacts, normal atmosphere flow uses a two-phase step:

1. compute portal transfer deltas from the same pre-step atmosphere snapshot;
2. clamp against source availability and portal capacity deterministically;
3. accumulate per-cell species deltas;
4. commit all cell deltas together.

High-flow emergency cases can use bounded substeps but still follow the same conservation rule.

## 10. Compartment Derivation

A gameplay/UI `CompartmentView` is derived from pressure cells and pressure-rated boundary state.

It can group cells that are effectively one controlled interior region, but it is not gas ownership authority.

Opening/closing a door changes portal connectivity and the derived compartment view without duplicating/reassigning gas quantities.

## 11. Split / Merge Safety

Because gas remains per stable pressure cell:

- closing a new boundary does not duplicate atmosphere;
- opening a boundary does not select one side's atmosphere;
- construction splitting a region assigns authored physical pressure-cell volumes and existing gas through explicit topology transition rules;
- deconstruction/portal changes conserve total species amounts except explicit vent/loss.

## 12. Topology Change Reconciliation

When pressure topology changes, Atmosphere receives a `PressureTopologyDelta` after the TA-2 gameplay commit.

Before the next atmosphere step it validates:

- cell creation/removal;
- volume changes;
- portal additions/removals;
- breach state;
- current gas ownership.

Any construction operation that would remove a gas-owning cell must first define where its gas physically goes: connected retained cells, storage/recovery process, or vent/loss.

## 13. Breaches

A breach is an exterior portal with authored severity/conductance.

Breach creation does not immediately set the cell to vacuum.

Pressure falls over Simulation Time according to actual cell gas, volume, opening, and current flow.

## 14. Decompression Force Facts

Atmosphere can produce simplified `DecompressionFlowFact` data:

- portal position/direction;
- pressure differential;
- severity/flow magnitude;
- affected local region.

TA-5/actor systems can turn this into bounded forces for eligible loose objects/characters.

Atmosphere does not directly teleport/eject actors.

## 15. Ventilation Network

Ventilation is represented as controlled gas-transfer links distinct from open structural portals.

A ventilation edge can require:

- powered fan/pump;
- open damper/valve;
- intact duct path;
- configured flow target.

It has finite throughput.

A closed physical door can therefore coexist with active ventilation exchange.

## 16. Life-Support Devices

Life-support equipment acts through explicit atmosphere operations:

- oxygen generator adds Oxygen from valid input/resource process;
- scrubber removes CO2/contaminant at bounded throughput;
- gas storage transfers gas into/out of cells;
- pressure pump moves bounded gas between storage/cells;
- climate control couples to Thermal rather than deleting heat.

Every operation consumes its actual required Power/Water/resource dependencies.

## 17. No Free Oxygen

Oxygen generation must reference a valid producing device/work process and input source where defined.

Atmosphere never creates Oxygen simply to maintain a target percentage.

## 18. Occupant Metabolism

Crew/other authored occupants can publish aggregate atmosphere demand/production facts per occupied cell/compartment:

- oxygen consumption;
- CO2 production;
- optional contaminant/heat contribution.

This is processed over Simulation Time without individual molecule simulation.

## 19. Fire Coupling

Fire consumes Oxygen and produces heat/contaminant through explicit event/device facts.

Atmosphere reports whether local conditions support combustion.

Fire ownership remains in damage/event systems; Atmosphere owns gas consequences.

## 20. Airlock State Machine Integration

Atmosphere provides physical pressure facts to the GDS airlock sequence:

```text
Opposite door sealed
→ chamber isolated
→ pump/vent gas
→ pressure differential reaches safe threshold
→ destination unlock eligible
```

Interlock/gameplay authority is station equipment/interaction state. Atmosphere provides pressure and transfer results.

## 21. Dangerous Door Override

A forced opening across unsafe differential simply creates the real portal state.

Gas flow and decompression follow the same solver.

No separate scripted “decompression cutscene” overrides conservation.

## 22. Automatic Isolation

Automation can command eligible pressure doors/valves closed.

The command still requires actual:

- device operability;
- power/local backup where needed;
- unobstructed closure;
- policy/authority.

Atmosphere never silently closes topology on its own.

## 23. Sensor Boundary

Physical atmosphere and known atmosphere are separate.

Failed sensors can reduce read-model precision while physical pressure/composition continues unchanged.

Automation requiring unavailable sensor data follows its configured degraded/safe behavior rather than receiving omniscient values.

## 24. Habitability Read Model

Atmosphere publishes immutable per-cell/derived-compartment facts:

- pressure;
- oxygen partial availability;
- CO2;
- contaminant;
- leak state;
- ventilation connectivity;
- sensor confidence;
- derived habitability band.

Thermal supplies temperature to final habitability classification.

## 25. Atmosphere Step Scheduling

Atmosphere does not require 60 Hz full-station solving under normal stable conditions.

It uses Simulation-Time scheduled stepping with adaptive bounded substeps for rapid decompression/airlock equalization.

TA-13 later sets numeric cadence/substep budgets.

Visible fast events remain sufficiently sampled to preserve equivalent physical outcomes and presentation.

## 26. Off-Screen Equivalence

Off-screen Horizon uses the same gas inventories, portals, processing capacities, and thresholds.

Larger time slices are allowed only if the integrator splits at significant boundaries such as:

- tank depletion;
- safe/unsafe threshold crossing;
- crop/crew exposure threshold;
- pressure-equalization target;
- emergency-isolation event;
- device shutdown.

It cannot jump across a threshold and miss the event.

## 27. Persistence

Persisted atmosphere state includes:

- per-cell species quantities;
- gas-storage inventories;
- portal/valve persistent states;
- active leaks/breaches;
- life-support configuration;
- relevant faults/sensor state.

Derived pressure/fractions/compartment components are recomputed during staging load.

## 28. Save / Load Invariant

Loading preserves gas quantity and temperature coupling; it never normalizes a room to authored default atmosphere merely because scene geometry was reconstructed.

## 29. Debugging

Development diagnostics expose:

- pressure cells and volumes;
- pressure portals;
- species amounts/fractions;
- total pressure;
- portal candidate/committed flow;
- breaches;
- ventilation paths;
- gas storage;
- derived compartment grouping;
- sensor confidence;
- conservation totals.

A conservation report can compare station-wide gas before/after a step plus explicit external/source/sink transactions.

## 30. Explicit Non-Goals

The atmosphere architecture does not implement:

- Navier-Stokes CFD;
- particle/molecule simulation;
- dozens of routine gas species;
- visual-room-label atmosphere ownership;
- instant atmosphere reset on scene load;
- magical pressure barriers.

## 31. Dependencies

Depends on TA-6 topology foundation, Power, Thermal/Water, TA-5 force/contact handoff, TA-2 persistence/transactions, GDS Atmosphere/Oxygen, Structural Rules, Damage/Repair, Automation, Player/Crew survival.

## 32. Open Questions

None within TA-6 atmosphere/pressure architecture.
