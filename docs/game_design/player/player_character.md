# Player Character

> **Status:** Design Complete  
> **Authority:** Player-character identity, persistence, physical presence, direct capability philosophy, failure ownership, customization baseline, and boundaries with combat/progression systems

## 1. Purpose

The player character is the persistent first-person operator and commander through whom the player directly explores, fights, repairs, mines, boards, travels, and interacts with Project StarForge's world.

## 2. Perspective and Physical Body

Normal on-foot gameplay is first-person.

The player always has an authoritative physical world body with:

- position;
- orientation;
- collision;
- locomotion state;
- biological Health state;
- equipped gear;
- carried inventory;
- environmental exposure;
- current interaction/action state.

The gameplay body exists even when first-person rendering shows only hands, held equipment, and selected suit elements.

## 3. Persistent Identity

There is exactly one primary persistent player character per save timeline.

Persistent state includes:

- player-character identity;
- chosen Callsign/display name;
- equipment and slot assignment;
- inventory and ownership;
- equipment condition/charges;
- Health/recovery state;
- environmental exposure states that persist;
- unlocked capability supplied by equipment/Research/knowledge;
- narrative state owned by Narrative.

## 4. Character-Creation Baseline

The baseline character setup allows the player to choose a **Callsign/display name**.

The baseline does **not** require a face sculptor, body-shape editor, selectable gameplay sex/body type, or recorded protagonist voice set.

The physical on-foot avatar uses a standardized gameplay body whose visible appearance is dominated by equipped suit, helmet, gloves, and gear.

Future cosmetic expansion requires an explicit presentation/content update but may not alter gameplay collision, reach, Health, movement, or statistics.

## 5. Protagonist Voice Baseline

The protagonist is **not voiced for conversational dialogue** in the baseline.

Dialogue choices are presented as player-selected text/intents. NPC dialogue may be voiced.

The player avatar may use non-linguistic embodiment sounds such as breathing, exertion, pain, suit reaction, and impact vocalization. These sounds do not establish a fixed spoken narrative voice/personality and remain subject to accessibility/audio settings.

## 6. No Generic Player Level

There is no universal Player Level that automatically increases:

- Health;
- damage;
- carrying capacity;
- mining speed;
- movement speed;
- Research power.

Direct personal progression comes from real capability such as:

- equipment;
- suit technology;
- weapons;
- tools/gadgets;
- backpack capacity;
- Research/Blueprints;
- spacecraft access;
- station infrastructure;
- robots;
- crew;
- knowledge/preparation.

## 7. No Generic Player Skill Tree

The baseline contains no RPG skill tree or attribute-point system based on passive percentage inflation.

Adding a permanent personal-skill system later requires an explicit accepted design change. Implementation must not invent one from progression data.

## 8. Physical Presence

The player obeys:

- collision;
- gravity;
- pressure/atmosphere;
- thermal/radiation/environment state;
- doors and traversal paths;
- hazards;
- combat damage;
- local interaction range.

Opening a live management interface does not remove or protect the body from the world. Global True Pause freezes the simulation when explicitly invoked.

## 9. At Horizon Station

At Horizon the player can physically:

- traverse accessible compartments;
- interact with crew/robots;
- operate reachable consoles;
- inspect/repair systems with valid capability;
- enter Construction Mode through valid construction control;
- configure automation;
- manage manufacturing/Research through authorized interfaces;
- configure/equip mission loadouts;
- board/operate spacecraft.

Every action still obeys its owning system's access, resource, safety, time, and transaction rules.

## 10. External Missions and Raids

On deployed content the player directly participates in:

- exploration;
- resource acquisition;
- mining/salvage;
- combat;
- rescue;
- environmental interaction;
- objective manipulation;
- robot command;
- boarding/sabotage where applicable;
- extraction/withdrawal.

Automation and strategic systems support rather than replace direct action.

## 11. Player Failure

Routine defeat does not permanently delete the player character or save.

Health zero produces `PlayerHealthState::Incapacitated`.

The current location/mission then resolves:

- local rescue when a valid rescue state exists; or
- mission/location failure followed by the GDS-14 Recovery Transit process.

External defeat is not an instant teleport to Horizon.

## 12. Horizon Incapacitation

If incapacitated at Horizon:

- station simulation continues while unpaused;
- reachable crew/medical/robot systems may create/execute rescue tasks;
- the station is not reset;
- local recovery requires valid physical access and medical/emergency capability;
- protected emergency recovery prevents a permanent hard-lock without granting free full restoration.

## 13. Player vs Crew

The player is not a normal Crew roster unit.

Crew profession workload, crew XP/skill bands, assignment slots, shift/rest needs, and crew-autonomy rules do not apply to the player.

The player may manually perform a task only when that task explicitly supports player execution and all required tools/access/resources are present.

