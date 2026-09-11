# Power Network Solver and Allocation

> **Status:** Architecture Complete  
> **Authority:** Station electrical-network runtime representation, producer/storage/load allocation, throughput-constrained routing, breaker/isolation state, deterministic load shedding, and power-derived operating facts

## 1. Purpose

This specification realizes GDS Station Power as a deterministic capacity-constrained graph simulation without attempting full electrical-engineering waveform simulation.

The architecture must preserve:

- physical network connectivity;
- transfer bottlenecks;
- producer/storage limits;
- load priorities;
- manual lockouts;
- deterministic allocation;
- local islands and black-start recovery;
- power/thermal/resource coupling.

## 2. Power Domain Store

Persistent electrical state is owned by `StationPowerStore`.

It contains project-owned records for:

```text
PowerNodeId
PowerEdgeId
PowerProducerState
PowerStorageState
PowerConsumerState
PowerBreakerState
PowerPolicyState
PowerFaultState
```

The store references `StationModuleId` / authored equipment identity but does not duplicate structural ownership.

## 3. Derived Power Graph

The runtime Power Graph is rebuilt from:

- power-capable utility links exposed by TA-6 topology;
- currently completed/commissioned devices;
- breaker/relay state;
- damaged/broken links;
- structurally available endpoints;
- docking shore-power links where active.

Node/edge order is canonical by typed ID.

## 4. Power Node Kinds

Logical runtime nodes include:

- producers;
- batteries/storage;
- buses/distribution points;
- relays/breakers;
- consumers;
- shore-power endpoints;
- optional explicit dump loads.

A physical module can contribute several logical electrical nodes.

## 5. Power Edge

A `PowerEdgeRuntime` contains at minimum:

- endpoint node IDs;
- current enabled/disabled state;
- transfer capacity in MW-equivalent units;
- damage/derating factor;
- directionality if authored;
- protection state;
- stable routing/tie-break key.

Edges do not model AC phase, reactive power, or voltage waveform.

## 6. Consumer Contract

A consumer publishes a `PowerDemandRequest` with:

- requested/nominal demand;
- minimum operating demand;
- standby demand;
- optional peak/startup demand;
- `PowerLoadPriority`;
- same-priority player order key;
- partial-operation support;
- manual disable/lock state;
- operating-state constraints.

Power decides deliverable allocation. The owning device decides the exact capability produced at that allocation.

## 7. Producer Contract

A producer publishes:

- current physically available output;
- minimum stable output where applicable;
- maximum/ramp-limited output;
- dispatch category/order;
- resource/fuel eligibility;
- thermal eligibility;
- startup/shutdown state;
- fault state.

Power never assumes nameplate maximum if the producer is thermally/fuel/damage constrained.

## 8. Storage Contract

Electrical storage publishes:

- stored MWh-equivalent energy;
- capacity;
- max charge/discharge MW;
- efficiency parameters;
- reserve floor;
- emergency-reserve permission;
- fault/derating state.

Stored energy is persistent conserved state.

## 9. Solver Epoch

A `PowerSolveEpoch` runs when any relevant input changes or on scheduled simulation update when storage/production energy must integrate.

Inputs include:

- topology revision;
- producer availability;
- consumer demand/state;
- breaker state;
- storage state/policy;
- player priority/order policy.

Unchanged graphs can reuse compiled adjacency/workspace.

## 10. Network Partitioning

The solver first partitions the enabled graph into connected allocatable islands.

Disconnected islands are solved independently.

Surplus in one island cannot supply another.

## 11. Deterministic Priority Allocation

Within each electrical island, demand is processed lexicographically:

1. `PowerLoadPriority::P0`;
2. P1;
3. P2;
4. P3;
5. P4.

Within one priority, explicit player-configured order is respected. Remaining ties use stable consumer ID.

## 12. Capacity-Constrained Routing

For each demand class, allocation is solved over residual edge capacities.

The required technical behavior is equivalent to deterministic max-flow routing with stable path/tie-break ordering.

A consumer can receive from several valid producers/routes.

A local bottleneck can therefore shed a load even when total generation elsewhere exceeds global demand.

## 13. Producer Dispatch Ordering

Eligible supply sources are exposed to the solver in deterministic configured dispatch order.

Conceptual default categories:

1. mandatory emergency source where policy requires;
2. preferred/low-cost/non-discretionary production;
3. normal primary generation;
4. reserve/expensive generation;
5. battery discharge subject to reserve policy.

The exact economic order is player/content policy, not hard-coded physics.

## 14. Load Satisfaction Semantics

All-or-nothing loads receive their required operating demand or are shed to standby/unpowered according to their definition.

Partial-capable loads can receive allocation between minimum and nominal and publish a normalized delivered fraction.

Allocation below minimum does not imply full capability.

## 15. Startup Loads

Startup is modeled as a distinct demand reservation/phase.

A device does not transition to operating state unless its startup requirement can be served while respecting higher-priority current loads and transfer capacities.

No transient startup power is fabricated outside the graph.

