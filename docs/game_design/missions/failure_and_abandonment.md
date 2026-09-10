# Mission Failure and Abandonment

> **Status:** Design Complete  
> **Authority:** Mission failure triggers, ordinary player defeat resolution, objective impossibility, active withdrawal, abandonment, secured/unsecured resource consequences, retry, and failure transaction ordering

## 1. Purpose

Failure must be consequential, deterministic, and readable without deleting unrelated long-term progression.

## 2. Failure Classes

Baseline failure classes:

- Player Defeat;
- Mandatory Objective Failure;
- Protected Entity Failure;
- Transport / Extraction Failure;
- Timed Failure;
- Strategic Mission Failure;
- Player Abandonment.

## 3. Player Defeat

When the player becomes Incapacitated and the mission has no valid local rescue/recovery state:

- mission fails;
- GDS-5 ordinary mission-defeat player recovery begins;
- mission inventory transaction resolves exactly once.

## 4. Recoverable Incapacitation

A mission can explicitly support a rescue window.

Examples:

- allied rescue robot;
- mission medical team;
- nearby safe ship crew.

If rescue succeeds:
- mission remains Active;
- no failure transaction commits.

If rescue becomes impossible:
- ordinary Player Defeat commits.

## 5. Mandatory Objective Failure

If every valid branch leading to mission success becomes impossible, the mission fails.

Example:
- required intact archive is destroyed and no alternate source exists.

## 6. Protected Entity Failure

A mission can define a protected survivor/transport/object as mandatory.

Failure condition must explicitly state whether:
- injury is acceptable;
- incapacitation is acceptable;
- destruction/death fails the mission;
- alternate recovery exists.

## 7. Unique Narrative Characters

Failure involving a unique main-story character follows GDS-7 authored retry/recovery rules.

Routine tactical failure does not silently remove the character from campaign canon.

## 8. Extraction Failure

If success requires extraction and every valid extraction path becomes impossible, mission fails unless a repair/rescue branch remains.

## 9. Timed Failure

A timed mission fails at zero only if its objective explicitly defines timer expiration as failure.

Timer is visible and uses Simulation Time.

## 10. Strategic Failure

Some missions can fail because a world-state condition occurs.

Examples:

- relay destroyed;
- convoy reaches hostile destination;
- target escapes;
- defended facility lost.

The trigger must be explicit.

## 11. Abandon Before Deployment

An Accepted mission can be cancelled before deployment.

Consequences can include:
- no physical loss beyond already incurred travel/preparation actions;
- possible faction/contract consequence if the mission definition says commitment mattered.

There is no hidden penalty by default.

## 12. Abandon After Deployment

The player can select Abandon during an Active mission when not in a modal/transient state.

Abandonment:

1. marks mandatory objectives unsuccessful;
2. closes future success-only objective progression;
3. retains valid extraction/withdrawal routes;
4. does not teleport the player;
5. does not refund supplies;
6. applies authored faction/contract consequences on resolution.

## 13. Withdrawal After Failure

After a non-player-defeat failure, the player can often still physically withdraw.

This allows preserving already acquired loot according to extraction/security rules.

## 14. Failure Transaction Ordering

On ordinary mission failure the canonical ordering is:

1. freeze new mission-success commits;
2. finalize Objective states;
3. snapshot authoritative physical ownership/security;
4. apply Player Defeat transaction if relevant;
5. preserve Vehicle/Extraction-Secured owners unless an explicit cargo-loss rule applies;
6. resolve Field-Unsecured loss/persistence;
7. apply ship/crew/survivor state;
8. apply faction/narrative failure consequences;
9. resolve eligible rewards/forfeiture;
10. close/convert Mission Instance;
11. commit recovery/retry state.

This ordering occurs once.

## 15. Secured Loadout

GDS-5 Secured Loadout follows the player-defeat transaction:

- recovered persistent equipment returns;
- unconsumed pre-mission supplies return;
- already consumed supplies remain consumed;
- condition damage can apply.

## 16. Field-Unsecured Player Loot

