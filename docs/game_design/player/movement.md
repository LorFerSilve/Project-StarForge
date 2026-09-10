# Player Movement

> **Status:** Design Complete  
> **Authority:** On-foot locomotion states, gravity movement, jumping, sprinting, crouching, falling, mantling, ladders/shafts, zero-gravity/EVA locomotion, load restrictions, and movement failure states

## 1. Purpose

Movement is responsive and readable while supporting station interiors, planetary terrain, boarding, EVA, hazards, and combat without introducing a generic traversal-stamina loop.

## 2. Canonical Locomotion States

The player occupies exactly one primary locomotion state from the applicable set:

- Grounded Standing;
- Walking;
- Sprinting;
- Crouched;
- Airborne;
- Mantling;
- Ladder / Shaft Traversal;
- Zero-G Free Movement;
- Incapacitated.

Independent modifiers such as Heavy Load, environmental exposure, or explicit injury/status effects may further constrain a state.

## 3. Walking

Walking is the default grounded locomotion with forward/backward movement and strafing.

Direction is camera-relative on ordinary walkable surfaces.

Diagonal input is normalized so combining axes does not increase maximum ground speed.

## 4. Sprinting

Sprint increases grounded movement speed.

Sprint is valid only when:

- the player is in a sprint-compatible grounded state;
- the player is not Crouched;
- Heavy Load or another explicit restriction has not disabled sprint;
- Player Health has not disabled sprint;
- required traversal clearance remains valid.

Ordinary sprint has no stamina cost and no maximum duration.

## 5. Crouching

Crouch reduces collision height and movement speed.

The player cannot stand until the standing collision volume is clear.

Entering/exiting crouch uses a short tuneable transition but collision validity is authoritative throughout.

## 6. Jump

Jump is allowed while grounded when no explicit movement restriction blocks it.

Jump consumes no stamina.

Heavy Load and Critical Player Health disable normal jumping.

## 7. Air Control

Airborne directional control exists for responsiveness and is weaker than grounded acceleration/control.

Air control cannot reverse momentum as strongly as grounded movement.

Exact coefficients are tuneable.

## 8. Falling and Impact

Landing impact severity derives from relative landing velocity and local gravity.

Movement reports the impact to Player Health/Combat after applicable movement/equipment protection has been evaluated.

## 9. Fall Protection

An equipment definition may explicitly provide fall-protection capability through a documented safe-impact threshold/modifier.

No fall protection exists merely because equipment is high tier.

## 10. Mantling

The player can mantle authored/reachable ledges when all conditions are valid:

- ledge height is within the configured mantle range;
- the approach surface/ledge is mantle-compatible;
- destination collision volume is clear;
- the movement path is unobstructed;
- the player state permits mantling;
- Heavy Load and Critical Player Health are not blocking it.

Mantling is not universal climbing.

## 11. Ladders and Maintenance Shafts

Compatible ladders/shafts use an explicit traversal state with authored entry/exit points.

The player cannot enter that state through arbitrary nearby geometry.

Losing a valid traversal path transitions to Airborne/Zero-G according to local gravity.

## 12. Elevators

Elevators physically move the player through the current station/ship environment.

Baseline elevators are **not fast-travel teleporters**.

If a future fast-travel mechanic is added, it requires a separate accepted design change and does not retroactively redefine elevator movement.

## 13. Doors and Dynamic Geometry

Closed/blocked solid doors prevent traversal.

Movement follows current authoritative collision state and never assumes a visual door opening before collision permits passage.

## 14. Moving Platforms

The player inherits enough supporting-platform motion/velocity to remain stable during ordinary platform movement.

Leaving the platform preserves physically appropriate inherited velocity according to the movement model.

## 15. Gravity Contexts

Playable environments expose one of:

- Standard / Local Gravity;
- Reduced Gravity;
- Zero Gravity.

Exact acceleration is location data.

Gravity changes jump arcs, fall velocity, grounded availability, and locomotion response rather than changing player identity/stat progression.

## 16. Reduced Gravity

Reduced gravity keeps normal grounded locomotion on valid surfaces but changes jump/fall timing and traction response according to location parameters.

## 17. Zero Gravity

Without magnetic/contact support, grounded locomotion is unavailable.

Zero-G Free Movement uses compatible powered suit maneuvering thrusters.

Thrusters apply acceleration/deceleration rather than directly setting velocity.

## 18. Zero-G Stabilization Mode

A compatible powered EVA-thruster system includes a player-toggleable **Stabilization Mode**.

When enabled it applies counter-thrust toward zero relative velocity within the suit's actual thrust and energy limits.

It cannot stop the player instantly, operate without energy/functioning thrusters, or negate external forces beyond its capability.

## 19. EVA

EVA uses Zero-G or local low-gravity movement plus all GDS-5 Field Survival requirements.

Movement does not itself grant pressure, oxygen, thermal, or radiation protection.

## 20. Magnetic Boots

A compatible suit may include Magnetic Boots.

