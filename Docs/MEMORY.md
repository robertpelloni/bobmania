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
