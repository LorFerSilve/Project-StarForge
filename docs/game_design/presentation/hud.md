# HUD

> **Status:** Design Complete  
> **Authority:** Default in-game HUD composition, adaptive visibility, combat/mission/ship/station/tactical information, markers, warning priority, and HUD accessibility

## 1. Purpose

The HUD communicates immediate information needed for action without becoming a permanent dashboard for every simulation subsystem.

## 2. HUD Philosophy

The HUD is **adaptive**.

Information appears when relevant and recedes when not needed.

The baseline does not permanently display every resource, crew state, station network, mission objective, robot statistic, and market value at once.

## 3. Persistent Minimal Layer

During ordinary first-person gameplay the minimal HUD can contain:

- reticle/context focus;
- Health state;
- personal Shield state when equipped;
- selected weapon/tool and immediate ammo/energy state;
- current critical environmental state where relevant;
- current objective summary when enabled.

## 4. Context Layer

Contextual elements appear for:

- interactable focus;
- nearby mission objective;
- pickup/loot;
- squad command target;
- hazard warning;
- identified actor;
- vehicle/docking interaction;
- repair/scanner result.

## 5. Combat Layer

When combat becomes relevant, HUD can expose:

- reticle state;
- loaded/reserve ammunition;
- weapon heat/charge where applicable;
- Health/Shield;
- active critical status effects;
- grenade/gadget availability;
- hostile identification when legitimately known;
- robot squad command/status summary;
- critical incoming damage direction feedback.

## 6. No Enemy Health-Bar Requirement

Standard enemies do not require a floating RPG health bar by default.

Where useful, the game can communicate condition through:

- physical damage;
- behavior;
- scan/targeting data;
- authored boss/system UI;
- accessibility setting.

Subsystem/ship/station targets can show known condition when sensors support it.

## 7. Hit Confirmation

Optional hit confirmation can communicate:

- valid hit;
- shield interaction;
- armor interaction;
- weak-point hit;
- target defeat/disable where confirmed.

It must not reveal hidden internal state beyond legitimate target knowledge.

Hit markers are configurable and may be disabled.

## 8. Weak-Point Feedback

A weak-point hit uses distinct visual/audio confirmation from an ordinary hit when the player could reasonably identify the effect.

No random critical-number burst is required.

## 9. Damage Direction

Player damage-direction indicators show approximate incoming direction where the attack/impact provides a physically perceivable cue.

They do not point through the world to an otherwise unknown hidden attacker with perfect accuracy.

## 10. Status Effects

Critical player status effects show:

- icon/symbol;
- name on first application/inspection;
- severity/duration where knowable;
- immediate consequence.

Color is secondary.

## 11. Environmental HUD

Environmental information appears when:

- suit is sealed;
- external condition is unsafe;
- a threshold is approached;
- scanner/environment panel is requested.

Relevant values can include:

- Life-Support Reserve;
- Suit Energy;
- pressure/atmosphere status;
- thermal state;
- radiation/contamination;
- gravity/zero-G state.

Safe indoor traversal need not permanently show all values.

## 12. Interaction Prompt

Focused interactables show a compact prompt containing:

- action verb;
- target name/type if known;
- input binding;
- blocker/state if action unavailable.

Example:

`[E] Cycle Airlock`

or

`Cycle Airlock — Blocked: outer door not sealed`

## 13. Loot/Pickup HUD

On focus/pickup, show:

- item/resource identity if known;
- quantity;
- Mass/Volume impact where relevant;
- ownership/security state when mission-relevant;
- inventory-capacity blocker.

Do not permanently display market value over all world loot.

## 14. Mission Objective HUD

Current tracked objective can show:

- concise objective text;
- distance/direction when known and allowed;
- progress only when the objective has meaningful measurable progress;
- extraction requirement.

Hidden objectives remain hidden.

## 15. Objective Markers

Markers follow knowledge state.

Possible states:

- exact location known;
- search area known;
- direction/signal known;
- location unknown.

The HUD must not collapse these into exact waypoint markers.

## 16. Search Area

If only an area is known, HUD/map shows the bounded search region rather than a fake precise target icon.

## 17. Extraction HUD

When extraction becomes relevant, show:

- extraction point/state;
- physical requirements;
- contested/blocked state;
- unsecured holdings warning;
- owned robots not yet secured;
- departure commit status.

## 18. Robot Squad HUD

The default squad summary displays only active field-relevant data:

- Robot ID/name/class shorthand;
- Operational/Degraded/Disabled state;
- communication state;
- current command;
- critical ammo/energy warning.

Detailed component condition belongs in tactical/management inspection.

## 19. TCC HUD

TCC is shown when composing/deploying or modifying the active squad.

During routine field play, a compact `used / available` TCC indicator may appear near squad controls.

## 20. Robot Command Feedback

Issued orders show command-state feedback:

- Accepted;
- Executing;
- Completed;
- Blocked;
- Failed;
- Disconnected.

Blocked/Failed state gives concise reason where known.

## 21. Spacecraft HUD

Primary ship-flight HUD provides:

- velocity vector/speed;
- orientation/reference cue;
- throttle/thrust state;
- target state;
- hull/critical subsystem condition;
- Shield state;
- weapon/ammo/energy state;
- Fuel/Propellant warning where relevant;
- power/thermal critical warnings;
- navigation/docking cues.

