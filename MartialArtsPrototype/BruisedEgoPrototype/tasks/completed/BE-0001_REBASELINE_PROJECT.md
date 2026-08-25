# BE-0001 — rebaseline the live Unreal project

Status: `ACCEPTED`  
Owner: `Codex`  
Reviewer: `Human`  
Created: `2026-08-19`

## Goal

Replace recorded/snapshot assumptions with an exact, reproducible baseline from the live `BruisedEgoPrototype` repository without changing game behaviour.

## Why now

The operating files were prepared outside the mounted Unreal repository. The live rebaseline prevents Codex and Claude from implementing against stale files, ambiguous short asset names, guessed paths, or the removed keyboard `J` test.

## Known-good baseline

- The live project resolves to Unreal Engine 5.8.1 at `H:\Epic Games\UE_5.8`.
- The six known combat C++ files match the recorded implementation and have no Git diff.
- `BruisedEgoPrototypeEditor Win64 Development` last built successfully after the newest live source timestamp: **COMPILES**.
- The user has **PLAYTEST-VERIFIED** that left mouse plays the jab, physical separation can miss, genuine contact logs once per window, and the temporary shove occurs.
- The latest preserved PIE log verifies `L_CombatGym`, the `hand_l` socket override, and repeated contact with `BP_ThirdPersonCharacter_C_1`.
- See `docs/CURRENT_STATE.md` and `evidence/BE-0001/`.

## Allowed changes

- `AGENTS.md` and the merged root `CLAUDE.md` for one-time operating-file installation.
- `docs/CURRENT_STATE.md`.
- `docs/UE_ASSET_MAP.md`.
- This task file and documentation consistency fixes.
- Evidence files under `evidence/BE-0001/`.
- A future `scripts/verify.ps1` only after a full build command is deliberately rerun and accepted.

## Non-goals

- No C++ behaviour changes.
- No Blueprint, animation, montage, Physics Asset, collision, plugin, project-setting, Config, or `.uproject` changes.
- No cleanup/refactor of temporary logs or the contact shove.
- No active-ragdoll/Physics Control implementation.
- No deletion or relocation of the original operating-files wrapper bundle.

## Resolved discrepancies

- `JabAction` remains in live C++; only the keyboard `J` binding was removed.
- The decoded live mapping is `IA_Jab → LeftMouseButton` and `IA_FightingStance → RightMouseButton`.
- The active montage is `/Game/Characters/Animations/Combat/AM_Jab`, not the older same-named asset under `Mannequins/Anims/Unarmed/Attack`.
- The active montage instance uses `hand_l`; the C++ class default remains `fist_l`.
- The project defaults to `Lvl_ThirdPerson`, while the latest combat PIE session used `L_CombatGym`.
- Physics Control is mounted by the current Editor installation but is not a project C++ dependency or explicit `.uproject` plugin.

## Acceptance criteria

- [x] Repository root, project root, branch/commit, and pre-existing dirty state are recorded without discarding user changes.
- [x] `.uproject` EngineAssociation, installed engine version, modules, targets, dependencies, and explicit plugins are recorded.
- [x] The live versions/paths of all six known C++ files are recorded and compared with the supplied snapshot.
- [x] Current input path is recorded; the keyboard `J` route is absent and the C++ `JabAction` route remains.
- [x] The duplicate `AM_Jab` assets and active Blueprint → montage → sequence dependency are distinguished.
- [x] Exact **Copy Reference** output for the recorded assets is pasted into `evidence/BE-0001/asset_references.md`.
- [x] AnimGraph, montage, layered blend, Control Rig, and root-motion relationships are checked in the live Editor.
- [x] Player and defender mesh/Physics Asset assignments, key bone/socket names, and collision setup are recorded from the Editor.
- [x] One baseline build succeeded with exact target/result recorded and its artifact shown newer than all live source.
- [x] One fresh BE-0001 PIE jab/contact test is captured without changing behaviour.
- [x] The missing `SKM_Manny` reference is resolved or shown not to affect the active path.
- [x] Human accepts the baseline and the task reaches its final evidence status.

