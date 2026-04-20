# Unified StepMania: AI Developer Memory & Context

## Project Architecture & Patterns

1. **The "Manager" Pattern:**
   To avoid polluting the core engine (`src/StepMania.cpp`, `src/ScreenGameplay.cpp`), all new features are encapsulated in global C++ Singleton Managers located in `src/` subdirectories (e.g., `src/Economy/EconomyManager.cpp`, `src/Gym/GymManager.cpp`).
   *   These managers are instantiated and deleted in `sm_main` within `src/StepMania.cpp`.
   *   They are hooked into the main update loop via `src/GameLoop.cpp`.
   *   They are bound to the Lua state using `Luna<T>` allowing the UI to call them.
   *   They must be registered in the respective `CMakeData-singletons.cmake` files.

2. **Strict UI Separation (Lua 5.1):**
   *   The C++ backend handles pure logic (networking, math, file I/O).
   *   The Lua frontend (`Themes/default/`) handles **100%** of the user interface.
   *   We **rigidly enforce** Lua 5.1 sandboxing to maintain compatibility with thousands of legacy StepMania 5.x themes and noteskins. Do not use Lua 5.3+ syntax (like true integers or bitwise operators).

3. **External Dependencies (The `extern/` folder):**
   *   We use vendored libraries for maximum cross-platform build stability without requiring users to install package managers.
   *   Key dependencies include: `jsoncpp` (Network/Marketplace parsing), `zlib` (Content Swarm unpacking), `lua-5.1` (Scripting), and `bobcoin` (The mock cryptocurrency backbone).
   *   Hardware stubs (like FFmpeg for `StreamManager` or Bluetooth for `HeartRateManager`) must be carefully `#ifdef`'d or mocked entirely if the headers are missing to prevent CI failures.

4. **Universal Configuration & Versioning:**
   *   The single source of truth for the version is the `VERSION` file in the repository root.
   *   Every commit must include a version bump in the message.
   *   `Docs/UNIVERSAL_LLM_INSTRUCTIONS.md` (symlinked/referenced by `LLM_UNIVERSAL.md`, `AGENTS.md`, etc.) is the master guide for all AI developers to prevent model drift between Claude, Gemini, and GPT.

5. **Modding & Gameplay Merges:**
   *   We use Etterna's `Wife3` (J4) scoring logic for competitive parity, integrated loosely into `ScoreKeeperNormal.cpp`.
   *   We support NotITG-style visual hooks, including `ActorMultiVertex`, `ActorFrameTexture`, and Lua Shader manipulation.

6. **File I/O Safety:**
   *   Always use `RageFile` for file operations to ensure cross-platform compatibility and correct virtual filesystem pathing (e.g., locking writes to the `Save/` directory). Never use raw `std::fstream`.

## Recent Decisions
*   **Hardware Drivers:** Decided to implement an `IHeartRateDriver` abstraction layer for Gym Mode. `HeartRateManager` currently uses a `HeartRateDriver_Mock` (sine wave generator) to prevent the game loop from crashing while actual Bluetooth OS drivers (BlueZ/WinRT) are developed.
*   **CI Constraints:** GitHub Actions runners have dropped macOS 13 and Node 20. CI `.github/workflows/ci.yml` is updated to target `macos-14` and skip XML Linting if the `Luadoc` folder is missing from the tree.
# Unified StepMania Memory Log (Technical Learnings)

This document contains ongoing technical observations, specific engine quirks, and legacy interactions learned during development.

## Lua 5.1 Boundaries
The engine rigidly enforces Lua 5.1 sandboxing to maintain compatibility with 15 years of user themes. We cannot arbitrarily inject Lua 5.3 features (like true integers or bitwise operators) without causing catastrophic failures in older `metrics.ini` and `BGAnimations/` files. All mathematical operations must assume floats (`lua_Number`).

## C++ Manager Integration
When integrating a new global singleton (e.g., `HeartRateManager`):
1.  Define the class in `src/Gym/HeartRateManager.h`.
2.  Implement in `src/Gym/HeartRateManager.cpp`.
3.  Add the pointer (`extern HeartRateManager* HRMAN;`) to `src/global.h`.
4.  Instantiate it in `sm_main` (`src/StepMania.cpp`), explicitly tying its lifecycle to the game executable.
5.  Call its `Update()` loop inside `GameLoop.cpp`.
6.  Bind it to Lua using `Luna<T>`, using `LUA_REGISTER_CLASS`.
7.  Add the `.cpp` file to the correct `CMakeData-*.cmake` (e.g., `src/CMakeData-singletons.cmake`). *Failing to do this will cause linker errors.*

## The HelpOverlay Architecture
The `HelpOverlay` (created in `Themes/default/Scripts/09 HelpOverlay.lua`) is a powerful tool. It relies on a global Lua table (`HelpOverlay.ScreenData`) that registers descriptive strings for every screen (`ScreenName`).
*   *Interaction:* When `SELECT` is pressed, a `HelpToggle` message is broadcast via `MESSAGEMAN:Broadcast("HelpToggle")`.
*   *Render Context:* It is drawn over *everything*, with a massive `draworder(2000)`.

## Missing Subsystems (The Stub Strategy)
Due to a lack of immediate APIs (e.g., a real cryptocurrency RPC, a real Bluetooth Heart Rate library, a real P2P tracker), we have adopted the "Stub" Strategy.
*   The `Manager` acts identically whether it is real or a mock.
*   We use hardcoded `std::vector` lists or sine-wave generated data inside `Update()` to ensure the UI can be fully built and tested.
*   This makes the eventual swap to real dependencies trivial.

## Pathing & File I/O
Never use standard `std::fstream` or `<fstream>` for file operations. Always use StepMania's `RageFile` wrapper and the virtual file system (`FILEMAN`). This guarantees cross-platform compatibility (Windows, macOS, Linux, and Android) and ensures files are saved to the correct user directory (`Save/`, `Data/`), handling permissions implicitly.
