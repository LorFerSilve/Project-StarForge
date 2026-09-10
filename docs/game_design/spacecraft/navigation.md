# Spacecraft Navigation

> **Status:** Design Complete  
> **Authority:** Strategic route discovery, route planning, travel requirements, Navigator contribution, travel states, interruption/failure boundary, and local/strategic transition

## 1. Purpose

Navigation connects bounded gameplay locations into a coherent traversable galaxy without requiring seamless real-time travel across astronomical distances.

## 2. Strategic Map

The galaxy is represented by strategic locations and routes.

Navigation operates on:

- current location;
- known destinations;
- known routes;
- route requirements;
- ship capability;
- fuel/propellant;
- hazards/intelligence.

## 3. Location

A strategic Location can represent:

- home station;
- planet/moon orbit;
- mission staging area;
- asteroid field;
- derelict zone;
- faction station;
- anomaly;
- deep-space point.

World owns exact location content.

## 4. Route

A Route defines:

- origin;
- destination;
- minimum Reach Class;
- travel work/time;
- fuel/propellant cost;
- known hazards;
- navigation difficulty;
- unlock/discovery requirements.

Routes can be directional if world logic requires it.

## 5. Route Discovery

A route can become known through:

- exploration;
- recovered data;
- faction information;
- research;
- narrative;
- sensors.

Unknown routes are not selectable.

## 6. Known vs Safe

Known route does not mean safe.

The route planner can show known:

- threat;
- environmental hazard;
- fuel risk;
- anomaly.

Unknown information remains unknown.

## 7. Navigation Solution

Before travel, the ship computes a valid solution.

Requirements can include:

- operational avionics;
- sufficient sensor/navigation data;
- functioning drive;
- reachable route;
- resource reserve.

## 8. Navigator Crew

A Navigator can improve:

- route calculation;
- hazard interpretation;
- efficiency within authored bounds;
- difficult-route eligibility;
- recovery from navigation faults.

A Navigator does not create unknown routes or bypass propulsion Reach Class.

## 9. Baseline Without Navigator

The player can navigate ordinary known routes using onboard avionics.

Navigator crew is not a hard early-game lock.

Advanced/exotic routes may require higher navigation capability or special knowledge.

## 10. Route Planning

The planner calculates:

- required Reach Class;
- fuel;
- propellant;
- travel duration;
- projected reserve;
- known risk;
- arrival conditions where known.

## 11. Multi-Leg Route

The player can plan multiple connected route legs.

Each leg must be individually valid.

The planner projects cumulative resource cost.

## 12. Reserve Protection

The planner respects propulsion reserve policy.

A route violating reserve shows a warning or is blocked until explicit override.

## 13. Travel States

Strategic travel uses:

- Planning;
- Ready;
- Departure;
- In Transit;
- Arrival;
- Interrupted/Diverted where applicable;
- Completed.

## 14. Departure Commit

At a defined departure commit:

- ship location transitions from local-zone ownership to In Transit;
- travel resources are committed according to Propulsion;
- the previous local flight state is no longer independently active.

This transaction occurs once.

## 15. In Transit

In Transit is active gameplay simulation.

Travel duration advances exclusively with Simulation Time.

The home station continues to simulate.

The player is not required to manually hold forward for interstellar distances.

## 16. Transit Gameplay

Some routes can trigger:

- encounter;
- distress event;
- system fault;
- optional diversion.

The player can remain in ship context during transit.

Dynamic transit-event content is owned by World/Missions and obeys the GDS-12 Dynamic Event and Simulation Time contracts.

## 17. No Real-World Offline Travel

Closing the game stops travel progression.

## 18. Arrival

Arrival places the ship into the destination's valid local-flight or docking/mission entry state.

Arrival cannot create duplicate ship instances.

## 19. Travel Duration

Duration is an in-game pacing parameter influenced by:

- route;
- drive;
- mass;
- navigation modifiers.

Travel should not impose long mandatory real-world waiting.

## 20. Simulation-Rate Boundary

Strategic transit uses the canonical **1.0x Simulation Time** rate.

Player-controlled fast-forward, slow-motion, selective time acceleration, and transit-only time compression are not part of the Design Complete baseline.

Adding a player-controlled simulation-rate feature later requires a formal design change under `../systems/time_and_simulation.md`, because Horizon simulation, travel, hazards, events, recovery, and combat share the same gameplay clock.

## 21. Diversion

A route event can create a diversion only when:

- a valid alternate location exists;
- travel state supports it;
- sufficient capability/resources remain.

The player is informed of consequence.

## 22. Abort

Before Departure Commit, travel can be cancelled without consuming committed travel resources.

After commit, cancellation requires an explicit return/diversion maneuver and cannot magically refund the trip.

## 23. Insufficient Fuel Mid-Transit

Under normal deterministic planning, the ship cannot begin a route with insufficient committed travel resources.

Fuel shortage mid-route can occur only from:

- leak;
- damage;
- explicit event;
- player-authorized resource use.

This becomes a stranded/emergency state rather than a hidden calculation error.

## 24. Stranded State

A stranded ship can require:

- distress call;
- repair;
- refueling;
- tow;
- emergency diversion.

Routine stranding should provide a recovery path.

## 25. Location Entry

World/Mission rules determine whether arrival produces:

- free local flight;
- automatic docking;
- mission deployment;
- controlled landing sequence.

Navigation does not redefine those local rules.

## 26. Navigation UI

The strategic map displays:

- current location;
- known routes;
- Reach requirements;
- cost;
- duration;
- reserve;
- known hazard;
- route blockers.

## 27. Route Comparison

The player can compare alternatives by:

- resource cost;
- duration;
- risk;
- Reach requirement.

A longer route may be safer or cheaper.

## 28. Persistence

Current route, committed resources, transit progress, ship state, and destination persist.

Save/load cannot reset travel cost or duplicate arrival.

## 29. Edge Cases

If the drive becomes invalid before Departure Commit, travel is blocked.

If damage occurs In Transit, travel can continue, divert, or strand according to remaining capability.

If destination becomes unavailable due to world state, the route system selects only an explicitly valid diversion/recovery path rather than deleting the ship.

If a save occurs at transition boundary, one atomic state owns the ship: origin, In Transit, or destination.

## 30. Explicit Non-Goals

Navigation does not provide:

- seamless galaxy flight;
- free cancellation after committed fuel use;
- route selection to undiscovered destinations;
- Navigator magic bypassing propulsion;
- offline real-world transit;
- baseline player-controlled fast-forward, slow-motion, or time acceleration.

## 31. Tuneable Parameters

Tuneable values include duration, route cost, navigation modifiers, hazard weights, reserve thresholds, and route-discovery presentation.

## 32. Dependencies

This specification depends on Propulsion, Ship Systems, GDS-3 Navigator, World/Galaxy, Missions, Dynamic Events, and Persistence.

## 33. Open Questions

None in the navigation-framework baseline.

Exact route graph and exotic-route content are owned by GDS-7.
