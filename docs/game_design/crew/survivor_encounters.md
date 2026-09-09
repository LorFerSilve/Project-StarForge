# Survivor Encounters

> **Status:** Draft  
> **Authority:** How recruitable and non-recruitable survivors appear, are discovered, rescued, evaluated, and persist before recruitment

## 1. Purpose

Survivor encounters connect exploration directly to station population and automation progression.

Finding a person can become as meaningful as finding a rare component.

## 2. Encounter Sources

Survivors can appear through:

- rescue missions;
- abandoned colonies;
- derelict ships;
- damaged stations;
- crash sites;
- hostile captivity;
- independent settlements;
- distress signals;
- story missions;
- faction interactions.

## 3. Encounter Classes

A discovered survivor can be:

- Rescue-Only NPC;
- Potential Recruit;
- Specialist Recruit;
- Narrative Character;
- Temporary Passenger.

These classes describe recruitment behavior, not combat power.

## 4. Discovery

A survivor becomes known when the player:

- physically discovers them;
- receives valid mission intelligence;
- detects a distress signal;
- is introduced through a faction/story event.

Undiscovered survivors do not appear in a global recruit menu.

## 5. Rescue State

Some survivors must first be brought to safety.

Rescue can require:

- clearing enemies;
- restoring atmosphere;
- repairing transport;
- escorting;
- medical stabilization;
- providing supplies.

## 6. Persistence During Mission

Once a survivor is materially part of an active mission state, their state persists through that mission according to mission rules.

Reload/save behavior cannot duplicate the character.

## 7. Non-Combatant Baseline

Most ordinary survivors are not intended to act as strong combat allies.

Their value is primarily what they become after rescue or recruitment.

## 8. Specialist Information

Before recruitment, the player can learn some or all of:

- current profession;
- known skills;
- background;
- visible traits;
- faction affiliation;
- special conditions.

Information can be incomplete until dialogue, rescue, or evaluation occurs.

## 9. Rescue Failure

Mission failure can cause a survivor opportunity to be lost for that mission.

A unique narrative survivor is handled by authored narrative rules rather than generic procedural deletion.

## 10. Temporary Passenger

A rescued person can travel as a temporary passenger without immediately becoming crew.

Temporary passengers require compatible transport capacity.

They are not allowed to perform permanent station assignments.

## 11. Station Arrival

A rescued survivor can be brought to the home station if:

- transport succeeds;
- station arrival is possible;
- emergency reception capacity exists.

Recruitment is then a separate decision.

## 12. Capacity Safety

The game should not force the player to abandon a rescued person solely because permanent habitation is full.

The station can support limited temporary emergency guest capacity through explicitly defined emergency facilities.

This capacity is not a permanent substitute for habitation expansion.

## 13. Recruitment Opportunity Persistence

A potential recruit at the station remains available for a reasonable authored period or until a clearly communicated condition changes.

The game should not hide sudden expiration of a recruitment opportunity.

## 14. Procedural Survivors

Procedural survivor generation can vary:

- name;
- appearance;
- profession;
- skills;
- traits;
- background hooks.

Generation must produce internally valid combinations.

## 15. Authored Survivors

Important characters can use fixed:

- identity;
- skills;
- profession;
- story;
- recruitment conditions.

Authored characters use the same core crew mechanics after recruitment unless explicitly specified.

## 16. No Reroll Exploit Requirement

The design does not require repeatedly reloading a mission to reroll survivor quality.

Procedural generation should bind to a stable mission/world seed or persistent encounter identity where practical.

## 17. UI

Survivor encounter UI communicates:

- name/identity if known;
- condition;
- rescue objective;
- recruitment potential if known;
- profession information discovered;
- transport requirement.

## 18. Edge Cases

If the player rescues a survivor but inventory/habitation is full, temporary-passenger rules apply where capacity exists.

If transport is destroyed, survivor state follows mission failure/rescue logic rather than teleporting to the station.

If the same persistent encounter is revisited, it does not generate a duplicate survivor.

## 19. Explicit Non-Goals

Survivors do not spawn as abstract cards directly into station inventory.

They are encountered through world or mission context.

## 20. Dependencies

This document depends on missions, spacecraft/passenger capacity, recruitment, crew health, narrative, and world generation.

## 21. Open Questions

None in the current baseline.
