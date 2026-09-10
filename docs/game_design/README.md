# Project StarForge — Authoritative Game Design Specification

This directory is the **single source of truth for game design** in Project StarForge.

A gameplay rule, mechanic, interaction, failure state, visual requirement, or progression rule is authoritative only when it is defined in this specification or in an explicitly accepted design decision referenced by it.

## Current Status

The project has completed **GDS-0 through GDS-14**. The authoritative gameplay and presentation specification is formally **Design Complete** under [Design Authority](00_design_authority.md).

The GDS-14 promotion sweep traversed **156 authoritative owning specifications**: **156/156 are Design Complete**, all **14 authoritative domains** are synchronized, and the final hard-blocker count is **0**.

Historical first-pass `CROSS_VALIDATION.md` files remain audit evidence and do not override the promoted owning contracts.

Gameplay implementation has **not** begun. The closed design gate now permits the separate **technical architecture and implementation-roadmap phase** before C++/OpenGL gameplay construction begins.

## Core Documents

- [Design Authority](00_design_authority.md)
- [Game Overview](01_game_overview.md)
- [Global Game Rules](02_global_game_rules.md)
- [GDS Roadmap](GDS_ROADMAP.md)
- [Structure Audit](STRUCTURE_AUDIT.md)
- [GDS-14 Audit](audit/)
- [Glossary](GLOSSARY.md)
- [Design Decisions](DESIGN_DECISIONS.md)
- [Subsystem Specification Template](SPECIFICATION_TEMPLATE.md)

## Authoritative Design Domains

- [World](world/)
- [Narrative](narrative/)
- [Player](player/)
- [Combat](combat/)
- [Home Station](station/)
- [Crew](crew/)
- [Spacecraft](spacecraft/)
- [Resources, Crafting & Research](resources/)
- [Missions](missions/)
- [Robots](robots/)
- [Raids](raids/)
- [Progression](progression/)
- [Cross-Cutting Systems](systems/)
- [Presentation](presentation/)

Historical/superseded design material remains under [`history/`](history/) and is not authoritative.

## Documentation Rule

Every rule has exactly **one authoritative home**.

Other documents may reference that rule but must not independently redefine it.

GDS-14 additionally establishes that generic repeated terms/states are domain-qualified. For example, `PowerLoadPriority::P1` and `AlarmPriority::P1` are not one universal priority value.

## GDS-14 Result

The final cross-system audit is indexed under [`audit/README.md`](audit/README.md).

It includes:

- project-wide authority/namespace validation;
- 25 compound multi-system scenarios;
- transaction/persistence/anti-duplication validation;
- presentation/accessibility validation;
- specification-maturity validation;
- a consolidated final report.

The audit discovered five cross-system issues and resolved them in authoritative design:

1. **Time authority:** Simulation Time is now the sole gameplay-progression clock;
2. **Horizon recovery:** Critical Recovery and Recovery Grace now have explicit state/trigger semantics;
3. **Communications:** Strategic communication and remote control now have one authoritative infrastructure/knowledge contract;
4. **External defeat:** Recovery Transit/destination rules prevent unsafe/instant defeat teleport behavior;
5. **Namespaces:** repeated generic state/priority names are explicitly typed/qualified.

Cross-system authority, compound scenarios, transactions/persistence, failure/softlock behavior, economy/progression/difficulty, presentation/accessibility, and orphan-mechanic checks all pass after those corrections.

See [`audit/FINAL_REPORT.md`](audit/FINAL_REPORT.md).

## Post-GDS Handoff

The former specification-maturity blocker is closed. Every authoritative owning specification now meets the project-defined `Design Complete` standard.

`Design Complete` is intentionally distinct from `Implementation Locked`. The next phase translates approved gameplay behavior into technical architecture, subsystem boundaries, data contracts, implementation sequencing, and engineering validation gates. Specific contracts become `Implementation Locked` only when they are explicitly handed to implementation under change control.

## Implementation Gate

Implementation of a subsystem may begin only when:

1. its authoritative specification is **Design Complete** or **Implementation Locked**;
2. all required dependencies are sufficiently specified;
3. no unresolved design question forces gameplay invention during coding;
4. relevant edge cases/failure states/persistence/presentation are defined;
5. tuneable balancing values are distinguished from fixed rules;
6. the GDS-14 maturity/promotion gate has passed for that contract.

## Current Sequence

Current state:

GDS-0 through GDS-13  
→ first-pass complete  
→ GDS-14 cross-system audit complete  
→ cross-system findings resolved  
→ Design Complete Promotion Sweep **PASS**  
→ authoritative GDS **DESIGN COMPLETE**  
→ **next: technical architecture and implementation roadmap**  
→ after those gates: C++/OpenGL scaffolding and gameplay implementation.

**Gameplay implementation has not begun.**
