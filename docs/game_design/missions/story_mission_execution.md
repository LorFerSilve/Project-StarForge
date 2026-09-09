# Main Story Mission Execution

> **Status:** Draft  
> **Authority:** GDS-8 gameplay-level execution contract for the canonical GDS-7 main-story mission IDs

## 1. Purpose

GDS-7 defines what each story mission must accomplish narratively.

This document defines how each mission maps into the GDS-8 mission framework:

- Primary Mission Type;
- zone composition;
- mandatory objective chain;
- extraction requirement;
- failure/retry behavior;
- fixed story output.

GDS-9, GDS-10, and GDS-11 later fill combat, robot, and raid-specific encounter mechanics without changing these mission contracts.

## 2. General Story-Mission Rules

All main-story missions:

- use fixed Mission IDs from GDS-7;
- use fixed canonical GDS-7 locations;
- cannot reroll mandatory evidence/characters;
- cannot permanently brick campaign progression through ordinary tactical failure;
- preserve committed unique knowledge according to each mission definition;
- use normal physical ownership and extraction rules;
- display hard progression blockers explicitly.

## 3. MS-P01 — Silent Signal

Location:
- Aster's Rest — Horizon Station

Primary Type:
- Reconnaissance / Survey

Secondary Type:
- Recovery / Data Retrieval

Zone Composition:
- Local Spaceflight Zone;
- EVA/Exterior access;
- initial Interior Zone.

Mandatory Objectives:

1. Identify the emergency/transponder source.
2. Approach Horizon Station.
3. Scan/identify at least one valid access route.
4. Establish physical entry into the station.
5. Reach the emergency access compartment.

Extraction:
- In-Place Resolution.

Success Commit:
- Horizon Station becomes an active Recovery Target;
- MS-P02 unlocks.

Failure:
- player/ship ordinary defeat;
- retry from pre-approach recovery state.

No unique story evidence is consumed/lost.

## 4. MS-P02 — Heart of the Station

Location:
- Horizon Station

Primary Type:
- Repair / Restoration

Zone Composition:
- Interior Zone.

Mandatory Objectives:

1. Restore emergency electrical continuity to Command Core route.
2. Restore a minimally safe breathable operational area.
3. Reach and activate Command Core.
4. Bring SERA online.
5. Complete emergency administrator authentication.
6. Restore minimum station operational state required by GDS-2.

Extraction:
- In-Place Resolution.

Success Commit:
- Horizon Station becomes player-controlled;
- Acting Recovery Administrator state granted;
- first Reach I location data becomes available.

Failure:
- local player incapacitation uses home-station recovery;
- mission remains retryable from persistent station damage/repair state.

Repairs already physically completed remain completed.

## 5. MS-A101 — Broken Relay

Location:
- Caldera — Relay ECHO-12

Primary Type:
- Repair / Restoration

Secondary Types:
- Reconnaissance;
- Recovery / Data Retrieval.

Zone Composition:
- Local Spaceflight;
- EVA Exterior;
- relay Interior.

Mandatory Objectives:

1. Reach Relay ECHO-12.
2. Survey relay failure state.
3. Establish safe access.
4. Restore minimum power/control path.
5. Restore relay navigation function.
6. Recover the Transit Lattice telemetry fragment.
7. Extract.

Extraction:
- Required through player ship / local-space departure.

Success Commit:
- Recovery Sector map expansion;
- first Transit Lattice telemetry evidence;
- route/network restoration world flag.

Failure:
- retryable;
- repairs/evidence already atomically committed remain according to world-state rules;
- unique telemetry cannot duplicate.

## 6. MS-A102 — The Living

Location:
- Morrow — Morrow Rescue Corridor

Primary Type:
- Rescue

Secondary Type:
- Repair / Restoration where generated rescue blocker requires it.

Zone Composition:
- Surface, Interior, or Mixed rescue site selected from authored Morrow templates.

Mandatory Objectives:

1. Detect/localize distress source.
2. Locate required survivor target.
3. Stabilize survivor if current health state requires it.
4. Resolve mandatory route/environment blocker.
5. Escort/transport survivor to player ship.
6. Commit survivor as Temporary Passenger.
7. Extract successfully.

Extraction:
- Required.

Success Commit:
- survivor reaches secured passenger state;
- first recruitment opportunity;
- Freehold contact lead.

Failure:
- procedural/nonunique survivor follows GDS-3 rescue failure rules;
- any unique authored story survivor uses retry-preserving authored state.

