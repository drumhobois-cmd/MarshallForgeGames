# BE-0004 — independent Sol implementation review

Date: **2026-08-25**
Reviewer: **Codex, with required Sol physics/lifecycle gate**  
Current evidence: **COMPILES**. The corrected source passed final Sol
re-review; PIE verification is authorized.

## Build evidence reviewed

The supplied UE 5.8.1 Editor Development build succeeded after compiling
`BECombatComponent.cpp`. It targets the `BruisedEgoPrototype 5.8` workspace;
the reviewed combat-source hashes match the canonical tracked project. The
only recorded warning is the pre-existing `IncludeOrderVersion = Unreal5_6`
upgrade notice. Add a retained raw UnrealBuildTool-log pointer before complete
build evidence is claimed.

## Initial blocking findings

### BE-0004-R1 [P2] — invalid body samples fabricate zero-valued measurements

**File:** `Source/BruisedEgoPrototype/BECombatComponent.cpp`, `TickComponent`
(around lines 565–584).

When `BodyValid=false`, transform and velocity values remain zero and are then
logged with physical units. Zero is plausible data, not an unavailable value.

**Correction:** emit explicit `Unavailable (invalid body instance)` values for
transform, linear velocity, angular velocity, point velocity, and response
projection whenever the body is invalid. Do not log numeric zero placeholders.

### BE-0004-R2 [P2] — sampled surface point is not body-attached after PreApply

**File:** `Source/BruisedEgoPrototype/BECombatComponent.cpp`, captured point
(around line 296) and `GetUnrealWorldVelocityAtPoint` (around line 580).

`ActiveResponseResolvedPoint` stores the original world-space Physics Asset
surface point. The UE 5.8.1 point-velocity API consumes a point relative to the
body's current centre of mass, so the fixed coordinate no longer represents the
surface material point after the body moves.

**Correction:** convert the resolved point to body-local space at PreApply.
Transform it using each sampled current body transform, log the current world
point, and pass that point to `GetUnrealWorldVelocityAtPoint`. A deliberately
fixed point must instead be labelled a fixed-world spatial probe.

### BE-0004-R3 [P2] — teardown incompletely clears sampling state

**File:** `Source/BruisedEgoPrototype/BECombatComponent.cpp`, `OnUnregister`
teardown branch (around lines 635–640).

The branch leaves response start time, configured duration, resolved point, and
response direction stale.

**Correction:** reset every BE-0004 sampling field in the teardown branch
without calling `SetComponentTickEnabled` during world teardown.

## Verified good

- `TG_PostPhysics` is correctly configured and executes before TimerManager.
- `GetBodyMass()` is correctly described as raw solver-body mass, not effective
  mass. Angular velocity is `rad/s`; linear and point velocity are `cm/s`.
- Fixed `200 cm/s`, `bVelChange=true`, 0.40 s response, named-body resolution,
  and the 250 cm/s fallback remain unchanged.

## Required resubmission

1. Correct R1–R3 without changing response behaviour or scope.
2. Add the raw UBT-log pointer to `evidence/BE-0004/build_summary.md`.
3. Rebuild `BruisedEgoPrototypeEditor Win64 Development`; record command,
   engine, result, and warnings.
4. Send the source diff and evidence to Codex. PIE may begin only after review
   passes.

## R1–R3 correction re-review — passed

Date: **2026-08-25**  
Reviewed commit: `6f5ffbb` — `fix(combat): apply BE-0004 R1-R3 review corrections`

All three P2 findings are resolved:

- **R1:** Every invalid-body physical field and the direction projection now
  emits `Unavailable (invalid body instance)`, rather than a plausible numeric
  zero, while validity booleans remain visible.
- **R2:** The resolved Physics Asset surface point is transformed into local
  body coordinates at PreApply, transformed back to the current world position
  in each post-physics sample, logged, and passed as the required world-space
  point to `GetUnrealWorldVelocityAtPoint`.
- **R3:** Normal restoration and the direct teardown path reset every BE-0004
  sampling field. Teardown does not call tick enable/disable.

The raw UBT log at
`C:\Users\samma\AppData\Local\UnrealBuildTool\Log.txt` confirms the UE
5.8.1 Editor Development rebuild: seven actions, `Result: Succeeded`, total
execution time 21.37 seconds. The built source hashes match the canonical
tracked source.

**PIE matrix authorized.** Evidence remains **COMPILES** until the named-body,
miss, fallback, and active-response teardown cases pass at 30 and 60 FPS.

### Non-blocking timing clarification

UE 5.8.1 executes `TG_PostPhysics` before TimerManager. An expiry-frame
post-physics sample can therefore appear before that frame's restoration. This
is expected bounded behaviour; record it during PIE rather than requiring its
absence.

## PIE crash diagnosis — source repair required

Date: **2026-08-25**
Evidence: the first named-body punch produced valid bounded post-physics
samples through `ElapsedSecs=0.4000 s`, then Unreal Editor crashed before a
`Phase=Restored` record. The call stack identifies the restore timer lambda at
`BECombatComponent.cpp:336`.

**Finding (high confidence):** the restore timer lambda calls
`ExecutePhysicsRestore()`, which calls `ClearTimer(PhysicsRestoreHandle)` on
the timer that is currently executing. In UE 5.8.1, the `Executing` path
removes that timer's `FTimerData` immediately. Its `FTimerDelegate` owns the
lambda closure, so the subsequent log reads of captured `CapturedBone` and
`CapturedRoot` are use-after-free. `TWeakObjectPtr` protects the component
pointer only; it does not keep the callback closure alive.

**Required correction:** make timer cancellation explicit, with no default
argument. The expiry callback must call
`ExecutePhysicsRestore(/* bCancelRestoreTimer */ false)`, while re-hit and
active-world unregister paths call it with `true`. Only the cancellation path
may call `ClearTimer`; both paths must invalidate the component's handle and
clear restore/sampling state. The one-shot expiry timer will then be removed by
`FTimerManager` after its callback returns. Do not substitute a weak-bound
delegate alone; that does not fix self-removal.

**Verification required after correction:** rebuild the named Editor target,
then test one normal expiry and `Phase=Restored` log, a re-hit inside 0.40 s,
30/60 FPS repeated named-body hits, and stopping PIE during an active response.
Confirm no crash, no `Invalid Bodies` warning, and exactly one restoration per
response. The evidence label remains **COMPILES**; PIE acceptance failed and
must restart after the repair.
