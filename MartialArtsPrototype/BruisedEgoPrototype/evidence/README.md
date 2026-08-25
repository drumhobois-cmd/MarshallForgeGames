# Evidence

Store task evidence under `evidence/BE-####/`. Evidence should make a claim reproducible, not merely make the folder look busy.

Recommended contents:

- `environment.md` — engine version, map, target, commit/worktree state, and test machine notes.
- `build_summary.md` — exact command/result and a pointer to the full local log; repository-wide `*.log` ignores mean raw logs are normally not committed.
- `asset_references.md` — copied asset references and relevant settings.
- `editor_inspection.md` — named Editor version, inspected UI settings, collision/physics facts, and screenshot links.
- `playtest.md` — setup, steps, repetitions, expected/actual result, evidence label, and tester/date.
- `approval.md` — the human review decision, its scope, date, and any conditions.
- Short, clearly named screenshots or clips.

Use names such as `jab_contact_side_60fps_01.mp4`, not `video_final2.mp4`. Do not commit huge raw captures without a repository storage policy; retain a small proof clip and note where the full capture lives.

## Playtest note template

```markdown
# BE-#### playtest

- Date:
- Tester:
- Engine/build/commit:
- Map and characters:
- Setup:
- Steps:
- Repetitions/frame-rate cases:
- Expected:
- Actual:
- Pass/fail:
- Logs/measurements:
- Evidence files:
- Remaining uncertainty:
```
