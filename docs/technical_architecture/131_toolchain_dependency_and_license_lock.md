# TA-16 — Toolchain, Dependency, and License Lock

> **Status:** Implementation Locked  
> **Baseline:** `TA16-V1`  
> **Authority:** Exact compiler/build/dependency snapshot for the first implementation baseline

## 1. Reproducibility Principle

Implementation uses an explicit toolchain snapshot. A developer or CI runner may not silently substitute a newer compiler, CMake, dependency registry baseline, or generated OpenGL loader and still claim `TA16-V1` certification.

A deliberate upgrade requires a reviewed baseline change, clean configure/build, affected adapter/content/persistence tests, and updated evidence.

## 2. Primary Windows Toolchain

The baseline primary development toolchain is:

```text
OS family: Windows x64
IDE/toolset distribution: Visual Studio 2022 / Build Tools 17.14.40
VS build: 17.14.37628.2
C++ toolset family: MSVC v143 x64
Language mode: C++23
CMake: 4.3.3
```

CI records the concrete `cl.exe` version emitted by the installed 17.14.40 image/toolset. A later MSVC family/Visual Studio servicing version is not automatically part of `TA16-V1`; it must pass the toolchain-upgrade gate.

## 3. Independent Compiler Line

The independent compiler/tooling baseline is:

```text
LLVM / Clang: 23.1.1
Target: x86_64
Primary purpose: portability diagnostics, clang-format/clang-tidy, supported sanitizers/headless verification
```

Windows/MSVC remains the primary runtime development target. Linux/Clang exists to detect nonportable project assumptions and to host sanitizer/static-analysis work where appropriate.

## 4. vcpkg Baseline

The authoritative manifest baseline is the immutable vcpkg commit:

```text
a1cae005c39be7b18ba319fced856b68d7276271
```

The commit was selected on 2026-09-13 from vcpkg master state dated 2026-09-11. `vcpkg.json` stores it as `builtin-baseline`.

No floating vcpkg `master`, registry `latest`, or machine-global package state is implementation authority.

## 5. Direct Dependency Resolution at the Baseline

The following direct project dependencies are locked by the selected vcpkg baseline unless the manifest explicitly selects a feature subset:

| Dependency | Locked version | Role |
|---|---:|---|
| GLFW (`glfw3`) | 3.5.1 | Windows/context/raw device platform adapter |
| GLM | 1.0.3 | foundational math |
| Jolt Physics (`joltphysics`) | 5.6.0#1 | physics backend |
| Recast/Detour (`recastnavigation`) | 1.6.0#1 | grounded navigation backend/cooking |
| miniaudio | 0.11.25 | audio backend |
| FreeType | 2.14.3 | glyph/font rasterization |
| HarfBuzz | 14.4.0 | Unicode shaping |
| fastgltf | 0.9.0 | glTF 2.0 import |
| KTX-Software (`ktx`) | 4.4.2 | KTX2 library/tools |
| meshoptimizer | 1.2 | offline mesh optimization/LOD generation |
| glslang | 16.4.0 | offline GLSL validation |
| Dear ImGui (`imgui`) | 1.92.9 | development tooling only |
| Catch2 | 3.16.0 | C++ test framework |
| simdjson | 4.6.8 | content-tool JSON parsing support where selected |

`#1` denotes the vcpkg port-version in addition to the upstream version.

Transitive packages are locked by the same registry baseline and feature graph. They are not independently upgraded without changing the manifest/baseline.

## 6. glad2 Lock

The vcpkg port named `glad` at the selected baseline resolves to glad **0.1.36**, which is the older glad1 generator/package family. StarForge architecture explicitly requires **glad2**.

Therefore:

```text
glad2 upstream tag: v2.0.8
API: OpenGL 4.6 Core
language: C/C++ loader output
extensions: only explicitly required extensions; no all-extensions default
repository path for reviewed generated output: third_party/generated/glad/
```

The vcpkg `glad` port is **not** used as a substitute.

Generated glad2 files are treated as generated third-party source. The generator version and generation arguments are recorded beside the output. Gameplay targets never include glad headers directly; only the OpenGL renderer/platform boundary may do so.

