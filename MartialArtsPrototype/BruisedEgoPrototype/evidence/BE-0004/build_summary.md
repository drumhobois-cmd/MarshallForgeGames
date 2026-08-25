# BE-0004 — build summary

## Command

```
powershell -Command "& 'H:\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat' BruisedEgoPrototypeEditor Win64 Development 'H:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype 5.8\BruisedEgoPrototype.uproject' -WaitMutex 2>&1"
```

## Target / configuration

| Item | Value |
| --- | --- |
| Target | `BruisedEgoPrototypeEditor` |
| Platform | `Win64` |
| Configuration | `Development` |
| Engine | UE 5.8.1, CL 56057345 |
| Project | `H:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype 5.8\BruisedEgoPrototype.uproject` |

## Result

**Succeeded** — exit code 0.

```
[1/4] Compile [x64] BECombatComponent.cpp
[2/4] Link [x64] UnrealEditor-BruisedEgoPrototype.lib
[3/4] Link [x64] UnrealEditor-BruisedEgoPrototype.dll
[4/4] WriteMetadata BruisedEgoPrototypeEditor.target [NoUba]

Total time in Unreal Build Accelerator local executor: 7.31 seconds
Output binary: H:\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe

Result: Succeeded
Total execution time: 8.43 seconds
```

## Warnings recorded

```
[Upgrade] Using backward-compatible include order. The latest version of UE has changed the order of includes, which may require code changes. The current setting is:
[Upgrade]     IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6
[Upgrade] Suppress this message by setting 'IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;' in BruisedEgoPrototypeEditor.Target.cs.
[Upgrade] Alternatively you can set this to 'EngineIncludeOrderVersion.Latest' to always use the latest include order. This will potentially cause compile errors when integrating new versions of the engine.
```

This `[Upgrade]` notice was present before BE-0004 and is not introduced by this task.

## Intermediate build errors (resolved before final build)

1. `error C2065: 'ClosestResult': undeclared identifier` — `FClosestPointOnPhysicsAsset ClosestResult` is declared inside `if (TargetSkel)`. The PreApply block lives in the outer `if (bPostBodyValid)` scope. Fixed by capturing `ClosestResult.ClosestWorldPosition` and `ClosestResult.Normal` into `ResolvedSurfacePoint` and `ResolvedSurfaceNormal` variables at the same scope as `ResolvedBone`.

2. `LNK2019: unresolved external symbol FBodyInstanceCore::ShouldInstanceSimulatingPhysics` — `FBodyInstance::IsInstanceSimulatingPhysics()` is an inline wrapper around `FBodyInstanceCore::ShouldInstanceSimulatingPhysics` which is in the `PhysicsCore` module (not listed in `Build.cs`). Resolved by using `USkeletalMeshComponent::IsSimulatingPhysics(BoneName)` instead, consistent with the existing `BE_CONTACT_SAMPLE_V1` approach.

## Evidence label

`COMPILES`
