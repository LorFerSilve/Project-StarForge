# TA-11 — HUD, Markers, Notifications, and Interaction Presentation

> **Status:** Architecture Complete  
> **Authority:** HUD composition, knowledge-filtered markers, reticle/prompts, notifications, interaction feedback, damage/ship/station/mission overlays, and commit-synchronized player feedback

## 1. Purpose

The HUD must communicate immediate actionable state without becoming an omniscient dashboard or a second gameplay model.

TA-11 therefore treats HUD output as a composition of knowledge-filtered Read Models plus committed presentation events.

## 2. HUD Ownership Boundary

The HUD owns only presentation of:

- currently known player/action state;
- contextual interaction information;
- selected/tracked objective state;
- legitimate markers;
- known warnings;
- notification/event summaries;
- presentation settings.

It does not own detection, damage, objectives, resource security, docking, robot commands, station incidents, or mission completion.

## 3. HudReadModel

A `HudReadModel` is published after a safe simulation boundary and contains only player-facing information allowed by the TA-2 Knowledge Filter.

It can contain purpose-specific subviews such as:

- PlayerStatus;
- Equipment/WeaponStatus;
- InteractionFocus;
- TrackedObjective;
- EnvironmentStatus;
- SquadSummary;
- SpacecraftFlightStatus;
- StationCriticalStatus;
- Raid/DefenseStatus;
- AlarmSummary;
- known marker descriptors.

The HUD never scans raw DomainStores or RuntimeEntityRegistry for extra information.

## 4. HUD Presets

Baseline HUD modes:

- Standard;
- Minimal;
- Expanded;
- Custom.

Presets control visibility/density only.

Every preset retains mandatory lethal/survival/interaction information required by GDS unless the player explicitly selects full HUD-off with the documented warning.

## 5. HUD Layering

Conceptual order:

1. world-space eligible markers;
2. reticle/focus/interact layer;
3. player/combat/environment status;
4. objective/squad/vehicle context;
5. alarms/critical banners;
6. notifications;
7. captions/subtitles where applicable;
8. system/loading/save feedback overlays.

Critical layers can visually pre-empt lower-priority screen space without deleting underlying incidents.

## 6. Reticle

Reticle presentation exposes configurable:

- size;
- thickness;
- opacity;
- center dot;
- outline/contrast;
- accessible semantic color preset.

Reticle settings do not change spread, recoil, collision, hit resolution, or aim-assist eligibility.

## 7. Interaction Focus

The Interaction domain supplies a filtered `InteractionFocusView` containing conceptually:

- target display identity/type if known;
- primary/alternate action availability;
- blocker/warning text token/data;
- action `ActionId` for prompt glyph resolution;
- progress state if meaningful;
- consequence/security/ownership information where legitimate.

The HUD does not independently choose target priority.

## 8. Interaction Prompt

A prompt is resolved from semantic action and current binding:

`[effective binding] + action verb + target + known blocker/warning`.

If a binding changes, prompt display updates from the effective bindings Read Model without changing interaction semantics.

## 9. Interaction Feedback State

Presentation states are:

- Available;
- AvailableWithWarning;
- InProgress;
- Blocked;
- Completed/Activated;
- Disabled/Unavailable;
- Unknown/Unidentified.

These states are projections of owning gameplay state/results, not independent interaction states.

## 10. Progress Indicators

Progress indicators are shown only when actual gameplay work/progress exists and knowing progress is useful.

A presentation animation cannot fabricate progress for an atomic action.

If partial progress is persistent, the Read Model supplies that value; UI animation is not the source.

## 11. Immediate Request Acknowledgement

A valid input request can receive immediate nonauthoritative acknowledgement such as focus/button response.

Success/ownership/reward/repair/docking/save-complete cues occur only after the corresponding authoritative commit/result.

## 12. Commit-Synchronized Feedback

Committed domain events are transformed into `PresentationEvent`s such as:

- item/resource transferred;
- objective updated/completed;
- robot command accepted/rejected;
- reload completed;
- docking capture/hard dock committed;
- Work Order completed;
- save queued/committed/failed.

Each event carries correlation/source information sufficient for deduplication and stale-event rejection.

## 13. No False Success

TA-11 presentation cannot show:

- loot acquired before ownership transfer;
- target defeated before confirmed committed state;
- repair complete before repair commit;
- Hard Dock before docking commit;
- save complete before TA-12 confirms write;
- final objective success before Mission commit.

## 14. Resource Ownership Feedback

