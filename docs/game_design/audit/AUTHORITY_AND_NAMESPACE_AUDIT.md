# GDS-14 Authority and Namespace Audit

> **Status:** Final Audit  
> **Authority:** Cross-domain ownership and terminology disambiguation for implementation handoff

## 1. Purpose

This audit verifies that each gameplay rule has one authoritative owner and that repeated generic words do not become accidental shared state types.

## 2. Authority Principle

A subsystem can consume another subsystem's result but must not redefine it.

Canonical pattern:

Source Domain State  
→ explicit interface/result  
→ Consumer Domain Consequence.

## 3. Project-Wide Authority Matrix

| Domain | Owns | Explicitly Does Not Own |
| --- | --- | --- |
| Global Rules | project invariants, scope, failure philosophy, simulation principles | subsystem formulas/content |
| GDS-2 Station | station structure, power, atmosphere, thermal, water, farming, logistics, manufacturing, automation, defenses, security, station damage/events | crew identity, generic combat math, economy |
| GDS-3 Crew | survivor/recruitment/crew identity, profession, skill, assignment, health, work capacity | station network physics, robot behavior |
| GDS-4 Resources | physical resource identity/ownership, mining, salvage, processing, crafting, Blueprint/Research knowledge | market price, weapon behavior, station execution |
| GDS-5 Player | player movement, interaction, health, inventory, equipment, tools, field survival | generic combat attack math, mission lifecycle |
| GDS-6 Spacecraft | ship identity/configuration, flight, propulsion, navigation, docking, cargo, ship systems/damage | world route graph, generic combat formulas, robot TCC |
| GDS-7 World | geography, setting, factions, canonical routes/locations, world truth | mission lifecycle, trade price, direct combat |
| Narrative | player role, story structure, characters, dialogue, endings | generic mission/combat/economy mechanics |
| GDS-8 Missions | Mission IDs/instances, objectives, zones, exploration, extraction, failure/rewards | direct combat math, raid specialization |
| GDS-9 Combat | hits, Damage Packets, weapons, ammo, shields/armor, statuses, enemy combat behavior | player biological state, robot persistence, mission result |
| GDS-10 Robots | Robot IDs, manufacturing/configuration, TCC, squads, commands, robot damage/recovery, station robot roles | generic damage formula, station task scheduler |
| GDS-11 Raids | fortified-target raid phases, sabotage, physical theft, escalation, Horizon Defense execution | base mission lifecycle, Dynamic Event creation |
| GDS-12 Cross-Cutting | economy, time, persistence, failure recovery, Dynamic Events, Difficulty, communication, Horizon recovery, Recovery Transit | subsystem-specific simulation |
| GDS-12 Progression | P0-P5 capability phases, cross-domain gates, finale readiness | granting capability by phase label |
| GDS-13 Presentation | visual/audio/UI/HUD/alarms/onboarding/accessibility | authoritative hidden gameplay state |
| GDS-14 Audit | cross-domain conflict resolution, ownership verification, implementation-readiness verdict | new feature scope unrelated to resolving design gaps |

## 4. Physical Ownership

GDS-4 remains sole authority for physical resource/item ownership.

Other domains may define valid transfers but cannot duplicate ownership.

Examples:

- Mission extraction changes security classification while preserving one physical owner.
- Trade transfers physical owner and Credits atomically.
- Raid theft transfers Station → Raider → extraction owner.
- Robot installation transfers a component into the Robot ID's configuration.

## 5. Knowledge Ownership

Blueprints, Research Evidence, analyzed route/story knowledge, and other committed Knowledge Assets are persistent non-physical state.

They are not physical cargo after commit unless a separate physical carrier also exists.

## 6. Identity Ownership

Unique persistent IDs are type-specific.

Examples:

- Crew ID;
- Robot ID;
- Ship ID;
- Mission ID;
- Dynamic Event ID;
- Defense Event ID.

An ID type cannot be substituted for another merely because both are globally unique identifiers.

## 7. Qualified State Namespace Rule

Generic state words are **qualified by their owning type/domain**.

Implementation, UI diagnostic text, serialized schema, and design references must not treat same-spelled labels as one universal enum.

## 8. Priority Namespace Finding — RESOLVED

GDS-14 found `P0` used in two valid but unrelated scales:

### Power Load Priority

- Power P0 — Emergency;
- Power P1 — Critical;
- Power P2 — Essential;
- Power P3 — Normal Operations;
- Power P4 — Discretionary.

### Alarm Priority

- Alarm P0 — Critical/Immediate Lethal or Catastrophic Risk;
- Alarm P1 — Severe;
- Alarm P2 — Warning;
- Alarm P3 — Advisory.

These scales are **not numerically interchangeable**.

Canonical implementation/presentation names are:

- `PowerLoadPriority`;
- `AlarmPriority`.

Player-facing diagnostic contexts use qualified forms such as `Power P1` and `Alarm P1` whenever ambiguity is possible.

