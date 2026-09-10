# Player Health, Damage and Defeat

> **Status:** Under Review — GDS-14 Corrected  
> **Authority:** Player health states, damage reception boundary, injury consequences, incapacitation, medical recovery, and ordinary defeat handling

## 1. Purpose

The player health system makes combat and environmental danger consequential without using routine permadeath.

Combat owns attack/damage formulas. This document owns how the player character receives resulting damage and transitions through health/failure states.

## 2. Core Layers

The player can have three defensive layers:

1. Personal Shield, when equipped;
2. Armor/Protective Equipment mitigation;
3. Biological Health.

Exact combat damage types and formulas are owned by GDS-9.

## 3. Health

Biological Health is a finite value.

When Health reaches zero, the player becomes Incapacitated.

Health does not become negative for gameplay resolution.

## 4. Health States

The baseline states are:

- Healthy;
- Wounded;
- Critical;
- Incapacitated;
- Recovering.

Thresholds are tuneable.

## 5. Healthy

No health-based restriction.

## 6. Wounded

Wounded state can create visible feedback and minor capability penalties only when explicitly configured.

It does not automatically make controls sluggish or frustrating.

## 7. Critical

Critical state communicates that the player is close to incapacitation.

Possible effects can include limited sprinting or stronger medical urgency, but exact thresholds are tuneable.

## 8. Incapacitated

At zero Health:

- direct movement/action stops;
- normal combat interaction stops;
- the player enters Incapacitated state;
- owning location/mission rules determine rescue or recovery.

The player cannot continue shooting/mining while incapacitated.

## 9. No Routine Permadeath

Ordinary combat, environmental hazards, and mission failure do not permanently delete the player character/save.

Narrative permanent death is not part of the baseline.

## 10. External Mission Defeat

When external-mission incapacitation has no valid local rescue state, the canonical sequence is:

1. GDS-8 mission failure commits according to its transaction ordering;
2. the deterministic inventory/security transaction resolves once;
3. unresolved Field-Unsecured physical resources are lost or persist only through an explicitly authored recovery cache/path;
4. recovered Secured Loadout ownership is preserved, subject to condition damage;
5. ammunition, fuel, propellant, medkits, and other supplies already consumed remain consumed;
6. the player enters **Recovery Transit** under `../systems/recovery_transit_and_destination.md`;
7. a valid recovery destination is selected from actual current world capability rather than assuming Horizon Station is always immediately safe/reachable;
8. medical recovery cost/time applies where defined.

External defeat is therefore **not** an instant teleport to Horizon Station.

Horizon remains the normal preferred long-term recovery hub when a valid safe recovery ingress/location exists.

## 11. Recoverable Incapacitation

A mission can explicitly support a local rescue window before final defeat.

Examples:

- allied rescue robot;
- mission medical team;
- nearby safe ship/crew.

If local rescue succeeds:

- mission can remain Active;
- the external defeat transaction does not commit;
- Recovery Transit does not begin.

## 12. Home Station Incapacitation

At Horizon Station:

- local crew/medical automation can create a rescue task;
- the player remains physically incapacitated until recovered;
- station simulation continues while unpaused;
- recovery transfers the player only through a physically valid local rescue/medical path.

If ordinary medical capability is unavailable, protected Command Core emergency capability provides or coordinates the minimum softlock-safe recovery path defined by GDS-12 failure/recovery.

This emergency path does not create free full healing, resources, or invulnerability.

## 13. Rescue by Robots/Crew

Eligible crew or rescue robots may move the incapacitated player only if:

- they can physically reach the player;
- hazard conditions permit their approach/protection;
- transport/manipulation capability exists;
- their own operational state allows it.

The player is not teleported through blocked geometry.

## 14. Environmental Damage

Field Survival reports exposures such as:

- vacuum;
- low pressure;
- low oxygen;
- toxic atmosphere;
- heat;
- cold;
- radiation.

Health resolves biological consequences.

## 15. Fall Damage

Movement reports impact severity.

Health applies resulting biological damage after the relevant equipment/protection rules.

## 16. Combat Damage Boundary

Combat defines:

- hit detection;
- damage type/channel;
- raw damage;
- shield interaction;
- armor penetration/mitigation.

Health receives resulting biological damage.

No duplicate combat formula exists here.

## 17. Personal Shield

A Personal Shield is optional equipment.

Shield behavior includes:

- finite capacity;
- GDS-9 channel absorption/overflow;
- temporary depletion;
- regeneration after a delay only while powered/operational.

A depleted shield does not remove equipped armor or biological Health.

## 18. Shield Power

Personal shields use suit/equipment energy capacity.

Regeneration can pause when:

- energy is insufficient;
- shield hardware is damaged;
- EMP/disruption applies.

Exact values are tuneable equipment/combat data.

## 19. Armor

Armor/protective equipment can mitigate damage according to GDS-9.

