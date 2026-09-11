# TA-7 — Component Storage and Query Model

> **Status:** Architecture Complete  
> **Authority:** Runtime component storage, structural mutation, component access, deterministic system queries, and component lifetime

## 1. Purpose

TA-7 uses typed runtime components for local high-frequency state without converting persistent StarForge domains into one universal ECS.

## 2. Storage Model

Each component type owns a dedicated typed pool under the active `RuntimeEntityRegistry`/scene runtime.

The baseline favors sparse-set / packed-array style pools or an equivalent typed structure with:

- dense iteration over present components;
- O(1)-class handle-to-component lookup;
- generation-safe entity validation;
- explicit structural add/remove;
- no polymorphic component base required.

The exact container implementation is tuneable if these contracts remain true.

## 3. Why Not Archetype Migration as Baseline

StarForge does not require a general-purpose archetype ECS baseline.

Entity component sets change relatively infrequently compared with per-tick component updates, and the project values simple explicit ownership over a generic engine feature set.

Archetype/chunk migration may be reconsidered only if TA-13 profiling demonstrates a real need.

## 4. Component Categories

Conceptual component families include:

### Spatial/runtime

- `TransformComponent`;
- `VelocityComponent` where project-owned outside a backend;
- `PhysicsProxyComponent`;
- `RenderProxyComponent`;
- `AudioProxyComponent`.

### Actor

- `ActorComponent`;
- `HealthRuntimeComponent`;
- `ShieldRuntimeComponent`;
- `EquipmentRuntimeComponent`;
- `CombatStateComponent`;
- `StatusSetComponent`;
- later TA-8 AI/navigation components.

### Player

- `PlayerControlComponent`;
- `PlayerInteractionComponent`;
- local camera/view-facing state references;
- active sustained-action state.

### World/interactable

- `InteractableComponent`;
- `WorldItemProxyComponent`;
- `ContainerProxyComponent`;
- `DoorRuntimeComponent`;
- `HazardRuntimeComponent`;
- `MissionTargetProxyComponent`.

### Combat

- `WeaponRuntimeComponent`;
- `ProjectileComponent`;
- `HitZoneComponent` or target linkage;
- `StatusSetComponent`;
- temporary attack/action state.

Component names are technical architecture labels; exact implementation schemas can refine them under the owning contracts.

## 5. Component Authority

A component is authoritative only for the runtime facet explicitly assigned to it.

Examples:

- `TransformComponent` can own an active leased local transform;
- `HealthRuntimeComponent` can own active leased current Health for a persistent actor;
- `WorldItemProxyComponent` does **not** own persistent inventory ownership merely because it represents an item in the scene;
- `MissionTargetProxyComponent` does **not** own objective completion.

Every component type documents whether its fields are:

- runtime authoritative leased state;
- transient runtime-only state;
- derived/cache state;
- immutable references to persistent/content data.

## 6. Structural Mutation

Adding/removing components is structural mutation and occurs only during approved simulation structural-mutation phases.

Systems do not add/remove components while another system is iterating the same affected pool unless the operation is buffered for later application.

## 7. Deferred Structural Command Buffer

Runtime structural changes use a typed `RuntimeStructuralCommandBuffer` or equivalent.

Typical requests:

- Spawn entity;
- Add component;
- Remove component;
- Mark Closing;
- Mark PendingDestroy;
- create/destroy backend proxy;
- change persistent binding only through activation/deactivation flow.

Commands are validated and applied deterministically at the designated boundary.

## 8. Direct Value Mutation

Within an owning system's scheduled phase, ordinary mutable fields of already-present components may be updated directly on the simulation thread when no cross-domain transaction is required.

Structural component changes and cross-domain authoritative effects still use their dedicated commit boundaries.

## 9. Component Access

Systems request typed component references/views.

A system cannot obtain an untyped `void*` bag and reinterpret arbitrary components.

Where a component reference can be invalidated by structural changes, it is valid only for the documented phase/scope.

Long-lived references use `RuntimeEntityHandle` and re-resolve.

## 10. Query Model

Systems may iterate intersections such as conceptually:

```text
ActorComponent + TransformComponent + HealthRuntimeComponent
```

or:

```text
ProjectileComponent + TransformComponent
```

The implementation may optimize these queries using a smallest-pool-first strategy or cached typed views.

## 11. Determinism

Gameplay-significant query order must be stable.

Packed-array swap-remove order alone is not allowed to decide conflicting gameplay results.

Where processing is commutative, packed order may be used for performance.

Where ordering matters, systems explicitly sort/use stable semantic keys.

## 12. Component Removal

Removing a component:

- cannot silently discard persistent leased state;
- must first return/commit required state to the owning persistent domain if the component owns a leased facet;
- must release backend proxies through their owning adapters;
- invalidates only the removed runtime facet, not the entity's persistent identity.

## 13. Component Initialization

Component initialization uses prepared descriptors/snapshots rather than querying arbitrary mutable stores during construction.

This keeps activation atomic and testable.

## 14. Content Data

Immutable static definition data is referenced through `ContentId`/resolved content handles rather than copied into every component unless a small cached value is required for performance.

Content definitions are not runtime entity identity.

## 15. Derived Components/Caches

Components may contain reconstructible cached values such as:

- effective equipment protection summary;
- cached interaction range;
- derived combat resistance summary;
- render/physics local mapping.

Caches carry source revision/generation where stale use would be dangerous.

## 16. No Component-Owned Cross-Domain Transactions

A component does not directly mutate another persistent domain.

Example:

```text
ProjectileComponent hit
-> produces HitFact
-> Combat validates/resolves
-> Inventory/Health/Station/etc. commit through owning boundaries
```

not:

```text
ProjectileComponent
-> directly edits target persistent record
```

## 17. Memory/Lifetime

Component pools are owned by the scene runtime and destroyed with the scene.

Pool capacity can grow and recycle memory, but stale external references are protected by entity generation/phase rules.

Numeric reserve/capacity budgets belong to TA-13.

## 18. Worker Use

Workers receive immutable copies/snapshots or read-only stable views with explicit lifetime/revision tokens.

They cannot mutate live component pools.

Prepared worker results must revalidate SceneGeneration, entity generation, and relevant revisions before commit.

## 19. Debugging

Development tools expose:

- component counts/capacity;
- entities per component;
- add/remove command queues;
- stale access detection in debug builds where practical;
- source persistent binding/revision;
- component authority classification.

## 20. Test Requirements

Tests cover:

- typed add/remove/get;
- stale entity lookup;
- component cleanup on destroy;
- deterministic ordered queries where required;
- buffered structural mutation;
- activation rollback leaving no components;
- leased state not discarded by component removal;
- worker stale-result rejection.

## 21. Explicit Non-Goals

The baseline does not require:

- runtime reflection for every component;
- generic serialization of component memory;
- arbitrary third-party plugins defining components;
- archetype/chunk ECS complexity before profiling justifies it;
- one component system owning strategic persistent state.

## 22. Dependencies

Depends on `44_runtime_entity_registry_and_handles.md`, TA-2 activation/revisions, TA-1 simulation phases, and later TA-13 budgets.

## 23. Open Questions

None in the TA-7 component-storage baseline.
