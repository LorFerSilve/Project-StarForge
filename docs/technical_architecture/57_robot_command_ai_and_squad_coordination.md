# TA-8 — Robot Command AI and Squad Coordination

> **Status:** Architecture Complete
> **Authority:** Runtime execution of player robot commands, command/fallback state, squad coordination, ROE, formation/rally behavior, communication degradation, role autonomy, and command-block reporting

## 1. Purpose

Player-owned robots must execute concise tactical orders reliably without becoming omniscient autonomous strategists or requiring RTS-style micromanagement.

## 2. Robot AI Priority Stack

Runtime robot behavior follows the GDS priority model:

1. hard physical/capability/safety constraints;
2. accepted player command;
3. mission-bounded command context;
4. class/role autonomy;
5. idle/formation behavior.

A lower layer cannot silently override a valid higher-priority command.

## 3. Command Runtime Record

An active robot command references the persistent/domain command identity/state and contains runtime execution data such as:

- command generation;
- recipient RobotId / group context;
- command kind;
- semantic target/area;
- execution state;
- current sub-action;
- current blocker;
- path request state;
- completion criteria;
- communications state/revision.

Runtime execution data never creates a duplicate strategic command authority.

## 4. Command State Mapping

TA-8 preserves GDS-10 states:

```text
Issued
→ Accepted
→ Executing
→ Completed
 | Blocked
 | Failed
 | Cancelled
```

`Blocked` is recoverable; `Failed` means the current command cannot legally complete without a new command/context change.

## 5. Command Acceptance

A robot accepts only if:

- robot is commandable/operational enough;
- communications permit receipt;
- command type is supported;
- target data is valid enough to attempt;
- mission/ownership authority permits it.

Acceptance is not path/action success.

## 6. Command Replacement

A new primary command increments command generation and cancels/replaces the previous primary command according to GDS rules.

Outstanding path/sensor worker results for the old generation become stale and are discarded.

## 7. Command Execution Decomposition

Commands decompose into typed sub-actions rather than one opaque behavior script.

Example `Repair Target`:

```text
Validate target/capability
→ acquire required parts if command permits
→ path to access anchor
→ position safely
→ perform repair interaction
→ monitor target/result
→ completion / blocked / failed
```

Each sub-action remains bound to ordinary ownership/path/interaction contracts.

## 8. Follow

`Follow` selects formation-relative destinations around the followed actor.

The formation system:

- uses authored slot offsets/roles;
- projects slots onto valid navigation;
- adapts spacing to corridor width;
- never teleports a robot to maintain formation;
- can collapse to column/single-file in narrow passages;
- reassigns slots deterministically if robots are lost/blocked.

## 9. Move To

`Move To` resolves a valid destination region under the robot traversal profile.

Completion requires physical arrival within authored tolerance.

If target is impossible or becomes inaccessible, command reports Blocked/Failed rather than snapping the robot.

## 10. Hold Position

A hold command stores a semantic center/region and hold radius.

The robot may reposition locally for:

- line of fire;
- cover;
- collision avoidance;
- immediate safety.

It does not pursue beyond the permitted hold envelope unless an explicit self-preservation rule authorizes temporary displacement.

## 11. Defend Area / Protected Target

Defend behavior combines:

- hold envelope;
- threat knowledge;
- ROE;
- target/area protection priority;
- reachable tactical positions.

There is no magical aggro acquisition outside legitimate perception/shared intel.

## 12. Attack / Focus Fire

An attack command provides high-priority target designation.

The robot still validates:

- target remains legitimate/known;
- path/fire position exists;
- ammo/energy/weapon state;
- friendly line-of-fire safety;
- command leash/mission limits.

Focus Fire changes priority only; it does not modify damage or accuracy.

## 13. Retreat / Return

Retreat and Return to Transport produce physical navigation goals.

`Return to Transport` completes only after the robot reaches and is transactionally secured into a valid rack/bay/transport state.

Being near the transport is insufficient.

## 14. Role-Specific Commands

TA-8 supports GDS role actions as typed executors:

- Scout Area;
- Repair Target;
- Hack Target;
- Project Shield;
- Breach Target;
- Demolish Target.

Each executor calls the owning subsystem for the actual task/interaction and cannot manufacture missing access/resources.

## 15. Scout Area

A Recon robot selects valid observation candidates within the commanded bounded area.

Coverage completion is owned by mission/survey state, not by merely visiting a fixed number of waypoints.

The robot avoids unnecessary combat according to ROE and current threat knowledge.

## 16. Repair AI

Repair execution requires:

- compatible repair capability;
- reachable repair anchor;
- real spare parts/resources where required;
- permitted target;
- hazard/safety policy.

The robot cannot claim a repair completed before the owning repair transaction commits.

## 17. Hacking AI

Hacking requires a valid physical/data interface and authorized command scope.

The robot may navigate to an access point and maintain the interaction, but does not independently choose diplomatic/strategic intrusion.

## 18. Shield AI

Shield robots choose position/orientation to satisfy the commanded protected actor/area using known threat geometry.

