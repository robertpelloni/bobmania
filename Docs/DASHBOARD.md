<<<<<<< Updated upstream
# StepMania 5.2 Unified - Project Dashboard

**Build:** 5.2.0-Unified
**Date:** 2025-05-24
**Status:** Feature Complete / Release Candidate

## Project Structure

*   **`src/`**: Source code core.
    *   **`src/Discord/`**: Discord RPC integration (Stub).
    *   **`src/Economy/`**: Economy research foundation (Tip wallet).
    *   **`src/GrooveStats/`**: GrooveStats networking foundation.
    *   **`src/LuaModules/`**: Sandboxed Lua I/O modules.
    *   **`src/arch/`**: Architecture-specific implementations (Window, Sound, etc.).
*   **`extern/`**: External dependencies (Submodules/Vendored).
*   **`Themes/`**: Default and fallback themes.
*   **`Docs/`**: Documentation and changelogs.
    *   `HANDOFF.md`: Detailed merge analysis and handoff notes.
    *   `PLANNED_FEATURES.md`: Original feature tracking.
    *   `Changelog_Unified.txt`: New features log.

## Submodules / External Libraries

| Library | Location | Version/Note |
| :--- | :--- | :--- |
| **FFmpeg** | `extern/ffmpeg` | Multimedia handling. |
| **Lua** | `extern/lua-5.1` | Scripting engine (Fixed 5.1). |
| **GLEW** | `extern/glew-1.5.8` | OpenGL extensions. |
| **JsonCpp** | `extern/jsoncpp` | JSON parsing. |
| **LibJPEG** | `extern/libjpeg` | Image handling. |
| **LibPNG** | `extern/libpng` | Image handling. |
| **TomCrypt** | `extern/crypto` | Cryptography utils. |
| **ZLib** | `extern/zlib` | Compression. |
| **Ogg/Vorbis** | `extern/newogg`, `extern/newvorbis` | Audio decoding. |

## Versioning Policy
*   **Current Version:** 5.2.0-Unified
*   **Policy:** Version number must be incremented on every build/release.
*   **References:** `src/ProductInfo.h`, `src/ProductInfo.inc`, `configure.ac`, `VERSION`.

## Build Information
*   **CMake:** Primary build system.
*   **Platform Support:** Windows, macOS, Linux (X11).
*   **Lua Environment:** Lua 5.1 (Sandboxed). No upgrade to 5.3+ permitted for backwards compatibility.
=======
# StepMania Network: Project Dashboard

## Build Information
*   **Version:** 5.6.1-Maintenance
*   **Codename:** StepMania Network (Bobcoin Edition)
*   **Status:** Alpha / Integrated

## Project Directory Structure

The project is organized into modular components within `src/` to separate the core engine from the new "Network" features.

| Directory | Description |
| :--- | :--- |
| `src/` | **Core Engine.** Contains the legacy StepMania 5 game loop (`GameLoop.cpp`), actor system (`Actor`), and screen management. |
| `src/Economy/` | **Economy System.** Handles the simulated cryptocurrency ledger (`EconomyManager`), wallets, and transactions. |
| `src/Economy/Bridge/` | **Blockchain Bridge.** Interfaces (`BlockchainBridge`) and implementations (`BobcoinBridge`) for token integration. |
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

## External Dependencies (Vendored & Submodules)

This project uses a mix of vendored libraries (for stability) and submodules (for active monorepo development).

| Library | Version | Location | Description |
| :--- | :--- | :--- | :--- |
| **Bobcoin** | Alpha | `extern/bobcoin` | **Submodule.** Privacy-focused blockchain token. Mining via gameplay. |
| **Lua** | 5.1.5 | `extern/lua-5.1/` | Scripting engine for Themes. |
| **JsonCpp** | 1.9.5 | `extern/jsoncpp/` | JSON serialization for Network/API data. |
| **zlib** | 1.2.11 | `extern/zlib/` | Compression support (Deflate/Gzip). |
| **libpng** | 1.6.37 | `extern/libpng/` | PNG image loading. |
| **libjpeg** | 8.0 (80) | `extern/libjpeg/` | JPEG image loading. |
| **libmad** | 0.15.1b | `extern/mad-0.15.1b/` | MPEG Audio Decoder. |
| **GLEW** | 1.5.8 | `extern/glew-1.5.8/` | OpenGL Extension Wrangler. |
| **FFmpeg** | 2.x (Headers) | `extern/ffmpeg/` | Multimedia framework (RTMP Streaming). |
| **TomCrypt** | (Custom) | `src/libtomcrypt/` | Cryptographic primitives (Score Signing). |

## Module Status (v5.6.1)

| Feature | Component | Status | Notes |
| :--- | :--- | :--- | :--- |
| **Economy** | `EconomyManager` | ✅ Integrated | Ledger active. |
| **Bobcoin** | `BobcoinBridge` | 🟡 Alpha | Bridge implemented (Stub); Submodule linked. |
| **Marketplace** | `ScreenMarketplace` | ✅ Integrated | Buy assets, check ownership. |
| **Tournament** | `Ladder/Bracket` | ✅ Integrated | Elo ranking and visual brackets. |
| **Gym Mode** | `CalorieGraph` | ✅ Integrated | Real-time burn overlay in Gameplay. |
| **VR Support** | `ArchHooks_VR` | ⚠️ Beta | Render loop hooks ready; requires HMD SDK. |
| **Chat** | `GameClient` | ✅ Integrated | Simulated lobby and in-game chat. |
| **Streaming** | `StreamManager` | ⚠️ Stub | FFmpeg headers integrated; video pipe logic required. |
| **Backend** | `Node.js Server` | ✅ Beta | WebSocket Server implemented (`server/`). |
>>>>>>> Stashed changes
