# Research

> **Status:** Design Complete  
> **Authority:** Technology knowledge, research projects, evidence, laboratory work, Scientist contribution, prerequisites, project states, unlock outputs, and research persistence

## 1. Purpose

Research converts exploration knowledge and scientific effort into new technological capability.

The system must reward:

- exploration;
- artifacts;
- unusual samples;
- specialist recruitment;
- station investment;
- long-term technical progression.

It must not reduce science to enemies dropping generic points.

## 2. Technology vs Blueprint

Research unlocks **technology understanding**.

Blueprints unlock **specific object designs**.

A research project can:

- unlock a technology;
- produce one or more blueprints;
- unlock a process;
- improve a system limit;
- reveal analysis capability.

Researching a technology does not automatically grant every design using it.

## 3. Research Domains

Canonical research domains are:

- Materials;
- Energy;
- Propulsion;
- Electronics and Computing;
- Robotics;
- Life Science;
- Medicine;
- Station Engineering;
- Weapons and Defense;
- Astrophysics;
- Xenotechnology;
- Anomaly Physics.

## 4. Evidence Model

Project StarForge does not use a spendable universal "Science Point" currency.

Instead, secured analysis creates persistent **Research Evidence**.

Evidence has:

- Evidence ID;
- one or more research-domain tags;
- Evidence Value per tag;
- provenance;
- discovery/analyzed state.

Evidence represents knowledge already learned and is **not consumed** by research projects.

## 5. Evidence Sources

Evidence can come from:

- analyzed physical sample;
- recovered data carrier;
- scanned anomaly;
- reverse-engineered device;
- mission scientific objective;
- faction data;
- narrative discovery;
- Scientist analysis.

## 6. Evidence Example

Analyzing an intact Voltaic Crystal sample could produce:

- Materials Evidence: 2
- Energy Evidence: 2
- Anomaly Physics Evidence: 1

The physical sample may be preserved or consumed according to its analysis procedure.

## 7. Evidence Threshold

A research project can require minimum accumulated evidence in specific domains.

Example:

Advanced Superconductors:
- Materials Evidence >= 8
- Energy Evidence >= 4

Evidence is checked, not spent.

Completing the project does not reduce those values.

## 8. Unique Evidence Requirements

Advanced projects can additionally require specific Evidence IDs.

Example:

Ancient Core Interface Theory may require:
- analyzed Ancient Technology Core;
- Xenotechnology Evidence threshold;
- Electronics Evidence threshold.

This prevents generic grinding from replacing a specific discovery.

## 9. Research Project Definition

Every research project defines:

- Research Project ID;
- domain;
- prerequisites;
- evidence thresholds;
- unique evidence requirements;
- required lab class;
- required Scientist skill where applicable;
- research work required;
- physical sample/component requirements if consumed;
- outputs.

## 10. Research States

A project can be:

- Hidden;
- Discovered;
- Blocked;
- Available;
- Queued;
- Active;
- Paused;
- Completed.

### Hidden

The player does not know the project exists.

### Discovered

The project is visible but at least one prerequisite may be missing.

### Blocked

Known project with clearly identified missing hard requirements.

### Available

All start requirements are satisfied.

### Queued

Waiting for compatible research capacity.

### Active

Research work is progressing.

### Paused

Started but unable to continue because an operational requirement is missing.

### Completed

Outputs are permanently granted.

## 11. Research Capacity

Research requires compatible station laboratory capacity.

A lab exposes:

- supported domains;
- research-work throughput;
- power demand;
- thermal demand;
- workstation capacity;
- containment capability.

The research system does not create a parallel abstract laboratory detached from station infrastructure.

## 12. Scientist Role

Scientists provide professional research supervision.

A project may define a minimum Science skill.

Higher skill can improve:

- throughput;
- analysis speed;
- difficult-project eligibility.

A Scientist cannot substitute for missing lab technology, evidence, or physical sample.

## 13. Baseline Research Without Scientist

Basic early research can be performed through automated station analysis at reduced capability where explicitly marked.

Advanced research normally requires qualified Scientist supervision.

This avoids a hard early-game lock before the player finds a Scientist.

## 14. Research Work

Research progresses through active-game **Research Work**.

