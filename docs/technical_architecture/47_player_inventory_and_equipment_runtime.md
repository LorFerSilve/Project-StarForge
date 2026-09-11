# TA-7 — Player, Inventory, and Equipment Runtime

> **Status:** Architecture Complete
> **Authority:** Locally controlled player runtime state, active equipment/use state, inventory-facing world proxies, quick-slot/equip references, sustained actions, and player capability derivation

## 1. Purpose

This specification maps persistent Player/Inventory/Equipment state into the active runtime without duplicating physical ownership.

## 2. Player Runtime Components

The local player runtime entity can contain:

- `PlayerControlComponent`;
- `CharacterMotor`/physics proxy linkage;
- `HealthRuntimeComponent`;
- `ShieldRuntimeComponent` where equipped;
- `EquipmentRuntimeComponent`;
- `PlayerInteractionComponent`;
- `StatusSetComponent`;
- `ActiveActionComponent`;
- camera/view-facing runtime state references.

## 3. Control State

Raw input is not owned here; TA-11 will map devices to actions. TA-7 owns the gameplay-facing local control intent/state consumed by movement, combat and interaction systems.

Control is suppressed or restricted by explicit gameplay state such as Incapacitated, Critical Health restrictions, sustained-action locks or context rules.

## 4. Health Runtime

While leased active, current biological Health and canonical Health state can live in `HealthRuntimeComponent`.

GDS-5 restrictions are derived deterministically from Health state. The runtime does not add undocumented low-health penalties.

At zero Health the component transitions only after Health owner/combat resolution commits Incapacitation semantics.

## 5. Equipment Runtime

`EquipmentRuntimeComponent` stores active-use facets and references to persistent player-owned item identities/stack identities.

It never copies an equipped item into a second physical owner.

Conceptually:

```text
Persistent UniqueItemId / stack identity
        ↓ active reference
EquipmentRuntimeComponent
```

not a duplicate item record.

## 6. Slot State

The canonical equipment slots remain GDS-5 slots. Runtime slot entries contain validated references plus use-state such as ready/swapping/reloading/using where owned by the corresponding system.

Changing slot assignment is a persistent ownership/equipment transaction; runtime presentation updates after commit.

## 7. Backpack and Quick Slots

Backpack Mass/Volume ownership stays in Inventory authority.

The active runtime consumes an immutable/revisioned inventory view and may cache:

- total carried Mass;
- Heavy Load state;
- current capacity;
- quick-slot references;
- containment eligibility summaries.

Quick slots remain references, never duplicate objects.

## 8. World Item Proxies

An item lying in the world is represented by a runtime entity/proxy tied to its actual world/container ownership record where persistence requires it.

Pickup flow:

1. target acquired physically;
2. runtime requests pickup command;
3. Inventory validates ownership/capacity/containment;
4. atomic ownership transfer commits;
5. world proxy closes/destroys or updates remaining stack quantity.

Visual disappearance before ownership commit is prohibited.

## 9. Partial Stack Pickup

For divisible stacks, runtime interaction uses the accepted quantity returned by Inventory. The source proxy updates to the remaining authoritative quantity.

## 10. Auto Pickup

Auto Pickup is a runtime proximity candidate mechanism only. It applies the same Inventory command/validation path as manual pickup and cannot bypass LOS/proximity/capacity/ownership rules.

## 11. Active Actions

Finite-time player actions use an `ActiveActionComponent` or equivalent runtime state containing:

- action kind;
- target handle/persistent target identity;
- start Simulation Time;
- progress/checkpoint state;
- required tool/equipment reference;
- interruption policy;
- source revisions where needed.

Examples include swap, reload, medical use, repair/mining/salvage and sustained interaction.

## 12. Action Commit Boundary

Progress alone does not grant the result. The owning target/domain commits the action result at its defined milestone.

If interrupted before commit, the target's retention policy applies.

## 13. Suit Energy / Life Support

Current active Suit Energy/Life-Support facets can be leased/runtime-updated when high-frequency local usage requires it, but physical item identity and equipment configuration remain persistent.

Energy consumption is deterministic Simulation-Time state and never frame-rate driven.

## 14. Equipment Condition

Active condition consequences are derived from the persistent/current item Condition profile. Runtime caches may summarize effective capability but cannot create hidden degradation rules.

## 15. Equip/Swap

Field swap uses finite `SwapTime`. Persistent slot assignment commits only at the authored transaction point. Interrupting before commit leaves the previous assignment authoritative.

Suit/Helmet/Backpack replacement requires the safe-context validations from GDS-5 and cannot be performed as a generic runtime component swap.

## 16. Incapacitation

When player becomes Incapacitated:

- direct movement/control actions stop;
- combat/tool use stops;
- ordinary inventory rearrangement stops;
- active actions are interrupted/resolved under their policies;
- player runtime entity remains until rescue/failure/recovery handoff commits.

## 17. Read Models

HUD/UI receives immutable player runtime read models combined with revisioned persistent Inventory/Equipment data. UI never directly edits components or inventory stores.

## 18. Save

Stable save capture combines active leased player facets with persistent Inventory/Equipment records. Runtime references, component indices and handles are excluded.

## 19. Tests

Tests cover equipped reference ownership, quick-slot nonduplication, pickup commit ordering, partial stack pickup, Heavy Load derivation, swap interruption, incapacitation action cancellation, suit-energy Simulation-Time behavior and active save export.

## 20. Non-Goals

No runtime-global inventory container, no duplicate item components as ownership, no instant equipment reassignment, no UI-authoritative capacity, and no passive resource regeneration.

## 21. Dependencies

Depends on TA-2 ownership/transactions, GDS-5 Player Inventory/Equipment/Health/Interaction, TA-5 CharacterMotor and TA-11 future input/UI bindings.

## 22. Open Questions

None.
