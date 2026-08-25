# BE-0004 — independent Sol implementation review

Date: **2026-08-25**  
Reviewer: **Codex, with required Sol physics/lifecycle gate**  
Current evidence: **COMPILES** only. **Do not begin PIE verification yet.**

## Build evidence reviewed

The supplied UE 5.8.1 Editor Development build succeeded after compiling
`BECombatComponent.cpp`. It targets the `BruisedEgoPrototype 5.8` workspace;
the reviewed combat-source hashes match the canonical tracked project. The
only recorded warning is the pre-existing `IncludeOrderVersion = Unreal5_6`
upgrade notice. Add a retained raw UnrealBuildTool-log pointer before complete
build evidence is claimed.

## Blocking findings

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
