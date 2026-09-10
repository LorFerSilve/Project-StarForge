# Player Interaction

> **Status:** Design Complete  
> **Authority:** First-person target selection, prompts, interaction types, sustained actions, interruption, access checks, remote interaction, ownership transfer, and conflict resolution

## 1. Purpose

Interaction is the common direct-player contract for doors, consoles, crew, pickups, containers, machines, repair, mining, salvage, mission objectives, and other world objects.

## 2. Interaction Target Contract

Every interactable declares:

- Interaction/Target ID;
- available action set;
- Primary action;
- optional Alternate action(s);
- required range;
- line-of-sight/physical-access rule;
- access/authorization requirements;
- tool/equipment requirements;
- current target state;
- known blocking reason;
- Instant or Sustained interaction type;
- duration when Sustained;
- interruption/progress-retention rule;
- transaction/commit behavior owned by the target system.

The Player Interaction layer does not invent missing target rules.

## 3. Target Acquisition

The primary interaction target is selected from valid candidates near the first-person center view.

Selection uses, in stable priority order:

1. explicit modal/mission-critical focus where authored and legitimately known;
2. valid line of sight;
3. interaction range;
4. view-center relevance;
5. distance/proximity;
6. target-authored interaction priority;
7. stable Target ID tie-break.

Interaction cannot pass through opaque solid geometry unless the target explicitly exposes a valid remote/interface path.

## 4. Interaction Range

Hand/console interaction uses finite short range.

Tools and remote-control systems may expose their own finite range and connectivity requirements.

Exact distances are tuneable per interaction class.

## 5. Prompt

When a target is known and focus-valid, UI communicates:

- action verb;
- target name/identity at the player's current knowledge level;
- effective input binding;
- interaction state;
- blocker if unavailable and known;
- progress for sustained actions;
- dangerous-action warning/confirmation where applicable.

Presentation may not reveal a hidden blocker that would leak unknown information.

## 6. Instant Interaction

An Instant interaction commits on confirmed input only when all requirements remain valid at commit.

Examples include:

- pressing a local button;
- opening an unlocked locker;
- picking up an eligible small item;
- acknowledging a local interface;
- beginning dialogue with reachable crew.

If validation fails before commit, no gameplay result is applied.

## 7. Sustained Interaction

A Sustained interaction progresses on **Simulation Time** while all continuing requirements remain valid.

Examples include:

- manual repair;
- salvage cutting;
- mining/tool work;
- emergency override;
- sample extraction.

Each target declares one progress-retention policy:

- **Reset** — interrupted progress returns to zero;
- **Retain** — completed progress remains;
- **Checkpointed** — only committed internal stages remain.

The policy is target data, never an implementation guess.

## 8. Sustained-Interaction Accessibility

For non-precision sustained interactions, accessibility may replace physical button holding with press-to-start / press-to-cancel.

Simulation Time, range, target validity, interruption, tool/resource consumption, and commit semantics are unchanged.

## 9. Interruption

A sustained action is interrupted when any mandatory continuing condition becomes false, including:

- player cancels/releases under the current input mode;
- player leaves permitted range;
- required line/path becomes invalid;
- target becomes invalid/destroyed;
- player becomes Incapacitated;
- required equipped tool changes/becomes unusable;
- an authored damage-interruption condition fires;
- current context changes to one that cannot sustain the action.

The target's progress-retention policy then applies.

## 10. Tool-Gated Interaction

When an action requires an active tool, merely owning the tool in backpack storage is insufficient.

The required tool/mode must be equipped/active and operational at the interaction commit/progress step.

## 11. Access-Gated Interaction

A target may require explicit:

- security permission;
- hacking/engineering access;
- Research/Blueprint capability;
- mission/objective state;
- power/control availability;
- pressure/safety state;
- ownership/authorization.

Known blockers are communicated directly.

## 12. Hazard Confirmation

High-consequence deliberate actions require a confirmation step before commit.

This includes at minimum:

- venting a detected occupied compartment;
- jettisoning protected/strategic cargo;
- overriding a known unsafe pressure interlock;
- scrapping/destroying protected owned equipment;
- explicitly bypassing a safety lock that can create immediate severe exposure.

Confirmation never overrides a physically impossible action.

## 13. Primary and Alternate Actions

A focused target exposes exactly one current Primary interaction.

Additional target actions are exposed through Alternate Interact/context UI.

If only one valid action exists, Alternate Interact performs no hidden second action.

## 14. Physical Pickup

Picking up a physical object/resource is an ownership transfer.

Commit occurs only if:

- item is still owned by the source/world;
- player is permitted to acquire it;
- Player Inventory accepts the transfer under Mass/Volume/containment rules.

If the full quantity cannot fit:

- stackable divisible quantities may transfer the maximum valid amount;
- indivisible items remain completely at source.

No item is duplicated.

## 15. Containers

Opening a container exposes its contents but does not automatically transfer them.

The baseline provides:

- individual transfer;
- quantity/split transfer for stackable content;
- **Take All**.

`Take All` processes eligible content in stable displayed order, transfers only quantities/items that satisfy all ownership/capacity/containment rules, leaves every remainder at source, and reports skipped blockers. It never exceeds capacity or deletes non-fitting items.

## 16. Crew Interaction

A reachable recruited crew member exposes, according to current state:

- `Talk` when dialogue/content is available;
- `Status`;
- `Current Task`;
- `Assignment` shortcut when assignment control is currently permitted.

Unavailable actions are omitted or shown blocked according to UI context; interaction does not fabricate dialogue/task state.

## 17. Consoles

A console opens the real owning subsystem interface through a valid Console/Terminal input context.

The UI is a view/control surface over authoritative state, not a duplicate simulation.

If the underlying target is destroyed/disconnected while the interface is open, controls update/disable/close as appropriate before further commands can commit.

## 18. Remote Interaction

Remote interaction is available only through an explicit remote-control path such as:

- station control network;
- ship control interface;
- remote drone;
- GDS-14 Strategic Communication/Remote Control capability.

It requires the actual target's remote capability, connectivity, authorization, and sufficient known state.

Remote interaction never means arbitrary range-free interaction with any visible object.

## 19. Combat

Combat presence alone does not globally disable interaction.

An interaction remains possible if its own requirements allow it. Long interactions remain risky because Simulation Time and normal interruption/damage rules continue unless True Pause is active.

## 20. One-Time / Idempotent Actions

One-time interactions carry an owning transaction/result state.

After commit they cannot grant the same one-time effect/reward again from repeated input or save/load.

## 21. Ownership and Foreign Objects

Player-owned objects normally permit their documented control subject to safety/state.

Foreign objects require whatever authorization, hacking, force, mission rule, or faction permission their owning system specifies.

Ownership never disappears simply because a prompt is visible.

## 22. Tool Primary Actions

Mining, salvage, scanning, repair, weapon/tool operations may use `Primary Action` rather than generic `Interact`.

They still obey the common range/access/blocker/commit principles where applicable.

## 23. True Pause

No world interaction progresses or commits from ordinary player input during True Pause.

A confirmation/UI operation that only changes paused settings may still function; resuming is required for Simulation-Time world work.

## 24. Feedback and Commit Boundary

Presentation distinguishes:

- Available;
- Unavailable / Blocked;
- In Progress;
- Interrupted;
- Awaiting Confirmation;
- Committed / Completed.

Success feedback occurs only after the owning gameplay transaction commits.

## 25. Persistence

Persistent interaction results belong to the target/owning subsystem.

The interaction layer persists only gameplay state required to resume a stable sustained action when that action is explicitly saveable. It never keeps a separate authoritative copy of inventory, doors, mission objectives, or machine state.

## 26. Edge Cases

- If a target leaves range during sustained work, that target's retention policy is applied.
- If inventory fills immediately before pickup commit, unaccepted ownership remains at source.
- If a target is destroyed while its UI is open, stale commands cannot execute.
- If two equally ranked targets overlap, stable Target ID ordering prevents frame-to-frame random focus switching.
- If `Take All` encounters a protected/illegal/incompatible item, it continues to later eligible entries and reports the skipped entry rather than aborting already committed transfers.
- If a dangerous-action confirmation is open and the world state changes so the action is no longer valid, confirmation becomes invalid and cannot commit the stale action.

## 27. Tuneable Parameters

Tuneable values include:

- normal interaction range;
- target aim cone/priority weights;
- sustained durations;
- damage-interruption thresholds where the target defines them;
- prompt delay.

Ownership, validation-at-commit, Simulation-Time progression, and Take-All conservation rules are fixed.

## 28. Explicit Non-Goals

Interaction does not provide:

- through-wall physical actions;
- global auto-loot teleportation;
- hidden implementation-chosen blockers;
- universal instant repair/mining/salvage;
- stale UI control of destroyed objects;
- Take-All capacity bypass;
- success presentation before commit.

## 29. Dependencies

Depends on Controls, Inventory, Equipment/Tools, Station systems, Crew, Missions, Combat, Security, Strategic Communications, Time/Simulation, Save/Persistence, and GDS-13 UI/Feedback/Accessibility.

## 30. Open Questions

None.
