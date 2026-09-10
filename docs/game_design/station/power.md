# Station Power System

> **Status:** Design Complete  
> **Authority:** Electrical generation, storage, distribution, allocation, protection, and failure behavior on the home station

## 1. Purpose

The power system makes station electricity a physical and strategic infrastructure network.

Power determines whether many station functions can operate.

The design must support:

- generation choices;
- distribution bottlenecks;
- redundancy;
- storage;
- priority-based load shedding;
- sabotage;
- repair;
- automation;
- readable failure states.

The system does not simulate real-world alternating-current engineering in full detail.

## 2. Core Model

Electrical infrastructure is represented as one or more connected **Power Networks**.

A Power Network contains:

- producers;
- storage;
- distribution connections;
- relays/breakers;
- consumers.

Power does not teleport between disconnected networks.

## 3. Units

The player-facing baseline uses:

- **MW** for instantaneous electrical power;
- **MWh** for stored electrical energy.

Exact numerical values are tuneable balancing parameters.

The game does not claim that every displayed value is a physically exact real-world engineering model.

## 4. Power Network Topology

Power topology is a graph.

Conceptually:

- producers, batteries, relays, buses, and consumers are nodes;
- power-capable utility connections are edges;
- edges may have throughput limits;
- opening or destroying a required connection changes network topology.

Two station modules that are structurally connected are not automatically electrically connected unless their connection supports power transfer.

## 5. Standard Power Backbone

Normal compatible station modules include a standard internal power backbone through authored utility connections.

This prevents the player from manually wiring every light or door.

The standard backbone has finite throughput.

High-demand equipment can require:

- a Heavy Power connection;
- a dedicated distribution module;
- multiple compatible feeds.

## 6. Power Producers

A producer exposes:

- maximum available output;
- current available output;
- current commanded output;
- fuel requirements where applicable;
- thermal requirements;
- startup state;
- failure state.

Examples include:

- emergency generator;
- fission reactor;
- fusion reactor;
- advanced energy reactor;
- solar generation where appropriate.

The exact technology catalog belongs to progression/resource specifications.

## 7. Available Output

A producer's nameplate maximum is not automatically available.

Available output may be reduced by:

- missing fuel;
- insufficient cooling;
- damage;
- maintenance faults;
- deliberate player limits;
- startup/shutdown state.

The power allocator can use only current available output.

## 8. Generator Dispatch

Dispatchable generators automatically adjust output toward current demand, subject to their operating rules.

Where several generators can satisfy demand, the station controller follows player-configurable generation policy.

The baseline policy can prioritize:

1. mandatory emergency generation;
2. low-cost or renewable sources;
3. normal primary reactors;
4. expensive or limited reserve generation.

Exact economic preferences are configurable rather than hard-coded as universal truth.

## 9. Non-Dispatchable Production

A producer that cannot fully throttle may create surplus power.

Surplus is handled in this order:

1. satisfy active consumer demand;
2. charge eligible storage;
3. supply explicitly configured dump loads if present;
4. curtail the source if technologically supported;
5. apply the source-specific surplus consequence.

The system never creates stored energy from surplus without storage capacity.

## 10. Power Consumers

Every powered device exposes an electrical demand model.

A consumer defines as applicable:

- disabled demand;
- standby demand;
- minimum operating demand;
- nominal demand;
- peak/transient demand;
- current requested demand;
- `PowerLoadPriority`;
- whether partial-power operation is supported.

## 11. Consumer Operating Modes

A consumer can be:

- **Disabled** — deliberately off;
- **Unpowered** — enabled but allocated insufficient power;
- **Standby** — receiving standby requirement;
- **Degraded** — operating between minimum and nominal where supported;
- **Operational** — nominal requirements satisfied;
- **Boosted** — temporarily consuming above nominal where explicitly supported;
- **Faulted** — unable to operate due to a non-power fault.

Not every consumer supports every state.

## 12. Minimum Power

A variable-load consumer may define a minimum operating threshold.