They manage real energy reserve and cannot create global squad immunity.

## 19. Breach / Demolition AI

Breaching/demolition:

- uses explicitly authorized targets;
- reaches actual placement/access point;
- validates blast/safety geometry;
- respects payload inventory;
- withdraws to required safe area before detonation where defined.

No arbitrary wall destruction is inferred from command type.

## 20. Rules of Engagement

ROE is an explicit input to tactical action generation:

- Hold Fire;
- Defensive;
- Engage Hostiles;
- Designated Target Only.

ROE does not alter perception truth; it constrains weapon-use decisions.

## 21. Safety Constraints

Before executing destructive or dangerous actions the robot validates:

- traversal capability;
- friendly line of fire;
- demolition safety radius;
- required physical capability;
- protected-resource policy;
- mission/command authorization.

An explicit dangerous Hold/sacrifice order can override self-preservation where GDS permits, but not impossible physics/capability.

## 22. Communication State

Robots consume the canonical:

- Connected;
- Degraded;
- Disconnected

communication state.

Connected permits normal live commands/shared knowledge.

Degraded applies authored latency/quality limits to command/knowledge delivery.

Disconnected rejects new live command delivery and activates fallback behavior.

## 23. Delayed Commands

A degraded-link command carries issuance time and command generation.

On eventual delivery it is accepted only if still current/valid.

An obsolete command is not replayed after a newer command/reconnection state has superseded it.

## 24. Fallback Orders

On disconnection the robot executes its configured fallback:

- Hold Position;
- Continue Current Objective;
- Return to Rally Point;
- Return to Transport;
- Self-Preserve / Seek Safe Position.

Fallback cannot initiate a new strategic objective, raid, extraction choice, or irreversible story action.

## 25. Reconnection

On reconnection:

- current robot state/position/task is reported;
- stale command packets are discarded;
- squad shared knowledge resumes subject to timestamps/confidence;
- player can issue a fresh command.

The robot is not reset to its old pre-disconnect position/task.

## 26. Squad Controller

A Tactical Squad has a lightweight runtime coordination record containing:

- member RobotIds/runtime handles;
- command groups;
- group command generations;
- shared ROE/fallback where configured;
- shared knowledge store reference;
- rally/transport reference;
- formation assignment;
- squad objective context.

It is not a second owner of robot Health/ammo/energy.

## 27. Formation Slot Assignment

Formation slots are assigned deterministically using:

1. role-required slots where authored;
2. robot class/size compatibility;
3. stable RobotId tie-break.

Heavy units are never assigned a slot whose corridor/clearance cannot support them.

## 28. Squad Coordination

Coordination can include:

- deconflicting movement destinations;
- sharing target information;
- distributing cover/observation slots;
- preventing several robots from selecting the exact same physical repair/hack anchor unless allowed;
- prioritizing commanded target;
- regrouping after path divergence.

Coordination never grants shared ammo/energy teleportation.

## 29. Resource Awareness

Robot AI reads real ammo, energy, repair payload and equipment state.

Low/empty resources can cause:

- reload;
- weapon switch;
- resupply request;
- reduced discretionary power;
- fallback/return;
- blocked task.

No hidden infinite reserve exists.

## 30. Self-Preservation

Self-preservation is a policy input, not a magical escape.

A critically damaged robot may seek reachable cover/retreat route unless an explicit valid command overrides that preference.

If no safe route exists, it remains physically exposed.

## 31. Mission Authority Boundary

Robots may execute bounded mission sub-actions but never independently:

- abandon mission;
- initiate extraction;
- choose final narrative branch;
- launch a raid;
- spend strategic resources outside authorized task scope.

TA-9 owns mission/raid state machines.

## 32. Persistence

Persistent gameplay state includes, where relevant:

- active command identity/type/target/state;
- command generation;
- ROE;
- fallback order;
- communication state;
- target/shared knowledge;
- current semantic task milestone;
- ammo/energy/damage via owning robot/equipment domains.

Path corridor, formation steering and worker handles reconstruct on activation.

## 33. Diagnostics

Debug tools show per robot/squad:

- command state/generation;
- current sub-action;
- blocker;
- ROE/fallback;
- communication state;
- target knowledge;
- path/formation slot;
- energy/ammo warnings;
- self-preservation decision;
- squad shared-knowledge entries.

## 34. Test Requirements

Tests cover command replacement, degraded delayed command supersession, disconnect fallback, reconnection stale-packet rejection, heavy-path incompatibility, formation narrowing, focus-fire no-stat-buff, friendly-fire block, finite resources, repair/hack authorization, transport securing, and mission-authority prohibition.

## 35. Explicit Non-Goals

No RTS command queue, no omniscient squad blackboard, no teleport formation, no command-caused resource creation, no autonomous strategic mission decisions, and no software override of missing physical capability.

## 36. Dependencies

Depends on `52`–`56`, TA-7 runtime/robot activation, GDS-10 Robot AI/Tactical Commands/Classes, Communications, Missions, and TA-9 for strategic mission/raid state.

## 37. Open Questions

None.