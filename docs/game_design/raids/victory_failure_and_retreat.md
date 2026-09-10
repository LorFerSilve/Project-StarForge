# Raid Victory, Failure and Retreat

> **Status:** Draft  
> **Authority:** Raid result semantics, objective-completion mapping, retreat, partial outcome, persistent target aftermath, casualty/loss handling, capture boundary, and no-game-over station-defense rules

## 1. Purpose

Raid outcomes must describe what actually happened rather than collapse every operation into win/lose by enemy body count.

## 2. Base Mission Result

Raid mission resolution reuses GDS-8 canonical mission result states:

- Succeeded;
- Failed;
- Abandoned;
- Resolved.

GDS-11 may add descriptive outcome tags but does not create a contradictory second success system.

## 3. Raid Success

A raid succeeds when:

- all mandatory objective conditions are committed;
- no mission failure condition has invalidated them;
- required extraction commits when Extraction Required.

## 4. Raid Failure

A raid fails when:

- a mandatory objective becomes irrecoverably impossible;
- protected target failure occurs;
- extraction becomes impossible where mandatory;
- player defeat commits without recovery;
- a mission-specific strategic failure condition commits.

## 5. Raid Abandonment

The player may explicitly abandon the operation.

Abandonment:

- forfeits unresolved success-only objectives/rewards;
- does not teleport the player;
- begins/permits withdrawal;
- preserves actual damage/loot/robot state;
- consumes already spent resources.

## 6. Retreat

Retreat is a physical tactical behavior, not itself a mission result.

A player can retreat after:

- success;
- failure;
- abandonment;
- partial objective completion.

The final result depends on objective/extraction state.

## 7. Descriptive Outcome Tags

For reporting/analytics, a resolved raid can receive non-authoritative descriptive tags such as:

- Objective Secured;
- Heavy Losses;
- Low-Signature Entry;
- Full Alarm;
- Minimal Damage;
- Major Sabotage;
- High-Value Loot;
- Robot Losses;
- Ship Disabled;
- Reinforced Resistance.

These tags do not independently grant victory.

## 8. Partial Outcome

A mission can define explicit partial-result branches.

Example:

Primary objective:
- recover 3 research cores.

Partial branch:
- extract at least 1 core.

If the mission explicitly supports partial resolution, that branch can grant reduced consequences/rewards while base Mission Result remains Failed or a named authored Partial outcome mapped through GDS-8 reward logic.

There is no universal automatic 50% success state.

## 9. Objective Completion Persists

A completed objective can remain committed even if later extraction fails, unless the objective definition explicitly requires maintaining its state through extraction.

Example:
- destroy relay = target relay remains destroyed;
- player later dies = mission may fail, but relay is not restored by failure.

## 10. Wrong Outcome Failure

If objective requires Capture and target is Destroyed:

- Capture objective fails.

If objective requires Preserve and target is destroyed:

- mission fails where mandatory.

GDS-9 combat-objective semantics remain authoritative.

## 11. No Kill-All Victory Requirement

Destroying every defender does not automatically complete:

- theft;
- rescue;
- hack;
- sabotage;
- extraction;
- capture.

The player must satisfy the actual objectives.

## 12. Defender Retreat

If defenders retreat:

- they physically leave/abandon positions;
- remaining target systems/loot persist;
- objective success depends on raid requirements.

Driving off defenders can itself be an objective if explicitly defined.

## 13. Target Surrender

A target can surrender only if authored doctrine/state supports it.

Surrender can:

- end active resistance;
- unlock negotiated access;
- permit capture/loot under mission rules.

It does not automatically transfer full station ownership.

## 14. Station Capture Boundary

Baseline raids do not convert arbitrary enemy stations into additional fully managed player bases.

A Capture objective can mean:

- seize local command node;
- capture ship/asset;
- capture person;
- temporarily control facility;
- secure station for allied faction/story.

Persistent second-home-station ownership requires a separate accepted design specification.

## 15. Target Destruction

A raid target can be destroyed only if its authored physical/system conditions support that outcome.

There is no universal station HP reaching zero.

A facility can instead end as:

- Operational;
- Damaged;
- Disabled;
- Abandoned;
- Captured by authored faction;
- Destroyed.

## 16. Persistent Aftermath

Raid resolution commits actual target changes such as:

- destroyed defenses;
- missing loot;
- hacked access;
- disabled systems;
- destroyed/removed components;
- pressure damage;
- surviving defenders;
- faction hostility;
- reinforcement losses.

## 17. Target Recovery

GDS-12 later defines how persistent targets can recover over active game progression.

Recovery must use explicit world/economic/system logic.

GDS-11 establishes that target state does not instantly reset at raid end.

## 18. Repeat Raid Boundary

A resolved target cannot be immediately re-raided as a fresh pristine copy simply to farm identical loot.

A later raid requires:

- persistent target still exists;
- new valid Mission/Raid ID;
- world state makes it available;
- current target inventory/defenses reflect prior history and recovery.

## 19. Player Resource Loss

Raid failure does not apply a universal percentage tax.

Player loss comes from actual:

- ammo/consumables spent;
- equipment damage/loss under existing rules;
- ship fuel/damage;
- robot destruction/loss;
- Field-Unsecured loot loss;
- mission-specific consequences.

