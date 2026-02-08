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
