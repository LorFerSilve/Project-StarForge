# Interaction Feedback

> **Status:** Draft  
> **Authority:** How player actions, targets, validity, progress, completion, interruption, failure, ownership transfer, and physical system response are communicated

## 1. Purpose

Every meaningful player action must provide enough feedback for the player to understand:

- what is targeted;
- whether the action is valid;
- whether it started;
- what is happening;
- whether it completed;
- why it failed or stopped;
- what state changed.

## 2. Interaction Feedback Stack

An interaction can communicate through several channels:

1. world animation/state;
2. contextual prompt;
3. reticle/focus response;
4. progress indicator where duration matters;
5. sound;
6. haptics where supported;
7. notification/result text.

Not every interaction needs every channel.

## 3. Focus State

When the player aims/focuses on an interactable, the presentation should identify:

- target;
- primary available action;
- input binding;
- critical state/blocker where relevant.

Focus highlighting is contextual, not permanently active on all objects.

## 4. Interactable States

Canonical presentation states:

- Available;
- Available with consequence/warning;
- In Progress;
- Blocked;
- Completed/Activated;
- Disabled/Unavailable;
- Unknown/Unidentified.

## 5. Available

The player receives a normal interaction prompt and subtle target acknowledgement.

## 6. Available With Warning

Used when action is valid but consequential.

Examples:

- cycling an airlock while atmosphere will vent;
- dismantling installed equipment;
- opening hostile route;
- overriding safety control.

The warning explains known consequence before commit.

## 7. Blocked

Blocked interaction shows the known immediate reason.

Examples:

- `No Power`;
- `Requires Cutting Tool`;
- `Inventory Volume Full`;
- `Access Denied: Helios Security`;
- `Outer Door Must Be Sealed`;
- `Target Not Breachable`.

## 8. Unknown Blocker

If the cause is not legitimately known:

- `Interface Unresponsive`;
- `Access Method Unknown`;
- `Signal Unresolved`;
- `Insufficient Diagnostic Data`.

Do not leak hidden cause through error text.

## 9. Hold Interactions

Actions with nontrivial duration use a progress indicator only when:

- progress is meaningful;
- interruption is possible;
- player benefits from knowing remaining work.

Examples:

- repair;
- hacking;
- cutting;
- medical stabilization;
- heavy cargo transfer.

## 10. Instant Interactions

Simple toggles/pickups need no artificial hold timer unless risk/design explicitly requires one.

Examples:

- press ordinary button;
- pick up small accessible item;
- open unlocked container.

## 11. Progress Semantics

A progress bar represents actual committed/in-progress work, not fake delay.

If interruption preserves partial progress, the bar/state persists accordingly.

If the action is atomic and no partial state exists, presentation should not imply otherwise.

## 12. Interruption

Interrupted interactions communicate:

- interrupted;
- current resulting state;
- whether partial progress remains;
- why it stopped when knowable.

## 13. Commit Feedback

At transaction commit, feedback should reflect the actual authoritative state change.

Examples:

- cargo transferred;
- item equipped;
- door unlocked;
- robot command accepted;
- repair completed;
- Blueprint acquired;
- Credits transferred.

## 14. Ownership Transfer

Physical transfer feedback should show source/destination where ambiguity matters.

Example:

`12 Titanium → Ship Cargo`

not merely:

`+12 Titanium`

when mission security/ownership state is important.

## 15. Field-Unsecured Feedback

Mission loot pickup clearly communicates that the item/resource is still Field-Unsecured until valid extraction.

This should be taught early and reinforced through icon/text without constant intrusive warnings.

## 16. Extraction Commit Feedback

When extraction secures cargo, presentation distinguishes:

- Field-Unsecured before commit;
- Vehicle/Extraction-Secured after commit;
- Station-Secured only after actual station transfer.

## 17. Inventory Full

Capacity failure identifies whether the limiting factor is:

- Mass;
- Volume;
- slot/equipment compatibility where relevant;
- destination unavailable.

## 18. Resource Reservation

When crafting/manufacturing reserves inputs, UI feedback shows them as reserved rather than removed without explanation.

Cancelling later reports recoverable/nonrecoverable committed inputs according to manufacturing rules.

## 19. Repair Feedback

Repair interaction communicates:

- target/component;
- required parts/resources;
- tool/capability;
- current damage;
- repair level being attempted;
- progress;
- resulting condition.

A repair sound/animation cannot claim full restoration if only stabilization occurred.

## 20. Hacking Feedback

Hacking communicates:

- valid interface/link;
- operation being attempted;
- security response/escalation where knowable;
- progress/state;
- success/failure;
- persistent access consequence.

No generic animated code screen is sufficient by itself.

## 21. Breaching Feedback

Breaching presents:

- target validity;
- tool/payload placement;
- pressure/safety warning where known;
- arming/progress;
- detonation/cut result;
- resulting access/pressure state.

## 22. Construction Feedback

Placement preview gives immediate response for:

- snap/socket;
- collision;
- orientation;
- structural connection;
- pressure boundary;
- utility requirement;
- resource cost.

Commit creates the actual construction transaction.

## 23. Demolition/Dismantling Feedback

Before commit, display:

- exact target;
- expected recoverable materials where known;
- affected connection/system warnings;
- irreversible consequences.

No accidental dismantling through one ordinary interaction press.

