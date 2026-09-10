# Project StarForge — Authoritative Game Design Specification

This directory is the **single source of truth for game design** in Project StarForge.

A gameplay rule, mechanic, interaction, failure state, visual requirement, or progression rule is authoritative only when it is defined in this specification or in an explicitly accepted design decision referenced by it.

## Current Status

The project remains in the **pre-implementation design/governance phase**.

First-pass design through GDS-13 is complete and the GDS-14 whole-project cross-system audit has been executed.

The cross-system audit is green after documented corrections, but the complete GDS is **not yet formally Design Complete**, because authoritative child specifications still require an explicit per-file maturity/status promotion sweep under [Design Authority](00_design_authority.md).

No gameplay development is authorized yet.

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

## Why Development Still Does Not Start

[Design Authority](00_design_authority.md) defines **Draft**, **Under Review**, **Design Complete**, and **Implementation Locked** as meaningful specification states.

Many authoritative child documents still explicitly carry Draft/Under Review status.

Therefore a directory-level green audit cannot silently convert those files into implementation contracts.

The remaining design-governance task is the **GDS-14 Design Complete Promotion Sweep** documented in:

- [`audit/SPECIFICATION_MATURITY_AUDIT.md`](audit/SPECIFICATION_MATURITY_AUDIT.md);
- [GDS Roadmap](GDS_ROADMAP.md).

That sweep must inspect each authoritative child specification, resolve remaining implementation-relevant ambiguous/stale Draft wording, normalize cross-cutting terminology, and promote only passing files to **Design Complete**.

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
→ GDS-14 cross-system audit **complete**  
→ five cross-system findings **resolved**  
→ Design Complete Promotion Sweep **next**  
→ only after that: technical architecture / implementation roadmap / scaffolding / development.

**Gameplay implementation has not begun and must not begin at the current project state.**