When enabled on a supported surface:

- the player is constrained to that surface;
- grounded movement rules apply in the local tangent frame;
- detaching returns the player to the appropriate gravity/Zero-G state.

Magnetic Boots require the equipment capability defined by the equipped suit and cannot attach to incompatible surfaces.

## 21. Carried Load

Inventory defines carried Mass and Heavy Load.

While Heavy Load:

- sprint is disabled;
- normal jump is disabled;
- mantle is disabled;
- walking/crouching remain available at reduced tuneable speed/acceleration.

The hard Mass/Volume capacities still prevent impossible pickup beyond absolute limits.

## 22. Equipment Movement Modifiers

Equipment may define explicit movement modifiers.

Only documented equipment properties affect movement; there are no hidden tier/rarity movement bonuses.

## 23. Player Health Restrictions

Player Health owns health-band restrictions.

The fixed baseline is:

- Healthy — no health-band movement restriction;
- Wounded — no inherent movement restriction;
- Critical — sprint, normal jump, and mantle disabled;
- Incapacitated — all direct locomotion disabled;
- Recovering — locomotion follows the current Health band plus any explicit injury/recovery status.

Independent combat/status/injury effects can impose additional documented restrictions.

## 24. Collision

The player uses a stable humanoid gameplay collision volume and cannot move through authoritative solid geometry.

Cosmetic appearance does not alter collision dimensions.

## 25. Step Handling

Small ledges within a tuneable step height are traversed automatically when destination clearance exists.

Larger obstacles require jump, mantle, ladder/shaft traversal, or another route.

## 26. Slopes

Surfaces at or below the configured walkable-angle limit can support grounded locomotion.

Steeper surfaces are non-walkable and transition the player to slide/fall behavior according to local gravity/collision.

## 27. Hazardous Surfaces

Movement reports physical contact with hazard surfaces.

The owning hazard/Combat/Field Survival system applies the consequence.

Movement never invents damage solely from a material label.

## 28. Swimming

Swimming is not part of the baseline.

Liquid mission spaces therefore require authored alternatives such as avoidance, suit traversal where explicitly supported, platforms, or vehicles; implementation must not invent a swimming controller.

## 29. Prone

Prone is not part of the baseline.

## 30. Dash / Dodge

No universal dash or dodge exists in the baseline.

## 31. Technical Unstuck

If a technical collision defect places the player in invalid geometry, Unstuck may return the character to the nearest recently valid safe position.

The recovery search cannot:

- cross a legitimate locked/security boundary;
- bypass mission gating;
- escape an authored hazard merely for advantage;
- duplicate/alter inventory.

Unstuck is technical recovery, not fast travel.

## 32. Save / Load

Persistent player location/orientation/current locomotion-compatible state is restored deterministically.

If version migration or invalid geometry makes the exact saved transform unusable, the safe-position recovery rule may relocate to the nearest valid position inside the same legitimate gameplay region.

Transient interpolation/animation phase may be reconstructed without changing authoritative position/state.

## 33. Camera Collision

First-person camera behavior cannot reveal gameplay-relevant visibility through opaque solid geometry.

Camera presentation may clip cosmetic held geometry where required, but world visibility/interaction/hit authority still respects solid boundaries.

## 34. Presentation

Movement feedback follows GDS-13 and communicates at minimum:

- current movement restriction when a requested action is blocked;
- Heavy Load restriction;
- Zero-G/Magnetic Boots state;
- mantle/ladder availability when relevant;
- environmental locomotion context.

Motion-reduction settings may reduce head bob/camera shake to zero without changing movement physics.

## 35. Edge Cases

- Attempting to stand below low clearance keeps the player Crouched.
- A mantle target becoming blocked before commit cancels the mantle into the physically valid fallback state.
- Suit energy reaching zero in Zero-G disables powered thrust/stabilization but preserves current momentum.
- Magnetic-boot surface destruction/detachment transitions immediately to the correct gravity state.
- Becoming Critical during sprint ends sprint and transitions to Walking at the next legal movement update.
- Becoming Incapacitated during ladder/mantle traversal ends direct control and hands position/rescue handling to Health/mission rules.

## 36. Tuneable Parameters

Tuneable values include:

- ground/crouch/heavy-load speeds;
- acceleration/deceleration;
- jump impulse;
- air control;
- gravity per environment;
- fall thresholds;
- mantle height/reach;
- step height;
- maximum walkable angle;
- Zero-G thrust/stabilization strength;
- crouch transition duration.

The available locomotion set and restriction relationships are fixed.

## 37. Explicit Non-Goals

The baseline does not include:

- stamina-limited ordinary sprint;
- prone;
- swimming;
- universal wall climbing;
- universal dash/dodge;
- elevator teleportation;
- camera peeking through walls.

## 38. Dependencies

Depends on Controls, Player Health, Equipment, Inventory, Field Survival, Combat, Spacecraft/Station geometry, World environments, and GDS-13 Presentation/Accessibility.

## 39. Open Questions

None.
