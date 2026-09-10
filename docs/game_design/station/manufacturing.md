# Station Manufacturing

> **Status:** Design Complete  
> **Authority:** Station production machines, processing queues, recipe execution, input/output behavior, manufacturing failure states, and industrial automation

## 1. Purpose

Manufacturing transforms acquired resources into useful progression outputs.

It links:

- expeditions;
- resources;
- station industry;
- crafting;
- construction;
- ships;
- robots;
- weapons;
- automation.

The station is the main location for scalable long-term production.

## 2. Manufacturing vs Field Crafting

Portable field tools may support limited emergency crafting where explicitly defined.

Industrial recipes requiring station machinery must use compatible station manufacturing equipment.

The player cannot craft large ship engines or station modules from a handheld menu without appropriate infrastructure.

## 3. Recipe Authority

Recipes are authoritative under `../resources/`.

A recipe defines:

- required inputs;
- output;
- quantity;
- compatible machine class;
- processing work/duration;
- power demand;
- thermal load;
- optional water/fluid demands;
- byproducts;
- prerequisite blueprint/research.

Manufacturing executes recipes but does not redefine them.

## 4. Machine Classes

Industrial equipment can include:

- refinery;
- smelter;
- fabricator;
- electronics assembler;
- chemical processor;
- food processor;
- ammunition plant;
- advanced component assembler;
- Robot Assembly Bay.

Technology progression may add new classes.

### Robot Assembly Bay

The Robot Assembly Bay is the station manufacturing capability used to assemble and commission persistent GDS-10 robots.

It provides:

- heavy/robot-scale assembly workspace;
- component installation support;
- control/software initialization;
- diagnostics;
- commissioning/output interface.

GDS-10 owns robot recipes, Robot ID creation, configuration validation, and robot-specific commissioning rules.

The Robot Assembly Bay still obeys this document's normal power, thermal, logistics, queue, work-in-progress, damage, and output-blocking rules.

## 5. Machine Local Storage

Each machine has authored local:

- input buffer;
- output buffer;
- work-in-progress state.

A station's global free storage does not bypass blocked local output or disconnected logistics.

## 6. Production Queue

A machine can contain an ordered production queue.

A queue entry contains:

- recipe;
- requested quantity;
- priority;
- repetition mode if supported;
- resource reservation state;
- progress.

## 7. Job States

A production job uses:

```text
Queued
↓
Waiting for Inputs
↓
Ready
↓
Processing
↓
Output Blocked
↓
Completed
```

Possible terminal state:

```text
Cancelled
```

A fault can temporarily interrupt Processing without destroying job identity.

## 8. Queued

The job exists but is not currently eligible to run because earlier jobs or machine policy take precedence.

## 9. Waiting for Inputs

At least one required input is unavailable, unreserved, or cannot be delivered.

## 10. Ready

Required inputs for the next execution unit are available and the machine's operational dependencies are satisfied.

## 11. Input Reservation

Before starting a recipe execution, required inputs are reserved.

Reservation prevents competing jobs from claiming the same resource.

The machine consumes inputs at the recipe-defined stage.

## 12. Processing

During Processing:

- progress advances with active game simulation;
- machine power is consumed;
- heat is produced;
- defined process inputs are consumed;
- the machine occupies its processing capacity.

Progress does not advance during real-world offline time.

## 13. Operational Requirements

A machine processes only while its required conditions are satisfied.

Typical conditions include:

- sufficient power;
- acceptable temperature;
- valid machine state;
- required inputs;
- output capacity;
- optional specialist/automation requirement where defined.

## 14. Power Interruption

If power falls below the machine's required operating threshold:

- processing pauses or enters safe state;
- completed progress is retained unless the recipe explicitly defines spoilage/instability;
- inputs are not duplicated.

## 15. Thermal Limitation

Overheated machinery follows thermal rules.

It can:

- slow;
- pause;
- shut down;
- fault.

Manufacturing output cannot ignore cooling limits.

## 16. Output

When a recipe execution completes, output is created exactly once.

Output first enters the machine's valid output buffer.

From there logistics can route it to storage or another consumer.

## 17. Output Blocked

If the output buffer cannot accept the completed output:

- the machine enters Output Blocked;
- it does not begin another conflicting production execution;
- output is not deleted.

## 18. Byproducts

A recipe may create one or more byproducts.

All required outputs must have valid handling according to the recipe before production proceeds where blockage would otherwise cause resource deletion.

## 19. Continuous Production

A queue entry may support:

- produce fixed quantity;
- repeat until cancelled;
- maintain target inventory.

Maintain-target mode uses accessible inventory and configured reservations to determine whether additional production is required.

## 20. Target Inventory

Automation can maintain a player-defined target.

Example:

> Keep at least 200 Packaged Rations accessible.

