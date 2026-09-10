# Raids and Station Defense Cross-Validation

> **Status:** Active Audit  
> **Authority:** GDS-11 consistency review  
> **Audit Revision:** Refreshed after GDS-12

## 1. Scope

This audit validates offensive raids, target intelligence, preparation, external assault, boarding/breaching, interior attack, sabotage, looting, escalation, extraction, outcomes, story integration, and Horizon Station defense against GDS-1 through GDS-12.

## 2. Raid as Specialized Mission — Result: PASS

Offensive raids remain specialized GDS-8 Missions and reuse Mission ID, lifecycle, objectives, stable seed, Threat, extraction, failure, and reward transactions.

## 3. Single-Player Core — Result: PASS

No synchronous or asynchronous PvP raid system is required. GDS-12 Multiplayer Position confirms single-player PvE as the complete baseline.

## 4. No Offline Raids — Result: PASS

Raids and Defense Events advance only through active Simulation Time. Real-world time while the game is closed cannot create, progress, resolve, repair, or replenish a raid state.

## 5. Persistent Raid Targets — Result: PASS

Committed damage, breaches, storage depletion, removed components, sabotage, security compromise, and unique-object removal remain persistent until an explicit world recovery/replenishment process changes them.

## 6. Target Recovery — Result: PASS

The previously unresolved long-term recovery dependency is now owned by GDS-12.

Recovery:

- uses Simulation Time;
- depends on target/faction/economic capability;
- cannot recreate a unique stolen object;
- cannot instantly restore a pristine target at mission end;
- may make prior Intel Stale when actual state changes.

## 7. No Raid Farming Reset — Result: PASS

A depleted target cannot be repeatedly re-entered as a fresh stockpile. A later raid uses current persistent target state plus legitimate recovery/replenishment.

## 8. Intelligence — Result: PASS

Unknown/Suspected/Confirmed/Stale remains authoritative. GDS-12 Economy may allow purchase of specific intel from valid contacts but does not provide a universal buy-all-intel button or omniscient target state.

## 9. Preparation — Result: PASS

Preparation validates real ship, Reach, player loadout, robots/TCC, transport, tools, cargo, survival, and extraction capability. Credits cannot substitute for missing physical capability.

## 10. External Attack/Boarding — Result: PASS

Station shields, defenses, power, sensors, access, docking, authored breach points, and spacecraft combat remain physical/systemic. No global station HP or automatic boarding trigger is introduced.

## 11. Interior Combat/Sabotage — Result: PASS

Interior control remains local and causal. Hacking/sabotage affects only reached and connected systems. GDS-12 Difficulty does not grant enemies omniscient tracking or hidden station-wide buffs.

## 12. Looting — Result: PASS

Raid loot remains physical finite ownership transfer. The player keeps only what is found, carried/transferred, secured, and extracted.

GDS-12 Loot/Rewards introduces no percentage theft or duplicate reward copy.

## 13. Raid Economic Value — Result: PASS

GDS-12 now resolves raid profitability context through:

- target finite stock;
- item/resource market value;
- physical cargo constraints;
- ammunition/fuel/repair costs;
- robot attrition;
- reputation consequences;
- target recovery time.

A raid is not guaranteed to be profitable.

## 14. Horizon Defensive Theft — Result: PASS

Hostile raiders must still physically reach, load, transport, and extract stolen resources. Permanent player loss occurs only after Hostile Extraction Commit.

## 15. Knowledge Protection — Result: PASS

Committed Research, Blueprint Unlocks, Research Evidence, and route knowledge cannot be stolen from player memory. Physical unanalyzed carriers may be stolen/destroyed according to ownership rules.

## 16. Reinforcements — Result: PASS

Reinforcements require a valid source, committed call, route, and finite force. GDS-12 Dynamic Events/Time do not create infinite waves or reset reinforcement ETAs on reload.

## 17. Escalation — Result: PASS

Escalation remains finite and deterministic from actual target/world capability. Difficulty does not replace it with endless stronger spawns.

## 18. Horizon Attack Generation — Result: PASS

GDS-12 now owns when a Defense Event becomes a candidate/created event based on world/faction/economic/current-station state.

