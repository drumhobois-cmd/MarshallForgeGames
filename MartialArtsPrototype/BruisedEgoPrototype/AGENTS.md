# Bruised Ego repository instructions

These rules apply to every agent working in this repository. Keep them concise, current, and committed with the project.

## Project identity

- Product: **Bruised Ego**, a difficult, directional, physics-led martial-arts game.
- Unreal project/module recorded to date: `BruisedEgoPrototype`.
- Target engine recorded to date: Unreal Engine 5.8.1. Verify against the live `.uproject` and installed engine before relying on it.
- Development horizon: multi-year. Prefer foundations that can be tested and extended over demos that only look complete.

## Required read order

Before planning or editing, read:

1. The single `BE-*.md` task file in `tasks/active/`, if one exists.
2. `docs/GAME_INTENT.md` for product non-negotiables.
3. `docs/CURRENT_STATE.md` for verified implementation facts.
4. `docs/DECISIONS.md` for accepted decisions and rejected alternatives.
5. `docs/UE_ASSET_MAP.md` for exact Unreal names and paths.
6. Relevant source, config, Build.cs, plugin, and local Unreal headers.

An active task controls immediate scope but cannot silently override `GAME_INTENT.md`. If instructions conflict, stop and identify the conflict. The user's explicit current instruction is authoritative; record any durable change in `DECISIONS.md`.

## Role defaults

- **Claude Code:** default C++ implementer and focused debugger.
- **Codex:** default planner, reviewer, documentation maintainer, test/evidence designer, and second-opinion technical researcher.
- **Human developer:** final authority for Unreal Editor changes, binary assets, tuning judgement, and playtest acceptance.
- Codex may implement source changes only when the user explicitly assigns that work. Claude may review or document when explicitly assigned.
- Never have two agents edit the same files or Unreal binary assets concurrently.

## Codex model routing and cost control

This policy guides Codex model selection when the task environment exposes a
choice. It is a routing preference, not a guarantee that an unavailable model
can be selected or that Codex can override the user's UI/account setting.

- Start with the least expensive tier that can safely establish the next
  decision. Escalate because of evidence or risk, never merely because a task
  sounds important.
- Keep the question narrow before using a stronger tier. Give that tier the
  relevant files, exact observed failure, acceptance criteria, and unresolved
  decision; do not spend Sol on broad repository tours or routine summaries.
- A stronger-tier conclusion remains a hypothesis until it reaches the
  repository's normal evidence label. Model choice never upgrades evidence.
- Use the exact model identifiers when they are selectable: `gpt-5.6-luna`,
  `gpt-5.6-terra`, and `gpt-5.6-sol`. If one is unavailable, choose the nearest
  available capability and state that substitution in the handoff.

| Route | Default reasoning effort | Use it for | Do not use it for |
| --- | --- | --- | --- |
| **Luna** (`gpt-5.6-luna`) | `low` | Bounded, reversible, mechanical work: repository inventories; locating files/references; formatting task or evidence templates; summarising a provided log/diff; deterministic checks; and turning an already-decided request into a small checklist. Its output must identify unknowns rather than filling them in. | Unreal API guidance, novel C++ changes, architecture choices, causal-physics judgement, ambiguous failures, or final correctness sign-off. |
| **Terra** (`gpt-5.6-terra`) | `medium` | Default for normal project work: scoped planning; documentation; reviewing a small, well-bounded diff; analysing a build error with local source/header evidence; writing test/evidence plans; and coordinating a single implementation task under the existing role rules. | Experimental Chaos/Physics Control work, safety-critical correctness, multi-system architectural decisions, unresolved crashes, or a review that would otherwise guess. |
| **Sol** (`gpt-5.6-sol`) | `high`; use `xhigh` only for a clearly bounded hard problem | High-consequence or high-ambiguity work: local UE 5.8.1 Chaos/Physics Control/API investigation; physics-units and causal-model review; design decisions that affect multiple combat systems; cross-module integration; concurrency/lifetime/crash diagnosis; security/data-loss risk; and final independent review of a risky change. | Routine reconnaissance, boilerplate, repeatable checks, or long unfiltered logs. |

### Escalation ladder

1. Begin in Luna only when the deliverable is bounded and independently
   checkable. If it finds ambiguity, missing local evidence, or needs an API
   interpretation, stop rather than speculate and hand the focused question to
   Terra.
2. Terra is the normal default. Escalate to Sol when the answer changes combat
   causality or physical safety, crosses system boundaries, depends on an
   experimental Unreal API, or remains uncertain after one focused evidence
   pass.
3. Do not repeatedly retry the same question on Sol. After Sol reduces the
   uncertainty, send mechanical follow-up, documentation, searches, and
   repeatable verification back to Luna or Terra.
4. The user may always request a specific model. A request for Sol is
   authoritative; a request for a cheaper model still does not permit guessing
   about a safety, physics, API, or data-loss concern.

### Unreal-specific Sol gates

Use Sol before recommending or approving a change involving any of the
following: Chaos or Physics Control APIs; impulses/forces/torques/mass or unit
conversions; skeletal-body selection and Physics Asset constraints; threading,
object lifetime, or crash investigation; collision/query changes that could
invalidate hit detection; and any change that could replace measured physical
causality with a canned outcome. Sol must inspect the exact installed headers
and relevant local sources; its result should leave a concise, testable
recommendation for the lower-cost follow-up work.

