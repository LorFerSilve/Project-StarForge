# Mining

> **Status:** Draft  
> **Authority:** Mineral deposit representation, extraction work, yield, depletion, mining tools, temporary extraction machinery, and mining-specific hazards

## 1. Purpose

Mining is the primary acquisition method for geological raw resources.

It should create decisions around:

- location;
- deposit richness;
- exposure;
- extraction speed;
- cargo capacity;
- mission danger;
- temporary machinery;
- what material is worth extracting.

## 2. Mining Targets

Mining operates on explicit **Resource Deposits**.

A deposit definition contains:

- resource identity;
- total recoverable reserve;
- yield grade;
- physical extent/interaction nodes;
- hardness/extraction requirement;
- hazard tags;
- optional environmental constraints.

## 3. Deposit Reserve

A deposit has finite recoverable quantity.

Mining permanently reduces remaining reserve for persistent locations.

A deposit does not refill because the player leaves and returns unless world-generation rules explicitly respawn that location.

## 4. Yield Grade

Yield Grade describes recovered canonical resource per unit of extraction work.

Baseline grade bands are:

- Poor;
- Standard;
- Rich;
- Exceptional.

These are deposit properties, not separate ore inventory rarities.

## 5. Deposit Scan

Before or during extraction, the player can determine information such as:

- resource type;
- estimated remaining quantity;
- yield grade;
- hazard;
- required mining capability.

Information quality depends on scanner/tool capability.

## 6. Manual Mining

Portable mining tools allow direct player extraction from compatible deposits.

Manual mining:

- consumes active game time;
- requires physical access;
- creates raw resource output;
- is limited by tool capability;
- is limited by player/nearby cargo capacity.

## 7. Mining Tool Capability

A mining tool defines:

- eligible hardness classes;
- extraction rate;
- energy/ammunition/consumable requirement where applicable;
- thermal behavior where applicable;
- effective range;
- hazard compatibility.

A basic tool cannot mine a deposit outside its supported class.

## 8. Extraction Work

Mining converts extraction work into recovered raw resource.

Conceptually:

`Recovered Resource = Extraction Work × Deposit Yield Grade × Tool/Process Efficiency`

Exact coefficients are tuneable.

Recovered quantity cannot exceed remaining deposit reserve.

## 9. Mining Waste

Deposit extraction may generate unusable waste implicitly.

Waste need not become a stored resource unless a later production chain gives it meaningful use.

## 10. No Random Ore Quality

Mining always produces the canonical resource named by the deposit.

Example:

Titanium deposit → Titanium Ore

It does not create "Epic Titanium Ore".

## 11. Temporary Mining Equipment

Selected missions can allow deployable extraction machinery such as:

- mining drill;
- extractor;
- pump;
- regolith harvester.

Deployment requires:

- compatible terrain/deposit;
- machinery item;
- power/fuel where required;
- setup time;
- valid cargo/output handling.

## 12. Mining Machinery States

A deployed miner can be:

- Packed;
- Deploying;
- Ready;
- Extracting;
- Output Blocked;
- Unpowered;
- Faulted;
- Depleted;
- Packing.

## 13. Output Buffer

Automated mining output enters a local buffer or connected cargo container.

If output capacity is full:

- extraction pauses;
- deposit reserve is not consumed for undelivered output.

## 14. Defense-Mining Loop

Some mining missions can intentionally create the loop:

Locate deposit → deploy extractor → begin extraction → hostile pressure increases → defend site → transfer cargo → extract.

Mission design owns enemy timing/objectives; mining owns resource extraction.

## 15. Mining on Asteroids

Asteroid mining uses the same deposit model.

The game does not require physically fragmenting an entire asteroid mesh into arbitrary chunks.

Authored/procedural deposit nodes represent extractable material.

## 16. Planetary Mining

Planetary deposits can appear as:

- exposed veins;
- underground access nodes;
- mineral fields;
- geothermal/resource vents where compatible.

