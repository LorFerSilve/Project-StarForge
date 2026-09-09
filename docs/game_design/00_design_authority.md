# Design Authority

> **Status:** Design Complete  
> **Authority:** Project-wide design governance  
> **Applies to:** All game-design documentation

## 1. Purpose

This document defines how Project StarForge's game design is specified, changed, reviewed, and handed to implementation.

Its purpose is to prevent gameplay behavior from being improvised during programming.

## 2. Single Source of Truth

The authoritative game-design specification under `docs/game_design/` is the source of truth for intended gameplay behavior.

If implementation and specification disagree, the discrepancy must be resolved explicitly. Code does not silently redefine design.

## 3. One Authoritative Home Per Rule

Every mechanic or gameplay rule must have exactly one authoritative location.

A different document may:

- link to the authoritative rule;
- explain how its own subsystem depends on the rule;
- describe subsystem-specific consequences.

It must not independently redefine the same rule.

## 4. Design Statuses

Every subsystem specification uses one of these statuses:

### Draft

The system is actively being designed. Open questions and unresolved rules are allowed.

### Under Review

The system has a complete proposed behavior and is being checked for contradictions, missing cases, and unwanted interactions.

### Design Complete

All gameplay behavior required to implement the system is defined. No implementer should need to invent intended behavior.

Tuneable balancing values may remain adjustable.

### Implementation Locked

The design is the implementation contract.

Changing fixed behavior requires an explicit design change and corresponding specification update.

## 5. Definition of Design Complete

A subsystem may be marked **Design Complete** only when all applicable areas are defined:

- purpose;
- player-facing behavior;
- participating entities;
- terminology;
- core rules;
- states and state transitions;
- inputs and outputs;
- player interactions;
- automation;
- resource interactions;
- dependencies;
- progression;
- failure states;
- recovery behavior;
- edge cases;
- UI requirements;
- visual feedback;
- audio feedback;
- persistence behavior;
- tuneable parameters;
- explicit non-goals.

If an unresolved question can change implementation behavior, the subsystem is not Design Complete.

## 6. Fixed Rules vs Tuneable Parameters

The specification distinguishes between **game rules** and **tuneable values**.

Example fixed rule:

> A weapon reload cannot fire a normal shot until the reload has completed or has been interrupted by an explicitly permitted action.

Example tuneable values:

- reload duration;
- magazine capacity;
- damage;
- recoil;
- fire rate.

Tuneable values may be modified through balancing and playtesting without redesigning the underlying mechanic.

## 7. Uncertainty Language

Words such as the following are not acceptable in Design Complete rules when they represent unresolved behavior:

- maybe;
- perhaps;
- possibly;
- could;
- might;
- to be decided;
- something like;
- optional, unless optionality itself is explicitly part of the design.

During Draft status, unresolved questions must be recorded explicitly rather than hidden behind vague wording.

## 8. Edge-Case Requirement

Specifications must describe not only normal behavior but also relevant abnormal states.

Examples include:

- insufficient power;
- full inventory;
- disconnected infrastructure;
- missing crew specialist;
- simultaneous failures;
- interrupted mission;
- destroyed target;
- invalid placement;
- unavailable path;
- loss of communication;
- resource shortage.

## 9. Presentation Is Part of Design

When relevant, a system specification must state how its important states are communicated through:

- geometry or animation;
- lighting;
- VFX;
- UI;
- sound;
- alarms;
- interaction prompts.

A system is not fully specified if important player-facing states exist without understandable feedback.

## 10. No Premature Implementation

Project StarForge does not begin implementation merely because a mechanic sounds desirable.

Design precedes implementation.

The project deliberately avoids building speculative engine systems before approved gameplay requires them.

## 11. Change Control

When an established design rule changes:

1. update the authoritative subsystem specification;
2. identify affected cross-references;
3. record a major design decision when the rationale is strategically important;
4. update implementation only after the design change is accepted.

## 12. No Hidden Design in Code

Implementation details may solve technical problems, but they must not silently introduce new gameplay rules.

If a technical constraint requires a gameplay change, that change must return to design review.

## 13. Scope Control

The specification must explicitly identify non-goals where ambiguity could cause scope growth.

A feature is not implicitly required because a similar commercial game contains it.

## 14. Final Principle

A programmer implementing an approved subsystem should be able to answer:

> "What should happen in this gameplay situation?"

from the specification, without inventing the intended game design.
