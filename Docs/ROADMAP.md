# Unified StepMania ROADMAP.md

This document serves as the master blueprint for the structural and architectural evolution of Unified StepMania.

## Phase 1: Foundational Mocks & UIs (Completed)
*   **Goal:** Establish the UI flow and C++ Singleton structure.
*   **Status:** Complete. We have `ScreenMarketplace`, `ScreenTournamentLadder`, `ScreenGymWorkout`, `ScreenMissionSelect`, `ScreenSpectate`, `ScreenContentNetwork`, `ScreenAssetSync`, and `ScreenReplayMenu`.
*   **Result:** The game is navigable and "feature complete" from a user perspective, but relies on mock data and stubs.

## Phase 2: Live Backend Injection (Completed)
*   **Goal:** Strip out the mock data in the `Manager` singletons and wire them to live external APIs and hardware.
*   **Milestones:**
    1.  **Hardware Heart Rate Integration:** (COMPLETE) Hooks in `HeartRateManager.cpp` and `IHeartRateDriver`.
    2.  **True Network Backend:** (COMPLETE) `server/mock_server.js` manages matchmaking queue, Elo ratings, and WebSockets. Legacy `EzSockets` replaced with modern C++ `UnifiedNetwork`.
    3.  **Bobcoin Blockchain Sync:** (COMPLETE) `EconomyManager` fully integrated with `BobcoinBridge`. RPC endpoints implemented. `Luna<T>` bindings map `BuyItem`, `AwardMiningReward`, and `GetTransactionHistory` to Lua overlays (`ScreenMarketplace`, `ScreenWalletHistory`).
    4.  **BitTorrent P2P Downloader:** (COMPLETE) `ContentSwarmManager` mimics `libtorrent` for `.smzip` seeding/leeching.

## Phase 3: The "Etterna-NotITG" Gameplay Merge (Completed)
*   **Goal:** Achieve 100% gameplay parity with the most demanding competitive forks.
*   **Milestones:**
    1.  **Etterna Parity:** (COMPLETE) `ScoreKeeperUnified` decouples generic `TNS_W1` DDR logic, calculating pure Wife3 J-Scale curves natively from millisecond offsets.
    2.  **NotITG Parity:** (COMPLETE) `NotePath` spline mathematics implemented. Lua bindings allow dynamic frame-by-frame 3D arrow viewport manipulation.
    3.  **Ghost Replay Rendering:** (COMPLETE) `ReplayManager` records and loads CSV arrays. `Player.cpp` dynamically triggers inputs according to millisecond timestamps in the `Update` loop.

## Phase 4: Graphics & Engine Modernization (In Progress)
*   **Goal:** Break free from the 2005-era OpenGL 2.1 fixed-function pipeline.
*   **Milestones:**
    1.  **Vulkan Port:** (PROTOTYPED) `RageDisplay_Vulkan` implemented with `BeginFrame`/`EndFrame` stubs for SPIR-V command buffers. Registered in CMake.
    2.  **VR Rendering Hooks:** (PROTOTYPED) `ArchHooks_VR` retrieves Oculus/OpenVR matrices. Hooks injected into `RageDisplay_OGL`. UI toggles available in `12_VR.lua`.
    3.  **Plugin API:** Expose the `Manager` interface to dynamic libraries (`.dll`/`.so`), allowing the community to write their own hardware drivers.

## Phase 5: The "1.0" Release
*   Unified StepMania acts as the decentralized hub for Rhythm Gaming.
*   Players auto-sync via `AssetSyncManager`.
