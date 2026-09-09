# Station Farming and Food

> **Status:** Draft  
> **Authority:** Home-station food production, crop growth, food storage behavior, agricultural automation requirements, and food-security failure states

## 1. Purpose

The farming system makes long-term crew support a station-development problem without turning the game into repetitive individual feeding.

It connects:

- recruited crew;
- water;
- power;
- atmosphere;
- production;
- storage;
- automation;
- the Farmer profession.

## 2. Food Model

The baseline uses two main food categories:

- **Fresh Produce** — harvested from station agriculture and useful directly or as processing input;
- **Packaged Rations** — processed food with high storage stability.

Specific food items can provide content variety, but they follow these mechanical categories unless a dedicated rule says otherwise.

## 3. Crew Consumption

Crew create aggregate food demand over simulation time.

The player does not manually feed individual crew members under normal operation.

Exact consumption rates are tuneable and depend on the future authoritative time model.

## 4. Farming Modules

Agricultural production occurs in dedicated compatible facilities such as:

- hydroponics bays;
- aeroponics bays;
- algae culture systems;
- advanced synthetic agriculture.

Technology progression may introduce new facility types.

## 5. Crop Bays

A farming module contains one or more authored crop bays.

Each bay can run one crop plan at a time.

A crop plan defines:

- growth duration;
- water demand;
- electrical demand;
- nutrient demand;
- atmosphere requirements;
- thermal range;
- produce yield.

## 6. Crop Growth

Crop growth progresses only while minimum required conditions are satisfied.

Conditions include as applicable:

- water delivery;
- power;
- lighting;
- nutrients;
- suitable atmosphere;
- suitable temperature.

Growth is not granted merely because a crop timer exists.

## 7. Crop Health

Each active crop batch has a health state.

Short interruptions can pause or reduce growth.

Extended or severe failure can damage or destroy the batch.

The exact tolerance windows are tuneable per crop/system.

## 8. Harvest

A mature crop becomes harvest-ready.

Harvest can be:

- performed manually;
- performed by assigned agricultural automation;
- performed by a qualified Farmer through station automation.

Harvest output enters a local farm output buffer before logistics transfer.

## 9. No Output Duplication

A crop produces its authored yield once per completed growth cycle.

Reloading, disconnecting, reassignment, or storage routing cannot duplicate a harvest.

## 10. Nutrients

Agriculture consumes a standardized **Nutrient Mix** resource unless a specific technology defines another input.

Nutrient Mix can be:

- manufactured;
- recovered in part through recycling systems;
- acquired externally.

The resource/crafting domain owns recipes.

## 11. Water

Farming consumes Fresh Water from the water system.

Advanced systems may recover a large fraction of water through station recycling, but farming cannot operate with zero water input unless an explicit technology says so.

## 12. Power

Farms consume electrical power for functions such as:

- lighting;
- pumps;
- environmental control;
- automation.

Loss of power does not instantly destroy crops; it begins condition degradation according to tolerance rules.

## 13. Atmosphere

Standard station crops require a suitable pressurized environment.

Agricultural modules interact with atmosphere and may influence gas composition where defined.

The baseline does not require detailed plant-by-plant gas simulation.

## 14. Temperature

Crops require an authored acceptable temperature range.

Thermal failure can:

- pause growth;
- reduce health;
- destroy a crop after sufficient exposure.

## 15. Farmer Role

The farming system exposes operational tasks for the crew domain.

A qualified Farmer can automate:

- crop scheduling;
- planting;
- nutrient management;
- water monitoring;
- harvesting;
- fault response;
- crop-priority decisions within player policy.

A Farmer does not generate food without functioning agricultural infrastructure and inputs.

## 16. No Mandatory Farmer

A station can operate farms without a recruited Farmer if the player has sufficient manual or technological capability.

Without a Farmer or advanced automation:

- the player performs more setup/response tasks;
- fault handling is less automatic;
- scheduling requires more direct management.

This ensures the Farmer is valuable without creating an unrecoverable progression lock.

## 17. Farmer Capacity

A Farmer has finite management capacity.

One early-game Farmer cannot perfectly supervise unlimited agriculture.

Capacity can improve through:

- skill;
- tools;
- station automation;
- agricultural control systems.

Exact capacity belongs to crew specifications.

## 18. Crop Scheduling

The player can configure production targets or crop priorities.

Examples:

- maintain minimum ration reserve;
- prioritize fresh produce;
- prioritize a recipe input;
- stop planting a crop.

