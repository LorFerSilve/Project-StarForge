# Player Role and Identity

> **Status:** Design Complete  
> **Authority:** Narrative identity of the player character, fixed backstory limits, commander status, station ownership context, Callsign, protagonist dialogue-voice baseline, and roleplaying freedom

## 1. Purpose

The player receives enough fixed narrative context to participate coherently in the campaign while preserving roleplaying freedom and avoiding an unnecessarily expensive fixed protagonist production burden.

## 2. Fixed Narrative Role

The player is an **independent recovery operator** active in the Recovery Sector before the opening mission.

Their ordinary work includes small salvage, repair, transport, and recovery contracts.

## 3. Starting Status

At game start the player is not:

- a famous admiral;
- a faction leader;
- a legendary scientist;
- a secret hereditary heir;
- a known historical Fracture survivor;
- formally sworn to a major faction.

Their strategic importance is earned through play.

## 4. Callsign / Display Name

During new-game setup the player chooses a **Callsign/display name**.

The Callsign is used by UI, logs, manifests, and dialogue text where a dynamic player identifier is appropriate.

Spoken NPC dialogue does not require recording every possible Callsign; NPCs normally address the player through contextual titles such as Commander, Administrator, Operator, or equivalent relationship-specific wording.

Changing a Callsign later, if the UI permits it, changes presentation only and does not rewrite narrative history or reputation.

## 5. Character-Appearance Baseline

The baseline does not require a face/body character creator.

The gameplay avatar uses the standardized player body defined by GDS-5 and is visually characterized primarily by equipped Suit, Helmet, gloves, weapons, tools, and gear.

Any later cosmetic expansion is non-statistical and must preserve the same collision/reach/gameplay body contract.

## 6. Fixed Backstory Scope

The only mandatory personal history is:

- the player is an adult;
- the player is legally/operationally independent;
- the player has practical recovery/tool/spaceflight experience sufficient for ordinary baseline operation;
- the player is not currently sworn to a major faction;
- no mandatory spouse, child, parent, sibling, birthplace, ethnicity, religion, or inherited political identity is fixed by the main campaign.

The story may allow the player to express attitudes but does not invent missing family trauma to force motivation.

## 7. Starting Contract

The inciting event begins when the player investigates a low-power emergency/transponder signal from an abandoned pre-Fracture station.

The signal is initially approached as a legitimate salvage/recovery opportunity.

## 8. Horizon Station Claim

The station's surviving emergency authority grants the player legitimate operational control as **Acting Recovery Administrator** after minimum Command Core recovery requirements are satisfied.

This is an emergency technical/legal authorization, not hereditary ownership or a faction appointment.

## 9. Long-Term Commander Role

As Horizon grows, the player's functional identity becomes:

- station commander/administrator;
- expedition leader;
- ship owner/operator;
- independent diplomatic actor;
- strategic decision-maker.

This role is earned through accumulated physical capability, relationships, knowledge, and story state rather than a Commander Level.

## 10. Station Independence

No faction automatically gains ownership/control of Horizon because it:

- financed a mission;
- sold/granted technology;
- supplied assistance;
- reached Allied reputation;
- supported the finale.

Any specific control/ownership concession would require an explicit consequential narrative choice. None is mandatory in the baseline campaign.

## 11. No Permanent Faction-Origin Choice

Character creation does not ask the player to choose a fixed faction origin/class.

Faction reputation and alliances emerge from gameplay.

## 12. Conversational Protagonist Voice

The baseline protagonist is **unvoiced for conversational dialogue**.

Player dialogue is represented through selectable written choices/intents.

NPCs may be fully/partially voiced according to content production.

Non-linguistic avatar sounds such as breathing, exertion, pain, and suit vocal reactions are permitted and do not establish a fixed spoken personality.

## 13. Dialogue Choice Styles

Where context supports them, choices may express styles such as:

- pragmatic;
- compassionate;
- skeptical;
- assertive;
- technical.

These are authored response tones, not permanent personality stats or dialogue-class points.

## 14. Mechanical Consequence Boundary

A dialogue option changes persistent state only when it is explicitly classified as consequential/operational under `dialogue_and_communication.md`.

Choosing a tone does not secretly modify generic hidden personality attributes.

## 15. Technical Competence

The player is narratively competent with ordinary recovery tools and ships.

Advanced capability still requires the actual:

- Research;
- Blueprint;
- equipment/tool;
- facility;
- specialist support;
- discovered information;
- authorization

specified by the owning gameplay system.

Narrative competence never bypasses mechanics.

## 16. Moral Agency

The player can make persistent decisions including, where authored:

- faction support/opposition;
- evidence disclosure;
- restoring/severing dangerous infrastructure;
- negotiation vs assault;
- distribution of scarce strategic assets;
- final Stabilize / Sever / Contain resolution.

The game does not collapse these into one morality meter unless a future accepted design change introduces one.

## 17. Faction Membership Boundary

The player may become Allied with a faction but remains an independent Horizon commander in the baseline.

There is no mandatory oath that replaces the player's independent role.

## 18. Motivation Arc

### Early

Recover survivable infrastructure and establish Horizon as a viable base.

### Mid

Secure resources, survivors, technology, routes, and relationships needed to expand operations.

### Late

Understand The Fracture, prepare a cross-domain response, and prevent the Second Cascade.

### Postgame

Continue operating Horizon in the persistent world shaped by the chosen final resolution.

## 19. Personal Stakes

Horizon Station, recruited crew, accumulated ships/robots, relationships, and the recovered regional network are the player's principal long-term stakes.

The main narrative does not require a prewritten kidnapped/dead relative as its emotional hook.

## 20. Failure and Death

Routine defeat follows GDS-5/GDS-8/GDS-12 Recovery Transit.

Narrative does not reinterpret ordinary Incapacitation as canonical death.

The baseline has no authored permanent player-character death ending.

## 21. Persistence

The save preserves:

- Callsign;
- committed player-role/story flags;
- consequential dialogue choices;
- station claim/commander status;
- faction/reputation consequences through their owning systems.

Changing presentation preferences does not rewrite committed narrative state.

## 22. Presentation and Accessibility

Dialogue UI identifies player choices clearly and supports text scaling/navigation/accessibility under GDS-13.

Because player conversational lines are unvoiced, no gameplay information may depend on hearing the protagonist speak.

## 23. Edge Cases

- If the player changes Callsign after an old log was authored as an immutable quoted historical artifact, the artifact may preserve its recorded prior name if its narrative data explicitly stores that value; ordinary dynamic UI uses the current Callsign.
- Faction hostility cannot erase the station claim unless an explicit authored world event changes physical control under the established campaign-protection rules.
- A dialogue choice cannot grant advanced technical operation if gameplay prerequisites are absent; it can only choose an outcome the target system says is feasible.
- A later cosmetic DLC/content update cannot require save migration that changes gameplay body scale/statistics.

## 24. Tuneable / Content Parameters

Authored dialogue wording, contextual titles, bark frequency, and cosmetic suit/avatar content are content parameters.

The fixed identity/backstory scope, Callsign model, independent-faction role, and unvoiced conversational protagonist are fixed baseline decisions.

## 25. Explicit Non-Goals

The player is not a chosen-one archetype, hereditary ruler, fixed faction loyalist, mandatory prewritten family character, generic morality-score avatar, or voiced conversational protagonist in the baseline.

## 26. Dependencies

Depends on GDS-5 Player Character, Setting, Main Story, Factions/Reputation, Horizon Station, Dialogue/Communication, Recovery/Failure, and GDS-13 Presentation/Accessibility.

## 27. Open Questions

None.
