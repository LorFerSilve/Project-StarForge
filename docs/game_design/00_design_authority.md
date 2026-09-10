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

## 7. Uncertainty and Modal Language

Words such as the following are not acceptable in Design Complete rules when they represent unresolved behavior:

- maybe;
- perhaps;
- possibly;
- could;
- might;
- to be decided;
- something like;
- optional, unless optionality itself is explicitly part of the design.

A Design Complete specification **may** still use modal words such as `can`, `may`, `possible`, or `optional` when they have one of these precise meanings:

1. **Player choice** — the player is explicitly permitted, but not required, to perform the action;
2. **authored data variation** — a concrete item, target, mission, location, faction, component, or content definition selects which listed behavior/capability applies;
3. **configuration/policy variation** — an explicit setting, loadout, automation policy, or installed capability determines the result;
4. **conditional physical possibility** — the action is available only when documented prerequisites are true;
5. **tuneable presentation/balance variation** — the rule is fixed while an explicitly named tuneable value or presentation parameter varies;
6. **explicit non-baseline future scope** — the text states that a feature is not part of the baseline and requires a later accepted design change before implementation.

In every case, modal wording describes **designed variability**. It never grants the implementer discretion to choose an unspecified gameplay behavior.

If a sentence containing modal wording does not identify or inherit a deterministic owner for that variation, it remains unresolved and blocks Design Complete status.

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

## 14. Design-Complete Interpretation Rule

For a specification marked **Design Complete**:

- omitted capabilities are absent unless another authoritative specification explicitly owns them;
- examples illustrate existing rules and do not create additional mechanics by implication;
- implementation may choose algorithms, data structures, rendering techniques, optimization strategies, and other technical details only when those choices preserve every observable gameplay rule;
- authored content may choose among explicitly permitted variants, but content authors may not invent a new gameplay rule without extending the appropriate authoritative specification;
- tuneable values may change through balancing without changing their semantic relationship or state-transition rule.

This rule prevents ambiguity from being reintroduced merely because a specification cannot enumerate every future content instance.

## 15. Final Principle

A programmer implementing an approved subsystem should be able to answer:

> "What should happen in this gameplay situation?"

from the specification, without inventing the intended game design.
