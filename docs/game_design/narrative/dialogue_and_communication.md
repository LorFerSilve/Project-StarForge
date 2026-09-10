# Dialogue and Communication

> **Status:** Design Complete  
> **Authority:** Dialogue delivery, communication channels, player-choice semantics, pause/hold behavior, interruption, bribery/payment choices, knowledge boundaries, and conversational persistence

## 1. Purpose

Dialogue communicates character, operational information, choices, faction state, and discovered knowledge without creating a second hidden gameplay ruleset.

## 2. Canonical Communication Channels

Dialogue/content uses these channels:

- Face-to-Face Dialogue;
- Station Intercom / PA;
- Ship Communications;
- Live Remote Transmission;
- Recorded Message;
- Text/Data Log.

Each channel obeys the physical/communication requirements below.

## 3. Face-to-Face Dialogue

Face-to-Face requires the speaker/interaction target to be physically present and reachable under Player Interaction rules.

It is used for recruitment, important local meetings, crew conversations, and direct decisions.

## 4. Live Remote Transmission

A Live Remote Transmission requires a valid Strategic Communication Link under `../systems/communications_and_remote_control.md`.

Its delivered detail, latency, interruption, and degradation follow that link's current state.

A character cannot communicate live across an unavailable route merely because the narrative scene expects them to.

## 5. Recorded Messages and Logs

Recorded content is persistent data recoverable from authored sources such as:

- physical data carrier;
- ship/station archive;
- mission terminal;
- committed communications log.

Playback does not require the original live sender to remain connected.

## 6. Dialogue Choice Classes

Every player dialogue choice is classified as one of:

- **Informational** — asks/reveals available information without a hidden major state change;
- **Roleplay** — expresses tone/personality without changing a core outcome unless explicitly displayed;
- **Operational** — selects a practical executable plan/action/resource commitment;
- **Consequential** — commits persistent narrative/faction/world/economic state.

A single option may be both Operational and Consequential when clearly indicated by context.

## 7. Informational Choices

Informational choices do not secretly alter major reputation, mission access, faction hostility, or resource ownership.

Minor authored conversational continuity can record that a topic was discussed.

## 8. Roleplay Choices

Roleplay wording may influence local dialogue flavor, but there is no hidden personality stat, Charisma score, or morality meter in the baseline.

Any persistent consequence beyond dialogue flavor must be presented/classified as Consequential.

## 9. Operational Choices

Operational choices can select:

- destination;
- mission approach;
- allocation/resource commitment;
- support provider;
- negotiation/assault branch;
- withdrawal/response plan.

The owning gameplay subsystem validates whether the selected operation is physically/legal-state feasible before commit.

## 10. Consequential Choices

A Consequential choice may change explicitly authored state such as:

- faction reputation/relations;
- access;
- persistent world state;
- character relationship/availability;
- mission branch;
- evidence disclosure;
- final story resolution.

The choice presentation communicates that the decision is consequential before irreversible commit when the player has enough contextual knowledge to understand that fact.

## 11. Protagonist Presentation

The baseline player protagonist is unvoiced for conversational dialogue.

Player choices appear as written options/intents. NPC speech may be voiced and is always representable through subtitles/text.

## 12. Dialogue and Simulation Time

Dialogue sequences declare one of two time behaviors:

- `DialogueTimeMode::Live` — Simulation Time continues;
- `DialogueTimeMode::Hold` — invokes the global Cinematic/Conversation Hold defined by Time/Simulation.

There is no hidden partial pause.

Baseline rules:

- face-to-face **major consequential/cinematic dialogue** uses `Hold` unless the authored scene is explicitly designed as live danger;
- ordinary face-to-face informational/crew dialogue uses `Hold` while the dedicated conversation interface owns input;
- routine radio chatter, PA announcements, short comm updates, and combat barks use `Live`;
- urgent operational transmissions may use `Live` and present choices only when the owning mission has explicitly defined live-time consequences.

The UI indicates whether Simulation is Running or Held where ambiguity is possible.

## 13. No Timed Dialogue Baseline

Ordinary dialogue selection has no countdown.

A mission may create an explicitly timed urgent decision only when:

- the timer is part of the owning mission/event state;
- Simulation Time is live;
- the timer is communicated clearly;
- expiration result is authored.

Timed dialogue is therefore a mission mechanic, not an implicit conversation default.

## 14. Interruption

A Live dialogue/communication can be interrupted by actual world conditions including:

- combat;
- loss of communication;
- decompression/emergency;
- target departure/incapacitation;
- player leaving required range.

A Hold dialogue does not advance those world conditions while Hold is active.

## 15. Critical Choice Protection

An irreversible choice cannot be lost because presentation is interrupted or closed before commit.

Before confirmation:

- no consequence commits;
- if interruption invalidates the scene, the choice is suspended and can be re-presented when its owning narrative state is valid again.

After commit:

- result persists;
- interruption cannot revert it.

## 16. Player Knowledge

Available options may depend on persistent knowledge such as:

- discovered archives;
- analyzed Research Evidence;
- known faction deception;
- survivor background;
- route/intelligence state.

The UI cannot expose an option whose wording itself reveals knowledge the player has not legitimately obtained unless that option is intentionally a discovery from currently observable context.

## 17. NPC Knowledge

NPC statements/actions are constrained by what that character/faction can plausibly know under authored narrative/world/communications state.

NPC dialogue is not an omniscient debug channel.

## 18. Reputation and Relationship

Dialogue can branch according to faction reputation tier and specific character relationship/history.

High reputation may unlock cooperation/candor; low reputation may create refusal/suspicion/hostility according to the owning Faction/Narrative rule.

There is no generic hidden persuasion percentage.

## 19. Persuasion

The baseline has no Charisma stat and no random persuasion roll.

Special persuasive outcomes are deterministic from explicit contextual prerequisites such as:

- known information;
- prior action;
- faction reputation;
- specific relationship;
- possession/control of contextual leverage;
- a valid payment/bribe option.

The player is told a known hard blocker when presentation can legitimately diagnose it.

## 20. Bribery / Credit Payment

Bribery is an **authored Consequential/Operational dialogue action**, not a universal `Pay to bypass` command.

When available, the option declares:

- exact Credit amount;
- recipient/transaction owner;
- explicitly promised immediate result;
- any known reputation/legal consequence;
- whether refusal is possible **before** payment.

Baseline transaction semantics:

1. dialogue validates sufficient Credits and current offer validity;
2. the player confirms;
3. Credit transfer and promised immediate dialogue/access result commit atomically where they belong to the same deal;
4. if validation fails before commit, Credits remain unchanged;
5. no random persuasion roll is performed after payment unless the offer explicitly described a wager/uncertain outcome, which ordinary bribery does not.

A bribe cannot bypass a physically impossible action or missing gameplay capability.

## 21. Repeatable Dialogue

Routine status/crew lines may repeat under authored cooldown/context.

Major narrative conversations record their completed state and never replay as new history solely because the player re-enters the area.

## 22. Travel Communications

Strategic transit may deliver valid:

- crew updates;
- faction transmissions;
- distress signals;
- event/mission offers;
- Horizon alerts.

Delivery obeys Strategic Communication and Dynamic Event/Mission knowledge rules.

## 23. Subtitle and Communication Log

All gameplay-relevant speech has subtitle/text representation.

Important received messages enter a communication history/log after receipt so the player can revisit them.

A log contains only messages legitimately delivered/recovered, not transmissions that failed to reach the player.

## 24. Skipping

The player may skip non-interactive dialogue/cinematic presentation after required state preparation is complete.

Skipping:

- never bypasses a consequential choice;
- never grants unearned knowledge/reward;
- commits the same authored noninteractive scene outcome as watching it;
- cannot be used to ignore a live mission timer unless the scene uses Hold.

## 25. Environmental Context

Dialogue may react to known persistent states such as:

- Horizon attack/recovery;
- injured crew;
- destroyed/repaired facility;
- faction hostility;
- major discovery;
- ending state.

Such reaction is authored flavor/state acknowledgement and cannot contradict the owning gameplay state.

## 26. Persistence

Save state records, where consequential:

- completed major dialogues;
- suspended critical choice state;
- committed choices;
- received/recovered messages;
- dialogue-dependent character/faction/narrative flags;
- authored repeat/cooldown state when gameplay relevant.

A choice transaction obeys Stable Save Boundaries.

## 27. Presentation / Accessibility

GDS-13 owns:

- subtitle/closed-caption display;
- text scaling;
- choice navigation/remapping;
- speaker/source labels;
- dialogue audio ducking;
- communication-log UI.

Accessibility changes presentation/control only and cannot expose unavailable choices/knowledge.

## 28. Edge Cases

- If a live remote link becomes Unavailable mid-conversation, unsent content is not received; the scene transitions to its authored disconnect state.
- If an NPC becomes physically unavailable before a face-to-face choice commits, the stale choice cannot commit remotely.
- If Credits change while a bribe confirmation is open, amount/eligibility is revalidated at commit.
- If a critical alarm occurs during a Hold dialogue, world state is frozen; UI may still show the pre-existing critical alert but no new Simulation-Time incident progresses until resume.
- If an authored Live dialogue is interrupted before a consequential commit, the consequence remains uncommitted.

## 29. Tuneable / Content Parameters

Tuneable/content values include:

- line timing;
- repeatable bark cooldowns;
- bribe amounts;
- local relationship thresholds;
- subtitle timing owned by Presentation.

Choice classes, no-random-persuasion rule, bribery atomicity, and time-mode semantics are fixed.

## 30. Explicit Non-Goals

The baseline does not include:

- generic Charisma stat;
- random persuasion dice;
- hidden reputation punishment from ordinary informational options;
- mandatory timed dialogue;
- universal bribery bypass;
- communication through unavailable links;
- voiced player conversational lines;
- partial-pause dialogue.

## 31. Dependencies

Depends on Player Role, Major Characters, Factions/Reputation, Crew, Missions, Economy/Credits, Strategic Communications, Time/Simulation, Save/Persistence, and GDS-13 Presentation/Accessibility.

## 32. Open Questions

None.
