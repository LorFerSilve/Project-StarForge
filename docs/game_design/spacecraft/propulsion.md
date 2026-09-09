# Spacecraft Propulsion

> **Status:** Draft  
> **Authority:** Main-drive families, propellant/fuel consumption, strategic range/reach, local thrust capability, drive requirements, and propulsion progression

## 1. Purpose

Propulsion is a major progression gate.

It determines both:

- how a ship accelerates in local flight;
- which strategic destinations it can reach.

## 2. Propulsion Layers

The baseline distinguishes:

- Main Drive;
- Maneuvering Thrusters;
- Strategic Drive Capability.

A single installed propulsion architecture can provide several of these functions.

## 3. Propulsion Resources

Propulsion can use:

- Reaction Propellant;
- Fusion Fuel Pellet;
- electrical energy;
- advanced exotic resource where future late-game drives require it.

The specific drive definition declares consumption.

## 4. Reaction Propellant

Reaction Propellant is the standard consumable reaction mass for early/conventional spacecraft propulsion.

It is a physical GDS-4 resource stored in ship tanks.

## 5. Fusion Fuel

Fusion propulsion uses Fusion Fuel Pellets as reactor/drive energy input where applicable.

A fusion ship can still require Reaction Propellant for momentum exchange unless its drive definition explicitly uses a different mechanism.

## 6. Propellant Tanks

Ship tanks have finite capacity.

Stored propellant:

- contributes mass;
- can be damaged;
- can leak where damage rules permit;
- can be refilled from valid station/field sources.

## 7. Fuel vs Propellant

Fuel and propellant are distinct concepts.

- Fuel supplies energy.
- Propellant supplies reaction mass.

Some drive technologies consume both.

The UI must not combine them into one ambiguous "fuel" bar when the ship requires both.

## 8. Canonical Drive Families

Baseline families:

### Chemical / Electrothermal Drive

Early technology.

Uses:
- Reaction Propellant;
- electrical/chemical energy.

Characteristics:
- low strategic reach;
- moderate local thrust;
- cheap and robust.

### Ion/Plasma Drive

Early-mid exploration technology.

Uses:
- Reaction Propellant;
- substantial electrical power.

Characteristics:
- efficient strategic travel;
- lower local high-thrust performance;
- strong Scout compatibility.

### Fusion Torch Drive

Mid-late technology.

Uses:
- Fusion Fuel Pellets;
- Reaction Propellant;
- high power/thermal infrastructure.

Characteristics:
- high thrust;
- high strategic range;
- supports heavier ships.

### Exotic/Quantum Drive

Late-game experimental family.

Uses:
- Fusion Fuel Pellets;
- Reaction Propellant;
- very high electrical power;
- Quantum Processor-based control hardware;
- Quantum Substrate-derived drive components.

Technology:
- Quantum Materials;
- Quantum Computing;
- advanced propulsion research derived from Anomaly Physics.

Characteristics:
- Reach IV capability;
- access to otherwise unreachable anomalous/high-energy strategic routes;
- very high power and thermal demand;
- expensive, fragile advanced hardware.

The exotic field system changes route accessibility and drive efficiency; it does not create reaction mass or free energy.

It does not provide unrestricted teleportation.

## 9. Strategic Drive Rating

Each drive has a **Strategic Reach Class**.

Baseline:

- Reach I — local system/nearby routes;
- Reach II — extended regional routes;
- Reach III — distant/high-energy routes;
- Reach IV — exotic/anomalous routes.

Route definitions declare the minimum Reach Class.

## 10. Reach Is Not Distance Alone

A route requirement can depend on:

- strategic distance;
- gravity/energy difficulty abstraction;
- hazard;
- anomaly;
- route technology.

Therefore a nearby anomaly route can require a higher Reach Class than a longer ordinary route.

## 11. Strategic Travel Cost

A route defines:

- minimum Reach Class;
- propellant cost;
- fuel/energy cost;
- travel duration;
- hazard modifiers;
- navigation requirement.

Exact route content belongs to World.

## 12. Mass Effect on Strategic Travel

Actual ship mass can increase propellant cost.

Heavier cargo therefore reduces range/endurance.

The exact function is tuneable.

## 13. Reserve Rule

Navigation must preserve a player-configurable minimum propulsion reserve unless explicitly overridden.

