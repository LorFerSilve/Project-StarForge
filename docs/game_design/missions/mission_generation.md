# Mission Generation

> **Status:** Design Complete  
> **Authority:** Procedural mission creation, deterministic seeds, content-selection constraints, validation, authored/procedural boundaries, offer stability, and anti-reroll behavior

## 1. Purpose

Procedural generation creates replayable mission variety without breaking world canon or producing impossible combinations.

## 2. Generation Layers

Mission content can be:

- Fully Authored;
- Authored Framework + Procedural Layout/Encounters;
- Procedural Contract using authored archetypes.

Main-story geography and required beats remain authored.

## 3. Mission Generation Inputs

A procedural mission is generated from:

- Mission Source;
- Strategic Location;
- sector/threat band;
- controlling faction;
- allowed Mission Types;
- location template;
- resource profile;
- hazard profile;
- available objective templates;
- current persistent world state;
- generation seed.

## 4. Stable Seed

At mission creation, one deterministic seed is assigned.

That seed remains attached to the Mission ID.

Reloading does not reroll the mission.

## 5. Stable Major Content

The seed determines or references stable:

- zone layout;
- major POIs;
- deposit placement;
- salvage targets;
- survivor identity;
- objective placement;
- major reward-bearing containers;
- hazard schedule;
- encounter composition seed.

Dynamic AI behavior can still vary through play.

## 6. Story Mission Generation

Story missions can procedurally vary:

- noncritical encounter placement;
- minor loot;
- selected geometry modules;
- ambient events.

They cannot randomize away:

- required location;
- mandatory character;
- story evidence;
- required objective;
- narrative outcome choice.

## 7. World Validation

Generated content must fit:

- biome;
- atmosphere;
- gravity;
- faction territory;
- resource plausibility;
- site history;
- Reach band.

## 8. Resource Validation

The generator only uses canonical GDS-4 Resource IDs.

Resource deposits obey the location's resource profile.

## 9. Objective Validation

Before a mission becomes Available, the generator verifies:

- all Primary Objectives can be completed;
- required targets exist exactly once where uniqueness matters;
- dependencies form a valid directed graph;
- extraction path can become valid;
- no objective contradiction exists.

## 10. Access Validation

The generator checks physical/logical feasibility such as:

- required doors have a possible access solution;
- required repair target is reachable;
- required survivor can reach/enter extraction path;
- required cargo fits at least one allowed mission transport class;
- objective is not spawned outside navigable zone.

## 11. Capability Requirements

A mission can require capabilities such as:

- Reach Class;
- landing;
- docking;
- pressure suit;
- mining hardness class;
- sample containment;
- passenger capacity.

Hard requirements are visible before deployment when known.

## 12. Difficulty Independence

The generator does not scale every mission to the player's equipment.

It selects from the location/faction threat envelope.

A mission can therefore be:

- below;
- near;
- above

current player capability.

## 13. Reward Correlation

Reward profile can consider:

- location scarcity;
- objective complexity;
- threat;
- travel cost;
- faction importance;
- uniqueness.

GDS-12 owns final economic balancing.

## 14. Procedural Survivors

When a procedural mission generates a survivor:

- identity is generated once;
- Persistent Character ID is created;
- profession/traits/background obey GDS-3;
- reload cannot reroll the person.

## 15. Unique World Content

Unique story evidence, Ancient Structures, named characters, and one-time faction locations cannot be duplicated through procedural generation.

## 16. Duplicate Protection

The generator checks persistent world flags before placing:

- unique Blueprint;
- unique Research Evidence;
- named survivor;
- story archive;
- route unlock.

## 17. Mission Offer Stability

Once a procedural mission becomes Available, its content is stable.

Declining it does not immediately regenerate an alternate version of the same opportunity for reroll farming.

## 18. Offer Replacement

Replacement of unaccepted procedural offers is governed by explicit Simulation-Time/world-state refresh rules later finalized in GDS-12.

No real-world clock is used.

## 19. Regeneration After Resolution

A later procedural mission can reuse the same general Strategic Location if the world allows repeatable content.

It receives:

- new Mission ID;
- new seed;
- valid resolved-world context.

Unique persistent changes remain respected.

## 20. Procedural Geometry

Procedural zone assembly uses authored modules/rules.

It must guarantee:

- connectivity;
- objective reachability;
- extraction reachability;
- no impossible overlap;
- hazard consistency;
- faction/architecture compatibility.

## 21. Content Budget

A mission template defines expected content budgets such as:

- POI count;
- encounter count;
- objective count;
- deposit/salvage density;
- traversal complexity.

Values are tuneable by mission/sector.

## 22. Randomness Visibility

The player does not need to see the numeric seed.

Outcomes should feel varied but causally plausible.

## 23. Save/Load

Generation results are saved or deterministically reconstructable from:

- Mission ID;
- seed;
- persistent state.

Save/load must never produce a different major mission.

## 24. Generation Failure

If validation fails:

- the invalid candidate is discarded before presentation;
- generator retries with bounded attempts;
- if no valid mission can be built, no broken offer is shown.

The game does not present an impossible mission.

## 25. Explicit Non-Goals

Generation does not provide:

- random main-story geography;
- save-scum rerolls;
- undefined new resources;
- impossible objective combinations;
- universal player-level scaling;
- duplicated unique characters/items.

## 26. Tuneable Parameters

Tuneable values include offer count, generation attempts, POI budgets, encounter budgets, reward weighting, objective complexity, and procedural variation.

## 27. Dependencies

This specification depends on World, Mission Types, Zones, Objectives, Threat, Resources, Crew, Narrative, Combat, Robots, and GDS-12 economy/dynamic events.

## 28. Open Questions

None in the procedural-generation framework.
