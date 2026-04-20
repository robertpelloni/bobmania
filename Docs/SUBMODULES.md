<<<<<<< HEAD
<<<<<<< HEAD:docs/SUBMODULES.md
# Unified StepMania Submodules & Dependencies

This document lists all external libraries and submodules used in the Unified StepMania project, located in `extern/`.

## Core Integrations

### 1. `extern/bobcoin`
*   **Purpose:** The backbone of the **EconomyManager**. Provides blockchain logic, wallet management, and mining simulation.
*   **Integration:** Linked via `src/Economy/Bridge/BobcoinBridge.cpp`.
*   **Status:** Submodule. Needs regular updates to sync with upstream Bobcoin improvements.

### 2. `extern/ffmpeg`
*   **Purpose:** Handles A/V encoding for the **SpectatorManager** and **StreamManager**. Used for live broadcasting matches.
*   **Integration:** `src/Network/StreamManager.cpp`.
*   **Status:** Vendored headers/libs (or submodule depending on setup). Critical for the "Tournament" feature set.

### 3. `extern/lua-5.1`
*   **Purpose:** The scripting engine that powers Themes, NoteSkins, and the new Unified Lua API.
*   **Integration:** `src/LuaManager.cpp`, `src/LuaBinding.cpp`.
*   **Constraint:** Must remain **5.1** for backward compatibility with StepMania themes. Do NOT upgrade to 5.3+ unless explicitly authorized.

## Support Libraries

*   **`extern/jsoncpp`**: JSON parsing for network messages (WebSocket payloads, API responses).
*   **`extern/zlib`**: Compression for content packages and network streams.
*   **`extern/mad`**: MPEG Audio Decoder (MP3 support).
*   **`extern/glew-1.5.8`**: OpenGL Extension Wrangler. Essential for cross-platform graphics.
*   **`extern/pcre`**: Perl Compatible Regular Expressions. Used in string utility functions.
*   **`extern/tomcrypt` / `extern/tommath`**: Cryptographic primitives. Used for secure hashing (password storage, signature verification).
*   **`extern/libjpeg` / `extern/libpng`**: Image loading support.
*   **`extern/vorbis` / `extern/ogg`**: Ogg Vorbis audio support.

## Management Protocol

1.  **Updating:**
    ```bash
    git submodule update --remote --merge
    ```
    *Be careful!* Always verify compilation after updating submodules.

2.  **Adding New:**
    Use `git submodule add <url> extern/<name>` to keep the repo structure clean.

3.  **Documentation:**
    Any change to a submodule version must be recorded in `CHANGELOG.md` and the `VERSION` file.
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
<<<<<<< HEAD
>>>>>>> origin/unified-ui-features-13937230807013224518:Docs/SUBMODULES.md
=======
>>>>>>> origin/unified-ui-features-13937230807013224518
