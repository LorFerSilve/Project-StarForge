# Combat System

> **Status:** Draft  
> **Authority:** Global direct-combat loop, combat states, valid combat participants, engagement rules, attack-resolution pipeline, combat resource principles, and cross-domain authority boundaries

## 1. Purpose

Combat provides direct first-person and spacecraft conflict while preserving StarForge's systemic identity.

Combat must reward:

- aim;
- positioning;
- cover;
- movement;
- target prioritization;
- preparation;
- ammunition management;
- shield/armor understanding;
- subsystem targeting;
- disengagement when appropriate.

## 2. Combat Contexts

Baseline contexts:

1. On-Foot Combat;
2. EVA Combat;
3. Spacecraft Combat;
4. Station/Facility Defense Combat;
5. Boarding Combat.

The same damage framework is reused where physically appropriate.

## 3. Direct Participation

The player directly participates in core combat.

Combat is not reduced to a menu-only power comparison.

## 4. Combat Participants

A valid Combat Actor can be:

- player;
- hostile human;
- hostile machine/robot;
- wildlife/biological entity;
- spacecraft;
- turret;
- destructible combat-relevant system/object.

Crew/robots may also participate where their owning domains permit it.

## 5. Combat State

An actor can conceptually be:

- Unaware;
- Alerted;
- Engaged;
- Disengaging;
- Disabled/Incapacitated;
- Defeated/Destroyed.

Enemy AI owns exact behavior transitions.

## 6. Attack Resolution Pipeline

Every damaging attack follows this canonical logical pipeline:

1. Validate attack can fire/execute.
2. Consume required ammunition/energy/use-state.
3. Resolve shot/projectile/melee trajectory.
4. Resolve collision/hit target.
5. Identify hit location/weak point.
6. Construct Damage Packet.
7. Resolve shield interaction.
8. Resolve armor/protection interaction.
9. Apply remaining damage to biological/system/structure target.
10. Apply eligible status payloads.
11. Apply impulse/stagger where defined.
12. Notify target/objective/AI systems.
13. Commit weapon cooldown/recoil/recovery state.

No later stage can retroactively refund an already-fired round because the shot missed.

## 7. Deterministic Critical Hits

The baseline has **no random critical-hit chance**.

Critical damage occurs because the player hits an authored:

- head;
- weak point;
- exposed component;
- vulnerable subsystem.

This makes critical performance skill/information based.

## 8. Damage Packet

A Damage Packet can contain one or more channels:

- Kinetic;
- Thermal;
- Explosive;
- Electrical/EMP;
- Corrosive.

A weapon can combine channels.

Example:
- plasma impact = Thermal + Kinetic-like impulse;
- explosive shell = Explosive + Kinetic fragments where authored.

## 9. Environmental Damage

Vacuum, heat, cold, radiation, contamination, and similar environmental hazards remain owned by survival/environment systems.

Combat uses the same Health/System consequence interfaces but does not redefine environmental exposure.

## 10. Friendly Fire

Baseline single-player rules:

- player attacks can damage allied combat-capable actors if the attack physically hits them;
- ordinary interaction safety may prevent firing in explicitly noncombat social safe zones;
- recruited persistent human crew are not given magical combat immunity during authored combat participation.

Faction/reputation consequences depend on knowledge/witness rules.

Exact allied-AI risk handling can be tuned later.

## 11. Safe-Zone Weapon State

Certain social/faction interiors can require weapons lowered/holstered by access rule.

This is an authored location/security rule.

The player is warned before violating a weapon restriction.

## 12. No Universal Combat Lock

Entering combat does not globally disable:

- looting;
- repair;
- interaction;
- extraction.

Individual actions can be blocked/interrupted because they are unsafe, time-consuming, or target-specific.

## 13. Combat Resources

Combat can consume:

- ammunition;
- weapon energy cells;
- suit energy;
- shield capacity;
- weapon heat capacity;
- weapon condition where wear is defined;
- consumables.

No combat resource regenerates from nothing unless its equipment explicitly has a powered recharge process.

## 14. Weapon Switching

Weapon switching is an action with finite time.

The player can carry:

