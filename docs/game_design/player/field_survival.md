# Player Field Survival

> **Status:** Draft  
> **Authority:** Player environmental protection, suit sealing, life-support reserve, pressure/oxygen/contaminant/temperature/radiation exposure, EVA safety, and environmental failure behavior

## 1. Purpose

Field Survival defines how the player's equipment interacts with dangerous environments.

Survival is preparation and engineering, not a constant hunger/thirst loop.

## 2. Environmental Inputs

The environment can report:

- total pressure;
- oxygen availability;
- carbon-dioxide/contaminant state;
- temperature;
- radiation;
- vacuum;
- fire/thermal hazard;
- special mission hazards.

## 3. Survival Layers

Player environmental safety is evaluated through:

1. external environment;
2. suit/helmet seal;
3. suit protection ratings;
4. Suit Life-Support Reserve;
5. suit energy for active protection;
6. biological exposure consequences.

## 4. Breathable External Atmosphere

An environment is externally breathable only when its atmosphere is within safe pressure, oxygen, contaminant, and temperature limits.

Field Survival consumes the atmosphere system's classification rather than redefining gas simulation.

## 5. Suit Seal

A pressure suit is environmentally sealed only when:

- compatible Suit is equipped;
- compatible Helmet is equipped and closed;
- both retain sufficient seal condition;
- no explicit breach is active.

## 6. Automatic Environmental Mode

The suit automatically chooses between:

- External-Air Mode;
- Sealed Life-Support Mode.

External-Air Mode is preferred when the environment is safely breathable.

Sealed Mode activates when external air is unsafe and the suit can protect the player.

The player can manually force Sealed Mode to avoid contamination exposure.

## 7. Suit Life-Support Reserve

A sealed suit has a finite **Life-Support Reserve**.

This abstraction represents the combined consumable capacity required to maintain breathable internal atmosphere, including stored breathing gas and scrubbing capability.

The HUD presents remaining endurance under current conditions.

## 8. Reserve Consumption

Life-Support Reserve decreases while Sealed Mode is providing independent atmosphere.

Consumption can vary with:

- activity;
- suit technology;
- damage;
- environment;
- emergency operating mode.

Exact rates are tuneable.

## 9. Reserve Refill

Life-Support Reserve can be restored at compatible:

- station life-support interface;
- spacecraft life-support interface;
- field refill station;
- portable life-support canister where defined.

Refill transfers real consumable capacity from the source.

## 10. No Infinite Suit Oxygen

A sealed suit cannot support indefinite EVA without resupply.

When reserve reaches critical thresholds, the player receives escalating warnings.

## 11. Reserve Depletion

At zero usable Life-Support Reserve in an unbreathable environment:

- suit can no longer maintain breathable internal atmosphere;
- biological exposure begins;
- Health receives consequences.

The suit does not instantly kill the player at the exact zero boundary; exposure severity progresses according to environment and tuneable timing.

## 12. Pressure Protection

A sealed pressure-rated suit protects against vacuum/low pressure within its rating.

If pressure protection is absent or seal fails:

- decompression exposure begins;
- Health receives pressure-related consequences.

## 13. Suit Breach

Damage can create a suit breach.

A breach can:

- increase Life-Support Reserve loss;
- reduce pressure protection;
- expose the player to contaminants.

A compatible emergency patch can temporarily stabilize the breach.

## 14. Emergency Seal Patch

The Engineering Multitool/field repair item can apply an emergency seal patch when:

- breach is accessible;
- patch consumable exists;
- interaction completes.

A patch restores temporary seal capability but does not count as full equipment repair.

## 15. Temperature Protection

Suit defines safe external temperature range.

Outside that range:

- active thermal regulation uses Suit Energy;
- exposure severity increases if regulation is insufficient;
- extreme conditions can damage equipment/Health.

## 16. Thermal Energy Priority

Thermal regulation is a high-priority suit consumer.

Low energy can force shutdown of lower-priority systems such as shield/scanner before environmental regulation is sacrificed.

## 17. Contaminants

A sealed suit protects from external contaminants while seal integrity is valid.

An unsealed player uses external atmosphere classification and can suffer contaminant exposure.

## 18. Radiation

Suit defines radiation protection rating.

Radiation exposure accumulates a **Radiation Dose** during hazardous exposure.

Dose is not a constantly decreasing health bar.

## 19. Radiation Dose States

Baseline states:

- Nominal;
- Elevated;
- High;
- Critical.

Higher dose can produce temporary health/recovery consequences.

Exact long-term medical effects are tuneable and later medical content may refine them.

## 20. Radiation Recovery

Radiation Dose decreases through:

- time in safe environment;
- medical treatment;
- advanced medication/technology.

It does not instantly reset when leaving the hazard.

## 21. Radiation Detection

The player receives radiation information only if:

- suit/helmet sensor supports it; or
- another valid sensor source provides it.

