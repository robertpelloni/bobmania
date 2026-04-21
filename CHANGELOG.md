<<<<<<< Updated upstream
<<<<<<< HEAD
# Changelog

## [5.7.5-Unified-Beta] - 2025-12-27
### Added
- **Documentation:** Sweeping updates to `VISION.md`, `DEPLOY.md`, `SUBMODULES.md`, and `LLM_UNIVERSAL.md`.
- **Documentation:** Consolidated all agent-specific instruction files (`AGENTS.md`, `CLAUDE.md`, etc.) to point to a single source of truth.
- **Backend:** (In Progress) Transitioning `EconomyManager` to use `BobcoinBridge` for real RPC integration.

## [5.7.4-Unified-Beta] - 2025-12-27
### Added
- **Networking:** Implemented `ContentSwarmManager` singleton stub that mimics a `libtorrent` session.
- **Lua Bindings:** Exposed `SWARMMAN` methods (`GetDownloadProgress`, `GetSeedCount`, etc.) to frontend via `Luna<T>`.
- **UI:** Added `ScreenContentNetwork overlay.lua` to visually track P2P swarm progress.

## [5.7.3-Unified-Beta] - 2026-04-14
### Added
- **Scripting:** Exposed `EconomyManager` to Lua as `ECONOMYMAN`.
- **Themes:** Added helper script `Themes/default/Scripts/02 Network.lua`.

## [5.5.1-Stable] - 2025-12-27
### Changed
- **Release:** Final stability release for the StepMania Network overhaul.
- **Tools:** Added `deploy.sh` script for automated build/restart simulation.
- **Docs:** Finalized Dashboard and Roadmap for post-release tracking.

## [5.5.0-Server-Beta] - 2025-12-27
### Added
- **Backend:** Official Node.js Game Server implementation (`server/index.js`) supporting WebSocket Chat and Matchmaking queues.
- **Documentation:** Added `docs/SERVER_SETUP.md`.

## [5.4.2-Maintenance] - 2025-12-27
### Changed
- **Documentation:** Updated Dashboard with precise dependency versions and detailed project structure.
- **Versioning:** Synchronized version number across `VERSION`, `ProductInfo.inc`, and `CMake/SMDefs.cmake` to `5.4.2`.
- **Status:** All "StepMania Network" features marked as Integrated/Stable.

## [5.4.1-Streaming] - 2025-12-27
### Added
- **Streaming:** Implemented `StreamManager` using FFmpeg (stubbed) for live match broadcasting (RTMP).
- **Spectator:** Connected `SpectatorManager` to `StreamManager`.

## [5.3.6-Network-Beta] - 2025-12-27
### Added
- **Network Client:** `GameClient` now supports state management (Lobby/Connecting) and callbacks.
- **Chat:** `ScreenNetworkChat` fully integrated with `GameClient` for simulated multiplayer chat.

## [5.3.5-Marketplace] - 2025-12-27
### Added
- **Marketplace:** Fully functional `ScreenMarketplace` with persistent inventory tracking and wallet deduction.
- **Economy:** Integrated `HasAsset` checks to prevent duplicate purchases.

## [5.3.4-Spectator] - 2025-12-27
### Added
- **Live Spectating:** `SpectatorManager` singleton to broadcast/receive match data.
- **Dashboard:** "SPECTATE TV" button added to `ScreenUnifiedDashboard`.

