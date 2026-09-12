# TA-12 — Profile and Application Settings Persistence

> **Status:** Architecture Complete  
> **Authority:** On-disk persistence of controls, accessibility, HUD, audio, display and application preferences separately from gameplay saves

## 1. Purpose

TA-11 deliberately separates presentation/input preferences from campaign gameplay state. TA-12 defines how those preferences persist without coupling a world save to one device, controller layout, subtitle preference, or graphics setup.

## 2. Separate Persistence Domain

Profile/application settings are stored outside campaign `.sfgsave` files.

Loading a campaign under another valid profile/settings configuration must not alter:

- world state;
- rewards;
- ownership;
- difficulty/save-owned gameplay settings;
- mission/progression state.

## 3. Canonical Profile File

Baseline profile persistence uses a project-owned versioned UTF-8 JSON document because the data is small, non-gameplay-authoritative, human-diagnosable configuration.

Conceptual path:

```text
<UserData>/StarForge/profiles/default/profile.sfprofile.json
```

The exact profile naming/multi-profile UX can remain implementation policy; one local default profile is sufficient for baseline.

## 4. Profile Envelope

Top-level structure conceptually:

```json
{
  "schema": "starforge.profile",
  "schema_version": 1,
  "defaults_revision": 1,
  "input": {},
  "accessibility": {},
  "hud": {},
  "audio": {},
  "display": {},
  "presentation": {}
}
```

The profile contains no gameplay Persistent IDs or campaign inventory/progression state.

## 5. Settings Ownership

Profile categories can persist:

- keyboard/mouse/controller bindings;
- sensitivities/deadzones/inversion;
- Hold/Toggle/Auto-Sprint/input assistance preferences;
- aim-assist presentation/input coefficients allowed by TA-11;
- UI/text scale and safe-area preferences;
- subtitle/Closed Caption settings;
- High Contrast/color-vision/readability settings;
- Reduced Motion/Effects/Photosensitivity settings;
- HUD mode/reticle presentation;
- audio volumes/dynamic range/mono/dialogue boost;
- haptic enable/intensity;
- graphics/display preferences;
- non-gameplay notification/tutorial preferences where appropriate.

## 6. Save-Owned Settings Exclusion

Settings that change authoritative gameplay simulation and are designated save-owned by the GDS do not live only in the profile.

Difficulty and Custom Difficulty axes persist with campaign state according to their owning domain/session section.

A profile can remember a preferred default for **new games**, but loading an existing save uses the save-owned gameplay value.

## 7. Profile Schema Version

`schema_version` is independent from:

- Save ContainerVersion;
- ApplicationSaveCompatibilityVersion;
- domain save SchemaVersions;
- ContentBuildId.

A profile format update cannot invalidate a campaign save.

## 8. Current-Version Validation

Within a supported profile schema:

- field types are validated;
- numeric values are bounded/clamped to documented safe ranges;
- binding records are validated through TA-11 conflict/action registries;
- invalid enum values use category defaults plus diagnostics;
- malformed individual optional settings can fall back without discarding unrelated valid categories.

Because profile data is not gameplay authority, fail-soft per-setting recovery is permitted.

## 9. Missing Fields

A missing known field receives the current project default for that setting.

This supports addition of new presentation settings without requiring every old profile to be rewritten before launch.

## 10. Unknown Fields

For a **supported current schema version**, unknown fields are ignored with development diagnostics rather than causing campaign failure.

They are preserved only if the implementation deliberately supports round-trip preservation; baseline does not require preservation.

A typo therefore can be diagnosed, but it cannot become executable/gameplay semantics.

## 11. Newer Profile Schema

If `profile.schema_version` is newer than the executable supports:

- campaign saves remain unaffected;
- application creates an in-memory validated default settings snapshot;
- original newer profile file is left untouched;
- UI warns that profile settings could not be loaded by this application version;
- writing back does not overwrite the newer file unless user explicitly resets/creates a compatible profile.

## 12. Historical Profile Migration

Supported older profile versions migrate deterministically through explicit small migration steps.

Profile migration may:

- rename setting keys;
- convert ranges/units;
- split/merge categories;
- map old binding representations to current ActionIds.

It cannot change campaign gameplay state.

## 13. Default Revision

`defaults_revision` identifies the project default set used when the profile was first created/reset.

A game update that changes defaults does **not** overwrite existing user customizations automatically.

New settings absent from the profile receive current defaults.

`Reset to Default` intentionally writes current defaults and updates the relevant revision/category.

## 14. Bindings Serialization

Input bindings serialize semantic ActionId identity and physical device control identity using project-defined stable textual/numeric representations.

They never serialize:

- GLFW runtime device pointer;
- transient connected-controller instance ID as long-term binding identity;
- gameplay Command objects.

## 15. Device-Specific Preference Boundary

Per-device-family settings can exist, but a disconnected device does not invalidate the profile.

Unsupported haptics/controller capabilities simply become inactive presentation settings until compatible hardware is available.

## 16. Atomic Profile Writes

Profile writes use the same high-level crash-safety principle as saves:

1. serialize validated complete profile candidate;
2. write unique pending file in profile directory;
3. flush/close;
4. parse/validate written candidate;
5. atomically replace/rename the active profile file while retaining recoverable previous generation/backup according to backend policy;
6. only then publish persistence success.

Unlike gameplay saves, profile files can use a compact latest-file model because loss affects preferences rather than world state, but the previous valid profile must not be destroyed before the candidate is valid.

## 17. Profile Backup

Baseline retains at least one previous valid profile generation/backup or enough atomic replacement semantics to recover from interrupted write.

On startup:

- active profile valid -> use it;
- active invalid + previous valid backup -> offer/use previous with diagnostic;
- none valid -> use project defaults and do not affect campaign saves.

## 18. Live Settings Apply vs Disk Persistence

Applying a setting and persisting it are distinct operations:

```text
SettingsChangeRequest -> validated SettingsSnapshot -> live apply
                                              |
                                              +-> async/short profile persistence
```

If disk persistence fails:

- current session may continue using the validated live setting;
- UI reports the preference could not be saved;
- gameplay state remains unaffected.

## 19. Display Preview/Revert

Risky display settings can use TA-11 preview/revert before final profile commit.

Only the confirmed effective display setting is persisted as current preference.

A crash during preview must not permanently commit an unconfirmed unusable display mode.

## 20. Secrets

Profile persistence contains no account credentials, OAuth tokens, passwords, or private service secrets.

If future online services introduce credentials, they require a separate secure credential-storage architecture.

## 21. Localization

Language/localization preference can live in profile/application settings.

Changing language affects presentation only and never changes ContentId/PersistentId/save identity.

## 22. Last-Played Convenience State

Profile/application state may store:

- last opened campaign storage ID;
- last selected committed save locator;
- menu preferences.

These are convenience pointers only. Missing/invalid values trigger catalog discovery, not gameplay repair.

## 23. Profile Diagnostics

Diagnostics report:

- profile schema/default revision;
- loaded/migrated/defaulted categories;
- invalid/clamped fields;
- binding conflicts/fallbacks;
- write/backup recovery state.

Sensitive user file paths can be normalized/redacted in user-facing logs where appropriate.

## 24. Tests

Tests must cover:

- campaign save unaffected by profile corruption;
- missing new field receives default;
- invalid numeric field clamps/defaults safely;
- old profile migrates deterministically;
- newer unsupported profile does not get overwritten automatically;
- custom binding survives default revision update;
- Reset to Default intentionally changes selected category;
- display preview crash restores confirmed profile value;
- profile write fault leaves previous valid profile usable;
- haptics/audio/UI settings cannot change gameplay simulation results.

## 25. Explicit Non-Goals

No baseline requirement for:

- cloud profile sync;
- platform account profiles;
- profile settings embedded in every save;
- profile corruption invalidating a campaign;
- arbitrary macros executing multiple gameplay actions;
- secrets/credentials in JSON profile data.

## 26. Dependencies

Depends on TA-11 settings/input/accessibility architecture, TA-12 storage backend principles, and GDS Settings Boundary.

## 27. Open Questions

None.