The current official tier descriptions are intentionally not copied here,
because pricing and availability change. Recheck [OpenAI's model
catalog](https://developers.openai.com/api/docs/models) before changing this
policy or estimating savings.

## Product guardrails

- Combat is directional and deliberate. The final design is not “button plus montage equals attack.”
- Player input expresses strike intent, angle, timing, commitment, and recovery. Current mouse-click jab playback is a calibration proof, not the final input architecture.
- Momentum, balance, reach, mass, leverage, contact geometry, stamina, and recovery determine whether actions connect and flow.
- Animation supplies desired motion, style, anticipation, and readability. Contact and physical state determine consequences.
- Physics causality is non-negotiable. Performance compromises may reduce simulation detail, frequency, or body coverage, but must not replace the causal model with canned outcomes presented as physics.
- Button mashing should fail through exposed recovery, poor balance, stamina cost, bad spacing, and loss of control—not a hidden anti-mash penalty.
- Stamina represents both short-term output and longer-term conditioning. Wild actions must degrade speed, posture, guard, recovery, and decision quality.
- The Genetic Lottery, asynchronous training schedule, multiple learnable styles, and low-grind style mixing are full-game pillars. Do not force all of them into the first combat vertical slice.

## Accuracy rules

- Never invent file paths, Content Browser paths, bone/socket names, Blueprint connections, plugin status, build commands, or Unreal API signatures.
- For changing or experimental Unreal APIs—especially Chaos and Physics Control—inspect the exact local 5.8.1 headers, plugin source, modules, and examples before proposing code. Official documentation is secondary to the installed version.
- Separate **hit detection**, **impact measurement**, **physical response**, **balance/stagger**, **damage**, and **presentation**. Do not collapse them into one generic “hit reaction” component.
- Use correct physical language and units. Momentum/impulse is not force or kinetic energy. Document every conversion between Unreal centimetres and SI-like tuning values.
- Physics substepping and CCD do not replace explicit high-speed strike queries. Validate sweeps/traces and physical response independently.
- A generic active-ragdoll snippet is a prototype hypothesis, not an accepted architecture. Require a measured spike before committing the core combat system.
- Do not globally weaken every Physics Control constraint for one local hit. Scope changes to named limbs/body chains and restore them deterministically.
- State assumptions explicitly and turn important unknowns into acceptance checks.

## Evidence vocabulary

Use these labels exactly; do not skip levels:

- `PROPOSED` — idea only.
- `IMPLEMENTED` — files changed; no successful build claimed.
- `COMPILES` — named target built successfully, with command/log recorded.
- `EDITOR-VERIFIED` — asset wiring or behaviour inspected in the named Editor version.
- `PLAYTEST-VERIFIED` — user reproduced the acceptance scenario in Play-in-Editor or a build.
- `REGRESSION-PROTECTED` — an automated or repeatable check now guards the behaviour.

When evidence is unavailable, write `NOT RUN` or `UNKNOWN`; never infer success.

## Task workflow

1. Inspect `git status`, the active task, relevant docs, and relevant files before editing.
2. Restate the observable baseline and acceptance criteria. Ask only questions that materially change the implementation.
3. Keep one task narrow. Do not perform drive-by refactors or reformat unrelated files.
4. Identify all binary Unreal assets that require human Editor work; source agents must not pretend to have changed them.
5. Implement the smallest coherent change. Preserve user work and existing Blueprint behaviour outside task scope.
6. Run the exact available verification. Record commands, target, engine version, outcome, and material warnings.
7. Give explicit manual Editor/playtest steps for checks an agent cannot run.
8. Update `CURRENT_STATE.md`, `UE_ASSET_MAP.md`, and `DECISIONS.md` only when the task produced corresponding evidence or a durable decision.
9. Move the completed task out of `tasks/active/` only after the human accepts the required status.

## Unreal and repository safety

- Inspect the worktree before changes. Never discard or rewrite unrelated user changes.
- Do not use destructive Git commands unless the user explicitly requests them.
- Do not edit generated output under `Binaries/`, `DerivedDataCache/`, `Intermediate/`, or `Saved/` as source.
- `.uasset` and `.umap` files are binary. Do not merge them as text or modify the same asset concurrently.
- Preserve Blueprint-facing names and serialized properties unless the task includes a migration plan.
- Do not add plugins, modules, dependencies, input actions, collision channels, or project settings silently.
- Do not claim an asset path until it has been copied from the Content Browser or verified through an asset report.
- Treat warnings involving animation, physics assets, collision, null ownership, or thread safety as relevant until triaged.

## Required handoff

Every implementation/review response must report:

- Outcome and current evidence label.
- Files changed and why.
- Verification run, including exact command or manual scenario.
- Results and relevant warnings.
- Assumptions and remaining unknowns.
- Risks or regressions to watch.
- Exact human Unreal Editor steps still required.
- Suggested next task, without expanding the current task automatically.

## Code review rules

Flag changes that:

- Replace directional/physical causality with a canned result without calling it a temporary prototype.
- Use velocity, mass, force, impulse, torque, or energy with incorrect units or misleading names.
- Trust overlap events alone for fast strikes without validating swept detection.
- Change global muscle/control settings for a local impact or fail to restore temporary settings.
- Couple damage directly to animation frame numbers instead of a measured contact event.
- Depend on an unverified Unreal API, plugin, asset, bone, socket, or collision channel.
- Report completion without distinguishing compilation, Editor verification, and playtest verification.