If allocation falls below that threshold:

- it becomes Unpowered or enters a subsystem-defined safe shutdown;
- it does not silently continue at full capability.

All-or-nothing devices require their defined operating draw.

## 13. Power Priorities

Enabled consumers belong to one load-shedding priority.

Baseline priorities use the dedicated `PowerLoadPriority` type. This namespace is not interchangeable with `AlarmPriority`, automation severity, mission threat, or any other P-scale.

### `PowerLoadPriority::P0` — Emergency

Reserved for protected emergency functions.

Not normally user-reassignable.

### `PowerLoadPriority::P1` — Critical

Systems whose continued operation protects station survival or immediate safety.

### `PowerLoadPriority::P2` — Essential

Important operational and defensive infrastructure.

### `PowerLoadPriority::P3` — Normal Operations

Industry, research, routine logistics, and similar normal work.

### `PowerLoadPriority::P4` — Discretionary

Comfort, decorative, or explicitly low-priority loads.

The default priority of each device is authored.

The player may change configurable priorities.

## 14. Manual Disable vs Priority

A deliberate player shutdown overrides automatic priority allocation.

Automation must not silently re-enable a manually locked-off consumer unless an explicit emergency policy authorizes that exact behavior.

## 15. Deterministic Load Shedding

If available supply cannot satisfy all requested demand, the power controller allocates power by priority.

Higher-priority loads are satisfied before lower-priority loads.

Within the same priority, the player can configure load order.

If no custom order exists, a stable deterministic device order is used.

The outcome must not depend on nondeterministic iteration order.

## 16. Partial-Power Allocation

A device that explicitly supports degraded operation may receive power between minimum and nominal demand.

Its performance scales according to that device's specification.

A device that does not support partial operation is either sufficiently powered or shed.

## 17. Power Storage

Electrical storage devices expose:

- maximum stored energy;
- current stored energy;
- maximum charge rate;
- maximum discharge rate;
- efficiency where applicable;
- reserve threshold;
- damage/fault state.

Examples include batteries, capacitor banks, or advanced storage.

## 18. Storage Charging

When supply exceeds current active demand, eligible storage charges up to:

- available surplus;
- its charge-rate limit;
- its remaining capacity;
- network transfer capacity.

Energy is not duplicated when crossing storage efficiency losses.

## 19. Storage Discharge

When generation is insufficient, eligible storage discharges automatically according to configured policy.

Discharge is limited by:

- stored energy;
- maximum discharge rate;
- network transfer capacity;
- reserve policy.

## 20. Reserve Threshold

A storage device may reserve a percentage or quantity of charge.

Normal automatic operation does not discharge below this threshold.

The player can configure whether emergency priorities may consume the reserve.

This permits designs such as keeping emergency batteries for life support rather than allowing industry to drain them.

## 21. Distribution Capacity

Power connections have finite throughput.

A network can therefore have enough total generation while a local section still lacks deliverable power because of a bottleneck.

Example:

```text
Reactor: 100 MW
      │
Relay capacity: 30 MW
      │
Industrial wing demand: 50 MW
```

The industrial wing cannot receive more than 30 MW through that sole path.

## 22. Power Routing

The simulation distributes available power across valid paths while respecting connection capacities.

The required gameplay behavior is equivalent to capacity-constrained flow.

The design does not require simulation of:

- voltage phase;
- reactive power;
- electromagnetic transients;
- AC frequency synchronization.

## 23. Multiple Supply Paths

A consumer may receive power through multiple valid routes where the topology permits.

Redundant routes can prevent a single destroyed relay from disabling the consumer.

Network allocation recalculates when topology changes.

## 24. Breakers and Relays

A controllable relay/breaker can:

- connect a power path;
- isolate a power path;
- trip due to defined protection behavior;
- be manually opened;
- be manually closed when safe;
- fail due to damage.

Opening a breaker changes the network graph immediately.

## 25. Protection Behavior

The station power system protects itself before ordinary overload causes arbitrary cascading destruction.

