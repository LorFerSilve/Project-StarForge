# Glossary

> **Status:** Draft  
> **Authority:** Project-wide game-design terminology

This glossary standardizes terminology used throughout Project StarForge.

A subsystem specification may introduce additional local terms, but it must not redefine a glossary term with a conflicting meaning.

## Automation

The delegation of recurring operational tasks to crew, robots, machinery, or control systems according to defined rules.

## Crew

A recruited persistent character assigned to the player's station or an authorized expedition role.

A rescued survivor is not automatically crew until recruitment is completed.

## Design Complete

A specification state indicating that all implementation-relevant gameplay behavior is defined and no unresolved design question requires an implementer to invent intended behavior.

See [Design Authority](00_design_authority.md).

## Expedition

A player operation outside the home station undertaken to complete objectives such as exploration, resource gathering, salvage, rescue, research, defense, or combat.

## Home Station

The player's persistent, physically traversable, modular space station and primary progression hub.

## Mission

A structured gameplay objective or set of objectives with defined entry conditions, success conditions, failure rules, and rewards.

## Mission Zone

A bounded playable 3D environment used for mission gameplay.

A mission zone is not assumed to represent an entire seamless planet, moon, asteroid, or station at real scale.

## Module

A structural or functional building unit integrated into a space station.

## Raid

An offensive operation against a hostile station or comparable fortified installation in which the player may attack external defenses, board the target, fight internally, sabotage infrastructure, acquire objectives or resources, and extract.

## Robot

A manufactured autonomous or semi-autonomous machine used for station operations, combat, support, or tactical deployment.

## Sector

A strategic subdivision of navigable game space containing one or more relevant systems, locations, routes, or points of interest.

## Station System

A connected operational network or functional domain within a station, such as power, atmosphere, thermal control, logistics, security, or production.

## Survivor

A non-hostile character encountered outside the player's established crew structure who can potentially be rescued, assisted, or recruited.

## Tuneable Parameter

A numerical or data-driven value intended to be adjustable during balancing without redefining the underlying gameplay rule.

Examples include damage, duration, capacity, range, production speed, and resource yield.

## Assignment

A persistent crew responsibility linking one crew member to a compatible facility, zone, network, or station-wide professional duty.

A crew member has one Primary Assignment at a time.

## Profession

A crew member's primary recognized role and default automation domain, such as Engineer, Farmer, Scientist, or Logistics Officer.

Profession does not erase independently learned secondary skills.

## Skill

A learned crew competence rated from 0 (Untrained) through 5 (Master) in a specific domain.

## Temporary Guest

A rescued or otherwise accepted character temporarily supported by the home station who has not yet become permanent assigned crew.

Temporary Guests consume applicable station support but cannot hold permanent crew assignments.

## Workload Capacity

The finite amount of professional supervision or assigned work a crew member can support at one time.

Workload Capacity depends on relevant skill, traits, health, and applicable automation infrastructure.

## Blueprint

A persistent Knowledge Asset representing possession of the concrete design required to build or manufacture a specific object.

A Blueprint is distinct from the underlying technology required to understand or produce that design.

## Knowledge Asset

Persistent non-physical information such as a Blueprint Unlock, Research Evidence record, or analyzed technology record.

Knowledge Assets do not occupy ordinary physical cargo capacity.

## Resource

A canonical physical material, fluid, consumable, fuel, or manufactured component used by gameplay systems.

Every physical resource quantity has one authoritative owner/location.

## Resource Deposit

A finite world source of a canonical raw resource with a defined remaining reserve, extraction requirement, and Yield Grade.

## Processing

The transformation of raw or bulk resources into refined materials or standardized industrial feedstock.

## Crafting

The transformation of refined materials/components into manufactured components, consumables, or finished items according to a Blueprint/Recipe.

## Research Evidence

Persistent non-consumable scientific knowledge derived from a secured analysis, scan, data record, or discovery.

Research Evidence can satisfy project requirements but is not spent like currency.

## Yield Grade

A property of a resource deposit describing the amount of canonical resource recovered per unit of extraction work.

Yield Grade is not a separate inventory-quality tier.

## Field-Unsecured

A physical resource or item acquired during an external mission that has not yet been committed to an authorized secure extraction owner.

Field-Unsecured loot is subject to ordinary mission-failure loss.

