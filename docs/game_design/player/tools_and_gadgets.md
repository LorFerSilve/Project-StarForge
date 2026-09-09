# Player Tools and Gadgets

> **Status:** Draft  
> **Authority:** Canonical portable utility-tool classes, tool modes, energy/condition behavior, gadget deployment, and tool interaction boundaries

## 1. Purpose

Tools give the player direct hands-on capability for engineering, mining, salvage, scanning, sample recovery, and emergency response.

Gadgets provide limited portable utility without replacing station infrastructure.

## 2. Tool Philosophy

The baseline uses a small set of distinct tools with clear jobs.

The game avoids dozens of nearly identical tool tiers whose only difference is a percentage stat increase.

Progression should primarily unlock:

- new target classes;
- better efficiency;
- safer operation;
- additional modes;
- higher energy capacity.

## 3. Canonical Tool Classes

The baseline portable tool classes are:

- Engineering Multitool;
- Mining Cutter;
- Salvage Cutter;
- Survey Scanner;
- Sample Collector.

Weapons are not tools and belong to GDS-9.

## 4. Engineering Multitool

Primary purpose:

- basic repair;
- system diagnostics;
- panel/service interaction;
- construction assistance;
- emergency bypass procedures explicitly permitted by the target.

The tool can expose modes:

- Diagnose;
- Repair;
- Interface.

## 5. Diagnose Mode

Diagnose can reveal faults only when:

- the target supports local diagnosis;
- the tool has sufficient diagnostic tier;
- the player has physical access.

It does not create hidden information the system cannot sense.

## 6. Repair Mode

Repair performs compatible manual repair interactions.

Repair requires:

- valid target;
- correct repair material/components;
- sufficient tool capability;
- active interaction time.

The tool does not replace repair resources.

## 7. Interface Mode

Interface supports compatible local equipment actions such as:

- connecting to service port;
- calibrating;
- resetting an eligible local controller;
- performing a documented emergency bypass.

It cannot bypass security/hacking rules unless the target explicitly allows an engineering override.

## 8. Mining Cutter

The Mining Cutter extracts compatible Resource Deposits.

It defines:

- supported hardness class;
- extraction rate;
- energy demand;
- thermal limit;
- range;
- condition.

Mining owns deposit/yield behavior.

## 9. Mining Cutter Progression

Higher mining technology can:

- mine harder deposit classes;
- improve extraction rate;
- reduce wasted extraction work;
- improve thermal endurance.

It cannot create higher-quality ore variants.

## 10. Salvage Cutter

The Salvage Cutter dismantles designated salvage targets.

It can:

- cut structural panels;
- separate components;
- recover machinery;
- open certain damaged access points.

Salvage rules own recovered yield.

## 11. Salvage Cutter vs Mining Cutter

Mining Cutter:

- optimized for geological deposits.

Salvage Cutter:

- optimized for manufactured structures/machinery.

The tools are not automatically interchangeable.

## 12. Survey Scanner

The Survey Scanner identifies and analyzes compatible targets.

Possible scan outputs include:

- resource deposit identity;
- Yield Grade estimate;
- remaining reserve estimate;
- hazard;
- salvageable components;
- mission clue;
- environmental reading;
- research/anomaly data.

The target/domain owns what information exists.

## 13. Scanner Knowledge Boundary

The scanner reveals information only when:

- its sensor class supports it;
- line/range requirements are met;
- the target is detectable.

It is not an omniscient wallhack.

## 14. Scanner Modes

Baseline scanner modes:

- Resource;
- Technical;
- Environmental.

Special research technology can add authored modes such as Anomaly analysis.

## 15. Sample Collector

The Sample Collector safely acquires designated:

- biological samples;
- hazardous scientific samples;
- delicate material specimens.

It defines containment compatibility.

A sample requiring specialized containment cannot be safely pocketed without it.

## 16. Tool Slot

One primary utility tool occupies the active Tool equipment slot.

Switching tools takes a short action time.

Additional tools can be carried in backpack storage if capacity permits.

## 17. Tool Energy

Powered tools use either:

- Suit Energy while connected; or
- an internal rechargeable energy reserve.

