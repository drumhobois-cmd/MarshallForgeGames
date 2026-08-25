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

## Crash-fix re-review — passed

Date: **2026-08-25**
Reviewed commit: `a5c2854` — `fix(combat): guard ClearTimer against executing restore timer`

The explicit `ExecutePhysicsRestore(bool bCancelRestoreTimer)` contract passes
independent Sol lifecycle review. The expiry lambda passes `false`, leaving its
executing `FTimerData` and captured closure alive until its later log reads
complete; UE then naturally removes the non-looping timer after the callback
returns. Re-hit and active-world unregister pass `true`, preserving pending
timer cancellation. Both paths invalidate the component handle and restore and
clear response state; teardown remains direct non-physics cleanup.

The exact UE 5.8.1 Editor Development rebuild recorded in
`build_summary.md` succeeded in 20.09 seconds, compiling
`BECombatComponent.cpp`; no new warnings were reported. Canonical and live
5.8-workspace combat-source hashes match.

**PIE matrix reauthorized.** Evidence remains **COMPILES** pending the complete
manual matrix. Begin with one normal expiry, then re-hit replacement, the 30/60
FPS named-body/miss/fallback cases, and active-response PIE teardown. Require
one `Phase=Restored` per completed replacement response, no post-restoration
samples, no crash, and no `Invalid Bodies` warning. Do not save assets.

Non-blocking documentation precision: the previous comment describes
`ClearTimer` as crashing TimerManager. UE supports clearing an executing timer;
the actual defect was the callback's subsequent use of its destroyed captured
closure.

## Partial PIE log review — matrix remains open

Date: **2026-08-25**

Two human-supplied PIE log captures were independently reviewed. They are
partial evidence only; no source or asset change was made for this review.

**Observed normal expiry:** one named-body `spine_05` response emitted one
finite `PreApply`, `Applied`, 49 finite `PostPhysics` samples (ordinal 0–48),
and exactly one `Restored`. Elapsed time increases from approximately zero to
`0.4044 s`; body validity/simulation, tracked body-local surface point, and
unit-labelled fields are present. The request remains approximately `200 cm/s`
with `bVelChange=true` and `ConfiguredDuration=0.4000 s`. No fatal error or
`Invalid Bodies` warning occurs in the supplied excerpt. This traverses the
former crash callback successfully for one response.

**Observed active-response teardown:** a second named-body response emitted 15
post-physics samples through `ElapsedSecs=0.1167 s`, then
`BeginTearingDown` and `CleanupWorld`. No fatal error or `Invalid Bodies`
warning occurs in the supplied excerpt. This does not directly demonstrate
internal state clearing or a successful subsequent PIE restart.

**Not evidenced:** explicit `t.MaxFPS 30` and `t.MaxFPS 60` runs (the observed
roughly 0.00833 s cadence is insufficient); re-hit replacement inside 0.40 s;
three named-body hits at each cap; range and lateral misses; no-eligible-body
fallback and its existing 250 cm/s shove with no named-body samples; restart
after teardown; no asset save; or final human acceptance.

Evidence remains **COMPILES**, not `PLAYTEST-VERIFIED`. The next manual pass
must record the FPS command/value, run the missing cases, and confirm no crash,
`Invalid Bodies`, late prior-window restoration, or post-restoration samples.

## 30 FPS PIE evidence — partial matrix pass

Date: **2026-08-25**

The supplied 19.63-second screen recording visibly shows `Cmd: t.MaxFPS 30`
and `t.MaxFPS = "30"`. Its matching PIE log uses `TickDelta=0.033333 s` and
0.0333-second post-physics intervals, corroborating the configured cap.

At that cap, named-body windows 1–3 each show one finite `PreApply`, one
`Applied`, 13 bounded `PostPhysics` samples through `ElapsedSecs=0.4000 s`,
and exactly one `Restored`. All samples report a valid, simulating `spine_05`
body; no crash or `Invalid Bodies` warning appears in the supplied evidence.

Windows 4–5 each report `Result=MISS`, zero hit candidates, and
`SampleCount=0`; no named-body response sample is present. The captures do not
identify one miss as range and the other as lateral, so that distinction remains
open.

Still required: a named no-eligible-body fallback showing its existing 250
cm/s capsule shove and no named-body response samples; a re-hit inside 0.40 s;
the equivalent explicit 60 FPS matrix; restart and clean hit after active
teardown; no-asset-save confirmation; and final human acceptance. Evidence
remains **COMPILES**.

## 60 FPS PIE evidence — partial matrix pass

Date: **2026-08-25**

The supplied recording visibly shows `Cmd: t.MaxFPS 60` and
`t.MaxFPS = "60"`. Its matching PIE log uses `TickDelta=0.016667 s` and
0.0167-second post-physics intervals. The recording itself is encoded at 30
fps, which is not treated as game-frame-rate evidence.

Named-body windows 2–4 each emit one finite `PreApply`, one `Applied`, 25
bounded `PostPhysics` samples through `ElapsedSecs=0.4000 s`, and exactly one
`Restored`. The sampled body is valid/simulating and `spine_05`; the fixed
request remains approximately 200 cm/s with `bVelChange=true` and a 0.40 s
duration. There is no crash or `Invalid Bodies` warning in the supplied log.

Windows 1 and 5 report `Result=MISS`, zero hit candidates, and `SampleCount=0`.
As in the 30 FPS capture, their range/lateral identity is not recorded. The
fallback, re-hit cancellation, active-response teardown at this cap, and
post-teardown restart remain untested. Evidence remains **COMPILES**.

