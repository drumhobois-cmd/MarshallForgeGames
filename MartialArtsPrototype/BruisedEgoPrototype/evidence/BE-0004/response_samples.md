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

Not yet captured: the no-eligible-body fallback, its existing 250 cm/s capsule
shove, and proof that it emits no named-body response samples.
