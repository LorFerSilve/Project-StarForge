# Player Health, Damage and Defeat

> **Status:** Design Complete  
> **Authority:** Player biological Health states, damage reception boundary, health-band restrictions, incapacitation, treatment, recovery, and ordinary defeat handling

## 1. Purpose

Player Health makes combat and environmental danger consequential without routine permadeath or arbitrary post-defeat penalties.

Combat owns attack/damage formulas. This document owns biological Health state and the player-specific consequences after resolved damage reaches Health.

## 2. Defensive Layers

Damage reaches the player through the applicable sequence defined by GDS-9:

1. Personal Shield, if operational;
2. Armor/protective equipment;
3. Biological Health.

This specification never duplicates shield/armor penetration formulas.

## 3. Biological Health

Health is finite and clamped to `[0, Maximum Health]`.

At zero Health, the player enters `PlayerHealthState::Incapacitated`.

Negative Health has no separate gameplay meaning.

## 4. Canonical Health States

The biological Health state is one of:

- `PlayerHealthState::Healthy`;
- `PlayerHealthState::Wounded`;
- `PlayerHealthState::Critical`;
- `PlayerHealthState::Incapacitated`;
- `PlayerHealthState::Recovering`.

Threshold values are tuneable; the behaviors below are fixed.

## 5. Healthy

Healthy imposes no Health-band restriction.

## 6. Wounded

Wounded communicates meaningful injury and medical need but imposes **no inherent movement, aiming, weapon, tool, interaction, or carrying penalty** by itself.

A separate explicitly applied Status Effect, equipment failure, environmental exposure, or authored injury may impose its own documented restriction.

This prevents an implementer from inventing hidden low-health control degradation.

## 7. Critical

Critical is the final nonzero Health band.

While Critical:

- sprint is disabled;
- normal jump is disabled;
- mantle is disabled;
- use of equipment explicitly tagged `HeavyOperation` is disabled;
- ordinary walking, crouching, aiming, firing an otherwise usable weapon, using ordinary portable tools, interacting, and inventory access remain available unless another explicit state blocks them;
- deployment from a safe staging location into a new external mission/raid is blocked until the player is at least Wounded and medically deployable.

Critical Health does not randomly alter aim, sensitivity, recoil, or movement direction.

## 8. Incapacitated

At zero Health:

- direct movement stops;
- combat/tool use stops;
- world interaction stops;
- normal inventory rearrangement stops;
- mission/location rescue or failure resolution takes authority.

The player may still use permitted pause/recovery UI.

## 9. Recovering

`Recovering` describes an active medical/recovery process after serious incapacitation.

Recovering **does not add an undocumented universal stat penalty**. Current usable capability derives from:

- the restored Health band;
- explicit temporary medical Status Effects/injuries;
- equipment condition;
- environmental state.

A player remains non-deployable while any explicit recovery state says `Deployable = false`.

Routine station movement/activity is permitted once local medical recovery returns control and the current Health/injury state allows it.

## 10. No Routine Permadeath

Ordinary combat, hazards, accidents, missions, and raids do not permanently delete the player character/save.

Narrative player permadeath is not part of the baseline.

## 11. External Mission Incapacitation

When an external Mission Instance supports a valid local rescue window, that rescue state is attempted under GDS-8.

If no local rescue remains valid:

1. Mission Failure resolves once under GDS-8 transaction ordering;
2. physical ownership/security states are committed;
3. consumed ammunition/fuel/propellant/consumables remain consumed;
4. Field-Unsecured loot resolves under mission rules;
5. Secured Loadout is preserved subject to documented condition consequences;
6. the player enters `RecoveryTransitState::InTransit` under `../systems/recovery_transit_and_destination.md`;
7. a valid Recovery Destination is selected from actual world capability;
8. recovery and medical costs/timers proceed through Simulation Time.

External defeat never performs zero-time teleportation to Horizon.

## 12. Local Rescue Window

A mission that supports local rescue defines:

