# StepMania Network: Project Dashboard

## Build Information
*   **Version:** 5.5.0-Server-Beta
*   **Codename:** StepMania Network
*   **Status:** Beta Release

## Project Directory Structure

The project is organized into modular components within `src/` to separate the core engine from the new "Network" features.

| Directory | Description |
| :--- | :--- |
| `src/` | **Core Engine.** Contains the legacy StepMania 5 game loop (`GameLoop.cpp`), actor system (`Actor`), and screen management. |
| `src/Economy/` | **Economy System.** Handles the simulated cryptocurrency ledger (`EconomyManager`), wallets, and transactions. |
| `src/Economy/Bridge/` | **Blockchain Bridge.** Interfaces (`BlockchainBridge`) and stubs (`EthereumBridge`) for connecting to real networks. |
| `src/Economy/Marketplace/` | **Marketplace.** UI (`ScreenMarketplace`) for purchasing in-game assets with wallet funds. |
| `src/Economy/Governance/` | **DAO.** Voting system (`ScreenGovernance`) for shareholders. |
| `src/Tournament/` | **Competition.** Elo ranking (`ScreenTournamentLadder`) and bracket generation (`TournamentBracket`). |
| `src/Gym/` | **Fitness.** Workout modes (`ScreenGymWelcome`), playlist generation (`GymPlaylistGenerator`), and calorie tracking. |
| `src/Network/` | **Connectivity.** P2P simulation (`ContentSwarmManager`), Real-time Client (`GameClient`), Streaming (`StreamManager`), and Spectating (`SpectatorManager`). |
| `src/Unified/` | **UI/UX.** Central Dashboard (`ScreenUnifiedDashboard`) and Profile Management (`AssetSyncManager`). |
| `src/arch/` | **HAL.** Hardware Abstraction Layer. Now includes `ArchHooks_VR` for VR support. |
| `extern/` | **Dependencies.** Vendored third-party libraries (See table below). |
| `server/` | **Backend.** Node.js WebSocket server for Matchmaking and Chat. |
| `Save/` | **Persistence.** Stores `Economy.ini` (Ledger), Profiles, and Preferences. |

## External Dependencies (Vendored)

This project *does not* use Git Submodules. All dependencies are vendored directly in `extern/` for stability and portability.

| Library | Version | Location | Description |
| :--- | :--- | :--- | :--- |
| **Lua** | 5.1.5 | `extern/lua-5.1/` | Scripting engine for Themes. |
| **JsonCpp** | 1.9.5 | `extern/jsoncpp/` | JSON serialization for Network/API data. |
| **zlib** | 1.2.11 | `extern/zlib/` | Compression support (Deflate/Gzip). |
| **libpng** | 1.6.37 | `extern/libpng/` | PNG image loading. |
| **libjpeg** | 8.0 (80) | `extern/libjpeg/` | JPEG image loading. |
| **libmad** | 0.15.1b | `extern/mad-0.15.1b/` | MPEG Audio Decoder. |
| **GLEW** | 1.5.8 | `extern/glew-1.5.8/` | OpenGL Extension Wrangler. |
| **FFmpeg** | 2.x (Headers) | `extern/ffmpeg/` | Multimedia framework (RTMP Streaming). |
| **TomCrypt** | (Custom) | `src/libtomcrypt/` | Cryptographic primitives (Score Signing). |
| **Bobcoin** | (Pending) | `extern/bobcoin` | Privacy-focused Blockchain Token (Solana/Monero hybrid). |

## Module Status (v5.6.0)

| Feature | Component | Status | Notes |
| :--- | :--- | :--- | :--- |
| **Economy** | `EconomyManager` | ✅ Integrated | Full ledger, betting, and persistence. |
| **Marketplace** | `ScreenMarketplace` | ✅ Integrated | Buy assets, check ownership. |
| **Tournament** | `Ladder/Bracket` | ✅ Integrated | Elo ranking and visual brackets. |
| **Gym Mode** | `CalorieGraph` | ✅ Integrated | Real-time burn overlay in Gameplay. |
| **VR Support** | `ArchHooks_VR` | ⚠️ Beta | Render loop hooks ready; requires HMD SDK. |
| **Chat** | `GameClient` | ✅ Integrated | Simulated lobby and in-game chat. |
| **Streaming** | `StreamManager` | ⚠️ Stub | FFmpeg headers integrated; video pipe logic required. |
| **Backend** | `Node.js Server` | ✅ Beta | WebSocket Server implemented (`server/`). |
| **Bridge** | `BobcoinBridge` | ⚠️ Stub | Integrated with Bobcoin logic (Dancing = Mining). |
