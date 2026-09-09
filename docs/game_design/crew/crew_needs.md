# Crew Needs

> **Status:** Draft  
> **Authority:** Persistent crew support requirements, habitability, population sustainability, habitation allocation, and shortage consequences

## 1. Purpose

Crew needs make population growth dependent on real station infrastructure without turning Project StarForge into a character-by-character survival micromanagement game.

## 2. Tracked Needs

The baseline tracks these station-level crew requirements:

- Habitation;
- Breathable Atmosphere;
- Safe Temperature;
- Food;
- Fresh Water;
- Medical Support.

The baseline does **not** track an individual hunger meter, thirst meter, happiness meter, or minute-by-minute sleep meter for every crew member.

## 3. Habitation

Every permanent crew member requires one valid habitation allocation.

A habitation allocation represents:

- sleeping accommodation;
- personal storage abstraction;
- basic sanitation access;
- safe off-duty space.

The game does not require the player to decorate a unique bedroom for each crew member.

## 4. Hard Habitation Capacity

Permanent recruitment requires available habitation capacity.

Temporary Guests use separate emergency guest capacity and cannot permanently substitute for normal habitation.

## 5. Habitable Environment

A habitation area must satisfy station atmosphere and thermal safety requirements.

A bed in a vacuum or lethal-temperature compartment does not count as usable habitation.

## 6. Food Demand

Every active crew member contributes to aggregate food consumption.

Consumption draws from accessible station food inventory.

The system uses station-level accounting rather than requiring manual meals.

## 7. Water Demand

Every active crew member contributes to aggregate fresh-water consumption.

Distribution and reserve policy follow the station water system.

## 8. Atmosphere Demand

Crew affects:

- oxygen demand;
- carbon-dioxide generation;
- life-support processing load.

The atmosphere system owns the physical gas behavior.

## 9. Thermal Requirement

Crew requires safe ambient temperature in occupied station areas.

Environmental safety is evaluated through the thermal system.

## 10. Medical Support

Crew population creates a baseline need for medical capacity.

Medical Support is represented by:

- medbay treatment capacity;
- medical supplies;
- qualified Medic coverage;
- recovery-bed availability.

A small crew can operate with minimal medical support but has slower or less capable recovery.

## 11. No Generic Morale Meter

The baseline has no continuously simulated station-wide morale percentage.

Narrative events, traits, or specific circumstances can create explicit behavior changes, but the player is not required to optimize an abstract happiness bar.

## 12. No Routine Social Needs Simulation

The baseline does not require explicit simulation of:

- entertainment;
- friendships;
- romance;
- social groups;
- personal decoration preferences.

These may appear as narrative flavor without becoming mandatory systemic needs.

## 13. Population Sustainability

The station computes projected sustainability from:

- accessible food;
- accessible fresh water;
- life-support capacity;
- habitation capacity;
- medical capacity.

This is advisory and operational, not one opaque "population cap."

## 14. Sustainability Forecast

The crew/station UI provides projections such as:

- food endurance;
- water endurance;
- life-support margin;
- free habitation;
- medical treatment capacity.

The player should understand why population expansion is safe or unsafe.

## 15. Food Shortage States

Food support progresses through:

- Secure;
- Low Reserve;
- Shortage;
- Critical Shortage.

Exact thresholds and elapsed times are tuneable.

## 16. Water Shortage States

Water support uses equivalent escalating shortage states.

The water system owns actual allocation.

## 17. Shortage Consequences

Shortage consequences are progressive rather than instant.

Possible consequences include:

- reduced work capacity;
- increased medical demand;
- forced nonessential work suspension;
- incapacitation under prolonged critical conditions.

Routine shortages do not permanently kill recruited crew.

## 18. Atmosphere and Temperature Emergency

Unsafe pressure, oxygen, contamination, or temperature causes immediate safety behavior according to severity.

Crew attempts to:

- evacuate;
- use available protective equipment where assigned/available;
- reach safe compartments;
- follow emergency response.

Environmental danger is not converted into a generic needs penalty.

## 19. Habitation Loss

If occupied habitation is destroyed:

1. affected crew loses permanent habitation allocation;
2. emergency guest/temporary bunk capacity is used if available;
3. otherwise the station enters an overcrowding condition;
4. the player is warned and must restore accommodation.

Crew does not vanish because their room was destroyed.

## 20. Overcrowding

Overcrowding is permitted as a temporary recovery state, not a normal progression strategy.

It can reduce:

- recovery quality;
- available emergency capacity;
- work efficiency after a configurable tolerance period.

It does not create hidden random death.

## 21. Medical Capacity

Medical treatment is constrained by:

- treatment beds;
- available supplies;
- Medic capability;
- power;
- safe atmosphere;
- functioning equipment.

When demand exceeds capacity, triage priority determines treatment order.

## 22. Default Medical Priority

Default triage priority is:

1. critical stabilization;
2. serious incapacitation;
3. moderate injury;
4. minor injury;
5. elective/nonurgent care.

The player can influence policy only within safe authored bounds.

## 23. Crew Duty and Needs

The game does not require scheduled meal and sleep simulation.

Crew AI may visually take breaks and return to quarters, but resource accounting uses aggregate time-based demand.

## 24. Off-Screen Simulation

Crew needs continue during active gameplay while the player is away from the station.

They stop progressing when the application is closed, consistent with global rules.

## 25. Emergency Reserves

The player can protect:

- food reserve;
- water reserve;
- medical supply reserve.

Automation cannot consume protected reserve for lower-priority purposes without permission.

## 26. Recruitment Warning

Recruitment UI shows the projected effect of adding the new crew member to:

- habitation;
- food;
- water;
- life support;
- medical capacity.

The player can recruit into a projected food/water shortage if physical habitation exists and immediate survival is possible, but receives an explicit warning.

## 27. Crew Needs UI

The station population view shows:

- crew count;
- habitation used/free;
- food stock and endurance;
- water stock and endurance;
- life-support margin;
- unsafe compartments;
- medical capacity;
- current shortages.

## 28. Persistence

Habitation allocation, population demand state, medical occupancy, shortage progression, and emergency accommodation state persist.

## 29. Edge Cases

If a station branch containing food becomes disconnected, that food stops counting as accessible unless the crew can legitimately access that branch.

If food exists but logistics cannot deliver it to a supported consumption point, the station reports a logistics-caused shortage.

If a crew member is incapacitated, their basic food/water accounting may change according to medical treatment, but they still occupy population support.

If a crew member is temporarily off-station in an authored narrative state, their station demand is removed only when the narrative state explicitly provides external support.

## 30. Progression

Early game:

- tight habitation;
- small reserves;
- low medical capacity;
- frequent manual awareness.

Mid game:

- dedicated quarters;
- stable farming/recycling;
- Medic and medbay;
- emergency reserves.

Late game:

- redundant life support;
- large reserves;
- distributed habitation;
- advanced medical recovery;
- high population sustainability.

## 31. Explicit Non-Goals

Crew needs do not include:

- individual meal clicking;
- personal thirst bars;
- mandatory happiness simulation;
- romance needs;
- minute-level sleep scheduling;
- arbitrary population number detached from infrastructure.

## 32. Tuneable Parameters

Tuneable values include food/water demand, shortage thresholds, tolerance durations, habitation capacity, medical capacity, and overcrowding penalties.

## 33. Dependencies

This specification depends on station atmosphere, thermal, water, farming/food, logistics, medical systems, recruitment, injuries, and time/simulation.

## 34. Open Questions

None in the current baseline.

The document remains Draft until medical, time, and resource systems are cross-validated.
