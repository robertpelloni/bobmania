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