## 14. Manual Competence

The player's baseline practical competence permits ordinary use of:

- common ship controls;
- Engineering Multitool functions unlocked by the tool;
- basic repair workflows;
- mining/salvage tools;
- first aid;
- construction assistance;
- ordinary terminals and equipment.

Advanced operations still require the actual Research, Blueprint, equipment, facility, information, or specialist dependency defined by the owning subsystem.

Narrative competence never bypasses gameplay capability.

## 15. Interaction Reach

Direct interaction uses the finite first-person interaction rules in `interaction.md`.

Solid geometry and security/access remain authoritative.

Remote interaction requires an explicit remote-control capability; the player has no innate range-free control power.

## 16. Gameplay Scale

The player uses one standardized humanoid gameplay collision/reach scale.

Callsign or any later cosmetic presentation cannot alter:

- collision;
- reach;
- movement;
- combat hit volume;
- inventory capacity;
- statistics.

## 17. First-Person Representation

Baseline rendering requires:

- hands/arms as needed for held equipment actions;
- held weapons/tools/gadgets;
- relevant suit/helmet presentation cues.

A full rendered first-person body below the camera is **not required by the baseline gameplay specification**. If later added as presentation polish, it must match the same authoritative body/collision and cannot reveal through geometry.

## 18. Contexts

The player transitions among:

- On Foot;
- Construction / Strategic interface state;
- Spacecraft Control;
- Console / UI Interaction;
- Incapacitated / Recovery Transit.

Input ownership is explicit under Controls/Time and no context duplicates the character or ship.

## 19. Equipment-Driven Capability

Examples of capability dependencies:

- vacuum traversal requires valid sealed environmental protection;
- advanced mining requires compatible Mining Cutter capability;
- hazardous material handling requires valid containment/protection;
- advanced scans require scanner mode/tier/sensor access;
- specialized repairs require corresponding tools/components/facilities;
- combat depends on actually equipped operational weapons/ammunition.

## 20. Persistent Gear Principle

Ordinary defeat does not permanently delete equipped persistent gear.

Gear may still:

- consume ammunition/charges;
- lose condition;
- become Disabled;
- require repair;
- be deliberately dropped/abandoned;
- be lost only through an explicit rule that transfers or destroys its physical ownership.

## 21. No Player Hunger / Thirst / Sleep Loop

The player has no routine personal hunger, thirst, sleep, or fatigue meters.

Food/water remain station/crew resources and may appear as explicit authored mission cargo/provision requirements rather than continuous personal metabolism bars.

## 22. Biological Health Regeneration

Biological Health does not automatically regenerate to full when combat ends.

Recovery requires the treatment/recovery processes defined by Player Health.

Personal Shield regeneration is a separate powered equipment/combat mechanic.

## 23. Presentation

The GDS-13 HUD/presentation layer communicates the player's legitimately known:

- Health band;
- Shield state when equipped;
- Suit Energy and Life-Support Reserve where relevant;
- ammunition/weapon/tool state;
- Mass/Volume limits;
- interaction prompt/blocker;
- mission/objective state;
- environmental danger;
- recovery state.

The presentation layer cannot redefine these states.

## 24. Persistence

Save state preserves all gameplay-relevant player identity, ownership, condition, location/recovery, and progression-linked capability references required to resume deterministically.

Transient animation/camera interpolation may be reconstructed and is not a separate gameplay authority.

## 25. Edge Cases

- Loading while Incapacitated restores that stable state or committed recovery state, never an invented healthy state.
- Losing a tool/weapon capability immediately removes actions that require it but does not delete unrelated progression.
- Entering a live UI while hazardous exposure exists does not halt exposure unless global True Pause is invoked.
- A later cosmetic system may change visual assets only; saves created before it remain compatible with the standardized gameplay body contract.
- Changing Callsign alters presentation text only and cannot rewrite completed narrative identity/history.

## 26. Tuneable Parameters

Player-character overview contains no balance-critical fixed numbers beyond subsystem data.

Movement, Health, inventory, equipment, and interaction values are tuneable only in their owning specifications.

## 27. Explicit Non-Goals

The baseline does not include:

- Player Level;
- generic RPG attributes/skill tree;
- stat-affecting cosmetics;
- mandatory face/body character editor;
- voiced conversational protagonist;
- routine player permadeath;
- personal hunger/thirst/sleep micromanagement;
- innate remote interaction;
- superhuman inventory capacity.

## 28. Dependencies

Refined by Controls, Movement, Interaction, Health, Inventory, Equipment, Tools/Gadgets, Field Survival, Combat, Missions, Spacecraft, Narrative Player Role, Recovery Transit, Persistence, and GDS-13 Presentation.

## 29. Open Questions

None.
