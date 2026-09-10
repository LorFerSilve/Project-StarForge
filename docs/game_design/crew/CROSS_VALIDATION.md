# Crew Cross-Validation

> **Status:** Active Audit  
> **Authority:** Crew-domain consistency review only  
> **Audit Revision:** Refreshed after GDS-12

## 1. Scope

This audit validates the first-pass Crew domain against Global Rules and GDS-2 through GDS-12, including survivor/recruitment, professions, skills, assignments, automation, needs, experience, injury, physical presence, time, progression, economy, raids, and persistence.

## 2. Persistent Crew — Result: PASS

Recruited crew remains a persistent long-term asset. A rescued Survivor becomes Crew only after recruitment commits.

Routine gameplay does not permanently kill recruited crew; injury and Incapacitation remain meaningful persistent consequences.

## 3. Profession, Skill, and Assignment — Result: PASS

Profession is primary role identity, skills are independent competence, and one Primary Assignment plus finite Workload Capacity prevents unlimited specialist coverage.

No GDS-12 progression phase creates a generic Crew Level or overrides these rules.

## 4. Crew Quality — Result: PASS

Crew value continues to come from profession, skills, traits, background, Special Knowledge, training, health, and actual assignment.

GDS-12 does not introduce Common/Rare/Epic/Legendary crew tiers.

## 5. Station Integration — Result: PASS

Crew work remains constrained by actual:

- station infrastructure;
- access/pathing;
- resources;
- tools;
- environment;
- workload;
- assignment/authorization.

Expertise does not bypass physical station rules.

## 6. Automation Boundary — Result: PASS

GDS-2 owns generic task scheduling/policies; GDS-3 owns human capability and professional responsibility.

Crew automation cannot independently select missions, raids, diplomacy, Research strategy, or unbounded strategic spending.

## 7. Needs — Result: PASS

Crew needs remain station-level support requirements such as habitation, food, water, atmosphere, temperature, and medical support rather than individual hunger/thirst/happiness/sleep micromanagement.

## 8. Resources and Research — Result: PASS

GDS-4 physical ownership, Research Evidence, laboratory work, Blueprints, and Research prerequisites remain compatible with Scientist and other specialist responsibilities.

Crew cannot generate physical inputs or scientific knowledge from nothing.

## 9. Spacecraft — Result: PASS

Passenger/life-support capacity, Navigator contribution, Engineer/Mechanic ship-service roles, docking, and recovery remain compatible with Crew rules.

Baseline ship operation does not require one mandatory recruited specialist.

## 10. Player and Combat — Result: PASS

Crew physical presence respects GDS-5 movement/interaction and GDS-9 combat.

Combat can wound/incapacitate crew but does not turn them into expendable tactical units or override routine no-permadeath protection.

## 11. Robots — Result: PASS

Robotics Specialists supervise eligible robot production/diagnostics/maintenance without replacing GDS-10 robot authority.

Robots remain the routinely expendable manufactured force; human crew does not inherit permanent robot attrition rules.

## 12. Raids and Horizon Defense — Result: PASS

GDS-11 now resolves crew behavior during station attacks and raids:

- security/response assignments;
- evacuation/safe-compartment behavior;
- engineering/medical/logistics response;
- injury/incapacitation;
- no routine permanent crew death.

## 13. Economy — Result: PASS

GDS-12 Economy can provide contracts, services, training inputs, and eligible recruitment/faction opportunities without converting Crew into purchasable rarity cards or a generic labor currency.

Credits do not replace habitation, skill, profession, or staffing requirements.

## 14. Crew Progression — Result: PASS

GDS-12 phase progression now supplies campaign pacing around the existing skill-specific XP/training model.

Crew still advances through meaningful work, training, mentorship, and Special Knowledge rather than one cross-domain XP bar.

## 15. Time and Simulation — Result: PASS

The previously unresolved time dependency is now resolved by GDS-12.

Crew:

- work;
- training;
- healing/recovery;
- needs consumption;
- off-screen station activity

advance on active Simulation Time only.

Real-world time while the application is closed grants no work, XP, healing, training, or needs consumption.

## 16. Off-Screen Crew Simulation — Result: PASS

When the player is away during active gameplay, crew may use lower-detail simulation, but outcomes must still respect actual workload, access, resources, health, assignment, and elapsed Simulation Time.

## 17. Persistence — Result: PASS

The previously unresolved persistence dependency is now resolved by GDS-12 Stable Save Boundaries.

Persisted Crew state includes applicable:

- identity;
- recruitment state;
- profession;
- skills/XP;
- traits/Special Knowledge;
- assignment;
- workload state;
- health/injury/recovery;
- training progress;
- location/behavior state where gameplay relevant.

Save/load cannot duplicate recruitment, XP, training completion, or recovery transactions.

## 18. Failure/Recovery — Result: PASS

Routine failure may create injury, Incapacitation, workload loss, emergency staffing pressure, and recovery resource/time costs.

It does not silently erase a recruited Crew identity or committed skills/knowledge.

## 19. Finale Integration — Result: PASS

GDS-12 MS-F01 readiness can use faction/Continuance support or self-sufficient Horizon capability without imposing one mandatory named crew member.

Qualified Crew can strengthen Research, engineering, logistics, navigation, medical, and defense readiness through their actual systems.

## 20. Difficulty — Result: PASS

Difficulty does not alter crew identity, skill levels, profession, assignment capacity, permanent-death protection, or progression unlock ownership.

## 21. Remaining Downstream Dependency — GDS-13

GDS-13 remains responsible for:

- crew management UI;
- assignment/workload feedback;
- needs and injury warnings;
- training/skill presentation;
- survivor/recruitment feedback;
- station-presence readability;
- accessibility.

These are presentation dependencies, not unresolved Crew gameplay rules.

## 22. Conclusion

The previously pending cross-cutting **Time and Persistence** dependency, plus later Raid/Economy/Progression interactions, are now first-pass resolved by GDS-11/GDS-12.

No blocking contradiction exists between GDS-3 and GDS-1 through GDS-12.

GDS-3 remains:

**First-Pass Complete — Cross-Validation Pending**

Its remaining scheduled downstream design dependency is GDS-13, followed by the GDS-14 whole-project audit.
