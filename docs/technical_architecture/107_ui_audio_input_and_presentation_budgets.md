# TA-13 — UI, Audio, Input, and Presentation Budgets

> **Status:** Architecture Complete  
> **Authority:** Numeric budgets for semantic input processing, retained UI/layout/text, markers/notifications, audio voices/streaming/callbacks, animation, haptics, presentation events, and accessibility-safe degradation

## 1. Purpose

TA-11 defines presentation authority boundaries. TA-13 ensures those systems cannot create unbounded CPU/memory/queue growth or steal enough main-thread/audio time to destabilize the fixed simulation.

## 2. Input Processing Budget

Fixed-tick semantic input routing targets:

- raw platform events consumed per rendered frame ordinary target: **<= 2,048**;
- fixed-tick action sampling/context routing CPU p95: **<= 0.25 ms**;
- registered semantic ActionIds baseline implementation capacity: **<= 512**;
- simultaneous active input contexts: **<= 16**.

Mouse/stick motion can coalesce to latest/accumulated semantic state where correct; discrete press/release edges cannot be silently lost.

## 3. Input Overflow Behavior

Platform/raw-event queue overflow is a severe diagnostic. The application may coalesce high-rate motion samples but cannot drop semantic button edges required for controls.

A device generating pathological noise is filtered by calibration/deadzone policy rather than consuming unbounded event memory.

## 4. Retained UI Tree Scale

One shipping UI composition targets:

- total live retained UI nodes: **<= 20,000**;
- simultaneously visible/layout-active nodes: **<= 8,000**;
- focusable nodes in one active navigation scope: **<= 1,024**.

Large inventory/crew/market lists must virtualize/recycle row widgets instead of materializing arbitrary history sizes.

## 5. UI CPU Budget

At 1080p High/reference UI scale:

- retained-state/update + layout p95: **<= 0.6 ms/frame**;
- text shaping/glyph lookup/upload preparation p95: **<= 0.4 ms/frame**;
- combined shipping UI CPU p95: **<= 1.0 ms/frame**;
- UI GPU composition target is TA-13 renderer's **<= 0.8 ms p95**.

Expensive screen-opening rebuilds may exceed steady-state budgets but are explicitly profiled.

## 6. Text/Glyph Budget

Shipping text system targets:

- glyph atlas GPU memory soft target: **64 MiB**;
- hard safety cap: **128 MiB**;
- shaped glyphs emitted in one frame ordinary target: **<= 50,000**;
- active atlas glyph entries ordinary target: **<= 16,384** across loaded font roles/sizes.

Unused glyph pages can be evicted/rebuilt as presentation cache. Text content/knowledge state is unaffected.

## 7. Marker Budget

Knowledge-filtered world/HUD markers target:

- simultaneously evaluated markers: **<= 512**;
- visible on-screen markers after policy/filtering: **<= 256**;
- high-priority objective/alarm markers are never dropped merely to satisfy decorative marker targets.

Off-screen/far marker aggregation is presentation-only and preserves knowledge precision.

## 8. Notification and Alarm Budget

Presentation storage targets:

- queued ordinary notifications: **<= 64** visible/recent active queue;
- active alarm presentation records: **<= 128**;
- presentation history/log can use bounded paging/storage independent of active queue.

Deduplication/aggregation can combine repeated identical presentation notices but cannot acknowledge/repair the owning gameplay incident.

## 9. Presentation Event Queue

Ordered committed `PresentationEvent` handoff has:

- ordinary queued-event soft target: **4,096**;
- hard safety cap: **16,384**;
- decorative repeated cues may coalesce by explicit cue semantics;
- critical committed feedback cannot be silently dropped.

If critical backlog approaches the hard cap, presentation enters a controlled degraded/flush state while gameplay authority remains intact.

## 10. Animation Budget

High-preset presentation target:

- locally animated/skinned actor instances: **<= 128 visible high-detail**;
- additional off-camera active actor animation can update at a lower presentation cadence only where it has no gameplay authority;
- animation graph/evaluation CPU p95: **<= 1.0 ms/frame** across the scene;
- root motion remains non-authoritative.

## 11. Camera Budget

Only the active shipping view and explicitly required auxiliary views execute full camera update/render behavior.

Ordinary gameplay target:

- full-resolution primary views: **1**;
- auxiliary render views (mirrors/preview/management) require explicit bounded feature policy and are not baseline free-cost features;
- camera presentation update CPU: **<= 0.2 ms/frame**.

