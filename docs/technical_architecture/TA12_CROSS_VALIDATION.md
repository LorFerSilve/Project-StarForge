# TA-12 — Persistence Implementation Cross-Validation

> **Status:** PASS — Architecture Complete  
> **Scope:** `90`–`99`, TA-2 persistence contracts, relevant TA-1/3/7/9/10/11 architecture, and Design Complete GDS Save/Persistence

## 1. Purpose

This audit verifies that TA-12 turns the already-approved persistence design into an implementable architecture without changing gameplay, ownership, timing, procedural determinism, content identity, or presentation authority.

The audit specifically checks:

- exact save-container representation;
- section/domain ownership;
- Stable Save Boundary capture;
- manual/quick/autosave behavior;
- crash safety;
- all-or-nothing load;
- migrations/content compatibility;
- profile settings separation;
- corruption diagnostics/recovery;
- active-runtime continuation;
- later-phase authority boundaries.

---

## 2. GDS Save/Persistence Compatibility

| # | Check | Result |
|---:|---|:---:|
| 1 | Manual Save remains supported | PASS |
| 2 | Quick Save remains supported | PASS |
| 3 | Autosave remains supported | PASS |
| 4 | No Ironman/single-save restriction introduced | PASS |
| 5 | Manual Save is not restricted to Horizon/safe rooms | PASS |
| 6 | Save during combat remains permitted at Stable Save Boundary | PASS |
| 7 | Save during offensive Raid remains permitted | PASS |
| 8 | Save during Horizon Defense remains permitted | PASS |
| 9 | Atomic transition save requests are queued, not discarded | PASS |
| 10 | Stable Save Boundary remains the only authoritative capture point | PASS |
| 11 | Extraction cannot be saved half-committed | PASS |
| 12 | Inventory ownership transfer cannot be saved half-committed | PASS |
| 13 | Mission failure/resolution cannot be saved half-committed | PASS |
| 14 | Manufacturing output/commissioning cannot be saved half-committed | PASS |
| 15 | Construction/demolition cannot be saved half-committed | PASS |
| 16 | Blueprint/Research reward cannot be saved half-committed | PASS |
| 17 | Irreversible story choice cannot be saved half-committed | PASS |
| 18 | Recovery relocation cannot be saved half-committed | PASS |
| 19 | Scene/context ownership transition cannot be saved half-committed | PASS |
| 20 | Save/load operation itself cannot nest an unsafe snapshot | PASS |
| 21 | Player Health/shield/ammo continuation is persistable | PASS |
| 22 | Consequential projectile/status state has a persistence path | PASS |
| 23 | AI awareness/Last Known Position has a persistence path where required | PASS |
| 24 | Robot active state has a persistence path | PASS |
| 25 | Mission state has an explicit Required section | PASS |
| 26 | Raid/Defense state has an explicit Required section | PASS |
| 27 | breaches/target damage/stolen cargo remain persisted by owning state | PASS |
| 28 | reinforcement committed/ETA state remains persistable | PASS |
| 29 | dynamic events remain persistable | PASS |
| 30 | markets/Credits remain persistable | PASS |
| 31 | research/Blueprint/knowledge remain persistable | PASS |
| 32 | persistent timers remain Simulation-Time based | PASS |
| 33 | procedural seeds/RNG cursors remain persistable | PASS |
| 34 | wall-clock elapsed time after save does not progress gameplay | PASS |
| 35 | loading replaces rather than merges current session state | PASS |
| 36 | older saves can deliberately restore older timeline state | PASS |
| 37 | deterministic same-snapshot reload cannot reroll major procedural content | PASS |
| 38 | reward atomicity/idempotency remains explicit | PASS |
| 39 | trade Credits/goods remain one committed state | PASS |
| 40 | WorkOrder/research WIP remains persistence-authoritative | PASS |
| 41 | simultaneous external mission + Horizon event can coexist in one snapshot | PASS |
| 42 | stable Incapacitated player state can be saved | PASS |
| 43 | writes preserve prior valid generation on failure | PASS |
| 44 | corrupt Required state is never partially loaded | PASS |
| 45 | pure presentation/input settings remain profile-global capable | PASS |
| 46 | save-owned gameplay settings remain campaign state | PASS |
| 47 | local saves require no cloud service | PASS |
| 48 | baseline save validation requires no server authority | PASS |
| 49 | serialization completeness can represent consequential next state | PASS |
| 50 | rolling Autosave baseline retains latest 10 committed generations | PASS |

