# Unified StepMania: Foundation Milestone 2 Handoff

**Status:** Milestone 2 (Economy & Connected Features) COMPLETED.
**Build:** 5.7.0-Unified-Alpha-v4
**Next Milestone:** Milestone 3 (Tournament Mode & Advanced Networking)

## Summary of Completed Features
1.  **Economy & Play-to-Earn:**
    - Real `BobcoinBridge` (JSON-RPC via EzSockets/JsonCpp) replaces all mocks.
    - `EconomyManager::AwardCompletionRewards` implements performance-based Bobcoin mining using the Wife3 metric.
    - Real-time marketplace item fetching and purchase logic.
2.  **Infrastructure & Architecture:**
    - All Singleton Managers refactored to use `std::unique_ptr` for safety and modern standards.
    - Centralized versioning in `VERSION.md` with a C++ loader (`src/ver.cpp`).
    - OS-specific Heart Rate Driver framework (`WinRT` for Windows, `BlueZ` for Linux).
3.  **Engine Parity Fixes:**
    - **Mine Fix:** DinsFire64 community fix for consistent triggering.
    - **Phantom Hold Fix:** FSX fix for preventing hold failure on passed rows.
4.  **UI & Visuals:**
    - **Ghost NoteField:** Implemented transparent replay visualization in the `Player` actor.
    - **Swarm Network:** Added "Seed" functionality to share local packs.
    - **Asset Sync:** Real-time progress and status reporting.

## Technical Notes
- **JSON-RPC:** Nodes are configured via `BobcoinNodeIP` and `BobcoinNodePort` preferences in `Data/Static.ini`.
- **VFS Versioning:** Version string is read via `RageFile` during early startup (`InitProductVersion` in `sm_main`).
- **Memory:** Use `std::make_unique` for all new manager sub-components.


### Summary of Recent CI Fixes & Structural Refinements
*   **Submodule Cleanup:** `bobcoin` and `Themes/Simply-Love-SM5` nested `.gitmodules` entries inside `itgmania/` have been removed to prevent recursive checkout failures in GitHub Actions, since these were already correctly defined in the root repository.
*   **CMake Test Code Restoration:** Missing test code (`CMake/TestCode/test_external.c` and `test_broken.c`) needed by `StepmaniaCore.cmake` compilation checks were correctly relocated to the root build environment to ensure pipeline compilation succeeds.
*   **Ghost Replay Fix:** Fixed `Player.cpp` where incorrect assumptions about `GameInput` enum values were resulting in non-mapped injection. Implemented the proper `GAMESTATE->GetCurrentStyle()->GameInputToColumn(vGhostInputs[i])` map for ghost note processing.
*   **Doc Syncing:** Mirrored core workspace docs to the root level.
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

## EXECUTIVE PROTOCOL: REPOSITORY SYNCHRONIZATION RUN (v5.71.0-Unified-Beta)
*   **Merge Operations:** Executed an intelligent dual-direction merge. Reconciled feature branch `jules-10182904622127905088-faec53aa` directly into `master`.
*   **Submodule Status:** Recursively synced `bobcoin` (to `546d3cb2`) and `Simply-Love-SM5` (to `06a56f5f`) to their latest upstream tracking commits per Executive Protocol #56. Note: `bg`, `freellm`, `jules-autopilot`, `tormentnexus`, `auto_dj_script`, `MilkDrop3`, and `MilkDrop3_fix` do not exist in this monorepo context.
*   **Version Update:** Updated global state text and deployment configurations to `5.71.0-Unified-Beta`.
