# Player Movement

> **Status:** Draft  
> **Authority:** On-foot locomotion states, gravity movement, jumping, sprinting, crouching, falling, mantling, ladders, zero-gravity/EVA locomotion boundary, and movement failure states

## 1. Purpose

Movement should feel responsive and readable while supporting interiors, planetary terrain, boarding, EVA, and station emergencies.

## 2. Standard Locomotion States

The player can be:

- Grounded Standing;
- Walking;
- Sprinting;
- Crouched;
- Airborne;
- Mantling;
- Ladder/Shaft Traversal;
- Zero-G Free Movement;
- Incapacitated.

## 3. Walking

Walking is default grounded movement with forward/back, strafing, and diagonal motion.

Direction is camera-relative on normal horizontal surfaces.

## 4. Sprinting

Sprint increases grounded movement speed.

Sprint requires valid grounded locomotion and no explicit movement restriction.

The baseline has **no general stamina bar** limiting ordinary sprint duration.

## 5. No Stamina Bar

Traversal challenge should come from environment, gear, combat, gravity, hazards, and terrain rather than periodically forcing basic movement to stop.

## 6. Crouching

Crouch reduces collision height and movement speed.

Standing is blocked if overhead clearance is insufficient.

## 7. Jump

The player can jump while grounded unless current state/equipment blocks it.

Jump does not consume stamina.

## 8. Air Control

Limited air control exists for responsiveness but is weaker than grounded directional control.

## 9. Falling

Impact severity derives from landing velocity and gravity context.

Health/combat integration owns damage values.

## 10. Fall Protection

Equipment may improve safe-fall thresholds through suit dampening or propulsion assist.

## 11. Mantling

The player can mantle valid reachable ledges within authored height/clearance limits.

Mantling is not universal wall climbing.

## 12. Mantle Validation

A mantle requires a reachable ledge, valid destination volume, valid surface, and an allowed player state.

## 13. Ladders and Shafts

Compatible ladders/maintenance shafts use an explicit traversal state with valid entry and exit points.

## 14. Elevators

Elevators move the physical player through station/ship space.

They are not teleporters unless a later fast-travel system explicitly defines one.

## 15. Doors

Closed doors block movement until actual collision/state permits traversal.

## 16. Moving Platforms

Player movement inherits valid moving-platform transform/velocity sufficiently to prevent normal platform motion from sliding the player incorrectly.

## 17. Gravity

Playable environments expose:

- Standard/Local Gravity;
- Reduced Gravity;
- Zero Gravity.

Exact acceleration is location data.

## 18. Reduced Gravity

Reduced gravity changes jump arc, fall velocity, airtime, and movement feel while preserving controlled grounded movement.

## 19. Zero Gravity

Without contact/magnetic support, grounded locomotion is unavailable.

Zero-g movement uses suit maneuvering thrusters when equipped and operational.

## 20. Zero-G Momentum

Thrusters accelerate and decelerate rather than setting velocity instantly.

A stabilization mode can counter drift when enabled.

## 21. EVA

EVA uses zero-g/low-gravity locomotion in vacuum/exterior environments and additionally requires field-survival protection.

## 22. Magnetic Boots

Compatible suits can use Magnetic Boots on supported surfaces.

When enabled, the player is constrained to the local surface and uses grounded movement rules.

## 23. Carried Load

Inventory can create a Heavy Load state when carried mass exceeds a soft-load threshold but remains below absolute capacity.

Heavy Load reduces sprint/jump capability.

## 24. Equipment Restrictions

Heavy equipment can define visible movement modifiers.

## 25. Health Restrictions

Health states may reduce speed or disable sprint/jump.

Health owns the restriction; Movement executes it.

## 26. Collision

The player uses a stable humanoid collision volume and cannot move through solid geometry.

## 27. Step Handling

Small height differences auto-step within a tuneable limit.

Larger obstacles require jump, mantle, or another route.

## 28. Slopes

Surfaces above the maximum walkable angle become non-walkable or cause sliding/fall.

## 29. Hazardous Surfaces

Movement detects hazard surfaces; damage/effect is owned by hazard/combat systems.

## 30. Swimming

Swimming is not part of the initial baseline.

## 31. Prone

Prone is not part of the baseline.

## 32. Dash/Dodge

No universal dash/dodge exists in the baseline.

## 33. Technical Unstuck

If a physics defect places the player in invalid geometry, technical recovery can restore the nearest recently valid safe location.

It cannot bypass legitimate locked/hazard boundaries for gameplay advantage.

## 34. Save/Load

Player location restores to a valid logical position.

Version migration can use nearby safe-position recovery where required.

## 35. Camera Collision

Camera behavior cannot permit gameplay-relevant peeking through opaque solid walls.

## 36. Tuneable Parameters

Tuneable values include speeds, acceleration, jump impulse, air control, gravity, fall thresholds, mantle height, step height, slope angle, zero-g thrust, and load penalties.

## 37. Explicit Non-Goals

Movement does not require stamina-limited sprint, prone, universal wall climbing, swimming, universal dash, or camera clipping through walls.

## 38. Dependencies

This specification depends on controls, health, equipment, inventory, field survival, combat, spacecraft/interiors, world environments, and presentation.

## 39. Open Questions

None in the baseline locomotion set.