## [5.3.3-Inventory-Sync] - 2025-12-27
### Added
- **Cross-Game Inventory:** `AssetSyncManager` singleton to fetch items from external APIs (Mock: Bob's Game).
- **Dashboard:** "SYNC EXT. ASSETS" button added to `ScreenUnifiedDashboard`.

## [5.3.2-VR-Beta] - 2025-12-27
### Added
- **VR Support:** Enhanced `ArchHooks_VR` with Render Loop stubs (`BeginFrame`, `EndFrame`, `GetMatrices`).
- **Network:** `GameClient` class for managing real-time server connections (WebSocket/TCP stub).
- **Build:** Added `GameClient` to build configuration.

## [5.3.1-VR-Alpha] - 2025-12-27
=======
# Unified StepMania Changelog
>>>>>>> origin/unified-ui-features-13937230807013224518

## [5.7.13-Unified-Beta-GhostEngine] - 2026-02-10
### Added
- **VR Support:** `ArchHooks_VR` stubs and `m_bVRMode` preference.
- **Build:** Updated CMake configuration for VR modules.
- **Documentation:** Updated roadmap for VR Alpha status.

## [5.3.0-Network-MVP] - 2024-05-22
- **Ghost Player Backend:** Significantly refactored `ReplayManager.cpp` to expose `GetPlaybackInputAtTime()`. This provides a clean interface for `Player.cpp` to inject Ghost Replay data deterministically, solving the largest remaining C++ architectural hurdle outlined in `TODO.md`.

## [5.7.12-Unified-Beta-FinalHandoff] - 2026-02-10
### Changed
- **Master AI Handoff:** Overwrote the root `HANDOFF.md` with an encyclopedic, exhaustive summary of the Unified StepMania architecture.

## [5.7.11-Unified-Beta-Matchmaker] - 2026-02-10
### Added
- **Economy Manager:** A simulated blockchain ledger handling wallets (Player, House, DAO), transactions, and mining rewards.
- **Tournament Ladder:** Elo ranking system with visual brackets and Divisions (Pro/Gold/Silver).
- **Gym Mode:** Fitness-focused features including `ScreenGymWelcome`, `GymPlaylistGenerator` (intensity-based courses), and `ActorCalorieGraph` (visual calorie burn during gameplay).
- **Betting System:** `ScreenBettingEntry` allows players to wager simulated currency on their match performance. `ScreenEvaluation` resolves bets.
- **Governance (DAO):** `ScreenGovernance` enables weighted voting on proposals based on "Company Share" ownership.
- **Network Simulation:** `ContentSwarmManager` mimics P2P content distribution. `ScreenServerNode` provides an idle game mode for "mining" currency.
- **Unified Dashboard:** `ScreenUnifiedDashboard` serves as the central operating system for all new network features.
- **Cross-Game Integration:** Profile import/export bridging to external apps (JSON format).
- **MSD Scoring:** Ported Etterna-style Density (MSD) scoring logic and visualization (`ActorMsdGraph`) in Evaluation.
- **QR Login:** Simulated QR code login flow in `ScreenLoginQR`.
- **Node.js Matchmaking Server:** Completely rewrote `server/mock_server.js`.

## [5.7.10-Unified-Beta-Handoff] - 2026-02-10
### Changed
- **Version:** Bumped from 5.1 to 5.3.0-Network-MVP.
- **Evaluation Screen:** Added MSD Graph and Betting Resolution logic.
- **Gameplay Screen:** Added Calorie Graph overlay for Gym modes.
- **Profile Manager:** Added `LoadProfileFromID` stub to support QR login simulation.

## [5.7.2-Unified-Beta] - 2026-04-11
### Fixed
- Initialization bug in `EconomyManager` where the ledger was not properly seeded on first run (fixed in Dashboard init).
- Compilation error in `ProfileManager.h` due to duplicate function declaration.
## [5.1.1] - 2025-12-25

## [5.7.1-Unified-Alpha] - 2026-04-10
### Added
- **Hardware:** Added `IHeartRateDriver` and `HeartRateDriver_Mock` for Gym mode heart rate monitoring.
- **Hardware:** Added `HeartRateManager` singleton to poll drivers and update `GYMMAN`.
- **Backend:** Connected `EconomyManager` to Bobcoin RPC node stubs.
### Changed
- **Final Handoff Restructuring:** Rebuilt `Docs/ROADMAP.md`, `Docs/TODO.md`, `Docs/IDEAS.md`, and `Docs/SUBMODULES.md` with explicit, prioritized tasks for translating the functional C++ stubs into production-ready network/hardware connections.
- Generated `Docs/HANDOFF_ANALYSIS.md` summarizing the entire Unified architecture design.

## [5.7.9-Unified-Beta-CloudSync] - 2026-02-10
### Added
- **Cloud Profile Sync:** Revamped `ScreenAssetSync` to display a detailed matrix comparing Local Profile stats vs Cloud Profile stats (Bobcoin, Calories, ELO, Items).

## [5.7.8-Unified-Beta-Merge] - 2026-02-10
### Added
- **Feature Branches:** Merged upstream branches `origin/x11_fullscreen` and `origin/merge-itgmania-features-3960352300100248355`.

### Fixed
- Resolved merge conflicts in `ProfileManager.cpp`, `ScreenGameplay.cpp`, and others.

## [5.7.0-Unified-Alpha] - 2026-02-08
### Added
- **Unified Dashboard:** Central hub for all game modes.
- **Economy:** Full wallet, marketplace, and mining system ().
- **Fitness:** Gym mode with weight tracking and workout logs ().
- **Competitive:** Tournament ladder and match scheduling ().
- **Missions:** Dynamic mission system with progression tracking ().
- **Documentation:** Comprehensive Vision, Manual, and Agent Instructions.

### Changed
- **Gameplay:** Added mining rewards hook on stage finish.
- **Options:** Added 'Judge Scale' modifier (Etterna-style).
- **Structure:** Added `Simply-Love-SM5` submodule and mocked `bobcoin` dependency.
<<<<<<< Updated upstream
## [5.7.7-Unified-Beta-Settings] - 2026-02-10
### Added
- **Unified Options Menu:** Created `ScreenUnifiedOptions` to expose backend modifiers.
=======
- **Final Documentation Restructuring:** Rebuilt `Docs/ROADMAP.md`, `Docs/TODO.md`, `Docs/IDEAS.md`, and `Docs/SUBMODULES.md`.

## [5.7.9-Unified-Beta-CloudSync] - 2026-02-10
### Added
- **Cloud Profile Sync:** Revamped `ScreenAssetSync`.

## [5.7.8-Unified-Beta-Merge] - 2026-02-10
### Added
- **Feature Branches:** Merged upstream branches.

## [5.7.7-Unified-Beta-Settings] - 2026-02-10
### Added
- **Unified Options Menu:** Created `ScreenUnifiedOptions`.

## [5.7.6-Unified-Beta-Discord] - 2026-02-10
### Added
- **Discord Rich Presence:** Created `Themes/default/Scripts/11 Discord.lua`.

## [5.7.5-Unified-Beta-EngineStubs] - 2026-02-10
### Added
- **ReplayManager:** Implemented the core engine singleton.

## [5.7.4-Unified-Beta-MissionEngine] - 2026-02-10
### Added
- **Mission Engine:** Implemented logic within `MissionManager.cpp`.

## [5.7.3-Unified-Beta-Documentation] - 2026-02-10
### Added
- Created `Docs/VISION.md`, `Docs/SUBMODULES.md`, `Docs/TODO.md`, `Docs/ROADMAP.md`, `Docs/LLM_UNIVERSAL.md`.
>>>>>>> origin/unified-ui-features-13937230807013224518
=======
>>>>>>> Stashed changes
