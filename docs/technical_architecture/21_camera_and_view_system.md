# Camera and View System

> **Status:** Architecture Complete  
> **Authority:** Render-camera representation, perspective/projection conventions, first-person/spacecraft/cockpit views, interpolation, camera cuts, frusta, FOV, exposure inputs, and view-family lifetime

## 1. Purpose

The camera system converts authoritative player/ship/cinematic presentation state into renderer-owned views without becoming movement or gameplay authority.

## 2. Camera Ownership

Gameplay domains own:

- player/ship authoritative transform;
- allowed camera mode/state;
- gameplay aiming direction where authoritative;
- camera-relevant status events.

The renderer owns:

- interpolated presentation pose;
- view/projection matrices;
- jitter-free frustum;
- exposure state;
- previous-frame matrices;
- render-only camera effects.

## 3. ViewDescriptor

Each rendered view uses an immutable `ViewDescriptor` containing at minimum:

- view kind;
- camera position/orientation in current TA-3 runtime frame;
- vertical FOV;
- aspect ratio;
- near plane;
- projection type;
- viewport/scissor;
- exposure profile;
- render-layer mask;
- SceneGeneration;
- OriginEpoch;
- camera-cut/discontinuity flag.

## 4. Baseline View Kinds

- FirstPerson;
- SpacecraftChase;
- Cockpit;
- Management/Strategic;
- Debug.

Cinematic scripted cameras are not required as a distinct runtime architecture; authored sequences can provide ViewDescriptors through the same interface.

## 5. First-Person Camera

FirstPerson view uses the player's presentation camera anchor plus bounded visual offsets such as:

- head bob;
- camera shake;
- damage response;
- cosmetic weapon sway relationship.

These offsets never alter the authoritative player collision/body transform.

## 6. First-Person FOV

Default vertical FOV is 75 degrees.

Supported user range target is 60-100 degrees as required by GDS-13 Accessibility.

The renderer validates projection parameters after aspect/framebuffer changes.

## 7. First-Person Near Plane

Baseline FirstPerson near plane target is 0.05 m.

It is a render setting, not a collision distance.

Weapon/viewmodel handling must not rely on changing world gameplay geometry merely to avoid near clipping.

## 8. Spacecraft Chase Camera

SpacecraftChase view follows the persistent ship's interpolated render pose using a gameplay/presentation-provided chase anchor and obstruction solution.

The camera must preserve readability of:

- ship silhouette;
- velocity vector versus orientation;
- nearby collision risk;
- target direction;
- weapon feedback.

TA-5 owns physical obstruction queries/camera collision integration.

## 9. Cockpit View

Cockpit is an optional alternate ship view using the same authoritative ship state and gameplay information as third-person flight.

Required HUD/state cannot disappear merely because it is not represented on cockpit geometry.

Cockpit meshes render through normal scene/material rules with camera-specific visibility flags where necessary.

## 10. Strategic / Management View

Management/strategic views are presentation cameras over already-known state.

They do not create world knowledge or alter strategic simulation.

Orthographic projection may be used where a management interface requires it; normal local gameplay remains perspective.

## 11. Reversed-Z Projection

Perspective views use the TA-4 reversed-Z zero-to-one depth convention.

The projection builder is project-owned and centralized so all rendering, frustum extraction, screen reconstruction, and shader depth interpretation share the same convention.

## 12. Infinite Far Projection

Perspective gameplay views use an effectively infinite far plane where compatible with scene profile and reversed-Z projection.

Render visibility still applies scene/LOD/culling limits; infinite projection does not mean infinite world residency.

## 13. Frustum

The renderer derives a stable world/runtime-space frustum from the non-jittered projection/view state.

Frustum culling never uses post-process screen distortion or camera shake beyond the actual rendered view pose in a way that can incorrectly hide visible gameplay geometry.

## 14. Interpolation

Camera anchors interpolate using the same previous/current simulation sampling discipline as entity render transforms.

The renderer does not extrapolate camera authority beyond committed simulation state.

## 15. Camera Discontinuity

A hard camera cut/discontinuity is declared when:

- scene context changes;
- player/ship is authoritatively relocated discontinuously;
- cockpit/chase transition requires a hard cut;
- OriginEpoch cannot be reconciled continuously;
- save/load activates a new scene representation.

On discontinuity, renderer resets incompatible temporal state.

## 16. Camera Shake

Camera shake is a presentation offset generated from known events.

Accessibility multiplier supports 0-100%; at 0% the offset is exactly zero.

Authoritative recoil/spread remains unaffected.

## 17. Head Bob

Head-bob presentation is similarly reducible to 0%.

It does not alter authoritative player height/collision.

## 18. Weapon Viewmodel

First-person weapon/hands can use a dedicated presentation transform derived from camera + authoritative weapon state.

Baseline does not require a second independent projection/depth world.

If a dedicated viewmodel depth/projection path is later needed to eliminate clipping, it must preserve world muzzle/hit authority and requires an architecture amendment.

## 19. Aim Authority

The renderer can display crosshair/weapon alignment but never decides whether a shot hits.

GDS-9/TA-5 use physical camera-to-muzzle reconciliation and collision/query authority.

## 20. Camera Obstruction

Third-person camera obstruction prevention is a query-driven presentation behavior.

It can move the camera closer to its anchor to avoid geometry clipping.

It cannot move the ship/player or provide visibility through opaque geometry.

## 21. Exposure

Each view has renderer-owned exposure state.

Baseline uses controlled auto-exposure with:

- scene luminance sampling/histogram or equivalent;
- clamped exposure range per profile;
- adaptation speed;
- manual brightness/gamma calibration applied separately to output.

Hard camera cuts/scene transitions initialize exposure from profile defaults or validated prior state to avoid extreme flashes.

## 22. Exposure Gameplay Boundary

Auto-exposure must not make a mandatory object permanently invisible because a bright source dominates the frame.

Scene lighting/content and accessibility visibility requirements remain higher-level constraints.

## 23. Multiple Render Views

Baseline shipping gameplay requires one primary 3D view.

Additional render views are allowed only for explicit features such as:

- shadow maps;
- reflection probes/capture;
- debug inspection.

Split-screen/local multiplayer is not baseline.

## 24. UI Coordinate Separation

Shipping UI uses output pixel/logical UI coordinates after world rendering.

Changing world FOV/aspect does not scale UI layout.

TA-11 owns UI coordinate/layout policy.

## 25. Resize

Framebuffer resize updates:

- viewport;
- aspect;
- projection;
- render targets.

It does not modify authoritative camera orientation or gameplay FOV setting.

## 26. Origin Rebase

Camera position is transformed into the new runtime frame during TA-3 origin rebase.

View continuity is preserved where possible; otherwise the frame is marked discontinuous and temporal render state resets.

## 27. Scene Profiles

SceneProfile can supply defaults for:

- exposure range;
- fog/atmospheric rendering parameters;
- sky/background source;
- far-visibility policy;
- primary directional light.

User accessibility/settings remain able to alter allowed presentation properties without changing world state.

## 28. Debug Camera

Development builds may provide a detached debug/free camera.

It is strictly a development tool and cannot commit player movement, discovery, interaction, combat, or mission state.

Shipping gameplay does not expose it.

## 29. Test Requirements

Tests/validation must cover:

- FOV bounds and aspect changes;
- reversed-Z matrix reconstruction;
- camera discontinuity reset;
- OriginEpoch rebase continuity;
- Camera Shake/Head Bob at 0%;
- debug camera cannot mutate gameplay;
- first-person camera transform cannot change collision/body state;
- cockpit/chase share identical gameplay state.

## 30. Explicit Non-Goals

No baseline requirement for:

- split screen;
- VR stereo views;
- renderer-authoritative aiming;
- unrestricted cinematic camera system;
- camera movement as gameplay teleportation;
- mandatory secondary viewmodel world.

## 31. Dependencies

Depends on TA-3 coordinates/scene profiles, TA-4 frame pipeline, GDS-5 camera/control requirements, GDS-6 spacecraft views, GDS-13 Visual/Accessibility, and later TA-5 camera obstruction queries/TA-11 UI.

## 32. Open Questions

None in the camera/view architecture.
