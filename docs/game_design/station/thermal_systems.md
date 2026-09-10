# Station Thermal Systems

> **Status:** Design Complete  
> **Authority:** Heat generation, temperature state, coolant transport, radiator rejection, thermal protection, and thermal failure on the home station

## 1. Purpose

The thermal system turns heat into a strategic infrastructure constraint.

High-output equipment cannot operate indefinitely without adequate heat removal.

The system supports:

- reactors;
- industry;
- weapons;
- environmental control;
- coolant loops;
- pumps;
- heat exchangers;
- radiators;
- redundancy;
- overheating and safe shutdown.

## 2. Thermal Model

The station uses a simplified heat-energy model.

Major thermal nodes track:

- current temperature;
- heat generation;
- heat capacity;
- heat transfer capability;
- operating limits.

The game does not simulate microscopic heat conduction through every polygon.

## 3. Thermal Domains

The baseline distinguishes:

- **equipment temperature**;
- **coolant-loop temperature**;
- **compartment ambient temperature**.

These can influence each other through defined transfer components.

## 4. Heat Sources

Heat sources include, where defined:

- reactors;
- generators;
- fabrication;
- refineries;
- research equipment;
- ship servicing;
- high-energy weapons;
- electronics;
- fires.

Each heat source reports heat generation based on current operating state.

## 5. Heat Sinks

Heat can be removed or redistributed through:

- coolant loops;
- heat exchangers;
- radiators;
- thermal storage;
- reduced equipment output;
- shutdown.

External radiators are the primary long-term rejection method for large station heat loads.

## 6. Coolant Networks

Coolant infrastructure forms one or more connected networks.

A coolant network contains:

- pumps;
- coolant inventory;
- pipes/service connections;
- heat exchangers;
- radiators;
- monitored equipment.

Disconnected loops do not exchange cooling capacity.

## 7. Coolant Flow

Coolant flow requires a valid path and operating pump capability unless a specific technology supports passive circulation.

Flow has finite throughput.

A loop can therefore have enough radiator capacity but still fail because flow to the hot equipment is insufficient.

## 8. Power Dependency

Most active thermal-control equipment requires electrical power.

A power failure can stop:

- pumps;
- active valves;
- control systems;
- powered heat exchangers.

Residual thermal inertia remains; temperature changes over time rather than instantly.

## 9. Radiators

Radiators reject heat from connected coolant.

Radiator effectiveness depends on:

- deployed/operational state;
- damage;
- connected coolant flow;
- current thermal conditions.

Radiators are external components and can be strategically vulnerable.

## 10. No Arbitrary Heat Deletion

Heat cannot disappear because a device is connected to a coolant loop.

A stable long-duration operating state requires enough heat rejection or an explicit consumable/temporary heat sink.

## 11. Equipment Temperature Bands

Thermally relevant equipment defines temperature bands:

- **Cold/Below Operating Range** where applicable;
- **Nominal**;
- **Elevated**;
- **Degraded**;
- **Critical**;
- **Emergency Shutdown**;
- **Damage Range**.

Exact thresholds belong to the device definition.

## 12. Degraded Operation

Equipment may reduce performance as temperature rises.

Examples:

- reactor output limit;
- slower fabrication;
- weapon fire-rate restriction;
- battery charge-rate reduction.

A device does not silently remain at full performance beyond its thermal design limit.

## 13. Automatic Thermal Protection

Eligible equipment automatically attempts protection before destructive overheating.

Possible actions:

1. reduce output;
2. increase pump demand;
3. open additional radiator paths;
4. shed optional heat-producing loads;
5. initiate controlled shutdown.

Automation behavior follows configured policy.

## 14. Emergency Shutdown

A device that reaches its emergency threshold can enter a protective shutdown.

Restart requires:

- temperature below restart threshold;
- blocking faults resolved;
- required power/control available.

The player cannot instantly restart equipment that is still thermally unsafe.

## 15. Thermal Damage

If protection fails or is overridden where permitted, excessive temperature can cause damage.

Damage may affect:

- efficiency;
- maximum output;
- coolant integrity;
- structural safety;
- fire probability.

Detailed damage rules belong to `damage_and_repairs.md`.

## 16. Thermal Runaway

Selected high-energy equipment may produce a runaway condition if heat generation rises as cooling performance fails.

Runaway is an authored device behavior, not a universal rule.

The system must clearly communicate escalating risk.

## 17. Compartment Temperature

Habitable compartments track ambient temperature.

Ambient temperature changes through:

- life-support climate control;
- equipment heat;
- adjacent heat exchange;
- external exposure abstractions;
- fire.

Crew comfort and safety depend on the resulting temperature.

## 18. Climate Control

Atmospheric climate-control equipment maintains target compartment temperature within capacity limits.

It requires:

- power;
- functioning equipment;
- sufficient thermal rejection path.

Climate control cannot violate conservation by cooling a station with no way to reject accumulated heat.

## 19. Player and Crew Exposure

The thermal system reports environmental state to player and crew systems.

Possible states:

- comfortable;
- cold;
- hot;
- dangerous cold;
- dangerous heat.

Protective suits can modify exposure.

## 20. Coolant Inventory

Coolant networks contain finite working fluid.

A leak reduces available coolant.

Insufficient coolant reduces loop performance and can disable circulation.

Coolant can be replenished through resource/logistics rules.

