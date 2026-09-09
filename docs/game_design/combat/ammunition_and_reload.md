# Ammunition and Reload

> **Status:** Draft  
> **Authority:** Ammunition item classes, carried reserve, weapon magazine/capacitor state, reload transactions, tactical/empty reload, energy-cell behavior, ammunition variants, and consumption persistence

## 1. Purpose

Ammunition must be finite, physical, and transactionally consistent with GDS-4 resource ownership.

## 2. Ammunition Ownership

Finished ammunition exists at one authoritative owner:

- station storage;
- ship cargo;
- player inventory;
- weapon internal magazine/capacitor;
- world container.

Reloading transfers ammunition; it does not create it.

## 3. Baseline Ammunition Classes

Combat owns finished ammunition item definitions.

Baseline classes:

- Sidearm Cartridge;
- Rifle Cartridge;
- Shotgun Shell;
- Precision / Heavy Cartridge;
- Rail Slug;
- Grenade Round;
- Rocket;
- Energy Cell / Weapon Charge Pack.

Exact calibers/model compatibility are content data.

## 4. Manufacturing Inputs

Ammunition recipes use canonical GDS-4 inputs such as:

- Structural Steel;
- Tungsten;
- Polymer Resin;
- Reaction Propellant;
- electronics for guided ammunition.

Finished ammunition is not added as raw GDS-4 resource material unless the resource authority later chooses to catalog a reusable bulk ammo feed.

## 5. Weapon Magazine

A conventional weapon has:

- Magazine Capacity;
- Current Loaded Rounds.

The internal loaded rounds are physically owned by the weapon while loaded.

## 6. No Individual Magazine Inventory Baseline

The baseline does **not** require simulation of detachable magazine objects.

Reserve ammunition is carried as finished ammunition stacks/counts.

Reload transfers rounds into the weapon's internal magazine.

This preserves physical ammunition without unnecessary inventory micromanagement.

## 7. Tactical Reload

If rounds remain in the weapon:

- reload preserves those rounds;
- it transfers only enough reserve ammunition to fill the magazine;
- no ammunition is discarded.

## 8. Empty Reload

If Current Loaded Rounds = 0:

- reload uses the weapon's Empty Reload duration;
- transfers up to Magazine Capacity from compatible reserve.

Empty Reload can be slower than Tactical Reload by weapon model.

## 9. Partial Reload

Weapons such as shotguns can reload incrementally.

Each shell/round transfer commits individually.

The player may interrupt after any committed transfer.

## 10. Magazine Reload

Most rifles/sidearms use one timed transfer completion.

If reload is interrupted before the commit point:
- no reserve rounds are moved unless the animation/state definition includes an earlier explicit transfer.

If interrupted after commit:
- loaded state remains.

## 11. Reload Atomicity

Every reload has explicit transaction point(s).

Save/load cannot duplicate ammunition by restoring both pre-transfer reserve and post-transfer magazine count.

## 12. Auto-Reload

When attempting to fire an empty weapon, the game may automatically initiate reload if compatible reserve ammo exists and player settings permit it.

Auto-reload uses the same real reload duration.

## 13. Manual Reload

The player can manually reload before empty.

## 14. Ammo Selection

If multiple compatible ammunition variants are carried, the player explicitly selects the active ammunition type where the weapon supports variants.

Reload does not silently consume a different rare ammunition type unless selection/fallback policy authorizes it.

## 15. Mixed Magazine

The baseline does not require mixing multiple ammunition variants inside one magazine.

Changing ammunition type reloads the weapon into one selected type.

Remaining loaded rounds are returned to reserve if the weapon can safely unload them.

## 16. Unloading

Outside immediate firing state, compatible weapons can unload current rounds back to Player Inventory.

No quantity is lost.

## 17. Ammunition Variants

Baseline ballistic variants can include:

- Standard;
- Armor-Piercing;
- High-Impact / Frangible;
- specialized EMP/electronic projectile only where technology supports it.

