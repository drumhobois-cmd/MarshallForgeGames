# BE-0004 — measure named-body response observability

Status: `IN PROGRESS — P2 REVIEW CORRECTIONS REQUESTED`  
Owner: `Claude`  
Reviewer: `Codex`  
Final acceptance: `Human`  
Created: `2026-08-25`

## Goal

For each eligible BE-0003 named-body reaction, record a bounded, unit-labelled
pre-apply and post-physics motion sample so a later task can calibrate a
physical response from evidence rather than the current fixed velocity demo.

## Why now

BE-0003 proved exact Physics Asset body-surface resolution, capsule fallback,
and teardown-safe restoration. Its `200 cm/s`, `bVelChange=true` response is
deliberately fixed and mass-independent. Changing that value before observing
the responding body would manufacture causality instead of measuring it.

## Known-good baseline

- BE-0003 is human-accepted; see `evidence/BE-0003/review.md`.
- The fist sweep currently selects Bob's capsule, then resolves exact Physics
  Asset surface geometry to an eligible body such as `spine_05`.
- The response enables the `spine_03` chain for `0.40 s`, applies the fixed
  `200 cm/s` velocity change with `bVelChange=true`, then restores once.
- A no-body result retains `LaunchCharacter(... * 250.0f, true, false)`.
- Current contact telemetry records only a kinematic scalar path-speed
  estimate. It does not measure strike momentum, contact impulse, force,
  torque, or effective mass.

## Allowed changes

- `Source/BruisedEgoPrototype/BECombatComponent.h`
- `Source/BruisedEgoPrototype/BECombatComponent.cpp`
- New versioned `BE_RESPONSE_SAMPLE_V1` telemetry and BE-0004 evidence/task
  records.
- A bounded transient component tick enabled only while the existing named-body
  response is active, with post-physics samples taken in `TG_PostPhysics`.

No Blueprint, map, montage, Animation Blueprint, Physics Asset, collision,
Config, `.uproject`, plugin, module, or Build.cs change is in scope.

## Non-goals

- Do not change the fixed `200 cm/s`, `bVelChange=true` request, its horizontal
  direction, `0.40 s` duration, named-body resolution, restore sequence, or
  `250 cm/s` capsule fallback.
- Do not apply an impulse at a position, force, torque, or angular impulse.
- Do not derive or claim strike momentum, kinetic energy, effective striking
  mass, transferred/contact impulse, force, torque, restitution, damage,
  stagger, balance, or injury.
- Do not treat the restore duration as contact duration.
- Do not profile or redesign `RecreatePhysicsState()` in this task.

## Inputs and references

- Applicable decisions: `BE-D002`, `BE-D003`, `BE-D009`, `BE-D010`.
- Product constraint: `docs/GAME_INTENT.md`, Animation and physics
  responsibilities.
- Local UE 5.8.1 evidence:
  - `Engine/Source/Runtime/Engine/Classes/Components/SkeletalMeshComponent.h`
    lines 306–318, 2017, 2074–2081, 2122, and 2376.
  - `Engine/Source/Runtime/Engine/Classes/PhysicsEngine/BodyInstance.h` lines
    898–904, 958, 1019–1022, and 1184–1217.
  - `Engine/Source/Runtime/Engine/Private/PhysicsEngine/BodyInstance.cpp`
    lines 3875–3888.
  - `Engine/Source/Runtime/PhysicsCore/Private/ChaosEngineInterface.cpp` lines
    851–878.
  - `Engine/Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h` lines
    97–98; `ActorComponent.h` lines 976 and 1002.

## Required telemetry

Each `BE_RESPONSE_SAMPLE_V1` record must use explicit unavailable reasons and
label all units.

- Kinematic fist velocity vector: `(SweepEnd - SweepStart) / TickDelta`, in
  `cm/s`; call it an estimate, not physical strike velocity.
- Resolved Physics Asset animation-pose surface point in world `cm`, surface
  normal (unitless), body name, and surface distance in `cm`.
- Before the velocity request: valid post-recreation `FBodyInstance` solver
  mass from `GetBodyMass()` in `kg`; COM/body transform in world `cm`; linear
  velocity and resolved-point velocity in `cm/s`; angular velocity in `rad/s`.
- Requested delta-v in `cm/s`, response direction (unitless),
  `bVelChange=true`, and configured duration in `s`.
- While the response is active: sample ordinal, actual elapsed time in `s`,
  body-valid/simulating state, COM/body transform, linear velocity, angular
  velocity, and resolved-point velocity. Raw vectors are primary; projections
  and displacement along the known response direction are permitted response
  observables.

`solver mass × requested delta-v` may be logged only as a **requested solver
impulse equivalent** in `kg·cm/s` (and optionally `N·s` after dividing by 100).
It is not a measured punch or contact impulse.

## Assumptions and unknowns