The system does not exceed policy indefinitely unless another job explicitly requests more.

## 21. Parallel Production

Multiple machines can operate simultaneously.

A single machine may have more than one production lane only if its authored machine design supports parallel capacity.

## 22. Machine Upgrades

Machine improvement can occur through:

- installed components;
- technology replacement;
- parallelization;
- improved automation.

An upgrade may modify tuneable performance but cannot bypass recipe technology locks.

## 23. Manufacturing Labor

Some early or specialized machines may require operator attention.

Automation progression can reduce routine staffing needs.

The exact crew requirement belongs to crew/automation specifications.

## 24. Engineer/Technician Support

Crew specialists can:

- maintain machines;
- diagnose faults;
- supervise automated lines;
- reduce downtime within defined skill rules.

They do not create input resources or bypass physical requirements.

## 25. Manual Player Operation

The player can operate or assist compatible machinery where designed.

Manual interaction is most valuable early-game or during fault recovery.

Late-game industry should not require the player to stand beside each machine.

## 26. Recipe Unlocking

A machine can execute only unlocked recipes.

Recipes may be unlocked through:

- research;
- recovered blueprints;
- faction access;
- story progression;
- salvaged technology.

Manufacturing capacity alone does not reveal unknown recipes.

## 27. Cancellation Before Consumption

If a queued job is cancelled before inputs are consumed:

- reservations are released;
- inventory is unchanged.

## 28. Cancellation After Consumption

If a job is cancelled after material consumption:

- unconsumed reserved inputs are released;
- consumed inputs are not recreated;
- work-in-progress salvage exists only if the recipe explicitly defines it.

## 29. Machine Faults

A machine can fault due to:

- damage;
- thermal stress;
- missing maintenance;
- control failure;
- recipe-specific process failure.

Fault state pauses affected production until resolved.

## 30. Manufacturing Priority

The player can prioritize queues/machines.

Priority influences:

- logistics supply;
- automation attention;
- optional power/cooling policies where those systems reference industrial priority.

Priority does not override critical life-support protections automatically.

## 31. Manufacturing Dashboard

The station interface communicates:

- machine status;
- active recipe;
- progress;
- queue;
- missing inputs;
- power status;
- thermal status;
- output blockage;
- projected completion;
- target-inventory status.

## 32. Visual Feedback

Operating machinery should visibly communicate activity through:

- moving mechanisms;
- emissive states;
- material flow;
- fabrication effects;
- progress indicators where appropriate.

Visual effects do not alter production results independently of system state.

## 33. Audio Feedback

Machine audio communicates:

- idle;
- active;
- warning;
- blocked;
- faulted;
- completed cycle.

## 34. Persistence

Save state preserves:

- queues;
- progress;
- reservations;
- consumed inputs;
- output buffers;
- target policies;
- machine faults.

Save/load must not duplicate recipe outputs or inputs.

## 35. Edge Case — Last Required Input Is Reserved Elsewhere

The job remains Waiting for Inputs.

It does not steal a valid reservation from another job unless player policy explicitly changes reservation priority.

## 36. Edge Case — Logistics Disconnects During Processing

Already-consumed inputs remain consumed.

The current job can continue if it no longer requires incoming material and all other dependencies remain valid.

Future inputs/outputs can block.

## 37. Edge Case — Output Network Disconnected

Output remains local.

If local capacity fills, production becomes Output Blocked.

## 38. Edge Case — Machine Destroyed Mid-Job

The work-in-progress state is resolved by damage/repair rules.

No automatic full input refund occurs.

## 39. Progression

Early game:

- primitive fabricator;
- few recipes;
- manual queues;
- low throughput.

Mid game:

- specialized processors;
- parallel machines;
- crew supervision;
- target-inventory automation.

Late game:

- industrial sectors;
- advanced production chains;
- high-throughput logistics;
- automated factories;
- specialized strategic manufacturing.

## 40. Explicit Non-Goals

The baseline does not allow:

- instant menu crafting of industrial-scale objects;
- production without inputs;
- production without required infrastructure;
- output teleportation through disconnected logistics;
- offline real-world production;
- random hidden recipe failure purely to add grind.

## 41. Tuneable Parameters

Tuneable values include:

- processing duration;
- machine throughput;
- power demand;
- heat output;
- buffer size;
- parallel capacity;
- efficiency;
- crew/automation modifiers.

## 42. Dependencies

This specification depends on:

- [Power](power.md);
- [Thermal Systems](thermal_systems.md);
- [Storage and Logistics](storage_and_logistics.md);
- [Water](water.md);
- `damage_and_repairs.md`;
- `automation.md`;
- `../resources/`;
- `../crew/`;
- `../progression/`.

## 43. Open Questions

None in the current manufacturing baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
