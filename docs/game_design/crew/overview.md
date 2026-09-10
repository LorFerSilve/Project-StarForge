# Crew Overview

> **Status:** Design Complete  
> **Authority:** Persistent recruited crew identity, role in station operation, global crew invariants, and crew-system boundaries

## 1. Purpose

Crew turns survivors found in the galaxy into persistent people who make the home station more capable, more automated, and more alive.

Crew members are not passive cards that merely add hidden stat bonuses.

They physically inhabit the station and provide expertise, labor, supervision, and automation capability.

## 2. Core Fantasy

The intended progression is:

Rescue or meet survivors → recruit suitable people → provide a sustainable place to live → assign meaningful work → improve their expertise → automate more of the station → build attachment to the people who made the station function.

## 3. Survivor vs Crew

A Survivor is a non-hostile character encountered outside the player's established crew structure.

A Crew Member is a survivor or other eligible character who has completed recruitment and permanently joined the home station.

Rescue does not automatically equal recruitment.

## 4. Persistent Identity

Every crew member has persistent identity data including:

- name;
- appearance identity;
- background;
- profession profile;
- skills;
- traits;
- current assignment;
- current availability;
- injury/recovery state;
- progression state;
- relevant narrative flags.

Crew members are not regenerated into different characters after save/load.

## 5. No Loot-Card Rarity System

Crew members do not use generic Common, Rare, Epic, Legendary card tiers as their primary quality model.

A crew member's value comes from:

- profession;
- skill levels;
- trait combination;
- personal background;
- special knowledge;
- mission or faction context;
- progression potential.

Some specialists can be difficult to find without becoming colored loot objects.

## 6. One Persistent Roster

The player maintains one persistent crew roster associated with the primary home station.

There is no separate abstract survivor inventory disconnected from station population.

## 7. No Arbitrary Crew Cap

The baseline does not use an arbitrary account-level maximum crew number as the primary limit.

Sustainable crew size is constrained by actual station support such as:

- habitation;
- food;
- fresh water;
- atmosphere;
- thermal safety;
- medical support.

A technical safety cap may exist in implementation but is not a progression fantasy or hidden gameplay rule.

## 8. Crew Are Not Disposable Combat Troops

Standard repeatable combat and raids use robots as the player's deployable tactical force.

Crew are not trained, consumed, and replaced like disposable Clash of Clans units.

Security crew can defend the station and authored missions can involve crew directly, but permanent crew is not the normal expendable army.

## 9. Standard Expedition Presence

In the baseline, recruited crew does not accompany the player as a standard humanoid combat squad on ordinary repeatable expeditions.

Crew can support expeditions through:

- mission preparation;
- research;
- logistics;
- intelligence;
- navigation;
- equipment servicing;
- remote support where communication permits.

Authored story missions can temporarily include named crew as NPC participants.

## 10. Crew Value

Crew provides value through:

- unlocking automation capability;
- increasing automation capacity;
- reducing response delay;
- diagnosing faults;
- operating specialized facilities;
- supervising robots;
- managing station domains;
- enabling advanced research or logistics;
- protecting the station internally.

Crew never creates missing physical resources by skill alone.

## 11. Station Presence

When the relevant area is loaded, crew should exist physically in believable station locations.

Crew may be found:

- at assigned workplaces;
- moving between relevant station areas;
- in crew quarters;
- in medical recovery;
- in safe muster areas during emergencies.

Detailed presence behavior is defined separately.

## 12. Crew Needs Scope

Crew requires station-level support but does not use a survival-game micro-management model.

The baseline tracks station capability for:

- food;
- water;
- breathable atmosphere;
- safe temperature;
- habitation;
- medical care.

The player does not manually feed or give water to each crew member.

## 13. No Full Social Simulation

The baseline does not require:

- individual relationship graphs;
- romance simulation;
- complex morale bars;
- jealousy systems;
- minute-by-minute sleep schedules.

Narrative dialogue and character stories may still give crew personality.

## 14. Work Model

Crew work is task and assignment based.

A crew member has:

- one primary active assignment;
- professional capabilities;
- finite workload capacity;
- current availability.

The station cannot treat one skilled person as infinitely scalable automation.

## 15. Cross-Training

A crew member can possess skills outside their primary profession.

Cross-training allows emergency substitution or career development.

It does not let one person perform several simultaneous full-capacity assignments.

## 16. Permanent Death Rule

Routine gameplay does not permanently kill recruited crew.

Crew can become:

- Injured;
- Critically Injured;
- Incapacitated;
- Recovering;
- Unavailable.

Permanent death is reserved for an explicitly authored narrative case or a later accepted global design change.

## 17. Player Responsibility

The player chooses:

- who to recruit;
- where to assign crew;
- which specialists receive training;
- how much infrastructure supports population;
- what tasks and emergency permissions crew can automate.

## 18. Crew Autonomy

Crew may execute routine tasks within assignment and policy.

They do not independently:

- spend protected strategic resources;
- abandon the station;
- change diplomacy;
- initiate raids;
- choose major construction;
- override deliberate player lockouts.

## 19. Crew and Automation

Crew is one of several automation layers.

A task may require:

- expertise from crew;
- physical execution by a robot;
- machinery;
- logistics;
- power;
- resources.

The crew member provides knowledge or supervision, not magical completion.

## 20. Progression Role

Crew progression should reduce fragility and repetitive station work.

Early station:
- player handles many tasks manually.

Developed station:
- specialists coordinate domains.

Late station:
- specialists supervise distributed automation and exceptional problems.

## 21. Persistence

Crew roster, identity, skills, traits, assignments, health state, and progression persist through save/load.

## 22. Explicit Non-Goals

The crew baseline does not provide:

- gacha recruitment;
- disposable human army units;
- arbitrary colored rarity tiers;
- routine permanent death;
- full Sims-style social simulation;
- manual individual feeding;
- unlimited productivity from one specialist.

## 23. Dependencies

Crew depends heavily on station infrastructure, missions, resources, narrative, automation, and progression.

## 24. Open Questions

None at overview level.

Detailed behavior is owned by the following crew specifications.
