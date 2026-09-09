# On-Foot Combat

> **Status:** Draft  
> **Authority:** First-person combat actions, movement-combat interaction, aiming, weapon handling, cover use, grenades, melee, healing/repair under threat, and combat-state action constraints

## 1. Purpose

On-foot combat should feel responsive and readable while remaining grounded in physical weapon, ammunition, shield, armor, and environment state.

## 2. Perspective

Normal on-foot combat is first-person.

This preserves Global Rule 3.

## 3. Baseline Combat Actions

Player combat actions include:

- Move;
- Sprint;
- Crouch;
- Jump where environment permits;
- Aim;
- Fire;
- Alternate Fire where weapon supports it;
- Reload;
- Switch Weapon;
- Melee;
- Throw/Deploy Gadget;
- Use Quick Consumable;
- Interact;
- Retreat/Extract.

## 4. No Combat Stamina Bar

Normal sprint, aiming, melee, and weapon handling do not consume a universal regenerating stamina bar.

Movement constraints come from:
- load;
- health;
- environment;
- status effects;
- equipment.

## 5. Sprint and Firing

The baseline does not allow normal accurate firing while full sprinting.

Firing input during sprint:
- exits sprint;
- transitions weapon to Ready;
- attack occurs only after weapon's sprint-to-fire time.

## 6. ADS and Sprint

Entering ADS exits sprint.

## 7. Crouch

Crouching can:
- reduce profile;
- improve weapon dispersion/recoil depending on weapon;
- reduce movement speed.

No prone stance is required for baseline.

## 8. Jump / Airborne Combat

The player can fire while airborne unless weapon/action forbids it.

Airborne dispersion is typically worse.

No arbitrary weapon lock is required merely because feet leave the ground.

## 9. Cover

There is no mandatory cover-snap system.

The player uses physical geometry naturally through first-person movement/crouch/lean only if lean is later added.

## 10. Lean

Manual lean is not required by baseline.

If added, it must preserve muzzle collision rules.

## 11. Weapon Ready State

Drawing/switching weapon requires finite handling time.

A holstered weapon cannot fire in zero time.

## 12. Hip Fire

Hip Fire:
- faster target acquisition;
- wider dispersion.

## 13. ADS

ADS:
- narrows dispersion;
- aligns sight/optic;
- can reduce movement speed by weapon;
- can use magnification.

ADS does not change raw damage.

## 14. Reload Under Threat

Reloading:
- takes finite time;
- can be interrupted;
- uses ammunition transaction rules.

## 15. Reload Cancellation

Player can cancel reload through:
- weapon switch;
- sprint;
- melee;
- firing when reload state permits interruption;
- damage/stagger where specified.

Transferred ammunition already committed remains transferred.

## 16. Weapon Switching

Switch time depends on weapon class.

Sidearms should generally draw faster than heavy primaries as a balance philosophy.

## 17. Quick Consumables

Using a medkit/consumable:
- requires inventory item;
- takes authored action time;
- can be interrupted;
- consumes item only at its defined transaction point.

## 18. Healing Under Fire

No universal "combat prevents healing" rule exists.

The player can attempt field healing if:
- action state permits;
- item available;
- not incapacitated.

Enemy pressure can interrupt it.

## 19. Personal Shield Recharge

Shield recharge follows Armor/Shields.

Combat state itself does not block recharge; taking qualifying shield damage resets Recharge Delay.

## 20. Melee

Melee is:
- close range;
- timed;
- physical hit-volume based.

Melee can be used while ammunition is empty.

## 21. Melee Interrupt

Heavy weapon firing/reload can be interrupted by melee only where weapon handling state permits it.

The exact cancel table is weapon-class data.

## 22. Grenades

Grenade use:
1. select/press grenade action;
2. begin throw/prime state;
3. release/commit throw;
4. physical projectile exists;
5. fuse/impact triggers.

The player cannot recover a consumed grenade after throw merely because it missed.

## 23. Grenade Cooking

Timed grenade cooking is allowed only for grenade models with a manual fuse.

Rockets/impact explosives do not inherit this behavior.

## 24. Self-Damage

The player can damage themselves with explosives/environmental weapon effects.

## 25. Friendly Fire

The player's physical attacks can hit allies.

Location/faction consequences remain outside raw damage math.

## 26. Weapon Collision at Close Range

Very long weapons can use presentation/aim obstruction near walls.

The baseline requires muzzle obstruction validation but not a complex weapon-lowering collision animation for every surface.

## 27. Interaction During Combat

The player can interact with:
- doors;
- consoles;
- loot;
- objectives;
- extraction

if their action rules permit.

There is no global "in combat" prohibition.

## 28. Hacking / Repair During Combat

Long technical interactions can be attempted but are vulnerable to interruption.

Owning systems define progress persistence.

## 29. Combat Movement Mobility

Baseline movement does not include:
- combat roll;
- dodge iframe;
- wall run;
- slide attack

as mandatory mechanics.

These can be added only through explicit design change/equipment capability.

## 30. No Invulnerability Frames

Ordinary jumps, crouch transitions, weapon swaps, reloads, and interactions do not grant invulnerability.

## 31. Hit Reaction

Receiving damage can produce:
- feedback;
- aim disturbance;
- Stagger only if threshold exceeded.

Every hit does not automatically cancel player input.

## 32. Critical Health

GDS-5 Critical Health can influence movement/action only through explicitly configured penalties.

Combat does not silently add random low-health aim penalties.

## 33. EVA Combat

In EVA:
- GDS-5 zero-g movement applies;
- weapon recoil can have presentation/physical impulse only where authored;
- no automatic grounded movement assumptions;
- suit energy/life support remain relevant.

## 34. Low/High Gravity Combat

Gravity changes player movement/trajectory behavior through GDS-5/World.

Hitscan weapon damage does not arbitrarily change because gravity differs.

Physical projectile arcs can change.

## 35. Combat Looting

Looting remains a normal Interaction.

The player accepts tactical risk while doing it.

## 36. Extraction Under Fire

If Extraction rules allow it, player can attempt extraction while hostiles remain.

Combat does not impose a hidden kill-all condition.

## 37. Incapacitation

At Health = 0:
- firing stops;
- movement stops;
- GDS-5 Incapacitated state owns player behavior.

## 38. Controls

Combat uses action-based input mapping.

Exact default bindings belong to controls/presentation and can be remapped.

## 39. Explicit Non-Goals

On-foot combat does not require:

- stamina bar;
- cover snapping;
- dodge i-frames;
- prone;
- universal slide;
- firing accurately while full sprint;
- combat lockout of healing/interaction;
- every-hit stagger.

## 40. Tuneable Parameters

Sprint-to-fire, ADS time, switch time, movement multipliers, crouch accuracy, airborne accuracy, heal time, grenade throw timing, and stagger thresholds are tuneable.

## 41. Dependencies

This specification depends on Player Movement/Controls/Health/Equipment, Weapons, Hit Resolution, Ammo, Armor/Shields, Status Effects, Missions, and Presentation.

## 42. Open Questions

None in the baseline first-person combat action framework.