When location/security matters, feedback includes authoritative source/destination/security semantics.

Example conceptually:

`12 Titanium -> Ship Cargo (Vehicle/Extraction-Secured)`

rather than an ambiguous `+12`.

## 15. Field Security Presentation

Mission resource presentation preserves:

- Field-Unsecured;
- Vehicle/Extraction-Secured;
- Station-Secured.

HUD/inventory icons/text use the same source state and never imply an earlier security promotion.

## 16. Objective HUD

The tracked objective view may contain:

- concise text;
- objective class;
- progress if measurable;
- legitimate direction/distance/search area;
- extraction requirement;
- blocker/state.

Hidden objectives are absent until legitimately discovered.

## 17. Marker Descriptor

A world/HUD marker is generated only from an eligible `MarkerDescriptor` containing conceptually:

- semantic marker kind;
- source/target persistent identity where allowed;
- knowledge state;
- location precision class;
- world/local position or search region if known;
- through-wall eligibility;
- max presentation range if any;
- icon/text style role;
- source revision/generation.

The HUD cannot manufacture precise positions from vague intel.

## 18. Marker Precision Classes

Baseline marker precision:

- Exact;
- SearchArea;
- DirectionOnly;
- Signal/Approximate;
- Unknown/no marker.

A less precise source cannot be upgraded to Exact by presentation convenience.

## 19. Knowledge State

Where applicable markers retain:

- Unknown;
- Suspected;
- Confirmed;
- Stale.

A stale/suspected marker uses distinct presentation and never silently becomes confirmed.

## 20. Through-Wall Markers

Through-wall markers are permitted only when the player legitimately has remote/telemetry/known-location information and the GDS allows that presentation.

Examples include assigned robot telemetry, known station emergency locations, or a tracked mission beacon.

Undetected hostiles never receive wallhack markers.

## 21. World-to-Screen Projection

Marker projection uses TA-4 camera/view data.

The renderer/UI can:

- edge-clamp directional indicators;
- hide behind-camera exact icons where appropriate;
- apply overlap/priority layout;
- reduce clutter.

These operations never modify marker source location or gameplay knowledge.

## 22. Marker Occlusion

Where an exact marker should respect world occlusion, TA-11 uses presentation visibility/occlusion data derived through TA-4/TA-5 safe query/read paths.

Occlusion evaluation is presentation-only and cannot reveal an otherwise ineligible entity.

## 23. Damage Direction

Damage-direction indicators originate from committed/legitimately perceivable impact facts.

They provide approximate direction consistent with the cue and must not identify a hidden attacker more precisely than the player could legitimately perceive.

## 24. Hit Confirmation

Optional hit confirmation can distinguish valid committed/confirmed outcomes such as:

- hit;
- shield interaction;
- armor interaction;
- weak point;
- defeat/disable when known.

It never substitutes for physical hit authority or leaks hidden target internals.

## 25. Enemy Health Presentation

Standard enemy health bars are not required.

If a content/accessibility/system case exposes known condition, it must come from a legitimate combat/scan Read Model. The HUD cannot query raw Health to bypass sensor/knowledge rules.

## 26. Environment Status

Environmental HUD appears adaptively from known player/suit/environment state and can include:

- Life-Support Reserve;
- Suit Energy;
- pressure/atmosphere;
- thermal state;
- radiation/contamination;
- gravity/Zero-G state.

Safe ordinary contexts can suppress noncritical values.

## 27. Robot Squad Summary

Field squad HUD contains only field-relevant known state such as:

- identity/class shorthand;
- Operational/Degraded/Disabled;
- communication state;
- current command;
- critical ammo/energy;
- compact TCC use where relevant.

Detailed component data remains in management/tactical inspection.

## 28. Spacecraft HUD

Flight HUD consumes a dedicated ship presentation Read Model for:

- ship nose/reference orientation;
- actual velocity vector/speed;
- target direction/relative state when known;
- thrust/throttle/Flight Assist state;
- hull/critical subsystems;
- Shield;
- weapon/ammo/energy;
- Fuel and Reaction Propellant warnings;
- power/thermal/life-support warnings;
- docking/navigation cues.

It cannot imply acceleration/docking not present in authoritative physics/state.

## 29. Docking Presentation

Docking UI distinctly represents:

- approach/alignment;
- capture eligibility;
- physical capture;
- Hard Dock;
- service-link state.

Alignment animation never equals docking commit.

## 30. Station Critical HUD

