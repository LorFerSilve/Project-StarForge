# Station Water System

> **Status:** Design Complete  
> **Authority:** Water storage, distribution, consumption, recycling, leakage, and station-level water security

## 1. Purpose

Water is a persistent station resource supporting:

- crew habitation;
- farming;
- selected life-support processes;
- sanitation abstractions;
- selected industrial processes;
- thermal-system make-up where defined.

Water matters strategically without creating a constant personal thirst-management minigame.

## 2. Water Categories

The baseline distinguishes:

- **Fresh Water** — usable for crew, farming, and processes requiring clean water;
- **Wastewater** — recoverable contaminated water awaiting recycling.

Special industrial fluids are separate resources and are not automatically treated as water.

## 3. Units

Water quantity is displayed in a consistent station-scale volume unit.

The exact display unit and numerical capacities are tuneable presentation/balance choices.

## 4. Water Network

Water infrastructure forms connected distribution networks.

A network can contain:

- fresh-water tanks;
- wastewater tanks;
- pumps;
- pipes/service connections;
- recyclers;
- producers/extractors;
- consumers;
- controllable valves.

Disconnected water networks do not share inventory.

## 5. Standard Water Connections

Modules designed for habitation, farming, or compatible industry expose authored water utility connections.

Structural adjacency alone does not guarantee water connectivity.

## 6. Fresh-Water Storage

Fresh-water tanks have finite capacity.

A tank tracks:

- current quantity;
- maximum quantity;
- connection state;
- damage/leak state;
- reserve policy.

## 7. Wastewater Storage

Wastewater remains a recoverable resource until:

- recycled;
- deliberately dumped;
- lost through damage.

Insufficient wastewater capacity can reduce recycling efficiency or force process shutdown according to system policy.

## 8. Crew Consumption

Crew population creates station-level fresh-water demand.

The simulation aggregates routine personal consumption rather than requiring the player to manually give individual crew members drinks.

Exact per-crew demand is tuneable.

## 9. Farming Consumption

Farming consumes water according to crop/system design.

Advanced hydroponics can recover a large fraction of water but never creates water from nothing.

Detailed farming behavior belongs to `farming_and_food.md`.

## 10. Life-Support Consumption

Some oxygen-generation or atmosphere-processing technologies may consume water.

The atmosphere specification owns the functional consequence; this specification owns the water transfer.

## 11. Industrial Consumption

Selected recipes may consume water.

Recipe ownership remains under `../resources/`.

The water network only determines whether the required quantity can be delivered.

## 12. Recycling

Water recyclers convert wastewater into fresh water.

A recycler requires:

- power;
- connected input;
- output capacity;
- functioning machinery.

Recycling has a defined recovery efficiency below or equal to 100%.

Any unrecovered fraction represents system loss or waste.

## 13. No Infinite Closed Loop

A developed station can become highly water-efficient but does not gain unlimited water from a perfect zero-loss cycle unless a future explicit technology defines an external water source.

Long-term population growth can therefore still require reserve expansion or resource acquisition.

## 14. External Water Acquisition

Water can enter station inventory through gameplay sources such as:

- mined ice;
- planetary resources;
- purchased supplies;
- recovered cargo;
- specialized extraction technology.

Exact acquisition sources belong to resources/world specifications.

## 15. Pumps

Water transfer requires pump capability where the network definition requires active flow.

Pumps consume electrical power.

A powerless network may retain stored water locally but cannot assume normal distribution.

## 16. Distribution Throughput

Water connections and pumps have finite throughput.

A station can therefore possess enough total water while a high-demand local branch is under-supplied.

The UI must distinguish storage shortage from distribution shortage.

## 17. Consumer Priority

Water consumers can be assigned allocation priority.

Default ordering favors:

1. critical life-support processes;
2. crew habitability;
3. essential farming;
4. important industry;
5. discretionary processes.

The player may configure eligible consumers.

## 18. Reserve Policy

Fresh-water storage can reserve a protected quantity.

Normal lower-priority consumption cannot use protected reserve unless policy permits.

This supports emergency planning.

## 19. Valves and Isolation

Valves can isolate branches.

