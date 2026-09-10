# Hit Resolution and Targeting

> **Status:** Design Complete  
> **Authority:** Ranged/melee hit detection, hitscan/projectile classification, spread, recoil relationship, target collision zones, aim assistance boundary, cover/occlusion, and weak-point targeting

## 1. Purpose

Hit resolution determines whether an attack physically connects before Damage Model processing.

## 2. No RPG Hit Roll

If a valid trace/projectile/melee volume intersects a target collider, the attack hits.

If it does not, the attack misses.

There is no secondary hidden accuracy percentage roll.

## 3. Attack Geometry Classes

Baseline:

- Hitscan Trace;
- Ballistic Multi-Trace;
- Physical Projectile;
- Melee Sweep/Volume;
- Area/Explosion.

## 4. Hitscan Trace

Used by baseline:

- conventional ballistic small arms;
- rail weapons;
- laser weapons.

A hitscan trace resolves from muzzle/weapon origin along final aim direction to maximum range.

This is a gameplay approximation suitable for combat distances.

## 5. Ballistic Multi-Trace

Used by shotgun/pellet attacks.

Each pellet has its own deterministic seeded dispersion direction.

## 6. Physical Projectile

Used by:

- plasma bolts;
- rockets;
- thrown grenades;
- selected slow heavy ordnance.

Projectile state includes:

- position;
- velocity;
- collision;
- lifetime;
- owner;
- payload.

## 7. Melee Sweep

Melee attacks use an authored attack window and swept collision volume.

One target is normally damaged once per attack swing unless the attack explicitly supports multi-hit.

## 8. Aim Origin

First-person on-foot aiming uses camera aim intent but validates the actual weapon/muzzle path.

A shot cannot pass through nearby cover merely because the camera can see around it.

## 9. Camera-to-Muzzle Reconciliation

Canonical process:

1. camera produces intended aim point/line;
2. weapon computes direction from muzzle toward that aim point;
3. muzzle trace/projectile resolves physical obstruction.

This prevents shooting through walls beside the player's head.

## 10. Hip-Fire Dispersion

Hip fire applies weapon dispersion around intended aim direction.

Dispersion magnitude is affected by weapon state and movement.

## 11. ADS Dispersion

ADS reduces dispersion to the weapon's precision value.

Precision weapons can approach near-zero dispersion when stationary/settled.

ADS does not remove recoil.

## 12. Movement Accuracy

Movement can increase dispersion.

Baseline relative ordering:

Stationary/Crouched best  
→ Stationary/Standing  
→ Walking  
→ Sprint-transition / airborne worst.

Exact multipliers are tuneable.

## 13. Recoil vs Dispersion

Recoil moves the player's aim/weapon.

Dispersion varies the actual shot direction around current aim.

They are separate mechanics and both are visible/testable.

## 14. Sustained-Fire Bloom

Automatic weapons can increase dispersion during sustained fire.

Bloom recovers when firing stops according to weapon data.

No bloom applies to a weapon whose definition declares fixed accuracy.

## 15. Deterministic Randomness

Pellet/spread sampling uses deterministic per-shot seeds for simulation/save/debug reproducibility.

The player cannot reroll a fired shot through frame-rate variation.

## 16. Hit Colliders

Combat actors expose simplified combat collision zones aligned with visible anatomy/components.

Visual mesh triangles are not required as the sole hit-detection representation.

## 17. Humanoid Hit Zones

Baseline:

- Head;
- Torso;
- Left Arm;
- Right Arm;
- Left Leg;
- Right Leg.

Armor coverage maps to these zones.

## 18. Weak-Point Zones

Machines/wildlife/ships can expose dedicated Weak-Point colliders.

Weak points must correspond to:

- visible component;
- plausible vulnerable anatomy;
- sensor-discoverable subsystem.

## 19. Friendly / Hostile Targeting

Hit detection is faction-neutral.

A trace does not ignore a friendly body to hit a hostile behind it.

## 20. Penetrating Traces

A weapon with Material Penetration can continue after a surface if remaining penetration budget exceeds material resistance.

The projectile/trace loses damage/penetration according to material definition.

## 21. Entity Penetration

Entity penetration is not universal.

A high-penetration rail/kinetic attack may pass through an actor only if the weapon profile supports it.

## 22. Ricochet

Ricochet is optional per weapon/material.

The baseline only enables ricochet for authored high-angle interactions.

It is not required for all ballistic weapons.

## 23. Laser Behavior

Baseline laser weapons are instantaneous hitscan beams.

They require line of sight and do not curve.

## 24. Plasma Behavior

Baseline plasma is a visible physical projectile.

It can be dodged at suitable ranges.

## 25. Rocket Behavior

Rockets are physical projectiles.

Guided rockets require target lock/guidance capability.

## 26. Grenades

Thrown grenades use physical ballistic trajectory and fuse/impact rules defined by grenade model.

## 27. Melee Reach

Melee requires target intersection inside actual attack volume.

No magnetic teleport-lunge is baseline.

A short authored step/lunge can exist if the animation/movement rule physically moves the attacker.

## 28. Aim Assistance

Because the core target platform/controls are not yet presentation-finalized, GDS-9 allows optional aim assistance as an Accessibility/Controller feature.

Aim assistance:

- cannot create hits through cover;
- cannot target undiscovered/invisible enemies;
- does not change weapon damage;
- is finalized in GDS-13.

Mouse input receives no mandatory target-snapping baseline.

## 29. Target Lock

On-foot conventional firearms do not use universal hard lock-on.

Specific guided weapons/gadgets can use lock systems.

Spacecraft targeting uses GDS-6/GDS-9 ship targeting.

## 30. Occlusion

Solid collision blocks hits unless penetration/explosion rules explicitly overcome it.

## 31. Hit Confirmation

UI can provide hit markers or equivalent feedback only after authoritative hit resolution.

Weak-point/shield/armor feedback can be differentiated.

## 32. Network Boundary

Core game is single-player.

The baseline does not require multiplayer lag compensation.

Future multiplayer must adapt without changing the core deterministic hit rules.

## 33. Edge Cases

If camera line sees the target but muzzle is blocked, the shot hits the blocker.

If an enemy limb is behind cover but head is exposed, only exposed collision zones can be hit.

If a projectile is destroyed before impact, its direct-hit Damage Packet never occurs; explosion payload may still occur if its definition detonates.

## 34. Explicit Non-Goals

Hit resolution does not use:

- hidden hit chance;
- bullets originating from camera through cover;
- universal lock-on;
- universal ricochet;
- magical melee magnetism;
- friendly-target phasing.

## 35. Tuneable Parameters

Tuneable values include dispersion, recoil, collision sizes, projectile velocity, penetration resistance, ADS multipliers, bloom/recovery, and aim-assist parameters.

## 36. Dependencies

This specification depends on Weapons, Damage Model, Player Movement/Controls, Spacecraft Targeting, AI, and Presentation.

## 37. Open Questions

None in the hit-resolution baseline.
