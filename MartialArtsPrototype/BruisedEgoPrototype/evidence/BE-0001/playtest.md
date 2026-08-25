# BE-0001 playtest evidence

- Date recorded: 2026-08-19
- Tester: Human developer for the prior PIE acceptance; Codex drove and inspected the fresh BE-0001 PIE session
- Engine: Unreal Engine 5.8.1, CL 56057345
- Map: `/Game/Maps/L_CombatGym.L_CombatGym`
- Player/target: spawned `BP_ThirdPersonCharacter_C_0`; placed target `BP_ThirdPersonCharacter_C_1`
- Evidence status: current jab/contact path **PLAYTEST-VERIFIED** by the user; fresh task-specific run **EDITOR-VERIFIED**

## Fresh BE-0001 session

Fresh PIE ran from `2026-08-19 09:14:13 UTC` to `09:21:58 UTC` (`19:14:13` to `19:21:58` Australia/Brisbane). The Editor reported that no Blueprints needed recompiling and copied `/Game/Maps/L_CombatGym.L_CombatGym` into the PIE world.

| Scenario | Log time | Contact lines in window | Result |
| --- | --- | ---: | --- |
| Initial spacing miss | `09:15:00` | 0 | Seven-tick jab window; no contact |
| Right-mouse stance, then jab | `09:15:01`–`09:15:02` | 0 | `Fighting Stance: ON`; seven-tick miss window |
| Forward approach, first jab | `09:16:18` | 0 | Seven-tick window; still outside the fist path |
| In-range jab | `09:16:21` | 1 | Exactly one `Fist contact: BP_ThirdPersonCharacter_C_1` in an eight-tick window |
| Five post-contact jabs | `09:17:12`–`09:17:18` | 0 each | Target remained separated after the shove |

Every window reported `Socket 'hand_l' exists: YES`. The hit window contains one—and only one—contact line between its `BEGIN` and `END` markers.

Bob's saved X position is `-30.46875`; after the hit the runtime Details panel showed X `-13.518238`, a shift of approximately `+16.95 cm` in the shove direction. This observation, the subsequent misses, and the implemented `250 cm/s` horizontal `LaunchCharacter` response support the visible temporary-shove result; it is not a bone-impulse measurement.

The earlier missing `SKM_Manny.SKM_Manny` warning did not recur anywhere in this fresh session. PIE shut down cleanly.

Fresh evidence: [fresh_pie_log_excerpt.md](fresh_pie_log_excerpt.md), [pie_stance.jpg](screenshots/pie_stance.jpg), [pie_after_shove.jpg](screenshots/pie_after_shove.jpg), and [target_runtime_assignment.jpg](screenshots/target_runtime_assignment.jpg).

## Existing accepted scenario

1. Enter PIE in `L_CombatGym`.
2. Use right mouse to enter/exit fighting stance.
3. Use left mouse to play the jab.
4. Test the target in range, beyond reach, and laterally outside the fist path.
5. Observe the notify-driven debug sweep and contact log.
6. Confirm at most one contact is processed during one notify window.
7. On genuine contact, observe the temporary horizontal shove.

Previously reported result:

- The montage plays from left mouse.
- Range and lateral separation can produce a miss.
- Genuine fist contact produces one registered hit during the active window.
- The target receives the temporary shove.

## Preserved live log evidence

Local log:

`E:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype\Saved\Logs\BruisedEgoPrototype.log`

Relevant recorded facts:

- PIE copied `/Game/Maps/L_CombatGym.L_CombatGym`.
- The active notify instance repeatedly reported `Socket 'hand_l' exists: YES`.
- Multiple genuine contacts logged `Fist contact: BP_ThirdPersonCharacter_C_1`.
- The source handling that contact applies the temporary `LaunchCharacter` shove.

The raw log is not committed because `Saved/` and `*.log` are ignored.

## Result

- Fresh task-specific hit, range misses, one-hit-per-window logging, stance input, `hand_l`, and shove/separation evidence: **PASS**.
- Fresh lateral miss was not repeated in this automated pass; it remains **PLAYTEST-VERIFIED** from the user's earlier accepted test.
- Unreal assets saved or changed by this run: none.
- Human baseline acceptance: **PASS** — accepted on 2026-08-19.
