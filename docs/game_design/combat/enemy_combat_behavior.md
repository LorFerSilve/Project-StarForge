# Enemy Combat Behavior

> **Status:** Design Complete  
> **Authority:** Generic hostile combat AI perception, alert states, target selection, movement/cover, firing discipline, communication, search, retreat, surrender boundary, and fairness/readability constraints

## 1. Purpose

Enemy combat AI should feel purposeful and faction/actor-specific without cheating with omniscient knowledge.

## 2. Authority Boundary

This document defines generic combat behavior.

Specific:
- robot classes belong to GDS-10;
- raid coordination belongs to GDS-11;
- faction identity belongs to GDS-7;
- weapon mechanics belong to GDS-9 Weapons.

## 3. Perception Channels

Baseline perception:

- Vision;
- Hearing;
- Sensor/Data Awareness;
- Shared Alert Information.

## 4. No Omniscience

Enemies do not know the player's current position merely because the player exists in the mission.

They require:
- direct perception;
- communicated information;
- sensor detection;
- scripted knowledge justified by mission context.

## 5. Vision

Vision evaluates:
- field of view;
- distance;
- line of sight;
- visibility/environment;
- target signature.

## 6. Hearing

Hearing can detect:
- gunshots;
- explosions;
- footsteps/machinery where relevant;
- alarms.

Suppressed weapons reduce but do not eliminate acoustic signature.

## 7. Sensor Awareness

Machines/ships/advanced enemies can use sensors consistent with their equipment.

Sensor detection cannot see through arbitrary occlusion unless the sensor type explicitly supports it.

## 8. Shared Alert

Allies can communicate:
- target seen;
- last known position;
- alarm;
- threat type.

Communication requires:
- local voice/radio;
- network;
- faction system

as appropriate.

Destroyed/jammed communication can reduce information sharing where GDS-11/Systems support it.

## 9. Awareness States

Canonical generic states:

- Unaware;
- Suspicious;
- Alert;
- Engaged;
- Searching;
- Retreating;
- Disabled/Defeated.

## 10. Unaware

No actionable hostile contact.

Actor follows patrol/work behavior.

## 11. Suspicious

Actor has incomplete evidence:
- sound;
- partial sighting;
- unexplained system event.

It investigates without perfect player location.

## 12. Alert

Actor knows a credible threat exists but may not have current line of sight.

It:
- prepares weapon;
- seeks advantageous position;
- communicates where possible.

## 13. Engaged

Actor has sufficient target information to attack/tactically maneuver.

## 14. Searching

After losing contact, actor searches around:
- last known position;
- likely routes;
- nearby cover.

Search expands/degrades over time.

The actor does not continuously track invisible player movement.

## 15. Retreating

An actor can retreat when:
- morale/doctrine rule permits;
- severely damaged;
- objective requires fallback;
- leader orders withdrawal.

Not every enemy fights to death.

## 16. Target Selection

Target score can consider:

- current threat;
- distance;
- visibility;
- recent damage source;
- mission objective relevance;
- target vulnerability;
- faction doctrine.

The AI cannot target information it does not possess.

## 17. Target Switching

AI can switch target when another valid target becomes more important.

It should avoid implausible frame-by-frame oscillation through hysteresis/cooldown.

## 18. Cover Evaluation

Humanoid ranged AI can evaluate authored/navigation-supported cover points for:

- line of fire;
- protection;
- distance;
- flank exposure;
- grenade risk.

## 19. No Cover Teleport

Enemies physically navigate to cover.

## 20. Peeking / Firing

Cover-using AI alternates between:
- protected state;
- aiming;
- firing exposure;
- repositioning.

Exact timings are tuneable.

## 21. Movement

Combat movement can include:
- advance;
- retreat;
- strafe;
- flank;
- take cover;
- reposition;
- pursue within mission leash/objective.

## 22. No Universal Rush

AI role determines preferred distance.

Examples:
- shotgun unit closes distance;
- marksman maintains range;
- support unit seeks protected line.

## 23. Firing Discipline

AI respects:
- weapon fire rate;
- reload;
- ammunition;
- line of fire;
- friendly obstruction;
- accuracy/dispersion.

