# BE-0003 — bone-aware upper-body response experiment

Status: `COMPLETED — HUMAN ACCEPTED 2026-08-25`  
Owner: `Claude`  
Reviewer: `Codex`  
Final acceptance: `Human`  
Created: `2026-08-19`

## Goal

Demonstrate one constrained, temporary upper-body reaction when a jab resolves to a named Physics Asset body, while preserving the approved capsule-contact behaviour as the fallback.

## Why now

BE-0002 is approved and shows that current hits consistently resolve to Bob's capsule (`CollisionCylinder`) with no named bone or valid body mass. This experiment must first establish named-body resolution before any larger physical-response system is attempted.

## Known-good baseline

- BE-0002 is human-approved; see `evidence/BE-0002/approval.md`.
- UE 5.8.1, map `/Game/Maps/L_CombatGym.L_CombatGym`.
- Current accepted hit preserves the existing `LaunchCharacter(... * 250.0f, true, false)` shove.
- Current telemetry remains the source of truth for selected component, bone, simulation state, and mass.

## Allowed changes

- Source-only implementation in the combat component and explicitly required Physics Control integration files.
- Read-only inspection of `SK_Mannequin`/`PA_Mannequin` and Bob's Blueprint setup.
- New telemetry markers and evidence documentation.
- No asset saves, Physics Asset edits, collision-profile changes, map/Blueprint edits, Config, `.uproject`, plugin, or Build.cs changes unless separately approved.

## Non-goals

- No full ragdoll, impulse/force system, balance, stumble, fatigue, damage, or injury model.
- No removal or recalibration of the existing capsule fallback or 250 cm/s shove.
- No broad Physics Control architecture.

## Acceptance criteria

- [x] A named Physics Asset body resolved from contact produces one bounded, visibly readable upper-body reaction. *(The capsule contact is resolved against exact Physics Asset body geometry; it is not a direct skeletal-mesh sweep hit.)*
- [x] A capsule/unnamed hit uses the existing fallback with no fabricated bone response.
- [x] Controls are softened only for a bounded interval and restored reliably. *(Completed responses restore once; active-response PIE teardown leaves no `Invalid Bodies` warning.)*
- [x] Misses produce no response.
- [x] 30/60 FPS hit and miss checks pass without crashes or new asset changes. *(Human confirmed no Unreal asset was saved during either PIE run.)*
- [x] Telemetry records selected bone/body, response branch, duration, and restoration.
- [x] Codex review finds no blocking scope or safety issues.
- [x] Human accepts the experiment in PIE. *(Recorded 2026-08-25.)*

## Safety gate

Keep the Physics Asset editor closed during implementation and testing. Do not modify or save `PA_Mannequin`; the prior Unreal crash occurred during Skeleton/Physics Asset editor teardown.

## Handoff

- Evidence label reached: `PLAYTEST-VERIFIED` for exact Physics Asset body-geometry resolution from capsule contact, fallback, restoration/teardown, and the 30/60 FPS hit/miss matrix. Final Codex review passed and the human accepted the experiment on 2026-08-25. See `evidence/BE-0003/review.md`.
- Suggested implementation owner: Claude; Codex reviews the diff and evidence.
- Suggested next task after acceptance: tune response calibration only after named-body resolution is reliable.
