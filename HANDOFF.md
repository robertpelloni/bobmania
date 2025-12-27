# Handoff Document: StepMania 5.2 (5.1-new) Merge Project

## 1. Session History (Dec 27, 2025 - Part 3)
**Focus:** Outfox Parity (Audio Backend)

### Completed Features
1.  **Modern Audio Backend (miniaudio)**
    *   **Files:** `src/arch/Sound/RageSoundDriver_MiniAudio.h`, `src/arch/Sound/RageSoundDriver_MiniAudio.cpp`, `src/arch/Sound/miniaudio.h`
    *   **Implementation:** Implemented a new audio driver using the `miniaudio` library.
    *   **Details:**
        *   Downloaded `miniaudio.h` (single-header library).
        *   Implemented `RageSoundDriver_MiniAudio` class inheriting from `RageSoundDriver`.
        *   Implemented `Init`, `GetPosition`, `GetPlayLatency`, `GetSampleRate`.
        *   Implemented `DataCallback` to feed audio data from `RageSoundDriver::Mix` to `miniaudio`.
        *   Updated `src/CMakeData-arch.cmake` to include the new driver.
        *   Updated `src/arch/arch_default.h` to add `MiniAudio` to the default driver list (priority over legacy drivers).
        *   Updated `src/CMakeLists.txt` to link required Windows libraries (`ole32`, `user32`, `advapi32`, `avrt`).

### Files Modified
*   `src/arch/Sound/RageSoundDriver_MiniAudio.h` (New)
*   `src/arch/Sound/RageSoundDriver_MiniAudio.cpp` (New)
*   `src/arch/Sound/miniaudio.h` (New)
*   `src/CMakeData-arch.cmake`
*   `src/arch/arch_default.h`
*   `src/CMakeLists.txt`
*   `PLANNED_FEATURES.md`

## 2. StepMania 5.2 Merge Status (Cumulative)
We have successfully merged key features from **ITGMania**, **NotITG**, **Etterna**, and **Outfox** into the **StepMania 5.1-new** branch.

### Feature Summary
*   **[COMPLETED] Economy & Gym:** Wallet, Market, Mining, Gym Mode, Playlist Gen.
*   **[COMPLETED] ITGMania:** LoadNewSongs, FastProfileSwitch, Rate Mods, Mine Fix.
*   **[COMPLETED] NotITG:** Shaders, Uniforms, NotePath (Lua), Modfiles.
*   **[COMPLETED] Etterna:** MSD Calculation, Wife Scoring.
*   **[COMPLETED] Outfox:** Modern Audio Backend.
*   **[PENDING] Outfox:** New Game Modes.

## 3. Previous Session History (Dec 27, 2025 - Part 2)
*   **Etterna Parity:**
    *   **MSD Calculation:** Implemented strain-based difficulty calculation in `src/Scoring/MsdCalculator.cpp`.
    *   **Wife Scoring:** Implemented millisecond-based scoring in `src/ScoreKeeperNormal.cpp` and `src/PlayerStageStats.cpp`.

## 4. Next Steps
*   **Outfox Parity:**
    *   **New Game Modes:** Implement support for Pump, KB7, or other modes.
*   **SMOnline Parity:**
    *   Enhance networking client if needed.