---

## 3. TA-1 Runtime / Stable Boundary Compatibility

| # | Check | Result |
|---:|---|:---:|
| 51 | snapshot capture occurs after current-tick commits settle | PASS |
| 52 | no render-frame save authority introduced | PASS |
| 53 | no physics-callback snapshot path introduced | PASS |
| 54 | no audio-callback save authority introduced | PASS |
| 55 | immutable snapshot can be encoded/written on worker | PASS |
| 56 | worker cannot mutate live gameplay state | PASS |
| 57 | True Pause does not advance Simulation Time to save | PASS |
| 58 | save during True Pause can use existing stable state | PASS |
| 59 | load does not advance Simulation Time | PASS |
| 60 | load does not create partial-pause strategic progression | PASS |
| 61 | queued save during atomic transaction waits for stable boundary | PASS |
| 62 | background file I/O need not block simulation after capture | PASS |
| 63 | save completion result is not gameplay mutation | PASS |
| 64 | load replacement occurs at controlled application boundary | PASS |
| 65 | saved tick is a completed Stable Simulation Boundary | PASS |
| 66 | resume convention is next tick = saved tick + 1 | PASS |
| 67 | saved tick is not processed twice after load | PASS |
| 68 | timers due after saved boundary resume deterministically | PASS |
| 69 | worker completion order cannot change snapshot bytes/semantics | PASS |
| 70 | future TA-13 owns numeric queue/thread budgets | PASS |

---

## 4. TA-2 Identity, Ownership, Transactions, Serialization

| # | Check | Result |
|---:|---|:---:|
| 71 | Persistent IDs remain strongly typed uint64 semantic identities | PASS |
| 72 | RuntimeEntityHandle is never serialized as identity | PASS |
| 73 | allocator NextValue persists | PASS |
| 74 | TransactionId allocator persists | PASS |
| 75 | one physical item/resource owner remains enforceable | PASS |
| 76 | Active Lease export produces one logical record | PASS |
| 77 | save does not require forced actor deactivation | PASS |
| 78 | immutable Save DTOs remain domain-owned | PASS |
| 79 | persistence owns encoding, not domain semantics | PASS |
| 80 | save container remains project-owned binary | PASS |
| 81 | little-endian representation remains fixed | PASS |
| 82 | per-section CRC32C remains fixed | PASS |
| 83 | initial codec remains None | PASS |
| 84 | deterministic collection ordering remains mandatory | PASS |
| 85 | raw pointers/references are prohibited | PASS |
| 86 | Jolt/OpenGL/GLFW/miniaudio types are prohibited | PASS |
| 87 | STL/hash bucket layout is not serialized | PASS |
| 88 | UTF-8 string encoding remains explicit | PASS |
| 89 | serialized enums have stable fixed numeric values | PASS |
| 90 | floating point persists explicit IEEE representation where required | PASS |
| 91 | Required sections cannot be silently omitted | PASS |
| 92 | duplicate singleton SectionKinds reject | PASS |
| 93 | corrupt lengths/counts are bounded before allocation | PASS |
| 94 | Strong Required references validate after staging import | PASS |
| 95 | allocator values validate above committed IDs | PASS |
| 96 | one-time transaction markers remain persisted | PASS |
| 97 | save load is all-or-nothing | PASS |
| 98 | no automatic corrupt save repair introduced | PASS |
| 99 | deterministic migration ID allocation retained | PASS |
| 100 | source save remains unchanged during migration | PASS |

