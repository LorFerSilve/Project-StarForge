# Project StarForge — Authoritative Game Design Specification

This directory is the **single source of truth for game design** in Project StarForge.

A gameplay rule, mechanic, interaction, failure state, visual requirement, or progression rule is authoritative only when it is defined in this specification or in an explicitly accepted design decision referenced by it.

## Current Status

The specification is in the **pre-implementation design phase**.

No subsystem is considered implementation-ready merely because its concept is mentioned. Each subsystem must progress through the design states defined in [Design Authority](00_design_authority.md).

## Core Documents

- [Design Authority](00_design_authority.md)
- [Game Overview](01_game_overview.md)
- [Global Game Rules](02_global_game_rules.md)
- [GDS Roadmap](GDS_ROADMAP.md)
- [Structure Audit](STRUCTURE_AUDIT.md)
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

Historical, superseded design material is preserved under [`history/`](history/) and is not authoritative.

## Documentation Rule

Every rule has exactly **one authoritative home**.

Other documents may reference that rule but must not redefine it. This prevents contradictory specifications and documentation drift.

## Implementation Gate

Implementation of a subsystem may begin only when:

1. its authoritative specification has status **Design Complete** or **Implementation Locked**;
2. all required dependencies are sufficiently specified;
3. no unresolved design question can force the implementer to invent gameplay behavior;
4. known edge cases and failure states are specified;
5. tuneable balancing parameters are clearly separated from fixed game rules.

## Current Sequence

The active sequencing authority is [GDS Roadmap](GDS_ROADMAP.md).

The project is currently completing **GDS-0** and **GDS-1** before detailed **GDS-2 Home Space Station** work.
