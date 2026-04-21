<<<<<<< Updated upstream
<<<<<<< HEAD:docs/SUBMODULES.md
# Unified StepMania Submodules & Dependencies

This document tracks all git submodules, external libraries, and dependencies used within the Unified StepMania project, detailing their locations, versions, and architectural purpose.

## Git Submodules

### 1. Bobcoin
*   **Path:** `extern/bobcoin`
*   **URL:** [Assuming standard GitHub URL or local path]
*   **Version/Commit:** Tracked via git submodule. (Currently pending update).
*   **Description:** The official implementation of the Bobcoin cryptocurrency node and wallet.
*   **Usage:** Integrated via `src/Economy/Bridge/BobcoinBridge.cpp` to handle real-world transactions, player balances, and mining reward distributions. It replaces local XML-based economy tracking.

### 2. Simply-Love-SM5
*   **Path:** `Themes/Simply-Love-SM5`
*   **URL:** https://github.com/Simply-Love/Simply-Love-SM5
*   **Version/Commit:** Tracked via git submodule.
*   **Description:** The premier, highly polished theme for StepMania 5.
*   **Usage:** Serves as the baseline for high-performance visual themes. We maintain strict Lua 5.1 compatibility to ensure this theme (and community modifications of it) run flawlessly on the Unified engine.

## External Libraries (Vendored in `extern/`)

### 1. Lua
*   **Version:** 5.1.5
*   **Path:** `extern/lua-5.1/`
*   **Usage:** The absolute core of the StepMania UI frontend. We rigidly enforce the 5.1 standard to maintain compatibility with 20 years of community themes.

### 2. JsonCpp
*   **Version:** 1.9.5 (approximate, embedded)
*   **Path:** `extern/jsoncpp/`
*   **Usage:** Used for parsing `Data/MarketplaceCatalog.json`, handling web requests, and processing JSON RPC responses from the Bobcoin node and matchmaking servers.

### 3. Zlib
*   **Version:** 1.2.11 (approximate, embedded)
*   **Path:** `extern/zlib/`
*   **Usage:** Essential for compressing/decompressing `.smzip` packages, network streams, and `ContentSwarmManager` chunk validation.

## Monorepo Workflow & Submodule Management

Unified StepMania treats submodules as active development zones, not just static dependencies.

3.  **Documentation:**
    Any change to a submodule version must be recorded in `CHANGELOG.md` and the `VERSION` file.
<<<<<<< Updated upstream
=======
=======
>>>>>>> origin/unified-ui-features-13937230807013224518
# Submodule & External Dependency Deep Analysis

Unified StepMania relies heavily on external libraries to extend the core 20-year-old C++ engine into a modern platform supporting cryptocurrency, live streaming, P2P networking, and robust JSON messaging.

This document explains *why* each submodule was chosen, its version, its location, and how it integrates into the broader Unified architecture.

## The `extern/` Ecosystem

### 1. `bobcoin` (v1.0.0-Beta)
*   **Location:** `extern/bobcoin/`
*   **Purpose:** The backbone of the virtual economy. Bobcoin is a privacy-focused cryptocurrency that powers the `EconomyManager`.
*   **Integration:** Currently, `src/Economy/Bridge/BobcoinBridge.h` interfaces with a static mock of this library. The ultimate goal is to link the full `libbobcoin` and have `EconomyManager` act as a light node, validating transactions and securely storing the player's wallet (`Save/Economy.xml.sig`).
*   **Why Chosen:** A custom fork of a cryptonote protocol allows us to define "Mining by Dancing"—rewarding proof-of-work via physical rhythm game inputs rather than GPU hashing.

### 2. `ffmpeg` (v4.3)
*   **Location:** `extern/ffmpeg/`
*   **Purpose:** Traditionally used in StepMania for decoding background movies (`.mp4`, `.avi`).
*   **Integration:** In the Unified framework, FFmpeg is the crucial pillar of the `SpectatorManager`. The goal is to use `libavformat` and `libavcodec` to encode the raw OpenGL framebuffer and push it to an RTMP server, allowing `ScreenSpectate` to pull live streams of high-level tournament matches.
*   **Why Chosen:** Industry standard for streaming; handles RTMP natively.

### 3. `lua-5.1` (v5.1.5)
*   **Location:** `extern/lua-5.1/`
*   **Purpose:** The scripting engine that drives 100% of the StepMania UI (`Themes/default/`) and the gameplay modifier hooks (NotITG style modfiles).
*   **Integration:** Bound extensively via `src/LuaBinding.h` and `Luna<T>`.
*   **Why Chosen & Locked:** **CRITICAL DIRECTIVE:** Do *not* upgrade to Lua 5.3 (as Project OutFox did). Upgrading breaks millions of lines of legacy theme code (`metrics.ini`, bitwise operations). Unified StepMania maintains backwards compatibility by strictly enforcing the 5.1 sandbox.

### 4. `jsoncpp` (v1.9.5)
*   **Location:** `extern/jsoncpp/`
*   **Purpose:** Serialization and deserialization of structured data.
*   **Integration:** Replaces legacy binary or XML parsing for modern network tasks. Used by `UnifiedNetwork` to send chat messages, ELO updates, and matchmaking payloads to the `server/mock_server.js` Node backend. Also used by `EconomyManager` to parse `Data/MarketplaceCatalog.json`.
*   **Why Chosen:** Extremely lightweight, cross-platform, and robust C++ JSON parser.

### 5. `zlib` (v1.2.11)
*   **Location:** `extern/zlib/`
*   **Purpose:** Data compression.
*   **Integration:** Powers `FileDriverZip` which allows the engine to mount `.smzip` files as virtual directories. Crucial for the `ContentSwarmManager`, which downloads zipped community packs via P2P and expects the engine to load them instantly.
*   **Why Chosen:** Universal standard for deflate compression.

### 6. Legacy Audio (`mad-0.15.1b`, `newogg`, `newvorbis`)
*   **Location:** `extern/`
*   **Purpose:** Audio decoding for `.mp3` and `.ogg` files.
*   **Integration:** Used by `RageSoundReader`.
*   **Why Chosen:** Retained for absolute compatibility with legacy `.sm` chart files, which rely heavily on specific MP3 frame decoding behaviors to maintain millisecond-perfect audio sync.

## Submodule Update Protocol
When maintaining this repository, AI agents and developers MUST execute the following to ensure the tree is clean:
```bash
git submodule update --init --recursive
git submodule update --remote --merge
```
*Note: Due to the size of the repository and the mock environment constraints during automated CI/LLM passes, large submodules (like ffmpeg) may be shallow-cloned or stubbed in the build system (`CMakeLists.txt`) using `-DWITH_FFMPEG=OFF`.*
>>>>>>> origin/unified-ui-features-13937230807013224518:Docs/SUBMODULES.md
=======
>>>>>>> Stashed changes
