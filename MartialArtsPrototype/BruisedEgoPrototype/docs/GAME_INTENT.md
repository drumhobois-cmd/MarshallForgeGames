# Bruised Ego — game intent

Status: **approved product direction**  
Last consolidated: **2026-08-19**

This is the short, operational companion to the long-form *Bruised Ego GDD and Technical Specification v0.2*. Agents must use it to prevent a technically convenient prototype from quietly becoming the wrong game.

## North star

Bruised Ego is a demanding martial-arts game in which players direct a physically limited human body rather than trigger a library of guaranteed attacks. The player should win by reading distance, creating angles, managing commitment and recovery, preserving balance, and exploiting real contact—not by memorising canned button strings.

The creator accepts a multi-year development schedule and sensible performance or content compromises. The causal physics identity of combat is the line that must not be compromised.

## Combat contract

### Directional and deliberate input

- Final input is built around high-bandwidth directional intent: fight pad/arcade-stick mappings and/or a dual-stick or mouse-drag method in the spirit of directional melee games.
- Direction, timing, body side, stance, current momentum, and commitment shape the attempted strike.
- A top-right intent should produce an attack whose path genuinely begins from that intent and current pose; it must not merely choose an unrelated stock animation by label.
- The current click-to-play jab proves the asset, montage, notify, and contact pipeline only. It is not evidence that the final control model is complete.

### Momentum over memorisation

- Combos are not guaranteed pre-authored strings.
- A follow-up works when balance, joint configuration, momentum, available limb, spacing, and stamina permit it.
- Styles may provide learned movement solutions and preferred transitions, but physics and state decide whether the transition is available and effective.
- Button mashing should create measurable disadvantages: overcommitment, slow recovery, lost guard, deteriorating balance, poor accuracy, and exhaustion.

### Animation and physics responsibilities

- Animation/mocap defines authored intent, style identity, readable technique, and desired pose trajectories.
- Collision and measured physical state determine whether, where, when, and how contact occurs.
- Physical response determines local body deflection, transferred momentum, balance loss, stagger, fall, and recovery opportunity.
- Damage and injury use those measured results; they do not manufacture the physical reaction after the fact.
- Canned hit animations may be used for temporary calibration, accessibility, distant LODs, or carefully documented fallbacks, but they are not the core causality model.

## Physical human limits

- Fighter height, reach, body-segment mass, total mass, stance width, centre of mass, leverage, and conditioning have trade-offs.
- Heavier fighters generally transfer and resist more momentum but pay costs in acceleration, endurance, and recovery.
- A valid simulation may be stylised and tuned for playability. “Physically caused” matters more than pretending every value is laboratory-perfect.
- Hardware compromises may reduce active bodies, constraint detail, substep frequency, reaction-chain length, or simulation distance. They must retain consistent cause and effect at the gameplay layer.

## Stamina and gassing out

- Stamina is ruthless but understandable. Wild power, repeated failed attacks, excessive tension, movement, blocking, and recovery all have costs.
- Conditioning affects capacity and recovery; fatigue affects strike speed, power delivery, posture, guard retention, reaction, balance, and decision windows.
- Exhaustion must emerge progressively and visibly. It should not be only a bar that disables attacks at zero.

## Genetic Lottery

- Players customise identity/face, while starting physical traits are rolled rather than min-maxed.
- Starting traits include height, frame/bone structure, natural mass distribution, metabolism, and athletic tendencies.
- The design asks players to adapt their strategy and training to the body they received.
- Anti-reroll enforcement, fairness, accessibility, and online integrity require separate design and testing; difficulty must not become contempt for the player.

## Training without wasting the player's real time

- Progression is physical adaptation and skill practice, not a conventional stat-point tree.
- Players schedule off-world gym work: hours, exercises, intensity, recovery, nutrition assumptions, and target muscle groups or qualities.
- The character can progress while the player is not actively watching repetitive gym scenes.
- Player interaction focuses on planning, meaningful technique practice, tests, sparring, trade-offs, and consequences—not chores or idle-time theatre.
- Training must include diminishing returns, recovery limits, injury/overtraining risk, weight and mobility trade-offs, and clear forecasts.

## Martial arts and style mixing

The long-term content ambition includes Wing Chun, Taekwondo, Jeet Kune Do, Kenpo/Kempo, Sambo, Brazilian Jiu-Jitsu, MMA, Karate, Kendo, Aikido, Judo, Japanese Jujutsu/Jiu-Jitsu, Ninjutsu-inspired material, Krav Maga, and other researched disciplines.

- Styles are capability and movement packages, not character classes that override physics.
- Learning a style should unlock meaningful stances, guards, footwork, entries, strikes, grappling concepts, transitions, and tactical knowledge without grind for its own sake.
- Players may mix learned material, but incompatible stance, range, equipment, momentum, or body position must still matter.
- Mocap packs and licensed animation are acceptable production compromises. They require skeleton, root-motion, contact, timing, style-authenticity, and gameplay validation before use.
- “Bushido” is primarily an ethical/philosophical tradition rather than a discrete unarmed fighting style. If represented as selectable game content, its mechanical meaning must be deliberately defined rather than assumed.
- Historical, cultural, and naming claims require credible subject-matter research and respectful presentation.

## Scope layers

### Full-game ambition

Directional physical combat, multiple disciplines, style mixing, genetic starting bodies, long-term conditioning, injury/recovery, opponents, progression, and a supporting world/meta-game.

### First combat vertical slice

One attacker, one defender, one or a few strikes, trustworthy swept contact, measured impact data, local physical response, balance consequence, stamina/recovery cost, instrumentation, and repeatable tuning cases.

### Current calibration proof

The imported jab can play through the current Animation Blueprint/montage path and opens a notify-driven fist sweep window. This validates plumbing only. It does not validate final input, impact physics, active ragdoll, damage, balance, stamina, AI, multiplayer, or multiple styles.

## Non-negotiable versus flexible

| Non-negotiable outcome | Flexible implementation/content choice |
| --- | --- |
| Direction and body state influence attacks | Exact controller scheme and assistance options |
| Contact consequences remain physically caused | Physics Control, Physical Animation, custom constraints, or a measured hybrid |
| Mashing produces systemic disadvantages | Exact stamina equations and UI presentation |
| Body traits create meaningful trade-offs | Exact number of simulated segments and LOD strategy |
| Training respects player time | Exact offline schedule UI and progression cadence |
| Styles feel mechanically distinct and can mix | Mocap, hand-keyed, procedural, or hybrid animation sources |

Any proposal that changes a non-negotiable needs explicit user approval and a new entry in `DECISIONS.md`.
