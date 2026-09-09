# Project StarForge — Authoritative Game Design Specification

This directory is the **single source of truth for game design** in Project StarForge.

A gameplay rule, mechanic, interaction, failure state, visual requirement, or progression rule is authoritative only when it is defined in this specification or in an explicitly accepted design decision referenced by it.

## Current Status

The specification is in the **pre-implementation design phase**.

No subsystem is considered implementation-ready merely because its concept is mentioned. Each subsystem must progress through the design states defined in [Design Authority](00_design_authority.md).

## Core Documents

- [Design Authority](00_design_authority.md)
- [Game Overview](01_game_overview.md)
- [Glossary](GLOSSARY.md)
- [Design Decisions](DESIGN_DECISIONS.md)
- [Subsystem Specification Template](SPECIFICATION_TEMPLATE.md)

## Planned Design Domains

The specification will be decomposed into authoritative thematic domains:

- `world/`
- `player/`
- `combat/`
- `station/`
- `crew/`
- `spacecraft/`
- `resources/`
- `missions/`
- `robots/`
- `raids/`
- `progression/`
- `systems/`
- `presentation/`

These directories are created progressively as their first authoritative specifications are introduced. Empty directories are intentionally not tracked.

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
