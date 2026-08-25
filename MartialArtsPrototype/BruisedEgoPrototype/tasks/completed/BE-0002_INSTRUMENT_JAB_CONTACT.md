# BE-0002 — instrument jab contact samples

Status: `COMPLETED — HUMAN APPROVED 2026-08-19`  
Owner: `Claude`  
Reviewer: `Codex`  
Final acceptance: `Human`  
Created: `2026-08-19`

## Goal

For every currently accepted jab contact, emit exactly one stable, unit-labelled contact sample that identifies the selected hit and measures the fist's frame-to-frame path speed, without changing hit selection, montage playback, miss behaviour, or the existing temporary `250 cm/s` character shove.

This task establishes trustworthy input evidence for a later bone-aware physical-response spike. It does not implement that response.

## Why now

BE-0001 proved genuine spatial contact and documented Bob's collision/Physics Asset setup, but the accepted sweep may currently select the character capsule rather than a named skeletal body. Applying Physics Control or an impulse before measuring the selected component, bone, timing, and path speed would mix detection, impact measurement, and response around an unverified assumption.

## Known-good baseline

- BE-0001 was human-accepted on 2026-08-19.
- The active jab uses a `6 cm` swept sphere from the previous to current `hand_l` location and queries Pawn objects while ignoring the owner.
- The first hit actor processed in a notify window logs `Fist contact:` and receives a temporary horizontal `LaunchCharacter` shove of `250 cm/s`.
- One processed target per window, physical range/lateral misses, left-click jab, right-click stance, and the shove are **PLAYTEST-VERIFIED**.
- Bob's capsule and mesh are both Pawn objects; his mesh is Query Only, does not simulate physics, and uses `PA_Mannequin` through `SKM_Quinn_Simple`.
- Evidence: `docs/CURRENT_STATE.md` and `evidence/BE-0001/`.

## Allowed changes

Claude exclusively owns these implementation files while the task is active:

- `Source/BruisedEgoPrototype/BECombatComponent.h`
- `Source/BruisedEgoPrototype/BECombatComponent.cpp`
- `Source/BruisedEgoPrototype/BEAnimNotifyState_JabWindow.cpp`, only as needed to pass `FrameDeltaTime`

Codex owns task/review/evidence documentation after Claude's implementation handoff:

- This task file.
- `evidence/BE-0002/`.
- `docs/CURRENT_STATE.md` when verified evidence changes.

## Non-goals

- No impulse, torque, force, physical animation, active ragdoll, Physics Control, bone simulation, balance, stagger, fall, or recovery response.
- No change to `LaunchCharacter` or its `250 cm/s` calibration value.
- No change to sweep radius, object query, hit ordering, one-target gating, montage/notify timing, or the active `hand_l` override.
- No collision-profile, Physics Asset, body, constraint, Blueprint, map, montage, sequence, input, Config, `.uproject`, plugin, module, or Build.cs change.
- No damage, injury, stamina, cross, guard, AI, networking, or broad combat framework.
- No cleanup of existing debug drawing or diagnostic markers.

If a forbidden change appears necessary, Claude must stop and record the reason rather than expand the task.

## Required contact schema

Use stable versioned markers suitable for log filtering, such as `BE_CONTACT_SAMPLE_V1` and `BE_CONTACT_WINDOW_V1`.

Each accepted-contact sample must record:

- Contact-window ID.
- Candidate count and skeletal-mesh candidate count for the selected sweep tick.
- Selected actor, component name, and component class.
- The selected `FHitResult::BoneName`, or explicit `None`.
- Sweep start/end positions in centimetres and radius in centimetres.
- Notify tick delta in seconds.
- Derived fist path-speed estimate in `cm/s`.
- Impact point in centimetres and impact normal as a unitless direction.
- Whether the selected component/body is simulating physics.
- Raw body mass in kilograms only when a valid selected skeletal body supplies it, including the source; otherwise explicit `Unavailable`.

The implementation must call speed a kinematic/path-speed estimate. It must not call raw body mass effective impact mass or derive force, impulse, torque, energy, damage, or response magnitude from these values.

Candidate inspection may count or describe results but must not reorder or replace the hit currently selected by the existing loop.

## Assumptions and unknowns

| Item | Assumption or unknown | Resolution in this task |
| --- | --- | --- |
| Selected component | The current first processed hit may be Bob's capsule rather than his mesh. | Log the exact selected component/class without changing selection. |
| Bone/body | `BoneName` and body mass may be unavailable on the selected hit. | Record explicit `None`/`Unavailable`; do not invent a fallback. |
| Frame sensitivity | Frame-to-frame path-speed estimates may differ between 30 and 60 FPS. | Capture actual values at both caps; do not force an invented tolerance. |
| Invalid delta | Notify tick delta should normally be positive and finite. | Guard division and emit explicit unavailable speed for invalid input. |

## Acceptance criteria

