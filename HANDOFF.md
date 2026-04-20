<<<<<<< Updated upstream
<<<<<<< HEAD
# StepMania Network: Final Project Handoff

**Date:** 2025-12-27
**Build:** 5.5.4-Tested
**Status:** Completed / Maintenance

## Executive Summary
This document summarizes the comprehensive overhaul of StepMania 5 into "StepMania Network". Over the course of this development session, the codebase was transformed from a legacy rhythm game engine into a modern, connected platform featuring a cryptocurrency economy, competitive matchmaking, and fitness tracking.

## Feature Implementation History

### 1. Economy & Marketplace (MVP)
*   **Ledger:** Implemented `EconomyManager` to simulate a blockchain wallet and transaction history.
*   **Marketplace:** Created `ScreenMarketplace` for users to buy "Assets" (songs, skins) using their wallet balance.
*   **Persistence:** Data is saved to `Save/Economy.ini`, ensuring progress is kept between sessions.
*   **Lua Bindings:** Exposed the economy to themes via `ECONOMYMAN` (GetBalance, Transfer).

### 2. Competition & Spectating
*   **Ladders:** Built `ScreenTournamentLadder` with Elo calculation logic and visual brackets.
*   **Spectating:** Developed `SpectatorManager` to handle live match broadcasting.
*   **Streaming:** Integrated `StreamManager` (using FFmpeg headers) to stub RTMP video output.

### 3. Fitness (Gym Mode)
*   **Workouts:** Created `ScreenGymWelcome` and `GymPlaylistGenerator` for custom intensity-based courses.
*   **Visualization:** Integrated `ActorCalorieGraph` into the core `ScreenGameplay` to show real-time exertion.

### 4. Network & Backend
*   **Client:** Implemented `GameClient` (C++) to handle WebSocket connections, chat state, and lobbies.
*   **Server:** Built a Node.js backend (`server/index.js`) to manage users, chat relay, and matchmaking queues.
*   **Sync:** Created `AssetSyncManager` to simulate fetching items from external games ("Bob's Game").

### 5. VR Support
*   **Architecture:** Added `ArchHooks_VR` with stubs for HMD initialization and render loops.
*   **Config:** Added `VRMode` preference to `PrefsManager`.

## Technical Reference

### Directory Structure
| Path | Component | Description |
| :--- | :--- | :--- |
| `src/Economy` | Economy | Ledger, Wallets, Marketplace, Governance. |
| `src/Tournament` | Competition | Ladders, Brackets. |
| `src/Gym` | Fitness | Gym Mode, Calorie Graph. |
| `src/Network` | NetCode | GameClient, StreamManager, SpectatorManager. |
| `src/Unified` | UI | Dashboard, AssetSync. |
| `server/` | Backend | Node.js WebSocket Server. |

### API Reference (Lua)
*   `ECONOMYMAN:GetBalance(addr)`
*   `ECONOMYMAN:Transfer(to, amount, reason)`
*   `SPECTATORMAN:ConnectToMatch(id)`
*   `ASSETSYNCMAN:SyncWithGame(name)`

### Testing & Deployment
*   **Unit Tests:** `src/tests/test_economy.cpp` (Run via `src/tests/run_tests.sh`).
*   **Deployment:** `deploy.sh` simulates the build and server restart process.

## Final Note
The project is fully integrated. All branches are merged to `main`. The version is synchronized to `5.5.4-Tested`.
# Handoff Document: StepMania 5.2 (5.1-new) Unified Fork Project
=======
# UNIFIED STEPMANIA: MASTER AI HANDOFF (v5.7.12)
>>>>>>> origin/unified-ui-features-13937230807013224518

**Author:** Jules (Google AI)
**Date:** 2026-02-10
**Target Models:** Gemini 3, Claude Opus 4.6, GPT Codex 5.3
**Project State:** 5.7.12-Unified-Beta-FinalHandoff

---

## 1. THE ULTIMATE VISION
This repository is the **Unified StepMania** platform. It aggressively merges the features of disparate forks (StepMania 5.2, OutFox, NotITG, Etterna) into a single, cohesive engine. It serves competitive players (Etterna-style milliseconds), modders (NotITG-style Actor hooks), fitness enthusiasts (Gym Mode), and introduces a Web3 virtual economy (Bobcoin) and P2P networking (Content Swarm).

## 2. THE ARCHITECTURE (CRITICAL RULES)

### 2.1 The "Singleton Manager" Paradigm
To prevent the 20-year-old `StepMania.cpp` and `ScreenGameplay.cpp` from collapsing under spaghetti code, all new features are built as **Global C++ Singletons**.
*   **Location:** `src/Economy/EconomyManager`, `src/Gym/GymManager`, `src/Tournament/TournamentManager`, `src/Unified/MissionManager`, `src/Scoring/ReplayManager`, etc.
*   **Initialization:** They are declared in `src/global.h`, instantiated in `sm_main` (`StepMania.cpp`), and updated in `GameLoop.cpp`.
*   **Lua Binding:** Every Manager exposes its methods via `Luna<T>` bindings so the UI can draw data asynchronously without blocking rendering.

