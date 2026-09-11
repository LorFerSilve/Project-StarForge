# Logistics, Reservations, and Transfer Runtime

> **Status:** Architecture Complete  
> **Authority:** Horizon physical inventory nodes, accessibility-aware logistics graph, resource reservation, transfer requests, deterministic routing, in-transfer ownership, local buffers, docking logistics, and failure recovery

## 1. Purpose

Station logistics must let Horizon present convenient high-level inventory views while preserving the GDS rule that physical resources always have exactly one authoritative owner.

The architecture therefore separates:

- inventory ownership;
- reservation entitlement;
- route eligibility;
- transfer execution;
- aggregate UI views.

## 2. Authoritative Ownership

Physical resources remain owned by TA-2-compatible inventory/buffer owners such as:

```text
StorageNodeId
MachineInputBufferId
MachineOutputBufferId
ConstructionBufferId
RepairBufferId
FarmInputBufferId
FarmOutputBufferId
DockTransferBufferId
ShipCargo owner
PlayerInventory owner
```

Logistics never creates a second “network inventory” copy.

## 3. Logistics Domain Store

Persistent station logistics state is owned by `StationLogisticsStore`.

It contains:

- storage-node policy;
- logistics endpoints;
- transfer requests;
- reservations;
- transfer jobs;
- interrupted-transfer records where needed;
- routing/priority policy;
- local buffer configuration;
- active docking cargo-link state.

Physical quantities continue to live in their owning inventories/buffers.

## 4. Logistics Graph

The runtime Logistics Graph is derived from:

- logistics-capable TA-6 utility links;
- standard/bulk transfer classes;
- active cargo hubs/lifts/transfer devices;
- docking cargo interfaces;
- current damage/power/operability;
- structural/traversal isolation where a transfer method depends on them.

Disconnected networks cannot automatically exchange cargo.

## 5. Logistics Edge

A runtime edge contains:

- endpoint IDs;
- transfer class;
- directional/bidirectional capability;
- maximum throughput per Simulation Time interval;
- current availability;
- required active equipment/power if any;
- cargo-category restrictions;
- route-cost metric;
- stable tie-break identity.

## 6. Storage Node Policy

Each storage endpoint publishes:

- accepted cargo categories;
- current finite capacity;
- preferred/blocked categories;
- keep-minimum reserve policies;
- export permission;
- security/access state;
- refrigeration/hazard capability where relevant;
- current accessibility/network membership.

## 7. Transfer Request

A consumer/automation/player action creates a typed `LogisticsTransferRequest` containing:

- requested resource/item ContentId or UniqueItemId;
- quantity;
- destination owner/buffer;
- priority;
- reservation/job correlation;
- allowed source policy;
- required transfer class;
- deadline/urgency if applicable;
- current request revision.

Creating a request does not move cargo.

## 8. Reservation Model

A reservation is a claim against a specific quantity/unique item at a real physical owner.

Reservation state contains:

- source owner;
- resource/item identity;
- reserved quantity;
- reservation owner/job ID;
- purpose;
- priority;
- expiry/cancellation policy if any;
- revision.

Reserved inventory remains physically at source until transfer/consumption commits.

## 9. Reservation Invariant

For a physical quantity:

```text
Total active reservations <= available reservable quantity
```

Protected/non-exportable/reserved-for-other-purpose quantities are excluded according to policy.

Two jobs cannot both reserve the same last item.

## 10. Deterministic Reservation Arbitration

Competing reservation requests are resolved by:

1. protected strategic policy;
2. request priority;
3. explicit player/job ordering;
4. stable request ID.

No unordered-container iteration can decide who receives scarce inventory.

## 11. Transfer Lifecycle

Canonical logistics transfer lifecycle:

```text
Requested
↓
SourceSelected
↓
Reserved
↓
ReadyForTransfer
↓
InTransfer
↓
Delivered
```

Nonterminal interruption states include:

```text
Blocked
RouteInvalidated
Paused
```

Terminal alternatives include:

```text
Cancelled
Lost (only through explicit damage/loss rule)
```

## 12. Source Selection

Source selection filters by:

- actual quantity/item ownership;
- reservation availability;
- storage policy;
- network accessibility;
- transfer-class compatibility;
- security/access;
- route capacity;
- destination acceptance.

Eligible sources are ranked deterministically by configured policy, route cost, and stable owner ID.

## 13. Route Planning

A route is a path through the Logistics Graph with sufficient compatible edge class/capacity.

Routing can use deterministic shortest-cost path/flow algorithms.

The exact implementation may be optimized later, but equivalent inputs must produce equivalent eligible routing decisions.

## 14. Throughput Scheduling

Edges have finite throughput budgets over Simulation Time.

Transfer jobs consume edge budgets according to priority and stable ordering.

A path bottleneck can queue a transfer even when both source and destination are otherwise valid.

## 15. Abstract vs Visible Transfer

The authoritative transfer does not require one visible robot per item.

Two execution classes are supported:

### Abstract Network Transfer

Used by built-in logistics trunks/conveyors/lifts where physical per-carrier simulation is not gameplay-critical.

Progress advances by route throughput and Simulation Time.

### Actor-Carried Transfer

Used where a service robot/player/crew actor physically carries cargo and route/access/security matter at actor level.

TA-7/TA-8 later define actor execution, while TA-6 owns the same reservation/ownership semantics.

## 16. In-Transfer Ownership

Cargo that has left its source but not reached the destination requires exactly one authoritative transfer owner.

Conceptually:

