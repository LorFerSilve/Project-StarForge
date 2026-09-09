# Blueprints

> **Status:** Draft  
> **Authority:** Design-knowledge ownership, blueprint unlock states, acquisition, reconstruction, prerequisites, persistence, and relationship between blueprints and recipes/construction

## 1. Purpose

Blueprints represent the concrete engineering design knowledge required to manufacture or construct a specific object.

Resources answer:

> "Do I have the matter?"

Research answers:

> "Do I understand the underlying technology?"

Blueprints answer:

> "Do I possess the actual design for this object?"

## 2. Blueprint as Knowledge Asset

A Blueprint Unlock is a non-physical persistent Knowledge Asset.

Once securely acquired:

- it does not occupy cargo space;
- it is not consumed by crafting;
- it is not lost when a manufactured object is destroyed;
- it persists across save/load.

## 3. Blueprint Identity

Every blueprint has a stable Blueprint ID.

A blueprint definition contains:

- Blueprint ID;
- display name;
- output/buildable design;
- prerequisite technologies;
- source categories;
- whether reconstruction from fragments is allowed;
- required reconstruction segments if applicable;
- associated crafting/construction recipe IDs.

## 4. Blueprint States

A blueprint can be:

- Unknown;
- Identified;
- Incomplete;
- Locked by Technology;
- Unlocked.

### Unknown

The player has no knowledge that the design exists.

### Identified

The player knows the design exists and can inspect known acquisition/prerequisite information.

### Incomplete

The player has acquired only part of a blueprint that explicitly supports reconstruction.

### Locked by Technology

The complete design is known/owned, but required underlying technology has not yet been researched.

### Unlocked

The player owns the complete design and all hard technology prerequisites are satisfied.

## 5. Acquisition Sources

Blueprints can be obtained through:

- starting technology;
- research completion;
- recovered data carriers;
- salvage of intact technical archives;
- faction reward;
- trade/service where economy later permits;
- boss/raid objective;
- authored narrative progression.

## 6. No Random Blueprint Rarity Tier

Blueprints do not have Common/Rare/Epic/Legendary quality versions.

A blueprint is valuable because:

- the design is powerful;
- its source is difficult;
- prerequisites are advanced;
- its acquisition is rare.

## 7. Complete Blueprint Acquisition

If a complete blueprint is successfully secured:

1. physical carrier is validated if one exists;
2. blueprint identity is recorded;
3. the Blueprint Unlock becomes persistent;
4. all associated recipes/build entries become eligible subject to technology prerequisites.

## 8. Fragmented Blueprints

Only blueprints explicitly marked **Fragment-Reconstructable** use fragments.

A fragmented blueprint defines a fixed set of logical segments, for example:

- Segment A;
- Segment B;
- Segment C.

The player must acquire every required unique segment.

Repeated acquisition of an already-owned identical segment does not advance reconstruction again.

## 9. Fragment Identity

Blueprint fragments use persistent segment identity, not a generic count of "blueprint shards."

Example:

`BP_FUSION_REACTOR_MK2_SEG_POWER`

This prevents blind grind through interchangeable fragments.

## 10. Duplicate Fragment Handling

A duplicate segment does not create blueprint progress.

If acquired as a physical item, it may later:

- be sold;
- be salvaged as a data carrier;
- provide tagged Research Evidence if analysis rules explicitly allow it.

It never counts twice toward the same missing segment.

## 11. Blueprint Reconstruction

When all required unique segments are secured:

- reconstruction becomes available;
- an eligible analysis/research facility combines the design information;
- reconstruction consumes active-game analysis time;
- the resulting complete Blueprint Unlock is permanent.

Knowledge fragments are not destroyed from history; they are marked integrated.

## 12. Technology Prerequisites

A complete blueprint can remain Locked by Technology.

Example:

The player recovers an advanced shield-emitter blueprint before researching Superconductive Materials.

The player owns the design but cannot manufacture it yet.

This prevents lore-breaking crafting from knowledge the station cannot technically support.

## 13. Research-Generated Blueprints

Some research projects directly produce a new blueprint.

Example:

