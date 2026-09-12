from pathlib import Path
import re


def write(path: str, text: str) -> None:
    Path(path).write_text(text, encoding='utf-8')


def replace_once(path: str, old: str, new: str, label: str) -> None:
    p = Path(path)
    text = p.read_text(encoding='utf-8')
    if old not in text:
        raise SystemExit(f'missing expected text for {label}: {path}')
    write(path, text.replace(old, new, 1))


# 1) Central architecture decisions.
p = Path('docs/technical_architecture/ARCHITECTURE_DECISIONS.md')
text = p.read_text(encoding='utf-8')
if '## AD-101 —' not in text:
    text += r'''

---

## AD-101 — Save History Uses Immutable Committed Generations and Logical Slot Supersession

**Status:** Accepted

### Decision

Every committed `.sfgsave` generation is immutable. Manual overwrite, Quick Save replacement, and Autosave rotation create a new SnapshotSequence generation first; older generations become superseded/cleanup candidates only after the new generation commits.

### Rationale

This removes in-place corruption risk and makes crash recovery/catalog reconstruction deterministic.

---

## AD-102 — Save Container V1 Has a Fixed 128-Byte Header and 64-Byte Section Entries

**Status:** Accepted

### Decision

Container v1 is little-endian with exact project-owned field offsets, `SFGSAVE\0` magic, header/directory CRC32C, 8-byte-aligned payloads, and 32-byte SHA-256 ContentBuildId metadata. Directory entries are exactly 64 bytes and payloads are ordered by stable SectionKind.

### Rationale

Exact byte contracts prevent implementation-defined padding, offsets, and compatibility behavior.

---

## AD-103 — Persistence Uses a Stable SectionKind Registry and Explicit Domain Codecs

**Status:** Accepted

### Decision

Required gameplay state is partitioned into fixed SectionKinds with explicit owning serializers/schema versions. SaveMetadata is the only baseline Optional section; LocalContextContinuation is a project-owned DTO section rather than a runtime-memory dump.

### Rationale

Persistence remains traceable to domain ownership while still capturing consequential active-local continuation state.

---

## AD-104 — Save Commit Is Validated Pending File to Unique Atomic Rename

**Status:** Accepted

### Decision

A save writes a unique same-directory pending file, flushes it, reopens it through the normal reader, validates container/section integrity, then atomically renames it into a previously unused committed `.sfgsave` generation name. Catalog update and old-generation cleanup happen after this commit point.

### Rationale

A failed write or cleanup cannot destroy the last known-good generation, and the catalog never becomes commit authority.

---

## AD-105 — Load Is Fully Staged and Replaces the SessionRoot Exactly Once

**Status:** Accepted

### Decision

Container decode, migrations, content resolution, domain import, local/cross-domain integrity validation, and active-context preparation all occur in isolated staging state. Only a fully valid staged session can replace the live SessionRoot, and the replacement is one application-level ownership swap.

### Rationale

No corrupt/incompatible save can partially mutate the running campaign or merge with its current inventory/economy.

---

## AD-106 — Save Migration Is Deterministic, Source-Preserving, and Identity-Preserving

**Status:** Accepted

### Decision

Supported historical saves migrate through explicit container/application/domain chains in staging. Source files are never rewritten during load; logical Persistent IDs and exactly-once transaction markers are preserved, with deterministic allocator-based creation only when a formal split/conversion rule requires new records.

### Rationale

Version upgrades must not create reload exploits, data loss, fresh identities, or migration outcomes dependent on timing/randomness.

---

## AD-107 — ContentBuildId Is Compatibility Metadata; Actual ContentId Resolution Is Authority

**Status:** Accepted

### Decision

A differing saved/current ContentBuildId does not by itself reject a save. Required saved ContentIds must resolve under the current registry or an explicit versioned rename/removal migration rule; missing required content without such a rule fails load.

### Rationale

Content builds can change without invalidating stable logical identity, while silent fuzzy aliases/deletions remain prohibited.

---

## AD-108 — Save Catalog and Campaign Manifests Are Rebuildable Storage Caches, Not Gameplay Authority

**Status:** Accepted

### Decision

Logical slot selection and menu metadata can use rebuildable index/manifest files, but committed `.sfgsave` headers/sections remain the authoritative storage evidence. Filesystem timestamps never decide save ordering; SnapshotSequence and validity do.

### Rationale

A crash between file commit and catalog update must not lose a successfully committed save.

---

## AD-109 — Profile/Application Settings Use Separate Versioned Fail-Soft Persistence

**Status:** Accepted

### Decision

Controls, accessibility, HUD, audio, display and other non-gameplay settings persist outside campaign saves in a versioned project-owned profile document. Missing/invalid settings can safely default/clamp, and profile corruption cannot invalidate campaign world state.

### Rationale

Presentation/input preferences require different recovery semantics from authoritative gameplay saves and must be portable across campaigns.

---

## AD-110 — Persistence Diagnostics Share the Shipping Parsers and Do Not Heuristically Repair Gameplay State

**Status:** Accepted

### Decision

Save inspection/recovery tooling reuses the same container reader, codecs, migrations and validators as the game. Recovery may rebuild catalog/quarantine storage debris, but there is no general automatic repair that invents resources, IDs, objective state, or ContentIds.

### Rationale

A permissive second loader or heuristic repair path would undermine the exact integrity guarantees of normal loading.

---

## AD-111 — Loading Resumes After the Saved Stable Tick Without Advancing Gameplay During Load

**Status:** Accepted

### Decision

A save represents complete state after `saved_simulation_tick` reaches its Stable Simulation Boundary. Loading/migration/backend reconstruction advances no gameplay; the next ordinary authoritative tick is `saved_simulation_tick + 1`.

### Rationale

This prevents duplicate timer/action processing and guarantees wall-clock load duration cannot progress the simulation.
'''
write(str(p), text)

