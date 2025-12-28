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
