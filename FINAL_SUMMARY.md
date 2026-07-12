[PROJECT_MEMORY]
# Unified StepMania Architecture, Patterns & Decisions

## 1. Omni-Workspace Paradigm & Git Constraints
*   **The Codebase:** This repository (`bobmania`) is an **Omni-Workspace**. It serves as a unified command center managing various autonomous game engine forks (`itgmania`, NotITG parity), network layers, fitness/crypto integrations, and Web3 `bobcoin` components.
*   **Continuous Autonomous Execution:** A strict directive mandates continuous, autonomous execution. Workflows and instructions for LLMs are strictly defined in `Docs/UNIVERSAL_LLM_INSTRUCTIONS.md`.
*   **Submodule Nuances:** Submodules like `bobcoin` and `Themes/Simply-Love-SM5` must be strictly managed recursively. Having nested `.gitmodules` entries inside child directories causes GitHub CI/CD checkout failures.

## 2. The Singleton Manager Pattern
*   To avoid overwhelming the legacy `StepMania.cpp` and `ScreenGameplay.cpp` architectures, all new features are built as modular **Global C++ Singletons**.
*   **Examples:** `ECONOMYMAN` (Ledger), `GYMMAN` (Fitness), `TOURNAMENTMAN` (Brackets), `REPLAYMAN` (Ghost data), `MISSIONMAN` (Achievements), `HEARTRATEMAN` (Hardware polling).
*   **Lifecycle:** These singletons are declared in `src/global.h`, instantiated/cleaned up in `StepMania.cpp` (`sm_main`), and periodically ticked in `GameLoop.cpp`.
*   **Lua Exposure:** They are aggressively wired into the UI via `Luna<T>` bindings, exposing C++ data to the Lua sandbox (e.g., `Themes/default/Scripts/05 Economy.lua`).

## 3. Lua Sandbox & Frontend Constraints
*   **Strict Lua 5.1 Enforcement:** The engine rigidly maintains a Lua 5.1 sandbox to ensure backward compatibility with 15+ years of community themes and NoteSkins.
*   **UI Representation:** All backend feature modules must be fully represented in the frontend using interactive elements. The C++ backend handles purely logic.

## 4. Ecosystem & Build Constraints
*   **Version Control:** The single source of truth for the project version is the `VERSION` file in the root directory. Updates must trigger a commit.
*   **Dependencies:** External libraries (FFmpeg, libtorrent, Zlib, JsonCpp, Lua) are vendored in `extern/`.
*   **Missing Subsystems (Stub Strategy):** Due to a lack of immediate APIs (e.g., real Bluetooth Heart Rate library, P2P tracker), we use the "Stub" Strategy. The Manager acts identically whether it is real or a mock (e.g., `HeartRateDriver_Mock` generating a sine wave).
*   **File I/O:** Never use standard `std::fstream`. Always use StepMania's `RageFile` wrapper and the virtual file system (`FILEMAN`) to ensure cross-platform compatibility.

## 5. Gameplay & Networking Merges
*   **Parity Integrations:** Includes `Wife3` (J4) scoring logic natively for Etterna compatibility, NotITG-style visual hooks (`BothAtOnce`, `NotePath` spline mathematics), and Replay Ghost tracking.
*   **Economy/Networking:** `EconomyManager` simulates a blockchain ledger and interfaces with the privacy-focused `bobcoin` token via the `BobcoinBridge`. `GameClient` (C++) manages WebSocket connections.
