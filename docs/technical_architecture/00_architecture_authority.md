# Technical Architecture Authority

> **Status:** Design Complete  
> **Authority:** Governance for translating GDS behavior into software architecture

## 1. Purpose

This document defines how technical decisions become authoritative for Project StarForge without allowing architecture to silently redesign gameplay.

## 2. Precedence

Precedence is:

1. accepted project/game-design decisions;
2. Design Complete GDS owning specification;
3. Technical Architecture owning specification;
4. Implementation contract/code/tests;
5. comments/convenience behavior.

A lower layer may not contradict a higher layer.

## 3. Technical Architecture Scope

Technical Architecture owns:

- source/module boundaries;
- runtime ownership/lifetime;
- update-loop structure;
- concurrency/thread affinity;
- data representation;
- persistent/runtime identity representation;
- rendering architecture;
- physics integration;
- AI/navigation infrastructure;
- serialization format/transaction implementation;
- asset/content pipeline;
- input/audio/UI adapters;
- build/dependency/toolchain strategy;
- observability/testing/performance gates.

It does not own gameplay outcomes already specified by GDS.

## 4. Status Model

### Draft

Technical behavior or ownership is still incomplete.

### Under Review

The architecture is internally specified but dependency/cross-system validation remains.

### Architecture Complete

The technical contract defines all implementation-relevant ownership, dataflow, lifetime, error, threading, persistence, and validation behavior required for the subsystem.

### Implementation Locked

The Architecture Complete contract has been explicitly handed to code under change control.

## 5. Architecture Complete Criteria

A technical subsystem is Architecture Complete only when applicable areas are defined:

- responsibility;
- public boundary/API shape;
- owning module;
- data ownership;
- object/entity identity;
- lifetime;
- initialization/shutdown;
- update cadence;
- thread affinity;
- dependencies;
- error/failure handling;
- deterministic behavior requirements;
- persistence/serialization;
- hot/reload/content behavior where relevant;
- debug/diagnostics;
- test strategy;
- performance constraints;
- explicit non-goals.

## 6. No Speculative General Engine

Architecture must solve requirements that exist in the Design Complete GDS or clearly necessary supporting infrastructure.

Do not create generic systems solely because a reusable engine might want them.

Examples not allowed without a StarForge requirement:

- arbitrary runtime plugin ABI;
- generic visual scripting VM;
- generic multiplayer replication layer;
- generic editor framework intended for unrelated games;
- universal reflection/object hierarchy with no concrete consumer.

## 7. Third-Party Libraries

A third-party library is acceptable when:

- it solves commodity infrastructure rather than StarForge's differentiating gameplay;
- its boundary is wrapped where replacement/coupling risk is meaningful;
- it does not become the authority for gameplay semantics;
- licensing is compatible;
- version is pinned for reproducibility;
- failure/removal strategy is understood.

## 8. Library Encapsulation Rule

External-library types do not cross broad game-domain boundaries unless the dependency is intentionally foundational.

Examples:

- Jolt types remain inside physics integration/adapter code;
- miniaudio types remain inside audio backend code;
- GLFW types remain inside platform/input boundary code;
- OpenGL object handles remain inside renderer/render-resource code;
- GLM value types may be used as foundational math types within runtime code if explicitly adopted project-wide.

## 9. GDS Traceability

Every technical domain must identify the GDS contracts it realizes.

A significant technical decision must answer:

> Which Design Complete behavior requires this architecture?

## 10. Technical Infeasibility

If an approved GDS rule proves technically infeasible or disproportionately costly:

1. implementation does not silently change behavior;
2. the issue is documented;
3. technical alternatives are evaluated;
4. any gameplay change requires a formal GDS amendment/accepted decision;
5. architecture is then updated to match.

## 11. Fixed Architecture vs Tuneable Implementation

Architecture distinguishes fixed contracts from implementation-tuneable details.

Fixed example:

> Authoritative gameplay mutation occurs on the simulation thread at Stable Simulation Boundaries.

Tuneable examples:

- worker-thread count;
- asset-cache budget;
- streaming radius;
- graphics quality settings;
- coarse off-screen subsystem cadence.

Tuneable details may change without redesign if they preserve contract semantics.

## 12. Architecture Decisions

Project-wide architecture choices with lasting rationale are recorded in `ARCHITECTURE_DECISIONS.md`.

Subsystem detail stays in the owning architecture specification.

## 13. Implementation Lock

Implementation Lock is applied per architecture contract, not to the entire repository at once.

A locked contract can still change, but the change must:

- update the authoritative architecture document;
- update affected tests/contracts;
- identify migration/refactor impact;
- preserve or formally amend GDS behavior.

## 14. Current Gate

The GDS is Design Complete.

Technical Architecture is now authorized.

Gameplay implementation/scaffolding begins only after the relevant initial technical architecture phases and implementation roadmap are complete.