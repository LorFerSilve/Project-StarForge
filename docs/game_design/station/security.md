# Station Security

> **Status:** Draft  
> **Authority:** Internal access control, intrusion detection, lockdown, boarding response, security zones, and internal defensive coordination

## 1. Purpose

Security protects the home station when threats reach or enter its structure.

It governs access, detection, lockdown, boarding response, and critical-area protection.

## 2. Security Domains

The baseline distinguishes:

- Access Control
- Detection
- Lockdown
- Internal Response
- Critical Asset Protection

External weapons remain part of station defenses.

## 3. Security Zones

Station spaces can be classified as:

- Crew
- Restricted
- Critical
- Quarantine
- Locked Down

A classification changes authorization rules but does not create a physical barrier by itself.

## 4. Identity and Authorization

The player, crew, approved visitors, robots, and intruders have security identities or classifications.

Doors and terminals can verify authorization while their control systems function.

## 5. Player Authority

The player has highest normal station authority.

Pressure and other safety interlocks can still require an explicit emergency override.

## 6. Crew Permissions

Crew receives access suitable to assignment.

Examples:

- Farmer: agricultural areas.
- Engineer: engineering and service spaces.
- Scientist: research areas.
- Security Officer: security-controlled areas.

The crew domain defines detailed permission assignment.

## 7. Door Security States

Security-controlled doors can be:

- Unlocked
- Locked
- Emergency Sealed
- Power-Failed
- Jammed
- Destroyed

Security and pressure safety are separate concerns.

## 8. Detection Sources

Intrusion can be detected through:

- docking authorization;
- access logs;
- motion/personnel sensors;
- cameras;
- breach detection;
- combat reports;
- crew observation.

Detection depends on functioning infrastructure.

## 9. Unknown Contacts

Unidentified entities are not automatically hostile unless security policy, faction state, or behavior classifies them as such.

Restricted zones can use stricter policy.

## 10. Boarding Alert

Confirmed hostile boarding can trigger:

- intruder tracking;
- bulkhead closure;
- critical-area protection;
- security-robot activation;
- crew warning;
- station lockdown.

## 11. Lockdown

Lockdown can:

- close and lock selected doors;
- restrict lifts;
- isolate sectors;
- block sensitive terminals.

It cannot physically close a destroyed or obstructed door.

## 12. Crew During Lockdown

Automation attempts to account for known crew locations.

Immediate containment may still trap crew with an intruder if that is necessary to stop spread.

Crew behavior determines evacuation and combat response.

## 13. Security Infrastructure

Security can include:

- security control room;
- local terminals;
- sensors;
- cameras;
- access controllers;
- armories;
- security-robot docks.

## 14. Security Officer

A Security Officer can automate:

- alert assessment;
- lockdown coordination;
- access policy;
- intruder tracking;
- robot deployment.

The officer cannot detect what no available sensor or witness can observe.

## 15. Security Robots

Internal response robots are controlled by robot rules.

Security creates tasks and response priorities.

## 16. Critical Areas

The player can mark high-protection targets such as:

- Command Core;
- reactor control;
- shield control;
- armory;
- secure storage;
- research vault.

Priority affects response, not hidden durability.

## 17. Internal Weapons

Internal automated weapons, if installed, require:

- target identification;
- power;
- line of fire;
- safety policy.

They obey combat rules and friendly-fire protection.

## 18. Hacking

A successful hack grants only the exact capability of the compromised system.

Examples:

- unlock one door network;
- suppress a local alarm;
- disable a camera;
- interrupt local turret control.

One generic terminal does not provide magical universal control.

## 19. Segmentation

Security-control networks can be segmented to reduce the consequence of one compromised node.

## 20. Alarm Levels

Security uses:

- Normal
- Suspicious
- Alert
- Boarding Confirmed
- Lockdown
- Recovery

Transitions depend on observed state.

## 21. Uncertain Information

Spoofed or damaged sensors can provide uncertain data where an explicit mechanic supports it.

The interface distinguishes confirmed and uncertain contacts.

## 22. Player Controls

The player can:

- set access policy;
- lock or unlock eligible doors;
- initiate lockdown;
- define protected zones;
- inspect alerts;
- deploy response;
- override eligible automation.

## 23. Power and Control Failure

Each security device has an authored fail behavior.

Examples include local backup, fail-locked, fail-unlocked, or manual mechanical access.

There is no universal power-failure door rule.

## 24. Damage

Destroyed sensors create blind spots.

Destroyed control nodes remove their functionality.

Destroyed doors become physical damage states rather than simply unlocked doors.

## 25. Recovery

The station returns toward Normal only after:

- hostile presence is cleared;
- compromised systems are reset/repaired;
- lockdown boundaries are verified;
- access policy is restored.

## 26. Persistence

Access policies, zone states, door security, damaged sensors, alerts, and known intruder state persist where applicable.

## 27. Edge Cases

A lockdown can trap both crew and intruder in one compartment.

Security unlock cannot override pressure danger without explicit safety override.

If central security fails, local controllers retain authored behavior.

If identity spoofing succeeds through an explicit hacking mechanic, security acts on available information until contradicted.

## 28. Progression

Early game: simple locks and manual response.

Mid game: Security Officer, zone policy, robots, segmentation.

Late game: layered sensing, hardened critical zones, redundant control, automated response.

## 29. Explicit Non-Goals

The baseline does not require:

- real-world cybersecurity simulation;
- complex social permission modeling for every conversation;
- universal station takeover from one hack;
- security rules overriding physical pressure or structure.

## 30. Tuneable Parameters

Tuneable values include detection time, sensor range, backup duration, door resistance, and response delay.

## 31. Dependencies

This specification depends on structural rules, atmosphere, power, automation, damage, crew, robots, raids, hacking, and combat.

## 32. Open Questions

None in the current baseline.

The document remains Draft until dependent domains are cross-validated.
