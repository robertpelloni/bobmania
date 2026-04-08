# Unified StepMania ROADMAP.md

This document serves as the master blueprint for the structural and architectural evolution of Unified StepMania.

## Phase 1: Foundational Mocks & UIs (Completed)
*   **Goal:** Establish the UI flow and C++ Singleton structure.
*   **Status:** Complete. We have `ScreenMarketplace`, `ScreenTournamentLadder`, `ScreenGymWorkout`, `ScreenMissionSelect`, `ScreenSpectate`, `ScreenContentNetwork`, `ScreenAssetSync`, and `ScreenReplayMenu`.
*   **Result:** The game is navigable and "feature complete" from a user perspective, but relies on mock data and stubs (e.g., sine waves for heart rates, dummy HTTP endpoints for content).

## Phase 2: Live Backend Injection (Current Priority)
*   **Goal:** Strip out the mock data in the `Manager` singletons and wire them to live external APIs and hardware.
*   **Milestones:**
    1.  **Hardware Heart Rate Integration:** Replace the logic in `HeartRateManager.cpp` with an `IHeartRateDriver`. Implement an OS-specific Bluetooth LE driver to pull live BPM.
    2.  **True Network Backend:** Update `server/mock_server.js` to handle PostgreSQL/Redis persistence. Update `UnifiedNetwork` to maintain persistent JSON/WebSocket connections.
    3.  **Bobcoin Blockchain Sync:** Replace `EconomyManager`'s local XML parsing with a true RPC connection to a Bobcoin node using the `extern/bobcoin` library.
    4.  **BitTorrent P2P Downloader:** Expand `ContentSwarmManager` using `libtorrent` (or similar) to actively download `.smzip` files from seeders, bypassing centralized CDNs.

## Phase 3: The "Etterna-NotITG" Gameplay Merge (In Progress)
*   **Goal:** Achieve 100% gameplay parity with the most demanding competitive forks.
*   **Milestones:**
    1.  **Etterna Parity:** `WifeScoring.cpp` exists but must entirely replace the legacy DDR-style scoring in `ScoreKeeperNormal`. Judge windows must be scaled to milliseconds.
    2.  **NotITG Parity:** `Player.cpp` must be rewritten to support arbitrary `Actor` hooks from Lua, allowing modders to execute Spline Math (`NotePath.cpp`) on every frame to bend and twist arrows dynamically.
    3.  **Ghost Replay Rendering:** `ReplayManager` records inputs successfully. We must rewrite `Player.cpp` to render a translucent second player field that consumes the `ReplayInput` vector, racing the user in real-time.

## Phase 4: Graphics & Engine Modernization (Future)
*   **Goal:** Break free from the 2005-era OpenGL 2.1 fixed-function pipeline.
*   **Milestones:**
    1.  **Vulkan / DirectX 12 Port:** Rip out `RageDisplay_OGL.cpp` and implement a modern rendering API. This is critical for drawing 4K/8K theme elements and complex NotITG shaders without bottlenecking the CPU.
    2.  **VR Rendering Hooks:** Expand the existing `m_bVRMode` preference to render stereoscopic side-by-side matrices and warp the viewport for Oculus/Meta Quest support.
    3.  **Plugin API:** Expose the `Manager` interface to dynamic libraries (`.dll`/`.so`), allowing the community to write their own hardware drivers (e.g., custom dance pad lighting controllers) without compiling the entire 2-million-line codebase.

## Phase 5: The "1.0" Release
*   Unified StepMania acts as the decentralized hub for Rhythm Gaming.
*   Players auto-sync via `AssetSyncManager`.
*   Tournaments run autonomously via `TournamentManager`.
*   Creators are paid out via `EconomyManager` (Bobcoin).
*   No known regressions from StepMania 5.1/5.2.
