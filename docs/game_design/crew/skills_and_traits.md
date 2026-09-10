# Crew Skills and Traits

> **Status:** Design Complete  
> **Authority:** Crew skill scale, professional competence, cross-training, trait behavior, and limits on crew statistical bonuses

## 1. Purpose

Skills and traits differentiate crew members without turning them into randomized loot cards.

Skills represent learned competence.

Traits represent persistent personal characteristics or background advantages/disadvantages.

## 2. Skill Scale

Professional skills use levels 0 through 5:

- 0: Untrained
- 1: Novice
- 2: Competent
- 3: Skilled
- 4: Expert
- 5: Master

Exact XP requirements are tuneable.

## 3. Skill Domains

Baseline skill domains correspond to professional areas such as:

- Agriculture
- Engineering
- Mechanics
- Robotics
- Science
- Medicine
- Security
- Weapons
- Logistics
- Navigation
- Intelligence

Additional narrowly defined skills can be introduced when a subsystem requires them.

## 4. Task Requirements

A specialized task can require a minimum skill level.

If the requirement is not met:

- the task is unavailable;
- automation seeks another actor;
- the player can use another method if defined.

The game does not secretly treat an untrained person as an expert.

## 5. Skill Effects

Higher skill can improve defined quantities such as:

- supervised workload capacity;
- diagnosis speed;
- repair efficiency;
- research throughput;
- fault-response time;
- resource efficiency;
- training ability.

Effects must be explicitly documented by the owning subsystem.

## 6. No Universal Percentage Bonus

There is no rule that every skill level simply gives a global percentage increase to everything the person touches.

Bonuses are domain-specific and bounded.

## 7. Cross-Training

Crew can gain skill outside their primary profession.

Cross-training enables:

- emergency substitution;
- hybrid specialists;
- eventual profession change.

It does not allow simultaneous full-capacity work in several assignments.

## 8. Traits

Traits are discrete characteristics.

Examples:

- Resourceful
- Calm Under Pressure
- Fast Learner
- Meticulous
- Field Experienced
- Former Pilot
- Alien Technology Familiarity
- Physically Resilient

Negative or mixed traits can exist if they create understandable gameplay rather than arbitrary punishment.

## 9. Trait Rules

Every trait must define:

- exact affected behavior;
- trigger/context;
- magnitude or qualitative effect;
- whether it can change;
- whether it stacks.

No trait may rely on vague hidden behavior.

## 10. Trait Count

Crew has a bounded small number of meaningful traits rather than dozens of trivial modifiers.

Exact generation count is tuneable.

## 11. Background Knowledge

Some traits can represent history or rare expertise.

Example:

Former Fusion Technician may provide an authored advantage when diagnosing specific reactor technology.

Such traits can also support narrative dialogue.

## 12. Trait Discovery

Most traits are known when the recruit is sufficiently evaluated.

Selected narrative traits can remain hidden until an authored reveal.

Hidden traits cannot secretly create major negative station consequences without reasonable discoverability.

## 13. Random Generation

Procedural crew generation uses weighted valid combinations.

Generation avoids contradictions such as a background requiring a profession the character cannot plausibly possess unless explicitly authored.

## 14. Progression

Skills can improve through work and training.

Traits normally change rarely.

Skill progression is defined in experience_and_progression.md.

## 15. UI

Crew detail shows:

- skill level;
- progress toward next level;
- trait descriptions;
- exact mechanical effects;
- assignment relevance.

## 16. Persistence

Skills, XP, traits, discovered-state, and special knowledge persist.

## 17. Edge Cases

If a task's required skill increases because equipment changes, an assigned worker may become insufficient and the task becomes blocked or degraded according to the owning system.

If a crew member changes primary profession, existing secondary skills remain.

## 18. Explicit Non-Goals

The system does not use:

- hidden random global stat bonuses;
- paid rerolls;
- rarity colors as quality;
- infinitely stacking traits.

## 19. Dependencies

This specification depends on professions, assignments, experience/progression, recruitment, and every subsystem that consumes crew skill.

## 20. Open Questions

None in the current baseline.