Automation uses these targets rather than making strategic food choices independently.

## 19. Fresh Produce Storage

Fresh Produce can spoil over time unless preserved.

Spoilage progression can be slowed by:

- refrigerated storage;
- processing;
- suitable preservation technology.

Exact spoilage duration is tuneable.

## 20. Packaged Rations

Packaged Rations are manufactured from suitable food inputs.

They:

- have much greater storage stability;
- are suitable for expedition provisioning;
- count toward station food reserve.

Exact recipes belong to resource/manufacturing specifications.

## 21. Food Processing

Food processors can convert agricultural outputs into stable rations or other authored food products.

Processing requires its defined:

- inputs;
- power;
- machinery;
- packaging materials where applicable.

## 22. Food Reserve

The player can configure a protected station food reserve.

Automation avoids consuming protected stock for nonessential purposes such as export or optional crafting unless explicitly authorized.

## 23. Food Logistics

Food is physically stored and moved through storage/logistics rules.

A farm output buffer that is full can block further harvesting.

A station having free storage elsewhere is insufficient if no valid logistics route exists.

## 24. Food Shortage

When available food is below projected crew demand, the station generates escalating shortage warnings.

Shortage consequences are handled by crew rules.

Routine food shortage does not instantly permanently kill recruited crew.

## 25. Farming Failure

Farming can fail because of:

- insufficient water;
- insufficient power;
- nutrient shortage;
- thermal failure;
- atmosphere failure;
- output blockage;
- damaged equipment;
- unavailable automation.

The UI reports the actual limiting condition.

## 26. Manual Interaction

The player can:

- select crop plans;
- start/stop planting;
- harvest manually;
- inspect crop health;
- replenish local inputs;
- clear output;
- repair compatible equipment;
- configure automation targets.

## 27. Farming Overview UI

The agricultural interface communicates:

- active crops;
- growth progress;
- crop health;
- missing inputs;
- projected harvest;
- food reserve;
- crew food demand;
- projected food endurance;
- Farmer/automation coverage;
- blocked outputs.

## 28. Persistence

Save state preserves:

- active crop type;
- growth progress;
- health;
- local inputs;
- output buffers;
- crop scheduling;
- food inventory;
- spoilage state;
- automation targets.

No offline real-world growth occurs.

## 29. Edge Case — Storage Full at Harvest

A crop does not duplicate or delete output.

If the local output buffer cannot accept the harvest:

- the batch remains harvest-ready or output-blocked;
- production of the next batch does not proceed normally;
- the player/automation must free capacity.

## 30. Edge Case — Temporary Power Loss

A brief outage pauses active systems and begins crop tolerance timing.

Restoring conditions before serious damage allows growth to continue.

## 31. Edge Case — Farmer Incapacitated

The farm infrastructure remains physically present.

Tasks previously handled by that Farmer return to:

- another eligible worker;
- available automation;
- player attention.

Food does not vanish and active crops do not instantly fail.

## 32. Edge Case — Crew Population Increases

Recruiting more crew increases projected food demand.

The game does not automatically scale farm output to match population.

The station must expand supply or rely on stored/imported food.

## 33. Progression

Early game:

- small hydroponics;
- manual scheduling;
- imported/salvaged food important;
- low reserves.

Mid game:

- dedicated Farmer;
- automated harvest;
- ration processing;
- larger storage.

Late game:

- multiple agriculture modules;
- advanced recycling;
- automatic reserve targeting;
- specialized crop production;
- high self-sufficiency.

## 34. Explicit Non-Goals

The baseline does not require:

- individual plant simulation;
- manual watering of every crop;
- personal hunger bars for routine player gameplay;
- food appearing directly in global inventory without logistics;
- unlimited farm output from one worker.

## 35. Tuneable Parameters

Tuneable values include:

- crew consumption;
- crop duration;
- water use;
- power use;
- nutrient use;
- yield;
- spoilage time;
- crop tolerance;
- Farmer capacity modifiers.

## 36. Dependencies

This specification depends on:

- [Power](power.md);
- [Atmosphere and Oxygen](atmosphere_and_oxygen.md);
- [Thermal Systems](thermal_systems.md);
- [Water](water.md);
- `storage_and_logistics.md`;
- `manufacturing.md`;
- `automation.md`;
- `../crew/`;
- `../resources/`;
- `../systems/time_and_simulation.md`.

## 37. Open Questions

None in the current farming baseline.

The document remains Draft until crew, resource, logistics, manufacturing, and time specifications are cross-validated.
