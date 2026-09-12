from pathlib import Path


def replace_required(path: str, old: str, new: str, label: str) -> None:
    p = Path(path)
    text = p.read_text(encoding="utf-8")
    if old not in text:
        raise SystemExit(f"missing expected text for {label} in {path}")
    p.write_text(text.replace(old, new, 1), encoding="utf-8")


# Architecture Decisions: append TA-11 decisions exactly once.
p = Path("docs/technical_architecture/ARCHITECTURE_DECISIONS.md")
text = p.read_text(encoding="utf-8")
if "## AD-088 —" not in text:
    text += r'''

---

## AD-088 — Player Input Is Sampled as Semantic Actions at the Fixed Simulation Boundary

**Status:** Accepted

### Decision

Raw platform input is converted to typed `ActionId` state and sampled at TA-1's fixed-tick Input Command Sampling phase. A low-level edge latch may preserve a short physical press until the next eligible tick, but it never retries a rejected gameplay action or becomes a general input-intent buffer.

### Rationale

This prevents render-frame timing from losing or multiplying button presses while preserving the GDS rule that invalid actions are not queued until they later become legal.

---

## AD-089 — Input Context Ownership Is Explicit and Priority-Consumed

**Status:** Accepted

### Decision

StarForge maintains an explicit InputContextStack in which modal/system UI, active UI/terminal, specialized gameplay and ordinary gameplay have deterministic ownership priority. A higher-priority context consumes conflicting input before lower contexts can observe it.

### Rationale

UI focus must never accidentally fire a weapon, issue a robot command or move the player through the same physical input.

---

## AD-090 — Shipping UI Is a StarForge-Owned Retained Interface; Dear ImGui Remains Development-Only

**Status:** Accepted

### Decision

Player-facing HUD, menus and management screens use a purpose-built retained StarForge UI tree rendered through TA-4. Dear ImGui remains exclusively a development/debug inspection surface.

### Rationale

The GDS requires controller navigation, accessibility, scalable layouts, knowledge filtering and stable screen semantics that should remain under project ownership rather than a debug UI framework.

---

## AD-091 — Shipping Text Uses HarfBuzz Shaping and FreeType Font Rasterization Behind Project Boundaries

**Status:** Accepted

### Decision

UTF-8 shipping text is shaped/positioned through HarfBuzz and scalable font faces/glyphs are accessed/rasterized through FreeType. StarForge owns text roles, line layout, wrapping, glyph-atlas lifetime and all UI semantics.

### Rationale

Correct Unicode shaping and robust scalable glyph rendering are commodity problems; screen logic, gameplay identity and accessibility layout remain project authority.

---

## AD-092 — UI Uses Knowledge-Filtered Read Models and Typed Commands; Preview Is Never Authority

**Status:** Accepted

### Decision

Shipping screens consume immutable revisioned Read Models and issue typed Commands/requests against stable IDs. Proposed construction, trade, route, loadout, squad and other plans remain transient preview state until the owning gameplay domain validates and commits them.

### Rationale

This prevents UI business-rule duplication, stale-row targeting, hidden-information leakage and half-applied gameplay changes.

---

## AD-093 — HUD Markers and Feedback Preserve Knowledge Precision and Commit Timing

**Status:** Accepted

### Decision

Markers retain authored knowledge/precision states such as Exact, SearchArea, DirectionOnly, Suspected, Confirmed and Stale. Interaction/reward/objective/docking/save success presentation is emitted only from authoritative committed state/results.

### Rationale

Presentation must never turn uncertain intel into exact waypoints or show success before the state it represents actually exists.

---

## AD-094 — miniaudio Is a Presentation Backend and Its Real-Time Callback Has No Gameplay Authority

**Status:** Accepted

### Decision

miniaudio is encapsulated behind the StarForge audio module. Its real-time callback consumes prepared backend-safe audio state and cannot mutate gameplay, query DomainStores/physics, issue gameplay Commands, or decide story/mission progression.

### Rationale

Audio device timing and callback constraints must remain isolated from authoritative deterministic simulation.

---

## AD-095 — Gameplay Hearing and Player Audio Are Separate Semantic Pipelines

**Status:** Accepted

### Decision

AI consumes project-owned `GameplaySoundEvent` semantics while player playback consumes knowledge/medium-filtered `PresentationAudioEvent` state. User mix, mute, dynamic range, headphones and audio-device behavior never alter AI hearing.

### Rationale

Accessibility and user playback configuration must not change stealth/gameplay, while legitimate audible events can still become player knowledge through the owning perception path.

---

## AD-096 — Player Audio Obeys Medium and Knowledge; Third-Person Ship Audio Uses Pilot Telemetry

**Status:** Accepted

### Decision

Open vacuum carries no ordinary distant atmospheric audio. Valid suit/internal, physical-conduction, radio and sensor/telemetry paths remain audible. Third-person spacecraft uses the GDS Pilot Telemetry Mix, which may synthesize only legitimately detected/known ship events.

### Rationale

This preserves physical/informational causality without making an external chase camera literally hear through vacuum.

---

## AD-097 — Alarm and Notification Priority Namespaces Remain Typed and Alarm Acknowledgement Is Presentation-Only

**Status:** Accepted

### Decision

`AlarmPriority::P0–P3` remains distinct from `NotificationPriority`, `PowerLoadPriority`, `AutomationReportSeverity` and other severity namespaces. Acknowledging an alarm records presentation acknowledgement only and never repairs, clears or de-escalates the owning incident by itself.

### Rationale

Similar labels must not collapse unrelated semantics, and player acknowledgement must never become hidden gameplay mutation.

---

## AD-098 — Accessibility Transforms Control and Presentation Without Rewriting Gameplay Rules

**Status:** Accepted

### Decision

Remapping, Hold/Toggle, Auto-Sprint, UI scale, High Contrast, subtitles/captions, Reduced Motion, Photosensitivity Safe, Reduced Effects, haptics and bounded Aim Assist operate within explicit input/presentation boundaries. Aim Assist has no bullet magnetism or hidden targeting, and `Pause While Viewing` invokes global True Pause only.

### Rationale

Accessibility must improve perceivability/control while preserving rewards, ownership, world knowledge, hit resolution and Simulation-Time semantics.

---

## AD-099 — Animation, Camera, VFX and Viewmodels Are Presentation Projections, Not Gameplay Authority

**Status:** Accepted

### Decision

Baseline character locomotion animation is in-place; there is no generic root-motion authority. Animation notifies, particles, decals, camera effects and first-person viewmodels can synchronize presentation but cannot deal damage, transfer ammo/resources, complete objectives or own physical items.

### Rationale

Presentation assets can fail, reload or be reduced for accessibility without changing authoritative movement, combat, ownership or persistence.

---

## AD-100 — Presentation Consumes Stable State and Committed Events on Its Own Clock Without Advancing Gameplay

**Status:** Accepted

### Decision

Persistent presentation reconstructs from immutable Read Models; one-shot feedback consumes ordered committed PresentationEvents exactly once. Presentation Time may drive fades, UI motion, audio envelopes and cosmetic blends, while all gameplay progress remains Simulation-Time/fixed-tick authority. Scene/Entity/Origin/Content/ReadModel generations reject stale results.

### Rationale

Variable render/audio rates and asynchronous presentation work must affect smoothness and latency only, never gameplay outcomes or causality.
'''
    p.write_text(text, encoding="utf-8")


