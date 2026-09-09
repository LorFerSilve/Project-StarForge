# Player Interaction

> **Status:** Draft  
> **Authority:** First-person target selection, prompts, interaction types, sustained actions, interruption, access checks, remote interaction, and conflict resolution

## 1. Purpose

Interaction is the common player-facing contract for doors, consoles, crew, pickups, containers, machines, repair, mining, salvage, and mission objectives.

## 2. Interaction Target

An interactable exposes:

- interaction identity;
- available actions;
- required range;
- access requirements;
- tool/equipment requirements;
- current state;
- blocking reason;
- duration if sustained;
- interruption rules.

## 3. Target Acquisition

The primary target is selected from valid objects near the first-person center view.

Selection considers line of sight, range, aim/proximity relevance, and explicit priority.

The system does not interact through opaque solid geometry.

## 4. Interaction Range

Normal hand/console interaction uses short finite range.

Special tools can define their own range.

## 5. Prompt

A valid target displays action verb, target name, input, and blocker if unavailable.

Example: E — Open Airlock Control.

## 6. Instant Interaction

Instant interactions resolve on confirmed input when all requirements are valid.

Examples include pressing a button, opening an unlocked locker, picking up a small item, or speaking to crew.

## 7. Sustained Interaction

Sustained interactions require progress over active game time.

Examples include manual repair, salvage, emergency override, and sample extraction.

The target defines whether interrupted progress is retained, partially retained, or reset.

## 8. Interruption

A sustained interaction can be interrupted by release, leaving range, target invalidation, incapacitation, switching the required tool, or explicit damage interruption.

## 9. Tool-Gated Interaction

A target can require a compatible actively equipped tool.

Owning a tool in backpack storage is insufficient when the action requires it in hand.

## 10. Access-Gated Interaction

Interactions can check security permission, research, mission state, power, target state, and relevant authorization.

The blocker is communicated.

## 11. Hazard Confirmation

High-consequence deliberate actions require confirmation.

Examples include venting an inhabited compartment, jettisoning strategic cargo, overriding a pressure interlock, or destroying owned critical equipment.

## 12. Interaction Priority

When one target has several actions, one is Primary and others can be exposed by Alternate Interact/context UI.

## 13. Pickups

Picking up a physical item/resource transfers ownership only if inventory can accept it.

If capacity is insufficient, the item remains at its source.

## 14. Containers

Opening a container does not automatically transfer all content.

The player chooses transfers; Take All may exist but still respects capacity.

## 15. Crew Interaction

Crew interaction can expose talk, status, assignment shortcut, current task, and authored dialogue.

## 16. Consoles

A console can open a dedicated UI context.

The UI controls the real underlying system rather than copying it into a separate fake state.

## 17. Remote Interaction

Remote interaction requires an explicit system such as a station control network, remote drone, or ship interface.

It respects connectivity, permission, sensors, and control availability.

## 18. Object State

Interaction obeys actual object state.

A destroyed door is not restored/opened by a normal Open command.

## 19. One-Time Actions

One-time interactions are idempotent where appropriate.

Repeating a completed mission interaction cannot duplicate its reward.

## 20. Ownership and Permission

Player-owned objects normally permit control subject to safety.

Foreign objects can require authorization, hacking, force, or mission state.

## 21. Tool Actions

Some tools use Primary Action instead of generic Interact.

Compatibility/blockers still use the interaction framework.

## 22. Interaction During Combat

The player can interact during combat unless the interaction explicitly prohibits it.

Long interactions create exposure through time/interruption.

## 23. Pause

No world interaction occurs during true Pause.

## 24. Feedback

Interaction feedback distinguishes available, unavailable, in-progress, completed, interrupted, and dangerous/confirmation-required states.

## 25. Persistence

Persistent state is owned by the target/system, not duplicated inside the input layer.

## 26. Edge Cases

If a target moves out of range during a sustained action, the target's interruption rule applies.

If inventory fills before pickup commit, ownership remains at source.

If a target is destroyed while its UI is open, the UI closes or updates and stale commands cannot execute.

## 27. Explicit Non-Goals

Interaction does not provide through-wall actions, auto-loot teleportation, hidden requirements, universal instant repair/mining, or stale UI control of destroyed objects.

## 28. Tuneable Parameters

Tuneable values include interaction range, sustained duration, aim cone, interruption tolerance, and prompt delay.

## 29. Dependencies

This specification depends on controls, inventory, equipment, tools, station systems, crew, missions, combat, security, and UI.

## 30. Open Questions

None in the core interaction framework.
