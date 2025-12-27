# Planned Features for StepMania 5.2 ("Merged" Fork)

This document outlines the features to be merged from various forks (ITGMania, NotITG) into the 5.1-new branch to unify development.

## 0. Recently Merged Features (v5.1.0)
*   **Economy System:** In-game economy, currency, and transactions.
*   **Tournament Mode:** QR login, tournament brackets, and scoring.
*   **Unified Dashboard:** Centralized hub for player stats and economy.
*   **Gym Mode:** Training and practice features.

## 1. ITGMania Parity
ITGMania is a fork of StepMania 5.1, so these features are high priority and easier to merge.

### Features
*   **[COMPLETED] Load New Songs from Song Select:**
    *   Allows reloading the song database without restarting the game.
    *   Command: `SSMReloadSongs`.
    *   Useful for tournaments and Stamina RPG unlocks.
*   **[COMPLETED] Fast Profile Switching:**
    *   Allows switching profiles directly from the Music Wheel or Song Select screen.
    *   Command: `SSMProfileSwitch`.
*   **[COMPLETED] Rate Mod Improvements:**
    *   Support for both Pitch Dependent (classic) and Pitch Independent (modern) rate mods.
    *   Implemented via `PitchDependentRate` preference.
*   **Built-in Networking (GrooveStats):**
    *   Native support for GrooveStats without an external launcher.
    *   Submit scores, view leaderboards in-game.
*   **Player-Specific Options:**
    *   Visual Delay (Audio/Video sync per player).
    *   Disable Timing Windows (Practice mode feature).

### Bug Fixes
*   **[COMPLETED] Mine Fix:** Logic fix for mines (Credit: DinsFire64).
    *   Fixed logic error preventing mines from triggering on press when `REQUIRE_STEP_ON_MINES` is false.
*   **Held Misses:** Tracking held misses in engine for pad debugging.
*   **Overlapping Hold Bug:** Fix for holds that overlap.

## 2. NotITG Parity
NotITG is based on StepMania 3.95. Parity here involves reimplementing features in the SM5 engine.

### Core Features
*   **[PARTIAL] Modding Lua API:**
    *   **[COMPLETED]** Expose more Actor properties to Lua (`SetShader`, `SetUniform`).
    *   Allow arbitrary pathing for notes (NotePath).
    *   Support for "Modfiles" (charts that heavily script the engine).
*   **[COMPLETED] Shaders:**
    *   GLSL shader support controllable via Lua.
    *   Support for Vertex and Fragment shaders.
*   **Window Manipulation:**
    *   Allow the game window to be moved/resized via Lua (classic NotITG effect).
*   **Gameplay Modes:**
    *   `BothAtOnce` mode (Mirror inputs to both sides).

## 3. Etterna Parity
Etterna is a keyboard-focused fork known for its advanced scoring and difficulty calculation.

### Features
*   **[COMPLETED] MSD (Mine Sweeper Density) Calculation:**
    *   Implemented strain-based difficulty calculation.
    *   Pattern detection (Stream, Jumpstream, Handstream, Jack).
*   **[COMPLETED] Wife Scoring:**
    *   Millisecond-based scoring system (Wife3-like).
    *   Implemented in `ScoreKeeperNormal` and `PlayerStageStats`.
*   **Replay System:**
    *   Save and load full replay data.

## 4. Outfox Parity
Project Outfox focuses on modernizing the codebase and supporting many game modes.

### Features
*   **Modern Audio Backend:**
    *   Update audio drivers.
*   **New Game Modes:**
    *   Support for additional rhythm game types.

## 5. Implementation Plan
1.  **[DONE] ITGMania - Song Management:** Implement `LoadNewSongs`.
2.  **[DONE] ITGMania - Profile Management:** Implement `FastProfileSwitch`.
3.  **[DONE] ITGMania - Gameplay Fixes:** Implement Rate Mods and Mine Fix.
4.  **[DONE] NotITG - Modding Foundation:** Implemented Shaders and Uniforms.
5.  **[DONE] Economy & Tournament:** Merged feature branches.
6.  **[DONE] Etterna - MSD:** Implemented difficulty calculation.
7.  **[DONE] Etterna - Wife Scoring:** Implemented millisecond scoring.
8.  **[NEXT] Outfox - Audio:** Investigate audio backend updates.
