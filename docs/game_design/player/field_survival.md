# Player Field Survival

> **Status:** Design Complete  
> **Authority:** Player environmental protection, suit sealing, Life-Support Reserve, pressure/oxygen/contaminant/temperature/radiation exposure, EVA safety, and environmental failure behavior

## 1. Purpose

Field Survival defines how the player's physical equipment interacts with hazardous environments.

Survival is based on preparation, protection, endurance, and engineering. It is not a constant hunger/thirst/sleep loop.

## 2. Environmental Inputs

Field Survival consumes authoritative environmental values/states including:

- total pressure;
- breathable oxygen availability;
- CO2/contaminant state;
- temperature;
- radiation intensity;
- vacuum;
- fire/thermal hazard;
- explicitly authored special mission hazards.

It does not duplicate station/world atmosphere or hazard simulation.

## 3. Evaluation Order

Player environmental safety is evaluated through:

1. external environment state;
2. Suit/Helmet seal validity;
3. passive protection ratings;
4. Life-Support Reserve when independent atmosphere is needed;
5. Suit Energy for active protection/processing;
6. accumulated exposure states;
7. biological Health/status consequences.

## 4. Breathable Environment

External atmosphere counts as breathable only when all relevant conditions are within the owning environment's safe thresholds:

- pressure;
- oxygen partial availability;
- CO2;
- contaminants;
- temperature for unprotected ordinary breathing/occupancy.

Radiation safety is evaluated separately: breathable air does not imply a radiation-safe location.

## 5. Valid Environmental Seal

The player is sealed only when:

- a compatible pressure Suit is equipped;
- a compatible Helmet is equipped and closed;
- both retain sufficient declared seal Condition;
- no active breach exceeds emergency sealing capability.

Seal validity is derived from equipment state and is not a separate invulnerability flag.

## 6. Suit Atmosphere Modes

The suit has two atmosphere modes:

- `SuitAtmosphereMode::ExternalAir`;
- `SuitAtmosphereMode::SealedLifeSupport`.

Automatic mode selection follows this rule:

- use ExternalAir only after available sensors/known environment state verifies it is safe;
- switch to SealedLifeSupport when external atmosphere becomes unsafe and the suit can seal;
- remain SealedLifeSupport when safety cannot be verified;
- the player may force SealedLifeSupport manually;
- the player may force opening/unsealing only through an explicit dangerous override when equipment physically permits it.

## 7. Life-Support Reserve

A sealed Suit has a finite **Life-Support Reserve** representing its combined stored breathable atmosphere/scrubbing consumable endurance.

While independent atmosphere is required, the reserve decreases on Simulation Time according to the Suit's defined consumption profile.

The HUD shows remaining endurance using current known consumption rate.

## 8. Life-Support Consumption

Consumption rate may be parameterized by documented inputs such as:

- player activity class;
- Suit model/efficiency;
- seal damage/leak severity;
- emergency operating mode.

Those coefficients are tuneable item data. No hidden difficulty-independent random consumption exists.

## 9. Life-Support Refill

Reserve can be replenished only through a compatible source:

- Horizon life-support interface;
- spacecraft life-support interface;
- field refill station;
- physical portable canister accepted by the Suit.

Refill consumes/transfers the actual stored capacity/resource from the source and obeys throughput/capacity limits.

## 10. Reserve Depletion

At zero usable Life-Support Reserve in an unbreathable environment:

- the Suit can no longer maintain breathable internal atmosphere;
- exposure begins immediately on Simulation Time;
- biological consequences increase through the owning exposure/Health model.

The exact zero boundary is not instant death. Health loss/incapacitation follows tuneable exposure rates so warnings/recovery remain meaningful.

## 11. Pressure Protection

A valid sealed Suit protects against vacuum/low pressure only within its authored pressure rating.

Absent/failed protection produces pressure exposure and passes the resulting biological consequence to Player Health.

## 12. Suit Breach

A breach has a defined severity and can:

- increase Life-Support Reserve loss;
- lower/disable pressure protection;
- admit contaminants;
- trigger known alarms.

Breach state persists until repaired/stabilized.

## 13. Emergency Seal Patch

An eligible accessible Suit breach may receive an emergency patch when:

