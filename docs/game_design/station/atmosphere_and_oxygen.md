# Station Atmosphere and Oxygen

> **Status:** Draft  
> **Authority:** Compartment pressure, breathable atmosphere, gas exchange, ventilation, decompression, and oxygen-support behavior on the home station

## 1. Purpose

The atmosphere system makes sealed station interiors physically meaningful.

It supports:

- breathable compartments;
- vacuum exposure;
- decompression;
- airlocks;
- oxygen production;
- carbon-dioxide removal;
- ventilation;
- compartment isolation;
- life-support failure;
- emergency automation.

The design uses a simplified gas model rather than full computational fluid dynamics.

## 2. Atmospheric State

Every sealed compartment tracks at minimum:

- total pressure;
- oxygen fraction;
- carbon-dioxide concentration;
- generalized harmful-contaminant concentration;
- temperature reference supplied by the thermal system.

These values determine habitability.

## 3. Units

Player-facing pressure is displayed in **kPa**.

Gas composition is displayed using percentages or clearly labeled concentration values.

Exact safe thresholds are tuneable parameters.

## 4. Vacuum

A compartment with effectively no atmosphere is in a Vacuum state.

Vacuum:

- is not breathable;
- prevents unprotected crew occupancy;
- requires the player to use suitable sealed equipment;
- changes fire behavior;
- allows pressure loss from connected pressurized spaces.

## 5. Pressure Boundaries

Atmosphere is contained by pressure-capable station geometry defined in `structural_rules.md`.

A compartment remains sealed only while all relevant:

- hull surfaces;
- bulkheads;
- windows;
- doors;
- hatches;
- service penetrations

remain pressure-capable in their current state.

## 6. Compartment Identification

The atmosphere system derives pressure volumes from structural compartment boundaries.

Room labels or decoration do not determine atmosphere.

Opening or closing a pressure-rated connection can merge or separate atmospheric volumes.

## 7. Open Connections

When two compartments are connected by an open pressure path, their atmospheres exchange.

The simulation moves pressure and gas composition toward equilibrium over time.

The exchange rate depends on the authored opening class and pressure difference.

## 8. Door Opening Under Pressure Difference

A pressure-rated door can refuse a normal open command when the pressure differential exceeds its safe automatic threshold.

The player may use an explicit emergency override only where the door specification allows it.

The UI must warn about the expected consequence before a dangerous override.

## 9. Decompression

A breach or unsafe opening from a pressurized compartment to a lower-pressure volume causes decompression.

Decompression:

- reduces pressure;
- moves gas through the opening;
- can propagate through open doors;
- can trigger alarms and automatic bulkhead response;
- may apply simplified directional forces to nearby loose objects or characters.

The game does not require full fluid-dynamic airflow simulation.

## 10. Breach Flow

Leak rate is determined from simplified factors including:

- pressure difference;
- breach/opening size class;
- compartment volume;
- current atmosphere.

The model must be deterministic and understandable.

## 11. Automatic Isolation

When a major decompression is detected, eligible automation may close pressure-rated doors and bulkheads to isolate the affected compartment.

Automation respects:

- door operability;
- available power or local backup;
- player-configured emergency policy;
- obstruction rules.

A physically jammed or destroyed door cannot close because automation requested it.

## 12. Occupant Safety During Isolation

Emergency isolation prioritizes containing the hazard.

The system also reports occupants detected in the affected area.

Automatic bulkhead behavior does not magically teleport crew to safety.

Crew response is defined by the crew domain.

## 13. Oxygen Requirement

A breathable compartment requires sufficient:

- total pressure;
- oxygen partial availability;
- acceptable carbon-dioxide level;
- acceptable contaminant level;
- temperature.

A high oxygen percentage at near-zero total pressure is not considered breathable.

## 14. Carbon Dioxide

Occupied compartments accumulate carbon dioxide through crew metabolism.

Life-support equipment removes carbon dioxide.

Excessive concentration creates progressively more severe habitability penalties.

