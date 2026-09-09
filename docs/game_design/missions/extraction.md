# Mission Extraction

> **Status:** Draft  
> **Authority:** Extraction availability, extraction points, withdrawal modes, secure-loot transitions, passenger/objective extraction, extraction commit, interruption, and post-extraction ownership

## 1. Purpose

Extraction is the explicit transition from field risk back to secured persistent ownership and mission resolution.

Picking up loot is not the same as extracting it.

## 2. Extraction Requirement

A mission explicitly declares one of:

- Extraction Required;
- Extraction Optional;
- In-Place Resolution.

Most external expeditions use Extraction Required.

## 3. Extraction Point

An Extraction Point is an authorized mission exit.

It defines:

- Extraction Point ID;
- mode;
- physical location;
- availability conditions;
- ship/transport compatibility;
- required entities/cargo;
- interruption/blocking conditions;
- destination/transition.

## 4. Extraction Modes

Baseline modes:

1. Player Ship Departure;
2. Shuttle / Mission Transport Pickup;
3. Dock / Airlock Withdrawal;
4. Local-Space Navigation Departure;
5. Authored Emergency Recovery.

## 5. Player Ship Departure

Used when the player's persistent spacecraft is present.

Typical sequence:

- reach ship;
- board/enter valid departure state;
- secure required passengers/objectives;
- launch/undock if required;
- cross mission departure boundary or commit navigation departure;
- extraction commits.

## 6. Shuttle / Mission Transport Pickup

Used when another authorized vehicle extracts the player.

The transport must physically arrive/be available according to mission rules.

Boarding/commit does not teleport distant protected entities into the shuttle.

## 7. Dock / Airlock Withdrawal

Used for missions where the player returns through a controlled connector into a safe owning location.

Example:
- return from hostile derelict to player's docked ship.

## 8. Local-Space Navigation Departure

For spacecraft missions, extraction can commit when the player:

- reaches valid departure condition;
- has operational navigation/propulsion;
- leaves the local mission zone through an authorized route.

## 9. Emergency Recovery

Emergency Recovery is not a universal free escape button.

It exists only where an authored mission/recovery system supports it.

It can carry:

- mission failure;
- resource loss;
- ship damage;
- recovery cost.

## 10. Extraction Point States

Canonical states:

- Unknown;
- Known;
- Unavailable;
- Available;
- Contested;
- Ready;
- Committing;
- Committed;
- Closed.

## 11. Available

The extraction method can be used if other requirements are met.

## 12. Contested

The extraction remains physically present but current conditions make commit unsafe or blocked.

Examples:

- docking clamps under hostile control;
- transport cannot land due to active anti-air;
- extraction vehicle disabled.

Combat presence alone does not automatically mean Contested unless the extraction definition says so.

## 13. Ready

All mandatory extraction requirements are satisfied.

## 14. Committing

The authoritative extraction transition is in progress.

The commit can require:

- boarding;
- launch;
- docking seal;
- navigation transition;
- transport departure.

## 15. Committed

Extraction ownership/state transition has occurred exactly once.

The active mission cannot then revert to pre-extraction through ordinary gameplay.

## 16. No Universal Kill-All Requirement

Extraction does not require every hostile entity to be dead unless the mission explicitly defines:

- area clear;
- airspace clear;
- threat suppressed.

The player can sometimes extract under fire.

## 17. Required Player State

For standard personal extraction, the player must be:

- not in terminal mission-failure resolution;
- physically within/on the authorized extraction owner/zone;
- in a state compatible with transport.

An Incapacitated player can only extract through a rescue/recovery mode explicitly capable of carrying them.

## 18. Backpack Security at Successful Extraction

Mission-acquired items still in the player's backpack remain player-owned.

At a successful extraction commit in which the player physically departs aboard/through an authorized secure extraction method:

- eligible Field-Unsecured backpack contents become Vehicle/Extraction-Secured;
- they are not automatically transferred into ship cargo;
- their physical owner remains Player Inventory.

This prevents tedious manual unloading of every small item into ship cargo while preserving ownership.

## 19. Pre-Secured Ship Cargo

Mission-acquired items already transferred into valid ship cargo can become Vehicle/Extraction-Secured before player extraction according to GDS-6 Cargo.

They remain secured if ordinary player defeat occurs later unless an explicit mission cargo-loss rule says otherwise.

## 20. Station-Secured Boundary