Research Work is generated only while:

- project is Active;
- compatible lab operates;
- required Scientist/automation is available;
- power/thermal requirements are satisfied;
- required physical inputs remain available.

No real-world offline research occurs.

## 15. Physical Inputs

Some projects require a physical item/sample.

The project definition states whether the input is:

- Required for Access but Preserved;
- Consumed During Analysis;
- Damaged/Transformed During Analysis.

The UI must disclose destructive analysis before start.

## 16. Research Queue

The player can queue projects.

Queue priority controls compatible laboratory allocation.

A queued project cannot start if its prerequisites are no longer satisfied.

## 17. Parallel Research

Parallel projects require separate available lab/workstation capacity and sufficient Scientist workload.

One Scientist cannot provide infinite simultaneous research supervision.

## 18. Research Cancellation

Cancelling before physical input consumption returns reservations.

Consumed analysis samples are not recreated.

Accumulated Research Work on a cancelled project is retained by default unless the project's procedure explicitly states that a nonrepeatable experiment stage was lost.

## 19. Research Completion

Completion atomically grants the project's defined outputs.

Possible outputs:

- Technology Unlock;
- Blueprint Unlock;
- Process Unlock;
- Analysis Capability;
- new visible research projects;
- bounded system improvement.

Completion occurs once.

## 20. No Random Research Failure

A correctly supplied standard research project does not have an arbitrary failure roll.

Operational interruption can pause research.

Hazardous experiments can create explicit risk only when the project defines the risk and mitigation.

## 21. Technology Tree Structure

Research is a directed prerequisite graph rather than one single linear tech tree.

Different branches support different play styles:

- industrial;
- defensive;
- robotics;
- exploration;
- research;
- propulsion.

Cross-domain prerequisite edges are allowed when technologically meaningful.

## 22. No Mandatory Total Research

The player need not unlock every branch to progress through the game.

Critical core progression has at least one reachable path.

Optional specialization can remain optional.

## 23. Canonical Foundational Technologies

The baseline establishes these technology concepts:

### TECH_BASIC_FABRICATION

Starting technology.  
Enables standard early component fabrication.

### TECH_ADVANCED_METALLURGY

Unlocks advanced titanium/tungsten/platinum processing classes.

### TECH_NUCLEAR_HANDLING

Unlocks Uranium Fuel Rod processing and safe fission-industrial handling.

### TECH_SUPERCONDUCTIVE_MATERIALS

Requires Materials/Energy Evidence.  
Unlocks Superconductive Material processing and Superconductive Coil crafting.

### TECH_FUSION_FUEL_HANDLING

Requires Energy/Propulsion Evidence.  
Unlocks Deuterium separation and Fusion Fuel Pellet processing.

### TECH_FUSION_ENGINEERING

Requires Fusion Fuel Handling + Superconductive Materials.  
Enables fusion-reactor/advanced-engine blueprint research.

### TECH_ADVANCED_SHIELDS

Requires Superconductive Materials + Energy Evidence.  
Enables advanced shield technology and related blueprints.

### TECH_ADVANCED_ROBOTICS

Requires Electronics + Robotics Evidence.  
Enables high-complexity robot control/components.

### TECH_QUANTUM_MATERIALS

Requires Superconductive Materials + Anomaly Physics Evidence + analyzed Voltaic Crystal + analyzed Exotic Matter Sample.  
Unlocks Quantum Substrate recipe.

### TECH_QUANTUM_COMPUTING

Requires Quantum Materials + Electronics/Computing Evidence.  
Unlocks Quantum Processor recipe and advanced computation designs.

### TECH_XENOBIOLOGY

Requires analyzed Alien Biomaterial.  
Unlocks alien biological analysis and advanced medical/biological research.

### TECH_ANCIENT_SYSTEMS

Requires analyzed Ancient Technology Core + Xenotechnology Evidence.  
Unlocks ancient-technology research branch rather than a single universal item.

## 24. Canonical Sample Analysis Procedures

The following baseline analyses are authoritative:

### ANALYSIS_VOLTAIC_CRYSTAL

Physical Input:
- 1 Voltaic Crystal

Procedure:
- Destructive

Output:
- Evidence Record: EVID_VOLTAIC_CRYSTAL

