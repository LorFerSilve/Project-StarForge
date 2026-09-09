# Crew Cross-Validation

> **Status:** Active Audit  
> **Authority:** Crew-domain consistency review only  
> **Purpose:** Verify that the first-pass crew design does not contradict established global and station rules, and record unresolved external dependencies.

## 1. Scope

This audit checks the complete first-pass crew domain against:

- Global Game Rules;
- Home Station Overview;
- Station Automation;
- Farming and Food;
- Station Security;
- Station Damage and Repairs;
- Station Events.

It does not mark future domains complete.

## 2. Global Rules — Result: PASS

### Persistent Crew

Crew specifications preserve the global rule that recruited crew is a long-term persistent asset.

### No Routine Permanent Death

`injuries_incapacitation_and_death.md` explicitly prevents routine permanent death and reserves it for authored narrative exceptions.

### Human Crew Are Not Disposable Troops

`overview.md` and `crew_presence_and_behavior.md` preserve the global rule that robots are the repeatable tactical assault force.

### No Real-World Offline Progression

Crew needs, training, healing, and automation advance only during active game simulation.

### Automation Boundary

Crew automation never selects missions, raids, diplomacy, or strategic resource use without player policy.

**Conclusion:** no conflict with `../02_global_game_rules.md`.

## 3. Home Station Overview — Result: PASS

Crew population is constrained by actual station support rather than an arbitrary account cap.

Crew physically inhabits the persistent home station.

Crew assignments do not bypass structural, utility, or access requirements.

**Conclusion:** no conflict with `../station/overview.md`.

## 4. Station Automation — Result: PASS

The station automation framework owns generic task states, scheduling, permissions, and policies.

The crew domain owns:

- specialist capability;
- workload capacity;
- professional responsibility;
- substitution;
- skill requirements.

This avoids duplicate authority.

**Conclusion:** authority boundary is clean.

## 5. Farming and Food — Result: PASS

The station farming specification states that a Farmer is valuable but not mandatory for basic farm operation.

Crew specifications preserve this by treating Farmer expertise as automation/supervision capability rather than as a magical requirement for crop existence.

**Conclusion:** no hard-lock conflict.

## 6. Station Security — Result: PASS

Crew security behavior respects:

- pressure safety;
- actual door state;
- access permissions;
- hazard knowledge;
- physical pathfinding.

Security Officers coordinate response but cannot create sensor information or override physics.

**Conclusion:** no authority conflict.

## 7. Damage and Repairs — Result: PASS

Engineer/Mechanic capability is separated from physical repair requirements.

Crew cannot repair without:

- access;
- labor;
- resources;
- tools;
- compatible environment.

**Conclusion:** crew expertise does not bypass station repair rules.

## 8. Station Events — Result: PASS

Crew can respond to events according to:

- availability;
- profession;
- assignment;
- automation policy;
- physical access.

The event system remains owner of incident lifecycle; crew owns character response.

**Conclusion:** no duplicate event rules.

## 9. Internal Crew Consistency — Result: PASS

The following relationships are internally consistent:

### Profession vs Skill

Profession is the primary role; skill is independent learned competence.

### Assignment vs Automation

Assignment defines responsibility; automation responsibility defines what the crew member may do within that assignment.

### Assignment vs Presence

A local task requires physical presence; remote supervision requires valid infrastructure.

### Injury vs Assignment

Incapacitation removes available workload capacity and forces reassignment/escalation.

### Needs vs Recruitment

Permanent recruitment requires habitation; food/water sustainability may warn rather than hard-lock if immediate support exists.

### Progression vs Rarity

Progression occurs through skills/training and does not require crew rarity upgrades.

## 10. Dependencies Preventing Design Complete

The following future domains must be cross-validated before crew files can progress to Design Complete.

### Missions

Required for:

- survivor rescue failure;
- extraction;
- temporary passengers;
- mission persistence;
- authored crew participation.

### Resources — First-Pass Resolved by GDS-4

**Result: PASS**

GDS-4 now defines canonical food, water, medical, repair, operational, and component resources together with physical ownership/reservation rules.

Crew does not conflict with the resource model.

Exact balance quantities remain tuneable and do not block crew rule definition.

### Research — First-Pass Resolved by GDS-4

**Result: PASS**

GDS-4 now defines Research Evidence, laboratory Research Work, Scientist requirements, technology prerequisites, Blueprint separation, and persistent research state.

Crew Scientist responsibilities and finite workload are compatible with the research framework.

### Spacecraft — First-Pass Resolved by GDS-6

**Result: PASS**

GDS-6 now defines passenger/life-support capacity, temporary survivor transport, Navigator contribution to route planning, Mechanic/Engineer ship-service boundaries, docking, and spacecraft recovery.

Crew remains optional for baseline piloting while specialists improve advanced operation without bypassing physical ship requirements.

### Combat

Required for:

- crew injury severity;
- security combat;
- protective equipment;
- boarding behavior.

### Robots

Required for:

- repair-drone supervision;
- security robots;
- Robotics Specialist;
- crew-to-robot supervision ratios.

### Narrative / Factions — First-Pass Resolved by GDS-7

**Result: PASS**

GDS-7 now defines authored major characters, Nia Calder's recruitable-character mapping, faction/reputation context, survivor story sources, and the rule that no baseline main-story mission requires recruited-crew permadeath.

Procedural crew remain governed by GDS-3 while authored crew can add fixed background/dialogue without bypassing core mechanics.

### Player / Movement / Interaction — First-Pass Resolved by GDS-5

**Result: PASS**

GDS-5 now defines first-person movement, collision, physical interaction, manual repair/tool use, player incapacitation, and rescue constraints.

Crew presence rules are compatible with the player collision/interaction baseline and do not require teleporting manual cooperation.

### Time and Persistence

Required for:

- exact aggregate food/water consumption cadence;
- training duration;
- recovery duration;
- off-screen activity timing.

## 11. No New Blocking Contradiction Found

The first-pass audit found no contradiction that requires changing the established global or station design baseline.

The crew domain can therefore remain the authoritative first-pass dependency for the next design phase.

## 12. Next Review Trigger

This audit must be revisited after completion of:

1. GDS-8 Missions;
2. GDS-9 Combat;
3. GDS-10 Robots;
4. relevant cross-cutting time/persistence specifications.

Until then, Crew remains **First-Pass Complete — Cross-Validation Pending**, not Design Complete.
