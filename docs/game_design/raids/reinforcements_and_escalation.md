# Reinforcements and Escalation

> **Status:** Draft  
> **Authority:** Raid escalation states, alarm triggers, reinforcement-call transaction, reinforcement arrival, defender mobilization, repair response, evacuation, escalation persistence, and anti-spawn-cheating rules

## 1. Purpose

Escalation creates increasing pressure as the raid continues.

The pressure must come from real target capability and committed events rather than arbitrary enemy spawning.

## 2. Escalation vs Security Alarm

GDS-2 Security owns local station security states.

GDS-11 uses a raid-wide **Escalation State** representing the strategic response to the attack.

The two can influence each other but are not duplicates.

## 3. Canonical Escalation States

Baseline:

1. Quiet;
2. Suspicious;
3. Alerted;
4. Mobilized;
5. Reinforcements En Route;
6. Reinforced / Full Response;
7. Withdrawal / Evacuation.

Not every raid reaches every state.

## 4. Quiet

The target has no confirmed raid awareness beyond ordinary security posture.

Low-signature actions can remain here while undetected.

## 5. Suspicious

The target has evidence of possible intrusion/hostile activity but lacks full confirmation.

Possible responses:

- local investigation;
- increased sensor attention;
- selected doors secured;
- patrol repositioning.

## 6. Alerted

The target confirms a credible hostile operation.

Possible responses:

- security alarm;
- weapons ready;
- defenders take positions;
- target sections lock;
- external defenses increase readiness.

## 7. Mobilized

The target commits its available local reserve.

Possible responses:

- security robots deploy;
- reserve defenders arm;
- defensive craft launch;
- repair teams activate;
- critical systems hardened;
- interior routes controlled.

## 8. Reinforcements En Route

A valid reinforcement call has successfully committed and an external force is traveling toward the raid.

Destroying communications after this point does not automatically cancel the force.

## 9. Reinforced / Full Response

The dispatched reinforcement force has physically arrived and entered the local operation.

This can include:

- ships;
- boarding defenders;
- additional robots;
- specialist response.

## 10. Withdrawal / Evacuation

The target chooses to evacuate assets/personnel or abandon selected positions because:

- objective is lost;
- damage is severe;
- doctrine favors survival;
- reactor/environment danger rises;
- player force dominates.

This state does not automatically equal player victory.

## 11. Escalation Triggers

Possible triggers:

- detected unauthorized docking;
- failed spoof/hack;
- sensor-confirmed hostile ship;
- weapon discharge;
- destroyed defense;
- breach alarm;
- defender casualty;
- restricted-area intrusion;
- objective theft;
- sabotage;
- explicit alarm activation;
- player discovery by defender;
- external distress transmission.

## 12. Trigger Determinism

Each trigger has an authored escalation effect.

There is no hidden random percentage determining whether obvious gunfire matters.

Uncertain detection can come from perception systems, not an arbitrary raid dice roll.

## 13. Escalation Can Skip States

A major event can immediately escalate several stages.

Example:

- explosive hull breach observed by security can move Quiet directly to Alerted or Mobilized.

## 14. Escalation De-Escalation

Escalation normally does not fully return to Quiet during an active confirmed raid.

However:

- Suspicious can return toward normal if investigation finds nothing;
- an Alerted target that loses contact can shift local defenders into search rather than omniscient tracking.

Strategic knowledge that an attack occurred is not forgotten instantly.

## 15. Reinforcement Capability

A target can call reinforcements only if:

- faction/world context provides a valid force/source;
- communication path exists;
- call authority is functional;
- route is reachable;
- call transaction completes.

An isolated derelict does not produce reinforcements merely because raid duration is long.

## 16. Reinforcement Call State

Canonical call states:

- Not Requested;
- Calling;
- Committed;
- Disrupted;
- Responding;
- Arrived;
- Cancelled by valid external cause.

## 17. Calling

A call has begun but not yet committed.

The attacker can interrupt it by:

- destroying/jamming communication;
- disabling call controller;
- hacking/cancelling authorization;
- physically controlling the node.

## 18. Committed

The message/order has successfully left the target and a valid response has accepted it.

At this point the incoming force is persistent.

## 19. Disrupted

A call interrupted before commit.

The target may attempt another call if:

- another communication path exists;
- the system is repaired;
- doctrine allows retry.

## 20. Responding

Reinforcement force is traveling.

Travel uses actual GDS-7/GDS-6 route capability at the appropriate abstraction.

## 21. Arrival

Reinforcements enter through a valid local-space/docking/boarding route.

They do not spawn inside a sealed player-controlled room.

## 22. Multiple Reinforcement Sources

A target can have multiple valid sources.

Each source must be explicit.

Examples:

- local patrol;
- nearby faction station;
- docked reserve craft;
- internal security reserve.

No infinite sequence exists unless a scenario explicitly defines a larger warfront source and still uses finite wave rules.

## 23. Reinforcement Wave Budget

A raid instance defines a finite planned/available reinforcement budget based on target/world state.

The system cannot create endless defenders simply because the player remains.

The exact composition/count is tuneable or generated from stable raid seed.

## 24. Stable Reinforcement Seed

