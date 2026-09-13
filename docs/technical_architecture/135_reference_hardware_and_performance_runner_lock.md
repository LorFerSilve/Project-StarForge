# TA-16 — Reference Hardware and Performance Runner Lock

> **Status:** Implementation Locked  
> **Baseline:** `TA16-V1`  
> **Authority:** Concrete hardware/runner identity for TA-13 numeric performance certification

## 1. Purpose

TA-13 fixed software performance targets but intentionally deferred the exact reference machine. TA-16 names that machine class and runner identity so future performance numbers have a stable meaning.

## 2. Reference Hardware Class

The first controlled Windows reference machine is:

```text
Runner logical name: SF-PERF-WIN-01
GitHub self-hosted labels:
  self-hosted
  Windows
  X64
  starforge-reference
  sf-perf-win-01

CPU: AMD Ryzen 7 5800X
Cores/threads: 8 / 16
GPU: NVIDIA GeForce RTX 3080 10 GiB
RAM: 32 GiB DDR4-3200 dual-channel
Storage: >= 1 TB NVMe SSD, PCIe 3.0 x4 or better
OS baseline: Windows 11 x64, 24H2 family
Display benchmark mode: 1920x1080, 60 Hz or higher physical/virtual presentation path
Power mode: High/Best Performance, no battery throttling
Thermal state: sustained clocks must remain within validated non-throttling envelope
```

The hardware is a project reference target, not a minimum-player specification.

## 3. Driver / Firmware Evidence

GPU driver, motherboard BIOS/AGESA, Windows build, chipset driver, storage model/firmware and actual RAM timings are recorded in every reference-baseline evidence record.

They are not allowed to drift silently underneath an accepted performance baseline. A servicing change triggers at least a benchmark re-baseline comparison before new certification evidence is accepted.

## 4. Software Toolchain on Reference Runner

The runner uses the same TA16-V1 software lock:

- Visual Studio / Build Tools 17.14.40;
- MSVC v143 x64;
- CMake 4.3.3;
- vcpkg baseline `a1cae005c39be7b18ba319fced856b68d7276271`;
- exact project commit SHA;
- Release/RelWithDebInfo profile required by the benchmark definition.

## 5. TA-13 Performance Target

On the reference machine, the baseline acceptance target remains:

```text
Resolution: 1920 x 1080
Quality preset: High
Authoritative simulation: fixed 60 Hz
Presentation target: 60 FPS
Simulation p95: <= 4.0 ms
Main-thread work p95: <= 12.0 ms
GPU p95: <= 13.5 ms
GPU p99: <= 16.0 ms
```

Scenario-specific memory/streaming/Hard-Hold limits remain owned by TA-13.

## 6. Certification Scenarios

The controlled runner eventually executes the TA-13 representative matrix covering:

- dense Horizon;
- Horizon defense;
- surface combat;
- local spaceflight;
- mixed boarding;
- off-screen Horizon concurrency;
- streaming traversal;
- save/load stress;
- UI/audio presentation;
- content build where the scenario is CPU/tool focused.

A scenario enters numeric certification only after its implementation/content fixture is stable and versioned.

## 7. Run Protocol

Steady-state benchmark scenarios use TA-13's locked protocol:

```text
warm-up: 30 seconds
measurement: 120 seconds
repetitions: >= 3
reported: p50/p95/p99 plus memory/queue/hold counters relevant to the scenario
```

Cold-start/load-specific tests use their owning protocol rather than pretending to be steady-state frame tests.

## 8. Runner Cleanliness

Certification runs require:

- no unrelated heavy foreground workload;
- no active game overlay/capture tool unless it is part of the measured shipping configuration;
- no thermal throttling evidence;
- stable GPU power/performance state;
- build/content artifacts tied to the exact candidate SHA;
- clean benchmark logs and metadata.

A contaminated run is infrastructure-invalid, not a semantic pass/fail reroll.

## 9. Hosted Runner Role

`windows-2025` and `ubuntu-24.04` hosted runners can certify correctness/build portability and can run coarse timing smoke. They do **not** certify TA-13 numeric CPU/GPU performance thresholds.

## 10. Reference Runner Availability

The named self-hosted runner need not be continuously online during early implementation. It becomes mandatory before the first milestone that claims numeric TA-13 performance certification and before release/Implementation Locked certification where AD-130 requires it.

The absence of the machine does not permit generic cloud timing to become the reference baseline.

## 11. Hardware Change Control

Replacing CPU/GPU/RAM/storage with a materially different part creates a new reference baseline identifier such as `SF-PERF-WIN-02` and requires reviewed cross-baseline comparison.

A faster replacement cannot retroactively make historical regressions disappear.

## 12. Lock Verdict

```text
Reference runner identity: SF-PERF-WIN-01
Reference CPU: Ryzen 7 5800X
Reference GPU: RTX 3080 10 GiB
Reference RAM: 32 GiB DDR4-3200
Reference storage: >= PCIe 3.0 x4 NVMe
OS family: Windows 11 24H2 x64
TA-13 1080p High / 60 FPS gates: LOCKED
Generic cloud numeric certification: PROHIBITED
```
