# Combat Objective Resolution

> **Status:** Design Complete  
> **Authority:** Exact combat-state meanings used by GDS-8 objectives: Kill, Incapacitate, Destroy, Disable, Drive Off, Capture, Protect, Survive, and Suppress

## 1. Purpose

Mission implementation must never guess what a combat objective verb means.

Every combat objective selects one explicit Resolution Type.

## 2. Canonical Resolution Types

Baseline:

- Kill;
- Incapacitate;
- Destroy;
- Disable;
- Drive Off;
- Capture;
- Protect;
- Survive;
- Suppress / Clear Area.

## 3. Kill

Valid for biological actors when the actor reaches an explicit Dead state.

### Baseline Constraint

Routine player and recruited crew do not use ordinary permanent death as their failure state.

Enemy biological actors can use Death where their actor definition supports it.

## 4. Incapacitate

Completes when target reaches Incapacitated state and remains a valid non-active combatant.

For the player, GDS-5 defines this at Health zero.

For other biological actors, actor data defines the equivalent threshold.

## 5. Destroy

Used for machines, objects, ships, turrets, or structures.

Completes only when target enters its explicit Destroyed/nonfunctional destruction state.

Temporary EMP disable does not count as Destroy.

## 6. Disable

Completes when target satisfies the objective's declared disabled condition without requiring destruction.

The Objective must specify one of:

- Generic Combat Disabled;
- Mobility Disabled;
- Weapons Disabled;
- Power Disabled;
- Control Disabled;
- Specific System Disabled.

## 7. Temporary Disable

A temporary EMP/Jam state counts only if the objective explicitly allows **Temporary Disable**.

Otherwise the target must reach a persistent disabled state such as physical system damage/shutdown.

## 8. Drive Off

Completes when the target commits to and reaches a mission-defined retreat/escape state.

Merely causing damage does not count.

## 9. Capture

Capture requires a completed ownership/control objective.

Depending on target this can require:

- surrender;
- boarding;
- command authorization;
- secured crew;
- control-system takeover;
- immobilization.

Combat damage alone cannot complete Capture unless it directly causes an authored surrender state followed by required control transfer.

## 10. Protect

Protect completes when the protected target satisfies its required survival/operational state at the declared end condition.

The Objective must define acceptable end state:

- alive;
- not Incapacitated;
- not Destroyed;
- above minimum Integrity;
- still operational.

## 11. Survive

Survive completes when:

- required Simulation-Time condition/event ends;
- player/protected target has not triggered failure condition.

It is not equivalent to "kill all."

## 12. Suppress / Clear Area

A **Clear Area** objective completes when no hostile actor capable of immediate combat remains inside the defined objective area.

Qualifying enemies can be:
- dead;
- incapacitated;
- disabled;
- surrendered;
- retreated outside area.

The objective must specify whether hidden/unaware hostile actors count.

## 13. Boss Objectives

Bosses use the same Resolution Type semantics.

A boss cannot require a secret unique "defeat" condition not represented in Objective data.

Multi-phase bosses can gate when their final valid resolution state becomes reachable.

## 14. Protected Unique Characters

Narrative unique characters use authored incapacity/retry rules and are not accidentally killed because a generic Kill/Destroy objective targeted the wrong Actor ID.

## 15. Objective Target Matching

Resolution is committed only for the intended:
- Actor ID;
- Actor Group ID;
- System ID;
- Area ID

defined by Objective.

Killing a visually similar enemy does not increment the wrong unique objective.

## 16. Group Objectives

A group objective can require:
- All;
- Count N;
- Percentage;
- Named subset.

The requirement is explicit.

## 17. Reinforcements and Clear Area

A Clear Area objective defines whether:
- currently spawned hostiles only;
- all scheduled wave hostiles;
- objective-area control for a duration

is required.

The mission cannot surprise implementation with an endless unknown spawn condition.

## 18. Surrendered Actors

Surrendered actors are no longer active hostiles while surrender remains valid.

They can satisfy:
- Incapacitate-like neutralization;
- Clear Area

only if Objective permits surrender as valid resolution.

## 19. Fleeing Actors

A fleeing actor counts for Drive Off when retreat commit/state is valid.

It does not count as Kill/Destroy.

## 20. Damage Credit

Objective completion is based on target state, not who dealt the final damage, unless the mission explicitly requires the player personally to perform the action.

Systemic/environmental/allied damage can therefore resolve a normal Destroy objective if the correct target is destroyed.

## 21. Player-Personal Requirement

If an authored mission requires player-personal action, the Objective definition includes:
- Required Instigator = Player.

This should be rare and narratively justified.

## 22. Double Resolution

A target state transition triggers an Objective resolution once.

Example:
- target becomes Disabled;
- later Destroyed.

A completed Disable objective does not award again at Destroy.

## 23. Objective Failure From Wrong Outcome

An objective can explicitly fail if the wrong combat outcome occurs.

Example:
- "Capture scientist alive"
- target dies
- Capture objective fails.

This must be visible/clear in briefing once known.

## 24. Edge Cases

If a target simultaneously enters Disabled and Destroyed in one deterministic damage sequence, Destroyed is final physical state; whether a Disable objective had already committed depends on event order explicitly defined by the target transition pipeline.

If a target retreats after surrender, Capture remains incomplete unless ownership/control condition was already committed.

If Clear Area is complete and a later unrelated patrol enters after mission resolution, the already committed objective does not reopen.

## 25. Explicit Non-Goals

Combat objectives do not use vague undefined terms such as:
- defeat;
- neutralize;
- eliminate

without mapping them to one canonical Resolution Type.

## 26. Dependencies

This specification depends on GDS-8 Objectives, Damage/Health, Enemy Behavior, Spacecraft Damage, Robots/Raids, and Narrative.

## 27. Open Questions

None in the combat-objective semantic baseline.