## Verification completed

### Repository and source

- Git root: `E:\MarshallForgeGames`.
- Project root: `E:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype`.
- Baseline branch/commit: `main` at `3b113f528814adb19770c9a48c5ef5a168370881`.
- Live source inspection: complete; no C++ diff.
- Operating-file installation: documentation only; Unreal files untouched.

### Build

- Recorded UBT invocation:
  `"H:\Epic Games\UE_5.8\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll" BruisedEgoPrototypeEditor Win64 Development E:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype\BruisedEgoPrototype.uproject`
- Result: `Succeeded` in 13.27 seconds on 2026-08-16.
- Built DLL timestamp: 2026-08-16 16:00:24 +10:00.
- Newest live source timestamp: 2026-08-16 15:58:42 +10:00.
- Current full rebuild was not launched because Unreal Editor and Live Coding were running.
- See `evidence/BE-0001/build_summary.md`.

### Live Editor and asset inspection

- Exact class-qualified Copy Reference strings are recorded for the map, Blueprint, Bob's actual mesh, Animation Blueprint, skeleton, both `AM_Jab` assets, sequences, input assets, and Physics Asset.
- The active input, montage, AnimGraph, root-motion, character assignment, collision, and Physics Asset setup were inspected directly without saving assets.
- See `evidence/BE-0001/asset_references.md` and `evidence/BE-0001/editor_inspection.md`.

### Fresh PIE evidence

- Fresh session: `2026-08-19 09:14:13`–`09:21:58 UTC` in `/Game/Maps/L_CombatGym.L_CombatGym`.
- Initial/range and post-shove windows missed cleanly.
- The in-range window logged exactly one `Fist contact: BP_ThirdPersonCharacter_C_1` across eight ticks and reported `hand_l` present.
- Bob shifted approximately `+16.95 cm` from the saved X position after the contact; subsequent jabs missed after separation.
- The stale missing `SKM_Manny.SKM_Manny` warning did not recur in the fresh session; active actors resolved to Quinn, and the separate Manny reference actor uses valid `SKM_Manny_Simple`.
- See `evidence/BE-0001/playtest.md`.

## Human acceptance

- Decision: **APPROVED** by the human reviewer on 2026-08-19.
- User statement: `Approve BE-0001 baseline`.
- The user-approved exact reload of `SK_Mannequin` completed successfully; both dirty-package lists were empty afterward and the disk hash was unchanged.
- See `evidence/BE-0001/approval.md`.

## Evidence

- `evidence/BE-0001/environment.md`
- `evidence/BE-0001/build_summary.md`
- `evidence/BE-0001/asset_references.md`
- `evidence/BE-0001/editor_inspection.md`
- `evidence/BE-0001/fresh_pie_log_excerpt.md`
- `evidence/BE-0001/playtest.md`
- `evidence/BE-0001/approval.md`
- `evidence/BE-0001/screenshots/`
- Full raw Unreal/UBT logs remain local because `Saved/` and `*.log` are ignored.

## Handoff

- Evidence labels reached: **COMPILES** for source/build, **EDITOR-VERIFIED** for the live asset baseline and fresh task pass, and **PLAYTEST-VERIFIED** for the user's accepted jab/contact behaviour.
- Live repository root: `E:\MarshallForgeGames`.
- Git state: dirty user assets and untracked content preserved; documentation-only additions made.
- Engine/target: Unreal Engine 5.8.1; `BruisedEgoPrototypeEditor Win64 Development`.
- Snapshot discrepancies: keyboard `J` removed but `JabAction` retained; active montage and socket override identified.
- Asset paths captured: exact live Editor Copy Reference output complete.
- Remaining gates: none for BE-0001. Bone-level physical response and measured impact physics remain future implementation work, not BE-0001 gaps.
- Human acceptance: **APPROVED** on 2026-08-19.
- Suggested next task: scope an instrumented, bone-aware physical-response spike; no implementation has started.