---

## 5. Exact Container / Section Architecture

| # | Check | Result |
|---:|---|:---:|
| 101 | v1 header is exactly 128 bytes | PASS |
| 102 | v1 directory entry is exactly 64 bytes | PASS |
| 103 | magic remains `SFGSAVE\0` | PASS |
| 104 | ContainerVersion is explicit | PASS |
| 105 | endian marker is explicit | PASS |
| 106 | ApplicationSaveCompatibilityVersion is explicit | PASS |
| 107 | saved SimulationTick is explicit | PASS |
| 108 | SnapshotSequence is persistence-only metadata | PASS |
| 109 | LogicalSaveSlotId is persistence-only metadata | PASS |
| 110 | SaveKind is explicit stable enum | PASS |
| 111 | file size is explicit and checked | PASS |
| 112 | directory CRC is explicit | PASS |
| 113 | header CRC is explicit with deterministic zeroed-field rule | PASS |
| 114 | ContentBuildId is fast compatibility metadata, not sole compatibility authority | PASS |
| 115 | directory entries are ascending by SectionKind | PASS |
| 116 | payloads are 8-byte aligned | PASS |
| 117 | structural padding/reserved bytes are zero | PASS |
| 118 | payload CRC covers decoded bytes | PASS |
| 119 | v1 writer emits codec None | PASS |
| 120 | payload kind/schema echo detects section swap/corruption | PASS |
| 121 | SectionKind registry values are explicit/non-renumberable | PASS |
| 122 | SaveMetadata is the only baseline Optional section | PASS |
| 123 | empty gameplay domain still emits a Required section | PASS |
| 124 | LocalContextContinuation is explicit rather than runtime dump | PASS |
| 125 | SimulationInfrastructure has explicit ownership boundary | PASS |

---

## 6. Save Catalog / Crash Safety

| # | Check | Result |
|---:|---|:---:|
| 126 | every committed save generation is immutable | PASS |
| 127 | new generation commits before superseded cleanup | PASS |
| 128 | manual overwrite retains logical slot ID but writes new generation | PASS |
| 129 | Quick Save is logical replacement, not in-place file mutation | PASS |
| 130 | Autosaves are independent full snapshots | PASS |
| 131 | Autosave retention deletion occurs only after new commit | PASS |
| 132 | catalog index is rebuildable/non-authoritative | PASS |
| 133 | filesystem timestamp is not ordering authority | PASS |
| 134 | SnapshotSequence orders generations deterministically | PASS |
| 135 | duplicate conflicting SnapshotSequence is not timestamp-resolved | PASS |
| 136 | pending files are not selectable committed saves | PASS |
| 137 | pending file is written in same target filesystem/directory | PASS |
| 138 | pending file is flushed before validation/commit | PASS |
| 139 | pending file is reopened through normal reader before commit | PASS |
| 140 | commit is atomic rename into unique committed namespace | PASS |
| 141 | catalog failure after rename cannot uncommit save | PASS |
| 142 | crash after rename can rebuild/discover committed generation | PASS |
| 143 | crash before rename cannot auto-select pending file | PASS |
| 144 | pending file is not auto-promoted on restart | PASS |
| 145 | out-of-space leaves previous committed generations intact | PASS |
| 146 | permission/read-only failure does not redirect to unsafe install path | PASS |
| 147 | cleanup failure cannot invalidate successful save | PASS |
| 148 | corrupt newest manual generation can expose older valid generation explicitly | PASS |
| 149 | corrupt newest quick/autosave can expose older valid generation explicitly | PASS |
| 150 | no delete-before-write rotation exists | PASS |

---

## 7. Load, Migration, Content Compatibility