## 7. MS-A103 — A Line Reopened

Location:
- Recovery Sector route network centered on Morrow, Caldera, and Aster's Rest

Primary Type:
- Reconnaissance / Survey

Secondary Type:
- Repair / Restoration

Zone Composition:
- multi-location strategic mission using bounded relay/navigation sub-zones.

Mandatory Objectives:

1. Verify restored ECHO-12 output.
2. Survey the Morrow corridor.
3. Calibrate/confirm at least the required route reference nodes.
4. Resolve any mandatory broken route-reference component.
5. Commit a valid route solution.
6. Return/report at Horizon Station or New Ember Habitat.

Extraction:
- Required from final field sub-zone;
- final reporting is In-Place at safe hub.

Success Commit:
- local corridor becomes Verified;
- stable outside contact established;
- Reach II progression path becomes discoverable.

Failure:
- individual completed calibration commits can persist;
- mission retries only unresolved mandatory route work.

## 8. MS-A201 — The Institute

Location:
- Vesper — Vesper Observatory

Primary Type:
- Research / Anomaly Investigation

Secondary Type:
- Reconnaissance / Survey

Zone Composition:
- docked faction facility Interior;
- optional controlled sensor test zone.

Mandatory Objectives:

1. Reach and dock at Vesper Observatory.
2. Establish formal contact with Asterion.
3. Present/validate recovered Fracture telemetry.
4. Complete required anomaly-analysis calibration or evidence verification.
5. Meet Dr. Mara Venn.
6. Accept/record Asterion analysis lead.

Extraction:
- In-Place Resolution at Vesper Observatory.

Success Commit:
- Asterion relationship opened;
- Anomaly Physics branch becomes visible;
- Echo Laboratory lead unlocked.

Failure:
- no ordinary combat failure required by baseline;
- access/interaction blockers remain retryable.

## 9. MS-A202 — Security Line

Location:
- Bastion — Bastion Gate

Primary Type:
- Escort / Transport / Access Resolution

Zone Composition:
- Local Spaceflight;
- docking/security interface;
- limited station Interior where authorized.

Mandatory Objectives:

1. Approach Bastion Gate through authorized corridor.
2. Submit to or otherwise resolve route-security inspection.
3. Establish player identity and station independence.
4. Resolve one valid access branch:
   - Compliance;
   - Helios service/contract;
   - documented evidence/authorization;
   - later reconciliation path if hostile.
5. Secure recognized transit/access state.

Extraction:
- In-Place Resolution once access state commits.

Success Commit:
- Helios relationship formally opened;
- sealed Fracture-record lead becomes known.

Failure:
- local hostility/defeat can fail the current attempt;
- campaign retains reconciliation/alternate access path.

## 10. MS-A203 — Custodian Seven

Location:
- Orison — Custodian Vault Seven

Primary Type:
- Research / Anomaly Investigation

Secondary Types:
- Repair / Restoration;
- Recovery / Data Retrieval.

Zone Composition:
- Local Spaceflight;
- EVA/Interior automated facility.

Mandatory Objectives:

1. Reach Custodian Vault Seven.
2. Survey active Continuance security/containment state.
3. Restore or bypass one valid authorized service path.
4. Reach CUSTODIAN-7 interface.
5. Establish one valid Continuance relation state sufficient for data interaction.
6. recover/record Containment Directive lead.
7. Extract.

Extraction:
- Required.

Success Commit:
- CUSTODIAN-7 persistent relation state;
- Containment Directive Archive lead.

Failure:
- tactical failure retryable;
- successful authentication remains committed if explicitly completed before later failure.

## 11. MS-A204 — Echo Laboratory

Location:
- Nacre — Nacre Echo Laboratory

Primary Type:
- Research / Anomaly Investigation

Secondary Types:
- Repair / Restoration;
- Recovery / Data Retrieval.

Zone Composition:
- planetary/space installation Interior;
- anomaly hazard sub-zones.

Mandatory Objectives:

1. Survey laboratory perimeter/environment.
2. Restore safe enough access to required laboratory sections.
3. activate/repair required analysis system.
4. Acquire designated anomaly measurements/sample evidence.
5. Recover Anomaly Model Archive.
6. secure archive/evidence.
7. Extract.

Extraction:
- Required.

Success Commit:
- major Anomaly Physics Research Evidence;
- Reach III technology path availability;
- Anomaly Model Archive acquired.

Failure:
- archive remains unique;
- no reroll of anomaly sample/site.