## 30 FPS fallback evidence — passed

Date: **2026-08-25**

The supplied 30 FPS recording shows the live attacker's
`UpperBodyChainRoot=NoEligibleChain` before the jab. Its visible Output Log
records `Phase=Applied | WindowId=1 | Branch=CapsuleFallback |
Reason=NoNamedBodyResolved | LaunchVelocity=250.0 cm/s`. The contact window is
a hit, and no `BE_RESPONSE_SAMPLE_V1` record is shown for that window. This is
the required no-eligible-chain fallback path; it does not alter hit geometry or
the project default `spine_03` setting.

No crash or `Invalid Bodies` warning is visible. The 60 FPS fallback, explicit
range/lateral identity, active-response teardown at both caps, re-hit
cancellation, post-teardown restart, no-save confirmation, and human
acceptance remain outstanding. Evidence remains **COMPILES**.

## 60 FPS fallback evidence — passed

Date: **2026-08-25**

The supplied recording visibly shows `t.MaxFPS = "60"`, a PIE world maximum
tick rate of 60, and the live attacker's `UpperBodyChainRoot=NoEligibleChain`
before the test jab. The visible Output Log records a hit window with
`TickDelta=0.016667 s`, followed by `Phase=Applied | WindowId=1 |
Branch=CapsuleFallback | Reason=NoNamedBodyResolved |
LaunchVelocity=250.0 cm/s`. No `BE_RESPONSE_SAMPLE_V1` record is visible for
that window.

The fallback requirement is now evidenced at both caps without changing the
project default `spine_03` setting. No crash or `Invalid Bodies` warning is
visible. Re-hit cancellation, active-response teardown at both caps,
post-teardown restart, explicit range/lateral identity, no-save confirmation,
and human acceptance remain outstanding. Evidence remains **COMPILES**.

## 60 FPS active-response teardown — passed

Date: **2026-08-25**

The supplied 60 FPS log enters the normal `Branch=NamedBody` response with
`ChainRoot=spine_03` and `TickDelta=0.016667 s`. It records 11 valid,
simulating post-physics samples (ordinal 0–10) through `ElapsedSecs=0.1667 s`,
then `BeginTearingDown` and `CleanupWorld`. There is no `Phase=Restored`, as
expected because PIE stopped inside the configured 0.40 s response, and no
fatal error or `Invalid Bodies` warning occurs.

The recording also visibly confirms `t.MaxFPS = "60"`. It contains editor
auto-save messages outside this response excerpt, so this does not establish
the separate no-asset-save criterion. Still required: the equivalent 30 FPS
active-response teardown, re-hit cancellation, post-teardown restart/clean
hit, explicit range/lateral identity, no-save confirmation, and human
acceptance. Evidence remains **COMPILES**.

## 30 FPS active-response teardown — passed

Date: **2026-08-25**

The supplied 30 FPS log enters the normal `Branch=NamedBody` response with
`ChainRoot=spine_03` and `TickDelta=0.033333 s`. It records seven valid,
simulating post-physics samples (ordinal 0–6) through `ElapsedSecs=0.2000 s`,
then `BeginTearingDown` and `CleanupWorld`. There is no `Phase=Restored`, as
expected because PIE stopped inside the configured 0.40 s response, and no
fatal error or `Invalid Bodies` warning occurs.

The recording visibly confirms `Cmd: t.MaxFPS 30` and `t.MaxFPS = "30"`.
Active-response teardown is therefore evidenced at both required caps. The
separate no-asset-save criterion remains unknown. Still required: re-hit
cancellation, post-teardown restart/clean hit, explicit range/lateral miss
identity, no-save confirmation, and human acceptance. Evidence remains
**COMPILES**.

## 60 FPS rapid double-click re-hit attempt — cancellation not exercised

Date: **2026-08-25**

The supplied 60 FPS capture uses `TickDelta=0.016667 s` and contains two
valid named-body torso-hit windows. It does **not** prove re-hit cancellation:
Window 1 completes its bounded samples and logs `Phase=Restored` at `0.4000 s`
before Window 2 emits `PreApply`/`Applied`. Window 2 then independently runs
to its own one normal restoration. No crash or `Invalid Bodies` warning is
present.

The rapid double-click therefore appears queued by the current calibration
input/montage path, rather than producing two contact windows inside the
response duration. This is neither a cancellation pass nor evidence that the
timer-cancellation implementation is wrong. Re-hit cancellation remains
unverified; do not alter assets to force it. If a second named hit cannot be
generated before the first restoration, record that constraint and decide a
separately scoped, source-level test hook with the human owner.

## 60 FPS restart after active teardown, then clean hit — passed

Date: **2026-08-25**

The supplied recording first returns to the Editor following an active-response
world teardown. It then starts a fresh PIE world, visibly reports a maximum
tick rate of 60, and performs a new normal hit. The fresh session's Output Log
shows its new jab/contact sequence, bounded response samples through the
configured `0.4000 s` window, and one `BE_UPPER_BODY_RESPONSE_V1 |
Phase=Restored | WindowId=1`. No crash or `Invalid Bodies` warning is visible.

This demonstrates that an active-response teardown does not prevent a clean
named-body response in the next PIE world. It does not establish the separate
no-asset-save criterion. Evidence remains **COMPILES** pending the other open
matrix items and human acceptance.
