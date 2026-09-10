# Player Design

> **Domain Status:** Design Complete

Authoritative design domain for the persistent player character and direct on-foot interaction.

## Specifications

- [`player_character.md`](player_character.md) — **Design Complete**
- [`controls_and_input.md`](controls_and_input.md) — **Design Complete**
- [`movement.md`](movement.md) — **Design Complete**
- [`interaction.md`](interaction.md) — **Design Complete**
- [`health_damage_death.md`](health_damage_death.md) — **Design Complete**
- [`inventory.md`](inventory.md) — **Design Complete**
- [`equipment.md`](equipment.md) — **Design Complete**
- [`tools_and_gadgets.md`](tools_and_gadgets.md) — **Design Complete**
- [`field_survival.md`](field_survival.md) — **Design Complete**
- [`CROSS_VALIDATION.md`](CROSS_VALIDATION.md) — historical first-pass validation artifact

## Authority Boundary

This domain owns player-character persistence, direct controls, on-foot movement, interaction, biological Health/Incapacitation, portable inventory, equipment, tools/gadgets, and environmental survival.

GDS-9 owns attack/damage resolution. GDS-8 owns mission/extraction/failure lifecycle. GDS-6 owns spacecraft state. Station/World own external environmental state. GDS-12 owns economy/progression/Difficulty/persistence coordination and Recovery Transit. GDS-13 owns presentation/accessibility.

## Design-Complete Baseline

The Player domain fixes:

- first-person on-foot play and one persistent physical player body;
- Callsign/display-name customization without a required face/body editor or voiced conversational protagonist;
- no Player Level or generic RPG skill tree;
- explicit keyboard/mouse and controller defaults with full rebinding;
- no global hidden input buffer;
- walk/sprint/crouch/jump/mantle/ladder/Zero-G/Magnetic-Boot locomotion;
- no sprint stamina, prone, swimming, or universal dash/dodge;
- deterministic Health bands and fixed Critical-state restrictions;
- no routine player permadeath or zero-time defeat teleport;
- Minimum Emergency Recovery to the Wounded threshold for anti-softlock;
- Mass + Volume Inventory with physical ownership and four Quick Slots;
- explicit Auto Pickup whitelist and Take-All conservation behavior;
- Equipment Condition/Repair Profiles and finite Suit Energy;
- Engineering/Mining/Salvage/Scanner/Sample tools plus five canonical gadget classes;
- finite Life-Support Reserve and exact radiation-dose semantics;
- no personal hunger/thirst/sleep/fatigue loop.

## GDS-14 Promotion Result

The Design Complete Promotion Sweep resolved the remaining implementation-relevant ambiguities involving:

- controller defaults and input buffering;
- Health-band behavior;
- Recovery Transit and emergency medical fallback;
- character customization/voice scope;
- Auto Pickup and Take All;
- equipment repair/swap profiles;
- Radiation Dose consequences;
- Tool/Gadget class/deployment behavior;
- Simulation-Time terminology.

No implementation-time gameplay decision remains inside the Player domain.

## Completion State

**Design Complete**

This status authorizes the Player rules to be used as stable design input for later technical architecture. It does not itself start implementation or mark the domain Implementation Locked.
