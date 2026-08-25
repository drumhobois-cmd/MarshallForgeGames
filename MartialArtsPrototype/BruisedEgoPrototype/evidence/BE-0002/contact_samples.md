# BE-0002 contact samples

- Status: **PLAYTEST-VERIFIED — matrix captured; final approval pending**
- Engine/map: Unreal Engine 5.8.1; `/Game/Maps/L_CombatGym.L_CombatGym`
- Required markers: `BE_CONTACT_SAMPLE_V1`, `BE_CONTACT_WINDOW_V1`
- Behaviour retained for comparison: `Fist contact:` and the temporary `250 cm/s` horizontal shove

## What to capture

For every hit, retain the complete `BE_CONTACT_SAMPLE_V1` line and its matching `BE_CONTACT_WINDOW_V1` line. For every miss, retain the window line and confirm there is no sample with that attacker/window ID.

Record the observed—not assumed—values for:

- attacker and window ID;
- raw candidate/skeletal-candidate counts and selected index;
- target actor, selected component/class, and `BoneName`;
- sweep start/end/radius;
- tick delta and kinematic path-speed estimate;
- impact point, unitless normal, blocking status, and initial-penetration status;
- component/named-body simulation status;
- raw named-body mass in kilograms or its explicit unavailable reason.

## Trial matrix

| FPS cap | Scenario | Required repetitions | Sample count per window | Window result | Actual |
| ---: | --- | ---: | ---: | --- | --- |
| 30 | In-range aligned hit | 3 | 1 | `HIT`, `SampleCount=1` | NOT RUN |
| 30 | Out-of-range miss | 1 | 0 | `MISS`, `SampleCount=0` | NOT RUN |
| 30 | Lateral miss | 1 | 0 | `MISS`, `SampleCount=0` | NOT RUN |
| 60 | In-range aligned hit | 3 | 1 | `HIT`, `SampleCount=1` | NOT RUN |
| 60 | Out-of-range miss | 1 | 0 | `MISS`, `SampleCount=0` | NOT RUN |
| 60 | Lateral miss | 1 | 0 | `MISS`, `SampleCount=0` | NOT RUN |

## Captured excerpts

The user ran one fresh PIE smoke test after the successful build. The log recorded stance ON/OFF, one jab window, one contact sample, and one matching hit summary.

BE_CONTACT_SAMPLE_V1 | WindowId=1 | Attacker=BP_ThirdPersonCharacter_C_0 | CandidateHitCount=1 | SkeletalCandidateHitCount=0 | SelectedHitIndex=0 | Actor=BP_ThirdPersonCharacter_C_1 | Component=CollisionCylinder | ComponentClass=CapsuleComponent | BoneName=None | SweepRadius=6.0 cm | TickDelta=0.008334 s | PathSpeedEstimate=210.9 cm/s (kinematic path-speed estimate) | ImpactNormal=(-0.916,-0.401,0.000) unitless | BlockingHit=true | StartPenetrating=false | ComponentSimulatingPhysics=false | BodySimulatingPhysics=Unavailable (not skeletal) | BodyMass=Unavailable (not skeletal)
BE_CONTACT_WINDOW_V1 | WindowId=1 | Attacker=BP_ThirdPersonCharacter_C_0 | Result=HIT | SampleCount=1 | TotalTicks=10 | HitFrameCandidateHitCount=1 | HitFrameSkeletalCandidateHitCount=0

Observed result: the selected component was Bob's CollisionCylinder capsule, with BoneName=None; no skeletal candidate or body mass was fabricated. The tick delta and path-speed estimate were finite and non-negative.

## Full matrix capture — 2026-08-19

At 30 FPS, three hit windows each emitted one sample and two additional windows emitted MISS with SampleCount=0. The hit path-speed estimates were 347.2, 380.6, and 347.2 cm/s; each tick delta was 0.033334 s and each window ticked 3 frames.

At 60 FPS, three hit windows each emitted one sample and two additional windows emitted MISS with SampleCount=0. The hit path-speed estimates were 387.3, 387.2, and 393.8 cm/s; each tick delta was 0.016667 s and each window ticked 5 frames.

All six hit samples selected CollisionCylinder / CapsuleComponent with BoneName=None, SkeletalCandidateHitCount=0, ComponentSimulatingPhysics=false, BodySimulatingPhysics=Unavailable, and BodyMass=Unavailable. Every hit window had SampleCount=1; every miss window had SampleCount=0.

The two miss windows at each cap were run in the requested out-of-range then lateral order. The telemetry schema records MISS but does not encode scenario names, so the individual range-versus-lateral mapping is based on test order rather than the marker itself.

## Result

**PLAYTEST-VERIFIED for the captured matrix.** Final human approval remains pending.
