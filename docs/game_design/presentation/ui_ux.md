# UI and UX

> **Status:** Design Complete  
> **Authority:** Project-wide information architecture, menu hierarchy, interaction model, planning interfaces, state communication, confirmation rules, error/blocker messaging, and UX consistency

## 1. Purpose

UI exists to expose authoritative game state without replacing the physical game world.

The player must be able to understand:

- current state;
- available actions;
- blockers;
- consequences;
- ownership;
- risk;
- progress;
- recovery paths.

## 2. UX Principle

The baseline uses a **hybrid diegetic/non-diegetic interface**.

World objects communicate function physically, while critical systemic information can appear through conventional HUD, panels, maps, and overlays.

No critical mechanic is hidden merely to preserve diegetic purity.

## 3. Information Hierarchy

UI information is divided into:

1. Immediate Action State;
2. Current Objective/Tactical State;
3. Local System State;
4. Strategic Management State;
5. Historical/Reference Information.

The default screen should emphasize the first two only.

## 4. Primary Interface Surfaces

Canonical interface surfaces:

- Pause Menu;
- Character/Loadout;
- Inventory;
- Mission Log;
- Galaxy Map;
- Station Overview;
- Construction Mode;
- Station Management;
- Research;
- Manufacturing;
- Crew Management;
- Robot/Squad Management;
- Spacecraft Configuration;
- Trade/Market;
- Raid Planning;
- Settings/Accessibility;
- Codex/Logs where authored.

## 5. Pause Menu

True Pause Menu pauses simulation under GDS-1.

It provides:

- Resume;
- Save;
- Load;
- Settings;
- Controls;
- Accessibility;
- Return to Main Menu.

Gameplay management interfaces do not automatically count as pause screens unless explicitly opened through true pause context.

## 6. Gameplay Interfaces

Interfaces used in-world during active operation may remain live, such as:

- quick inventory;
- tactical robot commands;
- ship targeting;
- station emergency overview;
- raid tactical state.

Their time behavior must be explicitly indicated.

## 7. Input Focus

At any moment one UI layer owns primary pointer/navigation focus.

The game must not simultaneously interpret the same input as:

- firing weapon;
- clicking UI;
- issuing robot command

unless a specific combined interaction is intentionally authored.

## 8. Back/Cancel Consistency

A standard Back/Cancel action always:

- closes the current subpanel;
- cancels uncommitted preview state;
- returns one hierarchy level.

It does not undo already committed transactions unless the action itself supports reversal.

## 9. Commit vs Preview

Potentially consequential operations distinguish:

- Preview;
- Confirm/Commit;
- Completed.

Examples:

- station construction;
- dismantling;
- large trade;
- protected-resource spending;
- dangerous route launch;
- robot demolition command;
- raid departure;
- finale departure.

## 10. Confirmation Policy

Confirmation dialogs are used only for actions with material irreversible or hard-to-reverse consequences.

Do not require confirmation for routine low-risk operations.

Examples requiring confirmation:

- dismantle unique/expensive object;
- abandon recoverable robot;
- overwrite manual save;
- hostile action against neutral strategic target;
- commit finale departure;
- destructive action involving protected infrastructure.

## 11. Blocker Messaging

When an action is unavailable, UI states the direct known reason.

Good:
- `Insufficient Propellant: requires 18 kg, available 11 kg.`
- `No Heavy Robot Bay installed.`
- `Route not Verified.`
- `Target interface not physically accessible.`

Bad:
- `Cannot do that.`
- `Requirements not met.`
- `Level too low.`

## 12. Unknown Information

UI must not leak hidden world knowledge.

If the game cannot legitimately explain a blocker because the cause is unknown to the player, present:

- Unknown Risk;
- Signal Unresolved;
- Requirement Unknown;
- Insufficient Intel

rather than revealing the hidden cause.

## 13. Known vs Suspected vs Confirmed

Raid and exploration interfaces preserve authoritative information states.

The UI visually/textually distinguishes:

- Unknown;
- Suspected;
- Confirmed;
- Stale

without converting uncertain intel into exact truth.

## 14. Tooltips

Tooltips explain:

- what a value represents;
- its unit;
- current source/modifier;
- major consequence;
- authoritative relationship when helpful.

They should not be required for immediate lethal warnings.

## 15. Units

Physical values display appropriate units consistently.

Examples:

- Mass: kg/t;
- Volume: L/m³ where applicable;
- Power: kW/MW;
- Energy: kWh/MWh or gameplay-defined equivalent;
- Pressure: kPa;
- Temperature: °C by default with unit setting where supported;
- Distance: m/km;
- Time: s/min/h.

Do not show unitless engineering values when the underlying rule has a meaningful physical unit.

## 16. Resource Ownership UI

Inventory summaries may aggregate holdings but must show location/owner when it matters.

Examples:

- Backpack;
- Ship Cargo;
- Horizon Storage;
- Reserved for Work Order;
- Field-Unsecured;
- Vehicle/Extraction-Secured.

The UI cannot imply that one item exists simultaneously in several places.

## 17. Inventory UX

Inventory emphasizes:

- item/resource;
- quantity;
- Mass;
- Volume;
- security/ownership state;
- equipped/reserved status;
- compatible actions.

Sorting/filtering can use:

- category;
- mass;
- volume;
- value;
- name;
- mission relevance.

## 18. Equipment UX

Equipment screen validates:

- slot compatibility;
- carried Mass;
- ammo/energy dependency;
- environmental capability;
- current condition.

Changing equipment does not create missing ammunition/components.

## 19. Station Overview UX

Station Overview provides layered views for:

- modules/structure;
- power;
- atmosphere;
- thermal;
- water;
- logistics;
- manufacturing;
- security;
- defense;
- crew/robot work.

One primary layer is emphasized at a time.

## 20. Network Overlay UX

A network overlay shows:

- producers/sources;
- consumers/sinks;
- connections;
- disconnected segments;
- bottlenecks;
- warnings;
- redundancy.

If exact downstream consequence is unknown due to sensor/access limits, it remains unknown.

## 21. Construction UX

Construction mode shows:

- selected piece/module;
- footprint;
- attachment sockets;
- orientation;
- required resources;
- utility/structural requirements;
- valid/warning/invalid state;
- exact known blocker.

Preview is noncommitted until Confirm.

## 22. Manufacturing UX

Work Orders expose:

- recipe;
- reserved inputs;
- missing inputs;
- facility;
- current stage;
- progress;
- power/logistics blocker;
- output destination;
- cancellation consequence.

## 23. Research UX

Research displays prerequisite relationships and distinguishes:

- Technology Research;
- Research Evidence;
- Blueprint ownership.

The interface must never imply Evidence is spent if the rule says it persists.

## 24. Crew UX

Crew screens show:

- identity;
- Profession;
- relevant Skills;
- Traits;
- Primary Assignment;
- workload;
- health/availability;
- training;
- current location/task when known.

No rarity color is used as a quality summary.

## 25. Robot UX

Robot management shows:

- Robot ID/name;
- Class;
- chassis/components;
- Operational/Degraded/Disabled/Destroyed state;
- energy/ammo;
- assignment;
- squad;
- Command Load;
- transport requirement;
- repair blockers.

Destroyed units are visually separated from deployable roster entries.

## 26. Squad UX

Squad composition simultaneously exposes:

- total Command Load;
- available TCC;
- transport Rack Units;
- Heavy Bay requirement;
- environmental compatibility;
- active/reserve status.

The player should immediately see whether the limiting factor is command, transport, configuration, or environment.

## 27. Spacecraft Configuration UX

Ship fitting shows real constraints:

- Mass;
- power generation/load;
- thermal capacity/load;
- fuel/propellant;
- cargo;
- life support;
- hardpoints/slots;
- Robot Rack/Heavy Bay;
- Strategic Reach.

Invalid configurations explain exact reasons.

## 28. Galaxy Map UX

Galaxy Map communicates:

- known locations;
- route connections;
- route state;
- minimum Reach;
- current ship capability;
- faction control where known;
- active missions/events;
- known hazard/intel;
- travel resource estimate.

Unknown space remains unknown.

## 29. Route Validation

Before travel, map reports:

- route known/verified state;
- Reach compatibility;
- fuel/propellant requirement;
- ship operational blockers;
- known access/faction restriction;
- known environmental/navigation risk.

Hidden hazards are not leaked.

## 30. Mission UX

Mission Log distinguishes:

- Offered;
- Accepted;
- Deployed;
- Success;
- Failed;
- Abandoned;
- Expired where applicable.

Objectives preserve Primary/Secondary/Optional/Extraction/Hidden semantics.

## 31. Threat UX

Mission Threat displays the four independent 0–5 axes:

- Hostile;
- Environmental;
- Operational Complexity;
- Extraction Risk.

Overall Threat may be highlighted, but individual axes remain visible so one severe danger is not hidden by averaging.