Unknown radiation can remain a hazard until detected.

## 22. Fire

Suit thermal/fire protection can reduce exposure to fire.

Fire damage itself is resolved by Health/Combat/Hazard systems.

## 23. Liquid/Corrosive Hazards

Special mission environments may define corrosive or chemical contact hazards.

Protection requires explicitly compatible suit rating.

There is no generic assumption that a vacuum suit resists every hazard.

## 24. Suit Energy

Active systems can depend on Suit Energy:

- thermal regulation;
- zero-g thrusters;
- personal shield;
- scanners/tools;
- selected filters/pumps.

Life-support safety has priority according to Equipment rules.

## 25. Energy Depletion

At zero Suit Energy:

- passive armor/seal remains if physically intact;
- active shield stops;
- active thermal regulation stops;
- thrusters stop;
- powered scanning/tools stop;
- passive life-support reserve can continue only if the suit model supports passive operation.

The baseline starting sealed suit supports passive emergency life support for a limited reserve period even at zero energy.

## 26. EVA

EVA requires:

- sealed pressure protection;
- Life-Support Reserve;
- compatible locomotion such as suit thrusters or Magnetic Boots;
- sufficient environment protection.

The player is warned before leaving a safe airlock if critical EVA requirements are missing.

## 27. Airlock Preflight Check

Before normal external airlock cycling, the suit system checks:

- Suit equipped;
- Helmet sealed;
- pressure protection valid;
- Life-Support Reserve above minimum safe-launch threshold;
- suit condition not Critical/Disabled.

The player can only bypass a failed safety check via an explicit dangerous override.

## 28. Mission Preparation

Mission briefing/loadout can display known environment requirements such as:

- vacuum;
- radiation;
- extreme cold;
- toxic atmosphere.

Unknown hazards remain legitimately unknown.

## 29. No Hunger/Thirst

The player has no routine personal hunger or thirst meter.

Long-duration mission provisioning can use authored mission consumables, but it is not a global always-running metabolism system.

## 30. No Sleep/Fatigue Meter

The baseline has no personal sleep/fatigue meter.

Mission pacing and health consequences should not require the player to stop playing to sleep.

## 31. Environmental HUD

The HUD can communicate:

- external atmosphere safety;
- suit seal;
- Life-Support Reserve;
- Suit Energy;
- temperature warning;
- radiation dose;
- contaminant warning;
- current EVA state.

## 32. Warning Priority

Critical environmental warnings override noncritical tool/loot notifications.

Warnings escalate before protection is fully exhausted when sensor data exists.

## 33. Environmental Interaction

Opening helmet, removing suit, or disabling protection in a hazardous environment uses explicit confirmation/safety interlock.

## 34. Health Integration

Field Survival reports exposure severity/state.

Health owns:

- biological damage;
- incapacitation;
- recovery.

The same environmental exposure rules apply whether the danger comes from a station breach or mission world.

## 35. Persistence

Life-Support Reserve, Suit Energy, Radiation Dose, suit breach state, and relevant protective condition persist.

Save/load cannot refill environmental consumables.

## 36. Edge Cases

If external air becomes breathable while sealed, Auto Mode can switch to External-Air Mode after safety validation.

If external air becomes unsafe, Auto Mode seals automatically if gear is capable.

If the helmet is damaged below sealing capability during EVA, reserve loss/exposure begins immediately according to breach severity.

If Suit Energy reaches zero while drifting in zero-g, thrusters stop; momentum persists and rescue/physical contact may be required.

If the player enters a breathable room with high radiation, breathable-air status does not imply radiation safety.

## 37. Progression

Early game:

- short Life-Support Reserve;
- basic thermal protection;
- limited radiation protection;
- short EVA range.

Mid game:

- longer endurance;
- stronger seal;
- Magnetic Boots;
- improved radiation/thermal protection;
- better sensors.

Late game:

- extended EVA;
- advanced hazardous-environment protection;
- efficient regulation;
- strong radiation shielding;
- specialized expedition suits.

## 38. Explicit Non-Goals

Field Survival does not require:

- personal hunger/thirst;
- sleep/fatigue;
- infinite oxygen;
- one suit protecting against every hazard;
- instant environmental death without readable exposure;
- safety checks that cannot be deliberately overridden when the design permits dangerous action.

## 39. Tuneable Parameters

Tuneable values include Life-Support Reserve capacity/consumption, refill rate, seal-loss rate, exposure thresholds, temperature ranges, radiation rates, dose recovery, and safety-check thresholds.

## 40. Dependencies

This specification depends on Equipment, Health, Movement, Tools, Inventory, station atmosphere/thermal, Missions, World hazards, Spacecraft life support, and Combat damage.

## 41. Open Questions

None in the environmental-survival baseline.

Exact content values and exotic hazard types remain future tuneable/content definitions.
