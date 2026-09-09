# Station Storage and Logistics

> **Status:** Draft  
> **Authority:** Physical station inventory ownership, storage capacity, resource routing, transfer priorities, local buffers, and logistics failure behavior

## 1. Purpose

Storage and logistics determine where station resources physically exist and whether they can reach systems that need them.

The station may present an aggregated inventory interface, but resources do not teleport between disconnected locations.

## 2. Physical Ownership

Every persistent physical resource stack or quantity belongs to a storage location or system buffer.

Examples:

- cargo storage;
- fuel tank;
- water tank;
- machine input buffer;
- machine output buffer;
- ship cargo;
- farm output buffer.

The UI can aggregate accessible inventory without removing physical ownership.

## 3. Storage Categories

Storage can define allowed cargo categories.

Baseline examples include:

- general cargo;
- bulk raw materials;
- manufactured components;
- refrigerated food;
- hazardous materials;
- ammunition;
- fuel;
- secure strategic items.

A storage module cannot accept a category it is not configured or rated to hold.

## 4. Capacity

Storage has finite capacity.

Capacity may be constrained by:

- item slots;
- volume/mass abstraction;
- fluid quantity;
- authored category limits.

The exact storage model for each item type is defined by the resource catalog.

## 5. Local Buffers

Machines can contain local:

- input buffers;
- work-in-progress state;
- output buffers.

Local buffers are not equivalent to general station storage.

A blocked output can stop production even when storage capacity exists elsewhere but cannot be reached.

## 6. Logistics Network

Station cargo movement uses one or more connected **Logistics Networks**.

A network consists of:

- compatible logistics-capable module connections;
- cargo hubs;
- transfer devices;
- storage nodes;
- production nodes;
- docking cargo interfaces.

Disconnected networks cannot automatically exchange cargo.

## 7. Standard Logistics Backbone

Normal station structural modules can provide a standard internal logistics trunk where authored.

This avoids requiring a visible conveyor for every small transfer.

The backbone still has:

- connectivity;
- throughput;
- routing;
- failure states.

## 8. Heavy/Bulk Transfer

Large or high-throughput industrial flows may require:

- dedicated cargo connections;
- bulk transfer systems;
- freight lifts;
- specialized industrial links.

A standard low-capacity logistics path does not provide unlimited throughput.

## 9. Transfer Requests

Consumers create logistics requests.

A request defines:

- requested item/resource;
- quantity;
- destination;
- priority;
- reservation owner where applicable.

The logistics scheduler routes eligible available inventory toward requests.

## 10. Reservations

Resources can be reserved for:

- construction;
- manufacturing;
- repairs;
- research;
- mission preparation;
- protected reserves.

Reserved resources remain physically stored but cannot be simultaneously allocated to conflicting jobs.

Reservation never duplicates inventory.

## 11. Transfer Lifecycle

A routed transfer follows the conceptual lifecycle:

```text
Requested
↓
Source Selected
↓
Reserved
↓
In Transfer
↓
Delivered
```

A failed route returns the request to an unresolved state without creating a second copy of the cargo.

## 12. Transfer Abstraction

The game does not require one fully simulated physical robot object for every transferred item.

Transfers can be represented through an abstract throughput simulation while using visible cargo drones, conveyors, lifts, or animations for feedback.

Gameplay outcomes must still respect physical connectivity and throughput.

## 13. Throughput

Logistics connections have finite transfer capacity.

High demand can create queues.

A local production bottleneck may therefore result from logistics even when enough input exists elsewhere.

## 14. Transfer Priority

Transfer requests use configurable priority.

Default high-priority categories include:

- life-support critical inputs;
- emergency repairs;
- protected defense supply;
- active construction/production according to player policy.

The player can configure eligible priorities.

## 15. Source Selection

When several valid sources contain an item, logistics chooses a source using deterministic policy based on:

- accessibility;
- reservation;
- configured storage policy;
- route cost/throughput;
- priority.

The exact solver may be optimized technically as long as gameplay-equivalent rules remain deterministic.

## 16. Storage Policies

The player can configure storage behavior such as:

- accepted categories;
- preferred categories;
- keep-minimum reserve;
- export allowed/blocked;
- priority.

Automation follows these rules.

## 17. Secure Storage

Selected rare or strategic resources may be stored in secure storage.

Secure storage can matter to:

- station raids;
- security;
- access control.

It does not make resources magically immune to every damage event.

## 18. Refrigerated Storage

Perishable goods stored in active refrigerated storage use improved spoilage behavior.

Refrigeration requires its defined power and thermal support.

Loss of refrigeration changes spoilage rate rather than instantly deleting food.

## 19. Hazardous Storage

Hazardous resources can require specialized storage.