## 16. Storage Discharge

After active producer supply is evaluated, storage can supply deficits subject to:

- discharge rate;
- residual edge capacity;
- stored energy for the current Simulation Time interval;
- reserve floor;
- emergency permission.

Storage energy decrement is committed exactly once from delivered discharge and efficiency rules.

## 17. Storage Charging

Charging occurs only after reachable active demand for the configured solve stage is satisfied.

Charging is limited by:

- producer surplus;
- residual network capacity;
- charge rate;
- remaining storage capacity;
- efficiency.

Energy is conserved through explicit losses.

## 18. Non-Dispatchable Surplus

Residual surplus after loads/storage is handled through authored source behavior:

- dump load;
- curtailment;
- source-specific consequence.

Power never converts surplus into invisible stored energy.

## 19. Breakers / Relays

A breaker changes graph connectivity at commit.

State includes:

- Open;
- Closed;
- Tripped;
- Faulted/Jammed where authored.

Manual Open is authoritative until policy explicitly permits an emergency override.

## 20. Protection

Before an ordinary capacity overload produces physical damage, station protection can:

- shed lower-priority demand;
- reroute;
- use permitted storage;
- trip/isolate the threatened path.

Actual overload damage requires a specific authored protection failure/event and is resolved by Damage, not inferred directly by the solver.

## 21. Power Allocation Result

A solve publishes immutable `PowerAllocationSnapshot` data including:

- island membership;
- producer dispatched output;
- storage charge/discharge;
- per-consumer delivered power;
- per-edge flow/capacity utilization;
- shed loads;
- bottleneck cause;
- blackout/island status.

Consumers consume this read model in deterministic station-system update order.

## 22. Operating-State Feedback

Device domains translate allocations into states such as:

```text
Unpowered
Standby
Degraded
Operational
Boosted
```

Power does not independently decide manufacturing speed, oxygen production, radiator behavior, etc.; it provides the electrical allocation fact.

## 23. Blackout

A graph island with no usable supply for enabled loads enters `PowerIslandState::Blackout`.

Configuration, breaker state, and stored device state persist.

A blackout is not a station-state reset.

## 24. Black Start

Protected Command Core/emergency circuitry is represented as explicit authored supply/load capability.

The black-start path is therefore normal graph behavior with protected device definitions, not an out-of-band cheat.

The recovery path must remain valid under GDS invariants.

## 25. Isolated Powered Branch

A structurally disconnected branch can form an independent Power island if it retains local generation/storage and intact internal links.

It cannot route across the severed structural/utility connection.

## 26. Thermal Coupling

Power producers/consumers emit thermal-load facts from actual operating allocation.

Thermal state can constrain future producer availability/consumer capability.

To avoid algebraic recursion inside one solver call, coupling uses deterministic station simulation phases with previous/committed current-step device limits, then next-phase thermal consequence and subsequent re-evaluation as defined by TA-6 station scheduling.

## 27. Resource/Fuel Coupling

Fuel availability is supplied by authoritative logistics/resource buffers.

A generator cannot count fuel that is merely present elsewhere but not owned/delivered to its valid local buffer/path.

Consumption occurs through explicit device work/resource transactions, not hidden inside graph traversal.

## 28. Control/Data Loss

Central advanced allocation can degrade when control/data capability is unavailable.

Local breaker/protection/device safety remains represented by device-local policy where authored.

Loss of central control does not delete physical generation or make every closed conductor disappear.

## 29. Update Frequency

Topology changes force immediate re-solve before downstream station systems rely on stale electrical connectivity.

Routine energy integration/allocation can run at a scheduled station-system cadence derived from Simulation Time rather than necessarily every 60 Hz physics tick.

TA-13 later sets numeric cadence/performance budgets.

## 30. Off-Screen Equivalence

The same `StationPowerStore`, graph rules, priorities, transfer capacities, and storage conservation apply when Horizon is off-screen.

Off-screen simulation may advance by larger deterministic time slices only when the solver preserves threshold/event ordering and equivalent outcomes.

## 31. Persistence

Persisted state includes:

- producer operating configuration/state;
- storage energy/reserve policy;
- breaker/relay states;
- manual enable/disable/priority policy;
- faults/damage references;
- topology-driving persistent state.

Derived flow assignments and adjacency workspace are reconstructed.

## 32. Debugging

Development tools expose:

- Power islands;
- producer availability/dispatch;
- storage reserves;
- consumer demand/allocation;
- edge capacity/use;
- residual routing;
- shed reason;
- breaker state;
- deterministic order/tie-break.

## 33. Explicit Non-Goals

The solver does not simulate:

- AC waveform;
- reactive power;
- voltage phase/frequency;
- electromagnetic transients;
- hidden global power pooling;
- infinite conductor capacity.

## 34. Dependencies

Depends on TA-2 transactions/state, TA-6 topology foundation, GDS Station Power, Thermal, Logistics/Resources, Damage/Repair, and Automation.

## 35. Open Questions

None within TA-6 power-network architecture.
