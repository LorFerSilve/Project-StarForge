# Combat Damage Model

> **Status:** Draft  
> **Authority:** Damage Packet structure, damage channels, hit-location multipliers, shield overflow, armor penetration/mitigation mathematics, biological/system damage output, and damage-event ordering

## 1. Purpose

The damage model converts a valid hit into deterministic consequences without random RPG damage rolls.

## 2. Damage Packet

A Damage Packet contains:

- Source Actor ID;
- Instigator/Faction context;
- Weapon/Attack ID;
- Target Actor ID;
- Hit Location ID;
- one or more Damage Components;
- Penetration value per relevant component;
- impulse;
- eligible Status Payloads;
- attack tags.

## 3. Damage Components

Canonical combat damage channels:

1. Kinetic;
2. Thermal;
3. Explosive;
4. Electrical/EMP;
5. Corrosive.

An attack can contain several components.

## 4. Kinetic

Represents:

- bullets;
- pellets;
- rail projectiles;
- fragments;
- physical impact.

Kinetic commonly interacts strongly with armor and penetration.

## 5. Thermal

Represents:

- lasers;
- plasma heat;
- incendiary/high-energy heating.

Thermal can be effective against exposed systems/organic targets depending on protection.

## 6. Explosive

Represents pressure/fragmentation energy from:

- grenades;
- rockets;
- explosive shells;
- detonations.

Blast exposure depends on distance and occlusion.

## 7. Electrical / EMP

Represents electrical disruption rather than pure biological damage.

It can:

- drain/disrupt shields;
- interfere with electronics;
- apply EMP Status;
- damage susceptible machines.

Organic biological damage from EMP is low/zero unless the attack explicitly has another damage component.

## 8. Corrosive

Represents persistent/chemical/material-degrading attacks.

It is uncommon and primarily relevant to:

- specific biological enemies;
- industrial hazards used offensively;
- alien technology.

Corrosive damage can also apply Corrosion Status.

## 9. Raw Damage

Each component begins with a non-random Raw Damage value determined by:

- weapon model;
- firing mode;
- projectile/charge state;
- distance falloff where applicable;
- hit count.

The baseline does not roll random +/- damage variance.

## 10. Distance Falloff

Weapons explicitly declare one of:

- No Falloff;
- Linear Falloff between Start/End range;
- Authored Curve.

Damage cannot fall below a weapon-defined minimum fraction unless the projectile ceases to be valid.

## 11. Hit Location

A target exposes authored Hit Locations.

For standard humanoid biological actors:

- Head;
- Torso;
- Arm Left/Right;
- Leg Left/Right.

## 12. Humanoid Default Multipliers

Default post-protection biological multipliers:

- Head: 2.0;
- Torso: 1.0;
- Arms: 0.75;
- Legs: 0.75.

These defaults are tuneable actor data but there is no random critical chance.

Armor can use different coverage per location.

## 13. Weak Points

Machines, wildlife, turrets, and ships can expose authored Weak Points.

A Weak Point defines:

- affected damage channels;
- multiplier;
- subsystem consequence;
- whether armor/shield still applies.

Weak points must be physically/visually or sensor-discoverable.

## 14. Shield Stage

Damage reaches Shield before physical armor where an active shield covers the hit.

Shield resolution is performed per Damage Component.

## 15. Shield Absorption Formula

Each shield defines a **Shield Cost Multiplier** per damage channel.

Required Capacity:

ShieldCost = IncomingDamage × ShieldCostMultiplier

If CurrentShield >= ShieldCost:
- all component damage is absorbed;
- Shield decreases by ShieldCost;
- no component damage passes onward.

If CurrentShield < ShieldCost:
- AbsorbedFraction = CurrentShield / ShieldCost;
- PassedDamage = IncomingDamage × (1 - AbsorbedFraction);
- Shield becomes zero.

This produces deterministic overflow rather than all-or-nothing collapse.

## 16. Default Shield Cost Multipliers

Baseline defaults:

- Kinetic: 1.00;
- Thermal: 1.00;
- Explosive: 1.25;
- Electrical/EMP: 2.00;
- Corrosive: 0.75.

These are balance values and can vary by shield model.

Electrical/EMP therefore drains ordinary shields efficiently.

## 17. Shield Does Not Use Hit Multipliers

Personal baseline shields absorb the incoming attack before humanoid hit-location biological multiplier.

A headshot into an intact personal shield does not consume double shield purely because it targeted the head.

Specific directional/weak-point shields can override this only if explicitly authored.

## 18. Armor Stage

Passed physical damage next resolves against the armor/protection covering the Hit Location.

Each armor layer can define a Rating by damage channel.

## 19. Penetration

An attack can provide Penetration per damage channel.

EffectiveArmor:

EffectiveArmor = max(0, ArmorRating - Penetration)

Penetration does not add bonus damage by itself.

## 20. Armor Mitigation Formula