| Item | Assumption or unknown | How this task will resolve/contain it |
| --- | --- | --- |
| Post-request read timing | A same-frame game-thread read can precede Chaos simulation. | Record bounded later samples in `TG_PostPhysics`; compare actual elapsed time, not sample index. |
| Resolved PA point | It is an animation-pose Physics Asset query from a capsule impact, not dynamic mesh collision. | Label it exactly and do not treat it as a measured contact point. |
| Solver mass | One valid body mass is not chain/effective opponent mass. | Use `FBodyInstance::GetBodyMass()` only as a raw solver value; make no effective-mass claim. |
| Logging cost | Per-tick telemetry can perturb timing. | Enable tick only during the maximum 0.40 s response and retain the bounded sample count in evidence. |

## Acceptance criteria

- [ ] One named-body hit emits exactly one finite PreApply response sample and
  at least one finite `TG_PostPhysics` sample with monotonically increasing
  actual elapsed time.
- [ ] Samples record every required raw field with units or an explicit
  unavailable reason; no field is mislabelled as contact impulse, force,
  torque, effective mass, or energy.
- [ ] Range/lateral misses and the capsule fallback emit no named-body response
  samples; fallback remains the 250 cm/s character shove.
- [ ] The existing response request remains exactly `200 cm/s`,
  `bVelChange=true`, and 0.40 s; do not require observed delta-v to equal
  200 cm/s.
- [ ] Sampling state and transient ticking clear on normal restore, re-hit
  cancellation, unregister, and world teardown; no `Invalid Bodies` warning
  or crash occurs.
- [ ] At 30 and 60 FPS, named hit/miss, fallback, and active-response teardown
  tests pass without asset saves or scope changes.
- [ ] The UE 5.8.1 Editor target builds using an exact recorded command and log.
- [ ] Codex independent review finds no blocking physics, unit, or lifecycle
  issue; the human accepts the PIE evidence.

## Verification plan

### Automated/build

- Command: `UNKNOWN UNTIL VERIFIED` — record the exact UBT/Build.bat invocation
  used against the live `H:\Epic Games\UE_5.8` installation.
- Target/configuration: `BruisedEgoPrototypeEditor Win64 Development`.
- Expected result: successful build with warnings recorded verbatim.

### Unreal Editor

1. Open `/Game/Maps/L_CombatGym.L_CombatGym` in UE 5.8.1; keep the Physics
   Asset editor closed and do not save assets.
2. Filter Output Log for `BE_RESPONSE_SAMPLE_V1`,
   `BE_UPPER_BODY_RESPONSE_V1`, and `BE_CONTACT_WINDOW_V1`.
3. Verify one PreApply plus bounded PostPhysics samples on a named-body hit,
   actual elapsed time increasing, and exactly one restoration.
4. Stop PIE inside the 0.40 s response and after a completed response; verify
   state clears with no `Invalid Bodies` warning.

### Playtest

At both `t.MaxFPS 30` and `t.MaxFPS 60`:

1. Run three eligible torso hits, one range miss, and one lateral miss.
2. Run a no-eligible-body fallback case and confirm only the existing 250 cm/s
   capsule shove occurs.
3. Stop PIE during one active named-body response.
4. Record raw vectors and actual elapsed-time curves; do not tune from sample
   count or demand a fixed observed delta-v.

### Regression checks

- Existing named-body resolution, 0.40 s restoration, and capsule fallback
  behaviour remain unchanged.
- One target maximum is processed per notify window.
- No Blueprint, asset, collision, plugin, or project-setting change occurs.

## Evidence required

- `evidence/BE-0004/build_summary.md` — command, target, UE version, result,
  warnings, and raw-log pointer.
- `evidence/BE-0004/response_samples.md` — concise 30/60/Fallback/teardown
  excerpts with raw units and unavailable reasons.
- `evidence/BE-0004/playtest.md` — human scenarios, no-save confirmation, and
  visual/log outcome.
- `evidence/BE-0004/review.md` — Codex unit/lifecycle/causality review.

## Handoff

- Evidence label reached: `PROPOSED`.
- Files changed: task definition only.
- Verification run: required Sol gate against local UE 5.8.1 headers/source;
  no implementation or playtest run.
- Warnings: direct collision contact, strike impulse, force, torque, and
  effective mass remain unknown.
- Manual steps remaining: implement, build, and run the named matrix.
- Risks/unknowns: post-physics logging cost, solver timing, and the distinction
  between a body solver mass and an effective constrained-chain mass.
- Suggested next task: decide calibration only after BE-0004 evidence exists;
  do not start that task automatically.

## Review findings

Independent Sol review found three P2 blockers before PIE: invalid-body samples
log zero-valued physical measurements instead of explicit unavailable values;
the sampled Physics Asset point is fixed in world space rather than attached
to the responding body; and teardown leaves part of the sampling state stale.
See `evidence/BE-0004/review.md`. Claude must correct and rebuild before PIE.
