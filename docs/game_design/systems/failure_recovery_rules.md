# Failure and Recovery Rules

> **Status:** Design Complete  
> **Authority:** Cross-domain failure hierarchy, permanent-vs-recoverable losses, recovery principles, softlock protection, cost conservation, catastrophic-state handling, and precedence between subsystem failure rules

## 1. Purpose

Project StarForge must allow meaningful setbacks without letting ordinary systemic failure randomly erase the campaign or contradict subsystem ownership.

## 2. Core Principle

Failure preserves what actually happened.

It does not:

- rewind unrelated progression;
- apply arbitrary percentage taxes;
- refund consumed resources;
- restore destroyed systems for free;
- erase protected long-term knowledge.

## 3. Canonical Asset-Loss Matrix

| Asset / State | Ordinary Permanent Loss? | Recovery Authority |
| --- | --- | --- |
| Player identity | No | GDS-5/GDS-8 |
| Recruited human crew | No | GDS-3 |
| Primary established player ship | No routine deletion | GDS-6 |
| Horizon Station identity/Command Core | No routine deletion | GDS-2/GDS-11 |
| Player robot | Yes, if Destroyed | GDS-10 |
| Field-Unsecured loot | Yes | GDS-8 |
| Consumed ammo/fuel/consumables | Yes | owning resource system |
| Station/ship physical resources | Only through real consumption/destruction/theft | GDS-4/GDS-11 |
| Equipment | No routine defeat deletion; can require repair | GDS-5 |
| Committed Blueprint/Research Evidence/route knowledge | No | GDS-4/GDS-7 |
| Credits | Only through explicit payment/fine/transaction | Economy |
| Reputation | Can increase/decrease | GDS-7 |

## 4. Player Incapacitation

Player Health zero enters GDS-5 Incapacitated.

GDS-8 resolves external mission rescue/failure.

There is no permanent player death baseline.

## 5. No Abstract Death Tax

Ordinary player defeat does not automatically remove a percentage of:

- Credits;
- station inventory;
- Research;
- Blueprints.

Costs arise from actual:

- consumed supplies;
- lost unsecured loot;
- equipment condition;
- medical treatment;
- ship/robot damage.

## 6. Mission Failure

GDS-8 mission failure transaction remains authoritative.

Completed world changes can persist even if final Mission Result is Failed.

Example:

Relay destroyed → player later defeated → mission fails, relay remains destroyed if that target state is persistent.

## 7. Crew Failure

Routine combat, emergencies, and raids can cause:

- Wounded;
- Critical;
- Incapacitated;
- unavailable/recovery states.

They do not routinely permanently delete recruited Crew IDs.

## 8. Robot Failure

GDS-10 robots can be permanently Destroyed.

Destroyed Robot ID cannot be restored into the same functioning unit.

Disabled robots can be repaired/recovered when physically possible.

## 9. Ship Failure

Established player ships can become:

- Damaged;
- Disabled;
- Derelict;
- Stranded.

Ordinary defeat does not automatically delete the Primary Ship.

Recovery can require:

- tow/rescue;
- repair;
- resources;
- travel delay;
- alternate craft.

## 10. Horizon Station Failure

Routine systemic/raid events can leave Horizon:

- heavily damaged;
- partially unpowered;
- depressurized;
- resource-starved;
- robot-depleted;
- security-compromised.

They cannot routinely delete Horizon Station ownership or campaign-critical Command Core identity.

## 11. Physical Resource Loss

A physical resource disappears from player ownership only through a valid process such as:

- crafting/consumption;
- firing/propulsion use;
- destructive analysis;
- physical destruction;
- jettison/discard;
- successful hostile theft extraction;
- transfer/sale.

A failure screen alone is not a resource sink.

## 12. Knowledge Protection

Once committed, normal failure cannot remove:

- Blueprint Unlock;
- completed Technology Research;
- Research Evidence;
- discovered canonical route knowledge;
- committed story facts.

Physical unanalyzed carriers remain losable before knowledge commit.

## 13. Credit Loss

Credits change only through explicit economic transactions, fines, authored contract penalties, or rewards.

There is no bankruptcy percentage deducted because a mission failed.

## 14. Reputation Consequences

Failure can change relevant faction reputation only when the faction can reasonably evaluate the outcome.

Examples:

- failed contract;
- identified hostile raid;
- abandoned protected convoy;
- broken agreement.

GDS-7 reputation scale remains authoritative.

## 15. Recovery Must Be Causal

Recovering from loss requires the actual relevant system:

- medicine for health;
- repair parts/tools/work for gear/ship/station/robots;
- replacement manufacturing for destroyed robots;
- economic purchase/trade for shortages;
- recovery mission/tow for stranded assets;
- security reset after compromise.

## 16. No One-Click Global Recovery

There is no universal button that restores all player systems after defeat.

Management UI may queue known recovery tasks, but resources/time/capability still apply.

## 17. Recovery Priority

When several systems are damaged, the default recovery priority is:

1. immediate player/crew survival;
2. catastrophic hazard containment;
3. Horizon Command Core/critical control availability;
4. life support/power/thermal minimum operation;
5. safe access/security;
6. ship/extraction access where needed;
7. production/logistics;
8. defenses;
9. noncritical full restoration.

Player policies can modify noncritical priorities.

## 18. Catastrophic State

A **Catastrophic State** is a severe but normally recoverable condition where the current configuration cannot continue normal operation without intervention.