- the required repair tool/mode is available;
- a compatible patch consumable exists;
- the sustained interaction completes.

The patch restores only the temporary seal capability declared by the repair profile. It never counts as full equipment restoration unless the item explicitly says so.

## 14. Temperature Protection

Each Suit declares a passive safe external temperature envelope and any active thermal-regulation capability.

Outside passive range:

- active regulation consumes Suit Energy when available;
- protection remains bounded by the Suit's capacity;
- residual exposure can create equipment/Health consequences.

## 15. Thermal Energy Priority

Environmental life-support/thermal protection uses the Equipment low-energy priority system and is protected above Personal Shield, scanning, and discretionary tools unless an explicit dangerous override changes policy.

## 16. Contaminants

A valid sealed Suit blocks external contaminants up to its authored compatibility/rating.

An unsealed player or an incompatible/damaged protection setup receives the owning contaminant exposure.

A pressure suit is not automatically chemical-proof.

## 17. Radiation Dose

Radiation exposure accumulates persistent **Radiation Dose** on Simulation Time after protection mitigation.

Dose uses `RadiationDoseState`:

- Nominal;
- Elevated;
- High;
- Critical.

State thresholds are tuneable; state behavior is fixed.

## 18. Radiation Dose Behaviors

### Nominal

- no radiation-specific capability penalty;
- ordinary background/fully managed exposure.

### Elevated

- visible warning/diagnostic state when the player has valid detection;
- no direct movement/combat penalty;
- further exposure continues accumulating dose.

### High

- applies `StatusEffect::RadiationSickness`;
- while dose remains High or Critical, Radiation Sickness produces tuneable gradual biological Health loss/recovery burden;
- it does not alter aim/recoil invisibly;
- new external deployment is allowed only if mission preparation verifies protection appropriate to the known hazard and the player is otherwise medically deployable.

### Critical

- retains Radiation Sickness at the stronger authored Critical rate;
- blocks starting a new external deployment from a safe staging location until dose falls below Critical;
- does not instantly set Health to zero;
- triggers highest applicable known medical/environmental warning.

## 19. Radiation Sickness

`RadiationSickness` is an explicit persistent Status Effect linked to dose state.

It is applied at entry to High and remains while dose is High/Critical.

Its biological Health-damage rate and medical recovery burden are tuneable.

It is removed when dose falls below High and any authored immediate treatment completion has committed.

## 20. Radiation Dose Recovery

Dose decreases only on Simulation Time through one or both valid processes:

- slow natural clearance while in a radiation-safe environment;
- explicit medical treatment/medication/technology that increases/removes dose according to its authored effect.

No offline reduction occurs.

Leaving the hazard does not instantly reset dose.

## 21. Radiation Detection

Exact radiation state/intensity is presented only when a valid suit/helmet/scanner/other sensor source provides it.

Without detection the physical exposure still occurs, but UI cannot reveal hidden source/intensity merely because the player is being affected. Symptoms/Health consequences may themselves become legitimate information.

## 22. Fire and Thermal Hazards

Suit fire/thermal protection reduces exposure only according to its explicit rating.

Fire simulation and resulting damage/status are owned by Station/Mission/Combat hazard rules.

## 23. Corrosive / Chemical Contact Hazards

Special hazards declare a Protection Requirement/compatibility tag.

If current Equipment lacks it, the hazard applies its explicit exposure state. Generic vacuum sealing does not satisfy unrelated chemical/corrosive protection.

## 24. Suit Energy Failure

At zero Suit Energy:

- passive armor remains according to Equipment;
- passive physical seal remains if Condition supports it;
- powered Personal Shield stops;
- active thermal regulation stops;
- Zero-G thrusters stop;
- powered scanning/tools stop;
- powered life-support machinery stops if the Suit requires power;
- the baseline starting sealed Suit retains a documented finite passive emergency Life-Support Reserve path even at zero Suit Energy.

## 25. EVA Eligibility

Normal EVA requires:

- valid pressure sealing;
- nonzero sufficient Life-Support Reserve for the configured safety threshold;
- compatible locomotion capability, such as powered thrusters or Magnetic Boots for the intended route;
- protection adequate for known environmental hazards;
- Equipment Condition above the relevant hard failure threshold.

