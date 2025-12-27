# StepMania Network: Handoff Notes

**Date:** 2025-12-27
**Build:** 5.3.1-VR-Alpha (StepMania Network MVP)
**Status:** Alpha / MVP Complete

## Executive Summary
This session successfully implemented the core "StepMania Network" MVP (v5.3.0) and the initial "VR Alpha" (v5.3.1) updates. The codebase now contains a fully simulated cryptocurrency economy, tournament ladder system, gym/fitness mode, and mock P2P networking infrastructure, alongside basic VR architecture hooks.

## Key Changes
1.  **Economy System (`src/Economy/`):**
    *   `EconomyManager`: Central singleton for ledger, mining, and betting.
    *   `ScreenBettingEntry`: UI for wagering on matches.
    *   `ScreenGovernance`: DAO voting interface.
    *   **Data:** Persists to `Save/Economy.ini`.

2.  **Tournament System (`src/Tournament/`):**
    *   `ScreenTournamentLadder`: Elo ranking display and matchmaking interface.
    *   `TournamentBracket`: Visual bracket generation.

3.  **Gym Mode (`src/Gym/`):**
    *   `ScreenGymWelcome`: Dedicated fitness launcher.
    *   `GymPlaylistGenerator`: Creates intensity-based course files.
    *   `ActorCalorieGraph`: Real-time calorie burn visualization (integrated into `ScreenGameplay`).

4.  **VR Support (`src/arch/ArchHooks/`):**
    *   `ArchHooks_VR`: Stubs for HMD initialization (OpenVR/OpenXR placeholder).
    *   `m_bVRMode`: Preference in `PrefsManager`.

5.  **Build System:**
    *   Updated `src/CMakeLists.txt`, `src/CMakeData-economy.cmake`, `src/CMakeData-arch.cmake`.
    *   **Note:** Dependencies (libmad, ffmpeg, gtk3) are currently missing in the build environment, preventing full binary compilation. Syntax checks (`g++ -fsyntax-only`) were used for verification.

## Current Tasks & Next Steps
1.  **Blockchain Bridge:** The `EconomyManager` is currently a simulation. The next step is to implement `BlockchainBridge` interfaces to connect to real networks (Ethereum/Solana).
2.  **Server Backend:** A Node.js/Go backend is needed to replace the P2P simulation.
3.  **Live Spectating:** Streaming support.

## Known Issues
*   **Compilation:** Full build fails due to missing system libraries (X11, ALSA, etc.).
*   **Include Paths:** Fixed relative include paths in `ScreenUnifiedDashboard` and others, but further "deep" includes might need verification if new modules are added.

## File Manifest (New/Modified)
*   `src/Economy/*`
*   `src/Gym/*`
*   `src/Tournament/*`
*   `src/Unified/*`
*   `src/arch/ArchHooks/ArchHooks_VR.*`
*   `docs/ROADMAP.md`
*   `docs/DASHBOARD.md`
*   `CHANGELOG.md`
