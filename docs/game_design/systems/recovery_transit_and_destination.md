# Recovery Transit and Destination Resolution

> **Status:** Under Review — Added by GDS-14  
> **Authority:** Player external-defeat recovery destination, recovery transit timing, Horizon-under-attack interaction, ship-location preservation, and anti-teleport recovery rules

## 1. Purpose

GDS-14 identified a cross-system ambiguity: external mission defeat normally returns the player through recovery fiction, while Horizon Station can simultaneously be under attack or in an unsafe recovery state.

This specification prevents defeat from becoming instant strategic travel and defines where the player actually recovers.

## 2. Recovery Is Not Extraction Success

Player defeat recovery:

- does not count as successful mission extraction;
- does not secure Field-Unsecured loot;
- does not refund consumed resources;
- does not automatically repair the ship;
- does not automatically recover abandoned robots;
- does not complete success-only objectives.

GDS-8 failure/ownership transactions remain authoritative.

## 3. Recovery Transit

When an external Player Defeat commits and no local rescue keeps the mission Active, the player enters **Recovery Transit** before appearing at a recovery location.

Recovery Transit is a recovery abstraction representing rescue, transport, stabilization, and return travel.

## 4. Simulation During Recovery Transit

Recovery Transit consumes Simulation Time.

During it, all otherwise valid systems continue, including:

- Horizon operation;
- Horizon Defense Events;
- Dynamic Events;
- market/recovery timers;
- target recovery;
- station work.

True Pause still freezes everything.

## 5. Anti-Teleport Rule

Recovery Transit cannot be used to return strategically faster than a plausible valid recovery route.

Its duration is at least the greater of:

- the authored minimum recovery-service duration for the mission context; and
- the shortest currently valid strategic return/recovery-route duration between the mission staging location and selected recovery destination.

Tuneable service overhead may make recovery slower.

## 6. Local Rescue Precedence

Before final Player Defeat commits, a mission-local rescue window has first precedence where GDS-8 explicitly supports it.

If local rescue succeeds:

- no external Recovery Transit begins;
- mission can remain Active;
- normal local rescue consequences apply.

## 7. Recovery Destination Requirements

A valid destination must provide all mandatory conditions for the chosen recovery method:

- reachable through the authored recovery network/route;
- not currently physically inaccessible to the recovery carrier;
- enough immediate safety to place an Incapacitated/Recovering player;
- a valid recovery/medical or protected emergency interface;
- no hostile control that makes the destination recovery point unusable.

## 8. Destination Precedence

After external defeat, select the first valid destination in this order:

1. **Player Primary Ship / mission support craft**, if it remains a valid safe recovery owner under the mission's recovery fiction;
2. **Horizon Station**, if a valid safe recovery point exists;
3. **Committed allied/faction recovery facility** already available through world/faction state;
4. **Nearest authored neutral/emergency recovery location** valid for that mission/region;
5. **Protected Horizon emergency recovery path** once the required safe ingress/recovery state can be established.

A destination is never invented ad hoc at failure time; the world/mission/recovery system must identify its valid source.

## 9. Horizon During Active Defense Event

An active Horizon Defense Event does not automatically make Horizon invalid.

Horizon remains a valid recovery destination only when:

- a physical recovery ingress is available;
- a protected recovery/medical area exists;
- that ingress/area is not under hostile control;
- station systems can accept the recovery arrival.

If these conditions are false, destination resolution uses the next valid recovery option.

## 10. Arrival During Ongoing Horizon Attack

If Horizon is selected and the Defense Event remains active when Recovery Transit completes:

- the player arrives into the current persistent defense state;
- already committed station damage/theft remains;
- the attack does not restart;
- the player enters at the valid protected recovery location;
- the player can join the current defense after their health/recovery state permits normal action.

## 11. Horizon Critical Recovery

If Horizon is in Critical Recovery but still has a valid protected recovery point, it can accept the player.

If no such point exists, destination resolution uses another valid location until a safe recovery path is restored.

## 12. Primary Ship Preservation

Player relocation does not teleport the Primary Ship.

The ship remains at its actual authoritative state/location unless the same recovery transaction explicitly includes a valid:

- tow;
- autopilot return;
- rescue-carrier transport;
- station retrieval.

Such ship recovery preserves real damage, cargo ownership, fuel use, and recovery cost.

## 13. Ship-Secured Cargo

