# BE-0003 — independent final review

Date: **2026-08-25**  
Reviewer: **Codex**  
Scope: the BE-0003 combat-component source, local Unreal Engine 5.8.1 headers,
and the fresh PIE log at `Saved/Logs/BruisedEgoPrototype.log`.  
Evidence label: **PLAYTEST-VERIFIED** for the named-body, fallback, restoration,
and 30/60 FPS cases. The source rebuild was reported successful by the human;
the exact build command and UnrealBuildTool result are **NOT RECORDED**, so this
review does not independently add a new **COMPILES** claim.

## Prior blocking findings resolved

### BE-0003-R1 — resolved

The response now uses UE 5.8.1's
`USkeletalMeshComponent::GetClosestPointOnPhysicsAsset(..., bApproximate=false)`.
The local header documents that `false` uses body distance rather than the
old bone-transform approximation. The implementation requires all of:

- a named Physics Asset body surface within `MaxBodyResolutionRadiusCm`;
- membership below `UpperBodyChainRoot`;
- a valid body before physics recreation; and
- a valid re-acquired body after recreation.

The fresh 30/60 FPS sessions resolved `spine_05` at 16.5–18.2 cm, reported
`ChainVerified=true` and `PostRecreateBodyValid=true`, and restored after the
configured 0.40 s interval. The contact sweep still selects Bob's capsule;
this is deliberately logged as exact Physics Asset body-geometry resolution
from the capsule impact, not a direct skeletal-mesh sweep hit.

### BE-0003-R2 — resolved

`ExecutePhysicsRestore()` clears and invalidates the timer, restores only a
valid pending mesh, then resets the pending reference. `OnUnregister()` avoids
mesh physics mutation when the world is tearing down and only clears state.
The duration is also required to be finite and strictly positive before any
physics-state change.

The recorded active-response teardown stopped PIE 67 ms after application,
before its 0.40 s restore timer could fire. No `Invalid Bodies` warning,
crash, or subsequent restore mutation was logged. Completed-response sessions
each logged exactly one restoration before PIE exit.

## Fresh PIE evidence

Map: `/Game/Maps/L_CombatGym.L_CombatGym`  
Engine: UE 5.8.1, CL 56057345  
Log: `Saved/Logs/BruisedEgoPrototype.log` (2026-08-25)

| Case | Recorded result |
| --- | --- |
| 30 FPS | Range miss (Window 1), three named `spine_05` body resolutions (Windows 2–4), each with one 0.40 s restoration, then lateral miss (Window 5). Contact windows had 3 ticks. |
| 60 FPS | Same range-miss / three named-body / lateral-miss sequence. Contact windows had 5 ticks. |
| Fallback | `Branch=CapsuleFallback`, `Reason=NoNamedBodyResolved`, and the retained `LaunchVelocity=250.0 cm/s` were recorded in separate PIE runs. No fabricated named-body response was emitted. |
| Teardown | PIE was stopped during a named-body response; no `Invalid Bodies` warning or crash followed. Completed-response exits were also warning-free in the reviewed log segment. |

The human reported that the source was rebuilt and that the named-body,
fallback, and teardown tests passed at 30 and 60 FPS. No save or asset change
is claimed by this review.

## Acceptance audit

| Criterion | Review result | Evidence / limit |
| --- | --- | --- |
| Named Physics Asset body resolution, bounded visible reaction | **PLAYTEST-VERIFIED** | Exact PA geometry resolved `spine_05` in the fresh 30/60 runs; the response remains the documented, temporary fixed 200 cm/s velocity change. |
| Capsule/unnamed fallback | **PLAYTEST-VERIFIED** | Fresh telemetry shows `CapsuleFallback` with the preserved 250 cm/s character shove. |
| Bounded reliable restoration | **PLAYTEST-VERIFIED** | One restoration per completed response; active-response teardown emitted no invalid-body warning. |
| Miss produces no response | **PLAYTEST-VERIFIED** | Range and lateral misses at both caps emitted no response. |
| 30/60 FPS hit and miss checks, no crash | **PLAYTEST-VERIFIED** | Three named-body hits and two misses at each cap; no crash recorded. |
| Telemetry records response state | **PLAYTEST-VERIFIED** | Branch, resolved body, geometry distance, chain/body guards, velocity, duration, and restore phase are logged. |
| Codex review finds no blocking scope or safety issue | **PASSED** | R1 and R2 are resolved in the reviewed source and evidence. |
| Human acceptance | **PENDING** | Required before moving the task from `tasks/active/`. |

## Residual limits and watch items

- This is exact Physics Asset geometry resolution from a capsule contact, not
  direct named-skeletal collision. That is appropriate to this constrained
  experiment but does not complete final body-geometry-aware hit detection.
- `200 cm/s` with `bVelChange=true` remains a fixed, mass-independent
  calibration response. It is not force, impulse, energy, or measured impact
  response.
- `RecreatePhysicsState()` cost is unprofiled. Watch for hitches before
  scaling beyond this single-target experiment.
- The component holds one pending restore target. Multi-target response
  management is outside BE-0003's scope.

## Review conclusion

**No blocking scope or safety issue remains for BE-0003.** The experiment is
ready for formal human PIE acceptance. The task remains active until that
acceptance is recorded.