**Result: PASS after qualification rule.**

## 9. Critical State Namespace

`Critical` is contextual.

Examples:

- PlayerHealthState::Critical;
- CrewHealthState::Critical;
- EquipmentThermalBand::Critical;
- HorizonRecoveryState::CriticalRecovery;
- AlarmPriority::P0/P1 can describe critical danger.

No generic project-wide `Critical=true` state exists.

## 10. Disabled State Namespace

`Disabled` is likewise typed.

Examples:

- RobotOperationalState::Disabled;
- ShipOperationalState::Disabled;
- ComponentState::Disabled;
- Combat Objective `Disable` resolves against explicit target criteria.

A target being disabled in one subsystem does not automatically satisfy every other `Disable` meaning.

## 11. Security vs Pressure Authority

Security can lock/unlock authorized doors.

Atmosphere/pressure safety can still block unsafe opening.

Security authority does not bypass pressure physics without explicit emergency override.

**Result: PASS.**

## 12. Power vs Thermal Authority

Power owns electrical allocation.

Thermal owns temperature/cooling consequence.

A reactor can have available electrical nameplate capacity but be thermally throttled; Power receives only current available output.

**Result: PASS.**

## 13. Atmosphere vs Player/Crew Health

Atmosphere owns physical environmental state.

Player/Crew owns biological consequence.

A pressure sensor failure can remove knowledge without changing the atmosphere itself.

**Result: PASS.**

## 14. Automation vs Physical Systems

Automation creates/assigns tasks and acts through valid workers/controllers.

It cannot create power, resources, access, repair parts, or capabilities.

**Result: PASS.**

## 15. Mission vs Combat

Mission owns objective state/result.

Combat owns physical attack outcome.

Combat verbs map to explicit mission resolution semantics rather than a generic `defeated` flag.

**Result: PASS.**

## 16. Mission vs Raid

Raids are specialized GDS-8 Missions.

GDS-11 adds phases/fortification behavior but does not create a second mission lifecycle.

**Result: PASS.**

## 17. Dynamic Event vs Defense Event

Dynamic Events owns whether/when a procedural Horizon attack is created.

After Defense Event ID creation, GDS-11 owns execution.

Dynamic Events cannot modify a Defense Event's combat outcome using an external hidden score.

**Result: PASS.**

## 18. Progression Phase vs Capability

P0-P5 describes actual cross-domain state.

The phase label grants no item, Reach, Research, TCC, stat, or station capacity.

**Result: PASS.**

## 19. Difficulty vs World Threat

Mission/location threat exists independently from Difficulty.

Difficulty modifies only documented execution-pressure axes and cannot change progression eligibility/loot/story/knowledge.

**Result: PASS.**

## 20. Accessibility vs Difficulty

Accessibility settings change presentation/control support without reward penalty or hidden world modification.

Gameplay-pressure reduction belongs to Assisted/Custom Difficulty, not hidden accessibility modifiers.

**Result: PASS.**

## 21. Presentation vs Knowledge

Presentation may expose only authoritative state the player legitimately knows.

No accessibility, HUD, marker, caption, music cue, or alarm can reveal hidden enemy/world information without a valid information source.

**Result: PASS.**

## 22. Communication Authority Finding — RESOLVED

GDS-14 found `Communication infrastructure` used across Narrative, Raids, Robots, Spacecraft, and Dynamic Events without one complete cross-system owner.

`../systems/communications_and_remote_control.md` now owns strategic communication availability and remote-control boundaries.

**Result: PASS after added authority.**

## 23. Horizon Critical Recovery Finding — RESOLVED

GDS-14 found `Critical Recovery` referenced by Dynamic Events without canonical state criteria.

`../systems/horizon_recovery_state.md` now owns the derived state and grace interaction.

**Result: PASS after added authority.**

## 24. Recovery Destination Finding — RESOLVED

GDS-14 found that external defeat plus simultaneous Horizon attack could otherwise leave recovery destination/timing ambiguous and create intentional-defeat fast travel.

`../systems/recovery_transit_and_destination.md` now owns that cross-system resolution.

**Result: PASS after added authority.**

## 25. Time Authority Finding — RESOLVED

GDS-14 found mixed wording between Active Game Time and Simulation Time.

`../systems/time_and_simulation.md` now establishes:

- Simulation Time = only gameplay timer authority;
- Active Game Time = unpaused user-facing playtime/UX cadence;
- global True Pause freezes both;
- no gameplay subsystem advances from wall clock.

**Result: PASS after correction.**

## 26. Orphan-Mechanic Result

After the GDS-14 corrections above, no implementation-critical mechanic in the audited baseline lacks an authoritative owner.

Remaining content values marked tuneable belong to balancing/content data rather than unresolved gameplay rules.

## 27. Result

**PASS — Authority ownership and namespace consistency are sufficient for final GDS-14 scenario validation.**