Evidence:
- Materials: 2
- Energy: 2
- Anomaly Physics: 1

The analyzed physical crystal is consumed.

### ANALYSIS_EXOTIC_MATTER

Physical Input:
- 1 Exotic Matter Sample

Procedure:
- Destructive

Output:
- Evidence Record: EVID_EXOTIC_MATTER

Evidence:
- Anomaly Physics: 4
- Energy: 1

The analyzed sample is consumed.

### ANALYSIS_ALIEN_BIOMATERIAL

Physical Input:
- 1 Alien Biomaterial

Procedure:
- Destructive

Output:
- Evidence Record: EVID_ALIEN_BIOMATERIAL

Evidence:
- Life Science: 3
- Medicine: 1
- Xenotechnology: 1

The analyzed sample is consumed.

### ANALYSIS_ANCIENT_CORE

Physical Input:
- 1 Ancient Technology Core

Procedure:
- Non-Destructive but Securely Occupied During Analysis

Output:
- Evidence Record: EVID_ANCIENT_CORE

Evidence:
- Xenotechnology: 4
- Electronics and Computing: 2
- Anomaly Physics: 2

The Ancient Technology Core remains a physical strategic item after successful analysis.

While analysis is active, it is reserved and unavailable to another project.

## 25. Evidence Accumulation

Evidence values are persistent and cumulative.

Repeated identical trivial scans do not infinitely generate evidence.

Every Evidence ID has one authoritative acquisition state.

Procedural discoveries may generate distinct evidence records only when world/mission rules classify them as scientifically distinct.

## 26. Duplicate Data

Recovering an identical already-integrated data record does not increase Evidence Value again.

It can still have economic/salvage value where relevant.

## 27. Research Discovery

Projects become visible through:

- prerequisite technology;
- evidence;
- blueprint discovery;
- narrative/faction information.

Hidden projects are not shown as unexplained locked silhouettes unless presentation intentionally uses a general undiscovered indicator.

## 28. Research and Crew Special Knowledge

Crew Special Knowledge can satisfy explicit project eligibility requirements or improve analysis.

It does not automatically add arbitrary Evidence Value unless the knowledge definition says it represents already-integrated evidence.

## 29. Research and Blueprints

Research can output Blueprint Unlocks.

Recovered blueprints can also reveal new research projects.

This creates two-way interaction without merging the concepts.

## 30. Research UI

The research interface shows:

- project description;
- domain;
- prerequisites;
- evidence requirements;
- unique evidence;
- physical input;
- Scientist requirement;
- lab requirement;
- work/progress;
- outputs;
- blocking conditions.

## 31. Persistence

Evidence, project visibility, progress, queues, consumed inputs, completed technologies, and outputs persist.

Save/load cannot duplicate completion or evidence.

## 32. Edge Cases

If a Scientist becomes incapacitated, affected project pauses or reassigns.

If the required lab loses power, progress pauses.

If a unique physical sample is consumed but the project is interrupted afterward, the project retains the analysis stage already completed rather than requiring impossible duplicate consumption unless explicitly defined.

If evidence threshold is later exceeded, previously Blocked projects update immediately.

## 33. Progression Philosophy

Early research improves basic station self-sufficiency and manufacturing.

Mid-game research branches into nuclear/fusion, robotics, defense, advanced materials, and navigation.

Late-game research depends increasingly on unique exploration evidence, exotic resources, faction technology, and ancient systems.

## 34. Explicit Non-Goals

Research does not use:

- generic enemy-dropped science points;
- evidence consumption as currency;
- random success/failure rolls for standard projects;
- real-world offline research;
- one Scientist supervising unlimited projects;
- automatic access to every blueprint in a researched field.

## 35. Tuneable Parameters

Tuneable values include evidence thresholds, Research Work requirements, lab throughput, Scientist modifiers, and parallel-capacity limits.

## 36. Dependencies

This specification depends on Resource Model, Resource Catalog, Blueprints, Processing, station power/thermal/manufacturing, Crew Scientist/skills/assignments, missions, world, factions, narrative, and progression.

## 37. Open Questions

None in the research-framework baseline.

Exact evidence thresholds and Research Work quantities remain balancing data, not unresolved rules.