For procedural raids, major reinforcement composition/timing is stable for the Raid/Mission ID after target state is created.

Reload does not reroll weaker reinforcements.

## 25. Arrival Telegraphing

If the player has valid sensors/intelligence, incoming reinforcements can be detected through:

- comm intercept;
- sensor contact;
- ETA;
- docking activity.

If no valid detection exists, the player can be surprised by a physically legitimate arrival.

## 26. ETA

Reinforcement ETA advances only during active game time.

True Pause stops it.

Closing the application does not progress it.

## 27. Communication Sabotage

Disabling communications before call commit can prevent/delay reinforcement.

After call commit it can still:

- reduce live coordination;
- prevent additional calls;
- reduce tactical updates.

It does not recall the dispatched force automatically.

## 28. Defender Mobilization

Mobilization uses actual local assets.

Examples:

- sleeping/off-duty defenders become active after time;
- security robots leave docks;
- turrets switch from standby to tracking;
- doors lock;
- repair teams begin response.

Assets cannot mobilize if:

- destroyed;
- cut off;
- unpowered;
- already committed elsewhere.

## 29. Defender Repair Response

After Alerted/Mobilized, defenders can create repair tasks for:

- power;
- shields;
- sensors;
- doors;
- turrets;
- communication.

Repair requires actual worker/robot, route, parts, power, and time.

## 30. Counter-Sabotage

The target can attempt to undo attacker sabotage.

This means a temporary disable is strategically different from destruction/removal.

## 31. Defender Resupply

Defenders can physically resupply from:

- armory;
- magazine;
- local cargo;
- docked reserve.

Destroyed/isolated supply reduces sustained response.

## 32. Interior Lockdown

Mobilized/full response can trigger GDS-2-compatible lockdown.

Lockdown uses actual doors/control and does not seal destroyed barriers.

## 33. Evacuation

A target can evacuate:

- noncombatants;
- command personnel;
- unique objective target;
- valuable cargo;
- defending ships.

Evacuation requires actual transport/routes and time.

## 34. Objective Escape

If the raid objective is a person/object that can be evacuated, mission can fail if that target successfully leaves under an explicit objective failure rule.

The player is warned when the escape becomes known/active.

## 35. Loot Relocation

Defenders can move high-value physical loot to safer storage or evacuation craft only through real logistics.

The item cannot teleport because alarm level increased.

## 36. Attacker Time Pressure

The raid has no universal countdown.

Escalation creates pressure through:

- stronger local response;
- incoming finite reinforcement;
- repair of disabled systems;
- objective evacuation;
- ship counterattack;
- resource consumption.

## 37. Escalation and Threat

GDS-8 Overall Threat can update as newly discovered/arrived forces change actual danger.

This does not reroll the original target to match player strength.

## 38. Retreat Logic

NPC defenders/attackers can retreat under doctrine/casualty/objective rules.

Retreating forces must physically reach valid departure route.

## 39. Full Response Ceiling

Once all locally available and committed reinforcement resources are active, escalation does not keep inventing stronger tiers forever.

The raid can remain dangerous without endless spawning.

## 40. Raid Boss/Special Response

A special response unit can exist if:

- target/world state supports it;
- it is part of stable authored/procedural raid content;
- arrival path is valid.

It is not spawned as a hidden punishment solely because the player looted for too long.

## 41. Persistence

Escalation state, alarm, call transaction, ETA, mobilized assets, destroyed/disabled defenders, and arrived reinforcements persist through valid saves.

## 42. Save/Load

Reload cannot:

- un-send a committed call;
- reset ETA;
- respawn defeated defenders;
- reroll reinforcement composition;
- return evacuation cargo to original room after transfer.

## 43. Defensive Raid Symmetry

When Horizon Station is attacked, hostile raiders also use finite escalation/reinforcement resources.

They cannot receive unlimited waves.

The player's own allied response can likewise occur only through valid world/faction support defined later.

## 44. Edge Cases

If the player destroys the communication node during Calling, call becomes Disrupted.

If it is destroyed one simulation step after Commit, reinforcement remains En Route.

If the response source itself is destroyed/disabled by a separate world event before departure, the call can be Cancelled by valid external cause.

If reinforcements arrive but target station has lost all docking paths, they can remain external, create another breach, withdraw, or use another valid route rather than teleport inside.

## 45. Explicit Non-Goals

Escalation does not provide:

- infinite enemy spawning;
- hidden arbitrary punishment waves;
- reinforcement without a source/route;
- instant defender repair;
- reset-on-reload timers;
- automatic recall after call commit;
- loot/objective teleportation during evacuation.

## 46. Tuneable Parameters

Escalation thresholds, call duration, reinforcement ETA, wave composition, mobilization delay, repair priority, evacuation timing, and doctrine thresholds are tuneable.

## 47. Dependencies

This specification depends on GDS-2 Security/Automation, GDS-7 routes/factions, GDS-8 Threat/Timers, GDS-9 AI/Combat, GDS-10 Robots, Sabotage, Interior Attack, External Attack, and future Dynamic Events/Economy.

## 48. Open Questions

None in the escalation/reinforcement baseline.