# 2) TA roadmap: replace TA-12 section and current sequence.
p = Path('docs/technical_architecture/TA_ROADMAP.md')
text = p.read_text(encoding='utf-8')
text = re.sub(
    r'## TA-12 — Persistence Implementation Architecture\n\n\*\*Status:\*\* Next\n\nMust define exact TA-2 save-container byte layout, section directory/manifest, domain serialization registry, CRC32C, write-new-then-commit, manual/quick/autosave layout, migrations, crash recovery, diagnostics, content-compatibility integration, and deterministic load validation\.',
    '''## TA-12 — Persistence Implementation Architecture

**Status:** Architecture Complete

Defines:

- PersistenceService save/load operation state machines, Stable Save Boundary capture, immutable SaveSnapshot handoff and explicit session tokens;
- exact little-endian v1 container bytes: 128-byte header, 64-byte directory entries, header/directory/per-payload CRC32C, 8-byte payload alignment, ContentBuildId metadata and codec None baseline;
- stable SectionKind registry for all required gameplay domains plus optional SaveMetadata, SimulationInfrastructure and LocalContextContinuation, with explicit versioned domain codecs;
- immutable committed save generations, SnapshotSequence ordering, logical Manual/Quick/Autosave slot semantics, rebuildable catalog/index state and 10-generation rolling Autosave retention;
- crash-safe pending-file -> flush -> reopen/validate -> unique atomic rename commit, with catalog update and superseded cleanup strictly post-commit;
- deterministic all-or-nothing load staging, domain/cross-domain/ownership/timer/RNG/content validation, RuntimeActivationPlan creation, one SessionRoot replacement and fresh backend reconstruction;
- explicit container/application/domain migration chains, deterministic migration-created ID allocation, exactly-once marker preservation, source-save immutability and ContentId rename/removal compatibility rules;
- separate versioned fail-soft profile/application settings persistence for controls/accessibility/HUD/audio/display preferences without campaign coupling;
- shared save inspector/diagnostic/recovery tooling, golden/corruption fixtures, quarantine and a prohibition on heuristic general gameplay repair;
- fixed runtime integration: no mid-tick capture, no load-time Simulation progression, no presentation one-shot replay, and resume from `saved_simulation_tick + 1`.

Artifacts: `90_persistence_service_and_snapshot_orchestration.md` through `99_ta12_runtime_integration_debugging_and_validation.md`, plus `TA12_CROSS_VALIDATION.md`.''',
    text,
    count=1,
)
text = text.replace('## TA-13 — Concurrency, Performance, Memory, and Streaming Budgets\n\n**Status:** Planned', '## TA-13 — Concurrency, Performance, Memory, and Streaming Budgets\n\n**Status:** Next', 1)
text = text.replace('→ **TA-12 next**  \n→ TA-13 ... TA-15', '→ TA-12 Architecture Complete  \n→ **TA-13 next**  \n→ TA-14 ... TA-15', 1)
write(str(p), text)

