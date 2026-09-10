# Raid Extraction and Withdrawal

> **Status:** Draft  
> **Authority:** Raid withdrawal initiation, extraction routes, player/robot/loot security, contested exits, ship departure, alternate extraction, abandoned assets, and extraction commit

## 1. Purpose

A successful raid requires getting the player and required objective state out of the hostile installation.

Completing the interior objective is not automatically equivalent to surviving the operation.

## 2. GDS-8 Authority

Raid extraction uses GDS-8 Extraction as its base transaction system.

GDS-11 specializes it for multi-phase fortified targets.

## 3. Withdrawal vs Extraction

**Withdrawal** means the player has begun disengaging from the raid objective area.

**Extraction** means an authorized exit transaction has committed.

Withdrawal can fail before extraction.

## 4. Withdrawal Can Begin Anytime

The player can begin retreating:

- before boarding;
- during external attack;
- during interior combat;
- after optional loot;
- after primary objective completion;
- after raid failure.

The game does not require the player to stay until every optional objective is resolved.

## 5. Primary Extraction Methods

Baseline raid extraction methods:

1. Return to Docked Player Ship;
2. Return via EVA to Stand-Off Player Ship;
3. Local-Space Departure in Player Ship;
4. Mission/Allied Transport;
5. Authored Emergency Recovery.

## 6. Return to Docked Ship

Sequence:

1. reach valid boarding route;
2. physically enter/secure player/robots/cargo;
3. detach/undock if required;
4. achieve local departure conditions;
5. commit GDS-8 extraction.

## 7. Docked Ship Is Not Safe Zone

Before departure commit, a docked ship can still be:

- attacked;
- boarded;
- disabled;
- docking-clamp trapped;
- cut off by lockdown;
- exposed to station defenses.

Being inside the ship does not automatically end the raid.

## 8. EVA Return

When ship is standing off:

- player/robots must physically traverse EVA route;
- life support/energy/robot environment limits remain active;
- loot/wreck mass must be transportable;
- ship must remain reachable.

## 9. Local-Space Departure

For ship-based extraction, the player must satisfy GDS-6 departure/strategic travel requirements.

Examples:

- propulsion operational;
- navigation available;
- no physical docking attachment;
- route not blocked;
- sufficient fuel/propellant;
- mission-specific warm-up complete.

## 10. No Instant Invulnerable Warp

Starting departure does not instantly make the ship immune.

If the chosen propulsion has a warm-up/transition window, combat can still affect it until commit.

## 11. Contested Extraction

A raid extraction can be Contested because of:

- defender control of dock;
- active boarding;
- disabled ship;
- blocked access route;
- active tractor/clamp if such system exists;
- required passenger/robot missing;
- mission-specific anti-departure system.

Hostile presence alone is not a universal extraction block.

## 12. Extraction Under Fire

If departure conditions are satisfied, the player may extract while defenders remain active.

No universal kill-all requirement exists.

## 13. Required Objective Cargo

If success requires a physical object, extraction readiness checks the exact required security state.

Examples:

- player carrying object;
- object in ship cargo;
- rescued person secured aboard;
- captured component secured.

The mission definition states the requirement.

## 14. Optional Loot

Optional raid loot never blocks extraction.

The player may leave it behind.

## 15. Robot Recovery

GDS-10 robot recovery rules apply.

A robot is safely recovered only if physically secured in valid transport/extraction.

## 16. Robot Warning

Before extraction commit, the UI warns when owned recoverable robots are known to remain behind.

The player can confirm abandonment if mission rules allow.

## 17. Destroyed Robot Wrecks

Wreck recovery is optional unless specifically required.

A wreck must physically fit appropriate cargo/recovery capacity.

## 18. Disabled Robot

A Disabled robot can be:

- repaired;
- towed/carried;
- secured in transport;
- abandoned.

Extraction cannot magically recover it from another room.

## 19. Reserve / Bridgehead Assets

All remaining:

- reserve robots;
- loot carriers;
- deployed gadgets;
- captured equipment

resolve according to their physical extraction state.

No blanket "retrieve all deployed assets" action exists.

## 20. Secured Loot During Withdrawal

Loot already Vehicle/Extraction-Secured in ship cargo remains secured while the ship survives/retains ownership.

Field-Unsecured backpack/robot-carried loot remains at risk until extraction commit.

## 21. Loot Transfer Before Departure

The player can spend additional time moving loot to the ship before leaving.

This increases exposure to escalation/attack.

## 22. Alternate Extraction

If the original exit becomes invalid, the player can use another method only if one physically exists/becomes available.

Examples:

- create second breach;
- use another dock;
- EVA to ship;
- restore docking controls;
- call mission transport if supported.

## 23. No Automatic Alternate Exit