- Primary A;
- Primary B;
- Sidearm

according to GDS-5 Equipment.

## 15. Reloading

Reloading is finite and interruptible according to Ammunition/Weapons rules.

The baseline does not grant infinite reserve ammo.

## 16. Aiming

On-foot ranged weapons support:

- Hip Fire;
- Aim Down Sights / Precision Aim

when their model supports it.

ADS improves aiming precision/visual alignment but does not magically increase raw damage unless a specific weapon mode defines it.

## 17. Recoil

Weapons can generate:

- view recoil;
- weapon displacement;
- recovery.

Recoil is deterministic around authored patterns plus bounded dispersion mechanics.

## 18. Accuracy

Accuracy is driven by:

- weapon base dispersion;
- movement;
- stance;
- ADS state;
- sustained fire/bloom where applicable;
- condition;
- status effects.

There is no hidden RPG accuracy roll after a ray/projectile physically hits.

## 19. Cover

Cover is physical world geometry.

If a shot is blocked by valid cover, it hits the cover.

Penetration through cover occurs only when the weapon/material rules explicitly support it.

## 20. Suppression

The baseline does not require an abstract player suppression meter.

Enemy AI can react to incoming fire and dangerous lines of fire through perception/behavior rules.

## 21. Melee

Melee uses physical range, swing/attack timing, collision, stamina-free action rules, and damage packets.

No universal regenerating stamina bar is introduced.

## 22. Grenades / Explosives

Explosives use explicit projectile/placement and blast rules.

Explosive damage is not guaranteed through solid cover.

## 23. Disable vs Kill

Combat supports distinct outcomes:

- Biological Incapacitation;
- Machine Disable;
- Structural/System Disable;
- Destruction.

Objectives must state which outcome is required.

## 24. Surrender

Human enemies may surrender only when Enemy Behavior/content explicitly supports it.

Surrender is not required for every encounter.

## 25. Combat End

Combat ends systemically when:

- hostile actors are no longer actively engaging or capable of engagement;
- the player disengages/escapes;
- mission state transitions.

There is no invisible universal "combat timer" controlling all mechanics.

## 26. Persistence

Gameplay-relevant combat state can persist through allowed saves:

- health;
- shields;
- armor/equipment condition;
- ammunition;
- weapon heat;
- status effects;
- enemy state;
- destroyed/disabled objects.

## 27. Player Defeat

At zero biological Health, GDS-5 owns Incapacitated state.

GDS-8 owns mission failure/recovery transaction.

Combat does not directly teleport the player home.

## 28. Spacecraft Defeat

GDS-6 owns Disabled/Derelict/Persistent Ship states.

Combat provides damage resolution only.

## 29. Station Damage

Station modules/systems receive combat results through GDS-2 damage/infrastructure rules.

Combat does not replace station power/atmosphere/thermal simulation.

## 30. Robots

GDS-10 owns player robot classes, squad commands, repair, manufacturing, and robot-specific persistence.

GDS-9 owns generic combat damage and enemy engagement interactions applied to robot bodies.

## 31. Raids

GDS-11 owns raid flow/intelligence/escalation.

GDS-9 owns direct fighting during raid phases.

## 32. Combat Readability

The player must be able to learn:

- shield hit/collapse;
- armor effectiveness;
- weak-point hit;
- immunity/resistance;
- status application;
- source/direction of major damage where perceptible.

Presentation is GDS-13 authority.

## 33. Explicit Non-Goals

Combat does not use:

- random critical-hit chance as baseline;
- universal RPG hit rolls;
- unlimited ammunition;
- global combat lock on interaction/extraction;
- arbitrary friendly-fire immunity;
- one universal combat-power number;
- full environment destruction.

## 34. Tuneable Parameters

Damage values, recoil, spread, reload time, fire rate, heat, impulse, stagger thresholds, and AI timing are tuneable.

The logical resolution rules are fixed.

## 35. Dependencies

This specification depends on Player, Equipment, Health, Missions, Resources, Spacecraft, Station Damage, Crew, future Robots/Raids, and Presentation.

## 36. Open Questions

None in the global combat-loop baseline.
