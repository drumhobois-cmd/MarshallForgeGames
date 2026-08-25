# Bruised Ego — decision log

Record durable choices here. Do not rewrite history: mark a decision superseded and link the replacement ID.

| ID | Date | Status | Decision | Reason / consequence |
| --- | --- | --- | --- | --- |
| BE-D001 | 2026-08-19 | Accepted | Treat Bruised Ego as a multi-year project rather than a six- or twelve-month promise. | Architecture can be staged without pretending the full scope belongs in the first slice. |
| BE-D002 | 2026-08-19 | Accepted | Preserve physics causality as a product non-negotiable. | Fidelity, coverage, and performance may be compromised; contact consequence cannot quietly become a canned animation system. |
| BE-D003 | 2026-08-19 | Accepted | Use animation/mocap for desired movement and style, while measured contact/physical state determines outcomes. | Makes mocap packs compatible with the physics-led identity. |
| BE-D004 | 2026-08-19 | Accepted | Claude Code is the default C++ implementer; Codex is the default planner/reviewer/documentation and verification agent; the user owns Editor and playtest acceptance. | Reduces overlapping edits and gives every code change an independent review path. |
| BE-D005 | 2026-08-19 | Accepted | The current `AM_Jab` mouse-click path is a calibration proof, not the final combat input model. | Existing work remains useful without redefining the directional design. |
| BE-D006 | 2026-08-19 | Accepted | Multiple martial arts may use licensed/mocap packs when they pass technical and authenticity validation. | Content breadth is large; asset sourcing is a permitted production compromise. |
| BE-D007 | 2026-08-19 | Accepted | Learned styles can mix, and learning them should not become repetitive grind. | Progress should emphasize capabilities, practice, tactical understanding, and meaningful choices. |
| BE-D008 | 2026-08-19 | Accepted | Most gym adaptation runs through a player-authored off-world schedule rather than watched real-time chores. | Respects real-life time while retaining training strategy and consequences. |
| BE-D009 | 2026-08-19 | Accepted | Exact local Unreal 5.8.1 headers/plugin source are authoritative for unstable Physics Control and Chaos APIs. | Prevents generic or outdated generated code from masquerading as an implementation. |
| BE-D010 | 2026-08-19 | Accepted | Prove the physics response architecture in a narrow, instrumented combat spike before scaling content. | The system is the highest-risk dependency and must be measured early. |
| BE-D011 | 2026-08-19 | Accepted | Do not guess build commands or `/Game/...` asset paths in automation/docs. | Unknown values are safer and faster to correct than plausible-looking false facts. |
| BE-D012 | 2026-08-20 | Accepted | Route Codex work through Luna for bounded mechanical tasks, Terra as the normal default, and Sol only for explicitly high-risk or high-ambiguity decisions, especially Unreal physics/API work. | Reduces routine model cost without allowing lower-cost work to invent Unreal facts or approve physics-sensitive changes. The routing table and escalation rules live in `AGENTS.md`. |
| BE-D013 | 2026-08-25 | Accepted | Accept BE-0004 with re-hit cancellation manually unexercised by the current queued left-click calibration path. | The source cancellation path compiled and passed independent lifecycle review, but a second input queued after the 0.40 s response. This is a test limitation only, not a final-combat-input decision. |

## Decision entry template

| ID | Date | Status | Decision | Reason / consequence |
| --- | --- | --- | --- | --- |
| BE-D### | YYYY-MM-DD | Proposed / Accepted / Superseded / Rejected | One testable choice. | Why it was chosen, trade-off, and replacement ID if superseded. |