## 21. Coolant Leaks

Damage may create a coolant leak.

A leak:

- loses coolant;
- reduces cooling capacity;
- can create local visual/audio effects;
- may eventually empty an isolated loop.

The game does not automatically refill leaked coolant.

## 22. Valves

Controllable valves can isolate thermal branches.

Isolation can:

- contain a coolant leak;
- protect an intact loop;
- intentionally disconnect equipment.

An isolated heat source loses that cooling path.

## 23. Multiple Cooling Loops

Players may build independent or interconnected cooling loops.

Multiple loops support:

- redundancy;
- specialization;
- damage containment.

A central loop may be efficient but creates larger failure domains.

## 24. Heat Storage

Advanced systems may temporarily store heat.

Heat storage has finite capacity.

It is useful for:

- burst weapon fire;
- emergency operation;
- temporary radiator loss.

Stored heat must eventually be rejected or otherwise processed.

## 25. Station Weapons

High-energy defensive weapons may create burst thermal loads.

Thermal capacity can therefore limit sustained defensive fire even when electrical power is sufficient.

This makes power and cooling distinct strategic constraints.

## 26. Reactor Coupling

A reactor's available electrical output can depend on cooling capacity.

If cooling falls:

- reactor control first reduces output where possible;
- emergency shutdown follows at unsafe conditions;
- severe consequences occur only if protection fails or is overridden.

## 27. Sensor Dependency

Temperature sensors provide data for automation and UI.

Sensor failure does not prevent the physical temperature from changing.

Local equipment may retain independent emergency protection where authored.

## 28. Player Controls

The player can, subject to installed technology:

- inspect thermal networks;
- monitor temperatures;
- change pump policy;
- open/close valves;
- assign cooling priority;
- change equipment output limits;
- deploy/retract compatible radiators;
- authorize emergency shutdown/restart.

## 29. Cooling Priority

When cooling capacity is insufficient, automation can prioritize equipment.

Default priorities favor:

1. critical safety and reactor stability;
2. life support;
3. essential station operation;
4. defense;
5. industry/research;
6. discretionary loads.

The player may configure eligible priorities.

Manual shutdown remains authoritative.

## 30. Thermal Overlay

The diagnostic interface communicates:

- equipment temperature;
- coolant-loop temperature;
- coolant quantity;
- flow;
- radiator load;
- thermal bottlenecks;
- leaks;
- isolated branches;
- overheating devices.

Color is not the sole indicator.

## 31. Visual Feedback

Thermal state may be represented through:

- glowing hot components;
- steam/vapor where physically appropriate;
- radiator animation;
- warning lights;
- coolant leakage;
- reduced machinery activity.

Visual presentation must remain consistent with the system state.

## 32. Audio Feedback

Important cues include:

- pump operation;
- radiator mechanisms;
- coolant alarms;
- high-temperature warnings;
- emergency shutdown;
- leak sounds.

Critical status must also be visible.

## 33. Persistence

Save state preserves:

- node temperatures;
- coolant quantities;
- valve states;
- pump configuration;
- leaks;
- thermal faults;
- heat-storage state.

Loading must not reset overheated equipment to nominal temperature.

## 34. Edge Case — Power Loss

When pump power is lost:

- active flow falls according to equipment behavior;
- heat generation may continue briefly;
- devices respond through protection logic;
- temperature evolves over time.

Power loss is therefore dangerous but not an instantaneous arbitrary explosion.

## 35. Edge Case — Radiators Destroyed

Loss of radiator capacity does not instantly overheat the station.

Existing thermal mass provides time.

Long-term heat accumulation eventually forces load reduction or shutdown.

## 36. Edge Case — Excess Cooling Capacity

Extra radiator/pump capacity improves margin but does not cool equipment below its authored controllable target without a system designed to do so.

## 37. Edge Case — Disconnected Powered Branch

A structurally disconnected branch with local power and local cooling may continue operating independently.

It cannot use cooling paths across the severed connection.

## 38. Recovery

Thermal failure is recovered by combinations of:

- reducing heat load;
- restoring power;
- repairing pumps;
- restoring coolant;
- isolating leaks;
- restoring radiator capacity;
- reconnecting loops;
- allowing temperature to fall.

## 39. Progression

Early game:

- one simple cooling loop;
- little spare capacity;
- vulnerable external radiator.

Mid game:

- redundant pumps;
- multiple radiators;
- automated control;
- reserve coolant.

Late game:

- multiple specialized loops;
- high-capacity heat rejection;
- thermal storage;
- automated isolation;
- resilient high-output support.

## 40. Explicit Non-Goals

The thermal system does not require:

- molecular thermodynamics;
- per-triangle heat simulation;
- full computational fluid dynamics;
- real orbital solar-heating calculations for every surface;
- arbitrary heat disappearance.

## 41. Tuneable Parameters

Tuneable values include:

- heat generation;
- heat capacity;
- temperature thresholds;
- pump throughput;
- radiator rejection;
- coolant capacity;
- leak rate;
- thermal-storage capacity;
- protection delays.

## 42. Dependencies

This specification depends on:

- [Power](power.md);
- [Atmosphere and Oxygen](atmosphere_and_oxygen.md);
- `water.md`;
- `damage_and_repairs.md`;
- `automation.md`;
- `../resources/`;
- `../crew/`.

## 43. Open Questions

None in the current thermal baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