Examples:

- Horizon lacks minimum sustained life support;
- Primary Ship stranded without local repair;
- player incapacitated with no field rescue;
- critical route capability lost through physical damage.

## 19. Catastrophic Recovery Path

Every ordinary Catastrophic State must expose at least one valid recovery path, such as:

- emergency rescue/tow;
- fallback station compartment;
- allied/faction assistance;
- basic repair from protected emergency capability;
- alternate craft;
- recovery mission;
- purchasable/barter service;
- reload of an older user save as last user-controlled option.

The system must not require the player to invent impossible resources.

## 20. Softlock Protection

Ordinary systemic failure may not permanently eliminate **all** paths required to continue the main campaign.

If a sole mandatory:

- item;
- route;
- character;
- ship capability;
- station capability

would otherwise become irrecoverable, its authored design must provide a replacement, relocation, alternate route, or recovery operation.

## 21. Softlock Protection Is Not Free Restoration

The recovery path may be costly or difficult.

It must not simply restore all losses without consequence.

Example:

A destroyed expedition robot can require manufacturing a replacement; the campaign does not resurrect it.

## 22. Unique Story Physical Object

If a unique mandatory physical object is lost before its Knowledge/Objective commit, it must retain a world-recoverable identity or move to an authored recovery source.

It cannot silently respawn as a duplicate.

## 23. Story Mission Tactical Failure

Mandatory story missions are retryable/recoverable after ordinary tactical failure.

A failed attempt does not permanently close the campaign.

Committed narrative choices can still remain irreversible within that save timeline.

## 24. Resource Exhaustion

If the player exhausts common operational supplies, recovery can use:

- local low-risk resource source;
- trade/barter;
- salvage;
- faction assistance;
- alternate production chain.

The baseline should not require one rare late-game resource to restart basic early station operation.

## 25. Emergency Minimum Capability

Horizon Station's campaign-critical core should retain or have access to a minimal recoverable emergency path for:

- command access;
- basic diagnostics;
- basic repair planning;
- communication when repaired/powered.

This does not mean free power/resources.

## 26. Equipment Recovery

Routine defeat preserves established equipped item ownership according to GDS-5/GDS-8 but can leave items damaged/Disabled.

Consumables already used remain consumed.

## 27. Field Loot

Field-Unsecured loot can be lost on ordinary mission defeat.

Vehicle/Extraction-Secured cargo follows actual ship/extraction outcome.

## 28. Raid Failure

Offensive raid failure preserves committed target damage and actual resource/robot losses.

There is no undo because the raid result was Failed.

## 29. Station Defense Failure

A failed Horizon defense can cause real theft/damage/robot destruction while respecting protected Horizon/crew/knowledge invariants.

Recovery begins from actual aftermath.

## 30. Market/Trade Failure

A failed or cancelled trade before commit leaves ownership/Credits unchanged.

A completed transaction is not reversed by a later mission failure.

## 31. Manufacturing Failure

Interrupted production retains committed Work-in-Progress and consumed/installed inputs according to GDS-4/GDS-2.

Cancellation does not guarantee full refund after irreversible transformation.

## 32. Research Failure

Interrupted research preserves valid completed progress/consumed physical samples according to Research rules.

Committed Research Evidence is not lost.

## 33. Dynamic Event Failure

Ignoring/failing an optional Dynamic Event applies only its authored local consequences.

Procedural events cannot apply arbitrary global punishment or campaign deletion.

## 34. Difficulty

Difficulty changes allowed pressure/recovery parameters but not the permanent-loss classification in the Asset-Loss Matrix.

Extreme does not enable ordinary crew permadeath; Assisted does not resurrect robots.

## 35. Save Interaction

Failure/recovery transactions are atomic Stable Save Boundary operations.

A save cannot capture both pre-loss and post-loss ownership in one snapshot.

## 36. No Resource Refund on Retry

Retrying a mission/raid does not automatically refund ammunition, fuel, consumables, robot losses, or target-state changes from the failed attempt unless an authored retry abstraction explicitly resets a protected tutorial/story setup.

Such exceptions must be documented.

## 37. Tutorial Protection

Early onboarding may use explicitly authored safeguards to prevent unrecoverable failure before systems are taught.

GDS-13 owns presentation/onboarding; safeguards must not silently continue as universal rules.

## 38. Recovery Completion

A system leaves Recovery when its owning subsystem's minimum operational/safety conditions are satisfied.

Full pristine restoration is not required before normal play resumes.

## 39. Player Choice

The player can often choose to operate in a damaged/reduced-capability state instead of waiting for full repair, provided hard safety requirements permit it.

## 40. Explicit Non-Goals

Failure/Recovery does not use:

- universal death tax;
- automatic full repair;
- routine player/crew permadeath;
- routine Primary Ship deletion;
- routine Horizon deletion;
- Knowledge Asset deletion;
- free robot resurrection;
- failure-screen resource percentages detached from physical events.

## 41. Tuneable Parameters

Medical/repair cost, recovery time, emergency service price, condition penalties, and grace values are tuneable.

The permanent-vs-recoverable classifications are fixed baseline rules.

## 42. Dependencies

This specification coordinates GDS-2 through GDS-11 and depends on Economy, Dynamic Events, Difficulty, Time, Save/Persistence, and Progression softlock gates.

## 43. Open Questions

None in the cross-domain failure/recovery baseline.