On ordinary defeat without successful extraction:

- Field-Unsecured mission-acquired backpack loot is lost by default;
- a specific mission can create a recoverable dropped-cache state;
- if no such rule exists, it is not duplicated/recoverable.

## 17. Vehicle/Extraction-Secured Cargo

Already secured ship/vehicle cargo remains with its actual surviving persistent owner by default.

A mission may explicitly define:
- cargo interception;
- vehicle capture;
- cargo destruction.

Such exceptions must be known/systemically caused, not arbitrary failure tax.

## 18. Station-Secured State

Previously Station-Secured resources are never rolled back because an external mission fails.

## 19. Consumed Supplies

Ammunition, medkits, fuel, propellant, batteries, and other supplies consumed during a failed/abandoned mission remain consumed.

## 20. Ship Damage

Ship damage persists.

Ordinary mission failure does not restore the ship to pristine condition.

Persistent player ship loss follows GDS-6 protections.

## 21. Crew / Temporary Passenger

Crew/survivor outcomes use their actual state.

Recruited crew do not routinely die permanently.

Temporary rescue targets can fail according to their explicit mission rules.

## 22. Reputation Consequences

Failure can affect faction reputation only when authored/contractually relevant.

Ordinary personal defeat does not automatically anger every faction.

## 23. Story Mission Retry

A failed main-story mission enters a retry/recovery state.

The player may need to:

- return;
- repair;
- restock;
- repeat travel;
- re-enter the mission.

Unique story knowledge already legitimately committed before failure persists only if the story mission explicitly marks it as committed independent progress.

## 24. Procedural Mission Failure

A failed procedural Mission ID becomes Resolved/Failed.

A later similar opportunity uses a new Mission ID rather than rewinding the old one.

## 25. Recoverable World Sites

Failure at a persistent world site can leave physical world changes intact where specified.

Example:
- player restored relay power but failed later defense.

The resulting world state follows objective commit rules rather than total mission rollback.

## 26. No Full World Rollback

Mission failure does not restore unrelated:
- station production;
- research;
- faction history;
- world changes already committed outside rollback scope.

## 27. Failure Summary

The result screen identifies:

- failure cause;
- completed/failed objectives;
- extracted/secured loot;
- lost loot;
- consumed loadout;
- ship/equipment damage;
- survivor/crew outcome;
- reputation/story consequences;
- retry availability.

## 28. Emergency Recovery Cost

Where recovery services are used, GDS-12 can define economic/service cost.

The absence of current economic tuning does not change the mission failure state.

## 29. Save/Load

A committed failure cannot be half-applied.

Persistence restores either:
- pre-failure active mission;
- or post-failure resolved/recovery state.

## 30. Edge Cases

If the player abandons after Vehicle/Extraction-Securing cargo but before personal extraction, the cargo remains secured with its owner while mission success is forfeited.

If a Primary Objective completes in the same simulation step as its failure trigger, the Objective's authored event priority must define which event logically occurs first; this priority is data, not frame-order accident.

If all extraction methods fail after objectives complete, the mission is not yet Success unless extraction was Optional/In-Place.

If a rescued survivor is already safely Vehicle-Secured but the player later fails, survivor outcome follows the actual secured passenger/ship recovery state.

## 31. Explicit Non-Goals

Failure does not provide:

- total save rollback;
- free supply refunds;
- arbitrary loss of secured cargo;
- instant abandonment teleport;
- routine permanent player/crew deletion;
- hidden timers;
- mission reset through reload rerolling.

## 32. Tuneable Parameters

Tuneable values include recovery costs, condition penalties, retry placement, optional reputation penalties, and rescue-window duration.

## 33. Dependencies

This specification depends on Mission System, Objectives, Extraction, Rewards, GDS-5 Health/Inventory, GDS-6 Ship Persistence/Cargo, GDS-3 Crew/Survivors, Narrative, Combat, and GDS-12 Persistence/Economy.

## 34. Open Questions

None in the failure/abandonment transaction baseline.
