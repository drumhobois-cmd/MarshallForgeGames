# BE-0001 environment baseline

- Recorded: 2026-08-19
- Recorder: Codex
- Platform: Windows
- Time zone: Australia/Brisbane
- Evidence status: **COMPILES** for source/build; **EDITOR-VERIFIED** for the live asset baseline
- Scope: project/source and live Editor inspection plus documentation/evidence installation; no Unreal gameplay file was saved or changed on disk

## Paths and source control

- Git root: `E:\MarshallForgeGames`
- Unreal project root: `E:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype`
- Project file: `E:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype\BruisedEgoPrototype.uproject`
- Baseline branch: `main`
- Baseline commit before operating-file installation: `3b113f528814adb19770c9a48c5ef5a168370881`

Pre-existing repository state was preserved:

- Modified `Content/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.uasset`.
- Modified `Content/Maps/L_CombatGym.umap`.
- Modified `Content/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.uasset`.
- Untracked `Content/Characters/Animations/`.
- Untracked `Content/CustomAnimations/`.
- Untracked `BruisedEgo_Project_Operating_Files_v1/`.
- Unrelated untracked Git-root siblings `Frames/`, `MoCap/`, and `RiggedCharacters/`.

BE-0001 added/merged operating documentation and resized primary-monitor Editor screenshots only. It did not discard or rewrite any existing change.

The inspection temporarily left only `SK_Mannequin` dirty in memory. After explicit user approval, Unreal reloaded exactly that package from disk and returned success. The post-reload dirty-content and dirty-map lists were empty, and its before/after SHA-256 was identical (`E9BA182B6DEB3E4AEFE331CECD755373A24C38AEDFE70C1237789676DBDF920D`). No Unreal asset was saved by the cleanup.

## Unreal Engine

- `.uproject` association: `{E70D4010-44D5-A15F-C9F1-72982D6C6314}`.
- HKCU registered-build mapping: `H:\Epic Games\UE_5.8`.
- Version: 5.8.1.
- Changelist: 56057345.
- Compatible changelist: 55116800.
- Branch: `++UE5+Release-5.8`.
- Engine build, Editor, and command-line Editor executables are present.
- Unreal Editor and Live Coding were running during the 2026-08-19 rebaseline, so a new full rebuild was not launched.

## Module and targets

- Runtime module: `BruisedEgoPrototype`.
- Loading phase: Default.
- Game target: `BruisedEgoPrototype`, BuildSettings V7.
- Editor target: `BruisedEgoPrototypeEditor`, BuildSettings V7.
- Public module dependencies: Core, CoreUObject, Engine, InputCore, EnhancedInput.
- Private dependencies: none.
- Explicit `.uproject` plugins:
  - Modeling Tools Editor Mode (Editor only).
  - Gameplay StateTree.
- Latest Editor log also mounted Enhanced Input, Control Rig, Physics Control, and Control Rig Physics. This is runtime/editor availability evidence, not a Physics Control project dependency.

## Live C++ inventory

| File | SHA-256 |
| --- | --- |
| `Source/BruisedEgoPrototype/BECharacter.h` | `F873F4DBE5D763310498880A56B14D0CE28B9B0043E8765F93416B37902F69E4` |
| `Source/BruisedEgoPrototype/BECharacter.cpp` | `D4B08C1B74BADE538C8C07769F0AD5E68613A151DA8678AD6AABF0AB5F75EC20` |
| `Source/BruisedEgoPrototype/BECombatComponent.h` | `1C507C7917D3BAEEB95872441D14A3E3A646BFACA4BE1DF2476663E83763F1B6` |
| `Source/BruisedEgoPrototype/BECombatComponent.cpp` | `F58BDEA6EE8EA0C5D8DCD021EE84CB1608937FFC9BAC0B78AB19E1E152C098C3` |
| `Source/BruisedEgoPrototype/BEAnimNotifyState_JabWindow.h` | `78300D9AC79F9B22D4FB66F03003DB4232800FEF654E3BFBE21BA6FAA65E580C` |
| `Source/BruisedEgoPrototype/BEAnimNotifyState_JabWindow.cpp` | `3A0BD5A362BA2443FDFB3EE38363D229472507B70AAFB418BDD836881FAA4AE9` |

All six files are tracked, have no Git diff, and match the recorded jab/contact implementation.