## 12. MS-A301 — Rook's Wake

Location:
- Kestrel — Rook's Wake

Primary Type:
- Mixed Expedition

Branches:

### Negotiated Branch

Types:
- Recovery / Data Retrieval;
- dialogue/transaction.

Mandatory Objectives:
1. establish contact with Captain Sable Rook;
2. meet negotiated condition;
3. receive verified archive/source information;
4. leave safely.

### Hostile Branch

Types:
- Boarding Operation;
- Recovery / Data Retrieval.

Mandatory Objectives:
1. gain physical access to the designated Blackwake archive carrier/section;
2. recover archive/source information;
3. withdraw.

If the hostile route escalates into a full fortified-station assault, its assault wrapper is GDS-11 Raid authority.

Extraction:
- Required for hostile branch;
- negotiated branch resolves on safe completed handoff/departure.

Success Commit:
- stolen archive path/source;
- Captain Rook relationship state.

Failure:
- does not permanently remove mandatory archive from campaign;
- later negotiation, alternative archive source, or retry remains available.

## 13. MS-A302 — Sealed Authority

Location:
- Lyra — Authority Archive L-4

Primary Type:
- Recovery / Data Retrieval

Secondary Types:
- Reconnaissance;
- Repair / Restoration.

Access Branches:
- authorized Helios access;
- recovered service route;
- authored alternate access path.

Mandatory Objectives:

1. reach Authority Archive L-4;
2. obtain access to sealed emergency records;
3. verify archive integrity;
4. recover the required institutional record set;
5. secure the records;
6. Extract.

Extraction:
- Required.

Success Commit:
- institutional emergency evidence;
- associated Helios/Asterion reputation/story consequences.

Failure:
- archive remains unique and campaign-recoverable;
- hostile failure can require reconciliation/alternate route.

## 14. MS-A303 — The Lattice Equation

Location:
- Horizon Station

Primary Type:
- Research / Anomaly Investigation

Zone Composition:
- Home Station research/command facilities.

Hard Requirements:
- Lattice Telemetry Archive;
- Containment Directive Archive;
- Anomaly Model Archive.

Mandatory Objectives:

1. register all three evidence packages.
2. run the required station research/analysis project.
3. review SERA/Asterion/available Continuance correlation.
4. identify multi-causal Fracture mechanism.
5. derive Fracture Verge location/reach requirements.

Extraction:
- In-Place Resolution.

Success Commit:
- Fracture Verge coordinates;
- Reach IV research path;
- Act IV unlocked.

Failure:
- no loss of already owned evidence;
- interrupted research resumes under GDS-4 persistence.

## 15. MS-A401 — Beyond the Map

Location:
- Eidolon — Eidolon Deep Dock / Verge Observatory

Primary Type:
- Repair / Restoration / Expedition Commissioning

Secondary Type:
- Reconnaissance / Survey

Hard Requirements:
- Reach IV-capable ship;
- required navigation/research capability.

Mandatory Objectives:

1. arrive at Eidolon staging infrastructure.
2. validate Reach IV ship configuration.
3. load/verify required expedition support.
4. restore/calibrate Verge Observatory route solution.
5. Verify the Eidolon → Threshold Reach IV route.
6. commission the Verge expedition.

Extraction:
- In-Place Resolution at Eidolon.

Success Commit:
- Threshold route becomes Verified;
- MS-A402 unlocked.

Failure:
- does not consume unique Reach IV unlock;
- failed ship validation simply reports blockers.

## 16. MS-A402 — Verge Entry

Location:
- Threshold — Threshold Entry Zone

Primary Type:
- Reconnaissance / Survey

Secondary Type:
- Spaceflight survival.

Zone Composition:
- strategic Reach IV transit;
- Local Spaceflight anomaly zone.

Mandatory Objectives:

1. complete Reach IV transit to Threshold.
2. stabilize local flight/navigation.
3. survey required anomaly reference points.
4. deploy/activate navigation beacon or equivalent reference system.
5. establish a persistent safe staging solution.

Extraction:
- In-Place Resolution after stable staging point is committed.

Success Commit:
- Threshold staging location established;
- Null Choir route work unlocked.

Failure:
- stranding/recovery follows GDS-6/GDS-8;
- route can be retried after repair/resupply.

## 17. MS-A403 — The Older Signal

Location:
- Null Choir — Choir Structure

Primary Type:
- Research / Anomaly Investigation

Secondary Types:
- Recovery / Data Retrieval;
- EVA/Interior Exploration.

