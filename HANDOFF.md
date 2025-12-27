# Handoff Document: StepMania 5.2 (5.1-new) Merge Project

## 1. Session History (Dec 27, 2025 - Part 2)
**Focus:** Etterna Parity (Scoring & Difficulty)

### Completed Features
1.  **MSD (Mine Sweeper Density) Calculation**
    *   **File:** `src/Scoring/MsdCalculator.cpp`
    *   **Implementation:** Replaced mock logic with a real strain-based algorithm.
    *   **Details:**
        *   Analyzes `NoteData` for patterns (Stream, Jumpstream, Handstream, Jack).
        *   Calculates difficulty based on Note Density (NPS) and pattern complexity over 1.0s windows.
        *   Generates tags (e.g., "Jumpstream", "Boss") based on analysis.
        *   Returns average peak strain (top 10%).

2.  **Wife Scoring System**
    *   **Files:** `src/ScoreKeeperNormal.cpp`, `src/PlayerStageStats.h`, `src/PlayerStageStats.cpp`, `src/HighScore.h`, `src/HighScore.cpp`
    *   **Implementation:** Millisecond-based scoring system (Wife3-like approximation).
    *   **Details:**
        *   Added `CalculateWifeScore` using a Gaussian curve for deviation.
        *   Updated `PlayerStageStats` to track `m_fWifeScore` and `m_fCurMaxWifeScore`.
        *   Exposed `GetWifeScore` and `GetCurMaxWifeScore` to Lua.
        *   Updated `HighScore` to serialize `WifeScore` to/from `stats.xml`.

### Files Modified
*   `src/Scoring/MsdCalculator.cpp`
*   `PLANNED_FEATURES.md`
*   `src/ScoreKeeperNormal.h`, `src/ScoreKeeperNormal.cpp`
*   `src/PlayerStageStats.h`, `src/PlayerStageStats.cpp`
*   `src/HighScore.h`, `src/HighScore.cpp`

## 2. StepMania 5.2 Merge Status (Cumulative)
We have successfully merged key features from **ITGMania**, **NotITG**, and **Etterna** into the **StepMania 5.1-new** branch.

### Feature Summary
*   **[COMPLETED] Economy & Gym:** Wallet, Market, Mining, Gym Mode, Playlist Gen.
*   **[COMPLETED] ITGMania:** LoadNewSongs, FastProfileSwitch, Rate Mods, Mine Fix.
*   **[COMPLETED] NotITG:** Shaders, Uniforms, NotePath (Lua), Modfiles.
*   **[COMPLETED] Etterna:** MSD Calculation, Wife Scoring.
*   **[PENDING] Outfox:** Modern Audio Backend, New Game Modes.

## 3. Previous Session History (Dec 27, 2025 - Part 1)
*   **Completed Economy & Gym Features:**
    *   **Gym Mode:** Fully implemented `GymPlaylistGenerator` (smart course generation based on intensity/meter), `GymCourseWriter` (CRS file generation), and `ScreenGymWelcome` (UI flow).
    *   **Economy Loop:** Hooked `EconomyManager::Update` into `GameLoop.cpp` to enable background mining rewards and shareholder dividends.
    *   **Governance:** Refactored `ScreenGovernance` to use persistent `EconomyManager` state instead of local dummy data.
    *   **Betting:** Implemented `ScreenBettingEntry` (Economy) and `ScreenEvaluationBetting` (Tournament) for wagering on gameplay performance.
    *   **Server Node:** Implemented `ScreenServerNode` for simulated distributed computing tasks.
    *   **Cleanup:** Removed duplicate/dead code in `src/Tournament/ScreenBettingEntry.cpp`.
    *   **Build System:** Updated `src/CMakeData-economy.cmake` to include all new files.
*   **NotITG Parity:**
    *   **NotePath:** Implemented `PlayerState::SetNotePath` (Lua) to allow arbitrary Lua functions to control note positions (X, Y, Z).
    *   **Engine Integration:** Modified `ArrowEffects` to use the Lua function if set, bypassing standard modifiers.
    *   **Culling:** Updated `NoteDisplay` and `NoteField` to disable visibility culling when NotePath is active.
    *   **Modfiles:** Implemented automatic loading of `mods.lua` from song directories in `ScreenGameplay`.
*   **Version Bump:** Updated version to 5.1.2.

## 4. Next Steps
*   **Outfox Parity:**
    *   **Audio Backend:** Investigate replacing `RageSound` with a modern library (miniaudio/FMOD) to support newer formats and lower latency.
    *   **New Game Modes:** Implement support for Pump, KB7, or other modes.
*   **SMOnline Parity:**
    *   Enhance networking client if needed.
