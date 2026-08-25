# BE-0002 playtest

- Status: **PLAYTEST-VERIFIED — matrix captured; final approval pending**
- Tester: Human developer
- Engine: Unreal Engine 5.8.1
- Map: `/Game/Maps/L_CombatGym.L_CombatGym`
- Prerequisite: successful `BruisedEgoPrototypeEditor Win64 Development` build — met on 2026-08-19
- Asset policy: do not save a map, Blueprint, montage, animation, Physics Asset, Config, or project setting for this test

## Setup

1. Open `L_CombatGym` after the successful full build.
2. Confirm no Editor package is dirty before PIE.
3. Open Output Log and filter for `BE_CONTACT_`, `Fist contact:`, `Jab Window`, and `Fighting Stance`.
4. Use the console to set the requested cap: `t.MaxFPS 30` or `t.MaxFPS 60`.
5. Start a fresh PIE session when Bob must be reset after a hit; do not save the map.

## Matrix at each FPS cap

1. Right-click and confirm fighting stance toggles.
2. Throw three aligned, in-range left-click jabs in fresh/reset setups. Each must visibly retain the shove and produce exactly one contact sample plus one `HIT` window summary.
3. Throw one jab from beyond fist reach. It must produce zero samples and one `MISS` summary.
4. Move laterally so the fist path clearly misses and throw one jab. It must produce zero samples and one `MISS` summary.
5. Repeat the complete sequence at the other cap.
6. Restore uncapped operation with `t.MaxFPS 0` after testing.

## Smoke-test result — 2026-08-19

The user completed one fresh PIE run after the successful build. The log confirms Fighting Stance ON/OFF, one Jab Window BEGIN/END, one BE_CONTACT_SAMPLE_V1, and one matching BE_CONTACT_WINDOW_V1 with Result=HIT and SampleCount=1, followed by clean PIE shutdown. No new project error was logged.

The smoke run verifies telemetry and one-hit gating. The full matrix below extends this with 30/60 FPS repetitions and miss windows.

## Full matrix result — 2026-08-19

- 30 FPS: three hit windows, each with one sample and one `HIT` summary; two miss windows, each with zero samples and one `MISS` summary. Reported hit speeds: 347.2, 380.6, and 347.2 cm/s.
- 60 FPS: three hit windows, each with one sample and one `HIT` summary; two miss windows, each with zero samples and one `MISS` summary. Reported hit speeds: 387.3, 387.2, and 393.8 cm/s.
- All accepted hits selected `CollisionCylinder`/`CapsuleComponent`, with `BoneName=None`, no skeletal candidates, component simulation false, and body/mass `Unavailable`; no fallback mass was fabricated.
- The two miss runs at each cap were performed in the prescribed out-of-range then lateral order. The current marker schema does not encode scenario names, so that mapping relies on test order.
- No new project/runtime errors were logged after startup, and the editor shut down cleanly after capture.

## Regression observations

| Check | Expected | Actual |
| --- | --- | --- |
| Right mouse stance | Still toggles | NOT RUN |
| Left mouse jab | Still plays active montage | NOT RUN |
| `hand_l` notify path | Still present | NOT RUN |
| Range miss | Zero contact samples | NOT RUN |
| Lateral miss | Zero contact samples | NOT RUN |
| Genuine contact | One sample/window maximum | NOT RUN |
| Temporary reaction | Existing visible shove remains | NOT RUN |
| Debug path | Yellow sphere/orange path remain | NOT RUN |
| Editor packages | No new dirty content/map packages | NOT RUN |

## Human result

**PLAYTEST-VERIFIED** — the 30/60 FPS matrix is complete; final human acceptance remains required.
