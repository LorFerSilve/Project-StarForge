# Multiplayer Position

> **Status:** Draft  
> **Authority:** Baseline multiplayer scope and the boundary a future multiplayer design must not cross without a new accepted specification

## 1. Core Decision

Project StarForge's complete baseline game is **single-player PvE**.

## 2. Not Required

The baseline has no dependency on:

- synchronous co-op;
- synchronous PvP;
- asynchronous PvP raids;
- matchmaking;
- dedicated servers;
- server-authoritative economy;
- anti-cheat infrastructure;
- account-wide shared galaxy state.

## 3. Raids

GDS-11 raids and Horizon Station attacks are against systemic NPC/faction targets and forces.

The player's station is not uploaded as an asynchronous PvP target in the baseline.

## 4. Economy

Markets are local single-player world state.

There is no player auction house, real-money market, or server-controlled global supply.

## 5. Saves

The baseline uses local authoritative single-player save state.

No server validation is required to accept progression.

## 6. Pause

True Pause is supported because the baseline does not need to keep a shared multiplayer simulation running.

## 7. Determinism

Deterministic seeds/state are used for consistency, save/load, and simulation—not as a commitment to lockstep multiplayer networking.

## 8. Future Multiplayer

Any future multiplayer mode requires a new accepted design specification covering at minimum:

- mode purpose;
- player count;
- host/server authority;
- world ownership;
- persistence;
- economy separation;
- save compatibility;
- pause behavior;
- mission/raid instancing;
- robot command ownership;
- friendly fire;
- latency/lag compensation;
- disconnect recovery;
- anti-cheat/security where relevant.

## 9. Baseline Compatibility Rule

A future multiplayer addition must not make the existing single-player campaign incomplete or require conversion of its economy/progression to an always-online service.

## 10. Save Separation

If future competitive/shared-state multiplayer requires authoritative online progression, it must use a clearly separated persistence model rather than silently trusting/importing unrestricted single-player inventory/Credits.

## 11. No Design Placeholders

Implementation must not build speculative networking architecture merely because multiplayer might exist someday.

DD-007 remains applicable: technical systems are built when approved game requirements need them.

## 12. Explicit Non-Goals

The baseline does not include:

- online-only requirement;
- multiplayer economy;
- PvP base theft;
- co-op balancing dependency;
- player-to-player trading;
- server-hosted progression.

## 13. Dependencies

This specification formalizes DD-010 and DD-065 and constrains Economy, Persistence, Difficulty, Raids, and future technical architecture.

## 14. Open Questions

None for the baseline game. Future multiplayer is a separate future design problem, not an unresolved GDS-12 dependency.
