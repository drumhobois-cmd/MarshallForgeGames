# Bruised Ego — current implementation state

Baseline date: **2026-08-25**  
Active task: **None — BE-0002 and BE-0003 are completed and archived**  
Highest current evidence: **PLAYTEST-VERIFIED** for the BE-0003 named-body upper-body reaction, fallback, restoration/teardown, and 30/60 FPS matrix.

This file records only facts checked against the live project. Verification maturity uses the canonical labels in `AGENTS.md`. Provenance—live source, serialized asset reference, Editor log, or user playtest—is stated separately.

## Environment

| Item | Verified value | Provenance |
| --- | --- | --- |
| Product | Bruised Ego | Approved project intent |
| Project root | `E:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype` | Live filesystem |
| Git root | `E:\MarshallForgeGames` | `git rev-parse --show-toplevel` |
| Baseline branch/commit | `main` at `3b113f528814adb19770c9a48c5ef5a168370881` | Live Git inspection before operating-file installation |
| Pre-existing dirty state | Modified `ABP_Unarmed.uasset`, `L_CombatGym.umap`, and `BP_ThirdPersonCharacter.uasset`; untracked combat/custom animation folders and operating bundle; unrelated untracked repository siblings `Frames/`, `MoCap/`, and `RiggedCharacters/` | Live Git inspection; preserved |
| Unreal association | `{E70D4010-44D5-A15F-C9F1-72982D6C6314}` → `H:\Epic Games\UE_5.8` | Live `.uproject` plus HKCU registered-build mapping |
| Installed engine | Unreal Engine 5.8.1, CL 56057345, `++UE5+Release-5.8` | Local `Engine\Build\Build.version` |
| Runtime module | `BruisedEgoPrototype` | Live `.uproject` |
| Targets | `BruisedEgoPrototype` (Game) and `BruisedEgoPrototypeEditor` (Editor), BuildSettings V7 | Live target files |
| Module dependencies | Core, CoreUObject, Engine, InputCore, EnhancedInput | Live `BruisedEgoPrototype.Build.cs` |
| Explicit project plugins | Modeling Tools Editor Mode; Gameplay StateTree | Live `.uproject` |
| Mounted in latest Editor log | Enhanced Input, Control Rig, Physics Control, Control Rig Physics, among other engine plugins | `Saved\Logs\BruisedEgoPrototype.log`; mounting does not mean the project C++ module depends on Physics Control |
| Latest source build | Human reported the BE-0003 fix rebuilt successfully on 2026-08-25; the updated code was then exercised in PIE | Exact build command/result: **NOT RECORDED**. The prior documented editor build remains **COMPILES**; the fresh fixed code is **PLAYTEST-VERIFIED** through its runtime telemetry. |

## Preserved project state

No gameplay source, configuration, `.uproject`, map, Blueprint, montage, sequence, or other Unreal binary asset file was saved or changed on disk by BE-0001. The operating files, evidence notes, and resized Editor screenshots are documentation/evidence-only additions.

The preview inspection temporarily left `SK_Mannequin` dirty in memory. With explicit user approval, Unreal reloaded exactly that package from disk. The reload succeeded, both Editor dirty-package lists were empty afterward, and the file's before/after SHA-256 was identical. No preview-only change remains.

The configured startup and game-default map remains:

`/Game/ThirdPerson/Lvl_ThirdPerson.Lvl_ThirdPerson`

The latest relevant PIE session instead ran:

`/Game/Maps/L_CombatGym.L_CombatGym`

## Current input path

The live input path is:

`LeftMouseButton` → `/Game/Input/Actions/IA_Jab` → `ABECharacter::JabAction` → `OnJabStarted()` → `UBECombatComponent::Jab()` → `/Game/Characters/Animations/Combat/AM_Jab`

Fighting stance is:

`RightMouseButton` → `/Game/Input/Actions/IA_FightingStance` → `ABECharacter::FightingStanceAction` → `ToggleFightingStance()`

- `JabAction` and `OnJabStarted()` remain intentionally present in C++.
- The old keyboard `J` binding is absent from the decoded live Enhanced Input mapping records and from project Config/C++; only the keyboard test route was removed.
- `DefaultInput.ini` selects `EnhancedPlayerInput` and `EnhancedInputComponent` and contains no legacy jab action mapping.
- `Jab()` currently has no fighting-stance gate, so the assigned montage may play while stance is off.
- The live Enhanced Input editor directly shows left mouse → `IA_Jab`, right mouse → `IA_FightingStance`, and no keyboard `J` row.

## Live C++ baseline

### `ABECharacter`

- Creates one native `UBECombatComponent` named `CombatComponent`.
- Exposes `IsInFightingStance()`.
- Binds the Blueprint-assigned fighting-stance and jab Input Actions on `ETriggerEvent::Started`.
- Forwards jab input to `CombatComponent->Jab()`.

### `UBECombatComponent`

