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