## 26. Airlock Preflight Check

Before a normal player-controlled external airlock cycle, the system checks known:

- Suit equipped/compatible;
- Helmet equipped and sealed;
- pressure protection valid;
- Life-Support Reserve above the configured minimum launch threshold;
- Suit/Helmet not in a Condition state that invalidates required protection;
- required EVA locomotion for the intended exit when known.

A failed check blocks the normal cycle. A dangerous override is available only where the airlock/equipment rules permit one and must clearly state known risk.

## 27. Mission Preparation

Mission briefing/loadout shows only known environmental requirements and uncertainties.

Known hazards can create hard preparation requirements when the mission physically cannot be survived/executed without them. Unknown hazards remain unknown and cannot leak through validation.

## 28. No Personal Hunger, Thirst, or Fatigue

There is no global continuous player hunger, thirst, sleep, or fatigue system.

Any long-duration authored provisioning objective is an explicit mission/resource rule rather than hidden metabolism.

## 29. Environmental HUD

When information is legitimately known, the HUD communicates:

- external atmosphere safety;
- Suit/Helmet seal;
- Life-Support Reserve/endurance;
- Suit Energy;
- temperature exposure;
- radiation dose/state;
- contaminants;
- EVA locomotion/support state.

Critical warnings outrank routine loot/tool notifications.

## 30. Safety Overrides

Opening the Helmet, unsealing Equipment, or disabling protection in known dangerous conditions requires explicit confirmation where physical design allows the action.

The confirmation does not grant immunity; environmental state applies immediately after commit.

## 31. Health Boundary

Field Survival owns environmental protection/exposure state.

Player Health owns biological Health bands, Incapacitation, treatment, and defeat/recovery.

GDS-9 owns applicable Damage/Status resolution where a hazard uses Combat damage semantics.

## 32. Persistence

Save state preserves:

- Suit atmosphere mode where consequential;
- Life-Support Reserve;
- Suit Energy through Equipment;
- Radiation Dose and RadiationSickness;
- Suit breach/patch state;
- persistent exposure state needed to resume deterministically.

Save/load never refills, cools, heals, or normalizes these values automatically.

## 33. Edge Cases

- When unsafe external air becomes verified-safe, Auto mode switches to ExternalAir after validation unless the player manually forced SealedLifeSupport.
- When verified-safe air becomes unsafe, Auto mode seals immediately if equipment is capable; if not, exposure starts.
- Helmet seal failure during EVA immediately updates reserve loss/pressure exposure from the actual breach state.
- Suit Energy reaching zero while drifting stops thrust/stabilization but preserves momentum.
- Entering breathable but radioactive atmosphere changes no radiation rule.
- Crossing High/Critical radiation thresholds in the same step as treatment resolves through deterministic transaction ordering; state is derived from final committed Dose.
- True Pause freezes dose accumulation/recovery and all exposure timers.

## 34. Progression

Progression improves field capability through authored Equipment/Research such as:

- greater Life-Support endurance;
- stronger pressure seal/repair resilience;
- Magnetic Boots/Zero-G mobility;
- broader thermal/radiation/chemical protection;
- better environmental sensing;
- more efficient active regulation.

Progression does not remove environmental rules globally.

## 35. Tuneable Parameters

Tuneable values include:

- Life-Support capacity/consumption;
- refill rate;
- seal/leak rates;
- pressure/temperature protection values;
- exposure/Health-damage rates;
- radiation thresholds/accumulation/clearance rates;
- Radiation Sickness rates;
- EVA safety thresholds.

State semantics and protection dependencies are fixed.

## 36. Explicit Non-Goals

The baseline does not include:

- personal hunger/thirst/sleep/fatigue bars;
- infinite EVA oxygen;
- one universal hazard-proof Suit;
- instant radiation reset;
- random unexplained environmental damage;
- instant death at Life-Support zero;
- safety warnings that secretly reveal unknown hazards.

## 37. Dependencies

Depends on Equipment, Player Health, Movement, Tools/Inventory, Station Atmosphere/Thermal, Missions/Hazards, World environment data, Spacecraft life support, Combat Status/Damage, Time/Simulation, and GDS-13 HUD/Alarms/Accessibility.

## 38. Open Questions

None.
