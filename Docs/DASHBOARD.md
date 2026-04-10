<<<<<<< HEAD
# Unified StepMania Dashboard & Project Structure

This document provides a comprehensive overview of the Unified StepMania project, including directory structure, submodules, and agent instructions.

**Version:** 5.7.2-Unified-Beta-GymPolish (See `VERSION` file)

## 1. Project Directory Structure

```
/
├── .github/              # CI/CD workflows
├── Announcers/           # Voice assets
├── BGAnimations/         # Background animations (Lua/XML)
├── bobcoin/              # [Submodule] Bobcoin Economy Logic (extern/bobcoin)
├── Characters/           # 3D Dancers
├── CMake/                # Build system scripts
├── Data/                 # Global data (MarketplaceCatalog.json, etc.)
├── Docs/                 # User Manuals & Feature Specs
│   ├── Manual_Unified.md # Comprehensive User Guide
│   ├── ROADMAP.md        # Feature Implementation Status
│   ├── HANDOFF_ANALYSIS.md # Deep Dive & Architecture
│   └── LLM_UNIVERSAL.md  # Unified Agent Instructions
├── extern/               # External Dependencies (Vendored)
│   ├── bobcoin/          # Cryptocurrency Logic
│   ├── ffmpeg/           # Streaming Support
│   ├── jsoncpp/          # JSON Serialization
│   └── lua-5.1/          # Lua Scripting Engine
├── NoteSkins/            # Arrow assets
├── Program/              # Compiled Binaries (Output)
├── Save/                 # User Data (Profiles, Economy.xml, Gym.xml)
├── Scripts/              # Global Lua Scripts
├── Songs/                # Music Content
├── src/                  # Source Code (C++)
│   ├── Economy/          # EconomyManager, Marketplace, Bridge
│   ├── Gym/              # GymManager, PlaylistGenerator, HeartRateManager
│   ├── Network/          # UnifiedNetwork, SpectatorManager, ChatManager
│   ├── Scoring/          # WifeScoring, ReplayManager
│   ├── Tournament/       # TournamentManager
│   └── Unified/          # MissionManager, AssetSyncManager
├── Themes/               # UI Skins
│   └── default/          # Primary Theme (Unified UI)
└── Utils/                # Helper Scripts
```

## 2. Submodules

| Submodule | Path | Description | Version |
| :--- | :--- | :--- | :--- |
| **Bobcoin** | `extern/bobcoin` | Core economy logic, mining algorithms, and wallet management. | v1.2.0 |
| **Lua 5.1** | `extern/lua-5.1` | The scripting engine powering the theme and gameplay logic. | 5.1.5 |
| **JsonCpp** | `extern/jsoncpp` | Library for parsing JSON data (Marketplace Catalog, etc.). | 1.9.5 |
| **FFmpeg** | `extern/ffmpeg` | Required for `SpectatorManager` streaming capabilities. | 4.4 |

## 3. Feature Status

*   **Economy:** 95% Complete. Bridge structure implemented.
*   **Gym:** 95% Complete. Persistence, Playlists, and Heart Rate done.
*   **Tournament:** 80% Complete. Network requests wired.
*   **Network:** 90% Complete. Chat and UnifiedNetwork added.
=======
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
>>>>>>> main