For channels that use armor mitigation:

MitigationFraction = EffectiveArmor / (EffectiveArmor + ArmorScale)

DamageAfterArmor = IncomingDamage × (1 - MitigationFraction)

Default ArmorScale = 100.

ArmorScale is a global tuneable balance constant.

This curve never creates mathematical 100% immunity from finite armor alone.

## 21. Armor Channel Behavior

Baseline:

- Kinetic uses full ArmorRating/Penetration.
- Thermal uses Thermal Protection rating.
- Explosive uses Blast Protection rating after blast attenuation.
- Electrical/EMP uses Electronic Hardening rather than physical armor.
- Corrosive uses Chemical/Corrosion Protection.

A suit/plate can expose several ratings.

## 22. No Hidden Minimum Damage

The formula itself determines passed damage.

If a target has explicit Immunity to a channel, that is authored as a separate capability rather than achieved through extreme armor numbers.

## 23. Resistance / Immunity

Special targets can define:

- Vulnerable;
- Normal;
- Resistant;
- Immune

for a damage/status channel.

Resistance modifies damage through an explicit data multiplier.

Immunity must be justified by target design.

## 24. Physical Hit Multiplier Ordering

For ordinary biological actors:

1. shield;
2. armor/protection;
3. hit-location biological multiplier;
4. Health.

This prevents head hit multipliers from artificially increasing shield drain or armor penetration.

## 25. Biological Damage

After shield/armor and hit-location resolution, remaining compatible damage becomes Biological Damage delivered to GDS-5 Health.

## 26. Machine/System Damage

Machine actors can route damage to:

- chassis integrity;
- local subsystem;
- component condition.

GDS-10 or target-system authority owns the resulting state thresholds.

## 27. Spacecraft Damage

Ship hit resolution uses:

1. ship shield;
2. armor/structure;
3. hit zone/module routing;
4. GDS-6 Module/Hull condition.

Ship subsystem multipliers are authored per hull/module zone.

## 28. Station/Turret Damage

Station objects route remaining damage into GDS-2 structural/module state.

Combat does not directly modify power/atmosphere values except through the damaged target's owning systems.

## 29. Blast Damage

An explosion defines:

- inner radius;
- outer radius;
- maximum raw damage;
- falloff curve;
- impulse;
- occlusion behavior.

Targets inside inner radius receive maximum base blast damage.

Between inner and outer radius damage falls according to the authored curve.

Outside outer radius there is no blast damage.

## 30. Blast Occlusion

Solid cover can reduce/block blast.

The baseline uses geometry visibility samples to determine exposure.

Explosions do not damage actors through arbitrarily thick sealed walls unless the explosive explicitly breaches that structure.

## 31. Projectile Penetration Through Cover

Only attacks with explicit Material Penetration capability can pass through cover.

Penetration reduces projectile energy/damage according to material resistance and traversed thickness.

No universal bullet penetration through all geometry exists.

## 32. Multiple Pellets / Fragments

Each valid pellet/fragment produces its own hit component.

A shotgun can therefore deal more total damage at close range because more pellets physically connect.

## 33. Damage Rounding

Combat calculations use floating-point internal damage.

UI can round displayed values.

State transitions use authoritative unrounded values.

## 34. Simultaneous Damage

All damage events in one simulation step are processed in deterministic event order.

If multiple attacks reduce Health/Integrity to zero, defeat is committed once.

## 35. Overkill

Biological Health clamps at zero for gameplay.

Excess damage can inform presentation/physics but does not create negative Health progression.

## 36. Self-Damage

Explosives and hazardous attacks can damage the instigator if physically exposed.

There is no universal player self-damage immunity.

## 37. Friendly Fire

Damage math does not change because the target is friendly.

Whether an attack is permitted/penalized is an interaction/faction issue.

## 38. Status Application Timing

Eligible status payloads are evaluated after the relevant hit/damage channel reaches its target according to Status Effects.

A fully shield-absorbed corrosive projectile cannot corrode body armor unless its status explicitly affects the shield itself.

## 39. Damage Logging

For debugging/design validation, each resolved Damage Event should be reconstructable from:

- raw component;
- shield absorption;
- armor effective value;
- mitigation;
- multiplier;
- final applied damage;
- status result.

## 40. Explicit Non-Goals

The damage model does not use:

- random damage rolls;
- random critical chance;
- level-based hidden damage scaling;
- one damage number that ignores channel;
- armor as a second generic health bar;
- infinite armor immunity from finite rating.

## 41. Tuneable Parameters

Tuneable values include ArmorScale, channel multipliers, hit multipliers, falloff curves, penetration values, blast curves, and target resistance multipliers.

## 42. Dependencies

This specification depends on Armor/Shields, Weapons, Status Effects, GDS-5 Health, GDS-6 Ship Damage, GDS-2 Station Damage, and future Robots.

## 43. Open Questions

None in the damage-resolution baseline.
