# TA-9 — Procedural Mission Generation and Offer Runtime

> **Status:** Architecture Complete  
> **Authority:** Deterministic mission-candidate generation, validation, stable offer packages, anti-reroll state, generation cursors, and procedural-to-MissionStore commit

## 1. Purpose

Procedural mission generation must create varied but world-valid missions without making worker timing, reload timing, or failed candidate order gameplay authority.

The generator therefore uses **staged immutable candidate packages** and commits a MissionId only after the chosen candidate passes every required validation gate.

## 2. Generator Ownership Boundary

The generator may select/assemble:

- mission archetype;
- valid Strategic Location/template;
- objective definitions;
- procedural zone/layout parameters;
- major POIs;
- deposits/salvage targets;
- survivor identity proposal;
- encounter composition;
- hazard schedule;
- major reward-bearing containers;
- stable reinforcement/raid package where the archetype requires it.

It does **not**:

- mutate player inventory;
- spawn the active Scene;
- begin mission deployment;
- grant rewards;
- change world state before candidate commit;
- invent noncanonical Resource/Content IDs.

## 3. Persistent Generation Cursor

Each procedural offer source/family maintains persistent scheduler state sufficient to make generation replay-safe, including conceptually:

- generation family/source key;
- generation ordinal;
- next eligible Simulation-Time boundary;
- cooldown/rotation state;
- last accepted/rejected committed evaluation metadata where needed.

A generation evaluation advances this cursor through an authoritative scheduler transaction whether or not a visible offer is ultimately committed.

Reloading before/after a Stable Save Boundary therefore cannot repeatedly roll the same evaluation until a preferred offer appears.

## 4. Deterministic RNG Scope

Procedural mission random uses TA-2 project-owned PCG32 derived from stable inputs such as:

- SaveSeed;
- mission-generation stream kind/version;
- source/family identity;
- Strategic Location identity;
- generation ordinal.

Standard-library random engines/distributions, wall-clock time, worker order, and memory address are not generation authority.

## 5. Candidate Build Pipeline

A generation evaluation conceptually performs:

```text
GenerationRequest
  -> snapshot eligible world/content inputs
  -> derive deterministic candidate stream
  -> build candidate 0
  -> validate
  -> if invalid, build candidate 1
  -> ... bounded attempts
  -> choose first valid candidate in deterministic attempt order
  -> prepare persistent identities/records
  -> commit MissionRecord + generated package + scheduler cursor
```

A worker may perform pure candidate construction/validation against immutable snapshots, but main-thread commit revalidates snapshot revisions and generation cursor.

## 6. Candidate Attempt Ordering

Candidate attempt number is semantic and deterministic.

If attempts 0 and 1 are processed in parallel, attempt 1 cannot win merely because its worker finished first.

The lowest-numbered valid candidate under the pinned input snapshot is selected.

If results arrive out of order, the scheduler waits only as permitted by the bounded job contract or evaluates serially; TA-13 later sets performance policy. Correct deterministic choice remains mandatory.

## 7. Generation Input Snapshot

The immutable generation snapshot includes all world facts required by the template, such as:

- current Reach/route access;
- location biome/atmosphere/gravity;
- faction ownership/relationship where relevant;
- resource and hazard profile;
- story/progression gates;
- persistent unique-content claims;
- target recovery/depletion state;
- dynamic-event source context;
- allowed mission types;
- generation/content schema versions.

The snapshot carries revisions. Commit fails/retries evaluation if required authoritative inputs changed before commit.

## 8. World Validity Gate

A candidate must match its actual world context:

- valid location/side-location;
- valid biome/environment;
- plausible resource set;
- faction/content compatibility;
- legal Reach band;
- authored story restrictions;
- no prohibited campaign-state conflict.

The generator does not relocate canonical story geography to make a candidate fit.

## 9. Objective Graph Gate

Before offer commit:

- objective graph must be acyclic;
- every mandatory objective must have a valid completion path;
- branch logic must be coherent;
- unique targets must exist exactly once;
- extraction can become valid;
- mandatory hidden objectives require valid discovery;
- objective security/end-state semantics must be explicit.

Failure rejects the candidate before the player sees it.

## 10. Physical/Traversal Feasibility Gate

Candidate validation must prove the authored/generated topology supports at least one legal path for required actors/cargo under the mission's declared requirements.

Checks can include:

- required objective reachable by an eligible traversal class;
- required door/access solution exists;
- survivor can physically reach a valid extraction route;
- required cargo can fit an allowed transport;
- mandatory ship docking/landing interface exists;
- no required target is outside the valid zone/navigation volume;
- unavoidable entry environment has a declared hard requirement rather than an impossible spawn.

TA-10 later owns nav/collision cooking data, but TA-9 owns the semantic generation-validation requirement.

## 11. Hard Requirements and Known Warnings

The generated MissionRecord stores explicit hard requirements and warning inputs rather than recomputing hidden arbitrary restrictions at deployment.

Unknown hazards may remain unknown when the design permits them, but a known unavoidable hard deployment condition must be represented in the mission contract.

## 12. Stable Major Content Package

Once the MissionRecord commits, its `GeneratedMissionPackage` is stable for that MissionId.

The package stores or deterministically references:

- selected template/module composition;
- POI identities/placements;
- deposit/salvage identities;
- survivor CharacterId where created;
- objective target assignments;
- major reward container assignments;
- hazard schedule seed/data;
- encounter composition seed/data;
- authored procedural variation choices.

Implementation may reconstruct deterministic derived geometry later, but it may not choose different major content after load.

## 13. Persistent Identity Creation

If candidate content creates persistent identities such as a procedural survivor or unique world object, those identities are allocated only in the final prepared commit.

Rejected candidates never consume persistent IDs.

The commit atomically inserts the MissionRecord, generated package, all required newly persistent child records, uniqueness claims, and allocator advances.

## 14. Unique Content Claims

The generator checks and commits explicit claims for one-time/unique content such as:

- named character;
- unique Blueprint;
- unique Research Evidence;
- unique archive;
- route unlock source;
- Ancient/author-specific site.

A competing same-boundary candidate cannot commit the same unique claim twice; normal TA-2 transaction conflict rules cause later commits to fail/re-evaluate.

## 15. Story Mission Boundary

Main-story mission identity/geography/mandatory characters/evidence/choices remain authored.

A story mission can call the procedural generator only for declared variation slots such as:

- minor encounter placement;
- ambient event selection;
- minor loot;
- compatible geometry modules.

The generated variation package cannot remove or replace canonical story requirements.

## 16. Mission Offer Stability

When a procedural MissionRecord enters `Available`, the complete major offer package is committed.

Inspecting, saving, loading, or declining does not regenerate it.

Declining/cancellation records the appropriate offer outcome. A later replacement uses a new generation ordinal and, when committed, a new MissionId.

## 17. Offer Expiration / Rotation

Where GDS-12 permits visible Simulation-Time expiration/rotation, the MissionRecord carries its persisted deadline/source DynamicEvent relation.

Wall-clock time is never consulted.

Accepted missions do not silently expire unless their explicit accepted contract says otherwise under approved GDS rules.

## 18. Difficulty Independence

Generation selects from the world's actual threat/content envelope.

It may use progression/Reach eligibility but does not inspect player gear to force parity.

The result can be below, near, or above current capability.

Difficulty settings may alter allowed GDS-12 pressure parameters where explicitly permitted, but cannot reroll geography or grant impossible enemy/world capabilities.

## 19. Rewards Boundary

Generation can select a reward **definition/profile** compatible with the mission, location, threat, faction, and economy rules.

No reward is physically granted during generation.

One-time reward realization remains mission-resolution/economy/resource authority.

## 20. Generation Failure

Generation attempts are bounded.

If no valid candidate exists:

- no broken MissionRecord is committed;
- scheduler cursor/cooldown outcome still follows its deterministic evaluation transaction;
- diagnostics record rejection reasons;
- the player sees no impossible offer.

The generator does not weaken hard validation on its final retry.

## 21. Dynamic Event Integration

A DynamicEvent may request mission generation with a stable source EventId and constrained archetype/location/context.

The event-to-mission link is committed atomically with MissionId creation.

After the MissionId exists, `MissionStore` owns mission lifecycle. The DynamicEvent retains only its relation and event-level consequence/timer state; it does not mirror objective/deployment state.

## 22. Target Recovery / Repeated Location

A later procedural mission may reuse a Strategic Location only after querying its current persistent target/world state.

Prior:

- destroyed systems;
- depleted loot;
- stolen unique items;
- restored facilities;
- changed faction posture

remain authoritative unless a valid Simulation-Time recovery/replenishment process has committed new state.

## 23. Save / Load

Persist or deterministically reconstruct from persisted state:

- generation cursors/ordinals;
- MissionId;
- mission seed/version;
- generated package;
- unique claims;
- deadlines/cooldowns;
- DynamicEvent relation.

A loader never invokes fresh generation simply because generated scene objects are not currently resident.

## 24. Diagnostics

Generation diagnostics provide per attempt:

- stream scope/version;
- input revisions;
- candidate ordinal;
- selected templates/targets;
- rejection gates/reasons;
- unique-claim conflicts;
- final selected candidate;
- committed MissionId.

Player-facing UI never exposes hidden seed/internal rejection data unless a developer tool is active.

## 25. Test Requirements

Tests prove:

- same snapshot/cursor creates same candidate package;
- worker completion order does not affect selected candidate;
- rejected candidates consume no persistent IDs;
- invalid DAG/access/extraction candidate never commits;
- unique content cannot duplicate;
- story mandatory content cannot be procedurally removed;
- Available offer remains identical after save/load;
- decline does not reroll the same MissionId;
- generator does not scale content directly to equipment;
- bounded total failure produces no broken offer.

## 26. Explicit Non-Goals

No wall-clock seeds, save-scum reroll loop, random canonical story geography, undefined resources, unlimited retries, worker-race candidate selection, or generated MissionRecord before semantic validation.

## 27. Dependencies

Depends on `61`/`62`, TA-2 RNG/transactions/identity, TA-3 world/location state, TA-8 navigation feasibility contracts, GDS-8 Mission Generation, GDS-12 Dynamic Events, and TA-10 future content-schema/cooking architecture.

## 28. Open Questions

None in the procedural mission-generation baseline.