Vehicle/Extraction-Secured cargo remains with its actual ship/vehicle owner.

If the player recovers elsewhere while the ship remains stranded, the cargo remains with that stranded ship.

## 14. Player Backpack

Field-Unsecured mission-acquired backpack loot resolves under GDS-8 failure and is not preserved merely because the player later arrives at a recovery destination.

Pre-mission Secured Loadout follows its established recovery rules.

## 15. Robots

Recovery Transit does not recall field robots.

Robot state follows GDS-10/GDS-8:

- secured in valid transport → preserved with that owner;
- field Operational/Disabled → recoverability/Lost rules;
- Destroyed → remains Destroyed.

## 16. Crew / Survivor Passengers

A recruited crew member or Temporary Passenger follows their actual secured transport/location state.

Player Recovery Transit cannot duplicate or teleport them into the player's destination.

## 17. Recovery Cost

Recovery can impose authored causal cost such as:

- Credits service charge;
- medical supplies;
- ship tow/service cost;
- recovery delay;
- faction obligation/reputation consequence where justified.

There is no universal percentage death tax.

## 18. No Valid Immediate Destination

If no destination is immediately valid:

- the player remains in Recovery Transit / pending rescue state;
- Simulation Time can advance through the authored rescue process;
- world systems continue;
- the system re-evaluates only legitimate recovery paths.

The baseline must retain at least one eventual softlock-protected recovery route under `failure_recovery_rules.md`.

## 19. User Save Fallback

Selecting an older user save remains a player-controlled fallback but is not treated as the in-world recovery transaction.

The game design should not silently rewrite the current timeline merely because a recovery route is expensive or delayed.

## 20. Strategic Travel Resources

A recovery method that physically moves the player's ship or another persistent vehicle consumes/records its relevant resources according to the actual method.

A recovery abstraction that transports only the player does not duplicate the player's ship or its cargo.

## 21. Save/Persistence

Persist:

- Recovery Transit state;
- chosen/eligible destination candidates;
- remaining Simulation Time;
- included recovery assets;
- excluded/stranded assets;
- committed recovery cost;
- current Horizon Defense state independently.

A save is committed only at a Stable Save Boundary.

## 22. Presentation

The failure/recovery summary must communicate:

- mission result;
- player recovery destination;
- expected Recovery Transit time;
- ship location/state;
- robot outcomes;
- secured vs lost cargo;
- Horizon attack/recovery status if relevant;
- known recovery cost.

The player must not infer that every owned asset returned merely because the player character did.

## 23. Intentional Defeat

Intentional self-defeat receives no special shortcut.

The same mission loss, Recovery Transit duration, asset rules, and world-time consequences apply.

## 24. Edge Case — Horizon Attack Begins During Transit

If Horizon was valid when Recovery Transit began but becomes unsafe before arrival:

- destination validity is re-evaluated before arrival commit;
- the player is rerouted only to another already-valid recovery path;
- the reroute can add Simulation Time/cost;
- the player does not spawn into an invalid hostile-controlled recovery point.

## 25. Edge Case — Horizon Becomes Safe Again

A later restored Horizon recovery point can become eligible before final destination commit if the recovery route can validly redirect there.

Rerouting is explicit and cannot reduce elapsed recovery below the Anti-Teleport Rule.

## 26. Edge Case — Ship Is Disabled but Safe

A Disabled ship can still be the player's immediate recovery destination if:

- internal environment is safe enough;
- player recovery access is possible;
- mission recovery fiction supports reaching it.

The mission can then be Failed while the player faces a ship repair/tow problem rather than being teleported home.

## 27. Explicit Non-Goals

Recovery Transit does not provide:

- instant fast travel through defeat;
- mission Success;
- free ship teleport;
- free robot recall;
- free repair;
- automatic loot security;
- arbitrary newly spawned rescue location.

## 28. Tuneable Parameters

Recovery service overhead, medical delay, service cost, and presentation timing are tuneable.

Destination precedence, anti-teleport behavior, Simulation-Time advancement, and asset-preservation boundaries are fixed.

## 29. Dependencies

Depends on GDS-5 Health, GDS-6 Ship Navigation/Damage, GDS-8 Failure/Extraction, GDS-10 Robots, GDS-11 Horizon Defense, GDS-12 Failure/Recovery/Time/Persistence/Economy, World locations, and GDS-13 Presentation.

## 30. Open Questions

None after GDS-14 reconciliation.
