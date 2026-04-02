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