## 32. Raid Planning UX

Raid Planning separates:

- Intel;
- Approach;
- Access;
- Interior Objective;
- Withdrawal;
- Loadout/Squad;
- Cargo capacity;
- known reinforcements/escalation risks.

Unknown/stale intelligence remains visibly uncertain.

## 33. Economy UX

Trade interfaces show before commit:

- quantity;
- buy/sell unit price;
- total Credits;
- market stock;
- market liquidity when relevant;
- player cargo impact;
- reputation/access modifiers if known;
- resulting ownership destination.

## 34. Price Transparency

The player can inspect why a known price differs through factors such as:

- local market condition;
- faction relation;
- scarcity;
- service fee.

The UI need not expose hidden simulation internals that the player has no access to.

## 35. Dynamic Event UX

Events communicate:

- source/location;
- event type;
- urgency/deadline if present;
- known threat;
- expected consequence/reward where known;
- whether accepting creates a Mission.

Gameplay timers use Simulation Time semantics rather than real-world countdown wording.

## 36. Save UX

Save interface distinguishes:

- Manual Saves;
- Quick Save;
- Autosaves.

If a save request is waiting for a Stable Save Boundary, show:

`Saving… waiting for current transaction to complete.`

Do not silently ignore the request.

## 37. Failure UX

Failure/recovery summary explains causal consequences:

- mission state;
- Field-Unsecured loss;
- secured cargo preserved;
- consumed ammunition/fuel;
- robot state/loss;
- ship/station damage;
- reputation/credit transaction where explicit;
- retry/recovery path.

No vague `You lost resources` summary.

## 38. Progression UX

Progression interfaces show actual capability prerequisites rather than a generic level.

Examples:

- Reach III Research incomplete;
- Route unverified;
- Advanced Fabricator missing;
- Command Link III not equipped;
- Robotics Specialist unavailable;
- Faction access alternative available.

## 39. Finale Readiness UX

MS-F01 displays the complete readiness categories:

- Story State;
- Reach-IV Ship;
- Departure Reserves;
- Player Loadout;
- Horizon Support;
- Field TCC;
- Tactical Squad;
- Navigation Support;
- Engineering/Recovery Support;
- Combat/Security Support;
- Analysis/Containment Support.

Each failed category expands into exact known blockers and valid known provider paths.

## 40. Search and Filtering

Large management lists support filtering/search where scale justifies it.

Examples:

- resources;
- Blueprints;
- crew;
- robots;
- Work Orders;
- missions;
- logs.

## 41. Notification Priority

Notifications use priority:

- Critical;
- High;
- Normal;
- Informational.

Critical alerts can interrupt/anchor attention; informational messages should not cover combat-critical screen space.

## 42. Notification Deduplication

Repeated identical routine events aggregate rather than spam.

Example:

`3 manufacturing orders completed`

is preferable to three overlapping banners if no immediate action is required.

## 43. Text Standards

UI labels use concise consistent nouns/verbs.

Canonical terms from `GLOSSARY.md` are reused rather than synonyms that create ambiguity.

## 44. Error Prevention

UI should prevent invalid transactions before commit where the invalidity is known.

It must not rely on rollback after an impossible operation if the game already knew it was invalid.

## 45. Accessibility Baseline

All major UI supports:

- UI scaling;
- text scaling where safe;
- keyboard/mouse and controller navigation architecture;
- remappable controls;
- color-independent states;
- reduced animation;
- subtitle/caption support;
- high-contrast modes/markers.

## 46. Persistent Preferences

UI, control, accessibility, audio, display, subtitle, and difficulty preferences persist separately from individual game-world save transactions where appropriate.

## 47. No Mobile-Game UX

The baseline avoids:

- premium-currency storefront language;
- real-world countdown pressure;
- daily login reward framing;
- notification badges designed to create compulsive check-in loops.

## 48. Explicit Non-Goals

No diegetic-purity requirement, generic `level too low` blockers, color-only states, hidden transaction consequences, menu-only replacement of physical logistics, or real-world timer framing.

## 49. Tuneable Parameters

Panel layout, spacing, font sizes, animation durations, notification durations, sorting defaults, overlay opacity, and information density are tuneable within these interaction rules.

## 50. Dependencies

References all GDS-1 through GDS-12 gameplay domains and is refined by HUD, Interaction Feedback, Alarms, Onboarding, and Accessibility.

## 51. Open Questions

None in the UI/UX architecture baseline.