If a connection would exceed its allowed throughput, the controller attempts:

1. load shedding;
2. alternate routing;
3. storage support;
4. protective trip/isolation where required.

Physical overload damage occurs only when a subsystem explicitly creates a protection failure or exceptional event.

## 26. Startup Demand

Selected high-power devices may define a startup requirement.

A device cannot transition from Off to Operational if the network cannot supply its required startup condition.

Startup demand is visible to the player before activation where practical.

## 27. Reactor Startup

Major reactors do not become fully available instantly.

A reactor can define states such as:

- Offline;
- Starting;
- Online;
- Throttled;
- Emergency Shutdown;
- Faulted.

Detailed reactor behavior can be specified in a future power-generation catalog without changing network rules.

## 28. Fuel Dependency

Generators that consume fuel require valid fuel availability.

Fuel consumption belongs to resource/logistics rules.

If fuel delivery stops:

- current local reserve may continue where defined;
- available output eventually drops;
- the generator transitions according to its shutdown behavior.

## 29. Thermal Dependency

Power generation and many consumers produce waste heat.

Electrical operation reports thermal load to the thermal system.

Insufficient cooling can reduce available output or trigger shutdown according to the device definition.

Power allocation does not ignore thermal constraints.

## 30. Control/Data Dependency

Advanced automatic power allocation requires functioning control capability.

If central automation/control is unavailable, local protective behavior still exists.

The player can interact with reachable local controls where supported.

Loss of advanced control does not cause every electrical device to instantly fail.

## 31. Emergency Power

The Command Core provides protected minimal emergency capability as defined by the station overview.

Emergency power exists to maintain recoverability.

It supports only explicitly defined emergency loads.

It cannot sustain normal production, research, large defenses, or a developed station indefinitely.

## 32. Blackout

A Power Network is in **Blackout** when it has no usable generation/storage supply for enabled loads.

Consequences are subsystem-specific.

Examples:

- normal lights fail;
- machinery stops;
- powered doors may enter emergency behavior;
- life support stops actively processing air;
- defenses stop unless locally backed up.

A blackout does not erase configuration.

## 33. Black Start

The station must support a defined recovery path from total blackout.

At minimum, protected emergency capability allows the player to:

- access critical controls;
- inspect power state;
- initiate repair;
- start an eligible generator.

A primary generator may require external/emergency startup power.

This requirement must be visible in its device specification.

## 34. Local Backup Power

Selected critical devices can contain local backup storage if their specification provides it.

Examples can include:

- emergency doors;
- alarm systems;
- medical equipment;
- security controls.

Local backup has finite energy and cannot be assumed for all devices.

## 35. Damage

Power infrastructure can be damaged.

Damage may:

- reduce throughput;
- reduce generation;
- reduce storage capacity;
- create a fault;
- break a network edge;
- force a breaker open;
- disable a device.

Damage effects are persistent until repaired or otherwise resolved.

## 36. Fault Isolation

Automated or crew-assisted fault handling can isolate a damaged section to preserve the rest of the network.

Isolation must follow configured automation policy.

A faulted branch does not grant power simply because another network elsewhere has surplus.

## 37. Engineer Automation

Qualified crew and automation may:

- detect faults;
- identify likely causes;
- open breakers;
- reroute eligible networks;
- adjust generator dispatch;
- dispatch repair systems;
- report unresolved failures.

They may not violate manual lockouts or consume protected strategic resources without authorization.

Detailed crew capability belongs to `../crew/`.

## 38. Player Controls

The player can, subject to access and technology:

- inspect network topology;
- inspect production and demand;
- inspect bottlenecks;
- enable/disable devices;
- change configurable load priority;
- configure battery reserve;
- configure generator policy;
- open/close controllable breakers;
- inspect faults;
- authorize repairs.

## 39. Power Overlay

The station interface provides a diagnostic overlay that communicates:

- connected networks;
- powered/unpowered modules;
- producer output;
- consumer demand;
- storage state;
- bottlenecks;
- broken links;
- open breakers;
- priority shedding.

Color is not the only state indicator.

## 40. Local Feedback

Powered equipment provides local physical feedback where appropriate.

Examples:

- lights;
- machine animation;
- displays;
- reactor glow;
- fan/pump activity.

Loss of power also changes local presentation.

## 41. Emergency Lighting

Critical inhabited areas can use emergency lighting if an eligible emergency source exists.

Emergency lighting is intentionally limited and visually distinct from normal illumination.

It does not imply that the entire compartment's normal systems remain powered.

## 42. Audio Feedback

Relevant power states use audio such as:

- generator hum;
- relay switching;
- startup sequence;
- shutdown;
- overload warning;
- breaker trip;
- blackout transition.

Critical information must also have visual/UI communication.

## 43. Persistence

The save state preserves:

- topology;
- breaker states;
- manual enable/disable states;
- priorities;
- generator configuration;
- storage charge;
- faults;
- damage;
- automation policy.

Loading must deterministically reconstruct power networks.

## 44. Edge Case — Isolated Powered Island

A structurally disconnected station branch may remain electrically powered if it contains its own generation/storage and internally valid power network.

It remains structurally disconnected and cannot exchange electricity across the severed physical connection.

## 45. Edge Case — Enough Generation, Not Enough Transfer

If generation exceeds total demand but an edge capacity blocks delivery, affected consumers can still be shed.

The UI identifies distribution rather than generation as the shortage cause.

## 46. Edge Case — Battery Charging During Local Shortage

Storage does not charge from surplus in one disconnected or capacity-isolated region while higher-priority reachable consumers in the same allocatable network are being shed.

Reachable demand is resolved before non-essential charging.

## 47. Edge Case — Player Changes Priority During Shortage

Priority changes trigger power reallocation immediately after confirmation.

This can intentionally turn one system off to restore another.

## 48. Edge Case — Consumer Fails While Allocated Power

If a powered consumer becomes Faulted, its unused demand is released and the network reallocates available power.

Faulted equipment does not continue consuming full nominal power unless its fault definition explicitly creates that behavior.

## 49. Edge Case — Producer Fails

Loss of a producer triggers immediate recalculation.

Storage and remaining generators respond before lower-priority load shedding where their configured rules permit.

## 50. Failure Recovery

A power failure is recovered by restoring one or more of:

- generation;
- fuel;
- thermal support;
- connection;
- relay state;
- storage;
- damaged hardware.

The power system reports the actual blocking dependency.

## 51. Progression

Early game:

- low generation margin;
- simple topology;
- manual priorities;
- limited storage;
- vulnerability to single failures.

Mid game:

- multiple producers;
- dedicated distribution;
- batteries;
- crew automation;
- redundancy.

Late game:

- multiple high-capacity networks;
- advanced storage;
- automated fault isolation;
- resilient redundant paths;
- high-demand weapons and industry;
- sophisticated generation policy.

Progression increases capability and resilience rather than removing the need for electrical design.

## 52. Explicit Non-Goals

The baseline power system does not simulate:

- AC waveform;
- voltage phase;
- reactive power;
- electromagnetic field propagation;
- manual wiring of every small appliance;
- arbitrary electrical engineering formulas that do not create useful gameplay.

## 53. Tuneable Parameters

Tuneable parameters include:

- MW generation;
- MWh storage;
- charge/discharge rates;
- connection capacities;
- standby demand;
- operating demand;
- startup demand;
- efficiency;
- reserve percentages;
- damage thresholds;
- generator ramp rates.

## 54. Dependencies

This specification depends on:

- [Home Station Overview](overview.md);
- [Station Structural Rules](structural_rules.md);
- `thermal_systems.md`;
- `storage_and_logistics.md`;
- `damage_and_repairs.md`;
- `automation.md`;
- `../resources/`;
- `../crew/`;
- `../presentation/`.

## 55. Open Questions

None in the current power-network baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