# 3) Technical architecture README current phase + baseline + docs + gate.
p = Path('docs/technical_architecture/README.md')
text = p.read_text(encoding='utf-8')
text = text.replace('- **TA-11 — Input, UI, Audio, and Presentation Integration**.\n\nThe next dependency is **TA-12 — Persistence Implementation Architecture**.', '- **TA-11 — Input, UI, Audio, and Presentation Integration**;\n- **TA-12 — Persistence Implementation Architecture**.\n\nThe next dependency is **TA-13 — Concurrency, Performance, Memory, and Streaming Budgets**.', 1)
marker = '## Architecture Documents\n'
if '### TA-12 Persistence Implementation' not in text:
    block = '''### TA-12 Persistence Implementation

TA-12 establishes:

- Stable Save Boundary persistence-service orchestration with immutable snapshots and background encoding/writing;
- an exact v1 `SFGSAVE` binary container with a 128-byte header, 64-byte section entries, deterministic little-endian encoding, CRC32C and explicit ContentBuildId metadata;
- a stable required SectionKind/domain-codec registry including consequential LocalContextContinuation without serializing runtime/backend memory;
- immutable save generations, logical manual/quick/autosave slots, SnapshotSequence ordering, rebuildable catalog caches and 10-slot rolling autosave retention;
- crash-safe pending-file validation and atomic unique rename commit with post-commit cleanup only;
- staged all-or-nothing load/migration/content validation followed by exactly one SessionRoot replacement and fresh runtime/backend reconstruction;
- deterministic source-preserving migrations with Persistent-ID/exactly-once preservation and explicit ContentId compatibility rules;
- separate fail-soft versioned profile persistence for non-gameplay controls/accessibility/HUD/audio/display settings;
- save inspection, corruption/quarantine diagnostics, golden fixtures and no heuristic gameplay-state repair;
- fixed load-resume semantics at `saved_simulation_tick + 1` with zero gameplay progression during file/migration/backend work.

'''
    text = text.replace(marker, block + marker, 1)
if '### TA-12\n\n- [`90_persistence_service_and_snapshot_orchestration.md`]' not in text:
    docs = '''### TA-12

- [`90_persistence_service_and_snapshot_orchestration.md`](90_persistence_service_and_snapshot_orchestration.md)
- [`91_save_container_v1_byte_layout.md`](91_save_container_v1_byte_layout.md)
- [`92_section_registry_domain_codecs_and_payload_rules.md`](92_section_registry_domain_codecs_and_payload_rules.md)
- [`93_save_catalog_slots_manual_quick_and_autosave.md`](93_save_catalog_slots_manual_quick_and_autosave.md)
- [`94_crash_safe_write_commit_and_recovery.md`](94_crash_safe_write_commit_and_recovery.md)
- [`95_load_staging_validation_and_session_activation.md`](95_load_staging_validation_and_session_activation.md)
- [`96_save_migration_and_content_compatibility.md`](96_save_migration_and_content_compatibility.md)
- [`97_profile_application_settings_persistence.md`](97_profile_application_settings_persistence.md)
- [`98_persistence_diagnostics_inspection_and_recovery_tooling.md`](98_persistence_diagnostics_inspection_and_recovery_tooling.md)
- [`99_ta12_runtime_integration_debugging_and_validation.md`](99_ta12_runtime_integration_debugging_and_validation.md)
- [`TA12_CROSS_VALIDATION.md`](TA12_CROSS_VALIDATION.md)

'''
    text = text.replace('### Governance\n', docs + '### Governance\n', 1)