# Toolchain additions.
p = Path("docs/technical_architecture/03_toolchain_and_dependencies.md")
text = p.read_text(encoding="utf-8")
if "## 12.1 FreeType" not in text:
    needle = """## 12. miniaudio

**miniaudio** supplies the audio device/backend and decoding/mixing facilities used by the StarForge audio layer.

Gameplay code emits semantic audio intents/events rather than calling miniaudio directly.
"""
    addition = needle + """
The real-time backend callback consumes only prepared presentation state and never owns gameplay mutation, AI hearing, mission progression, or persistent identity.

## 12.1 FreeType

**FreeType** supplies scalable font-face access and glyph rasterization behind the StarForge shipping text layer.

FreeType does not own UI layout, text semantics, gameplay identity, focus/navigation, or glyph-atlas lifetime. Those remain StarForge-owned.

## 12.2 HarfBuzz

**HarfBuzz** supplies Unicode text shaping and glyph positioning behind the StarForge shipping text layer.

StarForge owns UTF-8 string/content selection, line breaking/wrapping, UI layout, accessibility scaling, font-role selection, and rendering resource lifetime.

## 12.3 Windows Haptics Backend

The Windows x64 baseline exposes haptics through a StarForge-owned `IHapticsBackend`. Windows XInput vibration is an acceptable system/backend path for compatible controllers; it does not replace GLFW input discovery/action routing and never becomes gameplay authority.
"""
    if needle not in text:
        raise SystemExit("miniaudio toolchain anchor missing")
    text = text.replace(needle, addition, 1)
    p.write_text(text, encoding="utf-8")