Isolation is useful for:

- containing leaks;
- maintenance;
- protecting reserves;
- shutting down nonessential demand.

## 20. Leaks

Damage can create water leakage.

A leak:

- removes stored/distributed water over time;
- may reduce local pressure/throughput;
- can be isolated if topology permits;
- persists until repaired or its source is depleted.

Leaked water is not automatically returned to storage.

## 21. Water and Vacuum

Water released into vacuum is treated as lost for normal station inventory.

The game does not require physically simulating every phase transition of leaked water.

## 22. Automation

Qualified automation may:

- detect abnormal consumption;
- isolate leaks;
- protect reserve thresholds;
- adjust recycler operation;
- prioritize critical consumers;
- report projected shortage.

Automation follows player policy and does not override deliberate lockouts without explicit authorization.

## 23. Player Controls

The player can:

- inspect storage;
- inspect production/recycling;
- inspect demand;
- inspect throughput;
- set reserve policy;
- configure eligible priority;
- open/close valves;
- isolate leaks;
- authorize dumping where allowed.

## 24. Water Overview UI

The diagnostic interface communicates:

- fresh-water quantity;
- wastewater quantity;
- production/recovery rate;
- consumption rate;
- projected endurance at current net loss;
- isolated branches;
- leaks;
- bottlenecks;
- reserve state.

## 25. Crew Shortage Response

If available water cannot support the current crew population:

- water allocation follows priority;
- the station generates escalating warnings;
- crew habitability deteriorates according to crew rules;
- automation cannot fabricate missing water.

Routine shortage does not immediately kill crew permanently.

## 26. Farming Shortage Response

Farms receiving insufficient water reduce or stop production according to farming rules.

They do not continue producing normal food output from missing inputs.

## 27. Persistence

Save state preserves:

- fresh-water quantity;
- wastewater quantity;
- network topology;
- valve states;
- reserve settings;
- active leaks;
- recycler state.

## 28. Edge Case — Full Fresh-Water Storage

A recycler with nowhere to place recovered fresh water stops or reduces output rather than deleting or duplicating water.

Wastewater remains until storage/processing permits further recycling.

## 29. Edge Case — Full Wastewater Storage

Processes generating wastewater respond according to their owning specification.

The game does not silently destroy wastewater to keep all consumers operating normally.

## 30. Edge Case — Disconnected Reserve Tank

A disconnected tank retains its local inventory.

Its water is unavailable to other networks until a valid distribution path is restored.

## 31. Edge Case — Enough Water, Insufficient Pumping

If stored quantity is sufficient but throughput cannot meet demand, local consumers can still be under-supplied.

The UI reports a distribution bottleneck.

## 32. Recovery

Water shortages can be recovered by:

- importing water;
- mining ice;
- repairing leaks;
- restoring power/pumps;
- improving recycling;
- reducing demand;
- increasing storage;
- reconnecting networks.

## 33. Progression

Early game:

- small tanks;
- weak recycling;
- manual reserve awareness.

Mid game:

- larger storage;
- strong recycling;
- automated leak isolation;
- dedicated farming supply.

Late game:

- highly efficient closed-loop recovery;
- redundant storage;
- distributed networks;
- large emergency reserve;
- specialized external acquisition.

## 34. Explicit Non-Goals

The water system does not require:

- personal thirst bars during normal gameplay;
- individual drinking animations for resource accounting;
- full real-world plumbing hydraulics;
- perfect lossless recycling by default;
- water teleportation between disconnected networks.

## 35. Tuneable Parameters

Tuneable values include:

- crew demand;
- farming demand;
- storage capacity;
- recycler efficiency;
- pump throughput;
- leak rate;
- reserve quantity;
- industrial demand.

## 36. Dependencies

This specification depends on:

- [Power](power.md);
- [Atmosphere and Oxygen](atmosphere_and_oxygen.md);
- [Thermal Systems](thermal_systems.md);
- `farming_and_food.md`;
- `storage_and_logistics.md`;
- `damage_and_repairs.md`;
- `automation.md`;
- `../crew/`;
- `../resources/`.

## 37. Open Questions

None in the current water baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