text = text.replace('Later TA phases define persistence implementation details, performance/concurrency/memory/streaming budgets, testing/CI, final architecture integration audit, and implementation handoff/locking.', 'Later TA phases define performance/concurrency/memory/streaming budgets, testing/CI, final architecture integration audit, and implementation handoff/locking.', 1)
text = text.replace('**TA-11 Architecture Complete does not authorize C++/OpenGL/audio/UI scaffolding yet.**', '**TA-12 Architecture Complete does not authorize implementation scaffolding yet.**', 1)
write(str(p), text)

# 4) Root README project phase.
p = Path('README.md')
text = p.read_text(encoding='utf-8')
text = text.replace('Technical Architecture stages **TA-0 through TA-11** are complete/Architecture Complete. The next dependency is **TA-12 — Persistence Implementation Architecture**.', 'Technical Architecture stages **TA-0 through TA-12** are complete/Architecture Complete. The next dependency is **TA-13 — Concurrency, Performance, Memory, and Streaming Budgets**.', 1)
if 'TA-12 now fixes the concrete persistence implementation architecture' not in text:
    anchor = 'TA-11 now fixes the complete player-facing input/presentation integration: fixed-tick semantic action sampling and remapping, explicit input/focus contexts, a StarForge-owned retained shipping UI with HarfBuzz/FreeType text, knowledge-filtered HUD/markers/management/tutorial flows, miniaudio-backed audio with AI-hearing separation and vacuum/Pilot Telemetry rules, subtitles/captions/typed alarms/accessibility, animation/camera/VFX presentation-only boundaries, and deterministic stable-state/event handoff where presentation timing cannot mutate gameplay.\n'
    addition = anchor + '\nTA-12 now fixes the concrete persistence implementation architecture: exact v1 binary container bytes and SectionKind registry, Stable Save Boundary snapshot orchestration, immutable Manual/Quick/Autosave generations and crash-safe atomic commit, deterministic staged load/session replacement, explicit migrations and ContentId compatibility, separate profile-settings persistence, diagnostic/recovery tooling, and resume semantics that never advance gameplay during load.\n'
    if anchor not in text:
        raise SystemExit('missing root README TA-11 anchor')
    text = text.replace(anchor, addition, 1)
write(str(p), text)

# 5) Sanity checks.
checks = {
    'docs/technical_architecture/ARCHITECTURE_DECISIONS.md': ['## AD-101 —', '## AD-111 —'],
    'docs/technical_architecture/TA_ROADMAP.md': ['## TA-12 — Persistence Implementation Architecture', '**Status:** Architecture Complete', '**TA-13 next**'],
    'docs/technical_architecture/README.md': ['**TA-12 — Persistence Implementation Architecture**', '### TA-12 Persistence Implementation', 'TA12_CROSS_VALIDATION.md'],
    'README.md': ['TA-0 through TA-12', 'TA-13 — Concurrency, Performance, Memory, and Streaming Budgets', 'TA-12 now fixes the concrete persistence implementation architecture'],
}
for path, needles in checks.items():
    data = Path(path).read_text(encoding='utf-8')
    for needle in needles:
        if needle not in data:
            raise SystemExit(f'sanity check failed: {needle!r} not found in {path}')

print('TA-12 governance patch ready')