## Secured Loadout

Physical equipment, ammunition, consumables, tools, or supplies already owned before mission deployment and committed from persistent station/ship inventory.

Ordinary mission defeat preserves the unconsumed recovered portion according to Player Inventory rules.

## Heavy Load

A player movement state entered when carried Mass exceeds the backpack's soft-load threshold while remaining below hard capacity.

Heavy Load can reduce sprint and jump performance.

## Life-Support Reserve

The finite consumable capacity of a sealed player suit used to maintain breathable internal atmosphere when the external environment is unsafe.

It is distinct from electrical Suit Energy.

## Suit Energy

Finite electrical energy available to active player-suit systems such as thermal regulation, zero-gravity thrusters, personal shields, scanners, and powered tools.

## Incapacitated

A player or crew health state in which normal movement, work, and direct action are unavailable until rescue, stabilization, or recovery occurs.

## Ship Hull Class

A structural spacecraft category defining the normal scale and configuration envelope of a ship, including mass, module capacity, hardpoints, cargo, occupancy, and docking compatibility.

## Hull Variant

A specific authored spacecraft frame within a Ship Hull Class with exact geometry, slots, mass limits, interior scope, and docking interface.

## Flight Assist

An avionics mode that uses available thrusters to stabilize unwanted linear and angular motion while preserving the ship's actual thrust, mass, power, and damage constraints.

## Strategic Reach Class

A propulsion capability tier from Reach I through Reach IV used to determine whether a spacecraft can traverse a strategic galaxy route.

## Propellant

A physical spacecraft resource expelled or otherwise used as reaction mass to produce momentum.

Propellant is distinct from Fuel, which provides energy.

## Hard Dock

A physical spacecraft attachment state in which compatible docking connectors are captured and eligible finite service links such as cargo, power, atmosphere, or refueling can be established.

## Derelict

A spacecraft state in which the ship is Disabled and lacks normal active operator/control capability while remaining a persistent physical asset.

## Vehicle/Extraction-Secured

A mission-acquired physical resource or item that has been successfully committed to an authorized secure extraction owner such as valid spacecraft cargo but has not necessarily yet been transferred to Station-Secured ownership.

## The Fracture

The interstellar infrastructure collapse at 0 AF that disrupted the Transit Lattice, isolated regions, fragmented political authority, and produced the post-collapse world of Project StarForge.

## AF — After Fracture

The primary historical calendar notation used for years after The Fracture.

The game begins in 67 AF.

## Transit Lattice

The pre-Fracture interconnected ecosystem of route relays, navigation observatories, reference infrastructure, autonomous traffic control, and anomaly-prediction systems used to coordinate interstellar travel.

## Strategic Location

A selectable world-map node such as a station, planet orbit, asteroid field, derelict zone, faction facility, relay, or anomaly site.

## Faction Reputation

A persistent independent -100 to +100 relationship value used by standard human major factions to represent how that faction evaluates the player's demonstrated actions.

## The Continuance

A distributed machine-intelligence/infrastructure network descended from pre-Fracture autonomous systems operating fragmented preservation, restoration, and containment directives.

## Horizon Station

The player's persistent home station located in the Aster's Rest System of the Recovery Sector.

## Postgame Resolution State

The persistent world-state outcome created after the campaign finale, including whether the player chose Stabilize, Sever, or Contain and the resulting route/faction/epilogue consequences.

## Mission ID

A persistent unique identifier for one authored or procedural mission and its lifecycle, objectives, generation data, outcomes, and reward state.

## Mission Instance

The currently deployed runtime/persistent mission state associated with a Mission ID, including active zones, objective state, spawned entities, discoveries, hazards, and mission-specific world changes.

## Objective ID

A mission-local unique identifier for one objective and its prerequisites, state, completion condition, and failure condition.

## Point of Interest — POI

A discoverable local mission/world site such as a resource deposit, wreck, signal, survivor location, anomaly, cache, or structure.

## Overall Threat

The highest currently known value among a mission's Hostile Threat, Environmental Threat, Operational Complexity, and Extraction Risk axes.

## Extraction Point

An authorized physical or logical mission exit that can commit a defined extraction transition when its requirements are satisfied.

## Reward Delivery Claim

