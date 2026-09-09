# Armor and Shields

> **Status:** Draft  
> **Authority:** Personal/actor armor coverage, protection ratings, penetration interaction, shield capacity/regeneration, shield power/heat behavior, shield disablement, and repair/condition boundary

## 1. Purpose

Armor and shields provide distinct defensive engineering choices.

- Shields are rechargeable powered interception capacity.
- Armor is passive physical protection with coverage and material-specific ratings.

## 2. Personal Defensive Order

For a protected humanoid:

1. Active Personal Shield;
2. Armor/Protective Equipment;
3. Biological Health.

This matches GDS-5.

## 3. Armor Coverage

Armor protection is attached to Hit Locations.

A suit/helmet/plate declares which zones it protects.

Examples:

- Helmet → Head;
- Suit torso plate → Torso;
- arm sections → Arms;
- leg sections → Legs.

## 4. Armor Ratings

Armor can define:

- Kinetic Armor;
- Thermal Protection;
- Blast Protection;
- Electronic Hardening;
- Corrosion Protection.

Not every item has every rating.

## 5. Armor Penetration

Kinetic/other attacks can carry Penetration.

Damage Model subtracts penetration from relevant protection before mitigation.

## 6. Armor Is Not Generic HP

Armor does not absorb a fixed pool of damage before Health by default.

It reduces incoming damage according to ratings.

## 7. Armor Condition

Equipment Condition can reduce documented protection.

Condition states remain GDS-5:

- Operational/Pristine;
- Worn;
- Damaged;
- Critical;
- Disabled.

Exact protection degradation curves are item data.

## 8. Armor Repair

Armor repair requires physical materials/components and valid repair capability.

Combat does not repair armor automatically after encounter.

## 9. Personal Shield

A Personal Shield defines:

- Maximum Capacity;
- Current Capacity;
- Recharge Delay;
- Recharge Rate;
- Suit Energy draw;
- damage-channel Cost Multipliers;
- EMP/disruption behavior;
- Condition.

## 10. Personal Shield Coverage

Baseline personal shields provide omnidirectional protection to the player body.

They do not cover:

- unrelated carried mission objects;
- nearby allies;
- deployed equipment

unless a different shield device explicitly does so.

## 11. Shield Recharge Delay

After shield damage, regeneration does not begin until Recharge Delay has elapsed without further qualifying shield damage.

Delay uses active game time.

## 12. Shield Recharge

Recharge converts Suit Energy into Shield Capacity.

The shield cannot regenerate if:

- shield disabled;
- insufficient Suit Energy;
- EMP/disruption blocks recharge;
- equipment condition prevents operation.

## 13. No Free Shield Energy

Shield capacity is not created independently of the suit/ship energy model.

Exact energy-per-capacity ratio is shield model data.

## 14. Shield Overflow

If a hit exceeds current shield absorption capability, remaining damage passes into Armor according to Damage Model.

## 15. Shield Collapse

At Current Capacity = 0:

- shield is Depleted;
- remaining same-hit damage can overflow;
- recharge waits for Recharge Delay and valid power.

## 16. EMP Against Shields

Electrical/EMP attacks use increased Shield Cost by default and can apply Disrupted/EMP status.

A depleted shield does not automatically mean the entire suit loses electrical power.

## 17. Shield Recharge Under Fire

Any qualifying shield damage resets Recharge Delay.

A weapon/status can explicitly prevent regeneration for longer.

## 18. Shield Overcharge

No universal manual shield-overcharge mechanic is baseline.

Specific advanced shield models can add one if it has explicit power/thermal cost.

## 19. Deployable Shields

Gadgets/robots/station systems can project shields.

They reuse shield Damage Model principles but have their own:

- capacity;
- coverage geometry;
- power owner;
- state.

## 20. Ship Shields

Ship shields follow the same absorption/overflow logic unless a ship shield model declares directional segments.

Baseline ship shield configuration can be one hull-wide envelope for implementation simplicity.

## 21. Directional Ship Shields

Directional shield facings are not required for baseline.

A future ship module can introduce them through explicit design change/content definition.

## 22. Station Shields

Station shield systems can protect explicit geometry/coverage zones.

Power and system state remain GDS-2 authority.

## 23. Shield Weakness

An attack does not bypass shields merely because it is a headshot/weak-point hit unless that shield/device has an explicit coverage gap/bypass property.

## 24. Environmental Protection vs Armor

Pressure, radiation, temperature, contamination protection is distinct from Combat Armor Rating even if one suit provides both.

A high Kinetic Armor value does not imply vacuum safety.

## 25. Immunity

Armor/shields can provide explicit channel immunity only through authored capability.

Examples could include:
- EMP-hardened passive mechanical target;
- sealed nonbiological target immune to toxin status.

Immunity is not achieved through hidden huge numbers.

## 26. UI Requirements

Combat feedback must eventually communicate:

- shield current/max;
- recharge state;
- shield collapse;
- armor effectiveness/weakness where learnable;
- major protection damage.

GDS-13 owns presentation.

## 27. Persistence

Shield Current Capacity, equipment Condition, power state, and relevant disruption states persist through valid saves.

## 28. Edge Cases

If Suit Energy reaches zero with shield partially charged, remaining shield Capacity stays until consumed unless the shield model explicitly requires continuous holding power.

Recharge stops.

If helmet armor is Disabled but suit torso armor remains, Head and Torso use their respective actual protection.

If one hit depletes shield and passes damage onward, armor applies only to the overflow portion.

## 29. Explicit Non-Goals

Armor/Shields do not provide:

- armor as universal extra HP;
- free shield recharge;
- headshot shield bypass;
- one armor rating that also means vacuum/radiation protection;
- mandatory directional ship shields.

## 30. Tuneable Parameters

Armor ratings, shield capacity, recharge delay/rate, energy ratio, Cost Multipliers, coverage, and condition penalties are tuneable.

## 31. Dependencies

This specification depends on Damage Model, GDS-5 Equipment/Energy/Health, GDS-6 Ship Systems, Station Power/Defenses, Resources, and Status Effects.

## 32. Open Questions

None in the armor/shield baseline.