Mandatory Objectives:

1. reach Choir Structure.
2. identify valid entry/interface route.
3. survey non-human/Ancient systems.
4. collect mandatory Ancient/anomaly evidence.
5. recover or analyze the required Ancient Technology Core/data interaction.
6. identify Crownless Nexus relation.
7. Extract.

Extraction:
- Required.

Success Commit:
- Ancient Systems evidence;
- Crownless control-site coordinates;
- advanced Continuance/Anomaly knowledge.

Failure:
- Ancient evidence remains unique;
- site state persists;
- retry path remains.

## 18. MS-A404 — Wake Pattern

Location:
- Null Choir / Crownless observation route

Primary Type:
- Reconnaissance / Survey

Secondary Type:
- Research / Anomaly Investigation

Mandatory Objectives:

1. deploy/activate required observation package.
2. record synchronization at multiple required points.
3. correlate Transit Lattice wake pattern.
4. confirm active Second Cascade conditions.
5. recover/secure observation data.
6. withdraw from escalating hazard zone.

Extraction:
- Required.

Success Commit:
- Second Cascade crisis state;
- Coalition mission unlocked.

Failure:
- acquired measurements commit only when their Objective IDs complete;
- mission retry preserves completed persistent observation nodes where authored.

## 19. MS-F01 — Coalition

Location:
- Horizon Station

Primary Type:
- Mixed Strategic Preparation

Zone Composition:
- Home Station;
- remote/physical faction interactions.

Mandatory Objectives:

1. review Second Cascade response requirements.
2. select available coalition/support commitments.
3. satisfy minimum finale capability gate defined later by GDS-12.
4. prepare final expedition ship/loadout/robot capacity.
5. commit departure readiness.

Extraction:
- In-Place Resolution.

Success Commit:
- Finale becomes deployable.

Failure:
- no tactical fail state;
- insufficient capability creates explicit blockers, not mission failure.

## 20. MS-F02 — Second Cascade

Location:
- Crownless — Crownless Lattice Nexus

Primary Type:
- Mixed Expedition

Zone Composition:
- Reach IV transit;
- Local Spaceflight;
- EVA/boarding/Interior;
- final anomaly-control zone.

Mandatory Objective Phases:

### Phase A — Reach Crownless
- complete final route;
- survive/resolve local arrival blockers.

### Phase B — Gain Nexus Access
- create valid access to final control site.

Combat/raid implementation can use GDS-9/GDS-10/GDS-11.

### Phase C — Stabilize Control Path
- restore/secure required power/control/anomaly interfaces;
- reach final decision interface.

### Phase D — Final Resolution
Choose exactly one:
- Stabilize;
- Sever;
- Contain.

### Phase E — Survive/Complete Resolution
- commit chosen system transition;
- exit or reach authored safe resolution state.

Extraction:
- mission-specific final safe-resolution transition.

Success Commit:
- Postgame Resolution State;
- selected ending consequences;
- main campaign complete.

Failure:
- ordinary tactical defeat remains retryable;
- final choice is not committed until the explicit Final Resolution transaction;
- once committed, save/load cannot choose a second ending from the same post-choice state without returning to an earlier user-controlled save.

## 21. Story Mission Threat Ratings

Exact 0–5 axis values are authored per mission during balance/content pass.

They must remain within the GDS-7 sector threat envelope.

Threat values are tuneable; mission objective structure is fixed.

## 22. Story Mission Optional Content

Each mission may add optional:

- POIs;
- resources;
- salvage;
- logs;
- faction interactions;
- side objectives.

Optional generation cannot alter mandatory objective identity or story output.

## 23. Story Mission Persistence

Each main-story Mission ID is unique and persistent.

A completed main-story Mission ID never re-enters Available state.

Failed attempts use retry state associated with the same canonical story mission rather than creating alternate duplicate story IDs.

## 24. Explicit Non-Goals

This document does not define:

- weapon damage;
- enemy AI;
- robot squad behavior;
- full station-raid phases;
- final economy values.

Those are later authorities.

## 25. Dependencies

This specification depends on GDS-7 Story Missions/World Map, Mission System, Objectives, Zones, Extraction, Failure, GDS-4 Research, GDS-5 Player, GDS-6 Spacecraft, and future Combat/Robots/Raids/Progression.

## 26. Open Questions

None at the GDS-8 story-mission execution-contract level.

Encounter-level combat/raid details remain owned by their later authoritative domains.