# Technical architecture README current phase.
replace_required(
    "docs/technical_architecture/README.md",
    "- **TA-10 — Content and Asset Pipeline**.\n\nThe next dependency is **TA-11 — Input, UI, Audio, and Presentation Integration**.",
    "- **TA-10 — Content and Asset Pipeline**;\n- **TA-11 — Input, UI, Audio, and Presentation Integration**.\n\nThe next dependency is **TA-12 — Persistence Implementation Architecture**.",
    "technical README current phase",
)

p = Path("docs/technical_architecture/README.md")
text = p.read_text(encoding="utf-8")
if "- FreeType + HarfBuzz for shipping text shaping/rasterization;" not in text:
    text = text.replace(
        "- miniaudio behind a StarForge audio layer;\n",
        "- miniaudio behind a StarForge audio layer;\n- FreeType + HarfBuzz for shipping text shaping/rasterization;\n",
        1,
    )
if "### TA-11 Input, UI, Audio, and Presentation" not in text:
    anchor = "\n## Architecture Documents\n"
    block = """
### TA-11 Input, UI, Audio, and Presentation

TA-11 establishes:

- GLFW raw-device ingestion mapped to typed semantic `ActionId` samples at the fixed simulation boundary, with a one-tick edge latch that is explicitly not a gameplay input buffer;
- deterministic InputContextStack priority/consumption, device switching, controller calibration, Hold/Toggle, sustained-interaction support, Auto-Sprint and bounded knowledge-safe Aim Assist;
- fully remappable keyboard/mouse/controller profiles, conflict validation, effective prompt glyph resolution, settings snapshots and optional haptics behind a StarForge `IHapticsBackend`;
- a purpose-built retained shipping UI tree with one focus owner, keyboard/controller/pointer navigation, safe Back/Cancel, preview-versus-commit flows, responsive UI scaling/safe area and TA-4 native-resolution rendering;
- UTF-8 text shaping with HarfBuzz plus FreeType glyph/font rasterization behind StarForge-owned text layout and renderer-owned glyph atlases;
- knowledge-filtered HUD, interaction prompts, marker precision, notifications, station/ship/mission/raid presentation, and commit-synchronized success feedback;
- management/planning/tutorial/system screens built from immutable composite Read Models and typed Commands, with no UI-side business-rule or partial-pause authority;
- miniaudio behind a project audio adapter with isolated real-time callback, generation-checked voices, semantic buses, resident/streamed clip classes, device-failure degradation and explicit audio lifetime;
- separate GameplaySoundEvent and PresentationAudioEvent pipelines so user mix never alters AI hearing;
- atmosphere/vacuum/conduction/radio/Pilot Telemetry audio semantics, knowledge-safe occlusion, dialogue/radio, and non-omniscient adaptive music;
- subtitles/Closed Captions, typed `AlarmPriority::P0–P3`, acknowledgement/escalation/deduplication, High Contrast, Reduced Motion/Effects, Photosensitivity Safe and critical multi-channel redundancy;
- animation/camera/VFX/viewmodel projection where notifies, particles and camera effects never become movement/combat/resource/gameplay authority;
- one explicit runtime handoff from fixed-tick commits to immutable Read Models + ordered exactly-once PresentationEvents, with Simulation Time separated from Presentation Time and stale generation/revision rejection.

"""
    if anchor not in text:
        raise SystemExit("technical README architecture-doc anchor missing")
    text = text.replace(anchor, "\n" + block + "## Architecture Documents\n", 1)
if "### TA-11\n\n- [`80_input_device_action_and_context_routing.md`]" not in text:
    docs_block = """### TA-11

- [`80_input_device_action_and_context_routing.md`](80_input_device_action_and_context_routing.md)
- [`81_input_bindings_remapping_settings_and_haptics.md`](81_input_bindings_remapping_settings_and_haptics.md)
- [`82_shipping_ui_framework_text_layout_and_focus.md`](82_shipping_ui_framework_text_layout_and_focus.md)
- [`83_hud_markers_notifications_and_interaction_presentation.md`](83_hud_markers_notifications_and_interaction_presentation.md)
- [`84_management_planning_tutorial_and_system_ui.md`](84_management_planning_tutorial_and_system_ui.md)
- [`85_audio_backend_voice_bus_and_resource_lifecycle.md`](85_audio_backend_voice_bus_and_resource_lifecycle.md)
- [`86_audio_semantics_spatial_medium_music_and_dialogue.md`](86_audio_semantics_spatial_medium_music_and_dialogue.md)
- [`87_subtitles_captions_alarms_and_accessibility_presentation.md`](87_subtitles_captions_alarms_and_accessibility_presentation.md)
- [`88_animation_camera_vfx_and_presentation_state_integration.md`](88_animation_camera_vfx_and_presentation_state_integration.md)
- [`89_ta11_runtime_phase_integration_debugging_and_validation.md`](89_ta11_runtime_phase_integration_debugging_and_validation.md)
- [`TA11_CROSS_VALIDATION.md`](TA11_CROSS_VALIDATION.md)

"""
    if "### Governance\n" not in text:
        raise SystemExit("technical README governance anchor missing")
    text = text.replace("### Governance\n", docs_block + "### Governance\n", 1)
