# TA-14 — Diagnostics, Logging, Assertions, Crash Capture, and Debug Tooling

> **Status:** Architecture Complete  
> **Authority:** Diagnostic event model, logging, assertions, invariant failures, crash context, trace correlation, debug inspectors/commands, redaction, and diagnostic artifact requirements

## 1. Purpose

A complex simulation cannot be tested or debugged if failures are reported only as generic exceptions or console strings.

StarForge therefore uses typed, correlated, bounded diagnostics designed alongside the architecture rather than added after implementation.

## 2. Diagnostic Severity

Engineering diagnostics use a namespace separate from gameplay/presentation severities.

Canonical engineering levels:

```text
Trace
Debug
Info
Warning
Error
Fatal
```

These are not `AlarmPriority`, `AutomationReportSeverity`, or user notification priorities.

## 3. Diagnostic Event

A project diagnostic record can contain:

- severity;
- stable diagnostic code;
- subsystem/category;
- monotonic timestamp;
- SimulationTick where meaningful;
- thread ID/name;
- SceneGeneration/session token;
- typed persistent/runtime ID where safe;
- ContentId;
- revision/generation;
- human-readable message;
- structured key/value context.

## 4. Stable Diagnostic Codes

Important validation/runtime failures use stable machine-readable codes.

Examples conceptually:

```text
PERSISTENCE.CRC_PAYLOAD_MISMATCH
CONTENT.MISSING_REQUIRED_REFERENCE
RUNTIME.STALE_ENTITY_HANDLE
SIMULATION.BACKLOG_CRITICAL
NAV.STALE_RESULT_REJECTED
```

Tests/CI may assert codes rather than fragile prose.

## 5. Logging Boundary

Logging is observational.

A log call cannot:

- commit gameplay state;
- decide AI behavior;
- alter timing semantics;
- become required for correctness.

Failure to write a log file cannot mutate authoritative gameplay.

## 6. Structured Logging

Development logs should support both readable text and structured fields sufficient for CI/debug tooling.

Exact storage format is implementation data; semantics/codes remain project-owned.

## 7. Bounded Logging

Runtime logging uses bounded queues/buffers according to TA-13.

Under pressure:

- repetitive low-severity messages can be rate-limited/coalesced;
- Warning/Error/Fatal records retain priority;
- logging backpressure cannot stall the simulation indefinitely.

## 8. Rate Limiting

Repeated identical diagnostics include suppression counts rather than flooding millions of lines.

Suppression must not hide distinct IDs/causes when they matter for diagnosis.

## 9. Assertions

Assertion classes:

- **Programming invariant:** impossible state if code/contracts are correct;
- **Validation failure:** external/content/save/user data invalid, handled through typed result;
- **Recoverable runtime failure:** expected technical failure with defined fallback/abort path.

External invalid data must not rely on debug-only assertions for safety.

## 10. Development Assertions

Development builds aggressively assert architecture invariants such as:

- stale generation use;
- illegal direct cross-domain mutation;
- invalid transaction phase;
- backend call from forbidden thread;
- runtime handle serialization attempt;
- required deterministic ordering violation;
- double ownership/duplicate exactly-once marker.

## 11. Shipping Safety

Removing debug assertions in shipping cannot remove required validation.

Security/integrity-sensitive bounds, save/content validation and user-data checks remain active typed checks.

## 12. Fatal Invariant

A Fatal invariant violation stops the affected process/session through a controlled fatal path rather than attempting to continue with potentially corrupted authoritative state.

A last-chance save is attempted only if the failure type and persistence architecture explicitly permit a valid Stable Save Boundary; no unsafe crash save is fabricated.

## 13. Crash Context

A crash report bundle can include bounded diagnostic context such as:

- build/commit identifier;
- configuration/platform;
- active architecture/content build versions;
- current/last SimulationTick;
- current scene/context identifiers;
- rolling log tail;
- recent TA-13 trace/hitch events;
- thread names;
- failure diagnostic/assertion;
- stack trace/minidump where supported.

## 14. Privacy / Redaction

Diagnostics avoid collecting secrets or unnecessary personal/local-path data.

User-home paths are normalized/redacted where feasible.

No connected-account tokens, credentials, or arbitrary user documents belong in diagnostic artifacts.

