# Player Tools and Gadgets

> **Status:** Design Complete  
> **Authority:** Canonical portable utility-tool classes, tool modes, energy/Condition/thermal behavior, gadget classes/deployment, and tool interaction boundaries

## 1. Purpose

Tools provide direct hands-on engineering, mining, salvage, scanning, sample-recovery, and emergency capability. Gadgets provide bounded portable utility without replacing station/ship infrastructure.

## 2. Tool Philosophy

The baseline uses a small set of distinct tool classes with clear physical jobs.

Progression improves capability through documented axes such as:

- additional valid target classes;
- higher work efficiency;
- safer operation;
- additional authored modes;
- larger/efficient energy and thermal envelopes.

It does not create dozens of random rarity copies distinguished only by percentage inflation.

## 3. Canonical Tool Classes

The baseline contains these portable tool classes:

- Engineering Multitool;
- Mining Cutter;
- Salvage Cutter;
- Survey Scanner;
- Sample Collector.

Weapons are separate GDS-9 Equipment.

## 4. Engineering Multitool

The Engineering Multitool has exactly three primary modes:

- `Diagnose`;
- `Repair`;
- `Interface`.

A model may improve the capability/tier of those modes but does not silently add unrelated functions.

## 5. Diagnose Mode

Diagnose reveals only data that satisfies all of:

- the target exposes a diagnostic interface/path;
- the Multitool diagnostic capability supports that target/fault class;
- physical access/range is valid;
- required target power/control or passive diagnostic condition is met.

Diagnosis obeys the Player Knowledge boundary and cannot reveal hidden information unavailable to the underlying system.

## 6. Repair Mode

Repair performs a valid manual repair interaction only when:

- target is repairable at the available repair level;
- required materials/components are owned/accessible;
- Multitool capability is sufficient;
- target access/safety is valid;
- the sustained Simulation-Time interaction completes.

The tool supplies work/capability, not replacement matter.

## 7. Interface Mode

Interface performs only target-defined engineering-interface actions such as:

- service-port connection;
- calibration;
- eligible local controller reset;
- documented emergency bypass.

It cannot substitute for security hacking unless that exact target explicitly exposes an engineering override.

## 8. Mining Cutter

A Mining Cutter declares:

- supported deposit/hardness classes;
- extraction rate;
- energy source/demand;
- thermal profile;
- effective range;
- Condition profile.

GDS-4 Mining owns deposit depletion and extracted-resource ownership.

## 9. Mining Progression

Higher Mining Cutter capability may improve only documented axes:

- supported harder deposit classes;
- extraction rate;
- extraction efficiency/waste behavior defined by Mining;
- energy efficiency;
- thermal endurance;
- operating range where the model explicitly supports it.

It never creates an artificial higher-rarity ore from the same deposit.

## 10. Salvage Cutter

A Salvage Cutter is optimized for manufactured salvage and can act only on explicit salvage/breachable targets its capability supports.

Applicable operations include:

- cutting panels/fasteners;
- separating components;
- recovering machinery;
- opening designated damaged/service access.

GDS-4 Salvage owns resulting yield/ownership.

## 11. Mining vs Salvage Cutter

Mining Cutter targets geological/resource deposits.

Salvage Cutter targets manufactured/salvage structures.

They are not interchangeable unless a specific authored target explicitly accepts both capability classes.

## 12. Survey Scanner

The Survey Scanner has three baseline modes:

- Resource;
- Technical;
- Environmental.

Research may unlock explicitly authored additional modes such as Anomaly Analysis.

## 13. Scan Result Classes

A target may expose scan data from these classes when compatible with the active scanner mode/capability:

- resource identity;
- Yield Grade estimate;
- remaining-reserve estimate;
- environmental/hazard reading;
- salvageable-component information;
- technical fault/system state;
- mission clue/identifier;
- Research/anomaly observation.

The target/owning domain defines what data exists and its accuracy.

## 14. Scanner Knowledge Boundary

A scan succeeds only when:

- scanner mode/capability supports the target;
- range/path/signal requirements pass;
- target is detectable under its owning rules;
- required scan work completes.

Scanner UI does not reveal an undetectable target or undiscovered information simply because the player points at its location.

## 15. Sample Collector

The Sample Collector acquires designated biological, hazardous-scientific, or delicate material specimens.

Every sample definition declares its containment class. Collection is blocked or applies its explicit hazardous handling outcome if containment is inadequate.

## 16. Tool Slot and Switching

Exactly one primary Tool occupies the active Tool slot.

Additional Tools are Backpack objects.

Switching uses Equipment `SwapTime` on Simulation Time and interrupts any sustained action that requires the previous Tool.

## 17. Tool Energy

A powered Tool declares exactly one energy model:

- Suit Energy connection; or
- internal rechargeable energy reserve; or
- an explicitly authored consumable power source.

Energy consumption/recharge is finite and conserved.

## 18. Tool Thermal State

High-output tools that declare a thermal profile use:

- Ready;
- Warm;
- Overheated;
- Cooling.

At Overheated, high-output operation is blocked until temperature falls below the authored restart threshold. Cooling progresses on Simulation Time and follows environmental/equipment rules.

Tools without a thermal profile do not gain hidden overheating behavior.

## 19. Tool Condition

Tools use Equipment Condition Profiles.

