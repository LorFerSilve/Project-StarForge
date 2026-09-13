# TA-15 — Authority, Ownership, and Dependency Audit

> **Status:** Architecture Complete  
> **Authority:** Cross-validation of project authority, ownership, dependency direction, identity boundaries, and mutation rights across TA-0 through TA-14

## 1. Purpose

This audit verifies that Project StarForge has one authoritative owner for every consequential state class and that no implementation dependency requires a subsystem to bypass that ownership.

## 2. Governance Result

**PASS.**

The precedence chain from TA-0 is consistent with every later phase:

- GDS owns gameplay behavior;
- Technical Architecture owns realization;
- implementation/code/tests conform to architecture;
- tools/presentation/backends never outrank gameplay ownership.

No later TA phase grants itself permission to reinterpret a Design Complete gameplay rule.

## 3. Composition Root

`app` is the only composition root.

It may construct/wire all major modules but does not become a gameplay-rule owner.

**PASS:** no architecture requires a writable global God `GameState`, global service-locator mutation, or domain-rule dumping ground in `app`.

## 4. Authoritative Mutation

All gameplay-authoritative mutation commits on the Simulation Thread/main-thread baseline.

Workers may produce immutable results only.

Presentation/backends may report facts/results but never directly commit gameplay-domain mutation.

**PASS.**

## 5. Persistent Identity Ownership

Strongly typed persistent IDs are owned by persistent domains and survive activation/save/load.

Runtime identities are generation-checked and process/scene local:

- `RuntimeEntityHandle`;
- physics body handles;
- render resource handles;
- audio voice handles;
- navigation backend references;
- content runtime handles.

**PASS:** no runtime/backend handle is required as persistent identity.

## 6. Physical Ownership

Physical items/resources have exactly one authoritative owner at a time.

Cross-domain movement uses prepared transactions and explicit temporary transfer owners where necessary.

This rule composes across:

- player inventory;
- station storage/logistics;
- spacecraft cargo;
- manufacturing buffers;
- raid theft;
- mission extraction;
- robot commissioning;
- reward delivery.

**PASS.**

## 7. Credits and Nonphysical Ledgers

Credits/ledger state remain distinct from physical inventories and use atomic trade/reward transactions where both are involved.

**PASS:** no architecture treats currency as a physical stack or allows market/UI presentation to mutate balances directly.

## 8. Runtime Entity Ownership

The active `SceneInstance` owns one runtime registry for high-frequency local representation.

Persistent gameplay domains remain owners of long-lived identity/state. TA-2 Activation Leases make any temporarily runtime-authoritative facet explicit.

**PASS:** runtime ECS-like storage is not a second persistent world model.

## 9. Physics Ownership

Jolt/`physics` owns commodity collision/integration representation.

Gameplay owns:

- Health/damage;
- docking semantics;
- mission outcomes;
- station structure;
- traversal authorization;
- ownership;
- persistence.

**PASS:** contacts/queries become typed physical facts before gameplay consequences.

## 10. Renderer Ownership

`render` exclusively owns OpenGL object/state execution.

Gameplay/world/UI publish immutable render/presentation descriptions; rendering cannot mutate simulation truth.

**PASS.**

## 11. Navigation Ownership

Recast/Detour/project 3D-navigation infrastructure owns route computation only.

Gameplay/security/hazard/access state decides whether traversal is authorized and whether an action succeeds.

**PASS:** a valid geometric path is never permission to open/teleport through a door/airlock/security boundary.

## 12. Station Ownership

Horizon persistent station domains own topology, utilities, resources, work, policies and control state.

Physics/render/navigation are projections/consumers of committed station state.

TA-6 uses separate subsystem solvers instead of a universal resource graph.

**PASS.**

## 13. AI Ownership

AI selects intents/actions from knowledge-limited state.

AI does not own:

- target Health;
- inventory/resource mutation;
- mission resolution;
- station task completion;
- security authorization;
- persistent assignment history unless the owning gameplay policy explicitly commits it.

**PASS.**

## 14. Mission and Strategic State Ownership

Mission-domain state owns mission/opportunity/attempt/objective lifecycle.

Dynamic Events own strategic event existence/deadlines until execution is handed to specialized Mission/Defense state.

Defense Events own Horizon attacks independently of the external MissionInstance slot.

**PASS:** no duplicate quest/raid/event state machine is required.

## 15. Content Ownership

Content defines immutable/tuneable authored capabilities/configuration and runtime asset products.

Content files cannot:

- allocate persistent IDs;
- grant resources/rewards directly;
- execute arbitrary gameplay code;
- become save identity through file paths;
- change committed procedural results because of load timing.

**PASS.**

## 16. Persistence Ownership

Gameplay domains own the meaning of their state and export/import versioned DTOs.

`persistence` owns container encoding, integrity, catalogs, write/commit, staging and migration orchestration.

**PASS:** persistence does not infer/repair gameplay outcomes heuristically.

## 17. Input/UI/Audio Ownership

Input maps raw devices to semantic intents.

UI reads immutable Read Models and emits Commands.

Audio presents semantic events and is separate from gameplay hearing.

Accessibility modifies controls/presentation only within defined bounds.

**PASS.**

## 18. Testing/Diagnostics Ownership

Tests/diagnostics observe or issue explicit test/dev commands through approved boundaries.

Logging, state hashes, profilers, CI caches and debug UI never become gameplay authority.

**PASS.**

## 19. Dependency DAG

The integrated dependency direction is realizable without an ownership cycle:

```text
core
├─ platform
├─ content
├─ simulation
├─ physics interface/backend
├─ render interface/backend
├─ audio interface/backend
└─ persistence primitives

content + simulation + project interfaces
        ↓
world/runtime representation
        ↓
gameplay domain modules
        ↓
read models / presentation intents
        ↓
ui / audio presentation

app
└─ composition/wiring over all modules

tools/tests
└─ depend on public/dev interfaces; never shipping authority
```

Some concrete targets may depend sideways on narrow project-owned interfaces; that does not create semantic ownership reversal.

## 20. Backend Leakage Audit

Prohibited broad leakage remains consistent:

- Jolt types stay in physics integration;
- OpenGL handles stay in renderer;
- GLFW types stay platform/input boundary;
- miniaudio types stay audio backend;
- Recast/Detour references stay navigation adapter;
- fastgltf/KTX/meshoptimizer/glslang authoring/backend representations stay content tooling;
- FreeType/HarfBuzz internals stay text subsystem.

**PASS.**

## 21. No Required Circular Ownership

Cross-system communication is expressed through:

- typed IDs;
- immutable query/read data;
- Commands;
- committed Events/Facts;
- prepared Transactions;
- activation/deactivation DTOs;
- content IDs;
- backend-independent value types.

No domain requires direct writable ownership of another domain's store to implement its GDS behavior.

**PASS.**

## 22. TA-16 Boundary

TA-16 may choose exact CMake target decomposition and interface-library spelling, but it must preserve this audit's ownership/DAG constraints.

That is an implementation lock item, not an unresolved architecture question.

## 23. Verdict

```text
Authority precedence: PASS
Single-owner state model: PASS
Persistent/runtime identity separation: PASS
Backend encapsulation: PASS
Mutation rights: PASS
Dependency direction: PASS
Required circular ownership: NONE
Blocking finding: NONE
```
