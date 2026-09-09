# Status Effects

> **Status:** Draft  
> **Authority:** Combat status-effect identity, application, stacking, duration, resistance, periodic processing, cleanse/removal, persistence, and baseline status catalog

## 1. Purpose

Status Effects represent temporary/persistent combat consequences that cannot be expressed cleanly as immediate damage alone.

## 2. Status Instance

A Status Instance contains:

- Status ID;
- Source ID;
- Target ID;
- intensity/stack state;
- remaining duration or persistent condition;
- tick state where periodic;
- tags;
- removal conditions.

## 3. Application Rule

A status is applied only when its trigger condition succeeds.

Examples:

- damaging component reaches target;
- EMP contacts shield/electronics;
- corrosive material reaches unprotected armor/body;
- explosion exceeds stagger threshold.

## 4. No Random Status Chance Baseline

The baseline does not require random percentage proc chance.

Application depends on deterministic:
- hit;
- intensity;
- target resistance;
- threshold.

A weapon can still define a guaranteed status payload above a threshold.

## 5. Status Resistance

Targets can have explicit Resistance/Immunity to status classes.

Resistance can:
- raise application threshold;
- reduce duration;
- reduce intensity.

## 6. Duration

Status duration advances only in active simulation time.

True Pause stops it.

Closing game does not advance it.

## 7. Stacking Models

Each status declares one model:

- No Stack / Refresh Duration;
- Intensity Stack;
- Independent Stack;
- Accumulation Threshold.

No status uses undefined stacking behavior.

## 8. Baseline Combat Statuses

Canonical baseline:

1. Burning;
2. Disrupted / EMP;
3. Corroded;
4. Staggered;
5. Suppressed-System / Jammed Electronics for machine systems where applicable;
6. Bleeding only if later biological content explicitly uses it.

Bleeding is not mandatory baseline player micromanagement.

## 9. Burning

Source:
- high thermal/incendiary attack.

Effects:
- periodic Thermal/Biological or component damage;
- possible environmental fire interaction;
- visible active state.

Removal:
- duration;
- extinguishing method;
- environment incompatible with continued burning where authored.

## 10. Burning in Vacuum

Ordinary combustion cannot remain unchanged in vacuum.

A self-oxidizing/incendiary payload can continue only if its definition explicitly supports it.

## 11. Disrupted / EMP

Targets:
- shields;
- electronic equipment;
- machines;
- ship/station electronic modules.

Possible effects:
- shield recharge blocked;
- sensor degradation;
- powered ability disabled;
- control latency/temporary shutdown.

EMP does not automatically damage biological Health.

## 12. Corroded

Targets:
- armor;
- exposed machinery;
- biological protection.

Effects can:
- temporarily reduce relevant Armor/Protection rating;
- cause periodic Corrosive damage;
- degrade exposed equipment.

Exact target behavior is authored.

## 13. Staggered

Short-duration action interruption from sufficient impulse/damage.

Effects:
- cancels selected attack/action;
- temporarily limits immediate action.

Stagger is not a long stun-lock by default.

## 14. Stagger Threshold

A target defines resistance to stagger.

Attack impulse/stagger value must exceed threshold after relevant modifiers.

Repeated tiny hits do not automatically chain-stun unless accumulation is explicitly defined.

## 15. Electronic Jam

For machines/ships, an authored electronic-jam status can degrade:
- targeting;
- sensors;
- communications;
- guidance.

This is distinct from physical destruction.

## 16. Slow / Movement Impairment

No universal magical Slow status exists.

Movement impairment must come from:
- physical injury;
- environmental condition;
- specific adhesive/cryo/field technology with explicit mechanic.

## 17. Poison/Toxin

Toxin status belongs primarily to environmental/biological content.

Combat can apply it only through explicit attack technology.

## 18. Status and Shields

A status payload defines whether it targets:
- Shield;
- Armor;
- Body;
- Electronics;
- Structure.

A fully shield-absorbed body-targeting Corrosion/Burning payload does not pass through by default.

## 19. Status and Armor

Armor can prevent/reduce status application if its protection rating/tag supports it.

## 20. Periodic Damage

Periodic damage creates deterministic Damage Packets at fixed simulation intervals.

Total damage is framerate-independent.

## 21. Reapplication

Reapplication follows the status's declared Stacking Model.

It does not silently choose a behavior.

## 22. Cleansing / Removal

Status removal can require:
- time;
- medical item;
- repair;
- coolant/extinguishing;
- reboot;
- safe environment;
- station/ship service.

## 23. Status Persistence

Statuses persist across valid saves.

Mission extraction does not automatically clear combat statuses unless recovery/treatment does so.

## 24. Player Incapacitation

If a periodic status reduces Health to zero:
- GDS-5 Incapacitation applies;
- GDS-8 mission failure/recovery can follow.

## 25. Machine Disablement

EMP/Jam can temporarily disable a machine without destroying it.

Objective "Disable" can count such a state only if the objective definition permits temporary disablement.

## 26. Boss/Elite Immunity

No blanket boss immunity exists.

Specific actors can resist selected statuses for mechanical reasons.

Immunity must be visible/learnable.

## 27. Friendly Fire

Statuses follow the same hit/faction neutrality as damage.

## 28. Status UI

Presentation should communicate:
- status type;
- severity where relevant;
- duration/condition;
- removal path where known.

## 29. Explicit Non-Goals

Status Effects do not use:

- random proc chance baseline;
- universal magical slow;
- undefined stacking;
- automatic boss immunity;
- vacuum-ignoring ordinary fire;
- extraction-as-free-cleanse.

## 30. Tuneable Parameters

Thresholds, durations, tick intervals, intensities, resistance, and removal times are tuneable.

## 31. Dependencies

This specification depends on Damage Model, Armor/Shields, Player Health/Equipment, Ship/Station Systems, future Robots, Missions, and Presentation.

## 32. Open Questions

None in the baseline status framework.
