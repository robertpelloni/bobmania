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

StepMania Unified - Handoff Notes


### Summary of Recent CI Fixes & Structural Refinements
*   **Submodule Cleanup:** `bobcoin` and `Themes/Simply-Love-SM5` nested `.gitmodules` entries inside `itgmania/` have been removed to prevent recursive checkout failures in GitHub Actions, since these were already correctly defined in the root repository.
*   **CMake Test Code Restoration:** Missing test code (`CMake/TestCode/test_external.c` and `test_broken.c`) needed by `StepmaniaCore.cmake` compilation checks were correctly relocated to the root build environment to ensure pipeline compilation succeeds.
*   **Ghost Replay Fix:** Fixed `Player.cpp` where incorrect assumptions about `GameInput` enum values were resulting in non-mapped injection. Implemented the proper `GAMESTATE->GetCurrentStyle()->GameInputToColumn(vGhostInputs[i])` map for ghost note processing.
*   **Doc Syncing:** Mirrored core workspace docs to the root level.
[PROJECT_MEMORY]
# Unified StepMania Architecture, Patterns & Decisions

## 1. Omni-Workspace Paradigm & Git Constraints
*   **The Codebase:** This repository (`bobmania`) is an **Omni-Workspace**. It serves as a unified command center managing various autonomous game engine forks (`itgmania`, NotITG parity), network layers, fitness/crypto integrations, and Web3 `bobcoin` components.
*   **Continuous Autonomous Execution:** A strict directive mandates continuous, autonomous execution. Workflows and instructions for LLMs are strictly defined in `Docs/UNIVERSAL_LLM_INSTRUCTIONS.md`.
*   **Submodule Nuances:** Submodules like `bobcoin` and `Themes/Simply-Love-SM5` must be strictly managed recursively. Having nested `.gitmodules` entries inside child directories causes GitHub CI/CD checkout failures.

## 2. The Singleton Manager Pattern
*   To avoid overwhelming the legacy `StepMania.cpp` and `ScreenGameplay.cpp` architectures, all new features are built as modular **Global C++ Singletons**.
*   **Examples:** `ECONOMYMAN` (Ledger), `GYMMAN` (Fitness), `TOURNAMENTMAN` (Brackets), `REPLAYMAN` (Ghost data), `MISSIONMAN` (Achievements), `HEARTRATEMAN` (Hardware polling).
*   **Lifecycle:** These singletons are declared in `src/global.h`, instantiated/cleaned up in `StepMania.cpp` (`sm_main`), and periodically ticked in `GameLoop.cpp`.
*   **Lua Exposure:** They are aggressively wired into the UI via `Luna<T>` bindings, exposing C++ data to the Lua sandbox (e.g., `Themes/default/Scripts/05 Economy.lua`).

## 3. Lua Sandbox & Frontend Constraints
*   **Strict Lua 5.1 Enforcement:** The engine rigidly maintains a Lua 5.1 sandbox to ensure backward compatibility with 15+ years of community themes and NoteSkins.
*   **UI Representation:** All backend feature modules must be fully represented in the frontend using interactive elements. The C++ backend handles purely logic.

## 4. Ecosystem & Build Constraints
*   **Version Control:** The single source of truth for the project version is the `VERSION` file in the root directory. Updates must trigger a commit.
*   **Dependencies:** External libraries (FFmpeg, libtorrent, Zlib, JsonCpp, Lua) are vendored in `extern/`.
*   **Missing Subsystems (Stub Strategy):** Due to a lack of immediate APIs (e.g., real Bluetooth Heart Rate library, P2P tracker), we use the "Stub" Strategy. The Manager acts identically whether it is real or a mock (e.g., `HeartRateDriver_Mock` generating a sine wave).
*   **File I/O:** Never use standard `std::fstream`. Always use StepMania's `RageFile` wrapper and the virtual file system (`FILEMAN`) to ensure cross-platform compatibility.

## 5. Gameplay & Networking Merges
*   **Parity Integrations:** Includes `Wife3` (J4) scoring logic natively for Etterna compatibility, NotITG-style visual hooks (`BothAtOnce`, `NotePath` spline mathematics), and Replay Ghost tracking.
*   **Economy/Networking:** `EconomyManager` simulates a blockchain ledger and interfaces with the privacy-focused `bobcoin` token via the `BobcoinBridge`. `GameClient` (C++) manages WebSocket connections.

## EXECUTIVE PROTOCOL: REPOSITORY SYNCHRONIZATION RUN (v5.71.0-Unified-Beta)
*   **Merge Operations:** Executed an intelligent dual-direction merge. Reconciled feature branch `jules-10182904622127905088-faec53aa` directly into `master`.
*   **Submodule Status:** Recursively synced `bobcoin` (to `546d3cb2`) and `Simply-Love-SM5` (to `06a56f5f`) to their latest upstream tracking commits per Executive Protocol #56. Note: `bg`, `freellm`, `jules-autopilot`, `tormentnexus`, `auto_dj_script`, `MilkDrop3`, and `MilkDrop3_fix` do not exist in this monorepo context.
*   **Version Update:** Updated global state text and deployment configurations to `5.71.0-Unified-Beta`.