A persistent non-physical entitlement representing a physical mission-resolution reward that has been earned but not yet delivered into a valid physical inventory owner.

## Extraction Required

A mission rule stating that completing primary objectives alone is insufficient for Success and that an authorized extraction transaction must also commit.

## Damage Packet

The authoritative combat event produced after a valid hit, containing damage channel components, penetration, hit location, impulse, status payloads, and source/target identity.

## Penetration

An attack property that reduces the relevant Armor/Protection Rating before mitigation is calculated.

Penetration does not directly add bonus damage.

## Weak Point

An authored physical or sensor-discoverable target zone that produces an explicitly defined damage multiplier or subsystem consequence when hit.

## Shield Cost Multiplier

A per-damage-channel coefficient determining how much Shield Capacity is consumed to absorb incoming damage.

## Combat Disabled

A target state satisfying an explicitly defined combat-disable condition such as loss of mobility, weapons, control, power, or general operational capability without requiring destruction.

## Last Known Position

The most recent player/target position credibly perceived or communicated to an AI actor, retained with confidence that decays when no new information is obtained.

## Tactical Reload

A reload performed while ammunition remains loaded, preserving existing loaded rounds and transferring only the compatible amount needed from reserve.

## Clear Area

A combat-objective resolution condition in which no qualifying hostile actor capable of immediate combat remains inside the defined objective area under that objective's explicit rules.

## Robot ID

A persistent unique identifier for one player-owned robot and its physical configuration, condition, energy, location, assignment, squad membership, and destruction/loss state.

## Tactical Control Capacity — TCC

A non-currency command-system capability limiting the combined Command Load of actively commanded field robots.

## Command Load

The Tactical Control Capacity cost contributed by one active robot class/configuration.

## Robot Rack Unit

The standard spacecraft transport-footprint unit used by Light and Medium robots in compatible Robot Racks.

## Heavy Robot Bay

A dedicated spacecraft transport/service capability required for Heavy-class robots that cannot use ordinary Robot Rack capacity.

## Internal Energy Reserve

A robot's finite electrical charge state used for movement, control, sensors, communication, tools, shields, and weapons where applicable.

It is not a separate crafting currency.

## Class-Incapable

A robot state in which the unit remains physically operational/mobile but has lost the mandatory hardware capability required to perform its declared robot class role.

## Lost Robot

A player-owned Robot ID that is no longer available for normal deployment because it was abandoned or became unrecoverable, while not necessarily having been physically Destroyed.

## Robot Wreck

The non-operational physical remains of a Destroyed robot, potentially containing salvageable components/materials but no longer repairable into the same Robot ID.

## Rules of Engagement — ROE

A configured robot combat-authorization policy such as Hold Fire, Defensive, Engage Hostiles, or Engage Designated Target Only.

## Raid Target

A persistent fortified Strategic Location or installation with raid-relevant defenses, access routes, storage, security, system dependencies, intelligence, damage, and prior raid state.

## Raid Intelligence

Persistent information about a Raid Target classified as Unknown, Suspected, Confirmed, or Stale and acquired through legitimate reconnaissance, sensing, hacking, observation, or other world sources.

## Bridgehead

A temporarily secured tactical entry area during boarding that can support rallying, resupply, robot deployment/recovery, and access to the player's extraction route without becoming an invulnerable safe zone.

## Escalation State

The raid-wide strategic response state of a fortified target, progressing through states such as Quiet, Suspicious, Alerted, Mobilized, Reinforcements En Route, Reinforced/Full Response, and Withdrawal/Evacuation.

## Reinforcement Call

A persistent raid transaction through which a valid target requests external help, including Calling, Committed, Responding, and Arrived states plus explicit disruption/cancellation branches.

## Secured Entry

A raid entry route whose immediate physical access obstruction has been resolved and which is currently traversable, without implying permanent safety or ownership.

## Attacker-Controlled

A temporary interior tactical state in which qualifying local defenders are not immediately contesting an area and the attacker has valid access; it can become Contested again.

## Defense Event ID

A persistent unique identifier for one hostile raid against Horizon Station and its attacker composition, objectives, phase, damage, stolen cargo, withdrawal, and resolution state.

## Hostile Extraction Commit

The transaction point at which a hostile raid carrier or ship successfully departs with stolen physical assets, permanently removing those assets from the player's current ownership unless a later world event explicitly recovers them.

