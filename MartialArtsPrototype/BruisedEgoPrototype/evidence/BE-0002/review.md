# BE-0002 Codex source review

- Date: 2026-08-19
- Reviewer: Codex
- Implementation owner: Claude Code 2.1.235
- Engine API baseline: local Unreal Engine 5.8.1 headers and source
- Review result: **PASS at COMPILES**
- Build status: **SUCCEEDED** — `BruisedEgoPrototypeEditor Win64 Development`, UE 5.8.1

## Reviewed diff

Only these implementation files changed:

- `Source/BruisedEgoPrototype/BECombatComponent.h`
- `Source/BruisedEgoPrototype/BECombatComponent.cpp`
- `Source/BruisedEgoPrototype/BEAnimNotifyState_JabWindow.cpp`

No BE-0002 change was made to Build.cs, `.uproject`, Config, collision, plugins, maps, Blueprints, animations, Physics Assets, or other Unreal binary assets.

## Review rounds

The first Claude handoff preserved gameplay behaviour but had acceptance-blocking telemetry issues: incomplete finite-value checks, insufficient body-mass validity, root/component simulation reported in place of named-body simulation, ambiguous candidate counts, and incomplete unavailable/unit context.

Those findings were returned to Claude rather than patched by Codex. The corrected implementation now:

- passes `FrameDeltaTime` unchanged from the notify tick;
- reports delta only when finite and strictly positive;
- calculates the kinematic fist path-speed estimate in double precision and reports it only when delta, path distance, and quotient are finite and non-negative;
- uses raw `Hits.Num()` and an actor-filterless skeletal-result count without changing selection;
- records the selected raw hit index while preserving the original first-valid-actor order;
- distinguishes component simulation from named-body simulation;
- requires a skeletal component, non-`None` bone, non-null and valid body instance, and finite positive `GetBoneMass(BoneName, true)` result before reporting raw body mass in kilograms;
- records explicit unavailable reasons instead of inventing values;
- labels the impact normal unitless and records blocking/initial-penetration context;
- identifies the attacker plus per-component window ID;
- emits no phantom window summary when no sweep window is active.

## Preserved baseline behaviour

Static comparison confirms these remain unchanged:

- `SweepMultiByObjectType` against `ECC_Pawn`;
- owner ignored;
- `6.0 cm` swept-sphere radius;
- hit-array iteration from index zero upward;
- first result with a non-null actor is selected;
- one processed target per notify window;
- existing `Fist contact:` marker;
- `LaunchCharacter(ShoveDir * 250.0f, true, false)` calibration shove;
- `PreviousFistLocation` advances at the existing end point;
- existing debug sphere and path line.

## Local API verification

Verified against the installed UE 5.8.1 source:

- `FBodyInstance::IsValidBodyInstance()` — `Engine/Source/Runtime/Engine/Classes/PhysicsEngine/BodyInstance.h`.
- `USkeletalMeshComponent::GetBoneMass(FName, bool)` — `Engine/Source/Runtime/Engine/Classes/Components/SkeletalMeshComponent.h`.
- `FMath::IsFinite(float/double)` — platform math headers.
- `USkeletalMeshComponent::IsSimulatingPhysics(FName)` and `GetBodyInstance(FName)` — local component header.

`PhysicsEngine/BodyInstance.h` is provided by the existing Engine dependency; no new module or plugin is required.

## Checks run

- `git diff --check` — no whitespace errors; Windows LF-to-CRLF advisories only.
- Signature/call-site search — one `UpdateFistSweep` definition and one notify call, both with `FrameDeltaTime`.
- Source-scope comparison — exactly the three task-allowed implementation files changed.
- Stable-marker inspection — one `BE_CONTACT_SAMPLE_V1` log call and one `BE_CONTACT_WINDOW_V1` log call.
- Exact preservation searches for sweep configuration, `Fist contact:`, `LaunchCharacter`, and previous-fist advancement.
- Independent corrected-diff audit — no unresolved findings.
- Full target build — succeeded on 2026-08-19 with no compiler errors or compiler warnings; see `build_summary.md`.

## Remaining verification

- Named 30/60 FPS PIE matrix.
- Actual selected component/bone/body/mass evidence.
- Human playtest acceptance.

The implementation is now **COMPILES**. Runtime behaviour remains unverified until the named PIE matrix is completed.
