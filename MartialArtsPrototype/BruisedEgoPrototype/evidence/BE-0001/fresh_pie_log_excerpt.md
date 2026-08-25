# BE-0001 fresh PIE log excerpt

Source: local ignored file `Saved/Logs/BruisedEgoPrototype.log`. This is a selected, committed excerpt; line numbers refer to that file at inspection time.

## Session and map

```text
4968 [2026.08.19-09.14.13:227] LogPlayLevel: PlayLevel: No blueprints needed recompiling
4969 [2026.08.19-09.14.13:229] LogPlayLevel: Creating play world package: /Game/Maps/UEDPIE_0_L_CombatGym
4971 [2026.08.19-09.14.13:245] LogPlayLevel: PIE: Created PIE world by copying editor world from /Game/Maps/L_CombatGym.L_CombatGym to /Game/Maps/UEDPIE_0_L_CombatGym.L_CombatGym
5021 [2026.08.19-09.14.13:511] PIE: Play in editor total start time 0.305 seconds.
```

## Misses, stance, and hit

```text
5025 [2026.08.19-09.15.00:465] LogTemp: Jab Window BEGIN
5028 [2026.08.19-09.15.00:465] LogTemp:   Socket 'hand_l' exists: YES
5030 [2026.08.19-09.15.00:549] LogTemp: Jab Window END (ticked 7 frames)
5031 [2026.08.19-09.15.01:358] LogTemp: Fighting Stance: ON
5032 [2026.08.19-09.15.02:468] LogTemp: Jab Window BEGIN
5035 [2026.08.19-09.15.02:468] LogTemp:   Socket 'hand_l' exists: YES
5037 [2026.08.19-09.15.02:554] LogTemp: Jab Window END (ticked 7 frames)
5038 [2026.08.19-09.16.18:873] LogTemp: Jab Window BEGIN
5041 [2026.08.19-09.16.18:873] LogTemp:   Socket 'hand_l' exists: YES
5043 [2026.08.19-09.16.18:954] LogTemp: Jab Window END (ticked 7 frames)
5044 [2026.08.19-09.16.21:026] LogTemp: Jab Window BEGIN
5047 [2026.08.19-09.16.21:026] LogTemp:   Socket 'hand_l' exists: YES
5048 [2026.08.19-09.16.21:026] LogTemp: Jab Window TICK (first frame)
5049 [2026.08.19-09.16.21:048] LogTemp: Fist contact: BP_ThirdPersonCharacter_C_1
5050 [2026.08.19-09.16.21:113] LogTemp: Jab Window END (ticked 8 frames)
```

There is exactly one contact line between the hit window's `BEGIN` and `END`.

## Five post-shove miss windows

```text
5054 [2026.08.19-09.17.12:837] LogTemp: Jab Window BEGIN
5059 [2026.08.19-09.17.12:925] LogTemp: Jab Window END (ticked 8 frames)
5060 [2026.08.19-09.17.14:186] LogTemp: Jab Window BEGIN
5065 [2026.08.19-09.17.14:275] LogTemp: Jab Window END (ticked 8 frames)
5066 [2026.08.19-09.17.15:543] LogTemp: Jab Window BEGIN
5071 [2026.08.19-09.17.15:627] LogTemp: Jab Window END (ticked 8 frames)
5072 [2026.08.19-09.17.16:886] LogTemp: Jab Window BEGIN
5077 [2026.08.19-09.17.16:987] LogTemp: Jab Window END (ticked 9 frames)
5078 [2026.08.19-09.17.18:242] LogTemp: Jab Window BEGIN
5083 [2026.08.19-09.17.18:325] LogTemp: Jab Window END (ticked 8 frames)
```

No `Fist contact:` line occurs inside those five windows.

## Clean shutdown and stale warning check

```text
5085 [2026.08.19-09.21.58:672] LogWorld: BeginTearingDown for /Game/Maps/UEDPIE_0_L_CombatGym
5088 [2026.08.19-09.21.58:675] LogPlayLevel: Display: Shutting down PIE online subsystems
5095 [2026.08.19-09.21.58:910] LogPlayLevel: Display: Destroying online subsystem :Context_31
```

The file's only inspected `SKM_Manny.SKM_Manny` missing-object warning is earlier at line 3825 (`04:52:26 UTC`). No such warning occurs within the fresh session range, lines 4966–5095.