## 12. VFX Handoff Budget

Semantic presentation intents to TA-4 VFX target:

- new VFX intents: **<= 2,048/frame** ordinary target;
- repeated decorative intents can coalesce by source/cue rules;
- gameplay projectiles/hazards never exist only as VFX events.

Particle counts/GPU budgets are owned by the TA-13 renderer spec.

## 13. Audio Voice Budget

Baseline audio mixer targets:

- logical active voices: **<= 128**;
- simultaneously physically mixed voices: **<= 96**;
- spatialized world voices: **<= 64**;
- concurrent streamed long-form voices: **<= 16**;
- reserved critical dialogue/alarm/UI channels: **8 voices** inside the mixed-voice capacity.

Low-priority decorative voices can virtualize/stop by presentation priority. Gameplay hearing does not depend on playback voices.

## 14. Audio Memory Budget

Targets:

- resident decoded/ready audio cache: **<= 256 MiB**;
- stream/decode ring buffers: **<= 64 MiB**;
- transient audio command/voice state: **<= 16 MiB**.

Large music/dialogue remains stream-oriented rather than full uncompressed residency.

## 15. Audio Callback Budget

For the backend-selected device buffer period:

- real-time callback p99 CPU time: **< 25% of buffer period**;
- warning threshold: **>= 35%**;
- hard reliability threshold: **50%** of buffer period.

The callback performs no allocation, filesystem I/O, gameplay query, Jolt query, UI work, or blocking lock acquisition.

## 16. Audio Decode Budget

Background audio decode uses worker jobs and targets:

- no more than **4 concurrent audio decode jobs**;
- maintain at least **500 ms** buffered audio ahead for ordinary streamed music/dialogue under normal conditions;
- underrun is a presentation failure and must not alter dialogue/story completion state.

## 17. Audio Occlusion Budget

Presentation occlusion/acoustic queries target:

- high-priority spatial emitters queried: **<= 32/frame**;
- lower-priority emitters can update occlusion on staggered presentation cadence;
- gameplay hearing uses its separate semantic AI pipeline and is unaffected by audio occlusion budget.

## 18. Adaptive Music Budget

Adaptive music reads knowledge-safe committed state at bounded presentation cadence, target **<= 10 Hz** high-level state reevaluation.

Music stems/transitions do not poll gameplay stores every audio sample or become mission authority.

## 19. Subtitle/Caption Budget

Presentation target:

- simultaneous subtitle speaker tracks: **<= 8**;
- simultaneous closed-caption cues after aggregation: **<= 16**;
- critical alarm captions have priority over decorative ambient captions but cannot reveal unknown events.

## 20. Haptics Budget

The haptic mixer targets:

- active semantic haptic cues: **<= 32**;
- queued cues: **<= 64**;
- mixer/update CPU: **<= 0.1 ms/frame**.

Decorative lower-priority cues can be pre-empted; haptics are never the only critical channel.

## 21. Accessibility Invariants Under Pressure

Performance pressure cannot disable:

- subtitles/captions selected by the user;
- High Contrast required cues;
- Photosensitivity Safe limits;
- Reduced Motion settings;
- required alarm redundancy;
- UI navigation/Back/Cancel.

Optional decorative detail degrades before accessibility guarantees.

## 22. Presentation Time Queues

Presentation timers/fades use bounded records and should expire/reclaim promptly. A hidden UI/audio/VFX object cannot accumulate forever solely because Simulation Time is paused.

Persistent gameplay incidents reconstruct presentation from Read Models rather than requiring indefinite presentation-event retention.

## 23. Save/Load Interaction

Loading does not replay old one-shot presentation events. Persistent alarms/tutorial state reconstructs from current owning state.

Profile-settings load is small/fail-soft and should not block campaign semantic validation.

## 24. Diagnostics

TA-11/13 overlays expose:

- raw/action input counts and routing time;
- UI live/visible node counts and layout time;
- glyph atlas bytes/hit/miss/upload;
- marker/notification/alarm counts;
- presentation event depth/age;
- animation count/time;
- audio logical/mixed/spatial/stream voice counts;
- callback utilization/underruns;
- decode queue/buffer-ahead;
- haptic queue/mix state.

## 25. Non-Goals

No gameplay simplification from presentation overload, no AI-hearing dependence on voice budget, no critical cue removal by graphics/audio quality, no unbounded UI history materialization.

## 26. Open Questions

None in the TA-13 UI/audio/input/presentation budget architecture.