AI shots use the same hit/damage mechanics as player weapons unless a specific non-player weapon definition differs explicitly.

## 24. AI Accuracy

Enemy aim has modeled:
- reaction delay;
- aim error;
- tracking capability;
- movement effect;
- weapon recoil/spread.

It does not use guaranteed hits.

## 25. Reaction Time

Newly detected targets are not shot with zero reaction delay unless the actor was already aiming/expecting that exact attack lane.

## 26. Friendly Fire Avoidance

AI should avoid firing when an ally blocks the line.

If it fires and hits an ally, normal damage applies.

## 27. Grenade Use

AI grenade use checks:
- range;
- target position confidence;
- ally safety;
- available grenade;
- throw path.

It does not place grenades magically at player position.

## 28. Blind Fire

Blind fire is not a baseline requirement.

## 29. Suppressive Fire

AI can fire at/near a last-known position to deny movement if doctrine/ammunition permits.

This consumes real ammunition.

## 30. Reload Behavior

AI reloads according to actual weapon state.

It can seek cover before reload.

## 31. Ammunition Exhaustion

Finite-ammo AI can:
- switch weapon;
- seek resupply;
- retreat;
- use melee;
- become less effective.

The baseline can use actor ammo counts rather than full enemy inventory.

## 32. Flanking

AI can flank only through valid navigable routes.

It cannot spawn behind the player merely to simulate a flank.

## 33. Reinforcements

Reinforcements are mission/world actors entering through valid spawn/entry logic.

They do not materialize visibly in cleared rooms without an authored insertion explanation.

## 34. Spawn Fairness

Combat spawn points must avoid:
- direct unavoidable spawn on top of player;
- spawning in currently visible empty space without justified insertion;
- bypassing sealed physical boundaries.

## 35. Search Memory

AI stores Last Known Position and confidence.

Confidence decays while no new evidence arrives.

## 36. Stealth Boundary

GDS-9 does not define a full stealth game.

Perception supports:
- avoiding detection;
- losing contact;
- suppressed weapons;
- sensor disruption.

No separate stealth score is required.

## 37. Alarm Systems

An enemy can trigger a local alarm if:
- access exists;
- behavior/doctrine permits;
- alarm system is operational.

GDS-11/Station Security owns larger raid alarm infrastructure.

## 38. Surrender

Selected human actors can surrender when:
- isolated;
- heavily disadvantaged;
- morale rule permits;
- mission permits prisoners/surrender.

No universal surrender chance is required.

## 39. Wildlife

Wildlife can use simplified:
- perception;
- territory;
- pursuit;
- attack;
- retreat

without cover/firearm behavior.

## 40. Machines

Machines can use:
- sensor-based perception;
- coordinated targeting;
- no biological morale;
- subsystem damage reactions.

Specific Continuance/robot bodies are later content/GDS-10.

## 41. Bosses

Boss AI remains subject to:
- hit rules;
- damage channels;
- telegraphed attacks;
- status resistance.

Bosses cannot rely on unexplained immunity or perfect player tracking.

## 42. Difficulty Boundary

GDS-12 may modify AI parameters such as:
- reaction time;
- aim error;
- aggression;
- coordination.

It may not grant omniscience or break physical combat rules.

## 43. Persistence

Active mission AI state can persist:
- health;
- ammo;
- alert;
- last known position/confidence;
- disabled/dead state;
- objective state.

## 44. Explicit Non-Goals

Enemy AI does not use:

- omniscient player tracking;
- guaranteed shots;
- cover teleport;
- magical flanks;
- visible unjustified spawning;
- infinite grenade spam;
- universal fight-to-death behavior.

## 45. Tuneable Parameters

Vision range/FOV, hearing range, reaction time, aim error, burst length, cover preference, confidence decay, pursuit range, and morale thresholds are tuneable.

## 46. Dependencies

This specification depends on Weapons, Hit Resolution, Damage, Mission Zones, Factions, Status Effects, Robots/Raids, and GDS-12 Difficulty.

## 47. Open Questions

None in the generic enemy combat behavior baseline.
