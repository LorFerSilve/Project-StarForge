# Horizon Recovery State

> **Status:** Design Complete  
> **Authority:** Cross-system Horizon Recovery classification, Critical Recovery entry/exit, Recovery Grace trigger, and interaction between station aftermath and Dynamic Event attack eligibility

## 1. Purpose

GDS-14 identified `Critical Recovery` as a previously referenced but not canonically defined state.

This specification closes that gap without replacing GDS-2 subsystem states.

## 2. Derived State Only

**Horizon Recovery State** is a derived cross-system classification.

It does not create hidden station health or a global defense score.

It is calculated from actual station conditions.

## 3. Canonical Recovery States

Horizon can be classified as:

- Normal;
- Recovering;
- Critical Recovery.

## 4. Normal

Horizon is Normal when:

- no resolved attack/emergency recovery backlog requires special recovery classification; and
- no Critical Recovery criterion is active.

Individual noncritical faults can still exist.

## 5. Recovering

Horizon is Recovering when persistent damage, theft, crew injury, robot loss, security compromise, or repair backlog remains after an incident but minimum station survival/control capability is stable.

Recovering does not prevent normal player activity.

## 6. Critical Recovery

Horizon enters **Critical Recovery** when at least one of the following actual conditions is true:

1. an unresolved Horizon-specific Catastrophic State exists under `failure_recovery_rules.md`;
2. no reachable compartment currently provides a sustainable minimum safe environment for the recruited crew population that remains aboard;
3. no valid power path can sustain the minimum emergency/critical loads required for Command Core recovery plus essential life-support stabilization;
4. an uncontrolled station-wide hazard remains capable of escalating catastrophically, including an uncontained thermal runaway, propagating decompression, or equivalent owning-system catastrophic condition;
5. Command Core recovery/diagnostic authority is physically inaccessible and no alternate protected local-control path exists;
6. a station-wide security condition still prevents establishing a safe recovery area after the hostile raid/emergency should otherwise have ended.

These criteria reference real subsystem state; none is a hidden percentage threshold.

## 7. Entry

The derived classification updates whenever one of its source conditions changes.

Critical Recovery begins immediately when any criterion becomes true.

## 8. Exit

Critical Recovery exits only after:

- every Critical Recovery criterion is false; and
- the resulting minimum-safe state remains continuously valid for a short **Stability Confirmation Window**.

Initial design target for the Stability Confirmation Window: **60 seconds of Simulation Time**.

The duration is tuneable; requiring stable confirmation is fixed.

## 9. Recovery Does Not Mean Pristine

Leaving Critical Recovery does not require:

- full structural repair;
- replacement of destroyed robots;
- restoration of stolen resources;
- full defense readiness;
- completion of every Work Order.

It means the station is no longer in a catastrophic recovery condition.

## 10. Procedural Horizon Raid Grace

Every resolved **ordinary procedural Horizon Defense Event** starts a Recovery Grace timer.

Initial design target:

- **30 minutes of Simulation Time**.

This replaces the ambiguous earlier dependency on undefined `Major` or `Severe` outcome labels.

## 11. Grace Countdown During Critical Recovery

While Horizon is in Critical Recovery:

- Recovery Grace remains active;
- its remaining duration does **not** decrement.

When Horizon leaves Critical Recovery, the remaining Recovery Grace countdown resumes.

Therefore a player receives the complete remaining recovery window after catastrophic stabilization instead of emerging from Critical Recovery into an immediately eligible procedural follow-up raid.

## 12. Grace Scope

While Recovery Grace is active:

- ordinary procedural Horizon raid candidates are ineligible;
- unrelated non-hostile Dynamic Events can still occur;
- ordinary station operation/repair continues;
- the player can leave Horizon;
- faction/world state continues normally.

## 13. Authored Exception

An explicitly authored main-story crisis can override Recovery Grace only when:

- the authored mission/event declares that exception;
- the threat is telegraphed through valid information channels;
- the exception does not violate protected campaign softlock rules.

Procedural generation can never override Recovery Grace.

## 14. Repeated Minor Incidents

Ordinary non-raid minor station faults do not automatically create Recovery Grace.

Dynamic Event anti-frustration weighting can still suppress excessive event stacking under its own rules.

## 15. Save/Persistence

Persist:

- current derived Recovery classification inputs;
- Stability Confirmation progress;
- Recovery Grace remaining Simulation Time;
- source Defense Event ID that last started/reset the grace period.

Reloading cannot clear Critical Recovery or refill/reset grace arbitrarily.

## 16. Presentation

Presentation must communicate:

- `Critical Recovery` when active;
- the actual underlying blockers causing it;
- `Recovering` without implying full restoration;
- Recovery Grace as protection from ordinary procedural attacks without implying invulnerability from authored story events or existing threats.

The player should never see one opaque `Station Health` percentage in place of these causes.

## 17. Explicit Non-Goals

This state does not provide:

- a global station HP bar;
- a Defense Score;
- automatic repairs;
- free resources;
- invulnerability;
- offline recovery;
- cancellation of a Defense Event already created before grace became active.

## 18. Tuneable Parameters

Tuneable:

- Stability Confirmation Window;
- base Recovery Grace duration.

Fixed:

- actual-state derivation;
- pause while Critical Recovery;
- procedural attacks blocked during grace;
- no automatic restoration.

## 19. Dependencies

Depends on GDS-2 Power/Atmosphere/Thermal/Security/Damage, GDS-3 Crew, GDS-11 Station Defense, GDS-12 Dynamic Events/Failure Recovery/Time/Persistence, and GDS-13 Presentation.

## 20. Open Questions

None after GDS-14 reconciliation.