## 24. Power Interaction

Switching a breaker/system indicates:

- requested state;
- actual resulting state;
- blocked reason if the command cannot be fulfilled;
- downstream known affected systems when relevant.

## 25. Atmosphere Interaction

Airlock/vent/pressure controls communicate:

- both-side pressure state;
- door/seal state;
- cycle progress;
- unsafe venting risk;
- resulting pressure transition.

## 26. Manufacturing Feedback

Starting a Work Order clearly separates:

- queued;
- inputs reserved;
- working;
- blocked;
- output ready;
- output transferred.

## 27. Research Feedback

Research completion distinguishes:

- technology understood;
- Blueprint acquired/not acquired;
- new Research Evidence recorded;
- new known provider/location paths.

It must not imply a physical item was created unless one actually was.

## 28. Trade Feedback

A trade commit shows:

- quantity;
- Credits change;
- physical item destination/source;
- cargo effect;
- market stock change;
- reputation/access consequence if immediate and known.

## 29. Reputation Feedback

Reputation changes show:

- faction;
- direction/value change;
- resulting tier change if one occurred;
- known reason.

No global morality meter appears.

## 30. Combat Feedback

Combat feedback differentiates:

- fired/failed-to-fire;
- empty magazine;
- reload;
- hit;
- shield absorption;
- armor impact;
- weak-point hit;
- status application;
- target disable/destruction when known.

## 31. Dry Fire / Unable to Fire

Weapon failure-to-fire communicates the actual reason:

- no ammunition;
- reload required;
- overheated;
- insufficient energy;
- weapon disabled;
- safety/interaction state.

## 32. Reload Feedback

Reload presentation visually/audibly supports actual ammo transfer timing and completion.

Interruption cannot display a completed reload before the ammo transaction commits.

## 33. Damage Feedback

Player damage uses restrained combinations of:

- impact direction;
- shield/armor effect;
- audio;
- controller haptic;
- status indicator;
- animation/screen effect.

It avoids long opaque blood/damage overlays that make further play unreadable.

## 34. Robot Command Feedback

On command issue:

- selected recipients acknowledge/reject;
- Accepted command has distinct confirmation;
- Blocked/Failed gives known reason;
- Disconnected unit is visibly not receiving live order.

## 35. Partial Group Acceptance

When only some robots accept a group order, feedback explicitly indicates partial acceptance and identifies rejected/incompatible units.

## 36. Spacecraft Control Feedback

Flight input feedback exposes:

- thrust firing;
- Flight Assist state;
- actual velocity response;
- damaged thruster limitations;
- power/propellant restriction.

Camera movement alone must not imply acceleration if the ship did not physically accelerate.

## 37. Docking Feedback

Docking distinguishes:

- alignment assistance;
- capture eligibility;
- physical capture;
- Hard Dock;
- service-link establishment.

The player must know when docking is merely aligned versus truly committed.

## 38. Mission Objective Feedback

Objective update occurs only when objective state changes.

Presentation differentiates:

- discovered;
- active;
- completed;
- failed;
- optional;
- extraction required.

## 39. Dynamic Event Feedback

Event notification indicates whether it is:

- informational world state;
- optional offer;
- accepted Mission;
- urgent threat to Horizon;
- event already active.

## 40. Save Feedback

Save states:

- Save Requested;
- Waiting for Stable Save Boundary;
- Saving;
- Save Complete;
- Save Failed.

A failed save must never masquerade as successful.

## 41. Loading Feedback

Loading identifies selected save and protects against accidental overwrite/continuation ambiguity.

After load, no generic `resources restored` messaging should imply rollback was a gameplay recovery transaction.

## 42. Failure Feedback

Failure summary is causal, not punitive in tone.

It states:

- what failed;
- what was consumed/lost;
- what remained secured;
- robot/ship/station consequences;
- next valid recovery/retry actions.

## 43. Audio Redundancy

Important feedback should not depend exclusively on audio because audio may be disabled or inaccessible.

Critical audio events have visual/text equivalents.

## 44. Visual Redundancy

Critical state should not depend exclusively on color, flicker, or small icons.

Use label/shape/audio/haptic redundancy where useful.

## 45. Haptics

Haptics can communicate:

- weapon fire;
- impacts;
- low-frequency machinery;
- docking capture;
- warning pulses.

Haptics are optional and independently adjustable/disableable.

## 46. Feedback Latency

Input acknowledgement should occur immediately when the game receives a valid request, even when the physical action itself takes time.

This can be visual/audio acknowledgement without prematurely committing outcome.

## 47. No False Feedback

Presentation must never show:

- success before commit;
- resource acquisition before ownership transfer;
- enemy detected without valid knowledge;
- repair complete while component remains damaged;
- docked state before capture;
- save complete before persistence write succeeds.

## 48. Explicit Non-Goals

No universal hold-to-interact delay, fake progress bars, vague blockers, misleading `+loot` ownership messaging, or presentation that commits state earlier than gameplay.

## 49. Tuneable Parameters

Prompt timing, focus strength, progress animation, notification duration, haptic strength, hit-confirmation style, and sound mix are tuneable.

## 50. Dependencies

References all direct-interaction gameplay domains, UI/HUD, VFX, Audio, Alarms, Save/Persistence, and Accessibility.

## 51. Open Questions

None in the interaction-feedback baseline.