## Credits — Cr

The baseline non-physical ledger currency used for market purchases, sales, services, and authored economic rewards.

Credits are not a physical Resource and do not occupy Mass/Volume.

## Market Inventory

The finite physical stock currently owned by a market/vendor and available for eligible purchase.

Buying transfers ownership out of Market Inventory; selling transfers physical ownership into it.

## Market Liquidity

The finite Credit purchasing capacity available to a market/vendor for buying goods from the player during the relevant economic state.

## Active Game Time

Elapsed time during which the game is actively running rather than closed. It is distinct from wall-clock time and may include periods where Simulation Time is paused.

## Simulation Time

The authoritative gameplay-progression clock used by production, Research, farming, healing, markets, Dynamic Events, raids, recovery, and other simulation systems.

True Pause stops Simulation Time. Closing the application does not advance it.

## Stable Save Boundary

A game state at which no authoritative atomic transaction is half-applied and a consistent save snapshot can therefore be committed.

If a save is requested during an atomic transaction, it waits for the next Stable Save Boundary.

## Dynamic Event ID

A persistent unique identifier for one generated or authored cross-cutting world event and its seed, lifecycle, conditions, effects, and resolution state.

## Recovery Grace

A bounded active-Simulation-Time protection period after a qualifying severe Horizon Station setback during which ordinary severe follow-up station attacks are suppressed.

Recovery Grace does not repair the station or freeze unrelated gameplay.

## Difficulty Profile

A player-selected set of allowed execution-pressure modifiers such as Assisted, Standard, Veteran, Extreme, or Custom.

Difficulty Profile does not redefine world progression, Reach, Research/Blueprint prerequisites, loot eligibility, or story access.

## Capability Phase

One of the P0–P5 campaign pacing states that summarizes the player's actual cross-domain capability and story position.

A Capability Phase is not a Player Level and grants no free capability by itself.

## Finale Readiness Gate

The complete cross-domain validation performed through MS-F01 before MS-F02 becomes deployable, including story state, spacecraft, player loadout, Horizon support, robot/TCC capability, Support Channels, and the departure reserve manifest.

## Support Channel

One of the four finale preparation capability categories: Navigation and Route Support, Engineering and Recovery Support, Combat and Security Support, or Analysis and Containment Support.

A Support Channel can be fulfilled only by a valid external commitment or an explicitly defined self-sufficient capability path.

## Hybrid Presentation

The Project StarForge presentation model that combines physical/diegetic world communication with non-diegetic HUD, panels, maps, overlays, captions, and alerts when needed for reliable gameplay readability.

## Presentation Knowledge Boundary

The rule that UI, HUD, markers, audio, VFX, tutorials, and accessibility aids may communicate only information the player has legitimately perceived, discovered, scanned, received, or otherwise learned through authoritative gameplay state.

## Presentation Commit Boundary

The rule that presentation may acknowledge an input request immediately but may report success/completion only after the corresponding authoritative gameplay transaction commits.

## Alarm Priority

The GDS-13 presentation severity used for player-facing alerts: P0 Critical, P1 Severe, P2 Warning, or P3 Advisory.

Alarm Priority is a communication priority and does not replace the underlying gameplay severity/state.

## Objective Guidance

A presentation/accessibility setting controlling how explicitly already-known objective and route information is explained or marked.

Objective Guidance cannot reveal hidden objectives, secret routes, or unknown actors.

## Photosensitivity Safe Mode

An accessibility mode that reduces or removes rapid flashes, strobing, repeated high-contrast pulses, aggressive emissive flashes, and similar presentation while preserving the same gameplay warning through safer alternative channels.

## Reduced Effects Mode

An accessibility mode that reduces nonessential particles, smoke density, debris, sparks, bloom, distortion, and screen overlays while retaining all gameplay-critical hazard and interaction information.

## Aim Assistance

Optional bounded input assistance that can apply reticle slowdown/friction or mild camera rotation toward legitimately visible targets.

Aim Assistance cannot bend projectiles, create hidden hit chance, target through walls, reveal unknown enemies, or automatically select weak points.

## Closed Captions

Optional textual presentation of gameplay-relevant non-speech audio cues, constrained by the same audibility and knowledge rules as the underlying sound event.