## 22. Ship Flight Reference

Because flight is 6DoF/inertial, HUD must clearly distinguish:

- ship nose orientation;
- actual velocity vector;
- target direction;
- relative target velocity where sensors support it.

## 23. Docking HUD

Docking mode presents:

- selected docking port;
- alignment;
- relative position;
- relative velocity;
- capture envelope;
- incompatible/blocked reason.

It must not secretly auto-teleport the ship into Hard Dock state.

## 24. Ship Damage HUD

The main HUD emphasizes critical system losses rather than every percentage.

Examples:

- Main Drive Disabled;
- Reactor Critical;
- Cooling Over Capacity;
- Life Support Offline;
- Sensor Degraded;
- Weapon Mount Disabled.

Detailed schematic is available in ship systems UI.

## 25. Station HUD

While walking Horizon, only local/critical station state is shown by default.

Global station information appears when:

- critical alarm active;
- player opens station overview;
- focused console exposes it.

## 26. Station Emergency Banner

Critical station events can display:

- event category;
- affected zone/module;
- severity;
- immediate instruction/available response if known.

Multiple simultaneous events prioritize by threat and can be expanded in overview.

## 27. Raid HUD

Offensive raid HUD can show:

- current raid objective;
- escalation state;
- bridgehead/extraction state;
- known reinforcement status;
- squad state;
- current known target-system state;
- carried/extracted loot state when relevant.

Unknown reinforcement information remains unknown.

## 28. Defense HUD

During Horizon defense the player can access:

- attack phase;
- identified breach/approach points;
- known attackers;
- defense-system failures;
- boarding state;
- stolen-cargo tracking where known;
- withdrawal/extraction threat.

## 29. Compass Policy

A minimal directional compass/heading strip is allowed.

It can contain known tracked markers but must not become an omniscient radar.

## 30. Minimap Policy

A permanent omniscient minimap is not required.

If a local map/radar is available, its data is constrained by:

- explored geometry;
- sensors;
- station plans;
- faction intel;
- current jamming/system state.

## 31. Actor Markers

Friendly/hostile/neutral markers appear when identification is justified.

An unknown actor remains Unidentified until observed/scanned/communicated sufficiently.

## 32. Marker Occlusion

Through-wall markers are permitted only for information the player legitimately has and only when useful.

Examples:

- assigned robot location via telemetry;
- known station emergency module;
- tracked mission beacon.

Unknown enemies do not receive wallhack markers.

## 33. Notification Feed

A small feed can show noncritical state changes such as:

- resource acquired;
- objective updated;
- Work Order completed;
- Research completed;
- robot returned;
- reputation changed.

Repeated routine events aggregate.

## 34. Credits Display

Credits are shown in trade/economy contexts and transaction feedback.

They do not need permanent top-corner display during ordinary exploration/combat.

## 35. Floating Numbers

Floating damage/healing/XP numbers are not baseline.

Optional accessibility/debug-style numeric combat feedback may exist, but physical/audio/UI state is primary.

## 36. No XP Bar

There is no generic player XP/level bar because no generic Player Level exists.

Crew skill progress appears only in crew/training contexts.

## 37. HUD Modes

Baseline presets:

- Standard;
- Minimal;
- Expanded;
- Custom.

These alter information presentation, not gameplay rules.

## 38. Minimal Mode

Minimal retains mandatory lethal/survival/interaction information but suppresses routine objective/notification clutter where safe.

## 39. Expanded Mode

Expanded can retain more persistent:

- objective;
- squad;
- environmental;
- system data.

It must not reveal otherwise unknown information.

## 40. HUD Scale and Safe Area

HUD supports:

- global scale;
- text scale;
- safe-area adjustment;
- selected element opacity/size where feasible.

## 41. Motion and Flash

HUD elements avoid unnecessary pulsing, screen-edge flashing, and rapid animation.

Critical animation can be reduced through accessibility settings while retaining icon/text/audio redundancy.

## 42. Color Accessibility

Hostile/safe/warning/critical states pair color with:

- icon;
- label;
- shape;
- pattern;
- animation or audio where appropriate.

## 43. Reticle Accessibility

Reticle supports configurable:

- size;
- thickness;
- opacity;
- contrast;
- center dot;
- color from accessible presets.

Reticle options do not change weapon spread/hit mechanics.

## 44. Aim Assist Boundary

Aim-assistance presentation can support accessibility/controller use only as defined in Accessibility.

The HUD itself cannot fabricate hits or weak-point knowledge.

## 45. Screenshot/Cinematic Mode

A HUD-hide option can remove optional UI for screenshots.

If active during gameplay, mandatory lethal alerts may remain unless the player explicitly chooses full HUD off with warning.

## 46. Explicit Non-Goals

No permanent omniscient minimap, enemy health-bar requirement, generic XP bar, permanent Credits counter, floating damage-number requirement, or exact waypoint for unknown objectives.

## 47. Tuneable Parameters

Element placement, size, fade timing, opacity, marker range, notification duration, damage-indicator persistence, and default preset composition are tuneable.

## 48. Dependencies

References Player, Combat, Missions, Robots, Spacecraft, Station, Raids, GDS-12 systems, UI/UX, Interaction Feedback, Alarms, and Accessibility.

## 49. Open Questions

None in the HUD baseline.