A variant changes explicit Damage/Penetration/Status properties.

## 18. Armor-Piercing

AP ammunition increases Penetration at the cost of an authored tradeoff such as:

- lower raw biological damage;
- higher manufacturing cost;
- lower availability.

It does not simply dominate Standard ammo in every context.

## 19. Frangible / High-Impact

Can increase unarmored biological damage while reducing penetration.

## 20. Shotgun Ammunition

Shotguns can support authored variants such as:

- Buckshot;
- Slug.

Buckshot:
- multi-trace;
- close-range spread.

Slug:
- single trace;
- higher range/penetration.

## 21. Rail Slugs

Rail weapons use dedicated high-strength projectile ammunition.

The weapon can also consume substantial electrical energy/heat.

## 22. Rockets

Rockets are discrete ammunition.

Guided variants can require:

- sensor package;
- target lock;
- electronics.

## 23. Energy Cells

Energy weapons can use a physical Energy Cell/Charge Pack as ammunition.

A cell transfers finite weapon energy.

It is distinct from general Suit Energy unless the weapon explicitly supports suit-power draw.

## 24. Rechargeable Weapon Cells

A weapon cell model can be rechargeable only if:

- the item definition says so;
- a compatible power interface exists;
- recharge consumes actual electrical energy.

Rechargeable does not mean infinite in the field.

## 25. Capacitor Weapons

Some energy weapons can have an internal capacitor charged from:

- inserted energy cell;
- external/suit power if explicitly compatible.

Capacitor charge is finite and persistent.

## 26. Overcharge Consumption

Charged/overcharged shots can consume more:
- rounds;
- cell energy;
- heat capacity

as explicitly defined.

## 27. Carried Ammo Mass/Volume

Reserve ammunition contributes to GDS-5 Player Inventory Mass and Volume.

The player must choose how much ammunition to deploy with.

## 28. Mission Defeat

Ammunition consumed during a mission remains consumed.

Unconsumed Secured Loadout ammunition returns according to GDS-5/GDS-8 failure transactions.

## 29. Looted Ammo

Mission-acquired ammunition begins Field-Unsecured.

Extraction security follows GDS-8.

## 30. Weapon Swap

A weapon carries its own current loaded state when swapped/holstered.

Reloading one weapon does not fill another compatible weapon.

## 31. Ammo Pickup

Pickup transfers actual ammunition to Player Inventory if:
- capacity;
- compatibility/storage;
- ownership rules

permit it.

## 32. No Infinite Reserve

There is no hidden infinite reserve for:
- player;
- allies;
- enemies

unless a stationary actor explicitly uses a supplied infinite scenario source for authored testing/tutorial purposes.

Enemy AI can use abstract carried ammunition counts but must obey finite combat behavior where reload/resupply matters.

## 33. Enemy Ammunition Abstraction

To avoid excessive item simulation, enemy reserve ammunition may be stored as actor ammo counts rather than fully spawned inventory objects.

On defeat, loot generation determines what physical ammunition remains recoverable.

This does not affect player ammunition conservation.

## 34. UI

The HUD eventually shows:
- loaded rounds/charge;
- reserve compatible ammo;
- selected ammo type;
- reload state;
- low/empty state.

## 35. Explicit Non-Goals

Ammunition does not provide:

- infinite reserve;
- reload duplication;
- mandatory individual magazine inventory;
- silently consumed rare ammo;
- ammunition refund on mission failure;
- ammo weight exemption.

## 36. Tuneable Parameters

Magazine size, reload time, carried mass/volume, recipe quantities, ammo variant stats, recharge rate, and cell capacity are tuneable.

## 37. Dependencies

This specification depends on Weapons, GDS-4 Resources/Crafting, GDS-5 Inventory/Equipment, GDS-8 Mission security/failure, and Presentation.

## 38. Open Questions

None in the ammunition/reload baseline.
