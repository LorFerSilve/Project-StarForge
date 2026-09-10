# Communications and Remote Control

> **Status:** Under Review — Added by GDS-14  
> **Authority:** Cross-system local/strategic communication availability, live-vs-recorded contact, remote Horizon information, remote-order capability, communication loss/recovery, and strategic communication timing semantics

## 1. Purpose

GDS-14 identified communication as a cross-domain dependency used by Narrative, Horizon Defense, Robots, Factions, and Spacecraft without one complete authoritative baseline.

This document defines that baseline.

## 2. Communication Is Not Omniscience

Information or commands can cross distance only through a valid communication path.

A known event does not automatically become known to the player merely because it exists in simulation.

## 3. Communication Scales

The baseline distinguishes:

- **Local Communication** — within one station, ship, local-space zone, or mission environment;
- **Strategic Communication** — between different Strategic Locations.

## 4. Local Communication

Local communication can use:

- voice;
- intercom;
- radio;
- local data network;
- optical/direct link;
- robot squad communication.

Local communication can be disrupted by:

- distance;
- shielding/geometry where relevant;
- jamming;
- power loss;
- damaged transceiver/control network;
- explicit environmental/anomaly effects.

## 5. Strategic Communication Link

A **Strategic Communication Link** exists only when the current world state provides a valid route through compatible long-range communication infrastructure.

Potential providers include:

- functioning Horizon long-range communication equipment;
- player's ship long-range communication equipment;
- known operational relay/network infrastructure;
- faction/Continuance infrastructure the player is authorized to use;
- an authored alternative communication source.

## 6. No Universal Interstellar Connectivity

The player does not have permanent live contact with Horizon from every reachable location.

The Fracture and current infrastructure state can make strategic links:

- Available;
- Degraded;
- Unavailable.

## 7. Strategic Link State

Canonical states:

- **Available** — live gameplay-scale communication supported;
- **Degraded** — limited information/order capability according to the failing path;
- **Unavailable** — no live strategic information transfer.

## 8. Gameplay-Scale Latency

When a Strategic Communication Link is **Available**, baseline communication latency is treated as negligible at normal gameplay scale.

The game does not simulate real-world light-speed delay for ordinary supported strategic communication.

An authored route/anomaly can introduce a visible Strategic Timer delay, but delay is never silently invented per message.

## 9. Degraded Link

A Degraded link explicitly states its limitation, which can include:

- delayed messages;
- telemetry summary only;
- no live video;
- no remote orders;
- intermittent delivery;
- low-confidence sensor information.

The UI must never display the same fidelity as an Available link when the gameplay state is Degraded.

## 10. Unavailable Link

While Unavailable:

- Horizon still simulates;
- Defense Events still progress during unpaused Simulation Time;
- the player receives no live Horizon information through that path;
- the player cannot issue remote Horizon orders through that path.

## 11. Recorded / Queued Communication

A message can be stored for later delivery if:

- the originating system has a valid recording/outbox capability; and
- a later communication path becomes available.

Queued messages retain their original event timestamp/state context.

They do not pretend the information is current if it is stale.

## 12. Horizon Remote Alert

A player away from Horizon receives a live Defense Event warning only if:

- Horizon detects the event through actual sensors/security information; and
- a valid strategic communication path can deliver that information.

A communications blackout can therefore cause the player to learn of damage only later.

## 13. Horizon Remote Telemetry

With an Available link, remote telemetry can expose known high-level state such as:

- Defense Event phase;
- major confirmed damage;
- power/life-support/thermal critical status;
- crew/robot response summary;
- known theft/boarding state;
- requested strategic decisions.

Detailed telemetry still depends on functioning station sensors/control data.

## 14. Remote Orders

Baseline remote orders are **high-level strategic/operational commands** only.

Examples:

- change defense posture;
- authorize emergency battery reserve use;
- prioritize a critical repair;
- order a robot reserve to a defended zone;
- command evacuation/lockdown policy where automation supports it.

## 15. Remote Order Requirements

A remote order requires all of:

1. Available or explicitly order-capable Degraded Strategic Communication Link;
2. functioning receiving station control/automation interface;
3. player authorization;
4. owning subsystem capable of executing that command;
5. no physical impossibility preventing execution.

## 16. No Remote Physical Bypass

Remote control cannot:

- teleport the player/robots/resources;
- operate an unpowered actuator;
- close a destroyed door;
- repair without parts/workers;
- see through failed sensors;
- violate manual safety/pressure rules;
- directly aim/fire first-person turrets across interstellar distance as a baseline feature.

## 17. Tactical Robot Communication

Robot local squad communication remains GDS-10 authority.

This document only establishes that a strategic remote link cannot magically replace a robot's local command-network requirements.

## 18. Dialogue

Narrative live remote dialogue requires an Available communication path unless the authored scene provides another explicit medium.

When no live path exists, dialogue can use recorded/queued communication instead.

## 19. Faction Messages

Faction communication requires either:

- local presence;
- a valid strategic infrastructure path;
- an authored courier/recorded-message delivery.

Faction reputation does not itself create connectivity.

## 20. Communication and Dynamic Events

Dynamic Event knowledge follows this communication authority.

An offered event appears remotely only if a valid source/path informs the player.

Event simulation does not pause because the player is unaware.

## 21. Communication and Strategic Travel

Travel can change Strategic Communication Link availability as the player's ship changes location and route context.

Loss of contact does not cancel already committed station/world transactions.

## 22. Save/Persistence

Persist where relevant:

- strategic link availability source;
- queued/recorded messages;
- sent remote order transaction state;
- authored communication delay timers;
- known-vs-unknown information already delivered.

Reloading cannot retroactively give the player an alert that had no valid delivery path.

## 23. Presentation

UI distinguishes:

- Live;
- Degraded;
- Recorded/Delayed;
- Offline/Unavailable.

Remote state is timestamped where stale information matters.

Accessibility cannot increase communication knowledge beyond this boundary.

## 24. Failure and Recovery

Loss of communication is recoverable through actual relevant actions such as:

- repairing transceiver/power/control;
- restoring relay infrastructure;
- moving to a location with coverage;
- gaining access to compatible faction infrastructure.

## 25. Explicit Non-Goals

The baseline contains no:

- universal always-on interstellar chat;
- simulated light-speed delay for every normal message;
- remote first-person combat control across strategic distance;
- omniscient remote station telemetry;
- communication created by reputation alone.

## 26. Tuneable Parameters

Authored communication delay, degraded-link bandwidth/fidelity, notification cadence, and relay coverage can be tuneable/content data.

Availability/knowledge/physical-bypass rules are fixed.

## 27. Dependencies

Depends on World route/infrastructure state, Horizon power/control, Spacecraft systems/navigation, Narrative dialogue, Robots, Dynamic Events, Raids/Station Defense, Time, Save/Persistence, and Presentation.

## 28. Open Questions

None after GDS-14 reconciliation.
