# TA-8 — Perception, Memory, and Shared Knowledge

> **Status:** Architecture Complete
> **Authority:** AI sensing inputs, observation records, confidence/last-known-position memory, shared knowledge provenance, communication/jamming effects, and the no-omniscience technical boundary

## 1. Purpose

GDS-9 and GDS-10 prohibit omniscient AI. TA-8 therefore requires enemy/robot/crew decisions to consume explicit acquired knowledge rather than query unrestricted world truth whenever behavior depends on what an actor knows.

## 2. World Truth vs AI Knowledge

StarForge distinguishes:

```text
Authoritative World State
!=
Actor Knowledge State
```

An enemy can physically exist in world state while another actor has no knowledge of its location.

AI decision code may access unrestricted world truth only for technical/safety facts that are not knowledge-limited, such as validating its own body state. Targeting/search/tactical decisions use knowledge-aware views.

## 3. Perception Channels

Baseline channels:

- Vision;
- Hearing / semantic noise events;
- Equipment Sensor detection;
- Damage/source awareness justified by combat facts;
- Shared squad/faction/robot information;
- Player/mission command designation;
- Persistent authored mission/world intel.

Each observation records its source channel.

## 4. Perception Observation

A normalized `PerceptionObservation` contains at minimum:

- observer identity/runtime handle;
- observed subject ID or anonymous signature where identity is unknown;
- source channel;
- observed/estimated position;
- observation Simulation-Time timestamp/tick;
- confidence/quality;
- classification/hostility knowledge level;
- optional velocity/facing/signature estimate;
- source/provenance token;
- observation revision.

An observation is evidence, not automatically permanent perfect knowledge.

## 5. Vision

Vision queries consider:

- actor FOV;
- distance;
- TA-5 line of sight/occlusion;
- target visibility/signature modifiers;
- lighting/environment knowledge only where the perception model explicitly uses it;
- stance/exposure;
- sensor blindness/status effects.

Renderer culling/LOD is not AI visibility authority.

## 6. Vision Sampling

Expensive vision need not run every 60 Hz tick for every actor.

AI scheduling may distribute perception updates across ticks, but:

- reaction behavior respects resulting perception cadence;
- scheduling is Simulation-Time based;
- actor order is deterministic;
- difficulty tuning changes approved reaction/perception parameters rather than granting hidden knowledge.

Numeric cadence budgets belong to TA-13.

## 7. Occlusion

A visual observation requires valid physical visibility using TA-5 query filters appropriate to the sensor.

Ordinary vision cannot see through opaque geometry.

A specialized thermal/electronic sensor may use a different occlusion model only when its equipment definition explicitly supports it.

## 8. Hearing / Noise Facts

AI hearing does not inspect the final mixed miniaudio output.

Gameplay systems emit typed semantic `NoiseFact`s for relevant sounds, such as:

- gunshot;
- explosion;
- footsteps/machinery where authored;
- impact;
- alarm;
- door/breach action;
- vehicle/robot signature.

A NoiseFact contains source location, class, intensity/signature, timestamp, and source identity only when that identity is physically/semantically knowable.

Suppression/silencing modifies the semantic acoustic signature according to weapon/content data.

## 9. Hearing Propagation

Hearing can use bounded distance plus authored/scene acoustic obstruction/portal information sufficient for gameplay.

TA-8 does not require full wave acoustics.

A heard sound may create:

- suspicious location;
- threat direction/area;
- known source identity only if justified.

Hearing a gunshot through a wall does not reveal exact actor transform unless the sensor model supports that precision.

## 10. Sensors

Machines/ships/robots can consume equipment-backed sensor observations.

Sensor observations declare:

- sensor type;
- effective range;
- target signature requirement;
- occlusion/penetration rules;
- current quality/degradation;
- jamming/EMP interaction.

No generic `CanSeeThroughWalls` flag exists outside explicit sensor technology.

## 11. Combat Source Awareness

When an actor takes damage, the combat system may provide source/direction information only to the extent justified by the attack.

Examples:

- visible muzzle/impact can identify attacker;
- unseen sniper shot can provide approximate incoming direction but not necessarily exact identity;
- EMP/jammed sensor may reduce source resolution.

Damage is not a universal perfect target reveal.

## 12. Perception Memory

Each AI actor maintains bounded `PerceptionMemory` entries keyed by known subject/signature.

An entry can contain:

- current knowledge state;
- Last Known Position;
- last observed velocity/direction;
- last observation time;
- confidence;
- hostility/classification knowledge;
- source provenance;
- shared/local source distinction;
- search history.

## 13. Knowledge States

Canonical subject knowledge states:

```text
Unknown
DetectedSignature
Suspected
Identified
LocatedCurrent
LastKnown
Lost
```

Content can map these to UI/behavior without adding hidden perfect knowledge.

## 14. Confidence Decay

When no new observation arrives, position confidence degrades on Simulation Time according to actor/sensor doctrine.

A `LocatedCurrent` target can become `LastKnown`, then lower-confidence/lost knowledge.

The AI does not update Last Known Position to follow an unseen moving target.

## 15. Search Area

A stale Last Known Position can produce a bounded uncertainty/search area derived from:

- elapsed Simulation Time;
- last known velocity/direction where known;
- navigable nearby routes;
- actor doctrine;
- new noise/evidence.

It never samples the player's real hidden position merely to make the search efficient.

## 16. Shared Knowledge

Squad/faction/robot communication can transmit `SharedObservation` records.

Every shared record retains:

- original source/provenance;
- original observation timestamp;
- current message/receive timestamp;
- transmitted confidence/precision;
- subject classification known to source;
- communication channel/state.

Receiving an old message does not make the information current.

## 17. Communication States

Communication contracts from Robots/Systems apply:

- Connected;
- Degraded;
- Disconnected.

Degraded links can delay, reduce precision, or drop categories of shared data according to explicit communication policy.

Disconnected actors receive no new live shared observations.

## 18. No Shared-Knowledge Inflation

Sharing cannot improve information beyond what the source legitimately knew, except where multiple independent observations are explicitly fused by a sensor/intelligence capability.

A robot that only heard an approximate noise location cannot send an exact target coordinate to its squad.

## 19. Player Designation

A valid player target designation is an explicit knowledge source for commanded robots.

It carries the information available to the player's targeting/command interface and current communications.

A designation cannot mark an actor the player has never legitimately identified unless mission intel explicitly provides that identity/location.

## 20. Mission / Authored Intel

Mission state may provide known:

- area;
- target identity;
- objective location;
- patrol warning;
- hazard intel.

Such intel enters AI knowledge with an explicit source and precision/confidence rather than silently revealing world truth.

## 21. Squad Knowledge Store

A tactical squad can maintain a bounded shared knowledge store for current contacts/hazards/objectives.

Individual actors consume the shared view only if their communication link permits it.

The store is not a global omniscient blackboard.

## 22. Enemy Group Alerts

Enemy groups can share alert/contact records when local voice/radio/network communication is physically/logically available.

Jamming, destroyed relays, sealed distance, or isolated compartments can stop or delay propagation.

An alarm can communicate a threat exists without revealing the threat's exact current position.

## 23. Persistence

Gameplay-relevant active mission AI memory can persist:

- awareness state;
- Last Known Position;
- confidence;
- target identity/classification;
- search state;
- squad/shared alert state;
- communication state where owned by the persistent actor/mission.

Raw sensor query objects, visibility hit lists, and worker buffers are reconstructible runtime state.

## 24. Save / Load

Loading restores the persisted knowledge snapshot at the save's Simulation Time.

It does not run a free all-world perception scan before AI resumes.

Actors must reacquire updated knowledge through normal sensors.

## 25. Off-Screen Knowledge

Off-screen AI can process knowledge at reduced detail, but it still cannot access information unavailable to the actor/group.

Off-screen combat/search abstraction uses current known contacts, logical location, communication, and authored event facts.

## 26. Deterministic Ordering

When several observations arrive in one tick, update order is stable by:

1. observation semantic priority where defined;
2. subject stable identity/signature key;
3. source stable identity;
4. observation sequence.

No hash iteration or worker timing decides which observation wins.

## 27. Diagnostics

AI debug tooling can visualize, per selected actor:

- FOV/sensor range;
- current visible/sensed subjects;
- knowledge entries;
- Last Known Positions;
- confidence and timestamps;
- shared-message provenance;
- communication state;
- search uncertainty area;
- reasons a target is or is not known.

Debug visualization must not feed data back into gameplay.

## 28. Test Requirements

Tests cover occlusion, hearing without exact identity, confidence decay, loss of contact, stale shared messages, jamming/disconnection, player designation, save/load no-free-reveal, damage-source uncertainty, sensor-specific occlusion, deterministic observation ordering, and off-screen no-omniscience.

## 29. Explicit Non-Goals

No direct renderer-visibility authority, no mixed-audio hearing analysis, no global omniscient blackboard, no damage-equals-perfect-reveal, no shared-data precision inflation, and no save/load perception reset/reveal.

## 30. Dependencies

Depends on TA-5 queries, TA-7 runtime entities, robot communication rules, GDS-9 enemy behavior, missions/intel, and later TA-11 audio presentation only as a consumer—not authority.

## 31. Open Questions

None.