# Bruised Ego — Unreal asset and source map

Last updated: **2026-08-25**  
Active task: **None — BE-0004 accepted; no asset-path change made**

The package/object paths below were read from the live project and confirmed through the Unreal Editor's **Copy Reference** action. Verification maturity uses the canonical labels in `AGENTS.md`; provenance is stated separately. Exact class-qualified copied strings are in `evidence/BE-0001/asset_references.md`.

## Unreal assets

| Logical role | Asset/object | Type | Live package/object path | Verification and relationship |
| --- | --- | --- | --- | --- |
| Test map | `L_CombatGym` | Map | `/Game/Maps/L_CombatGym.L_CombatGym` | **EDITOR-VERIFIED** in the fresh PIE session; modified tracked asset |
| Configured default map | `Lvl_ThirdPerson` | Map | `/Game/ThirdPerson/Lvl_ThirdPerson.Lvl_ThirdPerson` | **IMPLEMENTED** in `DefaultEngine.ini`; not the latest combat PIE map |
| Player Blueprint | `BP_ThirdPersonCharacter` | Character Blueprint | `/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter` | **EDITOR-VERIFIED** assignments; behaviour **PLAYTEST-VERIFIED**; modified tracked asset; native parent is `ABECharacter` |
| Player mesh | `SKM_Quinn_Simple` | Skeletal Mesh | `/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple` | **EDITOR-VERIFIED** for player and Bob; mesh asset references `PA_Mannequin` |
| Player Animation Blueprint | `ABP_Unarmed` | Animation Blueprint | `/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed` | **EDITOR-VERIFIED** graph/settings; modified tracked asset; behaviour **PLAYTEST-VERIFIED** |
| Mannequin skeleton | `SK_Mannequin` | Skeleton | `/Game/Characters/Mannequins/Meshes/SK_Mannequin.SK_Mannequin` | **EDITOR-VERIFIED**; `hand_l` bone and `fist_l` socket confirmed |
| Active jab montage | `AM_Jab` | Animation Montage | `/Game/Characters/Animations/Combat/AM_Jab.AM_Jab` | **EDITOR-VERIFIED** setup; behaviour **PLAYTEST-VERIFIED**; referenced by the player Blueprint; currently untracked |
| Active jab sequence | `AS_Jab_v01` | Animation Sequence | `/Game/Characters/Animations/Combat/AS_Jab_v01.AS_Jab_v01` | **EDITOR-VERIFIED**; currently untracked; root motion disabled, Root Lock Ref Pose |
| Legacy jab montage | `AM_Jab` | Animation Montage | `/Game/Characters/Mannequins/Anims/Unarmed/Attack/AM_Jab.AM_Jab` | **EDITOR-VERIFIED** as the distinct existing asset; not assigned to the live player Blueprint |
| Jab contact window | `BEJabWindow` / `Jab Window` | `UBEAnimNotifyState_JabWindow` instance | Inside active `AM_Jab` | **EDITOR-VERIFIED** at `0.318862`–`0.406999` seconds with `hand_l`; behaviour **PLAYTEST-VERIFIED** |
| Stance/base attack pose | `MM_Attack_01` | Animation Sequence | `/Game/Characters/Mannequins/Anims/Unarmed/Attack/MM_Attack_01.MM_Attack_01` | **EDITOR-VERIFIED** as the stance blend pose |
| Input mapping | `IMC_Default` | Enhanced Input Mapping Context | `/Game/Input/IMC_Default.IMC_Default` | **EDITOR-VERIFIED**: left mouse jab, right mouse stance, no keyboard `J` row |
| Jab input | `IA_Jab` | Input Action | `/Game/Input/Actions/IA_Jab.IA_Jab` | **EDITOR-VERIFIED** mapping; **PLAYTEST-VERIFIED** through left mouse |
| Fighting-stance input | `IA_FightingStance` | Input Action | `/Game/Input/Actions/IA_FightingStance.IA_FightingStance` | **EDITOR-VERIFIED** mapping; **PLAYTEST-VERIFIED** through right mouse |
| Look input | `IA_Look` | Input Action | `/Game/Input/Actions/IA_Look.IA_Look` | **IMPLEMENTED** |
| Move input | `IA_Move` | Input Action | `/Game/Input/Actions/IA_Move.IA_Move` | **IMPLEMENTED** |
| Jump input | `IA_Jump` | Input Action | `/Game/Input/Actions/IA_Jump.IA_Jump` | **IMPLEMENTED** |
| Physics asset | `PA_Mannequin` | Physics Asset | `/Game/Characters/Mannequins/Rigs/PA_Mannequin.PA_Mannequin` | **EDITOR-VERIFIED**; 22 bodies, 22 primitives, 23 constraints; key bodies/limits recorded |
| Defender/Bob | placed `BP_ThirdPersonCharacter_C_1` | Character instance | Inside `L_CombatGym` | **EDITOR-VERIFIED**; label `BP_ThirdPersonCharacter`; uses `SKM_Quinn_Simple`, `ABP_Unarmed_C`, and `PA_Mannequin` through the mesh |