## 15. Deterministic Trace

Headless/runtime diagnostic trace can record semantic events with stable ordering for regression replay:

- tick/phase;
- typed event/command kind;
- TransactionId;
- IDs;
- revisions;
- scheduler boundaries.

Trace is bounded and diagnostic-only.

## 16. Performance Trace Integration

TA-13 profiler events and TA-14 diagnostics share correlation identifiers/timestamps where useful, allowing a hitch to be matched to:

- worker queue growth;
- streaming request;
- mission event;
- station boundary;
- save operation;
- backend failure.

## 17. Debug Tool Surface

Dear ImGui remains development tooling only and can host inspectors such as:

- runtime entities/components;
- domain-store summaries;
- transactions/events;
- physics bodies/contacts;
- station graphs/networks;
- nav/path requests;
- AI perception/memory;
- mission/objective DAG;
- streaming cells/residency;
- content registry/cache;
- persistence operations/catalog;
- audio voices/buses;
- profiler/budgets.

## 18. Inspector Authority

Inspectors are read-only by default.

A mutating debug action must issue an explicit development command through the same validation/commit mechanisms or be clearly classified as destructive test-only state injection.

Debug UI cannot become a hidden second gameplay API.

## 19. Debug Command Registry

Development builds expose typed debug/test commands with:

- stable command ID;
- help/arguments;
- authority level;
- expected phase;
- explicit side-effect classification.

Commands are excluded/disabled in shipping unless intentionally productized.

## 20. Cheats vs Diagnostics

Cheat-like commands used for testing are tagged as such and never serve as evidence that ordinary gameplay flow works.

CI scenario tests prefer semantic scripted commands available through production validation paths.

## 21. State Dump

On test failure/invariant breach, bounded state-dump helpers can output selected canonical state for relevant subsystem/IDs.

Dumps sort by semantic keys and avoid pointer-address noise.

## 22. Graph Dumps

Station/nav/objective dependency graphs can export deterministic textual/graph forms for diagnostics.

Generated graph artifacts are failure aids, not runtime authority.

## 23. Deadlock / Stall Diagnostics

Worker/job/runtime infrastructure records enough information to diagnose:

- queue age;
- currently executing job class;
- main-thread wait reason;
- blocking I/O state;
- GPU fence waits;
- save/load stage.

The architecture prohibits arbitrary worker waits from authoritative simulation; diagnostics verify violations.

## 24. Memory Diagnostics

Development builds can report category totals/high-water marks for:

- runtime entities/components;
- content cache;
- streaming staging;
- persistence snapshots;
- renderer/GPU tracking;
- audio buffers;
- test/trace buffers.

TA-13 budget warnings use the same category vocabulary.

## 25. Leak Diagnostics

Repeated create/destroy/scene-transition tests track resource counts/generations and require stable plateau after warm-up.

Backend handles are counted but not treated as persistent identity.

## 26. Diagnostic Artifact Trigger

CI failure artifacts are captured on failure, not on every successful fast job by default.

Scheduled/performance jobs may retain success summaries/baselines as defined elsewhere.

## 27. Crash Testability

Where platform tooling permits, controlled test processes can intentionally trigger an assertion/fatal crash and verify crash metadata/artifact generation without crashing the CI runner itself.

## 28. Failure Reproduction Header

Every CI/test failure report should expose at minimum:

```text
commit/build
configuration
platform
suite/test
seed if any
SimulationTick if any
ContentBuildId/fixture version where relevant
primary diagnostic code
```

## 29. Debug Build Policy

Debug/development builds enable richer invariant checking and diagnostic metadata.

Performance certification uses an optimized/profile configuration with required correctness checks still active where architecturally necessary.

## 30. Explicit Non-Goals

Diagnostics do not provide:

- production telemetry upload architecture;
- user analytics;
- remote live-debug service;
- hidden gameplay mutation from inspector widgets;
- guaranteed recovery after corrupted Fatal state.

## 31. Dependencies

Depends on all prior architecture diagnostics/testability contracts, especially TA-7/8/9 debug needs, TA-12 persistence inspection and TA-13 profiler/budget telemetry.

## 32. Open Questions

None within diagnostics/debug tooling architecture.