# BE-0004 — response sample excerpts

Evidence label: **COMPILES** (manual matrix partial)
Date: **2026-08-25**

This record summarizes the human-supplied PIE log captures. It records raw
observable values only; none are contact impulse, force, torque, effective
mass, energy, damage, balance, or injury measurements.

| Cap | Named response evidence | Motion sample cadence | Restore | Miss evidence |
| --- | --- | --- | --- | --- |
| `t.MaxFPS 30` | Windows 1–3: `spine_05`, valid/simulating, one `PreApply` and one `Applied` each | 13 samples/window, ordinal 0–12, `0.0000`–`0.4000 s`, `TickDelta=0.033333 s` | One `Restored`/window | Windows 4–5: zero candidates and `SampleCount=0`; range/lateral identity not recorded |
| `t.MaxFPS 60` | Windows 2–4: `spine_05`, valid/simulating, one `PreApply` and one `Applied` each | 25 samples/window, ordinal 0–24, `0.0000`–`0.4000 s`, `TickDelta=0.016667 s` | One `Restored`/window | Windows 1 and 5: zero candidates and `SampleCount=0`; range/lateral identity not recorded |

Across both caps, `RequestedDeltaV` remains approximately 200 cm/s,
`bVelChange=true`, and `ConfiguredDuration=0.4000 s`. All observed fields are
unit-labelled; body-local tracked surface point, body position/rotation,
linear velocity, angular velocity in rad/s, point velocity, and response-axis
linear velocity are present. No `Invalid Bodies` warning or fatal error occurs
in the supplied excerpts.

**30 FPS fallback:** a recording of the live attacker with
`UpperBodyChainRoot=NoEligibleChain` shows
`Branch=CapsuleFallback | Reason=NoNamedBodyResolved | LaunchVelocity=250.0 cm/s`
on a hit window. No `BE_RESPONSE_SAMPLE_V1` record is visible for that window.

**60 FPS fallback:** the equivalent runtime override records the same fallback
line on a hit window with `TickDelta=0.016667 s`; no
`BE_RESPONSE_SAMPLE_V1` record is visible for that window.

**60 FPS active teardown:** normal named-body window 1 records 11 valid,
simulating post-physics samples through `0.1667 s`, then PIE teardown begins.
No restoration, fatal error, or `Invalid Bodies` warning is present, as expected
for a response interrupted before its 0.40 s expiry.

**30 FPS active teardown:** normal named-body window 1 records seven valid,
simulating post-physics samples through `0.2000 s`, then PIE teardown begins.
No restoration, fatal error, or `Invalid Bodies` warning is present, as expected
for a response interrupted before its 0.40 s expiry.