## 20. Robot Loss

GDS-10 remains authoritative:

- Destroyed robots stay destroyed;
- Disabled robots require recovery;
- abandoned robots can become Lost;
- secured robots persist.

Raid success does not resurrect robot casualties.

## 21. Player Defeat

Player defeat uses GDS-5/GDS-8.

The raid does not add a second death/revival system.

## 22. Ship Defeat

Persistent player ship recovery uses GDS-6.

The raid can still fail or require rescue if the ship cannot extract.

## 23. Reputation / Faction Outcome

GDS-7/GDS-12 applies faction effects based on facts such as:

- target ownership;
- whether player identity is known;
- casualties;
- sabotage severity;
- stolen strategic assets;
- negotiated/surrender result.

Raid result alone does not blindly change every faction.

## 24. Identification

A hostile faction can respond differently depending on whether it credibly identifies the player.

Low-signature success can therefore reduce attribution without creating moral invisibility to actors who directly witnessed the player.

## 25. Evidence of Attack

Even if player identity is unknown, the target can know it was attacked.

Faction intelligence can later connect evidence only through explicit world/narrative systems.

## 26. Resolution Rewards

Success-only rewards use GDS-8 Rewards.

Physical field loot is separate.

Failure/retreat can still preserve already extracted physical loot.

## 27. Story Raid Failure

A failed mandatory story raid cannot permanently brick the campaign through ordinary tactical failure.

Retry/recovery follows GDS-7/GDS-8 story rules.

Committed world damage may be normalized by authored retry state only where narrative continuity requires it, and that exception must be explicit.

## 28. Story Target Persistence

Unique story raid targets cannot duplicate:

- objective item;
- unique character;
- archive;
- critical system

across retry branches.

## 29. Defensive Raid Victory

Horizon Station defense is successful when hostile mandatory raid objectives become impossible/abandoned and surviving attackers:

- retreat;
- are disabled/destroyed/captured;
- fail to extract required stolen objective.

The player does not need to destroy every fleeing attacker unless a mission objective says so.

## 30. Defensive Raid Failure

A Horizon Station defense can fail even though Horizon Station remains owned by the player.

Examples:

- attackers extract stolen resources;
- required station subsystem is sabotaged;
- attackers destroy target module;
- hostile objective succeeds.

This creates consequences without routine campaign deletion.

## 31. Horizon Station Is Not Routine Game Over

A normal procedural station-defense failure does not:

- delete Horizon Station;
- permanently remove Command Core;
- wipe player research/Blueprint knowledge;
- permanently kill recruited crew.

It can cause significant physical recovery work and resource/robot loss.

## 32. Defensive Aftermath

After attack:

- station damage remains;
- fires/breaches/faults remain until resolved;
- stolen resources remain gone only if enemy extraction committed;
- destroyed robots remain destroyed;
- crew injury persists;
- attacker wrecks/cargo can be recovered;
- security compromise can require reset/repair.

## 33. Recovery Phase

Station returns to Normal only after actual:

- hostile clearance;
- fire/hazard containment;
- pressure stabilization;
- security reset;
- critical repair;
- command/control verification.

There is no one-click full post-raid reset.

## 34. Scuttling / Self-Destruction

Baseline player raids do not include a generic target self-destruct timer.

An authored target can have scuttle capability if it is physically/systemically defined and telegraphed once activated.

## 35. No Reward for Repeated Save Outcome

A one-time reward/objective transaction commits once.

Save/load cannot re-award:

- reputation;
- Blueprint;
- target loot;
- unique salvage;
- story flag.

## 36. Failure Summary

Raid result UI later reports:

- Mission Result;
- mandatory/optional objective states;
- extracted loot;
- lost/unsecured loot;
- robot outcomes;
- ship/equipment damage;
- target persistent changes;
- reinforcement state;
- faction consequences;
- recovery/retry availability.

## 37. Edge Cases

If the player completes a sabotage objective, retreats, but is defeated before extraction in an extraction-required mission, target sabotage can remain while Mission Result is Failed.

If defenders surrender after the player has already destroyed the mission's Preserve target, surrender cannot repair the failed objective.

If enemy raiders carry stolen goods into their ship but are destroyed before departure commit, defensive raid can still count as success if the theft objective required extraction.

If Horizon Command Core is disabled during a routine attack, station enters recovery; ownership is not permanently removed.

## 38. Explicit Non-Goals

Raid outcome does not use:

- kill count as universal victory;
- percentage resource tax;
- instant target reset;
- automatic full station capture;
- free casualty restoration;
- routine Horizon deletion/game-over;
- generic partial success percentage.

## 39. Tuneable Parameters

Partial reward values, target recovery pace, attribution thresholds, surrender doctrine, casualty tolerance, and retry costs are tuneable or later-domain values.

## 40. Dependencies

This specification depends on GDS-7 Factions/Narrative, GDS-8 Mission Failure/Rewards, GDS-9 Combat Objectives, GDS-10 Robot Loss, Extraction, Looting, Sabotage, Station Defense Resolution, and future Economy/Persistence.

## 41. Open Questions

None in the raid-result baseline.
