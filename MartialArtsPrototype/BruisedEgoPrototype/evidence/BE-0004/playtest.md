# BE-0004 — human PIE playtest record

Evidence label: **COMPILES** (human matrix incomplete)
Date: **2026-08-25**

| Scenario | 30 FPS | 60 FPS | Result |
| --- | --- | --- | --- |
| Explicit frame cap | Recording shows `t.MaxFPS = "30"` | Recording shows `t.MaxFPS = "60"` | Observed |
| Three eligible torso hits | Three complete named-body responses | Three complete named-body responses | Observed |
| Range and lateral miss | Two zero-candidate misses; geometry labels absent | Two zero-candidate misses; geometry labels absent | Partial |
| No-eligible-body fallback | Runtime `NoEligibleChain`: 250 cm/s capsule fallback; no response sample shown | Same fallback at `TickDelta=0.016667 s`; no response sample shown | Observed |
| Stop PIE during active response | Earlier uncapped partial capture only | Named response stopped at `0.1667 s`; no restore/crash/`Invalid Bodies` | Partial / observed at 60 |
| Restart after active teardown, then normal hit | Not captured | Not captured | NOT RUN |
| Re-hit within 0.40 s | Not captured | Not captured | NOT RUN |
| No asset saves | Not recorded | Not recorded | UNKNOWN |

Observed normal responses restore once after the bounded 0.40 s sampling
period, with no supplied `Invalid Bodies` warning or crash. Human acceptance is
pending completion of the remaining scenarios.