Any Condition effect on efficiency, energy, modes, range, or Disabled behavior must be explicitly declared by that tool model.

There is no random break roll independent of Condition/damage.

## 20. Canonical Gadget Classes

The baseline contains these five canonical gadget classes:

- Portable Sensor Beacon;
- Emergency Power Link;
- Portable Shield Projector;
- Rescue Beacon;
- Field Storage Container.

They are baseline content classes but are not all starting equipment. Acquisition is governed by Research/Blueprint/Crafting/mission progression.

## 21. Portable Sensor Beacon

A deployed Sensor Beacon provides local sensor coverage with finite:

- sensor class/range;
- energy;
- durability;
- communications interface.

It shares data only through valid communication/control paths and never exceeds its sensor capability.

## 22. Emergency Power Link

An Emergency Power Link provides temporary electrical power to a compatible target.

It declares:

- stored-energy capacity or required external source;
- maximum output/throughput;
- connector compatibility;
- Condition.

It cannot supply a load beyond its actual output or generate energy from nothing.

## 23. Portable Shield Projector

A Portable Shield Projector creates a localized GDS-9-compatible shield volume after valid deployment.

It has finite:

- shield capacity;
- energy supply;
- recharge/cooldown behavior if supported;
- physical placement constraints;
- durability.

It does not provide whole-station protection or change GDS-9 damage rules.

## 24. Rescue Beacon

A Rescue Beacon broadcasts the player's/target's location through its actual communication capability.

Activation does not guarantee rescue. A response requires valid GDS-12 Dynamic Event/Mission/Recovery capability and an actual reachable responder.

## 25. Field Storage Container

A Field Storage Container is a deployed physical owner with finite Mass/Volume and containment capability.

Placing loot inside does **not** change it to Vehicle/Extraction-Secured unless a Mission/Extraction rule explicitly recognizes that specific owner as secure. Baseline portable field containers are not secure extraction owners.

## 26. Gadget Deployment Profile

Each Gadget declares a `DeploymentProfile`:

- `Instant` or `Sustained`;
- required surface/volume/environment;
- clearance;
- Simulation-Time deployment duration if Sustained;
- interruption policy;
- activation requirements.

Deployment consumes/transfers the same carried gadget object into a world owner; it does not spawn a duplicate.

## 27. Gadget Retrieval

A reusable deployed gadget returns to Player Inventory only when:

- it still exists and is retrievable;
- player can physically access it;
- current mission/objective state permits retrieval;
- Backpack Mass/Volume/containment permits transfer;
- retrieval interaction commits.

If capacity fails, it remains deployed.

## 28. Gadget Destruction

A destroyed deployed gadget follows its Equipment/world-item destruction/Condition rules.

Routine defeat never creates a replacement copy of a gadget already destroyed or abandoned.

## 29. Tool/Gadget Crafting

Recipes use GDS-4 canonical Resources/Components and Blueprints/Research.

Any authored tool/gadget model that needs a previously undefined material must first extend the Resource Catalog under GDS-4 authority.

## 30. Feedback

The HUD/tool presentation communicates legitimately known:

- target compatibility;
- active mode;
- energy;
- thermal state;
- Condition;
- work/deployment progress;
- missing material/tool/access blocker;
- scan confidence/state where applicable.

Completion feedback occurs only after the underlying action commits.

## 31. Persistence

Save state preserves:

- Tool/Gadget identity/owner;
- active mode;
- Condition;
- energy/thermal state where relevant;
- deployed transform/state;
- contained inventory for Field Storage;
- persistent scan/knowledge results only when their owning system marks them committed.

Transient presentation effects are reconstructable and non-authoritative.

## 32. Edge Cases

- If Suit Energy reaches a protected life-support threshold, connected Tools lose power according to Equipment priority; they cannot drain protected life support without authorized override.
- If Mining Cutter overheats, already committed extracted output remains valid and work pauses.
- A disconnected Sensor Beacon continues only local behavior explicitly declared by its profile; it does not keep sharing data magically.
- A Rescue Beacon with no communication path may remain broadcasting locally but creates no remote knowledge/response until a valid path exists.
- Destroying a Field Storage Container applies its container/content damage rule; contents are not teleported to Player Inventory.
- Losing a required Tool during a sustained task invokes that target's interruption policy.

## 33. Tuneable Parameters

Tuneable values include:

- Tool range/work speed;
- energy capacity/use;
- thermal thresholds/cooling;
- scanner ranges/work duration;
- gadget capacity/range/durability;
- shield values;
- deployment/retrieval duration.

Class set, mode semantics, physical ownership, and knowledge/energy constraints are fixed.

## 34. Explicit Non-Goals

The baseline does not include:

- one universal magic Tool;
- omniscient scanning;
- tool operation without required energy/materials;
- station-scale output from portable gadgets;
- generic random tool/gadget rarity tiers;
- automatic extraction security from a Field Storage Container;
- guaranteed rescue from beacon activation.

## 35. Dependencies

Depends on Interaction, Equipment, Inventory, Movement, GDS-4 Mining/Salvage/Resources/Crafting/Research, Station Damage/Power, Missions/Extraction, GDS-9 Combat/Shields, Dynamic Events, Recovery Transit, Time/Simulation, and GDS-13 Feedback/Accessibility.

## 36. Open Questions

None. Model-specific numerical data is authored/tuneable content under these fixed contracts.