- eligible rescue actor/system;
- rescue start condition;
- remaining Simulation Time/other failure boundary;
- required physical access/protection;
- successful recovery state.

If rescue succeeds before mission failure commits, the Mission Instance remains in the state allowed by that mission and Recovery Transit does not start.

## 13. Horizon Incapacitation

At Horizon:

- a reachable qualified crew member, rescue robot, or medical automation may create a rescue task;
- patient movement uses actual physical access;
- hazards continue while Simulation Time advances;
- recovery does not reset station state;
- medical treatment uses actual available capability/resources where required.

## 14. Protected Emergency Recovery Without a Medbay

The save must remain recoverable even if Horizon has no functioning developed medbay.

When no ordinary treatment path exists, protected Command Core emergency capability coordinates a **Minimum Emergency Recovery**:

- it requires a valid safe local recovery location or the Recovery Destination system first supplies one;
- it consumes a tuneable amount of Simulation Time;
- it restores the player only to the **minimum Wounded Health threshold**;
- it clears Incapacitated solely enough to return direct control;
- it does not repair equipment;
- it does not refill ammunition, Suit Energy, Life-Support Reserve, or inventory;
- it does not heal above the Wounded threshold;
- it requires no rare/late-game resource that could itself create a softlock.

Available medicine, Medic skill, medbay capability, or paid recovery service can improve recovery according to their own rules but are not required for this minimum anti-softlock path.

## 15. Rescue by Crew or Robots

A rescuer must:

- physically reach the player;
- survive/handle the local hazard using actual protection;
- possess suitable manipulation/transport capability;
- have an accessible route to the recovery destination.

No rescue actor teleports through sealed/blocked geometry.

## 16. Combat Damage Boundary

GDS-9 owns:

- hit resolution;
- Damage Packets/channels;
- Shield interaction;
- armor mitigation/penetration;
- status application.

Health receives only the resulting biological damage and applies Health-state transitions.

## 17. Environmental Damage Boundary

Field Survival reports exposure state/severity.

Environmental or GDS-9 hazard rules convert that exposure to biological damage/status where defined.

Health then applies the same Health-state logic as combat damage.

## 18. Fall Damage Boundary

Movement reports landing/impact severity.

After applicable equipment mitigation, the resulting biological damage enters Health normally.

## 19. Personal Shield

Personal Shield is equipment and uses GDS-9 shield rules.

A shield may regenerate only when its equipment definition permits it and its energy, hardware, delay, and disruption requirements are valid.

Shield depletion does not directly change biological Health state until damage actually reaches Health.

## 20. Armor and Equipment Condition

Armor mitigation belongs to Combat/Equipment.

Damage may reduce equipment Condition where the item defines that consequence.

Ordinary defeat does not delete established equipped gear simply because the player reached zero Health.

## 21. Healing Sources

Biological Health can be restored only by a defined treatment source:

- eligible field medical consumable;
- medical tool/first-aid interaction;
- qualified Crew Medic action;
- functioning medbay treatment;
- Minimum Emergency Recovery;
- authored medical service using the same resource/transaction principles.

Waiting alone does not restore Health.

## 22. Field Medical Consumables

A medical item defines:

- eligible Health/injury states;
- restored Health or stabilization effect;
- use duration;
- interruption policy;
- consumed quantity.

The item is consumed only at its authoritative treatment transaction point.

## 23. No Passive Full Regeneration

Biological Health never passively returns to full because combat ended or the player remained idle.

Any future regeneration technology would require an explicit equipment/Research rule.

## 24. Treatment Time

Treatment that is Sustained uses Simulation Time and Player Interaction interruption rules.

True Pause stops treatment progress.

## 25. Deployment Eligibility

The player may begin an ordinary external deployment only when:

- Health state is Healthy or Wounded;
- no explicit medical/injury status marks the player non-deployable;
- mission-specific survival/loadout requirements pass.

Critical, Incapacitated, and non-deployable Recovering states block new deployment and state the reason.