## 7. Manifest Feature Partition

The vcpkg manifest uses dependency features so a headless/bootstrap build does not restore the full renderer/content stack unnecessarily:

- `tests` — Catch2;
- `runtime` — runtime backend dependencies;
- `content` — offline content/import/cook dependencies;
- `dev-tools` — Dear ImGui/debug-only dependencies.

The default bootstrap enables only what its real targets require. Full implementation presets explicitly request the additional features.

## 8. Third-Party Boundary Rule

Backend/library types remain outside authoritative gameplay/persistence contracts:

- no Jolt `BodyID` in Save DTOs;
- no OpenGL object name as gameplay identity;
- no Recast polygon ref as persistent navigation identity;
- no miniaudio voice/device object in domain state;
- no fastgltf DOM object in runtime content identity;
- no ImGui type in shipping UI contracts;
- no filesystem path replacing `ContentId`.

## 9. Engineering License Inventory

The direct dependency set was selected from permissive/open-source licensing suitable for the project architecture. The manifest/release process must retain the installed/upstream copyright/license notices.

Recorded direct-license families include:

| Dependency | Recorded license family / engineering note |
|---|---|
| GLFW | Zlib |
| GLM | MIT |
| Jolt Physics | MIT |
| Recast/Detour | Zlib |
| miniaudio | Unlicense OR MIT-0 |
| FreeType | FreeType License OR GPL option; StarForge uses the normal permissive FreeType distribution path and preserves notices |
| HarfBuzz | MIT-family |
| fastgltf | MIT |
| KTX-Software | repository files are primarily Apache-2.0 with additional per-file licenses; complete notices/REUSE inventory must ship where applicable |
| meshoptimizer | MIT |
| glslang | multiple permissive licenses in glslang proper (BSD/MIT/Apache family); exact distributed notices are preserved |
| Dear ImGui | MIT |
| Catch2 | BSL-1.0 |
| simdjson | permissive multi-license composition recorded by the port |
| glad2 generated code/tooling | generated-loader/upstream notice retained with generated source |

This is an engineering dependency gate, not legal advice. Release packaging must generate/verify a third-party notice bundle from the exact installed/source artifacts rather than relying only on this summary.

## 10. KTX Special-Case Rule

KTX-Software documents repository files with licenses beyond its primary Apache-2.0 material, including a special `lib/etcdec.cxx` license. StarForge therefore treats KTX redistribution as a notice/BOM-sensitive dependency:

- preserve upstream license inventory;
- include only required runtime/tool components;
- retain all required notices;
- do not assume the single vcpkg metadata field fully describes every upstream file.

No KTX component with unresolved redistribution obligations may enter a release artifact.

## 11. glslang Notice Rule

glslang's upstream license file contains multiple applicable license texts. Build tooling may use it under the selected upstream terms, but distribution retains the relevant notices rather than collapsing them to one guessed SPDX identifier.

## 12. Tool Discovery Rule

The build may use only:

- repository-pinned/generated tools;
- the locked compiler/CMake family;
- vcpkg-resolved tools/libraries;
- explicitly documented OS SDK tools.

A random executable found earlier on `PATH` cannot silently become a content/build dependency.

## 13. Upgrade Procedure

A dependency/toolchain upgrade requires:

1. explicit version/baseline change;
2. release/license review;
3. clean dependency restore;
4. clean configure/build;
5. project unit/headless tests;
6. affected backend smoke tests;
7. affected content recook/determinism checks;
8. persistence compatibility checks when serialization/content resolution can be affected;
9. representative performance comparison when runtime code is affected;
10. reviewed update of `TA16-V1` metadata or creation of the next implementation baseline.

## 14. Lock Verdict

```text
Primary compiler/toolset: LOCKED
Independent compiler/tooling line: LOCKED
CMake: LOCKED
vcpkg registry baseline: LOCKED
Direct dependency resolution: LOCKED
OpenGL loader mismatch: RESOLVED through glad2 v2.0.8 generated-source lock
License inventory/notice policy: LOCKED
Floating dependency authority: PROHIBITED
```
