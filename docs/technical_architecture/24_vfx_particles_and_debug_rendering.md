# VFX, Particles, and Debug Rendering

> **Status:** Architecture Complete  
> **Authority:** Renderer-side VFX representation, particle simulation boundary, effect priority/culling, decals, screen-space effects, accessibility reductions, and development debug rendering

## 1. Purpose

VFX communicate authoritative gameplay/system events but never own those events.

## 2. VFX Input

The renderer receives immutable effect intents/state from presentation-facing read models/events.

Examples:

- shield impact/break;
- muzzle flash;
- projectile trail;
- explosion;
- fire/smoke;
- pressure vent;
- electrical arc;
- corrosion;
- thruster plume;
- scanner visualization;
- anomaly distortion;
- docking capture cue.

## 3. Gameplay Authority Boundary

Particles/effects never:

- apply damage;
- ignite targets;
- move physics bodies;
- mark scanner discovery;
- complete objectives;
- create pressure loss;
- decide projectile collision.

They visualize already-authoritative state/events.

## 4. Effect Instance Identity

Short-lived renderer-only effects use process-local `EffectInstanceHandle`/generation semantics and are not persistent gameplay IDs.

Persistent world damage presentation is reconstructed from owning persistent state rather than serialized GPU particle state.

## 5. CPU vs GPU Effect State

Baseline split:

- effect lifecycle/semantic intent and spawn parameters are prepared on CPU/main thread;
- high-count particle integration can execute on GPU compute;
- gameplay-critical projectile/hazard state remains in simulation.

## 6. GPU Particle System

Baseline particle renderer supports GPU-compute update for high-count presentation particles.

Particle data can include:

- position/velocity in TA-3 runtime frame;
- age/lifetime;
- size/rotation;
- color/opacity;
- sprite/frame index;
- effect flags.

GPU particle results are non-authoritative and need not serialize.

## 7. Simulation Time and Particles

World VFX tied to gameplay advance use Simulation-Time delta supplied by the presentation frame.

During True Pause or Hard Streaming Hold they stop evolving unless the effect is explicitly a UI/loading presentation effect driven by real presentation time.

## 8. Spawn Determinism Boundary

Gameplay consequence does not depend on exact particle count, particle RNG, or GPU execution.

Visual particle randomness can be renderer-local/nonpersistent.

Where visual continuity is important after load, the renderer reconstructs from persistent semantic state rather than requiring identical old particle trajectories.

## 9. Effect Priority

Canonical renderer priority mirrors GDS-13:

1. lethal hazard telegraph;
2. active/incoming combat threat;
3. interaction/objective feedback;
4. system-state effect;
5. decorative ambience.

Budget reduction removes/reduces lower priorities first.

## 10. Critical Representation Floor

Every gameplay-critical effect family defines a minimum representation that remains under:

- Low graphics preset;
- Reduced Effects;
- Photosensitivity Safe Mode;
- particle-budget pressure.

The minimum representation can use simpler geometry/icon/steady cue rather than full particles.

## 11. Reduced Effects

Reduced Effects can lower:

- spawn rate;
- particle count;
- smoke density;
- debris count;
- sparks;
- distortion;
- bloom contribution;
- decal/transient lifetime.

It cannot remove the final readable cue of a known gameplay-critical state.

## 12. Photosensitivity Safe Mode

Photosensitivity filtering applies before final effect submission.

It can replace:

- rapid flashes with steady/slow pulses;
- repeated high-contrast emissions with bounded intensity;
- full-screen flicker with localized/static cues.

The renderer receives/derives a safe variant rather than simply dropping the event.

## 13. Explosion VFX

Explosion presentation receives authoritative origin, scale/payload class, environment context, and semantic tags.

Vacuum and atmospheric variants are distinct.

Particle visuals do not define blast radius; combat already resolved it.

## 14. Fire and Smoke

Persistent fire/smoke systems are driven by actual environmental/fire state.

A rendered flame cannot persist after the authoritative fire state is gone except for a short authored fade-out clearly representing dissipation rather than ongoing hazard.

## 15. Vacuum Effects

Vacuum VFX avoid ordinary long-lived atmospheric smoke/fireball behavior unless the effect payload explicitly contains material/oxidizer.

## 16. Pressure / Leak VFX

Pressure-loss presentation is anchored to actual breach/leak locations and pressure-flow state supplied by station/world systems.

The renderer cannot invent a leak path.

## 17. Thrusters

Spacecraft/robot thruster effects receive authoritative active-thruster state and thrust direction.

Disabled thrusters do not visually fire.

## 18. Projectile Trails

A gameplay projectile can expose a render trail without the trail being the projectile collider.

Hitscan weapons can spawn short presentation traces/impact effects without simulating a slow visual bullet.