Successful mission extraction does **not** automatically make cargo Station-Secured.

Station-Secured requires:

- arrival at an authorized persistent station owner;
- actual transfer to station inventory where applicable.

Player-owned extracted backpack items remain player-owned until transferred.

## 21. Required Objective Cargo

If mission success requires specific physical cargo, the mission definition states whether it must be:

- carried by player;
- Vehicle/Extraction-Secured;
- aboard specific cargo hold;
- delivered to a destination.

Extraction checks the authoritative owner/state.

## 22. Survivors / Passengers

A survivor extraction objective can require:

- survivor located;
- stabilized if needed;
- physically escorted/transported;
- boarded as Temporary Passenger;
- valid life-support/passenger capacity.

The survivor cannot be secured by proximity to the ship alone.

## 23. Multiple Required Survivors

If a mission requires multiple protected passengers, all mandatory survivors must satisfy the required secured state unless the objective supports partial success.

## 24. Extraction Capacity

Extraction cannot commit required cargo/passengers beyond actual transport capacity.

Optional loot can be left behind.

## 25. Partial Cargo Extraction

The player may extract with less optional loot than is available.

Mission success depends only on explicit mandatory objectives.

## 26. Optional Objectives Before Extraction

Unresolved Optional Objectives can remain available until extraction commits unless their own rules close earlier.

Once extraction commits, they resolve according to their current states.

## 27. Extraction Interruption

Before commit, extraction can be interrupted by:

- player leaving required area;
- transport becoming unavailable;
- required passenger leaving/lost;
- ship becoming unable to depart;
- docking failure;
- mission-specific hostile control;
- player incapacitation.

Already secured cargo ownership does not roll back merely because extraction is interrupted.

## 28. Extraction Timer

There is no universal extraction countdown.

A mission may define one when justified.

Any countdown advances only in active game time and is clearly visible.

## 29. Abandonment and Extraction

Abandoning an active mission:

- marks mission unsuccessful;
- activates/retains valid withdrawal methods;
- does not automatically extract the player.

The player must physically withdraw or suffer ordinary failure/recovery.

## 30. Extraction Without Mission Success

A player can withdraw from some missions before completing mandatory objectives.

The result is:

- mission Failed or Abandoned;
- extracted eligible loot follows security rules;
- no success-only resolution reward.

## 31. Mission Success Commit

For Extraction Required missions, success normally commits at the same atomic transition that confirms:

- mandatory objectives complete;
- required entities/cargo secured;
- extraction committed.

## 32. Post-Extraction Instance

After extraction:

- active mission zone closes or transitions to persistent world state;
- unresolved Field-Unsecured world loot is not magically recovered;
- secured owners retain their contents;
- result/reward state resolves once.

## 33. Player Ship Damage

A damaged persistent ship can extract only if it still satisfies the extraction mode's departure requirements.

If it cannot, the mission becomes a repair/rescue/alternate-extraction problem rather than silently teleporting the ship home.

## 34. Save/Load Atomicity

At extraction boundary the save must own one deterministic state:

- pre-commit Active;
- or post-commit Extracted/Resolved.

It cannot simultaneously preserve field loot and also duplicate the extracted copy.

## 35. Edge Cases

If the player crosses the extraction commit boundary while an objective update happens in the same simulation step, mission transaction ordering evaluates mandatory objective state before final success commit.

If an optional resource stack is partly in ship cargo and partly in backpack, each physical owner keeps its own authoritative quantity.

If a survivor is inside the ship but life-support capacity becomes invalid before departure, extraction readiness updates and can block passenger-required success.

If the extraction vehicle is destroyed after cargo was secured but before player extraction, secured cargo remains with the actual surviving owner only; destruction consequences follow spacecraft/mission rules.

## 36. Explicit Non-Goals

Extraction does not provide:

- secure loot from mere pickup;
- station-secured teleportation;
- proximity-based survivor rescue;
- universal kill-all requirements;
- free emergency teleport;
- automatic success from reaching an exit before objectives are satisfied.

## 37. Tuneable Parameters

Tuneable values include extraction interaction time, boundary distance, shuttle arrival time, contested-clearance rules, and safety warnings.

## 38. Dependencies

This specification depends on Mission System, Objectives, GDS-5 Inventory/Health, GDS-6 Cargo/Flight/Docking, Crew Survivors, Failure, Rewards, Combat, and Persistence.

## 39. Open Questions

None in the extraction-transaction baseline.
