# BE-#### — task title

Status: `DRAFT` / `READY` / `IN PROGRESS` / `BLOCKED` / `HUMAN VERIFICATION` / `ACCEPTED`  
Owner: `Claude` / `Codex` / `Human`  
Reviewer: `Claude` / `Codex` / `Human`  
Created: `YYYY-MM-DD`

## Goal

One observable outcome. Describe what will be true for the player/developer, not merely which class will exist.

## Why now

What risk, dependency, or user-visible need makes this the next task?

## Known-good baseline

- Current evidence label and date.
- Exact behaviour before the change.
- Relevant assets/classes and verified paths.
- Link or path to evidence.

## Allowed changes

- Exact files/classes/assets/settings that may change.
- State whether C++, Blueprint, config, plugins/modules, or binary assets are in scope.

## Non-goals

- Name adjacent work that must not be pulled into this task.
- State temporary calibration behaviour that should remain unchanged.

## Inputs and references

- Applicable decisions: `BE-D###`.
- Relevant design section.
- Local Unreal header/plugin/example locations for version-sensitive APIs.
- Asset references copied from the Content Browser.

## Assumptions and unknowns

| Item | Assumption or unknown | How this task will resolve/contain it |
| --- | --- | --- |
| 1 |  |  |

## Acceptance criteria

Use observable, binary checks.

- [ ] Behavioural outcome occurs under a named setup.
- [ ] Failure/edge case is handled and observable.
- [ ] No listed baseline regression occurs.
- [ ] Instrumentation exposes the values required to tune or diagnose it.
- [ ] Documentation/asset map is updated where evidence changed.

## Verification plan

### Automated/build

- Command: `UNKNOWN UNTIL VERIFIED`
- Target/configuration: `UNKNOWN UNTIL VERIFIED`
- Expected result:

### Unreal Editor

1. Exact map/Blueprint/asset to open.
2. Exact values or connections to inspect.
3. Expected visual/log result.

### Playtest

1. Starting setup and controls.
2. Repetitions, frame-rate cases, and target positions.
3. Pass/fail observation.

### Regression checks

- Existing scenario(s) that must still work.

## Evidence required

- Build log path or excerpt.
- Screenshot/video filename.
- Output Log markers and key measured values.
- Engine version, map, asset revision/commit, and test date.

## Implementation notes

Filled by the implementer. Include units, ownership/lifetime, threading, Blueprint migration, and any deliberate temporary behaviour.

## Review findings

Filled by the reviewer. List findings by severity with file/asset references. State `No findings` explicitly when appropriate.

## Handoff

- Evidence label reached:
- Files changed:
- Verification run and result:
- Warnings:
- Manual steps remaining:
- Risks/unknowns:
- Suggested next task:
