# Weapons

> **Status:** Draft  
> **Authority:** Canonical baseline weapon families, firing modes, weapon state, accuracy/recoil/heat behavior, modification boundary, melee/explosive categories, and weapon progression philosophy

## 1. Purpose

Weapons provide differentiated combat tools through physical behavior, damage channels, range, ammunition, heat, and handling rather than loot-rarity stat inflation.

## 2. Weapon Identity

Every weapon model defines:

- Weapon ID;
- family;
- slot compatibility;
- firing geometry class;
- firing modes;
- damage components;
- penetration;
- effective range/falloff;
- fire rate;
- magazine/capacitor capacity;
- ammunition/energy type;
- reload/recharge behavior;
- recoil;
- dispersion;
- heat behavior where applicable;
- mass;
- condition;
- modification sockets;
- required Blueprint/Research.

## 3. No Loot-Rarity Weapon Tiers

The baseline does not use:

- Common;
- Rare;
- Epic;
- Legendary

versions of the same weapon with random stat multipliers.

Weapon progression comes from:

- new models;
- new technologies;
- specialization;
- modifications;
- ammunition types;
- better manufacturing access.

## 4. Canonical Weapon Families

Baseline on-foot families:

1. Ballistic Sidearm;
2. Ballistic Rifle;
3. Shotgun;
4. Precision / Marksman Weapon;
5. Rail Weapon;
6. Laser Weapon;
7. Plasma Weapon;
8. Explosive Launcher;
9. Melee Weapon;
10. Mission/Utility Weapon where explicitly authored.

## 5. Ballistic Sidearm

Role:

- compact fallback;
- fast handling;
- short/medium range;
- moderate damage;
- low carrying burden.

Baseline firing:
- semi-automatic hitscan.

Typical damage:
- Kinetic.

## 6. Ballistic Rifle

Role:

- primary general-purpose firearm;
- medium range;
- flexible sustained fire.

Possible modes:
- semi;
- burst;
- automatic.

Typical damage:
- Kinetic.

## 7. Shotgun

Role:

- close-range stopping power;
- multiple pellets;
- strong short-range damage;
- lower effective range.

Baseline firing:
- multi-trace pellet spread.

Shotgun damage emerges from physically connecting pellets, not one hidden range multiplier alone.

## 8. Precision / Marksman Weapon

Role:

- long-range accurate fire;
- weak-point targeting;
- high penetration;
- slower handling/fire cadence.

Baseline firing:
- hitscan.

It can be ballistic or energy-based by model, but the family emphasizes precision.

## 9. Rail Weapon

Role:

- very high projectile velocity;
- high penetration;
- strong weak-point/subsystem attack;
- heavy power/heat/carry cost.

Baseline firing:
- hitscan gameplay approximation.

Typical damage:
- Kinetic.

Rail weapons can support entity/material penetration when explicitly defined.

## 10. Laser Weapon

Role:

- precision energy weapon;
- instantaneous line-of-sight attack;
- low/no ballistic lead;
- power/heat management.

Baseline firing:
- hitscan beam or pulse.

Typical damage:
- Thermal.

A laser does not curve or pass through opaque cover.

## 11. Plasma Weapon

Role:

- high-energy short/medium range;
- visible projectile;
- high thermal damage;
- dodgeable at longer range.

Baseline firing:
- physical projectile.

Typical damage:
- Thermal;
- optional Kinetic/impact component.

## 12. Explosive Launcher

Role:

- area denial;
- grouped targets;
- destructible combat objects;
- cover displacement.

Baseline projectile types:
- grenade;
- rocket.

Typical damage:
- Explosive;
- optional Kinetic/Thermal secondary component.

## 13. Melee Weapon

Role:

- close-range fallback;
- silent/low-ammo use;
- selected armor/utility roles.

Melee uses:
- attack timing;
- swept volume;
- authored damage packet.

No universal stamina bar is required.

## 14. Utility / Mission Weapon

A mission can provide a nonstandard weapon/tool such as:

- cutting charge launcher;
- anti-machine disruptor;
- containment projector.

It must still use explicit combat rules and cannot remain undefined because it is "mission-specific."

## 15. Firing Modes

Possible firing modes:

- Semi-Automatic;
- Burst;
- Automatic;
- Charged;
- Continuous Beam;
- Alternate Fire.

A weapon exposes only authored modes.

## 16. Semi-Automatic

One attack per valid trigger press/cycle.

## 17. Burst

One trigger input schedules a fixed authored number of shots at authored cadence.

A burst stops early if:
- ammunition exhausted;
- weapon disabled;
- fire action invalidated.

## 18. Automatic

Weapon continues firing while input remains active and firing conditions are valid.

## 19. Charged Fire

Holding input increases charge until:

- release;
- maximum charge;
- interruption.

Charge can affect:
- damage;
- penetration;
- projectile velocity;
- energy/heat cost.

The exact relation is weapon data.

## 20. Continuous Beam

Continuous laser-type fire resolves repeated damage ticks while:

- input held;
- line of sight remains;
- power/ammo valid;
- heat limit not exceeded.

Damage tick frequency is implementation/balance data but total damage must be framerate-independent.

## 21. Fire Validation

A weapon cannot fire when:

- Disabled;
- no required ammunition/energy;
- reload state blocks firing;
- mandatory cooldown not complete;
- overheat lockout active;
- safety/location rule blocks attack;
- required charge/firing state invalid.

## 22. Weapon Heat

Some weapons produce heat.

Weapon heat state:

- Normal;
- Elevated;
- Critical;
- Overheated.

## 23. Overheat

At Overheated:

- firing is blocked or severely limited according to model;
- cooling begins according to weapon design;
- heat does not instantly reset.

## 24. Passive Cooling

Weapon heat can dissipate to environment over active time.

No infinite free cooling is assumed where the weapon is in an environment that explicitly impairs heat rejection.

For portable weapons the environmental model can remain simplified.

## 25. Recoil

Weapon definitions provide:

- vertical recoil impulse;
- horizontal pattern/range;
- recovery rate.

Recoil is applied to aim/weapon state.

## 26. Dispersion

Weapons provide separate:

- Hip Dispersion;
- ADS Dispersion;
- Movement Multipliers;
- Sustained-Fire Bloom where relevant.

## 27. First-Shot Precision

A weapon can declare First-Shot Precision after an authored recovery/settle condition.

It does not mean every ADS shot is perfectly accurate.

## 28. Damage Falloff

Each weapon explicitly defines falloff behavior.

Examples:

- shotgun begins falloff early;
- rifle later;
- rail/laser may have minimal gameplay-range falloff.

## 29. Ammunition

Physical ammunition/reload behavior is authoritative in ammunition_and_reload.md.

Equipping a weapon never creates ammunition.

## 30. Condition

Persistent weapon condition follows GDS-5 Equipment.

Condition can affect documented properties such as:

- dispersion;
- heat efficiency;
- reload reliability;
- maximum charge;
- operation availability.

The baseline does not require random weapon jams.

## 31. No Random Jam Baseline

Routine weapon use does not randomly jam because of an invisible dice roll.

A weapon can enter malfunction state only from:

- explicit severe condition rule;
- damage;
- EMP;
- authored environment/system effect.

## 32. Modifications

Weapon models can expose authored sockets such as:

- Optic;
- Barrel/Muzzle;
- Magazine/Feed;
- Power/Emitter;
- Utility.

A modification must define exact compatible models/effects.

## 33. No Universal Attachment Stack

Not every weapon supports every modification.

A railgun cannot automatically accept a ballistic suppressor just because both are "weapons."

## 34. Suppressors

A compatible ballistic suppressor can reduce audible detection signature.

It does not make the weapon silent.

Enemy hearing/perception is AI authority.

## 35. Optics

Optics affect:

- sight picture;
- zoom/magnification;
- aiming usability.

They do not increase raw damage.

## 36. Weapon Acquisition

Weapons can be:

- crafted;
- purchased later through economy;
- recovered;
- rewarded;
- salvaged intact where allowed.

Manufacturing requires GDS-4 Blueprint/Research/resources.

## 37. Faction Weapon Identity

Typical specialization:

- Helios: ballistic, rail, disciplined military weapons;
- Meridian: industrial/utility ballistic and heavy systems;
- Asterion: laser, plasma, shield-disruption technology;
- Freehold: practical repairable ballistic/mixed weapons;
- Blackwake: modified ballistic, shotgun, explosive, boarding-focused weapons;
- Continuance: integrated energy/electrical machine weapons.

These are profiles, not exclusivity rules.

## 38. Safe Handling

Weapons can be:

- Holstered;
- Ready;
- Aiming;
- Firing;
- Reloading;
- Cooling/Overheated;
- Disabled.

Transitions take authored time.

## 39. Persistence

Weapon:
- magazine/capacitor state;
- heat;
- condition;
- firing mode;
- modifications

persist through valid saves.

## 40. Explicit Non-Goals

Weapons do not use:

- random rarity stat tiers;
- random crit chance;
- infinite ammunition;
- random damage variance;
- mandatory random jams;
- attachment compatibility with everything;
- weapon damage scaling from Player Level.

## 41. Tuneable Parameters

Damage, penetration, range, fire rate, recoil, dispersion, reload time, heat, charge, projectile speed, and attachment effects are tuneable.

## 42. Dependencies

This specification depends on Damage Model, Hit Resolution, Ammunition, Equipment, Resources, Research/Blueprints, Status Effects, AI, and Presentation.

## 43. Open Questions

None in the baseline weapon-family framework.
