# BE-0001 asset references

Status: exact **Copy Reference** output captured in the live Unreal Editor; animation, character, collision, and Physics Asset relationships **EDITOR-VERIFIED**.

## Exact Editor Copy Reference output

These strings were copied through the live Content Browser's **Copy Reference** action:

| Role | Exact copied reference |
| --- | --- |
| L_CombatGym | `/Script/Engine.World'/Game/Maps/L_CombatGym.L_CombatGym'` |
| BP_ThirdPersonCharacter | `/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter'` |
| SKM_Quinn_Simple | `/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'` |
| Bob's actual mesh | `/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'` |
| ABP_Unarmed | `/Script/Engine.AnimBlueprint'/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed'` |
| SK_Mannequin | `/Script/Engine.Skeleton'/Game/Characters/Mannequins/Meshes/SK_Mannequin.SK_Mannequin'` |
| Active AM_Jab | `/Script/Engine.AnimMontage'/Game/Characters/Animations/Combat/AM_Jab.AM_Jab'` |
| Legacy AM_Jab | `/Script/Engine.AnimMontage'/Game/Characters/Mannequins/Anims/Unarmed/Attack/AM_Jab.AM_Jab'` |
| AS_Jab_v01 | `/Script/Engine.AnimSequence'/Game/Characters/Animations/Combat/AS_Jab_v01.AS_Jab_v01'` |
| MM_Attack_01 | `/Script/Engine.AnimSequence'/Game/Characters/Mannequins/Anims/Unarmed/Attack/MM_Attack_01.MM_Attack_01'` |
| IMC_Default | `/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'` |
| IA_Jab | `/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jab.IA_Jab'` |
| IA_FightingStance | `/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_FightingStance.IA_FightingStance'` |
| PA_Mannequin | `/Script/Engine.PhysicsAsset'/Game/Characters/Mannequins/Rigs/PA_Mannequin.PA_Mannequin'` |

## Active combat dependency

| Role | Live path | Provenance |
| --- | --- | --- |
| Player Blueprint | `/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter` | Live package and serialized reference |
| Player Animation Blueprint | `/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed` | Live player Blueprint reference |
| Player mesh | `/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple` | Live player Blueprint reference |
| Active jab montage | `/Game/Characters/Animations/Combat/AM_Jab.AM_Jab` | Live player Blueprint reference |
| Active jab sequence | `/Game/Characters/Animations/Combat/AS_Jab_v01.AS_Jab_v01` | Live active montage reference |
| Skeleton | `/Game/Characters/Mannequins/Meshes/SK_Mannequin.SK_Mannequin` | Live animation asset reference |
| Combat map | `/Game/Maps/L_CombatGym.L_CombatGym` | Live map package and latest PIE log |

The active montage and sequence are untracked. The referencing player Blueprint is tracked-but-modified. Preserve them as one dependency/commit unit.

## Same-name legacy asset

An older tracked montage also exists:

`/Game/Characters/Mannequins/Anims/Unarmed/Attack/AM_Jab.AM_Jab`

It references `MM_Attack_01` and is not the montage assigned to the current player Blueprint. Always use full paths when discussing `AM_Jab`.

## Input

| Asset | Live path | Mapping |
| --- | --- | --- |
| `IMC_Default` | `/Game/Input/IMC_Default.IMC_Default` | Mapping context |
| `IA_Jab` | `/Game/Input/Actions/IA_Jab.IA_Jab` | `LeftMouseButton` |
| `IA_FightingStance` | `/Game/Input/Actions/IA_FightingStance.IA_FightingStance` | `RightMouseButton` |

`IMC_Default.uasset` SHA-256:

`D68BF793E98C6601CD34DADE31C60EBAFB9DD2478ABD2056BB230AC5B6EB5FB3`

The mapping was decoded through the package's FName/import/mapping records and then viewed directly in the Enhanced Input editor. Its exact FName map has no standalone keyboard `J` entry, and the live UI shows no `J` mapping row.

## Notify and root motion

- Active notify-state name: `BEJabWindow` / `Jab Window`.
- Notify class: `UBEAnimNotifyState_JabWindow`.
- C++ socket default: `fist_l`.
- Active montage instance: `hand_l`; `hand_l` is a bone, while `fist_l` is an existing socket below it.
- Notify interval: `0.318862` to `0.406999` seconds, approximately `0.088137` seconds.
- Montage slot: `DefaultGroup.DefaultSlot`; section `Default`; length `0.733333` seconds.
- Linked sequence: `/Game/Characters/Animations/Combat/AS_Jab_v01.AS_Jab_v01`.
- `AS_Jab_v01`: root motion disabled, Root Lock `Ref Pose`, Force Root Lock disabled, normalized root-motion scale enabled.
- `ABP_Unarmed`: Root Motion Mode `Root Motion from Montages Only`.

## Animation Blueprint

The live graph is:

`Main States` → `Layered Blend per Bone` → `DefaultGroup.DefaultSlot` → `CR_Mannequin_FootIK` → `Output Pose`

- The fighting-stance pose is `MM_Attack_01`.
- The Branch Filter starts at `spine_01`, blend depth `1`, weight `1.0` while stance is active.
- Control Rig alpha is `1.0`; LOD threshold is `-1`.

## Physics

- Player and Bob both use `SKM_Quinn_Simple`, `ABP_Unarmed_C`, and the active combat montage.
- The mesh Physics Asset Override is `None`; `SKM_Quinn_Simple` itself references `/Game/Characters/Mannequins/Rigs/PA_Mannequin.PA_Mannequin`.
- `PA_Mannequin`: 22 bodies, 22 primitives (4 boxes and 18 capsules), 23 constraints, and 139 collision interactions.
- Inspected body geometry/mass: `hand_l` box (`16 × 5 × 10`, `0.845897 kg`); `spine_03` capsule (`4.919094 kg`); `neck_01` capsule (`1.633866 kg`); `head` capsule (`4.360557 kg`). All use linear/angular damping `0 / 0.01`.
- `hand_l : lowerarm_l` angular limits are `20° / 60° / 45°` for Swing 1 / Swing 2 / Twist.
- `head : neck_02` angular limits are `20° / 15° / 30°`.
- Target mesh: Custom, Query Only, Pawn, Simulate Physics off. Target capsule: Custom, Query and Physics, Pawn.
- Full inspection and screenshots: `evidence/BE-0001/editor_inspection.md`.

## Editor pass result

The Copy Reference checklist is complete. The live Editor inspection is recorded in `evidence/BE-0001/editor_inspection.md`; current screenshots are under `evidence/BE-0001/screenshots/`.