Once created, GDS-11 owns execution.

## 19. Recovery Grace — Result: PASS

After qualifying Major/Severe Horizon defense outcomes, ordinary severe follow-up attacks are suppressed for a bounded Recovery Grace period in active Simulation Time.

Initial design target: 30 active-game minutes; exact value remains tuneable.

Recovery Grace does not repair damage or pause unrelated events.

## 20. Off-Screen Horizon Defense — Result: PASS

When the player is away during active gameplay, lower-detail deterministic resolution still consumes actual station/attacker state. No opaque global Defense Score becomes authoritative.

## 21. Player Return — Result: PASS

Returning to Horizon joins the current Defense Event state. The attack does not restart or rewind, and abandoning an external mission retains normal GDS-8 consequences.

## 22. Difficulty — Result: PASS

Difficulty may alter only documented pressure axes. It cannot change raid target inventory, progression requirements, Reach, TCC, reputation reward entitlement, unique loot, or AI knowledge.

## 23. Failure/Retreat — Result: PASS

Raid failure or retreat does not impose a universal tax. Consequences remain actual:

- consumed ammunition/fuel;
- ship/equipment damage;
- destroyed/lost robots;
- Field-Unsecured loot loss;
- target changes;
- reputation/world effects.

## 24. Routine Horizon Failure — Result: PASS

A procedural defense failure may cause severe physical setbacks but cannot routinely:

- delete Horizon Station/Command Core;
- erase Knowledge Assets;
- permanently kill recruited crew.

Robot attrition and physical resource theft/destruction remain real.

## 25. Save/Persistence — Result: PASS

GDS-12 now resolves the final save policy for Raid/Defense Event ID, seed, target damage, loot ownership, breaches, reinforcement calls, escalation, robot losses, stolen cargo, and recovery state.

Saving is allowed during raids at Stable Save Boundaries.

## 26. Atomic Raid Transactions — Result: PASS

Save/load cannot half-apply:

- loot transfer;
- hostile theft extraction;
- player extraction;
- mission reward;
- robot destruction/recovery;
- final story commit.

A save requested during an atomic transaction queues until it completes.

## 27. Story Raid Integration — Result: PASS

GDS-11 remains explicitly bound to:

- hostile MS-A301 Rook's Wake when it becomes a fortified assault;
- MS-A302 only when an escalated access branch becomes a true fortified assault;
- MS-F02 fortified access/control-path phases.

## 28. MS-F01 Finale Readiness — Result: PASS

The previously unresolved GDS-12 coalition threshold is now defined before MS-F02 can deploy.

Readiness includes actual Reach IV ship capability, player loadout, Horizon support, departure resources, robot TCC/squad minimums, and four Support Channels.

## 29. Finale Raid Robot Capacity — Result: PASS

The finale requires at least 6 base Field TCC and a Tactical Squad of combined Command Load >= 4, while temporary coalition support can add +2 subject to cap 12.

This does not bypass transport/service requirements.

## 30. Faction Consequences — Result: PASS

GDS-11 records attributable raid facts; GDS-7/GDS-12 apply reputation/economic consequences. Low-signature play can affect attribution but cannot erase facts known by valid witnesses/sensors.

## 31. Postgame — Result: PASS

Persistent raid targets, faction hostility, damage, stock, and compatible Defense Events continue in P5 according to the chosen ending and GDS-12 world/economy/event rules.

No target reset occurs because the campaign finale completed.

## 32. Remaining Downstream Dependency — GDS-13

GDS-13 remains responsible for raid briefing/intel presentation, escalation feedback, squad/ship readiness UX, looting feedback, station alarms, defense status, failure/reward summaries, and accessibility.

## 33. Conclusion

The previously pending **GDS-12 Economy/Dynamic Events/Difficulty/Persistence/Recovery/Progression** dependencies are now first-pass resolved.

No blocking contradiction exists between GDS-11 and GDS-1 through GDS-12.

GDS-11 remains:

**First-Pass Complete — Cross-Validation Pending**

Its remaining scheduled downstream design dependency is GDS-13, followed by GDS-14 final audit.
