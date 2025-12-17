# Planned Features for StepMania 5.2 ("Merged" Fork)

This document outlines the features to be merged from various forks (ITGMania, NotITG) into the 5.1-new branch to unify development.

## 1. ITGMania Parity
ITGMania is a fork of StepMania 5.1, so these features are high priority and easier to merge.

### Features
*   **Load New Songs from Song Select:**
    *   Allows reloading the song database without restarting the game.
    *   Command: `SSMReloadSongs`.
    *   Useful for tournaments and Stamina RPG unlocks.
*   **Fast Profile Switching:**
    *   Allows switching profiles directly from the Music Wheel or Song Select screen.
    *   Command: `SSMProfileSwitch`.
*   **Built-in Networking (GrooveStats):**
    *   Native support for GrooveStats without an external launcher.
    *   Submit scores, view leaderboards in-game.
*   **Rate Mod Improvements:**
    *   Support for both Pitch Dependent (classic) and Pitch Independent (modern) rate mods.
*   **Player-Specific Options:**
    *   Visual Delay (Audio/Video sync per player).
    *   Disable Timing Windows (Practice mode feature).

### Bug Fixes
*   **Mine Fix:** Logic fix for mines (Credit: DinsFire64).
*   **Held Misses:** Tracking held misses in engine for pad debugging.
*   **Overlapping Hold Bug:** Fix for holds that overlap.

## 2. NotITG Parity
NotITG is based on StepMania 3.95. Parity here involves reimplementing features in the SM5 engine.

### Core Features
*   **Modding Lua API:**
    *   Expose more Actor properties to Lua.
    *   Allow arbitrary pathing for notes (NotePath).
    *   Support for "Modfiles" (charts that heavily script the engine).
*   **Shaders:**
    *   GLSL shader support controllable via Lua.
*   **Window Manipulation:**
    *   Allow the game window to be moved/resized via Lua (classic NotITG effect).
*   **Gameplay Modes:**
    *   `BothAtOnce` mode (Mirror inputs to both sides).

## 3. Implementation Plan
1.  **ITGMania - Song Management:** Implement `LoadNewSongs`.
2.  **ITGMania - Profile Management:** Implement `FastProfileSwitch`.
3.  **ITGMania - Gameplay Fixes:** Implement Mine Fix and Rate Mods.
4.  **NotITG - Modding Foundation:** Begin exposing necessary Lua bindings.