| # | Check | Result |
|---:|---|:---:|
| 151 | loader consumes one concrete committed generation | PASS |
| 152 | sections from different files are never merged | PASS |
| 153 | container corruption rejects before domain import | PASS |
| 154 | schema decode occurs only after payload CRC passes | PASS |
| 155 | historical DTO decode is version-specific | PASS |
| 156 | migration occurs in isolated workspace | PASS |
| 157 | migration cannot use current wall clock | PASS |
| 158 | migration cannot use fresh RNG entropy | PASS |
| 159 | migration cannot use live-session state | PASS |
| 160 | migration preserves logical Persistent IDs | PASS |
| 161 | split-record new IDs allocate deterministically | PASS |
| 162 | migration preserves exactly-once markers | PASS |
| 163 | differing ContentBuildId alone does not reject | PASS |
| 164 | actual required ContentId resolution determines content compatibility | PASS |
| 165 | ContentId rename requires explicit map | PASS |
| 166 | removed required content requires explicit semantic migration | PASS |
| 167 | no fuzzy ContentId lookup exists | PASS |
| 168 | no silent deletion of crew/ship/robot/items/progression | PASS |
| 169 | committed procedural outcomes are not regenerated with new algorithms | PASS |
| 170 | unsupported unfinished GenerationAlgorithmVersion fails/migrates explicitly | PASS |
| 171 | staging SessionRoot receives only current DTOs | PASS |
| 172 | domain-local validation precedes cross-domain activation | PASS |
| 173 | physical ownership validation occurs before root swap | PASS |
| 174 | TA-9 mission/raid/event/finale invariants validate before root swap | PASS |
| 175 | required activation ContentIds resolve before playable activation | PASS |
| 176 | content-registry generation change invalidates/revalidates staging | PASS |
| 177 | one SessionRoot swap replaces all domains together | PASS |
| 178 | backend handles are reconstructed fresh after load | PASS |
| 179 | presentation one-shot history is not replayed after load | PASS |
| 180 | persistent active alarms reconstruct from owning state | PASS |

---

## 8. Profile / Presentation Settings Boundary

| # | Check | Result |
|---:|---|:---:|
| 181 | profile settings remain separate from `.sfgsave` world state | PASS |
| 182 | profile format has independent schema version | PASS |
| 183 | profile corruption cannot invalidate campaign save | PASS |
| 184 | missing profile setting can receive current default | PASS |
| 185 | invalid profile setting can fail-soft to safe bounded value/default | PASS |
| 186 | newer unsupported profile is not overwritten automatically | PASS |
| 187 | existing customized bindings survive changed project defaults | PASS |
| 188 | reset-to-default is intentional category action | PASS |
| 189 | risky display preview is not persisted until confirmed | PASS |
| 190 | haptic/audio/UI preference persistence cannot alter gameplay authority | PASS |

---

## 9. Diagnostics / Tooling / Later-Phase Boundaries

| # | Check | Result |
|---:|---|:---:|
| 191 | save inspector reuses shipping persistence parsers/validators | PASS |
| 192 | inspector does not become permissive second loader | PASS |
| 193 | recovery tooling cannot invent resources/IDs/objective state | PASS |
| 194 | automatic general save repair remains prohibited | PASS |
| 195 | golden current/historical/corruption fixtures are required | PASS |
| 196 | deterministic primary rejection order is defined | PASS |
| 197 | TA-13 retains numeric persistence memory/I/O/concurrency budgets | PASS |
| 198 | TA-14 retains concrete CI/fuzz/fault/golden test gates | PASS |
| 199 | TA-15 retains full cross-architecture integration audit | PASS |
| 200 | TA-16 retains C++ target layout/platform API/pinning/implementation lock | PASS |

---

## 10. Compound Scenario Validation

### Scenario A — Quick Save During Combat

1. player presses Quick Save during active combat;
2. request becomes pending;
3. current tick completes combat/interaction/domain commits;
4. Stable Simulation Boundary is reached;
5. active actor/projectile/status/AI continuation exports coherently;
6. immutable snapshot is released to worker;
7. combat can continue while file encodes/writes;
8. new Quick generation commits by atomic unique rename;
9. only then is prior Quick generation eligible for cleanup.