## Critical dependency chain

The active path crosses one tracked-but-modified Blueprint and two untracked assets:

`BP_ThirdPersonCharacter`  
→ `/Game/Characters/Animations/Combat/AM_Jab`  
→ `/Game/Characters/Animations/Combat/AS_Jab_v01`

Treat these three assets as one review/commit unit. Committing only the Blueprint would leave a missing montage/sequence dependency on another checkout.

The older tracked montage with the same short name must always be identified by its full path.

## Current input mapping

The live `IMC_Default.uasset` package was structurally decoded rather than searched as arbitrary text:

| Action | Key |
| --- | --- |
| `/Game/Input/Actions/IA_Jab` | `LeftMouseButton` |
| `/Game/Input/Actions/IA_FightingStance` | `RightMouseButton` |

Its exact 58-entry FName map contains no standalone `J` entry, so no serialized mapping can reference keyboard `J`. The inspected file SHA-256 is:

`D68BF793E98C6601CD34DADE31C60EBAFB9DD2478ABD2056BB230AC5B6EB5FB3`

The same mapping is visible in the live Enhanced Input editor; screenshot evidence is stored under `evidence/BE-0001/screenshots/`.

## Animation graph relationship

Current relationship checked directly in the live Editor:

`Main States`  
→ `Layered Blend per Bone`  
→ `DefaultGroup.DefaultSlot`  
→ `CR_Mannequin_FootIK` Control Rig  
→ `Output Pose`

`BP_ThirdPersonCharacter`  
→ active `AM_Jab`  
→ `AS_Jab_v01`  
→ `DefaultGroup.DefaultSlot`  
→ `BEJabWindow` notify-state interval (`0.318862`–`0.406999` seconds)  
→ `hand_l` bone override in the live montage instance

`ABP_Unarmed` uses `MM_Attack_01` for fighting stance and blends from `spine_01` at depth `1`. The sequence has root motion disabled; the Anim Blueprint mode is `Root Motion from Montages Only`. The Control Rig is alpha `1.0`, after the montage slot.

## Physics and collision facts

- Player and Bob both use `SKM_Quinn_Simple`; Physics Asset Override is `None`, and the mesh asset references `PA_Mannequin`.
- Mesh: Custom, Query Only, object type Pawn, Simulate Physics off; Pawn/Visibility/Vehicle/Camera ignored.
- Capsule: Custom, Query and Physics, object type Pawn, radius `35`, half-height `90`; Visibility and Camera ignored.
- `PA_Mannequin`: 22 bodies; 22 primitives (4 boxes, 18 capsules); 23 constraints; 139 collision interactions.
- Editor-inspected bodies: `hand_l` box; `spine_03`, `neck_01`, and `head` capsules. Masses/damping in `evidence/BE-0001/editor_inspection.md`.
- Runtime-confirmed bodies: `spine_05` and `upperarm_l`. The final BE-0003 path resolves exact Physics Asset body-surface geometry from capsule impact (not bone-transform origin); fresh 30/60 FPS torso hits resolved `spine_05` at 16.5–18.2 cm, with a 50 cm maximum proximity gate and verified `spine_03` chain membership.
- `hand_l : lowerarm_l` limits: Swing 1 `20°`, Swing 2 `60°`, Twist `45°`.
- `head : neck_02` limits: Swing 1 `20°`, Swing 2 `15°`, Twist `30°`.
- The stale missing `SKM_Manny.SKM_Manny` warning did not occur in the fresh active-path PIE session. A separate non-combat reference actor validly uses `SKM_Manny_Simple`.

## Live source files

| Class | Live repository files | Verification |
| --- | --- | --- |
| `ABECharacter` | `Source/BruisedEgoPrototype/BECharacter.h`; `Source/BruisedEgoPrototype/BECharacter.cpp` | **COMPILES** |
| `UBECombatComponent` | `Source/BruisedEgoPrototype/BECombatComponent.h`; `Source/BruisedEgoPrototype/BECombatComponent.cpp` | **COMPILES** |
| `UBEAnimNotifyState_JabWindow` | `Source/BruisedEgoPrototype/BEAnimNotifyState_JabWindow.h`; `Source/BruisedEgoPrototype/BEAnimNotifyState_JabWindow.cpp` | **COMPILES** |
| Module rules | `Source/BruisedEgoPrototype/BruisedEgoPrototype.Build.cs` | **COMPILES** |
| Game target | `Source/BruisedEgoPrototype.Target.cs` | **COMPILES** |
| Editor target | `Source/BruisedEgoPrototypeEditor.Target.cs` | **COMPILES** |

## Copy Reference status

Complete. Exact live Editor output for the map, Blueprint, Bob's actual mesh, Animation Blueprint, skeleton, both `AM_Jab` assets, sequences, input assets, and Physics Asset is recorded in `evidence/BE-0001/asset_references.md`.