### 2.2 The Lua 5.1 Sandbox Constraint
**DO NOT UPGRADE THE LUA ENGINE.** The project strictly relies on Lua 5.1 to preserve backward compatibility with 15+ years of community themes and NoteSkins. If you attempt to inject Lua 5.3 features (true integers, bitwise operators), you will crash the engine on boot. The entire UI is located in `Themes/default/BGAnimations/`.

### 2.3 The "Mock" Strategy (Your Immediate Task)
To allow parallel development, many C++ Managers currently use "Mocks" or "Stubs" to generate data for the Lua UI. **Your job as the next LLM is to replace these stubs with real API implementations.**

*   `HeartRateManager`: Currently outputs a sine wave. **Action:** Replace with OS-specific Bluetooth LE driver polling.
*   `EconomyManager`: Currently reads a local `Save/Economy.xml.sig`. **Action:** Replace with an HTTP JSON-RPC to a live `bobcoin` network node.
*   `ContentSwarmManager`: Currently uses StepMania's legacy `FileDownload.cpp` (HTTP GET). **Action:** Replace with `libtorrent` to allow true P2P `.smzip` discovery.
*   `ReplayManager`: Successfully records inputs to CSVs. **Action:** Write C++ logic in `Player.cpp` that renders a translucent "Ghost NoteField" reading those CSVs in real-time.

## 3. WHAT HAS BEEN ACCOMPLISHED

## 6. Excluded Features (Analysis)
The following features from forks were analyzed but explicitly **excluded** from this merge:
*   **Direct Memory Access (`memory` namespace):** Present in NotITG for advanced hacks. Excluded due to extreme security risks and non-portability (Windows-specific memory layout dependence).
*   **Unsafe Lua:** Lua 5.1 sandbox is maintained. Upgrading to Lua 5.3+ (Project OutFox) was rejected to maintain backwards compatibility with existing StepMania 5.x content.

## Additional Notes from unified-ui-features Branch
StepMania Unified - Handoff Notes
<<<<<<< Updated upstream

StepMania Unified - Handoff Notes

=======
>>>>>>> Stashed changes
Last Session: Implemented MissionManager, Documentation, and Network Helpers.
Next Steps: Implement ReplayManager, Wife Scoring, and advanced NetPlay features.
Current Version: 5.7.0-Unified-Alpha
Submodules: Simply-Love-SM5, bobcoin (mock)
=======
### UI & Frontend
Every single C++ feature is exhaustively represented in the Lua 5.1 Theme.
*   **`ScreenUnifiedDashboard`:** The master hub linking to all new modes.
*   **`ScreenMarketplace` & `ScreenWalletHistory`:** Buy themes/songs with Bobcoin; view the transaction ledger.
*   **`ScreenTournamentLadder` & `ScreenBettingEntry`:** View ELO ranks, wager Bobcoins, and trigger the `ScreenTournamentDraft` pick/ban phase against opponents.
*   **`ScreenGymWelcome` & `ScreenGymSummary`:** Create biometric profiles, generate dynamic endurance playlists (`GymPlaylistGenerator`), and track calories.
*   **`ScreenSpectate`:** View active matches (populated by the Node.js server) and tune in to simulated RTMP streams.
*   **`ScreenContentNetwork` & `ScreenAssetSync`:** Browse P2P packs, push/pull high scores and wallet balances from the cloud.
*   **`ScreenMissionSelect`:** Track objectives (Calories Burned, Notes Hit, Songs Passed) fed dynamically from the C++ `MissionManager`.
*   **`ScreenUnifiedOptions`:** Exposes backend `PlayerOptions` (Ghost Tapping, Practice Mode, NotITG FOV Overrides) directly to the user.

### Backend Infrastructure
*   **Discord RPC:** Context-aware Rich Presence injected into every single Lua screen (`Themes/default/Scripts/11 Discord.lua`).
*   **Node.js Server:** `server/mock_server.js` was completely rewritten into a JSON WebSocket handler. It maintains a Matchmaking Queue, tracks mocked ELO, issues `MATCH_START` payloads, and broadcasts an `ACTIVE_MATCHES` list for Spectators.
*   **Git Merges:** The repository cleanly merged upstream `x11_fullscreen` (Linux fixes) and `merge-itgmania-features` (parity groundwork) without regressing any Unified modules.

## 4. DOCUMENTATION DIRECTORY
You must consult the `Docs/` directory before writing code.
*   **`Docs/LLM_UNIVERSAL.md`:** The absolute master directive set for AI agents.
*   **`Docs/ROADMAP.md` & `Docs/TODO.md`:** The exact, prioritized list of remaining C++ hardware/network implementations.
*   **`Docs/SUBMODULES.md`:** Detailed explanation of the 15+ external libraries (`ffmpeg`, `bobcoin`, `jsoncpp`, etc.).
*   **`Docs/IDEAS.md`:** Far-future pivots (Rust rewrite, Rhythm MMO).

## 5. NEXT STEPS (ACTION REQUIRED)
1. Boot the `mock_server.js`.
2. Connect `TournamentManager` to the WebSocket.
3. Open `src/Scoring/ReplayManager.cpp` and `src/Player.cpp` and implement Ghost Rendering.
4. Replace `EconomyManager`'s local XML parsing with Blockchain RPCs.
>>>>>>> origin/unified-ui-features-13937230807013224518
