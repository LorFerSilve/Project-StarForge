# Player Health, Damage and Defeat

> **Status:** Draft  
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
- owning location/mission rules determine recovery.

The player cannot continue shooting/mining while incapacitated.

## 9. No Routine Permadeath

Ordinary combat, environmental hazards, and mission failure do not permanently delete the player character/save.

Narrative permanent death is not part of the baseline.

## 10. External Mission Defeat

Ordinary external-mission incapacitation normally results in:

1. mission failure;
2. the deterministic inventory transaction defined in inventory.md;
3. loss of unresolved Field-Unsecured physical resources;
4. preservation of recovered Secured Loadout ownership, subject to condition damage;
5. no refund of ammunition/consumables already consumed;
6. return/recovery at the home station through mission recovery fiction;
7. medical recovery cost/time where defined.

Missions can define a local rescue window before final failure if they explicitly support it.

## 11. Home Station Incapacitation

At the home station:

- local crew/medical automation can create a rescue task;
- the player remains physically incapacitated until recovered;
- the station continues simulation;
- recovery transfers the player to a valid medical/recovery location.

If no normal medical capability exists, protected Command Core emergency recovery provides the minimum path to avoid save hard-lock.

## 12. Rescue by Robots/Crew

Eligible crew or rescue robots may move the incapacitated player if:

- they can physically reach the player;
- hazard conditions permit;
- transport capability exists.

The player is not teleported through blocked geometry.

## 13. Environmental Damage

Field Survival reports exposures such as:

- vacuum;
- low pressure;
- low oxygen;
- toxic atmosphere;
- heat;
- cold;
- radiation.

Health resolves biological consequences.

## 14. Fall Damage

Movement reports impact severity.

Health applies damage after equipment mitigation.

## 15. Combat Damage Boundary

Combat defines:

- hit detection;
- damage type;
- raw damage;
- shield interaction;
- armor penetration/mitigation rules.

Health receives the resulting biological damage.

This avoids duplicate combat authority.

## 16. Personal Shield

A Personal Shield is optional equipment.

Shield behavior includes:

- finite capacity;
- damage absorption according to GDS-9;
- temporary depletion;
- regeneration after a delay if powered/operational.

A depleted shield does not remove equipped armor or Health.

## 17. Shield Power

Personal shields use suit/equipment energy capacity.

Regeneration can pause when:

- energy is insufficient;
- shield hardware is damaged;
- an EMP/disruption effect applies.

Exact values belong to equipment/combat balancing.

## 18. Armor

Armor/protective suit components can mitigate damage.

Armor is not a second health bar unless a specific armor component defines condition/durability.

## 19. Equipment Condition

Damage can reduce equipment condition according to Equipment rules.

Ordinary defeat does not delete the item.

Destroyed/disabled condition requires repair before full use.

## 20. Healing

Health recovery methods can include:

- field medical consumable;
- medkit/tool;
- crew Medic assistance;
- medbay treatment;
- protected station recovery.

Field healing is finite and consumes actual medical resources.

## 21. No Passive Full Regeneration

Biological Health does not regenerate to full automatically after waiting.

A small stabilization mechanic can stop worsening states where defined, but restoring lost Health requires treatment.

## 22. Field Medical Consumables

Portable treatment may restore a bounded amount of Health or stabilize a condition.

Use consumes the finished medical item/resource.

The exact recipe/item is registered through GDS-4 when equipment content is finalized.

## 23. Treatment Time

Some medical actions are sustained interactions.

They can be interrupted by movement, damage, or target invalidation according to Interaction rules.

## 24. Recovery State

After serious incapacitation the player can enter Recovering.

Recovery may temporarily:

- reduce maximum active health capability;
- limit deployment;
- consume active game time.

The baseline prefers short meaningful recovery over long forced waiting.

## 25. No Real-World Waiting

Recovery progresses only through active game time.

The game must always provide meaningful alternative station activity rather than forcing the player to sit idle.

## 26. Respawn Terminology

The game avoids arcade "respawn" as the default fiction.

Ordinary recovery is represented as rescue/medical recovery.

Technical checkpoints may still implement the transition.

## 27. Damage Feedback

The player receives clear feedback for:

- shield hit;
- shield collapse;
- armor impact;
- biological damage;
- critical state;
- environmental damage.

Presentation details belong to GDS-13.

## 28. Damage Source Readability

When incapacitated, the post-failure summary should identify the primary cause when known.

This supports learning rather than unexplained defeat.

## 29. Save/Load During Incapacitation

Save rules can restrict saving during transient defeat resolution.

Loading cannot be used to duplicate lost field resources or undo an already committed mission-failure transaction outside normal save semantics.

## 30. Edge Cases

If the player reaches zero Health at the exact moment extraction commits, Mission rules determine transaction ordering explicitly.

If the player is incapacitated in vacuum, rescue requires valid environmental protection for responders.

If the player has no medbay, emergency station recovery still prevents permanent hard-lock.

If shield capacity hits zero from an attack with excess damage, GDS-9 defines whether overflow reaches armor/Health.

## 31. Explicit Non-Goals

The baseline does not use:

- routine player permadeath;
- instant free full healing;
- passive full health regeneration;
- generic respawn with no consequence;
- player combat damage formulas duplicated here.

## 32. Tuneable Parameters

Tuneable values include Health maximum, state thresholds, healing values, recovery durations, equipment condition penalties, and personal shield statistics.

## 33. Dependencies

This specification depends on Player Character, Movement, Equipment, Field Survival, Interaction, GDS-9 Combat, Missions, station medical systems, Crew Medic rules, and persistence.

## 34. Open Questions

None in the health/failure baseline.

Combat damage formulas are now authoritative in GDS-9, and the exact mission defeat transaction order is authoritative in GDS-8. No unresolved player-health rule remains at this boundary.