Research Project: Basic Shield Harmonics  
Output: Blueprint — Basic Shield Emitter

The research project owns the unlock event; the blueprint system records permanent design ownership.

## 14. Blueprint vs Recipe

A Blueprint Unlock and Recipe are related but distinct.

- Blueprint = permission/design knowledge.
- Recipe = exact physical transformation/manufacturing definition.

One blueprint can unlock multiple closely related recipes only when explicitly defined.

## 15. Blueprint vs Technology

Technology can unlock a general capability without granting every object design.

Example:

Researching Fusion Engineering does not automatically grant every possible fusion reactor blueprint.

## 16. Reverse Engineering

A compatible recovered finished object can be analyzed for its blueprint only if that object definition allows Reverse Engineering.

Reverse engineering may require:

- intact object condition;
- relevant Scientist/Engineer skill;
- research facility;
- prerequisite evidence;
- active-game analysis time.

It is not universally possible for every item.

## 17. Destructive Reverse Engineering

Some reverse-engineering procedures consume or destroy the physical object.

The UI must state this before the player confirms analysis.

The blueprint is granted only after successful completion.

## 18. Blueprint Security

Once stored as station knowledge, normal station storage loss does not delete the blueprint.

Knowledge persistence is part of the player's long-term progression.

A narrative event cannot silently wipe blueprints unless such irreversible loss is explicitly designed and approved.

## 19. Blueprint Sharing Across Station Systems

Unlocked blueprint state is globally readable by:

- construction catalog;
- crafting;
- manufacturing;
- ship construction;
- robot manufacturing;
- research UI.

The same blueprint is not separately unlocked in each machine.

## 20. Variant Designs

A mechanically distinct variant requires its own blueprint when its design materially changes:

- function;
- module class;
- technology;
- strategic role.

Pure cosmetic skins do not require new gameplay blueprints.

## 21. Upgrade Designs

An upgrade can be:

- component modification;
- in-place retrofit;
- replacement design.

Its blueprint explicitly defines which method applies.

The player cannot infer an upgrade path solely from a higher Mk number.

## 22. Starter Blueprint Set

The starting station/player has a small baseline set sufficient for recovery and early progression, including designs for:

- Structural Frame;
- Pressure Seal Assembly;
- Basic Circuit;
- Magnetic Coil;
- basic storage;
- basic corridor/structural module;
- basic power distribution;
- primitive fabricator;
- basic life-support support;
- basic repair consumables.

The exact finished station-module catalog is owned by future station content definitions.

## 23. Discovery UI

Blueprint UI shows:

- state;
- known source;
- required technology;
- missing unique fragments;
- associated outputs;
- compatible machine/facility;
- acquisition history where useful.

## 24. Notifications

A newly acquired blueprint is a high-value progression event and gets clear feedback.

Duplicate known data does not mimic a new unlock.

## 25. Persistence

Blueprint state, fragments, prerequisite state, reconstruction progress, and acquisition provenance persist.

Save/load cannot duplicate fragments or relock legitimately acquired knowledge.

## 26. Edge Cases

If all fragments are owned but research facility is unavailable, the blueprint remains Incomplete/Reconstruction Ready until analysis can occur.

If prerequisite technology completes while the complete blueprint is owned, state changes from Locked by Technology to Unlocked.

If technology is later rebalanced/renamed, migration must preserve legitimate blueprint ownership.

If the physical carrier is lost before secure acquisition, the blueprint is not granted.

## 27. Explicit Non-Goals

Blueprints do not use:

- consumable one-use recipes;
- random rarity colors;
- generic interchangeable blueprint shards;
- material ownership as a substitute for design knowledge;
- machine-specific duplicate unlocks.

## 28. Tuneable Parameters

Tuneable values include reconstruction duration and the number of unique segments for a fragment-reconstructable design.

## 29. Dependencies

This specification depends on Resource Model, Crafting, Research, gathering/salvage, missions, factions/economy, and future equipment/ship/robot/station content.

## 30. Open Questions

None in the blueprint-system baseline.

Specific future designs will register their Blueprint IDs as their domains are specified.
