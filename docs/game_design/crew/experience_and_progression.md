# Crew Experience and Progression

> **Status:** Design Complete  
> **Authority:** Crew skill experience, training, skill-level advancement, specialization development, and profession-transition rules

## 1. Purpose

Crew progression rewards long-term use of specialists and creates meaningful growth without turning crew into disposable level-number collectibles.

Progression improves competence, capacity, reliability, and access to advanced work.

## 2. Progression Objects

Crew progression is attached to persistent crew identity.

The baseline tracks:

- skill XP by skill domain;
- current skill level;
- training state;
- profession-development state;
- persistent unlocked special knowledge where applicable.

There is no separate generic "crew level" that replaces skill-specific competence.

## 3. Skill Levels

Skills use the authoritative scale:

- 0 — Untrained
- 1 — Novice
- 2 — Competent
- 3 — Skilled
- 4 — Expert
- 5 — Master

A crew member may have different levels in different domains.

## 4. Skill XP

Each skill domain has its own experience value.

Relevant XP is earned through:

- successfully completed professional tasks;
- meaningful supervised operation;
- resolved emergencies;
- formal training;
- authored narrative education events.

A Logistics task does not automatically grant Engineering XP.

## 5. Meaningful-Work Rule

XP is awarded for meaningful work, not every simulation tick.

The system should resist trivial exploit loops such as repeatedly toggling one harmless device to farm skill.

Owning subsystems define XP events with:

- task difficulty;
- task completion;
- supervision contribution;
- emergency significance.

## 6. Diminishing Trivial Work

Tasks far below the crew member's competence grant reduced or zero XP after their learning value is exhausted.

Example:

A Master Engineer does not meaningfully progress by repeatedly performing the simplest tutorial repair.

## 7. Failure and XP

A failed task may grant limited learning XP only when:

- genuine work was attempted;
- the failure was not caused by player exploit repetition;
- the task provided relevant experience.

Failure never gives more progression than successful resolution of equivalent difficulty.

## 8. Skill Advancement

When skill XP reaches its next threshold, the skill advances by one level.

XP thresholds increase by level.

Exact thresholds are tuneable.

## 9. No Random Level-Up Choice

Normal skill advancement is deterministic from XP/training.

The game does not randomly decide whether a qualified skill increase succeeds.

## 10. Training

Crew can train skills using appropriate:

- training facilities;
- mentor;
- simulator;
- educational material;
- research knowledge.

Training consumes Simulation Time and potentially resources.

It does not advance through real-world offline time.

## 11. Training Assignment

Formal training occupies the crew member's Primary Assignment state or a specifically allowed training slot.

A crew member in full-time training does not simultaneously provide full normal assignment capacity.

## 12. Mentorship

A higher-skilled crew member can mentor another when:

- the mentor has sufficient skill advantage;
- both are assigned to compatible training/work;
- workload capacity permits mentorship.

Mentorship consumes some mentor capacity.

It is not a free passive aura.

## 13. Learning Speed

Learning speed can be modified by:

- relevant traits;
- training quality;
- mentor quality;
- task difficulty;
- current skill level.

Modifiers are bounded and documented.

## 14. Skill Cap

The normal maximum is Master (5).

Progress beyond Master is not represented as endless skill levels.

Late-game improvement instead comes from:

- better station systems;
- team structure;
- automation;
- special knowledge;
- equipment;
- technology.

## 15. Primary Profession Development

A crew member's Primary Profession is the role they are currently recognized and organized around.

Profession progression follows improvement of the profession's core skill set.

There is no independent profession-level XP bar.

## 16. Profession Change

A crew member may change Primary Profession when:

1. the target profession's required core skill reaches at least Competent (2);
2. the crew member is not in an incompatible unavailable state;
3. the player explicitly confirms the change;
4. any authored certification/training prerequisite is satisfied.

Changing profession does not erase previous skills.

## 17. Profession Change Cost

The profession change itself does not consume arbitrary currency.

