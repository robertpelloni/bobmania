# StepMania Network: Project Dashboard

## Build Information
*   **Version:** 5.3.0-Network-MVP
*   **Build Date:** 2024-05-22
*   **Status:** Stable / Feature Complete

## Project Structure

### Core Directories
*   `src/`: Main source code (C++).
    *   `src/Economy/`: Cryptocurrency, Banking, and Marketplace logic.
    *   `src/Tournament/`: Ladder, Elo, and Bracket systems.
    *   `src/Gym/`: Fitness tracking, Calorie Graphs, and Workout generation.
    *   `src/Network/`: P2P Swarm and Distributed Server simulation.
    *   `src/Unified/`: Dashboard UI and profile bridging.
*   `extern/`: External libraries (Vendored dependencies).
*   `Themes/`: UI assets and scripts.
*   `Songs/`: Content content.

### External Dependencies (Vendored in `extern/`)
This project does not use Git Submodules. All dependencies are vendored directly in the `extern/` directory for stability and ease of build.

| Library | Version | Path | Description |
| :--- | :--- | :--- | :--- |
| **ffmpeg** | (Custom) | `extern/ffmpeg/` | Multimedia handling (Video/Audio decoding). |
| **lua** | 5.1 | `extern/lua-5.1/` | Scripting engine for Themes and Modding. |
| **jsoncpp** | (Unknown) | `extern/jsoncpp/` | JSON parsing for Network/API data. |
| **glew** | 1.5.8 | `extern/glew-1.5.8/` | OpenGL Extension Wrangler. |
| **libpng** | (Unknown) | `extern/libpng/` | PNG image support. |
| **libjpeg** | (Unknown) | `extern/libjpeg/` | JPEG image support. |
| **zlib** | (Unknown) | `extern/zlib/` | Compression support. |
| **mad** | 0.15.1b | `extern/mad-0.15.1b/` | MPEG Audio Decoder. |
| **pcre** | (Unknown) | `extern/pcre/` | Perl Compatible Regular Expressions. |
| **tomcrypt** | (Custom) | `src/libtomcrypt/` | Cryptographic primitives (used for Score Signing). |

## Module Status Overview

| Module | Status | Notes |
| :--- | :--- | :--- |
| **Economy Manager** | 🟢 Active | Ledger, Wallets, Dividends functional. |
| **Tournament Ladder** | 🟢 Active | Elo calculation and Brackets visible. |
| **Gym Mode** | 🟢 Active | Calorie Graph integrated into Gameplay. |
| **Network (Swarm)** | 🟡 Simulated | P2P logic is mocked for MVP. |
| **Governance** | 🟢 Active | Weighted voting implemented. |

## Quick Links
*   [System Architecture](SYSTEM_ARCHITECTURE.md)
*   [User Guide](USER_GUIDE.md)
*   [Roadmap & Status](ROADMAP.md)