The baseline does not require freeform terrain excavation.

## 17. Water-Ice Mining

Water Ice can be extracted from:

- icy asteroids;
- comet material;
- frozen planetary deposits.

The mined resource remains Water Ice until processed.

## 18. Helium-3 Extraction

Helium-3 can use specialized resource-extraction sites rather than generic pickaxe/drill interactions.

Extraction may require:

- regolith processing;
- gas collector;
- specialized mission equipment.

The resource identity remains canonical Helium-3.

## 19. Uraninite Handling

Uraninite is Radioactive.

Mining and carrying it can require suitable protection/container rules defined by future player/ship equipment.

The mining UI must communicate the hazard before extraction when identified.

## 20. Exotic Deposit Mining

Voltaic Crystal deposits can require higher-tier tools and create environmental/energy hazards.

Exotic Matter Sample and Ancient Technology Core are not ordinary geological mining outputs by default.

## 21. Mining Noise/Threat

Mining activity can generate mission-specific threat or detection.

This is owned by mission/enemy design.

Mining exposes an activity intensity value so mission systems can react without redefining extraction.

## 22. Interruption

If manual or automated mining is interrupted:

- completed extraction remains valid;
- deposit reserve reflects only completed recovered quantity;
- partial work may retain progress where the tool/process supports it.

## 23. Deposit Depletion

When remaining reserve reaches zero:

- the deposit enters Depleted;
- it yields no further resource;
- scanning reports depletion.

## 24. Cargo Limitation

Mining does not bypass cargo capacity.

When available cargo/buffer fills:

- portable mining stops creating transferable output;
- automated miners enter Output Blocked.

## 25. Deposit Ownership and Missions

Mission/world rules determine whether a deposit is:

- freely harvestable;
- faction-owned;
- mission-restricted;
- illegal to mine.

Mining itself does not decide diplomacy.

## 26. Persistence

For persistent locations, remaining deposit reserve persists.

For generated mission instances, mission generation/persistence defines whether a future instance contains a new deposit.

Save/load cannot restore previously extracted reserve.

## 27. Edge Cases

If the deposit is destroyed by combat, remaining recoverable reserve follows authored destruction/salvage behavior.

If the miner is destroyed, extracted output already in its buffer remains subject to container damage; unextracted reserve remains in the deposit.

If multiple extractors work the same deposit, all draw from one authoritative remaining reserve.

If a deposit becomes inaccessible, extraction pauses rather than continuing remotely without a valid worker/machine.

## 28. UI

Mining UI communicates:

- resource;
- yield grade;
- remaining estimate;
- tool compatibility;
- extraction progress/rate;
- cargo capacity;
- hazards;
- output blockage.

## 29. Visual and Audio Feedback

Extraction state must correspond to visible/audio mining activity.

A depleted node should not visually behave as actively yielding material.

## 30. Progression

Early game:
- portable mining;
- common deposits;
- limited cargo;
- slow extraction.

Mid game:
- better tools;
- deployable miners;
- richer/dangerous deposits;
- specialized extraction.

Late game:
- high-tier mining machinery;
- exotic resource access;
- larger cargo logistics;
- automated mission-site extraction where mission design permits.

## 31. Explicit Non-Goals

Mining does not require:

- unrestricted voxel terrain destruction;
- infinite deposits;
- random ore rarity variants;
- resource generation after output buffer fills;
- universal one-tool mining of every material.

## 32. Tuneable Parameters

Tuneable values include deposit reserve, grade multipliers, extraction rates, tool efficiency, setup time, buffer size, and machinery power demand.

## 33. Dependencies

This specification depends on Resource Model, Resource Catalog, gathering, player tools/inventory, missions, spacecraft cargo, environmental hazards, and world persistence.

## 34. Open Questions

None in the current mining baseline.

The document remains Draft until player tools, missions, world generation, and cargo systems are cross-validated.
