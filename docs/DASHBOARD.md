# Unified StepMania Dashboard & Project Structure

This document provides a comprehensive overview of the Unified StepMania project, including directory structure, submodules, and agent instructions.

**Version:** 5.7.1-Unified-Beta (See `VERSION` file)

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
│   ├── Economy/          # EconomyManager, Marketplace
│   ├── Gym/              # GymManager, PlaylistGenerator
│   ├── Network/          # SpectatorManager, NetSync
│   ├── Scoring/          # WifeScoring, ReplayManager
│   ├── Tournament/       # TournamentManager
│   └── Unified/          # MissionManager, Dashboard
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

## 3. Agent Instructions

All AI agents (Claude, Gemini, GPT, Copilot) **MUST** refer to `Docs/LLM_UNIVERSAL.md` before making changes.

### Quick Reference:
*   **Version Control:** Always increment `VERSION` for significant changes.
*   **Code Style:** Follow existing StepMania C++ conventions.
*   **Documentation:** Update `Docs/Manual_Unified.md` when adding user-facing features.
*   **Testing:** Verify compilation (`g++ -fsyntax-only`) before committing.

## 4. Feature Implementation Status

See `Docs/ROADMAP.md` for the live tracking list.

*   **Economy:** 90% Complete (Missing real blockchain bridge).
*   **Gym:** 85% Complete (Persistence & Playlists done).
*   **Tournament:** 60% Complete (Match execution needed).
*   **Network:** 70% Complete (Spectator backend stubbed).
