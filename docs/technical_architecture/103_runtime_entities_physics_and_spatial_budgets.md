# TA-13 — Runtime Entity, Physics, and Spatial Budgets

> **Status:** Architecture Complete  
> **Authority:** Active-scene entity capacities, high-frequency actor/projectile limits, physics/query/contact targets, spatial-index budgets, and overload behavior

## 1. Purpose

TA-7 and TA-5 deliberately define correct entity/physics architecture without numeric scale. TA-13 fixes an initial scale envelope large enough for StarForge gameplay while keeping the custom runtime measurable and bounded.

## 2. Runtime Registry Capacity

Per active SceneInstance:

- soft target total live runtime entities: **<= 8,192**;
- semantic hard registry capacity: **16,384** live slots;
- Closing/PendingDestroy entities count until reclaimed.

Content/runtime state that requires more than the hard capacity cannot silently omit entities. Scene activation/content validation must fail or be redesigned.

## 3. High-Frequency Actor Budget

At one time the active scene targets:

- detailed mobile actors: **<= 256**;
- simultaneously combat-engaged actors: **<= 128**;
- player-controlled actor: **1**;
- actively simulated player squad robots: **<= 32**;
- high-frequency crew actors in local scene: **<= 128**.

These are implementation-scale budgets, not GDS population caps. Persistent off-screen identities can exceed local high-frequency representation.

## 4. Interactable Runtime Budget

Per active scene:

- live interactable/world-object proxies soft target: **<= 2,048**;
- temporary interaction/trigger helpers soft target: **<= 1,024**;
- hard combined interactable/trigger helper cap: **4,096**.

Static visual geometry does not require one RuntimeEntity per mesh.

## 5. Projectile and Attack-Volume Budget

Targets:

- swept/project-owned active projectiles: **<= 1,024**;
- dynamic rigid projectiles: **<= 256**;
- temporary melee/attack volumes: **<= 512**;
- short-lived normalized combat facts buffered in one tick: **<= 4,096** ordinary target.

If a weapon/effect would exceed projectile budget, authored fire behavior must be optimized/batched semantically before shipping; runtime may not delete gameplay projectiles based on distance/FPS.

## 6. Physics Body Budgets

Per active PhysicsWorld:

- total registered bodies soft target: **<= 8,192**;
- active dynamic + kinematic bodies: **<= 2,048**;
- dynamic rigid bodies target: **<= 1,024**;
- sensors/triggers target: **<= 2,048**;
- static geometry should use cooked compound/mesh representation rather than one body per render mesh.

A semantic safety cap of **16,384 physics bodies** exists for content/runtime validation.

## 7. Physics Step Time

On the TA-16 reference CPU in benchmark scenes:

- Jolt/adapter physics step + contact collection p50: **<= 1.2 ms**;
- p95: **<= 2.0 ms**;
- p99: **<= 3.0 ms**.

These measurements include StarForge preparation/result normalization but exclude later combat/domain consequence resolution.

## 8. Contact Pair Budget

Ordinary active contact manifolds target **<= 4,096**.

A scene with > **8,192** simultaneous meaningful contact pairs is a content/performance red state and must be investigated. Contacts are not dropped to restore frame rate.

## 9. Physics Query Budget

Normal fixed-tick target:

- total gameplay physics queries: **<= 4,096/tick**;
- expensive shape sweeps: **<= 1,024/tick**;
- broad overlap queries: **<= 1,024/tick**;
- camera/presentation-only queries are budgeted separately and cannot starve gameplay queries.

Query counts above target are profiled by caller.

## 10. CharacterMotor Budget

Detailed CharacterMotor instances target **<= 192** simultaneously.

Each character must batch/cache ground/support/clearance tests where correctness permits instead of issuing unbounded duplicate queries from separate systems.

## 11. Spacecraft Budget

One local-space scene targets:

- fully dynamic spacecraft: **<= 64**;
- high-detail combat spacecraft: **<= 32**;
- docking constraints concurrently active: **<= 32**.

