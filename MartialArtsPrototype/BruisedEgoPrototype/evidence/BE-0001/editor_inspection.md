# BE-0001 live Editor inspection

- Date: 2026-08-19
- Inspector: Codex, using the live Unreal Editor UI
- Editor: Unreal Engine 5.8.1, CL 56057345
- Project/map: `BruisedEgoPrototype`; `/Game/Maps/L_CombatGym.L_CombatGym`
- Result: **EDITOR-VERIFIED**
- Mutation policy: read-only inspection, PIE, and the user-approved exact-package reload described below; no Unreal asset was saved

The live Editor, its Content Browser **Copy Reference** action, the engine's asset serializer, and the fresh PIE log were used together. The project-scoped Git status after the pass still contains only the pre-existing modified Unreal assets plus BE-0001 documentation/evidence additions.

Editor hygiene cleanup completed: after the preview inspection reported only `/Game/Characters/Mannequins/Meshes/SK_Mannequin` dirty in memory, the user explicitly approved discarding that preview-only change. Unreal reloaded exactly that package from disk and returned `(True, Text(""))`. The post-reload dirty-content and dirty-map lists were both empty. The asset's SHA-256 remained `E9BA182B6DEB3E4AEFE331CECD755373A24C38AEDFE70C1237789676DBDF920D` before and after, confirming that no disk save occurred.

## Input

`IMC_Default` visibly contains:

| Action | Key |
| --- | --- |
| `IA_FightingStance` | Right Mouse Button |
| `IA_Jab` | Left Mouse Button |

There is no keyboard `J` row. Screenshot: [input_mapping.jpg](screenshots/input_mapping.jpg).

## Active animation path

The active path is:

`Main States` → `Layered Blend per Bone` → `DefaultGroup.DefaultSlot` → `CR_Mannequin_FootIK` Control Rig → `Output Pose`

- The stance pose is `MM_Attack_01`.
- The layer uses Branch Filter mode from `spine_01`, blend depth `1`, with weight `1.0` when fighting stance is active.
- The Animation Blueprint root-motion mode is `Root Motion from Montages Only`.
- The Control Rig is `CR_Mannequin_FootIK`, alpha `1.0`, LOD threshold `-1`.
- Graph captures: [anim_graph.jpg](screenshots/anim_graph.jpg), [layered_blend.jpg](screenshots/layered_blend.jpg), and [control_rig.jpg](screenshots/control_rig.jpg).

The Blueprint's assigned montage is `/Game/Characters/Animations/Combat/AM_Jab.AM_Jab`. It uses:

- linked sequence `/Game/Characters/Animations/Combat/AS_Jab_v01.AS_Jab_v01`;
- skeleton `/Game/Characters/Mannequins/Meshes/SK_Mannequin.SK_Mannequin`;
- `DefaultSlot`, section `Default`, montage length `0.733333` seconds;
- `UBEAnimNotifyState_JabWindow` / `BEJabWindow` from `0.318862` to `0.406999` seconds, approximately `0.088137` seconds;
- active notify property `Fist Socket Name = hand_l`.

`hand_l` is a bone. `fist_l`, the C++ default, is an existing socket below `hand_l`. Captures: [active_montage.jpg](screenshots/active_montage.jpg) and [skeleton_hand_fist.jpg](screenshots/skeleton_hand_fist.jpg).

`AS_Jab_v01` has:

- Enable Root Motion: off;
- Root Motion Root Lock: Ref Pose;
- Force Root Lock: off;
- Use Normalized Root Motion Scale: on.

Capture: [sequence_root_motion.jpg](screenshots/sequence_root_motion.jpg).

## Player, defender, and collision

The spawned player (`BP_ThirdPersonCharacter_C_0`) and placed target/Bob (`BP_ThirdPersonCharacter_C_1`, actor label `BP_ThirdPersonCharacter`) use the same Blueprint defaults:

- mesh: `SKM_Quinn_Simple`;
- Anim Class: `ABP_Unarmed_C`;
- mesh Physics Asset Override: `None`;
- `SKM_Quinn_Simple` asset Physics Asset: `PA_Mannequin`.

The separate map actor labelled `CR_Mannequin_Body` uses `SKM_Manny_Simple`; it is not Bob and is not the contact target.

| Component | Verified collision/simulation setup |
| --- | --- |
| Mesh | Custom; Query Only; object type Pawn; ignores Pawn, Visibility, Vehicle, and Camera; hit events off; overlaps off; per-poly collision off; Simulate Physics off |
| Capsule | Custom; Query and Physics; object type Pawn; radius `35`, half-height `90`; Visibility and Camera ignored; Pawn uses the inherited blocking response; step-up disabled |

Captures: [target_runtime_assignment.jpg](screenshots/target_runtime_assignment.jpg), [target_mesh_physics_asset.jpg](screenshots/target_mesh_physics_asset.jpg), [target_mesh_collision.jpg](screenshots/target_mesh_collision.jpg), [target_capsule_collision.jpg](screenshots/target_capsule_collision.jpg), and [target_mesh_simulation.jpg](screenshots/target_mesh_simulation.jpg).

## Physics Asset

`PA_Mannequin` reports 22 bodies, 22 primitives (4 boxes and 18 capsules), 23 constraints (3 cross constraints), and 139 collision interactions.

| Body | Shape | Mass | Damping (linear/angular) | Coverage |
| --- | --- | ---: | --- | --- |
| `hand_l` | Box, `16 × 5 × 10` | `0.845897 kg` | `0 / 0.01` | Left hand |
| `spine_03` | Capsule, radius `11.2208`, length `6.18942` | `4.919094 kg` | `0 / 0.01` | Upper torso |
| `neck_01` | Capsule, radius `7.41185`, length `1.26805` | `1.633866 kg` | `0 / 0.01` | Lower neck |
| `head` | Capsule, radius `10.5957`, length `6.07124` | `4.360557 kg` | `0 / 0.01` | Head |

All four inspected bodies use the default Physics Type, have gravity enabled, one shape, and collision response enabled.

- `hand_l : lowerarm_l`: linear X/Y/Z locked; Swing 1 limited `20°`; Swing 2 limited `60°`; Twist limited `45°`; adjacent-body collision disabled.
- `head : neck_02`: linear X/Y/Z locked; Swing 1 limited `20°`; Swing 2 limited `15°`; Twist limited `30°`.
- The graph connects `spine_03` to `spine_02` and `spine_04`; `neck_01` to `spine_05` and `neck_02`; and `head` to `neck_02`.

Captures: [physics_hand_body.jpg](screenshots/physics_hand_body.jpg), [physics_hand_constraint.jpg](screenshots/physics_hand_constraint.jpg), [physics_spine03_body.jpg](screenshots/physics_spine03_body.jpg), [physics_neck01_body.jpg](screenshots/physics_neck01_body.jpg), [physics_head_body.jpg](screenshots/physics_head_body.jpg), and [physics_head_constraint.jpg](screenshots/physics_head_constraint.jpg).

## Missing Manny reference

The log contains one earlier warning at `2026-08-19 04:52:26 UTC` for the absent `/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny`. It did not recur between fresh PIE startup at `09:14:13 UTC` and shutdown at `09:21:58 UTC`.

The active player and Bob both resolved to `SKM_Quinn_Simple`; the separate reference actor resolved to `SKM_Manny_Simple`. The stale non-`_Simple` warning therefore does not affect the current combat path. Its originating picker/soft reference has not been identified and can be cleaned up separately if it reappears during an active-path action.