## 26. Recovery Transit Boundary

Player Health supplies the patient's condition and local rescue outcome.

`recovery_transit_and_destination.md` owns:

- strategic recovery destination;
- recovery transport mode;
- elapsed Recovery Transit Simulation Time;
- player/ship/robot/crew asset-location separation.

## 27. Primary Ship and Other Assets

Recovering the player does not automatically move:

- Primary Ship;
- ship cargo;
- robots;
- recruited crew;
- Temporary Passengers;
- mission-world objects.

Each retains its actual authoritative location/owner/state.

## 28. Damage and Recovery Feedback

Presentation distinguishes:

- Shield damage/depletion;
- armor/equipment impact where known;
- biological Health damage;
- Wounded/Critical state;
- Incapacitation;
- local rescue availability;
- Recovery Transit;
- recovery destination/time/cost;
- current deployment blocker.

No feedback may report recovery as completed before commit.

## 29. Save / Load

A stable Incapacitated or Recovering state can be saved at a Stable Save Boundary.

Once a failure/recovery atomic transaction begins, a save request waits for the next Stable Save Boundary.

Loading restores either the valid pre-transaction or post-transaction state, never both.

## 30. Simultaneous Extraction and Health Zero

If extraction completion and Health reaching zero are scheduled in the same Simulation Time step, GDS-8 deterministic event priority resolves the order before either terminal result commits.

Render-frame order cannot choose the result.

## 31. Hazard While Incapacitated

Environmental hazards continue affecting an incapacitated player until:

- rescue changes exposure;
- failure/recovery transaction removes the player from the local state;
- True Pause freezes Simulation Time.

Responder access/protection remains mandatory.

## 32. Difficulty

Difficulty may change documented incoming-damage/recovery-pressure parameters under GDS-12 but does not change:

- routine no-permadeath;
- Health-state semantic restrictions;
- ownership-loss classes;
- Minimum Emergency Recovery existence.

## 33. Accessibility

Accessibility may reduce damage presentation/camera effects and offer control aids but does not silently change Health thresholds or injury outcomes.

Assisted Difficulty, not accessibility presentation options, owns reduced gameplay pressure.

## 34. Edge Cases

- Becoming Critical while sprinting immediately ends sprint and Movement transitions to legal walking/crouching behavior.
- Healing from Critical into Wounded restores sprint/jump/mantle eligibility unless another state still blocks it.
- A Critical player already inside a Mission Instance is not automatically extracted; the deployment restriction applies to starting a new deployment, not forced mid-mission removal.
- Minimum Emergency Recovery cannot run in an actively lethal local spot until a safe recovery location is established.
- If the selected Recovery Destination becomes invalid before arrival, Recovery Transit re-evaluates through its deterministic fallback rules without duplicating the player or assets.

## 35. Tuneable Parameters

Tuneable values include:

- Maximum Health;
- Healthy/Wounded/Critical thresholds;
- healing quantities;
- treatment durations;
- Minimum Emergency Recovery duration;
- medical service costs;
- equipment-condition damage values;
- shield statistics owned by Equipment/Combat.

Health-state behavior and no-permadeath semantics are fixed.

## 36. Explicit Non-Goals

The baseline does not include:

- routine player permadeath;
- passive full Health regeneration;
- hidden low-health aim/recoil penalties;
- zero-time defeat teleport;
- automatic asset recall with player recovery;
- free full healing from emergency recovery;
- generic percentage death tax;
- duplicate combat damage formulas.

## 37. Dependencies

Depends on Player Character, Movement, Equipment, Field Survival, Interaction, GDS-9 Combat, GDS-8 Missions/Extraction/Failure, GDS-6 Spacecraft, Horizon medical/emergency capability, Crew Medic rules, GDS-12 Time/Persistence/Failure Recovery, Recovery Transit, Difficulty, and GDS-13 Presentation/Accessibility.

## 38. Open Questions

None.