Far/noninteractive strategic ships remain persistent/off-screen representations rather than full local rigid bodies.

## 12. Spatial Index Capacity

The TA-3 gameplay spatial grid targets:

- live indexed entries: **<= 16,384**;
- entries requiring position-cell update in one tick: **<= 4,096**;
- candidate results from one ordinary query: **<= 1,024**, after which caller-specific bounded filtering is required.

A query whose semantics truly require all entries uses a dedicated deterministic iteration path rather than overflowing a fixed result array.

## 13. Visibility Is Separate

Renderer visibility counts do not change these gameplay budgets.

A culled actor still counts as active if its simulation is active. Conversely, static visible geometry can be rendered without consuming high-frequency gameplay-entity capacity.

## 14. Collision LOD Rule

Performance tiers may choose approved cooked collision representations only when they are semantically equivalent for gameplay.

There is no graphics-quality option that removes collision or widens/narrows access paths.

## 15. Sleeping

Jolt sleeping can reduce solver cost for eligible Dynamic bodies.

Sleep is a physics optimization only:

- persistent state remains;
- wake triggers are deterministic physical/gameplay events;
- sleeping cannot suppress required hazards, timers or mission state.

## 16. Debris and Cosmetic Physics

Decorative debris may use presentation-only bounded effects instead of authoritative physics when it has no gameplay interaction.

Gameplay-relevant debris/cargo remains under the ordinary physical budgets and cannot be silently converted to cosmetic state under load.

## 17. Pooling

Runtime may pool/reuse transient component/projectile storage after semantic destruction and generation invalidation.

Pooling does not preserve stale identity or skip lifecycle/deferred-destruction rules.

## 18. Allocation Policy

Per-tick gameplay loops should avoid general heap churn for common transient facts. Bounded arenas/pools are appropriate for:

- contact normalization;
- combat facts;
- interaction candidate lists;
- command buffers;
- deferred destruction lists.

Arena overflow is a diagnostic/controlled fallback, never permission to discard committed facts.

## 19. Origin Rebase Cost

Floating-origin rebase is infrequent and targets **<= 4.0 ms** main-thread cost on representative maximum active-scene budgets.

It occurs only at Stable Simulation Boundary and may exceed ordinary per-tick budget rarely, but must never create synthetic contact/damage.

## 20. Activation/Deactivation Throughput

Ordinary live streaming target:

- activate/deactivate **<= 128 runtime entities per rendered frame**;
- main-thread structural activation/deactivation work **<= 1.5 ms/frame p95**.

Larger scene transitions occur under explicit loading/hold states and can process more work without pretending to meet live-gameplay frame budgets.

## 21. Pressure Behavior

When entity/physics targets are exceeded:

- stop speculative activations outside deterministic required set;
- evict/deactivate only entities already eligible under TA-3 gameplay rules;
- reduce decorative presentation physics;
- diagnose authored density;
- enter Hard Streaming Hold/activation failure if required correct state cannot fit.

The runtime does not despawn required enemies/loot/robots to meet budget.

## 22. Metrics

Development telemetry records:

- entities by component/archetype-like signature;
- Closing/PendingDestroy age;
- physics bodies by motion/layer;
- active/sleeping bodies;
- contacts;
- query counts/time by caller;
- CharacterMotor count/time;
- projectile count/sweep time;
- spatial entries/update/query candidates;
- activation/deactivation rate.

## 23. Validation Scenarios

TA-13 benchmark scenes must include:

- dense Horizon interior with crew/robots;
- raid boarding with doors/debris/projectiles;
- local-space ship combat;
- decompression with dynamic props;
- mixed scene activation/deactivation churn.

## 24. Non-Goals

No universal ECS hard limit for persistent state, no gameplay entity culling from camera visibility, no dropping contacts/projectiles for frame rate, no collision changes per graphics preset.

## 25. Open Questions

None in the TA-13 entity/physics/spatial budget architecture.