Default route planning avoids consuming the final emergency reserve.

## 14. Stranding Prevention

The route planner warns before a one-way trip that leaves insufficient known return capability.

The player may accept the risk through explicit confirmation.

The game does not secretly grant return fuel.

## 15. Local Main Thrust

A drive defines:

- maximum local thrust;
- power demand;
- heat;
- propellant flow;
- boost behavior;
- condition.

Flight converts thrust and mass into acceleration.

## 16. Maneuvering Thrusters

Maneuvering thrusters use electrical power and/or Reaction Propellant according to design.

They support:

- translation;
- rotation;
- braking;
- docking.

## 17. Propulsion Heat

High-output propulsion generates heat.

Thermal limitation can reduce:

- sustained thrust;
- boost duration;
- strategic-drive readiness.

## 18. Propulsion Power

Electrical propulsion/fusion systems require power.

Insufficient power reduces available thrust or prevents strategic travel.

## 19. Drive Warm-Up

Strategic/high-output drives can require warm-up.

Warm-up requires:

- functioning drive;
- power;
- thermal margin;
- valid navigation solution.

It can be interrupted by faults/combat.

## 20. Strategic Travel Initiation

Strategic travel begins only when:

1. route selected;
2. destination known/reachable;
3. minimum Reach Class satisfied;
4. navigation solution valid;
5. required fuel/propellant reserve available;
6. drive operational;
7. no location/mission rule blocks departure.

## 21. Consumption Commit

Fuel/propellant is consumed according to travel transaction rules at a defined commit point.

The future Navigation/World transition must prevent save/load duplication or free cancellation after consumption.

## 22. Drive Damage

Damage can:

- reduce thrust;
- reduce efficiency;
- increase heat;
- leak propellant;
- disable strategic travel;
- disable boost.

## 23. Emergency Limp Capability

A partially damaged ship may retain limited local maneuvering even when strategic drive is disabled.

This enables rescue/recovery gameplay.

## 24. Refueling

Refueling requires:

- compatible fuel/propellant source;
- docking/service connection or approved field transfer;
- storage capacity.

Transfer is physical.

## 25. Field Refueling

Field refueling is possible only when an explicit mission/resource setup provides:

- compatible propellant/fuel;
- transfer equipment;
- safe connection.

The ship does not automatically refine raw Water Ice directly inside its tank unless an installed module explicitly supports that processing.

## 26. Propulsion Research Progression

Typical progression:

- Chemical/Electrothermal;
- Ion/Plasma;
- Fusion Torch;
- Exotic/Quantum.

Unlocks require Research + Blueprints + compatible hull/module.

## 27. No Universal Best Drive

Drive families have tradeoffs in:

- thrust;
- efficiency;
- heat;
- power;
- mass;
- slot size;
- strategic reach.

A high-reach drive can be inefficient for a small combat interceptor.

## 28. UI

Propulsion UI shows:

- drive family;
- Reach Class;
- thrust;
- propellant;
- fuel;
- projected range/routes;
- mass effect;
- heat;
- power;
- reserve;
- faults.

## 29. Persistence

Fuel, propellant, drive condition, warm-up state where meaningful, and selected reserve policy persist.

## 30. Edge Cases

If cargo is loaded after route planning, route cost recalculates before departure.

If fuel is transferred away before departure, route becomes invalid.

If drive damage occurs during local flight, available thrust updates immediately.

If strategic travel is committed and transition fails technically, persistence must resolve to one deterministic pre/post-travel state rather than refunding and moving simultaneously.

## 31. Explicit Non-Goals

Propulsion does not provide:

- infinite fuel;
- one ambiguous fuel resource for every drive;
- galaxy-wide teleportation;
- travel ignoring ship mass;
- progression that only increases top speed.

## 32. Tuneable Parameters

Tuneable values include thrust, propellant flow, fuel use, mass-cost scaling, reserve fraction, warm-up time, Reach thresholds, and boost behavior.

## 33. Dependencies

This specification depends on GDS-4 resources/research, Ship Systems, Flight, Navigation, ship configuration, World routes, Missions, and Damage.

## 34. Open Questions

None in the propulsion-framework baseline.

Future World/Progression content defines which Reach IV routes exist and when their blueprints become obtainable, but the drive's physical operating inputs and rules are fixed.
