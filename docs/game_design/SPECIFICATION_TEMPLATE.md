# Subsystem Specification Template

> **Status:** Template  
> **Authority:** Documentation structure only

Use this structure for authoritative subsystem specifications. Sections that truly do not apply may be marked **Not Applicable** with a short reason rather than silently omitted.

---

# <Subsystem Name>

> **Status:** Draft  
> **Authority:** <authoritative scope>

## 1. Purpose

Define why the subsystem exists and what gameplay problem it solves.

## 2. Player Experience

Describe what the player perceives, understands, and does when interacting with this system.

## 3. Terminology

Define subsystem-specific terms.

## 4. Participating Entities

List the objects, characters, components, resources, or networks that participate.

## 5. Core Rules

Define normative gameplay behavior.

Avoid unresolved ambiguity.

## 6. States

Enumerate relevant states.

## 7. State Transitions

Define what causes transitions and what happens when they occur.

## 8. Player Interactions

Define allowed actions, requirements, restrictions, cancellation behavior, and results.

## 9. Automation

Define what crew, robots, machinery, or control systems may perform automatically.

## 10. Inputs and Outputs

Define resources, information, energy, items, state changes, or other consumed and produced values.

## 11. Dependencies

Link to authoritative specifications for systems this subsystem depends on.

Do not redefine their rules.

## 12. Failure States

Define how failure occurs and its direct consequences.

## 13. Recovery

Define how normal operation can be restored.

## 14. Edge Cases

Explicitly cover abnormal or ambiguous situations that could otherwise force implementation-time design decisions.

## 15. Progression

Define how the system changes from early to mid to late game, where applicable.

## 16. Persistence

Define which state survives:

- leaving an area;
- mission completion;
- player death;
- save/load;
- application restart.

## 17. UI / UX

Define necessary information, controls, prompts, warnings, screens, and interaction flows.

## 18. Visual Feedback

Define visually distinguishable operational states.

## 19. Audio Feedback

Define important audio states, warnings, ambience, and interaction feedback.

## 20. Tuneable Parameters

List values that balancing may change without redesigning the mechanic.

Do not mix tuneable values with unresolved game rules.

## 21. Explicit Non-Goals

State adjacent behaviors this subsystem intentionally does not support.

## 22. Examples

Provide concrete scenarios that demonstrate the rules without replacing normative definitions.

## 23. Open Questions

List unresolved implementation-relevant design questions.

**This section must be empty before the specification can become Design Complete.**

## 24. Cross-Reference Checklist

List authoritative documents that reference or are referenced by this subsystem and verify that no rule is defined twice.