text = text.replace(
    "Later TA phases define input/UI/audio/presentation integration, persistence implementation details, performance/concurrency/memory/streaming budgets, testing/CI, final architecture integration audit, and implementation handoff/locking.",
    "Later TA phases define persistence implementation details, performance/concurrency/memory/streaming budgets, testing/CI, final architecture integration audit, and implementation handoff/locking.",
)
text = text.replace(
    "**TA-10 Architecture Complete does not authorize C++/OpenGL scaffolding yet.**",
    "**TA-11 Architecture Complete does not authorize C++/OpenGL/audio/UI scaffolding yet.**",
)
p.write_text(text, encoding="utf-8")


# Roadmap: complete TA-11 and advance TA-12.
p = Path("docs/technical_architecture/TA_ROADMAP.md")
text = p.read_text(encoding="utf-8")
old = """## TA-11 — Input, UI, Audio, and Presentation Integration

**Status:** Next

Must define raw input → semantic actions, input contexts/focus/routing, keyboard/mouse/controller remapping, controller deadzones/aim-assist boundary, shipping UI/HUD architecture and read models, menus/settings, loading/failure/mission/strategic presentation, subtitles/captions, alarms/notifications, miniaudio adapter/resource lifetime/buses/spatialization/streaming, animation/presentation integration, and accessibility behavior without crossing gameplay knowledge/authority boundaries.
"""
new = """## TA-11 — Input, UI, Audio, and Presentation Integration

**Status:** Architecture Complete

Defines:

- fixed-tick semantic ActionId sampling from GLFW device state, one-use physical edge latching, deterministic context priority/consumption and safe device switching;
- full remapping, conflict/essential-action validation, controller calibration/deadzones/inversion, Hold/Toggle, Auto-Sprint, sustained-interaction support and bounded knowledge-safe Aim Assist;
- profile/application settings snapshots and optional semantic haptics behind a StarForge-owned backend;
- purpose-built retained shipping UI, one focus owner, pointer/keyboard/controller navigation, preview/confirm/commit separation, safe area and responsive UI/text scaling;
- HarfBuzz + FreeType shipping text shaping/rasterization behind project-owned text/layout and renderer-owned glyph atlases;
- knowledge-filtered HUD, interaction prompts, markers, notifications and ship/station/mission/raid presentation with no false success before commit;
- management/planning/tutorial/save/loading/failure/system UI using immutable composite Read Models and typed Commands without UI-side gameplay authority or partial pause;
- miniaudio adapter ownership, callback isolation, generation-checked voices, semantic buses, resident/streamed clips, listener/emitter/device lifecycle and safe silent degradation;
- GameplaySoundEvent vs PresentationAudioEvent separation, medium/vacuum/conduction/Pilot Telemetry rules, dialogue/radio and knowledge-safe adaptive music;
- subtitles/Closed Captions, typed alarms, acknowledgement/escalation, redundant critical channels and accessibility presentation transforms;
- animation/camera/VFX/viewmodels as presentation projections with no generic root-motion, hit, resource or objective authority;
- explicit fixed-tick → stable Read Model/PresentationEvent → variable-rate presentation integration, Simulation Time vs Presentation Time separation, stale-generation rejection and headless semantic testability.

Artifacts: `80_input_device_action_and_context_routing.md` through `89_ta11_runtime_phase_integration_debugging_and_validation.md`, plus `TA11_CROSS_VALIDATION.md`.
"""
if old not in text:
    raise SystemExit("roadmap TA-11 anchor missing")