While walking Horizon, ordinary global station telemetry remains suppressed unless requested through an appropriate screen/console.

Critical alarms may surface through a station emergency banner using known:

- category;
- location/scope;
- AlarmPriority;
- known instruction/response state.

## 31. Raid/Defense HUD

Raid/Defense overlays use only known state from TA-9 including:

- objective/phase;
- escalation;
- bridgehead/extraction;
- known reinforcement state;
- breach/approach points;
- known attackers;
- theft/cargo state;
- current known target-system/defense failure state.

Unknown reaction/ETA remains unknown.

## 32. Notification Feed

Routine committed presentation events can enter a small notification feed.

Baseline typed `NotificationPriority` values are:

- Critical;
- High;
- Normal;
- Informational.

`NotificationPriority` is a presentation namespace and is not `AlarmPriority`, mission threat, automation severity, or gameplay damage severity.

## 33. Notification Deduplication

Repeated equivalent routine notifications aggregate by an explicit semantic grouping/correlation key.

Presentation never guesses that unrelated events share one cause merely because they occur close in time.

Example: three Work Order completion events can aggregate when the event type explicitly permits it.

## 34. Alarm vs Notification

`AlarmPriority::P0–P3` remains the GDS alarm namespace.

An active alarm is tied to an underlying incident and can remain active/acknowledged/cleared independently from a transient notification banner.

Critical notifications do not automatically create P0 alarms.

## 35. Notification Lifetime

Routine notification duration is presentation-time configuration and accessibility adjustable.

Expiration removes only the banner/feed entry, never the underlying gameplay state or historical log where that domain retains one.

## 36. Screenshot/HUD Off

HUD hide can suppress optional layers.

Mandatory critical alerts remain unless the user explicitly enables full HUD-off after a high-risk warning.

Full HUD-off still cannot change gameplay state or disable underlying incidents.

## 37. Reduced Effects/Photosensitivity

HUD feedback has alternate paths that avoid:

- rapid flashing;
- high-frequency pulsing;
- aggressive full-screen overlays;
- excessive screen-edge effects.

Icon/text/steady-state cues preserve semantics.

## 38. True Pause

When True Paused:

- HUD presents the latest stable known gameplay state;
- gameplay timers/progress do not visually advance as if Simulation Time were advancing;
- menu/UI hover/fade effects may continue with Presentation Time;
- no new gameplay PresentationEvent is synthesized without a committed source.

## 39. Presentation Event Ordering

For one stable simulation boundary, presentation events are ordered by:

1. committed simulation tick;
2. deterministic domain/event ordering;
3. event sequence/correlation.

Render frame timing cannot reverse causally ordered feedback.

## 40. Stale Events

Presentation events can carry SceneGeneration/RuntimeEntity generation/source revision where relevant.

Events targeting a destroyed/replaced scene object are discarded or converted to a stable persistent-identity summary if their contract explicitly supports that.

## 41. Debugging

Development HUD diagnostics expose:

- active HudReadModel generation;
- marker eligibility/precision/knowledge state;
- notification aggregation keys;
- alarm source mapping;
- interaction focus source;
- presentation event queue;
- stale event drops;
- effective HUD preset/settings.

Privileged hidden-world overlays remain separate debug tools.

## 42. Validation Requirements

Tests must verify:

- hidden objective/hostile never gains a marker;
- SearchArea intel cannot render an Exact marker;
- through-wall eligibility requires explicit knowledge source;
- success feedback follows commit;
- Field-Unsecured state remains correct through pickup/extraction;
- docking stages are not collapsed;
- one event does not duplicate across render frames;
- notification deduplication does not merge unrelated incidents;
- HUD presets never change gameplay state;
- True Pause does not visually advance Simulation-Time progress.

## 43. Explicit Non-Goals

No baseline requirement for:

- omniscient minimap/radar;
- mandatory enemy health bars;
- floating damage/XP numbers;
- generic XP bar;
- permanent Credits display;
- UI-side interaction targeting;
- UI-side objective progression;
- success-before-commit feedback.

## 44. Dependencies

Depends on TA-2 Read Models/Knowledge Filter/events, TA-3 scene generations, TA-4 camera/UI rendering, TA-5 perception/physical facts, TA-7 runtime state, TA-9 missions/events, TA-11 input/UI framework, and GDS HUD/Interaction Feedback/Alarms/Accessibility.

## 45. Open Questions

None in the HUD/marker/notification/interaction presentation architecture.