## 19. Shield Effects

Shield effects support:

- localized impact;
- surface reveal;
- break/depletion;
- recharge/return;
- projector geometry.

Shield state comes from gameplay read models/events.

## 20. Scanner Effects

Scanner visualization is built only from already-eligible known/detected scanner result data.

No GPU depth/material analysis is allowed to reveal hidden gameplay entities outside the scanner/knowledge contract.

## 21. Decals

Two decal classes:

- Persistent/Semantic Decal — reconstructed from authored/persistent damage/world state;
- Transient Presentation Decal — renderer lifetime only.

Examples:

- persistent breach scorch;
- authored signage;
- temporary bullet impact mark.

## 22. Decal Lifetime

Transient decal lifetime is graphics/performance data.

Removing a transient decal cannot repair or erase the associated gameplay damage.

## 23. Screen-Space Distortion

Distortion is restricted to explicit effect families such as:

- shield impact;
- heat haze;
- anomaly;
- selected glass/refraction.

It is applied before tone mapping where HDR scene sampling is required and respects Reduced Effects/Photosensitivity settings.

## 24. Full-Screen Overlays

Full-screen damage/status overlays are minimized.

When used, they are bounded in opacity/duration and independently reducible under GDS-13 accessibility rules.

## 25. World Highlights

Interaction/objective/accessibility highlights are renderer overlays driven by knowledge-filtered IDs/geometry.

They support Subtle/Standard/Strong presentation parameters without expanding interaction range or discovering hidden objects.

## 26. Outline Technique

Baseline world highlighting uses a mask/ID or geometry-based edge/composite technique selected by the renderer.

The architecture requires:

- depth/occlusion respect;
- knowledge-safe input;
- bounded thickness/intensity;
- native scene alignment.

Exact kernel is implementation-lock data.

## 27. Effect Resource Sharing

Effect definitions reference shared:

- meshes;
- sprites/textures;
- materials;
- curves/parameter presets;
- audio semantic linkage outside renderer.

They do not allocate unique shader programs/textures per effect instance.

## 28. GPU Capacity

GPU particle buffers use bounded capacities.

If capacity pressure occurs:

1. preserve critical effects;
2. reduce/drop decorative spawns;
3. log counters in development;
4. never block gameplay simulation waiting for cosmetic particle capacity.

## 29. Particle Collision

GPU presentation particles do not perform authoritative collision.

Optional visual collision against scene depth can improve appearance but cannot produce gameplay consequences.

## 30. Lighting From VFX

Selected effect families can submit transient render lights.

Examples:

- explosion flash;
- plasma bolt glow;
- welding arc.

These are presentation lights and follow light/shadow budgets.

Their absence/reduction under quality settings cannot alter AI visibility or gameplay detection unless a separate gameplay light/sensor contract owns that effect.

## 31. Debug Rendering

Development debug rendering is a separate presentation layer.

Supported primitives include:

- lines;
- rays;
- points;
- AABBs/OBBs;
- spheres/capsules;
- frusta;
- text labels;
- graph/node visualization where useful.

## 32. Debug Sources

Gameplay/physics/navigation/station systems can submit immutable debug-draw descriptors through development-only interfaces.

They do not expose writable renderer pointers.

## 33. Debug Lifetime

Debug primitives can be:

- one frame;
- N presentation seconds;
- persistent until cleared by debug tooling.

They are nonshipping/nonpersistent gameplay state.

## 34. Debug Safety

Debug overlays can reveal hidden information only in development builds/tools.

Shipping builds must not accidentally expose developer omniscient overlays through player accessibility/settings.

## 35. GPU Profiling Markers

Every significant render pass/effect compute stage emits debug/profiling groups/markers in development where supported.

This enables RenderDoc/driver/debug tooling without changing shipping semantics.

## 36. Test Requirements

Validation must cover:

- VFX cannot mutate gameplay;
- paused Simulation-Time effect freeze;
- Reduced Effects preserves critical cue;
- Photosensitivity mode substitutes unsafe flash patterns;
- scanner/highlight does not receive hidden entities;
- particle-capacity overflow prioritizes critical effects;
- transient decal removal does not change persistent damage;
- debug omniscience unavailable in shipping configuration.

## 37. Explicit Non-Goals

No baseline requirement for:

- gameplay-authoritative GPU particles;
- serializing individual cosmetic particles;
- universal volumetric fluid simulation;
- full-screen strobe effects;
- infinite particle buffers;
- debug rendering as shipping HUD.

## 38. Dependencies

Depends on GDS-13 VFX/Accessibility, TA-4 render graph/materials/visibility, TA-3 environment/scene state, and later TA-10 effect assets/TA-11 UI-audio integration.

## 39. Open Questions

None in the VFX/particle/debug-render architecture.
