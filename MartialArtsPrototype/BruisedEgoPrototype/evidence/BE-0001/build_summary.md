# BE-0001 build summary

Evidence status: **COMPILES**

## Verified successful build

Local UnrealBuildTool log:

`C:\Users\samma\AppData\Local\UnrealBuildTool\Log.txt`

Log timestamp:

`2026-08-16T16:00:26.3692570+10:00`

Recorded UBT invocation:

`"H:\Epic Games\UE_5.8\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll" BruisedEgoPrototypeEditor Win64 Development E:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype\BruisedEgoPrototype.uproject`

Result:

- Target: `BruisedEgoPrototypeEditor`.
- Platform: Win64.
- Configuration: Development.
- Result: `Succeeded`.
- Total execution time: 13.27 seconds.
- Output DLL timestamp: `2026-08-16T16:00:24.7638986+10:00`.
- Newest live source timestamp: `2026-08-16T15:58:42.3684162+10:00` (`BECombatComponent.cpp`).

The output artifact is newer than every current source file, so the inspected live C++ baseline is covered by this successful build.

## Material build message

The target uses backward-compatible include ordering:

`IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6`

Unreal suggests `Unreal5_8`. This is an upgrade notice, not a failed build. BE-0001 does not change target settings.

## Safe repeat command

Not run during this rebaseline because Unreal Editor and Live Coding were open:

`& 'H:\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat' BruisedEgoPrototypeEditor Win64 Development '-Project=E:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype\BruisedEgoPrototype.uproject' -WaitMutex -NoHotReloadFromIDE -NoEngineChanges`

Before running it, close Unreal Editor and Live Coding. `-NoEngineChanges` prevents the project build from overwriting engine outputs.

The raw `*.log` file remains local because repository policy ignores `Saved/` and `*.log`.
