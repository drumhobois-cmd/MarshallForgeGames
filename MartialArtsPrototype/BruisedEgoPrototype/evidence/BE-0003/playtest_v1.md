# BE-0003 — playtest evidence v1

Date: **2026-08-20**  
Session map: `/Game/Maps/L_CombatGym.L_CombatGym`  
Engine: UE 5.8.1, CL 56057345  
Evidence label reached: **PLAYTEST-VERIFIED**

## Session summary

User confirmed: **"it worked"** — a bounded, visibly readable upper-body reaction was observed on all
successful hits.

## Telemetry — full window summary

| WindowId | Result | InferredBone | InferredDist cm | AppliedVelocity cm/s | Restored |
| --- | --- | --- | --- | --- | --- |
| 1 | MISS | — | — | — | — |
| 2 | HIT / NamedBody | spine_05 | 31.7 | (199.9,−4.5,0.0) | true |
| 3 | HIT / NamedBody | spine_05 | 31.6 | (199.9,−4.5,0.0) | true |
| 4 | HIT / NamedBody | spine_05 | 30.5 | (199.9,−4.5,0.0) | true |
| 5 | HIT / NamedBody | spine_05 | 31.7 | (199.9,−4.5,0.0) | true |
| 6 | HIT / NamedBody | upperarm_l | 28.2 | (194.9,−45.1,0.0) | true |
| 7 | HIT / NamedBody | upperarm_l | 28.6 | (194.9,−45.1,0.0) | true |
| 8 | HIT / NamedBody | spine_05 | 31.6 | (195.9,−40.1,0.0) | true |
| 9 | HIT / NamedBody | spine_05 | 32.2 | (195.9,−40.1,0.0) | true |

All 8 hits used `Branch=NamedBody`. The capsule-fallback branch was never needed this session because the
PA body lookup always resolved.

## Acceptance criteria check

| Criterion | Status | Notes |
| --- | --- | --- |
| Named body hit produces bounded visible reaction | **PLAYTEST-VERIFIED** | User confirmed visual reaction; all 8 hits showed `Branch=NamedBody` |
| Capsule/unnamed fallback used when no body resolves | **IMPLEMENTED** | Code path verified; not exercised this session (PA lookup always resolved) |
| Controls restored after bounded interval | **PLAYTEST-VERIFIED** | `Phase=Restored \| CollisionRestored=true` on all 8 hits; no stuck state observed |
| Misses produce no response | **PLAYTEST-VERIFIED** | WindowId=1 logged MISS; no response telemetry emitted |
| 30/60 FPS repeated hit/miss checks pass without crash | **NOT RUN** | Single 120 Hz session; multi-FPS matrix not yet run |
| Telemetry records bone/body, branch, duration, restore | **PLAYTEST-VERIFIED** | All fields present across 8 hits |
| Codex review | **NOT RUN** | Pending |
| Human accepts experiment in PIE | **PLAYTEST-VERIFIED** (informal) | User confirmed "it worked"; formal close pending |

## New physics facts discovered

- `spine_05` is a confirmed `PA_Mannequin` body. Frontal torso hits at Z ≈ 134–138 cm
  consistently resolve to it (distance ~30–32 cm from capsule impact point).
- `upperarm_l` is a confirmed `PA_Mannequin` body. Angled/side hits at the same height
  resolve to it (distance ~28 cm).
- The runtime sequence `SetCollisionEnabled(QueryAndPhysics)` → `RecreatePhysicsState()`
  → `SetAllBodiesBelowSimulatePhysics(spine_03, true, true)` successfully activates Chaos
  body simulation for the upper-body chain on a mesh that was initialized as QueryOnly.
- Restoration via `SetAllBodiesBelowSimulatePhysics(false)` → `SetCollisionEnabled(QueryOnly)`
  → `RecreatePhysicsState()` reliably returns the mesh to kinematic/animated state.

## "Invalid Bodies" warning change

- First BE-0003 attempt (no reaction): warning appeared at **PIE startup**.
- This session (working reaction): warning appeared at **PIE teardown**, triggered by
  `OnUnregister` calling `RecreatePhysicsState()` with QueryOnly during world tear-down.
  This is expected and harmless.

## Runtime parameters used

All at component default values (not tuned from Details panel this session):

- `UpperBodyReactionDurationSecs` = 0.40
- `UpperBodyResponseVelocity` = 200.0
- `UpperBodyChainRoot` = spine_03

## Known gaps

- Capsule-fallback path not exercised (PA always resolved); a test where no PA is present
  or body lookup genuinely fails is not covered.
- `AppliedVelocity` is a fixed-magnitude velocity change (`bVelChange=true`); it does not
  scale with fist speed or strike angle. Calibration is the suggested next task.
- `RecreatePhysicsState()` cost not measured; brief hitches not reported by user but not
  formally profiled.
- Multi-FPS matrix (30/60 FPS) not run.