The game does not spawn a rescue shuttle simply because the original ship is disabled.

Emergency recovery must be an authored available capability.

## 24. Ship Disabled

If player ship becomes Disabled:

- raid can continue locally;
- standard extraction becomes unavailable;
- player must repair, obtain alternate extraction, or fail/be recovered through valid system.

## 25. Ship Destroyed Boundary

Persistent primary player-ship protection/recovery follows GDS-6.

GDS-11 does not override it.

The raid still resolves whether the ship can perform extraction.

## 26. Docking Route Lost

If defenders close/destroy the path to the ship:

- extraction readiness becomes unavailable/contested;
- player must reopen/re-route.

## 27. Pressure/Hazard Block

A previously safe corridor can become:

- vacuum;
- fire;
- toxic;
- structurally blocked.

The route remains physical; the player needs protection/alternate route.

## 28. Reinforcement Arrival During Withdrawal

Incoming defenders can intercept the player/ship through valid paths.

Withdrawal does not freeze escalation.

## 29. Mission Success Commit

For Extraction Required raids, success transaction commits only when:

1. mandatory objectives are complete;
2. required objective entities/cargo satisfy security;
3. extraction commit succeeds.

## 30. Raid Failure With Successful Escape

The player can physically escape after failing/abandoning the raid.

Result:

- raid mission = Failed/Abandoned;
- eligible extracted loot remains secured;
- no success-only Resolution Reward;
- persistent target damage/sabotage can remain.

## 31. Successful Objective With Failed Escape

If extraction is mandatory and the player completes objective but cannot extract:

- raid does not become Success;
- objective state can remain Completed;
- raid fails if extraction/recovery becomes impossible.

## 32. In-Place Resolution Raid

A purely external/remote sabotage raid may define In-Place Resolution if GDS-8 says no physical extraction is required.

This must be explicit in mission definition.

## 33. Attacker Pursuit

Defenders may pursue:

- through station;
- into external space;
- along local departure route

within their actual capability/doctrine.

They do not follow beyond impossible strategic reach.

## 34. Enemy Boarding of Player Ship

During withdrawal, defenders can board/counter-board the player ship only through valid docking/breach paths.

Ship interior defense follows GDS-6/GDS-9/GDS-11 boundaries.

## 35. Cargo Interception

Secured ship cargo changes ownership only if an explicit physical event occurs:

- ship captured;
- cargo hold breached/looted;
- cargo jettisoned;
- ship/cargo destroyed.

Raid failure alone does not remove it.

## 36. Extraction Commit Atomicity

The extraction transaction must atomically finalize:

- player state;
- required passengers;
- robot recovery/abandonment;
- secured/unsecured loot;
- ship state;
- mission success/failure;
- target persistent changes;
- unresolved optional objectives.

No asset may exist both inside the raid instance and in extracted ownership after commit.

## 37. Post-Extraction Target

After player departure:

- target remains in its committed persistent raid state;
- defenders can enter recovery according to GDS-12/world rules;
- player cannot instantly reopen same active instance to respawn loot/defenders.

## 38. Defensive Raid Attacker Extraction

Hostile raiders attacking Horizon Station must also physically withdraw.

Stolen resources are not permanently removed from player ownership until the hostile carrier/ship successfully commits extraction.

## 39. Defender Interception of Thieves

If player defenses destroy/disable fleeing hostile carriers before extraction:

- stolen goods remain physically recoverable;
- target ownership can be restored only through actual recovery/transfer.

## 40. Edge Cases

If player enters ship with a required object in backpack and immediately undocks, backpack remains player-owned while successful extraction makes it Vehicle/Extraction-Secured per GDS-8.

If a Robot ID is physically aboard but rack restraint is invalid/destroyed, extraction eligibility follows actual transport safety rules rather than proximity.

If player abandons a robot after confirming warning and later the target remains persistent/recoverable, that Robot ID can become Lost rather than Destroyed.

If defenders restore a sabotaged shield during departure, this can affect the escaping ship until extraction commit.

## 41. Explicit Non-Goals

Raid extraction does not provide:

- automatic completion at objective pickup;
- ship-as-invulnerable-safe-room;
- kill-all requirements;
- magical robot recall;
- free emergency shuttle;
- frozen escalation during retreat;
- loot duplication at extraction.

## 42. Tuneable Parameters

Departure warm-up, extraction interaction duration, warning thresholds, pursuit distance, dock release timing, and emergency-recovery costs are tuneable.

## 43. Dependencies

This specification depends on GDS-6 Docking/Navigation, GDS-8 Extraction/Failure, GDS-10 Robot Recovery, Interior Attack, External Attack, Looting, Escalation, Victory/Failure, and future Persistence/Presentation.

## 44. Open Questions

None in the raid-extraction baseline.