Armor is not a second biological Health bar.

## 20. Equipment Condition

Damage can reduce equipment condition according to Equipment rules.

Ordinary defeat does not delete established equipped items.

Disabled/damaged items require actual repair before full use.

## 21. Healing

Health recovery methods can include:

- field medical consumable;
- medical tool;
- Crew Medic assistance;
- medbay treatment;
- protected emergency recovery.

Field healing is finite and consumes actual medical resources where required.

## 22. No Passive Full Regeneration

Biological Health does not automatically regenerate to full merely through waiting.

Restoring lost Health requires a valid treatment/recovery process.

## 23. Field Medical Consumables

Portable treatment can restore a bounded amount of Health or stabilize an eligible condition.

Use consumes the actual finished item/resource at its authoritative transaction point.

## 24. Treatment Time

Medical actions can be sustained interactions.

They can be interrupted by movement, damage, lost access, or target invalidation according to Interaction/Combat rules.

## 25. Recovering State

After serious incapacitation the player can enter Recovering.

Recovery may temporarily:

- reduce available Health capability;
- limit deployment;
- require treatment;
- consume Simulation Time.

The baseline prefers short meaningful recovery over long forced idle waiting.

## 26. No Real-World Waiting

Recovery progresses only on Simulation Time.

Closing the application or entering True Pause does not advance it.

The game should leave meaningful alternative activity available where the player's current medical state permits it.

## 27. Recovery Transit Boundary

Recovery Transit is owned by GDS-12/GDS-14 cross-cutting recovery authority.

This Player domain supplies:

- Incapacitated state;
- medical condition;
- eligible player recovery requirements.

It does not choose strategic destination or transport time by itself.

## 28. Primary Ship Boundary

Recovering the player does not automatically recover or relocate the Primary Ship.

Ship location/damage/cargo remain GDS-6/GDS-8 authority and follow the Recovery Transit asset rules.

## 29. Robot/Crew Boundary

Player Recovery Transit does not recall robots, crew, or Temporary Passengers.

Each persists at its actual owner/location under the relevant mission/robot/crew rules.

## 30. Respawn Terminology

The game avoids arcade `respawn` as default fiction.

Ordinary recovery is represented as rescue, stabilization, transport, and medical recovery.

Technical implementation may use scene/checkpoint transitions without changing this gameplay contract.

## 31. Damage Feedback

The player receives clear feedback for:

- shield hit/collapse;
- armor impact;
- biological damage;
- Critical state;
- environmental damage;
- Incapacitation/recovery state.

GDS-13 owns presentation.

## 32. Failure/Recovery Feedback

After defeat, the player-facing recovery summary must distinguish:

- mission result;
- lost Field-Unsecured loot;
- preserved Secured Loadout;
- consumed supplies;
- ship state/location;
- robot/crew outcomes;
- Recovery Transit destination/time;
- known Horizon emergency/defense state;
- medical/recovery cost where known.

## 33. Save/Load During Incapacitation

A stable Incapacitated state can be saved when GDS-12 Stable Save Boundary conditions are met.

Once failure/recovery atomic transaction begins, a save request queues until the next Stable Save Boundary.

Loading cannot duplicate field resources or split pre/post recovery state.

## 34. Simultaneous Extraction and Incapacitation

If Health reaches zero in the same Simulation Time step as extraction completion, GDS-8's deterministic mission/extraction event ordering resolves whether extraction commits before the failure transaction.

The result cannot depend on render-frame order.

## 35. Incapacitation in Hazard

If the player is incapacitated in vacuum, fire, toxic atmosphere, or equivalent hazard:

- local rescue requires valid responder protection/access;
- the hazard continues on Simulation Time until recovery/failure transition;
- no helper teleports through unsafe/blocked geometry.

## 36. No Medbay

Absence of a developed medbay cannot hard-lock the save.

Protected emergency recovery can stabilize/recover the player to the minimum playable state, potentially with meaningful delay/cost/limitations rather than free full restoration.

## 37. Explicit Non-Goals

The baseline does not use:

- routine player permadeath;
- instant defeat teleport to Horizon;
- instant free full healing;
- passive full Health regeneration;
- generic no-consequence respawn;
- automatic ship/robot recall with player recovery;
- player combat damage formulas duplicated in Health.

## 38. Tuneable Parameters

Tuneable values include Health maximum, state thresholds, healing values, recovery durations, medical costs, equipment-condition consequences, and shield statistics.

## 39. Dependencies

Depends on Player Character, Movement, Equipment, Field Survival, Interaction, GDS-9 Combat, GDS-8 Missions, GDS-6 Spacecraft, Station medical/emergency systems, Crew Medic, GDS-12 Time/Persistence/Failure Recovery, `../systems/recovery_transit_and_destination.md`, and GDS-13 Presentation.

## 40. Open Questions

None after GDS-14 external-defeat/recovery reconciliation.