- Tracks fighting stance and can enter, exit, or toggle it.
- Holds a Blueprint-editable `JabMontage` and calls `Montage_Play` on the owning character's AnimInstance.
- `BeginFistSweep` stores the fist location and opens a one-hit window.
- `UpdateFistSweep` performs a sphere sweep from the previous to current fist position:
  - radius: `6.0` Unreal units (centimetres in the default Unreal scale);
  - object type: `ECC_Pawn`;
  - owner ignored;
  - only the first actor per window is processed;
  - a character target receives a temporary horizontal `LaunchCharacter` shove of `250.0 cm/s`.
- `EndFistSweep` closes the window and resets its one-hit state.
- BE-0002 adds one versioned `BE_CONTACT_SAMPLE_V1` record for the accepted hit and one `BE_CONTACT_WINDOW_V1` hit/miss summary per active window.
- The sample records the raw selected hit, unit-labelled sweep geometry and tick delta, a validated frame-to-frame fist path-speed estimate, component/named-body simulation state, and valid named-body mass or an explicit unavailable reason.
- Candidate inspection preserves the original hit-array order and the existing first-valid-actor selection.
- The shove is temporary calibration behaviour, not the intended physical-response architecture.

### `UBEAnimNotifyState_JabWindow`

- C++ default socket: `fist_l`.
- Calls `BeginFistSweep`, `UpdateFistSweep`, and `EndFistSweep` across the notify state, passing the notify tick's `FrameDeltaTime` unchanged into the sweep update.
- Retains temporary diagnostic logging.
- The active montage instance directly shows `hand_l`, proving the editor-facing property overrides the C++ default. `hand_l` is a bone; `fist_l` is an existing socket below it.

## Live asset relationship

The modified player Blueprint references the new combat montage, and the montage references the new sequence:

`/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter`  
→ `/Game/Characters/Animations/Combat/AM_Jab`  
→ `/Game/Characters/Animations/Combat/AS_Jab_v01`

The montage and sequence are currently untracked while the referencing Blueprint is tracked-but-modified. They form one dependency chain and must not be separated in a future commit.

A second, older montage also exists:

`/Game/Characters/Mannequins/Anims/Unarmed/Attack/AM_Jab`

It references `MM_Attack_01` and is not the montage serialized into the current player Blueprint.

## Current verified behaviour

| Behaviour | Verification status | Provenance and limits |
| --- | --- | --- |
| Mouse click plays the imported jab through `ABP_Unarmed` | **PLAYTEST-VERIFIED** | User PIE report; direct input/montage/AnimGraph inspection; fresh PIE window from left-click |
| Jab contact is driven by a timed notify-state fist sweep | **PLAYTEST-VERIFIED** | User PIE report, live source, direct montage inspection, and fresh PIE log |
| Range/lateral separation can make the jab miss | **PLAYTEST-VERIFIED** | Prior user range/lateral test; fresh task pass also produced range and post-shove misses |
| One target is processed per notify window | **PLAYTEST-VERIFIED** | User acceptance plus fresh hit window containing exactly one contact across eight ticks |
| Contact with the second character is logged | **EDITOR-VERIFIED** | Fresh `L_CombatGym` session records exactly one `Fist contact: BP_ThirdPersonCharacter_C_1` in the hit window |
| BE-0002 telemetry smoke hit | **PLAYTEST-VERIFIED** | User smoke run recorded exactly one `BE_CONTACT_SAMPLE_V1` and one matching `BE_CONTACT_WINDOW_V1`; selected component was the capsule, bone/body/mass unavailable |
| Current Bob reaction is a generic horizontal character shove | **PLAYTEST-VERIFIED** | Prior user observation plus fresh approximately `+16.95 cm` runtime displacement; source uses `LaunchCharacter`, not bone physics |
| BE-0003 Physics Asset body resolution | **PLAYTEST-VERIFIED** | After the sweep selected Bob's capsule (`SkeletalCandidateHitCount=0`), exact Physics Asset surface resolution selected `spine_05` at 16.5–18.2 cm. Proximity, upper-chain membership, and body validity were telemetry-confirmed. This is not a direct skeletal-mesh sweep hit. |
| BE-0003 bounded upper-body physics reaction | **PLAYTEST-VERIFIED** | A visible reaction occurred in the 30/60 FPS runs. It is a temporary fixed `200 cm/s`, mass-independent velocity change, not a measured impact response. |
| BE-0003 restoration and teardown | **PLAYTEST-VERIFIED** | Every completed fresh response logged exactly one `Phase=Restored`; an active-response PIE stop produced no `Invalid Bodies` warning, extra restoration, or crash. |
| BE-0003 miss produces no response | **PLAYTEST-VERIFIED** | WindowId=1 MISS; no `BE_UPPER_BODY_RESPONSE_V1` Applied entry emitted |
| BE-0003 30/60 FPS response matrix | **PLAYTEST-VERIFIED** | At each cap, one range miss (WindowId 1), three exact-PA-geometry `spine_05` responses (WindowIds 2–4), and one lateral miss (WindowId 5) were recorded. Each hit applied and restored the 0.40 s response; no crash or teardown warning was recorded. See `evidence/BE-0003/review.md`. |
| Cross and guard | **PROPOSED** | Target-slice goals only; no live implementation identified |