```text
TransferCargoOwner{TransferJobId}
```

At source-removal commit:

```text
Source -> TransferCargoOwner
```

At delivery commit:

```text
TransferCargoOwner -> Destination
```

This avoids ambiguous “half moved” quantities.

## 17. Abstract Transfer Atomic Milestones

An abstract transfer can model progress continuously, but physical ownership changes only at explicit milestones.

For ordinary station internal transport, baseline ownership milestones are:

1. pickup/source-removal commit;
2. delivery/destination commit.

Save/load can therefore represent the cargo as source-owned, transfer-owned, or destination-owned—never duplicated.

## 18. Route Break Mid-Transfer

If a route becomes invalid after pickup:

- transfer cargo remains owned by `TransferCargoOwner`;
- scheduler searches for a valid continuation/return/recovery endpoint;
- if none exists, the transfer becomes Blocked at a defined logical/physical handoff location;
- explicit damage/loss can remove cargo only through a separate committed consequence.

No automatic source refund duplicates cargo.

## 19. Local Buffers

Machine/farm/repair/construction input/output buffers are first-class physical owners.

A global station summary can include them but cannot spend them while policy/reservation/process state makes them unavailable.

## 20. Output Blocking

A machine/farm output remains in its local output owner until logistics moves it.

When output capacity is full:

- new output cannot be committed unless the process definition has valid alternate destination capacity;
- production/harvest enters its GDS blocked state;
- output is not deleted.

## 21. Docking Logistics

An active cargo-transfer link to a docked ship adds explicit Logistics Graph endpoints/edges with finite throughput.

Ship cargo remains a separate owner.

Station and ship inventory never merge into one container.

Undocking invalidates the cargo link only after in-flight transfer milestones are safely resolved/blocked under docking transaction rules.

## 22. Manual Carry

Player manual carry uses the same inventory transfer semantics but route validation comes from player accessibility/interactions rather than the abstract logistics graph.

The station scheduler cannot treat manual carry capacity as automatically available while the player is elsewhere.

## 23. Secure / Hazard / Refrigerated Storage

Storage capability metadata constrains eligible destinations.

A hazardous item cannot be routed into incompatible general storage merely because capacity is free.

Refrigeration state affects spoilage systems but does not change physical ownership.

## 24. Accessible Inventory Read Model

The station inventory UI builds a read model with categories such as:

```text
Accessible
Reserved
InTransfer
Isolated
LocalBuffer
Protected
```

These are views over physical ownership/reservation/network state, not extra inventories.

## 25. Consumption Boundary

Manufacturing/construction/repair/research can consume only inventory that:

- is owned by the required source/local buffer;
- is reserved/authorized for that job where required;
- is actually delivered before the defined consumption stage.

A remote storage count alone does not satisfy local process input.

## 26. Cancellation

Cancelling a request/job:

- releases unconsumed reservations;
- does not recreate already consumed inputs;
- does not teleport transfer-owned cargo;
- routes/reassigns in-transfer cargo according to normal recovery policy.

## 27. Damage / Storage Loss

Storage damage can reduce capacity or expose cargo.

Physical loss is committed by the owning damage/hazard rule against the real owner quantity/item.

Logistics only reacts to the resulting state change.

## 28. Network Topology Invalidation

A `LogisticsTopologyRevision` change invalidates cached routes.

Requests/transfers revalidate before the next routing milestone.

Already delivered/consumed state is never rolled back merely because a path later breaks.

## 29. Off-Screen Transfer

Off-screen Horizon may aggregate abstract transfer progress using larger deterministic Simulation-Time slices.

The scheduler must split at:

- pickup ownership commit;
- delivery ownership commit;
- source depletion;
- destination full;
- route invalidation;
- relevant priority/deadline boundary;
- attack/damage event that can affect cargo.

Actor-carried transfers can use TA-8 off-screen abstractions later, but must preserve the same ownership milestones/capacity/time.

## 30. Persistence

Persisted logistics state includes:

- storage policies;
- request state;
- reservation state;
- transfer jobs;
- transfer-cargo ownership;
- configured priorities;
- persistent docking cargo-link facts where applicable.

Derived routes/queues/edge budgets can be rebuilt deterministically.

## 31. Integrity Validation

Save/load validation checks:

- every reservation references an existing owner/job;
- reserved quantity does not exceed eligible source quantity;
- each transfer-owned cargo quantity has exactly one TransferJob;
- no UniqueItemId has multiple owners;
- destinations/categories remain schema-valid;
- completed/cancelled jobs do not retain illegal active reservations.

## 32. Debugging

Development tooling exposes:

- logistics graph/islands;
- edge capacity/use;
- storage policies;
- active reservations;
- source-selection candidates;
- transfer routes/progress;
- in-transfer owners;
- blocked reason;
- aggregated inventory reconciliation.

A station-wide conservation report must reconcile physical quantities across storage, buffers, transfers, ships, and explicit consumption/loss.

## 33. Explicit Non-Goals

TA-6 logistics does not provide:

- global teleporting inventory;
- infinite throughput;
- implicit storage merging;
- duplicate reservations;
- automatic item creation on route recovery;
- a requirement to visibly simulate every package.

## 34. Dependencies

Depends on TA-2 ownership/transactions, TA-6 topology/Power, TA-5 docking/physical access boundaries, GDS Storage & Logistics, Construction, Manufacturing, Damage/Repair, Resources, Spacecraft Cargo, and Automation.

## 35. Open Questions

None within TA-6 logistics architecture.