The item definition specifies which.

Energy is conserved and must be replenished.

## 18. Tool Thermal State

High-output tools can accumulate heat.

Baseline thermal states:

- Ready;
- Warm;
- Overheated;
- Cooling.

An Overheated tool pauses high-output use until temperature falls.

## 19. Tool Condition

Tools use Equipment Condition.

Low condition can:

- reduce efficiency;
- increase energy use;
- block advanced modes;
- disable the tool at critical failure.

Effects must be explicit.

## 20. No Random Tool Failure

An operational tool does not randomly break during normal use without a defined condition/damage mechanism.

## 21. Canonical Gadget Classes

The baseline allows gadgets including:

- Portable Sensor Beacon;
- Emergency Power Link;
- Portable Shield Projector;
- Rescue Beacon;
- Field Storage Container.

These are utility objects, not mandatory starting equipment.

## 22. Portable Sensor Beacon

A deployed beacon provides local sensor coverage to systems that can receive its data.

It has finite:

- range;
- energy;
- durability.

It does not reveal information beyond its sensor capability.

## 23. Emergency Power Link

A portable power link can provide limited temporary power to a compatible target.

It has:

- finite stored energy or external source requirement;
- maximum output;
- connector compatibility.

It cannot power arbitrary station-scale loads beyond its rating.

## 24. Portable Shield Projector

A projector creates a temporary localized defensive shield where future Combat rules permit.

It has finite:

- energy;
- capacity;
- duration/cooldown;
- placement restrictions.

It does not replace station shields.

## 25. Rescue Beacon

A Rescue Beacon marks and communicates the player's position where communication conditions permit.

Mission rules determine whether anyone can respond.

Activating the beacon does not guarantee extraction.

## 26. Field Storage Container

A deployable container provides temporary local physical storage.

It has:

- finite mass/volume capacity;
- world ownership;
- mission persistence according to mission rules.

It does not secure loot automatically merely because resources are inside it.

## 27. Gadget Deployment

Deployment requires:

- valid surface/space;
- unobstructed placement volume;
- correct environment;
- carried gadget;
- active deployment time where appropriate.

## 28. Gadget Retrieval

Reusable gadgets can be retrieved when:

- not destroyed;
- accessible;
- not locked by active mission state;
- backpack capacity permits return.

Retrieval transfers the same object back to inventory.

## 29. Gadget Destruction

A destroyed deployed gadget is resolved as actual item damage/loss according to its item definition.

Routine player defeat does not duplicate a destroyed gadget.

## 30. Tool Interaction Feedback

The HUD/tool presentation communicates:

- valid target;
- incompatible target;
- energy;
- heat;
- condition;
- mode;
- progress;
- missing material/tool requirement.

## 31. Tool Switching

Switching away from a tool interrupts any sustained action that requires it.

Progress retention follows the target interaction rule.

## 32. Tool Crafting

Tool/gadget recipes are registered through GDS-4 and use canonical materials/components.

A future tool model cannot invent an undefined resource without Resource Catalog update.

## 33. Edge Cases

If Suit Energy reaches the protected life-support reserve threshold, connected tools lose access according to Equipment priority.

If a Mining Cutter overheats during extraction, completed mining output remains valid and extraction pauses.

If a deployed gadget loses communication, it continues only local autonomous behavior explicitly defined by that gadget.

If the player lacks inventory space to retrieve a reusable gadget, it remains deployed.

## 34. Explicit Non-Goals

The baseline does not provide:

- one magic tool that performs every specialist action;
- tool operation without energy/resources where required;
- omniscient scanning;
- station-scale output from a portable gadget;
- random tool rarity tiers as core progression.

## 35. Tuneable Parameters

Tuneable values include tool range, extraction/repair speed, energy capacity, heat generation, cooldown, gadget capacity, sensor range, and deployment time.

## 36. Dependencies

This specification depends on Interaction, Equipment, Inventory, Movement, GDS-4 Mining/Salvage/Resources, Station Damage/Power, Missions, Combat, and Research.

## 37. Open Questions

None in the portable tool/gadget framework.

Specific model statistics are tuneable content data.