## Animation, physics, and collision facts

- `ABP_Unarmed` evaluates `Main States` → upper-body `Layered Blend per Bone` from `spine_01` depth `1` → `DefaultGroup.DefaultSlot` → `CR_Mannequin_FootIK` → output.
- `AS_Jab_v01` has root motion disabled; the Animation Blueprint mode is `Root Motion from Montages Only`.
- Player and Bob both use `SKM_Quinn_Simple`, `ABP_Unarmed_C`, and `PA_Mannequin` through the mesh asset; mesh Physics Asset Override is `None`.
- Bob's mesh is Query Only at rest, object type Pawn, and ignores Pawn/Visibility/Vehicle/Camera. The character capsule is Query and Physics, object type Pawn, with Visibility and Camera ignored.
- BE-0003 runtime sequence: `SetCollisionEnabled(QueryAndPhysics)` → `RecreatePhysicsState()` transitions the mesh to simulation-capable; `SetAllBodiesBelowSimulatePhysics(spine_03, true, true)` then activates the upper-body chain. Restoration reverses this. The mesh response channels remain unchanged (still Ignore for all), so the fist sweep continues hitting only the capsule.
- `PA_Mannequin` has 22 bodies, 22 primitives (4 boxes and 18 capsules), 23 constraints, and 139 collision interactions.
- Confirmed PA bodies: `spine_05` and `upperarm_l`. Fresh BE-0003 torso contact resolves exact Physics Asset body-surface geometry to `spine_05` at 16.5–18.2 cm from the capsule impact; the older 28–32 cm bone-origin measurements are superseded. Previously Editor-inspected: `hand_l` (box), `spine_03`, `neck_01`, `head` (capsules). Masses: approximately `0.846`, `4.919`, `1.634`, `4.361 kg`.
- `spine_05` and `upperarm_l` were not previously listed; their presence was first established by BE-0003 runtime Physics Asset iteration and is now checked through exact body-surface resolution.
- `hand_l : lowerarm_l` limits are Swing 1 `20°`, Swing 2 `60°`, Twist `45°`. `head : neck_02` limits are `20°`, `15°`, and `30°`.
- The earlier missing-object warning for `/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny` did not recur during the fresh combat session. The active actors use Quinn; a separate non-combat actor uses valid `SKM_Manny_Simple`. The current combat path is unaffected.

## Not yet proven

- Final directional strike input.
- Direct named-skeletal-body hit detection. BE-0003 now resolves exact Physics Asset body geometry from the capsule impact with a valid-body, proximity, and chain-membership gate; it is not final body-geometry-aware strike collision.
- BE-0002 range/lateral misses, 30/60 FPS repetitions, and the remaining runtime matrix measurements.
- Strike velocity, effective impact mass, impulse, or torque measurement; the implemented fist value is only a frame-to-frame kinematic path-speed estimate.
- Bone-level physical response calibration: the BE-0003 experiment shows a visible reaction, but the velocity magnitude is fixed (200 cm/s, `bVelChange=true`) and not tied to fist speed, angle, or effective mass. This is the next calibration target.
- `RecreatePhysicsState()` hitch cost: not profiled; no user-reported stutter in the session, but not formally measured.
- Balance, stagger, fall, knockdown, or recovery state.
- Damage/injury, stamina, fatigue, AI, networking, or automated regression coverage.

## BE-0001 closure

The human reviewer approved the BE-0001 baseline on 2026-08-19. The accepted task is archived under `tasks/completed/`.

BE-0002 added `BE_CONTACT_SAMPLE_V1` and `BE_CONTACT_WINDOW_V1` telemetry and was reviewed and accepted (see `evidence/BE-0002/review.md`).

BE-0003 was human-accepted on 2026-08-25 and archived under `tasks/completed/`. The 2026-08-25 source fix replaced nearest-bone inference with exact Physics Asset surface resolution, added valid-body/proximity/chain gates, and made restore teardown-safe. The fresh 30/60 FPS matrix, capsule fallback, and active-response teardown all passed in PIE; final Codex review found no blocking issue. This remains a constrained calibration experiment, not final combat-physics architecture.

## Evidence

- `evidence/BE-0001/environment.md`
- `evidence/BE-0001/build_summary.md`
- `evidence/BE-0001/asset_references.md`
- `evidence/BE-0001/editor_inspection.md`
- `evidence/BE-0001/fresh_pie_log_excerpt.md`
- `evidence/BE-0001/playtest.md`
- `evidence/BE-0001/approval.md`
- `evidence/BE-0002/build_summary.md`
- `evidence/BE-0002/review.md`
- `evidence/BE-0003/playtest_v1.md`
- `evidence/BE-0003/playtest_fps_matrix.md`
- `evidence/BE-0003/review.md`