**Result: PASS.** No no-save combat rule, half-tick state, or delete-before-write behavior introduced.

### Scenario B — Crash During 11th Autosave Write

1. 10 valid autosaves exist;
2. 11th snapshot is captured;
3. pending write crashes before rename;
4. previous 10 remain committed;
5. startup ignores interrupted pending generation for normal selection;
6. retention cleanup never ran, so no valid history was deleted.

**Result: PASS.**

### Scenario C — Crash After 11th Autosave Rename but Before Catalog Update

1. new generation passes validation;
2. atomic rename commits it;
3. process crashes before `catalog.sfindex` update;
4. restart scans committed files;
5. new SnapshotSequence is discovered;
6. catalog rebuild identifies newest 10; oldest becomes cleanup candidate.

**Result: PASS.** Catalog is not commit authority.

### Scenario D — Load Older Save With Renamed Weapon ContentId

1. container/CRC passes;
2. historical/current domain DTO decode succeeds;
3. saved ContentBuildId differs from current;
4. explicit old weapon ContentId -> new weapon ContentId migration map applies;
5. UniqueItemId, ownership, condition/ammo state are preserved under explicit rule;
6. current staging validators pass;
7. one SessionRoot swap occurs;
8. source file remains unchanged.

**Result: PASS.**

### Scenario E — Corrupt Inventory Section But Valid Metadata

1. save menu can display safe metadata/preflight;
2. full load finds InventoryResources CRC failure;
3. Required section never reaches semantic import;
4. staged load aborts;
5. current live session remains unchanged;
6. another valid save can be selected.

**Result: PASS.**

### Scenario F — Profile File Corrupt, Campaign Valid

1. profile loader cannot parse active profile;
2. previous valid backup or project defaults are used;
3. campaign catalog/save remains available;
4. loading campaign restores world-owned Difficulty/state from save;
5. input/UI/audio use recovered profile defaults/preferences only.

**Result: PASS.**

### Scenario G — Save During Extraction Commit

1. save request arrives while extraction transaction is indivisible;
2. request waits;
3. extraction commits Field-Unsecured -> Vehicle/Extraction-Secured ownership/security state atomically;
4. next Stable Save Boundary captures only post-commit state;
5. load cannot recreate both unsecured and secured copies.

**Result: PASS.**

### Scenario H — Load Active Raid

1. Raid/Mission/Inventory/Robot/Ship/LocalContext sections decode/migrate;
2. cross-domain state validates target damage, breaches, escalation, reinforcement commit, cargo owners and active context;
3. content required for target/local scene resolves;
4. one SessionRoot swaps;
5. Jolt/render/audio/runtime handles are reconstructed fresh;
6. no already-committed raid reward/action replays;
7. next gameplay tick is saved tick + 1.

**Result: PASS.**

---

## 11. TA-12 Architectural Decisions Requiring Central Registration

TA-12 establishes central decisions for:

- immutable save generations and logical slot supersession;
- exact v1 128-byte header / 64-byte directory format;
- stable SectionKind registry and explicit domain codecs;
- unique pending-file -> validated atomic rename commit;
- catalog/index as rebuildable non-authoritative cache;
- all-or-nothing staged SessionRoot load;
- deterministic migration/content compatibility with source-file preservation;
- separate fail-soft versioned profile persistence;
- explicit LocalContextContinuation rather than runtime memory dump;
- load resume at `saved_tick + 1` and no load-time gameplay advancement;
- shared inspector/loader validation code and no heuristic general repair.

These are registered in `ARCHITECTURE_DECISIONS.md` during TA-12 governance handoff.

---

## 12. Final Result

```text
TA-12 — Persistence Implementation Architecture
Status: ARCHITECTURE COMPLETE
Cross-validation: PASS
Checks: 200 / 200 PASS
Blocking contradictions: 0
Implementation-critical open questions: 0
```

**Next dependency:** TA-13 — Concurrency, Performance, Memory, and Streaming Budgets.
