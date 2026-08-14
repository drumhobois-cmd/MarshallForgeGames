# Bruised Ego Prototype

## Project

Working title: Bruised Ego

Engine: Unreal Engine 5.8
Language: C++ with Blueprint used for presentation/editor-facing work.

This is an early prototype for a grounded martial arts RPG.

The current goal is NOT to build the full game.

The current goal is simply:

> Make punching Bob feel good.

---

## Current Prototype Scope

The prototype currently consists of:

- One small test gym / grey-box room
- One player character
- One opponent / training target named Bob
- Basic third-person movement and camera
- Fighting stance
- Jab
- Cross
- Guard
- Physical fist contact detection
- Bob reacting when genuinely struck

An attack should miss if the fist does not physically reach the opponent.

Distance, timing and positioning must matter.

STOP once this basic combat loop is working well enough to evaluate.

---

## Explicitly Out of Scope

DO NOT implement any of the following unless explicitly requested:

- Open world
- City systems
- Procedural city generation
- Quests
- Dialogue systems
- Reputation systems
- Economy
- Inventory
- Equipment
- Shops
- Medical systems
- Permanent injury systems
- Detailed anatomical trauma
- Blood/gore systems
- Skill trees
- Character progression
- Training progression
- Multiple martial arts disciplines
- Grappling
- Ground fighting
- Takedown systems
- Multiplayer
- Networking
- Save-game architecture
- Large AI systems
- Full Body IK systems
- Motion Matching architecture
- MetaHuman integration
- Character customization

Those may come later.

Bob gets punched first.

---

## Combat Design Principles

Combat is grounded martial arts, not an arcade combo system.

The eventual game should prioritize:

- Physical distance
- Timing
- Footwork
- Balance
- Fatigue
- Body positioning
- Actual contact
- Technique

Do not implement arbitrary RPG-style attack damage simply because an attack animation played.

For the current prototype:

A punch should only register when the attacking fist physically reaches a valid target.

A punch thrown from too far away should miss.

Position and range must matter.

Avoid lock-on assisted teleportation or snapping the player into attack range.

Avoid canned combat systems that undermine physical positioning.

---

## Architecture Rules

Prefer C++ for:

- Combat simulation
- Gameplay state
- Hit/contact evaluation
- Reusable gameplay components
- Mathematical calculations

Prefer Blueprint/editor assets for:

- Animation assignment
- Animation Montages
- VFX
- Audio
- UI
- Level setup
- Designer tuning

Gameplay C++ should expose useful parameters to Blueprint using appropriate Unreal properties.

Do not place all combat logic inside the player character class.

Prefer small reusable components where appropriate.

Do not create large architectural frameworks before they are actually required.

---

## Working With This Project

Before making changes:

1. Inspect the existing implementation.
2. Understand the Unreal template structure.
3. State what files you intend to modify.
4. Keep the change as small as practical.

After making changes, report:

1. Files created.
2. Files modified.
3. What was implemented.
4. Important Unreal concepts involved.
5. Build result.
6. Compiler warnings/errors.
7. Any manual Unreal Editor steps required.

Never claim something was tested in the Unreal Editor unless it actually was.

If a task requires manual Editor configuration, explain the steps clearly.

---

## Build

Unreal Engine location:

H:\Epic Games\UE_5.8

Project:

E:\MarshallForgeGames\MartialArtsPrototype\BruisedEgoPrototype\BruisedEgoPrototype.uproject

When appropriate, compile the Unreal Editor target and report the actual result.

Do not modify Unreal Engine source files.

---

## Git

This project is tracked by Git.

Repository root:

E:\MarshallForgeGames

Before significant changes, inspect:

git status

Do not:

- delete the Git repository
- rewrite Git history
- force push
- reset --hard
- discard user changes

unless explicitly instructed.

Keep changes focused so they are easy to review and revert.

---

## Teaching Requirement

The user is learning Unreal Engine.

When implementing important systems, briefly explain:

- what the relevant C++ class/component does
- why it exists
- how it connects to Unreal/Blueprint
- what the user should look for inside the Editor

Do not create a project that can only be understood by reading generated code.

---

## Current Priority

1. Existing Third Person character works.
2. Player can enter/exit fighting stance.
3. Player can throw a jab.
4. Jab has physical fist contact detection.
5. Bob reacts only when actually contacted.
6. Add cross.
7. Add guard.
8. Evaluate feel before adding anything else.

Bob gets punched first.