## 15. Generalized Contaminants

The baseline tracks a generalized harmful-contaminant value for events such as:

- smoke;
- toxic industrial release;
- chemical contamination.

Special mission gases may use their own authored systems, but normal station atmosphere does not simulate dozens of individual gas species.

## 16. Life-Support Equipment

Atmospheric infrastructure can include:

- oxygen generators;
- carbon-dioxide scrubbers;
- air processors;
- circulation/ventilation equipment;
- pressure pumps;
- gas storage tanks;
- sensors.

Each device requires its defined dependencies.

## 17. Oxygen Generation

Oxygen-generation equipment produces breathable oxygen using a defined process.

A generator may require:

- electrical power;
- water or another feedstock;
- functioning machinery;
- available storage or connected compartment demand.

Oxygen is not created for free.

## 18. Scrubbing

Scrubbers remove carbon dioxide and selected contaminants.

A scrubber has finite processing capacity.

If crew production exceeds available scrubbing capacity, air quality degrades even when oxygen remains sufficient.

## 19. Ventilation Network

Closed compartments can exchange processed atmosphere through compatible ventilation infrastructure.

Ventilation has finite throughput.

A physically sealed door therefore does not necessarily imply zero air-service connection.

Valves or dampers may isolate ventilation branches.

## 20. Atmosphere Storage

Pressurized gas storage can hold atmospheric reserve.

Storage can support:

- emergency repressurization;
- airlock operation;
- local life-support reserve.

Stored gas is finite.

## 21. Repressurization

A depressurized compartment can be repressurized only if:

1. its pressure boundary is sufficiently sealed;
2. a valid gas source exists;
3. the connection path is available;
4. required pumps/valves can operate.

Repressurization consumes stored or newly produced gas.

## 22. Airlocks

A normal airlock cycle follows:

```text
Verify opposite door closed
    ↓
Seal current chamber
    ↓
Adjust chamber pressure toward destination side
    ↓
Verify safe differential
    ↓
Unlock destination door
```

Both main airlock doors cannot be normally opened simultaneously.

Emergency override behavior must explicitly warn the player.

## 23. Player Exposure

The player character's response to atmosphere is defined by the player health/survival specifications.

The atmosphere system reports environmental conditions such as:

- breathable;
- low oxygen;
- high CO2;
- toxic;
- low pressure;
- vacuum.

A sealed suit can provide independent life support for a finite duration.

## 24. Crew Exposure

Crew require habitable atmosphere in occupied areas.

Routine environmental failure causes evacuation, injury, or incapacitation according to crew rules rather than routine permanent deletion of crew.

## 25. Atmosphere and Fire

Where fire exists:

- fire consumes available oxygen;
- produces heat;
- produces smoke/contaminants;
- may self-extinguish if atmospheric conditions no longer support combustion.

Detailed fire rules belong to station events/damage specifications.

## 26. Atmosphere and Power

Active life-support devices usually require power.

Loss of power does not make existing atmosphere vanish instantly.

Instead:

- ventilation stops or degrades;
- oxygen production stops;
- scrubbing stops;
- pressure pumps stop;
- gas quality then changes over time.

This creates recovery time rather than immediate arbitrary death.

## 27. Local Backup

Selected emergency atmosphere controls may have local backup power.

Backup duration is finite.

The presence of emergency lighting does not imply active life-support processing.

## 28. Atmosphere Sensors

Sensors provide compartment pressure and composition data.

If sensors fail, the physical atmosphere still exists but exact UI information may become unavailable or uncertain.

Local alarms may also fail if their sensor/control dependency is lost.

## 29. Manual Control

The player may, where equipment permits:

- close/open valves;
- isolate ventilation branches;
- seal bulkheads;
- vent a compartment;
- initiate repressurization;
- change life-support targets;
- inspect sensor data;
- authorize emergency overrides.

## 30. Deliberate Venting

The player can deliberately vent an eligible compartment to space.

The action requires explicit confirmation if:

- crew are detected inside;
- valuable atmosphere will be lost;
- connected compartments risk decompression.

Venting can be tactically useful for:

- fire suppression;
- contamination control;
- preparation for external maintenance.

## 31. Habitability States

A compartment is classified for UI/AI purposes as:

- **Habitable**;
- **Warning**;
- **Unsafe**;
- **Critical**;
- **Vacuum**.

Classification is derived from actual atmosphere values.

It is not a separate manually assigned property.

## 32. Alarm Behavior

Atmospheric alarms can trigger for:

- rapid pressure loss;
- unsafe low pressure;
- low oxygen;
- high CO2;
- contamination;
- airlock fault.

Critical alarms are communicated through visual and audio channels.

## 33. Station-Wide Atmosphere Overview

The diagnostic interface provides:

- compartment pressure;
- oxygen;
- CO2;
- contamination;
- ventilation connectivity;
- isolated compartments;
- active leaks;
- gas-storage status;
- life-support capacity.

## 34. Persistent State

The save state preserves:

- compartment gas state;
- tank contents;
- valve states;
- active leaks;
- life-support configuration;
- relevant sensor faults.

Save/load must not magically normalize atmosphere.

## 35. Edge Case — Door Opens Between Different Atmospheres

Opening a connection immediately creates gas exchange.

The resulting composition is calculated from both source volumes rather than choosing one side's atmosphere.

## 36. Edge Case — Compartments Become One Volume

If structural changes create a continuously open pressure volume, the atmosphere system treats the combined space as communicating volumes and moves toward equilibrium.

## 37. Edge Case — Compartment Is Split

If a new valid pressure boundary divides one atmosphere volume, each new compartment inherits its physically corresponding share of current atmospheric state.

The system does not duplicate gas mass.

## 38. Edge Case — Life Support Over Capacity

If crew and compartment demand exceed processing capacity:

- oxygen may decline;
- CO2 may rise;
- recovery requires reducing demand or adding capacity.

The game does not grant hidden scaling capacity merely because more crew were recruited.

## 39. Edge Case — Sensor Failure

A failed sensor hides or degrades data but does not change physical gas state.

Automation that requires that sensor may become unavailable or fall back to safe local behavior.

## 40. Recovery

Atmospheric failure can be recovered through combinations of:

- sealing breaches;
- isolating damaged sections;
- restoring power;
- restoring life support;
- supplying water/feedstock;
- using stored gas;
- repairing pumps/valves;
- repressurizing.

The interface should identify the actual blocking dependency.

## 41. Progression

Early game:

- small sealed volume;
- low reserve;
- basic oxygen generation;
- limited automation.

Mid game:

- multiple life-support zones;
- gas storage;
- automated isolation;
- redundancy.

Late game:

- distributed processing;
- redundant ventilation;
- large reserve capacity;
- advanced contamination control;
- automated fault handling.

## 42. Explicit Non-Goals

The atmosphere system does not require:

- full Navier-Stokes simulation;
- molecule-level gas tracking;
- dozens of normal atmospheric gas species;
- instant death from a momentary life-support power loss;
- ordinary doors behaving as magical pressure barriers when destroyed or open.

## 43. Tuneable Parameters

Tuneable values include:

- safe pressure thresholds;
- oxygen thresholds;
- CO2 thresholds;
- leak rates;
- ventilation throughput;
- oxygen production;
- scrubber capacity;
- gas tank capacity;
- airlock cycle rates;
- suit exposure thresholds.

## 44. Dependencies

This specification depends on:

- [Structural Rules](structural_rules.md);
- [Power](power.md);
- `thermal_systems.md`;
- `water.md`;
- `damage_and_repairs.md`;
- `automation.md`;
- `../crew/`;
- `../player/`.

## 45. Open Questions

None in the current atmosphere baseline.

The document remains Draft until player survival, crew, thermal, water, damage, and automation behavior are cross-validated.