- [x] Only the allowed source files change; pre-existing dirty Unreal assets remain untouched.
- [x] One stable contact sample is emitted for the first currently processed target in a hit window.
- [x] Every window emits one stable hit/miss summary; a miss emits no contact sample.
- [x] Every sample contains the required fields and explicit units or an explicit unavailable value.
- [x] Delta time and derived speed are finite and non-negative when reported; zero/invalid delta cannot divide by zero or fabricate a value.
- [x] Missing component, bone, body, or mass information is handled without a crash.
- [x] Candidate inspection does not alter hit ordering, selected actor, or one-target-per-window behaviour.
- [x] `BruisedEgoPrototypeEditor Win64 Development` builds successfully using the recorded UE 5.8.1 installation.
- [ ] At least one range miss and one lateral miss in `L_CombatGym` each produce a miss summary and zero contact samples.
- [x] Three in-range jabs at 30 FPS and three at 60 FPS each produce exactly one sample per hit window; actual measurements are recorded.
- [ ] Left-click jab, right-click stance, montage/notify path, genuine-contact gating, and visible temporary shove remain unchanged.
- [x] No Unreal asset, map, Config, project setting, or plugin/module setting is saved or changed.
- [x] Codex completes an independent review with no unresolved blocking correctness or scope findings.
- [x] The human reproduces the PIE scenario and accepts the result.

## Verification plan

### Automated/build

- Target: `BruisedEgoPrototypeEditor Win64 Development`.
- Engine: `H:\Epic Games\UE_5.8` (UE 5.8.1).
- Build only after Unreal Editor and Live Coding are closed.
- Confirm no source/config/project files outside the allowed list changed.
- Filter the implementation for stable markers, unit labels, finite-value guards, and unchanged `LaunchCharacter(... * 250.0f, true, false)` behaviour.

### Unreal Editor

1. Open `/Game/Maps/L_CombatGym.L_CombatGym` in UE 5.8.1.
2. Do not save any asset.
3. Clear/filter Output Log for `BE_CONTACT_`, `Fist contact:`, `Jab Window`, and `Fighting Stance`.
4. Run the named 30/60 FPS PIE matrix below.

### Playtest matrix

At both `t.MaxFPS 30` and `t.MaxFPS 60`:

1. Three in-range jabs: one sample and one hit summary per contact window, with visible shove.
2. One out-of-range jab: zero samples and one miss summary.
3. One lateral miss: zero samples and one miss summary.
4. Confirm right mouse still toggles stance and left mouse still plays the active jab.

### Regression checks

- Physical separation still causes misses.
- One target maximum is processed per notify window.
- Existing `Fist contact:` marker and temporary shove remain.
- No Editor package becomes dirty from the test.

## Evidence required

- `evidence/BE-0002/build_summary.md` — exact command, target, result, warnings, and local raw-log pointer.
- `evidence/BE-0002/contact_samples.md` — concise 30/60 FPS hit/miss excerpts, counts, actual selected component/bone/mass availability, and measurements.
- `evidence/BE-0002/review.md` — Codex findings, terminology/unit review, regression comparison, and resolutions.
- `evidence/BE-0002/playtest.md` — human setup, repetitions, expected/actual behaviour, no-save confirmation, and result.
- `evidence/BE-0002/approval.md` — human acceptance statement and date.

Raw `*.log` files remain local because repository policy ignores them.

## Implementation notes

Claude implemented the source-only telemetry in the three allowed files. The contact component now owns a monotonic per-component window ID and window counters; notify tick delta is passed unchanged into the sweep update; the selected raw hit emits `BE_CONTACT_SAMPLE_V1`; and every active window emits `BE_CONTACT_WINDOW_V1` with hit/miss and sample count.

The implementation reports a kinematic path-speed estimate only after finite-value validation. Named-body simulation and raw mass require an exact skeletal hit bone plus a valid body instance. Missing information remains explicit `Unavailable`. No physical response, collision, module, plugin, or asset change was added.

## Review findings

Codex returned the initial telemetry correctness findings to Claude for correction and then reviewed the stable corrected diff against local UE 5.8.1 headers. Final result: **no unresolved findings**. See `evidence/BE-0002/review.md`. Build and runtime findings remain pending.

## Handoff

- Evidence label reached: **PLAYTEST-VERIFIED**; the UE 5.8.1 Editor Development target succeeded and the 30/60 FPS matrix was captured on 2026-08-19. Human approval was recorded in `evidence/BE-0002/approval.md`; this task is archived.
- Files changed: the three allowed C++ files plus Codex-owned task/review/build-status documentation; no Unreal assets.
- Verification run: stable diff review, exact local UE 5.8.1 API inspection, scope/preservation searches, and `git diff --check`; no unresolved findings.
- Warnings: selected component/bone is still a runtime unknown and must remain an observed result. LF-to-CRLF advisories are informational.
- Manual steps remaining: none. Human approval recorded in `evidence/BE-0002/approval.md`.
- Risks/unknowns: frame sensitivity and capsule-versus-skeletal selection.
- Suggested follow-up after acceptance: scope the measured bone/body-resolution and local physical-response spike; do not start it automatically.