It can require active-game training time or certification where the target profession justifies it.

## 18. Hybrid Specialists

Because skills persist independently, a crew member can become a useful hybrid.

Example:

Primary Profession: Engineer  
Engineering: 4  
Mechanics: 3  
Robotics: 2

The crew member remains constrained by one Primary Assignment at a time.

## 19. Special Knowledge

Some progression unlocks **Special Knowledge** rather than a normal skill level.

Examples:

- Fusion Reactor Certification;
- Alien Bio-Systems Familiarity;
- Ancient Navigation Protocols;
- Advanced Shield Calibration.

Special Knowledge is binary or tiered only where explicitly defined.

## 20. Sources of Special Knowledge

Special Knowledge may come from:

- authored survivor background;
- research;
- story missions;
- rare training;
- faction instruction;
- direct discovery.

It is not randomly awarded merely for generic XP accumulation.

## 21. Special Knowledge Effects

Every knowledge item must specify exactly what it enables.

Possible effects:

- satisfy a commissioning requirement;
- unlock an analysis option;
- enable a specific repair;
- reveal extra intelligence;
- enable a research branch.

It cannot act as a vague hidden stat boost.

## 22. Injury and Training

Injured or recovering crew can train only if their medical state explicitly permits it.

Critical or incapacitated crew do not progress through normal work/training.

## 23. Off-Screen Progression

Crew can earn task XP while the player is away from the station during active gameplay if they genuinely perform qualifying work.

No skill XP accrues merely because the application was closed.

## 24. XP Attribution

When several crew members jointly supervise a task, XP is distributed according to actual participation/assignment rather than duplicating full XP to every nearby crew member.

Exact weighting is tuneable.

## 25. Robot Delegation

A specialist supervising robots can earn relevant supervision XP when the task requires their professional contribution.

The crew member does not receive hands-on XP for work they had no professional role in.

## 26. Skill Decay

The baseline has **no skill decay**.

A Master Engineer does not forget expertise because they were assigned elsewhere for several missions.

## 27. Respec

The baseline has no generic skill-point respec because skills are learned through experience rather than allocated from a global point pool.

Changing Primary Profession remains possible through cross-training.

## 28. UI

Crew progression UI shows:

- skill levels;
- XP progress;
- relevant recent learning sources;
- training eligibility;
- mentor options;
- special knowledge;
- profession-change requirements.

## 29. Persistence

All XP, skill levels, training progress, profession, and special knowledge persist through save/load.

No duplicate training completion occurs from save/load.

## 30. Edge Cases

If a crew member completes a task exactly when reaching a skill threshold, the level-up is applied once after task resolution.

If training infrastructure fails, training pauses and retains valid progress.

If a mentor becomes unavailable, mentorship stops but the trainee keeps completed progress.

If a profession changes, existing assignment becomes invalid if incompatible and must be reassigned.

## 31. Progression Pace Philosophy

Crew should become meaningfully better through sustained use, but maxing every skill on every person is not intended as a trivial grind target.

Specialization should remain valuable.

## 32. Explicit Non-Goals

The progression system does not use:

- generic crew rarity upgrades;
- paid XP boosts;
- skill decay;
- infinite prestige levels;
- instant respec of learned expertise;
- AFK/offline real-world XP.

## 33. Tuneable Parameters

Tuneable values include XP thresholds, task XP, training rate, mentorship efficiency, trivial-task reduction, and trait learning modifiers.

## 34. Dependencies

This specification depends on:

- [Skills and Traits](skills_and_traits.md);
- [Professions](professions.md);
- [Assignments](assignments.md);
- [Crew Presence and Behavior](crew_presence_and_behavior.md);
- [Injuries, Incapacitation and Death](injuries_incapacitation_and_death.md);
- station automation and task systems.

## 35. Open Questions

None in the current progression baseline.

GDS-14 cross-domain validation is complete; remaining numeric balance and authored content values are governed as tuneable data under Design Authority.
