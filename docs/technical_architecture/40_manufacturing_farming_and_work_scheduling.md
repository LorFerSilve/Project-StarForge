# Manufacturing, Farming, and Work Scheduling

> **Status:** Architecture Complete  
> **Authority:** Persistent station work orders, production/crop progress, prerequisite evaluation, resource consumption/output commit, worker-capacity hooks, and off-screen scheduling

## 1. Core Model

Long-running station activity uses persistent typed `WorkOrderId` records. A work order has an owning subsystem, target, state, priority, progress, prerequisite references, reservation references, assignment requirements, and blocking reason.

The shared technical lifecycle is:

```text
Queued -> WaitingPrerequisites -> Ready -> Active -> Completed
```

with `Paused`, `Blocked`, `OutputBlocked`, `Faulted`, and `Cancelled` where applicable.

Owning domains keep their GDS-specific semantics.

## 2. No Long Transaction

A process lasting seconds or minutes is never one open TA-2 transaction.

```text
Start/Reserve transaction
-> persistent WorkOrder
-> Simulation-Time progress
-> completion eligibility
-> completion transaction
```

This preserves save boundaries and exactly-once output.

## 3. Progress

Nominal progress is:

```text
ProgressDelta = EffectiveWorkRate * DeltaSimulationTime
```

`EffectiveWorkRate` can depend only on explicit inputs such as machine capacity, delivered Power, Thermal state, worker/automation capacity, and authored process modifiers.

No work progresses while Simulation Time is stopped or while the application is closed.

## 4. Deterministic Scheduling

Jobs competing for finite capacity are ordered by:

1. protected/emergency class where applicable;
2. configured priority;
3. explicit player order;
4. stable creation sequence / WorkOrderId.

Container iteration order is never gameplay authority.

## 5. Capacity Providers

Capacity can come from fixed machine lanes, construction machinery, crew, robots/drones, player assistance, or local automation.

A work definition states whether physical actor presence is required. Off-screen simulation cannot invent required labor.

TA-7/TA-8 later own active actor representation/navigation; TA-6 counts only capacity reported as validly assigned/working.

## 6. Manufacturing Runtime

Each machine persists:

- machine state and installed capability;
- ordered queue;
- active lane(s);
- local input/output buffers;
- fault state;
- target-inventory policy.

Recipe definitions remain owned by Resources/content.

## 7. Manufacturing Eligibility

A recipe can become Active only when all authored requirements pass, including as applicable:

- recipe unlocked;
- compatible machine/lane;
- required inputs reserved/delivered;
- output/byproduct capacity;
- adequate power;
- acceptable thermal state;
- required water/fluid;
- worker/automation requirement;
- machine not Faulted/Destroyed.

## 8. Input Consumption

Recipe data identifies when each input is consumed: start, progressive milestone, or final stage.

Consumption uses TA-2/TA-6 ownership transactions. Consumed material is never restored by pause, power loss, save/load, or ordinary cancellation.

## 9. Power and Thermal Interruption

If operating requirements stop being met, the owning recipe/machine rule can pause or derate progress. Completed progress remains unless the recipe explicitly defines a physical instability consequence.

No output is created early.

## 10. Manufacturing Completion

At completion eligibility the scheduler prepares a transaction that revalidates machine/job revision, final inputs, output capacity, and machine state.

The commit then atomically:

- consumes final inputs;
- creates outputs/byproducts exactly once in valid local output buffers;
- updates repetition/queue state;
- records completion.

If output cannot be accepted, the job becomes `OutputBlocked` and the next conflicting cycle does not start.

## 11. Repetition

Supported modes are fixed quantity, repeat-until-cancelled, and maintain-target.

Maintain-target uses accessible/reserved inventory read models, not a magical global inventory count.

## 12. Cancellation

Before consumption, cancellation releases reservations. After consumption, only unconsumed reservations release; consumed inputs are not recreated. Work-in-progress salvage exists only when explicitly defined.

## 13. Robot Assembly

Robot Assembly Bay jobs obey normal queue, logistics, power, thermal, and fault rules.

Final Robot commissioning remains a separate robot/TA-2 transaction that allocates `RobotId` only after physical assembly/configuration/diagnostics are valid.

## 14. Farming Runtime

Each crop bay persists:

- crop-plan ContentId;
- lifecycle state;
- growth progress;
- crop health;
- environmental tolerance/exposure accumulators;
- local inputs;
- harvest-ready state;
- local output buffer;
- scheduling policy.

## 15. Crop Growth

Growth rate is derived from actual Fresh Water, Power, nutrients, atmosphere, temperature, farm condition, and required labor/automation.

Missing requirements cannot yield nominal growth. Short interruptions can pause growth and accumulate authored stress; long/severe exposure can reduce health or destroy the batch.

## 16. Harvest

A mature batch becomes `HarvestReady`. Harvest requires valid labor/automation where authored and sufficient output-buffer capacity.

The harvest transaction creates the authored yield exactly once and advances the batch state. If the output buffer is full, the crop remains blocked/harvest-ready.

## 17. Spoilage

Perishable inventory can own Simulation-Time spoilage state/deadlines derived from storage conditions. Refrigeration modifies future spoilage rate; it never restores already spoiled material.

## 18. Aggregate Crew Demand

Routine station food/water consumption can be aggregated over Simulation Time. It consumes physically accessible inventory through TA-2/TA-6 ownership rules; individual hunger/thirst runtime entities are not required for routine accounting.

## 19. Construction/Repair Hook

Construction and Repair use the same scheduler for finite labor/capacity and progress but own their material/topology semantics in the TA-6 construction/repair specification.

## 20. Manual Assistance

A validated player-assistance state can contribute work capacity while the player physically remains eligible. It cannot bypass missing materials, invalid topology, unsafe state, or commissioning requirements.

## 21. Faults

When a machine/target faults or is destroyed, affected work pauses/faults. Already consumed inputs stay consumed. Pending reservations remain or release according to job policy. Repair/reconstruction is a separate work order.

## 22. Off-Screen Scheduling

Off-screen Horizon can advance work by event/deadline stepping rather than 60 Hz ticks.

The advance must stop at the earliest relevant boundary, including:

- job completion;
- input depletion;
- output full;
- power/thermal/water eligibility change;
- crop tolerance threshold;
- spoilage threshold;
- worker loss;
- station damage/defense event;
- automation policy event.

The boundary is resolved before the remaining interval continues. A coarse step may not skip an earlier failure and incorrectly grant later production.

## 23. Persistence

Persisted state includes WorkOrder identity/state/progress, queues/lanes, reservations, consumption milestones, local buffers, crop batches/health/growth, tolerance/spoilage state, target policies, faults/blockers, and persistent assignment references.

Load restores queues exactly and never replays an already committed completion/harvest.

## 24. Debugging

Tools expose work orders, priority/order, capacity providers, progress/rate, prerequisite failures, reservations/consumption milestones, output capacity, crop environmental factors, next off-screen boundary, and exactly-once completion state.

## 25. Non-Goals

No real-world offline progress, production without inputs/infrastructure, infinite worker capacity, save/load output duplication, or one generic recipe type replacing domain rules.

## 26. Dependencies

Depends on TA-2 transactions/persistence, TA-6 Power/Atmosphere/Thermal/Water/Logistics, and GDS Manufacturing/Farming/Construction/Repair/Resources/Crew/Robots/Automation.

## 27. Open Questions

None within TA-6 production/work scheduling architecture.