Incorrect storage is not allowed when the resource definition explicitly requires containment.

Damage to hazardous storage can create subsystem-defined hazards.

## 20. Docking Logistics

A docked compatible ship can connect its cargo system to station logistics.

Transfer requires:

- compatible dock;
- cargo-transfer connection;
- access authorization;
- available throughput.

Ship cargo is not part of station inventory while physically disconnected.

## 21. Manual Carrying

The player can manually move selected portable items between accessible locations.

Manual carrying provides an early-game or emergency alternative to automation.

It does not bypass item-size restrictions or impossible traversal.

## 22. Logistics Automation

Automation may:

- route inputs;
- clear outputs;
- maintain configured reserves;
- supply construction;
- supply repairs;
- restock ammunition;
- balance storage.

It must not consume a protected or locked resource for an unauthorized purpose.

## 23. Logistics Officer Interface

The logistics system exposes tasks to the crew domain.

A qualified Logistics Officer can improve:

- routing oversight;
- shortage response;
- storage-policy management;
- fault detection.

They do not create throughput beyond physical infrastructure unless their skills explicitly improve operational efficiency within defined limits.

## 24. Network Failure

Logistics can fail because of:

- destroyed connection;
- disabled cargo hub;
- power loss to active transport;
- full destination;
- missing source;
- priority starvation;
- blocked docking interface.

The UI identifies the blocking cause.

## 25. Isolated Inventory

If a station branch becomes isolated:

- resources physically stored there remain there;
- other networks cannot spend them;
- local systems may continue using them if connected locally.

Aggregated UI must distinguish unavailable isolated inventory from accessible inventory.

## 26. Global Inventory View

The player can inspect a station-wide inventory summary.

The view distinguishes:

- accessible total;
- reserved;
- in transfer;
- isolated/unavailable;
- local-buffer quantities.

A global view is informational, not a magical global container.

## 27. Full Destination

If destination storage/buffer is full:

- the transfer waits or selects another valid destination;
- cargo is not destroyed;
- upstream production may become output-blocked.

## 28. No Valid Destination

A generated output with no valid storage remains in the producer's output buffer if possible.

If the producing recipe requires empty output capacity before execution, the job does not begin until capacity exists.

## 29. Damage and Cargo Loss

Storage damage can:

- reduce capacity;
- make cargo inaccessible;
- expose cargo to loss;
- create leaks for fluid storage.

Cargo loss occurs only according to explicit damage rules.

## 30. Persistence

Save state preserves:

- storage contents;
- storage policies;
- reservations;
- local buffers;
- in-progress transfer state or an equivalent deterministic reconstruction;
- logistics topology;
- priorities.

No resource may duplicate because a save occurred during transfer.

## 31. Edge Case — Source Destroyed During Transfer

A transfer can deliver only cargo already removed into valid transfer state.

Cargo that had not left the source is subject to source-damage rules.

The system must maintain exactly one authoritative ownership state for each quantity.

## 32. Edge Case — Route Breaks Mid-Transfer

Cargo enters a defined interrupted-transfer state or returns to the nearest valid ownership node according to transfer implementation.

The result must conserve quantity except for explicit damage/loss.

## 33. Edge Case — Reservation Job Cancelled

Unused reserved resources are released.

Resources already consumed by the owning process are not recreated.

## 34. Edge Case — Two Jobs Request Last Item

Reservation resolution is deterministic.

Only one job receives the available quantity.

The other remains waiting.

## 35. Progression

Early game:

- small storage;
- manual movement;
- low throughput;
- simple routing.

Mid game:

- specialized storage;
- automated routing;
- cargo hubs;
- Logistics Officer support.

Late game:

- distributed high-throughput networks;
- secure reserves;
- automated balancing;
- redundant routes;
- large ship/industrial transfer capacity.

## 36. Explicit Non-Goals

The baseline does not require:

- every item to have a permanently visible carrier NPC;
- global teleporting inventory;
- infinite storage;
- instant transfer across disconnected modules;
- arbitrary resource duplication through reservations or save/load.

## 37. Tuneable Parameters

Tuneable values include:

- storage capacity;
- transfer throughput;
- route cost;
- queue priority weights;
- refrigerated spoilage modifiers;
- secure-storage capacity;
- manual carry limits.

## 38. Dependencies

This specification depends on:

- [Structural Rules](structural_rules.md);
- [Power](power.md);
- [Station Construction](construction.md);
- `manufacturing.md`;
- `damage_and_repairs.md`;
- `automation.md`;
- `../resources/`;
- `../spacecraft/`;
- `../crew/`.

## 39. Open Questions

None in the current logistics baseline.

The document remains Draft until resource inventory, spacecraft cargo, damage, automation, and crew specifications are cross-validated.
