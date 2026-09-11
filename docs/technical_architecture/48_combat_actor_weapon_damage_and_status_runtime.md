# TA-7 — Combat Actor, Weapon, Damage, and Status Runtime

> **Status:** Architecture Complete
> **Authority:** High-frequency combat-facing runtime state, weapon use state, hit/damage fact routing, Health/shield/status runtime facets, and combat outcome handoff

## 1. Purpose

TA-7 provides the runtime representation required to execute GDS-9 combat while preserving TA-2 domain authority and TA-5 physics separation.

## 2. Combat Actor Components

A combat-capable runtime entity may contain:

- `CombatActorComponent`;
- `HealthRuntimeComponent` or system/structure target proxy;
- `ShieldRuntimeComponent`;
- `ArmorRuntimeSummaryComponent` or references to equipment/system protection;
- `StatusSetComponent`;
- `WeaponRuntimeComponent`/mounted weapon state;
- hit-zone target linkage;
- faction/relationship read reference;
- later TA-8 tactical AI components.

Not every combat participant uses biological Health.

## 3. Combat State

`CombatActorComponent` can expose runtime engagement capability states equivalent to the GDS concepts:

- Unaware;
- Alerted;
- Engaged;
- Disengaging;
- Disabled/Incapacitated;
- Defeated/Destroyed.

TA-8 owns detailed AI transition logic. TA-7 only provides the runtime state slots/contracts.

## 4. Weapon Runtime

A `WeaponRuntimeComponent` stores only active-use state required at high frequency, such as:

- equipped/mounted item or weapon definition reference;
- fire mode;
- current fire/recovery/cooldown state;
- reload state/progress;
- internal magazine/charge state when that facet is leased to active runtime;
- heat state;
- recoil/dispersion state required for deterministic firing;
- jam/disabled state when defined.

Persistent item identity, Condition, modifications and physical ammunition ownership remain with their owning Inventory/Equipment/resource records except for explicitly leased active facets.

## 5. Fire Pipeline

A runtime fire request follows the GDS-9 order:

1. validate actor/weapon can fire;
2. atomically consume/commit required ammunition/energy/use state at the defined fire milestone;
3. create trace/projectile/melee intent;
4. TA-5 resolves physical trajectory/contact;
5. TA-7/Combat constructs normalized `HitFact`/`DamageIntent`;
6. Combat resolves shield/armor/damage/status rules;
7. owning Health/Robot/Ship/Station/object domain commits consequence;
8. runtime components reflect committed result;
9. weapon recovery/recoil/cooldown proceeds.

A miss never refunds a round already validly fired.

## 6. Physical Hit Facts

TA-5 collision/query output is normalized into project facts containing only required semantic data, such as:

- attacker/source identity;
- target runtime/persistent binding;
- hit position/normal;
- hit-zone/weak-point identity where applicable;
- collision material;
- relative impulse/trajectory information;
- attack instance/projectile identity;
- Simulation Time/tick ordering information.

Jolt callbacks do not edit Health/status/components directly.

## 7. Damage Routing

Combat resolution produces a typed consequence request/result. Biological Health, robot structure, ship subsystem state, station damage and destructible object state remain owned by their respective gameplay domains/components/leases.

Runtime Health/shield components may be authoritative only for facets explicitly leased from those persistent owners.

## 8. Friendly Fire

No runtime entity filter removes allied actors from physical hit resolution solely because of faction relation. TA-5 collision and GDS-9 friendly-fire rules remain authoritative.

## 9. Hit Zones

Hit zones are runtime query/collision targets linked to one semantic target and authored zone definition.

A hit-zone runtime shape never owns Health. It reports zone identity to the combat resolver.

## 10. Shield Runtime

When a personal/ship shield requires high-frequency local update, its current capacity, recharge delay, disruption and active state may live in a leased `ShieldRuntimeComponent`.

Shield energy/source dependencies remain real resource/equipment state.

## 11. StatusSetComponent

A status set contains runtime `StatusInstance`s with:

- stable status definition ContentId;
- source identity/reference;
- target entity;
- stack/intensity;
- remaining Simulation Time or persistent-condition marker;
- periodic next-tick deadline;
- removal/condition state;
- instance-local deterministic ordering key.

## 12. Status Application

Status application is deterministic from the owning combat/status rules. No generic random-proc framework is introduced.

Reapplication follows the definition's explicit stacking model:

- Refresh;
- Intensity Stack;
- Independent Stack;
- Accumulation Threshold.

## 13. Periodic Status Processing

Periodic statuses schedule fixed Simulation-Time deadlines. Each periodic tick creates the same typed damage/consequence path as an authored attack where appropriate.

Render FPS does not alter tick count or total damage.

True Pause/Hard Streaming Hold stop progress because Simulation Time stops.

## 14. Status Persistence

For persistent actors, any status that must survive save/deactivation is part of the leased/persistent actor state and is returned during deactivation/export.

Transient-only statuses on transient entities disappear only when the owning entity's semantic lifetime ends.

Extraction does not implicitly clear statuses.

## 15. Incapacitation / Disable / Destruction

Health zero or system-disable thresholds first produce the owning semantic outcome:

- biological Incapacitation;
- machine Disable;
- system/structure Disable;
- Destruction.

Runtime lifecycle is updated after commit. TA-7 does not equate `Health == 0` with immediate entity deletion.

## 16. Attack Instance Identity

Melee swings, explosions and other multi-contact attacks use a runtime `AttackInstanceId`/ordinal scoped to the scene/tick sequence so one authored attack can deduplicate repeated contact with the same target where required.

This identity is runtime-local unless an owning persistent specification explicitly requires otherwise.

## 17. Deterministic Conflict Ordering

When multiple combat consequences occur in the same fixed step, processing uses the TA-1/GDS deterministic event priority plus stable source/target/attack tie-breakers. Render order and container iteration never choose the winner.

## 18. Save

Stable save export preserves all gameplay-relevant combat state required by GDS: active Health/shield, persistent statuses, weapon ammunition/charge/heat where applicable, equipment condition and defeated/disabled persistent outcomes. Runtime handles are not saved.

## 19. Debugging

Combat diagnostics expose attack instance, source/target bindings, hit zone, damage packet stages, shield/armor resolution, status application/stacking and final committed outcome.

## 20. Tests

Tests cover fire-cost commit, no miss refund, friendly-fire physical blocking, hit-zone routing, shield-before-Health ordering, deterministic status stacking/ticks, Health-zero without immediate deletion, stale target rejection and save/deactivation persistence.

## 21. Non-Goals

No random critical-hit layer, no generic RPG hit roll, no physics-owned damage, no component-owned persistent loot/economy mutation, no blanket boss/status immunity and no immediate despawn on defeat.

## 22. Dependencies

Depends on GDS-9, GDS-5 Health/Equipment, TA-2 transactions/leases, TA-5 collision/projectiles and TA-8 future AI.

## 23. Open Questions

None.