text = text.replace(old, new, 1)
text = text.replace(
    "## TA-12 — Persistence Implementation Architecture\n\n**Status:** Planned",
    "## TA-12 — Persistence Implementation Architecture\n\n**Status:** Next",
    1,
)
text = text.replace("numeric TA-3 through TA-10 budgets", "numeric TA-3 through TA-11 budgets", 1)
text = text.replace(
    "content-build concurrency, and strategic backlog limits.",
    "content-build concurrency, UI/layout/text/glyph-atlas work, input/controller processing, marker/presentation queues, audio voices/streams/decode/occlusion, animation/VFX work, and strategic backlog limits.",
    1,
)
text = text.replace(
    "renderer/physics/station/runtime-entity/AI/navigation/mission/raid/event/content smoke and deterministic tests, formatting/tidy/warnings, sanitizers, CI gates, and debug-tool requirements.",
    "renderer/physics/station/runtime-entity/AI/navigation/mission/raid/event/content/input/UI/text/audio/subtitle/caption/alarm/accessibility/presentation smoke and deterministic tests, formatting/tidy/warnings, sanitizers, CI gates, and debug-tool requirements.",
    1,
)
text = text.replace(
    "→ TA-10 Architecture Complete  \n→ **TA-11 next**  \n→ TA-12 ... TA-15",
    "→ TA-10 Architecture Complete  \n→ TA-11 Architecture Complete  \n→ **TA-12 next**  \n→ TA-13 ... TA-15",
    1,
)
p.write_text(text, encoding="utf-8")


# Root README.
p = Path("README.md")
text = p.read_text(encoding="utf-8")
text = text.replace(
    "Technical Architecture stages **TA-0 through TA-10** are complete/Architecture Complete. The next dependency is **TA-11 — Input, UI, Audio, and Presentation Integration**.",
    "Technical Architecture stages **TA-0 through TA-11** are complete/Architecture Complete. The next dependency is **TA-12 — Persistence Implementation Architecture**.",
    1,
)
if "TA-11 now fixes the complete player-facing input/presentation integration" not in text:
    anchor = "TA-10 now fixes the complete content/asset path: canonical source layout and closed JSON schemas, path-independent ContentIds, SHA-256 content fingerprints/ContentBuildId, glTF import through fastgltf, meshoptimizer offline mesh/LOD processing, KTX2/KTX-Software texture cooking, glslang shader validation, explicit collision/navigation/terrain cooking, gameplay/procedural module definitions without a scripting VM, versioned loose cooked assets plus immutable Content Registry/CPU cache, deterministic dependency-driven incremental builds, safety-classified hot reload, headless content validation, and registry-first runtime scene loading where I/O timing cannot change gameplay or reroll committed procedural content.\n"
    addition = anchor + "\nTA-11 now fixes the complete player-facing input/presentation integration: fixed-tick semantic action sampling and remapping, explicit input/focus contexts, a StarForge-owned retained shipping UI with HarfBuzz/FreeType text, knowledge-filtered HUD/markers/management/tutorial flows, miniaudio-backed audio with AI-hearing separation and vacuum/Pilot Telemetry rules, subtitles/captions/typed alarms/accessibility, animation/camera/VFX presentation-only boundaries, and deterministic stable-state/event handoff where presentation timing cannot mutate gameplay.\n"
    if anchor not in text:
        raise SystemExit("root README TA-10 anchor missing")
    text = text.replace(anchor, addition, 1)
if "- FreeType + HarfBuzz for shipping UI text" not in text:
    text = text.replace(
        "- miniaudio behind a StarForge audio layer\n",
        "- miniaudio behind a StarForge audio layer\n- FreeType + HarfBuzz for shipping UI text\n",
        1,
    )
p.write_text(text, encoding="utf-8")


# Sanity checks.
checks = {
    "docs/technical_architecture/ARCHITECTURE_DECISIONS.md": ["AD-088", "AD-100"],
    "docs/technical_architecture/03_toolchain_and_dependencies.md": ["FreeType", "HarfBuzz", "IHapticsBackend"],
    "docs/technical_architecture/README.md": ["TA-12 — Persistence Implementation Architecture", "TA11_CROSS_VALIDATION.md"],
    "docs/technical_architecture/TA_ROADMAP.md": ["TA-11 Architecture Complete", "**TA-12 next**"],
    "README.md": ["TA-0 through TA-11", "TA-12 — Persistence Implementation Architecture"],
}
for path, terms in checks.items():
    data = Path(path).read_text(encoding="utf-8")
    for term in terms:
        if term not in data:
            raise SystemExit(f"sanity check failed: {term} absent in {path}")
