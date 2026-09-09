# Spacecraft Cargo and Utility

> **Status:** Draft  
> **Authority:** Ship cargo ownership, mass/volume capacity, secure extraction storage, hazardous cargo, passenger/robot transport, utility-bay behavior, and cargo transfer

## 1. Purpose

Cargo turns spacecraft into the bridge between field acquisition and persistent station economy.

## 2. Cargo Ownership

Each cargo hold is a physical inventory owner under GDS-4.

Cargo does not simultaneously exist in:

- player inventory;
- station storage;
- another ship;
- mission world.

## 3. Capacity

Ship cargo uses:

- Mass Capacity;
- Volume Capacity;
- category compatibility.

Cargo also contributes to total ship mass.

## 4. Cargo Holds

A hull can contain one or more cargo holds.

A hold defines:

- mass limit;
- volume limit;
- allowed categories;
- hazard rating;
- security rating;
- refrigeration where applicable.

## 5. General Cargo

General holds accept compatible:

- solid bulk resources;
- packaged components;
- finished items;
- mission cargo.

## 6. Fluid/Gas Cargo

Large quantities of fluids/gases require compatible tanks.

Examples:

- Reaction Propellant;
- Helium-3;
- Deuterium;
- Industrial Coolant;
- Fresh Water.

A general crate does not hold arbitrary cryogenic gas.

## 7. Hazardous Cargo

Hazardous materials can require:

- radioactive containment;
- biohazard containment;
- high-energy secure storage;
- pressure/cryogenic tank.

Invalid cargo transfer is blocked or produces an explicit hazard only when emergency/manual override rules allow unsafe loading.

## 8. Refrigerated Cargo

Fresh Produce benefits from refrigerated cargo where long transit would otherwise allow spoilage.

Refrigeration consumes power.

## 9. Secure Cargo

Strategic items can be stored in secure holds.

Security affects:

- raid/boarding access;
- authorization;
- containment.

It does not make cargo indestructible.

## 10. Vehicle/Extraction-Secured

During an external mission, mission-acquired items become **Vehicle/Extraction-Secured** when the mission rules commit them to valid ship cargo or another authorized secure extraction owner.

Placing an item near the ship is insufficient.

## 11. Secure Commit Conditions

A normal ship-cargo secure commit requires:

- valid cargo interface or authorized transfer action;
- sufficient capacity;
- compatible cargo category;
- successful ownership transfer.

The mission system later defines whether the ship must still successfully leave/extract before final Station-Secured conversion.

## 12. Player-to-Ship Transfer

The player can move portable inventory into cargo at a valid accessible cargo interface.

Transfer conserves quantity.

## 13. Station-to-Ship Transfer

A docked ship can request resources from station logistics.

Availability depends on:

- source inventory;
- reservation;
- route;
- dock throughput;
- cargo capacity.

## 14. Ship-to-Station Transfer

Cargo becomes Station-Secured only after actual transfer to station ownership or another explicitly defined secured station intake.

Docking alone does not automatically unload everything.

## 15. Cargo Mass and Flight

Cargo changes actual ship mass and therefore:

- acceleration;
- braking;
- maneuvering;
- strategic travel cost;
- landing margin.

## 16. Overload Prevention

Normal transfer cannot push ship above:

- cargo hold capacity;
- hull Maximum Safe Operating Mass.

Partial transfer can occur for stackable resources.

## 17. Cargo Jettison

The player can jettison eligible cargo in local flight where mission/world rules allow.

Jettison transfers ownership to a world cargo object.

High-value/mission cargo requires confirmation.

## 18. Cargo Damage

Cargo can be lost/damaged if:

- cargo hold is destroyed;
- containment fails;
- the ship is destroyed in an explicitly high-stakes state;
- jettisoned cargo is unrecovered.

Damage rules determine exact loss.

## 19. Cargo During Routine Player Defeat

If the player's persistent ship survives/recovery logic retains it, cargo already Vehicle/Extraction-Secured remains owned by that ship unless the mission explicitly defines interception/cargo-loss consequences.

Field-Unsecured backpack loot remains governed by Player Inventory/Mission rules.

## 20. Passenger Transport

Ships can transport:

- rescued survivors;
- crew;
- narrative passengers.

Passenger capacity requires:

- seat/berth;
- life-support capacity;
- safe environment.

## 21. Temporary Survivor Transport

A rescued survivor can become a Temporary Passenger before station recruitment.

The ship must have valid capacity.

## 22. Robot Transport

Robot transport consumes:

- physical bay/cargo capacity;
- mass;
- charging/service capacity where required.

Combat robot squad rules belong to GDS-10.

## 23. Utility Bays

Utility bays can host specialized mission systems such as:

- mining equipment;
- salvage equipment;
- repair drone rack;
- sensor array;
- field storage expansion;
- robot deployment rack.

## 24. Utility Capacity

A utility system occupies a real slot/bay and contributes:

- mass;
- power demand;
- heat;
- cargo displacement where relevant.

## 25. No Universal Cargo Conversion

Cargo capacity cannot automatically transform into passenger seats, robot racks, or cryogenic tanks without installing compatible modules.

## 26. Cargo Manifest

The ship UI provides:

- total mass;
- used/free volume;
- categories;
- mission-security state;
- hazards;
- reserved cargo;
- destination/transfer task where relevant.

## 27. Reservations

Cargo can be reserved for:

- mission objective;
- station production delivery;
- trade;
- repair;
- ship operation.

Reservations do not move ownership.

## 28. Field Storage Container Integration

A field storage container is not automatically ship cargo.

Its contents require a valid transfer.

## 29. Persistence

Cargo ownership, quantities, container state, reservations, security state, and passenger/robot occupancy persist.

## 30. Edge Cases

If cargo is loaded while route is planned, Navigation recalculates cost.

If a refrigerated hold loses power, spoilage behavior changes rather than deleting food.

If a passenger is aboard while life support degrades, occupant safety follows ship-environment/crew rules.

If cargo interface is destroyed, cargo remains onboard but transfer may become impossible until repaired or alternative access exists.

## 31. Explicit Non-Goals

Cargo does not provide:

- infinite hold capacity;
- cargo with no mass;
- automatic station teleportation;
- instant secure status from proximity;
- arbitrary category conversion.

## 32. Tuneable Parameters

Tuneable values include hold mass/volume, refrigeration power, containment capacity, transfer throughput, passenger count, and utility-bay capacity.

## 33. Dependencies

This specification depends on GDS-4 Resource Model, GDS-5 Inventory, Ship Classes/Configuration, Flight/Propulsion, Docking, Missions, Crew Survivor/Recruitment, Robots, and Station Logistics.

## 34. Open Questions

None in the cargo/utility baseline.
