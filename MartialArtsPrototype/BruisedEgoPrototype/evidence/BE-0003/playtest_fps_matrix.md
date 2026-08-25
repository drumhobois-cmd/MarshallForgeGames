# BE-0003 — 30/60 FPS PIE matrix

Date: **2026-08-21**  
Map: `/Game/Maps/L_CombatGym.L_CombatGym`  
Engine: UE 5.8.1, CL 56057345  
Evidence label: **PLAYTEST-VERIFIED** for the recorded hit/miss matrix.

## Method and provenance

The human tester ran PIE at the two requested caps, recording the visible play
session and Output Log. The recordings show the 30/60 cap setup, gameplay, and
the emitted telemetry. The original recordings remain local rather than copied
into the repository:

- 30 FPS: `20260821-0148-09.6691053.mp4`
- 60 FPS: `20260821-0149-22.8510366.mp4`

The tester identified the same window mapping at both caps: WindowId 1 is the
range miss and WindowId 5 is the lateral miss. The three intervening windows
are in-range hits.

## Results

| Cap | Range miss | In-range hits | Lateral miss | Result |
| --- | --- | --- | --- | --- |
| 30 FPS | WindowId 1: `MISS`, zero samples, 3 ticks | WindowIds 2–4: each `HIT`, exactly one sample, `Branch=NamedBody`, `InferredBone=spine_05`, 0.40 s response, and `Phase=Restored` | WindowId 5: `MISS`, zero samples, 3 ticks | **PLAYTEST-VERIFIED** |
| 60 FPS | WindowId 1: `MISS`, zero samples, 5 ticks | WindowIds 2–4: each `HIT`, exactly one sample, `Branch=NamedBody`, `InferredBone=spine_05`, 0.40 s response, and `Phase=Restored` | WindowId 5: `MISS`, zero samples, 5 ticks | **PLAYTEST-VERIFIED** |

At 30 FPS, contact ticks reported `TickDelta=0.033334 s`; at 60 FPS,
`TickDelta=0.016667 s`. The response and restoration telemetry appeared once
per accepted hit. The recordings visibly show the bounded upper-body reaction
at both caps. No crash was observed.

## Warning and remaining checks

- The existing `Invalid Bodies` warning appeared at PIE teardown at both caps.
  Earlier BE-0003 evidence identifies it as the known teardown-time
  `RecreatePhysicsState()` warning; it did not coincide with a crash.
- The tester explicitly confirmed that they did not use Save or Save All during
  either PIE run. No Unreal asset was saved during the matrix.
- This matrix does not exercise the capsule/unnamed fallback branch; the
  closest-body lookup resolved `spine_05` for every hit.
- Codex independent source/evidence review and formal human acceptance remain
  pending.
